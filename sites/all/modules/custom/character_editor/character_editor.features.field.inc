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
        'module' => 'image',
        'settings' => array(
          'preview_image_style' => 'thumbnail',
          'progress_indicator' => 'throbber'
        ),
        'type' => 'image_image',
        'weight' => '4'
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
  // Exported field: 'character_editor_character-controlled-field_characters'
  $fields['character_editor_character-controlled-field_characters'] = array(
    'field_config' => array(
      'active' => '1',
      'cardinality' => '-1',
      'deleted' => '0',
      'entity_types' => array(),
      'field_name' => 'field_characters',
      'foreign keys' => array(),
      'indexes' => array(),
      'module' => 'relation_select',
      'settings' => array(),
      'translatable' => '0',
      'type' => 'relation'
    ),
    'field_instance' => array(
      'bundle' => 'controlled',
      'default_value' => NULL,
      'deleted' => '0',
      'description' => '',
      'display' => array(
        'default' => array(
          'label' => 'hidden',
          'settings' => array(),
          'type' => 'hidden',
          'weight' => '3'
        )
      ),
      'entity_type' => 'character_editor_character',
      'field_name' => 'field_characters',
      'label' => 'Characters',
      'required' => 0,
      'settings' => array(
        'relation_type' => 'character_state',
        'relation_view' => 'relation_select:default',
        'user_register_form' => FALSE
      ),
      'widget' => array(
        'active' => 0,
        'module' => 'relation_dummy_field',
        'settings' => array(),
        'type' => 'relation_default',
        'weight' => '1'
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
          'label' => 'above',
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
            'image_style' => ''
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
        'module' => 'image',
        'settings' => array(
          'preview_image_style' => 'thumbnail',
          'progress_indicator' => 'throbber'
        ),
        'type' => 'image_image',
        'weight' => '4'
      )
    )
  );
  // Exported field: 'character_editor_character-dna-field_characters'
  $fields['character_editor_character-dna-field_characters'] = array(
    'field_config' => array(
      'active' => '1',
      'cardinality' => '-1',
      'deleted' => '0',
      'entity_types' => array(),
      'field_name' => 'field_characters',
      'foreign keys' => array(),
      'indexes' => array(),
      'module' => 'relation_select',
      'settings' => array(),
      'translatable' => '0',
      'type' => 'relation'
    ),
    'field_instance' => array(
      'bundle' => 'dna',
      'default_value' => NULL,
      'deleted' => '0',
      'description' => '',
      'display' => array(
        'default' => array(
          'label' => 'hidden',
          'settings' => array(),
          'type' => 'hidden',
          'weight' => '2'
        )
      ),
      'entity_type' => 'character_editor_character',
      'field_name' => 'field_characters',
      'label' => 'Characters',
      'required' => 0,
      'settings' => array(
        'relation_type' => 'character_state',
        'relation_view' => 'relation_select:default',
        'user_register_form' => FALSE
      ),
      'widget' => array(
        'active' => 0,
        'module' => 'relation_dummy_field',
        'settings' => array(),
        'type' => 'relation_default',
        'weight' => '6'
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
            'flickr' => 0,
            'public' => 'public',
            'youtube' => 0
          ),
          'allowed_types' => array(
            'audio' => 0,
            'default' => 0,
            'image' => 'image',
            'video' => 0
          ),
          'browser_plugins' => array(
            'library' => 'library',
            'media_default--media_browser_1' => 0,
            'media_internet' => 0,
            'upload' => 'upload'
          ),
          'progress_indicator' => 'throbber'
        ),
        'type' => 'media_generic',
        'weight' => '0'
      )
    )
  );
  // Exported field: 'character_editor_character-group-field_characters'
  $fields['character_editor_character-group-field_characters'] = array(
    'field_config' => array(
      'active' => '1',
      'cardinality' => '-1',
      'deleted' => '0',
      'entity_types' => array(),
      'field_name' => 'field_characters',
      'foreign keys' => array(),
      'indexes' => array(),
      'module' => 'relation_select',
      'settings' => array(),
      'translatable' => '0',
      'type' => 'relation'
    ),
    'field_instance' => array(
      'bundle' => 'group',
      'default_value' => NULL,
      'deleted' => '0',
      'description' => '',
      'display' => array(
        'default' => array(
          'label' => 'hidden',
          'settings' => array(),
          'type' => 'hidden',
          'weight' => '3'
        )
      ),
      'entity_type' => 'character_editor_character',
      'field_name' => 'field_characters',
      'label' => 'Characters',
      'required' => 0,
      'settings' => array(
        'relation_type' => 'character',
        'relation_view' => 'relation_select:default',
        'user_register_form' => FALSE
      ),
      'widget' => array(
        'active' => 0,
        'module' => 'character_editor',
        'settings' => array(),
        'type' => 'characters',
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
            'image_style' => ''
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
        'module' => 'image',
        'settings' => array(
          'preview_image_style' => 'thumbnail',
          'progress_indicator' => 'throbber'
        ),
        'type' => 'image_image',
        'weight' => '4'
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
  // Exported field: 'character_editor_character-numeric-field_characters'
  $fields['character_editor_character-numeric-field_characters'] = array(
    'field_config' => array(
      'active' => '1',
      'cardinality' => '-1',
      'deleted' => '0',
      'entity_types' => array(),
      'field_name' => 'field_characters',
      'foreign keys' => array(),
      'indexes' => array(),
      'module' => 'relation_select',
      'settings' => array(),
      'translatable' => '0',
      'type' => 'relation'
    ),
    'field_instance' => array(
      'bundle' => 'numeric',
      'default_value' => NULL,
      'deleted' => '0',
      'description' => '',
      'display' => array(
        'default' => array(
          'label' => 'hidden',
          'settings' => array(),
          'type' => 'hidden',
          'weight' => '2'
        )
      ),
      'entity_type' => 'character_editor_character',
      'field_name' => 'field_characters',
      'label' => 'Characters',
      'required' => 0,
      'settings' => array(
        'relation_type' => 'character_state',
        'relation_view' => 'relation_select:default',
        'user_register_form' => FALSE
      ),
      'widget' => array(
        'active' => 0,
        'module' => 'relation_dummy_field',
        'settings' => array(),
        'type' => 'relation_default',
        'weight' => '6'
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
            'image_style' => ''
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
  // Exported field: 'character_editor_character-text-field_characters'
  $fields['character_editor_character-text-field_characters'] = array(
    'field_config' => array(
      'active' => '1',
      'cardinality' => '-1',
      'deleted' => '0',
      'entity_types' => array(),
      'field_name' => 'field_characters',
      'foreign keys' => array(),
      'indexes' => array(),
      'module' => 'relation_select',
      'settings' => array(),
      'translatable' => '0',
      'type' => 'relation'
    ),
    'field_instance' => array(
      'bundle' => 'text',
      'default_value' => NULL,
      'deleted' => '0',
      'description' => '',
      'display' => array(
        'default' => array(
          'label' => 'hidden',
          'settings' => array(),
          'type' => 'hidden',
          'weight' => '2'
        )
      ),
      'entity_type' => 'character_editor_character',
      'field_name' => 'field_characters',
      'label' => 'Characters',
      'required' => 0,
      'settings' => array(
        'relation_type' => 'character_state',
        'relation_view' => 'relation_select:default',
        'user_register_form' => FALSE
      ),
      'widget' => array(
        'active' => 0,
        'module' => 'relation_dummy_field',
        'settings' => array(),
        'type' => 'relation_default',
        'weight' => '5'
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
  // Exported field: 'character_editor_project-default_character_project-field_char_proj_taxa'
  $fields['character_editor_project-default_character_project-field_char_proj_taxa'] = array(
    'field_config' => array(
      'active' => '1',
      'cardinality' => '-1',
      'deleted' => '0',
      'entity_types' => array(),
      'field_name' => 'field_char_proj_taxa',
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
            'vocabulary' => 'classification'
          )
        ),
        'profile2_private' => FALSE
      ),
      'translatable' => '0',
      'type' => 'taxonomy_term_reference'
    ),
    'field_instance' => array(
      'bundle' => 'default_character_project',
      'default_value' => NULL,
      'deleted' => '0',
      'description' => '',
      'display' => array(
        'default' => array(
          'label' => 'above',
          'module' => 'taxonomy',
          'settings' => array(),
          'type' => 'taxonomy_term_reference_link',
          'weight' => '1'
        )
      ),
      'entity_type' => 'character_editor_project',
      'field_name' => 'field_char_proj_taxa',
      'label' => 'Taxa',
      'required' => 0,
      'settings' => array(
        'user_register_form' => FALSE
      ),
      'widget' => array(
        'active' => 1,
        'module' => 'tree_widget',
        'settings' => array(),
        'type' => 'tree_widget',
        'weight' => '2'
      )
    )
  );
  // Exported field: 'character_editor_project-default_character_project-field_characters'
  $fields['character_editor_project-default_character_project-field_characters'] = array(
    'field_config' => array(
      'active' => '1',
      'cardinality' => '-1',
      'deleted' => '0',
      'entity_types' => array(),
      'field_name' => 'field_characters',
      'foreign keys' => array(),
      'indexes' => array(),
      'module' => 'relation_select',
      'settings' => array(),
      'translatable' => '0',
      'type' => 'relation'
    ),
    'field_instance' => array(
      'bundle' => 'default_character_project',
      'default_value' => NULL,
      'deleted' => '0',
      'description' => '',
      'display' => array(
        'default' => array(
          'label' => 'above',
          'module' => 'relation_select',
          'settings' => array(),
          'type' => 'relation_select',
          'weight' => 3
        )
      ),
      'entity_type' => 'character_editor_project',
      'field_name' => 'field_characters',
      'label' => 'Character groups',
      'required' => 0,
      'settings' => array(
        'relation_type' => 'character',
        'relation_view' => 'relation_select:default',
        'user_register_form' => FALSE
      ),
      'widget' => array(
        'active' => 0,
        'module' => 'character_editor',
        'settings' => array(),
        'type' => 'characters',
        'weight' => '3'
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
            'image_style' => ''
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
            'flickr' => 0,
            'public' => 'public',
            'youtube' => 0
          ),
          'allowed_types' => array(
            'audio' => 0,
            'default' => 0,
            'image' => 'image',
            'video' => 0
          ),
          'browser_plugins' => array(
            'library' => 'library',
            'media_default--media_browser_1' => 0,
            'media_internet' => 0,
            'upload' => 'upload'
          ),
          'progress_indicator' => 'throbber'
        ),
        'type' => 'media_generic',
        'weight' => '2'
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
  // Translatables
  // Included for use with string extractors like potx.
  t('Character groups');
  t('Characters');
  t('Description');
  t('Image');
  t('Please select the unit of measurement.');
  t('State');
  t('States');
  t('Taxa');
  t('Unit');
  return $fields;
}
