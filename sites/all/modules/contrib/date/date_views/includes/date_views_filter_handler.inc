<?php
// $Id: date_views_filter_handler.inc,v 1.7 2011/01/20 13:07:24 karens Exp $
/**
 * @file
 * A flexible, configurable date filter.
 */

/**
 * This filter allows you to select a granularity of date parts to filter on,
 * such as year, month, day, etc.
 *
 * Each part can be set to blank to show all values; 'now' to filter for
 * the current value of that part, or a specific value.
 *
 * An adjustment field is provided that will adjust the selected filter
 * value by something like '+90 days' or '-1 month';
 */
class date_views_filter_handler extends views_handler_filter_numeric {
  var $date_handler = NULL;

  // Add a date handler to the filter.
  function construct() {
    parent::construct();
    module_load_include('inc', 'date_api', 'date_api_sql');

    $this->date_handler = new date_sql_handler();
    $this->date_handler->construct();
   }

  function init(&$view, &$options) {
    parent::init($view, $options);

    $this->date_handler->granularity = isset($options['granularity']) ? $options['granularity'] : 'day';
    $this->date_hander->adjustment_field = isset($options['adjustment_field']) ? $options['adjustment_field'] : 0;
    $this->format = $this->date_handler->views_formats($this->options['granularity'], 'sql');

    if (empty($this->view->date_info)) $this->view->date_info = new stdClass();

    // Set the view range, do this only if not already set in case there are multiple date arguments.
    if (empty($this->view->date_info->min_allowed_year)) {
      $range = date_range_years($this->options['year_range']);
      $this->view->date_info->min_allowed_year = !empty($range) && is_array($range) ? $range[0] : variable_get('min_allowed_year', 100);
      $this->view->date_info->max_allowed_year = !empty($range) && is_array($range) ? $range[1] : variable_get('max_allowed_year', 4000);
    }
    if (empty($this->view->date_info->date_fields)) {
      $this->view->date_info->date_fields = array();
    }
    $this->view->date_info->date_fields = array_merge($this->view->date_info->date_fields, $this->options['date_fields']);
    $this->definition['allow empty'] = TRUE;

    // If no value has been submitted on an exposed filter it is treated as
    // a submitted value. Send a flag to the date widget processors so they 
    // know to set #default_value instead of 'input' in that case.
    $this->force_value = FALSE;
    if (empty($this->options['exposed']) || (isset($this->options['expose']['identifier']) && !isset($_GET[$this->options['expose']['identifier']]))) {
      $this->force_value = TRUE;
    }
  }

  // Set default values for the date filter.
  function option_definition() {
    $options = parent::option_definition();
    $options['date_fields'] = array('default' => array());
    $options['date_method'] = array('default' => 'OR');
    $options['granularity'] = array('default' => 'day');
    $options['form_type'] = array('default' => 'date_select');
    $options['default_date'] = array('default' => '');
    $options['default_to_date'] = array('default' => '');
    $options['year_range'] = array('default' => '-3:+3');
    return $options;
  }

  /**
   * Set the granularity of the date parts to use in the filter.
    */
  function has_extra_options() { return TRUE; }

  /**
   * Date selection options.
   * 
   * TODO Only select widget is working right now.
   */
  function widget_options() {
    $options = array(
      'date_select' => t('Select'), 
      'date_text' => t('Text'), 
      'date_popup' => t('Popup'),
      );
    if (!module_exists('date_popup')) {
      unset($options['date_popup']);
    }  
    return $options;
  }

  function year_range() {
    $year_range = explode(':', $this->options['year_range']);
    if (substr($this->options['year_range'], 0, 1) == '-' || $year_range[0] < 0) {
      $this_year = date_format(date_now(), 'Y');
      $year_range[0] = $this_year + $year_range[0];
      $year_range[1] = $this_year + $year_range[1];
    }
    return $year_range;
  }

