<?php

/**
 * Simply returns an array of groups
 */
function emonocot_itis_groups($vid){
  return array(
    array(
      'group_name' => 'group_etymology',
      'entity_type' => 'taxonomy_term',
      'bundle' => 'itis_vocabulary',
      'mode' => 'form',
      'parent_name' => 'group_vertical_tabs',
      'label' => 'Etymology',
      'weight' => '10',
      'children' => array(
        0 => 'field_itis_em_etymology'
      ),
      'format_type' => 'htab',
      'format_settings' => array(
        'formatter' => 'closed',
        'instance_settings' => array(
          'classes' => '',
          'required_fields' => 1
        )
      )
    ),
    array(
      'group_name' => 'group_hybrids',
      'entity_type' => 'taxonomy_term',
      'bundle' => 'itis_vocabulary',
      'mode' => 'form',
      'parent_name' => 'group_vertical_tabs',
      'label' => 'Hybridisation',
      'weight' => '10',
      'children' => array(
        0 => 'field_hp_' . $vid
      ),
      'format_type' => 'htab',
      'format_settings' => array(
        'formatter' => 'closed',
        'instance_settings' => array(
          'classes' => '',
          'required_fields' => 1
        )
      )
    ),
    array(
      'group_name' => 'group_typification',
      'entity_type' => 'taxonomy_term',
      'bundle' => 'itis_vocabulary',
      'mode' => 'form',
      'parent_name' => 'group_vertical_tabs',
      'label' => 'Typification',
      'weight' => '10',
      'children' => array(
        0 => 'field_type_ref',
        1 => 'field_type_taxon_' . $vid
      ),
      'format_type' => 'htab',
      'format_settings' => array(
        'formatter' => 'closed',
        'instance_settings' => array(
          'classes' => '',
          'required_fields' => 1
        )
      )
    )
  );
}

/**
 * Additional fields that are unique per vocabulary
 */
function emonocot_itis_fields_per_vocabulary(){
  return array();
}

/**
 * Some fields need to go into groups defined by the itis_term module, they are listed here
 */
function emonocot_itis_fields_groups(){
  return array(
    'group_reference' => array(
      'field_itis_em_other_ref',
      'field_itis_em_url'
    ),
    'group_misc' => array(
      'field_id'
    )
  );
}

/**
 * Simply returns an array of fields
 */
