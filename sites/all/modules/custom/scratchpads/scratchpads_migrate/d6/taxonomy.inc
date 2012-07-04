<?php

/**
 * Handling specific to a Drupal 6 source for taxonomy terms.
 */
class ScratchpadTerm6Migration extends DrupalTerm6Migration{

  public function __construct(array $arguments){
    // Remap destination vocabulary to machine name
    $vocabulary = taxonomy_vocabulary_load($arguments['destination_vocabulary']);
    $arguments['destination_vocabulary'] = $vocabulary->machine_name;
    parent::__construct($arguments);
  }
}
