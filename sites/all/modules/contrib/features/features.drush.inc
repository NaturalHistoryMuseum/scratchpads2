<?php

/**
 * @file
 *   Features module drush integration.
 */

/**
 * Implements hook_drush_command().
 * 
 * @See drush_parse_command() for a list of recognized keys.
 *
 * @return
 *   An associative array describing your command(s).
 */
function features_drush_command() {
  $items = array();

  $items['features-list'] = array(
    'description' => "List all the available features for your site.",
    'drupal dependencies' => array('features'),
    'aliases' => array('fl', 'features'),
  );
  $items['features-export'] = array(
    'description' => "Export a feature from your site into a module.",
    'arguments' => array(
      'feature' => 'Feature name to export or a single component.',
      'components' => '(optional) List of components to include, like source:component [source2:component2]...'
    ),
    'options' => array(
      'destination' => "Destination path (from Drupal root) of the exported feature. Defaults to 'sites/all/modules'",
    ),
    'drupal dependencies' => array('features'),
    'aliases' => array('fe'),
  );
  $items['features-add'] = array(
    'description' => "Add a component to a feature module.",
    'drupal dependencies' => array('features'),
    'arguments' => array(
      'feature' => 'Feature name to add to.',
      'components' => 'List of components to add.',
    ),
    'aliases' => array('fa'),
  );
  $items['features-update'] = array(
    'description' => "Update a feature module on your site.",
    'arguments' => array(
      'feature' => 'A space delimited list of features.',
    ),
    'drupal dependencies' => array('features'),
    'aliases' => array('fu'),
  );
  $items['features-update-all'] = array(
    'description' => "Update all feature modules on your site.",
    'arguments' => array(
      'feature_exclude' => 'A space-delimited list of features to exclude from being updated.',
    ),
    'drupal dependencies' => array('features'),
    'aliases' => array('fu-all', 'fua'),
  );
  $items['features-revert'] = array(
    'description' => "Revert a feature module on your site.",
    'arguments' => array(
      'feature' => 'A space delimited list of features.',
    ),
    'options' => array(
      'force' => "Force revert even if Features assumes components' state are default.",
    ),
    'drupal dependencies' => array('features'),
    'aliases' => array('fr'),
  );
  $items['features-revert-all'] = array(
    'description' => "Revert all enabled feature module on your site.",
    'arguments' => array(
      'feature_exclude' => 'A space-delimited list of features to exclude from being reverted.',
    ),
    'options' => array(
      'force' => "Force revert even if Features assumes components' state are default.",
    ),
    'drupal dependencies' => array('features'),
    'aliases' => array('fr-all', 'fra'),
  );
  $items['features-diff'] = array(
    'description' => "Show the difference between the default and overridden state of a feature.",
    'arguments' => array(
      'feature' => 'The feature in question.',
    ),
    'drupal dependencies' => array('features', 'diff'),
    'aliases' => array('fd'),
  );

  return $items;
}

/**
 * Implements hook_drush_help().
 */
function features_drush_help($section) {
  switch ($section) {
    case 'drush:features':
      return dt("List all the available features for your site.");
    case 'drush:features-export':
      return dt("Export a feature from your site into a module. If called with no arguments, display a list of available components. If called with a single argument, attempt to create a feature including the given component with the same name. The option '--destination=foo' may be used to specify the path (from Drupal root) where the feature should be created. The default destination is 'sites/all/modules'.");
    case 'drush:features-update':
      return dt("Update a feature module on your site.");
    case 'drush:features-update-all':
      return dt("Update all feature modules on your site.");
    case 'drush:features-revert':
      return dt("Revert a feature module on your site.");
    case 'drush:features-revert-all':
      return dt("Revert all enabled feature module on your site.");
    case 'drush:features-revert':
      return dt("Revert a feature module on your site.");
    case 'drush:features-diff':
      return dt("Show a diff of a feature module.");
    case 'drush:features-add':
      return dt("Add a component to a feature module.");
  }
}

/**
 * Get a list of all feature modules.
 */
