<?php

/**
 *  @file
 *  Hooks available for modules to implement Styles functionality.
 */

/**
 * @addtogroup hooks
 * @{
 */

/**
 * Define information about style containers provided by a module.
 *
 * This hook enables modules to define style containers provided by this module.
 *
 * @return
 *   An array of available style containers.Each container is defined as an
 *   array keyed by the field type, each containing an associative array keyed
 *   on a machine-readable style container name, with the following items:
 *   - "label": The human-readable name of the effect.
 *   - "data": An array of data that each container might require.
 *   - "preview theme": (optional) A theme function to call when previewing
 *     a style during administration.
 *   - "help": (optional) A brief description of the style container that will
 *     be displayed to the administrator when configuring styles.
 */
function hook_styles_containers() {
  return array(
    'media' => array(
      'image' => array(
        'label' => t('Image Styles'),
        'data' => array(
          'streams' => array(
            'public://',
            'private://',
          ),
          'mimetypes' => array(
            'image/png',
            'image/gif',
            'image/jpeg',
          ),
        ),
        'preview theme' => 'media_styles_image_style_preview',
        'help' => t('Image Styles will transform images to your choosing, such as by scaling and cropping. You can !manage.', array('!manage' => l(t('manage your image styles here'), 'admin/config/image/image-styles'))),
      ),
    ),
  );
}

function hook_styles_styles() {
  $styles = array();
  foreach (image_styles() as $style_name => $style) {
    $styles[$style_name] = $style;
  }
  return array(
    'media' => array(
      'image' => $styles,
    ),
  );
}
