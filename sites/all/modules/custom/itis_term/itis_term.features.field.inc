<?php

/**
 * Implementation of hook_field_default_fields().
 */
function itis_term_field_default_fields(){
  $fields = array();
  // Exported field: 'taxonomy_term-itis_vocabulary-field_associated_accepted_name'
  $fields['taxonomy_term-itis_vocabulary-field_associated_accepted_name'] = array(
    'field_config' => array(
      'active' => '1',
      'cardinality' => '1',
      'deleted' => '0',
      'entity_types' => array(),
      'field_name' => 'field_associated_accepted_name',
      'foreign keys' => array(
        'tid' => array(
          'columns' => array(
            'tid' => 'tid'
          ),
          'table' => 'taxonomy_term_data'
        )
      ),
      'indexes' => array(
        'tid' => array(
          '0' => 'tid'
        )
      ),
      'module' => 'taxonomy',
      'settings' => array(
        'allowed_values' => array(
          '0' => array(
            'parent' => '0',
            'vocabulary' => 'itis_vocabulary'
          )
        )
      ),
      'translatable' => '1',
      'type' => 'taxonomy_term_reference'
    ),
    'field_instance' => array(
      'bundle' => 'itis_vocabulary',
      'default_value' => NULL,
      'deleted' => '0',
      'description' => '',
      'display' => array(
        'default' => array(
          'label' => 'above',
          'module' => 'taxonomy',
          'settings' => array(),
          'type' => 'taxonomy_term_reference_link',
          'weight' => 10
        )
      ),
      'entity_type' => 'taxonomy_term',
      'field_name' => 'field_associated_accepted_name',
      'label' => 'Associated accepted name',
      'required' => 0,
      'settings' => array(
        'user_register_form' => FALSE
      ),
      'widget' => array(
        'active' => 0,
        'module' => 'taxonomy',
        'settings' => array(
          'autocomplete_path' => 'taxonomy/autocomplete',
          'size' => 60
        ),
        'type' => 'taxonomy_autocomplete',
        'weight' => '11'
      )
    )
  );
  // Exported field: 'taxonomy_term-itis_vocabulary-field_authors'
  $fields['taxonomy_term-itis_vocabulary-field_authors'] = array(
    'field_config' => array(
      'active' => '1',
      'cardinality' => '1',
      'deleted' => '0',
      'entity_types' => array(),
      'field_name' => 'field_authors',
      'foreign keys' => array(
        'format' => array(
          'columns' => array(
            'format' => 'format'
          ),
          'table' => 'filter_format'
        )
      ),
      'indexes' => array(
        'format' => array(
          '0' => 'format'
        )
      ),
      'module' => 'text',
      'settings' => array(
        'max_length' => '255'
      ),
      'translatable' => '1',
      'type' => 'text'
    ),
    'field_instance' => array(
      'bundle' => 'itis_vocabulary',
      'default_value' => NULL,
      'deleted' => '0',
      'description' => '',
      'display' => array(
        'default' => array(
          'label' => 'above',
          'module' => 'text',
          'settings' => array(),
          'type' => 'text_default',
          'weight' => 12
        )
      ),
      'entity_type' => 'taxonomy_term',
      'field_name' => 'field_authors',
      'label' => 'Authors',
      'required' => 0,
      'settings' => array(
        'text_processing' => '0',
        'user_register_form' => FALSE
      ),
      'widget' => array(
        'active' => 1,
        'module' => 'text',
        'settings' => array(
          'size' => '60'
        ),
        'type' => 'text_textfield',
        'weight' => '13'
      )
    )
  );
  // Exported field: 'taxonomy_term-itis_vocabulary-field_page_number'
  $fields['taxonomy_term-itis_vocabulary-field_page_number'] = array(
    'field_config' => array(
      'active' => '1',
      'cardinality' => '1',
      'deleted' => '0',
      'entity_types' => array(),
      'field_name' => 'field_page_number',
      'foreign keys' => array(
        'format' => array(
          'columns' => array(
            'format' => 'format'
          ),
          'table' => 'filter_format'
        )
      ),
      'indexes' => array(
        'format' => array(
          '0' => 'format'
        )
      ),
      'module' => 'text',
      'settings' => array(
        'max_length' => '255'
      ),
      'translatable' => '1',
      'type' => 'text'
    ),
    'field_instance' => array(
      'bundle' => 'itis_vocabulary',
      'default_value' => NULL,
      'deleted' => '0',
      'description' => '',
      'display' => array(
        'default' => array(
          'label' => 'above',
          'module' => 'text',
          'settings' => array(),
          'type' => 'text_default',
          'weight' => 13
        )
      ),
      'entity_type' => 'taxonomy_term',
      'field_name' => 'field_page_number',
      'label' => 'Page number',
      'required' => 0,
      'settings' => array(
        'text_processing' => '0',
        'user_register_form' => FALSE
      ),
      'widget' => array(
        'active' => 1,
        'module' => 'text',
        'settings' => array(
          'size' => '60'
        ),
        'type' => 'text_textfield',
        'weight' => '15'
      )
    )
  );
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
          'Animal' => array(
            'Class' => 'Class',
            'Family' => 'Family',
            'Genus' => 'Genus',
            'Infraclass' => 'Infraclass',
            'Infraorder' => 'Infraorder',
            'Kingdom' => 'Kingdom',
            'Order' => 'Order',
            'Phylum' => 'Phylum',
            'Species' => 'Species',
            'Subclass' => 'Subclass',
            'Subfamily' => 'Subfamily',
            'Subgenus' => 'Subgenus',
            'Subkingdom' => 'Subkingdom',
            'Suborder' => 'Suborder',
            'Subphylum' => 'Subphylum',
            'Subspecies' => 'Subspecies',
            'Subtribe' => 'Subtribe',
            'Superclass' => 'Superclass',
            'Superfamily' => 'Superfamily',
            'Superorder' => 'Superorder',
            'Tribe' => 'Tribe'
          ),
          'Moneran' => array(
            'Class' => 'Class',
            'Family' => 'Family',
            'Genus' => 'Genus',
            'Infraclass' => 'Infraclass',
            'Infraorder' => 'Infraorder',
            'Kingdom' => 'Kingdom',
            'Order' => 'Order',
            'Phylum' => 'Phylum',
            'Species' => 'Species',
            'Subclass' => 'Subclass',
            'Subfamily' => 'Subfamily',
            'Subgenus' => 'Subgenus',
            'Subkingdom' => 'Subkingdom',
            'Suborder' => 'Suborder',
            'Subphylum' => 'Subphylum',
            'Subspecies' => 'Subspecies',
            'Subtribe' => 'Subtribe',
            'Superclass' => 'Superclass',
            'Superfamily' => 'Superfamily',
            'Superorder' => 'Superorder',
            'Tribe' => 'Tribe'
          ),
          'Plant/Fungi' => array(
            'Class' => 'Class',
            'Division' => 'Division',
            'Family' => 'Family',
            'Form' => 'Form',
            'Genus' => 'Genus',
            'Kingdom' => 'Kingdom',
            'Order' => 'Order',
            'Section' => 'Section',
            'Species' => 'Species',
            'Subclass' => 'Subclass',
            'Subdivision' => 'Subdivision',
            'Subfamily' => 'Subfamily',
            'Subform' => 'Subform',
            'Subgenus' => 'Subgenus',
            'Subkingdom' => 'Subkingdom',
            'Suborder' => 'Suborder',
            'Subsection' => 'Subsection',
            'Subspecies' => 'Subspecies',
            'Subtribe' => 'Subtribe',
            'Subvariety' => 'Subvariety',
            'Tribe' => 'Tribe',
            'Variety' => 'Variety'
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
  // Exported field: 'taxonomy_term-itis_vocabulary-field_reference'
  $fields['taxonomy_term-itis_vocabulary-field_reference'] = array(
    'field_config' => array(
      'active' => '1',
      'cardinality' => '1',
      'deleted' => '0',
      'entity_types' => array(),
      'field_name' => 'field_reference',
      'foreign keys' => array(
        'nid' => array(
          'columns' => array(
            'nid' => 'nid'
          ),
          'table' => 'node'
        )
      ),
      'indexes' => array(
        'nid' => array(
          '0' => 'nid'
        )
      ),
      'module' => 'node_reference',
      'settings' => array(
        'referenceable_types' => array(
          'article' => 0,
          'page' => 0
        )
      ),
      'translatable' => '1',
      'type' => 'node_reference'
    ),
    'field_instance' => array(
      'bundle' => 'itis_vocabulary',
      'default_value' => NULL,
      'deleted' => '0',
      'description' => '',
      'display' => array(
        'default' => array(
          'label' => 'above',
          'module' => 'node_reference',
          'settings' => array(),
          'type' => 'node_reference_default',
          'weight' => 14
        )
      ),
      'entity_type' => 'taxonomy_term',
      'field_name' => 'field_reference',
      'label' => 'Reference',
      'required' => 0,
      'settings' => array(
        'user_register_form' => FALSE
      ),
      'widget' => array(
        'active' => 1,
        'module' => 'options',
        'settings' => array(),
        'type' => 'options_select',
        'weight' => '14'
      )
    )
  );
  // Exported field: 'taxonomy_term-itis_vocabulary-field_unacceptability_reason'
  $fields['taxonomy_term-itis_vocabulary-field_unacceptability_reason'] = array(
    'field_config' => array(
      'active' => '1',
      'cardinality' => '1',
      'deleted' => '0',
      'entity_types' => array(),
      'field_name' => 'field_unacceptability_reason',
      'foreign keys' => array(),
      'indexes' => array(
        'value' => array(
          '0' => 'value'
        )
      ),
      'module' => 'list',
      'settings' => array(
        'allowed_values' => array(
          'Animalia & Monera' => array(
            'junior synonym' => 'junior synonym',
            'original name/combination' => 'original name/combination',
            'subsequent name/combination' => 'subsequent name/combination',
            'junior homonym' => 'junior homonym',
            'homonym & junior synonym' => 'homonym & junior synonym',
            'unavailable, database artifact' => 'unavailable, database artifact',
            'unavailable, literature misspelling' => 'unavailable, literature misspelling',
            'unavailable, incorrect original spelling' => 'unavailable, incorrect original spelling',
            'unavailable, suppressed by ruling' => 'unavailable, suppressed by ruling',
            'unavailable, nomen nudum' => 'unavailable, nomen nudum',
            'unavailable, other' => 'unavailable, other',
            'unjustified emendation' => 'unjustified emendation',
            'unnecessary replacement' => 'unnecessary replacement',
            'nomen oblitum' => 'nomen oblitum',
            'misapplied' => 'misapplied',
            'pro parte' => 'pro parte',
            'other' => 'other',
            'nomen dubium' => 'nomen dubium'
          ),
          'Plantae & Fungi' => array(
            'synonym' => 'synonym',
            'homonym (illegitimate)' => 'homonym (illegitimate)',
            'superfluous renaming (illegitimate)' => 'superfluous renaming (illegitimate)',
            'rejected name' => 'rejected name',
            'invalidly published, nomen nudum' => 'invalidly published, nomen nudum',
            'invalidly published, other' => 'invalidly published, other',
            'misapplied' => 'misapplied',
            'pro parte' => 'pro parte',
            'horticultural' => 'horticultural',
            'database artifact' => 'database artifact',
            'orthographic variant (misspelling)' => 'orthographic variant (misspelling)',
            'other' => 'other'
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
          'weight' => 11
        )
      ),
      'entity_type' => 'taxonomy_term',
      'field_name' => 'field_unacceptability_reason',
      'label' => 'Unacceptability Reason',
      'required' => 0,
      'settings' => array(
        'user_register_form' => FALSE
      ),
      'widget' => array(
        'active' => 1,
        'module' => 'options',
        'settings' => array(),
        'type' => 'options_select',
        'weight' => '12'
      )
    )
  );
  // Exported field: 'taxonomy_term-itis_vocabulary-field_unit_indicator1'
  $fields['taxonomy_term-itis_vocabulary-field_unit_indicator1'] = array(
    'field_config' => array(
      'active' => '1',
      'cardinality' => '1',
      'deleted' => '0',
      'entity_types' => array(),
      'field_name' => 'field_unit_indicator1',
      'foreign keys' => array(),
      'indexes' => array(
        'value' => array(
          '0' => 'value'
        )
      ),
      'module' => 'list',
      'settings' => array(
        'allowed_values' => array(
          'x' => 'x'
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
          'weight' => 5
        )
      ),
      'entity_type' => 'taxonomy_term',
      'field_name' => 'field_unit_indicator1',
      'label' => 'Unit indicator 1',
      'required' => 0,
      'settings' => array(
        'user_register_form' => FALSE
      ),
      'widget' => array(
        'active' => 1,
        'module' => 'options',
        'settings' => array(),
        'type' => 'options_select',
        'weight' => '6'
      )
    )
  );
  // Exported field: 'taxonomy_term-itis_vocabulary-field_unit_indicator2'
  $fields['taxonomy_term-itis_vocabulary-field_unit_indicator2'] = array(
    'field_config' => array(
      'active' => '1',
      'cardinality' => '1',
      'deleted' => '0',
      'entity_types' => array(),
      'field_name' => 'field_unit_indicator2',
      'foreign keys' => array(),
      'indexes' => array(
        'value' => array(
          '0' => 'value'
        )
      ),
      'module' => 'list',
      'settings' => array(
        'allowed_values' => array(
          'x' => 'x'
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
          'weight' => 6
        )
      ),
      'entity_type' => 'taxonomy_term',
      'field_name' => 'field_unit_indicator2',
      'label' => 'Unit indicator 2',
      'required' => 0,
      'settings' => array(
        'user_register_form' => FALSE
      ),
      'widget' => array(
        'active' => 1,
        'module' => 'options',
        'settings' => array(),
        'type' => 'options_select',
        'weight' => '7'
      )
    )
  );
  // Exported field: 'taxonomy_term-itis_vocabulary-field_unit_indicator3'
  $fields['taxonomy_term-itis_vocabulary-field_unit_indicator3'] = array(
    'field_config' => array(
      'active' => '1',
      'cardinality' => '1',
      'deleted' => '0',
      'entity_types' => array(),
      'field_name' => 'field_unit_indicator3',
      'foreign keys' => array(),
      'indexes' => array(
        'value' => array(
          '0' => 'value'
        )
      ),
      'module' => 'list',
      'settings' => array(
        'allowed_values' => array(
          'f.' => 'f.',
          'ssp.' => 'ssp.',
          'subf.' => 'subf.',
          'subsp.' => 'subsp.',
          'subvar.' => 'subvar.',
          'var.' => 'var.',
          'x' => 'x'
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
          'weight' => 7
        )
      ),
      'entity_type' => 'taxonomy_term',
      'field_name' => 'field_unit_indicator3',
      'label' => 'Unit indicator 3',
      'required' => 0,
      'settings' => array(
        'user_register_form' => FALSE
      ),
      'widget' => array(
        'active' => 1,
        'module' => 'options',
        'settings' => array(),
        'type' => 'options_select',
        'weight' => '8'
      )
    )
  );
  // Exported field: 'taxonomy_term-itis_vocabulary-field_unit_indicator4'
  $fields['taxonomy_term-itis_vocabulary-field_unit_indicator4'] = array(
    'field_config' => array(
      'active' => '1',
      'cardinality' => '1',
      'deleted' => '0',
      'entity_types' => array(),
      'field_name' => 'field_unit_indicator4',
      'foreign keys' => array(),
      'indexes' => array(
        'value' => array(
          '0' => 'value'
        )
      ),
      'module' => 'list',
      'settings' => array(
        'allowed_values' => array(
          'f.' => 'f.',
          'ssp.' => 'ssp. (Plants and Fungi)',
          'subf.' => 'subf.',
          'subsp.' => 'subsp. (Animals and Monerans)',
          'subvar.' => 'subvar.',
          'var.' => 'var.'
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
          'weight' => 8
        )
      ),
      'entity_type' => 'taxonomy_term',
      'field_name' => 'field_unit_indicator4',
      'label' => 'Unit indicator 4',
      'required' => 0,
      'settings' => array(
        'user_register_form' => FALSE
      ),
      'widget' => array(
        'active' => 1,
        'module' => 'options',
        'settings' => array(),
        'type' => 'options_select',
        'weight' => '9'
      )
    )
  );
  // Exported field: 'taxonomy_term-itis_vocabulary-field_unit_name1'
  $fields['taxonomy_term-itis_vocabulary-field_unit_name1'] = array(
    'field_config' => array(
      'active' => '1',
      'cardinality' => '1',
      'deleted' => '0',
      'entity_types' => array(),
      'field_name' => 'field_unit_name1',
      'foreign keys' => array(
        'format' => array(
          'columns' => array(
            'format' => 'format'
          ),
          'table' => 'filter_format'
        )
      ),
      'indexes' => array(
        'format' => array(
          '0' => 'format'
        )
      ),
      'module' => 'text',
      'settings' => array(
        'max_length' => '255'
      ),
      'translatable' => '1',
      'type' => 'text'
    ),
    'field_instance' => array(
      'bundle' => 'itis_vocabulary',
      'default_value' => NULL,
      'deleted' => '0',
      'description' => '',
      'display' => array(
        'default' => array(
          'label' => 'above',
          'module' => 'text',
          'settings' => array(),
          'type' => 'text_default',
          'weight' => 1
        )
      ),
      'entity_type' => 'taxonomy_term',
      'field_name' => 'field_unit_name1',
      'label' => 'Unit name 1',
      'required' => 0,
      'settings' => array(
        'text_processing' => '0',
        'user_register_form' => FALSE
      ),
      'widget' => array(
        'active' => 1,
        'module' => 'text',
        'settings' => array(
          'size' => '60'
        ),
        'type' => 'text_textfield',
        'weight' => '2'
      )
    )
  );
  // Exported field: 'taxonomy_term-itis_vocabulary-field_unit_name2'
  $fields['taxonomy_term-itis_vocabulary-field_unit_name2'] = array(
    'field_config' => array(
      'active' => '1',
      'cardinality' => '1',
      'deleted' => '0',
      'entity_types' => array(),
      'field_name' => 'field_unit_name2',
      'foreign keys' => array(
        'format' => array(
          'columns' => array(
            'format' => 'format'
          ),
          'table' => 'filter_format'
        )
      ),
      'indexes' => array(
        'format' => array(
          '0' => 'format'
        )
      ),
      'module' => 'text',
      'settings' => array(
        'max_length' => '255'
      ),
      'translatable' => '1',
      'type' => 'text'
    ),
    'field_instance' => array(
      'bundle' => 'itis_vocabulary',
      'default_value' => NULL,
      'deleted' => '0',
      'description' => '',
      'display' => array(
        'default' => array(
          'label' => 'above',
          'module' => 'text',
          'settings' => array(),
          'type' => 'text_default',
          'weight' => 2
        )
      ),
      'entity_type' => 'taxonomy_term',
      'field_name' => 'field_unit_name2',
      'label' => 'Unit name 2',
      'required' => 0,
      'settings' => array(
        'text_processing' => '0',
        'user_register_form' => FALSE
      ),
      'widget' => array(
        'active' => 1,
        'module' => 'text',
        'settings' => array(
          'size' => '60'
        ),
        'type' => 'text_textfield',
        'weight' => '3'
      )
    )
  );
  // Exported field: 'taxonomy_term-itis_vocabulary-field_unit_name3'
  $fields['taxonomy_term-itis_vocabulary-field_unit_name3'] = array(
    'field_config' => array(
      'active' => '1',
      'cardinality' => '1',
      'deleted' => '0',
      'entity_types' => array(),
      'field_name' => 'field_unit_name3',
      'foreign keys' => array(
        'format' => array(
          'columns' => array(
            'format' => 'format'
          ),
          'table' => 'filter_format'
        )
      ),
      'indexes' => array(
        'format' => array(
          '0' => 'format'
        )
      ),
      'module' => 'text',
      'settings' => array(
        'max_length' => '255'
      ),
      'translatable' => '1',
      'type' => 'text'
    ),
    'field_instance' => array(
      'bundle' => 'itis_vocabulary',
      'default_value' => NULL,
      'deleted' => '0',
      'description' => '',
      'display' => array(
        'default' => array(
          'label' => 'above',
          'module' => 'text',
          'settings' => array(),
          'type' => 'text_default',
          'weight' => 3
        )
      ),
      'entity_type' => 'taxonomy_term',
      'field_name' => 'field_unit_name3',
      'label' => 'Unit name 3',
      'required' => 0,
      'settings' => array(
        'text_processing' => '0',
        'user_register_form' => FALSE
      ),
      'widget' => array(
        'active' => 1,
        'module' => 'text',
        'settings' => array(
          'size' => '60'
        ),
        'type' => 'text_textfield',
        'weight' => '4'
      )
    )
  );
  // Exported field: 'taxonomy_term-itis_vocabulary-field_unit_name4'
  $fields['taxonomy_term-itis_vocabulary-field_unit_name4'] = array(
    'field_config' => array(
      'active' => '1',
      'cardinality' => '1',
      'deleted' => '0',
      'entity_types' => array(),
      'field_name' => 'field_unit_name4',
      'foreign keys' => array(
        'format' => array(
          'columns' => array(
            'format' => 'format'
          ),
          'table' => 'filter_format'
        )
      ),
      'indexes' => array(
        'format' => array(
          '0' => 'format'
        )
      ),
      'module' => 'text',
      'settings' => array(
        'max_length' => '255'
      ),
      'translatable' => '1',
      'type' => 'text'
    ),
    'field_instance' => array(
      'bundle' => 'itis_vocabulary',
      'default_value' => NULL,
      'deleted' => '0',
      'description' => '',
      'display' => array(
        'default' => array(
          'label' => 'above',
          'module' => 'text',
          'settings' => array(),
          'type' => 'text_default',
          'weight' => 4
        )
      ),
      'entity_type' => 'taxonomy_term',
      'field_name' => 'field_unit_name4',
      'label' => 'Unit name 4',
      'required' => 0,
      'settings' => array(
        'text_processing' => '0',
        'user_register_form' => FALSE
      ),
      'widget' => array(
        'active' => 1,
        'module' => 'text',
        'settings' => array(
          'size' => '60'
        ),
        'type' => 'text_textfield',
        'weight' => '5'
      )
    )
  );
  // Exported field: 'taxonomy_term-itis_vocabulary-field_usage'
  $fields['taxonomy_term-itis_vocabulary-field_usage'] = array(
    'field_config' => array(
      'active' => '1',
      'cardinality' => '1',
      'deleted' => '0',
      'entity_types' => array(),
      'field_name' => 'field_usage',
      'foreign keys' => array(),
      'indexes' => array(
        'value' => array(
          '0' => 'value'
        )
      ),
      'module' => 'list',
      'settings' => array(
        'allowed_values' => array(
          'Plants and Fungi' => array(
            'accepted' => 'accepted',
            'not accepted' => 'not accepted'
          ),
          'Animals and Monerans' => array(
            'valid' => 'valid',
            'invalid' => 'invalid'
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
          'weight' => 9
        )
      ),
      'entity_type' => 'taxonomy_term',
      'field_name' => 'field_usage',
      'label' => 'Usage',
      'required' => 0,
      'settings' => array(
        'user_register_form' => FALSE
      ),
      'widget' => array(
        'active' => 1,
        'module' => 'options',
        'settings' => array(),
        'type' => 'options_select',
        'weight' => '10'
      )
    )
  );
  // Exported field: 'taxonomy_term-itis_vocabulary-field_vernacular_names'
  $fields['taxonomy_term-itis_vocabulary-field_vernacular_names'] = array(
    'field_config' => array(
      'active' => '1',
      'cardinality' => '-1',
      'deleted' => '0',
      'entity_types' => array(),
      'field_name' => 'field_vernacular_names',
      'foreign keys' => array(
        'format' => array(
          'columns' => array(
            'format' => 'format'
          ),
          'table' => 'filter_format'
        )
      ),
      'indexes' => array(
        'format' => array(
          '0' => 'format'
        )
      ),
      'module' => 'text',
      'settings' => array(
        'max_length' => '255'
      ),
      'translatable' => '1',
      'type' => 'text'
    ),
    'field_instance' => array(
      'bundle' => 'itis_vocabulary',
      'default_value' => NULL,
      'deleted' => '0',
      'description' => '',
      'display' => array(
        'default' => array(
          'label' => 'above',
          'module' => 'text',
          'settings' => array(),
          'type' => 'text_default',
          'weight' => 15
        )
      ),
      'entity_type' => 'taxonomy_term',
      'field_name' => 'field_vernacular_names',
      'label' => 'Vernacular names',
      'required' => 0,
      'settings' => array(
        'text_processing' => '0',
        'user_register_form' => FALSE
      ),
      'widget' => array(
        'active' => 1,
        'module' => 'text',
        'settings' => array(
          'size' => '60'
        ),
        'type' => 'text_textfield',
        'weight' => '16'
      )
    )
  );
  // Translatables
  // Included for use with string extractors like potx.
  t('Associated accepted name');
  t('Authors');
  t('Page number');
  t('Rank');
  t('Reference');
  t('Unacceptability Reason');
  t('Unit indicator 1');
  t('Unit indicator 2');
  t('Unit indicator 3');
  t('Unit indicator 4');
  t('Unit name 1');
  t('Unit name 2');
  t('Unit name 3');
  t('Unit name 4');
  t('Usage');
  t('Vernacular names');
  return $fields;
}
