<?php

/**
 * @file
 * Provide views data for video.module.
 */
module_load_include('inc', 'content', '/includes/views/content.views');

/**
 * Implementation of hook_views_handlers().
 */
function video_views_handlers() {
  return array(
    'info' => array(
      'path' => drupal_get_path('module', 'video') . '/views',
    ),
    'handlers' => array(
      // field handlers
      'video_views_handler_field_data' => array(
        'parent' => 'content_handler_field',
      ),
    ),
  );
}

/**
 * Implementation of hook_views_data()
 */
function video_views_data() {
  $data = array();
  $data['video']['table']['group'] = t('Video');
  $widgets = array('videoftp_widget', 'uploadfield_widget');
  foreach (field_info_fields () as $field) {
    if ($field['module'] == 'filefield' && isset($field['widget']['type']) && in_array($field['widget']['type'], $widgets)) {
      $views_data = content_views_field_views_data($field);
      $table_alias = content_views_tablename($field);
      $db_info = content_database_info($field);

      $title = t('@label (!name) thumbnail', array('@label' => t($field['widget']['label']), '!name' => $field['field_name']));
      $types = array();
      foreach (content_types () as $type) {
        if (isset($type['fields'][$field['field_name']])) {
          // TODO : run check_plain here instead of on the imploded string below ?
          $types[] = $type['name'];
        }
      }

      $additional_fields = array();
      foreach ($db_info['columns'] as $column => $attributes) {
        // Select explicitly enabled field columns.
        if (!empty($attributes['views'])) {
          $db_columns[$column] = $attributes;
        }
        // Ensure all columns are retrieved.
        $additional_fields[$attributes['column']] = $attributes['column'];
      }

      $data[$table_alias][$field['field_name'] . '_thumbnail'] = array(
        'group' => t('Video'),
        'title' => $title,
//        'help' =>  t($field_types[$field['type']]['label']) .' - '. t('Appears in: @types', array('@types' => implode(', ', $types))),
        // #931616
        'help' => t('Appears in: @types', array('@types' => implode(', ', $types))),
      );
      $data[$table_alias][$field['field_name'] . '_thumbnail']['field'] = array(
        'title' => $title,
        'field' => $db_info['columns']['data']['column'],
        'table' => $db_info['table'],
        'handler' => 'video_views_handler_field_data',
        'click sortable' => FALSE,
        'content_field_name' => $field['field_name'],
        'additional fields' => $additional_fields,
        'access callback' => 'content_access',
        'access arguments' => array('view', $field),
      );
    }
  }
  return $data;
}
