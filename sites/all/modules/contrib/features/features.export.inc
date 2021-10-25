<?php

/**
 * @file
 * Contains functions that export configuration into feature modules.
 */

/**
 * Populates an export array with additional keys.
 *
 * @param array $info
 *   Feature info array
 *   Format:
 *     $['features'][$component][$name] = $name
 *     $['dependencies'][$dependency] = $dependency.
 * @param string $module_name
 *   Module name of the feature being generated.
 *
 * @return array
 *   Fully populated export array.
 *   Format:
 *     $['features']['features_api']['api:' . FEATURES_API] = TRUE
 *     $['features'][$component][$name] = $name
 *     $['dependencies'][$dependency] = $dependency
 *     $['conflicts'] = [..]
 *     $['features_exclude'][$component][$name] = $name
 */
function features_populate($info, $module_name) {
  // Sanitize items.
  $items = !empty($info['features']) ? array_filter($info['features']) : array();
  $items['dependencies'] = !empty($info['dependencies']) ? drupal_map_assoc(array_filter($info['dependencies'])) : array();

  // Populate stub.
  $stub = array('features' => array(), 'dependencies' => array(), 'conflicts' => array()) + $info + array('features_exclude' => array());
  $export = _features_populate($items, $stub, $module_name, TRUE);

  // Add Features API version. Any module with this entry in the .info file
  // will be treated as a Feature and included in the admin/build/features UI.
  $export['features']['features_api']['api:' . FEATURES_API] = TRUE;
  // Allow other modules to alter the export.
  /* @see \hook_features_export_alter() */
  drupal_alter('features_export', $export, $module_name);

  // Clean up and standardize order.
  foreach (array_keys($export['features']) as $k) {
    ksort($export['features'][$k]);
  }
  ksort($export['features']);
  ksort($export['dependencies']);
  ksort($export['features_exclude']);

  return $export;
}

/**
 * Populates an export array with additional components from the "pipe".
 *
 * The mechanism allows each component to add objects from other components as
 * dependencies, using hook_features_export().
 *
 * @param string[][] $pipe
 *   Format: $[$component][] = $name
 *   Original list of components.
 * @param array $export
 *   Associative array of items, and module dependencies which define a feature.
 *   Passed by reference.
 *   Format:
 *     $['features'][$component][$name] = $name
 *     $['dependencies'][$dependency] = $dependency
 *     $['conflicts'] = [..]
 *     $['features_exclude'][$component][$name] = $name.
 *   During the recursive calls to this function, the export array will be
 *   filled up with additional components and module dependencies.
 * @param string $module_name
 *   Module name of the feature being generated.
 * @param bool $reset
 *   TRUE, to reset the static cache for this function.
 *
 * @return array
 *   Fully populated $export array.
 *   Format:
 *     $['features'][$component][$name] = $name
 *     $['dependencies'][$dependency] = $dependency
 *     $['conflicts'] = [..]
 *     $['features_exclude'][$component][$name] = $name
 *   The return value is the same as the value of the by-reference $export
 *   parameter after the function was executed.
 *
 * @see \hook_features_export()
 * @see \hook_features_pipe_alter()
 * @see \hook_features_pipe_COMPONENT_alter()
 *
 * @todo The return value is pointless, see #3074424.
 */
function _features_populate($pipe, &$export, $module_name = '', $reset = FALSE) {
  // Ensure that the export will be created in the english language.
  $language = _features_export_language();

  if ($reset) {
    drupal_static_reset(__FUNCTION__);
  }
  $processed = &drupal_static(__FUNCTION__, array());
  features_include();
  foreach ($pipe as $component => $data) {
    // Convert already defined items to dependencies.
//    _features_resolve_dependencies($data, $export, $module_name, $component);
    // Remove any excluded items.
    if (!empty($export['features_exclude'][$component])) {
      $data = array_diff($data, $export['features_exclude'][$component]);
      if ($component == 'dependencies' && !empty($export['dependencies'])) {
        $export['dependencies'] = array_diff($export['dependencies'], $export['features_exclude'][$component]);
      }
    }
    if (!empty($data) && $function = features_hook($component, 'features_export')) {
      // Pass module-specific data and export array.
      // We don't use features_invoke() here since we need to pass $export by
      // reference.
      $more = $function($data, $export, $module_name, $component);
      // Add the context information.
      $export['component'] = $component;
      $export['module_name'] = $module_name;
      // Allow other modules to manipulate the pipe to add in additional
      // modules.
      drupal_alter(array('features_pipe', 'features_pipe_' . $component), $more, $data, $export);
      // Remove the component information.
      unset($export['component']);
      unset($export['module_name']);
      // Allow for export functions to request additional exports, but avoid
      // circular references on already processed components.
      $processed[$component] = isset($processed[$component]) ? array_merge($processed[$component], $data) : $data;

      if (!empty($more)) {
        // Remove already processed components.
        foreach ($more as $component_name => $component_data) {
          if (isset($processed[$component_name])) {
            $more[$component_name] = array_diff($component_data, $processed[$component_name]);
          }
        }
        if ($more = array_filter($more)) {
          _features_populate($more, $export, $module_name);
        }
      }
    }
  }
  _features_export_language($language);
  return $export;
}

/**
 * Iterates over data and convert to dependencies if already defined elsewhere.
 *
 * @param string[] $data
 *   Names of component items/objects, e.g. "node-article-body".
 * @param array $export
 *   Export array.
 * @param string $module_name
 *   Module name.
 * @param string $component
 *   E.g. "field_instance".
 */
function _features_resolve_dependencies(&$data, &$export, $module_name, $component) {
  if ($map = features_get_default_map($component)) {
    foreach ($data as $key => $item) {
      // If this node type is provided by a different module, add it as a
      // dependency.
      if (isset($map[$item]) && $map[$item] != $module_name) {
        $export['dependencies'][$map[$item]] = $map[$item];
        unset($data[$key]);
      }
    }
  }
}

/**
 * Minimizes a list of dependencies.
 *
 * Iterates over a list of dependencies and kills modules that are
 * captured by other modules 'higher up'.
 *
 * @param string[] $dependencies
 *   List of module names required by the given module.
 *   Format: $[$dependency] = $dependency.
 * @param string $module_name
 *   Module name whose dependencies are being processed.
 *
 * @return string[]
 *   Format: $[$dependency] = $dependency
 */
function _features_export_minimize_dependencies($dependencies, $module_name = '') {
  // Ensure that the module doesn't depend upon itself.
  if (!empty($module_name) && !empty($dependencies[$module_name])) {
    unset($dependencies[$module_name]);
  }

  // Do some cleanup:
  // - Remove modules required by Drupal core.
  // - Protect against direct circular dependencies.
  // - Remove "intermediate" dependencies.
  $required = drupal_required_modules();
  foreach ($dependencies as $k => $v) {
    if (empty($v) || in_array($v, $required)) {
      unset($dependencies[$k]);
    }
    else {
      $module = features_get_modules($v);
      if ($module && !empty($module->info['dependencies'])) {
        // If this dependency depends on the module itself, we have a circular
        // dependency.
        // Don't let it happen. Only you can prevent forest fires.
        if (in_array($module_name, $module->info['dependencies'])) {
          unset($dependencies[$k]);
        }
        // Iterate through the dependency's dependencies and remove any
        // dependencies that are captured by it.
        else {
          foreach ($module->info['dependencies'] as $j => $dependency) {
            if (array_search($dependency, $dependencies) !== FALSE) {
              $position = array_search($dependency, $dependencies);
              unset($dependencies[$position]);
            }
          }
        }
      }
    }
  }
  return drupal_map_assoc(array_unique($dependencies));
}

