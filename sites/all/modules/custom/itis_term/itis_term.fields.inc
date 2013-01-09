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
      'weight' => '11',
      'children' => array(
        0 => 'field_vernacular_names',
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
        4 => 'group_misc'
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
        'description' => '',
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
        'description' => '',
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
        'description' => '',
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
        'description' => '',
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
        'description' => '',
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
        'description' => '',
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
        'description' => '',
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
        'description' => '',
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
        'description' => '',
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
        'description' => '',
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
        'description' => '',
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
        'description' => '',
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
        'description' => '',
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
        'description' => '',
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
    ),
    array(
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
        'description' => '',
        'display' => array(
          'default' => array(
            'label' => 'inline',
            'module' => 'text',
            'settings' => array(),
            'type' => 'text_default',
            'weight' => -5
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
          'settings' => array(),
          'type' => 'text_textfield',
          'weight' => '16'
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
  