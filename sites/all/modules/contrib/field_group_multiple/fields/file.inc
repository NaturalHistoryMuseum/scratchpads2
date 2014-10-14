<?php
/**
 * @file
 *
 * Override for file_field_widget_form
 *
 */
function field_group_multiple_file_field_widget_form(&$form, &$form_state, $field, $instance, $langcode, $items, $delta, $element) {

  $defaults = array(
    'fid' => 0,
    'display' => !empty($field['settings']['display_default']),
    'description' => '',
  );

  // Load the items for form rebuilds from the field state as they might not be
  // in $form_state['values'] because of validation limitations. Also, they are
  // only passed in as $items when editing existing entities.
  $field_state = field_form_get_state($element['#field_parents'], $field['field_name'], $langcode, $form_state);
  if (isset($field_state['items'])) {
    $items = $field_state['items'];
  }

  // FIELD GROUP MULTIPLE: if delta then map
  if (isset($items[$delta])) {
    $tmp = $items[$delta];
    $items[0] = $tmp;
  }

  // Essentially we use the managed_file type, extended with some enhancements.
  $element_info = element_info('managed_file');
  $element += array(
    '#type' => 'managed_file',
    '#upload_location' => file_field_widget_uri($field, $instance),
    '#upload_validators' => file_field_widget_upload_validators($field, $instance),
    '#value_callback' => 'file_field_widget_value',
  //'#process' => array_merge($element_info['#process'], array('file_field_widget_process')),
    '#process' => array_merge($element_info['#process'], array('field_group_multiple_file_field_widget_process')),  
    '#progress_indicator' => $instance['widget']['settings']['progress_indicator'],
    // Allows this field to return an array instead of a single value.
    '#extended' => TRUE,
  );

  // FIELD GROUP MULTIPLE:
  if (true || $field['cardinality'] == 1) {
    // Set the default value.
    $element['#default_value'] = !empty($items) ? $items[0] : $defaults;
    //$element['#default_value'] = !empty($items) ? $items[$delta] : $defaults;
    // If there's only one field, return it as delta 0.
    if (empty($element['#default_value']['fid'])) {
      $element['#description'] = theme('file_upload_help', array('description' => $element['#description'], 'upload_validators' => $element['#upload_validators']));
    }
    //     $elements = array();
    //     $elements[$delta] = $element;
    //     $elements[$delta]['#weight'] = $delta;
    $elements = array($element);
  }
  else {
    // If there are multiple values, add an element for each existing one.
    foreach ($items as $item) {
      $elements[$delta] = $element;
      $elements[$delta]['#default_value'] = $item;
      $elements[$delta]['#weight'] = $delta;
      $delta++;
    }
    // And then add one more empty row for new uploads except when this is a
    // programmed form as it is not necessary.

    // FIELD GROUP MULTIPLE: Disable Add more button
    //     if (($field['cardinality'] == FIELD_CARDINALITY_UNLIMITED || $delta < $field['cardinality']) && empty($form_state['programmed'])) {
    //       $elements[$delta] = $element;
    //       $elements[$delta]['#default_value'] = $defaults;
    //       $elements[$delta]['#weight'] = $delta;
    //       $elements[$delta]['#required'] = ($element['#required'] && $delta == 0);
    //     }

    // The group of elements all-together need some extra functionality
    // after building up the full list (like draggable table rows).
    $elements['#file_upload_delta'] = $delta;
    $elements['#theme'] = 'file_widget_multiple';
    $elements['#theme_wrappers'] = array('fieldset');
    $elements['#process'] = array('file_field_widget_process_multiple');
    $elements['#title'] = $element['#title'];
    $elements['#description'] = $element['#description'];
    $elements['#field_name'] = $element['#field_name'];
    $elements['#language'] = $element['#language'];
    $elements['#display_field'] = $field['settings']['display_field'];

    // Add some properties that will eventually be added to the file upload
    // field. These are added here so that they may be referenced easily through
    // a hook_form_alter().
    $elements['#file_upload_title'] = t('Add a new file');

    // FIELD GROUP MULTIPLE: modification 0 => $delta
    // $elements['#file_upload_description'] = theme('file_upload_help', array('description' => '', 'upload_validators' => $elements[0]['#upload_validators']));
    $elements['#file_upload_description'] = theme('file_upload_help', array('description' => '', 'upload_validators' => $elements[$delta]['#upload_validators']));
  }

  return $elements;
}

function field_group_multiple_file_field_widget_process($element, &$form_state, $form) {
  $prefix = $element['#prefix'];
  $suffix = $element['#suffix'];

  $backup_ajax = array();
  foreach (element_children($element) as $key) {
    if (isset($element[$key]['#ajax'])) {
      $backup_ajax[$key]['#ajax']['path'] = $element[$key]['#ajax']['path'];
      $backup_ajax[$key]['#ajax']['wrapper'] = $element[$key]['#ajax']['wrapper'];
    }
  }

  $element = file_field_widget_process($element, $form_state, $form);

  // hack for cardinality check
  foreach (element_children($element) as $key) {
    if (isset($backup_ajax[$key]['#ajax'])) {
      $element[$key]['#ajax']['path'] = $backup_ajax[$key]['#ajax']['path'];
      $element[$key]['#ajax']['wrapper'] = $backup_ajax[$key]['#ajax']['wrapper'];
    }
  }

  // restore prefix and suffix
  // Prefix and suffix used for Ajax replacement.
  $element['#prefix'] = $prefix;
  $element['#suffix'] = $suffix;

  return $element;
}


function field_group_multiple_process_field_file_submit(&$values, $field_name, $langcode, $field_info, $form_values) {

  $function = $field_info['module'] . '_field_is_empty';

  foreach ($form_values as $delta => $value) {
    if (isset($value[$field_name])) {

      if (isset($value[$field_name]['0'])) {
        // composite value

        $values[$field_name]['empty'][$delta] = $function($value[$field_name]['0'], $field_info);
        $values[$field_name]['values'][$langcode][$delta] = $value[$field_name]['0'];
        if ($values[$field_name]['empty'][$delta]) {
          $values[$field_name]['values'][$langcode][$delta] = NULL;
        }

      }
      /* else {
        // single value
        $value_key = array_pop($columns);
        $values[$field_name]['empty'][$delta] = $function($value[$field_name], $info);
        $values[$field_name]['values'][$langcode][$delta] = $value[$field_name];
      } */
    }
  }
}