  function extra_options_form(&$form, &$form_state) {
    parent::extra_options_form($form, $form_state);
    $form['form_type'] = array(
      '#type' => 'radios',
      '#title' => t('Date form type'),
      '#default_value' => $this->options['form_type'],
      '#options' => $this->widget_options(),
      '#description' => t('Choose the form element to use for date selection.'),
      );

    $form['granularity'] = $this->date_handler->granularity_form($this->options['granularity']);
    $form['granularity']['#description'] = '<p>' . t("Select a granularity for the date filter. For instance, selecting 'day' will create a filter where users can select the year, month, and day.") . '</p>';

    $form['year_range'] = array(
      '#title' => t('Date year range'),
      '#type' => 'textfield',
      '#default_value' => $this->options['year_range'],
      '#description' => t("Set the allowable minimum and maximum year range for this argument, either a -X:+X offset from the current year, like '-3:+3' or an absolute minimum and maximum year, like '2005:2010' . When the argument is set to a date outside the range, the page will be returned as 'Page not found (404)' ."),
    );
    $fields = date_views_fields($this->view->base_table);
    $options = array();
    foreach ($fields['name'] as $name => $field) {
      $options[$name] = $field['label'];
    }

    // If this filter was added as a CCK field filter and no other date field
    // has been chosen, update the default with the right date.
    if (empty($this->options['date_fields']) && $this->field != 'date_filter') {
      $this->options['date_fields'] = array($this->table .'.'. $this->field);
    }

    $form['date_fields'] = array(
      '#title' => t('Date field(s)'),
      '#type' => 'checkboxes',
      '#options' => $options,
      '#default_value' => $this->options['date_fields'],
      '#multiple' => FALSE,
      '#description' => t('Select date field(s) to filter with this argument.'),
      '#required' => TRUE,
    );
    $form['date_method'] = array(
      '#title' => t('Method'),
      '#type' => 'radios',
      '#options' => array('OR' => t('OR'), 'AND' => t('AND')),
      '#default_value' => $this->options['date_method'],
      '#description' => t('Method of handling multiple date fields in the same query. Return items that have any matching date field (date = field_1 OR field_2), or only those with matches in all selected date fields (date = field_1 AND field_2).'),
      );
  }

  function extra_options_validate($form, &$form_state) {

    $check_fields = array_filter($form_state['values']['options']['date_fields']);
    if (empty($check_fields)) {
      form_error($form['date_fields'], t('You must select at least one date field for this filter.'));
    }
    if (!preg_match('/^(?:\-[0-9]{1,4}|[0-9]{4}):(?:[\+|\-][0-9]{1,4}|[0-9]{4})$/', $form_state['values']['options']['year_range'])) {
      form_error($form['year_range'], t('Date year range must be in the format -9:+9, 2005:2010, -9:2010, or 2005:+9'));
    }
  }

  function extra_options_submit($form, &$form_state) {
    $form_state['values']['options']['date_fields'] = array_filter($form_state['values']['options']['date_fields']);
  }

  /**
   * Add the selectors to the value form using the date handler.
   */
  function value_form(&$form, &$form_state) {
    // We use different values than the parent form, so we must
    // construct our own form element.
    $form['value'] = array();
    $form['value']['#tree'] = TRUE;

    // We have to make some choices when creating this as an exposed
    // filter form. For example, if the operator is locked and thus
    // not rendered, we can't render dependencies; instead we only
    // render the form items we need.
    $which = 'all';
    $source = '';
    if (!empty($form['operator'])) {
      $source = ($form['operator']['#type'] == 'radios') ? 'radio:options[operator]' : 'edit-options-operator';
    }
    $identifier = '';
    if (!empty($form_state['exposed'])) {
      $identifier = $this->options['expose']['identifier'];

      if (empty($this->options['expose']['use_operator']) || empty($this->options['expose']['operator'])) {
        // exposed and locked.
        $which = in_array($this->operator, $this->operator_values(2)) ? 'minmax' : 'value';
      }
      else {
        $source = 'edit-' . drupal_html_id($this->options['expose']['operator']);
      }
    }

    $handler = $this->date_handler;

    if ($which == 'all' || $which == 'value') {
      $form['value'] += $this->date_parts_form($form_state, 'value', $source, $which, $this->operator_values(1), $identifier);
      if ($this->force_value) {
        $form['value']['value']['#force_value'] = TRUE;
      }
    }

    if ($which == 'all' || $which == 'minmax') {
      $form['value'] += $this->date_parts_form($form_state, 'min', $source, $which, $this->operator_values(2), $identifier);
      $form['value'] += $this->date_parts_form($form_state, 'max', $source, $which, $this->operator_values(2), $identifier);
      if ($this->force_value) {
        $form['value']['min']['#force_value'] = TRUE;
        $form['value']['max']['#force_value'] = TRUE;
      }
    }

    // Add some text to make it clear when additional options are available.
    $extra = t("You can use any values PHP's date_create() can understand, like between '12AM today' and '12AM tomorrow.");
    $form['value']['default_date'] = array(
      '#type' => 'textfield',
      '#title' => t('Date default'),
      '#default_value' => $this->options['default_date'],
      '#prefix' => '<div class="form-item"><label>' . t('Relative value') . '</label><p>' . t("Relative values will be used if no date is set above. Use 'now' to default to the current date at runtime or add modifiers like 'now +1 day' . The To date default value is used when the operator is set to 'between' or 'not between'.") . ' ' . $extra . '</p><p>' . t('If the filter is exposed, these values will be used to set the inital value of the exposed filter. Leave both date and default values blank to start with no value in the exposed filter.') . '</p></div>',
      );
    $form['value']['default_to_date'] = array(
      '#type' => 'textfield',
      '#title' => t('To date default'),
      '#default_value' => $this->options['default_to_date'],
      );

    // Test if this value is in the UI or exposed, only show these elements in the UI.
    // We'll save it as an option rather than a value to store it for use
    // in the exposed filter.
    if (!empty($form_state['exposed'])) {
      $form['value']['default_date']['#type'] = 'value';
      $form['value']['default_date']['#value'] = $form['value']['default_date']['#default_value'];
      $form['value']['default_to_date']['#type'] = 'value';
      $form['value']['default_to_date']['#value'] = $form['value']['default_to_date']['#default_value'];
      unset($form['value']['default_date']['#prefix']);
    }

    //$form['value']['#theme'] = 'date_views_filter_form';  

  }

