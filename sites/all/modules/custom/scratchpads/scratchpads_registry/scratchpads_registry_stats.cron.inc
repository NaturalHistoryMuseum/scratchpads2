<?php
/**
 * Does all the heavy lifting for this module's hook_cron().
 */
use GBIF\TechnicalInstallation;
use GBIF\Organization;

function _scratchpads_registry_stats_cron(){
  if(variable_get('scratchpads_registry_stats_cron_weekly', 0) < (time() - 604800)){
    scratchpads_registry_stats_cron_weekly();
  }
  if(variable_get('scratchpads_registry_stats_cron_daily', 0) < (time() - 86400)){
    scratchpads_registry_stats_cron_daily();
  }
  // Add any additional functions here that need performing more often than once
  // per day.
}

/**
 * Weekly cron
 *
 * Performs functions once a week. Non-essential tasks, like updating nodes/
 * organizations/scratchpads should be placed here.
 *
 * FIXME - Not sure what the "tag" is going to be for a "scratchpad", although
 * it is likely to be "scratchpad". This needs checking.
 */
function scratchpads_registry_stats_cron_weekly(){
  $gbif_registry = GBIFRegistry::singleton();
  // foreach($gbif_registry->getTechnicalInstallations('scratchpad') as
  // $technical_installation){
  foreach($gbif_registry->getTechnicalInstallations() as $technical_installation){
    // Check that the organization that this installation is linked to exists.
    // If it doesn't, we need to create it.
    $row = db_select('field_data_field_uuid', 'f')->fields('f')->condition('field_uuid_value', $technical_installation->getKey())->execute()->fetchAssoc();
    if(!$row){
      // We first need to check if the organization for this TI has been
      // created, if not, we need to create it.
      $hosting_organization = $technical_installation->getHostingOrganization();
      $row2 = db_select('field_data_field_uuid', 'f')->fields('f')->condition('field_uuid_value', $hosting_organization->getKey())->execute()->fetchAssoc();
      $organization_nid = 0;
      if(!$row2){
        // Create the organization.
        $organization_node = scratchpads_registry_stats_create_organization($hosting_organization);
        $organization_nid = $organization_node->nid;
      }else{
        $organization_nid = $row2['entity_id'];
      }
      $technical_installation_node = scratchpads_registry_stats_create_technical_installation($technical_installation, $organization_nid);
    }
  }
  // Now that we have all the TechnicalInstallations, we update the endpoints
  // for each one.
  $results = db_select('node', 'n')->fields('n', array(
    'nid'
  ))->condition('type', 'scratchpad')->execute();
  foreach($results as $row){
    $node = node_load($row->nid);
    $scratchpad = new TechnicalInstallation($node->field_uuid[LANGUAGE_NONE][0]['value']);
    foreach($scratchpad->getEndpoints() as $endpoint){
      print_r($endpoint);
      exit();
    }
  }
}

/**
 * Daily cron
 *
 * Perform daily updates, such as querying a Scratchpad for metadata.
 */
function scratchpads_registry_stats_cron_daily(){
  // FIXME - For debugging - remove the comments on the next line
  // variable_set('scratchpads_registry_stats_cron_daily', time());
}

function scratchpads_registry_stats_create_organization($organization){
  $node = new stdClass();
  $node->title = $organization->getTitle();
  $node->field_uuid = array(
    LANGUAGE_NONE => array(
      array(
        'value' => $organization->getKey()
      )
    )
  );
  $node->field_homepage = array(
    LANGUAGE_NONE => array(
      array(
        'url' => $organization->getHomepage()
      )
    )
  );
  $node->body = array(
    LANGUAGE_NONE => array(
      array(
        'value' => $organization->getDescription()
      )
    )
  );
  $node->type = 'organization';
  // FIXME - Add option to set the UID to a specific value/user.
  $node->uid = 0;
  node_save($node);
  return $node;
}

/**
 * Create a new TI.
 */
function scratchpads_registry_stats_create_technical_installation($technical_installation, $organization_nid){
  $node = new stdClass();
  $node->title = $technical_installation->getTitle();
  $node->field_uuid = array(
    LANGUAGE_NONE => array(
      array(
        'value' => $technical_installation->getKey()
      )
    )
  );
  $node->field_homepage = array(
    LANGUAGE_NONE => array(
      array(
        'url' => $technical_installation->getHomepage()
      )
    )
  );
  $node->body = array(
    LANGUAGE_NONE => array(
      array(
        'value' => $technical_installation->getDescription()
      )
    )
  );
  $node->field_organization = array(
    LANGUAGE_NONE => array(
      array(
        'nid' => $organization_nid
      )
    )
  );
  $node->type = 'scratchpad';
  // FIXME - Add option to set the UID to a specific value/user.
  $node->uid = 0;
  node_save($node);
  return $node;
}