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
class ScratchpadsNodeBiblioMigration extends ScratchpadsNodeMigration{

  public function __construct(array $arguments){
    parent::__construct($arguments);
    $this->destination = new ScratchpadsMigrateDestinationNodeBiblio($this->destinationType);
    $this->addUnmigratedDestinations(array(
      'vid',
      'biblio_other_author_affiliations'
    ));
  }

  public function sourceFieldList(){
    $fields = parent::sourceFieldList();
    $query = Database::getConnection('default', $this->sourceConnection)->select('biblio_fields', 'bf')->fields('bf', array(
      'name',
      'type'
    ));
    $fields += $query->execute()->fetchAllKeyed();
    $fields['biblio_type'] = 'integer';
    return $fields;
  }

  protected function nodeQuery(){
    // Only import nodes which are in the forum vocab
    $query = parent::nodeQuery();
    $query->innerJoin('biblio', 'b', 'b.vid = n.vid');
    $query->fields('b');
    return $query;
  }

  public function prepareRow($row){
    parent::prepareRow($row);
    // Add the keywords
    $query = Database::getConnection('default', $this->sourceConnection)->select('biblio_keyword_data', 'bkd')->fields('bkd', array(
      'word'
    ));
    $query->innerJoin('biblio_keyword', 'bk', 'bk.kid=bkd.kid');
    $query->condition('bk.nid', $row->nid);
    $row->biblio_keywords = $query->execute()->fetchCol();
  }

  protected function buildStubNode($migration, $nid){
  	$node = parent::buildStubNode($migration, $nid);
  	$node->biblio_type = 100;
  	return $node;
  }
}
