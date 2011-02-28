<?php
// $Id: date_views_argument_handler.inc,v 1.8 2011/01/20 13:07:24 karens Exp $

/**
 * @file
 * Date API views argument handler.
 */

/**
 * Date API argument handler.
 */
class date_views_argument_handler extends views_handler_argument_date {
  function construct() {
    parent::construct();
    module_load_include('inc', 'date_api', 'date_api_sql');

    $this->date_handler = new date_sql_handler();
    $this->date_handler->construct();     

  }

  /**
   * Get granularity and use it to create the formula and a format
   * for the results.
   */
  function init(&$view, &$options) {
    parent::init($view, $options);

    // Identify the type of display we're using.
    $this->display_handler = $view->display_handler->definition['handler'];

    // Add a date handler to the display.
    $this->date_handler->granularity = $this->options['granularity'];
    $this->format = $this->date_handler->views_formats($this->date_handler->granularity, 'display');
    $this->sql_format = $this->date_handler->views_formats($this->date_handler->granularity, 'sql');

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
  }

  /**
   * Default value for the date_fields option.
   */
  function option_definition() {
    $options = parent::option_definition();
    $options['date_fields'] = array('default' => array());
    $options['year_range'] = array('default' => '-3:+3');
    $options['date_method'] = array('default' => 'OR');
    $options['granularity'] = array('default' => 'month');

    return $options;
  }

  /**
   * Add a form element to select date_fields for this argument.
   */
  function options_form(&$form, &$form_state) {
    parent::options_form($form, $form_state);
    $options = $this->date_handler->date_parts();
    unset($options['second'], $options['minute']);
    $options += array('week' => t('Week', array(), array('context' => 'datetime')));
    $form['granularity'] = array(
      '#title' => t('Granularity'),
      '#type' => 'radios',
      '#options' => $options,
      '#default_value' => $this->options['granularity'],
      '#multiple' => TRUE,
      '#description' => t("Select the type of date value to be used in defaults, summaries, and navigation. For example, a granularity of 'month' will set the default date to the current month, summarize by month in summary views, and link to the next and previous month when using date navigation."),
    );

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

    // If this argument was added as a CCK field argument and no other date field
    // has been chosen, update the default with the right date.
    if (empty($this->options['date_fields']) && $this->field != 'date_argument') {
      $this->options['date_fields'] = array($this->table .'.'. $this->field);
    }

    $form['date_fields'] = array(
      '#title' => t('Date field(s)'),
      '#type' => 'checkboxes',
      '#options' => $options,
      '#default_value' => $this->options['date_fields'],
      '#multiple' => TRUE,
      '#description' => t("Select one or more date fields to filter with this argument. Do not select both the 'From date' and 'To date' for CCK date fields, only one of them is needed."),
    );
    $form['date_method'] = array(
      '#title' => t('Method'),
      '#type' => 'radios',
      '#options' => array('OR' => t('OR'), 'AND' => t('AND')),
      '#default_value' => $this->options['date_method'],
      '#description' => t('Method of handling multiple date fields in the same query. Return items that have any matching date field (date = field_1 OR field_2), or only those with matches in all selected date fields (date = field_1 AND field_2). '),
      );

  }

  function options_validate(&$form, &$form_state) {
    // It is very important to call the parent function here:
    parent::options_validate($form, $form_state);

    if ($form_state['values']['form_id'] == 'views_ui_config_item_form') {
      $check_fields = array_filter($form_state['values']['options']['date_fields']);
      if (empty($check_fields)) {
        form_error($form['date_fields'], t('You must select at least one date field for this argument.'));
      }
      if (!preg_match('/^(?:\-[0-9]{1,4}|[0-9]{4}):(?:[\+|\-][0-9]{1,4}|[0-9]{4})$/', $form_state['values']['options']['year_range'])) {
        form_error($form['year_range'], t('Date year range must be in the format -9:+9, 2005:2010, -9:2010, or 2005:+9'));
      }
    }
  }

  function options_submit(&$form, &$form_state) {
    // It is very important to call the parent function here:
    parent::options_submit($form, $form_state);

    if ($form_state['values']['form_id'] == 'views_ui_config_item_form') {
      $form_state['values']['options']['date_fields'] = array_filter($form_state['values']['options']['date_fields']);
    }
  }

