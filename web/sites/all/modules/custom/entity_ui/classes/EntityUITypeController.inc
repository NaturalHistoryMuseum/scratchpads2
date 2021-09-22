<?php

class EntityUITypeController extends EntityUIController{

  public function hook_forms(){
    $forms = parent::hook_forms();
    $forms[$this->entityType . '_form']['callback'] = 'entity_ui_entity_type_form';
    return $forms;
  }
}