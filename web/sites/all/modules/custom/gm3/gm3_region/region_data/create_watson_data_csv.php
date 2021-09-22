<?php
/**
 * Use the ogrinfo command to extract the data from the shapefile into a text
 * file.  This can then be parsed with the function below to create a much more
 * usable CSV file.
 *
 * $ ogrinfo file.shp layer_name > watson_vice_counties
 *
 * Note, this function is never called, it's here to help update the data.
 *
 * CREATE TABLE IF NOT EXISTS temp_load (code INT(11), name VARCHAR(255), polygon LONGTEXT);
 * TRUNCATE temp_load;
 * LOAD DATA INFILE 'watson_vice_counties.csv' INTO TABLE temp_load FIELDS TERMINATED BY ',' OPTIONALLY ENCLOSED BY '"';
 * DELETE FROM cache_gm3_polygon WHERE cid LIKE '1:10:GRB:GRB-OO:%';
 * UPDATE gm3_region_data SET polygons = (SELECT polygon FROM temp_load WHERE code = level_5_code) WHERE level_5_code IN (SELECT code FROM temp_load);
 *
 * Then you will need to rebuild the cache by requesting each polygon
 *
 * $ for i in $(seq 1 112)
 * $ do
 * $   wget http://example.com/gm3_region/callback/1:10:GRB:GRB-00:$i -O /dev/null
 * $ done
 *
 * To update the SQL files, simply dump the database tables.
 * UPDATE gm3_region_data SET mysql_polygons = '';
 * $ mysqldump databasename cache_gm3_polygon gm3_region_data | grep ^INSERT | sed "s/{/LBRACE/g;s/}/RBRACE/g;s/\`gm3_region_data\`/{gm3_region_data}/;s/\`cache_gm3_polygon\`/{cache_gm3_polygon}/" > regions.sql
 * $ split -l1 -d -a3 regions.sql sql_files/regions.sql.
 * UPDATE gm3_region_data SET mysql_polygons = POLYGONFROMTEXT(polygons);
 * 
 * Note.  Depending on the order of the columns in the gm3_region_data table, you may need to add something like
 *
 * $ sed "s/VALUES/(name, continent, iso_code, level_4_code, level_3_code, level_2_code, level_1_code, polygons, mysql_polygons, level_5_code) VALUES/" regions.sql -i
 */
include "../phpcoord/phpcoord-2.3.php";
$f = fopen('watson_vice_counties', 'r');
$w = fopen('watson_vice_counties.csv', 'w');
while(($line = fgets($f)) != FALSE){
  switch(strtolower(substr($line, 0, 9))){
    case '  vcname ':
      $line = explode('=', $line);
      $fields['name'] = trim(array_pop($line));
      break;
    case '  vcnumbe':
      $line = explode('=', $line);
      $fields['level_5_code'] = trim(array_pop($line));
      break;
    case '  multipo':
    case '  polygon':
      preg_match_all('/[0-9\.]+\s[0-9\.]+/', $line, $matches);
      foreach($matches[0] as $match){
        $parts = explode(" ", $match);
        $os = new OSRef($parts[0], $parts[1]);
        $latlng = $os->toLatLng();
        $latlng->OSGB36ToWGS84();
        $line = str_replace($match, round($latlng->lng, 5) . ' ' . round($latlng->lat, 5), $line);
      }
      $fields['polygons'] = trim($line);
      fputcsv($w, $fields);
  }
}
fclose($w);
