<?php

class EntityUIEntityController extends EntityUIController{

  static public function ops(){
    return array(
      'edit' => 'entity_ui_entity_edit_form',
      'delete' => 'entity_ui_entity_delete_form'
    );
  }

  public function hook_menu(){
    $plural_label = isset($this->entityInfo['plural label']) ? $this->entityInfo['plural label'] : $this->entityInfo['label'] . 's';
    $items = array();
    $items[$this->path] = array(
      'title' => $plural_label,
      'page callback' => 'drupal_get_form',
      'page arguments' => array(
        $this->entityType . '_overview_form',
        $this->entityType
      ),
      'description' => 'Manage ' . $plural_label . '.',
      'access callback' => 'entity_access',
      'access arguments' => array(
        'view',
        $this->entityType
      ),
      'file' => 'includes/entity.ui.inc',
      'type' => MENU_LOCAL_TASK
    );
    $items[$this->path . '/add'] = array(
      'title callback' => 'entity_ui_get_action_title',
      'title arguments' => array(
        'add',
        $this->entityType
      ),
      'page callback' => 'entity_ui_add_page',
      'access callback' => 'entity_access',
      'access arguments' => array(
        'create',
        $this->entityType
      ),
      // Needs to be MENU_NORMAL_ITEM so system_admin_menu_block() works
      // Alter it entity_ui_menu_local_tasks_alter
      'type' => MENU_NORMAL_ITEM
    );
    // Add menu items to add each different type of entity.
    foreach(entity_ui_get_entities($this->entityInfo['type of']) as $type){
      $items[$this->path . '/add/' . $type->type] = array(
        'title' => 'Add ' . $type->label,
        'page callback' => 'drupal_get_form',
        'page arguments' => array(
          'entity_ui_entity_form',
          entity_ui_create($this->entityType, array(
            'type' => $type->type
          ))
        ),
        'access callback' => 'entity_access',
        'access arguments' => array(
          'create',
          $this->entityType
        )
      );
    }
    // Initiate an entity so we can get the path 
    $entity = new EntityUI(array(), $this->entityType);
    $path = entity_ui_entity_uri($entity) . '/%';
    $arg_pos = substr_count($path, '/');
    $items[$path] = array(
      'title callback' => 'entity_ui_page_title',
      'title arguments' => array(
        $this->entityType,
        $arg_pos
      ),
      'page callback' => 'entity_ui_page_view',
      'page arguments' => array(
        $this->entityType,
        $arg_pos
      ),
      'access callback' => 'entity_access',
      'access arguments' => array(
        'view',
        $this->entityType
      )
    );
    if(count($this->ops())){
      // Set view as the default task
      $items[$path . '/view'] = array(
        'title' => 'View',
        'type' => MENU_DEFAULT_LOCAL_TASK,
        'weight' => -10
      );
      $op_count = 0;
      foreach($this->ops() as $op => $op_callback){
        // Edit actually going to take you to the edit grid view
        $items[$path . '/' . $op] = array(
          'title' => ucfirst($op),
          'page callback' => 'drupal_get_form',
          'page arguments' => array(
            $op_callback,
            $this->entityType,
            $arg_pos
          ),
          'access callback' => 'entity_access',
          'access arguments' => array(
            'view',
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

  public function overviewTable($conditions = array()){
    $entities = entity_load($this->entityType, FALSE, $conditions);
    ksort($entities);
    $rows = array();
    foreach($entities as $entity){
      $rows[] = $this->overviewTableRow($conditions, entity_id($this->entityType, $entity), $entity);
    }
    // Assemble the right table header.
    $header = array(
      t('Label')
    );
    $header[] = array(
      'data' => t('Operations'),
      'colspan' => count($this->ops())
    );
    $render = array(
      '#theme' => 'table',
      '#header' => $header,
      '#rows' => $rows,
      '#empty' => t('None.')
    );
    return $render;
  }

  /**
   * Generates the row for the passed entity and may be overridden in order to
   * customize the rows.
   *
   * @param $additional_cols
   * Additional columns to be added after the entity label column.
   */
  protected function overviewTableRow($conditions, $id, $entity, $additional_cols = array()){
    $entity_uri = entity_uri($this->entityType, $entity);
    $row[] = array(
      'data' => array(
        '#theme' => 'entity_ui_overview_item',
        '#label' => entity_label($this->entityType, $entity),
        '#name' => !empty($this->entityInfo['exportable']) ? entity_id($this->entityType, $entity) : FALSE,
        '#url' => $entity_uri ? $entity_uri : FALSE,
        '#entity_type' => $this->entityType
      )
    );
    if(count($this->ops())){
      foreach(array_keys($this->ops()) as $op){
        $row[] = l($op, $entity_uri['path'] . '/' . $op);
      }
      return $row;
    }
  }
}