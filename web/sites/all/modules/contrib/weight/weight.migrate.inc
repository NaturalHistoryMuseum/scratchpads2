<?php

/**
 * @file
 * Migrate support for weight.
 */

/**
 * Implements hook_migrate_api().
 */
function weight_migrate_api() {
  return array(
    'api' => 2,
    'destination handlers' => array(
      'WeightNodeHandler',
    ),
  );
}

class WeightNodeHandler extends MigrateDestinationHandler {
  public function __construct() {
    $this->registerTypes(array('node'));
  }

  public function fields($entity_type, $bundle) {
    $settings = _weight_get_settings($bundle);
    if (!empty($settings) && $settings['enabled']) {
      return array(
        'weight' => t('The node weight ranging between @min and @max.', array(
          '@max' => $settings['range'],
          '@min' => (int)$settings['range']*-1,
        )),
      );
    }
  }

  public function prepare($entity, stdClass $row) {
    $settings = _weight_get_settings($entity->type);
    if (!empty($settings) && $settings['enabled']) {
      // Use the mapped entity value to populate the real value
      if (isset($entity->weight)) {
        $entity->weight_weight = $entity->weight;
        unset($entity->weight);
      }
      else {
        $entity->weight_weight = $settings['default'];
      }
    }
  }
}