/**
 * Completes a list of dependencies by adding all indirect dependencies as well.
 *
 * Mathematically this would be called a 'transitive closure'.
 *
 * This function is recursive, some of its parameters are only meant to be used
 * in recursive calls.
 *
 * @param string[] $dependencies
 *   Original list of dependencies.
 *   Format: $[*] = $module
 *   The array keys will be ignored, which means this has the same result for
 *   serial or associative arrays.
 * @param string $module_name
 *   (obsolete) Name of the module whose dependencies are being processed.
 *   This has no effect whatsoever, so it can be safely omitted.
 * @param string[] $maximized
 *   (recursive) List of modules that were already processed in previous
 *   recursion levels. Omit in non-recursive call.
 * @param bool $first
 *   (recursive) TRUE, if this is not a recursive call.
 *
 * @return string[]
 *   Complete list of direct and indirect dependencies.
 *   Format: $[] = $module
 *
 * @see _module_build_dependencies()
 */
function _features_export_maximize_dependencies($dependencies, $module_name = '', $maximized = array(), $first = TRUE) {
  foreach ($dependencies as $k => $v) {
    $parsed_dependency = drupal_parse_dependency($v);
    $name = $parsed_dependency['name'];
    if (!in_array($name, $maximized)) {
      $maximized[] = $name;
      $module = features_get_modules($name);
      if ($module && !empty($module->info['dependencies'])) {
        $maximized = array_merge($maximized, _features_export_maximize_dependencies($module->info['dependencies'], $module_name, $maximized, FALSE));
      }
    }
  }
  return array_unique($maximized);
}

/**
 * Prepares a feature export array into a finalized info array.
 *
 * @param array $export
 *   An exported feature definition.
 *   This has the same structure as a module *.info array, but it may be missing
 *   some keys, or have the keys in the wrong order.
 * @param string $module_name
 *   The name of the module to be exported.
 * @param bool $reset
 *   TRUE to reset the module cache. Only set to true when
 *   doing a final export for delivery.
 * @param bool $add_deprecated
 *   TRUE to also add deprecated components.
 *
 * @return array
 *   Complete *.info array with all required keys added and standardized order.
 */
function features_export_prepare($export, $module_name, $reset = FALSE, $add_deprecated = TRUE) {
  $existing = features_get_modules($module_name, $reset);

  // Copy certain exports directly into info.
  $copy_list = array('scripts', 'stylesheets');
  foreach ($copy_list as $item) {
    if (isset($export[$item])) {
      $existing->info[$item] = $export[$item];
    }
  }

  // Prepare info string -- if module exists, merge into its existing info file.
  $defaults = !empty($existing->info) ? $existing->info : array('core' => '7.x', 'package' => 'Features');
  $export = array_merge($defaults, $export);

  $deprecated = features_get_deprecated();
  // Cleanup info array.
  foreach ($export['features'] as $component => $data) {
    // If performing the final export, do not export deprecated components.
    if (($reset || !$add_deprecated) && !empty($deprecated[$component])) {
      unset($export['features'][$component]);
    }
    else {
      $export['features'][$component] = array_keys($data);
    }
  }
  if (isset($export['dependencies'])) {
    $export['dependencies'] = array_values($export['dependencies']);
  }
  if (isset($export['conflicts'])) {
    unset($export['conflicts']);
  }

  // Order info array.
  $standard_info = array();
  foreach (array_merge(array('name', 'description', 'core', 'package', 'version', 'project', 'dependencies'), $copy_list) as $item) {
    if (isset($export[$item])) {
      $standard_info[$item] = $export[$item];
    }
  }
  if (isset($export['php']) && ($export['php'] != DRUPAL_MINIMUM_PHP)) {
    $standard_info['php'] = $export['php'];
  }
  unset($export['php']);

  $export = features_array_diff_assoc_recursive($export, $standard_info);
  ksort($export);
  return array_merge($standard_info, $export);
}

/**
 * Generate an array of hooks and their raw code.
 *
 * @param array $export
 *   Export array.
 * @param string $module_name
 *   Feature module name.
 * @param bool $reset
 *   TRUE to reset the cache.
 *
 * @return string[][]
 *   Format: $[$component][$hook_name] = $function_body_php
 *   E.g. $['node']['node_info'] = '$items = array(..); [..] return $items;'
 */
function features_export_render_hooks($export, $module_name, $reset = FALSE) {
  features_include();
  $code = array();

  // Sort components to keep exported code consistent.
  ksort($export['features']);

  foreach ($export['features'] as $component => $data) {
    if (!empty($data)) {
      // Sort the items so that we don't generate different exports based on
      // order.
      asort($data);
      /* @see \hook_features_export_render() */
      if (features_hook($component, 'features_export_render')) {
        $hooks = features_invoke($component, 'features_export_render', $module_name, $data, $export);
        $code[$component] = $hooks;
      }
    }
  }
  return $code;
}

/**
 * Render feature export into an array representing its files.
 *
 * @param array $export
 *   An exported feature definition.
 * @param string $module_name
 *   The name of the module to be exported.
 * @param bool $reset
 *   Boolean flag for resetting the module cache. Only set to true when
 *   doing a final export for delivery.
 *
 * @return array
 *   Array of info file and module file contents.
 */
