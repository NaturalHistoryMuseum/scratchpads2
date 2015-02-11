<?php

/**
 * Implementation of hook_drush_command().
 */
function webform_drush_command() {
  $items = array();

  $items['webform-export'] = array(
    'description' => 'Exports webform data to a file.',
    'arguments' => array(
      'nid' => 'The node ID of the webform you want to export (required)',
    ),
    'options' => array(
      'file' => 'The file path to export to (defaults to print to stdout)',
      'format' => 'The exporter format to use. Out-of-the-box this may be "delimited" or "excel".',
      'delimiter' => 'Delimiter between columns (defaults to site-wide setting). This option may need to be wrapped in quotes. i.e. --delimter="\t".',
      'components' => 'Comma-separated list of components IDs to include in the export. May also include the built-in special options "serial", "sid", "time", "draft", "ip_address", "uid", and "username".',
      'header-keys' => 'Integer 0 or 1 value. Set to 1 to print header components by their field keys instead of labels.',
      'select-keys' => 'Integer 0 or 1 value. Set to 1 to print select list values by their field keys instead of labels.',
      'select-format' => 'Set to "separate" (default) or "compact" to determine how select list values are exported.',
      'range-type' => 'To export a specific range of submissions, specify one of the following range types: "all", "new", "latest", or "range".',
      'range-latest' => 'Integer specifying the latest X submissions will be downloaded. Used if "range-type" is "latest" or no other range options are provided.',
      'range-start' => 'Alternative to "start" option. The submission ID at which to start exporting. Used if "range-type" is "range" or no other range options are provided.',
      'range-end' => 'Alternative to "end" option. The submission ID at which to end exporting. Used if "range-type" is "range" or no other range options are provided.',
      'batch-size' => 'The size of batches in rows (default 10000). If encoutering out of memory errors, set this number lower to export fewer submissions per batch.',
      'skip-header' => 'Skip output of header rows (defaults to printing header)',
    ),
    'aliases' => array('wfx'),
  );

  return $items;
}

/**
 * Exports a webform in a series of batches, useful for large data dumps that
 * would otherwise time out due to memory consumption.
 *
 * @param int $nid
 *   Node ID of the webform that we want to export.
 * @param $file_name
 *   The name of the file where we're temporarily storing the exported webform
 *   data. Because this function calls itself repeatedly, you never need to
 *   specify a filename. It will be automatically generated on the first run,
 *   and will continue to use that file on all subsequent runs.
 * @param $batch_number
 *   Number of this particular batch run. Like $file_name, this shouldn't be
 *   called explicitly, but is passed back to this function on subsequent runs.
 */
function drush_webform_export($nid = FALSE, $file_name = NULL, $batch_number = NULL) {
  if (!$nid) {
    return drush_set_error('The node ID of the webform you want to export is required');
  }

  module_load_include('inc', 'webform', 'includes/webform.submissions');
  module_load_include('inc', 'webform', 'includes/webform.export');
  module_load_include('inc', 'webform', 'includes/webform.components');
  module_load_include('inc', 'webform', 'includes/webform.report');

  $node = node_load($nid);

  // Pull in options from drush to override the defaults.
  $format = drush_get_option('format', 'delimited');
  $options = webform_results_download_default_options($node, $format);
  foreach ($options as $option_name => $option_value) {
    $options[$option_name] = drush_get_option(str_replace('_', '-', $option_name), $option_value);
  }
  $options['components'] = is_array($options['components']) ? $options['components'] : explode(',', $options['components']);

  // Get the range options.
  $range_input = drush_get_merged_prefixed_options('range-');
  foreach ($range_input as $option_name => $option_value) {
    $option_name = str_replace('range-', '', $option_name);
    $option_name = str_replace('-', '_', $option_name);
    $option_name = $option_name === 'type' ? 'range_type' : $option_name;
    $range_options[$option_name] = $option_value;
  }

  // Determine the range type based on provided input.
  $options['range']['range_type'] = drush_get_option('range-type', NULL);
  if (!isset($options['range']['range_type'])) {
    if (isset($options['range']['start'])) {
      $options['range']['range_type'] = 'range';
    }
    elseif (isset($options['range']['latest'])) {
      $options['range']['range_type'] = 'latest';
    }
    else {
      $options['range']['range_type'] = 'all';
    }
  }

  // Set the export options.
  $options['range']['batch_size'] = drush_get_option('batch-size', 10000);
  $options['file_name'] = drush_get_option('file', tempnam(variable_get('file_directory_temp', file_directory_temp()), 'webform_'));

  $batch = webform_results_export_batch($node, $format, $options);
  batch_set($batch);
  drush_backend_batch_process();

  // If no filename was specified, print the file and delete it.
  if (drush_get_option('file', FALSE) === FALSE) {
    drush_print(file_get_contents($options['file_name']));  // The @ makes it silent.
    @unlink($options['file_name']);  // Clean up, the @ makes it silent.
  }
}
