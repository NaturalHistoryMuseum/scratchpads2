<?php

/**
 * @file
 * Support for paths in core Drupal objects
 */

class MigratePathEntityHandler extends MigrateDestinationHandler {
  public function __construct() {
    $this->registerTypes(array('entity'));
  }

  public function fields() {
    // TODO: Only surface when path module is enabled, and for appropriate entities
    return array('path' => t('Node: Path alias'));
  }

  public function prepare($entity, stdClass $row) {
    if (isset($entity->path)) {
      $path = $entity->path;
      $entity->path = array();
      $entity->path['alias'] = $path;
    }
  }
}
