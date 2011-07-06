<?php

/**
 * UI controller.
 */
class CharacterEditorProjectTypeUIController extends EntityDefaultUIController{
	
  /**
   * Overrides hook_menu() defaults.
   */
  public function hook_menu(){
    $items = parent::hook_menu();
    $items[$this->path]['description'] = t('Manage projects, groups & characters.');
    $items[$this->path]['title'] = t('Character editor');
    
    return $items;
  }
	
}

/**
 * UI controller.
 */
class CharacterEditorProjectUIController extends EntityDefaultUIController{
  
  /**
   * Builds the entity overview form.
   */
  public function overviewForm($form, &$form_state){
    $this->path = request_path();
    // By default just show a simple overview for all entities.
    $form['table'] = $this->overviewTable();
    return $form;
  }

  public function overviewTable($conditions = array()){
    $entities = entity_load($this->entityType);
    ksort($entities);
    $rows = array();
    foreach($entities as $id => $entity){
      $additional_cols = array(
        l('List characters', CHARACTER_EDITOR_ADMIN_PATH . '/characters/' . $entity->name)
      );
      $rows[] = $this->overviewTableRow($conditions, $id, $entity, $additional_cols);
    }
    // Assemble the right table header.
    $header = array(
      t('Project'),
      t('Characters')
    );
    if(!empty($this->entityInfo['exportable'])){
      $header[] = t('Status');
    }
    // Add operations with the right colspan.
    $field_ui = !empty($this->entityInfo['bundle of']) && module_exists('field_ui');
    $exportable = !empty($this->entityInfo['exportable']);
    $colspan = 3;
    $colspan = $field_ui ? $colspan + 2 : $colspan;
    $colspan = $exportable ? $colspan + 1 : $colspan;
    $header[] = array(
      'data' => t('Operations'),
      'colspan' => $colspan
    );
    $render = array(
      '#theme' => 'table',
      '#header' => $header,
      '#rows' => $rows,
      '#empty' => t('None.')
    );
    return $render;
  }
}

class CharacterEditorTypeUIController extends EntityDefaultUIController{

  /**
   * Overrides hook_menu() defaults.
   */
  public function hook_menu(){
    $items = parent::hook_menu();
    $items[$this->path]['type'] = MENU_LOCAL_ACTION;
    return $items;
  }
}

class CharacterEditorCharacterUIController extends EntityDefaultUIController{

  /**
   * Builds the entity overview form.
   */
  public function overviewForm($form, &$form_state){
    $this->path = str_replace('%character_editor_project', arg(4), $this->path);
    // By default just show a simple overview for all entities.
    $form['table'] = $this->overviewTable();
    return $form;
  }

  public function overviewTable($conditions = array()){
    // Load the project
    $project = character_editor_project_load(arg(4));
    // entity_load conditions is deprecated so get the IDs for all characters in this project
    $ids = array_keys(character_editor_get_characters(array('pid' => $project->pid)));
    $entities = entity_load($this->entityType, $ids);
    ksort($entities);
    $rows = array();
    foreach($entities as $id => $entity){
      $rows[] = $this->overviewTableRow($conditions, $id, $entity);
    }
    // Assemble the right table header.
    $header = array(
      t('Label')
    );
    if(!empty($this->entityInfo['exportable'])){
      $header[] = t('Status');
    }
    // Add operations with the right colspan.
    $field_ui = !empty($this->entityInfo['bundle of']) && module_exists('field_ui');
    $exportable = !empty($this->entityInfo['exportable']);
    $colspan = 3;
    $colspan = $field_ui ? $colspan + 2 : $colspan;
    $colspan = $exportable ? $colspan + 1 : $colspan;
    $header[] = array(
      'data' => t('Operations'),
      'colspan' => $colspan
    );
    $render = array(
      '#theme' => 'table',
      '#header' => $header,
      '#rows' => $rows,
      '#empty' => t('None.')
    );
    return $render;
  }
}

/**
 * Generates the character project editing form.
 */
function character_editor_project_type_form($form, &$form_state, $type, $op = 'edit'){
	
  if($op == 'clone'){
    $type->label .= ' (cloned)';
    $type->name = '';
  }
  $form['label'] = array(
    '#title' => t('Label'),
    '#type' => 'textfield',
    '#default_value' => isset($type->label) ? $type->label : '',
    '#description' => t('The human-readable name of this chararacter project.'),
    '#required' => TRUE,
    '#size' => 30
  );
  // Machine-readable type name.
  $form['type'] = array(
    '#type' => 'machine_name',
    '#default_value' => isset($type->type) ? $type->type : '',
    '#maxlength' => 32,
    '#machine_name' => array(
      'exists' => 'character_editor_project_type_load',
      'source' => array(
        'label'
      )
    ),
    '#description' => t('A unique machine-readable name for this character project. It must only contain lowercase letters, numbers, and underscores.')
  );

  $form['actions'] = array(
    '#type' => 'actions'
  );
  $form['actions']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save character project'),
    '#weight' => 40
  );
  if($op != 'add'){
    $form['actions']['delete'] = array(
      '#type' => 'submit',
      '#value' => t('Delete character project'),
      '#weight' => 45,
      '#limit_validation_errors' => array(),
    );
  }
  return $form;
}

