<?php

class ScratchpadsMigrateDestinationNode extends MigrateDestinationNode{

  static public function getKeySchema(){
    return array(
      'vid' => array(
        'type' => 'int',
        'unsigned' => TRUE,
        'description' => 'Verion ID of destination node'
      )
    );
  }

  public function fields($migration = NULL){
    $migration = Migration::currentMigration();
    $fields = parent::fields($migration);
    // Add any extra fields which won't be picked up by the scan of field elements
    $fields['path'] = t('Term path');
    $fields['auto_nodetitle_applied'] = t('Auto node title');
    $fields['menu'] = t('Menu');
    $fields['weight_weight'] = t('Weight');
    $fields['vid'] = t('VID');
    return $fields;
  }

  public function prepare($entity, stdClass $source_row){
    parent::prepare($entity, $source_row);
  }

  /**
   * Implement preImport()
   *
   * - Ensure that autotagging won't take place
   */
  public function preImport(){
    $node_types = array_keys(node_type_get_types());
    foreach($node_types as $type){
      $autotag = variable_get('autotag_vids_' . $type, NULL);
      if($autotag !== NULL){
        $GLOBALS['conf']['_autotags_vids_' . $type] = $autotag;
        unset($GLOBALS['conf']['autotags_vids_' . $type]);
      }
    }
  }

  /**
   * Implement postImport()
   *
   * - Re-enable autotagging
   */
  public function postImport(){
    $node_types = array_keys(node_type_get_types());
    foreach($node_types as $type){
      $autotag = variable_get('_autotag_vids_' . $type, NULL);
      if($autotag !== NULL){
        $GLOBALS['conf']['autotags_vids_' . $type] = $autotag;
        unset($GLOBALS['conf']['_autotags_vids_' . $type]);
      }
    }
  }

