<?php

/**
 * Simply returns an array of groups
 */
function itis_term_groups(){
  return array(
    array(
      'group_name' => 'group_reference',
      'entity_type' => 'taxonomy_term',
      'bundle' => 'itis_vocabulary',
      'mode' => 'form',
      'parent_name' => 'group_vertical_tabs',
      'label' => 'Reference',
      'weight' => '10',
      'children' => array(
        0 => 'field_authors',
        1 => 'field_page_number',
        2 => 'field_reference'
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
      'group_name' => 'group_scientific_name',
      'entity_type' => 'taxonomy_term',
      'bundle' => 'itis_vocabulary',
      'mode' => 'form',
      'parent_name' => 'group_vertical_tabs',
      'label' => 'Scientific name',
      'weight' => '8',
      'children' => array(
        0 => 'field_rank',
        1 => 'field_unit_indicator1',
        2 => 'field_unit_indicator2',
        3 => 'field_unit_indicator3',
        4 => 'field_unit_indicator4',
        5 => 'field_unit_name1',
        6 => 'field_unit_name2',
        7 => 'field_unit_name3',
        8 => 'field_unit_name4'
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
      'group_name' => 'group_usage',
      'entity_type' => 'taxonomy_term',
      'bundle' => 'itis_vocabulary',
      'mode' => 'form',
      'parent_name' => 'group_vertical_tabs',
      'label' => 'Usage',
      'weight' => '9',
      'children' => array(
        0 => 'field_aan',
        1 => 'field_unacceptability_reason',
        2 => 'field_usage'
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
      'group_name' => 'group_vernacular_names',
      'entity_type' => 'taxonomy_term',
      'bundle' => 'itis_vocabulary',
      'mode' => 'form',
      'parent_name' => 'group_vertical_tabs',
      'label' => 'Vernacular names',
      'weight' => '10',
      'children' => array(
        0 => 'field_vernacular_name_collection',
        1 => 'description'
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
      'group_name' => 'group_misc',
      'entity_type' => 'taxonomy_term',
      'bundle' => 'itis_vocabulary',
      'mode' => 'form',
      'parent_name' => 'group_vertical_tabs',
      'label' => 'Misc.',
      'weight' => '13',
      'children' => array(
        0 => 'description'
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
      'group_name' => 'group_vertical_tabs',
      'entity_type' => 'taxonomy_term',
      'bundle' => 'itis_vocabulary',
      'mode' => 'form',
      'parent_name' => '',
      'label' => 'Vertical tabs',
      'weight' => '8',
      'children' => array(
        0 => 'group_reference',
        1 => 'group_scientific_name',
        2 => 'group_usage',
        3 => 'group_vernacular_names',
        4 => 'group_misc',
        5 => 'group_vernacular_names'
      ),
      'format_type' => 'htabs',
      'format_settings' => array(
        'formatter' => NULL,
        'instance_settings' => array(
          'classes' => ''
        )
      )
    )
  );
}

/**
 * Additional fields that are unique per vocabulary
 */
function itis_term_fields_per_vocabulary(){
  return array(
    array(
      'field_config' => array(
        'locked' => '1',
        'active' => '1',
        'cardinality' => '1',
        'deleted' => '0',
        'entity_types' => array(),
        'field_name' => 'field_aan',
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
              'parent' => '0',
              'vocabulary' => 'tags'
            )
          )
        ),
        'translatable' => '1',
        'type' => 'taxonomy_term_reference'
      ),
      'field_instance' => array(
        'bundle' => 'tags',
        'default_value' => NULL,
        'deleted' => '0',
        'description' => t('The scientific name of the valid or accepted taxon identified as the currently accepted name used for a given invalid or not accepted name. Each name that is in synonymy (junior synonyms, obsolete combinations, etc.) must be connected to one accepted or valid name.'),
        'display' => array(
          'default' => array(
            'label' => 'inline',
            'module' => 'taxonomy',
            'settings' => array(),
            'type' => 'hidden',
            'weight' => -10
          )
        ),
        'entity_type' => 'taxonomy_term',
        'field_name' => 'field_aan',
        'label' => 'Associated accepted name',
        'required' => 0,
        'settings' => array(
          'user_register_form' => FALSE
        ),
        'widget' => array(
          'active' => 0,
          'module' => 'taxonomy',
          'settings' => array(
            'autocomplete_path' => 'taxonomy/autocomplete'
          ),
          'type' => 'taxonomy_autocomplete',
          'weight' => '11'
        )
      )
    )
  );
}

/**
 * Simply returns an array of fields
 */
function itis_term_fields(){
  return array(
    array(
      'field_config' => array(
        'locked' => '1',
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
            0 => 'format'
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
        'bundle' => 'tags',
        'default_value' => NULL,
        'deleted' => '0',
        'description' => t('The author(s) associated with the name of a taxon. This can include the year of publication and brackets, e.g. "(Quoy and Gaimard, 1832)".'),
        'display' => array(
          'default' => array(
            'label' => 'inline',
            'module' => 'text',
            'settings' => array(),
            'type' => 'text_default',
            'weight' => -11
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
          'settings' => array(),
          'type' => 'text_textfield',
          'weight' => '13'
        )
      )
    ),
    array(
      'field_config' => array(
        'locked' => '1',
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
            0 => 'format'
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
        'bundle' => 'tags',
        'default_value' => NULL,
        'deleted' => '0',
        'description' => t('The page number the taxon was described on in the protologue.'),
        'display' => array(
          'default' => array(
            'label' => 'inline',
            'module' => 'text',
            'settings' => array(),
            'type' => 'text_default',
            'weight' => -6
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
          'settings' => array(),
          'type' => 'text_textfield',
          'weight' => '15'
        )
      )
    ),
    array(
      'field_config' => array(
        'locked' => '1',
        'active' => '1',
        'cardinality' => '1',
        'deleted' => '0',
        'entity_types' => array(),
        'field_name' => 'field_rank',
        'foreign keys' => array(),
        'indexes' => array(
          'value' => array(
            0 => 'value'
          )
        ),
        'module' => 'list',
        'settings' => array(
          'allowed_values_function' => 'itis_term_allowed_values'
        ),
        'translatable' => '1',
        'type' => 'list_text'
      ),
      'field_instance' => array(
        'bundle' => 'tags',
        'default_value' => NULL,
        'deleted' => '0',
        'description' => t('The label associated with the specific level of a taxonomic hierarchy, e.g. "Family", "Genus" or "Species".'),
        'display' => array(
          'default' => array(
            'label' => 'inline',
            'module' => 'list',
            'settings' => array(),
            'type' => 'list_default',
            'weight' => -20
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
    ),
    array(
      'field_config' => array(
        'locked' => '1',
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
            0 => 'nid'
          )
        ),
        'module' => 'node_reference',
        'settings' => array(
          'referenceable_types' => array(
            'article' => 'biblio'
          )
        ),
        'translatable' => '1',
        'type' => 'node_reference'
      ),
      'field_instance' => array(
        'bundle' => 'tags',
        'default_value' => NULL,
        'deleted' => '0',
        'description' => '',
        'display' => array(
          'default' => array(
            'label' => 'inline',
            'module' => 'node_reference',
            'settings' => array(),
            'type' => 'node_reference_default',
            'weight' => -7
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
          'module' => 'node_reference',
          'settings' => array(
            'autocomplete_match' => 'contains',
            'autocomplete_path' => 'node_reference/autocomplete',
            'size' => 60
          ),
          'type' => 'node_reference_autocomplete',
          'weight' => '14'
        )
      )
    ),
    array(
      'field_config' => array(
        'locked' => '1',
        'active' => '1',
        'cardinality' => '1',
        'deleted' => '0',
        'entity_types' => array(),
        'field_name' => 'field_unacceptability_reason',
        'foreign keys' => array(),
        'indexes' => array(
          'value' => array(
            0 => 'value'
          )
        ),
        'module' => 'list',
        'settings' => array(
          'allowed_values_function' => 'itis_term_allowed_values'
        ),
        'translatable' => '1',
        'type' => 'list_text'
      ),
      'field_instance' => array(
        'bundle' => 'tags',
        'default_value' => NULL,
        'deleted' => '0',
        'description' => t('An explanation regarding why a given scientific name is judged to be invalid or not accepted.'),
        'display' => array(
          'default' => array(
            'label' => 'inline',
            'module' => 'list',
            'settings' => array(),
            'type' => 'list_default',
            'weight' => -8
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
    ),
    array(
      'field_config' => array(
        'locked' => '1',
        'active' => '1',
        'cardinality' => '1',
        'deleted' => '0',
        'entity_types' => array(),
        'field_name' => 'field_unit_indicator1',
        'foreign keys' => array(),
        'indexes' => array(
          'value' => array(
            0 => 'value'
          )
        ),
        'module' => 'list',
        'settings' => array(
          'allowed_values_function' => 'itis_term_allowed_values'
        ),
        'translatable' => '1',
        'type' => 'list_text'
      ),
      'field_instance' => array(
        'bundle' => 'tags',
        'default_value' => NULL,
        'deleted' => '0',
        'description' => t('Indicator of an occurrence of a plant hybrid at the generic level.'),
        'display' => array(
          'default' => array(
            'label' => 'inline',
            'module' => 'list',
            'settings' => array(),
            'type' => 'list_default',
            'weight' => -15
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
    ),
    array(
      'field_config' => array(
        'locked' => '1',
        'active' => '1',
        'cardinality' => '1',
        'deleted' => '0',
        'entity_types' => array(),
        'field_name' => 'field_unit_indicator2',
        'foreign keys' => array(),
        'indexes' => array(
          'value' => array(
            0 => 'value'
          )
        ),
        'module' => 'list',
        'settings' => array(
          'allowed_values_function' => 'itis_term_allowed_values'
        ),
        'translatable' => '1',
        'type' => 'list_text'
      ),
      'field_instance' => array(
        'bundle' => 'tags',
        'default_value' => NULL,
        'deleted' => '0',
        'description' => t('A category indicator positioned between the first and second parts of a binomial/polynomial taxonomic name.'),
        'display' => array(
          'default' => array(
            'label' => 'inline',
            'module' => 'list',
            'settings' => array(),
            'type' => 'list_default',
            'weight' => -14
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
    ),
    array(
      'field_config' => array(
        'locked' => '1',
        'active' => '1',
        'cardinality' => '1',
        'deleted' => '0',
        'entity_types' => array(),
        'field_name' => 'field_unit_indicator3',
        'foreign keys' => array(),
        'indexes' => array(
          'value' => array(
            0 => 'value'
          )
        ),
        'module' => 'list',
        'settings' => array(
          'allowed_values_function' => 'itis_term_allowed_values'
        ),
        'translatable' => '1',
        'type' => 'list_text'
      ),
      'field_instance' => array(
        'bundle' => 'tags',
        'default_value' => NULL,
        'deleted' => '0',
        'description' => t('A category indicator located within a polynomial taxonomic name. Allowed values are: "ssp." (algae, fungi and plants), "subsp." (animals, bacteria and archaea), "var.", "subvar.", "f.", "subf.", and "X".'),
        'display' => array(
          'default' => array(
            'label' => 'inline',
            'module' => 'list',
            'settings' => array(),
            'type' => 'list_default',
            'weight' => -13
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
    ),
    array(
      'field_config' => array(
        'locked' => '1',
        'active' => '1',
        'cardinality' => '1',
        'deleted' => '0',
        'entity_types' => array(),
        'field_name' => 'field_unit_indicator4',
        'foreign keys' => array(),
        'indexes' => array(
          'value' => array(
            0 => 'value'
          )
        ),
        'module' => 'list',
        'settings' => array(
          'allowed_values_function' => 'itis_term_allowed_values'
        ),
        'translatable' => '1',
        'type' => 'list_text'
      ),
      'field_instance' => array(
        'bundle' => 'tags',
        'default_value' => NULL,
        'deleted' => '0',
        'description' => t('A category indicator located within a polynomial taxonomic name.'),
        'display' => array(
          'default' => array(
            'label' => 'inline',
            'module' => 'list',
            'settings' => array(),
            'type' => 'list_default',
            'weight' => -12
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
    ),
    array(
      'field_config' => array(
        'locked' => '1',
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
            0 => 'format'
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
        'bundle' => 'tags',
        'default_value' => NULL,
        'deleted' => '0',
        'description' => t('The singular or first part of a scientifically accepted label for an occurrence of Taxonomic Units. For uninomials (1-part names like family or genus names) this will be the only name field entered. For binomials/polynomials, this field will be used for the first part of the name (the genus name). The names in this position require the initial letter to be capitalized.'),
        'display' => array(
          'default' => array(
            'label' => 'inline',
            'module' => 'text',
            'settings' => array(),
            'type' => 'text_default',
            'weight' => -19
          )
        ),
        'entity_type' => 'taxonomy_term',
        'field_name' => 'field_unit_name1',
        'label' => 'Unit name 1',
        'required' => 1,
        'settings' => array(
          'text_processing' => '0',
          'user_register_form' => FALSE
        ),
        'widget' => array(
          'active' => 1,
          'module' => 'text',
          'settings' => array(),
          'type' => 'text_textfield',
          'weight' => '2'
        )
      )
    ),
    array(
      'field_config' => array(
        'locked' => '1',
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
            0 => 'format'
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
        'bundle' => 'tags',
        'default_value' => NULL,
        'deleted' => '0',
        'description' => t('The second part of a scientifically accepted label for a binomial/polynomial occurrence of Taxonomic Units (the species epithet).'),
        'display' => array(
          'default' => array(
            'label' => 'inline',
            'module' => 'text',
            'settings' => array(),
            'type' => 'text_default',
            'weight' => -18
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
          'settings' => array(),
          'type' => 'text_textfield',
          'weight' => '3'
        )
      )
    ),
    array(
      'field_config' => array(
        'locked' => '1',
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
            0 => 'format'
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
        'bundle' => 'tags',
        'default_value' => NULL,
        'deleted' => '0',
        'description' => t('The third portion of a scientifically accepted label for a polynomial occurrence of Taxonomic Units (e.g. subspecies name).'),
        'display' => array(
          'default' => array(
            'label' => 'inline',
            'module' => 'text',
            'settings' => array(),
            'type' => 'text_default',
            'weight' => -17
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
          'settings' => array(),
          'type' => 'text_textfield',
          'weight' => '4'
        )
      )
    ),
    array(
      'field_config' => array(
        'locked' => '1',
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
            0 => 'format'
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
        'bundle' => 'tags',
        'default_value' => NULL,
        'deleted' => '0',
        'description' => t('The fourth part of a scientifically accepted label for a polynomial occurrence of Taxonomic Units.'),
        'display' => array(
          'default' => array(
            'label' => 'inline',
            'module' => 'text',
            'settings' => array(),
            'type' => 'text_default',
            'weight' => -16
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
          'settings' => array(),
          'type' => 'text_textfield',
          'weight' => '5'
        )
      )
    ),
    array(
      'field_config' => array(
        'locked' => '1',
        'active' => '1',
        'cardinality' => '1',
        'deleted' => '0',
        'entity_types' => array(),
        'field_name' => 'field_usage',
        'foreign keys' => array(),
        'indexes' => array(
          'value' => array(
            0 => 'value'
          )
        ),
        'module' => 'list',
        'settings' => array(
          'allowed_values_function' => 'itis_term_allowed_values'
        ),
        'translatable' => '1',
        'type' => 'list_text'
      ),
      'field_instance' => array(
        'bundle' => 'tags',
        'default_value' => NULL,
        'deleted' => '0',
        'description' => t('Current standing of an occurrence of Taxonomic Units.'),
        'display' => array(
          'default' => array(
            'label' => 'inline',
            'module' => 'list',
            'settings' => array(),
            'type' => 'list_default',
            'weight' => -9
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

function itis_term_collections($vocabulary_name){
  return array(
    array(
      'field_config' => array(
        'locked' => '1',
        'active' => '1',
        'cardinality' => '-1',
        'deleted' => '0',
        'entity_types' => array(),
        'field_name' => 'field_vernacular_name_collection',
        'foreign keys' => array(),
        'indexes' => array(),
        'module' => 'field_collection',
        'settings' => array(
          'path' => ''
        ),
        'translatable' => '0',
        'type' => 'field_collection'
      ),
      'field_instance' => array(
        'bundle' => $vocabulary_name,
        'default_value' => NULL,
        'deleted' => '0',
        'description' => '',
        'display' => array(
          'default' => array(
            'label' => 'above',
            'module' => 'field_collection',
            'settings' => array(
              'add' => 'Add',
              'delete' => 'Delete',
              'description' => TRUE,
              'edit' => 'Edit',
              'view_mode' => 'full'
            ),
            'type' => 'field_collection_view',
            'weight' => 0
          )
        ),
        'entity_type' => 'taxonomy_term',
        'field_name' => 'field_vernacular_name_collection',
        'label' => 'Vernacular names',
        'required' => 0,
        'settings' => array(
          'user_register_form' => FALSE
        ),
        'widget' => array(
          'active' => 0,
          'module' => 'field_collection',
          'settings' => array(),
          'type' => 'field_collection_embed',
          'weight' => '31'
        )
      )
    )
  );
  // Translatables
  // Included for use with string extractors like potx.
  t('Vernacular names');
}

function itis_term_collection_fields(){
  return array(
    array(
      'field_config' => array(
        'locked' => '1',
        'active' => '1',
        'cardinality' => '1',
        'deleted' => '0',
        'entity_types' => array(),
        'field_name' => 'field_vernacular_name',
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
        'bundle' => 'field_vernacular_name_collection',
        'default_value' => NULL,
        'deleted' => '0',
        'description' => '',
        'display' => array(
          'default' => array(
            'label' => 'inline',
            'module' => 'text',
            'settings' => array(),
            'type' => 'text_default',
            'weight' => '1'
          ),
          'linked_node' => array(
            'label' => 'above',
            'settings' => array(),
            'type' => 'hidden',
            'weight' => 0
          )
        ),
        'entity_type' => 'field_collection_item',
        'field_name' => 'field_vernacular_name',
        'label' => 'Vernacular name',
        'required' => 0,
        'settings' => array(
          'linkit' => array(
            'enable' => 0,
            'insert_plugin' => ''
          ),
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
          'weight' => '0'
        )
      )
    ),
    array(
      'field_config' => array(
        'locked' => '1',
        'active' => '1',
        'cardinality' => '1',
        'deleted' => '0',
        'entity_types' => array(),
        'field_name' => 'field_language',
        'foreign keys' => array(),
        'indexes' => array(
          'value' => array(
            0 => 'value'
          )
        ),
        'module' => 'languagefield',
        'settings' => array(
          'enabled' => '1',
          'excluded_languages' => array(),
          'format' => array(
            'iso' => 'iso',
            'name' => 'name',
            'name_native' => 0
          ),
          'groups' => '',
          'languages' => array()
        ),
        'translatable' => '1',
        'type' => 'language_field'
      ),
      'field_instance' => array(
        'bundle' => 'field_vernacular_name_collection',
        'default_value' => NULL,
        'deleted' => '0',
        'description' => '',
        'display' => array(
          'default' => array(
            'label' => 'inline',
            'module' => 'languagefield',
            'settings' => array(
              'format' => array(
                'name' => 'name'
              )
            ),
            'type' => 'language_default',
            'weight' => 2
          ),
          'linked_node' => array(
            'label' => 'above',
            'settings' => array(),
            'type' => 'hidden',
            'weight' => 0
          )
        ),
        'entity_type' => 'field_collection_item',
        'field_name' => 'field_language',
        'label' => 'Language',
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
    ),
    array(
      'field_config' => array(
        'locked' => '1',
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
            0 => 'nid'
          )
        ),
        'module' => 'node_reference',
        'settings' => array(
          'referenceable_types' => array(
            'biblio' => 'biblio',
            'managed_url' => 0,
            'media_gallery' => 0,
            'page' => 0,
            'spm' => 0
          ),
          'view' => array(
            'args' => array(),
            'display_name' => '',
            'view_name' => ''
          )
        ),
        'translatable' => '1',
        'type' => 'node_reference'
      ),
      'field_instance' => array(
        'bundle' => 'field_vernacular_name_collection',
        'default_value' => NULL,
        'deleted' => '0',
        'description' => '',
        'display' => array(
          'default' => array(
            'label' => 'inline',
            'module' => 'node_reference',
            'settings' => array(),
            'type' => 'node_reference_default',
            'weight' => '2'
          ),
          'linked_node' => array(
            'label' => 'above',
            'settings' => array(),
            'type' => 'hidden',
            'weight' => 0
          )
        ),
        'entity_type' => 'field_collection_item',
        'entityconnect_unload_add' => '0',
        'entityconnect_unload_edit' => '0',
        'field_name' => 'field_reference',
        'label' => 'Source',
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
          'weight' => '2'
        )
      )
    ),
    array(
      'field_config' => array(
        'locked' => '1',
        'active' => '1',
        'cardinality' => '1',
        'deleted' => '0',
        'entity_types' => array(),
        'field_name' => 'field_vernacular_locality',
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
        'translatable' => '1',
        'type' => 'text_long'
      ),
      'field_instance' => array(
        'bundle' => 'field_vernacular_name_collection',
        'default_value' => NULL,
        'deleted' => '0',
        'description' => '',
        'display' => array(
          'default' => array(
            'label' => 'inline',
            'module' => 'text',
            'settings' => array(),
            'type' => 'text_default',
            'weight' => '3'
          ),
          'linked_node' => array(
            'label' => 'above',
            'settings' => array(),
            'type' => 'hidden',
            'weight' => 0
          )
        ),
        'entity_type' => 'field_collection_item',
        'field_name' => 'field_vernacular_locality',
        'label' => 'Area from which the vernacular name usage originates',
        'required' => 0,
        'settings' => array(
          'linkit' => array(
            'enable' => 0,
            'insert_plugin' => ''
          ),
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
          'weight' => '3'
        )
      )
    ),
    array(
      'field_config' => array(
        'locked' => '1',
        'active' => '1',
        'cardinality' => '1',
        'deleted' => '0',
        'entity_types' => array(),
        'field_name' => 'field_vernacular_name_remarks',
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
        'translatable' => '1',
        'type' => 'text_long'
      ),
      'field_instance' => array(
        'bundle' => 'field_vernacular_name_collection',
        'default_value' => NULL,
        'deleted' => '0',
        'description' => '',
        'display' => array(
          'default' => array(
            'label' => 'inline',
            'module' => 'text',
            'settings' => array(),
            'type' => 'text_default',
            'weight' => '5'
          ),
          'linked_node' => array(
            'label' => 'above',
            'settings' => array(),
            'type' => 'hidden',
            'weight' => 0
          )
        ),
        'entity_type' => 'field_collection_item',
        'field_name' => 'field_vernacular_name_remarks',
        'label' => 'Vernacular name remarks',
        'required' => 0,
        'settings' => array(
          'linkit' => array(
            'enable' => 0,
            'insert_plugin' => ''
          ),
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
          'weight' => '5'
        )
      )
    ),
    array(
      'field_config' => array(
        'locked' => '1',
        'active' => '1',
        'cardinality' => '1',
        'deleted' => '0',
        'entity_types' => array(),
        'field_name' => 'field_language',
        'foreign keys' => array(),
        'indexes' => array(
          'value' => array(
            0 => 'value'
          )
        ),
        'module' => 'languagefield',
        'settings' => array(
          'enabled' => '1',
          'excluded_languages' => array(),
          'format' => array(
            'iso' => 'iso',
            'name' => 'name',
            'name_native' => 0
          ),
          'groups' => '',
          'languages' => array()
        ),
        'translatable' => '1',
        'type' => 'language_field'
      ),
      'field_instance' => array(
        'bundle' => 'field_vernacular_name_collection',
        'default_value' => NULL,
        'deleted' => '0',
        'description' => '',
        'display' => array(
          'default' => array(
            'label' => 'above',
            'module' => 'languagefield',
            'settings' => array(
              'format' => array(
                'name' => 'name'
              )
            ),
            'type' => 'language_default',
            'weight' => 7
          ),
          'linked_node' => array(
            'label' => 'above',
            'settings' => array(),
            'type' => 'hidden',
            'weight' => 0
          )
        ),
        'entity_type' => 'field_collection_item',
        'field_name' => 'field_language',
        'label' => 'Language',
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
    ),
    array(
      'field_config' => array(
        'locked' => '1',
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
            0 => 'nid'
          )
        ),
        'module' => 'node_reference',
        'settings' => array(
          'referenceable_types' => array(
            'biblio' => 'biblio',
            'managed_url' => 0,
            'media_gallery' => 0,
            'page' => 0,
            'spm' => 0
          ),
          'view' => array(
            'args' => array(),
            'display_name' => '',
            'view_name' => ''
          )
        ),
        'translatable' => '1',
        'type' => 'node_reference'
      ),
      'field_instance' => array(
        'bundle' => 'field_vernacular_name_collection',
        'default_value' => NULL,
        'deleted' => '0',
        'description' => '',
        'display' => array(
          'default' => array(
            'label' => 'inline',
            'module' => 'node_reference',
            'settings' => array(),
            'type' => 'node_reference_default',
            'weight' => '2'
          ),
          'linked_node' => array(
            'label' => 'above',
            'settings' => array(),
            'type' => 'hidden',
            'weight' => 0
          )
        ),
        'entity_type' => 'field_collection_item',
        'entityconnect_unload_add' => '0',
        'entityconnect_unload_edit' => '0',
        'field_name' => 'field_reference',
        'label' => 'Source',
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
          'weight' => '2'
        )
      )
    ),
    array(
      'field_config' => array(
        'locked' => '1',
        'active' => '1',
        'cardinality' => '1',
        'deleted' => '0',
        'entity_types' => array(),
        'field_name' => 'field_vernacular_locality',
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
        'translatable' => '1',
        'type' => 'text_long'
      ),
      'field_instance' => array(
        'bundle' => 'field_vernacular_name_collection',
        'default_value' => NULL,
        'deleted' => '0',
        'description' => '',
        'display' => array(
          'default' => array(
            'label' => 'inline',
            'module' => 'text',
            'settings' => array(),
            'type' => 'text_default',
            'weight' => '3'
          ),
          'linked_node' => array(
            'label' => 'above',
            'settings' => array(),
            'type' => 'hidden',
            'weight' => 0
          )
        ),
        'entity_type' => 'field_collection_item',
        'field_name' => 'field_vernacular_locality',
        'label' => 'Area from which the vernacular name usage originates',
        'required' => 0,
        'settings' => array(
          'linkit' => array(
            'enable' => 0,
            'insert_plugin' => ''
          ),
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
          'weight' => '3'
        )
      )
    ),
    array(
      'field_config' => array(
        'locked' => '1',
        'active' => '1',
        'cardinality' => '1',
        'deleted' => '0',
        'entity_types' => array(),
        'field_name' => 'field_vernacular_name_remarks',
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
        'translatable' => '1',
        'type' => 'text_long'
      ),
      'field_instance' => array(
        'bundle' => 'field_vernacular_name_collection',
        'default_value' => NULL,
        'deleted' => '0',
        'description' => '',
        'display' => array(
          'default' => array(
            'label' => 'inline',
            'module' => 'text',
            'settings' => array(),
            'type' => 'text_default',
            'weight' => '5'
          ),
          'linked_node' => array(
            'label' => 'above',
            'settings' => array(),
            'type' => 'hidden',
            'weight' => 0
          )
        ),
        'entity_type' => 'field_collection_item',
        'field_name' => 'field_vernacular_name_remarks',
        'label' => 'Vernacular name remarks',
        'required' => 0,
        'settings' => array(
          'linkit' => array(
            'enable' => 0,
            'insert_plugin' => ''
          ),
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
          'weight' => '5'
        )
      )
    )
  );
}
// Translatables
// Included for use with string extractors like potx.
t('Vernacular name');
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
t('Common names collection');
t('Is preferred name');
t('Language');
t('Source');
t('Area from which the vernacular name usage originates');
t('Vernacular name remarks');
