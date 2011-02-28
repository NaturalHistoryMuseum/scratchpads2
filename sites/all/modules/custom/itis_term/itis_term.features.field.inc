<?php

/**
 * Implementation of hook_field_default_fields().
 */
function itis_term_field_default_fields(){
  $fields = array();
  // Exported field: 'taxonomy_term-itis_vocabulary-field_rank'
  $fields['taxonomy_term-itis_vocabulary-field_rank'] = array(
    'field_config' => array(
      'active' => '1',
      'cardinality' => '1',
      'deleted' => '0',
      'entity_types' => array(),
      'field_name' => 'field_rank',
      'foreign keys' => array(),
      'indexes' => array(
        'value' => array(
          '0' => 'value'
        )
      ),
      'module' => 'list',
      'settings' => array(
        'allowed_values' => array(
          'Plant/Fungi' => array(
            'Kingdom' => 'Kingdom',
            'Subkingdom' => 'Subkingdom',
            'Division' => 'Division',
            'Subdivision' => 'Subdivision',
            'Class' => 'Class',
            'Subclass' => 'Subclass',
            'Order' => 'Order',
            'Suborder' => 'Suborder',
            'Family' => 'Family',
            'Subfamily' => 'Subfamily',
            'Tribe' => 'Tribe',
            'Subtribe' => 'Subtribe',
            'Genus' => 'Genus',
            'Subgenus' => 'Subgenus',
            'Section' => 'Section',
            'Subsection' => 'Subsection',
            'Species' => 'Species',
            'Subspecies' => 'Subspecies',
            'Variety' => 'Variety',
            'Subvariety' => 'Subvariety',
            'Form' => 'Form',
            'Subform' => 'Subform'
          ),
          'Animal' => array(
            'Kingdom' => 'Kingdom',
            'Subkingdom' => 'Subkingdom',
            'Phylum' => 'Phylum',
            'Subphylum' => 'Subphylum',
            'Superclass' => 'Superclass',
            'Class' => 'Class',
            'Subclass' => 'Subclass',
            'Infraclass' => 'Infraclass',
            'Superorder' => 'Superorder',
            'Order' => 'Order',
            'Suborder' => 'Suborder',
            'Infraorder' => 'Infraorder',
            'Superfamily' => 'Superfamily',
            'Family' => 'Family',
            'Subfamily' => 'Subfamily',
            'Tribe' => 'Tribe',
            'Subtribe' => 'Subtribe',
            'Genus' => 'Genus',
            'Subgenus' => 'Subgenus',
            'Species' => 'Species',
            'Subspecies' => 'Subspecies'
          ),
          'Moneran' => array(
            'Kingdom' => 'Kingdom',
            'Subkingdom' => 'Subkingdom',
            'Phylum' => 'Phylum',
            'Subphylum' => 'Subphylum',
            'Superclass' => 'Superclass',
            'Class' => 'Class',
            'Subclass' => 'Subclass',
            'Infraclass' => 'Infraclass',
            'Superorder' => 'Superorder',
            'Order' => 'Order',
            'Suborder' => 'Suborder',
            'Infraorder' => 'Infraorder',
            'Superfamily' => 'Superfamily',
            'Family' => 'Family',
            'Subfamily' => 'Subfamily',
            'Tribe' => 'Tribe',
            'Subtribe' => 'Subtribe',
            'Genus' => 'Genus',
            'Subgenus' => 'Subgenus',
            'Species' => 'Species',
            'Subspecies' => 'Subspecies'
          )
        ),
        'allowed_values_function' => ''
      ),
      'translatable' => '1',
      'type' => 'list_text'
    ),
    'field_instance' => array(
      'bundle' => 'itis_vocabulary',
      'default_value' => NULL,
      'deleted' => '0',
      'description' => '',
      'display' => array(
        'default' => array(
          'label' => 'above',
          'module' => 'list',
          'settings' => array(),
          'type' => 'list_default',
          'weight' => 0
        )
      ),
      'entity_type' => 'taxonomy_term',
      'field_name' => 'field_rank',
      'label' => 'Rank',
      'required' => 0,
      'settings' => array(
        'user_register_form' => FALSE
      ),
      'widget' => array(
        'active' => 1,
        'module' => 'options',
        'settings' => array(),
        'type' => 'options_select',
        'weight' => '1'
      )
    )
  );
  // Translatables
  // Included for use with string extractors like potx.
  t('Rank');
  return $fields;
}
