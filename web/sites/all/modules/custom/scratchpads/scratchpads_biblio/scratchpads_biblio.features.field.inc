<?php

/**
 * @file
 * biblio.features.field.inc
 */
/**
 * Implements hook_field_default_fields().
 */
function scratchpads_biblio_field_default_fields(){
  $fields = array();
  // Exported field: 'node-biblio-field_file'
  $fields['node-biblio-field_file'] = array(
    'field_config' => array(
      'locked' => '1',
      'active' => '1',
      'cardinality' => '-1',
      'deleted' => '0',
      'entity_types' => array(),
      'field_name' => 'field_file',
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
        'display_default' => 1,
        'display_field' => 1,
        'uri_scheme' => 'public'
      ),
      'translatable' => '0',
      'type' => 'file'
    ),
    'field_instance' => array(
      'bundle' => 'biblio',
      'deleted' => '0',
      'description' => '',
      'display' => array(
        'default' => array(
          'label' => 'above',
          'module' => 'file',
          'settings' => array(),
          'type' => 'file_default',
          'weight' => 11
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
      'field_name' => 'field_file',
      'label' => 'File attachments',
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
          'allowed_schemes' => array(),
          'allowed_types' => array(
            'audio' => 'audio',
            'default' => 'default',
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
        'weight' => '19'
      )
    )
  );
  // Translatables
  // Included for use with string extractors like potx.
  t('File attachments');
  return $fields;
}
