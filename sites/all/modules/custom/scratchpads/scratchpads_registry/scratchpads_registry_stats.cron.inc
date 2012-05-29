<?php

/**
 * Does all the heavy lifting for this module's hook_cron().
 */
function _scratchpads_registry_stats_cron(){
  // Increase the timeout.
  ini_set('max_execution_time', 3600);
  if(variable_get('scratchpads_registry_stats_cron_weekly', 0) < (time() - 604800)){
    scratchpads_registry_stats_cron_weekly();
    // If we've carried out the weekly cron, we don't do the daily or hourly
    // stuff.
    return;
  }
  if(variable_get('scratchpads_registry_stats_cron_daily', 0) < (time() - 86400)){
    scratchpads_registry_stats_cron_daily();
    // If we've carried out the daily cron, we don't do the hourly stuff.
    return;
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
  variable_set('scratchpads_registry_stats_cron_weekly', time());
}

/**
 * Daily cron
 *
 * Perform daily updates, such as querying a Scratchpad for metadata.
 */
function scratchpads_registry_stats_cron_daily(){
  $results = db_select('node', 'n')->condition('type', 'scratchpad')->fields('n', array(
    'nid'
  ))->execute();
  $context = stream_context_create(array(
    'http' => array(
      'timeout' => 3
    )
  ));
  foreach($results as $row){
    $node = node_load($row->nid);
    if(@isset($node->field_homepage[LANGUAGE_NONE][0]['url']) && $node->field_homepage[LANGUAGE_NONE][0]['url']){
      $url = $node->field_homepage[LANGUAGE_NONE][0]['url'];
      if(!strpos($url, ':', 6) && substr($url, -1) == '/'){
        // Get the metadata.
        $data = @file_get_contents($url . 'metadata.json', NULL, $context);
        if($data && $data = json_decode($data, TRUE)){
          scratchpads_registry_stats_create_statistics_nodes($data, $row->nid);
        }
      }
    }
  }
  variable_set('scratchpads_registry_stats_cron_daily', time());
}

/**
 * Create the statistics nodes.
 */
function scratchpads_registry_stats_create_statistics_nodes($data, $scratchpad_nid){
  foreach($data['entities'] as $type => $entity_data){
    if($type == 'user'){
      $node = new stdClass();
      $node->field_scratchpad = array(
        LANGUAGE_NONE => array(
          array(
            'nid' => $scratchpad_nid
          )
        )
      );
      $node->title = "$scratchpad_nid:$type:" . time();
      $node->type = 'user_data';
      $node->field_active_in_last_7_days = array(
        LANGUAGE_NONE => array(
          array(
            'value' => $entity_data['active_in_last_7_days']
          )
        )
      );
      $node->field_active_in_last_28_days = array(
        LANGUAGE_NONE => array(
          array(
            'value' => $entity_data['active_in_last_28_days']
          )
        )
      );
      $node->field_last_login = array(
        LANGUAGE_NONE => array(
          array(
            'value' => $entity_data['last_login']
          )
        )
      );
      $node->uid = variable_get('scratchpads_registry_stats_user', 1);
      node_save($node);
    }
    $node = new stdClass();
    $node->field_scratchpad = array(
      LANGUAGE_NONE => array(
        array(
          'nid' => $scratchpad_nid
        )
      )
    );
    $node->field_machine_name = array(
      LANGUAGE_NONE => array(
        array(
          'value' => $type
        )
      )
    );
    $node->field_label = array(
      LANGUAGE_NONE => array(
        array(
          'value' => $entity_data['label']
        )
      )
    );
    $node->field_quantity = array(
      LANGUAGE_NONE => array(
        array(
          'value' => $entity_data['count']
        )
      )
    );
    $node->title = "$scratchpad_nid:$type:" . time();
    $node->type = 'entity_data';
    $node->uid = variable_get('scratchpads_registry_stats_user', 1);
    node_save($node);
  }
}

/**
 * Create a new organization.
 */
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
  $node->uid = variable_get('scratchpads_registry_stats_user', 1);
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
  $node->uid = variable_get('scratchpads_registry_stats_user', 1);
  node_save($node);
  return $node;
}