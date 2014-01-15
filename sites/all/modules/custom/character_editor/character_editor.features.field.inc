<?php

/**
 * @file
 * ce.features.field.inc
 */
/**
 * Implementation of hook_field_default_fields().
 */
function character_editor_field_default_fields(){
  $fields = array();
  // Exported field: 'character_editor_character-controlled-field_char_description'
  $fields['character_editor_character-controlled-field_char_description'] = array(
    'field_config' => array(
      'active' => '1',
      'cardinality' => '1',
      'deleted' => '0',
      'entity_types' => array(),
      'field_name' => 'field_char_description',
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
      'bundle' => 'controlled',
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
        )
      ),
      'entity_type' => 'character_editor_character',
      'field_name' => 'field_char_description',
      'label' => 'Description',
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
        'weight' => '0'
      )
    )
  );
  // Exported field: 'character_editor_character-controlled-field_char_image'
  $fields['character_editor_character-controlled-field_char_image'] = array(
    'field_config' => array(
      'active' => '1',
      'cardinality' => '1',
      'deleted' => '0',
      'entity_types' => array(),
      'field_name' => 'field_char_image',
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
      'bundle' => 'controlled',
      'deleted' => '0',
      'description' => '',
      'display' => array(
        'default' => array(
          'label' => 'hidden',
          'module' => 'image',
          'settings' => array(
            'image_link' => '',
            'image_style' => 'thumbnail'
          ),
          'type' => 'image',
          'weight' => '0'
        )
      ),
      'entity_type' => 'character_editor_character',
      'field_name' => 'field_char_image',
      'label' => 'Image',
      'required' => 0,
      'settings' => array(
        'alt_field' => 0,
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
          'allowed_schemes' => array(
            0 => 'public',
            1 => 'private'
          ),
          'allowed_types' => array(
            0 => 'image'
          ),
          'browser_plugins' => array(),
          'preview_image_style' => 'thumbnail',
          'progress_indicator' => 'throbber'
        ),
        'type' => 'media_generic',
        'weight' => '1'
      )
    )
  );
  // Exported field: 'character_editor_character-controlled-field_char_states'
  $fields['character_editor_character-controlled-field_char_states'] = array(
    'field_config' => array(
      'active' => '1',
      'cardinality' => '-1',
      'deleted' => '0',
      'entity_types' => array(),
      'field_name' => 'field_char_states',
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
      'bundle' => 'controlled',
      'default_value' => NULL,
      'deleted' => '0',
      'description' => '',
      'display' => array(
        'default' => array(
          'label' => 'hidden',
          'module' => 'field_collection',
          'settings' => array(
            'add' => '',
            'delete' => '',
            'description' => 0,
            'edit' => '',
            'view_mode' => 'full'
          ),
          'type' => 'field_collection_view',
          'weight' => '4'
        )
      ),
      'entity_type' => 'character_editor_character',
      'field_name' => 'field_char_states',
      'label' => 'States',
      'required' => 0,
      'settings' => array(
        'user_register_form' => FALSE
      ),
      'widget' => array(
        'active' => 0,
        'module' => 'field_collection',
        'settings' => array(),
        'type' => 'field_collection_embed',
        'weight' => '3'
      )
    )
  );
  // Exported field: 'character_editor_character-dna-field_char_description'
  $fields['character_editor_character-dna-field_char_description'] = array(
    'field_config' => array(
      'active' => '1',
      'cardinality' => '1',
      'deleted' => '0',
      'entity_types' => array(),
      'field_name' => 'field_char_description',
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
      'bundle' => 'dna',
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
      'entity_type' => 'character_editor_character',
      'field_name' => 'field_char_description',
      'label' => 'Description',
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
        'weight' => '2'
      )
    )
  );
  // Exported field: 'character_editor_character-dna-field_char_image'
  $fields['character_editor_character-dna-field_char_image'] = array(
    'field_config' => array(
      'active' => '1',
      'cardinality' => '1',
      'deleted' => '0',
      'entity_types' => array(),
      'field_name' => 'field_char_image',
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
      'bundle' => 'dna',
      'deleted' => '0',
      'description' => '',
      'display' => array(
        'default' => array(
          'label' => 'above',
          'module' => 'image',
          'settings' => array(
            'image_link' => '',
            'image_style' => 'thumbnail'
          ),
          'type' => 'image',
          'weight' => '1'
        )
      ),
      'entity_type' => 'character_editor_character',
      'field_name' => 'field_char_image',
      'label' => 'Image',
      'required' => 0,
      'settings' => array(
        'alt_field' => 0,
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
          'allowed_schemes' => array(
            0 => 'public',
            1 => 'private'
          ),
          'allowed_types' => array(
            0 => 'image'
          ),
          'browser_plugins' => array(),
          'preview_image_style' => 'thumbnail',
          'progress_indicator' => 'throbber'
        ),
        'type' => 'media_generic',
        'weight' => '3'
      )
    )
  );
  // Exported field: 'character_editor_character-group-field_char_description'
  $fields['character_editor_character-group-field_char_description'] = array(
    'field_config' => array(
      'active' => '1',
      'cardinality' => '1',
      'deleted' => '0',
      'entity_types' => array(),
      'field_name' => 'field_char_description',
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
      'bundle' => 'group',
      'default_value' => NULL,
      'deleted' => '0',
      'description' => '',
      'display' => array(
        'default' => array(
          'label' => 'above',
          'module' => 'text',
          'settings' => array(),
          'type' => 'text_default',
          'weight' => '2'
        )
      ),
      'entity_type' => 'character_editor_character',
      'field_name' => 'field_char_description',
      'label' => 'Description',
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
  );
  // Exported field: 'character_editor_character-group-field_char_image'
  $fields['character_editor_character-group-field_char_image'] = array(
    'field_config' => array(
      'active' => '1',
      'cardinality' => '1',
      'deleted' => '0',
      'entity_types' => array(),
      'field_name' => 'field_char_image',
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
      'bundle' => 'group',
      'deleted' => '0',
      'description' => '',
      'display' => array(
        'default' => array(
          'label' => 'above',
          'module' => 'image',
          'settings' => array(
            'image_link' => '',
            'image_style' => 'thumbnail'
          ),
          'type' => 'image',
          'weight' => '1'
        )
      ),
      'entity_type' => 'character_editor_character',
      'field_name' => 'field_char_image',
      'label' => 'Image',
      'required' => 0,
      'settings' => array(
        'alt_field' => 0,
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
          'allowed_schemes' => array(
            0 => 'public',
            1 => 'private'
          ),
          'allowed_types' => array(
            0 => 'image'
          ),
          'browser_plugins' => array(),
          'preview_image_style' => 'thumbnail',
          'progress_indicator' => 'throbber'
        ),
        'type' => 'media_generic',
        'weight' => '3'
      )
    )
  );
  // Exported field: 'character_editor_character-numeric-field_char_description'
  $fields['character_editor_character-numeric-field_char_description'] = array(
    'field_config' => array(
      'active' => '1',
      'cardinality' => '1',
      'deleted' => '0',
      'entity_types' => array(),
      'field_name' => 'field_char_description',
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
      'bundle' => 'numeric',
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
      'entity_type' => 'character_editor_character',
      'field_name' => 'field_char_description',
      'label' => 'Description',
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
        'weight' => '2'
      )
    )
  );
  // Exported field: 'character_editor_character-numeric-field_char_image'
  $fields['character_editor_character-numeric-field_char_image'] = array(
    'field_config' => array(
      'active' => '1',
      'cardinality' => '1',
      'deleted' => '0',
      'entity_types' => array(),
      'field_name' => 'field_char_image',
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
      'bundle' => 'numeric',
      'deleted' => '0',
      'description' => '',
      'display' => array(
        'default' => array(
          'label' => 'hidden',
          'module' => 'image',
          'settings' => array(
            'image_link' => '',
            'image_style' => 'thumbnail'
          ),
          'type' => 'image',
          'weight' => '1'
        )
      ),
      'entity_type' => 'character_editor_character',
      'field_name' => 'field_char_image',
      'label' => 'Image',
      'required' => 0,
      'settings' => array(
        'alt_field' => 0,
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
          'allowed_schemes' => array(
            0 => 'public',
            1 => 'private'
          ),
          'allowed_types' => array(
            0 => 'image'
          ),
          'browser_plugins' => array(),
          'preview_image_style' => 'thumbnail',
          'progress_indicator' => 'throbber'
        ),
        'type' => 'media_generic',
        'weight' => '3'
      )
    )
  );
  // Exported field: 'character_editor_character-numeric-field_char_unit'
  $fields['character_editor_character-numeric-field_char_unit'] = array(
    'field_config' => array(
      'active' => '1',
      'cardinality' => '1',
      'deleted' => '0',
      'entity_types' => array(),
      'field_name' => 'field_char_unit',
      'foreign keys' => array(),
      'indexes' => array(
        'value' => array(
          0 => 'value'
        )
      ),
      'module' => 'list',
      'settings' => array(
        'allowed_values' => array(
          'cm' => 'Centimetre',
          'gr' => 'Gram',
          'kg' => 'Kilogram',
          'm' => 'Metre',
          'mm' => 'Millimetre',
          'none' => 'None',
          'other' => 'Other'
        ),
        'allowed_values_function' => ''
      ),
      'translatable' => '0',
      'type' => 'list_text'
    ),
    'field_instance' => array(
      'bundle' => 'numeric',
      'default_value' => NULL,
      'deleted' => '0',
      'description' => 'Please select the unit of measurement.',
      'display' => array(
        'default' => array(
          'label' => 'hidden',
          'module' => 'list',
          'settings' => array(),
          'type' => 'list_default',
          'weight' => '3'
        )
      ),
      'entity_type' => 'character_editor_character',
      'field_name' => 'field_char_unit',
      'label' => 'Unit',
      'required' => 0,
      'settings' => array(
        'user_register_form' => FALSE
      ),
      'widget' => array(
        'active' => 1,
        'module' => 'cck_select_other',
        'settings' => array(
          'select_list_options' => 'none|None
mm|Millimetre
cm|Centimetre
m|Metre
gr|Gram
kg|Kilogram
other|Other',
          'select_list_options_fieldset' => array(
            'advanced_options' => array(
              'select_list_options_php' => ''
            )
          )
        ),
        'type' => 'cck_select_other',
        'weight' => '7'
      )
    )
  );
  // Exported field: 'character_editor_character-text-field_char_description'
  $fields['character_editor_character-text-field_char_description'] = array(
    'field_config' => array(
      'active' => '1',
      'cardinality' => '1',
      'deleted' => '0',
      'entity_types' => array(),
      'field_name' => 'field_char_description',
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
      'bundle' => 'text',
      'default_value' => NULL,
      'deleted' => '0',
      'description' => '',
      'display' => array(
        'default' => array(
          'label' => 'hidden',
          'module' => 'text',
          'settings' => array(),
          'type' => 'text_plain',
          'weight' => '0'
        )
      ),
      'entity_type' => 'character_editor_character',
      'field_name' => 'field_char_description',
      'label' => 'Description',
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
  );
  // Exported field: 'character_editor_character-text-field_char_image'
  $fields['character_editor_character-text-field_char_image'] = array(
    'field_config' => array(
      'active' => '1',
      'cardinality' => '1',
      'deleted' => '0',
      'entity_types' => array(),
      'field_name' => 'field_char_image',
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
      'bundle' => 'text',
      'deleted' => '0',
      'description' => '',
      'display' => array(
        'default' => array(
          'label' => 'hidden',
          'module' => 'image',
          'settings' => array(
            'image_link' => '',
            'image_style' => 'thumbnail'
          ),
          'type' => 'image',
          'weight' => '1'
        )
      ),
      'entity_type' => 'character_editor_character',
      'field_name' => 'field_char_image',
      'label' => 'Image',
      'required' => 0,
      'settings' => array(
        'alt_field' => 0,
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
          'allowed_schemes' => array(
            0 => 'public',
            1 => 'private'
          ),
          'allowed_types' => array(
            0 => 'image'
          ),
          'browser_plugins' => array(),
          'preview_image_style' => 'thumbnail',
          'progress_indicator' => 'throbber'
        ),
        'type' => 'media_generic',
        'weight' => '3'
      )
    )
  );
  // Exported field: 'character_editor_project-default_character_project-field_char_proj_description'
  $fields['character_editor_project-default_character_project-field_char_proj_description'] = array(
    'field_config' => array(
      'active' => '1',
      'cardinality' => '1',
      'deleted' => '0',
      'entity_types' => array(),
      'field_name' => 'field_char_proj_description',
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
      'bundle' => 'default_character_project',
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
      'entity_type' => 'character_editor_project',
      'field_name' => 'field_char_proj_description',
      'label' => 'Description',
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
  );
  // Exported field: 'character_editor_project-default_character_project-field_char_proj_flags'
  $fields['character_editor_project-default_character_project-field_char_proj_flags'] = array(
    'field_config' => array(
      'active' => '1',
      'cardinality' => '-1',
      'deleted' => '0',
      'entity_types' => array(),
      'field_name' => 'field_char_proj_flags',
      'foreign keys' => array(),
      'indexes' => array(),
      'module' => 'field_collection',
      'settings' => array(
        'hide_blank_items' => 0,
        'path' => ''
      ),
      'translatable' => '0',
      'type' => 'field_collection'
    ),
    'field_instance' => array(
      'bundle' => 'default_character_project',
      'default_value' => array(),
      'deleted' => '0',
      'description' => 'List of flags (and abbreviation) that can be used to mark values, such as "rare" or "guessed". "computed" and "inherited" are reserved for computed and inherited values, but you can change the abbreviation here.',
      'display' => array(
        'default' => array(
          'label' => 'hidden',
          'settings' => array(),
          'type' => 'hidden',
          'weight' => '1002'
        )
      ),
      'entity_type' => 'character_editor_project',
      'field_name' => 'field_char_proj_flags',
      'label' => 'Flags',
      'required' => 0,
      'settings' => array(
        'user_register_form' => FALSE
      ),
      'widget' => array(
        'active' => 0,
        'module' => 'field_collection_table',
        'settings' => array(),
        'type' => 'field_collection_table',
        'weight' => '2'
      )
    )
  );
  $flag_defaults = array(
    'c' => 'computed',
    'i' => 'inherited',
    'a' => 'approximate',
    'g' => 'guessed',
    'r' => 'rare'
  );
  foreach($flags_defaults as $flag_abbr => $flag){
    $fields['character_editor_project-default_character_project-field_char_proj_flags']['field_instance']['default_value'][] = array(
      'field_char_proj_flags_abbr' => array(
        'und' => array(
          0 => array(
            'value' => $flag_abbr
          )
        )
      ),
      'field_char_proj_flags_flag' => array(
        'und' => array(
          0 => array(
            'value' => $flag
          )
        )
      ),
      'field_char_proj_flags_id' => array(
        'und' => array(
          0 => array(
            'value' => $flag
          )
        )
      ),
      'remove_button' => 'Remove',
      'entity' => entity_import('field_collection_item', '{
            "item_id" : null,
            "revision_id" : null,
            "field_name" : "field_char_proj_flags",
            "default_revision" : true,
            "archived" : false,
            "field_char_proj_flags_abbr" : { "und" : [ { "value" : "' . $flag_abbr . '" } ] },
            "field_char_proj_flags_flag" : { "und" : [ { "value" : "' . $flag . '" } ] },
            "field_char_proj_flags_id" : { "und" : [ { "value" : "' . $flag . '" } ] }
          }')
    );
  }
  // Exported field: 'character_editor_project-default_character_project-field_taxonomic_name'
  $fields['character_editor_project-default_character_project-field_taxonomic_name'] = array(
    'field_config' => field_info_field('field_taxonomic_name'),
    'field_instance' => array(
      'bundle' => 'default_character_project',
      'default_value' => NULL,
      'deleted' => '0',
      'description' => 'The taxa included in this character project.',
      'display' => array(
        'default' => array(
          'label' => 'above',
          'module' => 'taxonomy_formatter',
          'settings' => array(
            'element_class' => '',
            'element_option' => '- None -',
            'links_option' => 1,
            'separator_option' => ', ',
            'wrapper_class' => '',
            'wrapper_option' => '- None -'
          ),
          'type' => 'taxonomy_term_reference_csv',
          'weight' => '1000'
        )
      ),
      'entity_type' => 'character_editor_project',
      'field_name' => 'field_taxonomic_name',
      'label' => 'Taxonomic name',
      'required' => 1,
      'settings' => array(
        'user_register_form' => FALSE
      ),
      'widget' => array(
        'active' => 1,
        'module' => 'tree_widget',
        'settings' => array(),
        'type' => 'tree_widget',
        'weight' => '-3'
      )
    )
  );
  $bio_vids = variable_get('biological_vids', array());
  $i = 0;
  foreach($bio_vids as $vid => $value){
    $voc = taxonomy_vocabulary_load($vid);
    if($voc){
      $field['character_editor_project-default_character_project-field_taxonomic_name']['field_config']['settings']['allowed_values'][$i] = array(
        'parent' => 0,
        'vocabulary' => $voc->machine_name
      );
      $i++;
    }
  }
  // Exported field: 'character_editor_project-default_character_project-field_char_proj_specimen'
  $fields['character_editor_project-default_character_project-field_char_proj_specimen'] = array(
    'field_config' => array(
      'locked' => '1',
      'active' => '1',
      'cardinality' => '-1',
      'deleted' => '0',
      'entity_types' => array(),
      'field_name' => 'field_char_proj_specimen',
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
          'specimen_observation' => 'specimen_observation'
        )
      ),
      'translatable' => '1',
      'type' => 'node_reference'
    ),
    'field_instance' => array(
      'bundle' => 'default_character_project',
      'default_value' => NULL,
      'deleted' => '0',
      'description' => '',
      'display' => array(
        'default' => array(
          'label' => 'above',
          'module' => 'node_reference',
          'settings' => array(),
          'type' => 'node_reference_default',
          'weight' => 1001
        )
      ),
      'entity_type' => 'character_editor_project',
      'entityconnect_unload_add' => '1',
      'entityconnect_unload_edit' => '1',
      'field_name' => 'field_char_proj_specimen',
      'label' => 'Specimen',
      'required' => 0,
      'settings' => array(
        'user_register_form' => FALSE
      ),
      'widget' => array(
        'active' => 1,
        'module' => 'insert_from_view',
        'settings' => array(
          'insert-from-view' => array(
            'insert-from-view-view' => 'specimen_node_ref_selection',
            'insert-from-view-display' => 'default',
            'insert-from-view-field' => 'nothing',
            'insert-from-view-width' => '75%',
            'insert-from-view-height' => '75%'
          )
        ),
        'type' => 'insert_from_view_node_reference',
        'weight' => '-2'
      )
    )
  );
  // Exported field: 'field_collection_item-field_char_states-field_char_state_image'
  $fields['field_collection_item-field_char_states-field_char_state_image'] = array(
    'field_config' => array(
      'active' => '1',
      'cardinality' => '1',
      'deleted' => '0',
      'entity_types' => array(),
      'field_name' => 'field_char_state_image',
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
      'bundle' => 'field_char_states',
      'deleted' => '0',
      'description' => '',
      'display' => array(
        'default' => array(
          'label' => 'above',
          'module' => 'image',
          'settings' => array(
            'image_link' => '',
            'image_style' => 'thumbnail'
          ),
          'type' => 'image',
          'weight' => 1
        )
      ),
      'entity_type' => 'field_collection_item',
      'field_name' => 'field_char_state_image',
      'label' => 'Image',
      'required' => 0,
      'settings' => array(
        'alt_field' => 0,
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
          'allowed_schemes' => array(
            0 => 'public',
            1 => 'private'
          ),
          'allowed_types' => array(
            0 => 'image'
          ),
          'browser_plugins' => array(),
          'preview_image_style' => 'thumbnail',
          'progress_indicator' => 'throbber'
        ),
        'type' => 'media_generic',
        'weight' => '3'
      )
    )
  );
  // Exported field: 'field_collection_item-field_char_states-field_char_state_label'
  $fields['field_collection_item-field_char_states-field_char_state_label'] = array(
    'field_config' => array(
      'active' => '1',
      'cardinality' => '1',
      'deleted' => '0',
      'entity_types' => array(),
      'field_name' => 'field_char_state_label',
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
      'bundle' => 'field_char_states',
      'default_value' => NULL,
      'deleted' => '0',
      'description' => '',
      'display' => array(
        'default' => array(
          'label' => 'above',
          'module' => 'text',
          'settings' => array(),
          'type' => 'text_default',
          'weight' => 0
        )
      ),
      'entity_type' => 'field_collection_item',
      'field_name' => 'field_char_state_label',
      'label' => 'State',
      'required' => 1,
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
  // Exported field: 'relation-character-endpoints'
  $fields['relation-character-endpoints'] = array(
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
      'bundle' => 'character',
      'default_value' => NULL,
      'deleted' => '0',
      'description' => '',
      'display' => array(
        'default' => array(
          'label' => 'above',
          'module' => 'relation_endpoint',
          'settings' => array(),
          'type' => 'relation_endpoint',
          'weight' => 0
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
        'weight' => 0
      )
    )
  );
  // Exported field: 'relation-character-field_character_weight'
  $fields['relation-character-field_character_weight'] = array(
    'field_config' => array(
      'active' => '1',
      'cardinality' => '1',
      'deleted' => '0',
      'entity_types' => array(),
      'field_name' => 'field_character_weight',
      'foreign keys' => array(),
      'indexes' => array(),
      'module' => 'number',
      'settings' => array(),
      'translatable' => '0',
      'type' => 'number_integer'
    ),
    'field_instance' => array(
      'bundle' => 'character',
      'default_value' => NULL,
      'deleted' => '0',
      'description' => '',
      'display' => array(
        'default' => array(
          'label' => 'above',
          'module' => 'number',
          'settings' => array(
            'decimal_separator' => '.',
            'prefix_suffix' => TRUE,
            'scale' => 0,
            'thousand_separator' => ' '
          ),
          'type' => 'text_default',
          'weight' => 1
        )
      ),
      'entity_type' => 'relation',
      'field_name' => 'field_character_weight',
      'label' => 'Weight',
      'required' => 0,
      'settings' => array(
        'max' => '',
        'min' => '',
        'prefix' => '',
        'suffix' => '',
        'user_regiser_form' => FALSE
      ),
      'widget' => array(
        'active' => 1,
        'module' => 'number',
        'settings' => array(),
        'type' => 'number',
        'weight' => '1'
      )
    )
  );
  // Exported field: 'relation-character_state-endpoints'
  $fields['relation-character_state-endpoints'] = array(
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
      'bundle' => 'character_state',
      'default_value' => NULL,
      'deleted' => '0',
      'description' => '',
      'display' => array(
        'default' => array(
          'label' => 'above',
          'module' => 'relation_endpoint',
          'settings' => array(),
          'type' => 'relation_endpoint',
          'weight' => 0
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
        'weight' => '0'
      )
    )
  );
  // Exported field: 'relation-character_state-field_character_state_data'
  $fields['relation-character_state-field_character_state_data'] = array(
    'field_config' => array(
      'active' => '1',
      'cardinality' => '1',
      'deleted' => '0',
      'entity_types' => array(),
      'field_name' => 'field_character_state_data',
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
      'bundle' => 'character_state',
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
      'entity_type' => 'relation',
      'field_name' => 'field_character_state_data',
      'label' => 'Data',
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
  );
  // Exported field: 'relation-character_state-field_character_state_meta'
  $fields['relation-character_state-field_character_state_meta'] = array(
    'field_config' => array(
      'active' => '1',
      'cardinality' => '1',
      'deleted' => '0',
      'entity_types' => array(),
      'field_name' => 'field_character_state_meta',
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
      'bundle' => 'character_state',
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
      'entity_type' => 'relation',
      'field_name' => 'field_character_state_meta',
      'label' => 'Metadata',
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
  );
  // Exported field: 'field_collection_item-field_char_proj_flags-field_char_proj_flags_abbr'
  $fields['field_collection_item-field_char_proj_flags-field_char_proj_flags_abbr'] = array(
    'field_config' => array(
      'active' => '1',
      'cardinality' => '1',
      'deleted' => '0',
      'entity_types' => array(),
      'field_name' => 'field_char_proj_flags_abbr',
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
        'max_length' => '5'
      ),
      'translatable' => '0',
      'type' => 'text'
    ),
    'field_instance' => array(
      'bundle' => 'field_char_proj_flags',
      'default_value' => NULL,
      'deleted' => '0',
      'description' => '',
      'display' => array(
        'default' => array(
          'label' => 'above',
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
      'field_name' => 'field_char_proj_flags_abbr',
      'label' => 'Abbreviation',
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
          'size' => '5'
        ),
        'type' => 'text_textfield',
        'weight' => '1'
      )
    )
  );
  // Exported field: 'field_collection_item-field_char_proj_flags-field_char_proj_flags_flag'
  $fields['field_collection_item-field_char_proj_flags-field_char_proj_flags_flag'] = array(
    'field_config' => array(
      'active' => '1',
      'cardinality' => '1',
      'deleted' => '0',
      'entity_types' => array(),
      'field_name' => 'field_char_proj_flags_flag',
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
      'bundle' => 'field_char_proj_flags',
      'default_value' => NULL,
      'deleted' => '0',
      'description' => '',
      'display' => array(
        'default' => array(
          'label' => 'above',
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
      'field_name' => 'field_char_proj_flags_flag',
      'label' => 'Flag',
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
          'size' => '20'
        ),
        'type' => 'text_textfield',
        'weight' => '0'
      )
    )
  );
  // Exported field: 'field_collection_item-field_char_proj_flags-field_char_proj_flags_id'
  $fields['field_collection_item-field_char_proj_flags-field_char_proj_flags_id'] = array(
    'field_config' => array(
      'active' => '1',
      'cardinality' => '1',
      'deleted' => '0',
      'entity_types' => array(),
      'field_name' => 'field_char_proj_flags_id',
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
      'bundle' => 'field_char_proj_flags',
      'default_value' => NULL,
      'deleted' => '0',
      'description' => '',
      'display' => array(
        'default' => array(
          'label' => 'hidden',
          'settings' => array(),
          'type' => 'hidden',
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
      'field_name' => 'field_char_proj_flags_id',
      'label' => 'Id',
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
          'size' => '20'
        ),
        'type' => 'text_textfield',
        'weight' => '2'
      )
    )
  );
  // Translatables
  // Included for use with string extractors like potx.
  t('Data');
  t('Character groups');
  t('Characters');
  t('Description');
  t('Image');
  t('Please select the unit of measurement.');
  t('State');
  t('States');
  t('Taxa');
  t('Unit');
  t('Abbreviation');
  t('Flag');
  t('Flags');
  t('Id');
  t('List of flags (and abbreviation) that can be used to mark values, such as "rare" or "guessed". "computed" and "inherited" are reserved for computed and inherited values, but you can change the abbreviation here.');
  return $fields;
}