function emonocot_itis_fields($vid){
  $vocab = taxonomy_vocabulary_load($vid);
  return array(
    array(
      'field_config' => array(
        'active' => '1',
        'cardinality' => '1',
        'deleted' => '0',
        'entity_types' => array(),
        'field_name' => 'field_itis_em_etymology',
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
            0 => 'format'
          )
        ),
        'module' => 'text',
        'settings' => array(),
        'translatable' => '0',
        'type' => 'text_long'
      ),
      'field_instance' => array(
        'bundle' => 'test',
        'default_value' => NULL,
        'deleted' => '0',
        'description' => '',
        'display' => array(
          'default' => array(
            'label' => 'above',
            'module' => 'text',
            'settings' => array(),
            'type' => 'text_default',
            'weight' => 17
          )
        ),
        'entity_type' => 'taxonomy_term',
        'field_name' => 'field_itis_em_etymology',
        'label' => 'Etymology',
        'required' => 0,
        'settings' => array(
          'text_processing' => '0',
          'user_register_form' => FALSE
        ),
        'widget' => array(
          'active' => 1,
          'module' => 'text',
          'settings' => array(
            'rows' => '5'
          ),
          'type' => 'text_textarea',
          'weight' => '1'
        )
      )
    ),
    array(
      'field_config' => array(
        'active' => '1',
        'cardinality' => '2',
        'deleted' => '0',
        'entity_types' => array(),
        'field_name' => 'field_hp_' . $vid,
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
            0 => 'tid'
          )
        ),
        'module' => 'taxonomy',
        'settings' => array(
          'allowed_values' => array(
            0 => array(
              'parent' => 0,
              'vocabulary' => $vocab->machine_name
            )
          )
        ),
        'translatable' => '0',
        'type' => 'taxonomy_term_reference'
      ),
      'field_instance' => array(
        'bundle' => 'test',
        'default_value' => NULL,
        'deleted' => '0',
        'description' => 'Separate taxa using a comma',
        'display' => array(
          'default' => array(
            'label' => 'above',
            'module' => 'taxonomy',
            'settings' => array(),
            'type' => 'taxonomy_term_reference_link',
            'weight' => 16
          )
        ),
        'entity_type' => 'taxonomy_term',
        'field_name' => 'field_hp_' . $vid,
        'label' => 'Hybrid parents',
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
    ),
    array(
      'field_config' => array(
        'active' => '1',
        'cardinality' => '-1',
        'deleted' => '0',
        'entity_types' => array(),
        'field_name' => 'field_type_ref',
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
            0 => 'nid'
          )
        ),
        'module' => 'node_reference',
        'settings' => array(
          'referenceable_types' => array(
            'biblio' => 'biblio'
          )
        ),
        'translatable' => '0',
        'type' => 'node_reference'
      ),
      'field_instance' => array(
        'bundle' => 'test',
        'default_value' => NULL,
        'deleted' => '0',
        'description' => 'Subsequent publications changing the status of a type (e.g. lectotype designation)',
        'display' => array(
          'default' => array(
            'label' => 'above',
            'module' => 'node_reference',
            'settings' => array(),
            'type' => 'node_reference_default',
            'weight' => 19
          )
        ),
        'entity_type' => 'taxonomy_term',
        'field_name' => 'field_type_ref',
        'label' => 'Type references',
        'required' => 0,
        'settings' => array(
          'user_register_form' => FALSE
        ),
        'widget' => array(
          'active' => 1,
          'module' => 'node_reference',
          'settings' => array(
            'autocomplete_match' => 'contains',
            'autocomplete_path' => 'node_reference/autocomplete',
            'size' => '60'
          ),
          'type' => 'node_reference_autocomplete',
          'weight' => '16'
        )
      )
    ),
    array(
      'field_config' => array(
        'active' => '1',
        'cardinality' => '1',
        'deleted' => '0',
        'entity_types' => array(),
        'field_name' => 'field_type_taxon_' . $vid,
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
            0 => 'tid'
          )
        ),
        'module' => 'taxonomy',
        'settings' => array(
          'allowed_values' => array(
            0 => array(
              'parent' => 0,
              'vocabulary' => $vocab->machine_name
            )
          )
        ),
        'translatable' => '0',
        'type' => 'taxonomy_term_reference'
      ),
      'field_instance' => array(
        'bundle' => 'test',
        'default_value' => NULL,
        'deleted' => '0',
        'description' => 'The type taxon of a genus or family group name.',
        'display' => array(
          'default' => array(
            'label' => 'above',
            'module' => 'taxonomy',
            'settings' => array(),
            'type' => 'taxonomy_term_reference_link',
            'weight' => 18
          )
        ),
        'entity_type' => 'taxonomy_term',
        'field_name' => 'field_type_taxon_' . $vid,
        'label' => 'Type taxon',
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
          'weight' => '10'
        )
      )
    ),
    array(
      'field_config' => array(
        'active' => '1',
        'cardinality' => '1',
        'deleted' => '0',
        'entity_types' => array(),
        'field_name' => 'field_id',
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
            0 => 'format'
          )
        ),
        'module' => 'text',
        'settings' => array(
          'max_length' => '255'
        ),
        'translatable' => '0',
        'type' => 'text'
      ),
      'field_instance' => array(
        'bundle' => 'happy_feet',
        'default_value' => NULL,
        'deleted' => '0',
        'description' => '',
        'display' => array(
          'default' => array(
            'label' => 'above',
            'module' => 'text',
            'settings' => array(),
            'type' => 'hidden',
            'weight' => 20
          ),
          'linked_node' => array(
            'label' => 'above',
            'settings' => array(),
            'type' => 'hidden',
            'weight' => 0
          )
        ),
        'entity_type' => 'taxonomy_term',
        'field_name' => 'field_id',
        'label' => 'WCM-ID',
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
          'weight' => '18'
        )
      )
    ),
    array(
      'field_config' => array(
        'active' => '1',
        'cardinality' => '1',
        'deleted' => '0',
        'entity_types' => array(),
        'field_name' => 'field_itis_em_other_ref',
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
            0 => 'format'
          )
        ),
        'module' => 'text',
        'settings' => array(
          'max_length' => '255'
        ),
        'translatable' => '0',
        'type' => 'text'
      ),
      'field_instance' => array(
        'bundle' => 'happy_feet',
        'default_value' => NULL,
        'deleted' => '0',
        'description' => '',
        'display' => array(
          'default' => array(
            'label' => 'above',
            'module' => 'text',
            'settings' => array(),
            'type' => 'text_default',
            'weight' => 21
          ),
          'linked_node' => array(
            'label' => 'above',
            'settings' => array(),
            'type' => 'hidden',
            'weight' => 0
          )
        ),
        'entity_type' => 'taxonomy_term',
        'field_name' => 'field_itis_em_other_ref',
        'label' => 'WCM Protologue Reference',
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
    ),
    array(
      'field_config' => array(
        'active' => '1',
        'cardinality' => '1',
        'deleted' => '0',
        'entity_types' => array(),
        'field_name' => 'field_itis_em_url',
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
            0 => 'format'
          )
        ),
        'module' => 'text',
        'settings' => array(
          'max_length' => '255'
        ),
        'translatable' => '0',
        'type' => 'text'
      ),
      'field_instance' => array(
        'bundle' => 'happy_feet',
        'default_value' => NULL,
        'deleted' => '0',
        'description' => '',
        'display' => array(
          'default' => array(
            'label' => 'above',
            'module' => 'text',
            'settings' => array(),
            'type' => 'text_default',
            'weight' => 22
          ),
          'linked_node' => array(
            'label' => 'above',
            'settings' => array(),
            'type' => 'hidden',
            'weight' => 0
          )
        ),
        'entity_type' => 'taxonomy_term',
        'field_name' => 'field_itis_em_url',
        'label' => 'Protologue Link',
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
          'weight' => '17'
        )
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
}
  