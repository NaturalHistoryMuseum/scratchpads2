<?php

/**
 *  @file
 *  Helper functions related to media types.  CRUD for saving their settings mainly.
 *
 *  Also contains the media entity class definition.
 *  @see media.install for a list of the base types.
 */

/**
 * Implements hook_file_type_info().
 */
function media_file_type_info() {
  $types = array();
  foreach (media_type_get_types() as $type => $type_object) {
    $types[$type] = array(
      'label' => $type_object->label,
      'weight' => $type_object->weight,
    );
  }
  return $types;
}

/**
 * Implements hook_file_type_info_alter().
 */
function media_file_type_info_alter(array &$info) {
  // Add default view callback support for all file types.
  foreach ($info as $type => $type_info) {
    $info[$type] += array(
      'default view callback' => 'media_file_type_media_default_view',
    );
  }
}

/**
 * Implements hook_file_type_TYPE_default_view().
 *
 * Returns a drupal_render() array for displaying the file when there are no
 * administrator-configured formatters, or none of the configured ones return a
 * display.
 */
function media_file_type_media_default_view($file, $view_mode, $langcode) {
  // During preview, or when custom attribute values are needed on the displayed
  // element, use a media icon.
  if ($view_mode == 'media_preview' || isset($file->override)) {
    return array(
      '#theme' => 'media_formatter_large_icon',
      '#file' => $file,
    );
  }

  // Finally, fall back to File module's generic file display.
  return array(
    '#theme' => 'file_link',
    '#file' => $file,
  );
}

/**
 *  Update an existing media type or create a new one.
 *
 *  The default media types are currently 'Audio', 'Image', 'Video', and
 *  'Other', which are defined in media_install().
 *
 *  @param object &$type
 *    $type is an object with the following fields:
 *      ->name => The name of the media asset type, such as 'video';
 *      ->label => The human readable name;
 *      ->base => boolean: If the media type cannot be removed.
 *      ->type_callback => Function call to filter an instance to its bundle.
 *      ->type_callback_args => An array of args to be passed to type_callback.
 *  @return void;
 */
function media_type_save(&$type) {
  if (empty($type->name)) {
    throw new Exception('Enable to add type, name not provided');
  }

  $type = media_type_set_defaults($type);
  if (!is_array($type->type_callback_args)) {
    throw new Exception('type_callback_args should be an array');
  }

  $type->type_callback_args = serialize($type->type_callback_args);

  $ret = db_merge('media_type')
    ->key(array('name' => $type->name))
    ->fields((array)$type)
  ->execute();

  // Clear the caches
  drupal_static_reset('media_type_get_types');
  drupal_static_reset('media_type_get_mime_map');
  return;
}

/**
 * @todo Remove this function after ensuring that nothing (including update
 *   functions) call it. It is deprecated with the change from media entity
 *   containing a file field to just a file entity.
 */
function media_type_configure_formatters($name, $view_modes_to_formatters) {
}

/**
 * Loads a media type based on its machine name.
 *
 * @param string $name
 * @return StdClass
 */
function media_type_load($name) {
  $types = media_type_get_types();
  if (isset($types[$name])) {
    return $types[$name];
  }
}

/**
 *  Loads all media types into an array keyed by machine name and sorted
 *  and weighted lexographically.
 *
 * @return array
 *  Media types keyed by machine name.
 */
function media_type_get_types() {
  $types =& drupal_static(__FUNCTION__);
  if (!$types) {
    $types = db_select('media_type', 'mt')
      ->orderBy('weight')
      ->fields('mt')
      ->execute()
      ->fetchAllAssoc('name'); // Will key by the name field.
    foreach ($types as &$type) {
      // I really hate this.
      $type->type_callback_args = unserialize($type->type_callback_args);
    }
  }

  return $types;
}

/**
 *  Create the basic class and defaults for a media entity bundle type.
 */
function media_type_set_defaults($info) {
  $type = new stdClass();

  // This is used to filter a file to the proper bundle.
  $type->type_callback = 'media_is_type';
  $type->type_callback_args = array();
  $type->weight = 0;

  foreach ($info as $k => $v) {
    $type->{$k} = $v;
  }

  return $type;
}

/**
 * Determines the type of media a passed in $file is.
 *
 * @todo: integrate this properly with other APIs in media when fields is done
 * @param unknown_type $file
 * @return unknown_type
 */
function media_get_type($file) {
  $types = media_type_get_types();
  foreach ($types as $name => $type) {
    if (call_user_func_array($type->type_callback, array($file, $type->type_callback_args))) {
      return $name;
    }
  }
  throw new Exception('Unable to determine type of media from ' . var_export($file, 1));
}

/**
 * Default callback used to determine if a file is of a given type.
 *
 * @TODO: document 'any' and 'all' matching.
 *
 * @param $file
 *   The file object.
 * @param $args
 *
 * @return unknown_type
 */
function media_is_type($file, $args) {
  $match_type = !empty($args['match_type']) ? 'any' : $args['match_type'];
  $match_all = $match_type == 'all';
  if (!empty($args['mimetypes'])) {
    foreach ($args['mimetypes'] as $expression) {
      if (preg_match($expression, $file->filemime)) {
        if (!$match_all) {
          return TRUE;
        }
      }
    }
    // Was not matched, so return
    if ($match_all) {
      return FALSE;
    }
  }

  if (!empty($args['extensions'])) {
    if (in_array(pathinfo($file->uri, PATHINFO_EXTENSION), $args['extensions'])) {
      if (!$match_all) {
        return TRUE;
      }
    }
    // Was not matched, so return
    if ($match_all) {
      return FALSE;
    }
  }

  if (!empty($args['streams'])) {
  }
}

/**
 * Implements hook_file_default_displays().
 *
 * Provides default display configurations for media types.
 *
 * @see file_entity_schema()
 */
function media_file_default_displays() {
  $default_displays = array();

  $default_image_styles = array(
    'media_preview' => 'square_thumbnail',
    'media_large' => 'large',
    'media_original' => ''
  );

  foreach ($default_image_styles as $view_mode => $image_style) {
    $display_name = 'image__' . $view_mode . '__file_image';
    $default_displays[$display_name] = (object) array(
      'api_version' => 1,
      'name' => $display_name,
      'status' => 1,
      'weight' => 5,
      'settings' => array('image_style' => $image_style),
    );
  }

  return $default_displays;
}