  /**
   * A form element to select date part values.
   *
   * @param string $prefix
   *   A prefix for the date values, 'value', 'min', or 'max' .
   * @param string $source
   *   The operator for this element.
   * @param string $which
   *   Which element to provide, 'all', 'value', or 'minmax' .
   * @param array $operator_values
   *   An array of the allowed operators for this element.
   * @param array $limit
   *   An array of date parts to limit this element to.
   *
   * @return
   *   The form date part element for this instance.
   */
  function date_parts_form($form_state, $prefix, $source, $which, $operator_values, $identifier) {
    module_load_include('inc', 'date_api', 'date_api_elements');
    switch($prefix) {
      case 'min':
        $name = t('From date');
        break;
      case 'max':
        $name = t('To date');
        break;
      default:
        $name = '';
        break;
    }

    $type = $this->options['form_type'];
    if ($type == 'date_popup' && !module_exists('date_popup')) {
      $type = 'date_text';
    }

    $format = $this->date_handler->views_formats($this->options['granularity'], 'sql');
    $granularity = array_keys($this->date_handler->date_parts($this->options['granularity']));

    // Don't set a default date in the UI, only in the exposed form.
    $default_date = '';
    if (!empty($form_state['exposed'])) {
      $default_date = $this->default_value($prefix);
    }

    $id = !empty($form_state['exposed']) ? 'edit-date-filter-' . $prefix : 'edit-options-value-' . $prefix;

    $form[$prefix] = array(
      '#title' => $name,
      '#type' => $type,
      '#size' => 20,
      '#default_value' => !empty($this->value[$prefix]) ? $this->value[$prefix] : $default_date,
      '#date_format' => date_limit_format($format, $granularity),
      '#date_label_position' => 'within',
      '#date_year_range' => $this->options['year_range'],
      '#process' => array($type . '_element_process'),
      '#prefix' => '<div id="' . $id . '-wrapper"><div id="' . $id . '">',
      '#suffix' => '</div></div>',
    );
    if ($which == 'all') {
      $form[$prefix]['#process'] = array('ctools_dependent_process', $type . '_element_process');
      $form[$prefix]['#dependency'] = array($source => $operator_values);  
    }

    if (!empty($form_state['exposed']) && !isset($form_state['input'][$identifier][$prefix])) {
      $form_state['input'][$identifier][$prefix] = $this->value[$prefix];
    }        
    return $form;
  }

  function default_value($prefix, $options = NULL) {
    $default_date = '';
    if (empty($options)) {
      $options = $this->options;
    }

    // If this is a remembered value, use the value from the SESSION.
    if (!empty($this->options['expose']['remember'])) {
      $display_id = ($this->view->display_handler->is_defaulted('filters')) ? 'default' : $this->view->current_display;
      return $_SESSION['views'][$this->view->name][$display_id]['date_filter'][$prefix];
    }

    // This is a date that needs to be constructed from options like 'now' .
    $default_option = $prefix == 'max' ? $options['default_to_date'] : $options['default_date'];
    if (!empty($default_option)) {
      $date = date_create($default_option, date_default_timezone_object());
      $default_date = !empty($date) ? $date->format($this->format) : '';
    }
    // This a fixed date.
    else {
      $default_date = $options['value'][$prefix];
    }
    return $default_date;
  }

