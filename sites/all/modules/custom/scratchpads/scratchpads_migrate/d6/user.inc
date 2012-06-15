<?php

/**
 * Common mappings for the Drupal 6 node migrations.
 */
class ScratchpadsUser6Migration extends DrupalUser6Migration{

  public function __construct(array $arguments){
    parent::__construct($arguments);
    $this->addFieldMapping('legal_accept')->defaultValue(0);
  }
}