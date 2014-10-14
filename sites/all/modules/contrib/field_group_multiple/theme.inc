<?php
/**
 * @file
 *
 * Theme functions for field_group_multiple
 */

/**
 * TODO implement drag & drop for order and remove item row
 */
function theme_field_group_multiple_container_form($variables) {
  drupal_add_css(drupal_get_path('module', 'field_group_multiple') . '/field_group_multiple.css');

  $element = $variables['elements'];
  
  $items = $element['items'];
  $group = $element['#group'];
  $id = $group->identifier;

  $entries = array(
    '#type' => 'container',
    '#attributes' => array(
      'class' => array(
        'field-group-multiple',
        'field-group-multiple-container',
      ),
    ),
  );


  foreach (element_children($items, TRUE) as $delta) {
    $field_data = $items[$delta];
    $entry = array(
      '#type' => 'container',
      '#attributes' => array(
        'class' => array(
          'multiple-inline-element',
          'multiple-item-' . drupal_html_class($delta),
          'clearfix',
          $delta % 2 == 0 ? 'even' : 'odd',
        ),
      ),
      '#weight' => $delta,
    );

    $order = 0;
    foreach (element_children($field_data, TRUE) as $field_name) {
      $data = $field_data[$field_name];

      $elem = array(
        '#type' => 'container',
        '#attributes' => array(
          'class' => array(
            'multiple-element',
            'multiple-' . drupal_html_class($field_name),
          ),
        ),
        '#weight' => $order,
        'data' => $data,
      );

      $order++;
      $entry[$field_name] = $elem;
    }
    $entries[] = $entry;
  }

  $output = '<div class="form-item">';
  $output .= drupal_render($entries);
  $output .= isset($element['#description']) ? '<div class="description">' . $element['#description'] . '</div>' : '';
  $output .= '</div>';

  return $output;
}

/**
 * TODO implement drag & drop for order and remove item row
 */
function theme_field_group_multiple_label_value_form($variables) {
  drupal_add_css(drupal_get_path('module', 'field_group_multiple') . '/field_group_multiple.css');

  $element = $variables['elements'];

  $items = $element['items'];
  $group = $element['#group'];
  $id = $group->identifier;
  
  $header = array();
  $rows = array();


  $label_field = $group->format_settings['instance_settings']['label_value_field'];
  $diff = array_diff($group->children, array($label_field));
  $value_field = array_shift($diff);

  $_header = array(
    'label' => array('t' => 'Label', 'd' => 'Label for values', 'f' => $label_field),
    'value' => array('t' => 'Value', 'd' => '', 'f' => $value_field),
  );

  foreach ($_header as $data) {
    $header_entry = '<div class="th-title-label">' . t($data['t']) . '</div>';
    if (!empty($data['d'])) {
      $header_entry .= '<div class="th-title-description">' . t($data['d']) . '</div>';
    }
    $header[] = '<div class="th-title">' . $header_entry . '</div>';
  }

  foreach (element_children($items, TRUE) as $delta) {
    $field_data = $items[$delta];
    $row = array();
    foreach ($_header as $type => $_data) {
      $data = $field_data[$_data['f']];
      $row[] = array('data' => drupal_render($data), 'class' => '');
    }
    $rows[] = array('data' => $row, 'class' => array('row-delta-' . $delta));
  }

  $wrapper_id = drupal_html_id($id . '-add-more-wrapper');
  $table_id = drupal_html_id($id . '_values');

  // TODO drag & drop and remove
  $output = '<div class="field-group-multiple-label-value"><div class="form-item">';
  $output .= theme('table', array('header' => $header, 'rows' => $rows, 'attributes' => array('id' => $table_id, 'class' => array('field-group-multiple-table'))));
  $output .= isset($element['#description']) ? '<div class="description">' . $element['#description'] . '</div>' : '';
  $output .= '</div></div>';

  return $output;
}

/**
 * TODO implement drag & drop for order and remove item row
 */
