<?php
/**
 * @file
 * All the menus, pages, and functionality related to administering entities.
 */

/**
 * This function creates the menu items related to entity administration
 * @param $entity_type
 *  (String) the entity type
 * @param $bundle
 *  (String) the bundle of the entity
 *
 * this function is called from hook_menu()
 * @see eck_menu()
 */
function eck__entity__menu($entity_type, $bundle) {
  $menu = array();
  
  // DELETE Bundle
  $menu["admin/structure/eck/{$entity_type->name}/{$bundle->name}/delete"] = array(
    'title' => "Delete {$entity_type->name}'s Bundle: {$bundle->name}",
    'page callback' => "drupal_get_form",//"eck__bundle__delete",
    'page arguments' => array('eck__bundle__delete_form', $entity_type, $bundle),
    'access arguments' => array("delete {$entity_type->name} {$bundle->name} bundle"),
    'file' => 'eck.bundle.inc'
  );
  
  $admin_info = get_bundle_admin_info($entity_type->name, $bundle->name);
  
  // OVERVIEW Entity
  $menu[$admin_info['path']] = array(
    'title' => "{$bundle->label} Entities",
    'description' => "View all entites of type {$entity_type->label} with bundle {$bundle->label}",
    'page callback' => "eck__entity__overview",
    'page arguments' => array($entity_type, $bundle),
    'access arguments' => array("administer {$entity_type->name} {$bundle->name} entities"),
    'weight' => 0,
    'file' => 'eck.entity.inc'
  );

  $menu[$admin_info['path']."/overview"] = array(
    'title' => "Overview",
    'type' => MENU_DEFAULT_LOCAL_TASK,
    'weight' => 0
  );
  
  $crud_info = get_bundle_crud_info($entity_type->name, $bundle->name);
  
  foreach($crud_info as $action => $info){
      
    $action_label = ucfirst($action);
    $arg_labels = array('id');
    $args = array();
    
    foreach($arg_labels as $arg){
      
      if(array_key_exists($arg, $info['args'])){
        $args[] = $info['args'][$arg];
      }
    }
    
    $args = array_merge(array($entity_type, $bundle), $args);
    
    $menu[$info['path']] = array(
      'title' => "{$action_label} {$bundle->label} Entity",
      'description' => "{$action_label} an entity of type {$entity_type->label} with bundle {$bundle->label}",
      'page callback' => "eck__entity__{$action}",
      'page arguments' => $args,
      'access arguments' => array("{$action} {$entity_type->name} {$bundle->name} entities"),
      'file' => 'eck.entity.inc'
    );
  }
  
  //dpm($menu, "Menu");

  return $menu;
}

//a few helper function to get data our of the info array
function get_bundle_admin_info($entity_type, $bundle){
  $info = entity_get_info();
  
  return $info[$entity_type]['bundles'][$bundle]['admin'];
}
function get_bundle_crud_info($entity_type, $bundle){
  $info = entity_get_info();
  return $info[$entity_type]['bundles'][$bundle]['crud'];
}

/**
 * This is the callback function for the entity overview page. This page shows all
 * of the entities created of a given type and bundle
 * @param $entity_type
 *  (String) entity type
 * @param $bundle
 *  (String) Bundle
 */
function eck__entity__overview($entity_type, $bundle) {

  $table = "eck_{$entity_type->name}";

  // @todo Mabye I should use entity field query
  // Get all entity instances of this type
  $results = db_select($table, 't')->fields('t')->condition('type', $bundle->name, '=')->execute();

  $ids = array();
  foreach ($results as $record) {
    $ids[] = $record->id;
  }

  $entities = entity_load($entity_type->name, $ids);
  
  //Because of the flexible paths capabilities, we are not guaranteed to see a local action for the add here,
  //so lets add a link ourselves until we figure out whether there is a better solution
  $crud_info = get_bundle_crud_info($entity_type->name, $bundle->name);
  $build['add'] = array('#markup' => "+ ".l(t('Add Entity'), $crud_info['add']['path']));

  $build['table'] = entity_table($entities, TRUE);

  $build['sample_code'] = array(
    '#theme' => 'form_element',
    '#title' => t('Optional entity type class'),
    '#description' => t('You may add this custom class for the bundle. To create a new instance of this class programmatically call: %code', array(
      '%code' => "entity_create('{$entity_type->name}', array('type' => '{$bundle->name}'));"
    )),
    '#children' => "<pre>" . check_plain("class " . eck_get_class_name($bundle->name, 'EntityType') . " extends Entity{\n\n}") . "</pre>"
  );

  return $build;
}

/**
 * Call back for the local action add (It adds a new entity)
 *
 * @param $entity_type
 *  (String) entity type
 * @param $bundle
 *  (String) Bundle
 */
function eck__entity__add($entity_type, $bundle) {
  
  $entity = eck__entity_create($entity_type->name, $bundle->name);
  
  return drupal_get_form("eck__entity__form", $entity);
}

