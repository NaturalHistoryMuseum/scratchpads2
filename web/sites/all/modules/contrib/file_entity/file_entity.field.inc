<?php

/**
 * @file
 * Field API integration for the file_entity module.
 */

/**
 * Implements hook_field_formatter_info().
 */
function file_entity_field_formatter_info() {
  $info['file_rendered'] = array(
    'label' => t('Rendered file'),
    'description' => t('Display the file in a specific view mode'),
    'field types' => array('file', 'image'),
    'settings' => array('file_view_mode' => 'default'),
  );
  return $info;
}

/**
 * Implements hook_field_formatter_settings_form().
 */
function file_entity_field_formatter_settings_form($field, $instance, $view_mode, $form, &$form_state) {
  $display = $instance['display'][$view_mode];
  $settings = $display['settings'];
  $element = array();

  if ($display['type'] == 'file_rendered') {
    $element['file_view_mode'] = array(
      '#title'   => t('View mode'),
      '#type'    => 'select',
      '#options' => file_entity_view_mode_labels(),
      '#default_value' => $settings['file_view_mode'],
      // Never empty, so no #empty_option
    );
  }

  return $element;
}

/**
 * Implements hook_field_formatter_settings_summary().
 */
function file_entity_field_formatter_settings_summary($field, $instance, $view_mode) {
  $display = $instance['display'][$view_mode];
  $settings = $display['settings'];
  $summary = NULL;

  if ($display['type'] === 'file_rendered') {
    $view_mode_label = file_entity_view_mode_label($settings['file_view_mode'], t('Unknown'));
    $summary = t('View mode: %mode', array('%mode' => $view_mode_label));
  }

  return $summary;
}

/**
 * Implements hook_field_formatter_view().
 */
function file_entity_field_formatter_view($entity_type, $entity, $field, $instance, $langcode, $items, $display) {
  $settings = $display['settings'];
  $element = array();

  if ($display['type'] == 'file_rendered') {
    $view_mode = $settings['file_view_mode'];

    // To prevent infinite recursion caused by reference cycles, we store
    // diplayed nodes in a recursion queue.
    $recursion_queue = &drupal_static(__FUNCTION__, array());

    // If no 'referencing entity' is set, we are starting a new 'reference
    // thread' and need to reset the queue.
    // @todo Bug: $entity->referencing_entity on files referenced in a different
    // thread on the page. E.g: 1 references 1+2 / 2 references 1+2 / visit homepage.
    // We'd need a more accurate way...
    if (!isset($entity->referencing_entity)) {
      $recursion_queue = array();
    }

    // The recursion queue only needs to track files.
    if ($entity_type == 'file') {
      list($id) = entity_extract_ids($entity_type, $entity);
      $recursion_queue[$id] = $id;
    }

    // Prevent 'empty' fields from causing a WSOD.
    $items = array_filter($items);

    // Check the recursion queue to determine which nodes should be fully
    // displayed, and which nodes will only be displayed as a title.
    $files_display = array();
    foreach ($items as $delta => $item) {
      if (!isset($recursion_queue[$item['fid']])) {
        $files_display[$item['fid']] = file_load($item['fid']);
      }
    }

    // Load and build the fully displayed nodes.
    if ($files_display) {
      foreach ($files_display as $fid => $file) {
        $files_display[$fid]->referencing_entity = $entity;
        $files_display[$fid]->referencing_field = $field['field_name'];
      }
      $files_built = file_view_multiple($files_display, $view_mode);
    }

    // Assemble the render array.
    foreach ($items as $delta => $item) {
      if (isset($files_built[$item['fid']])) {
        $element[$delta] = $files_built[$item['fid']];
      }
    }
  }

  return $element;
}