function features_export_render($export, $module_name, $reset = FALSE) {
  $code = array();

  // Generate hook code.
  $component_hooks = features_export_render_hooks($export, $module_name, $reset);
  $components = features_get_components();
  $deprecated = features_get_deprecated($components);

  // Group component code into their respective files.
  foreach ($component_hooks as $component => $hooks) {
    if ($reset && !empty($deprecated[$component])) {
      // Skip deprecated components on final export.
      continue;
    }
    $file = array('name' => 'features');
    if (isset($components[$component]['default_file'])) {
      switch ($components[$component]['default_file']) {
        case FEATURES_DEFAULTS_INCLUDED:
          $file['name'] = "features.$component";
          break;

        case FEATURES_DEFAULTS_CUSTOM:
          $file['name'] = $components[$component]['default_filename'];
          break;
      }
    }

    if (!isset($code[$file['name']])) {
      $code[$file['name']] = array();
    }

    foreach ($hooks as $hook_name => $hook_info) {
      // These are purely files that will be copied over.
      if (is_array($hook_info) && (!empty($hook_info['file_path']) || !empty($hook_info['file_content']))) {
        $code['_files'][$hook_name] = $hook_info;
        continue;
      }
      $hook_code = is_array($hook_info) ? $hook_info['code'] : $hook_info;
      $hook_args = is_array($hook_info) && !empty($hook_info['args']) ? $hook_info['args'] : '';
      $hook_file = is_array($hook_info) && !empty($hook_info['file']) ? $hook_info['file'] : $file['name'];
      $code[$hook_file][$hook_name] = features_export_render_defaults($module_name, $hook_name, $hook_code, $hook_args);
    }
  }

  // Finalize strings to be written to files.
  $code = array_filter($code);
  foreach ($code as $filename => $contents) {
    if ($filename != '_files') {
      $code[$filename] = "<?php\n\n/**\n * @file\n * {$module_name}.{$filename}.inc\n */\n\n" . implode("\n\n", $contents) . "\n";
    }
  }

  // Allow extra files to be added to the feature via ['_files'].
  if ($files = module_invoke_all('features_export_files', $module_name, $export)) {
    $code['_files'] = !empty($code['_files']) ? $code['_files'] + $files : $files;
  }
  if (!empty($code['_files'])) {
    drupal_alter('features_export_files', $code['_files'], $module_name, $export);
  }

  // Generate info file output.
  $export = features_export_prepare($export, $module_name, $reset);
  $code['info'] = features_export_info($export);

  // Used to create or manipulate the generated .module for features.inc.
  $modulefile_features_inc = "<?php\n\n/**\n * @file\n * Code for the {$export['name']} feature.\n */\n\ninclude_once '{$module_name}.features.inc';\n";
  $modulefile_blank = "<?php\n\n/**\n * @file\n * Drupal needs this blank file.\n */\n";

  // Prepare the module.
  // If module exists, let it be and include it in the files.
  if ($existing = features_get_modules($module_name, TRUE)) {
    $code['module'] = file_get_contents($existing->filename);

    // If the current module file does not reference the features.inc include,.
    // @TODO this way of checking does not account for the possibility of inclusion instruction being commented out.
    if (isset($code['features']) && strpos($code['module'], "{$module_name}.features.inc") === FALSE) {
      // If .module does not begin with <?php\n, just add a warning.
      if (strpos($code['module'], "<?php\n") !== 0) {
        features_log(t('@module does not appear to include the @include file.', array('@module' => "{$module_name}.module", '@include' => "{$module_name}.features.inc")), 'warning');
      }
      else {
        // Remove the old message if it exists, else just remove the <?php.
        $length = strpos($code['module'], $modulefile_blank) === 0 ? strlen($modulefile_blank) : 6;
        $code['module'] = $modulefile_features_inc . substr($code['module'], $length);
      }
    }

    if ($reset) {
      // Only check for deprecated files on final export
      // Deprecated files. Display a message for any of these files letting the
      // user know that they may be removed.
      $deprecated_files = array(
        "{$module_name}.defaults",
        "{$module_name}.features.views",
        "{$module_name}.features.node",
      );
      // Add deprecated components.
      foreach ($deprecated as $component) {
        $info = features_get_components($component);
        $filename = isset($info['default_file']) && $info['default_file'] == FEATURES_DEFAULTS_CUSTOM ? $info['default_filename'] : "features.{$component}";
        $deprecated_files[] = "{$module_name}.$filename";
      }
      foreach (file_scan_directory(drupal_get_path('module', $module_name), '/.*/') as $file) {
        if (in_array($file->name, $deprecated_files, TRUE)) {
          features_log(t('The file @filename has been deprecated and can be removed.', array('@filename' => $file->filename)), 'status');
        }
        elseif ($file->name === "{$module_name}.features" && empty($code['features'])) {
          // Attempt to remove the "include_once '*.features.inc';" statement in
          // the *.module file.
          if (strpos($code['module'], "{$module_name}.features.inc")) {
            $code['module'] = str_replace($modulefile_features_inc, $modulefile_blank, $code['module']);
          }
          // Check if the removal was successful.
          if (strpos($code['module'], "{$module_name}.features.inc")) {
            // The removal of the "include_once" was not successful, perhaps due
            // to custom modifications in the *.module file.
            // Leave a comment in *.features.inc to clean this up manually.
            $code['features'] = "<?php\n\n// This file is deprecated and can be removed.\n// Please remove include_once('{$module_name}.features.inc') in {$module_name}.module as well.\n";
          }
          else {
            // The "include_once" was removed successfully.
            // However, the *.features.inc file still needs to be removed
            // manually.
            $code['features'] = "<?php\n\n// This file is deprecated and can be removed.\n";
          }
        }
      }
    }
  }
  // Add a stub module to include the defaults.
  elseif (!empty($code['features'])) {
    $code['module'] = $modulefile_features_inc;
  }
  else {
    $code['module'] = $modulefile_blank;
  }
  return $code;
}

/**
 * Detect differences between DB and code components of a feature.
 *
 * @param \stdClass $module
 *   A module info object.
 *
 * @return string[][]
 *   Format: $[$component] = ['normal' => '..', 'default' => '..']
 */
function features_detect_overrides($module) {
  $cache = &drupal_static(__FUNCTION__, array());
  if (!isset($cache[$module->name])) {
    // Rebuild feature from .info file description and prepare an export from
    // current DB state.
    $export = features_populate($module->info, $module->name);
    $export = features_export_prepare($export, $module->name, FALSE, FALSE);

    $overridden = array();

    // Compare feature info.
    features_sanitize($module->info);
    features_sanitize($export);

    $compare = array('normal' => features_export_info($export), 'default' => features_export_info($module->info));
    if ($compare['normal'] !== $compare['default']) {
      $overridden['info'] = $compare;
    }

    // Collect differences at a per-component level.
    $states = features_get_component_states(array($module->name), FALSE);
    foreach ($states[$module->name] as $component => $state) {
      if ($state != FEATURES_DEFAULT) {
        $normal = features_get_normal($component, $module->name);
        $default = features_get_default($component, $module->name);
        features_sanitize($normal, $component);
        features_sanitize($default, $component);

        $compare = array('normal' => features_var_export($normal), 'default' => features_var_export($default));
        if (_features_linetrim($compare['normal']) !== _features_linetrim($compare['default'])) {
          $overridden[$component] = $compare;
        }
      }
    }
    $cache[$module->name] = $overridden;
  }
  return $cache[$module->name];
}

/**
 * Gets the available default hooks keyed by components.
 *
 * @param string|null $component
 *   A component name, e.g. 'field_instance', or NULL to list all components.
 * @param bool $reset
 *   (optional) If TRUE, the components cache will be cleared.
 *
 * @return string[]|string|null
 *   Return value depending on parameters:
 *   - If $component is NULL:
 *     The default hook for each component.
 *     Format: $[$component] = $default_hook
 *   - If $component is provided:
 *     The default hook (string), or NULL.
 */
function features_get_default_hooks($component = NULL, $reset = FALSE) {
  return features_get_components($component, 'default_hook', $reset);
}

/**
 * Gets the available default hooks keyed by components.
 *
 * @param string $component
 *   Component name.
 *
 * @return string|false
 *   The alter hook name, or FALSE if no altering should happen.
 */
