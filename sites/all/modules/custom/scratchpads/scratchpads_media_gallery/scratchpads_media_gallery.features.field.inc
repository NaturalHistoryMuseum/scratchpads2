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
        'weight' => '-4'
      )
    )
  );
  // Exported field: 'node-media_gallery-field_media'
  $fields['node-media_gallery-field_media'] = array(
    'field_config' => array(
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
      'description' => '',
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
        'module' => 'media',
        'settings' => array(
          'allowed_schemes' => array(
            'public' => 'public'
          ),
          'allowed_types' => array(
            'audio' => 'audio',
            'default' => 0,
            'image' => 'image',
            'video' => 'video'
          ),
          'browser_plugins' => array(
            'library' => 0,
            'media_default--media_browser_1' => 0,
            'upload' => 0
          ),
          'progress_indicator' => 'throbber'
        ),
        'type' => 'media_generic',
        'weight' => '-3'
      )
    )
  );
  // Exported field: 'node-media_gallery-field_media'
  $fields['node-page-field_media'] = array(
    'field_config' => array(
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
      'description' => '',
      'display' => array(
        'default' => array(
          'label' => 'above',
          'module' => 'file',
          'settings' => array(),
          'type' => 'file_default',
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
        'module' => 'media',
        'settings' => array(
          'allowed_schemes' => array(
            'public' => 'public'
          ),
          'allowed_types' => array(
            'audio' => 'audio',
            'default' => 0,
            'image' => 'image',
            'video' => 'video'
          ),
          'browser_plugins' => array(
            'library' => 0,
            'media_default--media_browser_1' => 0,
            'upload' => 0
          ),
          'progress_indicator' => 'throbber'
        ),
        'type' => 'media_generic',
        'weight' => '-3'
      )
    )
  );
  // Exported field: 'file-audio-field_smg_galleries'
  $fields['file-audio-field_smg_galleries'] = array(
    'field_config' => array(
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
      'display' => array(),
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
      'display' => array(),
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
      'display' => array(),
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
  // Exported field: 'file-image-field_cc_licence'
  $fields['file-image-field_cc_licence'] = array(
    'field_config' => array(
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
          7 => '7'
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
          'licence' => '5'
        )
      ),
      'deleted' => '0',
      'description' => '',
      'display' => array(),
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
          7 => '7'
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
          'licence' => '5'
        )
      ),
      'deleted' => '0',
      'description' => '',
      'display' => array(),
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
          7 => '7'
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
          'licence' => '5'
        )
      ),
      'deleted' => '0',
      'description' => '',
      'display' => array(),
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
  // Translatables
  // Included for use with string extractors like potx.
  t('Licence');
  t('Media galleries');
  t('Body');
  t('Media');
  return $fields;
}
