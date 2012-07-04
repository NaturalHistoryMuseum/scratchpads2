<?php

/**
 * Common mappings for the Drupal 6 node migrations.
 */
class ScratchpadsUser6Migration extends DrupalUser6Migration{

  public function __construct(array $arguments){
    parent::__construct($arguments);
    $this->addFieldMapping('legal_accept')->defaultValue(0);
    // Handle the profile fields in the profile migration
    $this->addUnmigratedDestinations(array(
      'role_names',
      'group_audience',
      'field_user_country',
      'field_user_family_name',
      'field_user_family_name:format',
      'field_user_family_name:language',
      'field_user_given_names',
      'field_user_given_names:format',
      'field_user_given_names:language',
      'field_user_institution',
      'field_user_institution:format',
      'field_user_institution:language',
      'field_user_taxonomic_interest',
      'field_user_taxonomic_interest:format',
      'field_user_taxonomic_interest:language',
      'field_user_title'
    ));
  }
}