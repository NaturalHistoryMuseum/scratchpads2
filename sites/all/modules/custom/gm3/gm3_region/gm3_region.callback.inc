<?php

/**
 * gm3_region_get_points
 */
function gm3_region_get_points($region_type, $region_id){
  $cid = "{$region_type}-{$region_id}";
  $polygons = cache_get($cid, 'cache_gm3_polygon');
  if($polygons === false){
    module_load_include('functions.inc', 'gm3');
    switch($region_type){
      case 'countries':
        $result = db_select('gm3_region_data', 'g')->condition('iso_code', $region_id)->fields('g')->execute();
        $polygons = array();
        foreach($result as $something){
          foreach(gm3_convert_polygon_string($something->polygons) as $polygon){
            $polygons[] = $polygon;
          }
        }
        break;
      case 'level4':
        $region_id = explode("-", $region_id);
        $result = db_select('gm3_region_data', 'g')->condition('level_4_code', $region_id[1])->condition('level_3_code', $region_id[0])->fields('g')->execute();
        foreach($result as $something){
          foreach(gm3_convert_polygon_string($something->polygons) as $polygon){
            $polygons[] = $polygon;
          }
        }
        break;
      case 'level3':
        $result = db_select('gm3_region_data', 'g')->condition(db_or()->condition('level_3_code', $region_id)->condition('name', $region_id))->condition('level_4_code', '')->fields('g')->execute();
        foreach($result as $something){
          foreach(gm3_convert_polygon_string($something->polygons) as $polygon){
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
          foreach(gm3_convert_polygon_string($something->polygons) as $polygon){
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