<?php

/**
 * @file
 * This file contains documentation on hooks provided by this module.
 */

/**
 * @defgroup references_dialog API Hooks
 * @{
 */

/**
 * Define a widget to which you want to attach add, search or edit
 * links.
 *
 * @return
 *   An array keyed by the widget you want to attach links to. This array
 *   should contain the following keys:
 *   - 'element_type': The type of FAPI element the widget is.
 *   - 'format': The format in which the data should be inserted as a value
 *     into the form element. The following patterns are provided, $label, $entity_id and $entity_type.
 *   - operations: An array of available operations.
 *     Usually search, edit and add should be provided.
 *     Each array should contain a callback funciton to render the links and a
 *     label that will be shown on the widget administration page.
 */
function hook_references_dialog_widgets() {
  return array(
    'node_reference_autocomplete' => array(
      'element_type' => 'textfield',
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
  );
}

/**
 * Provide the admin paths for where different entity types can be edited.
 * This is used by references dialog to work properly with the entity reference module.
 * @return
 *   An array keyed by entity type containing the following keys:
 *   - 'add': Where the admin page is located to add a new entity of this type.
 *   - 'edit': Where the admin page is located to edit an entity of this type.
 *   You can use the following replacement patterns: [bundle-sanitized], [entity_id], [bundle]
 */
function hook_references_dialog_entity_admin_paths() {
  return array(
    'node' => array(
      'add' => 'node/add/[bundle-sanitized]',
      'edit' => 'node/[entity_id]/edit',
    ),
  );
}

/**
 * Return all 'attachables' that can be used together with views. An attachable
 * is just a name that the views search reference plugin uses to know what
 * to attach itself to. You can define your own attachables if you want to
 * use the references dialog search functionality outside of the realm of fields.
 *
 * @return
 *   An array keyed by entity and a unique name containing the following:
 *   - 'label': The label to use in views.
 */
function hook_references_dialog_search_attachables() {
  // Return search views attachables for nodes.
  return array(
    'node' => array(
      'mysearchplugin' => array(
        'label' => t('A pretty label'),
      ),
    ),
  );
}

/**
 * @} End of "addtogroup hooks".
 */
