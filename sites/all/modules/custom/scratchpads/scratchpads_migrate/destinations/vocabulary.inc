<?php

/**
 * Destination class implementing migration into vocabularies.
 */
class MigrateDestinationVocabulary extends MigrateDestinationEntity{

  static public function getKeySchema(){
    return array(
      'vid' => array(
        'type' => 'int',
        'description' => 'ID of destination vocabulary'
      )
    );
  }

  public function __construct(array $options = array()){
    parent::__construct('taxonomy_vocabulary', 'taxonomy_vocabulary', $options);
  }

  /**
   * Returns a list of fields available to be mapped for this vocabulary (bundle)
   */
  public function fields(){
    return array(
      'vid' => t('Vocabulary: Existing vocabulary ID'),
      'name' => t('Vocabulary: Name'),
      'description' => t('Vocabulary: Description'),
      'hierarchy' => t('Vocabulary: Hierarchy level'),
      'weight' => t('Term: Weight')
    );
  }

  /**
   * Import a single vocabulary.
   */
  public function import(stdClass $vocabulary, stdClass $row){
    $status = false;
    $this->prepare($vocabulary, $row);
    switch($row->module){
      case 'forum':
        // The forum classifcation should already exist if the forum module has been activated
        if(!module_exists('forum')){
          throw new MigrateException(t('Forum module is not active'));
          return false;
        }
        $status = $this->updateVocabulary('forums', $row->vid);
        variable_set('forum_nav_vocabulary', $row->vid);
        break;
      case 'scratchpadify':
        switch($vocabulary->name){
          case 'Keywords':
            $machine_name = 'image_keywords';
            break;
          case 'Imaging Technique':
            $machine_name = 'imaging_technique';
            break;
          case 'Preparation Technique':
            $machine_name = 'preparation_technique';
            break;
        }
        if(isset($machine_name)){
          $status = $this->updateVocabulary($machine_name, $row->vid);
        }else{
          throw new MigrateException(t('Unknown scratchpadify vocabulary %name', array(
            '%name' => $vocabulary->name
          )));
          return false;
        }
        break;
      // If module is taxonomy, it's been created by user & just needs to bve created on the new site  
      case 'taxonomy':
        $vocabulary->machine_name = $this->machineName($vocabulary->name);
        // Ensure the vocabulary doesn't already exist
        $result = db_query('SELECT COUNT(*) AS machine_name_exists FROM {taxonomy_vocabulary} WHERE machine_name = :machine_name', array(
          ':machine_name' => $vocabulary->machine_name
        ))->fetch();
        if($result->machine_name_exists){throw new MigrateException(t('Vocabulary %name already exists.', array(
            '%name' => $vocabulary->machine_name
          )));}
        // Is this a biological classifcation?
        if(!empty($row->classification_type)){
          itis_term_taxonomy_vocabulary_biological_classification($vocabulary, $row->classification_type);
        }
        // We need to insert a dummy row in to the taxonomy_vocabulary table first
        // as we are specifying the VID of the taxonomy.
        $vocabulary_array = (array)$vocabulary;
        drupal_write_record('taxonomy_vocabulary', $vocabulary_array);
        if(taxonomy_vocabulary_save($vocabulary)){
          $status = SAVED_NEW;
        }
        break;
    }
    if($status){
      if($status == SAVED_UPDATED){
        $this->numUpdated++;
        $op = 'Updated';
      }else{
        $this->numCreated++;
        $op = 'Created';
      }
      watchdog('scratchpads_migrate', t('!op vocabulary %name'), array(
        '!op' => $op,
        '%name' => $vocabulary->name
      ), WATCHDOG_INFO);
      return array(
        $vocabulary->vid
      );
    }
  }

  /**
   * Make a machine name
   */
  public function machineName($name){
    return preg_replace('/[^0-9a-z_]/', '', preg_replace('/\ +/', '_', strtolower($name)));
  }

  /**
   * Update a vocabulary using the machine name as key
   */
  public function updateVocabulary($machine_name, $vid){
    // Load the forum vocabulary
    $vocabulary = taxonomy_vocabulary_machine_name_load($machine_name);
    if(!$vocabulary){
      throw new MigrateException(t('Could not load the %name vocabulary', array(
        '%name' => $machine_name
      )));
      return false;
    }
    try{
      db_update('taxonomy_vocabulary')->fields(array(
        'vid' => $vid
      ))->condition('machine_name', $vocabulary->machine_name)->execute();
      return SAVED_UPDATED;
    }
    catch(PDOException $e){
      throw new MigrateException($e->getMessage());
      return false;
    }
  }

  /**
   * Delete a migrated vocabulary.
   */
  public function rollback(array $key){
    // Only delete taxonomy vocabularies
    $vocabulary = taxonomy_vocabulary_load($key['destid1']);
    if($vocabulary && $vocabulary->module == 'taxonomy'){
      taxonomy_vocabulary_delete($key['destid1']);
    }
  }
}
