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
      'countriesmap' => MIGRATE_NONE, // do not migrate
      'forum' => MIGRATE_ALL, // migrate content only
      'glossary' => MIGRATE_NONE, // Migrate everything
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
      'webform' => MIGRATE_NONE
    );
    // Do not migrate term nodes
    $term_nodes = scratchpads_migrate_get_term_vocabularies();
    foreach($term_nodes as $term_node){
      $mappings[$term_node] = MIGRATE_NONE; // do not migrate
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
    $query->innerJoin('node', 'n', 'n.type = nt.type');
    $query->groupBy('nt.type');
    $query->condition('nt.module', 'node');
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
}