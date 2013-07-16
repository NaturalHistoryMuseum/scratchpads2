<?php

/**
 * Implementation of hook_field_default_fields().
 */
function scratchpads_front_field_default_fields(){
  $fields = array();
  // Exported field: 'file-image-field_promote_to_home_page'
  $fields['file-image-field_promote_to_home_page'] = array(
    'field_config' => array(
      'locked' => '1',
      'active' => '1',
      'cardinality' => '1',
      'deleted' => '0',
      'entity_types' => array(),
      'field_name' => 'field_promote_to_home_page',
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
      'description' => 'Check this box to display this item on your site home page.',
      'display' => array(
        'default' => array(
          'label' => 'hidden',
          'module' => 'list',
          'settings' => array(),
          'type' => 'hidden',
          'weight' => 0
        ),
        'file_styles_medium' => array(
          'label' => 'hidden',
          'settings' => array(),
          'type' => 'hidden',
          'weight' => 0
        ),
        'file_styles_slideshow_large' => array(
          'label' => 'hidden',
          'settings' => array(),
          'type' => 'hidden',
          'weight' => 0
        ),
        'file_styles_slideshow_thumbnail' => array(
          'label' => 'hidden',
          'settings' => array(),
          'type' => 'hidden',
          'weight' => 0
        ),
        'file_styles_square_thumbnail' => array(
          'label' => 'hidden',
          'settings' => array(),
          'type' => 'hidden',
          'weight' => 0
        ),
        'file_styles_thumbnail' => array(
          'label' => 'hidden',
          'settings' => array(),
          'type' => 'hidden',
          'weight' => 0
        ),
        'media_large' => array(
          'label' => 'hidden',
          'settings' => array(),
          'type' => 'hidden',
          'weight' => 0
        ),
        'media_link' => array(
          'label' => 'hidden',
          'settings' => array(),
          'type' => 'hidden',
          'weight' => 0
        ),
        'media_original' => array(
          'label' => 'hidden',
          'settings' => array(),
          'type' => 'hidden',
          'weight' => 0
        ),
        'media_preview' => array(
          'label' => 'hidden',
          'settings' => array(),
          'type' => 'hidden',
          'weight' => 0
        ),
        'media_small' => array(
          'label' => 'hidden',
          'settings' => array(),
          'type' => 'hidden',
          'weight' => 0
        )
      ),
      'entity_type' => 'file',
      'field_name' => 'field_promote_to_home_page',
      'label' => 'Promote to home page',
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
        'weight' => '20'
      )
    )
  );
  // Translatables
  // Included for use with string extractors like potx.
  t('Check this box to display this item on your site home page.');
  t('Promote to home page');
  return $fields;
}