  // Update the summary values to show selected granularity.
  function admin_summary() {
    $fields = date_views_fields($this->view->base_table);
    if (!empty($this->options['date_fields'])) {
      $output = array();
      foreach ($this->options['date_fields'] as $field) {
        $output[] = $fields['name'][$field]['label'];
      }
      return implode('<br />' . $this->options['date_method'] . ' ', $output);
    }
    else {
      return parent::admin_summary();
    }
  }

  /**
   * Set the empty argument value to the current date, 
   * formatted appropriately for this argument.
   */
  function get_default_argument($raw = FALSE) {
    $granularity = $this->options['granularity'];
    if (!$raw && $this->options['default_argument_type'] == 'date') {
      if ($granularity == 'week') {
        $now = date_now();
        $week = date_week($now);
        return date_format($now, 'Y') .'-W'. $week;
      } 
      else {
        return date($this->format(), REQUEST_TIME);
      }
    }
    else {
      return parent::get_default_argument($raw);
    }
  }

  function format() {
    if (!empty($this->options['granularity'])) {
      $date_handler = new date_sql_handler();
      return $date_handler->views_formats($this->options['granularity']);
    }
    else {
      return !empty($this->options[$this->option_name]) ? $this->options[$this->option_name] : 'Y-m';
    }
  }

  /**
   * Provide a link to the next level of the view from the summary.
   */
  function summary_name($data) {
    $format = $this->date_handler->views_formats($this->options['granularity'], 'display');
    $value = $data->{$this->name_alias};
    $range = $this->date_handler->arg_range($value);
    return date_format_date($range[0], 'custom', $format);
  }

  /**
   * Provide the argument to use to link from the summary to the next level;
   * this will be called once per row of a summary, and used as part of
   * $view->get_url().
   *
   * @param $data
   *   The query results for the row.
   */
  function summary_argument($data) {
    $format = $this->date_handler->views_formats($this->options['granularity'], 'sql');
    $value = $data->{$this->name_alias};
    $range = $this->date_handler->arg_range($value);
    return date_format_date($range[0], 'custom', $format);
  }

  /**
   * Provide a link to the next level of the view from the argument.
   */
  function title() {
    $format = $this->date_handler->views_formats($this->options['granularity'], 'display');
    $range = $this->date_handler->arg_range($this->argument);
    return date_format_date($range[0], 'custom', $format);
  }

  /**
   * Create a summary query that matches the granularity.
   *
   * Needed or Views will do a groupby on the complete date instead
   * of only the part of the date actually used in the argument.
   */
  function summary_query() {
    $this->get_query_fields();

    // No way to do summaries on more than one field at a time.
    if (count($this->query_fields) > 1) {
      return;
    }

    $field = $this->query_fields[0]['field'];
    $date_handler = $this->query_fields[0]['date_handler'];

    // Get the SQL format for this granularity, like Y-m,
    // and use that as the grouping value.
    $format = $date_handler->views_formats($this->options['granularity'], 'sql');
    $this->formula = $date_handler->sql_format($format, $date_handler->sql_field($field['fullname']));

    // Add the computed field.
    $this->base_alias = $this->name_alias = $this->query->add_field(NULL, $this->formula, $field['query_name']);
    $this->query->set_count_field(NULL, $this->formula, $field['query_name']);   
    return $this->summary_basics(FALSE);

  }

  function get_query_fields() {
    $fields = date_views_fields($this->view->base_table);
    $fields = $fields['name'];
    $min_date = isset($this->min_date) ? $this->min_date : NULL;
    $min_utc = isset($this->min_utc) ? $this->min_utc : NULL;
    $max_date = isset($this->max_date) ? $this->max_date : NULL;
    $max_utc = isset($this->max_utc) ? $this->max_utc : NULL;
    $this->query_fields = array();
    foreach ($this->options['date_fields'] as $delta => $name) {
      if (array_key_exists($name, $fields) && $field = $fields[$name]) {
        $date_handler = new date_sql_handler();
        $date_handler->construct($field['sql_type'], date_default_timezone());
        $date_handler->granularity = $this->options['granularity'];
        date_views_set_timezone($date_handler, $this, $field);
        $this->query_fields[] = array('field' => $field, 'date_handler' => $date_handler);
      }
    }
  }

