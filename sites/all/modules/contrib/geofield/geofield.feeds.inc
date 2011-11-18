<?php

/**
 * @file
 * Provides integration with Feeds module (http://drupal.org/project/feeds)
 */

/**
 * Implements hook_feeds_node_processor_targets_alter().
 */
function geofield_feeds_processor_targets_alter(&$targets, $entity_type,
  $bundle_name) {
    foreach (field_info_instances($entity_type, $bundle_name) as
      $name => $instance) {
    $info = field_info_field($name);
    if ($info['type'] == 'geofield') {
      $targets[$info['field_name'] . ':wkt'] = array(
        'name' => t($instance['label'] . ' WKT'),
        'callback' => 'geofield_set_target',
        'real_target' => $info['field_name'],
      );
      $targets[$info['field_name'] . ':geo_type'] = array(
        'name' => t($instance['label'] . ' Type'),
        'callback' => 'geofield_set_target',
        'real_target' => $info['field_name'],
      );
      $targets[$info['field_name'] . ':lat'] = array(
        'name' => t($instance['label'] . ' Latitude'),
        'callback' => 'geofield_set_target',
        'real_target' => $info['field_name'],
      );
      $targets[$info['field_name'] . ':lon'] = array(
        'name' => t($instance['label'] . ' Longitude'),
        'callback' => 'geofield_set_target',
        'real_target' => $info['field_name'],
      );
      $targets[$info['field_name'] . ':left'] = array(
        'name' => t($instance['label'] . ' Left Latitude'),
        'callback' => 'geofield_set_target',
        'real_target' => $info['field_name'],
      );
      $targets[$info['field_name'] . ':top'] = array(
        'name' => t($instance['label'] . ' Top Longitude'),
        'callback' => 'geofield_set_target',
        'real_target' => $info['field_name'],
      );
      $targets[$info['field_name'] . ':right'] = array(
        'name' => t($instance['label'] . ' Right Latitude'),
        'callback' => 'geofield_set_target',
        'real_target' => $info['field_name'],
      );
      $targets[$info['field_name'] . ':bottom'] = array(
        'name' => t($instance['label'] . ' Bottom Longitude'),
        'callback' => 'geofield_set_target',
        'real_target' => $info['field_name'],
      );
      $targets[$info['field_name'] . ':srid'] = array(
        'name' => t($instance['label'] . ' Projection (SRID)'),
        'callback' => 'geofield_set_target',
        'real_target' => $info['field_name'],
      );
    }
  }
}

/**
 * Example callback specified in hook_feeds_processor_targets_alter().
 *
 * @param $source
 *   Field mapper source settings.
 * @param $entity
 *   An entity object, for instance a node object.
 * @param $target
 *   A string identifying the target on the node.
 * @param $value
 *   The value to populate the target with.
 *
 */
function geofield_set_target($source, $entity, $target, $value) {
  list($field_name, $sub_field) = explode(':', $target, 2);
  $entity->{$field_name}['und'][0][$sub_field] = $value;
}

