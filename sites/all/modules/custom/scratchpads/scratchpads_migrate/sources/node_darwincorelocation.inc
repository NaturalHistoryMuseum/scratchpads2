<?php

/**
 * Base class for all node migrations - handles commonalities across all
 * supported source Drupal versions.
 *
 * In addition to the arguments supported by DrupalMigration, the following
 * must be passed in the $arguments array:
 *
 * source_type - Drupal 6 content type machine name.
 * destination_type - Drupal 7 content type machine name (bundle).
 *
 * The following optional arguments may be passed:
 *
 * user_migration - Machine name of a user migration, used to establish
 * dependencies and a sourceMigration for the uid mapping.
 * default_uid - Drupal 7 (destination) uid of the user account to use as
 * the default.
 */
class ScratchpadsNodeDarwincorelocationMigration extends ScratchpadsNodeMigration{

  public function __construct(array $arguments){
    // Add manual field mappingss
    $this->manualFieldMapping = array(
      'continentocean' => 'field_continent_or_ocean',
      'islandgroup' => 'field_island_group',
      'island' => 'field_island',
      'stateprovince' => 'field_state_province',
      'county' => 'field_county',
      'locality' => 'field_locality',
      'geodeticdatum' => 'field_geodetic_datum',
      'verbatimcoordinatesystem' => 'field_coordinate_system',
      'georeferenceprotocol' => 'field_georeference_protocol',
      'coordinateuncertainty' => 'field_coordinate_uncertainty',
      'georeferenceremarks' => 'field_georeference_remarks',
      'minelevation' => 'field_min_elevation',
      'maxelevation' => 'field_max_elevation',
      'mindepth' => 'field_min_depth',
      'maxdepth' => 'field_max_depth',
      'location_country' => 'field_country',
      'location_latlong' => 'field_map'
    );
    parent::__construct($arguments);
  }

  public function sourceFieldList(){
    $fields = parent::sourceFieldList();
    $fields['location_country'] = 'text';
    $fields['location_latlong'] = 'integer';
    return $fields;
  }

  protected function nodeQuery(){
    // Only import nodes which are in the forum vocab
    $query = parent::nodeQuery();
    $query->innerJoin('darwincorelocation', 'dl', 'dl.vid = n.vid');
    $query->fields('dl', array(
      'continentocean',
      'islandgroup',
      'island',
      'stateprovince',
      'county',
      'locality',
      'geodeticdatum',
      'verbatimcoordinatesystem',
      'georeferenceprotocol',
      'coordinateuncertainty',
      'georeferenceremarks',
      'minelevation',
      'maxelevation',
      'mindepth',
      'maxdepth'
    ));
    return $query;
  }

  public function prepareRow($row){
    parent::prepareRow($row);
    $term_mappings = array(
      'continentocean' => array(
        1178 => 'Africa',
        1185 => 'Antarctic',
        1179 => 'Asia-Temperate',
        1180 => 'Asia-Tropical',
        1188 => 'Atlantic Ocean',
        1181 => 'Australasia',
        1187 => 'BodyOfWater',
        1177 => 'Continent',
        1186 => 'Europe',
        1189 => 'Indian Ocean',
        1193 => 'North-Central Pacific',
        1183 => 'Northern America',
        1192 => 'Northwestern Pacific',
        1182 => 'Pacific',
        1191 => 'South-Central Pacific',
        1184 => 'Southern America',
        1190 => 'Southwestern Pacific'
      )
    );
    foreach($term_mappings as $delta => $term_mapping){
      if(isset($row->{$delta})){
        if($row->{$delta} == 0){
          unset($row->{$delta});
        }else{
          if(array_key_exists($row->{$delta}, $term_mapping)){
            $row->{$delta} = $term_mapping[$row->{$delta}];
          }else{
            watchdog('scratchpads_migrate', 'Error, %delta term %term doesn\'t exist', array(
              '%delta' => $delta,
              '%term' => $row->{$delta}
            ));
          }
        }
      }
    }
    // Add the location data
    $query = Database::getConnection('default', $this->sourceConnection)->select('location', 'l');
    $query->fields('l', array(
      'country',
      'latitude',
      'longitude'
    ));
    $query->innerJoin('location_instance', 'li', 'li.lid = l.lid');
    $query->condition('li.vid', $row->vid);
    $location = $query->execute()->fetchObject();
    if($location){
      if($location->country){
        $row->location_country = strtoupper($location->country);
      }
      if($location->latitude && $location->longitude){
        $row->location_latlong = $location->latitude . ',' . $location->longitude;
      }
    }
  }
}
