<?php
/**
 *
 * @file
 *
 * ENTITY TYPE
 *
 * Entity Types represent types of data. Drupal core contains multiple
 * entity types nodes, users, vocabularies, etc.
 *
 * ECK allows you to create entity types. This file contains all
 * of the entity type specific functionality
 *
 */

/**
 * Passthrough from hook_menu().
 *
 * It creates the menu items related to entity type management.
 */
function eck__entity_type__menu() {
  $menu = array();

  // OVERVIEW Entity Type.
  // View all of the created entity types.
  $menu['admin/structure/eck'] = array(
    'title' => 'Entity Type',
    'description' => 'A centralized administrative section for entity types',
    'page callback' => 'eck__entity_type__overview',
    'access arguments' => array('administer entity types'),
    'file' => 'eck.entity_type.inc'
  );

  // ADD Entity.
  $menu['admin/structure/eck/add'] = array(
    'title' => 'Add Entity Type',
    'description' => 'Add new entity type',
    'page callback' => 'drupal_get_form',
    'page arguments' => array('eck__entity_type__form'),
    'access arguments' => array('add entity types'),
    'type' => MENU_LOCAL_ACTION,
    'weight' => 1,
    'file' => 'eck.entity_type.inc'
  );

  module_load_include('inc', 'eck', 'eck.bundle');
  // Each entity type can have multiple bundles.
  // Now lets create the menus for the bundle administration of each
  // entity type.
  foreach (eck__entity_type__load() as $entity_type) {
    $menu = array_merge($menu, eck__bundle__menu($entity_type));
  }

  return $menu;
}

/**
 * Load an entity type object
 * @arg $entity_type
 *  (String) the name of the entity_type object to load
 * 
 * if no entity_type is given, all the objects are loaded
 */
function eck__entity_type__load($entity_type_name = NULL) {
  $query = db_select('eck_entity_type', 'e')->fields('e');
  
  if($entity_type_name){
    $query->condition('name', $entity_type_name);
  }
  
  $data = $query->execute();
  
  if ($entity_type_name) {
    $entity_type = $data->fetchObject();
    // Why the f*ck did I specify the 'serialize' flag in hook_schema() if it's useless anyway?!
    $entity_type->properties = unserialize($entity_type->properties);
    $entity_type->custom_properties = unserialize($entity_type->custom_properties);
    return $entity_type;
  }
  else {
    $entity_types = array();
    foreach ($data as $entity_type) {
      $entity_type->properties = unserialize($entity_type->properties);
      $entity_type->custom_properties = unserialize($entity_type->custom_properties);
      $entity_types[] = $entity_type;
    }
    return $entity_types;
  }
}

/**
 * Callback for the entity_type overview.
 */
function eck__entity_type__overview() {
  $header = array(t('Name'), array('data' => t('Operations'), 'colspan' => '1'));
  $rows = array();

  foreach (eck__entity_type__load() as $entity_type) {

    $rows[] = 
    array(
      l(t("{$entity_type->label}"), "admin/structure/eck/{$entity_type->name}"),
      l(t("delete"), "admin/structure/eck/{$entity_type->name}/delete")
    );
  }
  $build['entity_table'] = array(
    '#theme' => 'table',
    '#header' => $header,
    '#rows' => $rows,
  );

  return $build;
}

/**
 * Callback for adding entity types functionality.
 * @param $form
 *  Form array provided by the Form API
 * @param $state
 *  array provided by the Form API
 * @param $entity_type_name
 *   (String) the name of an existing entity type
 */
