<?php

/**
 * @file
 * Implements dialogs for node reference and user reference fields.
 */

/**
 * Implements hook_references_dialog_widgets().
 */
function references_dialog_references_dialog_widgets() {
  return array(
    'node_reference_autocomplete' => array(
      'element_type' => 'textfield',
      'dialog_form' => 'node_reference_dialog_form',
      'entity_type' => 'node',
      'format' => '$label [nid: $entity_id]',
      'views_query' => 'references_dialog_node_reference_views_query',
      'operations' => array(
        'search' => array(
          'function' => 'references_dialog_get_field_search_links',
          'title' => t('Search Dialog'),
        ),
        'edit' => array(
          'function' => 'references_dialog_node_reference_edit_link',
          'title' => t('Edit dialog'),
        ),
        'add' => array(
          'function' => 'references_dialog_node_reference_add_link',
          'title' => t('Add dialog'),
        ),
      ),
    ),
    'user_reference_autocomplete' => array(
      'element_type' => 'textfield',
      'entity_type' => 'user',
      'format' => '$label [uid: $entity_id]',
      'operations' => array(
        'search' => array(
          'function' => 'references_dialog_get_field_search_links',
          'title' => t('Search Dialog'),
        ),
        'edit' => array(
          'function' => 'references_dialog_user_reference_edit_link',
          'title' => t('Edit dialog'),
        ),
        'add' => array(
          'function' => 'references_dialog_user_reference_add_link',
          'title' => t('Add dialog'),
        ),
      ),
    ),
    'entityreference_autocomplete' => array(
      'element_type' => 'textfield',
      'dialog_form' => 'entityreference_autocomplete_dialog_form',
      'views_query' => 'references_dialog_entityreference_views_query',
      'type_callback' => 'references_dialog_entityreference_get_type',
      'format' => '$label ($entity_id)',
      'operations' => array(
        'edit' => array(
          'function' => 'references_dialog_entityreference_edit_link',
          'title' => t('Edit dialog'),
        ),
        'add' => array(
          'function' => 'references_dialog_entityreference_add_link',
          'title' => t('Add dialog'),
        ),
        'search' => array(
          'function' => 'references_dialog_get_field_search_links',
          'title' => t('Search Dialog'),
        ),
      ),
    ),
  );
}

/**
 * Edit link callback for node references.
 */
function references_dialog_node_reference_edit_link($element, $widget_settings, $field, $instance) {
  if (isset($element['#default_value'])) {
    $node = node_load($element['#default_value']);
    if (node_access('update', $node)) {
      return array(
        array(
          'title' => t('Edit'),
          'href' => 'node/' . $element['#default_value'] . '/edit'
        ),
      );
    }
  }
  return array();
}

/**
 * Add link callback for node references.
 */
function references_dialog_node_reference_add_link($element, $widget_settings, $field, $instance) {
  $add_links = array();
  foreach ($field['settings']['referenceable_types'] as $type => $active) {
    if ($active !== 0) {
      $node_type = node_type_load($type);
      if (node_access('create', $node_type->type)) {
        $add_links[] = array(
          'title' => t('Create @type', array('@type' => $node_type->name)),
          'href' => 'node/add/' . strtr($type, array('_' => '-')),
        );
      }
    }
  }
  return $add_links;
}

/**
 * View query callback for node references.
 */
function references_dialog_node_reference_views_query($view, $instance, $field) {
  // We need to make sure that no entries that we can't add to our field shows
  // up, so we need to limit the data here.
  $types = array();
  foreach ($field['settings']['referenceable_types'] as $type => $active) {
    if ($active !== 0) {
      $types[] = $type;
    }
  }
  $view->query->add_where(0, "$view->base_table.type", $types);
}

/**
 * Edit callback for user references.
 */
function references_dialog_user_reference_edit_link($element, $widget_settings, $field, $instance) {
  global $user;
  $user_links = array();
  if (is_numeric($element['#default_value']) && user_load($element['#default_value']) &&
    user_access('access user profiles') &&
    (user_access('administer users') || $user->uid == $element['#default_value'])) {
    $user_links[] = array(
      'title' => t('Edit'),
      'href' => 'user/' . $element['#default_value'] . '/edit',
    );
  }
  return $user_links;
}

