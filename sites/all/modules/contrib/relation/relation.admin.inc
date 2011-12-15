<?php

/**
 * @file
 * Administrative functions for Relation module.
 */

/**
 * List all relation_types (page callback).
 */
function relation_type_list() {
  $relation = relation_entity_info();
  $field_ui = module_exists('field_ui');
  $headers = array(
    array('data' => t('Name'), 'width' => '40%'),
    array('data' => t('Operations'), 'colspan' => $field_ui ? 5 : 3));
  $rows = array();
  foreach ($relation['relation']['bundles'] as $type => $relation_type) {
    $url = 'admin/structure/relation/manage/' . $type;
    $row = array(l($relation_type['label'], $url));
    $row[] = l(t('edit'), $url . '/edit');
    if ($field_ui) {
      $row[] =  l(t('manage fields'), $url . '/fields');
      $row[] =  l(t('display fields'), $url . '/display');
    }
    $row[] =  l(t('export'), $url . '/export');
    $row[] =  l(t('delete'), $url . '/delete');
    $rows[] = $row;
  }
  $output = array(
    '#theme' => 'table',
    '#header' => $headers,
    '#rows' => $rows,
    '#empty' => t('No relationship types available. <a href="@link">Add relationship type</a>.', array('@link' => url('admin/structure/relation/add'))),
  );
  return $output;
}

/**
 * Relation relation type bundle settings form.
 *
 * @param $relation_type
 *   Relation type machine name. If this is not provided, assume that we're
 *   creating a new relation type.
 */