  public function import(stdClass $node, stdClass $row){
    // Turn off node comment statistics - see comment_node_insert()
    global $conf;
    $conf['comment_maintain_node_statistics'] = false;
    // Always turn on node revisions, if there's an old revision we want to import
    $conf['node_options_' . $this->bundle] = array(
      'status',
      'promote',
      'revision'
    );
    // Hackery - add the missing TID for forum topics
    if(property_exists($node, 'taxonomy_forums') && empty($node->taxonomy_forums[0]) && isset($row->tid)){
      $node->taxonomy_forums[0] = $row->tid;
    }
    // More hackery, can't be arse to find out why this throws a notice
    if(property_exists($node, 'body') && !empty($node->body) && isset($node->body['arguments']['summary']) && !isset($node->body['arguments']['summary'][0])){
      unset($node->body['arguments']['summary']);
    }
    $migration = Migration::currentMigration();
    scratchpads_migrate_unset_empty_fields($node);
    // Hackery to allow image gallery nodes to save without an error
    if(!isset($node->original->uid)){
      if(!isset($node->original)){
        $node->original = new stdClass();
      }
      $node->original->uid = $node->uid;
    }
    // If there's no VID set it to the NID
    if(isset($node->nid) && !isset($node->vid)){
      $node->vid = $node->nid;
    }
    if(!isset($node->{OG_AUDIENCE_FIELD}[LANGUAGE_NONE])){
      unset($node->{OG_AUDIENCE_FIELD});
    }
    // We cannot call parent::import() at this point, cos it checks nid against migrate_map_destid1
    // But we use VID, so change the checking to VID
    //
    // Updating previously-migrated content? Or creating actual content for stub nodes.
    $migration = Migration::currentMigration();
    $node_replace_stub = FALSE;
    $revision_only = FALSE;
    if(isset($row->migrate_map_destid1)){
      // Make sure is_new is off
      $node->is_new = FALSE;
      if(isset($node->vid)){
        if($node->vid != $row->migrate_map_destid1){throw new MigrateException(t("Incoming vid !vid and map destination vid !destid1 don't match", array(
            '!vid' => $node->nid,
            '!destid1' => $row->migrate_map_destid1
          )));}
      }else{
        $node->vid = $row->migrate_map_destid1;
      }
      // Get the existing nid, tnid so updates don't generate notices
      $nrquery = db_select('node_revision', 'nr');
      $nrquery->addJoin('INNER', 'node', 'n', 'n.nid = nr.nid');
      $nrquery->fields('n', array(
        'nid',
        'tnid'
      ))->condition('nr.vid', $node->vid);
      $values = $nrquery->execute()->fetchAssoc();
      if(empty($values)){throw new MigrateException(t("Incoming node VID !vid no longer exists", array(
          '!vid' => $node->vid
        )));}
      $node->nid = $values['nid'];
      if(empty($row->tnid)){
        $node->tnid = $values['tnid'];
      }
    }else{
      // Do we already have this node? IF we do save a revision of it
      if($original = entity_load_unchanged('node', $node->nid)){
        $is_stub = preg_match('/^Stub for \d+$/', trim($original->title));
        $is_stub = $is_stub || ($original->title == '[node:field-institution-code] - [node:field-collection-code] - [node:field-catalogue-number]');
        if($is_stub){
          if($node->vid != $original->vid){
            // We're updating a stub, but this is not the active VID
            unset($node->is_new);
            $node->revision = 1;
            $revision_only = TRUE;
          }else{
            unset($node->is_new);
            unset($node->revision);
            $node_replace_stub = TRUE;
          }
        }else{
          unset($node->is_new);
          $node->revision = 1;
          $node->revision_uid = $node->uid;
        }
      }
    }
    // The rest of this function is a direct copy of MigrateDestinationNode:import()
    if($migration->getSystemOfRecord() == Migration::DESTINATION){
      if(!isset($node->nid)){throw new MigrateException(t('System-of-record is DESTINATION, but no destination nid provided'));}
      $old_node = node_load($node->nid);
      if(empty($old_node)){throw new MigrateException(t('System-of-record is DESTINATION, but node !nid does not exist', array(
          '!nid' => $node->nid
        )));}
      if(!isset($node->created)){
        $node->created = $old_node->created;
      }
      if(!isset($node->vid)){
        $node->vid = $old_node->vid;
      }
      if(!isset($node->status)){
        $node->status = $old_node->status;
      }
      if(!isset($node->uid)){
        $node->uid = $old_node->uid;
      }
    }elseif(!isset($node->type)){
      // Default the type to our designated destination bundle (by doing this
      // conditionally, we permit some flexibility in terms of implementing
      // migrations which can affect more than one type).
      $node->type = $this->bundle;
    }
    // Set some required properties.
    if($migration->getSystemOfRecord() == Migration::SOURCE){
      if(!isset($node->language)){
        $node->language = $this->language;
      }
      // Apply defaults, allow standard node prepare hooks to fire.
      // node_object_prepare() will blow these away, so save them here and
      // stuff them in later if need be.
      if(isset($node->created)){
        $created = MigrationBase::timestamp($node->created);
      }else{
        // To keep node_object_prepare() from choking
        $node->created = REQUEST_TIME;
      }
      if(isset($node->changed)){
        $changed = MigrationBase::timestamp($node->changed);
      }
      if(isset($node->uid)){
        $uid = $node->uid;
      }
      node_object_prepare($node);
      if(isset($created)){
        $node->created = $created;
      }
      // No point to resetting $node->changed here, node_save() will overwrite it
      if(isset($uid)){
        $node->uid = $uid;
      }
    }
    // Invoke migration prepare handlers
    $this->prepare($node, $row);
    // Trying to update an existing node
    if($migration->getSystemOfRecord() == Migration::DESTINATION){
      // Incoming data overrides existing data, so only copy non-existent fields
      foreach($old_node as $field => $value){
        // An explicit NULL in the source data means to wipe to old value (i.e.,
        // don't copy it over from $old_node)
        if(property_exists($node, $field) && $node->$field === NULL){
          // Ignore this field
        }elseif(!isset($node->$field)){
          $node->$field = $old_node->$field;
        }
      }
    }
    if(isset($node->nid) && !(isset($node->is_new) && $node->is_new)){
      $updating = TRUE;
    }else{
      $updating = FALSE;
    }
    if($node_replace_stub || isset($row->migrate_map_destid1)){
      // And we don't want to create a revision if we're just updating a stub
      unset($node->revision);
    }
    migrate_instrument_start('node_save');
    scratchpads_migrate_node_save($node, $revision_only);
    migrate_instrument_stop('node_save');
    if(isset($node->nid)){
      if($updating){
        $this->numUpdated++;
      }else{
        $this->numCreated++;
      }
      // Unfortunately, http://drupal.org/node/722688 was not accepted, so fix
      // the changed timestamp
      if(isset($changed)){
        db_update('node')->fields(array(
          'changed' => $changed
        ))->condition('nid', $node->nid)->execute();
        $node->changed = $changed;
      }
      // Potentially fix uid and timestamp in node_revisions.
      $query = db_update('node_revision')->condition('vid', $node->vid);
      if(isset($changed)){
        $fields['timestamp'] = $changed;
      }
      $revision_uid = isset($node->revision_uid) ? $node->revision_uid : $node->uid;
      if($revision_uid != $GLOBALS['user']->uid){
        $fields['uid'] = $revision_uid;
      }
      if(!empty($fields)){
        // We actually have something to update.
        $query->fields($fields);
        $query->execute();
        if(isset($changed)){
          $node->timestamp = $changed;
        }
      }
      // Save Organic groups memberships associated with this node
      if(!empty($row->og_memberships)){
        $associated_groups = og_get_group_ids('node', array(
          $node->nid
        ));
        if(isset($associated_groups[$node->nid])){
          $og_group_id = $associated_groups[$node->nid];
          foreach($row->og_memberships as $og_membership_info){
            if(!og_get_group_membership($og_group_id, $og_membership_info['type'], $og_membership_info['id'])){
              $og_membership = og_membership_create($og_group_id, $og_membership_info['type'], $og_membership_info['id']);
              if($og_membership){
                $og_membership->save();
              }
            }
          }
        }
      }
      // Prepare return value
      $return = array(
        $node->vid
      );
    }else{
      $return = FALSE;
    }
    $this->complete($node, $row);
    return $return;
  }

  public function rollback(array $key){
    $vid = $key['destid1'];
    $node = node_load(null, $vid);
    if($node){
      $key = array(
        'destid1' => $node->nid
      );
      $status = parent::rollback($key);
      // Delete any paths
      path_delete(array(
        'source' => 'node/' . $node->nid
      ));
      // Delete the menu item if it exists
      menu_node_delete($node);
    }
    return $status;
  }

  /**
   * Delete a batch of nodes at once.
   *
   * @param $vids
   *  Array of node VIDs to be deleted.
   */
  public function bulkRollback(array $vids){
    // Get the nids for thse nodes
    $result = db_query('SELECT n.nid FROM {node} n WHERE n.vid IN (:vids)', array(
      ':vids' => $vids
    ));
    parent::bulkRollback($result->fetchCol());
  }
}