<?php

/**
 * @file
 * Implements the Rules module API for Relation.
 */

/**
 * Implements hook_rules_event_info().
 */
function relation_rules_event_info() {
  return array(
    'relation_insert' => array(
      'label' => t('After saving a new relation'),
      'group' => t('Relation'),
      'variables' => array(
        'relation' => array('type' => 'relation', 'label' => t('relation'), 'description' => t('The relation.')),
      ),
    ),
    'relation_update' => array(
      'label' => t('After updating a relation'),
      'group' => t('Relation'),
      'variables' => array(
        'relation' => array('type' => 'relation', 'label' => t('relation'), 'description' => t('The relation.')),
        'relation_unchanged' => array('type' => 'relation', 'label' => t('unchanged relation'), 'description' => t('The unchanged relation.'), 'handler' => 'rules_events_entity_unchanged'),
      ),
    ),
    'relation_delete' => array(
      'label' => t('After deleting a relation'),
      'group' => t('Relation'),
      'variables' => array(
        'relation' => array('type' => 'relation', 'label' => t('relation')),
      ),
    ),
  );
}

/**
 * Implements hook_rules_action_info().
 */
function relation_rules_action_info() {
  $items = array(
    'relation_rules_load_related' => array(
      'label' => t('Loads related entities'),
      'group' => t('Relation'),
      'parameter' => array(
        'left' => array(
          'type' => 'entity',
          'label' => t('Entity'),
        ),
        'relation_type' => array(
          'type' => 'text',
          'label' => t('Relation type'),
          'options list' => 'relation_rules_get_type_options',
        ),
      ),
      'provides' => array(
        'endpoints' => array(
          'type' => 'list<entity>',
          'label' => t('List of related entities'),
        ),
      ),
    ),
    'relation_rules_fetch_endpoint' => array(
      'label' => t('Fetch relation endpoints'),
      'group' => t('Relation'),
      'description' => 'Fetch relation endpoint(s) of at a particular entity type.',
      'parameter' => array(
        'relation' => array(
          'type' => 'relation',
          'label' => t('Relation'),
          'restriction' => 'selector',
        ),
      ),
      'provides' => array(
        'endpoint_fetched' => array(
          'type' => 'entity',
          'label' => t('Fetched Endpoint'),
        ),
      ),
    ),
  );
  return $items;
}


/**
 * Options list callback for fetching relation types.
 */
function relation_rules_get_type_options() {
  $options = array();
  $types = relation_get_types();
  foreach ($types as $type) {
    $options[$type->relation_type] = $type->label;
  }
  return $options;
}

/**
 * Callback for creating a relation, in the form Rules wants it.
 */
function relation_rules_create($values = array()) {
  return relation_create($values['relation_type'], array());
}

/**
 * Access callback for creating a relation.
 *
 * For now, everyone has permission to trigger the creation of a relation.
 */
function relation_rules_access($op, $entity = NULL, $account = NULL) {
  return TRUE;
}

/**
 * Endpoint property getter callback.
 */
function relation_rules_get_endpoints($relation, array $options, $property_name, $entity_type) {
  $array = array();
  foreach (relation_get_endpoints($relation) as $entity_type => $entities) {
    foreach ($entities as $entity) {
      $array[] = entity_metadata_wrapper($entity_type, $entity);
    }
  }
  return $array;
}

/**
 * Entity-type specific property getter callback.
 */
function relation_rules_get_specific_endpoints($relation, array $options, $property_name, $entity_type, $info) {
  if ($info['endpoint_type'] == 'source') {
    if ($info['relation_directional']) {
      $endpoints = array($relation->endpoints[LANGUAGE_NONE][0]);
    }
    else {
      $endpoints = $relation->endpoints[LANGUAGE_NONE];
    }
  }
  else {
    $endpoints = array_slice($relation->endpoints[LANGUAGE_NONE], 1);
  }

  $array = array();
  foreach ($endpoints as $endpoint) {
    $entities = entity_load($endpoint['entity_type'], array($endpoint['entity_id']));
    $entity_id = entity_extract_ids($endpoint['entity_type'], reset($entities));
    $array[] = $entity_id[0];
  }
  return $array;
}

