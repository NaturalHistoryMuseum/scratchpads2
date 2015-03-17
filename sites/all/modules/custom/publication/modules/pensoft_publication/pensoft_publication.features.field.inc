<?php

/**
 * @file
 * pensoft_publication.features.field.inc
 */
/**
 * Implements hook_field_default_fields().
 */
function pensoft_publication_field_default_fields(){
  $fields = array();
  // Exported field: 'field_collection_item-field_publication_authors-field_publication_author_rights'
  $fields['field_collection_item-field_publication_authors-field_publication_author_rights'] = array(
    'field_config' => array(
      'active' => '1',
      'cardinality' => '1',
      'deleted' => '0',
      'entity_types' => array(),
      'field_name' => 'field_publication_author_rights',
      'foreign keys' => array(),
      'indexes' => array(
        'value' => array(
          0 => 'value'
        )
      ),
      'module' => 'list',
      'settings' => array(
        'allowed_values' => array(
          'Edit & comment' => 'Edit & comment',
          'Comment only' => 'Comment only'
        ),
        'allowed_values_function' => ''
      ),
      'translatable' => '0',
      'type' => 'list_text'
    ),
    'field_instance' => array(
      'bundle' => 'field_publication_authors',
      'default_value' => array(
        0 => array(
          'value' => 'Edit & comment'
        )
      ),
      'deleted' => '0',
      'description' => '',
      'display' => array(
        'default' => array(
          'label' => 'above',
          'module' => 'list',
          'settings' => array(),
          'type' => 'list_default',
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
      'field_name' => 'field_publication_author_rights',
      'label' => 'Author rights',
      'required' => 0,
      'settings' => array(
        'user_register_form' => FALSE
      ),
      'widget' => array(
        'active' => 1,
        'module' => 'options',
        'settings' => array(),
        'type' => 'options_select',
        'weight' => '2'
      )
    )
  );
  // Exported field: 'field_collection_item-field_publication_authors-field_publication_author_submit'
  $fields['field_collection_item-field_publication_authors-field_publication_author_submit'] = array(
    'field_config' => array(
      'active' => '1',
      'cardinality' => '1',
      'deleted' => '0',
      'entity_types' => array(),
      'field_name' => 'field_publication_author_submit',
      'foreign keys' => array(),
      'indexes' => array(
        'value' => array(
          0 => 'value'
        )
      ),
      'module' => 'list',
      'settings' => array(
        'allowed_values' => array(
          0 => '0',
          1 => '1'
        ),
        'allowed_values_function' => ''
      ),
      'translatable' => '0',
      'type' => 'list_boolean'
    ),
    'field_instance' => array(
      'bundle' => 'field_publication_authors',
      'default_value' => array(
        0 => array(
          'value' => 0
        )
      ),
      'deleted' => '0',
      'description' => '',
      'display' => array(
        'default' => array(
          'label' => 'above',
          'module' => 'list',
          'settings' => array(),
          'type' => 'list_default',
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
      'field_name' => 'field_publication_author_submit',
      'label' => 'Submitting author?',
      'required' => 0,
      'settings' => array(
        'user_register_form' => FALSE
      ),
      'widget' => array(
        'active' => 1,
        'module' => 'options',
        'settings' => array(
          'display_label' => 1
        ),
        'type' => 'options_onoff',
        'weight' => '1'
      )
    )
  );
  // Exported field: 'field_collection_item-field_publication_authors-field_publication_author_user'
  $fields['field_collection_item-field_publication_authors-field_publication_author_user'] = array(
    'field_config' => array(
      'active' => '1',
      'cardinality' => '1',
      'deleted' => '0',
      'entity_types' => array(),
      'field_name' => 'field_publication_author_user',
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
          'display_name' => 'references_1',
          'view_name' => 'users_references'
        )
      ),
      'translatable' => '0',
      'type' => 'user_reference'
    ),
    'field_instance' => array(
      'bundle' => 'field_publication_authors',
      'default_value' => NULL,
      'deleted' => '0',
      'description' => '',
      'display' => array(
        'default' => array(
          'label' => 'above',
          'module' => 'publication',
          'settings' => array(),
          'type' => 'publication_author',
          'weight' => '0'
        ),
        'linked_node' => array(
          'label' => 'above',
          'settings' => array(),
          'type' => 'hidden',
          'weight' => 0
        )
      ),
      'entity_type' => 'field_collection_item',
      'entityconnect_unload_add' => '1',
      'entityconnect_unload_edit' => '1',
      'field_name' => 'field_publication_author_user',
      'label' => 'Author',
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
        'weight' => '0'
      )
    )
  );
  // Exported field: 'field_collection_item-field_publication_contributors-field_publication_cbtor_rights'
  $fields['field_collection_item-field_publication_contributors-field_publication_cbtor_rights'] = array(
    'field_config' => array(
      'active' => '1',
      'cardinality' => '1',
      'deleted' => '0',
      'entity_types' => array(),
      'field_name' => 'field_publication_cbtor_rights',
      'foreign keys' => array(),
      'indexes' => array(
        'value' => array(
          0 => 'value'
        )
      ),
      'module' => 'list',
      'settings' => array(
        'allowed_values' => array(
          'Edit & comment' => 'Edit & comment',
          'Comment only' => 'Comment only'
        ),
        'allowed_values_function' => ''
      ),
      'translatable' => '0',
      'type' => 'list_text'
    ),
    'field_instance' => array(
      'bundle' => 'field_publication_contributors',
      'default_value' => NULL,
      'deleted' => '0',
      'description' => '',
      'display' => array(
        'default' => array(
          'label' => 'above',
          'module' => 'list',
          'settings' => array(),
          'type' => 'list_default',
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
      'field_name' => 'field_publication_cbtor_rights',
      'label' => 'Contributor rights',
      'required' => 0,
      'settings' => array(
        'user_register_form' => FALSE
      ),
      'widget' => array(
        'active' => 1,
        'module' => 'options',
        'settings' => array(),
        'type' => 'options_select',
        'weight' => '2'
      )
    )
  );
  // Exported field: 'field_collection_item-field_publication_contributors-field_publication_cbtor_role'
  $fields['field_collection_item-field_publication_contributors-field_publication_cbtor_role'] = array(
    'field_config' => array(
      'active' => '1',
      'cardinality' => '1',
      'deleted' => '0',
      'entity_types' => array(),
      'field_name' => 'field_publication_cbtor_role',
      'foreign keys' => array(),
      'indexes' => array(
        'value' => array(
          0 => 'value'
        )
      ),
      'module' => 'list',
      'settings' => array(
        'allowed_values' => array(
          'Copy editor' => 'Copy editor',
          'Mentor' => 'Mentor',
          'Potential reviewer' => 'Potential reviewer',
          'Colleague / Friend' => 'Colleague / Friend'
        ),
        'allowed_values_function' => ''
      ),
      'translatable' => '0',
      'type' => 'list_text'
    ),
    'field_instance' => array(
      'bundle' => 'field_publication_contributors',
      'default_value' => NULL,
      'deleted' => '0',
      'description' => '',
      'display' => array(
        'default' => array(
          'label' => 'above',
          'module' => 'list',
          'settings' => array(),
          'type' => 'list_default',
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
      'field_name' => 'field_publication_cbtor_role',
      'label' => 'Contributor role',
      'required' => 0,
      'settings' => array(
        'user_register_form' => FALSE
      ),
      'widget' => array(
        'active' => 1,
        'module' => 'options',
        'settings' => array(),
        'type' => 'options_select',
        'weight' => '3'
      )
    )
  );
  // Exported field: 'field_collection_item-field_publication_contributors-field_publication_cbtor_user'
  $fields['field_collection_item-field_publication_contributors-field_publication_cbtor_user'] = array(
    'field_config' => array(
      'active' => '1',
      'cardinality' => '1',
      'deleted' => '0',
      'entity_types' => array(),
      'field_name' => 'field_publication_cbtor_user',
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
          'display_name' => 'references_1',
          'view_name' => 'users_references'
        )
      ),
      'translatable' => '0',
      'type' => 'user_reference'
    ),
    'field_instance' => array(
      'bundle' => 'field_publication_contributors',
      'default_value' => NULL,
      'deleted' => '0',
      'description' => '',
      'display' => array(
        'default' => array(
          'label' => 'above',
          'module' => 'publication',
          'settings' => array(),
          'type' => 'publication_author',
          'weight' => '0'
        ),
        'linked_node' => array(
          'label' => 'above',
          'settings' => array(),
          'type' => 'hidden',
          'weight' => 0
        )
      ),
      'entity_type' => 'field_collection_item',
      'entityconnect_unload_add' => '1',
      'entityconnect_unload_edit' => '1',
      'field_name' => 'field_publication_cbtor_user',
      'label' => 'Contributor',
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
        'weight' => '1'
      )
    )
  );
  // Exported field: 'field_collection_item-field_publication_figures-field_publication_figure_desc'
  $fields['field_collection_item-field_publication_figures-field_publication_figure_desc'] = array(
    'field_config' => array(
      'active' => '1',
      'cardinality' => '1',
      'deleted' => '0',
      'entity_types' => array(),
      'field_name' => 'field_publication_figure_desc',
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
      'bundle' => 'field_publication_figures',
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
        )
      ),
      'entity_type' => 'field_collection_item',
      'field_name' => 'field_publication_figure_desc',
      'label' => 'Description',
      'required' => 0,
      'settings' => array(
        'linkit' => array(
          'enable' => 0,
          'insert_plugin' => ''
        ),
        'text_processing' => '1',
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
  );
  // Exported field: 'field_collection_item-field_publication_figures-field_publication_figure_figure'
  $fields['field_collection_item-field_publication_figures-field_publication_figure_figure'] = array(
    'field_config' => array(
      'active' => '1',
      'cardinality' => '1',
      'deleted' => '0',
      'entity_types' => array(),
      'field_name' => 'field_publication_figure_figure',
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
      'module' => 'image',
      'settings' => array(
        'default_image' => 0,
        'uri_scheme' => 'public'
      ),
      'translatable' => '0',
      'type' => 'image'
    ),
    'field_instance' => array(
      'bundle' => 'field_publication_figures',
      'deleted' => '0',
      'description' => '',
      'display' => array(
        'default' => array(
          'label' => 'hidden',
          'module' => 'image',
          'settings' => array(
            'image_link' => 'content',
            'image_style' => 'medium'
          ),
          'type' => 'image',
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
      'field_name' => 'field_publication_figure_figure',
      'label' => 'Figure',
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
        'active' => 1,
        'module' => 'media',
        'settings' => array(
          'allowed_schemes' => array(),
          'allowed_types' => array(
            'audio' => 0,
            'default' => 0,
            'image' => 'image',
            'video' => 0
          ),
          'browser_plugins' => array(
            'media_default--media_browser_1' => 0,
            'media_default--media_browser_my_files' => 0,
            'upload' => 0
          ),
          'progress_indicator' => 'throbber'
        ),
        'type' => 'media_generic',
        'weight' => '2'
      )
    )
  );
  // Exported field: 'field_collection_item-field_publication_figures-field_publication_figure_title'
  $fields['field_collection_item-field_publication_figures-field_publication_figure_title'] = array(
    'field_config' => array(
      'active' => '1',
      'cardinality' => '1',
      'deleted' => '0',
      'entity_types' => array(),
      'field_name' => 'field_publication_figure_title',
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
      'bundle' => 'field_publication_figures',
      'default_value' => NULL,
      'deleted' => '0',
      'description' => '',
      'display' => array(
        'default' => array(
          'label' => 'hidden',
          'module' => 'text',
          'settings' => array(),
          'type' => 'text_default',
          'weight' => '0'
        ),
        'linked_node' => array(
          'label' => 'above',
          'settings' => array(),
          'type' => 'hidden',
          'weight' => 0
        )
      ),
      'entity_type' => 'field_collection_item',
      'field_name' => 'field_publication_figure_title',
      'label' => 'Title',
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
          'size' => '200'
        ),
        'type' => 'text_textfield',
        'weight' => '1'
      )
    )
  );
  // Exported field: 'field_collection_item-field_publication_tables-field_publication_table'
  $fields['field_collection_item-field_publication_tables-field_publication_table'] = array(
    'field_config' => array(
      'active' => '1',
      'cardinality' => '1',
      'deleted' => '0',
      'entity_types' => array(),
      'field_name' => 'field_publication_table',
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
      'bundle' => 'field_publication_tables',
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
        )
      ),
      'entity_type' => 'field_collection_item',
      'field_name' => 'field_publication_table',
      'label' => 'Table',
      'required' => 0,
      'settings' => array(
        'linkit' => array(
          'enable' => 0,
          'insert_plugin' => ''
        ),
        'text_processing' => '1',
        'user_register_form' => FALSE
      ),
      'widget' => array(
        'active' => 1,
        'module' => 'text',
        'settings' => array(
          'rows' => '5'
        ),
        'type' => 'text_textarea',
        'weight' => '2'
      )
    )
  );
  // Exported field: 'field_collection_item-field_publication_tables-field_publication_table_id'
  $fields['field_collection_item-field_publication_tables-field_publication_table_id'] = array(
    'field_config' => array(
      'active' => '1',
      'cardinality' => '1',
      'deleted' => '0',
      'entity_types' => array(),
      'field_name' => 'field_publication_table_id',
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
      'bundle' => 'field_publication_tables',
      'default_value' => NULL,
      'deleted' => '0',
      'description' => 'This is the table id, and it is used to track references to table. Do not change after a table has been created, otherwise references to the table will not work.
Tables are numbered once the publication is saved.',
      'display' => array(
        'default' => array(
          'label' => 'inline',
          'module' => 'text',
          'settings' => array(),
          'type' => 'text_default',
          'weight' => '0'
        ),
        'linked_node' => array(
          'label' => 'above',
          'settings' => array(),
          'type' => 'hidden',
          'weight' => 0
        )
      ),
      'entity_type' => 'field_collection_item',
      'field_name' => 'field_publication_table_id',
      'label' => 'Table id',
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
        'weight' => 0
      )
    )
  );
  // Exported field: 'field_collection_item-field_publication_tables-field_publication_table_title'
  $fields['field_collection_item-field_publication_tables-field_publication_table_title'] = array(
    'field_config' => array(
      'active' => '1',
      'cardinality' => '1',
      'deleted' => '0',
      'entity_types' => array(),
      'field_name' => 'field_publication_table_title',
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
      'bundle' => 'field_publication_tables',
      'default_value' => NULL,
      'deleted' => '0',
      'description' => '',
      'display' => array(
        'default' => array(
          'label' => 'hidden',
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
      'field_name' => 'field_publication_table_title',
      'label' => 'Title',
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
          'size' => '200'
        ),
        'type' => 'text_textfield',
        'weight' => '1'
      )
    )
  );
  // Exported field: 'publication-pensoft-field_pensoft_acknowledgments'
  $fields['publication-pensoft-field_pensoft_acknowledgments'] = array(
    'field_config' => array(
      'active' => '1',
      'cardinality' => '1',
      'deleted' => '0',
      'entity_types' => array(),
      'field_name' => 'field_pensoft_acknowledgments',
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
      'bundle' => 'pensoft',
      'default_value' => NULL,
      'deleted' => '0',
      'description' => '',
      'display' => array(
        'default' => array(
          'label' => 'above',
          'module' => 'text',
          'settings' => array(),
          'type' => 'text_default',
          'weight' => '15'
        )
      ),
      'entity_type' => 'publication',
      'field_name' => 'field_pensoft_acknowledgments',
      'label' => 'Acknowledgments',
      'required' => 0,
      'settings' => array(
        'linkit' => array(
          'enable' => 0,
          'insert_plugin' => ''
        ),
        'text_processing' => '1',
        'user_register_form' => FALSE
      ),
      'widget' => array(
        'active' => 1,
        'module' => 'text',
        'settings' => array(
          'rows' => '5'
        ),
        'type' => 'text_textarea',
        'weight' => '12'
      )
    )
  );
  // Exported field: 'publication-pensoft-field_pensoft_files'
  $fields['publication-pensoft-field_pensoft_files'] = array(
    'field_config' => array(
      'active' => '1',
      'cardinality' => '-1',
      'deleted' => '0',
      'entity_types' => array(),
      'field_name' => 'field_pensoft_files',
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
      'bundle' => 'pensoft',
      'deleted' => '0',
      'description' => '',
      'display' => array(
        'default' => array(
          'label' => 'above',
          'module' => 'styles',
          'settings' => array(),
          'type' => 'styles_file_thumbnail',
          'weight' => '21'
        )
      ),
      'entity_type' => 'publication',
      'field_name' => 'field_pensoft_files',
      'label' => 'Supplementary files',
      'required' => 0,
      'settings' => array(
        'description_field' => 0,
        'file_directory' => '',
        'file_extensions' => 'txt jpg png gif xls xlsx doc docx pdf xml svg wav mp3 avi mpg',
        'max_filesize' => '',
        'user_register_form' => FALSE
      ),
      'widget' => array(
        'active' => 1,
        'module' => 'media',
        'settings' => array(
          'allowed_schemes' => array(),
          'allowed_types' => array(
            'audio' => 0,
            'default' => 'default',
            'image' => 'image',
            'video' => 0
          ),
          'browser_plugins' => array(
            'library' => 'library',
            'media_default--media_browser_1' => 0,
            'upload' => 'upload'
          ),
          'progress_indicator' => 'throbber'
        ),
        'type' => 'media_generic',
        'weight' => '13'
      )
    )
  );
  // Exported field: 'publication-pensoft-field_publication_abstract'
  $fields['publication-pensoft-field_publication_abstract'] = array(
    'field_config' => array(
      'active' => '1',
      'cardinality' => '1',
      'deleted' => '0',
      'entity_types' => array(),
      'field_name' => 'field_publication_abstract',
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
        'profile2_private' => FALSE
      ),
      'translatable' => '0',
      'type' => 'text_long'
    ),
    'field_instance' => array(
      'bundle' => 'pensoft',
      'default_value' => NULL,
      'deleted' => '0',
      'description' => '',
      'display' => array(
        'default' => array(
          'label' => 'above',
          'module' => 'text',
          'settings' => array(),
          'type' => 'text_default',
          'weight' => '3'
        )
      ),
      'entity_type' => 'publication',
      'field_name' => 'field_publication_abstract',
      'label' => 'Abstract',
      'required' => 0,
      'settings' => array(
        'text_processing' => '1',
        'user_register_form' => FALSE
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
  // Exported field: 'publication-pensoft-field_publication_authors'
  $fields['publication-pensoft-field_publication_authors'] = array(
    'field_config' => array(
      'active' => '1',
      'cardinality' => '-1',
      'deleted' => '0',
      'entity_types' => array(),
      'field_name' => 'field_publication_authors',
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
      'bundle' => 'pensoft',
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
          'weight' => 22
        )
      ),
      'entity_type' => 'publication',
      'field_name' => 'field_publication_authors',
      'label' => 'Authors',
      'required' => 0,
      'settings' => array(
        'user_register_form' => FALSE
      ),
      'widget' => array(
        'active' => 0,
        'module' => 'field_collection',
        'settings' => array(),
        'type' => 'field_collection_embed',
        'weight' => '35'
      )
    )
  );
  // Exported field: 'publication-pensoft-field_publication_contributors'
  $fields['publication-pensoft-field_publication_contributors'] = array(
    'field_config' => array(
      'active' => '1',
      'cardinality' => '-1',
      'deleted' => '0',
      'entity_types' => array(),
      'field_name' => 'field_publication_contributors',
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
      'bundle' => 'pensoft',
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
          'weight' => 23
        )
      ),
      'entity_type' => 'publication',
      'field_name' => 'field_publication_contributors',
      'label' => 'Contributors',
      'required' => 0,
      'settings' => array(
        'user_register_form' => FALSE
      ),
      'widget' => array(
        'active' => 0,
        'module' => 'field_collection',
        'settings' => array(),
        'type' => 'field_collection_embed',
        'weight' => '36'
      )
    )
  );
  // Exported field: 'publication-pensoft-field_publication_data_resources'
  $fields['publication-pensoft-field_publication_data_resources'] = array(
    'field_config' => array(
      'active' => '1',
      'cardinality' => '1',
      'deleted' => '0',
      'entity_types' => array(),
      'field_name' => 'field_publication_data_resources',
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
      'bundle' => 'pensoft',
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
        )
      ),
      'entity_type' => 'publication',
      'field_name' => 'field_publication_data_resources',
      'label' => 'Data resources',
      'required' => 0,
      'settings' => array(
        'linkit' => array(
          'enable' => 0,
          'insert_plugin' => ''
        ),
        'text_processing' => '1',
        'user_register_form' => FALSE
      ),
      'widget' => array(
        'active' => 1,
        'module' => 'text',
        'settings' => array(
          'rows' => '5'
        ),
        'type' => 'text_textarea',
        'weight' => '4'
      )
    )
  );
  // Exported field: 'publication-pensoft-field_publication_discussion'
  $fields['publication-pensoft-field_publication_discussion'] = array(
    'field_config' => array(
      'active' => '1',
      'cardinality' => '1',
      'deleted' => '0',
      'entity_types' => array(),
      'field_name' => 'field_publication_discussion',
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
      'bundle' => 'pensoft',
      'default_value' => NULL,
      'deleted' => '0',
      'description' => '',
      'display' => array(
        'default' => array(
          'label' => 'above',
          'module' => 'text',
          'settings' => array(),
          'type' => 'text_default',
          'weight' => '14'
        )
      ),
      'entity_type' => 'publication',
      'field_name' => 'field_publication_discussion',
      'label' => 'Discussion',
      'required' => 0,
      'settings' => array(
        'linkit' => array(
          'enable' => 0,
          'insert_plugin' => ''
        ),
        'text_processing' => '1',
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
  // Exported field: 'publication-pensoft-field_publication_figures'
  $fields['publication-pensoft-field_publication_figures'] = array(
    'field_config' => array(
      'active' => '1',
      'cardinality' => '-1',
      'deleted' => '0',
      'entity_types' => array(),
      'field_name' => 'field_publication_figures',
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
      'bundle' => 'pensoft',
      'default_value' => NULL,
      'deleted' => '0',
      'description' => '',
      'display' => array(
        'default' => array(
          'label' => 'above',
          'module' => 'field_collection',
          'settings' => array(
            'add' => '',
            'delete' => '',
            'description' => 0,
            'edit' => '',
            'view_mode' => 'full'
          ),
          'type' => 'field_collection_view',
          'weight' => '19'
        )
      ),
      'entity_type' => 'publication',
      'field_name' => 'field_publication_figures',
      'label' => 'Figures',
      'required' => 0,
      'settings' => array(
        'user_register_form' => FALSE
      ),
      'widget' => array(
        'active' => 0,
        'module' => 'field_collection',
        'settings' => array(),
        'type' => 'field_collection_embed',
        'weight' => '34'
      )
    )
  );
  // Exported field: 'publication-pensoft-field_publication_introduction'
  $fields['publication-pensoft-field_publication_introduction'] = array(
    'field_config' => array(
      'active' => '1',
      'cardinality' => '1',
      'deleted' => '0',
      'entity_types' => array(),
      'field_name' => 'field_publication_introduction',
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
        'profile2_private' => FALSE
      ),
      'translatable' => '0',
      'type' => 'text_long'
    ),
    'field_instance' => array(
      'bundle' => 'pensoft',
      'default_value' => NULL,
      'deleted' => '0',
      'description' => '',
      'display' => array(
        'default' => array(
          'label' => 'above',
          'module' => 'text',
          'settings' => array(),
          'type' => 'text_default',
          'weight' => '3'
        )
      ),
      'entity_type' => 'publication',
      'field_name' => 'field_publication_introduction',
      'label' => 'Introduction',
      'required' => 0,
      'settings' => array(
        'linkit' => array(
          'enable' => 0,
          'insert_plugin' => ''
        ),
        'text_processing' => '1',
        'user_register_form' => FALSE
      ),
      'widget' => array(
        'active' => 1,
        'module' => 'text',
        'settings' => array(
          'rows' => '5'
        ),
        'type' => 'text_textarea',
        'weight' => '29'
      )
    )
  );
  // Exported field: 'publication-pensoft-field_publication_keywords'
  $fields['publication-pensoft-field_publication_keywords'] = array(
    'field_config' => array(
      'active' => '1',
      'cardinality' => '1',
      'deleted' => '0',
      'entity_types' => array(),
      'field_name' => 'field_publication_keywords',
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
        'profile2_private' => FALSE
      ),
      'translatable' => '0',
      'type' => 'text_long'
    ),
    'field_instance' => array(
      'bundle' => 'pensoft',
      'default_value' => NULL,
      'deleted' => '0',
      'description' => 'Please enter a list of publication keywords, comma separated.',
      'display' => array(
        'default' => array(
          'label' => 'above',
          'module' => 'text',
          'settings' => array(),
          'type' => 'text_default',
          'weight' => '4'
        )
      ),
      'entity_type' => 'publication',
      'field_name' => 'field_publication_keywords',
      'label' => 'Keywords',
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
        'weight' => '19'
      )
    )
  );
  // Exported field: 'publication-pensoft-field_publication_methods'
  $fields['publication-pensoft-field_publication_methods'] = array(
    'field_config' => array(
      'active' => '1',
      'cardinality' => '1',
      'deleted' => '0',
      'entity_types' => array(),
      'field_name' => 'field_publication_methods',
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
      'bundle' => 'pensoft',
      'default_value' => NULL,
      'deleted' => '0',
      'description' => '',
      'display' => array(
        'default' => array(
          'label' => 'above',
          'module' => 'text',
          'settings' => array(),
          'type' => 'text_default',
          'weight' => '12'
        )
      ),
      'entity_type' => 'publication',
      'field_name' => 'field_publication_methods',
      'label' => 'Material & Methods',
      'required' => 0,
      'settings' => array(
        'linkit' => array(
          'enable' => 0,
          'insert_plugin' => ''
        ),
        'text_processing' => '1',
        'user_register_form' => FALSE
      ),
      'widget' => array(
        'active' => 1,
        'module' => 'text',
        'settings' => array(
          'rows' => '5'
        ),
        'type' => 'text_textarea',
        'weight' => '9'
      )
    )
  );
  // Exported field: 'publication-pensoft-field_publication_references'
  $fields['publication-pensoft-field_publication_references'] = array(
    'field_config' => array(
      'active' => '1',
      'cardinality' => '-1',
      'deleted' => '0',
      'entity_types' => array(),
      'field_name' => 'field_publication_references',
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
      'translatable' => '0',
      'type' => 'node_reference'
    ),
    'field_instance' => array(
      'bundle' => 'pensoft',
      'default_value' => NULL,
      'deleted' => '0',
      'description' => '',
      'display' => array(
        'default' => array(
          'label' => 'above',
          'module' => 'scratchpads_biblio',
          'settings' => array(),
          'type' => 'scratchpads_biblio_biblio_ref',
          'weight' => '20'
        )
      ),
      'entity_type' => 'publication',
      'entityconnect_unload_add' => '1',
      'entityconnect_unload_edit' => '1',
      'field_name' => 'field_publication_references',
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
          'size' => '100'
        ),
        'type' => 'node_reference_autocomplete',
        'weight' => '23'
      )
    )
  );
  // Exported field: 'publication-pensoft-field_publication_tables'
  $fields['publication-pensoft-field_publication_tables'] = array(
    'field_config' => array(
      'active' => '1',
      'cardinality' => '-1',
      'deleted' => '0',
      'entity_types' => array(),
      'field_name' => 'field_publication_tables',
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
      'bundle' => 'pensoft',
      'default_value' => NULL,
      'deleted' => '0',
      'description' => '',
      'display' => array(
        'default' => array(
          'label' => 'above',
          'module' => 'field_collection',
          'settings' => array(
            'add' => '',
            'delete' => '',
            'description' => 0,
            'edit' => '',
            'view_mode' => 'full'
          ),
          'type' => 'field_collection_view',
          'weight' => '18'
        )
      ),
      'entity_type' => 'publication',
      'field_name' => 'field_publication_tables',
      'label' => 'Tables',
      'required' => 0,
      'settings' => array(
        'user_register_form' => FALSE
      ),
      'widget' => array(
        'active' => 0,
        'module' => 'field_collection',
        'settings' => array(),
        'type' => 'field_collection_embed',
        'weight' => '33'
      )
    )
  );
  // Exported field: 'publication-pensoft-field_publication_title'
  $fields['publication-pensoft-field_publication_title'] = array(
    'field_config' => array(
      'active' => '1',
      'cardinality' => '1',
      'deleted' => '0',
      'entity_types' => array(),
      'field_name' => 'field_publication_title',
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
        'profile2_private' => FALSE
      ),
      'translatable' => '0',
      'type' => 'text_long'
    ),
    'field_instance' => array(
      'bundle' => 'pensoft',
      'default_value' => NULL,
      'deleted' => '0',
      'description' => '',
      'display' => array(
        'default' => array(
          'label' => 'hidden',
          'module' => 'text',
          'settings' => array(),
          'type' => 'text_default',
          'weight' => '0'
        )
      ),
      'entity_type' => 'publication',
      'field_name' => 'field_publication_title',
      'label' => 'Publication title',
      'required' => 0,
      'settings' => array(
        'text_processing' => '1',
        'user_register_form' => FALSE
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
  // Exported field: 'publication-pensoft-field_publication_treatments'
  $fields['publication-pensoft-field_publication_treatments'] = array(
    'field_config' => array(
      'active' => '1',
      'cardinality' => '-1',
      'deleted' => '0',
      'entity_types' => array(),
      'field_name' => 'field_publication_treatments',
      'foreign keys' => array(),
      'indexes' => array(),
      'module' => 'relation_select',
      'settings' => array(),
      'translatable' => '0',
      'type' => 'relation_select'
    ),
    'field_instance' => array(
      'bundle' => 'pensoft',
      'default_value' => array(
        0 => array(
          'relation_fields' => array(
            'field_publication_treat_fields' => array(
              'und' => array(
                0 => array(
                  'value' => ''
                )
              )
            ),
            'field_publication_treatment_name' => array(
              'und' => array(
                0 => array(
                  'value' => ''
                )
              )
            )
          ),
          'exposed_filters' => array(
            'element_id' => 'edit-field-publication-treatments-und-0',
            'form_build_id' => 'form-vCyfhrwmrKK3LRKcqIpjjfvOAjnbYpbpxDno5KgiAS8',
            'form_id' => 'relation_select_views_exposed_form',
            'form_token' => 'UkyUBr-IkxFInqL3Zr0lxrLiBeM-z4kp20Tz-bRV0xg',
            'title' => '',
            'view_display_id' => 'default',
            'view_name' => 'relation_select_publication_taxo'
          )
        ),
        1 => array(
          'exposed_filters' => array(
            'element_id' => 'edit-field-publication-treatments-und-1',
            'form_build_id' => 'form-Hdvxq_LVYH_ZPzhuZgqHgJJLNCSbfITWW87PYmAJa_k',
            'form_id' => 'relation_select_views_exposed_form',
            'form_token' => 'UkyUBr-IkxFInqL3Zr0lxrLiBeM-z4kp20Tz-bRV0xg',
            'title' => '',
            'view_display_id' => 'default',
            'view_name' => 'relation_select_publication_taxo'
          ),
          'relation_fields' => array(
            'field_publication_treat_fields' => array(
              'und' => array(
                0 => array(
                  'value' => ''
                )
              )
            ),
            'field_publication_treatment_name' => array(
              'und' => array(
                0 => array(
                  'value' => ''
                )
              )
            )
          )
        )
      ),
      'deleted' => '0',
      'description' => '',
      'display' => array(
        'default' => array(
          'label' => 'above',
          'module' => 'relation_select',
          'settings' => array(),
          'type' => 'relation_select',
          'weight' => '14'
        )
      ),
      'entity_type' => 'publication',
      'field_name' => 'field_publication_treatments',
      'label' => 'Taxon treatments',
      'required' => 0,
      'settings' => array(
        'relation_type' => 'publication_treatments',
        'relation_view' => 'relation_select_publication_taxo:default',
        'user_register_form' => FALSE
      ),
      'widget' => array(
        'active' => 0,
        'module' => 'relation_select',
        'settings' => array(),
        'type' => 'relation_select',
        'weight' => '22'
      )
    )
  );
  // Exported field: 'relation-publication_treatments-endpoints'
  $fields['relation-publication_treatments-endpoints'] = array(
    'field_config' => array(
      'active' => '1',
      'cardinality' => '-1',
      'deleted' => '0',
      'entity_types' => array(),
      'field_name' => 'endpoints',
      'foreign keys' => array(),
      'indexes' => array(
        'relation' => array(
          0 => 'entity_type',
          1 => 'entity_id',
          2 => 'r_index'
        )
      ),
      'module' => 'relation_endpoint',
      'settings' => array(),
      'translatable' => '0',
      'type' => 'relation_endpoint'
    ),
    'field_instance' => array(
      'bundle' => 'publication_treatments',
      'default_value' => NULL,
      'deleted' => '0',
      'description' => '',
      'display' => array(
        'default' => array(
          'label' => 'above',
          'settings' => array(),
          'type' => 'hidden',
          'weight' => '1'
        )
      ),
      'entity_type' => 'relation',
      'field_name' => 'endpoints',
      'label' => 'endpoints',
      'required' => FALSE,
      'settings' => array(
        'user_register_form' => FALSE
      ),
      'widget' => array(
        'module' => 'relation_endpoint',
        'settings' => array(),
        'type' => 'relation_endpoint',
        'weight' => '2'
      )
    )
  );
  // Exported field: 'relation-publication_treatments-field_publication_treat_fields'
  $fields['relation-publication_treatments-field_publication_treat_fields'] = array(
    'field_config' => array(
      'active' => '1',
      'cardinality' => '1',
      'deleted' => '0',
      'entity_types' => array(),
      'field_name' => 'field_publication_treat_fields',
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
      'bundle' => 'publication_treatments',
      'default_value' => NULL,
      'deleted' => '0',
      'description' => '',
      'display' => array(
        'default' => array(
          'label' => 'above',
          'settings' => array(),
          'type' => 'hidden',
          'weight' => '5'
        )
      ),
      'entity_type' => 'relation',
      'field_name' => 'field_publication_treat_fields',
      'label' => 'Included fields',
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
        'weight' => '6'
      )
    )
  );
  // Exported field: 'relation-publication_treatments-field_publication_treat_holotype'
  $fields['relation-publication_treatments-field_publication_treat_holotype'] = array(
    'field_config' => array(
      'active' => '1',
      'cardinality' => '1',
      'deleted' => '0',
      'entity_types' => array(),
      'field_name' => 'field_publication_treat_holotype',
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
          'location' => 0,
          'media_gallery' => 0,
          'page' => 0,
          'specimen_observation' => 'specimen_observation',
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
      'bundle' => 'publication_treatments',
      'default_value' => NULL,
      'deleted' => '0',
      'description' => '',
      'display' => array(
        'default' => array(
          'label' => 'above',
          'module' => 'node_reference',
          'settings' => array(),
          'type' => 'node_reference_default',
          'weight' => '2'
        )
      ),
      'entity_type' => 'relation',
      'entityconnect_unload_add' => '1',
      'entityconnect_unload_edit' => '1',
      'field_name' => 'field_publication_treat_holotype',
      'label' => 'Holotype',
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
        'weight' => '4'
      )
    )
  );
  // Exported field: 'relation-publication_treatments-field_publication_treat_paratype'
  $fields['relation-publication_treatments-field_publication_treat_paratype'] = array(
    'field_config' => array(
      'active' => '1',
      'cardinality' => '-1',
      'deleted' => '0',
      'entity_types' => array(),
      'field_name' => 'field_publication_treat_paratype',
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
          'location' => 0,
          'media_gallery' => 0,
          'page' => 0,
          'specimen_observation' => 'specimen_observation',
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
      'bundle' => 'publication_treatments',
      'default_value' => NULL,
      'deleted' => '0',
      'description' => '',
      'display' => array(
        'default' => array(
          'label' => 'above',
          'module' => 'node_reference',
          'settings' => array(),
          'type' => 'node_reference_default',
          'weight' => '4'
        )
      ),
      'entity_type' => 'relation',
      'entityconnect_unload_add' => '1',
      'entityconnect_unload_edit' => '1',
      'field_name' => 'field_publication_treat_paratype',
      'label' => 'Paratype(s)',
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
  // Exported field: 'relation-publication_treatments-field_publication_treat_type'
  $fields['relation-publication_treatments-field_publication_treat_type'] = array(
    'field_config' => array(
      'active' => '1',
      'cardinality' => '1',
      'deleted' => '0',
      'entity_types' => array(),
      'field_name' => 'field_publication_treat_type',
      'foreign keys' => array(),
      'indexes' => array(
        'value' => array(
          0 => 'value'
        )
      ),
      'module' => 'list',
      'settings' => array(
        'allowed_values' => array(
          'New taxon' => 'New taxon',
          'Redescription or species observation' => 'Redescription or species observation'
        ),
        'allowed_values_function' => ''
      ),
      'translatable' => '0',
      'type' => 'list_text'
    ),
    'field_instance' => array(
      'bundle' => 'publication_treatments',
      'default_value' => NULL,
      'deleted' => '0',
      'description' => '',
      'display' => array(
        'default' => array(
          'label' => 'above',
          'module' => 'list',
          'settings' => array(),
          'type' => 'list_default',
          'weight' => '0'
        )
      ),
      'entity_type' => 'relation',
      'field_name' => 'field_publication_treat_type',
      'label' => 'Type of treatment',
      'required' => 0,
      'settings' => array(
        'user_register_form' => FALSE
      ),
      'widget' => array(
        'active' => 1,
        'module' => 'options',
        'settings' => array(),
        'type' => 'options_select',
        'weight' => '3'
      )
    )
  );
  // Exported field: 'relation-publication_treatments-field_publication_treat_genus'
  $fields['relation-publication_treatments-field_publication_treat_genus'] = array(
    'field_config' => array(
      'active' => '1',
      'cardinality' => '1',
      'deleted' => '0',
      'entity_types' => array(),
      'field_name' => 'field_publication_treat_genus',
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
      'bundle' => 'publication_treatments',
      'default_value' => NULL,
      'deleted' => '0',
      'description' => 'The Genus for this treatment',
      'display' => array(
        'default' => array(
          'label' => 'hidden',
          'settings' => array(),
          'type' => 'hidden',
          'weight' => '6'
        )
      ),
      'entity_type' => 'relation',
      'field_name' => 'field_publication_treat_genus',
      'label' => 'Genus',
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
        'weight' => '0'
      )
    )
  );
  // Exported field: 'relation-publication_treatments-field_publication_treat_species'
  $fields['relation-publication_treatments-field_publication_treat_species'] = array(
    'field_config' => array(
      'active' => '1',
      'cardinality' => '1',
      'deleted' => '0',
      'entity_types' => array(),
      'field_name' => 'field_publication_treat_species',
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
      'bundle' => 'publication_treatments',
      'default_value' => NULL,
      'deleted' => '0',
      'description' => 'The species for this treatment',
      'display' => array(
        'default' => array(
          'label' => 'hidden',
          'settings' => array(),
          'type' => 'hidden',
          'weight' => '6'
        )
      ),
      'entity_type' => 'relation',
      'field_name' => 'field_publication_treat_species',
      'label' => 'Species',
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
        'weight' => '1'
      )
    )
  );
  // Exported field: 'relation-publication_treatments-field_publication_treat_authy'
  $fields['relation-publication_treatments-field_publication_treat_authy'] = array(
    'field_config' => array(
      'active' => '1',
      'cardinality' => '1',
      'deleted' => '0',
      'entity_types' => array(),
      'field_name' => 'field_publication_treat_authy',
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
      'bundle' => 'publication_treatments',
      'default_value' => NULL,
      'deleted' => '0',
      'description' => 'The treatment author/year',
      'display' => array(
        'default' => array(
          'label' => 'hidden',
          'settings' => array(),
          'type' => 'hidden',
          'weight' => '6'
        )
      ),
      'entity_type' => 'relation',
      'field_name' => 'field_publication_treat_authy',
      'label' => 'Author / Year',
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
  // Translatables
  // Included for use with string extractors like potx.
  t('Abstract');
  t('Acknowledgments');
  t('Author');
  t('Author rights');
  t('Authors');
  t('Contributor');
  t('Contributor rights');
  t('Contributor role');
  t('Contributors');
  t('Data resources');
  t('Description');
  t('Discussion');
  t('Figure');
  t('Figures');
  t('Genus');
  t('Species');
  t('Author / Year');
  t('Holotype');
  t('If this taxon should have a different name in the final publication, enter it here.');
  t('Included fields');
  t('Introduction');
  t('Keywords');
  t('Material & Methods');
  t('Paratype(s)');
  t('Please enter a list of publication keywords, comma separated.');
  t('Publication title');
  t('Reference');
  t('Results');
  t('Submitting author?');
  t('Supplementary files');
  t('Systematics');
  t('Table');
  t('Table id');
  t('Tables');
  t('Taxon treatments');
  t('This is the table id, and it is used to track references to table. Do not change after a table has been created, otherwise references to the table will not work.
Tables are numbered once the publication is saved.');
  t('Title');
  t('Type of treatment');
  t('Endpoints');
  return $fields;
}