  /**
   * Make sure the date field is added to the query.
   * 
   * Do this in pre_query() so it will get done even if the argument 
   * is the wildcard, since query() is skipped when the wildcard is used.
   */
  function pre_query() { 
    // Unset invalid date values before the query runs.
    if (!empty($this->view->args) && count($this->view->args) > $this->position) {
      $argument = $this->view->args[$this->position];
      $parts = $this->date_handler->arg_parts($argument);
      if (empty($parts[0]['date']) && empty($parts[0]['period'])) {
        unset($this->view->args[$this->position]); 
      }
    }
  }

  /**
   * Set up the query for this argument.
   *
   * The argument sent may be found at $this->argument.
   */
  function query() {
    $this->get_query_fields();
    $block_identifier = date_block_identifier($this->view);
    if (!empty($this->view->block_identifier) || isset($_GET[$block_identifier])) {
      // Retrieve the block arguments in a way that will work for
      // urls like user/%/calendar/2009-04.
      if (!empty($_GET[$block_identifier])) {
        $path_args = explode('/', $this->view->get_path());
        $mini_args = explode('/', $_GET[$block_identifier]);
        foreach ($path_args as $pos => $key) {
          if ($path_args[$pos] != '%') {
            unset($mini_args[$pos]);
          }
        }
        // Get rid of gaps in the array caused by embedded args.
        $mini_args = array_values($mini_args);
        $this->view->args = $mini_args;
      }
      $i = 0;
      foreach ($this->view->argument as $argument) {
        if ($argument->field == 'date_argument') {
          $this->argument = $this->view->args[$argument->position];
          break;
        }
        $i++;
      }
    }
    $parts = $this->date_handler->arg_parts($this->argument);

    foreach ($parts as $type) {
      foreach ($type as $part) {
        foreach ($part as $key => $value) {
          if (!empty($value)) {
            // The last part evaluated is the one that will 'stick'
            // as the date type.
            $this->granularity = $key;
            $this->$key = $value;
          }
        }
      }
    }
    $range = $this->date_handler->arg_range($this->argument);
    $min_date = $range[0];
    $max_date = $range[1];
    $this->min_date = $min_date;
    $this->max_date = $max_date;
    // See if we're outside the allowed date range for our argument.
    if (date_format($min_date, 'Y') < $this->view->date_info->min_allowed_year || date_format($max_date, 'Y') > $this->view->date_info->max_allowed_year) {
      $this->forbid = TRUE;
      $this->query->add_where_expression('date', "0=1", array());
      return;
    }

    // The second option seems to work better in the block view if 
    // set to something other than the original value. 
    // Need to keep an eye on this to be sure nothing else breaks.
    //$format = $this->date_handler->views_formats($this->options['granularity'], 'sql');
    $format = $this->date_handler->views_formats($this->granularity, 'sql');
    if (!empty($this->query_fields)) {
      // Use set_where_group() with the selected date_method 
      // of 'AND' or 'OR' to create the where clause.
      $this->query->set_where_group($this->options['date_method'], 'date');
      foreach ($this->query_fields as $query_field) {
        $field = $query_field['field'];
        $date_handler = $query_field['date_handler'];
        if ($field['table_name'] != $this->table || !empty($this->relationship)) {
          $this->related_table_alias = $this->query->queue_table($field['table_name'], $this->relationship);
        }
        $table_alias = !empty($this->related_table_alias) ? $this->related_table_alias : $field['table_name'];
        $from_field = str_replace($field['table_name'] .'_', $table_alias .'.', $field['fromto'][0]);
        $to_field = str_replace($field['table_name'] .'_', $table_alias .'.', $field['fromto'][1]);
        if ($this->granularity != 'week') {
          $from = $date_handler->sql_where_format($format, $from_field, '<=', date_format($max_date, $format));
          $to   = $date_handler->sql_where_format($format, $to_field, '>=', date_format($min_date, $format));
        }
        else {
          $format = DATE_FORMAT_DATETIME;
          $from = $date_handler->sql_where_date('DATE', $from_field, '<=', date_format($max_date, $format));
          $to   = $date_handler->sql_where_date('DATE', $to_field, '>=', date_format($min_date, $format));          
        }
        $sql = str_replace('***table***', $field['table_name'], "($from AND $to)");
        if ($sql) {
          $this->query->add_where_expression('date', $sql, array());

        }
      }
    }
  }
}