function drush_features_list() {
  module_load_include('inc', 'features', 'features.export');
  $rows = array(array(dt('Name'), dt('Feature'), dt('Status'), dt('State')));
  foreach (features_get_features(NULL, TRUE) as $k => $m) {
    switch (features_get_storage($m->name)) {
      case FEATURES_DEFAULT:
      case FEATURES_REBUILDABLE:
        $storage = '';
        break;
      case FEATURES_OVERRIDDEN:
        $storage = dt('Overridden');
        break;
      case FEATURES_NEEDS_REVIEW:
        $storage = dt('Needs review');
        break;
    }
    $rows[] = array(
      $m->info['name'],
      $m->name,
      $m->status ? dt('Enabled') : dt('Disabled'),
      $storage
    );
  }
  drush_print_table($rows, TRUE);
}

/**
 * Create a feature module based on a list of components.
 */
function drush_features_export() {
  $args = func_get_args();

  if (count($args) == 1) {
    // Assume that the user intends to create a module with the same name as the
    // "value" of the component.
    list($source, $component) = explode(':', $args[0]);
    $stub = array($source => array($component));
    $sources = features_get_components();
    if (empty($component) || empty($sources[$source])) {
      _features_drush_set_error($args[0], 'FEATURES_COMPONENT_NOT_FOUND');
      exit;
    }
    else {
      _drush_features_export($stub, $component);
    }
  }
  elseif (count($args) > 1) {
    // Assume that the user intends to create a new module based on a list of 
    // components. First argument is assumed to be the name.
    $name = array_shift($args);
    $stub = array();
    foreach ($args as $v) {
      list($source, $component) = explode(':', $v);
      $stub[$source][] = $component;
    }
    _drush_features_export($stub, array(), $name);
  }
  else {
    $rows = array(array(dt('Available sources')));
    foreach (features_get_components(TRUE) as $component => $info) {
      if ($options = features_invoke($component, 'features_export_options')) {
        foreach ($options as $key => $value) {
          $rows[] = array($component .':'. $key);
        }
      }
    }
    drush_print_table($rows, TRUE);
  }
}

 /**
 * Add a component to a features module.
 */
function drush_features_add() {
  if ($args = func_get_args()) {
    // Get a complete list of components.
    $all_components = array();
    foreach (features_get_components(TRUE) as $source => $info) {
      if ($options = features_invoke($source, 'features_export_options')) {
        foreach ($options as $key => $value) {
          $all_components[$key][] = $source;
        }
      }
    }
    $module = array_shift($args);
    while ($arg = trim(array_shift($args))) {
      list($source, $component) = explode(':', $arg);
      if ($component) {
        if (isset($all_components[$component]) && in_array($source, $all_components[$component])) {
          $items[$source][] = $component;
        }
        else {
          $choice = _drush_features_component_find($component, $source);
          if ($choice !== FALSE) {
            drush_print(dt('Selected component "!component".', array('!component' => $choice->source . ':' . $choice->component)));
            $items[$choice->source][] = $choice->component;
            continue;
          }
          return drush_set_error('', dt('Unknown component !arg', array('!arg' => $arg)));
        }
      }
      else {
        if ($all_components[$source] && sizeof($all_components[$source]) == 1) {
          $items[$all_components[$source][0]][] = $source;
        }
        elseif (isset($all_components[$source])) {
          return drush_set_error('', dt('Ambiguous component !component, possible sources: !sources', array('!component' => $source, '!sources' => join(', ', $all_components[$source]))));
        }
        else {
          $choice = _drush_features_component_find($component, $source);
          if ($choice !== FALSE) {
            drush_print(dt('Selected component "!component".', array('!component' => $choice->source . ':' . $choice->component)));
            $items[$choice->source][] = $choice->component;
            continue;
          }
          return drush_set_error('', dt('Unknown component !arg', array('!arg' => $arg)));
        }
      }
    }

    if (!$items) {
      // Print out a list of available items.
      _drush_features_component_list();
      return;
    }

    if (($feature = feature_load($module, TRUE)) && module_exists($module)) {
      module_load_include('inc', 'features', 'features.export');
      _features_populate($items, $feature->info, $feature->name);
      _drush_features_export($feature->info['features'], $feature->info['dependencies'], $feature->name, dirname($feature->filename));
    }
    elseif ($feature) {
      _features_drush_set_error($module, 'FEATURES_FEATURE_NOT_ENABLED');
    }
    else {
      _features_drush_set_error($module);
    }
  }
  else {
    // By default just show features that are available.
    $rows = array(array(dt('Available features')));
    foreach (features_get_features(NULL, TRUE) as $name => $info) {
      $rows[] = array($name);
    }
    drush_print_table($rows, TRUE);
  }
}

