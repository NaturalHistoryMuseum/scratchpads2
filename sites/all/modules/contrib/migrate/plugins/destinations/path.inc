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
    if (module_exists('path')) {
      return array('path' => t('Node: Path alias'));
    }
    else {
      return array();
    }
  }

  public function prepare($entity, stdClass $row) {
    if (module_exists('path') && isset($entity->path)) {
      $path = $entity->path;
      $entity->path = array();
      $entity->path['alias'] = $path;
    }
  }
}
