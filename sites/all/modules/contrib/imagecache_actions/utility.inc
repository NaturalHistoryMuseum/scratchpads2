<?php
/**
 * Utility form, conversion and rendering functions for image processes
 */

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
 * Given only a file filename, track back to see if we can detect the parent
 * node and provide some context info.
 *
 * This will be different in different cases.
 * Supported :
 * image.module image nodes
 * imagefield cck fields (may be multiple)
 * upload.module attachments
 *
 * TODO: image_attach attachments
 *
 * @param $filepath
 * @param $file_data MAY get some details filled in on it by reference if data
 * was found.
 *
 * @return a loaded $node object
 */
function imagecache_actions_node_from_filepath($filepath, &$file_data = NULL) {

  // lookup upload.module attachments
  if (module_exists('upload')) {
    $sql = "SELECT nid, f.fid FROM {upload} AS u INNER JOIN {files} AS f ON u.fid = f.fid WHERE f.filepath = '%s'";
    $results = db_query_range("SELECT nid, f.fid FROM {upload} AS u INNER JOIN {files} AS f ON u.fid = f.fid WHERE f.filepath = :f.filepath", array(':f.filepath' => $filepath), 0, 1);
    if ( $row = db_fetch_array($results)) {
      // Return immediately
      $node = node_load($row['nid']);
      // also include the file description
      $file_data = $node->files[$row['fid']];
      return $node;
    }
  }

  // Lookup image.module nodes
  if (module_exists('image')) {
    $sql = "SELECT nid FROM {image} AS i INNER JOIN {files} AS f ON i.fid = f.fid WHERE f.filepath = '%s'";
    if ( $nid = db_query_range("SELECT nid FROM {image} AS i INNER JOIN {files} AS f ON i.fid = f.fid WHERE f.filepath = :f.filepath", array(':f.filepath' => $filepath), 0, 1)->fetchField()) {
      // Return immediately
      return node_load($nid);
    }
  }


  // Lookup filefield imagefield CCK attachments.
  //
  // Drupal 6 version here based largely on work done by mikeytown2
  // http://drupal.org/node/363434

  // This is a terrible way to retrieve information, but CCK doesn't provide a way to reverse-lookup like this
  // BAD CODE follows
  // If someone could roll these DBlookups into a smaller ball, that would be fun.
  // Due to CCKs use of dynamically created table names .. I don't know how.

  // Multiple file ID's might have the same name, get all
  // (but return just the first successful match)
  $result = db_query("SELECT fid FROM {files} WHERE filepath = :filepath", array(':filepath' => $filepath));
  $fids = array();
  while ($row = db_fetch_array($result)) {
    $fids[] = $row['fid'];
  }

  if (! empty($fids)) {
    // Find out if any filefield contains this file, and if so, which field
    // and node it belongs to. Required for later access checking.
    // CCK field analysis is in the outer loop,
    // fids are scanned in the inner loop for a little speed.

    // Get A List Of ALL CCK Fields and look for them individually,
    // it's the only way we can reverse the lookups
    foreach (content_fields() as $field) {
      // If Field is an Image (imagefield.module) or filefield then
      if ($field['type'] == 'image' || $field['type'] == 'filefield') {
        // Need to do lots of lookups to find out what the storage tables look like.
        // Grab All DB Column Names for that CCK Field
        $db_info = content_database_info($field);
        // Get Content Type DB name - FROM statement
        $tablename = $db_info['table'];
        //Get File ID DB Column Name - WHERE statement
        $fid_column = $db_info['columns']['fid']['column'];

        // Construct a Query that looks for all known fids in one go.
        // eg:
        // SELECT nid FROM content_type_story
        //   WHERE field_illustration_fid = 77
        //   OR field_illustration_fid = 99;

        $wheres = array();
        $query_args = array();
        foreach ($fids as $fid) {
          $wheres[] = " %s = %d ";
          $query_args[] = $fid_column;
          $query_args[] = $fid;
        }

        // TODO Please convert this statement to the D7 database API syntax.
        $result = db_query('SELECT nid FROM {' . $tablename . '} WHERE ' . join(' OR ', $wheres), $query_args);

        while ($row = db_fetch_array($result)) {
          // This while is a dummy loop - Just break out and return the first matching node.
          // If more than one node owns this image then ???
          $node = node_load($row['nid']);

          // Add even more info - the description "data" that MAY have been added
          // to this file on this node using filefield_meta and stuff.
          // Return the data associated specifically with this file also;
          // Do this via the handle on the file_data object we were passed in.
          // Slightly mushed together - I want it to mostly resemble the traditional file attachment object.

          // We have the node but lost track of the file in the process.
          // Need to scan again to make sure we got the right one :-{
          if ( $file_fields = $node->{$field['field_name']} ) {
            foreach ($file_fields as $file_field) {
              if ($file_field['fid'] == $fid) {
                $actual_file = $file_field;
              }
            }
            $file_data = (object) array_merge((array) $file_data, $actual_file['data'], $actual_file);

          }
          return $node;
        }
      }
    }
  }
}

function imagecache_actions_percent_filter($value, $current_pixels) {
  if (strpos($value, '%') !== false) {
    $value = str_replace('%', '', $value) * 0.01 * $current_pixels;
  }
  return $value;
}