/**
 * Endpoint property setter callback.
 *
 * @param $data
 *   The relation object that we are going to modify.
 * @param $name
 *   Name of the provided Rules variable.
 * @param $endpoint_wrappers
 *   Array of entity wrappers that we are going to add to the relation object.
 */
function relation_rules_set_endpoints(&$relation = NULL, $name = NULL, $entity_wrappers = NULL) {
  // Check that we are creating a new relation. Updating existing relations
  // aren't supported.
  if (isset($relation->rid) || empty($entity_wrappers)) {
    return;
  }

  foreach ($entity_wrappers as $i => $entity_wrapper) {
    $entity = $entity_wrapper->value();
    $entity_type = $entity_wrapper->type();
    $id_key = $entity_wrapper->entityKey('id');
    $bundle_key = $entity_wrapper->entityKey('bundle');

    if (isset($entity->{$id_key})) {
      $relation->endpoints[LANGUAGE_NONE][$i] = array(
        'entity_type' => $entity_wrapper->type(),
        'entity_id' => $entity->{$id_key},
        'entity_bundle' => isset($entity->{$bundle_key}) ? $entity->{$bundle_key} : $entity_type,
        'r_index' => $i,
      );
    }
  }
}

/**
 * Related entities getter callback.
 */
function relation_rules_get_related_entities($entity, array $options, $name, $type, $info) {
  $source_entity = entity_metadata_wrapper($type, $entity);
  $source_entity_type = $source_entity->type();
  $source_entity_id = $source_entity->getIdentifier();
  $results = relation_query($source_entity_type, $source_entity_id)
    ->entityCondition('bundle', $info['relation_type'])
    ->range(0, 50)
    ->execute();
  $rids = array_keys($results);
  $entities_ids = array();
  if (!$rids) {
    return $entities_ids;
  }
  foreach (relation_load_multiple($rids) as $relation) {
    foreach ($relation->endpoints[LANGUAGE_NONE] as $endpoint) {
      if ($endpoint['entity_type'] == $info['target_type']) {
        $entities_ids[] = $endpoint['entity_id'];
      }
    }
  }
  return $entities_ids;
}

/**
 * Info alter callback for the load_related action.
 */
function relation_rules_load_related_info_alter(&$element_info, $element) {
  if (!empty($element->settings['relation_type'])) {
    // We only make this parameter available after we've selected the relation type. This way we
    // can limit the entity type list to only those relative to the selected relation.
    $element_info['parameter']['entity_type_op'] = array(
      'type' => 'text',
      'label' => t('Entity type'),
      'options list' => 'relation_rules_fetch_endpoint_type_options',
      'optional' => TRUE,
      'default value' => '',
      'description' => t('Optional: Select the specific type of entities to return. This will allow you to access their field/property data.'),
    );
    if (!empty($element->settings['entity_type_op'])) {
      // Set the returned entity type so we can access all the data.
      $element_info['provides']['endpoints']['type'] = 'list<' . $element->settings['entity_type_op'] . '>'; // more then one.
    }
  }
}

/**
 * Form alter callback for the load_related action.
 */
function relation_rules_load_related_form_alter(&$form, &$form_state, $options, RulesAbstractPlugin $element) {
  $first_step  = empty($element->settings['relation_type']);
  $form['reload'] = array(
    '#weight' => 5,
    '#type' => 'submit',
    '#name' => 'reload',
    '#value' => $first_step ? t('Continue') : t('Reload form'),
    '#submit' => array('rules_action_type_form_submit_rebuild'),
    '#ajax' => rules_ui_form_default_ajax(),
    '#description' => $first_step ? '' : t('Reload the form to change the entity types list.'),
  );

  if ($first_step) {
    // In the first step only show relevant parameters.
    foreach (element_children($form['parameter']) as $key) {
      if (($key != 'relation_type') && ($key != 'left')) {
        unset($form['parameter'][$key]);
      }
    }
    unset($form['submit']);
    unset($form['provides']);
  }
  // Add #ajax to the relation_type selection dropdown to reload the form.
  if (isset($form['parameter']['relation_type'])) {
    $form['parameter']['relation_type']['#ajax'] = rules_ui_form_default_ajax() + array(
      'event' => 'change',
      'trigger_as' => array('name' => 'reload'),
    );
  }
}