/**
 * Help the user select a Features component if one can be recommended.
 *
 * @param $arg
 *   A specific component to search for. Used as variable-get command.
 * @param $source
 *   Restrict component lookup to the specified source.
 * @param $limit
 *   Limit the number of options that will be offered. If "0", no limit. If the
 *   limit is reached, send back all. Default: 10.
 */
function _drush_features_component_find($arg = NULL, $source = NULL, $limit = 10) {
  $components = _drush_features_component_list($arg, $source, FALSE);
  $count = count($components);

  if (!empty($components)) {
     if ($limit > 0 && $count > $limit) {
       $proceed = drush_confirm(dt('There are !count components that may match "!component". Review all?', array(
         '!count' => $count,
         '!component' => $source . ':' . $arg,
       )));
     }
     else {
       $proceed = TRUE;
     }

     if ($proceed) {
       $choice = drush_choice($components, 'Enter a number to choose which component to use.');
       if ($choice !== FALSE) {
         $retn = new stdClass;
         list ($retn->source, $retn->component) = explode(':', $components[$choice]);
         return $retn;
       }
     }
  }
  else {
    $components = _drush_features_component_list($arg, $source, TRUE);
  }
  return FALSE;
}

/**
 * List all possible features components.
 *
 * @param $arg
 *   A specific component to search for. Used as variable-get command.
 * @param $source
 *   Restrict component lookup to the specified source.
 * @param $render
 *   Determine whether the results will be immediately printed to the screen or
 *   returned for further processing.
 */
function _drush_features_component_list($arg = NULL, $source = NULL, $render = TRUE) {
  static $items;

  $index = empty($source) ? 'all' : $source;
  $index .= ':';
  $index .= empty($arg) ? 'all' : $arg;

  if (empty($items[$index])) {
    $components = features_get_components(TRUE);
    if ($arg) {
      $heading = dt('Components similar to "!arg"', array('!arg' => $arg));
    }
    else {
      $heading = dt('Available components');
    }
    if ($source) {
      $heading .= ' ' . dt('in category "!source"', array('!source' => $source));
      if (array_key_exists($source, $components)) {
        $components = array($source => $components[$source]);
      }
      else {
        return FALSE;
      }
    }

    $rows = array(array($heading));
    foreach ($components as $component => $info) {
      if ($options = features_invoke($component, 'features_export_options')) {
        foreach ($options as $key => $value) {
          if (empty($arg) || strpos($key, $arg) !== FALSE) {
            $rows[$component .':'. $key] = array($component .':'. $key);
          }
        }
      }
    }
    $items[$index] = $rows;
  }
  if ($render) {
    drush_print_table(array_values($items[$index]), TRUE);
    return array_keys($items[$index]);
  }
  return array_slice(array_keys($items[$index]), 1);
}

/**
 * Update an existing feature module.
 */
function drush_features_update() {
  if ($args = func_get_args()) {
    foreach ($args as $module) {
      if (($feature = feature_load($module, TRUE)) && module_exists($module)) {
        _drush_features_export($feature->info['features'], $feature->info['dependencies'], $feature->name, dirname($feature->filename));
      }
      else if ($feature) {
        _features_drush_set_error($module, 'FEATURES_FEATURE_NOT_ENABLED');
      }
      else {
        _features_drush_set_error($module);
      }
    }
  }
  else {
    // By default just show contexts that are available.
    $rows = array(array(dt('Available features')));
    foreach (features_get_features(NULL, TRUE) as $name => $info) {
      $rows[] = array($name);
    }
    drush_print_table($rows, TRUE);
  }
}

/**
 * Update all enabled features. Optionally pass in a list of features to
 * exclude from being updated.
 */