function relation_type_form($form, &$form_state, $relation_type = array(), $op = 'edit') {
  $form['#write_record_keys'] = array();
  $form['#attached']['css'] = array(
    drupal_get_path('module', 'relation') . '/relation.admin.css',
  );

  if ($relation_type) {
    $relation_type = (object) $relation_type;
    if (empty($relation_type->in_code_only)) {
      $form['#write_record_keys'][] = 'relation_type';
    }
  }
  else {
    $relation_type = (object) array(
      'relation_type' => '',
      'label' => '',
      'reverse_label' => '',
      'bundles' => array(),
      'directional' => FALSE,
      'transitive' => FALSE,
      'r_unique' => FALSE,
      'min_arity' => 2,
      'max_arity' => 2,
      'source_bundles' => array(),
      'target_bundles' => array(),
    );
  }
  $form['labels'] = array(
    '#type' => 'container',
    '#attributes' => array(
      'class' => array('relation-type-form-table'),
    ),
    '#suffix' => '<div class="clearfix"></div>',
  );
  $form['labels']['name'] = array( // use 'name' for /misc/machine-name.js
    '#type'          => 'textfield',
    '#title'         => t('Label'),
    '#description'   => t('Display name of the relation type. This is also used as the predicate in natural language formatters (ie. if A is related to B, you get "A [label] B")'),
    '#default_value' => $relation_type->label,
    '#size'          => 40,
    '#required'      => TRUE,
  );
  $form['labels']['relation_type'] = array(
    '#type'          => 'machine_name',
    '#default_value' => $relation_type->relation_type,
    '#maxlength' => 32,
    '#disabled'      => $relation_type->relation_type,
    '#machine_name' => array(
      'source' => array('labels', 'name'),
      'exists' => 'relation_type_load',
    ),
  );
  $form['labels']['reverse_label'] = array(
    '#type'          => 'textfield',
    '#size'          => 40,
    '#title'         => t('Reverse label'),
    '#description'   => t('Reverse label of the relation type. This is used as the predicate by formatters of directional relations, when you need to display the reverse direction (ie. from the target entity to the source entity). If this is not supplied, the forward label is used.'),
    '#default_value' => $relation_type->reverse_label,
    '#states' => array(
      'visible' => array(   // action to take.
        ':input[name="directional"]' => array('checked' => TRUE),
      ),
    ),
  );
  $form['directional'] = array(
    '#type'           => 'checkbox',
    '#title'          => 'Directional',
    '#description'   => t('A directional relation is one that does not imply the reverse relation. For example, a "likes" relation is directional (A likes B does not neccesarily mean B likes A), whereas a "similar to" relation is non-directional (A similar to B implies B similar to A. Non-directional relations are also known as symmetric relations.'),
    '#default_value'  => $relation_type->directional,
  );
  // More advanced options, hide by default.
  $form['advanced'] = array(
    '#type' => 'fieldset',
    '#title' => t('Advanced options'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
    '#weight' => 50,
    '#tree' => TRUE,
  );
  $form['advanced']['transitive'] = array(
    '#type'           => 'checkbox',
    '#title'          => 'Transitive',
    '#description'   => t('A transitive relation implies that the relation passes through intermediate entities (ie. A=>B and B=>C implies that A=>C). For example "Ancestor" is transitive: your ancestor\'s ancestor is also your ancestor. But a "Parent" relation is non-transitive: your parent\'s parent is not your parent, but your grand-parent.'),
    '#default_value'  => $relation_type->transitive,
  );
  $form['advanced']['r_unique'] = array(
    '#type'           => 'checkbox',
    '#title'          => t('Unique'),
    '#description'    => t('Whether relations of this type are unique (ie. they can not contain exactly the same end points as other relations of this type).'),
    '#default_value'  => $relation_type->r_unique,
  );
  // these should probably be changed to numerical (validated) textfields.
  $options = array('2' => '2', '3' => '3', '4' => '4', '5' => '5', '6' => '6', '7' => '7', '8' => '8');
  $form['advanced']['min_arity'] = array(
    '#type' => 'select',
    '#title' => t('Minimum Arity'),
    '#options' => $options,
    '#description' => t('Minimum number of entities joined by relations of this type (e.g. three siblings in one relation). <em>In nearly all cases you will want to leave this set to 2</em>.'),
    '#default_value' => $relation_type->min_arity ? $relation_type->min_arity : 2,
    '#states' => array(
      'disabled' => array(   // action to take.
        ':input[name="directional"]' => array('checked' => TRUE),
      ),
    ),
  );

  $options = array('2' => '2', '3' => '3', '4' => '4', '5' => '5', '6' => '6', '7' => '7', '8' => '8', '0' => t('Infinite'));
  $form['advanced']['max_arity'] = array(
    '#type' => 'select',
    '#title' => t('Maximum Arity'),
    '#options' => $options,
    '#description' => t('Maximum number of entities joined by relations of this type. <em>In nearly all cases you will want to leave this set to 2</em>.'),
    '#default_value' => isset($relation_type->max_arity) ? $relation_type->max_arity : 2,
    '#states' => array(
      'disabled' => array(   // action to take.
        ':input[name="directional"]' => array('checked' => TRUE),
      ),
    ),
  );
  $counter = 0;
  foreach (entity_get_info() as $entity_type => $entity) {
    $bundles[$entity['label']]["$entity_type:*"] = 'all ' . $entity['label'] . ' bundles';
    $counter += 2;
    if (isset($entity['bundles'])) {
      foreach ($entity['bundles'] as $bundle_id => $bundle) {
        $bundles[$entity['label']]["$entity_type:$bundle_id"] = $bundle['label'];
        $counter++;
      }
    }
  }
  $form['endpoints'] = array(
    '#type' => 'container',
    '#attributes' => array(
      'class' => array('relation-type-form-table'),
    ),
    '#suffix' => '<div class="clearfix"></div>',
  );
  $form['endpoints']['source_bundles'] = array(
    '#type'          => 'select',
    '#title'         => 'Available source bundles',
    '#options'       => $bundles,
    '#size'          => max(12, $counter),
    '#default_value' => $relation_type->source_bundles,
    '#multiple'      => TRUE,
    '#description'   => 'Bundles that are not selected will not be available as sources for directional, or end points of non-directional relations relations. Ctrl+click to select multiple. Note that selecting all bundles also include bundles not yet created for that entity type.',
  );
  $form['endpoints']['target_bundles'] = array(
    '#type'          => 'select',
    '#title'         => 'Available target bundles',
    '#options'       => $bundles,
    '#size'          => max(12, $counter),
    '#default_value' => $relation_type->target_bundles,
    '#multiple'      => TRUE,
    '#description'   => 'Bundles that are not selected will not be available as targets for directional relations. Ctrl+click to select multiple.',
    '#states' => array(
      '!visible' => array(   // action to take.
        ':input[name="directional"]' => array('checked' => FALSE),
      ),
    ),
  );
  $form['submit'] = array(
    '#type' => 'submit',
    '#weight' => 100,
    '#value' => t('Save'),
  );
  return $form;
}

/**
 * Submit data from bundle settings page.
 */
function relation_type_form_submit($form, &$form_state) {
  $relation_type = $form_state['values']['relation_type'];
  $min_arity = $form_state['values']['directional'] ? 2 : $form_state['values']['advanced']['min_arity'];
  $max_arity = $form_state['values']['directional'] ? 2 : $form_state['values']['advanced']['max_arity'];
  $record = array(
    'relation_type'   => $relation_type,
    'min_arity'   => $min_arity,
    'max_arity'   => $max_arity,
    'label' => $form_state['values']['name'],
    'reverse_label' => $form_state['values']['reverse_label'],
    'directional' => $form_state['values']['directional'],
    'transitive' => $form_state['values']['advanced']['transitive'],
    'r_unique' => $form_state['values']['advanced']['r_unique'],
    'source_bundles' => $form_state['values']['source_bundles'],
    'target_bundles' => $form_state['values']['target_bundles'],
  );
  relation_type_save($record, $form['#write_record_keys']);
  $form_state['redirect'] = "admin/structure/relation/edit/$relation_type";

  drupal_set_message(t('The %relation_type relation type has been saved.', array('%relation_type' => $relation_type)));
}

/**
 * Menu callback; deletes a single relation type.
 */
function relation_type_delete_confirm($form, &$form_state, $relation_type) {
  $form['relation_type'] = array('#type' => 'value', '#value' => $relation_type->relation_type);
  $form['label'] = array('#type' => 'value', '#value' => $relation_type->label);

  $message = t('Are you sure you want to delete the %label relation type?', array('%label' => $relation_type->label));
  $caption = '';

  $num_relations = relation_query()
    ->propertyCondition('relation_type', $relation_type->relation_type)
    ->count()
    ->execute();
  if ($num_relations) {
    $caption .= '<p>' . format_plural($num_relations,
      'The %label relation type is used by 1 relation on your site. If you remove this relation type, you will not be able to edit  %label relations and they may not display correctly.',
      'The %label relation type is used by @count relations on your site. If you remove %label, you will not be able to edit %label relations and they may not display correctly.',
      array('%label' => $relation_type->label, '@count' => $num_relations)) . '</p>';
  }

  $caption .= '<p>' . t('This action cannot be undone.') . '</p>';

  return confirm_form($form, $message, 'admin/structure/relation', $caption, t('Delete'));
}

/**
 * Process relation type delete confirm submissions.
 */
function relation_type_delete_confirm_submit($form, &$form_state) {
  relation_type_delete($form_state['values']['relation_type']);

  $t_args = array('%label' => $form_state['values']['label']);
  drupal_set_message(t('The %label relation type has been deleted.', $t_args));
  watchdog('relation', 'Deleted the %label relation type.', $t_args, WATCHDOG_NOTICE);

  // TODO: relation_types_rebuild() ?;
  menu_rebuild();

  $form_state['redirect'] = 'admin/structure/relation';
  return;
}

/**
 * Generate relations
 */
function relation_generate_form($form, &$form_state) {
  $types = relation_get_types();

  if (empty($types)) {
    $form['explanation']['#markup'] = t("Before you can generate relations, you need to define one or more !link.", array("!link" => l(t('relation types'), 'admin/structure/relation')));
    return $form;
  }
  foreach ($types as $id => $type) {
    $types[$id] = $type->label;
  }

  $form['relation_types'] = array(
    '#type' => 'checkboxes',
    '#title' => t('Relation types'),
    '#description' => t('Select relation types to create relations from. If no types are selected, relations will be generated for all types.'),
    '#options' => $types,
  );
  $form['relation_number'] = array(
    '#type' => 'textfield',
    '#title' => t('How many relations would you like to generate of each type?'),
    '#default_value' => 10,
    '#size' => 10,
  );
  $form['relation_kill'] = array(
    '#type' => 'checkbox',
    '#title' => t('Kill all existing relations first.'),
  );

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Generate'),
  );
  return $form;
}

