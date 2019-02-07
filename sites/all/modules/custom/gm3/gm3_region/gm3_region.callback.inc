<?php

/**
 * Get the region_id from the lat,lng and zoom
 */
function gm3_region_get_region_id_from_latlng($latlng, $iso_codes, $level = 4){
  $iso_codes = strtoupper($iso_codes);
  gm3_region_switch_to_gm3_database();
  drupal_add_http_header('Content-Type', 'application/json; charset=utf-8');
  // Because the TDWG data and Google data don't quite match, here we convert
  // some of the codes sent to us by Google.
  // TODO: Is this still the case now we use OSM?
  $convert_codes = array(
    'GB' => 'UK',
    'RS' => 'YU',
    'ME' => 'MK',
    'SS' => 'SD',
    'UNKNOWN' => array(
      'YU',
      'IN'
    )
  );
  if(isset($convert_codes[$iso_codes])){
    $iso_codes = $convert_codes[$iso_codes];
  }
  if(!is_array($iso_codes)){
    $iso_codes = array(
      $iso_codes
    );
  }

  $latlng = preg_replace("/[)(]/", "", $latlng);
  $latlng = explode(", ", $latlng);
  $point_wkt = "POINT({$latlng[1]} {$latlng[0]})";
  // Find regions that contain the point (filter out continents with level_5_code != '')
  $sql = "SELECT * FROM {gm3_region_data} WHERE iso_code IN (:iso_codes) AND WITHIN(POINTFROMTEXT(:point_wkt), mysql_polygons) AND level_5_code != ''";
  $results = db_query($sql, array(
    ':point_wkt' => $point_wkt,
    ':iso_codes' => $iso_codes
  ));

  // Apparently mysql is not always so accurate for geo so we use geophp to do further filtering
  // Load the Library.
  gm3_load_geophp();
  $wkt_reader = new WKT();
  // Create a point object from the sent data.
  $point = geoPHP::load($point_wkt, 'wkt');

  $region_id = null;

  // Find the first polygon that contains the selected point
  foreach($results as $result){
    $polygon = $wkt_reader->read($result->polygons, TRUE);

    if ($polygon->contains($point)) {
      break;
    }
  }

  // Form the tdwg region ID
  // Todo: This is messy, can it be made neater?
  switch($level){
    case 5:
      $region_id = "{$result->level_1_code}:{$result->level_2_code}:{$result->level_3_code}:{$result->level_3_code}-{$result->level_4_code}" . ($result->level_5_code != '00' ? ":{$result->level_5_code}" : '');
      break;
    case 4:
      $region_id = "{$result->level_1_code}:{$result->level_2_code}:{$result->level_3_code}:{$result->level_3_code}-{$result->level_4_code}";
      break;
    case 3:
      $region_id = "{$result->level_1_code}:{$result->level_2_code}:{$result->level_3_code}";
      break;
    case 2:
      $region_id = "{$result->level_1_code}:{$result->level_2_code}";
      break;
    case 1:
      $region_id = "{$result->level_1_code}";
      break;
  }

  print json_encode($region_id);
  // Set back to the default database, in case some exit functions require the
  // original database.
  db_set_active();
  // Perform end-of-request tasks.
  ajax_footer();
  exit();
}

/**
 * gm3_region_get_points
 */
function gm3_region_get_points($region_ids){
  drupal_add_http_header('Content-Type', 'application/json; charset=utf-8');
  $region_ids = explode(",", $region_ids);
  print "[";
  $first = TRUE;
  foreach($region_ids as $region_id){
    if($first){
      $first = FALSE;
    }else{
      print ",";
    }
    gm3_region_switch_to_gm3_database();
    $polygons = cache_get($region_id, 'cache_gm3_polygon');
    if($polygons === false){
      module_load_include('functions.inc', 'gm3');
      $region_id_parts = explode(":", $region_id);
      // Load the geoPHP library.
      gm3_load_geophp();
      $wkt_reader = new WKT();
      switch(count($region_id_parts)){
        case 5:
          if($region_id_parts[4] != '00'){
            $row = db_select('gm3_region_data', 'g')->condition('level_5_code', $region_id_parts[4])->fields('g')->execute()->fetch();
            break;
          }
          array_pop($region_id_parts);
          $region_id = implode(':', $region_id_parts);
        case 4:
          $region_id_4_parts = explode("-", $region_id_parts[3]);
          $row = db_select('gm3_region_data', 'g')->condition('level_4_code', $region_id_4_parts[1])->condition('level_3_code', $region_id_4_parts[0])->fields('g')->execute()->fetch();
          break;
        case 3:
          $row = db_select('gm3_region_data', 'g')->condition(db_or()->condition('level_3_code', $region_id_parts[2])->condition('name', $region_id_parts[2]))->condition('level_4_code', '')->fields('g')->execute()->fetch();
          break;
        case 2:
          if(is_numeric($region_id_parts[1])){
            $row = db_select('gm3_region_data', 'g')->condition('level_2_code', $region_id_parts[1])->condition('level_3_code', '')->fields('g')->execute()->fetch();
          }else{
            $row = db_select('gm3_region_data', 'g')->condition('name', $region_id_parts[1])->condition('level_3_code', '')->fields('g')->execute()->fetch();
          }
          break;
        case 1:
          if(is_numeric($region_id_parts[0])){
            $row = db_select('gm3_region_data', 'g')->condition('level_1_code', $region_id_parts[0])->condition('level_2_code', 0)->fields('g')->execute()->fetch();
          }else{
            $row = db_select('gm3_region_data', 'g')->condition('name', $region_id_parts[0])->condition('level_2_code', 0)->fields('g')->execute()->fetch();
          }
          break;
      }
      if($row){
        $matches = array();
        preg_match_all('/\.[0-9]{1,20}/', $row->polygons, $matches);
        $matches = $matches[0];
        usort($matches, 'gm3_region_sort_by_length');
        $replaces = array();
        foreach($matches as $match){
          $replaces[] = substr($match, 0, 5); // Accurate to 4 decimal places, or 11 metres.
        }
        $row->polygons = str_replace($matches, $replaces, $row->polygons);
        $polygons = $wkt_reader->read($row->polygons, TRUE);
        $polygons = json_decode($polygons->out('json'));
        $polygons = json_encode(array(
          "regionId" => $region_id,
          "name" => $row->name,
          "shape" => $polygons
        ));
      }
      cache_set($region_id, $polygons, 'cache_gm3_polygon', CACHE_PERMANENT);
    }else{
      $polygons = $polygons->data;
    }
    print $polygons;
  }
  print "]";
  db_set_active();
  // Perform end-of-request tasks.
  ajax_footer();
}

/**
 * Helper function to sort the array of matches by their length.
 */
function gm3_region_sort_by_length($a, $b){
  return strlen($b) - strlen($a);
}