function features_get_default_alter_hook($component) {
  $default_hook = features_get_components($component, 'default_hook');
  $alter_hook = features_get_components($component, 'alter_hook');
  $alter_type = features_get_components($component, 'alter_type');
  return empty($alter_type) || $alter_type != 'none' ? ($alter_hook ? $alter_hook : $default_hook) : FALSE;
}

/**
 * Return a code string representing an implementation of a defaults module hook.
 *
 * @param string $module
 *   The module name or component name.
 * @param string $hook
 *   The hook name.
 * @param string $code
 *   Function body.
 * @param string $args
 *   (optional) A parameter list.
 *
 * @return string
 *   The PHP code for a function declaration with doc comment.
 */
function features_export_render_defaults($module, $hook, $code, $args = '') {
  $output = array();
  $output[] = "/**";
  $output[] = " * Implements hook_{$hook}().";
  $output[] = " */";
  $output[] = "function {$module}_{$hook}(" . $args . ") {";
  $output[] = $code;
  $output[] = "}";
  return implode("\n", $output);
}

/**
 * Generate code friendly to the Drupal .info format from a structured array.
 *
 * @param mixed $info
 *   An array or single value to put in a module's .info file.
 * @param string[] $parents
 *   Array of parent keys (internal use only).
 *
 * @return string
 *   A code string ready to be written to a module's .info file.
 *
 * @todo It would be faster to pass a string for $parents.
 */
function features_export_info($info, $parents = array()) {
  $output = '';
  if (is_array($info)) {
    foreach ($info as $k => $v) {
      $child = $parents;
      $child[] = $k;
      $output .= features_export_info($v, $child);
    }
  }
  elseif (!empty($info) && count($parents)) {
    $line = array_shift($parents);
    foreach ($parents as $key) {
      $line .= is_numeric($key) ? "[]" : "[{$key}]";
    }
    $line .= " = {$info}\n";
    return $line;
  }
  return $output;
}

/**
 * Tar creation function. Written by dmitrig01.
 *
 * @param string $name
 *   Filename of the file to be tarred.
 * @param string $contents
 *   File contents to be written as tar.
 *
 * @return string
 *   The raw tar file contents.
 */
function features_tar_create($name, $contents) {
  /* http://www.mkssoftware.com/docs/man4/tar.4.asp */
  /* http://www.phpclasses.org/browse/file/21200.html */
  $tar = '';
  $bigheader = $header = '';
  if (strlen($name) > 100) {
    $bigheader = pack("a100a8a8a8a12a12a8a1a100a6a2a32a32a8a8a155a12",
        '././@LongLink', '0000000', '0000000', '0000000',
        sprintf("%011o", strlen($name)), '00000000000',
        '        ', 'L', '', 'ustar ', '0',
        '', '', '', '', '', '');

    $bigheader .= str_pad($name, floor((strlen($name) + 512 - 1) / 512) * 512, "\0");

    $checksum = 0;
    for ($i = 0; $i < 512; $i++) {
      $checksum += ord(substr($bigheader, $i, 1));
    }
    $bigheader = substr_replace($bigheader, sprintf("%06o", $checksum) . "\0 ", 148, 8);
  }
  $header = pack("a100a8a8a8a12a12a8a1a100a6a2a32a32a8a8a155a12", // Book the memorie area.
    substr($name,0,100),  //  0     100     File name
    '100644 ',            // File permissions
    '   765 ',            // UID,
    '   765 ',            // GID,
    sprintf("%11s ", decoct(strlen($contents))), // Filesize,
    sprintf("%11s", decoct(REQUEST_TIME)),       // Creation time
    '        ',        // 148     8         Check sum for header block
    '',                // 156     1         Link indicator / ustar Type flag
    '',                // 157     100     Name of linked file
    'ustar ',          // 257     6         USTAR indicator "ustar"
    ' ',               // 263     2         USTAR version "00"
    '',                // 265     32         Owner user name
    '',                // 297     32         Owner group name
    '',                // 329     8         Device major number
    '',                // 337     8         Device minor number
    '',                // 345     155     Filename prefix
    '');               // 500     12         ??

  $checksum = 0;
  for ($i = 0; $i < 512; $i++) {
    $checksum += ord(substr($header, $i, 1));
  }
  $header = substr_replace($header, sprintf("%06o", $checksum) . "\0 ", 148, 8);
  $tar = $bigheader . $header;

  $buffer = str_split($contents, 512);
  foreach ($buffer as $item) {
    $tar .= pack("a512", $item);
  }
  return $tar;
}

/**
 * Alternative to var_export(), with a more pleasant output format.
 *
 * The function is recursive, some parameters should only be provided on
 * recursive calls.
 *
 * The function is inspired/adapted from views_var_export().
 *
 * @param mixed $var
 *   The value to export.
 * @param string $prefix
 *   (recursive) Prefix for indentation.
 * @param bool $init
 *   (recursive) TRUE, if this is the first level of recursion.
 * @param int $count
 *   (recursive) The recursion depth. Starts with 0.
 *
 * @return string
 *   A php statement whose return value is $var.
 *
 * @see \views_var_export()
 * @see \var_export()
 */
function features_var_export($var, $prefix = '', $init = TRUE, $count = 0) {
  if ($count > 50) {
    watchdog('features', 'Recursion depth reached in features_var_export', array());
    return '';
  }

  if (is_object($var)) {
    $output = method_exists($var, 'export') ? $var->export() : features_var_export((array) $var, '', FALSE, $count + 1);
  }
  elseif (is_array($var)) {
    if (empty($var)) {
      $output = 'array()';
    }
    else {
      $output = "array(\n";
      foreach ($var as $key => $value) {
        // Using normal var_export() on the key, to ensure correct quoting.
        $output .= "  " . var_export($key, TRUE) . " => " . features_var_export($value, '  ', FALSE, $count + 1) . ",\n";
      }
      $output .= ')';
    }
  }
  elseif (is_bool($var)) {
    $output = $var ? 'TRUE' : 'FALSE';
  }
  elseif (is_int($var)) {
    $output = intval($var);
  }
  elseif (is_numeric($var)) {
    $floatval = floatval($var);
    if (is_string($var) && ((string) $floatval !== $var)) {
      // Do not convert a string to a number, if the string representation of
      // that number is not identical to the original value.
      $output = var_export($var, TRUE);
    }
    else {
      $output = $floatval;
    }
  }
  elseif (is_string($var) && strpos($var, "\n") !== FALSE) {
    // Replace line breaks in strings with a token for replacement at the very
    // end. This protects whitespace in strings from unintentional indentation.
    $var = str_replace("\n", "***BREAK***", $var);
    $output = var_export($var, TRUE);
  }
  else {
    $output = var_export($var, TRUE);
  }

  if ($prefix) {
    $output = str_replace("\n", "\n$prefix", $output);
  }

  if ($init) {
    $output = str_replace("***BREAK***", "\n", $output);
  }

  return $output;
}