function theme_field_group_multiple_table_form($variables) {

  drupal_add_css(drupal_get_path('module', 'field_group_multiple') . '/field_group_multiple.css');
  $element = $variables['elements'];

  $items = $element['items'];
  $group = $element['#group'];  
  $id = $group->identifier;  

  $titles = (isset($group->format_settings['instance_settings']['row_titles'])) ? $group->format_settings['instance_settings']['row_titles'] : NULL;
  if ($titles) {
    $row_titles = array_map('check_plain', explode(',', $titles));
    if ($row_titles[0] == '-none-') {
      $row_titles[0] = '';
    }
  }

  $header = array();
  $rows = array();

  if ($titles) {
    $header[] = '<div class="th-title">' . $row_titles[0] . '</div>';
  }

  $weights = array();
  foreach ($group->fields as $field_name => $field_state) {
    $field_instance_state = $field_state['state']['instance'];
    $field_title = check_plain($field_instance_state['label']);
    $field_description = check_plain($field_instance_state['description']);

    $header_entry = '<div class="th-title-label">' . t($field_title) . '</div>';
    if (!empty($field_description)) {
      $header_entry .= '<div class="th-title-description">' . t($field_description) . '</div>';
    }
    
    $weight = $field_instance_state['widget']['weight'];
    $weights[$field_name] = $weight; // Cache weights for rows
    $header[$weight] = '<div class="th-title">' . $header_entry . '</div>';
  }

  ksort($header);
  
  foreach (element_children($items, TRUE) as $delta) {
    $field_data = $items[$delta];
    $row = array();

    if ($titles) {
      $row_title = '';
      if (($delta + 1) < count($row_titles)) {
        $row_title = $row_titles[$delta + 1];
      }
      $row[] = array('data' => $row_title, 'class' => '');
    }

    foreach (element_children($field_data, TRUE) as $field_name) {
      $data = $field_data[$field_name];
      $weight = $weights[$field_name];
      $row[$weight] = array('data' => drupal_render($data), 'class' => '');
    }
    ksort($row);
    $rows[] = array('data' => $row, 'class' => array('row-delta-' . $delta));
  }


  $wrapper_id = drupal_html_id($id . '-add-more-wrapper');
  $table_id = drupal_html_id($id . '_values');

  // TODO drag & drop and remove
  $output = '<div class="field-group-multiple-table"><div class="form-item">';
  $output .= theme('table', array('header' => $header, 'rows' => $rows, 'attributes' => array('id' => $table_id, 'class' => array('field-group-multiple-table'))));
  $output .= isset($element['#description']) ? '<div class="description">' . $element['#description'] . '</div>' : '';
  $output .= '</div></div>';

  return $output;
}


function template_preprocess_field_group_multiple_container(&$variables) {
  drupal_add_css(drupal_get_path('module', 'field_group_multiple') . '/field_group_multiple.css');

  $group = $variables['group'];
  $fields = $variables['fields'];
  $items = $variables['items'];

  // unset other children
  foreach (element_children($fields, TRUE) as $field_name) {
    unset($fields[$field_name]['#items']);
    foreach (element_children($fields[$field_name], TRUE) as $nr) {
      unset($fields[$field_name][$nr]);
    }
  }

  $entries = array();
  foreach (element_children($items, TRUE) as $delta) {
    $entry = array();
    $container_row = $items[$delta];
    foreach (element_children($container_row, TRUE) as $field_name) {
      $entry[$field_name] = $fields[$field_name];
      $entry[$field_name]['#items'][0] = $entry[$field_name][0] = $items[$delta][$field_name];
    }
    $entries[$delta] = $entry;
  }

  $variables['entries'] = $entries;
  if (empty($entries)) {
    $variables['is_empty'] = TRUE;
  }
  else {
    $variables['is_empty'] = FALSE;
  }

  if (empty($variables['theme_hook_suggestions'])) $variables['theme_hook_suggestions'] = array();

  $suggestions_prefix = 'field_group_multiple_container';
  $variables['theme_hook_suggestions'] = _template_field_group_multiple_suggestions($suggestions_prefix, $group);
}


