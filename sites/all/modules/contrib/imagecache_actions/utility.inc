<?php
/**
 * @file utility.inc: uitility form, conversion and rendering functions for
 * image processing.
 */

/*
 * File field handling.
 */
/**
 *
 * @param unknown_type $element
 * @param unknown_type $form_status
 */
function imagecache_actions_file_field_description() {
  return t('File is either a file with one of the valid schemes (e.g. private://, public://, module://{module}/{component}, temporary://), or an absolute or relative path (relative to the current directory, probably the Drupal site root).');
}

/**
 *
 * Enter description here ...
 * @param unknown_type $element
 * @param unknown_type $form_status
 */
function imagecache_actions_validate_file(&$element, &$form_status) {
  if (!imagecache_actions_find_file($element['#value'])) {
    form_error($element, t("Unable to find the file '%file'. Please check the path.", array('%file' => $element['#value'])) );
  }
}


/**
 * Looks up and returns the full path of the given file.
 *
 * We accept files with the following schemes:
 * - private://
 * - public://
 * - temporary://
 * - module://{module}/{component}
 *
 * Files without a scheme are looked up as are:
 * - relative: relative to the current directory (probably $drupal_root).
 * - absolute: as is.
 *
 * @param string $file
 *   A file name, with a scheme, relative, or absolute.
 *
 * @return string|false
 *   The full file path of the file, so the image toolkit knows exactly where it
 *   is.
 */
function imagecache_actions_find_file($file) {
  $result = FALSE;
  if (is_readable($file)) {
    $result = drupal_realpath($file);
  }
  return $result;
}

/**
 * Loads the given file as an image object.
 *
 * @param string $file
 *   A file name, with a scheme, relative, or absolute.
 *
 * @return object|null
 *   The image object.
 *
 * @see imagecache_actions_find_file()
 * @see image_load()
 */
function imagecache_actions_image_load($file, $toolkit = FALSE) {
  $full_path = imagecache_actions_find_file($file);
  if (!$full_path) {
    trigger_error("Failed to find file $file.", E_USER_ERROR);
    return FALSE;
  }

  $image = image_load($full_path, $toolkit);
  if (!$image) {
    trigger_error("Failed to open file '$file' as an image resource.", E_USER_ERROR);
  }

  return $image;
}


/**
 * Return an array with context information about the image.
 *
 * This information can e.g. be used by effects that allow custom PHP like
 * - Custom action.
 * - Text from PHP code.
 * - Text from alt or title.
 *
 * @param object $image
 *   The image object.
 * @param array $data
 *   An associative array with the effect options.
 *
 * @return array
 *   An associative array with context information about the image.
 */
function imagecache_actions_get_image_context($image, $data) {
  // Store context about the image.
  $image_context = array(
        'effect_data' => $data,
        'managed_file' => NULL,
        'referring_entities' => array(),
        'entity' => NULL,
        'image_field' => NULL,
  );

  // Find the managed file object (at most 1 object as 'uri' is a unique index).
  $managed_file = reset(file_load_multiple(array(), array('uri' => $image->source)));
  if ($managed_file !== FALSE) {
    $image_context['managed_file'] = $managed_file;
    // And find the entities referring to this managed file.
    $references = file_get_file_references($managed_file, NULL, FIELD_LOAD_CURRENT, 'image');
    if ($references) {
      // Load referring entities.
      foreach ($references as $field_name => $field_references) {
        foreach ($field_references as $entity_type => $entity_stubs) {
          $image_context['referring_entities'][$field_name][$entity_type] = entity_load($entity_type, array_keys($entity_stubs));
        }
      }

      // Make it easy to access the '1st' entity and its referring image field.
      reset($image_context['referring_entities']);
      list($field_name, $field_references) = each($image_context['referring_entities']);
      reset($field_references);
      list($entity_type, $entities) = each($field_references);
      reset($entities);
      list($entity_id, $image_context['entity']) = each($entities);
      $image_field = field_get_items($entity_type, $image_context['entity'], $field_name);
      if ($image_field !== FALSE) {
        // Get referring item
        foreach ($image_field as $image_field_value) {
          if ($image_field_value['fid'] === $managed_file->fid) {
            $image_context['image_field'] = $image_field_value;
          }
        }
      }
    }
  }

  // @todo: support for media module, or is that based upon managed files?
  return $image_context;
}

