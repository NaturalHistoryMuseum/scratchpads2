<?php

/**
 * Base class for all node migrations - handles commonalities across all
 * supported source Drupal versions.
 *
 * In addition to the arguments supported by DrupalMigration, the following
 * must be passed in the $arguments array:
 *
 * source_type - Drupal 6 content type machine name.
 * destination_type - Drupal 7 content type machine name (bundle).
 *
 * The following optional arguments may be passed:
 *
 * user_migration - Machine name of a user migration, used to establish
 * dependencies and a sourceMigration for the uid mapping.
 * default_uid - Drupal 7 (destination) uid of the user account to use as
 * the default.
 */
class ScratchpadsNodeOrganicGroupMigration extends ScratchpadsNodeMigration{

  public function __construct(array $arguments){
    parent::__construct($arguments);
    $this->addFieldMapping('group_group')->defaultValue(true);
    // Remove the node group fields - not needed for the actual group node
    $this->removeFieldMapping('group_audience');
    $this->removeFieldMapping('group_content_access');
    $this->removeFieldMapping('body');
    $this->addFieldMapping('body', 'og_description')->defaultValue(true);
    $this->addUnmigratedSources(array(
      'body'
    ));
  }

  protected function nodeQuery(){
    // Only import nodes which are in the forum vocab
    $query = Database::getConnection('default', $this->sourceConnection)->select('node', 'n')->fields('n', array(
      'nid',
      'vid',
      'language',
      'title',
      'uid',
      'status',
      'created',
      'changed',
      'comment',
      'promote',
      'moderate',
      'sticky'
    ))->condition('type', array(
      'og',
      'group'
    ), 'IN'); // Organic groups seems to have changed it's node type at some point
    $query->innerJoin('node_revisions', 'nr', 'n.vid=nr.vid');
    $query->fields('nr', array(
      'body',
      'teaser',
      'format'
    ));
    $query->innerJoin('og', 'og', 'og.nid = n.nid');
    $query->fields('og', array(
      'og_description'
    ));
    return $query;
  }

  protected function countQuery(){
    $query = Database::getConnection('default', $this->sourceConnection)->select('node', 'n');
    $query->addExpression('COUNT(DISTINCT(n.nid))');
    $query->innerJoin('og', 'og', 'og.nid = n.nid');
    $query->condition('type', array(
      'og',
      'group'
    ), 'IN');
    return $query;
  }

  /**
   * Enable the OG module
   */
  protected function preImport(){
    parent::preImport();
    if(!module_exists('og')){
      module_enable(array(
        'og'
      ));
    }
  }
}
