<?php

/**
 * @file
 * Colorbox theme functions.
 */


/**
 * Returns HTML for an Colorbox image field formatter.
 *
 * @param $variables
 *   An associative array containing:
 *   - item: An array of image data.
 *   - image_style: An optional image style.
 *   - path: An array containing the link 'path' and link 'options'.
 *
 * @ingroup themeable
 */
function theme_colorbox_image_formatter($variables) {
  $item = $variables['item'];
  $node = $variables['node'];
  $field = $variables['field'];
  $settings = $variables['display_settings'];

  $image = array(
    'path' => $item['uri'],
    'alt' => $item['alt'],
    'title' => $item['title'],
    'style_name' => $settings['colorbox_node_style'],
  );

  if (isset($item['width']) && isset($item['height'])) {
    $image['width'] = $item['width'];
    $image['height'] = $item['height'];
  }

  switch ($settings['colorbox_caption']) {
     case 'auto':
      // If the title is empty use alt or the node title in that order.
      if (!empty($image['title'])) {
        $caption = $image['title'];
      }
      elseif (!empty($image['alt'])) {
        $caption = $image['alt'];
      }
      elseif (!empty($node->title)) {
        $caption = $node->title;
      }
      else {
        $caption = '';
      }
      break;
    case 'title':
      $caption = $image['title'];
      break;
    case 'alt':
      $caption = $image['alt'];
      break;
    case 'node_title':
      $caption = $node->title;
      break;
    default:
      $caption = '';
  }

  // Shorten the caption for the example styles or when caption shortening is active.
  $colorbox_style = variable_get('colorbox_style', 'default');
  $trim_length = variable_get('colorbox_caption_trim_length', 75);
  if (((strpos($colorbox_style, 'colorbox/example') !== FALSE) || variable_get('colorbox_caption_trim', 0)) && (drupal_strlen($caption) > $trim_length)) {
    $caption = drupal_substr($caption, 0, $trim_length - 5) . '...';
  }

  // Build the gallery id.
  $nid = !empty($node->nid) ? $node->nid : 'nid';
  switch ($settings['colorbox_gallery']) {
    case 'post':
      $gallery_id = 'gallery-' . $nid;
      break;
    case 'page':
      $gallery_id = 'gallery-all';
      break;
    case 'field_post':
      $gallery_id = 'gallery-' . $nid . '-' . $field['field_name'];
      break;
    case 'field_page':
      $gallery_id = 'gallery-' . $field['field_name'];
      break;
    case 'custom':
      $gallery_id = $settings['colorbox_gallery_custom'];
      break;
    default:
      $gallery_id = '';
  }

  if ($style_name = $settings['colorbox_image_style']) {
    $path = image_style_url($style_name, $image['path']);
  }
  else {
    $path = file_create_url($image['path']);
  }

  return theme('colorbox_imagefield', array('image' => $image, 'path' => $path, 'title' => $caption, 'gid' => $gallery_id));
}

/**
 * Returns HTML for an image using a specific Colorbox image style.
 *
 * @param $variables
 *   An associative array containing:
 *   - image: image item as array.
 *   - path: The path of the image that should be displayed in the Colorbox.
 *   - title: The title text that will be used as a caption in the Colorbox.
 *   - gid: Gallery id for Colorbox image grouping.
 *
 * @ingroup themeable
 */
function theme_colorbox_imagefield($variables) {
  $class = array('colorbox');

  if ($variables['image']['style_name'] == 'hide') {
    $image = '';
    $class[] = 'js-hide';
  }
  else if (!empty($variables['image']['style_name'])) {
    $image = theme('image_style', $variables['image']);
  }
  else {
    $image = theme('image', $variables['image']);
  }

  $options = array(
    'html' => TRUE,
    'attributes' => array(
      'title' => $variables['title'],
      'class' => implode(' ', $class),
      'rel' => $variables['gid'],
    )
  );

  return l($image, $variables['path'], $options);
}

/**
 * Preprocess variables for the colorbox-insert-image.tpl.php file.
 */
function template_preprocess_colorbox_insert_image(&$variables) {
  $class = array();
  $file = file_load($variables['item']['fid']);

  if (!empty($variables['widget']['settings']['insert_class'])) {
    $class = explode(' ', $variables['widget']['settings']['insert_class']);
  }
  $class[] = 'image-' . $variables['item']['style_name'];

  foreach ($class as $key => $value) {
    $class[$key] = drupal_html_class($value);
  }

  $variables['image_path'] = image_style_url($variables['item']['style_name'], $file->uri);

  if ($style_name = variable_get('colorbox_image_style', '')) {
    $variables['link_path'] = image_style_url($style_name, $file->uri);
  }
  else {
    $variables['link_path'] = file_create_url($file->uri);
  }

  $variables['class'] = implode(' ', $class);

  $variables['gallery_id'] = '';
  switch (variable_get('colorbox_insert_gallery', 0)) {
    case 0:
    case 1:
    case 2:
      $variables['gallery_id'] = 'gallery-all';
      break;
    case 3:
      $variables['gallery_id'] = '';
      break;
  }
}
