<?php

// Belgium

function location_province_list_be() {
  return array(
    'VAN' => "Antwerpen",
    'VBR' => "Vlaams Brabant",
    'VLI' => "Limburg",
    'VOV' => "Oost-Vlaanderen",
    'VWV' => "West-Vlaanderen",
    'WBR' => "Brabant Wallon",
    'WHT' => "Hainaut",
    'WLG' => "Liege",
    'WLX' => "Luxembourg",
    'WNA' => "Namur",
    // While technically not a province, Brussels-Capital Region is needed here
    // because some places would be completely without a province if we did not
    // include it.
    // See http://drupal.org/node/228766 and http://drupal.org/node/291590.
    'BRU' => "Brussels",
  );
}

/**
 * Returns minimum and maximum latitude and longitude needed to create a bounding box.
 */
function location_bounds_be() {
  return array(
    'minlng' => 2.5104,
    'minlat' => 49.518533,
    'maxlng' => 6.3713,
    'maxlat' => 51.528667,
  );
}