function template_preprocess_field_group_multiple_label_value(&$variables) {
  drupal_add_css(drupal_get_path('module', 'field_group_multiple') . '/field_group_multiple.css');

  $group = $variables['group'];
  $fields = $variables['fields'];
  $items = $variables['items'];

  $field_name_label = $group->format_settings['instance_settings']['label_value_field'];
  $arr_label = array($field_name_label);
  $diff = array_diff($group->children, $arr_label);
  $field_name_value = array_shift($diff);

  $tmp_entry = $fields[$field_name_value];
  unset($tmp_entry['#items']);
  // unset other children
  foreach (element_children($tmp_entry, TRUE) as $nr) {
    unset($tmp_entry[$nr]);
  }

  $entries = array();
  foreach (element_children($items, TRUE) as $delta) {
    $values = $items[$delta];
    $entry = $tmp_entry;
    $entry['#title'] = $values[$field_name_label]['#markup'];
    $entry['#items'][0] = $entry[0] = $values[$field_name_value];
    $entries[$delta] = $entry;
  }
  $variables['entries'] = $entries;
  if (empty($entries)) {
    $variables['is_empty'] = TRUE;
  }
  else {
    $variables['is_empty'] = FALSE;
  }

  $suggestions_prefix = 'field_group_multiple_label_value';
  $variables['theme_hook_suggestions'] = _template_field_group_multiple_suggestions($suggestions_prefix, $group);
}


function template_preprocess_field_group_multiple_table(&$variables) {
  drupal_add_css(drupal_get_path('module', 'field_group_multiple') . '/field_group_multiple.css');

  $group = $variables['group'];
  $fields = $variables['fields'];
  $items = $variables['items'];

  $header = array();
  $rows = array();

  $titles = (isset($group->format_settings['instance_settings']['row_titles'])) ? $group->format_settings['instance_settings']['row_titles'] : NULL;
  if ($titles) {
    $row_titles = array_map('check_plain', explode(',', $titles));
    if ($row_titles[0] == '-none-') {
      $row_titles[0] = '';
    }
  }

  if ($titles) {
    $header[] = $row_titles[0];
  }


  $id = $group->group_name;
  foreach (element_children($fields, TRUE) as $field_name) {
    $header[] = t($fields[$field_name]['#title']);
  }

  foreach (element_children($items, TRUE) as $delta) {
    $row_values = $items[$delta];
    $row = array();

    if ($titles) {
      $row_title = '';
      if (($delta + 1) < count($row_titles)) {
        $row_title = $row_titles[$delta + 1];
      }
      $row[] = array('data' => $row_title, 'class' => '');
    }

    foreach (element_children($row_values, TRUE) as $field_name) {
      $data = $row_values[$field_name];
      $row[] = array('data' => drupal_render($data), 'class' => '');
    }
    $rows[] = array('data' => $row, 'class' => array('row-delta-' . $delta));
  }

  $table_id = drupal_html_id($id . '_values');
  $variables['table'] = theme('table', array('header' => $header, 'rows' => $rows, 'attributes' => array('id' => $table_id, 'class' => array('field-group-multiple-table'))));

  $suggestions_prefix = 'field_group_multiple_table';
  $variables['theme_hook_suggestions'] = _template_field_group_multiple_suggestions($suggestions_prefix, $group);
}



function _template_field_group_multiple_suggestions($suggestions_prefix, $group) {
  $theme_hook_suggestions = array();
  $theme_hook_suggestions[] = $suggestions_prefix . '__' . $group->group_name;
  $theme_hook_suggestions[] = $suggestions_prefix . '__' . $group->entity_type;
  $theme_hook_suggestions[] = $suggestions_prefix . '__' . $group->entity_type . '__' . $group->group_name;
  $theme_hook_suggestions[] = $suggestions_prefix . '__' . $group->entity_type . '__' . $group->bundle;
  $theme_hook_suggestions[] = $suggestions_prefix . '__' . $group->entity_type . '__' . $group->bundle . '__' . $group->group_name;
  return $theme_hook_suggestions;
}
