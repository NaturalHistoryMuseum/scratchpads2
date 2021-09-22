<?php

/**
 *  @file
 *  Theme functions for File styles.
 */

/**
 *  A wrapper for the image style preview.
 */
function theme_file_styles_image_style_preview($variables) {
  $style_name = $variables['style_name'];
  $styles = image_styles();
  if (isset($styles[$style_name])) {
    // The theme_image_style_preview function is in image.admin,
    // but not properly referenced in hook_theme.
    // @TODO: File a bug report and fix this in core.
    module_load_include('inc', 'image', 'image.admin');
    drupal_add_css(drupal_get_path('module', 'image') . '/image.admin.css');

    // Ensure we revert the module & storage namespace for the Image module.
    if (isset($styles[$style_name]['image_module'])) {
      $styles[$style_name]['module'] = $styles[$style_name]['image_module'];
      $styles[$style_name]['storage'] = $styles[$style_name]['image_storage'];
    }

    return theme('image_style_preview', array('style' => $styles[$style_name]));
  }
}

function theme_file_styles_styles_default($variables) {
  $file = $variables['object'];
  $path = drupal_get_path('module', 'file');
  $thumbnail = theme('image',
    array(
      'path' => $path . '/images/file-unknown.png',
      'alt' => t('Thumbnail for !filename.', array('!filename' => $file->filename)),
      'attributes' => array('width' => 100, 'height' => 75, 'class' => 'file-unknown'),
    )
  );
  return $thumbnail;
}

function theme_file_styles_preview_default($variables) {
  return 'default: theme_file_styles_preview_default';
}

function theme_file_styles_field_formatter_styles_default($variables) {
  $file = $variables['object'];
  $file_url = file_create_url($file->uri);
  return l($file->filename, $file_url);
}

function theme_file_styles_field_formatter_styles($variables) {
  // The formatter name needs to match what theme_field_formatter_image expects.
  $variables['element']['#formatter'] = '__' . $variables['style']['name'];

  // Add Alt & Title to the element.
  $variables['element']['#item']['alt'] = $variables['element']['#item']['title'] = $variables['element']['#item']['description'];

  // Defer to Image Styles for the rest.
  return theme('field_formatter_image', $variables);

}

function theme_file_styles_image($variables) {
  $file = $variables['object'];
  $style_name = $variables['style_name'];
  $vars = array();
  $vars['path'] = $file->uri;
  // Allow image attributes to be provided by the passed-in file object. 'alt'
  // and 'title' need to be separated out because they are treated specially by
  // theme_image().
  if (isset($file->override)) {
    $vars['attributes'] = $file->override;
    foreach (array('alt', 'title') as $attribute) {
      if (isset($vars['attributes'][$attribute])) {
        $vars[$attribute] = $vars['attributes'][$attribute];
        unset($vars['attributes'][$attribute]);
      }
    }
  }
  // Add default values for 'alt' and 'title'.
  if (!isset($vars['alt'])) {
    $vars['alt'] = isset($variables['description']) ? $variables['description'] : (isset($file->description) ? $file->description : '');
  }
  if (!isset($vars['title']) || $vars['title'] === '') {
    $vars['title'] = isset($variables['title']) ? $variables['title'] : $vars['alt'];
  }

  // Special case for 'original'.
  // @TODO Certainly can be more elegant than this.
  if ($style_name == 'original') {
    $vars['getsize'] = FALSE;
    return theme('image', $vars);
  }

  $vars['style_name'] = $style_name;
  //@TODO: How to add fields here?
  return theme('image_style', $vars);
}

/**
 *  Support for HTML5 videos out of the box.
 */
function theme_file_styles_video($variables) {
  $file = $variables['object'];
  $style_name = $variables['style_name'];

  $vars = array();
  $width = isset($variables['width']) ? $variables['width'] : NULL;
  $height = isset($variables['height']) ? $variables['height'] : NULL;

  $path = file_create_url($file->uri);

  $preset_name = $variables['preset_name'];
  $preset = styles_containers_available_styles('file', 'video', $preset_name);

  // By default, we don't display as a thumbnail.
  $thumbnail = FALSE;

  foreach ($preset['effects'] as $effect) {
    switch ($effect['name']) {
      case 'resize':
        $width = isset($effect['data']['width']) ? $effect['data']['width'] : $width;
        $height = isset($effect['data']['height']) ? $effect['data']['height'] : $height;
        break;
      case 'thumbnail':
        $thumbnail = TRUE;
        break;
    }
  }

  if (isset($variables['object']->override)) {
    $override = $variables['object']->override;

    $width = isset($override['width']) ? $override['width'] : $width;
    $height = isset($override['height']) ? $override['height'] : $height;
    if (isset($override['wysiwyg']) && $override['wysiwyg']) {
      $thumbnail = TRUE;
    }
  }

  $vars['attributes'] = array('width' => $width, 'height' => $height);

  if ($thumbnail) {
    return theme('image', array('path' => drupal_get_path('module', 'file_styles') . '/images/file-video.png', 'attributes' => $vars['attributes'], 'getsize' => FALSE));
  }

  $width = isset($vars['attributes']['width']) ? ('width="' . $vars['attributes']['width'] . '"') : '';
  $height = isset($vars['attributes']['height']) ? ('height="' . $vars['attributes']['height'] . '"') : '';

  switch($file->filemime) {
    case 'video/ogg':
      $source = "<source src='$path' type='video/ogg; codecs=\"theora, vorbis\"'>";
      break;
    case 'video/mp4':
      $source = "<source src='$path' type='video/mp4; codecs=\"avc1.42E01E, mp4a.40.2\"'>";
      break;
  }

  $output = "<video $width $height controls>$source</video>";

  return $output;
}
