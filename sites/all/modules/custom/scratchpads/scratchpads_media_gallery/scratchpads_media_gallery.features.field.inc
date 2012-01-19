<?php

/**
 * @file
 * image_gallery.features.field.inc
 */
/**
 * Implements hook_field_default_fields().
 */
function scratchpads_media_gallery_field_default_fields(){
  $fields = array();
  // Exported field: 'node-image_gallery-body'
  $fields['node-image_gallery-body'] = array(
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
      'bundle' => 'image_gallery',
      'default_value' => NULL,
      'deleted' => '0',
      'description' => '',
      'display' => array(
        'default' => array(
          'label' => 'hidden',
          'module' => 'text',
          'settings' => array(),
          'type' => 'text_default',
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
        'weight' => '-4'
      )
    )
  );
  // Exported field: 'node-image_gallery-field_scratchpads_media_gallery'
  $fields['node-image_gallery-field_scratchpads_media_gallery'] = array(
    'field_config' => array(
      'active' => '1',
      'cardinality' => '-1',
      'deleted' => '0',
      'entity_types' => array(),
      'field_name' => 'field_scratchpads_media_gallery',
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
      'bundle' => 'image_gallery',
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
      'field_name' => 'field_scratchpads_media_gallery',
      'label' => 'Media',
      'required' => 1,
      'settings' => array(
        'description_field' => 0,
        'file_directory' => '',
        'file_extensions' => 'png gif jpg jpeg', // FIXME - Add additional file types.
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
  // Exported field: 'file-audio-field_media_galleries'
  $fields['file-audio-field_media_galleries'] = array(
    'field_config' => array(
      'active' => '1',
      'cardinality' => '-1',
      'deleted' => '0',
      'entity_types' => array(),
      'field_name' => 'field_media_galleries',
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
          'image_gallery' => 'image_gallery',
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
      'bundle' => 'audio',
      'default_value' => NULL,
      'deleted' => '0',
      'description' => '',
      'display' => array(
        'default' => array(
          'label' => 'above',
          'module' => 'node_reference',
          'settings' => array(),
          'type' => 'node_reference_default',
          'weight' => 0
        ),
        'file_styles_medium' => array(
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
        'file_styles_thumbnail' => array(
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
      'field_name' => 'field_media_galleries',
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
  // Exported field: 'file-image-field_media_galleries'
  $fields['file-image-field_media_galleries'] = array(
    'field_config' => array(
      'active' => '1',
      'cardinality' => '-1',
      'deleted' => '0',
      'entity_types' => array(),
      'field_name' => 'field_media_galleries',
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
          'image_gallery' => 'image_gallery',
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
          'label' => 'above',
          'module' => 'node_reference',
          'settings' => array(),
          'type' => 'node_reference_default',
          'weight' => 1
        ),
        'file_styles_medium' => array(
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
        'file_styles_thumbnail' => array(
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
      'field_name' => 'field_media_galleries',
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
  // Exported field: 'file-video-field_media_galleries'
  $fields['file-video-field_media_galleries'] = array(
    'field_config' => array(
      'active' => '1',
      'cardinality' => '-1',
      'deleted' => '0',
      'entity_types' => array(),
      'field_name' => 'field_media_galleries',
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
          'image_gallery' => 'image_gallery',
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
          'label' => 'above',
          'module' => 'node_reference',
          'settings' => array(),
          'type' => 'node_reference_default',
          'weight' => 0
        ),
        'file_styles_medium' => array(
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
        'file_styles_thumbnail' => array(
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
      'field_name' => 'field_media_galleries',
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
  // Translatables
  // Included for use with string extractors like potx.
  t('Media galleries');
  t('Body');
  t('Media');
  return $fields;
}
