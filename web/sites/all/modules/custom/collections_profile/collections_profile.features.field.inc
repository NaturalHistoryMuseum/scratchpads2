<?php

/**
 * @file
 * collections_profile.features.field.inc
 */
/**
 * Implements hook_field_default_fields().
 */
function collections_profile_field_default_fields(){
  $fields = array();
  // Exported field: 'node-collections_profile-body'
  $fields['node-collections_profile-body'] = array(
    'field_config' => array(
      'active' => '1',
      'cardinality' => '1',
      'deleted' => '0',
      'entity_types' => array(
        0 => 'node'
      ),
      'field_name' => 'body',
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
      'type' => 'text_with_summary'
    ),
    'field_instance' => array(
      'bundle' => 'collections_profile',
      'default_value' => NULL,
      'deleted' => '0',
      'description' => '',
      'display' => array(
        'default' => array(
          'label' => 'hidden',
          'module' => 'text',
          'settings' => array(),
          'type' => 'text_default',
          'weight' => '2'
        ),
        'linked_node' => array(
          'label' => 'above',
          'settings' => array(),
          'type' => 'hidden',
          'weight' => 0
        ),
        'teaser' => array(
          'label' => 'hidden',
          'module' => 'text',
          'settings' => array(
            'trim_length' => 600
          ),
          'type' => 'text_summary_or_trimmed',
          'weight' => 0
        )
      ),
      'entity_type' => 'node',
      'field_name' => 'body',
      'label' => 'Body',
      'required' => FALSE,
      'settings' => array(
        'display_summary' => TRUE,
        'text_processing' => 1,
        'user_register_form' => FALSE
      ),
      'widget' => array(
        'module' => 'text',
        'settings' => array(
          'rows' => 20,
          'summary_rows' => 5
        ),
        'type' => 'text_textarea_with_summary',
        'weight' => '1'
      )
    )
  );
  // Exported field: 'node-collections_profile-field_coll_approx_size'
  $fields['node-collections_profile-field_coll_approx_size'] = array(
    'field_config' => array(
      'active' => '1',
      'cardinality' => '1',
      'deleted' => '0',
      'entity_types' => array(),
      'field_name' => 'field_coll_approx_size',
      'foreign keys' => array(),
      'indexes' => array(),
      'module' => 'number',
      'settings' => array(),
      'translatable' => '0',
      'type' => 'number_integer'
    ),
    'field_instance' => array(
      'bundle' => 'collections_profile',
      'default_value' => NULL,
      'deleted' => '0',
      'description' => 'An integer representing the approximate number in collection',
      'display' => array(
        'default' => array(
          'label' => 'inline',
          'module' => 'number',
          'settings' => array(
            'decimal_separator' => '.',
            'prefix_suffix' => TRUE,
            'scale' => 0,
            'thousand_separator' => ' '
          ),
          'type' => 'number_integer',
          'weight' => '3'
        ),
        'linked_node' => array(
          'label' => 'above',
          'settings' => array(),
          'type' => 'hidden',
          'weight' => 0
        ),
        'teaser' => array(
          'label' => 'above',
          'settings' => array(),
          'type' => 'hidden',
          'weight' => 0
        )
      ),
      'entity_type' => 'node',
      'field_name' => 'field_coll_approx_size',
      'label' => 'Approximate size of collection',
      'required' => 0,
      'settings' => array(
        'max' => '',
        'min' => '',
        'prefix' => '',
        'suffix' => '',
        'user_register_form' => FALSE
      ),
      'widget' => array(
        'active' => 0,
        'module' => 'number',
        'settings' => array(),
        'type' => 'number',
        'weight' => '6'
      )
    )
  );
  // Exported field: 'node-collections_profile-field_coll_digitization_notes'
  $fields['node-collections_profile-field_coll_digitization_notes'] = array(
    'field_config' => array(
      'active' => '1',
      'cardinality' => '1',
      'deleted' => '0',
      'entity_types' => array(),
      'field_name' => 'field_coll_digitization_notes',
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
      'bundle' => 'collections_profile',
      'default_value' => NULL,
      'deleted' => '0',
      'description' => '',
      'display' => array(
        'default' => array(
          'label' => 'inline',
          'module' => 'text',
          'settings' => array(),
          'type' => 'text_default',
          'weight' => '10'
        ),
        'linked_node' => array(
          'label' => 'above',
          'settings' => array(),
          'type' => 'hidden',
          'weight' => 0
        ),
        'teaser' => array(
          'label' => 'above',
          'settings' => array(),
          'type' => 'hidden',
          'weight' => 0
        )
      ),
      'entity_type' => 'node',
      'field_name' => 'field_coll_digitization_notes',
      'label' => 'Notes on collection digitization',
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
        'weight' => '11'
      )
    )
  );
  // Exported field: 'node-collections_profile-field_coll_earliest_spec_date'
  $fields['node-collections_profile-field_coll_earliest_spec_date'] = array(
    'field_config' => array(
      'active' => '1',
      'cardinality' => '1',
      'deleted' => '0',
      'entity_types' => array(),
      'field_name' => 'field_coll_earliest_spec_date',
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
        'todate' => '',
        'tz_handling' => 'none'
      ),
      'translatable' => '0',
      'type' => 'datetime'
    ),
    'field_instance' => array(
      'bundle' => 'collections_profile',
      'deleted' => '0',
      'description' => '',
      'display' => array(
        'default' => array(
          'label' => 'inline',
          'module' => 'date',
          'settings' => array(
            'format_type' => 'long',
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
        ),
        'teaser' => array(
          'label' => 'above',
          'settings' => array(),
          'type' => 'hidden',
          'weight' => 0
        )
      ),
      'entity_type' => 'node',
      'field_name' => 'field_coll_earliest_spec_date',
      'label' => 'Date of earliest specimen',
      'required' => 0,
      'settings' => array(
        'default_value' => 'now',
        'default_value2' => 'same',
        'default_value_code' => '',
        'default_value_code2' => '',
        'user_register_form' => FALSE
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
        'weight' => '8'
      )
    )
  );
  // Exported field: 'node-collections_profile-field_coll_institution_code'
  $fields['node-collections_profile-field_coll_institution_code'] = array(
    'field_config' => array(
      'active' => '1',
      'cardinality' => '1',
      'deleted' => '0',
      'entity_types' => array(),
      'field_name' => 'field_coll_institution_code',
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
        'max_length' => '100'
      ),
      'translatable' => '0',
      'type' => 'text'
    ),
    'field_instance' => array(
      'bundle' => 'collections_profile',
      'default_value' => NULL,
      'deleted' => '0',
      'description' => 'The code which connects institution adminstering the collection. This code is the same one as listed under institution code in specimen/observations and links this profile to the specimens for statistics etc.',
      'display' => array(
        'default' => array(
          'label' => 'inline',
          'settings' => array(),
          'type' => 'hidden',
          'weight' => '11'
        ),
        'linked_node' => array(
          'label' => 'above',
          'settings' => array(),
          'type' => 'hidden',
          'weight' => 0
        ),
        'teaser' => array(
          'label' => 'above',
          'settings' => array(),
          'type' => 'hidden',
          'weight' => 0
        )
      ),
      'entity_type' => 'node',
      'field_name' => 'field_coll_institution_code',
      'label' => 'Institution collection code',
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
        'weight' => '4'
      )
    )
  );
  // Exported field: 'node-collections_profile-field_coll_institution_image'
  $fields['node-collections_profile-field_coll_institution_image'] = array(
    'field_config' => array(
      'active' => '1',
      'cardinality' => '1',
      'deleted' => '0',
      'entity_types' => array(),
      'field_name' => 'field_coll_institution_image',
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
        'default_image' => 0,
        'uri_scheme' => 'public'
      ),
      'translatable' => '0',
      'type' => 'file'
    ),
    'field_instance' => array(
      'bundle' => 'collections_profile',
      'deleted' => '0',
      'description' => 'This is an introductory image which will display as a small thumbnail in the text',
      'display' => array(
        'default' => array(
          'label' => 'hidden',
          'module' => 'styles',
          'settings' => array(),
          'type' => 'styles_file_square_thumbnail',
          'weight' => '1'
        ),
        'linked_node' => array(
          'label' => 'above',
          'settings' => array(),
          'type' => 'hidden',
          'weight' => 0
        ),
        'teaser' => array(
          'label' => 'above',
          'settings' => array(),
          'type' => 'hidden',
          'weight' => 0
        )
      ),
      'entity_type' => 'node',
      'field_name' => 'field_coll_institution_image',
      'label' => 'Institution image',
      'required' => 0,
      'settings' => array(
        'alt_field' => 0,
        'default_image' => 0,
        'file_directory' => '',
        'file_extensions' => 'png gif jpg jpeg',
        'max_filesize' => '',
        'max_resolution' => '',
        'min_resolution' => '',
        'title_field' => 0,
        'user_register_form' => FALSE
      ),
      'widget' => array(
        'module' => 'media_multiselect',
        'settings' => array(
          'allowed_schemes' => array(
            'module' => 0,
            'public' => 'public'
          ),
          'allowed_types' => array(
            'image' => 'image'
          ),
          'browser_plugins' => array(),
          'progress_indicator' => 'throbber'
        ),
        'type' => 'media_multiselect',
        'weight' => '4'
      )
    )
  );
  // Exported field: 'node-collections_profile-field_coll_institution_location'
  $fields['node-collections_profile-field_coll_institution_location'] = array(
    'field_config' => array(
      'active' => '1',
      'cardinality' => '1',
      'deleted' => '0',
      'entity_types' => array(),
      'field_name' => 'field_coll_institution_location',
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
          'biblio' => 0,
          'collections_profile' => 0,
          'location' => 'location',
          'media_gallery' => 0,
          'museum_profile' => 0,
          'page' => 0,
          'specimen_observation' => 0,
          'spm' => 0
        ),
        'view' => array(
          'args' => array(),
          'display_name' => '',
          'view_name' => ''
        )
      ),
      'translatable' => '0',
      'type' => 'node_reference'
    ),
    'field_instance' => array(
      'bundle' => 'collections_profile',
      'default_value' => NULL,
      'deleted' => '0',
      'description' => '',
      'display' => array(
        'default' => array(
          'label' => 'inline',
          'module' => 'node_reference',
          'settings' => array(),
          'type' => 'node_reference_default',
          'weight' => '9'
        ),
        'linked_node' => array(
          'label' => 'above',
          'settings' => array(),
          'type' => 'hidden',
          'weight' => 0
        ),
        'teaser' => array(
          'label' => 'above',
          'settings' => array(),
          'type' => 'hidden',
          'weight' => 0
        )
      ),
      'entity_type' => 'node',
      'entityconnect_unload_add' => '1',
      'entityconnect_unload_edit' => '1',
      'field_name' => 'field_coll_institution_location',
      'label' => 'Institution location',
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
        'weight' => '5'
      )
    )
  );
  // Exported field: 'node-collections_profile-field_coll_institution_name'
  $fields['node-collections_profile-field_coll_institution_name'] = array(
    'field_config' => array(
      'active' => '1',
      'cardinality' => '1',
      'deleted' => '0',
      'entity_types' => array(),
      'field_name' => 'field_coll_institution_name',
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
      'bundle' => 'collections_profile',
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
        ),
        'teaser' => array(
          'label' => 'above',
          'settings' => array(),
          'type' => 'hidden',
          'weight' => 0
        )
      ),
      'entity_type' => 'node',
      'field_name' => 'field_coll_institution_name',
      'label' => 'Institution name',
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
        'weight' => '2'
      )
    )
  );
  // Exported field: 'node-collections_profile-field_coll_main_period'
  $fields['node-collections_profile-field_coll_main_period'] = array(
    'field_config' => array(
      'active' => '1',
      'cardinality' => '1',
      'deleted' => '0',
      'entity_types' => array(),
      'field_name' => 'field_coll_main_period',
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
      'bundle' => 'collections_profile',
      'default_value' => NULL,
      'deleted' => '0',
      'description' => '',
      'display' => array(
        'default' => array(
          'label' => 'inline',
          'module' => 'text',
          'settings' => array(),
          'type' => 'text_default',
          'weight' => '6'
        ),
        'linked_node' => array(
          'label' => 'above',
          'settings' => array(),
          'type' => 'hidden',
          'weight' => 0
        ),
        'teaser' => array(
          'label' => 'above',
          'settings' => array(),
          'type' => 'hidden',
          'weight' => 0
        )
      ),
      'entity_type' => 'node',
      'field_name' => 'field_coll_main_period',
      'label' => 'Main collecting period',
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
        'weight' => '7'
      )
    )
  );
  // Exported field: 'node-collections_profile-field_coll_signif_collectors'
  $fields['node-collections_profile-field_coll_signif_collectors'] = array(
    'field_config' => array(
      'active' => '1',
      'cardinality' => '1',
      'deleted' => '0',
      'entity_types' => array(),
      'field_name' => 'field_coll_signif_collectors',
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
          2 => 0,
          3 => 0,
          4 => 0,
          5 => 0
        ),
        'referenceable_status' => array(
          0 => 0,
          1 => 0
        ),
        'view' => array(
          'args' => array(),
          'display_name' => '',
          'view_name' => ''
        )
      ),
      'translatable' => '0',
      'type' => 'user_reference'
    ),
    'field_instance' => array(
      'bundle' => 'collections_profile',
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
        ),
        'teaser' => array(
          'label' => 'above',
          'settings' => array(),
          'type' => 'hidden',
          'weight' => 0
        )
      ),
      'entity_type' => 'node',
      'entityconnect_unload_add' => '1',
      'entityconnect_unload_edit' => '1',
      'field_name' => 'field_coll_signif_collectors',
      'label' => 'Significant collectors incorporated',
      'required' => 0,
      'settings' => array(
        'user_register_form' => FALSE
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
        'weight' => '10'
      )
    )
  );
  // Exported field: 'node-collections_profile-field_coll_significant'
  $fields['node-collections_profile-field_coll_significant'] = array(
    'field_config' => array(
      'active' => '1',
      'cardinality' => '1',
      'deleted' => '0',
      'entity_types' => array(),
      'field_name' => 'field_coll_significant',
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
      'bundle' => 'collections_profile',
      'default_value' => NULL,
      'deleted' => '0',
      'description' => '',
      'display' => array(
        'default' => array(
          'label' => 'inline',
          'module' => 'text',
          'settings' => array(),
          'type' => 'text_default',
          'weight' => '4'
        ),
        'linked_node' => array(
          'label' => 'above',
          'settings' => array(),
          'type' => 'hidden',
          'weight' => 0
        ),
        'teaser' => array(
          'label' => 'above',
          'settings' => array(),
          'type' => 'hidden',
          'weight' => 0
        )
      ),
      'entity_type' => 'node',
      'field_name' => 'field_coll_significant',
      'label' => 'Significant collections incorporated',
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
        'weight' => '9'
      )
    )
  );
  // Exported field: 'node-collections_profile-field_coll_website'
  $fields['node-collections_profile-field_coll_website'] = array(
    'field_config' => array(
      'active' => '1',
      'cardinality' => '1',
      'deleted' => '0',
      'entity_types' => array(),
      'field_name' => 'field_coll_website',
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
      'bundle' => 'collections_profile',
      'default_value' => NULL,
      'deleted' => '0',
      'description' => '',
      'display' => array(
        'default' => array(
          'label' => 'inline',
          'module' => 'link',
          'settings' => array(),
          'type' => 'link_default',
          'weight' => '8'
        ),
        'linked_node' => array(
          'label' => 'above',
          'settings' => array(),
          'type' => 'hidden',
          'weight' => 0
        ),
        'teaser' => array(
          'label' => 'above',
          'settings' => array(),
          'type' => 'hidden',
          'weight' => 0
        )
      ),
      'entity_type' => 'node',
      'field_name' => 'field_coll_website',
      'label' => 'Collections website',
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
        'rel_remove' => 'default',
        'title' => 'optional',
        'title_maxlength' => '128',
        'title_value' => '',
        'url' => 0,
        'user_register_form' => FALSE,
        'validate_url' => 1
      ),
      'widget' => array(
        'active' => 0,
        'module' => 'link',
        'settings' => array(),
        'type' => 'link_field',
        'weight' => '12'
      )
    )
  );
  // Translatables
  // Included for use with string extractors like potx.
  t('An integer representing the approximate number in collection');
  t('Approximate size of collection');
  t('Body');
  t('Collections website');
  t('Date of earliest specimen');
  t('Institution collection code');
  t('Institution image');
  t('Institution location');
  t('Institution name');
  t('Main collecting period');
  t('Notes on collection digitization');
  t('Significant collections incorporated');
  t('Significant collectors incorporated');
  t('The code which connects institution adminstering the collection. This code is the same one as listed under institution code in specimen/observations and links this profile to the specimens for statistics etc.');
  t('This is an introductory image which will display as a small thumbnail in the text');
  return $fields;
}
