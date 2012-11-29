<?php

class MigrateDestinationBlock extends MigrateDestination{

  static public function getKeySchema(){
    return array(
      'bid' => array(
        'type' => 'int',
        'description' => 'Block ID.'
      )
    );
  }

  public function __toString(){
    return t('Create blocks');
  }

  public function import(stdClass $object, stdClass $row){
    $delta = db_insert('block_custom')->fields(array(
      'body' => $object->body,
      'info' => $object->info,
      'format' => 'filtered_html'
    ))->execute();
    $query = db_insert('block')->fields(array(
      'visibility',
      'pages',
      'custom',
      'title',
      'module',
      'theme',
      'status',
      'weight',
      'delta',
      'cache',
      'region'
    ));
    $query->values(array(
      'visibility' => $object->visibility,
      'pages' => $object->pages,
      'custom' => 0,
      'title' => $object->title,
      'module' => 'block',
      'theme' => 'scratchpads',
      'status' => $object->status,
      'weight' => 1,
      'delta' => $delta,
      'cache' => DRUPAL_NO_CACHE,
      'region' => ($object->status ? 'sidebar' : '-1') // Add all blocks to the sidebar region - this is the only place for them
    ));
    $query->execute();
    $this->numCreated++;
    return array(
      $delta
    );
  }

  public function fields(){
    return array(
      'pages' => t('Pages'),
      'title' => t('Title'),
      'body' => t("Body"),
      'info' => t("Info"),
      'visibility' => t('Visibility'),
      'status' => t('Status')
    );
  }

  public function rollback(array $ids){
    $delta = $ids['destid1'];
    db_delete('block_custom')->condition('bid', $delta)->execute();
    db_delete('block')->condition('module', 'block')->condition('delta', $delta)->execute();
  }
}