/**
 * Creates PHP code with t() statements for given translatable strings.
 *
 * This PHP code can be added to generated code, to allow string extractors like
 * potx to extract a list of translatables from the exported module.
 *
 * This is typically called from hook_features_export_render() implementations.
 *
 * @param string[] $translatables
 *   List of translatable strings.
 *   Format: $[] = $string.
 * @param string $indent
 *   Indentation to prepend to each line of code.
 *
 * @return string
 *   PHP code with t() statements for each translatable string.
 */
function features_translatables_export($translatables, $indent = '') {
  $output = '';
  $translatables = array_filter(array_unique($translatables));
  if (!empty($translatables)) {
    $output .= "{$indent}// Translatables\n";
    $output .= "{$indent}// Included for use with string extractors like potx.\n";
    sort($translatables);
    foreach ($translatables as $string) {
      $output .= "{$indent}t(" . features_var_export($string) . ");\n";
    }
  }
  return $output;
}

/**
 * Gets a summary storage state for a feature.
 *
 * @param string $module_name
 *   Module name.
 *
 * @return int
 *   Value of one of the component state constants listed below.
 *   The number is the maximum of all individual component states for the given
 *   module, or FEATURES_DEFAULT if no component states found in the module.
 *
 * @see FEATURES_REBUILDABLE
 * @see FEATURES_DEFAULT
 * @see FEATURES_OVERRIDDEN
 * @see FEATURES_NEEDS_REVIEW
 * @see FEATURES_REBUILDING
 */
function features_get_storage($module_name) {
  // Get component states, and array_diff against array(FEATURES_DEFAULT).
  // If the returned array has any states that don't match FEATURES_DEFAULT,
  // return the highest state.
  $states = features_get_component_states(array($module_name), FALSE);
  $states = array_diff($states[$module_name], array(FEATURES_DEFAULT));
  $storage = !empty($states) ? max($states) : FEATURES_DEFAULT;
  return $storage;
}

/**
 * Gets an md5 signature for a the state of an object in code or database.
 *
 * Wrapper around features_get_[storage]() to return an md5 hash of a normalized
 * defaults/normal object array. Can be used to compare normal/default states
 * of a module's component.
 *
 * @param string $state
 *   One of 'cache', 'default' or 'normal'.
 * @param string $module_name
 *   A module name.
 * @param string $component
 *   A component name, e.g. 'field_instance'.
 * @param bool $reset
 *   If TRUE, the static cache for features_get_default() or
 *   features_get_normal() will be reset.
 *
 * @return string|false
 *   An md5 signature, or FALSE if not found.
 */
function features_get_signature($state = 'default', $module_name, $component, $reset = FALSE) {
  switch ($state) {
    case 'cache':
      // Load the last known stored signature from the database.
      switch (_features_get_signature_storage_type()) {
        case 'table':
          // The database is fully updated.
          // All signatures are stored in a dedicated database table.
          $qr = db_select('features_signature', 'fs')
            ->fields('fs', array('signature'))
            ->condition('module', $module_name)
            ->condition('component', $component)
            ->execute();
          return $qr ? $qr->fetchField() : FALSE;

        case 'cache':
          // The database is not fully updated, only to schema version 7201.
          // Signatures are stored in a cache table.
          $cache = cache_get('features_codecache', 'cache_featurestate');
          if (isset($cache->data[$module_name][$component])) {
            return $cache->data[$module_name][$component];
          }
          // No stored signature for this component.
          return FALSE;

        case 'variable':
        default:
          // The database is not fully updated, schema version before 7201.
          // Signatures are stored in a variable.
          $signaturess = variable_get('features_codecache', array());
          if (isset($signaturess[$module_name][$component])) {
            return $signaturess[$module_name][$component];
          }
          // No stored signature for this component.
          return FALSE;
      }

    case 'default':
      // Get the component data as currently in code.
      $objects = features_get_default($component, $module_name, TRUE, $reset);
      break;

    case 'normal':
      // Get the component data as currently in the database.
      $objects = features_get_normal($component, $module_name, $reset);
      break;
  }
  if (!empty($objects)) {
    // Build a signature hash from the component data.
    features_sanitize($objects, $component);
    return md5(_features_linetrim(features_var_export($objects)));
  }
  return FALSE;
}

/**
 * Updates a module/component signature in the database.
 *
 * The signature stored in the database reflects the last known state of the
 * component in code.
 *
 * @param string $module
 *   A feature module name.
 * @param string $component
 *   A component name, e.g. 'field_instance'.
 * @param string|null|false $signature
 *   An md5 signature, or NULL to generate one from the current state in code,
 *   or FALSE to delete the signature.
 * @param string|null $message
 *   (optional) Message to store along with the updated signature.
 */
function features_set_signature($module, $component, $signature = NULL, $message = NULL) {

  if ($signature === NULL) {
    // Build signature from current state in code.
    $signature = features_get_signature('default', $module, $component, TRUE);
  }

  // Support un-updated databases.
  switch (_features_get_signature_storage_type()) {
    case 'table':
      // The database is fully updated.
      // All signatures are stored in a dedicated database table.
      if ($signature === FALSE) {
        // Delete the signature.
        db_delete('features_signature')
          ->condition('module', $module)
          ->condition('component', $component)
          ->execute();
      }
      else {
        // Insert or update the signature.
        db_merge('features_signature')
          ->key(array(
            'module' => $module,
            'component' => $component,
          ))
          ->fields(array(
            'signature' => $signature,
            'updated' => time(),
            'message' => $message,
          ))
          ->execute();
      }
      break;

    case 'cache':
      // The database is not fully updated, only to schema version 7201.
      // Signatures are stored in a cache table.
      $cache = cache_get('features_codecache', 'cache_featurestate');
      if (!empty($cache->data)) {
        $signaturess = $cache->data;
      }
      $signaturess[$module][$component] = $signature;
      cache_set('features_codecache', $signaturess, 'cache_featurestate');
      break;

    case 'variable':
    default:
      // The database is not fully updated, schema version before 7201.
      // Signatures are stored in a variable.
      $signaturess = variable_get('features_codecache', array());
      $signaturess[$module][$component] = $signature;
      variable_set('features_codecache', $signaturess);
      break;
  }
}

/**
 * Gets the current storage type for features component signatures.
 *
 * This is needed to prevent breakage in a database that is not fully updated
 * yet, e.g. in deployment operations that run before the database update.
 *
 * The signatures used to be stored in a variable.
 * Since #1325288, it was stored in a cache table. This only applies to projects
 * that were using a -dev branch after 7.x-2.11.
 * Since #3162854, it is stored in a dedicated non-cache table.
 *
 * @return string
 *   One of 'table', 'cache' or 'type'.
 *   On a fully updated database, this value will be 'table'.
 *
 * @see \features_get_signature()
 * @see \features_set_signature()
 * @see \features_update_7202()
 */
function _features_get_signature_storage_type() {
  $type = &drupal_static(__FUNCTION__);
  if ($type !== NULL) {
    return $type;
  }
  if (db_table_exists('features_signature')) {
    $type = 'table';
  }
  elseif (db_table_exists('cache_featurestate')) {
    $type = 'cache';
  }
  else {
    $type = 'variable';
  }
  return $type;
}