function eck__entity_type__form($form, &$state, $entity_type_name = NULL) {
  
  if (!isset($entity_type_name)) {
    $entity_type = (object) array(
      'is_new' => TRUE,
      'label' => '',
      'name' => '',
      'type_label' => '',
      'type' => '',
      'properties' => array(),
      'custom_properties' => array(),
    );
  }
  else {
    $entity_type = eck__entity_type__load($entity_type_name);
    $entity_type->is_new = FALSE;
  }
  
  $form['entity_type'] = array(
    '#type' => 'value',
    '#value' => $entity_type,
  );
  
  $form['entity_type_label'] = array(
    '#type' => 'textfield',
    '#title' => t('Entity Type'),
    '#default_value' => $entity_type->label,
    '#description' => t('A human readable name for the entity type'),
    '#required' => TRUE,
    '#disabled' => empty($entity_type->is_new),
  );

  $form['entity_type_name'] = array(
    '#type' => 'machine_name',
    '#default_value' => $entity_type->name,
    '#disabled' => empty($entity_type->is_new),
    '#machine_name' => array(
      'exists' => '_eck_fake_exists',
      'source' => array('entity_type_label'),
    ),
  );

  
  $form['#validate'][] = 'eck__entity_type__form_validate';
  

  // Only allow entering the desired bundle name when creating a new entity.
  if (!empty($entity_type->is_new)) {
    
    $form['bundle_label'] = array(
      '#type' => 'textfield',
      '#title' => 'Bundle (optional)',
      '#description' => 'A bundle with the same name as the entity type is created by default, this will override the default',
    );

    $form['bundle_name'] = array(
      '#type' => 'machine_name',
      '#required' => FALSE,
      '#machine_name' => array(
        'exists' => '_eck_fake_exists',
        'source' => array('bundle_label'),
      ),
    );
    
  }

  // Enable/disable properties.
  $form['properties'] = array(
    '#type' => 'fieldset',
    '#title' => t('Properties'),
    '#description' => t('Note: unchecking will delete the property and all data stored within!'),
    '#collapsible' => TRUE,
    '#tree' => TRUE,
  );
  
  module_load_include('inc', 'eck', 'eck.properties');
  foreach (eck_default_properties() as $property => $label) {
    $form['properties'][$property] = array(
      '#type' => 'checkbox',
      '#title' => $label,
      '#default_value' => !empty($entity_type->properties[$property]) || !empty($entity_type->is_new) ? 1 : 0,
    );
  }

  // Enable/disable custom properties.
  $form['custom_properties'] = array(
    '#type' => 'fieldset',
    '#title' => t('Custom properties'),
    '#description' => t('Note: unchecking will delete the property and all data stored within!'),
    '#access' => !empty($entity_type->custom_properties),
    '#collapsible' => TRUE,
    '#tree' => TRUE,
  );

  foreach ($entity_type->custom_properties as $property => $info) {
    $form['custom_properties'][$property] = array(
      '#type' => 'checkbox',
      '#title' => eck__entity_type__custom_property_label($info),
      '#default_value' => 1,
    );
  }

  // Add custom property.
  $form['add'] = array(
    '#type' => 'fieldset',
    '#title' => t('Add custom property'),
    '#access' => empty($entity_type->is_new),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
    '#tree' => TRUE,
  );

  $types[t('Generic')] = array(
    'text' => t('Text'),
    'decimal' => t('Decimal'),
    'integer' => t('Integer'),
    'date' => t('Timestamp'),
  );
  foreach (entity_get_info() as $name => $data) {
    $types[t('Entities')][$name] = $data['label'];
  }

  $form['add']['type'] = array(
    '#type' => 'select',
    '#title' => t('Type'),
    '#options' => array('' => t('- Please choose -')) + $types,
  );

  $form['add']['label'] = array(
    '#type' => 'textfield',
    '#title' => t("Name"),
    '#description' => t("A human readable name for the property."),
  );

  $form['add']['name'] = array(
    '#type' => 'machine_name',
    '#required' => FALSE,
    '#machine_name' => array(
      'label' => t("Database column name"),
      'exists' => '_eck_fake_exists',
      'source' => array('add', 'label'),
    ),
  );

  $form['submit'] = array(
    '#type' => 'submit',
    '#weight' => 10000,
    '#value' => t('Save'),
  );

  return $form;
  
}

/**
 * When an entity type is being addes, we need to make sure that
 * its name is unique.
 *
 * @param $form
 *  Form array provided by the Form API
 * @param $state
 *  array provided by the Form API
 */
