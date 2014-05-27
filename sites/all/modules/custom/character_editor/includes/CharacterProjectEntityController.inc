<?php

class CharacterProjectEntityController extends EntityAdminEntityController{

  static public function ops(){
    $ops = parent::ops();
    $ops['settings'] = 'entity_admin_entity_form';
    $ops['characters'] = 'character_editor_characters_order_form';
    $ops['export'] = 'character_editor_project_export';
    return $ops;
  }

  public function hook_menu(){
    $items = parent::hook_menu();
    // We actually want to map edit to the character project view
    $items['character-editor-project/%entity_object/edit']['page callback'] = 'character_editor_project_view';
    $items['character-editor-project/%entity_object']['page callback'] = 'character_editor_project_view';
    // The first argument in the edit callback is the form id
    // Remove it - character_editor_project_view() expects just the entity
    array_shift($items['character-editor-project/%entity_object/edit']['page arguments']);
    return $items;
  }
}