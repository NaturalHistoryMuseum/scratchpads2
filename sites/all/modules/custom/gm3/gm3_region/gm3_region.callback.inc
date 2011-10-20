<?php

/**
 * gm3_region_get_points
 */
function gm3_region_get_points($region_type, $region_id){
  $cid = "{$region_type}-{$region_id}";
  $polygons = cache_get($cid, 'cache_gm3_polygon');
  if($polygons === false){
    switch($region_type){
      case 'countries':
        $result = db_select('gm3_region_data', 'g')->condition('iso_code', $region_id)->fields('g')->execute();
        $polygons = array();
        foreach($result as $something){
          foreach(gm3_region_convert_string($something->polygons) as $polygon){
            $polygons[] = $polygon;
          }
        }
        break;
      case 'level4':
        $region_id = explode("-", $region_id);
        $result = db_select('gm3_region_data', 'g')->condition('level_4_code', $region_id[1])->condition('level_3_code', $region_id[0])->fields('g')->execute();
        foreach($result as $something){
          foreach(gm3_region_convert_string($something->polygons) as $polygon){
            $polygons[] = $polygon;
          }
        }
        break;
      case 'level3':
        $result = db_select('gm3_region_data', 'g')->condition(db_or()->condition('level_3_code', $region_id)->condition('name', $region_id))->condition('level_4_code', '')->fields('g')->execute();
        foreach($result as $something){
          foreach(gm3_region_convert_string($something->polygons) as $polygon){
            $polygons[] = $polygon;
          }
        }
        break;
      case 'level2':
        if(is_numeric($region_id)){
          $result = db_select('gm3_region_data', 'g')->condition('level_2_code', $region_id)->condition('level_3_code', '')->fields('g')->execute();
        }else{
          $result = db_select('gm3_region_data', 'g')->condition('name', $region_id)->condition('level_3_code', '')->fields('g')->execute();
        }
        foreach($result as $something){
          foreach(gm3_region_convert_string($something->polygons) as $polygon){
            $polygons[] = $polygon;
          }
        }
        break;
      case 'level1':
        if(is_numeric($region_id)){
          $result = db_select('gm3_region_data', 'g')->condition('level_1_code', $region_id)->condition('level_2_code', 0)->fields('g')->execute();
        }else{
          $result = db_select('gm3_region_data', 'g')->condition('name', $region_id)->condition('level_2_code', 0)->fields('g')->execute();
        }
        foreach($result as $something){          
          foreach(gm3_region_convert_string($something->polygons) as $polygon){
            $polygons[] = $polygon;
          }
        }
        break;
    }
    $polygons = json_encode($polygons);
    cache_set($cid, $polygons, 'cache_gm3_polygon', CACHE_PERMANENT);
  } else {
    $polygons = $polygons->data;
  }
  print $polygons;
  exit();
}

/**
 * Helper function to convert a POLYGON/MULTIPOLYGON string to an array of
 * points.
 */
function gm3_region_convert_string($poly_string){
  // Increase the memory limit, as some shapes are a little on the large side.
  // This won't be a problem once they're cached.
  ini_set('memory_limit', '256M');
  switch(substr($poly_string, 0, 7)){
    case 'POLYGON':
      // Get rid of unneeded text      
      $poly_string = substr($poly_string, strpos($poly_string, "("));
      $poly_string = str_replace("(", "", $poly_string);
      $poly_string = str_replace(")", "", $poly_string);
      $lat_longs = explode(",", $poly_string);
      foreach($lat_longs as $index => $lat_long){
        $lat_long_parts = explode(" ", $lat_long);
        $lat_longs[$index] = array(
          'long' => $lat_long_parts[0],
          'lat' => $lat_long_parts[1]
        );
      }
      return array(
        $lat_longs
      );
      break;
    case 'MULTIPO':
      // Split the string, and then call this same function with the split
      // strings.
      $poly_string = substr($poly_string, strpos($poly_string, "("));
      $poly_strings = explode("),(", $poly_string);
      $polygons = array();
      foreach($poly_strings as $poly_string){
        $polygons[] = array_pop(gm3_region_convert_string('POLYGON ' . $poly_string));
      }
      return $polygons;
      break;
    default:
      // WTF!  We really shouldn't be here.
      return FALSE;
      break;
  }
}