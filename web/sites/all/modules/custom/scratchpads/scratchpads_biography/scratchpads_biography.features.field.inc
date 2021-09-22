<?php

/**
 * @file
 * scratchpads_biography.features.field.inc
 */
/**
 * Implements hook_field_default_fields().
 */
function scratchpads_biography_field_default_fields(){
  $fields = array();
  // Exported field: 'user-user-field_alternative_name'
  $fields['user-user-field_alternative_name'] = array(
    'field_config' => array(
      'active' => '1',
      'cardinality' => '-1',
      'deleted' => '0',
      'entity_types' => array(),
      'field_name' => 'field_alternative_name',
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
      'bundle' => 'user',
      'default_value' => NULL,
      'deleted' => '0',
      'description' => '',
      'display' => array(
        'default' => array(
          'label' => 'inline',
          'module' => 'textformatter',
          'settings' => array(
            'textformatter_class' => 'textformatter-list',
            'textformatter_comma_and' => 0,
            'textformatter_comma_full_stop' => 0,
            'textformatter_comma_override' => 0,
            'textformatter_comma_tag' => 'div',
            'textformatter_contrib' => array(),
            'textformatter_separator_custom' => '',
            'textformatter_separator_custom_class' => 'textformatter-separator',
            'textformatter_separator_custom_tag' => 'span',
            'textformatter_term_plain' => 0,
            'textformatter_type' => 'comma'
          ),
          'type' => 'textformatter_list',
          'weight' => '4'
        ),
        'linked_node' => array(
          'label' => 'above',
          'settings' => array(),
          'type' => 'hidden',
          'weight' => 0
        )
      ),
      'entity_type' => 'user',
      'field_name' => 'field_alternative_name',
      'label' => 'Alternative Name',
      'required' => 0,
      'settings' => array(
        'linkit' => array(
          'enable' => 0,
          'insert_plugin' => ''
        ),
        'text_processing' => '0',
        'user_register_form' => 1
      ),
      'widget' => array(
        'active' => 1,
        'module' => 'text',
        'settings' => array(
          'size' => '60'
        ),
        'type' => 'text_textfield',
        'weight' => '29'
      )
    )
  );
  // Exported field: 'user-user-field_areas_of_professional_inte'
  $fields['user-user-field_areas_of_professional_inte'] = array(
    'field_config' => array(
      'active' => '1',
      'cardinality' => '1',
      'deleted' => '0',
      'entity_types' => array(),
      'field_name' => 'field_areas_of_professional_inte',
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
      'bundle' => 'user',
      'default_value' => NULL,
      'deleted' => '0',
      'description' => '',
      'display' => array(
        'default' => array(
          'label' => 'inline',
          'module' => 'textformatter',
          'settings' => array(
            'textformatter_class' => 'textformatter-list',
            'textformatter_comma_and' => 0,
            'textformatter_comma_full_stop' => 0,
            'textformatter_comma_override' => 0,
            'textformatter_comma_tag' => 'div',
            'textformatter_contrib' => array(),
            'textformatter_separator_custom' => '',
            'textformatter_separator_custom_class' => 'textformatter-separator',
            'textformatter_separator_custom_tag' => 'span',
            'textformatter_term_plain' => 0,
            'textformatter_type' => 'comma'
          ),
          'type' => 'textformatter_list',
          'weight' => '7'
        ),
        'linked_node' => array(
          'label' => 'above',
          'settings' => array(),
          'type' => 'hidden',
          'weight' => 0
        )
      ),
      'entity_type' => 'user',
      'field_name' => 'field_areas_of_professional_inte',
      'label' => 'Areas of professional interest ',
      'required' => 0,
      'settings' => array(
        'linkit' => array(
          'enable' => 0,
          'insert_plugin' => ''
        ),
        'text_processing' => '1',
        'user_register_form' => 1
      ),
      'widget' => array(
        'active' => 1,
        'module' => 'text',
        'settings' => array(
          'rows' => '5'
        ),
        'type' => 'text_textarea',
        'weight' => '18'
      )
    )
  );
  // Exported field: 'user-user-field_biography'
  $fields['user-user-field_biography'] = array(
    'field_config' => array(
      'active' => '1',
      'cardinality' => '1',
      'deleted' => '0',
      'entity_types' => array(),
      'field_name' => 'field_biography',
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
      'bundle' => 'user',
      'default_value' => NULL,
      'deleted' => '0',
      'description' => '',
      'display' => array(
        'default' => array(
          'label' => 'above',
          'module' => 'text',
          'settings' => array(),
          'type' => 'text_default',
          'weight' => '11'
        ),
        'linked_node' => array(
          'label' => 'above',
          'settings' => array(),
          'type' => 'hidden',
          'weight' => 0
        )
      ),
      'entity_type' => 'user',
      'field_name' => 'field_biography',
      'label' => 'Biography',
      'required' => 0,
      'settings' => array(
        'linkit' => array(
          'enable' => 0,
          'insert_plugin' => ''
        ),
        'text_processing' => '1',
        'user_register_form' => 1
      ),
      'widget' => array(
        'active' => 1,
        'module' => 'text',
        'settings' => array(
          'rows' => '5'
        ),
        'type' => 'text_textarea',
        'weight' => '35'
      )
    )
  );
  // Exported field: 'user-user-field_birth_date'
  $fields['user-user-field_birth_date'] = array(
    'field_config' => array(
      'active' => '1',
      'cardinality' => '1',
      'deleted' => '0',
      'entity_types' => array(),
      'field_name' => 'field_birth_date',
      'foreign keys' => array(),
      'indexes' => array(),
      'module' => 'date',
      'settings' => array(
        'cache_count' => '4',
        'cache_enabled' => 0,
        'granularity' => array(
          'day' => 'day',
          'hour' => 0,
          'minute' => 0,
          'month' => 'month',
          'second' => 0,
          'year' => 'year'
        ),
        'timezone_db' => '',
        'todate' => 'optional',
        'tz_handling' => 'none'
      ),
      'translatable' => '0',
      'type' => 'datetime'
    ),
    'field_instance' => array(
      'bundle' => 'user',
      'deleted' => '0',
      'description' => 'If the exact date is unknown, please use a date range.',
      'display' => array(
        'default' => array(
          'label' => 'inline',
          'module' => 'date',
          'settings' => array(
            'format_type' => 'short',
            'fromto' => 'both',
            'multiple_from' => '',
            'multiple_number' => '',
            'multiple_to' => ''
          ),
          'type' => 'date_default',
          'weight' => '7'
        ),
        'linked_node' => array(
          'label' => 'above',
          'settings' => array(),
          'type' => 'hidden',
          'weight' => 0
        )
      ),
      'entity_type' => 'user',
      'field_name' => 'field_birth_date',
      'label' => 'Birth Date',
      'required' => 0,
      'settings' => array(
        'default_value' => 'blank',
        'default_value2' => 'blank',
        'default_value_code' => '',
        'default_value_code2' => '',
        'user_register_form' => 1
      ),
      'widget' => array(
        'active' => 1,
        'module' => 'date',
        'settings' => array(
          'increment' => 15,
          'input_format' => 'Y-m-d H:i:s',
          'input_format_custom' => '',
          'label_position' => 'above',
          'text_parts' => array(),
          'year_range' => '-3:+3'
        ),
        'type' => 'date_text',
        'weight' => '32'
      )
    )
  );
  // Exported field: 'user-user-field_birth_place'
  $fields['user-user-field_birth_place'] = array(
    'field_config' => array(
      'active' => '1',
      'cardinality' => '1',
      'deleted' => '0',
      'entity_types' => array(),
      'field_name' => 'field_birth_place',
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
      'bundle' => 'user',
      'default_value' => NULL,
      'deleted' => '0',
      'description' => '',
      'display' => array(
        'default' => array(
          'label' => 'inline',
          'module' => 'text',
          'settings' => array(),
          'type' => 'text_default',
          'weight' => '8'
        ),
        'linked_node' => array(
          'label' => 'above',
          'settings' => array(),
          'type' => 'hidden',
          'weight' => 0
        )
      ),
      'entity_type' => 'user',
      'field_name' => 'field_birth_place',
      'label' => 'Birth Place',
      'required' => 0,
      'settings' => array(
        'linkit' => array(
          'enable' => 0,
          'insert_plugin' => ''
        ),
        'text_processing' => '0',
        'user_register_form' => 1
      ),
      'widget' => array(
        'active' => 1,
        'module' => 'text',
        'settings' => array(
          'size' => '60'
        ),
        'type' => 'text_textfield',
        'weight' => '31'
      )
    )
  );
  // Exported field: 'user-user-field_comments'
  $fields['user-user-field_comments'] = array(
    'field_config' => array(
      'active' => '1',
      'cardinality' => '1',
      'deleted' => '0',
      'entity_types' => array(),
      'field_name' => 'field_comments',
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
      'bundle' => 'user',
      'default_value' => NULL,
      'deleted' => '0',
      'description' => '',
      'display' => array(
        'default' => array(
          'label' => 'above',
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
      'entity_type' => 'user',
      'field_name' => 'field_comments',
      'label' => 'Comments',
      'required' => 0,
      'settings' => array(
        'linkit' => array(
          'enable' => 0,
          'insert_plugin' => ''
        ),
        'text_processing' => '1',
        'user_register_form' => 1
      ),
      'widget' => array(
        'active' => 1,
        'module' => 'text',
        'settings' => array(
          'rows' => '5'
        ),
        'type' => 'text_textarea',
        'weight' => '20'
      )
    )
  );
  // Exported field: 'user-user-field_death_date'
  $fields['user-user-field_death_date'] = array(
    'field_config' => array(
      'active' => '1',
      'cardinality' => '1',
      'deleted' => '0',
      'entity_types' => array(),
      'field_name' => 'field_death_date',
      'foreign keys' => array(),
      'indexes' => array(),
      'module' => 'date',
      'settings' => array(
        'cache_count' => '4',
        'cache_enabled' => 0,
        'granularity' => array(
          'day' => 'day',
          'hour' => 0,
          'minute' => 0,
          'month' => 'month',
          'second' => 0,
          'year' => 'year'
        ),
        'timezone_db' => '',
        'todate' => 'optional',
        'tz_handling' => 'none'
      ),
      'translatable' => '0',
      'type' => 'datetime'
    ),
    'field_instance' => array(
      'bundle' => 'user',
      'deleted' => '0',
      'description' => 'If the exact date is unknown, please use a date range.',
      'display' => array(
        'default' => array(
          'label' => 'inline',
          'module' => 'date',
          'settings' => array(
            'format_type' => 'short',
            'fromto' => 'both',
            'multiple_from' => '',
            'multiple_number' => '',
            'multiple_to' => ''
          ),
          'type' => 'date_default',
          'weight' => '9'
        ),
        'linked_node' => array(
          'label' => 'above',
          'settings' => array(),
          'type' => 'hidden',
          'weight' => 0
        )
      ),
      'entity_type' => 'user',
      'field_name' => 'field_death_date',
      'label' => 'Date of Death',
      'required' => 0,
      'settings' => array(
        'default_value' => 'blank',
        'default_value2' => 'blank',
        'default_value_code' => '',
        'default_value_code2' => '',
        'user_register_form' => 1
      ),
      'widget' => array(
        'active' => 1,
        'module' => 'date',
        'settings' => array(
          'increment' => 15,
          'input_format' => 'Y-m-d H:i:s',
          'input_format_custom' => '',
          'label_position' => 'above',
          'text_parts' => array(),
          'year_range' => '-3:+3'
        ),
        'type' => 'date_text',
        'weight' => '33'
      )
    )
  );
  // Exported field: 'user-user-field_example_of_handwriting'
  $fields['user-user-field_example_of_handwriting'] = array(
    'field_config' => array(
      'active' => '1',
      'cardinality' => '-1',
      'deleted' => '0',
      'entity_types' => array(),
      'field_name' => 'field_example_of_handwriting',
      'foreign keys' => array(
        'fid' => array(
          'columns' => array(
            'fid' => 'fid'
          ),
          'table' => 'file_managed'
        )
      ),
      'indexes' => array(
        'fid' => array(
          0 => 'fid'
        )
      ),
      'module' => 'file',
      'settings' => array(
        'display_default' => 0,
        'display_field' => 0,
        'uri_scheme' => 'public'
      ),
      'translatable' => '0',
      'type' => 'file'
    ),
    'field_instance' => array(
      'bundle' => 'user',
      'deleted' => '0',
      'description' => 'Allowed file types: png, gif, jpg, jpeg, pdf',
      'display' => array(
        'default' => array(
          'label' => 'inline',
          'module' => 'styles',
          'settings' => array(),
          'type' => 'styles_file_square_thumbnail',
          'weight' => '12'
        ),
        'linked_node' => array(
          'label' => 'above',
          'settings' => array(),
          'type' => 'hidden',
          'weight' => 0
        )
      ),
      'entity_type' => 'user',
      'field_name' => 'field_example_of_handwriting',
      'label' => 'Example of handwriting',
      'required' => 0,
      'settings' => array(
        'description_field' => 0,
        'file_directory' => 'signature',
        'file_extensions' => 'png gif jpg jpeg pdf',
        'max_filesize' => '',
        'user_register_form' => 1
      ),
      'widget' => array(
        'active' => 1,
        'module' => 'media',
        'settings' => array(
          'allowed_schemes' => array(),
          'allowed_types' => array(
            0 => 0,
            'audio' => 0,
            'default' => 0,
            'image' => 'image',
            'video' => 0
          ),
          'browser_plugins' => array(
            'media_default--media_browser_1' => 'media_default--media_browser_1',
            'media_default--media_browser_my_files' => 'media_default--media_browser_my_files',
            'upload' => 'upload'
          ),
          'progress_indicator' => 'throbber'
        ),
        'type' => 'media_generic',
        'weight' => '34'
      )
    )
  );
  // Exported field: 'user-user-field_gender'
  $fields['user-user-field_gender'] = array(
    'field_config' => array(
      'active' => '1',
      'cardinality' => '1',
      'deleted' => '0',
      'entity_types' => array(),
      'field_name' => 'field_gender',
      'foreign keys' => array(),
      'indexes' => array(
        'value' => array(
          0 => 'value'
        )
      ),
      'module' => 'list',
      'settings' => array(
        'allowed_values' => array(
          'male' => 'Male',
          'female' => 'Female',
          'other' => 'Other'
        ),
        'allowed_values_function' => ''
      ),
      'translatable' => '0',
      'type' => 'list_text'
    ),
    'field_instance' => array(
      'bundle' => 'user',
      'default_value' => array(
        0 => array(
          'value' => 'unknown'
        )
      ),
      'deleted' => '0',
      'description' => '',
      'display' => array(
        'default' => array(
          'label' => 'inline',
          'module' => 'list',
          'settings' => array(),
          'type' => 'list_default',
          'weight' => '6'
        ),
        'linked_node' => array(
          'label' => 'above',
          'settings' => array(),
          'type' => 'hidden',
          'weight' => 0
        )
      ),
      'entity_type' => 'user',
      'field_name' => 'field_gender',
      'label' => 'Gender',
      'required' => 0,
      'settings' => array(
        'user_register_form' => 1
      ),
      'widget' => array(
        'active' => 1,
        'module' => 'options',
        'settings' => array(),
        'type' => 'options_select',
        'weight' => '30'
      )
    )
  );
  // Exported field: 'user-user-field_known_for'
  $fields['user-user-field_known_for'] = array(
    'field_config' => array(
      'active' => '1',
      'cardinality' => '1',
      'deleted' => '0',
      'entity_types' => array(),
      'field_name' => 'field_known_for',
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
      'bundle' => 'user',
      'default_value' => NULL,
      'deleted' => '0',
      'description' => '',
      'display' => array(
        'default' => array(
          'label' => 'inline',
          'module' => 'textformatter',
          'settings' => array(
            'textformatter_class' => 'textformatter-list',
            'textformatter_comma_and' => 0,
            'textformatter_comma_full_stop' => 0,
            'textformatter_comma_override' => 0,
            'textformatter_comma_tag' => 'div',
            'textformatter_contrib' => array(),
            'textformatter_separator_custom' => '',
            'textformatter_separator_custom_class' => 'textformatter-separator',
            'textformatter_separator_custom_tag' => 'span',
            'textformatter_term_plain' => 0,
            'textformatter_type' => 'comma'
          ),
          'type' => 'textformatter_list',
          'weight' => '6'
        ),
        'linked_node' => array(
          'label' => 'above',
          'settings' => array(),
          'type' => 'hidden',
          'weight' => 0
        )
      ),
      'entity_type' => 'user',
      'field_name' => 'field_known_for',
      'label' => 'Known for',
      'required' => 0,
      'settings' => array(
        'linkit' => array(
          'enable' => 0,
          'insert_plugin' => ''
        ),
        'text_processing' => '0',
        'user_register_form' => 1
      ),
      'widget' => array(
        'active' => 1,
        'module' => 'text',
        'settings' => array(
          'size' => '60'
        ),
        'type' => 'text_textfield',
        'weight' => '20'
      )
    )
  );
  // Exported field: 'user-user-field_main_collaborators_co_auth'
  $fields['user-user-field_main_collaborators_co_auth'] = array(
    'field_config' => array(
      'active' => '1',
      'cardinality' => '-1',
      'deleted' => '0',
      'entity_types' => array(),
      'field_name' => 'field_main_collaborators_co_auth',
      'foreign keys' => array(
        'uid' => array(
          'columns' => array(
            'uid' => 'uid'
          ),
          'table' => 'users'
        )
      ),
      'indexes' => array(
        'uid' => array(
          0 => 'uid'
        )
      ),
      'module' => 'user_reference',
      'settings' => array(
        'referenceable_roles' => array(
          2 => '2',
          3 => '3',
          4 => '4',
          5 => '5'
        ),
        'referenceable_status' => array(
          0 => 0,
          1 => '1'
        ),
        'view' => array(
          'args' => array(),
          'display_name' => 'references_1',
          'view_name' => 'users_references'
        )
      ),
      'translatable' => '0',
      'type' => 'user_reference'
    ),
    'field_instance' => array(
      'bundle' => 'user',
      'default_value' => NULL,
      'deleted' => '0',
      'description' => '',
      'display' => array(
        'default' => array(
          'label' => 'inline',
          'module' => 'user_reference',
          'settings' => array(),
          'type' => 'user_reference_default',
          'weight' => '5'
        ),
        'linked_node' => array(
          'label' => 'above',
          'settings' => array(),
          'type' => 'hidden',
          'weight' => 0
        )
      ),
      'entity_type' => 'user',
      'entityconnect_unload_add' => '1',
      'entityconnect_unload_edit' => '1',
      'field_name' => 'field_main_collaborators_co_auth',
      'label' => 'Main collaborators/co-authors',
      'required' => 0,
      'settings' => array(
        'user_register_form' => 1
      ),
      'widget' => array(
        'active' => 1,
        'module' => 'user_reference',
        'settings' => array(
          'autocomplete_match' => 'contains',
          'autocomplete_path' => 'user_reference/autocomplete',
          'size' => '60'
        ),
        'type' => 'user_reference_autocomplete',
        'weight' => '19'
      )
    )
  );
  // Exported field: 'user-user-field_make_this_profile_public_'
  $fields['user-user-field_make_this_profile_public_'] = array(
    'field_config' => array(
      'active' => '1',
      'cardinality' => '1',
      'deleted' => '0',
      'entity_types' => array(),
      'field_name' => 'field_make_this_profile_public_',
      'foreign keys' => array(),
      'indexes' => array(
        'value' => array(
          0 => 'value'
        )
      ),
      'module' => 'list',
      'settings' => array(
        'allowed_values' => array(
          0 => 'No',
          1 => 'Yes'
        ),
        'allowed_values_function' => ''
      ),
      'translatable' => '0',
      'type' => 'list_boolean'
    ),
    'field_instance' => array(
      'bundle' => 'user',
      'default_value' => array(
        0 => array(
          'value' => 0
        )
      ),
      'deleted' => '0',
      'description' => 'Do you want the user profile to be public?',
      'display' => array(
        'default' => array(
          'label' => 'hidden',
          'settings' => array(),
          'type' => 'hidden',
          'weight' => '4'
        ),
        'linked_node' => array(
          'label' => 'above',
          'settings' => array(),
          'type' => 'hidden',
          'weight' => 0
        )
      ),
      'entity_type' => 'user',
      'field_name' => 'field_make_this_profile_public_',
      'label' => 'Make this profile public?',
      'required' => 0,
      'settings' => array(
        'user_register_form' => 1
      ),
      'widget' => array(
        'active' => 1,
        'module' => 'options',
        'settings' => array(
          'display_label' => 1
        ),
        'type' => 'options_onoff',
        'weight' => '2'
      )
    )
  );
  // Exported field: 'user-user-field_orcid'
  $fields['user-user-field_orcid'] = array(
    'field_config' => array(
      'active' => '1',
      'cardinality' => '1',
      'deleted' => '0',
      'entity_types' => array(),
      'field_name' => 'field_orcid',
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
      'bundle' => 'user',
      'default_value' => NULL,
      'deleted' => '0',
      'description' => 'ORCID is an open, non-profit, community-based effort to provide a registry of unique researcher identifiers and a transparent method of linking research activities and outputs to these identifiers. (Format of an orcid: 0000-0000-0000-0000)',
      'display' => array(
        'default' => array(
          'label' => 'inline',
          'module' => 'scratchpads_biography',
          'settings' => array(),
          'type' => 'scratchpads_biography_link_to_orcid',
          'weight' => '6'
        ),
        'linked_node' => array(
          'label' => 'above',
          'settings' => array(),
          'type' => 'hidden',
          'weight' => 0
        )
      ),
      'entity_type' => 'user',
      'field_name' => 'field_orcid',
      'label' => 'ORCID identifier',
      'required' => 0,
      'settings' => array(
        'linkit' => array(
          'enable' => 0,
          'insert_plugin' => ''
        ),
        'text_processing' => '0',
        'user_register_form' => 1
      ),
      'widget' => array(
        'active' => 1,
        'module' => 'text',
        'settings' => array(
          'size' => '60'
        ),
        'type' => 'text_textfield',
        'weight' => '31'
      )
    )
  );
  // Exported field: 'user-user-field_places_worked'
  $fields['user-user-field_places_worked'] = array(
    'field_config' => array(
      'active' => '1',
      'cardinality' => '-1',
      'deleted' => '0',
      'entity_types' => array(),
      'field_name' => 'field_places_worked',
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
      'bundle' => 'user',
      'default_value' => NULL,
      'deleted' => '0',
      'description' => '',
      'display' => array(
        'default' => array(
          'label' => 'inline',
          'module' => 'textformatter',
          'settings' => array(
            'textformatter_class' => 'textformatter-list',
            'textformatter_comma_and' => 0,
            'textformatter_comma_full_stop' => 0,
            'textformatter_comma_override' => 0,
            'textformatter_comma_tag' => 'div',
            'textformatter_contrib' => array(),
            'textformatter_separator_custom' => '',
            'textformatter_separator_custom_class' => 'textformatter-separator',
            'textformatter_separator_custom_tag' => 'span',
            'textformatter_term_plain' => 0,
            'textformatter_type' => 'comma'
          ),
          'type' => 'textformatter_list',
          'weight' => '4'
        ),
        'linked_node' => array(
          'label' => 'above',
          'settings' => array(),
          'type' => 'hidden',
          'weight' => 0
        )
      ),
      'entity_type' => 'user',
      'field_name' => 'field_places_worked',
      'label' => 'Places worked',
      'required' => 0,
      'settings' => array(
        'linkit' => array(
          'enable' => 0,
          'insert_plugin' => ''
        ),
        'text_processing' => '0',
        'user_register_form' => 1
      ),
      'widget' => array(
        'active' => 1,
        'module' => 'text',
        'settings' => array(
          'size' => '60'
        ),
        'type' => 'text_textfield',
        'weight' => '22'
      )
    )
  );
  // Exported field: 'user-user-field_related_links'
  $fields['user-user-field_related_links'] = array(
    'field_config' => array(
      'active' => '1',
      'cardinality' => '-1',
      'deleted' => '0',
      'entity_types' => array(),
      'field_name' => 'field_related_links',
      'foreign keys' => array(),
      'indexes' => array(),
      'module' => 'link',
      'settings' => array(
        'attributes' => array(
          'class' => '',
          'rel' => '',
          'target' => 'default'
        ),
        'display' => array(
          'url_cutoff' => 80
        ),
        'enable_tokens' => 1,
        'title' => 'optional',
        'title_maxlength' => 128,
        'title_value' => '',
        'url' => 0
      ),
      'translatable' => '0',
      'type' => 'link_field'
    ),
    'field_instance' => array(
      'bundle' => 'user',
      'default_value' => NULL,
      'deleted' => '0',
      'description' => '',
      'display' => array(
        'default' => array(
          'label' => 'inline',
          'module' => 'link',
          'settings' => array(),
          'type' => 'link_default',
          'weight' => '6'
        ),
        'linked_node' => array(
          'label' => 'above',
          'settings' => array(),
          'type' => 'hidden',
          'weight' => 0
        )
      ),
      'entity_type' => 'user',
      'field_name' => 'field_related_links',
      'label' => 'Related links',
      'required' => 0,
      'settings' => array(
        'attributes' => array(
          'class' => '',
          'configurable_title' => 0,
          'rel' => '',
          'target' => 'default',
          'title' => ''
        ),
        'display' => array(
          'url_cutoff' => '80'
        ),
        'enable_tokens' => 1,
        'linkit' => array(
          'enable' => 0,
          'insert_plugin' => ''
        ),
        'title' => 'optional',
        'title_maxlength' => '128',
        'title_value' => '',
        'url' => 0,
        'user_register_form' => 1,
        'validate_url' => 1
      ),
      'widget' => array(
        'active' => 0,
        'module' => 'link',
        'settings' => array(),
        'type' => 'link_field',
        'weight' => '21'
      )
    )
  );
  // Exported field: 'user-user-field_standard_form'
  $fields['user-user-field_standard_form'] = array(
    'field_config' => array(
      'active' => '1',
      'cardinality' => '-1',
      'deleted' => '0',
      'entity_types' => array(),
      'field_name' => 'field_standard_form',
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
      'bundle' => 'user',
      'default_value' => NULL,
      'deleted' => '0',
      'description' => 'The standard form of a name is a surname, or an abbreviation of it (e.g. Adans. for Adanson), or rarely a contraction of it (e.g. Michx. for Michaux), with or without initials or other distinguishing appendages',
      'display' => array(
        'default' => array(
          'label' => 'inline',
          'module' => 'textformatter',
          'settings' => array(
            'textformatter_class' => 'textformatter-list',
            'textformatter_comma_and' => 0,
            'textformatter_comma_full_stop' => 0,
            'textformatter_comma_override' => 0,
            'textformatter_comma_tag' => 'div',
            'textformatter_contrib' => array(),
            'textformatter_separator_custom' => '',
            'textformatter_separator_custom_class' => 'textformatter-separator',
            'textformatter_separator_custom_tag' => 'span',
            'textformatter_term_plain' => 0,
            'textformatter_type' => 'comma'
          ),
          'type' => 'textformatter_list',
          'weight' => '5'
        ),
        'linked_node' => array(
          'label' => 'above',
          'settings' => array(),
          'type' => 'hidden',
          'weight' => 0
        )
      ),
      'entity_type' => 'user',
      'field_name' => 'field_standard_form',
      'label' => 'Standard Form',
      'required' => 0,
      'settings' => array(
        'linkit' => array(
          'enable' => 0,
          'insert_plugin' => ''
        ),
        'text_processing' => '0',
        'user_register_form' => 1
      ),
      'widget' => array(
        'active' => 1,
        'module' => 'text',
        'settings' => array(
          'size' => '60'
        ),
        'type' => 'text_textfield',
        'weight' => '30'
      )
    )
  );
  // Exported field: 'user-user-field_user_keywords'
  $fields['user-user-field_user_keywords'] = array(
    'field_config' => array(
      'active' => '1',
      'cardinality' => '-1',
      'deleted' => '0',
      'entity_types' => array(),
      'field_name' => 'field_user_keywords',
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
            'vocabulary' => 0,
            'parent' => 0
          ),
          1 => array(
            'parent' => 0,
            'vocabulary' => 0
          ),
          2 => array(
            'parent' => 0,
            'vocabulary' => 0
          ),
          3 => array(
            'parent' => 0,
            'vocabulary' => 0
          ),
          4 => array(
            'parent' => 0,
            'vocabulary' => 0
          ),
          5 => array(
            'parent' => 0,
            'vocabulary' => 0
          ),
          6 => array(
            'parent' => 0,
            'vocabulary' => 0
          ),
          7 => array(
            'parent' => 0,
            'vocabulary' => 'user_keywords'
          ),
          8 => array(
            'parent' => 0,
            'vocabulary' => 0
          ),
          9 => array(
            'parent' => 0,
            'vocabulary' => 0
          ),
          10 => array(
            'parent' => 0,
            'vocabulary' => 0
          )
        )
      ),
      'translatable' => '0',
      'type' => 'taxonomy_term_reference'
    ),
    'field_instance' => array(
      'bundle' => 'user',
      'default_value' => NULL,
      'deleted' => '0',
      'description' => 'A comma delimited list of key words that describe a person.',
      'display' => array(
        'default' => array(
          'label' => 'inline',
          'module' => 'taxonomy',
          'settings' => array(),
          'type' => 'taxonomy_term_reference_plain',
          'weight' => '4'
        ),
        'linked_node' => array(
          'label' => 'above',
          'settings' => array(),
          'type' => 'hidden',
          'weight' => 0
        )
      ),
      'entity_type' => 'user',
      'field_name' => 'field_user_keywords',
      'label' => 'Keywords',
      'required' => 0,
      'settings' => array(
        'user_register_form' => 1
      ),
      'widget' => array(
        'active' => 0,
        'module' => 'scratchpads_taxonomic_name_field',
        'settings' => array(
          'autocomplete_path' => 'taxonomy/autocomplete',
          'size' => 60
        ),
        'type' => 'taxonomy_autocomplete',
        'weight' => '19'
      )
    )
  );
  // Translatables
  // Included for use with string extractors like potx.
  t('A comma delimited list of key words that describe a person.');
  t('Allowed file types: png, gif, jpg, jpeg, pdf');
  t('Alternative Name');
  t('Areas of professional interest ');
  t('Biography');
  t('Birth Date');
  t('Birth Place');
  t('Comments');
  t('Date of Death');
  t('Do you want the user profile to be public?');
  t('Example of handwriting');
  t('Gender');
  t('If the exact date is unknown, please use a date range.');
  t('Keywords');
  t('Known for');
  t('Main collaborators/co-authors');
  t('Make this profile public?');
  t('ORCID identifier');
  t('ORCID is an open, non-profit, community-based effort to provide a registry of unique researcher identifiers and a transparent method of linking research activities and outputs to these identifiers. (Format of an orcid: 0000-0000-0000-0000)');
  t('Places worked');
  t('Related links');
  t('Standard Form');
  t('The standard form of a name is a surname, or an abbreviation of it (e.g. Adans. for Adanson), or rarely a contraction of it (e.g. Michx. for Michaux), with or without initials or other distinguishing appendages');
  return $fields;
}


