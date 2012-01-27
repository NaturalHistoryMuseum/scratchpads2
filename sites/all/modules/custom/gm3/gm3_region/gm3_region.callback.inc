<?php

/**
 * Get the region_id from the lat,lng and zoom
 */
function gm3_region_get_region_id_from_latlng($latlng, $iso_codes, $level){
  // Because the TDWG data and Google data don't quite match, here we convert
  // some of the codes sent to us by Google.
  $convert_codes = array(
    'GB' => 'UK',
    'RS' => 'YU',
    'ME' => 'MK',
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
  // Load the Library.
  gm3_load_geophp();
  $wkb_reader = new WKT();
  // Create a point object from the sent data.
  $latlng = preg_replace("/[)(]/", "", $latlng);
  $latlng = explode(", ", $latlng);
  $point = geoPHP::load("POINT ({$latlng[1]} {$latlng[0]})", "wkt");
  foreach($iso_codes as $iso_code){
    // Loop through each Level 1 region.
    $region_id = '';
    $level4s = db_select('gm3_region_data', 'g')->fields('g')->condition('level_4_code', '', '!=')->condition('iso_code', $iso_code, 'LIKE')->execute();
    $level_2_codes = array();
    foreach($level4s as $level4){
      $level_2_codes[$level4->level_2_code] = $level4->level_2_code;
      $polygon = $wkb_reader->read($level4->polygons, TRUE);
      // There appears to be a tiny bug with the WKB.class, resulting in a
      // notice about an unknown variable.
      if(@$point->within($polygon)){
        $region_id = "{$level4->level_1_code}:{$level4->level_2_code}:{$level4->level_3_code}:{$level4->level_3_code}-{$level4->level_4_code}";
        // Stop on the first match.
        break;
      }
    }
    if(!$region_id){
      // Due to a fuck up with the TDWG data, we can't restrict by the iso_code
      // being null, so instead we search by the level_2_code.
      foreach($level_2_codes as $level_2_code){
        $level4s = db_select('gm3_region_data', 'g')->fields('g')->condition('level_4_code', '', '!=')->condition('level_2_code', $level_2_code)->execute();
        foreach($level4s as $level4){
          $polygon = $wkb_reader->read($level4->polygons, TRUE);
          // There appears to be a tiny bug with the WKB.class, resulting in a
          // notice about an unknown variable.
          if(@$point->within($polygon)){
            $region_id = "{$level4->level_1_code}:{$level4->level_2_code}:{$level4->level_3_code}:{$level4->level_3_code}-{$level4->level_4_code}";
            // stop on the first match
            break 2;
          }
        }
      }
    }
    if($region_id){
      break;
    }
  }
  if($region_id && $level < 4){
    $region_id = explode(":", $region_id);
    for($i = 4; $i > $level; $i--){
      array_pop($region_id);
    }
    $region_id = implode(":", $region_id);
  }
  print json_encode($region_id);
  exit();
}

/**
 * gm3_region_get_points
 */
function gm3_region_get_points($region_ids){
  $region_ids = explode(",", $region_ids);
  print "[";
  $first = TRUE;
  foreach($region_ids as $region_id){
    if($first){
      $first = FALSE;
    }else{
      print ",";
    }
    $cid = $region_id;
    $polygons = cache_get($cid, 'cache_gm3_polygon');
    if($polygons === false){
      module_load_include('functions.inc', 'gm3');
      $region_id_parts = explode(":", $region_id);
      // Load the geoPHP library.
      gm3_load_geophp();
      $wkb_reader = new WKT();
      switch(count($region_id_parts)){
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
        $polygons = $wkb_reader->read($row->polygons, TRUE);
        $polygons = $polygons->out('json');
        $polygons = '{"' . $region_id . '":{"name":"'.$row->name.'","shape":' . $polygons . '}}';
      }
      cache_set($cid, $polygons, 'cache_gm3_polygon', CACHE_PERMANENT);
    }else{
      $polygons = $polygons->data;
    }
    print $polygons;
  }
  print "]";
  exit();
}