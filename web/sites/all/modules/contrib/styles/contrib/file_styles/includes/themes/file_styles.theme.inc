<?php

/**
 * @file file_styles/includes/themes/file_styles.theme.inc
 *
 * Theme and preprocess functions for the File Styles module.
 */

/**
 * Display an image according to the style presented, or raw as specified.
 */
function theme_file_styles_image($variables) {
  // @TODO: Check to see if the image is local as well, for getsize.
  if (isset($variables['image_style'])) {
    $output = theme('image_style', array('style_name' => $variables['image_style'], 'path' => $variables['image_uri'], 'alt' => $variables['alt'], 'title' => $variables['title'], 'getsize' => FALSE, 'attributes' => $variables['attributes']));
  }
  else {
    $output = theme('image', array('path' => $variables['image_uri'], 'alt' => $variables['alt'], 'title' => $variables['title'], 'getsize' => FALSE, 'attributes' => $variables['attributes']));
  }
  return $output;
}

/**
 * Preview image for Styles UI.
 */
function theme_file_styles_image_preview($variables) {
  // Media requires a file object.
  // Also, image_style_url() crashes if you send it a file from a module.
  // Thus, we need to copy it to the public:// directory first, for styles
  // that need to use an image style.
  // @see http://drupal.org/node/987846#comment-3949112
  $sample_image = file_styles_preview_image();
  $variables['object'] = file_styles_uri_to_object($sample_image);
  $variables['field_type'] = 'file';
  return theme('styles', $variables);
}