/**
 * Gets, sets or deletes a semaphore for a given component.
 *
 * @param string $op
 *   One of 'get', 'set' or 'del'.
 * @param string $component
 *   A component name, e.g. 'field_instance'.
 *
 * @return int|false|void
 *   If $op is 'get', the semaphore, or FALSE if none found for the component.
 *   If $op is 'set' or 'del', nothing is returned.
 */
function features_semaphore($op, $component) {
  // Note: we don't use variable_get() here as the inited variable
  // static cache may be stale. Retrieving directly from the DB narrows
  // the possibility of collision.
  $semaphore = db_query("SELECT value FROM {variable} WHERE name = :name", array(':name' => 'features_semaphore'))->fetchField();
  $semaphore = !empty($semaphore) ? unserialize($semaphore) : array();

  switch ($op) {
    case 'get':
      return isset($semaphore[$component]) ? $semaphore[$component] : FALSE;

    case 'set':
      $semaphore[$component] = REQUEST_TIME;
      variable_set('features_semaphore', $semaphore);
      break;

    case 'del':
      if (isset($semaphore[$component])) {
        unset($semaphore[$component]);
        variable_set('features_semaphore', $semaphore);
      }
      break;
  }
}

/**
 * Get normal objects for a given module/component pair.
 *
 * @param string $component
 *   The component name, e.g. 'field_instance'.
 * @param string $module_name
 *   The name of the exported module.
 * @param bool $reset
 *   If TRUE, the static cache will be reset.
 *
 * @return mixed|false
 *   The normal objects that would be written into the feature on update.
 */
function features_get_normal($component, $module_name, $reset = FALSE) {
  if ($reset) {
    drupal_static_reset(__FUNCTION__);
  }
  $cache = &drupal_static(__FUNCTION__, array());
  if (!isset($cache[$module_name][$component])) {
    features_include();
    $code = NULL;
    $module = features_get_features($module_name);

    // Special handling for dependencies component.
    if ($component === 'dependencies') {
      $cache[$module_name][$component] = isset($module->info['dependencies']) ? array_filter($module->info['dependencies'], '_features_module_exists') : array();
    }
    // All other components.
    else {
      $default_hook = features_get_default_hooks($component);
      if ($module && $default_hook && isset($module->info['features'][$component]) && features_hook($component, 'features_export_render')) {
        $code = features_invoke($component, 'features_export_render', $module_name, $module->info['features'][$component], NULL);
        $cache[$module_name][$component] = isset($code[$default_hook]) ? eval($code[$default_hook]) : FALSE;
      }
    }

    // Clear out vars for memory's sake.
    unset($code);
    unset($module);
  }
  return isset($cache[$module_name][$component]) ? $cache[$module_name][$component] : FALSE;
}

/**
 * Helper function to determine if a module is enabled.
 *
 * @param string $module
 *   This module name comes from the .info file and can have version info in it.
 *
 * @return bool
 *   TRUE, if the module is enabled.
 */
function _features_module_exists($module) {
  $parsed_dependency = drupal_parse_dependency($module);
  $name = $parsed_dependency['name'];
  return module_exists($name);
}

/**
 * Get defaults for a given module/component pair.
 *
 * @param string $component
 *   A component name, e.g. 'field_instance'.
 * @param string|null $module_name
 *   (optional) If specified, only return defaults for this module.
 * @param bool $alter
 *   (optional) If TRUE, the defaults will be passed through an alter hook.
 * @param bool $reset
 *   If TRUE, the static cache will be reset.
 *
 * @return array[]|object[]|mixed[]|false
 *   Format: $[$name] = $item_export_data
 *   Object data as defined in code. For most components, this is the data
 *   returned from the component hook implementation(s).
 *   For some components these are actual objects, for others they are arrays or
 *   perhaps just strings.
 */
function features_get_default($component, $module_name = NULL, $alter = TRUE, $reset = FALSE) {
  $cache = &drupal_static(__FUNCTION__, array());
  // Ensure $alter is a true/false boolean.
  $alter = !empty($alter);
  features_include();
  features_include_defaults($component);
  $default_hook = features_get_default_hooks($component);
  $components = features_get_components();

  // Collect defaults for all modules if no module name was specified.
  if (isset($module_name)) {
    $modules = array($module_name);
  }
  else {
    if ($component === 'dependencies') {
      $modules = array_keys(features_get_features());
    }
    else {
      $modules = array();
      foreach (features_get_component_map($component) as $component_modules) {
        $modules = array_merge($modules, $component_modules);
      }
      $modules = array_unique($modules);
    }
  }

  // Collect and cache information for each specified module.
  foreach ($modules as $m) {
    if (!isset($cache[$component][$alter][$m]) || $reset) {
      // Special handling for dependencies component.
      if ($component === 'dependencies') {
        $module = features_get_features($m);
        $cache[$component][$alter][$m] = isset($module->info['dependencies']) ? $module->info['dependencies'] : array();
        unset($module);
      }
      // All other components.
      else {
        if ($default_hook && module_hook($m, $default_hook)) {
          $cache[$component][$alter][$m] = call_user_func("{$m}_{$default_hook}");
          if (is_array($cache[$component][$alter][$m])) {
            $alter_type = features_get_components('alter_type', $component);
            if ($alter && (!isset($alter_type) || $alter_type == FEATURES_ALTER_TYPE_NORMAL)) {
              if ($alter_hook = features_get_default_alter_hook($component)) {
                drupal_alter($alter_hook, $cache[$component][$alter][$m]);
              }
            }
          }
          else {
            $cache[$component][$alter][$m] = FALSE;
          }
        }
        else {
          $cache[$component][$alter][$m] = FALSE;
        }
      }
    }
  }

  // A specific module was specified. Retrieve only its components.
  if (isset($module_name)) {
    return isset($cache[$component][$alter][$module_name]) ? $cache[$component][$alter][$module_name] : FALSE;
  }
  // No module was specified. Retrieve all components.
  $all_defaults = array();
  if (isset($cache[$component][$alter])) {
    foreach (array_filter($cache[$component][$alter]) as $module_components) {
      $all_defaults = array_merge($all_defaults, $module_components);
    }
  }
  return $all_defaults;
}

/**
 * Gets a map of components to their providing modules.
 *
 * This function only cares what is returned from the (generated) default hooks,
 * not what is in the info file.
 *
 * @param string $component
 *   The component name, e.g. 'field_instance'.
 * @param string|null $attribute
 *   Object property name or array key to get the id from exported object data.
 *   These ids will be used as array keys in the return value.
 * @param callable|null $callback
 *   Callback to get the object id from exported object data. These ids will be
 *   used as array keys in the return value.
 * @param bool $reset
 *   If TRUE, the static cache entry for this specific component is reset.
 *
 * @return string[]|false
 *   Format: $[$name] = $module
 *   E.g. $['node-article-field_body'] = 'mysite_ct_article'
 *   The module where each item is exported.
 *   If the $GLOBALS['features_ignore_conflicts'] is TRUE(-ish), this function
 *   will return FALSE.
 */
