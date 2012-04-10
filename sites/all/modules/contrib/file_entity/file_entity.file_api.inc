<?php

/**
 * @file
 * API extensions of Drupal core's file.inc.
 */

/**
 * Returns information about file types from hook_file_type_info().
 *
 * @param $file_type
 *   (optional) A file type name. If ommitted, all file types will be returned.
 *
 * @return
 *   Either a file type description, as provided by hook_file_type_info(), or an
 *   array of all existing file types, keyed by file type name.
 */
function file_info_file_types($file_type = NULL) {
  $info = &drupal_static(__FUNCTION__);
  if (!isset($info)) {
    $info = module_invoke_all('file_type_info');

    // Add support for the standard file types until this can be fully
    // abstracted out of Media module.
    $info += array(
      'application' => array('label' => t('Application (multipurpose)')),
      'audio' => array('label' => t('Audio')),
      'image' => array('label' => t('Image')),
      'text' => array('label' => t('Text')),
      'video' => array('label' => t('Video')),
    );

    drupal_alter('file_type_info', $info);
    uasort($info, '_file_entity_sort_weight_label');
  }
  if ($file_type) {
    if (isset($info[$file_type])) {
      return $info[$file_type];
    }
  }
  else {
    return $info;
  }
}

/**
 * Determines the file type of a passed in file object.
 *
 * The file type is determined by extracting the 'first' part of the file's
 * MIME type. For example, a PNG image with a MIME type of 'image/png' will
 * have a file type of 'image'.
 *
 * @link http://www.iana.org/assignments/media-types/index.html IANA list of official MIME media types @endlink
 */
function file_get_type($file) {
  // Ensure that a MIME type has been determined first.
  if (empty($file->filemime)) {
    $file->filemime = file_get_mimetype($file->uri);
  }
  return substr($file->filemime, 0, strpos($file->filemime, '/'));
}

/**
 * Returns information about file formatters from hook_file_formatter_info().
 *
 * @param $formatter_type
 *   (optional) A file formatter type name. If ommitted, all file formatter
 *   will be returned.
 *
 * @return
 *   Either a file formatter description, as provided by
 *   hook_file_formatter_info(), or an array of all existing file formatters,
 *   keyed by formatter type name.
 */
function file_info_formatter_types($formatter_type = NULL) {
  $info = &drupal_static(__FUNCTION__);
  if (!isset($info)) {
    $info = module_invoke_all('file_formatter_info');
    drupal_alter('file_formatter_info', $info);
    uasort($info, '_file_entity_sort_weight_label');
  }
  if ($formatter_type) {
    if (isset($info[$formatter_type])) {
      return $info[$formatter_type];
    }
  }
  else {
    return $info;
  }
}

/**
 * Clears the file info cache.
 */
function file_info_cache_clear() {
  drupal_static_reset('file_info_file_types');
  drupal_static_reset('file_info_formatter_types');
}

/**
 * Construct a drupal_render() style array from an array of loaded files.
 *
 * @param $files
 *   An array of files as returned by file_load_multiple().
 * @param $view_mode
 *   View mode.
 * @param $weight
 *   An integer representing the weight of the first file in the list.
 * @param $langcode
 *   A string indicating the language field values are to be shown in. If no
 *   language is provided the current content language is used.
 *
 * @return
 *   An array in the format expected by drupal_render().
 */
function file_view_multiple($files, $view_mode = 'full', $weight = 0, $langcode = NULL) {
  if (empty($files)) {
    return array();
  }

  field_attach_prepare_view('file', $files, $view_mode, $langcode);
  entity_prepare_view('file', $files, $langcode);

  $build = array();
  foreach ($files as $file) {
    $build[$file->fid] = file_view($file, $view_mode, $langcode);
    $build[$file->fid]['#weight'] = $weight;
    $weight++;
  }
  $build['#sorted'] = TRUE;

  return $build;
}

/**
 * Generate an array for rendering the given file.
 *
 * @param $file
 *   A file object.
 * @param $view_mode
 *   View mode.
 * @param $langcode
 *   (optional) A language code to use for rendering. Defaults to the global
 *   content language of the current request.
 *
 * @return
 *   An array as expected by drupal_render().
 */
