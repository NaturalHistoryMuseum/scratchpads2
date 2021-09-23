<?php

/**
 * Helper function to convert a POLYGON/MULTIPOLYGON string to an array of
 * points.
 * 
 * REALLY NEED TO REMOVE THIS FUNCTION - We have the geoPHP library installed
 * for a reason.
 */
function gm3_convert_polygon_string($poly_string){
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
        $lat_long_parts = explode(" ", trim($lat_long));
        $lat_longs[$index] = array(
          $lat_long_parts[0],
          $lat_long_parts[1]
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
        $polygons[] = array_pop(gm3_convert_polygon_string('POLYGON ' . $poly_string));
      }
      return $polygons;
      break;
    default:
      // WTF!  We really shouldn't be here.
      return FALSE;
      break;
  }
}