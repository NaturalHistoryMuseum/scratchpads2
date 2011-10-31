<?php

/**
 * gm3_region_get_points
 */
function gm3_region_get_points($region_id){
  $cid = $region_id;
  $polygons = cache_get($cid, 'cache_gm3_polygon');
  if($polygons === false){
    module_load_include('functions.inc', 'gm3');
    $region_id_parts = explode(":", $region_id);
    switch(count($region_id_parts)){
      case 4:
        /* 
         * Need to add a way of querying by a country code.
        $result = db_select('gm3_region_data', 'g')->condition('iso_code', $region_id)->fields('g')->execute();
        $polygons = array();
        foreach($result as $something){
          foreach(gm3_convert_polygon_string($something->polygons) as $polygon){
            $polygons[] = $polygon;
          }
        }*/
        $region_id = explode("-", $region_id_parts[3]);
        $result = db_select('gm3_region_data', 'g')->condition('level_4_code', $region_id[1])->condition('level_3_code', $region_id[0])->fields('g')->execute();
        foreach($result as $something){
          foreach(gm3_convert_polygon_string($something->polygons) as $polygon){
            $polygons[] = $polygon;
          }
        }
        break;
      case 3:
        $region_id = array_pop($region_id_parts);
        $result = db_select('gm3_region_data', 'g')->condition(db_or()->condition('level_3_code', $region_id)->condition('name', $region_id))->condition('level_4_code', '')->fields('g')->execute();
        foreach($result as $something){
          foreach(gm3_convert_polygon_string($something->polygons) as $polygon){
            $polygons[] = $polygon;
          }
        }
        break;
      case 2:
        $region_id = array_pop($region_id_parts);
        if(is_numeric($region_id)){
          $result = db_select('gm3_region_data', 'g')->condition('level_2_code', $region_id)->condition('level_3_code', '')->fields('g')->execute();
        }else{
          $result = db_select('gm3_region_data', 'g')->condition('name', $region_id)->condition('level_3_code', '')->fields('g')->execute();
        }
        foreach($result as $something){
          foreach(gm3_convert_polygon_string($something->polygons) as $polygon){
            $polygons[] = $polygon;
          }
        }
        break;
      case 1:
        $region_id = array_pop($region_id_parts);
        if(is_numeric($region_id)){
          $result = db_select('gm3_region_data', 'g')->condition('level_1_code', $region_id)->condition('level_2_code', 0)->fields('g')->execute();
        }else{
          $result = db_select('gm3_region_data', 'g')->condition('name', $region_id)->condition('level_2_code', 0)->fields('g')->execute();
        }
        foreach($result as $something){          
          foreach(gm3_convert_polygon_string($something->polygons) as $polygon){
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