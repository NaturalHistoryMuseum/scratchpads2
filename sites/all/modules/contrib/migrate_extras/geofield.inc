<?php
/**
 * Primary value passed to this field must be the geometry type of the geofield: Point, LineString, Polygon
 *
 * Arguments are used to specify all the other values:
 *   'wkt' - Well Known Text
 *   'lat' - Latitude.
 *   'lon' - Longitude
 *   'top' - Top Latitude
 *   'bottom' - Bottom Latitude
 *   'right' - Right Longitude
 *   'left' - Left Longitude
 *
 * Add the source field mappings to the argument array then add null mappings to
 * avoid having fields flagged as as unmapped:
 * @code
 *   $geo_arguments = array(
 *     'lat' => array('source_field' => 'source_field_latidute'),
 *     'lon' => array('source_field' => 'source_field_longitude'),
 *   );
 *   // The geometry type should be passed in as the primary value.
 *   $this->addFieldMapping('field_geofield_dest', 'field_source_geotype')
 *        ->arguments($geo_arguments);
 *   // Since the excerpt is mapped via an argument, add a null mapping so it's
 *   // not flagged as unmapped.
 *   $this->addFieldMapping(NULL, 'source_field_latitude');
 *   $this->addFieldMapping(NULL, 'source_field_longitude');
 * @endcode
 */
class MigrateGeoFieldHandler extends MigrateFieldHandler {
  public function __construct() {
    $this->registerTypes(array('geofield'));
  }

  public function prepare($entity, array $field_info, array $instance, array $values) {
    $arguments = array();

    if (isset($values['arguments'])) {
      $arguments = array_filter($values['arguments']);
      unset($values['arguments']);
    }
    $language = $this->getFieldLanguage($entity, $field_info, $arguments);

    // Setup the standard Field API array for saving.
    $delta = 0;
    foreach ($values as $value) {
      $return[$language][$delta] = array('geo_type' => $value) + array_intersect_key($arguments, $field_info['columns']);
      $delta++;
    }

    return isset($return) ? $return : NULL;
  }
}
