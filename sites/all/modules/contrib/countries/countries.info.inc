<?php

/**
 * Implements hook_entity_property_info_alter().
 *
 * Add Continent name property.
 */
function countries_entity_property_info_alter(&$info) {
  $info['country']['properties']['continent_name'] = array(
    'label' => t('Continent name'),
    'type' => 'text',
    'getter callback' => 'countries_get_properties',
  );
}
