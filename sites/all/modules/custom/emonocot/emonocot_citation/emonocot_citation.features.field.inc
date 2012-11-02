<?php
/**
 * @file
 * fgjrst.features.field.inc
 */

/**
 * Implements hook_field_default_fields().
 */
function emonocot_citation_field_default_fields() {
  $fields = array();

  // Exported field: 'node-biblio-field_emonocot_acknowledgement'
  $fields['node-biblio-field_emonocot_acknowledgement'] = array(
    'field_config' => array(
      'active' => '1',
      'cardinality' => '1',
      'deleted' => '0',
      'entity_types' => array(),
      'field_name' => 'field_emonocot_acknowledgement',
      'foreign keys' => array(
        'format' => array(
          'columns' => array(
            'format' => 'format',
          ),
          'table' => 'filter_format',
        ),
      ),
      'indexes' => array(
        'format' => array(
          0 => 'format',
        ),
      ),
      'module' => 'text',
      'settings' => array(
        'max_length' => '1000',
      ),
      'translatable' => '0',
      'type' => 'text',
    ),
    'field_instance' => array(
      'bundle' => 'biblio',
      'default_value' => NULL,
      'deleted' => '0',
      'description' => '',
      'display' => array(
        'default' => array(
          'label' => 'above',
          'module' => 'text',
          'settings' => array(),
          'type' => 'text_default',
          'weight' => 12,
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
      'field_name' => 'field_emonocot_acknowledgement',
      'label' => 'eMonocot acknowledgement',
      'required' => 0,
      'settings' => array(
        'linkit' => array(
          'enable' => 0,
          'insert_plugin' => '',
        ),
        'text_processing' => '1',
        'user_register_form' => FALSE,
      ),
      'widget' => array(
        'active' => 1,
        'module' => 'text',
        'settings' => array(
          'size' => '250',
        ),
        'type' => 'text_textfield',
        'weight' => '20',
      ),
    ),
  );

  // Translatables
  // Included for use with string extractors like potx.
  t('eMonocot acknowledgement');

  return $fields;
}