function file_view($file, $view_mode = 'full', $langcode = NULL) {
  if (!isset($langcode)) {
    $langcode = $GLOBALS['language_content']->language;
  }

  // Populate $file->content with a render() array.
  file_build_content($file, $view_mode, $langcode);

  $build = $file->content;
  // We don't need duplicate rendering info in $file->content.
  unset($file->content);

  $build += array(
    '#theme' => 'file_entity',
    '#file' => $file,
    '#view_mode' => $view_mode,
    '#language' => $langcode,
  );

  // Add contextual links for this file, except when the file is already being
  // displayed on its own page. Modules may alter this behavior (for example,
  // to restrict contextual links to certain view modes) by implementing
  // hook_file_view_alter().
  if (!empty($file->fid) && !($view_mode == 'full' && file_is_page($file))) {
    $build['#contextual_links']['file'] = array('file', array($file->fid));
  }

  // Allow modules to modify the structured file.
  $type = 'file';
  drupal_alter(array('file_view', 'entity_view'), $build, $type);

  return $build;
}

/**
 * Builds a structured array representing the file's content.
 *
 * @param $file
 *   A file object.
 * @param $view_mode
 *   View mode, e.g. 'default', 'full', etc.
 * @param $langcode
 *   (optional) A language code to use for rendering. Defaults to the global
 *   content language of the current request.
 */
function file_build_content($file, $view_mode = 'full', $langcode = NULL) {
  if (!isset($langcode)) {
    $langcode = $GLOBALS['language_content']->language;
  }

  // Remove previously built content, if exists.
  $file->content = array();

  // Build the actual file display.
  // @todo Figure out how to clean this crap up.
  $file->content['file'] = file_view_file($file, $view_mode, $langcode);
  if (isset($file->content['file'])) {
    if (isset($file->content['file']['#theme']) && $file->content['file']['#theme'] != 'file_link') {
      unset($file->content['file']['#file']);
    }
    unset($file->content['file']['#view_mode']);
    unset($file->content['file']['#language']);
  }
  else {
    unset($file->content['file']);
  }

  // Build fields content.
  // In case of a multiple view, file_view_multiple() already ran the
  // 'prepare_view' step. An internal flag prevents the operation from running
  // twice.
  field_attach_prepare_view('file', array($file->fid => $file), $view_mode, $langcode);
  entity_prepare_view('file', array($file->fid => $file), $langcode);
  $file->content += field_attach_view('file', $file, $view_mode, $langcode);

  $links = array();
  $file->content['links'] = array(
    '#theme' => 'links__file',
    '#pre_render' => array('drupal_pre_render_links'),
    '#attributes' => array('class' => array('links', 'inline')),
  );
  $file->content['links']['file'] = array(
    '#theme' => 'links__file__file',
    '#links' => $links,
    '#attributes' => array('class' => array('links', 'inline')),
  );

  // Allow modules to make their own additions to the file.
  module_invoke_all('file_view', $file, $view_mode, $langcode);
  module_invoke_all('entity_view', $file, 'file', $view_mode, $langcode);
}

/**
 * Generate an array for rendering just the file portion of a file entity.
 *
 * @param $file
 *   A file object.
 * @param $displays
 *   Can be either:
 *   - the name of a view mode;
 *   - or an array of custom display settings, as returned by file_displays().
 * @param $langcode
 *   (optional) A language code to use for rendering. Defaults to the global
 *   content language of the current request.
 *
 * @return
 *   An array as expected by drupal_render().
 */
function file_view_file($file, $displays = 'full', $langcode = NULL) {
  if (!isset($langcode)) {
    $langcode = $GLOBALS['language_content']->language;
  }

  // Prepare incoming display specifications.
  if (is_string($displays)) {
    $view_mode = $displays;
    $displays = file_displays($file->type, $view_mode);
  }
  else {
    $view_mode = '_custom_display';
  }
  drupal_alter('file_displays', $displays, $file, $view_mode);
  _file_sort_array_by_weight($displays);

  // Attempt to display the file with each of the possible displays. Stop after
  // the first successful one. See file_displays() for details.
  $element = NULL;
  foreach ($displays as $formatter_type => $display) {
    if (!empty($display['status'])) {
      $formatter_info = file_info_formatter_types($formatter_type);
      // Under normal circumstances, the UI prevents enabling formatters for
      // incompatible file types. In case this was somehow circumvented (for
      // example, a module updated its formatter definition without updating
      // existing display settings), perform an extra check here.
      if (isset($formatter_info['file types']) && !in_array($file->type, $formatter_info['file types'])) {
        continue;
      }
      if (isset($formatter_info['view callback']) && ($function = $formatter_info['view callback']) && function_exists($function)) {
        $display['type'] = $formatter_type;
        if (!empty($formatter_info['default settings'])) {
          if (empty($display['settings'])) {
            $display['settings'] = array();
          }
          $display['settings'] += $formatter_info['default settings'];
        }
        $element = $function($file, $display, $langcode);
        if (isset($element)) {
          break;
        }
      }
    }
  }

  // If none of the configured formatters were able to display the file, attempt
  // to display the file using the file type's default view callback.
  if (!isset($element)) {
    $file_type_info = file_info_file_types($file->type);
    if (isset($file_type_info['default view callback']) && ($function = $file_type_info['default view callback']) && function_exists($function)) {
      $element = $function($file, $view_mode, $langcode);
    }
  }

  // If a render element was returned by a formatter or the file type's default
  // view callback, add some defaults to it and return it.
  if (isset($element)) {
    $element += array(
      '#file' => $file,
      '#view_mode' => $view_mode,
      '#language' => $langcode,
    );
    return $element;
  }
}

