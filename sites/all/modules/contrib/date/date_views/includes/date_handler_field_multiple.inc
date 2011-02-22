<?php
// $Id: date_handler_field_multiple.inc,v 1.2 2011/01/18 00:37:35 karens Exp $

/**
 * @file
 * An extended subclass for field handling that adds multiple field grouping.
 *
 * Fields that want multiple value grouping options in addition to basic
 * field and formatter handling can extend this class.
 */
class date_handler_field_multiple extends views_handler_field_field {

  function option_definition() {
    $options = parent::option_definition();
    $options['repeat'] = array(
      'contains' => array(
        'show_repeat_rule' => array('default' => ''),
      )
    );  
    $options['multiple'] = array(
      'contains' => array(
        'multiple_to' => array('default' => ''),
      )
    );
    $options['fromto'] = array(
      'contains' => array(
        'fromto' => array('default' => 'both'),
      )
    );

    return $options;  
  }

  /**
   * Provide 'group multiple values' option,
   * adapted to the needs of the Date module.
   */
  function options_form(&$form, &$form_state) {
    parent::options_form($form, $form_state);
    unset($form['multiple']);

    $field_name = $this->content_field_name;
    $field = field_info_field($field_name);
    $options = $this->options;

    //$form += date_formatter_settings($form_state, $field, $options, TRUE);
    $form['multiple']['#weight']  = 1;   
    $form['multiple']['group'] = array(
      '#title' => t('Group multiple values'),
      '#type' => 'checkbox',
      '#default_value' => $options['multiple']['group'],
      '#description' => t('If unchecked, each item in the field will create a new row, which may appear to cause duplicates. This setting is not compatible with click-sorting in table displays.'),
    );
  }

  function pre_render($values) {

    // If there are no values to render (displaying a summary, or query returned no results),
    // or if this is not a grouped field, do nothing specific.
    if (isset($this->view->build_info['summary']) || empty($values) || !$this->defer_query) {
      return parent::pre_render($values);
    }

    $field_name = $this->content_field_name;
    $field = field_info_field($field_name);
    $db_info = date_api_database_info($field);
    $options = $this->options;
    $this->view->date_info->date_handler_fields = date_handler_fields($this->view);

    // Build the list of vids to retrieve.
    // TODO: try fetching from cache_content first ??
    $vids = array();
    $this->field_values = array();
    foreach ($values as $result) {
      if (isset($result->{$this->field_alias})) {
        $vids[] = $result->{$this->field_alias};
      }
    }

    // List columns to retrieve.
    $alias = content_views_tablename($field);
    // Prefix aliases with '_' to avoid clashing with field columns names.
    $query_columns = array(
      'node.vid AS _vid',
      "$alias.delta as _delta",
      // nid is needed to generate the links for 'link to node' option.
      'node.nid AS _nid',
    );
    // The actual field columns.
    foreach ($db_info['columns'] as $column => $attributes) {
      $query_columns[] = "$alias.$attributes[column] AS $column";
      $query_fields[] = "$alias.$attributes[column]";
    }
    // Retrieve all values, we limit them in date_prepare_node(),
    // a function that is used both by the handler and by the 
    // node theme to take advantage of formatter settings.
    $where = array('1');
    $query = 'SELECT ' . implode(', ', $query_columns) .
             ' FROM {' . $db_info['table'] . "} $alias" .
             " LEFT JOIN {node} node ON node.vid = $alias.vid" .
             " WHERE node.vid IN (" . implode(',', $vids) . ') AND ' . implode(' OR ', $where) .
             " ORDER BY node.nid ASC, $alias.delta ASC";
    $result = db_query($query);

    while ($item = db_fetch_array($result)) {
      // Clean up the $item from vid and delta. We keep nid for now.
      $vid = $item['_vid'];
      unset($item['_vid']);
      $delta = !empty($item['_delta']) ? $item['_delta'] : 0;
      $item['#delta'] = $item['_delta'];
      unset($item['_delta']);
      $this->field_values[$vid][$delta] = $item;
    }
  }

  function render($values) {

    // By this time $values is a pseudo node that will be passed
    // to the theme. Add view information to it.
    $values->date_info = !empty($this->view->date_info) ? $this->view->date_info : new stdClass();
    $values->date_info->date_handler_fields = date_handler_fields($this->view);

    // Add the formatter settings to the pseudo node.
    $values->date_info->formatter_settings = $this->options;
    $values->date_info->aliases = $this->aliases;

    // If this is not a grouped field, use content_handler_field::render().
    if (!$this->defer_query) {
      return parent::render($values);
    }

    $field_name = $this->content_field_name;
    $field = field_info_field($field_name);
    $options = $this->options;

    $vid = $values->{$this->field_alias};
    if (isset($this->field_values[$vid])) {
      // Build a pseudo-node from the retrieved values.
      $node = clone($values);
      // content_format and formatters will need a 'type' .
      $node->type = $values->{$this->aliases['type']};
      $node->nid = $values->{$this->aliases['nid']};
      $node->vid = $values->{$this->aliases['vid']};
      $items = $this->field_values[$vid];
      $node->$field_name = $items;

      // Some formatters need to behave differently depending on the build_mode
      // (for instance: preview), so we provide one.
      // TODO This has changed in D7.
      $node->build_mode = NODE_BUILD_NORMAL;

      // Render items.
      $formatter_name = $options['format'];
      if ($items && ($formatter = _content_get_formatter($formatter_name, $field['type']))) {
        $rendered = array();

        // Multiple values formatter.
        $output = content_format($field, $items, $formatter_name, $values);
        if (!empty($output)) {
          $rendered[] = $this->render_link($output, (object) array('nid' => $this->aliases['nid']));
        }
      }

      if (count($rendered) > 1) {
        // TODO: could we use generic field display ?
        return theme('field_view_multiple_field', $rendered, $field, $values);
      }
      elseif ($rendered) {
        return $rendered[0];
      }
    }

    return '';    

  }

}