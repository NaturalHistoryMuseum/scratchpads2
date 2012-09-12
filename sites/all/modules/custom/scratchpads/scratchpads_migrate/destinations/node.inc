<?php

class ScratchpadsMigrateDestinationNode extends MigrateDestinationNode{

  public function fields($migration = NULL){
    $migration = Migration::currentMigration();
    $fields = parent::fields($migration);
    // Add any extra fields which won't be picked up by the scan of field elements
    $fields['path'] = t('Term path');
    $fields['auto_nodetitle_applied'] = t('Auto node title');
    $fields['menu'] = t('Menu');
    return $fields;
  }

  public function import(stdClass $node, stdClass $row){
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
      $node->original->uid = $node->uid;
    }
    // If there's no VID set it to the NID
    if(isset($node->nid) && !isset($node->vid)){
      $node->vid = $node->nid;
    }
    if(!isset($node->{OG_AUDIENCE_FIELD}[LANGUAGE_NONE])){
      unset($node->{OG_AUDIENCE_FIELD});
    }
    $status = parent::import($node, $row);
    // Save all the URL aliases
    if(isset($row->url_alias)){
      foreach($row->url_alias as $url_alias){
        $path = array(
          'alias' => $url_alias,
          'source' => 'node/' . $node->nid,
          'language' => isset($node->language) ? $node->language : LANGUAGE_NONE
        );
      }
    }
    return $status;
  }

  public function rollback(array $key){
    $status = parent::rollback($key);
    // Delete any paths
    path_delete(array(
      'source' => 'node/' . $key['destid1']
    ));
    // Delete the menu item if it exists
    $node = new stdClass();
    $node->nid = $key['destid1'];
    menu_node_delete($node);
    return $status;
  }
}