<?php

// Estonia

function location_province_list_ee() {
  return array('37' => "Harju County",
    '39' => "Hiiu County",
    '44' => "Ida-Viru County",
    '49' => "J�geva County",
    '51' => "JArva County",
    '57' => "LAAne County",
    '59' => "LAAne-Viru County",
    '65' => "P�lva County",
    '67' => "PArnu County",
    '70' => "Rapla County",
    '74' => "Saare County",
    '78' => "Tartu County",
    '82' => "Valga County",
    '84' => "Viljandi County",
    '86' => "V�ru County");
}

/**
 * Returns minimum and maximum latitude and longitude needed to create a bounding box.
 */
function location_bounds_ee() {
  return array(
    'minlng' => 21.75065,
    'minlat' => 57.6099,
    'maxlng' => 28.1741,
    'maxlat' => 59.6835,
  );
}