/**
 * Get the entities view
 *
 * @param $entity_type
 *  (String) entity type
 * @param $id
 *  (int) The entities id
 */
function eck__entity__build($entity_type, $bundle, $id) {
  if (is_numeric($id)) {
    
    $entities = entity_load($entity_type->name, array($id));
    if(array_key_exists($id, $entities)){
     $entity = $entities[$id];
    }else{
      $entity = NULL;
    }
  }else{
    drupal_not_found();
    exit();
  }
  
  if(!$entity){
    drupal_not_found();
    exit();
  }else{
    if($entity->type == $bundle->name){
      return $entity->view();
    }else{
      drupal_not_found();
      exit();
    }

  }
}

/**
 * Callback function for an entities edit page
 *
 * @param $entity_type
 *  (String) entity type
 * @param $bundle
 *  (String) Bundle
 * @param $id
 *  (int) the Id of the entity to be edited
 */
function eck__entity__edit($entity_type, $bundle, $id) {
  if (is_numeric($id)) {
    $entities = entity_load($entity_type->name, array($id));
    $entity = $entities[$id];
  }

  global $user;

  $entity->uid = $user->uid;
  $entity->changed = time();

  return drupal_get_form("eck__entity__form", $entity);
}

/**
 * Callback function for the delete functionality
 *
 * @param $form
 *  Form array provided by the Form API
 * @param $form_state
 *  array provided by the Form API
 * @param $entity_type
 *  (String) entity type
 * @param $bundle
 *  (String) Bundle
 * @param $id
 *  (int) the Id of the entity to be deleted
 */
function eck__entity__delete($entity_type, $bundle, $id) {
  return drupal_get_form('eck__entity__delete_form', $entity_type, $bundle, $id);
  
}

function eck__entity__delete_form($form, &$form_state, $entity_type, $bundle, $id){
  $entities = entity_load($entity_type->name, array($id));

  $form['entity'] =
      array(
        '#type' => 'value',
        '#value' => $entities[$id],
  );

  $form['entity_type'] =
      array(
        '#type' => 'value',
        '#value' => $entity_type,
  );

  $form['bundle'] =
      array(
        '#type' => 'value',
        '#value' => $bundle,
  );

  $form['submit_redirect'] =
  array(
    '#type' => 'value',
    '#value' => "admin/structure/eck/{$entity_type->name}/{$bundle->name}",
  );

  $message = t("Are you sure that you want to delete %id",
          array("%id" => $id));

  $caption = t("This action cannot be undone.");

  return confirm_form($form, $message, "admin/structure/eck/{$entity_type->name}", $caption, t('Delete'));
  
}

/**
 * Sumbmit function for the delete functionality
 *
 * @param $form
 *  Form array provided by the Form API
 * @param $form_state
 *  array provided by the Form API
 */
function eck__entity__delete_form_submit($form, &$form_state) {
  $entity = $form_state['values']['entity'];
  $entity_type = $form_state['values']['entity_type'];
  $bundle = $form_state['values']['bundle'];
  $submit_redirect = $form_state['values']['submit_redirect'];

  $entity->delete();

  drupal_cron_run();

  // Ok, lets delete the entity
  $form_state['redirect'] = $submit_redirect;
}

/**
 * Sets up an entities form
 *
 * @param $form
 *  Form array provided by the Form API
 * @param $form_state
 *  array provided by the Form API
 * @param $entity
 *  an object as returned by entity_load()
 */
function eck__entity__form($form, $form_state, $entity) {
  $form['entity'] = array(
    '#type' => 'value',
    '#value' => $entity
  );

  $form['submit'] = array(
    '#type' => 'submit',
    '#weight' => 10000,
    '#value' => t('Save'),
  );

  field_attach_form($entity->entityType(), $entity, $form, $form_state);

  return $form;
}

/**
 * Submit function for entity form
 *
 * @param $form
 *  Form array provided by the Form API
 * @param $form_state
 *  array provided by the Form API
 */
function eck__entity__form_submit($form, &$form_state) {
  $entity = $form_state['values']['entity'];

  if (isset($form_state['field'])) {
    foreach (array_keys($form_state['field']) as $field) {
      $entity->{$field} = _field_unset_empty($field, $form_state['values'][$field]);
    }
  }

  $entity->save();

  drupal_set_message(t("Entity {$entity->id} - @entity_label has been saved", array("@entity_label" => entity_label($form['#entity_type'], $entity)) ));
}


/**
 * Creates a renderable array to show an entity
 *
 * @param $entity_type
 *  (String) entity type
 * @param $bundle
 *  (String) Bundle
 * @param $id
 *  (int) the Id of the entity to be deleted
 */
function eck__entity__view($entity_type, $bundle, $id) {
  $build = array();

  $entity_view = eck__entity__build($entity_type, $bundle, $id);

  $build["{$entity_type->name}_{$bundle->name}_page"] = $entity_view;

  return $build;
}
