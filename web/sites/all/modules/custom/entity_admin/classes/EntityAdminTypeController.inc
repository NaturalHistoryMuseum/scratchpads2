<?php

class EntityAdminTypeController extends EntityAdminController{

  public function hook_forms(){
    $forms = parent::hook_forms();
    $forms[$this->entityType . '_form']['callback'] = 'entity_admin_entity_type_form';
    return $forms;
  }
}