/**
 * Given two imageapi objects with dimensions, and some positioning values,
 * calculate a new x,y for the layer to be placed at.
 *
 * This is a different approach to imagecache_actions_keyword_filter() - more
 * like css.
 *
 * The $style is an array, and expected to have 'top,bottom, left,right'
 * attributes set. These values may be positive, negative, or in %.
 *
 * % is calculated relative to the base image dimensions.
 * Using % requires that the layer is positioned CENTERED on that location, so
 * some offsets are added to it. 'right-25%' is not lined up with a margin 25%
 * in, it's centered at a point 25% in - which is therefore identical with
 * left+75%
 *
 * @return a keyed array of absolute x,y co-ordinates to place the layer at.
 */
function imagecache_actions_calculate_relative_position($base, $layer, $style) {
  // both images should now have size info available.

  if (isset($style['bottom'])) {
    $ypos = imagecache_actions_calculate_offset('bottom', $style['bottom'], $base->info['height'], $layer->info['height']);
  }
  if (isset($style['top'])) {
    $ypos = imagecache_actions_calculate_offset('top', $style['top'], $base->info['height'], $layer->info['height']);
  }
  if (isset($style['right'])) {
    $xpos = imagecache_actions_calculate_offset('right', $style['right'], $base->info['width'], $layer->info['width']);
  }
  if (isset($style['left'])) {
    $xpos = imagecache_actions_calculate_offset('left', $style['left'], $base->info['width'], $layer->info['width']);
  }
  if (! isset($ypos)) {
    // assume center
    $ypos = ($base->info['height'] / 2) - ($layer->info['height'] / 2);
  }
  if (! isset($xpos)) {
    // assume center
    $xpos = ($base->info['width'] / 2) - ($layer->info['width'] / 2);
  }
  #dpm(__FUNCTION__ . " Calculated offsets");
  #dpm(get_defined_vars());

  return array('x' => $xpos, 'y' => $ypos);
}

/**
 * Positive numbers are IN from the edge, negative offsets are OUT.
 *
 * $keyword, $value, $base_size, $layer_size
 * eg
 * left,20 200, 100 = 20
 * right,20 200, 100 = 80 (object 100 wide placed 20 px from the right = x=80)
 *
 * top,50%, 200, 100 = 50 (Object is centered when using %)
 * top,20%, 200, 100 = -10
 * bottom,-20, 200, 100 = 220
 * right, -25%, 200, 100 = 200 (this ends up just offscreen)
 *
 *
 * Also, the value can be a string, eg "bottom-100", or "center+25%"
 */
