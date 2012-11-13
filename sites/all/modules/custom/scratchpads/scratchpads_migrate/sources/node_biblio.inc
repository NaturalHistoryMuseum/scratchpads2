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
      'vid'
    ));
    $source_fields = $this->sourceFieldList();
    if(!array_key_exists('biblio_other_author_affiliations', $source_fields)){
      $this->addUnmigratedDestinations(array(
        'biblio_other_author_affiliations'
      ));
    }
  }

  public function sourceFieldList(){
    $fields = parent::sourceFieldList();
    $query = Database::getConnection('default', $this->sourceConnection)->select('biblio_fields', 'bf')->fields('bf', array(
      'name',
      'type'
    ));
    $fields += $query->execute()->fetchAllKeyed();
    $fields['biblio_type'] = 'integer';
    $fields['biblio_contributors'] = 'text';
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
    $query->condition('bk.vid', $row->vid);
    $row->biblio_keywords = $query->execute()->fetchCol();
    // Add the authors
    $query = Database::getConnection('default', $this->sourceConnection)->select('biblio_contributor_data', 'bcd')->fields('bcd', array(
      'name'
    ));
    $query->innerJoin('biblio_contributor', 'bc', 'bc.cid=bcd.cid');
    $query->fields('bc', array(
      'auth_category',
      'auth_type',
      'rank'
    ));
    $query->condition('bc.vid', $row->vid);
    $result = $query->execute();
    while($record = $result->fetchAssoc()){
      $row->biblio_contributors[] = $record;
    }
    // Biblio abstracts are filtered html
    if(isset($row->biblio_abst_e)){
      $row->biblio_abst_e = array(
        'value' => $row->biblio_abst_e,
        'format' => 'filtered_html'
      );
    }
  }

  protected function buildStubNode($migration, $nid){
    $node = parent::buildStubNode($migration, $nid);
    $node->biblio_type = 100;
    return $node;
  }
}
