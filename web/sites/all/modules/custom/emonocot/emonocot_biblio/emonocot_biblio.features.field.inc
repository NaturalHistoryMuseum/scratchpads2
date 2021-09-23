<?php
/**
 * @file
 * emonocot_biblio.features.field.inc
 */

/**
 * Implements hook_field_default_fields().
 */
function emonocot_biblio_field_default_fields() {
  $fields = array();

  // Exported field: 'node-biblio-field_containing_work'
  $fields['node-biblio-field_containing_work'] = array(
    'field_config' => array(
      'active' => '1',
      'cardinality' => '1',
      'deleted' => '0',
      'entity_types' => array(),
      'field_name' => 'field_containing_work',
      'foreign keys' => array(
        'nid' => array(
          'columns' => array(
            'nid' => 'nid',
          ),
          'table' => 'node',
        ),
      ),
      'indexes' => array(
        'nid' => array(
          0 => 'nid',
        ),
      ),
      'module' => 'node_reference',
      'settings' => array(
        'referenceable_types' => array(
          'biblio' => 'biblio',
          'location' => 0,
          'media_gallery' => 0,
          'page' => 0,
          'specimen_observation' => 0,
          'spm' => 0,
        ),
        'view' => array(
          'args' => array(),
          'display_name' => '',
          'view_name' => '',
        ),
      ),
      'translatable' => '0',
      'type' => 'node_reference',
    ),
    'field_instance' => array(
      'bundle' => 'biblio',
      'default_value' => NULL,
      'deleted' => '0',
      'description' => '',
      'display' => array(
        'default' => array(
          'label' => 'above',
          'module' => 'node_reference',
          'settings' => array(),
          'type' => 'node_reference_default',
          'weight' => 13,
        ),
        'linked_node' => array(
          'label' => 'above',
          'settings' => array(),
          'type' => 'hidden',
          'weight' => 0,
        ),
        'teaser' => array(
          'label' => 'above',
          'settings' => array(),
          'type' => 'hidden',
          'weight' => 0,
        ),
      ),
      'entity_type' => 'node',
      'entityconnect_unload_add' => '0',
      'entityconnect_unload_edit' => '0',
      'field_name' => 'field_containing_work',
      'label' => 'Containing Work',
      'required' => 0,
      'settings' => array(
        'user_register_form' => FALSE,
      ),
      'widget' => array(
        'active' => 1,
        'module' => 'node_reference',
        'settings' => array(
          'autocomplete_match' => 'contains',
          'autocomplete_path' => 'node_reference/autocomplete',
          'size' => '60',
        ),
        'type' => 'node_reference_autocomplete',
        'weight' => '21',
      ),
    ),
  );

  // Exported field: 'node-biblio-field_licence_for_content_derive'
  $fields['node-biblio-field_licence_for_content_derive'] = array(
    'field_config' => array(
      'active' => '1',
      'cardinality' => '1',
      'deleted' => '0',
      'entity_types' => array(),
      'field_name' => 'field_licence_for_content_derive',
      'foreign keys' => array(),
      'indexes' => array(
        'licence' => array(
          0 => 'licence',
        ),
      ),
      'module' => 'creative_commons',
      'settings' => array(
        'allowed_values' => array(
          1 => '1',
          8 => '8',
          9 => '9',
          2 => '2',
          3 => '3',
          4 => '4',
          5 => '5',
          6 => '6',
          7 => '7',
        ),
        'jurisdiction' => '',
      ),
      'translatable' => '0',
      'type' => 'creative_commons',
    ),
    'field_instance' => array(
      'bundle' => 'biblio',
      'default_value' => NULL,
      'deleted' => '0',
      'description' => '',
      'display' => array(
        'default' => array(
          'label' => 'above',
          'module' => 'creative_commons',
          'settings' => array(),
          'type' => 'creative_commons_small_image',
          'weight' => 14,
        ),
        'linked_node' => array(
          'label' => 'above',
          'settings' => array(),
          'type' => 'hidden',
          'weight' => 0,
        ),
        'teaser' => array(
          'label' => 'above',
          'settings' => array(),
          'type' => 'hidden',
          'weight' => 0,
        ),
      ),
      'entity_type' => 'node',
      'field_name' => 'field_licence_for_content_derive',
      'label' => 'Licence for content derived from this work',
      'required' => 0,
      'settings' => array(
        'user_register_form' => FALSE,
      ),
      'widget' => array(
        'active' => 0,
        'module' => 'creative_commons',
        'settings' => array(),
        'type' => 'select',
        'weight' => '22',
      ),
    ),
  );

  // Translatables
  // Included for use with string extractors like potx.
  t('Containing Work');
  t('Licence for content derived from this work');

  return $fields;
}