/**
 * Action callback loading all related entities.
 */
function relation_rules_load_related($source_entity, $relation_type, $entity_type_op = NULL) {
  $endpoints = array();
  $source_entity_type = $source_entity->type();
  $source_entity_id = $source_entity->getIdentifier();
  foreach ($source_entity->getPropertyInfo() as $property_name => $property) {
    if (isset($property['relation_type']) && $property['relation_type'] == $relation_type && isset($property['target_type'])) {
      $related_entities = $source_entity->$property_name->value();
      if (!empty($related_entities)) {
        foreach ($related_entities as $related_entity) {
          if (empty($entity_type_op) || $entity_type_op == $property['target_type']) {
            $endpoint_wrapper = entity_metadata_wrapper($property['target_type'], $related_entity);
            if ($endpoint_wrapper->type() != $source_entity_type || $endpoint_wrapper->getIdentifier() != $source_entity_id) {
              $endpoints[] = $related_entity;
            }
          }
        }
      }
    }
  }
  return array('endpoints' => $endpoints);
}

/**
 * Returns the options list of available endpoint entity types for the chosen relation type.
 */
function relation_rules_fetch_endpoint_type_options(RulesAbstractPlugin $element, $param_name = NULL) {
  $options = $types = array();
  // The parameter is optional
  if ($param_name = 'entity_type_op') {
    $options[''] = t('--All types--');
  }
  $all_entity_types = rules_entity_type_options();
  if (!empty($element->settings['relation_type'])) {
    $types[] = $element->settings['relation_type'];
  }
  elseif ($wrapper = $element->applyDataSelector($element->settings['relation:select'])) {
    // If we can: limit the list of entity types to those relative to the selected relation type.
    if (($info = $wrapper->info()) && !empty($info['bundle'])) {
      $types[] = $info['bundle'];
    }
  }
  $relation_types = relation_get_types($types);
  foreach ($relation_types as $name => $relation_type) {
    // Add the allowed source entity types to the list
    if (!empty($relation_type->source_bundles)) {
      foreach ($relation_type->source_bundles as $source_bundle) {
        list($entity_type, ) = explode(':', $source_bundle, 2);
        $options[$entity_type] = $all_entity_types[$entity_type];
      }
    }
    // Add the allowed target entity types to the list
    if (!empty($relation_type->target_bundles)) {
      foreach ($relation_type->target_bundles as $target_bundle) {
        list($entity_type, ) = explode(':', $target_bundle, 2);
        $options[$entity_type] = $all_entity_types[$entity_type];
      }
    }
  }
  return $options;
}

/**
 * Info alter callback for the fetch_endpoint action.
 */
function relation_rules_fetch_endpoint_info_alter(&$element_info, $element) {
  $element->settings += array('relation:select' => NULL);
  if ($wrapper = $element->applyDataSelector($element->settings['relation:select'])) {
    // We only make this parameter available after we've selected the relation. This way we
    // can limit the entity type list to only those relative to the selected relation.
    $element_info['parameter']['entity_type'] = array(
      'type' => 'text',
      'label' => t('Entity type'),
      'options list' => 'relation_rules_fetch_endpoint_type_options',
      'description' => t('Select the specific entity type to return.'),
    );
    if (!empty($element->settings['entity_type'])) {
      // Having a number parameter allows us to be flexible between returning a list or a single entity.
      $element_info['parameter']['number'] = array(
        'type' => 'integer',
        'label' => t('How many endpoints to return'),
        'default value' => 1,
        'description' => t('The number of enitites to return that match the above entity type criteria and in what form (single entity or a list). !zero returns a list containing every entity found; The default !one will return a single entity; !gt1 returns a list with maximum the specified number of entities.', array('!zero' => '<strong>0</strong>', '!one' => '<strong>1</strong>', '!gt1' => '<strong># &gt; 1</strong>')),
      );
      // Set the returned entity type so we can access all the data.
      if (!empty($element->settings['number']) && (1 == $element->settings['number'])) {
        $element_info['provides']['endpoint_fetched']['type'] = $element->settings['entity_type']; // only one.
      }
      else {
        $element_info['provides']['endpoint_fetched']['type'] = 'list<' . $element->settings['entity_type'] . '>'; // more then one.
      }
    }
  }
}