function imagecache_actions_calculate_offset($keyword, $value, $base_size, $layer_size) {
  $offset = 0; // used to account for dimensions of the placed object
  $direction = 1;
  $base = 0;
  if ($keyword == 'right' || $keyword == 'bottom') {
    $direction = -1;
    $offset = -1 * $layer_size;
    $base = $base_size;
  }
  if ($keyword == 'middle' || $keyword == 'center') {
    $base = $base_size / 2;
    $offset = -1 * ($layer_size / 2);
  }

  // Keywords may be used to stand in for numeric values
  switch ($value) {
    case 'left':
    case 'top':
      $value = 0;
      break;
    case 'middle':
    case 'center':
      $value = $base_size / 2;
      break;
    case 'bottom':
    case 'right':
      $value = $base_size;
  }

  // Handle keyword-number cases
  // @see imagecache_actions_keyword_filter
  // top+50% or bottom-100px
  if (preg_match('/^(.+)([\+\-])(\d+)([^\d]*)$/', $value, $results)) {
    list($match, $value_key, $value_mod, $mod_value, $mod_unit) = $results;
    if ($mod_unit == '%') {
      $mod_value = $mod_value / 100 * $base_size;
      $mod_unit = 'px';
    }
    $mod_direction = ($value_mod == '-') ? -1 : + 1;
    switch ($value_key) {
      case 'left':
      case 'top':
        $mod_base = 0;
        break;
      case 'middle':
      case 'center':
        $mod_base = $base_size / 2;
        break;
      case 'bottom':
      case 'right':
        $mod_base = $base_size;
    }
    $modified_value = $mod_base + ($mod_direction * $mod_value);
    return $modified_value;
  }
  #dpm(get_defined_vars());

  // handle % values
  if (substr($value, strlen($value) -1, 1) == '%') {
    $value = intval($value / 100 * $base_size);
    $offset = -1 * ($layer_size / 2);
  }
  $value = $base + ($direction * $value);

  #dpm(__FUNCTION__ . " Placing an object $layer_size big on a range of $base_size at a position of $value , $offset");
  #dpm(get_defined_vars());

  // Add any extra offset to position the item
  return $value + $offset;
}


/**
 * Convert a hex string to its RGBA (Red, Green, Blue, Alpha) integer
 * components.
 *
 * Stolen from imageapi D6 2011-01
 *
 * @param $hex
 *   A string specifing an RGB color in the formats:
 *   '#ABC','ABC','#ABCD','ABCD','#AABBCC','AABBCC','#AABBCCDD','AABBCCDD'
 * @return
 *   An array with four elements for red, green, blue, and alpha.
 */
function imagecache_actions_hex2rgba($hex) {
  $hex = ltrim($hex, '#');
  if (preg_match('/^[0-9a-f]{3}$/i', $hex)) {
    // 'FA3' is the same as 'FFAA33' so r=FF, g=AA, b=33
    $r = str_repeat($hex{0}, 2);
    $g = str_repeat($hex{1}, 2);
    $b = str_repeat($hex{2}, 2);
    $a = '0';
  }
  elseif (preg_match('/^[0-9a-f]{6}$/i', $hex)) {
    // #FFAA33 or r=FF, g=AA, b=33
    list($r, $g, $b) = str_split($hex, 2);
    $a = '0';
  }
  elseif (preg_match('/^[0-9a-f]{8}$/i', $hex)) {
    // #FFAA33 or r=FF, g=AA, b=33
    list($r, $g, $b, $a) = str_split($hex, 2);
  }
  elseif (preg_match('/^[0-9a-f]{4}$/i', $hex)) {
    // 'FA37' is the same as 'FFAA3377' so r=FF, g=AA, b=33, a=77
    $r = str_repeat($hex{0}, 2);
    $g = str_repeat($hex{1}, 2);
    $b = str_repeat($hex{2}, 2);
    $a = str_repeat($hex{3}, 2);
  }
  else {
    //error: invalide hex string, TODO: set form error..
    return FALSE;
  }

  $r = hexdec($r);
  $g = hexdec($g);
  $b = hexdec($b);
  $a = hexdec($a);
  // alpha over 127 is illegal. assume they meant half that.
  if ($a > 127) {
    $a = (int)$a/2;
  }
  return array('red' => $r, 'green' => $g, 'blue' => $b, 'alpha' => $a);
}



/**
 * Accept a keyword (center, top, left, etc) and return it as an offset in pixels.
 * Called on either the x or y values.
 *
 * May  be something like "20", "center", "left+20", "bottom+10". + values are
 * in from the sides, so bottom+10 is 10 UP from the bottom.
 *
 * "center+50" is also OK.
 *
 * "30%" will place the CENTER of the object at 30% across. to get a 30% margin,
 * use "left+30%"
 *
 * @param $value
 *   string or int value.
 * @param $current_size
 *   int size in pixels of the range this item is to be placed in
 * @param $object_size
 *   int size in pixels of the object to be placed
 *
 *
 */