function eck__entity_type__form_validate($form, &$state) {
  $info = entity_get_info();
  $entity_types = array_keys($info);
  $entity_type = $state['values']['entity_type'];
  if (in_array($entity_type, $entity_types)) {
    form_set_error('name', t("Entity {$entity_type} already exists"));
  }
}

/**
 * Submit handler for adding and entity type.
 *
 * @param $form
 *  Form array provided by the Form API
 * @param $state
 *  array provided by the Form API
 */
function eck__entity_type__form_submit($form, &$state) {
  // This are required so I don't have to do any checks.
  $entity_type = $state['values']['entity_type'];
  
  $entity_type_name = $state['values']['entity_type_name'];
  $entity_type->name = $entity_type_name;
  
  $entity_type_label = $state['values']['entity_type_label'];
  $entity_type->label = $entity_type_label;
  
  $properties = array_filter($state['values']['properties']);
  
  if(array_key_exists('custom_properties', $state['values'])){
    $custom_properties = array_filter($state['values']['custom_properties']);
  }else{
    $custom_properties =  array();
  }
  
  $entity_type->new_properties = $properties;
  $entity_type->new_custom_properties = $custom_properties;
  
  if($entity_type->is_new){
    //if the entity_type is new, we need to set up the properties
    //in the entity object, this are used to create then entity_type
    //table which looks at the properties in the entity_type 
    //to know what the table should look like
    
    $entity_type->properties = $properties;
  }
  
  // If the table does not exist, then this is a valid entity name, and we can save it.
  if (!db_table_exists("eck_{$entity_type_name}")) {
    // Let's add the type to the table.
    if (!empty($state['values']['bundle_name'])) {
      $bundle_name = $state['values']['bundle_name'];
      if (!empty($state['values']['bundle_label'])) {
        $bundle_label = $state['values']['bundle_label'];
      }
      else {
        $bundle_label = ucfirst($bundle_name);
      }
    }
    else {
      $bundle_name = $entity_type_name;
      $bundle_label = $entity_type_label;
    }

    db_insert('eck_bundle')
        ->fields(array(
          'entity_type' => $entity_type_name,
          'name' => $bundle_name,
          'label' => $bundle_label,
        ))
        ->execute();

    db_insert('eck_entity_type')
        ->fields(array(
          'name' => $entity_type_name,
          'label' => $entity_type_label,
          'properties' => serialize($properties),
          'custom_properties' => serialize($custom_properties),
        ))
        ->execute();
        
    db_create_table("eck_{$entity_type_name}", eck__entity_type__schema($entity_type));

    // Clear info caches in order to pick up newly created entities.
    drupal_get_schema(NULL, TRUE);
    entity_info_cache_clear();
    // Rebuild the menu to pick up entity related paths.
    menu_rebuild();

    drupal_set_message(t('Entity type %entity_type has been created.', array('%entity_type' => $entity_type_label)));

    drupal_goto("admin/structure/eck/{$entity_type->name}");
  }
  else {
    // @todo Move into validation handler.
    //drupal_set_message(t('Database table %name already exists', array('%name' => $name)), 'error');

    // Process properties.
    $removed_properties = array_diff_key($entity_type->properties, $properties);
    foreach ($removed_properties as $property => $value) {
      db_drop_field("eck_{$entity_type->name}", $property);
    }

    $new_properties = array_diff_key($properties, $entity_type->properties);
    foreach ($new_properties as $property => $value) {
      $schema = array();
      // Add property to database schema.
      eck_add_property_schema($schema, $entity_type, $property);
      foreach ($schema['fields'] as $field => $spec) {
        db_add_field("eck_{$entity_type->name}", $field, $spec);
      }
      foreach ($schema['indexes'] as $index => $fields) {
        db_add_index("eck_{$entity_type->name}", $index, $fields);
      }
    }

    db_merge('eck_entity_type')
      ->key(array('name' => $entity_type->name))
      ->fields(array('properties' => serialize($properties)))
      ->execute();

    // Process custom properties.
    $removed_properties = array_diff_key($entity_type->custom_properties, $custom_properties);
    
    foreach ($removed_properties as $property => $value) {
      db_drop_field("eck_{$entity_type_name}", $property);
      unset($entity_type->custom_properties[$property]);
    }
   
    if (!empty($state['values']['add']['type']) && !empty($state['values']['add']['name'])) {
      // Add database column.
      $spec = eck_get_custom_property_schema($state['values']['add']);
      db_add_field("eck_{$entity_type_name}", $state['values']['add']['name'], $spec);

      $entity_type->custom_properties += array(
        $state['values']['add']['name'] => array(
          'type' => $state['values']['add']['type'],
          'label' => $state['values']['add']['label'],
        )
      );
    }

    db_merge('eck_entity_type')
      ->key(array('name' => $entity_type_name))
      ->fields(array('custom_properties' => serialize($entity_type->custom_properties)))
      ->execute();

    // Clear info caches in order to pick up newly created entities.
    drupal_get_schema(NULL, TRUE);
    entity_info_cache_clear();
  }
}
/**
 * Delete the entity type
 * 
 * @param $entity_type 
 *  (String) entity type to be deleted 
 */
