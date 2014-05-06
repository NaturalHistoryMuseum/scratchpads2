<?php

/**
 * Feeds processor plugin. Import data into an existing character project
 */
class FeedsCharacterEditorProcessor extends FeedsProcessor{

  /**
   * Process the result of the parsing stage.
   *
   * @param FeedsSource $source
   *   Source information about this import.
   * @param FeedsParserResult $parser_result
   *   The result of the parsing stage.
   */
  public function process(FeedsSource $source, FeedsParserResult $parser_result){
    // Load the character project
    $project_w = character_editor_wrapper('character_editor_project', $source->importer->processor->config['character_project']);
    $project_model = new CharacterProjectModel($project_w);
    $variables = $project_model->getAvailableVariables();
    module_load_include('inc', 'character_editor', 'includes/CharacterExpressionParser');
    $state = $source->state(FEEDS_PROCESS);
    $mappings = $source->importer->processor->config['mappings'];
    foreach($mappings as $key => $value){
      unset($mappings[$key]);
      $mappings[$value['source']] = $value;
    }
    while(($item = $parser_result->shiftItem()) != FALSE){
      $relation_entity = FALSE;
      foreach($item as $key => $value){
        // We loop through once, simply to find the ID and to load what
        // entity we're linking to.
        if($mappings[$key]['target'] == 'id'){
          $entity_w = character_editor_wrapper($value);
          break;
        }
      }
      if($entity_w){
        foreach($item as $key => $value){
          if(substr($mappings[$key]['target'], 0, 27) == 'character_editor_character:'){
            $character_w = character_editor_wrapper($mappings[$key]['target']);
            $map = character_editor_get_states_map($character_w);
            try{
              if($map){
                switch($character_w->field_char_and_or->value()){
                  case 'AND':
                    $value = "['" . implode("'&'", str_getcsv($value)) . "']";
                    break;
                  case 'OR':
                    $value = "['" . implode("'|'", str_getcsv($value)) . "']";
                    break;
                    $e = new CharacterExpressionParser($value); // $expr is ['stalk'|'body']
                    $e->rename($variables['character_map'], $map);
                    $value = $e->getExpression(); // now $expr is ['123'|'43']
                }
              }else{
                $value = str_getcsv($value);
                $value = $value[0];
              }
            }
            catch(Exception $e){
              $a = $e;
            }
            character_editor_set_character_value($character_w, $entity_w, $value);
          }
        }
      }
    }
  }

  /**
   * We don't do anything here.
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
   * Do nothing here, the entity is effectively saved at the processing stage.
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
        $targets['character_editor_character:' . $character['wrapper']->id->value()] = array(
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