/**
 * Add link callback for user references.
 */
function references_dialog_user_reference_add_link($element, $widget_settings, $field, $instance) {
  $user_links = array();
  // Check permissions for adding users.
  if (user_access('administer users')) {
    $user_links[] = array(
      'title' => t('Create user'),
      'href' => 'admin/people/create'
    );
  }
  return $user_links;
}

/**
 * Edit link callback for entity references.
 */
function references_dialog_entityreference_edit_link($element, $widget_settings, $field, $instance) {
  if (isset($element['#default_value'])) {
    $entity_type = $field['settings']['target_type'];
    // Take "label (entity id)', match the id from parenthesis.
    if (preg_match("/.+\((\d+)\)/", $element['#default_value'], $matches)) {
        $value = $matches[1];
      }
    if (isset($value)) {
      $entity = current(entity_load($entity_type, array($value)));
      if (entity_access('update', $entity_type, $entity)) {
        return array(
          array(
            'title' => t('Edit'),
            'href' => references_dialog_get_admin_path($entity_type, 'edit', NULL, $entity),
          ),
        );
      }
    }
  }
  return array();
}

/**
 * Add link callback for entity references.
 */
function references_dialog_entityreference_add_link($element, $widget_settings, $field, $instance) {
  $add_links = array();
  $entity_type = $field['settings']['target_type'];
  $entity_info = entity_get_info($entity_type);
  $entity_bundles = array_keys($entity_info['bundles']);
  if (!empty($field['settings']['handler_settings']['target_bundles'])) {
    $bundles = $field['settings']['handler_settings']['target_bundles'];
  }
  elseif (isset($entity_info['bundles'])) {
    // If the entity target bundles is empty, it means all target bundles are allowed. Fill it all up!
    $bundles = $entity_bundles;
  }
  // Create a link for each allowed bundles.
  if (isset($bundles)) {
    foreach ($bundles as $bundle) {
      if (in_array($bundle, $entity_bundles) && $link = references_dialog_entityreference_link_helper($entity_type, $bundle)) {
        $add_links[] = $link;
      }
    }
  }
  return $add_links;
}

function references_dialog_entityreference_link_helper($entity_type, $bundle = NULL) {
  $wrapper = entity_metadata_wrapper($entity_type, NULL, array('bundle' => $bundle));
  $info = $wrapper->entityInfo();
  if (isset($bundle)) {
    $label = $info['bundles'][$bundle]['label'];
  }
  else {
    $label = $info['label'];
  }
  // We use entity_access here. We provide the bundle if this is a node type,
  // since node_access expects that to be passed to it as the entity when you
  // run node_access('create')
  if (entity_access('create', $entity_type, $entity_type == 'node' ? $bundle : NULL) &&
    $path = references_dialog_get_admin_path($entity_type, 'add', $bundle)) {
    $link = array(
      'title' => t('Create @type', array('@type' => $label)),
      'href' => $path,
    );
    return $link;
  }
  return FALSE;
}

/**
 * View query callback for entityreference references.
 */
function references_dialog_entityreference_views_query($view, $instance, $field) {
  // We need to make sure that no entries that we can't add to our field shows
  // up, so we need to limit the data here.
  $types = array();
  if (!empty($field['settings']['handler_settings']['target_bundles'])) {
    $entity_info = entity_get_info($field['settings']['target_type']);
    if (isset($entity_info['entity keys']['bundle'])) {
      // Taxonomy terms don't have their bundle in the table, so we handle them
      // specially, for now.
      if ($field['settings']['target_type'] == 'taxonomy_term') {
        $table = 'taxonomy_vocabulary';
        $column = 'machine_name';
      }
      else {
        $table = $view->base_table;
        $column = $entity_info['entity keys']['bundle'];
      }
      // Add the bundle property as a default.
      $view->query->add_where(0, "$table." . $column, array_values($field['settings']['handler_settings']['target_bundles']));
    }
  }
}

function references_dialog_entityreference_get_type($instance, $field) {
  return $field['settings']['target_type'];
}
