<?php

/**
 * @file
 *   Provides several different handlers for exporting webform results.
 */

/**
 * Implements hook_webform_exporters().
 *
 * Defines the exporters this module implements.
 *
 * @return
 *   An "array of arrays", keyed by content-types. The 'handler' slot
 *   should point to the PHP class implementing this flag.
 */
function webform_webform_exporters() {
  $exporters = array(
    'delimited' => array(
      'title' => t('Delimited text'),
      'description' => t('A plain text file delimited by commas, tabs, or other characters.'),
      'handler' => 'webform_exporter_delimited',
      'file' => drupal_get_path('module', 'webform') . '/includes/exporters/webform_exporter_delimited.inc',
      'weight' => 10,
    ),
    'excel' => array(
      'title' => t('Microsoft Excel'),
      'description' => t('A file readable by Microsoft Excel.'),
      'handler' => 'webform_exporter_excel_xlsx',
      'file' => drupal_get_path('module', 'webform') . '/includes/exporters/webform_exporter_excel_xlsx.inc',
      'weight' => -1,
      // Tells the options to use consistent dates instead of user-defined
      // formats.
      'options' => array(
        'iso8601_time' => TRUE,
        'iso8601_date' => TRUE,
      ),
    ),
    'excel_legacy' => array(
      'title' => t('Microsoft Excel (older versions)'),
      'description' => t('A file readable by older versions of Microsoft Excel.'),
      'handler' => 'webform_exporter_excel_delimited',
      'file' => drupal_get_path('module', 'webform') . '/includes/exporters/webform_exporter_excel_delimited.inc',
      'weight' => 0,
    ),
  );

  // The new Excel exporter is only available if ZipArchive is available.
  if (!class_exists('ZipArchive')) {
    unset($exporters['excel']);
  }
  // By default the legacy Excel exporter is disabled.
  if (!webform_variable_get('webform_excel_legacy_exporter')) {
    unset($exporters['excel_legacy']);
  }

  return $exporters;
}

/**
 * Return a list of exporters suitable for display in a select list.
 */
function webform_export_list() {
  $exporters = webform_export_fetch_definition();
  $list = array();
  foreach ($exporters as $name => $exporter) {
    $list[$name] = $exporter['title'];
  }
  return $list;
}

/**
 * Returns a Webform exporter definition.
 */
function webform_export_fetch_definition($format = NULL) {
  static $cache;
  if (!isset($cache)) {
    $cache = module_invoke_all('webform_exporters');
    drupal_alter('webform_exporters', $cache);
    foreach ($cache as $key => $exporter) {
      $cache[$key] += array('weight' => 0);
      $cache[$key]['name'] = $key; // Used in sorting.
    }
    uasort($cache, '_webform_components_sort');
  }

  if (isset($format)) {
    if (isset($cache[$format])) {
      return $cache[$format];
    }
  }
  else {
    return $cache;
  }
}

/**
 * Instantiates a new Webform handler based on the format.
 */
function webform_export_create_handler($format, $options) {
  $definition = webform_export_fetch_definition($format);
  if (isset($definition['file'])) {
    include_once $definition['file'];
  }
  if (isset($definition)) {
    $handler = new $definition['handler']($options);
  }
  else {
    // TODO: Create a default broken exporter.
    $handler = new webform_exporter_broken($options);
  }

  return $handler;
}
