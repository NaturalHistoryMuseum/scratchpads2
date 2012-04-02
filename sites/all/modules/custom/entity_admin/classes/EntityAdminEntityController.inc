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
      dd($this->path);
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
    $entity_path = entity_admin_entity_get_path($this->entityType, $this->entityInfo);
    $wildcard = isset($this->entityInfo['admin ui']['menu wildcard']) ? $this->entityInfo['admin ui']['menu wildcard'] : '%entity_object';
    $entity_path .= '/' . $wildcard;
    $arg_pos = substr_count($entity_path, '/', 0, strpos($entity_path, '%'));
    $items[$entity_path] = array(
      // The value in the position of the load function is always passed through first.
      'load arguments' => (isset($this->entityInfo['admin ui']['load arguments']) ? isset($this->entityInfo['admin ui']['load arguments']) : array(
        $this->entityType
      )),
      'title callback' => 'entity_admin_page_title',
      'title arguments' => (isset($this->entityInfo['admin ui']['title arguments']) ? isset($this->entityInfo['admin ui']['title arguments']) : array(
        $arg_pos
      )),
      'page callback' => 'entity_admin_page_view',
      'page arguments' => array(
        $arg_pos
      ),
      'page arguments' => (isset($this->entityInfo['admin ui']['page arguments']) ? isset($this->entityInfo['admin ui']['page arguments']) : array(
        $arg_pos
      )),
      'access callback' => 'entity_access',
      'access arguments' => (isset($this->entityInfo['admin ui']['access arguments']) ? isset($this->entityInfo['admin ui']['access arguments']) : array(
        'view',
        $this->entityType
      ))
    );
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

  public function overviewTable($conditions = array()){
    $entities = entity_load($this->entityType, FALSE, $conditions);
    ksort($entities);
    $rows = array();
    foreach($entities as $entity){
      $rows[] = $this->overviewTableRow($conditions, entity_id($this->entityType, $entity), $entity);
    }
    // Assemble the right table header.
    $header = array(
      t('Label'),
      t("Type")
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
    list($entity_id, $vid, $bundle) = entity_extract_ids($this->entityType, $entity);
    $row[] = array(
      'data' => array(
        '#theme' => 'entity_ui_overview_item',
        '#label' => entity_label($this->entityType, $entity),
        '#name' => !empty($this->entityInfo['exportable']) ? entity_id($this->entityType, $entity) : FALSE,
        '#url' => $entity_uri ? $entity_uri : FALSE,
        '#entity_type' => $this->entityType
      )
    );
    $row[] = $bundle;
    if(count($this->ops())){
      foreach(array_keys($this->ops()) as $op){
        $row[] = l($op, $entity_uri['path'] . '/' . $op);
      }
      return $row;
    }
  }
}