function drush_features_update_all() {
  $features_to_update = array();
  $features_to_exclude = func_get_args();
  foreach (features_get_features() as $module) {
    if ($module->status && !in_array($module->name, $features_to_exclude)) {
      $features_to_update[] = $module->name;
    }
  }
  drush_print(dt('The following modules will be updated: !modules', array('!modules' => implode(', ', $features_to_update))));
  if (drush_confirm(dt('Do you really want to continue?'))) {
    foreach ($features_to_update as $module_name) {
      drush_invoke_process(drush_sitealias_get_record('@self'), 'features-update', array($module_name));
    }
  }
  else {
    drush_die('Aborting.');
  }
}

/**
 * Write a module to the site dir.
 *
 * @param $requests
 *   An array of the context requested in this export.
 * @param $module_name
 *  Optional. The name for the exported module.
 */
function _drush_features_export($stub, $dependencies, $module_name = NULL, $directory = NULL) {
  $root = drush_get_option(array('r', 'root'), drush_locate_root());
  if ($root) {
    $destination = drush_get_option(array('destination'), 'sites/all/modules');
    $directory = isset($directory) ? $directory : $destination . '/' . $module_name;
    if (is_dir($directory)) {
      drush_print(dt('Module appears to already exist in !dir', array('!dir' => $directory)));
      if (!drush_confirm(dt('Do you really want to continue?'))) {
        drush_die('Aborting.');
      }
    }
    else {
      drush_op('mkdir', $directory);
    }
    if (is_dir($directory)) {
      drupal_flush_all_caches();
      module_load_include('inc', 'features', 'features.export');
      $export = features_populate($stub, $dependencies, $module_name);
      if (!feature_load($module_name)) {
        $export['name'] = $module_name;
      }
      $files = features_export_render($export, $module_name, TRUE);
      foreach ($files as $extension => $file_contents) {
        if (!in_array($extension, array('module', 'info'))) {
          $extension .= '.inc';
        }
        drush_op('file_put_contents', "{$directory}/{$module_name}.$extension", $file_contents);
      }
      drush_log(dt("Created module: !module in !directory", array('!module' => $module_name, '!directory' => $directory)), 'ok');
    }
    else {
      drush_die(dt('Couldn\'t create directory !directory', array('!directory' => $directory)));
    }
  }
  else {
    drush_die(dt('Couldn\'t locate site root'));
  }
}

/**
 * Revert a feature to it's code definition.
 */
function drush_features_revert() {
  if ($args = func_get_args()) {
    module_load_include('inc', 'features', 'features.export');
    features_include();

    // Determine if revert should be forced.
    $force = drush_get_option('force');
    foreach ($args as $module) {
      if (($feature = feature_load($module, TRUE)) && module_exists($module)) {

        $components = array();
        // Forcefully revert all components of a feature.
        if ($force) {
          foreach (array_keys($feature->info['features']) as $component) {
            if (features_hook($component, 'features_revert')) {
              $components[] = $component;
            }
          }
        }
        // Only revert components that are detected to be Overridden/Needs review.
        else {
          $states = features_get_component_states(array($feature->name), FALSE);
          foreach ($states[$feature->name] as $component => $state) {
            if (in_array($state, array(FEATURES_OVERRIDDEN, FEATURES_NEEDS_REVIEW)) && features_hook($component, 'features_revert')) {
              $components[] = $component;
            }
          }
        }

        if (empty($components)) {
          drush_log(dt('Current state already matches defaults, aborting.'), 'ok');
        }
        else {
          foreach ($components as $component) {
            if (drush_confirm(dt('Do you really want to revert !component?', array('!component' => $component)))) {
              features_revert(array($module => array($component)));
              drush_log(dt('Reverted !component.', array('!component' => $component)), 'ok');
            }
            else {
              drush_log(dt('Skipping !component.', array('!component' => $component)), 'ok');
            }
          }
        }
      }
      else if ($feature) {
        _features_drush_set_error($module, 'FEATURES_FEATURE_NOT_ENABLED');
      }
      else {
        _features_drush_set_error($module);
      }
    }
  }
  else {
    drush_features_list();
    return;
  }
}

/**
 * Revert all enabled features to their definitions in code. Optionally pass in
 * a list of features to exclude from being reverted.
 */
