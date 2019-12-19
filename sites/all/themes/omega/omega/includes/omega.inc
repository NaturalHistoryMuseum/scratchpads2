<?php

/**
 * @file
 * Helper functions for the Omega base theme.
 */

/**
 * Retrieve a setting for the current theme or for a given theme.
 *
 * The final setting is obtained from the last value found in the following
 * sources:
 * - the default global settings specified in this function
 * - the default theme-specific settings defined in any base theme's .info file
 * - the default theme-specific settings defined in the theme's .info file
 * - the saved values from the global theme settings form
 * - the saved values from the theme's settings form
 * To only retrieve the default global theme setting, an empty string should be
 * given for $theme.
 *
 * @param string $setting_name
 *   The name of the setting to be retrieved.
 * @param mixed $default
 *   (Optional) A default value. Defaults to NULL.
 * @param string $theme
 *   (Optional) The name of a given theme. Defaults to the NULL which
 *   evaluates to the current theme.
 *
 * @return mixed|null
 *   The value of the requested setting, or the $default value if the setting
 *   does not exist.
 *
 * @see theme_get_setting()
 */
function omega_theme_get_setting($setting_name, $default = NULL, $theme = NULL) {
  $cache = &drupal_static('theme_get_setting', array());

  // If no key is given, use the current theme if we can determine it.
  if (!isset($theme)) {
    $theme = !empty($GLOBALS['theme_key']) ? $GLOBALS['theme_key'] : '';
  }

  if (empty($cache[$theme])) {
    // If the cache has not been filled yet, invoke theme_get_setting to
    // retrieve the value. This will populate the cache and make it available
    // for subsequent requests.
    if (($setting = theme_get_setting($setting_name, $theme)) !== NULL) {
      // Use the default value if the setting does not exist.
      return $setting;
    }
  }
  elseif (isset($cache[$theme][$setting_name])) {
    // Retrieve the value from the cache.
    return $cache[$theme][$setting_name];
  }

  // Use the default value if the setting does not exist.
  return $default;
}

/**
 * Builds the full theme trail (deepest base theme first) for a theme.
 *
 * @param string $theme
 *   (Optional) The key (machine-readable name) of a theme. Defaults to the key
 *   of the current theme.
 *
 * @return array
 *   An array of all themes in the trail, keyed by theme key.
 */
function omega_theme_trail($theme = NULL) {
  $theme = isset($theme) ? $theme : $GLOBALS['theme_key'];

  if (($cache = &drupal_static(__FUNCTION__)) && isset($cache[$theme])) {
    return $cache[$theme];
  }

  $cache[$theme] = array();

  if ($theme == $GLOBALS['theme'] && isset($GLOBALS['theme_info']->base_themes)) {
    $cache[$theme] = $GLOBALS['theme_info']->base_themes;
  }

  $themes = list_themes();
  if (empty($cache[$theme]) && isset($themes[$theme]->info['base theme'])) {
    $cache[$theme] = system_find_base_themes($themes, $theme);
  }

  // Add our current subtheme ($key) to that array.
  $cache[$theme][$theme] = $themes[$theme]->info['name'];

  return $cache[$theme];
}

/**
 * Helper function to remove unwanted css or js.
 *
 * @param array $data
 *   The css or js array.
 * @param string $type
 *   (Optional) Either 'css' or 'js' depending on the file array. Defaults to
 *   'css'.
 */
function omega_css_js_alter(&$data, $type = 'css') {
  if (!$excludes = omega_theme_get_setting("omega_{$type}_exclude")) {
    return;
  }

  require_once drupal_get_path('theme', 'omega') . '/includes/assets.inc';
  if (!$cache = cache_get("omega:{$GLOBALS['theme_key']}:excludes:$type")) {
    // Explode and trim the values for the exclusion rules.
    $steps = omega_assets_regex_steps($excludes);

    cache_set("omega:{$GLOBALS['theme_key']}:excludes:$type", $steps, 'cache', CACHE_TEMPORARY);
  }
  else {
    $steps = $cache->data;
  }

  $mapping = omega_assets_generate_mapping($data);
  $data = array_diff_key($data, omega_assets_regex_execute($mapping, $steps));
}

