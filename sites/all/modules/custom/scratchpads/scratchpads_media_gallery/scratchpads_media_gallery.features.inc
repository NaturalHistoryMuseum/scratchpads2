<?php
/**
 * @file
 * image_gallery.features.inc
 */

/**
 * Implements hook_node_info().
 */
function scratchpads_media_gallery_node_info() {
  $items = array(
    'image_gallery' => array(
      'name' => t('Image gallery'),
      'base' => 'node_content',
      'description' => t('Upload media, or select media already uploaded to display together in a gallery.'),
      'has_title' => '1',
      'title_label' => t('Title'),
      'help' => '',
    ),
  );
  return $items;
}