function drush_features_revert_all() {
  $features_to_revert = array();
  $features_to_exclude = func_get_args();
  $force = drush_get_option('force');
  foreach (features_get_features() as $module) {
    if ($module->status && !in_array($module->name, $features_to_exclude)) {
      $features_to_revert[] = $module->name;
    }
  }
  drush_print(dt('The following modules will be reverted: !modules', array('!modules' => implode(', ', $features_to_revert))));
  if (drush_confirm(dt('Do you really want to continue?'))) {
    foreach ($features_to_revert as $module_name) {
      drush_invoke_process(drush_sitealias_get_record('@self'), 'features-revert', array($module_name), array('force' => $force, '#integrate' => TRUE));
    }
  }
  else {
    drush_die('Aborting.');
  }
}

/**
 * Show the diff of a feature module.
 */
function drush_features_diff() {
  if (!$args = func_get_args()) {
    drush_features_list();
    return;
  }
  $module = $args[0];
  $feature = feature_load($module);
  if (!module_exists($module)) {
    drush_log(dt('No such feature is enabled: ' . $module), 'error');
    return;
  }
  module_load_include('inc', 'features', 'features.export');
  $overrides = features_detect_overrides($feature);
  if (empty($overrides)) {
    drush_log(dt('Feature is in its default state. No diff needed.'), 'ok');
    return;
  }
  module_load_include('inc', 'diff', 'diff.engine');

  if (!class_exists('DiffFormatter')) {
    if (drush_confirm(dt('It seems that the Diff module is not available. Would you like to download and enable it?'))) {
      // Download it if it's not already here.
      $project_info = drush_get_projects();
      if (empty($project_info['diff']) && !drush_invoke_process(drush_sitealias_get_record('@self'), 'dl', array('diff'), array('#integrate' => TRUE))) {
        return drush_set_error(dt('Diff module could not be downloaded.'));
      }

      if (!drush_invoke_process(drush_sitealias_get_record('@self'), 'en', array('diff'), array('#integrate' => TRUE))) {
        return drush_set_error(dt('Diff module could not be enabled.'));
      }
    }
    else {
      return drush_set_error(dt('Diff module is not enabled.'));
    }
    // If we're still here, now we can include the diff.engine again.
    module_load_include('inc', 'diff', 'diff.engine');
  }

  $formatter = new DiffFormatter();
  $formatter->leading_context_lines = 2;
  $formatter->trailing_context_lines = 2;
  $formatter->show_header = FALSE;

  if (drush_get_context('DRUSH_NOCOLOR')) {
    $red = $green = "%s";
  }
  else {
    $red = "\033[31;40m\033[1m%s\033[0m";
    $green = "\033[0;32;40m\033[1m%s\033[0m";
  }

  // Print key for colors
  drush_print(dt('Legend: '));
  drush_print(sprintf($red,   dt('Code:       drush features-revert will remove the overrides.')));
  drush_print(sprintf($green, dt('Overrides:  drush features-update will update the exported feature with the displayed overrides')));
  drush_print();

  foreach ($overrides as $component => $items) {
    $diff = new Diff(explode("\n", $items['default']), explode("\n", $items['normal']));
    drush_print();
    drush_print(dt("Component: !component", array('!component' => $component)));
    $rows = explode("\n", $formatter->format($diff));
    foreach ($rows as $row) {
      if (strpos($row, '>') === 0) {
        drush_print(sprintf($green, $row));
      }
      elseif (strpos($row, '<') === 0) {
        drush_print(sprintf($red, $row));
      }
      else {
        drush_print($row);
      }
    }
  }
}

/**
 * Helper function to call drush_set_error().
 *
 * @param $feature
 *   The string name of the feature.
 * @param $error
 *   A text string identifying the type of error.
 * @return
 *   FALSE.  See drush_set_error().
 */
function _features_drush_set_error($feature, $error = '') {
  $args = array('!feature' => $feature);

  switch ($error) {
    case 'FEATURES_FEATURE_NOT_ENABLED':
      $message = 'The feature !feature is not enabled.';
      break;
    case 'FEATURES_COMPONENT_NOT_FOUND':
      $message = 'The given component !feature could not be found.';
      break;
    default:
      $error = 'FEATURES_FEATURE_NOT_FOUND';
      $message = 'The feature !feature could not be found.';
  }

  return drush_set_error($error, dt($message, $args));
}
