<?php

class EntityAdminEntityController extends EntityAdminController{

  static public function ops(){
    return array(
      'edit' => 'entity_admin_entity_form',
      'delete' => 'entity_admin_entity_delete_form'
    );
  }

  public function hook_menu(){
    $plural_label = isset($this->entityInfo['plural label']) ? $this->entityInfo['plural label'] : $this->entityInfo['label'] . 's';
    $items = array();
    // Do we want an admin interface?
    if(!isset($this->entityInfo['admin ui']['access']) || $this->entityInfo['admin ui']['access']){
      // Add menu items to add each different type of entity.
      foreach(entity_admin_get_entities($this->entityInfo['type of']) as $type){
        $items[$this->path . '/add/' . $type->type] = array(
          'title' => 'Add ' . $type->label,
          'page callback' => 'drupal_get_form',
          'page arguments' => array(
            'entity_admin_entity_form',
            entity_admin_create($this->entityType, array(
              'type' => $type->type
            ))
          ),
          'access callback' => 'entity_access',
          'access arguments' => array(
            'create',
            $this->entityType
          ),
          'type' => MENU_LOCAL_TASK
        );
      }
    }
    // Generate the view /edit / delete callbacks for the entity itself
    $wildcard = isset($this->entityInfo['admin ui']['menu wildcard']) ? $this->entityInfo['admin ui']['menu wildcard'] : '%entity_object';
    $entity_path = entity_admin_entity_get_path($this->entityType, $this->entityInfo, $wildcard);
    $arg_pos = substr_count($entity_path, '/', 0, strpos($entity_path, '%'));
    $items[$entity_path] = array(
      // The value in the position of the load function is always passed through first.
      'load arguments' => (isset($this->entityInfo['admin ui']['load arguments']) ? isset($this->entityInfo['admin ui']['load arguments']) : array(
        $this->entityType
      )),
      'title callback' => (isset($this->entityInfo['admin ui']['title callback']) ? $this->entityInfo['admin ui']['title callback'] : 'entity_admin_page_title'),
      'title arguments' => (isset($this->entityInfo['admin ui']['title arguments']) ? isset($this->entityInfo['admin ui']['title arguments']) : array(
        $arg_pos,
        $this->entityType
      )),
      'page callback' => (isset($this->entityInfo['admin ui']['page callback']) ? $this->entityInfo['admin ui']['page callback'] : 'entity_admin_page_view'),
      'page arguments' => array(
        $arg_pos,
        $this->entityType
      ),
      'page arguments' => (isset($this->entityInfo['admin ui']['page arguments']) ? isset($this->entityInfo['admin ui']['page arguments']) : array(
        $arg_pos,
        $this->entityType
      )),
      'access callback' => 'entity_access',
      'access arguments' => (isset($this->entityInfo['admin ui']['access arguments']) ? isset($this->entityInfo['admin ui']['access arguments']) : array(
        'view',
        $this->entityType
      ))
    );
    if(isset($this->entityInfo['admin ui']['file'])){
      $items[$entity_path]['file'] = $this->entityInfo['admin ui']['file'];
    }
    if(isset($this->entityInfo['admin ui']['file path'])){
      $items[$entity_path]['file path'] = $this->entityInfo['admin ui']['file path'];
    }
    if(count($this->ops())){
      // Set view as the default task
      $items[$entity_path . '/view'] = array(
        'title' => 'View',
        'type' => MENU_DEFAULT_LOCAL_TASK,
        'weight' => -10
      );
      $op_count = 0;
      foreach($this->ops() as $op => $op_callback){
        // Edit actually going to take you to the edit grid view
        $items[$entity_path . '/' . $op] = array(
          'title' => ucfirst($op),
          'page callback' => 'drupal_get_form',
          'page arguments' => array(
            $op_callback,
            $arg_pos
          ),
          'access callback' => 'entity_access',
          'access arguments' => array(
            $op,
            $this->entityType
          ),
          'weight' => $op_count++,
          'type' => MENU_LOCAL_TASK,
          'context' => MENU_CONTEXT_PAGE | MENU_CONTEXT_INLINE
        );
      }
    }
    return $items;
  }
}