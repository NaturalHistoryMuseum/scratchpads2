<?php

/**
 * Feeds processor plugin. Import data into an existing character project
 */
class FeedsCharacterEditorProcessor extends FeedsProcessor{

  /**
   * 
   */
  public function entityDeleteMultiple($entity_ids){}

  /**
   * Define entity type.
   */
  public function entityType(){
    return 'relation';
  }

  /**
   * Creates a new term in memory and returns it.
   */
  protected function newEntity(FeedsSource $source){}

  /**
   * Saves a term.
   *
   * We de-array parent fields with only one item.
   * This stops leftandright module from freaking out.
   */
  protected function entitySave($relation){}

  /**
   * Override parent::configDefaults().
   */
  public function configDefaults(){
    return array(
      'character_project' => 0,
      'mappings' => array()
    );
  }

  /**
   * Bundle type this processor operates on.
   *
   * Defaults to the entity type for entities that do not define bundles.
   *
   * @return string|NULL
   *   The bundle type this processor operates on, or NULL if it is undefined.
   */
  public function character_project(){
    return $this->config['character_project'];
  }

  /**
   * Overrides parent::configForm().
   */
  public function configForm(&$form_state){
    $projects = db_select('character_editor_project', 'c')->fields('c', array(
      'id',
      'title'
    ))->execute()->fetchAllKeyed();
    $form['character_project'] = array(
      '#type' => 'select',
      '#options' => $projects,
      '#title' => t('Character project'),
      '#required' => TRUE,
      '#default_value' => $this->character_project()
    );
    return $form;
  }

  /**
   * Override setTargetElement to operate on a target item that is a taxonomy term.
   */
  public function setTargetElement(FeedsSource $source, $target_term, $target_element, $value){}

  /**
   * Return available mapping targets.
   */
  public function getMappingTargets(){
    // One target for the term/specimen, and one target for each character.
    $targets = array(
      'id' => array(
        'name' => t('Taxon or Specimen'),
        'description' => t('The taxon name, or the title of the specimen'),
        'optional_unique' => FALSE
      )
    );
    $project_w = character_editor_wrapper('character_editor_project', $this->character_project());
    $tree = character_editor_get_tree($project_w);
    foreach($tree as $character){
      if($character['wrapper']->type->value() != 'group'){
        $targets['character_' . $character['wrapper']->id->value() . '_'] = array(
          'name' => $character['wrapper']->title->value(),
          'description' => $character['wrapper']->field_char_description->value()
        );
      }
    }
    try{
      self::loadMappers();
      $entity_type = 'character_editor_project';
      $character_project = $this->character_project();
      $bundle = 'character_project';
      drupal_alter('feeds_processor_targets', $targets, $entity_type, $bundle);
    }
    catch(Exception $e){
      // Do nothing.
    }
    return $targets;
  }
}

/**
 * Simple class to make loading a row for the importer easier.
 */
class CharacterEditorRow{

  function load($ids, $conditions){
    $project_model = new CharacterProjectModel('character_editor_project', $conditions['character_project_id']);
    // Get the character tree
    $tree = $project_model->getTree();
    $flags = $project_model->getFlags();
    // Get the row values
    $return = array();
    foreach($ids as $key => $id){
      $ids[$key] = new stdClass();
      $ids[$key]->id = $id;
    }
    return $project_model->getRowValues($ids, 0, count($ids));
  }
}