function imagecache_actions_keyword_filter($value, $base_size, $layer_size) {
  // See above for the patterns this matches
  if (! preg_match('/([a-z]*)([\+\-]?)(\d*)([^\d]*)/', $value, $results) ) {
    trigger_error("imagecache_actions had difficulty parsing the string '$value' when calculating position. Please check the syntax.", E_USER_WARNING);
  }
  list($match, $keyword, $plusminus, $value, $unit) = $results;

  #dpm(__FUNCTION__ . " Placing an object $layer_size big on a range of $base_size at a position of $value");
  #dpm(get_defined_vars());

  return imagecache_actions_calculate_offset($keyword, $plusminus . $value . $unit, $base_size, $layer_size);
}

/**
 * imagecache is conservative with its inclusion of inc files, but sometimes I
 * need to use them - eg crop. This function finds and includes it if needed.
 */
function imagecache_include_standard_actions() {
  //$cropaction = imagecache_action_definition('imagecache_crop');
  //include_once DRUPAL_ROOT . '/' . $cropaction['file'];
}

/**
 * Given a file path relative the default file scheme, tries to locate it and,
 * is successful, finds all fields that use it.
 *
 * @param $filepath Actually a Drupal file URI, probably.
 *
 * @return
 *   A nested array of basic entity data, grouped by entity type and field name.
 */
function imagecache_actions_fields_from_filepath($filepath, $load_entity = TRUE) {
  if (!module_exists('file')) {
    return;
  }

  // Unsure if we are given a real filepath (the normal assumption) or a
  // Drupal scheme URI. Resolve either.
  if (! file_valid_uri($filepath)) {
    $filepath = file_build_uri($filepath);
  }

  $files = file_load_multiple(array(), array('uri' => $filepath));

  if ($files) {
    $fields = array();
    foreach ($files as $fid => $file) {
      $references = file_get_file_references($file, NULL, FIELD_LOAD_CURRENT, 'image');
      $fields[$fid] = $references;
    }

    if ($load_entity) {
      // These references are pretty low on information. Load the actual nodes/entities too.
      imagecache_actions_entities_from_references($fields);
    }


    return $fields;
  }
}

/**
 * Load additional data - Fully load the entities that actually use the given file, with all their data.
 *
 * Replaces the full entity object in place of the placeholder object that file_get_file_references() gives us.
 */
function imagecache_actions_entities_from_references(&$fields) {
  foreach ($fields as $fid => $field_ids) {
    foreach ($field_ids as $field_id => $entity_types) {
      foreach ($entity_types as $entity_type => $entity_instances) {
        #$entities = entity_load($entity_type, array_keys($entity_instances));
        foreach ($entity_instances as $entity_id => $entity) {
          $entity = entity_load_single($entity_type, $entity_id);
          // Add this extra data to the return info, replacing the lightweight version
          $fields[$fid][$field_id][$entity_type][$entity_id] = $entity;

          #// Also bubble up the field info so it's easier to get at? No, that's way too deep.
          #// Still, it would be a common use-case to fetch the 'title' from that file.
          #// Don't know WHICH of the files our target file is yet, so need to scan. Boring
          #$entity_refs = $references[$fid][$field_id][$entity_type][$entity_id][$field_id][$entity->language];
          #foreach ($entity_refs as $delta => $file_details) {
          #  if ($file_details['fid'] == $fid) {
          #    // Found it. This has the alt and title metadata we expect.
            #    $file_info = $file_details;
            #    // Now what?
          #  }
          #} // Scan actual field data to find the file again.

        } // All actual entities
      } // All types of entity that ref the file
    } // All references to this file
  } // All entries for this file found in the db (expected to be only 1)
}


function imagecache_actions_percent_filter($value, $current_pixels) {
  if (strpos($value, '%') !== false) {
    $value = str_replace('%', '', $value) * 0.01 * $current_pixels;
  }
  return $value;
}