function relation_generate_form_submit($form, &$form_state) {
  $number = $form_state['values']['relation_number'];
  $types = $form_state['values']['relation_types'];
  $kill = $form_state['values']['relation_kill'];
  if (is_numeric($number) && $number > 0) {
    include_once drupal_get_path('module', 'relation') . '/relation.drush.inc';
    $types = array_keys(array_filter($types));
    $rids = relation_generate_relations($number, $types, $kill);
  }
}

/**
 * Menu callback for admin/content/relation. Displays all relations on the site.
 */
function relation_admin_content() {
  // Set up header row.
  $header = array(
    'label' => array('data' => t('Title'), 'field' => 'r.rid', 'sort' => 'asc'),
    'type' => array('data' => t('Type'), 'field' => 'r.relation_type'),
    t('Relation'),
    'operations' => array('data' => t('Operations'), 'colspan' => '2'),
  );

  // Grab all relations.
  $query = db_select('relation', 'r')->extend('PagerDefault')->extend('TableSort');
  $query->join('relation_type', 'rt', 'r.relation_type = rt.relation_type');
  $query->fields('r', array('rid', 'relation_type'))
    ->fields('rt', array('directional'))
    ->limit(50)
    ->orderByHeader($header);
  $relations = $query->execute();

  return theme('relation_admin_content', array('relations' => $relations, 'header' => $header));
}