/**
 * Returns an array of possible displays to use for a file type in a given view mode.
 *
 * It is common for a site to be configured with broadly defined file types
 * (e.g., 'video'), and to have different files of this type require different
 * displays (for example, the code required to display a YouTube video is
 * different than the code required to display a local QuickTime video).
 * Therefore, the site administrator can configure multiple displays for a given
 * file type. This function returns all of the displays that the administrator
 * enabled for the given file type in the given view mode. file_view_file() then
 * invokes each of these, and passes the specific file to display. Each display
 * implementation can inspect the file, and either return a render array (if it
 * is capable of displaying the file), or return nothing (if it is incapable of
 * displaying the file). The first render array returned is the one used.
 *
 * @param $file_type
 *   The type of file.
 * @param $view_mode
 *   The view mode.
 *
 * @return
 *   An array keyed by the formatter type name. Each item in the array contains
 *   the following key/value pairs:
 *   - status: Whether this display is enabled. If not TRUE, file_view_file()
 *     skips over it.
 *   - weight: An integer that determines the order of precedence within the
 *     returned array. The lowest weight display capable of displaying the file
 *     is used.
 *   - settings: An array of key/value pairs specific to the formatter type. See
 *     hook_file_formatter_info() for details.
 *
 * @see hook_file_formatter_info()
 * @see file_view_file()
 */
function file_displays($file_type, $view_mode) {
  $cache = &drupal_static(__FUNCTION__, array());

  // If the requested view mode isn't configured to use a custom display for its
  // fields, then don't use a custom display for its file either.
  if ($view_mode != 'default') {
    $view_mode_settings = field_view_mode_settings('file', $file_type);
    $view_mode = !empty($view_mode_settings[$view_mode]['custom_settings']) ? $view_mode : 'default';
  }

  if (!isset($cache[$file_type][$view_mode])) {
    // Load the display configurations for the file type and view mode. If none
    // exist for the view mode, use the default view mode.
    $displays = file_displays_load($file_type, $view_mode, TRUE);
    if (empty($displays) && $view_mode != 'default') {
      $cache[$file_type][$view_mode] = file_displays($file_type, 'default');
    }
    else {
      // Convert the display objects to arrays and remove unnecessary keys.
      foreach ($displays as $formatter_name => $display) {
        $displays[$formatter_name] = array_intersect_key((array) $display, drupal_map_assoc(array('status', 'weight', 'settings')));
      }
      $cache[$file_type][$view_mode] = $displays;
    }
  }

  return $cache[$file_type][$view_mode];
}

/**
 * Returns an array of {file_display} objects for the file type and view mode.
 */
function file_displays_load($file_type, $view_mode, $key_by_formatter_name = FALSE) {
  ctools_include('export');

  $display_names = array();
  $prefix = $file_type . '__' . $view_mode . '__';
  foreach (array_keys(file_info_formatter_types()) as $formatter_name) {
    $display_names[] = $prefix . $formatter_name;
  }
  $displays = ctools_export_load_object('file_display', 'names', $display_names);

  if ($key_by_formatter_name) {
    $prefix_length = strlen($prefix);
    $rekeyed_displays = array();
    foreach ($displays as $name => $display) {
      $rekeyed_displays[substr($name, $prefix_length)] = $display;
    }
    $displays = $rekeyed_displays;
  }

  return $displays;
}

/**
 * Saves a {file_display} object to the database.
 */
function file_display_save($display) {
  ctools_include('export');
  ctools_export_crud_save('file_display', $display);
}