/**
 * Form alter callback for the fetch_endpoint action.
 */
function relation_rules_fetch_endpoint_form_alter(&$form, &$form_state, $options, RulesAbstractPlugin $element) {
  $first_step  = empty($element->settings['relation:select']);
  $second_step = (!$first_step && empty($element->settings['entity_type']));
  $form['reload'] = array(
    '#weight' => 5,
    '#type' => 'submit',
    '#name' => 'reload',
    '#value' => $first_step ? t('Continue') : t('Reload form'),
    '#limit_validation_errors' => array(array('parameter', 'relation')),
    '#submit' => array('rules_action_type_form_submit_rebuild'),
    '#ajax' => rules_ui_form_default_ajax(),
    '#description' => $first_step ? '' : t('Reload the form to change the entity/bundle types list.'),
  );
  // Use ajax and trigger as the reload button.
  $form['parameter']['relation']['settings']['relation:select']['#ajax'] = $form['reload']['#ajax'] + array(
    'event' => 'blur',
    'trigger_as' => array('name' => 'reload'),
  );

  if ($first_step  || $second_step) {
    // In the first step and second step only show relevant parameters.
    foreach (element_children($form['parameter']) as $key) {
      if (($key != 'relation') && !($second_step && ($key == 'entity_type'))) {
        unset($form['parameter'][$key]);
      }
    }
    unset($form['submit']);
    unset($form['provides']);
  }
  else {
    // Change the entity parameter to be not editable.
    $form['parameter']['relation']['settings']['#access'] = FALSE;
    $form['parameter']['relation']['info'] = array(
      '#prefix' => '<p>',
      '#markup' => t('<strong>Selected relation:</strong> %selector', array('%selector' => $element->settings['relation:select'])),
      '#suffix' => '</p>',
    );
    // Hide the reload button in case js is enabled and it's not the first step.
    $form['reload']['#attributes'] = array('class' => array('rules-hide-js'));
  }
  // Add #ajax to the entity_type selection dropdown to reload the form.
  if (isset($form['parameter']['entity_type'])) {
    $form['parameter']['entity_type']['#ajax'] = rules_ui_form_default_ajax() + array(
      'event' => 'change',
      'trigger_as' => array('name' => 'reload'),
    );
  }
  // Add #ajax to the number parameter to allow us to change the type of the provided variable.
  if (isset($form['parameter']['number'])) {
    $form['parameter']['number']['#ajax'] = rules_ui_form_default_ajax() + array(
      'event' => 'change',
      'trigger_as' => array('name' => 'reload'),
    );
  }

  // Disable #ajax for the 'relation:select' as it has troubles with lazy-loaded JS.
  // @TODO: Re-enable once JS lazy-loading is fixed in core.
  unset($form['parameter']['relation']['settings']['relation:select']['#ajax']);
}

/**
 * Action callback fetching a given number of endpoint entities for a particular relation.
 */
function relation_rules_fetch_endpoint($relation, $entity_type, $number = 1) {
  // Make sure we have the fully loaded relation entity.
  $loaded_relation = relation_load($relation->rid);
  // Load the endpoints
  $endpoints = field_get_items('relation', $loaded_relation, 'endpoints');

  $entity_ids = array();
  foreach ($endpoints as $endpoint) {
    // We only want to return entities of the selected type.
    if (!empty($endpoint['entity_type']) && $entity_type == $endpoint['entity_type']) {
      $entity_ids[] = $endpoint['entity_id'];
      if ($number == count($entity_ids)) break;
    }
  }
  if ($entity_ids) {
    $return = entity_load($entity_type, $entity_ids);
    // Return a list unless we are only supposed to return a single entity.
    if (1 == $number) {
      $return = reset($return);
      if (!$return) {
        throw new RulesEvaluationException('Unable to load relation endpoint of type "@type" for @entity with id "@id".', array('@type' => $entity_type, '@entity' => $relation->relation_type, '@id' => $relation->rid));
      }
    }
    return array('endpoint_fetched' => $return);
  }
  // We didn't find any entities in the relation that matched the provided conditions.
  return NULL;
}
