<?php
/**
 * @file field_group_multiple.field_ui.inc
 */

function field_group_multiple_field_ui_overview_form_alter(&$form, &$form_state, $display_overview = FALSE) {
  $form['#validate'][] = "field_group_multiple_field_overview_validate";
  $form['#submit'][] = "field_group_multiple_field_overview_submit";
}


function field_group_multiple_field_overview_validate($form, &$form_state) {
  _field_group_multiple_field_ui_overview_operation($form, $form_state, 'validate');
}


function field_group_multiple_field_overview_submit($form, &$form_state) {
  _field_group_multiple_field_ui_overview_operation($form, $form_state, 'submit');
}


function _field_group_multiple_field_ui_overview_operation($form, &$form_state, $op) {
  if (!isset($form['fields'])) return;

  $map = array();
  foreach (element_children($form['fields'], TRUE) as $field_name) {
    if (isset($form['fields'][$field_name]['#row_type']) && $form['fields'][$field_name]['#row_type'] == 'field') {
      if (isset($form['fields'][$field_name]['parent_wrapper']['parent']['#value']) && !empty($form['fields'][$field_name]['parent_wrapper']['parent']['#value'])) {
        $map[$field_name] = $form['fields'][$field_name]['parent_wrapper']['parent']['#value'];
      }
    }
  }

  if (isset($form_state['values']['fields'])) {
    foreach ($form_state['values']['fields'] as $field_name => $data) {
      if (isset($data['format']['type']) && _is_field_group_multiple($data['format']['type'])) {
        $function = '_' . $data['format']['type'] . '_format_' . $op;
        if (function_exists($function)) {
          $group = new stdClass();
          $group->group_name = $field_name;
          $group->format_type = $data['format']['type'];

          $unsorted = array_keys($map, $field_name);
          $sorted = array();
          foreach ($unsorted as $unsorted_field) {
            $sorted[$unsorted_field] = array('#weight' => $form_state['values']['fields'][$unsorted_field]['weight']);
          }
          $group->children = element_children($sorted, TRUE);
          $function($form, $form_state, $group);
        }
      }
    }
  }
}


function _field_group_multiple_get_fields_infos($field_names, $key) {
  $cardinalities = array();
  foreach ($field_names as $field_name) {
    $instance = field_info_field($field_name);
    $cardinalities[$field_name] = $instance[$key];
  }
  return $cardinalities;
}


function _field_group_multiple_allowed_parent_formatter($view_mode) {
  // TODO currently only two formatter allowed, check the other
  $allowed_formatter = array("div", "fieldset");
  $view_mode = $view_mode == 'form' ? $view_mode : 'display';
  $formatter = field_group_formatter_info();
  $formatter = $formatter[$view_mode];
  $parent_formatter = array("" => t("None"));
  foreach ($formatter as $key => $value) {
    if (in_array($key, $allowed_formatter)) {
      $parent_formatter[$key] = $value['label'];
    }
  }
  return $parent_formatter;
}


/**
 * Default settings
 * TODO attach validate and submit functions directly to the element
 */
function _field_group_multiple_default_format_settings($group) {
  $form = array();

  $default_parent_type = isset($group->format_settings['instance_settings']['parent_format_type']) ? $group->format_settings['instance_settings']['parent_format_type'] : "";

  $formatter_options = _field_group_multiple_allowed_parent_formatter($group->mode);

  $form['instance_settings']['parent_format_type'] = array(
    '#title' => t("Parent format type"),
    '#type' => 'select',
    '#options' => $formatter_options,
    '#default_value' => $default_parent_type,
    '#attributes' => array('class' => array('field-group-parent-type')),
  );


  if (!empty($default_parent_type)) {
    $field_group_types = field_group_formatter_info();
    $mode = $group->mode == 'form' ? 'form' : 'display';
    $formatter = $field_group_types[$mode][$default_parent_type];

    $form['instance_settings']['show_label'] = array(
      '#title' => t('Show label'),
      '#type' => 'select',
      '#options' => array(0 => t('No'), 1 => t('Yes')),
      '#default_value' => isset($group->format_settings['instance_settings']['show_label']) ? $group->format_settings['instance_settings']['show_label'] : $formatter['instance_settings']['show_label'],
      '#weight' => 2,
    );

    $form['instance_settings']['label_element'] = array(
      '#title' => t('Label element'),
      '#type' => 'select',
      '#options' => array('h2' => t('Header 2'), 'h3' => t('Header 3')),
      '#default_value' => isset($group->format_settings['instance_settings']['label_element']) ? $group->format_settings['instance_settings']['label_element'] : $formatter['instance_settings']['label_element'],
      '#weight' => 2,
    );

    $form['instance_settings']['effect'] = array(
      '#title' => t('Effect'),
      '#type' => 'select',
      '#options' => array('none' => t('None'), 'blind' => t('Blind')),
      '#default_value' => isset($group->format_settings['instance_settings']['effect']) ? $group->format_settings['instance_settings']['effect'] : $formatter['instance_settings']['effect'],
      '#weight' => 2,
    );

    $form['instance_settings']['speed'] = array(
      '#title' => t('Speed'),
      '#type' => 'select',
      '#options' => array('none' => t('None'), 'slow' => t('Slow'), 'fast' => t('Fast')),
      '#default_value' => isset($group->format_settings['instance_settings']['speed']) ? $group->format_settings['instance_settings']['speed'] : $formatter['instance_settings']['speed'],
      '#weight' => 3,
    );
  }
  return $form;
}