function eck__entity_type__delete($entity_type){
  module_load_include('inc', 'eck', 'eck.bundle');
  //delete all the bundles from this entity type
  $bundles = eck__bundle__load($entity_type->name);
  foreach($bundles as $bundle){
    eck__bundle__delete($entity_type, $bundle);
  }
  
  //delete the entity type record from the eck table
  db_delete('eck_entity_type')
  ->condition('name', $entity_type->name)
  ->execute();
  
  // then delete the entity type table
  db_drop_table('eck_'.$entity_type->name);
  drupal_set_message("Entity type '{$entity_type->name}' has been deleted");
  //return "<h1>Deletion Completed</h1> <h3>Entity type '{$entity_type}' has been deleted";
}

function eck__entity_type__delete_form($form, &$state, $entity_type){
  $form['entity_type'] = 
  array(
  '#type' => 'value',
  '#value' => $entity_type
  );
  
  $form['submit_redirect'] =
  array(
    '#type' => 'value',
    '#value' => "admin/structure/eck",
  );
  
  $message = t("Are you sure that you want to delete the entity type '{$entity_type->name}'");

  $caption = t("All of the data (entities and bundles) from this entity type 
  will be deleted. This action cannot be undone.");

  return 
  confirm_form($form, $message, 
  "admin/structure/eck", $caption, t('Delete'));
}

function eck__entity_type__delete_form_submit($form, &$state){
  $entity_type = $state['values']['entity_type'];
  $submit_redirect = $state['values']['submit_redirect'];
  
  // Ok, lets delete the entity type
  
  eck__entity_type__delete($entity_type);
  
  $state['redirect'] = $submit_redirect;
}

/**
 * Create the default schema for an entity type.
 *
 * @param $entity_type
 *  (Object) entity type as returned by eck__entity_type__load
 *
 * Passthrough for hook_schema().
 */
function eck__entity_type__schema($entity_type) {
  $schema = array(
    'description' => "The base table for a(n) {$entity_type->name}.",
    'fields' => array(
      'id' => array(
        'description' => "The primary identifier for a(n) {$entity_type->name}.",
        'type' => 'serial',
        'not null' => TRUE,
      ),
      'type' => array(
        'description' => 'The bundle of the entity',
        'type' => 'varchar',
        'default' => '',
        'length' => 255,
        'not null' => TRUE,
      )
    ),
    'primary key' => array('id'),
  );
  
  // Add properties to schema definition.
  module_load_include('inc', 'eck', 'eck.properties');
  foreach (eck_default_properties() as $property => $label) {
    if (!empty($entity_type->properties[$property])) {
      eck_add_property_schema($schema, $entity_type, $property);
    }
  }

  // Add custom properties.
  foreach ($entity_type->custom_properties as $name => $info) {
    $schema['fields'][$name] = eck_get_custom_property_schema($info);
  }

  // Allow other modules to alter the schema.
  drupal_alter('eck_schema', $schema, $entity_type);

  return $schema;
}

/**
 * Generate the entity info for a specific entity
 *
 * @param $entity_type
 *  (Object) as returned by eck__entity_type__load
 */
function eck__entity_type__info($entity_type) {
  module_load_include('inc', 'eck', 'eck.bundle');
  $info = array();
  $entity_type_label = $entity_type->label;

  if (!drupal_autoload_class($entity_class = eck_get_class_name($entity_type->name, 'Entity'))) {
    $entity_class = 'EckEntity';
  }
  if (!drupal_autoload_class($controller_class = eck_get_class_name($entity_type->name, 'Controller'))) {
    $controller_class = 'EckController';
  }

  $info[$entity_type->name] = array(
    'label' => t($entity_type_label),
    'base table' => "eck_{$entity_type->name}",
    'entity class' => $entity_class,
    'controller class' => $controller_class,
    'module' => 'eck',
    'fieldable' => TRUE,
    'entity keys' => array(
      'id' => 'id',
      'bundle' => 'type'
    ),
    'label callback' => 'eck__entity__label',
    'uri callback' => 'eck__entity__uri',
    // Bundles are defined by the entity types below.
    'bundles' => array(),
    // Bundle keys tell the FieldAPI how to extract information from the bundle objects.
    'bundle keys' => array(
      'bundle' => 'type'
    ),
    // I guess we need at least one view mode for entity_view_modes (the module) to work.
    'view modes' => array(
      'teaser' => array(
          'label' => t('Teaser'),
          'custom settings' => TRUE,
      )
    )
  );

  foreach (eck__bundle__load($entity_type->name) as $bundle) {
    $bundle_label = $bundle->label;
    $path = "admin/structure/eck/{$entity_type->name}/{$bundle->name}";
    $info[$entity_type->name]['bundles'][$bundle->name] = array(
      'label' => $bundle_label,
      'admin' => array(
        'path' => $path,
        'args' => array('entity_type' => 3, 'bundle' => 4),
        'access arguments' => array('administer entities')
      ),
      'crud' =>
      array(
        'add' => 
        array(
          'path' => $path."/add", 
          'args'=> array('entity_type' => 3, 'bundle' => 4)
        ),
        'edit' => 
        array(
          'path' => $path."/%/edit",
          'args' => array('entity_type' => 3, 'bundle' => 4, 'id' => 5)
        ),
        'delete' => 
        array(
          'path' => $path."/%/delete",
          'args' => array('entity_type' => 3, 'bundle' => 4, 'id' => 5)
        ),
        'view' => 
        array(
          'path' => "{$entity_type->name}/{$bundle->name}/%",
          'args' => array('entity_type' => 0, 'bundle' => 1, 'id' => 2)
        )
      )
    );
  }
  

  return $info;
}

/**
 * Entity Type specific implementation of property info alter.
 */
function eck__entity_type__property_info(&$info, $entity_type_object) {
   $properties = &$info['properties'];
  
  //@TODO Shouldn't we be checking on the properies array of our entity_type_object.. maybe entity api
  //is correctly doing this automatically (CHECK)
  if (isset($properties['uid'])) {
    $properties['uid']['label'] = t('User');
    $properties['uid']['type'] = 'user';
    $properties['uid']['description'] = t('The author of this entity.');
  }
  if (isset($properties['created'])) {
    $properties['created']['label'] = t('Created');
    $properties['created']['type'] = 'date';
    $properties['created']['description'] = t('The Unix timestamp when the entity has been created.');
  }
  if (isset($properties['changed'])) {
    $properties['changed']['label'] = t('Changed');
    $properties['changed']['type'] = 'date';
    $properties['changed']['description'] = t('The Unix timestamp when the entity was most recently saved.');
  }

  // Add custom properties.
  foreach ($entity_type_object->custom_properties as $name => $property) {
    $properties[$name]['label'] = t($property['label']);
    $properties[$name]['type'] = $property['type'];
    //$properties[$name]['description'] = '';
  }
}

function eck__entity_type__custom_property_label($property) {
  $type = $property['type'];
  if (!in_array($type, array('text', 'decimal', 'integer', 'date'))) {
    $type = l($type, "admin/structure/eck/{$type}");
  }
  return $property['label'] . ' (' . t('type: !type', array('!type' => $type)) . ')';
}