/**
 * Generate a table of all relations on this site.
 */
function theme_relation_admin_content($variables) {
  $relations = $variables['relations'];
  $header = $variables['header'];

  $rows = array();
  if (empty($relations)) {
    // Give a message if there are no relations returned.
    $message = t('There are currently no relations on your site.');

    $rows[] = array(
      array('data' => $message, 'colspan' => 5),
    );
  }
  else {
    foreach ($relations as $relation) {
      // Load the relation.
      $r = relation_load($relation->rid);
      // Get the endpoints for this relation.
      $endpoints = field_get_items('relation', $r, 'endpoints');

      if (!empty($endpoints)) {
        $relation_entities = array();
        foreach ($endpoints as $endpoint) {
          $entities = entity_load($endpoint['entity_type'], array($endpoint['entity_id']));
          $entity = reset($entities);
          $title = entity_label($endpoint['entity_type'], $entity);
          $path = entity_uri($endpoint['entity_type'], $entity);

          // Logic to process how the different entities return a uri.
          // see this issue: http://drupal.org/node/1057242
          if ($endpoint['entity_type'] == 'file') {
            $path = array('path' => file_create_url($path));
          }
          if ($endpoint['entity_type'] == 'taxonomy_vocabulary') {
            $path = array('path' => 'admin/structure/taxonomy/' . $entity->machine_name);
          }
          $relation_entities[] = array('title' => $title, 'path' => $path['path']);
        }
      }

      // Build the column for the relation entities.
      $relation_column = array();
      foreach ($relation_entities as $entity) {
        $relation_column[] = l($entity['title'], $entity['path']);
      }

      // Build the rows to pass to the table theme function.
      // Directional is implemented, not sure how well it works.
      $rows[] = array(
        l(t('Relation') . ' ' . $relation->rid, 'relation/' . $relation->rid),
        $relation->relation_type,
        implode(($relation->directional) ? " -> " : " -- ", $relation_column),
        user_access('edit relations') ? l(t('Edit'), 'relation/' . $relation->rid . '/edit') : '',
        user_access('delete relations') ? l(t('Delete'), 'relation/' . $relation->rid . '/delete') : '',
      );
    }
  }

  return theme('table', array('header' => $header, 'rows' => $rows)).theme('pager');
}

/**
 * Page callback to import a relation.
 */
function relation_type_import($form) {
  $form['name'] = array(
    '#type' => 'textfield',
    '#title' => t('Relation type name'),
    '#description' => t('Enter the machine name to use for this relation type if it is different from the relation type to be imported. Leave blank to use the name of the relaiton type below.'),
  );
  $form['relation_type'] = array(
    '#type' => 'textarea',
    '#rows' => 10,
  );
  $form['import'] = array(
    '#type' => 'submit',
    '#value' => t('Import'),
  );
  return $form;
}

function relation_type_import_validate($form, &$form_state) {
  if (substr($form_state['values']['relation_type'], 0, 5) == '<?php') {
    $form_state['values']['relation_type'] = substr($form_state['values']['relation_type'], 5);
  }
  ob_start();
  eval($form_state['values']['relation_type']);
  ob_end_clean();
  if (!is_object($relation_type)) {
    return form_error($form['relation_type'], t('Unable to interpret relation type code.'));
  }
  // relation_type name must be alphanumeric or underscores, no other punctuation.
  if (!empty($form_state['values']['name']) && preg_match('/[^a-zA-Z0-9_]/', $form_state['values']['name'])) {
    form_error($form['name'], t('Relation type name must be alphanumeric or underscores only.'));
  }

  if ($form_state['values']['name']) {
    $relation_type->relation_type = $form_state['values']['name'];
  }

  if (relation_type_load($relation_type->relation_type)) {
    form_set_error('name', t('A relation type by that name already exists; please choose a different name'));
  }
  $form_state['relation_type'] = $relation_type;
}

function relation_type_import_submit($form, &$form_state) {
  relation_type_save($form_state['relation_type']);
  $relation_type = $form_state['relation_type']->relation_type;
  drupal_set_message(t('Successfully imported @relation_type', array('@relation_type' => $relation_type)));
  $form_state['redirect'] = 'admin/structure/relation/manage/' . $relation_type;
}