/**
 * Default summary
 */
function _field_group_multiple_default_format_summary($group) {
  $cardinalities = _field_group_multiple_get_fields_infos($group->children, 'cardinality');
  $min_cardinality = _field_group_multiple_get_max_cardinality($cardinalities);
  $info = '<em>This value depends on the shortest selected "number of values" of the grouped fields.</em>';
  if ($min_cardinality == FIELD_CARDINALITY_UNLIMITED) {
    return '<div class="description">' . t('The maximal amount of items that can be added is unlimited.<br/>' . $info , array('!count' => $min_cardinality)) . '</div>';
  }
  else {
    return '<div class="description">' . t('The maximal amount of items that can be added is !count.<br/>' . $info , array('!count' => $min_cardinality)) . '</div>';
  }

}

function _field_group_multiple_default_format_validate($form, &$form_state, $group) {
  $cardinalities = _field_group_multiple_get_fields_infos($group->children, 'cardinality');
  if (!empty($cardinalities)) {
    if (min($cardinalities) != max($cardinalities)) {
      form_set_error("fields[" . $group->group_name . "][format][type]",
      t('Field cardinalities in group !group_name must be equal.', array('!group_name' => $group->group_name)));
    }
  }
}

function _field_group_multiple_default_format_submit($form, &$form_state, $group) {

}


/**
 * Table settings
 */
function _field_group_multiple_table_format_settings($group) {

  $form = _field_group_multiple_default_format_settings($group);

  $form['instance_settings']['row_titles'] = array(
    '#title' => t("Row Titles"),
    '#description' => t("If you want col 1 to render as row titles enter them as csv's, the first value is the col header, use -none- as the first to omit header e.g -none-,Red,Green,Blue or Colors,Red,Green,Blue ."),
    '#type' => 'textarea',
    '#default_value' => isset($group->format_settings['instance_settings']['row_titles']) ? $group->format_settings['instance_settings']['row_titles'] : '',
  );

  return $form;
}


function _field_group_multiple_table_format_summary($group) {
  return _field_group_multiple_default_format_summary($group);
}

function _field_group_multiple_table_format_validate($form, &$form_state, $group) {
  _field_group_multiple_default_format_validate($form, $form_state, $group);
}




function _field_group_multiple_table_format_submit($form, &$form_state, $group) {
  _field_group_multiple_default_format_submit($form, $form_state, $group);
}


function _field_group_multiple_container_format_settings($group) {
  $form = _field_group_multiple_default_format_settings($group);
  return $form;
}


function _field_group_multiple_container_format_summary($group) {
  return _field_group_multiple_default_format_summary($group);
}


function _field_group_multiple_container_format_validate($form, &$form_state, $group) {
  _field_group_multiple_default_format_validate($form, $form_state, $group);
}


function _field_group_multiple_container_format_submit($form, &$form_state, $group) {

}


function _field_group_multiple_label_value_format_settings($group) {
  $form = _field_group_multiple_default_format_settings($group);

  $label_field = isset($group->format_settings['instance_settings']['label_value_field']) ? $group->format_settings['instance_settings']['label_value_field'] : "";

  $fields = array();
  foreach ($group->children as $field_name) {
    $field = field_info_field($field_name);
    if ($field['type'] == 'text') {
      $fields[$field_name] = $field_name;
    }
  }

  $form['instance_settings']['label_value_field'] = array(
    '#title' => t("Field with the label"),
    '#type' => 'select',
    '#options' => $fields,
    '#default_value' => $label_field,
    '#attributes' => array('class' => array('field-group-label-field')),
  );
  return $form;
}


function _field_group_multiple_label_value_format_summary($group) {
  return _field_group_multiple_default_format_summary($group);
}


function _field_group_multiple_label_value_format_validate($form, &$form_state, $group) {
  // TODO check if this works
  _field_group_multiple_default_format_validate($form, $form_state, $group);

  if (isset($form_state['values']['fields'][$group->group_name]['format_settings'])) {
    $_form = $form_state['values']['fields'][$group->group_name]['format_settings'];

    $label_field = isset($_form['settings']['instance_settings']['label_value_field']) ? $_form['settings']['instance_settings']['label_value_field'] : NULL;
    if (is_null($label_field) || empty($label_field)) {
      form_set_error("fields[" . $group->group_name . "]", t('Label field must be selected in group "!group_name"', array('!group_name' => $group->group_name)));
    }
    else {
    }
  }

  if (count($group->children) != 2) {
    form_set_error("fields[" . $group->group_name . "]", t('Only two fields can be attached to "!group_name" because the group-type is of label-value.', array('!group_name' => $group->group_name)));
  }
}


function _field_group_multiple_label_value_format_submit($form, &$form_state, $group) {

}