  /**
   * Value validation.
   * 
   * TODO add in more validation.
   * 
   * We are setting an extra option using a value form
   * because it makes more sense to set it there. 
   * That's not the normal method, so we have to manually
   * transfer the selected value back to the option.
   */
  function value_validate($form, &$form_state) {

    if (($form_state['values']['options']['operator'] == 'between' || $form_state['values']['options']['operator'] == 'not between') && 
    !empty($form_state['values']['options']['value']['default_date']) && empty($form_state['values']['options']['value']['default_to_date'])) {
      form_error($form['value']['default_to_date'], t('Please set a default value for the To date as well as the From date when using default values with the Between or Not between operators.'));
    }
    if (isset($form_state['values']['options']['value']['default_date'])) {
      $this->options['default_date'] = $form_state['values']['options']['value']['default_date'];
      $this->options['default_to_date'] = $form_state['values']['options']['value']['default_to_date'];
    }
    parent::value_validate($form, $form_state);
  }

  // Update the summary values to provide
  // meaningful information for each option.
  function admin_summary() {
    if (empty($this->options['date_fields'])) {
      return t('Missing date fields!');
    }
    $handler = $this->date_handler;

    $fields = date_views_fields($this->view->base_table);
    if (!empty($this->options['date_fields'])) {
      $output = array();
      foreach ($this->options['date_fields'] as $field) {
        if (array_key_exists($field, $fields['name'])) {
          $output[] = $fields['name'][$field]['label'];
        }
      }
    }
    $field = implode(' ' . $this->options['date_method'] . ' ', $output);
    $output = "$field " . check_plain($this->operator) . ' ';
    $parts = $handler->date_parts();
    $widget_options = $this->widget_options();
    // If the filter is exposed, display the granularity.
    if ($this->options['exposed']) {
      return t('(@field) <strong>Exposed</strong> @widget @format', array('@field' => $field, '@format' => $parts[$handler->granularity], '@widget' => $widget_options[$this->options['form_type']]));
    }
    // If not exposed, display the value.
    if (in_array($this->operator, $this->operator_values(2))) {
      $min = check_plain(!empty($this->options['default_date']) ? $this->options['default_date'] : $this->options['value']['min']);
      $max = check_plain(!empty($this->options['default_to_date']) ? $this->options['default_to_date'] : $this->options['value']['max']);
      $output .= t('@min and @max', array('@min' => $min, '@max' => $max));
    }
    else {
      $output .= check_plain(!empty($this->options['default_date']) ? $this->options['default_date'] : $this->options['value']['value']);
    }
    return $output;
  }

   // Views treats the form as though it has already been submitted
  // even when it hasn't, so we when it is really not submitted we 
  // have to adjust the values to match what should have been the default.
  // Overriding exposed_submit() will ensure that the $input value
  // used by Views has been adjusted to the right value.
  function exposed_submit(&$form, &$form_state) { 
    if ($this->force_value) {
      $default = array();
      foreach (array('value', 'min', 'max') as $prefix) {
        $default[$prefix] = $this->default_value($prefix);
      }
      $form_state['values'][$this->options['expose']['identifier']] = $default;
    }
  }

