<?php

/**
 * @file
 * media_gallery.features.field.inc
 */
/**
 * Implements hook_field_default_fields().
 */
function scratchpads_media_gallery_field_default_fields(){
  $fields = array();
  // Exported field: 'node-media_gallery-body'
  $fields['node-media_gallery-body'] = array(
    'field_config' => array(
      'locked' => '0',
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
      'bundle' => 'media_gallery',
      'default_value' => NULL,
      'deleted' => '0',
      'description' => '',
      'display' => array(),
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
  // Exported field: 'node-media_gallery-field_media'
  $fields['node-media_gallery-field_media'] = array(
    'field_config' => array(
      'locked' => '1',
      'active' => '1',
      'cardinality' => '-1',
      'deleted' => '0',
      'entity_types' => array(),
      'field_name' => 'field_media',
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
      'bundle' => 'media_gallery',
      'deleted' => '0',
      'description' => 'Select media to link them to this gallery. Once media is selected, it may be edited, or removed from this media gallery.',
      'display' => array(),
      'entity_type' => 'node',
      'field_name' => 'field_media',
      'label' => 'Media',
      'required' => 1,
      'settings' => array(
        'description_field' => 0,
        'file_directory' => '',
        'file_extensions' => FIELD_MEDIA_FILE_EXTENSIONS,
        'max_filesize' => '',
        'user_register_form' => FALSE
      ),
      'widget' => array(
        'active' => 1,
        'module' => 'media_multiselect',
        'settings' => array(
          'allowed_schemes' => array(
            'module' => 0,
            'public' => 'public'
          ),
          'allowed_types' => array(
            'audio' => 'audio',
            'default' => 0,
            'image' => 'image',
            'video' => 'video'
          ),
          'browser_plugins' => array(
            'media_default--media_browser_1' => 0,
            'media_default--media_browser_my_files' => 0,
            'upload' => 0
          ),
          'progress_indicator' => 'throbber'
        ),
        'type' => 'media_multiselect',
        'weight' => '2'
      )
    )
  );
  // Exported field: 'node-media_gallery-field_media'
  $fields['node-page-field_media'] = array(
    'field_config' => array(
      'locked' => '1',
      'active' => '1',
      'cardinality' => '-1',
      'deleted' => '0',
      'entity_types' => array(),
      'field_name' => 'field_media',
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
      'bundle' => 'page',
      'deleted' => '0',
      'description' => 'Select media to link them to this page. Selected media will appear as links at the bottom of the page. Once media is selected, it may be edited, or removed from this page.',
      'display' => array(
        'default' => array(
          'label' => 'above',
          'module' => 'file_entity',
          'settings' => array(
            'file_view_mode' => 'file_styles_square_thumbnail'
          ),
          'type' => 'file_rendered',
          'weight' => '12'
        ),
        'linked_node' => array(
          'label' => 'above',
          'settings' => array(),
          'type' => 'hidden',
          'weight' => 1
        ),
        'teaser' => array(
          'label' => 'above',
          'settings' => array(),
          'type' => 'hidden',
          'weight' => 0
        )
      ),
      'entity_type' => 'node',
      'field_name' => 'field_media',
      'label' => 'Media',
      'required' => 0,
      'settings' => array(
        'description_field' => 0,
        'file_directory' => '',
        'file_extensions' => FIELD_MEDIA_FILE_EXTENSIONS,
        'max_filesize' => '',
        'user_register_form' => FALSE
      ),
      'widget' => array(
        'active' => 1,
        'module' => 'media_multiselect',
        'settings' => array(
          'allowed_schemes' => array(
            'module' => 0,
            'public' => 'public'
          ),
          'allowed_types' => array(
            'audio' => 'audio',
            'default' => 0,
            'image' => 'image',
            'video' => 'video'
          ),
          'browser_plugins' => array(
            'media_default--media_browser_1' => 0,
            'media_default--media_browser_my_files' => 0,
            'upload' => 0
          ),
          'progress_indicator' => 'throbber'
        ),
        'type' => 'media_multiselect',
        'weight' => '-3'
      )
    )
  );
  // Exported field: 'file-audio-field_smg_galleries'
  $fields['file-audio-field_smg_galleries'] = array(
    'field_config' => array(
      'locked' => '1',
      'active' => '1',
      'cardinality' => '-1',
      'deleted' => '0',
      'entity_types' => array(),
      'field_name' => 'field_smg_galleries',
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
          'media_gallery' => 'media_gallery'
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
      'bundle' => 'audio',
      'default_value' => NULL,
      'deleted' => '0',
      'description' => '',
      'display' => array(
        'default' => array(
          'label' => 'hidden',
          'settings' => array(),
          'type' => 'hidden',
          'weight' => 0
        )
      ),
      'entity_type' => 'file',
      'field_name' => 'field_smg_galleries',
      'label' => 'Media galleries',
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
        'weight' => '-3'
      )
    )
  );
  // Exported field: 'file-image-field_smg_galleries'
  $fields['file-image-field_smg_galleries'] = array(
    'field_config' => array(
      'locked' => '1',
      'active' => '1',
      'cardinality' => '-1',
      'deleted' => '0',
      'entity_types' => array(),
      'field_name' => 'field_smg_galleries',
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
          'media_gallery' => 'media_gallery',
          'page' => 0
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
      'bundle' => 'image',
      'default_value' => NULL,
      'deleted' => '0',
      'description' => '',
      'display' => array(
        'default' => array(
          'label' => 'hidden',
          'settings' => array(),
          'type' => 'hidden',
          'weight' => 0
        )
      ),
      'entity_type' => 'file',
      'field_name' => 'field_smg_galleries',
      'label' => 'Media galleries',
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
  );
  // Exported field: 'file-video-field_smg_galleries'
  $fields['file-video-field_smg_galleries'] = array(
    'field_config' => array(
      'locked' => '1',
      'active' => '1',
      'cardinality' => '-1',
      'deleted' => '0',
      'entity_types' => array(),
      'field_name' => 'field_smg_galleries',
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
          'media_gallery' => 'media_gallery',
          'page' => 0
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
      'bundle' => 'video',
      'default_value' => NULL,
      'deleted' => '0',
      'description' => '',
      'display' => array(
        'default' => array(
          'label' => 'hidden',
          'settings' => array(),
          'type' => 'hidden',
          'weight' => 0
        )
      ),
      'entity_type' => 'file',
      'field_name' => 'field_smg_galleries',
      'label' => 'Media galleries',
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
        'weight' => '-3'
      )
    )
  );
  // Exported field: 'file-image-field_taxonomic_name'
  $fields['file-image-field_taxonomic_name'] = array(
    'field_config' => field_info_field('field_taxonomic_name'),
    'field_instance' => array(
      'bundle' => 'image',
      'default_value' => NULL,
      'deleted' => '0',
      'description' => 'Taxon name for this file',
      'display' => array(
        'default' => array(
          'label' => 'above',
          'module' => 'taxonomy_formatter',
          'settings' => array(
            'element_class' => '',
            'element_option' => '- None -',
            'links_option' => 1,
            'append_vocabulary' => 1,
            'separator_option' => ', ',
            'wrapper_class' => '',
            'wrapper_option' => '- None -'
          ),
          'type' => 'taxonomy_term_reference_csv',
          'weight' => '1000'
        ),
        'file_styles_colorbox' => array(
          'label' => 'above',
          'module' => 'taxonomy_formatter',
          'settings' => array(
            'element_class' => '',
            'element_option' => '- None -',
            'links_option' => 1,
            'append_vocabulary' => 1,
            'separator_option' => ', ',
            'wrapper_class' => '',
            'wrapper_option' => '- None -'
          ),
          'type' => 'taxonomy_term_reference_csv',
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
      'entity_type' => 'file',
      'field_name' => 'field_taxonomic_name',
      'label' => 'Taxonomic name',
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
        'weight' => '-3'
      )
    )
  );
  // Exported field: 'file-video-field_taxonomic_name'
  $fields['file-video-field_taxonomic_name'] = array(
    'field_config' => field_info_field('field_taxonomic_name'),
    'field_instance' => array(
      'bundle' => 'video',
      'default_value' => NULL,
      'deleted' => '0',
      'description' => 'Taxon name for this file',
      'display' => array(
        'default' => array(
          'label' => 'above',
          'module' => 'taxonomy_formatter',
          'settings' => array(
            'element_class' => '',
            'element_option' => '- None -',
            'links_option' => 1,
            'append_vocabulary' => 1,
            'separator_option' => ', ',
            'wrapper_class' => '',
            'wrapper_option' => '- None -'
          ),
          'type' => 'taxonomy_term_reference_csv',
          'weight' => '1'
        ),
        'linked_node' => array(
          'label' => 'none',
          'settings' => array(),
          'type' => 'hidden',
          'weight' => 0
        ),
        'teaser' => array(
          'label' => 'none',
          'settings' => array(),
          'type' => 'hidden',
          'weight' => 0
        )
      ),
      'entity_type' => 'file',
      'field_name' => 'field_taxonomic_name',
      'label' => 'Taxonomic name',
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
  );
  // Exported field: 'file-audio-field_taxonomic_name'
  $fields['file-audio-field_taxonomic_name'] = array(
    'field_config' => field_info_field('field_taxonomic_name'),
    'field_instance' => array(
      'bundle' => 'audio',
      'default_value' => NULL,
      'deleted' => '0',
      'description' => 'Taxon name for this file',
      'display' => array(
        'default' => array(
          'label' => 'above',
          'module' => 'taxonomy_formatter',
          'settings' => array(
            'element_class' => '',
            'element_option' => '- None -',
            'links_option' => 1,
            'append_vocabulary' => 1,
            'separator_option' => ', ',
            'wrapper_class' => '',
            'wrapper_option' => '- None -'
          ),
          'type' => 'taxonomy_term_reference_csv',
          'weight' => '1'
        ),
        'linked_node' => array(
          'label' => 'none',
          'settings' => array(),
          'type' => 'hidden',
          'weight' => 0
        ),
        'teaser' => array(
          'label' => 'none',
          'settings' => array(),
          'type' => 'hidden',
          'weight' => 0
        )
      ),
      'entity_type' => 'file',
      'field_name' => 'field_taxonomic_name',
      'label' => 'Taxonomic name',
      'required' => 1,
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
        'weight' => '-1000'
      )
    )
  );
  // Exported field: 'file-image-field_cc_licence'
  $fields['file-image-field_cc_licence'] = array(
    'field_config' => array(
      'locked' => '1',
      'active' => '1',
      'cardinality' => '1',
      'deleted' => '0',
      'entity_types' => array(),
      'field_name' => 'field_cc_licence',
      'foreign keys' => array(),
      'indexes' => array(
        'licence' => array(
          0 => 'licence'
        )
      ),
      'module' => 'creative_commons',
      'settings' => array(
        'allowed_values' => array(
          1 => '1',
          2 => '2',
          3 => '3',
          4 => '4',
          5 => '5',
          6 => '6',
          7 => '7',
          8 => '8',
          9 => '9'
        ),
        'jurisdiction' => ''
      ),
      'translatable' => '0',
      'type' => 'creative_commons'
    ),
    'field_instance' => array(
      'bundle' => 'image',
      'default_value' => array(
        0 => array(
          'licence' => variable_get('creative_commons_block_licence_type', CC_BY)
        )
      ),
      'deleted' => '0',
      'description' => '',
      'display' => array(
        'default' => array(
          'label' => 'above',
          'module' => 'creative_commons',
          'settings' => array(),
          'type' => 'creative_commons_small_image',
          'weight' => 2
        ),
        'file_styles_colorbox' => array(
          'label' => 'above',
          'module' => 'creative_commons',
          'settings' => array(),
          'type' => 'creative_commons_small_image',
          'weight' => '4'
        ),
        'file_styles_square_thumbnail' => array(
          'label' => 'above',
          'settings' => array(),
          'type' => 'hidden',
          'weight' => 0
        )
      ),
      'entity_type' => 'file',
      'field_name' => 'field_cc_licence',
      'label' => 'Licence',
      'required' => 0,
      'settings' => array(
        'user_register_form' => FALSE
      ),
      'widget' => array(
        'active' => 0,
        'module' => 'creative_commons',
        'settings' => array(),
        'type' => 'select',
        'weight' => '3'
      )
    )
  );
  // Exported field: 'file-audio-field_cc_licence'
  $fields['file-audio-field_cc_licence'] = array(
    'field_config' => array(
      'locked' => '1',
      'active' => '1',
      'cardinality' => '1',
      'deleted' => '0',
      'entity_types' => array(),
      'field_name' => 'field_cc_licence',
      'foreign keys' => array(),
      'indexes' => array(
        'licence' => array(
          0 => 'licence'
        )
      ),
      'module' => 'creative_commons',
      'settings' => array(
        'allowed_values' => array(
          1 => '1',
          2 => '2',
          3 => '3',
          4 => '4',
          5 => '5',
          6 => '6',
          7 => '7',
          8 => '8',
          9 => '9'
        ),
        'jurisdiction' => ''
      ),
      'translatable' => '0',
      'type' => 'creative_commons'
    ),
    'field_instance' => array(
      'bundle' => 'audio',
      'default_value' => array(
        0 => array(
          'licence' => variable_get('creative_commons_block_licence_type', CC_BY)
        )
      ),
      'deleted' => '0',
      'description' => '',
      'display' => array(
        'default' => array(
          'label' => 'above',
          'module' => 'creative_commons',
          'settings' => array(),
          'type' => 'creative_commons_small_image',
          'weight' => 2
        ),
        'file_styles_colorbox' => array(
          'label' => 'above',
          'module' => 'creative_commons',
          'settings' => array(),
          'type' => 'creative_commons_small_image',
          'weight' => '4'
        ),
        'file_styles_square_thumbnail' => array(
          'label' => 'hidden',
          'settings' => array(),
          'type' => 'hidden',
          'weight' => 0
        )
      ),
      'entity_type' => 'file',
      'field_name' => 'field_cc_licence',
      'label' => 'Licence',
      'required' => 0,
      'settings' => array(
        'user_register_form' => FALSE
      ),
      'widget' => array(
        'active' => 0,
        'module' => 'creative_commons',
        'settings' => array(),
        'type' => 'select',
        'weight' => '3'
      )
    )
  );
  // Exported field: 'file-video-field_cc_licence'
  $fields['file-video-field_cc_licence'] = array(
    'field_config' => array(
      'locked' => '1',
      'active' => '1',
      'cardinality' => '1',
      'deleted' => '0',
      'entity_types' => array(),
      'field_name' => 'field_cc_licence',
      'foreign keys' => array(),
      'indexes' => array(
        'licence' => array(
          0 => 'licence'
        )
      ),
      'module' => 'creative_commons',
      'settings' => array(
        'allowed_values' => array(
          1 => '1',
          2 => '2',
          3 => '3',
          4 => '4',
          5 => '5',
          6 => '6',
          7 => '7',
          8 => '8',
          9 => '9'
        ),
        'jurisdiction' => ''
      ),
      'translatable' => '0',
      'type' => 'creative_commons'
    ),
    'field_instance' => array(
      'bundle' => 'video',
      'default_value' => array(
        0 => array(
          'licence' => variable_get('creative_commons_block_licence_type', CC_BY)
        )
      ),
      'deleted' => '0',
      'description' => '',
      'display' => array(
        'default' => array(
          'label' => 'above',
          'module' => 'creative_commons',
          'settings' => array(),
          'type' => 'creative_commons_small_image',
          'weight' => 2
        ),
        'file_styles_colorbox' => array(
          'label' => 'above',
          'module' => 'creative_commons',
          'settings' => array(),
          'type' => 'creative_commons_small_image',
          'weight' => '4'
        ),
        'file_styles_square_thumbnail' => array(
          'label' => 'hidden',
          'settings' => array(),
          'type' => 'hidden',
          'weight' => 0
        )
      ),
      'entity_type' => 'file',
      'field_name' => 'field_cc_licence',
      'label' => 'Licence',
      'required' => 0,
      'settings' => array(
        'user_register_form' => FALSE
      ),
      'widget' => array(
        'active' => 0,
        'module' => 'creative_commons',
        'settings' => array(),
        'type' => 'select',
        'weight' => '3'
      )
    )
  );
  // Exported field: 'file-image-field_imaging_technique'
  $fields['file-image-field_imaging_technique'] = array(
    'field_config' => array(
      'locked' => '1',
      'active' => '1',
      'cardinality' => '1',
      'deleted' => '0',
      'entity_types' => array(),
      'field_name' => 'field_imaging_technique',
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
            'vocabulary' => 'imaging_technique'
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
          )
        )
      ),
      'translatable' => '0',
      'type' => 'taxonomy_term_reference'
    ),
    'field_instance' => array(
      'bundle' => 'image',
      'default_value' => NULL,
      'deleted' => '0',
      'description' => 'Select the imaging technique that was used to produce the images.',
      'display' => array(
        'default' => array(
          'label' => 'above',
          'module' => 'taxonomy',
          'settings' => array(),
          'type' => 'taxonomy_term_reference_plain',
          'weight' => 3
        ),
        'file_styles_colorbox' => array(
          'label' => 'above',
          'module' => 'taxonomy',
          'settings' => array(),
          'type' => 'taxonomy_term_reference_plain',
          'weight' => 3
        ),
        'file_styles_square_thumbnail' => array(
          'label' => 'above',
          'settings' => array(),
          'type' => 'hidden',
          'weight' => 0
        )
      ),
      'entity_type' => 'file',
      'field_name' => 'field_imaging_technique',
      'label' => 'Imaging technique',
      'required' => 0,
      'settings' => array(
        'user_register_form' => FALSE
      ),
      'widget' => array(
        'active' => 1,
        'module' => 'options',
        'settings' => array(),
        'type' => 'options_select',
        'weight' => '4'
      )
    )
  );
  // Exported field: 'file-image-field_keywords'
  $fields['file-image-field_keywords'] = array(
    'field_config' => array(
      'locked' => '1',
      'active' => '1',
      'cardinality' => '-1',
      'deleted' => '0',
      'entity_types' => array(),
      'field_name' => 'field_keywords',
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
            'vocabulary' => 'image_keywords',
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
          )
        )
      ),
      'translatable' => '0',
      'type' => 'taxonomy_term_reference'
    ),
    'field_instance' => array(
      'bundle' => 'image',
      'default_value' => NULL,
      'deleted' => '0',
      'description' => 'Comma-separated keywords describing the image Parts, sex, form, developmental stage, view angle, people, building etc. Enclose phrases in "quotation marks". Examples: Antenna, Head, Juvenile, "First instar", conference, museum, specimen.',
      'display' => array(
        'default' => array(
          'label' => 'above',
          'module' => 'taxonomy_formatter',
          'settings' => array(
            'element_class' => '',
            'element_option' => '- None -',
            'links_option' => 1,
            'append_vocabulary' => 1,
            'separator_option' => ', ',
            'wrapper_class' => '',
            'wrapper_option' => '- None -'
          ),
          'type' => 'taxonomy_term_reference_csv',
          'weight' => '5'
        ),
        'file_styles_colorbox' => array(
          'label' => 'above',
          'module' => 'taxonomy_formatter',
          'settings' => array(
            'element_class' => '',
            'element_option' => '- None -',
            'links_option' => 1,
            'append_vocabulary' => 1,
            'separator_option' => ', ',
            'wrapper_class' => '',
            'wrapper_option' => '- None -'
          ),
          'type' => 'taxonomy_term_reference_csv',
          'weight' => '5'
        ),
        'file_styles_square_thumbnail' => array(
          'label' => 'above',
          'settings' => array(),
          'type' => 'hidden',
          'weight' => 0
        )
      ),
      'entity_type' => 'file',
      'field_name' => 'field_keywords',
      'label' => 'Keywords',
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
        'weight' => '6'
      )
    )
  );
  // Exported field: 'file-image-field_preparation_technique'
  $fields['file-image-field_preparation_technique'] = array(
    'field_config' => array(
      'locked' => '1',
      'active' => '1',
      'cardinality' => '1',
      'deleted' => '0',
      'entity_types' => array(),
      'field_name' => 'field_preparation_technique',
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
            'vocabulary' => 'preparation_technique'
          )
        )
      ),
      'translatable' => '0',
      'type' => 'taxonomy_term_reference'
    ),
    'field_instance' => array(
      'bundle' => 'image',
      'default_value' => NULL,
      'deleted' => '0',
      'description' => 'Describe how the specimen was prepared for imaging (Simply leave blank if your image isn\'t a specimen)',
      'display' => array(
        'default' => array(
          'label' => 'above',
          'module' => 'taxonomy',
          'settings' => array(),
          'type' => 'taxonomy_term_reference_plain',
          'weight' => 5
        ),
        'file_styles_colorbox' => array(
          'label' => 'above',
          'module' => 'taxonomy',
          'settings' => array(),
          'type' => 'taxonomy_term_reference_plain',
          'weight' => 5
        ),
        'file_styles_square_thumbnail' => array(
          'label' => 'above',
          'settings' => array(),
          'type' => 'hidden',
          'weight' => 0
        )
      ),
      'entity_type' => 'file',
      'field_name' => 'field_preparation_technique',
      'label' => 'Preparation technique',
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
        'weight' => '5'
      )
    )
  );
  // Exported field: 'file-audio-field_description'
  $fields['file-audio-field_description'] = array(
    'field_config' => array(
      'locked' => '1',
      'active' => '1',
      'cardinality' => '1',
      'deleted' => '0',
      'entity_types' => array(),
      'field_name' => 'field_description',
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
      'bundle' => 'audio',
      'default_value' => NULL,
      'deleted' => '0',
      'description' => '',
      'display' => array(
        'default' => array(
          'label' => 'hidden',
          'module' => 'text',
          'settings' => array(),
          'type' => 'text_default',
          'weight' => '3'
        ),
        'file_styles_colorbox' => array(
          'label' => 'none',
          'module' => 'text',
          'settings' => array(),
          'type' => 'text_default',
          'weight' => 0
        ),
        'file_styles_large' => array(
          'label' => 'none',
          'settings' => array(),
          'type' => 'hidden',
          'weight' => 0
        ),
        'file_styles_medium' => array(
          'label' => 'none',
          'settings' => array(),
          'type' => 'hidden',
          'weight' => 0
        ),
        'file_styles_original' => array(
          'label' => 'none',
          'settings' => array(),
          'type' => 'hidden',
          'weight' => 0
        ),
        'file_styles_slideshow_large' => array(
          'label' => 'none',
          'settings' => array(),
          'type' => 'hidden',
          'weight' => 0
        ),
        'file_styles_slideshow_thumbnail' => array(
          'label' => 'none',
          'settings' => array(),
          'type' => 'hidden',
          'weight' => 0
        ),
        'file_styles_species_slideshow_large' => array(
          'label' => 'none',
          'settings' => array(),
          'type' => 'hidden',
          'weight' => 0
        ),
        'file_styles_species_slideshow_thumbnail' => array(
          'label' => 'none',
          'settings' => array(),
          'type' => 'hidden',
          'weight' => 0
        ),
        'file_styles_square_thumbnail' => array(
          'label' => 'none',
          'settings' => array(),
          'type' => 'hidden',
          'weight' => 0
        ),
        'file_styles_square_thumbnail_large' => array(
          'label' => 'none',
          'settings' => array(),
          'type' => 'hidden',
          'weight' => 0
        ),
        'file_styles_thumbnail' => array(
          'label' => 'none',
          'settings' => array(),
          'type' => 'hidden',
          'weight' => 0
        ),
        'linked_node' => array(
          'label' => 'none',
          'settings' => array(),
          'type' => 'hidden',
          'weight' => 0
        ),
        'media_large' => array(
          'label' => 'none',
          'settings' => array(),
          'type' => 'hidden',
          'weight' => 0
        ),
        'media_link' => array(
          'label' => 'none',
          'settings' => array(),
          'type' => 'hidden',
          'weight' => 0
        ),
        'media_original' => array(
          'label' => 'none',
          'settings' => array(),
          'type' => 'hidden',
          'weight' => 0
        ),
        'media_preview' => array(
          'label' => 'none',
          'settings' => array(),
          'type' => 'hidden',
          'weight' => 0
        ),
        'media_small' => array(
          'label' => 'none',
          'settings' => array(),
          'type' => 'hidden',
          'weight' => 0
        )
      ),
      'entity_type' => 'file',
      'field_name' => 'field_description',
      'label' => 'Description',
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
        'weight' => '5'
      )
    )
  );
  // Exported field: 'file-image-field_description'
  $fields['file-image-field_description'] = array(
    'field_config' => array(
      'locked' => '1',
      'active' => '1',
      'cardinality' => '1',
      'deleted' => '0',
      'entity_types' => array(),
      'field_name' => 'field_description',
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
      'bundle' => 'image',
      'default_value' => NULL,
      'deleted' => '0',
      'description' => '',
      'display' => array(
        'default' => array(
          'label' => 'none',
          'module' => 'text',
          'settings' => array(),
          'type' => 'text_default',
          'weight' => 6
        ),
        'file_styles_colorbox' => array(
          'label' => 'none',
          'module' => 'text',
          'settings' => array(),
          'type' => 'text_default',
          'weight' => 0
        ),
        'file_styles_large' => array(
          'label' => 'none',
          'settings' => array(),
          'type' => 'hidden',
          'weight' => 0
        ),
        'file_styles_medium' => array(
          'label' => 'none',
          'settings' => array(),
          'type' => 'hidden',
          'weight' => 0
        ),
        'file_styles_original' => array(
          'label' => 'none',
          'settings' => array(),
          'type' => 'hidden',
          'weight' => 0
        ),
        'file_styles_slideshow_large' => array(
          'label' => 'none',
          'settings' => array(),
          'type' => 'hidden',
          'weight' => 0
        ),
        'file_styles_slideshow_thumbnail' => array(
          'label' => 'none',
          'settings' => array(),
          'type' => 'hidden',
          'weight' => 0
        ),
        'file_styles_species_slideshow_large' => array(
          'label' => 'none',
          'settings' => array(),
          'type' => 'hidden',
          'weight' => 0
        ),
        'file_styles_species_slideshow_thumbnail' => array(
          'label' => 'none',
          'settings' => array(),
          'type' => 'hidden',
          'weight' => 0
        ),
        'file_styles_square_thumbnail' => array(
          'label' => 'none',
          'settings' => array(),
          'type' => 'hidden',
          'weight' => 0
        ),
        'file_styles_square_thumbnail_large' => array(
          'label' => 'none',
          'settings' => array(),
          'type' => 'hidden',
          'weight' => 0
        ),
        'file_styles_thumbnail' => array(
          'label' => 'none',
          'settings' => array(),
          'type' => 'hidden',
          'weight' => 0
        ),
        'linked_node' => array(
          'label' => 'none',
          'settings' => array(),
          'type' => 'hidden',
          'weight' => 0
        ),
        'media_large' => array(
          'label' => 'none',
          'settings' => array(),
          'type' => 'hidden',
          'weight' => 0
        ),
        'media_link' => array(
          'label' => 'none',
          'settings' => array(),
          'type' => 'hidden',
          'weight' => 0
        ),
        'media_original' => array(
          'label' => 'none',
          'settings' => array(),
          'type' => 'hidden',
          'weight' => 0
        ),
        'media_preview' => array(
          'label' => 'none',
          'settings' => array(),
          'type' => 'hidden',
          'weight' => 0
        ),
        'media_small' => array(
          'label' => 'none',
          'settings' => array(),
          'type' => 'hidden',
          'weight' => 0
        )
      ),
      'entity_type' => 'file',
      'field_name' => 'field_description',
      'label' => 'Description',
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
        'weight' => '11'
      )
    )
  );
  // Exported field: 'file-video-field_description'
  $fields['file-video-field_description'] = array(
    'field_config' => array(
      'locked' => '1',
      'active' => '1',
      'cardinality' => '1',
      'deleted' => '0',
      'entity_types' => array(),
      'field_name' => 'field_description',
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
      'bundle' => 'video',
      'default_value' => NULL,
      'deleted' => '0',
      'description' => '',
      'display' => array(
        'default' => array(
          'label' => 'hidden',
          'module' => 'text',
          'settings' => array(),
          'type' => 'text_default',
          'weight' => '3'
        ),
        'file_styles_colorbox' => array(
          'label' => 'none',
          'module' => 'text',
          'settings' => array(),
          'type' => 'text_default',
          'weight' => 0
        ),
        'file_styles_large' => array(
          'label' => 'none',
          'settings' => array(),
          'type' => 'hidden',
          'weight' => 0
        ),
        'file_styles_medium' => array(
          'label' => 'none',
          'settings' => array(),
          'type' => 'hidden',
          'weight' => 0
        ),
        'file_styles_original' => array(
          'label' => 'none',
          'settings' => array(),
          'type' => 'hidden',
          'weight' => 0
        ),
        'file_styles_slideshow_large' => array(
          'label' => 'none',
          'settings' => array(),
          'type' => 'hidden',
          'weight' => 0
        ),
        'file_styles_slideshow_thumbnail' => array(
          'label' => 'none',
          'settings' => array(),
          'type' => 'hidden',
          'weight' => 0
        ),
        'file_styles_species_slideshow_large' => array(
          'label' => 'none',
          'settings' => array(),
          'type' => 'hidden',
          'weight' => 0
        ),
        'file_styles_species_slideshow_thumbnail' => array(
          'label' => 'none',
          'settings' => array(),
          'type' => 'hidden',
          'weight' => 0
        ),
        'file_styles_square_thumbnail' => array(
          'label' => 'none',
          'settings' => array(),
          'type' => 'hidden',
          'weight' => 0
        ),
        'file_styles_square_thumbnail_large' => array(
          'label' => 'none',
          'settings' => array(),
          'type' => 'hidden',
          'weight' => 0
        ),
        'file_styles_thumbnail' => array(
          'label' => 'none',
          'settings' => array(),
          'type' => 'hidden',
          'weight' => 0
        ),
        'linked_node' => array(
          'label' => 'none',
          'settings' => array(),
          'type' => 'hidden',
          'weight' => 0
        ),
        'media_large' => array(
          'label' => 'none',
          'settings' => array(),
          'type' => 'hidden',
          'weight' => 0
        ),
        'media_link' => array(
          'label' => 'none',
          'settings' => array(),
          'type' => 'hidden',
          'weight' => 0
        ),
        'media_original' => array(
          'label' => 'none',
          'settings' => array(),
          'type' => 'hidden',
          'weight' => 0
        ),
        'media_preview' => array(
          'label' => 'none',
          'settings' => array(),
          'type' => 'hidden',
          'weight' => 0
        ),
        'media_small' => array(
          'label' => 'none',
          'settings' => array(),
          'type' => 'hidden',
          'weight' => 0
        )
      ),
      'entity_type' => 'file',
      'field_name' => 'field_description',
      'label' => 'Description',
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
        'weight' => '12'
      )
    )
  );
  // Exported field: 'file-image-field_sticky'
  $fields['file-image-field_sticky'] = array(
    'field_config' => array(
      'locked' => '1',
      'active' => '1',
      'cardinality' => '1',
      'deleted' => '0',
      'entity_types' => array(),
      'field_name' => 'field_sticky',
      'foreign keys' => array(),
      'indexes' => array(
        'value' => array(
          0 => 'value'
        )
      ),
      'module' => 'list',
      'settings' => array(
        'allowed_values' => array(
          0 => '',
          1 => ''
        ),
        'allowed_values_function' => ''
      ),
      'translatable' => '0',
      'type' => 'list_boolean'
    ),
    'field_instance' => array(
      'bundle' => 'image',
      'default_value' => array(
        0 => array(
          'value' => 0
        )
      ),
      'deleted' => '0',
      'description' => 'Check this box to display this item at top of lists (for example, on the species pages)',
      'display' => array(
        'default' => array(
          'label' => 'above',
          'settings' => array(),
          'type' => 'hidden',
          'weight' => 2
        ),
        'file_styles_colorbox' => array(
          'label' => 'above',
          'settings' => array(),
          'type' => 'hidden',
          'weight' => 0
        ),
        'file_styles_large' => array(
          'label' => 'above',
          'settings' => array(),
          'type' => 'hidden',
          'weight' => 0
        ),
        'file_styles_medium' => array(
          'label' => 'above',
          'settings' => array(),
          'type' => 'hidden',
          'weight' => 0
        ),
        'file_styles_original' => array(
          'label' => 'above',
          'settings' => array(),
          'type' => 'hidden',
          'weight' => 0
        ),
        'file_styles_slideshow_large' => array(
          'label' => 'above',
          'settings' => array(),
          'type' => 'hidden',
          'weight' => 0
        ),
        'file_styles_slideshow_thumbnail' => array(
          'label' => 'above',
          'settings' => array(),
          'type' => 'hidden',
          'weight' => 0
        ),
        'file_styles_species_slideshow_large' => array(
          'label' => 'above',
          'settings' => array(),
          'type' => 'hidden',
          'weight' => 0
        ),
        'file_styles_species_slideshow_thumbnail' => array(
          'label' => 'above',
          'settings' => array(),
          'type' => 'hidden',
          'weight' => 0
        ),
        'file_styles_square_thumbnail' => array(
          'label' => 'above',
          'settings' => array(),
          'type' => 'hidden',
          'weight' => 0
        ),
        'file_styles_square_thumbnail_large' => array(
          'label' => 'above',
          'settings' => array(),
          'type' => 'hidden',
          'weight' => 0
        ),
        'file_styles_thumbnail' => array(
          'label' => 'above',
          'settings' => array(),
          'type' => 'hidden',
          'weight' => 0
        ),
        'linked_node' => array(
          'label' => 'above',
          'settings' => array(),
          'type' => 'hidden',
          'weight' => 0
        ),
        'media_large' => array(
          'label' => 'above',
          'settings' => array(),
          'type' => 'hidden',
          'weight' => 0
        ),
        'media_link' => array(
          'label' => 'above',
          'settings' => array(),
          'type' => 'hidden',
          'weight' => 0
        ),
        'media_original' => array(
          'label' => 'above',
          'settings' => array(),
          'type' => 'hidden',
          'weight' => 0
        ),
        'media_preview' => array(
          'label' => 'above',
          'settings' => array(),
          'type' => 'hidden',
          'weight' => 0
        ),
        'media_small' => array(
          'label' => 'above',
          'settings' => array(),
          'type' => 'hidden',
          'weight' => 0
        )
      ),
      'entity_type' => 'file',
      'field_name' => 'field_sticky',
      'label' => 'Sticky at top of lists',
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
        'weight' => '21'
      )
    )
  );
  // Exported field: 'file-image-field_creator'
  $fields['file-image-field_creator'] = array(
    'field_config' => array(
      'locked' => '1',
      'active' => '1',
      'cardinality' => '1',
      'deleted' => '0',
      'entity_types' => array(),
      'field_name' => 'field_creator',
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
      'bundle' => 'image',
      'default_value' => NULL,
      'deleted' => '0',
      'description' => '',
      'display' => array(
        'default' => array(
          'label' => 'above',
          'module' => 'text',
          'settings' => array(),
          'type' => 'text_default',
          'weight' => 7
        ),
        'file_styles_colorbox' => array(
          'label' => 'above',
          'module' => 'text',
          'settings' => array(),
          'type' => 'text_default',
          'weight' => 7
        ),
        'file_styles_large' => array(
          'label' => 'above',
          'settings' => array(),
          'type' => 'hidden',
          'weight' => 0
        ),
        'file_styles_medium' => array(
          'label' => 'above',
          'settings' => array(),
          'type' => 'hidden',
          'weight' => 0
        ),
        'file_styles_original' => array(
          'label' => 'above',
          'settings' => array(),
          'type' => 'hidden',
          'weight' => 0
        ),
        'file_styles_slideshow_large' => array(
          'label' => 'above',
          'settings' => array(),
          'type' => 'hidden',
          'weight' => 0
        ),
        'file_styles_slideshow_thumbnail' => array(
          'label' => 'above',
          'settings' => array(),
          'type' => 'hidden',
          'weight' => 0
        ),
        'file_styles_species_slideshow_large' => array(
          'label' => 'above',
          'settings' => array(),
          'type' => 'hidden',
          'weight' => 0
        ),
        'file_styles_species_slideshow_thumbnail' => array(
          'label' => 'above',
          'settings' => array(),
          'type' => 'hidden',
          'weight' => 0
        ),
        'file_styles_square_thumbnail' => array(
          'label' => 'above',
          'settings' => array(),
          'type' => 'hidden',
          'weight' => 0
        ),
        'file_styles_square_thumbnail_large' => array(
          'label' => 'above',
          'settings' => array(),
          'type' => 'hidden',
          'weight' => 0
        ),
        'file_styles_thumbnail' => array(
          'label' => 'above',
          'settings' => array(),
          'type' => 'hidden',
          'weight' => 0
        ),
        'linked_node' => array(
          'label' => 'above',
          'settings' => array(),
          'type' => 'hidden',
          'weight' => 0
        ),
        'media_large' => array(
          'label' => 'above',
          'settings' => array(),
          'type' => 'hidden',
          'weight' => 0
        ),
        'media_link' => array(
          'label' => 'above',
          'settings' => array(),
          'type' => 'hidden',
          'weight' => 0
        ),
        'media_original' => array(
          'label' => 'above',
          'settings' => array(),
          'type' => 'hidden',
          'weight' => 0
        ),
        'media_preview' => array(
          'label' => 'above',
          'settings' => array(),
          'type' => 'hidden',
          'weight' => 0
        ),
        'media_small' => array(
          'label' => 'above',
          'settings' => array(),
          'type' => 'hidden',
          'weight' => 0
        )
      ),
      'entity_type' => 'file',
      'field_name' => 'field_creator',
      'label' => 'Creator',
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
        'weight' => '22'
      )
    )
  );
  // Exported field: 'file-image-field_weight'
  $fields['file-image-field_weight'] = array(
    'field_config' => array(
      'locked' => '1',
      'active' => '1',
      'cardinality' => '1',
      'deleted' => '0',
      'entity_types' => array(),
      'field_name' => 'field_weight',
      'foreign keys' => array(),
      'indexes' => array(
        'value' => array(
          0 => 'value'
        )
      ),
      'module' => 'list',
      'settings' => array(
        'allowed_values' => array(
          -100 => '-100',
          -99 => '-99',
          -98 => '-98',
          -97 => '-97',
          -96 => '-96',
          -95 => '-95',
          -94 => '-94',
          -93 => '-93',
          -92 => '-92',
          -91 => '-91',
          -90 => '-90',
          -89 => '-89',
          -88 => '-88',
          -87 => '-87',
          -86 => '-86',
          -85 => '-85',
          -84 => '-84',
          -83 => '-83',
          -82 => '-82',
          -81 => '-81',
          -80 => '-80',
          -79 => '-79',
          -78 => '-78',
          -77 => '-77',
          -76 => '-76',
          -75 => '-75',
          -74 => '-74',
          -73 => '-73',
          -72 => '-72',
          -71 => '-71',
          -70 => '-70',
          -69 => '-69',
          -68 => '-68',
          -67 => '-67',
          -66 => '-66',
          -65 => '-65',
          -64 => '-64',
          -63 => '-63',
          -62 => '-62',
          -61 => '-61',
          -60 => '-60',
          -59 => '-59',
          -58 => '-58',
          -57 => '-57',
          -56 => '-56',
          -55 => '-55',
          -54 => '-54',
          -53 => '-53',
          -52 => '-52',
          -51 => '-51',
          -50 => '-50',
          -49 => '-49',
          -48 => '-48',
          -47 => '-47',
          -46 => '-46',
          -45 => '-45',
          -44 => '-44',
          -43 => '-43',
          -42 => '-42',
          -41 => '-41',
          -40 => '-40',
          -39 => '-39',
          -38 => '-38',
          -37 => '-37',
          -36 => '-36',
          -35 => '-35',
          -34 => '-34',
          -33 => '-33',
          -32 => '-32',
          -31 => '-31',
          -30 => '-30',
          -29 => '-29',
          -28 => '-28',
          -27 => '-27',
          -26 => '-26',
          -25 => '-25',
          -24 => '-24',
          -23 => '-23',
          -22 => '-22',
          -21 => '-21',
          -20 => '-20',
          -19 => '-19',
          -18 => '-18',
          -17 => '-17',
          -16 => '-16',
          -15 => '-15',
          -14 => '-14',
          -13 => '-13',
          -12 => '-12',
          -11 => '-11',
          -10 => '-10',
          -9 => '-9',
          -8 => '-8',
          -7 => '-7',
          -6 => '-6',
          -5 => '-5',
          -4 => '-4',
          -3 => '-3',
          -2 => '-2',
          -1 => '-1',
          0 => '0',
          1 => '1',
          2 => '2',
          3 => '3',
          4 => '4',
          5 => '5',
          6 => '6',
          7 => '7',
          8 => '8',
          9 => '9',
          10 => '10',
          11 => '11',
          12 => '12',
          13 => '13',
          14 => '14',
          15 => '15',
          16 => '16',
          17 => '17',
          18 => '18',
          19 => '19',
          20 => '20',
          21 => '21',
          22 => '22',
          23 => '23',
          24 => '24',
          25 => '25',
          26 => '26',
          27 => '27',
          28 => '28',
          29 => '29',
          30 => '30',
          31 => '31',
          32 => '32',
          33 => '33',
          34 => '34',
          35 => '35',
          36 => '36',
          37 => '37',
          38 => '38',
          39 => '39',
          40 => '40',
          41 => '41',
          42 => '42',
          43 => '43',
          44 => '44',
          45 => '45',
          46 => '46',
          47 => '47',
          48 => '48',
          49 => '49',
          50 => '50',
          51 => '51',
          52 => '52',
          53 => '53',
          54 => '54',
          55 => '55',
          56 => '56',
          57 => '57',
          58 => '58',
          59 => '59',
          60 => '60',
          61 => '61',
          62 => '62',
          63 => '63',
          64 => '64',
          65 => '65',
          66 => '66',
          67 => '67',
          68 => '68',
          69 => '69',
          70 => '70',
          71 => '71',
          72 => '72',
          73 => '73',
          74 => '74',
          75 => '75',
          76 => '76',
          77 => '77',
          78 => '78',
          79 => '79',
          80 => '80',
          81 => '81',
          82 => '82',
          83 => '83',
          84 => '84',
          85 => '85',
          86 => '86',
          87 => '87',
          88 => '88',
          89 => '89',
          90 => '90',
          91 => '91',
          92 => '92',
          93 => '93',
          94 => '94',
          95 => '95',
          96 => '96',
          97 => '97',
          98 => '98',
          99 => '99',
          100 => '100'
        ),
        'allowed_values_function' => ''
      ),
      'translatable' => '0',
      'type' => 'list_integer'
    ),
    'field_instance' => array(
      'bundle' => 'image',
      'default_value' => array(
        0 => array(
          'value' => '0'
        )
      ),
      'deleted' => '0',
      'description' => '',
      'display' => array(
        'default' => array(
          'label' => 'hidden',
          'settings' => array(),
          'type' => 'hidden',
          'weight' => '8'
        ),
        'file_styles_colorbox' => array(
          'label' => 'above',
          'settings' => array(),
          'type' => 'hidden',
          'weight' => 0
        ),
        'file_styles_large' => array(
          'label' => 'above',
          'settings' => array(),
          'type' => 'hidden',
          'weight' => 0
        ),
        'file_styles_medium' => array(
          'label' => 'above',
          'settings' => array(),
          'type' => 'hidden',
          'weight' => 0
        ),
        'file_styles_original' => array(
          'label' => 'above',
          'settings' => array(),
          'type' => 'hidden',
          'weight' => 0
        ),
        'file_styles_slideshow_large' => array(
          'label' => 'above',
          'settings' => array(),
          'type' => 'hidden',
          'weight' => 0
        ),
        'file_styles_slideshow_thumbnail' => array(
          'label' => 'above',
          'settings' => array(),
          'type' => 'hidden',
          'weight' => 0
        ),
        'file_styles_species_slideshow_large' => array(
          'label' => 'above',
          'settings' => array(),
          'type' => 'hidden',
          'weight' => 0
        ),
        'file_styles_species_slideshow_thumbnail' => array(
          'label' => 'above',
          'settings' => array(),
          'type' => 'hidden',
          'weight' => 0
        ),
        'file_styles_square_thumbnail' => array(
          'label' => 'above',
          'settings' => array(),
          'type' => 'hidden',
          'weight' => 0
        ),
        'file_styles_square_thumbnail_large' => array(
          'label' => 'above',
          'settings' => array(),
          'type' => 'hidden',
          'weight' => 0
        ),
        'file_styles_thumbnail' => array(
          'label' => 'above',
          'settings' => array(),
          'type' => 'hidden',
          'weight' => 0
        ),
        'linked_node' => array(
          'label' => 'above',
          'settings' => array(),
          'type' => 'hidden',
          'weight' => 0
        ),
        'media_large' => array(
          'label' => 'above',
          'settings' => array(),
          'type' => 'hidden',
          'weight' => 0
        ),
        'media_link' => array(
          'label' => 'above',
          'settings' => array(),
          'type' => 'hidden',
          'weight' => 0
        ),
        'media_original' => array(
          'label' => 'above',
          'settings' => array(),
          'type' => 'hidden',
          'weight' => 0
        ),
        'media_preview' => array(
          'label' => 'above',
          'settings' => array(),
          'type' => 'hidden',
          'weight' => 0
        ),
        'media_small' => array(
          'label' => 'above',
          'settings' => array(),
          'type' => 'hidden',
          'weight' => 0
        )
      ),
      'entity_type' => 'file',
      'field_name' => 'field_weight',
      'label' => 'Weight',
      'required' => 1,
      'settings' => array(
        'user_register_form' => FALSE
      ),
      'widget' => array(
        'active' => 1,
        'module' => 'options',
        'settings' => array(),
        'type' => 'options_select',
        'weight' => '23'
      )
    )
  );
  // Translatables
  // Included for use with string extractors like potx.
  t('Creator');
  t('Licence');
  t('Media galleries');
  t('Body');
  t('Media');
  t('Comma-separated keywords describing the image Parts, sex, form, developmental stage, view angle, people, building etc. Enclose phrases in "quotation marks". Examples: Antenna, Head, Juvenile, "First instar", conference, museum, specimen.');
  t('Describe how the specimen was prepared for imaging (Simply leave blank if your image isn\'t a specimen)');
  t('Imaging technique');
  t('Keywords');
  t('Preparation technique');
  t('Select the imaging technique that was used to produce the images.');
  t('Check this box to display this item at top of lists (for example, on the species pages)');
  t('Sticky at top of lists');
  t('Weight');
  return $fields;
}
