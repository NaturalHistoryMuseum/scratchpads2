<?php

/**
 * gm3_country_get_points
 */
function gm3_country_get_points(){}

/**
 * Helper function to convert a POLYGON/MULTIPOLYGON string to an array of
 * points.
 */
function gm3_country_convert_string($poly_string){
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
        $polygons[] = array_pop(gm3_country_convert_string('POLYGON ' . $poly_string));
      }
      return $polygons;
      break;
    default:
      // WTF!  We really shouldn't be here.
      return FALSE;
      break;
  }
}