/**
 * Retrieves the array of enabled extensions for a theme.
 *
 * Extensions can be registered through the .info file. Each extension can
 * define a theme setting form altering function named
 * 'THEMENAME_extension_EXTENSION_theme_settings_form_alter()' through a file
 * named 'THEME_ROOT/includes/EXTENSION/EXTENSION.settings.inc' to have it
 * automatically included whenever the theme settings form is displayed.
 *
 * Each extension can also define a
 * 'THEMENAME_extension_EXTENSION_theme_registry_alter()' function through a
 * file named 'THEME_ROOT/includes/EXTENSION/EXTENSION.inc' to register custom
 * hooks with the theme registry.
 *
 * @param string $theme
 *   (Optional) The key (machine-readable name) of a theme. Defaults to the key
 *   of the current theme.
 * @param bool $reset
 *   (Optional) Whether the internal cache should be rebuilt. Defaults to FALSE.
 *
 * @return array
 *   The theme info array of the passed or current theme.
 *
 * @see _system_default_theme_features()
 * @see omega_extension_development_theme_settings_form_alter()
 * @see omega_extension_development_theme_registry_alter()
 */
function omega_extensions($theme = NULL, $reset = FALSE) {
  $theme = isset($theme) ? $theme : $GLOBALS['theme_key'];

  if (!$reset) {
    if (($extensions = &drupal_static(__FUNCTION__)) && isset($extensions[$theme])) {
      return $extensions[$theme];
    }

    if (($cache = cache_get('omega:' . $theme . ':extensions')) !== FALSE) {
      return $extensions[$theme] = $cache->data;
    }
  }

  // Extensions can't be hidden.
  $extensions[$theme] = omega_discovery('extension', $theme);

  foreach ($extensions[$theme] as $extension => &$info) {
    // Make sure that the theme variable is never altered.
    $context = $theme;
    drupal_alter('omega_extension_info', $info, $context);

    // Determine if the extension is enabled.
    $info['enabled'] = omega_theme_get_setting('omega_toggle_extension_' . $extension, !empty($info['info']['enabled']));

    // Check if all dependencies are met.
    $info['errors'] = FALSE;
    if (!empty($info['info']['dependencies'])) {
      foreach ($info['info']['dependencies'] as $dependency) {
        $dependency = drupal_parse_dependency($dependency);

        if ((!$module = system_get_info('module', $dependency['name'])) || omega_check_incompatibility($dependency, $module['version'])) {
          $info['errors'] = TRUE;
        }
      }
    }
  }

  // Write to the cache.
  cache_set('omega:' . $theme . ':extensions', $extensions[$theme]);

  return $extensions[$theme];
}

/**
 * Determines if an extension is enabled.
 *
 * @param string $extension
 *   The machine-readable name of an extension.
 * @param string $theme
 *   (Optional) The key (machine-readable name) of a theme. Defaults to the key
 *   of the current theme.
 *
 * @return bool
 *   TRUE if the extension is enabled, FALSE otherwise.
 */
function omega_extension_enabled($extension, $theme = NULL) {
  $theme = isset($theme) ? $theme : $GLOBALS['theme_key'];
  if (($extensions = omega_extensions($theme)) && isset($extensions[$extension])) {
    // Disabled if there were errors or disabled by overriding.
    if (!empty($extensions[$extension]['errors']) || !variable_get('omega_toggle_extension_' . $extension, TRUE)) {
      return FALSE;
    }
    // Enabled by theme setting or enabled by overriding.
    return !empty($extensions[$extension]['enabled']) || variable_get('omega_toggle_extension_' . $extension, FALSE);
  }
}

/**
 * Retrieves the full info array of a theme.
 *
 * @param string $theme
 *   (Optional) The key (machine-readable name) of a theme. Defaults to the key
 *   of the current theme.
 *
 * @return array
 *   The theme info array of the passed or current theme.
 */
function omega_theme_info($theme = NULL) {
  $theme = isset($theme) ? $theme : $GLOBALS['theme_key'];

  // If this is the current theme, just load the theme info from the globals.
  // Note: The global 'theme_key' property is not reliable in this case because
  // it gets overridden on theme settings pages.
  if ($theme == $GLOBALS['theme']) {
    return $GLOBALS['theme_info']->info;
  }

  $themes = list_themes();
  return $themes[$theme]->info;
}

/**
 * Invoke a hook in all themes in the theme trail that implement it.
 *
 * @param string $hook
 *   The name of the hook to invoke.
 * @param string $theme
 *   (Optional) The key (machine-readable name) of a theme. Defaults to the key
 *   of the current theme.
 * @param ...
 *   Arguments to pass to the hook.
 *
 * @return array
 *   An array of return values of the hook implementations. If themes return
 *   arrays from their implementations, those are merged into one array.
 *
 * @see module_invoke_all()
 */
