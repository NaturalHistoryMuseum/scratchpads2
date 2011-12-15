<?php

class CharacterProjectEntityUIController extends EntityUIEntityController{

  static public function ops(){
  	
  	$ops = parent::ops();
  	$ops['settings'] = 'entity_ui_entity_edit_form';
  	return $ops;

  }
  
  public function hook_menu(){
  	
  	$items = parent::hook_menu();
  	// We actually want to map edit to the character project view
  	$items['character-project/%/edit']['page callback'] = 'character_editor_project_view';
  	// And get rid of the form id in the page args
  	array_shift($items['character-project/%/edit']['page arguments']);
  	return $items;
  	
  }
  
}