function features_get_default_map($component, $attribute = NULL, $callback = NULL, $reset = FALSE) {
  $map = &drupal_static(__FUNCTION__, array());

  global $features_ignore_conflicts;
  if ($features_ignore_conflicts) {
    return FALSE;
  }

  features_include();
  features_include_defaults($component);
  if ((!isset($map[$component]) || $reset) && $default_hook = features_get_default_hooks($component)) {
    $map[$component] = array();
    foreach (module_implements($default_hook) as $module) {
      if ($defaults = features_get_default($component, $module)) {
        foreach ($defaults as $key => $object) {
          if (isset($callback)) {
            if ($object_key = $callback($object)) {
              $map[$component][$object_key] = $module;
            }
          }
          elseif (isset($attribute)) {
            if (is_object($object) && isset($object->{$attribute})) {
              $map[$component][$object->{$attribute}] = $module;
            }
            elseif (is_array($object) && isset($object[$attribute])) {
              $map[$component][$object[$attribute]] = $module;
            }
          }
          elseif (!isset($attribute) && !isset($callback)) {
            if (!is_numeric($key)) {
              $map[$component][$key] = $module;
            }
          }
          else {
            return FALSE;
          }
        }
      }
    }
  }
  return isset($map[$component]) ? $map[$component] : FALSE;
}

/**
 * Retrieve an array of features/components and their current states.
 *
 * @param string[] $features
 *   (optional) Machine names of feature modules.
 *   If empty, states for all features will be returned.
 * @param bool $rebuild_only
 *   (optional) If TRUE, only rebuildable components are returned.
 * @param bool $reset
 *   (optional) If TRUE, relevant caches will be reset before fetching.
 *
 * @return int[][]
 *   Component states by module and component.
 *   Format: $[$module][$component] = $state
 *   where $state will be one of the states listed below.
 *
 * @see FEATURES_REBUILDABLE
 * @see FEATURES_DEFAULT
 * @see FEATURES_OVERRIDDEN
 * @see FEATURES_NEEDS_REVIEW
 * @see FEATURES_REBUILDING
 */
function features_get_component_states($features = array(), $rebuild_only = TRUE, $reset = FALSE) {
  // Ensure that the export will be created in the English language.
  $language = _features_export_language();

  if ($reset) {
    drupal_static_reset(__FUNCTION__);
  }
  $cache = &drupal_static(__FUNCTION__, array());

  $all_features = features_get_features();
  $features = !empty($features) ? $features : array_keys($all_features);

  // Retrieve only rebuildable components if requested.
  features_include();
  $components = array_keys(features_get_components(NULL, NULL, $reset));
  if ($rebuild_only) {
    foreach ($components as $k => $component) {
      if (!features_hook($component, 'features_rebuild')) {
        unset($components[$k]);
      }
    }
  }

  foreach ($features as $feature) {
    $cache[$feature] = isset($cache[$feature]) ? $cache[$feature] : array();
    if (module_exists($feature) && !empty($all_features[$feature]->components)) {
      foreach (array_intersect($all_features[$feature]->components, $components) as $component) {
        if (!isset($cache[$feature][$component])) {
          $normal = features_get_signature('normal', $feature, $component, $reset);
          $default = features_get_signature('default', $feature, $component, $reset);
          $codecache = features_get_signature('cache', $feature, $component, $reset);
          $semaphore = features_semaphore('get', $component);

          // DB and code states match, there is nothing more to check.
          if ($normal == $default) {
            $cache[$feature][$component] = FEATURES_DEFAULT;

            // Stale semaphores can be deleted.
            features_semaphore('del', $component);

            // Update code cache if it is stale, clear out semaphore if it is
            // stale.
            if ($default != $codecache) {
              features_set_signature($feature, $component, $default, __FUNCTION__ . '(): $normal === $default');
            }
          }
          // Component properly implements exportables.
          elseif (!features_hook($component, 'features_rebuild')) {
            $cache[$feature][$component] = FEATURES_OVERRIDDEN;
          }
          // Component does not implement exportables.
          else {
            if (empty($semaphore)) {
              // Exception for dependencies. Dependencies are always
              // rebuildable.
              if ($component === 'dependencies') {
                $cache[$feature][$component] = FEATURES_REBUILDABLE;
              }
              // All other rebuildable components require comparison.
              else {
                // Code has not changed, but DB does not match. User has DB
                // overrides.
                if ($codecache == $default) {
                  $cache[$feature][$component] = FEATURES_OVERRIDDEN;
                }
                // DB has no modifications to prior code state (or this is
                // initial install).
                elseif (empty($codecache) || $codecache == $normal) {
                  $cache[$feature][$component] = FEATURES_REBUILDABLE;
                }
                // None of the states match. Requires user intervention.
                elseif ($codecache != $default) {
                  $cache[$feature][$component] = FEATURES_NEEDS_REVIEW;
                }
              }
            }
            else {
              // Semaphore is still within processing horizon. Do nothing.
              if ((REQUEST_TIME - $semaphore) < FEATURES_SEMAPHORE_TIMEOUT) {
                $cache[$feature][$component] = FEATURES_REBUILDING;
              }
              // A stale semaphore means a previous rebuild attempt did not
              // complete.
              // Attempt to complete the rebuild.
              else {
                $cache[$feature][$component] = FEATURES_REBUILDABLE;
              }
            }
          }
        }
      }
    }
  }

  // Filter cached components on the way out to ensure that even if we have
  // cached more data than has been requested, the return value only reflects
  // the requested features/components.
  $return = $cache;
  $return = array_intersect_key($return, array_flip($features));
  foreach ($return as $k => $v) {
    $return[$k] = array_intersect_key($return[$k], array_flip($components));
  }

  _features_export_language($language);

  return $return;
}

/**
 * Helper function to eliminate whitespace differences in code.
 *
 * @param string $code
 *   Original (multi-line) piece of code.
 *
 * @return string
 *   Modified piece of code with no leading or trailing whitespace per line.
 */
function _features_linetrim($code) {
  $code = explode("\n", $code);
  foreach ($code as $k => $line) {
    $code[$k] = trim($line);
  }
  return implode("\n", $code);
}

/**
 * Helper function to "sanitize" an array or object.
 *
 * Converts everything to an array, sorts the keys, removes recursion.
 *
 * @param array|object $array
 *   The object or array to "sanitize".
 *   After this call, this variable will be an array.
 * @param string $component
 *   (optional) Name of a component type, e.g. "field_instance".
 * @param bool $remove_empty
 *   (optional) If TRUE, remove null or empty values for assoc arrays.
 *
 * @todo Needs unit tests.
 * @todo A better name might be "normalize", not "sanitize".
 */
function features_sanitize(&$array, $component = NULL, $remove_empty = TRUE) {
  $array = features_remove_recursion($array);
  if (isset($component)) {
    $ignore_keys = _features_get_ignore_keys($component);
    // Remove keys to be ignored.
    if (count($ignore_keys)) {
      _features_remove_ignores($array, $ignore_keys);
    }
  }
  _features_sanitize($array, $remove_empty);
}