/**
 * Form API submit callback for the character project & chracter type forms
 */
function character_editor_project_type_form_submit(&$form, &$form_state){
  $entity = entity_ui_form_submit_build_entity($form, $form_state);
  // Save and go back.
  $entity->save();
  $form_state['redirect'] = CHARACTER_EDITOR_ADMIN_PATH;
}

/**
 * Generates the character project editing form.
 */
function character_editor_project_form($form, &$form_state, $project, $op = 'edit'){
  if($op == 'clone'){
    $project->label .= ' (cloned)';
    $project->name = '';
  }
  $form['label'] = array(
    '#title' => t('Label'),
    '#type' => 'textfield',
    '#default_value' => $project->label,
    '#description' => t('The human-readable name of this chararacter project.'),
    '#required' => TRUE,
    '#size' => 30
  );
  // Machine-readable type name.
  $form['name'] = array(
    '#type' => 'machine_name',
    '#default_value' => isset($project->name) ? $project->name : '',
    '#maxlength' => 32,
    '#machine_name' => array(
      'exists' => 'character_editor_project_load',
      'source' => array(
        'label'
      )
    ),
    '#description' => t('A unique machine-readable name for this character project. It must only contain lowercase letters, numbers, and underscores.')
  );
  $form['description'] = array(
    '#title' => t('Description'),
    '#type' => 'textarea',
    '#default_value' => $project->description,
    '#description' => t('Description for this chararacter project.'),
    '#size' => 30
  );
  
  if(!isset($project->type)){
  	$project->type = arg(3);	
  }
  
  
  
  field_attach_form('character_editor_project', $project, $form, $form_state);
  
  $form['actions'] = array(
    '#type' => 'actions'
  );
  $form['actions']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save character project'),
    '#weight' => 40
  );
  if($op != 'add'){
    $form['actions']['delete'] = array(
      '#type' => 'submit',
      '#value' => t('Delete character project'),
      '#weight' => 45,
      '#limit_validation_errors' => array(),
    );
  }
  return $form;
}

/**
 * Form API submit callback for the character project & chracter type forms
 */
function character_editor_project_form_submit(&$form, &$form_state){
  $entity = entity_ui_form_submit_build_entity($form, $form_state);
  // Save and go back.
  $entity->save();
  $form_state['redirect'] = CHARACTER_EDITOR_ADMIN_PATH . '/' . $entity->type;
}

/**
 * Generates the character type editing form.
 */
function character_editor_type_form($form, &$form_state, $character_type, $op = 'edit'){
  if($op == 'clone'){
    $character_type->label .= ' (cloned)';
    $character_type->name = '';
  }
  $form['label'] = array(
    '#title' => t('Label'),
    '#type' => 'textfield',
    '#default_value' => $character_type->label,
    '#description' => t('The human-readable name.'),
    '#required' => TRUE,
    '#size' => 30
  );
  // Machine-readable type name.
  $form['type'] = array(
    '#type' => 'machine_name',
    '#default_value' => isset($character_type->name) ? $character_type->name : '',
    '#maxlength' => 32,
    '#machine_name' => array(
      'exists' => 'character_editor_project_load',
      'source' => array(
        'label'
      )
    ),
    '#description' => t('A unique machine-readable name for this character project. It must only contain lowercase letters, numbers, and underscores.')
  );
  $form['actions'] = array(
    '#type' => 'actions'
  );
  $form['actions']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save character project'),
    '#weight' => 40
  );
  if($op != 'add'){
    $form['actions']['delete'] = array(
      '#type' => 'submit',
      '#value' => t('Delete character project'),
      '#weight' => 45,
      '#limit_validation_errors' => array(),
      '#submit' => array(
        'character_editor_form_submit_delete'
      )
    );
  }
  return $form;
}

/**
 * Form API submit callback for the character project & chracter type forms
 */
function character_editor_type_form_submit(&$form, &$form_state){
  $entity = entity_ui_form_submit_build_entity($form, $form_state);
  // Save and go back.
  $entity->save();
  $form_state['redirect'] = CHARACTER_EDITOR_ADMIN_PATH . '/character-type';
}