  /**
   * Custom implementation of query() so we can get the
   * AND and OR methods in the right places.
   */
  function query() {
    $this->ensure_my_table();
    $this->get_query_fields();

    // If we don't add a dummy where clause and there is no other filter defined for this view, 
    // Views will dump in an invalid WHERE () in addition to our custom filters, so give it a valid value.
    // @TODO This is probably the wrong way to solve this problem.
    if (empty($this->query->where[0]['conditions'])) {
      $this->query->add_where_expression(NULL, '1=1', array());
    }

    if (!empty($this->query_fields)) {
      foreach ((array) $this->query_fields as $query_field) {
        $field = $query_field['field'];
        if ($field['table_name'] != $this->table || !empty($this->relationship)) {
          $this->related_table_alias = $this->query->queue_table($field['table_name'], $this->relationship);
        }
        $table_alias = !empty($this->related_table_alias) ? $this->related_table_alias : $field['table_name'];
        $query_field['field']['fullname'] = $table_alias .'.'. $query_field['field']['field_name'];
        $sql = '';
        $sql_parts = array();
        switch ($this->operator) {
          case 'between':
            $sql_parts[] = $this->date_filter('min', $query_field, '>=');
            $sql_parts[] = $this->date_filter('max', $query_field, '<=');
            $sql = implode(' AND ', array_filter($sql_parts));
            break;
          case 'not between':
            $sql_parts[] = $this->date_filter('min', $query_field, '<');
            $sql_parts[] = $this->date_filter('max', $query_field, '>');
            $sql = implode(' OR ', array_filter($sql_parts));
            break;
          default:
            $sql = $this->date_filter('value', $query_field, $this->operator);
            break;
        }
        if (!empty($sql)) {
          // Use set_where_group() with the selected date_method 
          // of 'AND' or 'OR' to combine the field WHERE clauses.
          $this->query->set_where_group($this->options['date_method'], 'date');
          $this->query->add_where_expression('date', $sql, array());
        }
      }
    }
  }

  function date_filter($prefix, $query_field, $operator) {
    $field = $query_field['field'];

    // Handle the simple operators first.
    if ($operator == 'empty') {
      return $field['fullname'] . ' IS NULL';
    }
    elseif ($operator == 'not empty') {
     return $field['fullname'] . ' IS NOT NULL';
    }

    // Views treats the default values as though they are submitted
    // so we when it is really not submitted we have to adjust the
    // query to match what should have been the default.
    $value_parts = !is_array($this->value[$prefix]) ? array($this->value[$prefix]) : $this->value[$prefix];

    foreach ($value_parts as $part) {
      $default = $this->default_value($prefix);
      if (!empty($this->force_value) && !empty($default)) {
        $this->value[$prefix] = $default;
      }
      else {
        if (empty($part)) {
          return '';
        }
      }
    }

    $granularity = $this->options['granularity'];
    $date_handler = $query_field['date_handler'];
    $date = date_now();
    $date->setFuzzyDate($this->value[$prefix], $this->format);
    $value = date_format($date, $this->format);

    $range = $this->date_handler->arg_range($value);
    $year_range = date_range_years($this->options['year_range']);
    if ($this->operator != 'not between') {
      switch ($operator) {
        case '>':
        case '>=':
          $range[1] = new DateObject(date_pad($year_range[1], 4) . '-12-31 23:59:59');  
          if ($operator == '>') {
            date_modify($range[0], '+1 second');
          }
          break;
        case '<':
        case '<=':
          $range[0] = new DateObject(date_pad($year_range[0], 4) . '-01-01 00:00:00');
          if ($operator == '<') {
            date_modify($range[1], '-1 second');
          }
          break;
      }      
    }

    $min_date = $range[0];
    $max_date = $range[1];

    $this->min_date = $min_date;
    $this->max_date = $max_date;
    $this->year = date_format($date, 'Y');
    $this->month = date_format($date, 'n');
    $this->day = date_format($date, 'j');
    $this->week = date_week(date_format($date, DATE_FORMAT_DATE));
    $this->date_handler = $date_handler;

    if ($this->date_handler->granularity == 'week') {
      $this->format = DATE_FORMAT_DATETIME;
    }
    switch ($prefix) {
      case 'min':
        $value = date_format($min_date, $this->format);
        break;
      case 'max':
        $value = date_format($max_date, $this->format);
        break;
      default:
        $value = date_format($date, $this->format);
        break;
    }
    if ($this->date_handler->granularity != 'week') {
      $sql = $date_handler->sql_where_format($this->format, $field['fullname'], $operator, $value);
    }
    else {
      $sql = $date_handler->sql_where_date('DATE', $field['fullname'], $operator, $value);
    }
    return $sql;
  }

  function get_query_fields() {
    $fields = date_views_fields($this->view->base_table);
    $fields = $fields['name'];
    $this->query_fields = array();
    foreach ((array) $this->options['date_fields'] as $delta => $name) {
      if (array_key_exists($name, $fields) && $field = $fields[$name]) {
        $date_handler = new date_sql_handler();
        $date_handler->construct($field['sql_type'], date_default_timezone());
        $date_handler->granularity = $this->options['granularity'];
        $this->query_fields[] = array('field' => $field, 'date_handler' => $date_handler);
      }
    }
  }
}