/**
 * Creates a new {file_display} object.
 */
function file_display_new($file_type, $view_mode, $formatter_name) {
  ctools_include('export');
  $display = ctools_export_crud_new('file_display');
  $display->name = implode('__', array($file_type, $view_mode, $formatter_name));
  return $display;
}

/**
 * Helper function to sort an array by the value of each item's 'weight' key, while preserving relative order of items that have equal weight.
 */
function _file_sort_array_by_weight(&$a) {
  $i=0;
  foreach ($a as $key => $item) {
    if (!isset($a[$key]['weight'])) {
      $a[$key]['weight'] = 0;
    }
    $original_weight[$key] = $a[$key]['weight'];
    $a[$key]['weight'] += $i/1000;
    $i++;
  }
  uasort($a, 'drupal_sort_weight');
  foreach ($a as $key => $item) {
    $a[$key]['weight'] = $original_weight[$key];
  }
}

/**
 * User sort function to sort by weight, then label/name.
 */
function _file_entity_sort_weight_label($a, $b) {
  $a_weight = isset($a['weight']) ? $a['weight'] : 0;
  $b_weight = isset($b['weight']) ? $b['weight'] : 0;
  if ($a_weight == $b_weight) {
    $a_label = isset($a['label']) ? $a['label'] : '';
    $b_label = isset($b['label']) ? $b['label'] : '';
    return strcasecmp($a_label, $b_label);
  }
  else {
    return $a_weight < $b_weight ? -1 : 1;
  }
}

/**
 * Returns a file object which can be passed to file_save().
 *
 * @param $uri
 *   A string containing the URI, path, or filename.
 * @param $use_existing
 *   (Optional) If TRUE and there's an existing file in the {file_managed}
 *   table with the passed in URI, then that file object is returned.
 *   Otherwise, a new file object is returned. Default is TRUE.
 *
 * @return
 *   A file object, or FALSE on error.
 *
 * @todo This should probably be named file_load_by_uri($uri, $create_if_not_exists).
 * @todo Remove this function when http://drupal.org/node/685818 is fixed.
 */
function file_uri_to_object($uri, $use_existing = TRUE) {
  $file = FALSE;
  $uri = file_stream_wrapper_uri_normalize($uri);

  if ($use_existing) {
    // We should always attempt to re-use a file if possible.
    $files = entity_load('file', FALSE, array('uri' => $uri));
    $file = !empty($files) ? reset($files) : FALSE;
  }

  if (empty($file)) {
    $file = new stdClass();
    $file->uid = $GLOBALS['user']->uid;
    $file->filename = basename($uri);
    $file->uri = $uri;
    $file->filemime = file_get_mimetype($uri);
    // This is gagged because some uris will not support it.
    $file->filesize = @filesize($uri);
    $file->timestamp = REQUEST_TIME;
    $file->status = FILE_STATUS_PERMANENT;
  }

  return $file;
}

/**
 * Delete multiple files.
 *
 * Unlike core's file_delete(), this function does not care about file usage
 * or skip on invalid URIs. Just deletes the damn file like it should.
 *
 * @param array $fids
 *   An array of file IDs.
 */
function file_delete_multiple(array $fids) {
  $transaction = db_transaction();
  if (!empty($fids) && $files = file_load_multiple($fids)) {
    try {
      foreach ($files as $fid => $file) {
        module_invoke_all('file_delete', $file);
        module_invoke_all('entity_delete', $file, 'file');
        // Skip calling field_attach_delete as file_entity_file_delete() does this.
        //field_attach_delete('file', $file);

        // Remove this file from the search index if needed.
        // This code is implemented in file_entity module rather than in search module,
        // because node module is implementing search module's API, not the other
        // way around.
        if (module_exists('search')) {
          search_reindex($fid, 'file');
        }

        // Make sure the file is deleted before removing its row from the
        // database, so UIs can still find the file in the database.
        if (file_valid_uri($file->uri)) {
          file_unmanaged_delete($file->uri);
        }
      }

      db_delete('file_managed')
        ->condition('fid', $fids, 'IN')
        ->execute();
      db_delete('file_usage')
        ->condition('fid', $fids, 'IN')
        ->execute();
    }
    catch (Exception $e) {
      $transaction->rollback();
      watchdog_exception('file', $e);
      throw $e;
    }

    // Clear the page and block and file_load_multiple caches.
    entity_get_controller('file')->resetCache();
  }
}
