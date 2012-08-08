<?php

class ScratchpadsMigrateDestinationNode extends MigrateDestinationNode{

  public function fields($migration = NULL){
    $migration = Migration::currentMigration();
    $fields = parent::fields($migration);
    // Add any extra fields which won't be picked up by the scan of field elements
    $fields['path'] = t('Term path');
    $fields['auto_nodetitle_applied'] = t('Auto node title');
    return $fields;
  }

  public function import(stdClass $node, stdClass $row){
    $migration = Migration::currentMigration();
    scratchpads_migrate_unset_empty_fields($node);
    $status = parent::import($node, $row);
    // If the term has a path, save it
    if(isset($node->path) && !empty($node->path)){
      $path = array(
        'source' => 'node/' . $node->nid,
        'alias' => $node->path
      );
      path_save($path);
    }
    return $status;
  }

  public function rollback(array $key){
    $status = parent::rollback($key);
    // Delete any paths
    path_delete(array(
      'source' => 'node/' . $key['destid1']
    ));
    return $status;
  }
}