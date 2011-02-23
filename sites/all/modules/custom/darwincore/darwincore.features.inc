<?php

/**
 * Implementation of hook_node_info().
 */
function darwincore_node_info() {
  $items = array(
    'location' => array(
      'name' => t('Location'),
      'base' => 'node_content',
      'description' => t('A location which conforms to Darwincore 1.2.1, and can be associated with multiple specimens'),
      'has_title' => '1',
      'title_label' => t('Location identifier'),
      'help' => '',
    ),
    'specimen_observation' => array(
      'name' => t('Specimen/Observation'),
      'base' => 'node_content',
      'description' => t('A Specimen content type based upon Darwincore 1.2.1'),
      'has_title' => '1',
      'title_label' => t('Specimen identifier'),
      'help' => '',
    ),
  );
  return $items;
}
