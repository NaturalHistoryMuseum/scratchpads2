<?php

/**
 * @file
 * Functions needed to execute image effects provided by Image module.
 */

/**
 * Implement hook_file_effect_info().
 */
function file_styles_file_effect_info() {
  $effects = array(
    'file_styles_image' => array(
      'label' => t('Image styles'),
      'styles' => array(),
      'streams' => array('public://', 'private://'),
      'mimetypes' => array('image/gif', 'image/jpeg', 'image/png'),
      'module' => 'image',
      'preview-theme' => 'image_style_preview',
      'add-new-link' => l(t('Add new image style'), 'admin/config/file/image-styles/add'),
    )
  );

  foreach (image_styles() as $style_name => $style) {
    $effects['file_styles_image']['styles'][$style_name] = $style;
  }

  return $effects;
}
