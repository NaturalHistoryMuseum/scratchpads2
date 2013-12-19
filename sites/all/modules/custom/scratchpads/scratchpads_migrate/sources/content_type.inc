<?php

class DrupalContentTypeMigration extends DrupalMigration{

  /**
   * Field name mappings 
   */
  public static function getContentTypeMappings($type = null){
    $mappings = array(
      'biblio' => MIGRATE_ALL, // migrate content only
      'blog' => MIGRATE_ALL, // migrate content only
      'nexus_project' => MIGRATE_NONE, // do not migrate
      'countriesmap' => array(
        'entity_type' => 'node',
        'bundle' => 'spm'
      ),
      'forum' => MIGRATE_ALL, // migrate content only
      'og' => MIGRATE_FIELD, // Handle groups elsewhere, but still migrate any fields
      'group' => MIGRATE_FIELD, // Handle groups elsewhere, but still migrate any fields
      'image' => MIGRATE_NONE, // do not migrate
      'ispecies' => MIGRATE_NONE, // do not migrate
      'darwincorelocation' => array(
        'entity_type' => 'node',
        'bundle' => 'location'
      ),
      'simplenews' => MIGRATE_NONE, // do not migrate
      'nexus_controlled_state' => MIGRATE_NONE, // do not migrate
      'nexus_free_state' => MIGRATE_NONE, // do not migrate
      'nexus_note' => MIGRATE_NONE, // do not migrate
      'page' => MIGRATE_ALL, // migrate using the same entity & bundle type
      'tree' => MIGRATE_NONE, // migrate using the same entity / bundle
      'poll' => MIGRATE_NONE, // migrate using the same entity & bundle type
      'publication' => MIGRATE_NONE,
      'publication_image_caption' => MIGRATE_NONE,
      'publication_section' => MIGRATE_NONE,
      'publication_taxon_description' => MIGRATE_NONE,
      'publication_taxon_section' => MIGRATE_NONE,
      'profile' => MIGRATE_NONE,
      'darwincore' => array(
        'entity_type' => 'node',
        'bundle' => 'specimen_observation'
      ),
      'story' => MIGRATE_NONE, // Not in use
      'spm' => MIGRATE_ALL, // migrate using the same entity & bundle type
      'webform' => MIGRATE_ALL, // migrate content. Settings will be migrated additionally.
      'fileshare' => MIGRATE_NONE
    );
    // Do not migrate term nodes
    $term_nodes = scratchpads_migrate_get_term_vocabularies();
    // NB: Itis fields are handled in DrupalFieldVocabularyItisTermMigration 
    foreach($term_nodes as $term_node){
      $mappings[$term_node] = MIGRATE_NONE; // do not migrate
    }
    // If the user has no itis_term vocabularies, need to manually add itis term
    if(!array_key_exists('itis_term', $mappings)){
      $mappings['itis_term'] = MIGRATE_NONE;
    }
    if($type){
      if(isset($mappings[$type])){
        return $mappings[$type];
      }else{
        return false;
      }
      return $mappings[$type];
    }else{
      return $mappings;
    }
  }

  /**
   * @param array $arguments
   */
  public function __construct(array $arguments){
    parent::__construct($arguments);
    $this->source = new MigrateSourceSQL($this->sourceQuery(), $this->sourceFields, NULL, $this->sourceOptions);
    $this->map = new MigrateSQLMap($this->machineName, array(
      'type' => array(
        'type' => 'varchar',
        'length' => 255
      )
    ), MigrateDestinationContentType::getKeySchema());
    $this->destination = new MigrateDestinationContentType();
    $this->addSimpleMappings(array(
      'type',
      'name',
      'description',
      'title_label',
      'help'
    ));
  }

  /**
   * Query for the basic menu data.
   *
   * @return QueryConditionInterface
   */
  protected function sourceQuery(){
    $query = Database::getConnection('default', $this->sourceConnection)->select('node_type', 'nt');
    $query->fields('nt', array(
      'type',
      'name',
      'description',
      'title_label',
      'help'
    ));
    // BUG FIX: removed, we do want node types even if there's no nodes
    // $query->innerJoin('node', 'n', 'n.type = nt.type');
    $query->groupBy('nt.type');
    $query->condition('nt.module', array(
      'node',
      'geotime' // Nannotax has custom module type
    ), 'IN');
    $query->condition('nt.type', array_keys($this->getContentTypeMappings()), 'NOT IN');
    return $query;
  }

  /**
   * Implementation of Migration::prepareRow().
   */
  public function prepareRow($row){
    if(parent::prepareRow($row) === FALSE){return FALSE;}
    // Check whether or not this content type already exists.
    $node_info = node_entity_info();
    if(isset($node_info['node']['bundles'][$row->type])){
      watchdog('scratchpads_migrate', t('Skipped node %type - already exists.'), array(
        '%type' => $row->type
      ), WATCHDOG_NOTICE);
      return FALSE;
    }
    return TRUE;
  }

  protected function postImport(){
    parent::postImport();
    // Turn on weight settings for all nodes types
    foreach(array_keys(node_type_get_types()) as $node_type){
      $query = db_insert('weight_settings')->fields(array(
        'type' => $node_type,
        'weight_enabled' => 1,
        'weight_range' => 100,
        'menu_weight' => 0,
        'weight_default' => 0
      ))->execute();
    }
  }
}