function omega_invoke_all($hook, $theme = NULL) {
  $theme = isset($theme) ? $theme : $GLOBALS['theme_key'];

  $args = func_get_args();
  // Remove $hook from the arguments.
  unset($args[0], $args[1]);

  $return = array();
  foreach (omega_theme_trail($theme) as $key => $name) {
    $function = $key . '_' . $hook;

    if (function_exists($function)) {
      $result = call_user_func_array($function, array_merge(array($theme), array_values($args)));
      if (isset($result) && is_array($result)) {
        // Append the 'theme' property to each array element.
        foreach ($result as &$item) {
          $item['theme'] = $key;
        }
        $return = array_merge_recursive($return, $result);
      }
      elseif (isset($result)) {
        $return[] = $result;
      }
    }
  }
  return $return;
}

/**
 * Custom implementation of drupal_array_get_nested_value().
 *
 * This function also supports objects instead of just arrays.
 *
 * @param array|object $object
 *   The array or object from which to get the value.
 * @param array $parents
 *   An array of parent keys of the value, starting with the outermost key.
 * @param bool $key_exists
 *   (Optional) If given, an already defined variable that is altered by
 *   reference.
 *
 * @return mixed
 *   The requested nested value. Possibly NULL if the value is NULL or not all
 *   nested parent keys exist. $key_exists is altered by reference and is a
 *   Boolean that indicates whether all nested parent keys exist (TRUE) or not
 *   (FALSE). This allows to distinguish between the two possibilities when NULL
 *   is returned.
 *
 * @see drupal_array_get_nested_value()
 */
function omega_get_nested_value(&$object, array $parents, &$key_exists = NULL) {
  $ref = &$object;
  foreach ($parents as $parent) {
    if (is_array($ref) && array_key_exists($parent, $ref)) {
      $ref = &$ref[$parent];
    }
    elseif (is_object($ref) && property_exists($ref, $parent)) {
      $ref = &$ref->$parent;
    }
    else {
      $key_exists = FALSE;
      return NULL;
    }
  }
  $key_exists = TRUE;
  return $ref;
}

/**
 * Retrieves the info array for all available layouts.
 *
 * @return array
 *   An array of available layouts for the given theme.
 */
function omega_layouts_info() {
  if (($layouts = &drupal_static(__FUNCTION__)) !== NULL) {
    return $layouts;
  }

  // Try to retrieve the layouts definitions from cache.
  if (($cache = cache_get('omega:layouts')) !== FALSE) {
    return $layouts = $cache->data;
  }

  // Layouts do not have a specific theme scope.
  $layouts = omega_discovery('layout', FALSE);
  foreach ($layouts as $layout => &$info) {
    $info['attached'] = array();
    $info['template'] = isset($info['info']['template']) ? $info['info']['template'] : $layout;
    $root = drupal_get_path('theme', $info['theme']);

    if (isset($info['info']['stylesheets'])) {
      foreach ($info['info']['stylesheets'] as $media => $files) {
        foreach ($files as $key => $file) {
          if (is_file($info['path'] . '/' . $file)) {
            // First, check if the file exists in the layout's path.
            $path = $info['path'] . '/' . $file;
          }
          elseif (is_file($root . '/' . $file)) {
            // Otherwise, check if the file exists in the theme's path.
            $path = $root . '/' . $file;
          }
          else {
            // The specified file does not exist.
            continue;
          }

          $info['attached']['css']["$media:$key"] = array(
            'data' => $path,
            'media' => $media,
            'group' => CSS_THEME,
            'weight' => -10,
          );
        }
      }
    }

    // Look up possible CSS and JS file overrides.
    if (isset($info['info']['scripts'])) {
      foreach ($info['info']['scripts'] as $key => $file) {
        if (is_file($info['path'] . '/' . $file)) {
          // First, check if the file exists in the layout's path.
          $path = $info['path'] . '/' . $file;
        }
        elseif (is_file($root . '/' . $file)) {
          // Otherwise, check if the file exists in the theme's path.
          $path = $root . '/' . $file;
        }
        else {
          // The specified file does not exist.
          continue;
        }

        $info['attached']['js'][$key] = array(
          'data' => $path,
          'group' => JS_THEME,
          'weight' => -10,
        );
      }
    }
  }

  // Give modules and themes a chance to alter the layout info array.
  drupal_alter('omega_layouts_info', $layouts);

  // Cache the layout definitions in the database.
  cache_set('omega:layouts', $layouts);

  return $layouts;
}

/**
 * Retrieves the active layout for the current page.
 *
 * @return array|bool
 *   The info array for the active layout or FALSE if the current page does not
 *   use an alternative page layout.
 */
