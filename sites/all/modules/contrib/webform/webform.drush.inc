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
      'header-keys' => 'Integer -1 for none, 0 for label (default) or 1 for field key.',
      'select-keys' => 'Integer 0 or 1 value. Set to 1 to print select list values by their field keys instead of labels.',
      'select-format' => 'Set to "separate" (default) or "compact" to determine how select list values are exported.',
      'range-type' => 'To export a specific range of submissions, specify one of the following range types: "all", "new", "latest", "range" (by sid) or "range-serial".',
      'range-latest' => 'Integer specifying the latest X submissions will be downloaded. Used if "range-type" is "latest" or no other range options are provided.',
      'range-start' => 'Alternative to "start" option. The submission ID at which to start exporting. Used if "range-type" is "range" or no other range options are provided.',
      'range-end' => 'Alternative to "end" option. The submission ID at which to end exporting. Used if "range-type" is "range" or no other range options are provided.',
      'completion-type' => 'Submissions to be included: "finished", "draft" or "all" (default).', 
      'batch-size' => 'The size of batches in rows (default 10000). If encountering out of memory errors, set this number lower to export fewer submissions per batch.',
    ),
    'aliases' => array('wfx'),
  );

  return $items;
}

/**
 * Exports a webform via drush, useful for large data dumps that would otherwise
 * time out due to memory consumption.
 *
 * @param int $nid
 *   Node ID of the webform that we want to export.
 */
function drush_webform_export($nid = FALSE) {
  if (!$nid) {
    return drush_set_error('The node ID of the webform you want to export is required.');
  }
  $node = node_load($nid);
  if (!$node) {
    return drush_set_error(dt('Node !nid was not found.', array('!nid' => $nid)));
  }

  module_load_include('inc', 'webform', 'includes/webform.submissions');
  module_load_include('inc', 'webform', 'includes/webform.export');
  module_load_include('inc', 'webform', 'includes/webform.components');
  module_load_include('inc', 'webform', 'includes/webform.report');


  // Pull in options from drush to override the defaults.
  $format = drush_get_option('format', 'delimited');
  $options = webform_results_download_default_options($node, $format);
  foreach ($options as $option_name => $option_value) {
    $options[$option_name] = drush_get_option(str_replace('_', '-', $option_name), $option_value);
  }
  $options['components'] = is_array($options['components']) ? $options['components'] : explode(',', $options['components']);

  // Get the range options.
  unset($options['range']['range_type']);
  foreach (drush_get_merged_prefixed_options('range-') as $option_name => $option_value) {
    if ($option_name == 'type' && in_array($option_value, array('all', 'new', 'latest', 'range', 'range-serial'))) {
      $options['range']['range_type'] = str_replace('-', '_', $option_value);
    }
    elseif (in_array($option_name, array('start', 'end', 'latest')) && is_numeric($option_value)) {
      $options['range'][$option_name] = $option_value;
    }
    else {
      return drush_set_error(dt('Unsupported range option or argument: !opt=!val',
                                array('!opt' => "range-$option_name", '!val' => $option_value)));
    }
  }
  
  // Determine the range type based on provided input, if not explicitly set.
  if (empty($options['range']['range_type'])) {
    $options['range']['range_type'] = isset($options['range']['start'])
                                        ? 'range'
                                        : (isset($options['range']['latest'])
                                           ? 'latest'
                                           : 'all');
  }
  
  // Set defaults for any missing range arguments.
  switch ($options['range']['range_type']) {
    case 'latest':
      if (empty($options['range']['latest'])) {
        drush_log('Argument range-latest defaulted to 100.', 'ok');
        $options['range']['latest'] = 100;
      }
      break;
    case 'range':
    case 'range_serial':
      if (empty($options['range']['start'])) {
        $options['range']['start'] = 1;
      }
      break;
  }

  // Get the preferred completion type
  $options['range']['completion_type'] = drush_get_option('completion-type', NULL); 
  if (isset($options['range']['completion_type']) && !in_array($options['range']['completion_type'], array('finished', 'draft', 'all'))) {
    return drush_set_error('Unsupported completion-type. The available options are "finished", "draft", or "all".');
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
