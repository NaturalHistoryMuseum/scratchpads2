<?php
/**
 * Implementation of hook_node_info
 */
function darwincore_node_info(){
  return array(
    'specimen_observation' => array(
      'name' => t('Specimen/Observation'),
      'base' => 'specimen_observation',
      'description' => t('Specimen/Observation data type for storing information about a museum specimen, or field observation.'),
      'help' => '',//darwincore_help('node/add/specimen_observation', array()),
      'has_title' => FALSE,
      'has_body' => FALSE,
      'locked' => TRUE
    ),
    'darwincorelocation' => array(
      'name' => t('Location'),
      'base' => 'darwincorelocation',
      'description' => t('Location data type for storing localities associated with a Specimen/Observation'),
      'help' => '',//darwincore_help('node/add/darwincorelocation', array()),
      'has_title' => FALSE,
      'has_body' => FALSE,
      'locked' => TRUE    
    )
  );
}