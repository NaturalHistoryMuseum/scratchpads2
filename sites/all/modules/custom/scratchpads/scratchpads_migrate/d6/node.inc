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
class ScratchpadsNode6Migration extends DrupalNode6Migration{

  public function __construct(array $arguments){
    parent::__construct($arguments);
    $fields = $this->sourceFieldList();
    foreach($fields as $delta => $field){
      // Numeric keys are vocabulary fields
      if(is_numeric($delta)){
        $vocabulary = taxonomy_vocabulary_load($delta);
        $vids = variable_get('biological_vids', array());
        // TODO - REMOVE
        $vids = array(
          3
        );
        if($vocabulary){
          if($vocabulary->module != 'forum'){
            // Is this a biological classification
            if(in_array($delta, $vids)){
              $field_name = 'field_taxonomic_name';
            }else{
              $field_name = DrupalFieldTaxonomyMigration::getFieldName($vocabulary->name);
            }
          }else{
            $field_name = 'taxonomy_forums';
          }
          $sourceMigration = scratchpads_migrate_term_machine_name($vocabulary->name);
          // Remove any existing mapping
          $this->removeFieldMapping($field_name);
          $this->addFieldMapping($field_name, $delta)->sourceMigration($sourceMigration);
          $this->addFieldMapping($field_name . ':source_type')->defaultValue('tid');
          $this->addFieldMapping($field_name . ':create_term')->defaultValue(false);
          
        }else{
          // Do nothing with Image Galleries vocab
          $this->addFieldMapping(NULL, $delta);
        }
      }else{ // Other field types
        // Load the field
        $field = field_info_field($delta);
        if($field){
          switch($field['type']){
            case 'image':
            case 'file':
              $this->addFieldMapping('field_media', $delta)->sourceMigration('File');
              $this->addFieldMapping('field_media:file_class')->defaultValue('MigrateFileFid');
              break;
            default:
              $this->addFieldMapping($delta, $delta);
              break;
          }
        }
      }
    }
    // Use english as the default language
    $this->removeFieldMapping('language');
    $this->addFieldMapping('language', 'language')->defaultValue('en');
    $this->removeFieldMapping('is_new');
    $this->addFieldMapping('is_new', NULL)->defaultValue(true);
    $this->addFieldMapping('group_audience', 'group_nid'); // Remap this in prepare row - needs to use OG id
    $this->addFieldMapping('group_content_access', 'og_public')->defaultValue(0);
    $this->addUnmigratedDestinations(array(
      'body:summary',
      'body:format',
      'body:language',
      'field_media:language',
      'field_media:description',
      'field_media:display'
    ));
     // TODO - Turn back on
  //    $this->addSimpleMappings(array(
  //      'nid',
  //      'vid'
  //    ));
  }

  protected function nodeQuery(){
    // Only import nodes which are in the forum vocab
    $query = parent::nodeQuery();
    $query->leftJoin('og_ancestry', 'oga', 'oga.nid = n.nid');
    $query->addField('oga', 'group_nid', 'group_nid');
    $query->leftJoin('og_access_post', 'ogap', 'ogap.nid = n.nid');
    $query->addField('ogap', 'og_public', 'og_public');
    return $query;
  }

  public function prepareRow($row){
    parent::prepareRow($row);
    // Doesn't seem to use the default value, unless it's unset
    if(empty($row->language)){
      unset($row->language);
    }
    // Get the group ID
    if(!empty($row->group_nid)){
      $group_id = db_query('SELECT gid FROM {og} WHERE og.etid = :group_nid', array(
        ':group_nid' => $row->group_nid
      ))->fetchField();
      if($group_id){
        $row->group_nid = $group_id;
      }else{
        watchdog('scratchpads_migrate', 'Could not find group id for group node %group_nid', array(
          '%group_nid' => $row->group_nid
        ), WATCHDOG_ERROR);
      }
    }
    switch($row->og_public){
      case 0:
        $row->og_public = 2; // Private
        break;
      case 1:
        break;
      default:
        $row->og_public = 0; // Group defaults
        break;
    }
  }

  /**
   * Over-write DrupalNode6Migration::handle_type_filefield()
   * We want to use the fid, not filename
   */
  public function handle_type_filefield($fid){
    return $fid;
  }
}
