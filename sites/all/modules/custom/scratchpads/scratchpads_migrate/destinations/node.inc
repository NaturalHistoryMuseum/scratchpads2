<?php

class ScratchpadsMigrateDestinationNode extends MigrateDestinationNode{

  public function fields($migration = NULL){
    $migration = Migration::currentMigration();
    $fields = parent::fields($migration);
    // Add any extra fields which won't be picked up by the scan of field elements
    $fields['path'] = t('Term path');
    $fields['auto_nodetitle_applied'] = t('Auto node title');
    $fields['menu'] = t('Menu');
    $fields['something'] = t('Menu');
    return $fields;
  }

  public function import(stdClass $node, stdClass $row){
    $migration = Migration::currentMigration();
    scratchpads_migrate_unset_empty_fields($node);
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