/**
 * Internal. "Sanitizes" a (nested) array (or object) recursively.
 *
 * The following operations are performed on every recursion level:
 * - Objects are converted to array via get_object_vars().
 * - "Associative" arrays are sorted by key.
 * - Non-associative/serial arrays are sorted by value.
 * - Empty values and empty sub-trees are removed if $remove_empty is TRUE.
 *
 * @param array|object|mixed $array
 *   Array or object to be sanitized.
 *   If $array is an object, it will be converted to array via get_object_vars().
 *   Any nested objects will be converted to array in the same way.
 *   If $array is not an array or object, it will be left as-is.
 * @param bool $remove_empty
 *   If TRUE, remove null or empty values for assoc arrays.
 *   This will also remove empty arrays or objects nested in the hierarchy.
 *
 * @internal
 * This function is designed for internal use within @see features_sanitize().
 *
 * @todo Needs unit tests.
 * @todo A better name might be "normalize", not "sanitize".
 */
function _features_sanitize(&$array, $remove_empty = TRUE) {
  if (is_object($array)) {
    $array = get_object_vars($array);
  }
  if (is_array($array)) {
    $is_assoc = _features_is_assoc($array);
    if ($is_assoc) {
      ksort($array, SORT_STRING);
      if ($remove_empty) {
        $array = array_filter($array);
      }
    }
    else {
      sort($array);
    }
    foreach ($array as $k => $v) {
      if (is_array($v) or is_object($v)) {
        _features_sanitize($array[$k]);
        if ($remove_empty && $is_assoc && empty($array[$k])) {
          unset($array[$k]);
        }
      }
    }
  }
}

/**
 * Internal. Checks whether the array is "associative".
 *
 * An array is considered "associative" by this function, if:
 * - it is empty (no values), or
 * - it has any non-integer keys, or
 * - some numeric indices are missing, e,g, [0 => $v, 1 => $v, 3 => $v].
 *
 * Note: The order of numeric indices is irrelevant.
 * E.g. [1 => $v, 0 => $v] is considered NOT associative.
 *
 * Borrowed from: http://www.php.net/manual/en/function.is-array.php#96724
 *
 * @param array $array
 *   The array to be tested.
 *
 * @return bool
 *   TRUE if the array is considered "associative".
 *
 * @internal
 * This function is designed for internal use within @see _features_sanitize().
 *
 * @todo Needs unit tests.
 */
function _features_is_assoc($array) {
  return (is_array($array) && (0 !== count(array_diff_key($array, array_keys(array_keys($array)))) || count($array) == 0));
}

/**
 * Returns a deep copy of the object or array without recursion.
 *
 * Properties or array values with recursive references are replaced with dummy
 * string values.
 *
 * The algorithm operates on a serialized version of the data. It was introduced
 * in features in #2543306, mainly for performance reasons. It is taken from
 * https://code.google.com/p/formaldehyde/source/browse/trunk/formaldehyde.php.
 * The algorithm is also used in the node_export module.
 *
 * @param object|array|mixed $o
 *   Original object or array, or an arbitrary value.
 *
 * @return object|array|mixed
 *   A copy of the object or array with recursion removed.
 *   If the original value was not an object or array, it is returned unaltered.
 *
 * @todo Needs unit tests.
 */
function features_remove_recursion($o) {
  if (is_array($o) || is_object($o)) {
    $re = '#(r|R):([0-9]+);#';
    $serialize = serialize($o);
    if (preg_match($re, $serialize)) {
      $last = $pos = 0;
      while (FALSE !== ($pos = strpos($serialize, 's:', $pos))) {
        $chunk = substr($serialize, $last, $pos - $last);
        if (preg_match($re, $chunk)) {
          $length = strlen($chunk);
          $chunk = preg_replace_callback($re, '_features_remove_recursion', $chunk);
          $serialize = substr($serialize, 0, $last) . $chunk . substr($serialize, $last + ($pos - $last));
          $pos += strlen($chunk) - $length;
        }
        $pos += 2;
        $last = strpos($serialize, ':', $pos);
        $length = substr($serialize, $pos, $last - $pos);
        $last += 4 + $length;
        $pos = $last;
      }
      $serialize = substr($serialize, 0, $last) . preg_replace_callback($re, '_features_remove_recursion', substr($serialize, $last));
      $o = unserialize($serialize);
    }
  }
  return $o;
}

/**
 * Callback function for preg_replace_callback() to remove recursion.
 *
 * The regular expression in calling code is '#(r|R):([0-9]+);#'. The source
 * string is a serialized array or object. The modified source string will be
 * sent back to unserialize(), so all replacements must be compatible with that.
 *
 * @param string[] $m
 *   Match parts from preg_replace_callback().
 *   $m[0] contains the complete matching fragment for "(r|R):([0-9]+);".
 *   $m[1] contains the sub-fragment for "(r|R)".
 *   $m[2] contains the sub-fragment for "([0-9]+)".
 *
 * @return string
 *   Replacement string fragment.
 *
 * @internal
 * Designed for internal use within @see features_remove_recursion().
 *
 * @see preg_replace_callback()
 */
function _features_remove_recursion($m) {
  $r = "\x00{$m[1]}ecursion_features";
  return 's:' . strlen($r . $m[2]) . ':"' . $r . $m[2] . '";';
}

/**
 * Helper to removes a set of keys an object/array.
 *
 * @param object|array $item
 *   An object or array passed by reference.
 * @param int[] $ignore_keys
 *   Array of keys to be ignored. Values are the level of the key.
 *   Format: $[$key] = $level.
 * @param int $level
 *   Level of key to remove. Up to 2 levels deep because $item can still be
 *   recursive.
 *
 * @internal
 * Designed for internal use within @see features_sanitize().
 */
function _features_remove_ignores(&$item, $ignore_keys, $level = -1) {
  $is_object = is_object($item);
  if (!is_array($item) && !is_object($item)) {
    return;
  }
  foreach ($item as $key => &$value) {
    if (isset($ignore_keys[$key]) && ($ignore_keys[$key] == $level)) {
      if ($is_object) {
        unset($item->$key);
      }
      else {
        unset($item[$key]);
      }
    }
    elseif (($level < 2) && (is_array($value) || is_object($value))) {
      _features_remove_ignores($value, $ignore_keys, $level + 1);
    }
  }
  unset($value);
}

/**
 * Returns an array of keys to be ignored for various exportables.
 *
 * @param string $component
 *   Name of a component type, e.g. "field_instance".
 *
 * @return int[]
 *   Map of keys to ignore at specific recursion levels.
 *   Format: $[$key] = $level
 *
 * @internal
 * Designed for use within @see features_sanitize().
 *
 * @see features_features_ignore()
 *
 * @todo Add hook_features_ignore() in features.api.php.
 */
function _features_get_ignore_keys($component) {
  static $cache;
  if (!isset($cache[$component])) {
    $cache[$component] = module_invoke_all('features_ignore', $component);
  }
  return $cache[$component];
}