function omega_layout() {
  if (($cache = &drupal_static(__FUNCTION__)) !== NULL) {
    return $cache;
  }

  // Load the default layout from the theme settings.
  $layout = omega_extension_enabled('layouts') ? omega_theme_get_setting('omega_layout', 'simple') : NULL;
  drupal_alter('omega_layout', $layout);

  if (empty($layout)) {
    $cache = NULL;
  }
  else {
    $layouts = omega_layouts_info();
    $cache = isset($layouts[$layout]) ? $layouts[$layout] : NULL;
  }

  return $cache;
}

/**
 * Loads the theme-bound assets of a layout, if any.
 *
 * @param string $layout
 *   The layout for which to load the assets.
 */
function omega_layout_load_theme_assets($layout) {
  $stylesheets = array();
  $scripts = array();

  foreach (omega_theme_trail() as $key => $name) {
    $path = drupal_get_path('theme', $key);
    $info = omega_theme_info($key);
    if (!isset($info['layout'][$layout])) {
      continue;
    }

    // Build arrays of stylesheets and scripts. Files with the same relative
    // path and name will be overridden.
    $assets = $info['layout'][$layout];
    if (!empty($assets['stylesheets'])) {
      foreach ($assets['stylesheets'] as $media => $files) {
        foreach ($files as $file) {
          $stylesheets[$media][$file] = "$path/$file";
        }
      }
    }

    if (!empty($assets['scripts'])) {
      foreach ($assets['scripts'] as $file) {
        $scripts[$file] = "$path/$file";
      }
    }
  }

  foreach ($stylesheets as $media => $files) {
    foreach ($files as $file) {
      drupal_add_css($file, array('group' => CSS_THEME, 'media' => $media));
    }
  }

  foreach ($scripts as $file) {
    drupal_add_js($file, array('group' => JS_THEME));
  }
}

/**
 * Allow themes to easily define libraries.
 *
 * @param string $theme
 *   (Optional) The key (machine-readable name) of a theme. Defaults to the key
 *   of the current theme.
 *
 * @return array
 *   An array of libraries defined by themes in the theme trail of the given
 *   theme.
 */
function omega_theme_libraries_info($theme = NULL) {
  $theme = isset($theme) ? $theme : $GLOBALS['theme_key'];

  // Check if the libraries have already been statically cached.
  if (($libraries = &drupal_static(__FUNCTION__)) && isset($libraries[$theme])) {
    return $libraries[$theme];
  };

  // Try to retrieve the library definitions from cache.
  if (($cache = cache_get("omega:$theme:libraries")) !== FALSE) {
    return $libraries[$theme] = $cache->data;
  }

  // Retrieve the libraries by invoking the hook.
  $libraries[$theme] = omega_invoke_all('omega_theme_libraries_info');
  foreach ($libraries[$theme] as $key => &$library) {
    $library['path'] = omega_library_path($key);
  }

  $context = $theme;
  // Give modules and themes a chance to alter the libraries info array.
  drupal_alter('omega_theme_libraries_info', $libraries[$theme], $context);

  // Cache the layout definitions in the database.
  cache_set("omega:$theme:libraries", $libraries[$theme]);

  return $libraries[$theme];
}

/**
 * Discovers layouts, extensions or other plugins in the theme trail.
 *
 * @param string $type
 *   A theme extension type (e.g. layout or extension).
 * @param string $theme
 *   (Optional) The key (machine-readable name) of a theme. Defaults to the key
 *   of the current theme.
 *
 * @return array
 *   An array containing the discovered definitions.
 */
function omega_discovery($type, $theme = NULL) {
  $theme = isset($theme) ? $theme : $GLOBALS['theme_key'];

  if (($discovery = &drupal_static(__FUNCTION__, array())) && isset($discovery[$theme][$type])) {
    return $discovery[$theme][$type];
  }

  $discovery[$theme][$type] = array();

  // Retrieve all themes from the theme trail of the given theme.
  $themes = $theme === FALSE ? list_themes() : omega_theme_trail($theme);

  // Collect paths to all sub-themes grouped by base themes. These will be
  // used for filtering. This allows base themes to have sub-themes in its
  // folder hierarchy without affecting the base themes template discovery.
  $paths = array();
  foreach (list_themes() as $key => $info) {
    if (!empty($info->base_theme)) {
      $paths[$info->base_theme][$key] = dirname($info->filename);
    }
  }
  foreach ($paths as $basetheme => $subthemes) {
    foreach ($subthemes as $subtheme => $path) {
      if (isset($paths[$subtheme])) {
        $paths[$basetheme] = array_merge($paths[$basetheme], $paths[$subtheme]);
      }
    }
  }

  $strlen = strlen($type) + 1;
  foreach ($themes as $key => $label) {
    // Retrieve the array of paths that should be ignored for this theme.
    $ignore = isset($paths[$key]) ? $paths[$key] : array();
    $path = drupal_get_path('theme', $key);

    // Support files without '.inc' extension for backwards compatibility.
    foreach (file_scan_directory($path, '/\.' . $type . '(\.inc)?$/', array('key' => 'name')) as $name => $file) {
      // Ignore sub-theme implementations for the current theme.
      if (strpos($file->uri, str_replace($ignore, '', $file->uri)) !== 0) {
        continue;
      }

      if (substr($name, -$strlen) === '.' . $type) {
        $name = substr($name, 0, strlen($name) - $strlen);
      }

      if ($info = drupal_parse_info_file($file->uri)) {
        $discovery[$theme][$type][$name] = array(
          'name' => $name,
          'path' => dirname($file->uri),
          'file' => $file->uri,
          'info' => $info,
          'theme' => $key,
        );
      }
    }
  }

  return $discovery[$theme][$type];
}