/**
 * Generates the character type editing form.
 */
function character_editor_character_form($form, &$form_state, $character, $op = 'edit'){
  if(!$character_types = character_editor_list_bundles('character_editor_character')){
    // Ensure there are character types before proceeding - there always should be 'cos we'll lock the base ones
    drupal_set_message(t('There are no character types. Please define some character types before creating any characters.'), 'error');
    return array();
  }
  // Load & store the project
  if(!isset($form_state['#project'])){
    $form_state['#project'] = character_editor_project_load(arg(4));
  }
  // Set the default character type
  if(isset($form_state['input']['type'])){
    $character->type = $form_state['input']['type'];
  }elseif(!isset($character->type)){
    // Default value will be a character group
    $character->type = 'group';
  }
  $form['label'] = array(
    '#title' => t('Label'),
    '#type' => 'textfield',
    '#default_value' => $character->label,
    '#description' => t('The human-readable name.'),
    '#required' => TRUE,
    '#size' => 30
  );
  $form['description'] = array(
    '#title' => t('Description'),
    '#type' => 'textarea',
    '#default_value' => $character->description,
    '#description' => t('Description for this chararacter project.'),
    '#size' => 30
  );
  $form['pid'] = array(
    '#type' => 'hidden',
    '#value' => $form_state['#project']->pid
  );
  // Character type.
  $form['type'] = array(
    '#title' => t('Character type'),
    '#type' => 'select',
    '#options' => $character_types,
    '#default_value' => $character->type,
    '#limit_validation_errors' => array(),
    '#ajax' => array(
      'callback' => 'character_editor_character_form_callback_get_types',
      'wrapper' => 'character-fields-wrapper',
      'method' => 'replace'
    )
  );
  // Get all the groups for this character project
  $groups = character_editor_get_characters(array('pid' => $form_state['#project']->pid, 'type' => 'group'));
  // If there are no character groups for this project, only allow groups to be added
  if(!count($groups)){
    $form['type']['#default_value'] = 'group';
    $form['type']['#disabled'] = true;
    $form['type']['#description'] = t('You must add a group to this project before you can add other characters.');
  }elseif($op == 'edit' && $form['type']['#default_value'] == 'group' && count(character_editor_character_get_hierarchy(array('parent' => $character->cid)))){ // If this character is a group, check if it has children
    $form['type']['#disabled'] = true;
    $form['type']['#description'] = t('This group has children - the character type cannot be changed.');
  }
  // Attach any additional fields for this character type
  $fields = array();
  field_attach_form('character_editor_character', $character, $fields, $form_state);
  $form['fields'] = $fields;
  $form['fields']['#prefix'] = '<div id="character-fields-wrapper">';
  $form['fields']['#suffix'] = '</div>';
  $form['structure'] = array(
    '#type' => 'fieldset',
    '#title' => t('Structure'),
    '#collapsible' => TRUE
  );
  $form['structure']['group'] = array(
    '#type' => 'textfield',
    '#title' => t('Group'),
    '#type' => 'select',
    '#options' => $groups,
    '#default_value' => (isset($character->group) ? $character->group : ''),
    '#states' => array(
      'invisible' => array(
        '#edit-type' => array(
          'value' => 'group'
        )
      )
    )
  );
  $form['structure']['weight'] = array(
    '#type' => 'textfield',
    '#title' => t('Weight'),
    '#size' => 6,
    '#default_value' => (isset($character->weight) ? $character->weight : 0),
    '#description' => t('Characters are displayed in ascending order by weight.'),
    '#required' => TRUE
  );
  $form['actions'] = array(
    '#type' => 'actions'
  );
  $form['actions']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save character'),
    '#weight' => 40
  );
  if($op != 'add'){
    $form['actions']['delete'] = array(
      '#type' => 'submit',
      '#value' => t('Delete character'),
      '#weight' => 45,
      '#limit_validation_errors' => array(),
      '#submit' => array(
        'character_editor_form_submit_delete'
      )
    );
  }
  return $form;
}

function character_editor_character_form_submit(&$form, &$form_state){
  $character = entity_ui_form_submit_build_entity($form, $form_state);
  // Save the character
  $character->save();
  character_editor_character_update_hierarchy($character->cid, ($character->type == 'group' ? 0 : $character->group));
  $form_state['redirect'] = CHARACTER_EDITOR_ADMIN_PATH . '/characters/' . $form_state['#project']->name;
}

/**
 * 
 * CHaracter form #akax callback - list all character types
 * @param array $form
 * @param array $form_state
 */
function character_editor_character_form_callback_get_types($form, $form_state){
  return $form['fields'];
}