/**
 * Checks whether a version is compatible with a given dependency.
 *
 * This is a wrapper for drupal_check_incompatibility() which strips the core
 * version and any potential development version suffix from the given string.
 *
 * @param array $dependency
 *   The parsed dependency structure from drupal_parse_dependency().
 * @param string $current
 *   The version to check against (like 4.2).
 *
 * @return null|string
 *   NULL if compatible, otherwise the original dependency version string that
 *   caused the incompatibility.
 *
 * @see drupal_check_incompatibility()
 * @see drupal_parse_dependency()
 */
function omega_check_incompatibility($dependency, $current) {
  // Remove the core version from the version string.
  $current = preg_replace('/^' . DRUPAL_CORE_COMPATIBILITY . '-/', '', $current);
  // Remove any potential development version suffixes from the string.
  $current = preg_replace('/-dev$/', '', $current);

  return drupal_check_incompatibility($dependency, $current);
}

/**
 * Finds the first occurrence of a given file in the theme trail.
 *
 * @param string $file
 *   The relative path to a file.
 * @param string $theme
 *   (Optional) The key (machine-readable name) of a theme. Defaults to the key
 *   of the current theme.
 *
 * @return string
 *   The path to the file. If the file does not exist at all, it will simply
 *   return the path of the file as it would be if it existed in the given theme
 *   directly. This ensures that the code that uses this function does not break
 *   if a file does not exist anywhere.
 */
function omega_theme_trail_file($file, $theme = NULL) {
  $theme = isset($theme) ? $theme : $GLOBALS['theme_key'];

  // Iterate over all themes in the theme trail (starting with the active theme)
  // and return the first match.
  $current = NULL;
  foreach (array_reverse(omega_theme_trail($theme)) as $name => $info) {
    $current = drupal_get_path('theme', $name) . '/' . $file;
    if (file_exists($current)) {
      return $current;
    }
  }

  // The default (fallback) path is the path of the active theme, even if it
  // does not actually have that file.
  return drupal_get_path('theme', $theme) . '/' . $file;
}

/**
 * Gets the path of a library.
 *
 * @param string $name
 *   The machine name of a library to return the path for.
 * @param string $theme
 *   (Optional) The key (machine-readable name) of a theme. Defaults to the key
 *   of the current theme.
 *
 * @return string|null
 *   The path to the specified library or NULL if the library wasn't found.
 */
function omega_library_path($name, $theme = NULL) {
  if (($path = omega_theme_trail_file("libraries/$name", $theme)) && is_dir($path)) {
    return $path;
  }

  // Use the libraries module's API if available.
  if (module_exists('libraries')) {
    return libraries_get_path($name);
  }

  $profile = drupal_get_path('profile', drupal_get_profile());
  $config = conf_path();

  // Similar to 'modules' and 'themes' directories in the root directory,
  // certain distributions may want to place libraries into a 'libraries'
  // directory in Drupal's root directory.
  $directories[] = 'libraries';

  // Similar to 'modules' and 'themes' directories inside an installation
  // profile, installation profiles may want to place libraries into a
  // 'libraries' directory.
  $directories[] = "$profile/libraries";

  // Always search sites/all/libraries.
  $directories[] = 'sites/all/libraries';

  // Also search sites/<domain>/*.
  $directories[] = "$config/libraries";

  // Return the first occurrence.
  foreach (array_reverse($directories) as $dir) {
    if (is_dir("$dir/$name")) {
      return "$dir/$name";
    }
  }
}
