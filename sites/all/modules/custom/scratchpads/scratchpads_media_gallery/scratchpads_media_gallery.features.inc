<?php

/**
 * @file
 * media_gallery.features.inc
 */
/**
 * Implements hook_node_info().
 */
function scratchpads_media_gallery_node_info(){
  $items = array(
    'media_gallery' => array(
      'name' => t('Media gallery'),
      'description' => t('Upload media, or select media already uploaded to display together in a gallery.'),
      'has_title' => '1',
      'title_label' => t('Title'),
      'help' => '',
      'base' => 'node_content'
    )
  );
  return $items;
}

/**
 * Implements hook_styles_default_presets_alter().
 */
function scratchpads_media_gallery_styles_default_presets_alter(&$presets){
  foreach(array(
    'medium',
    'species_slideshow_large',
    'species_slideshow_thumbnail'
  ) as $image_style_to_link_to_media){
    foreach(array(
      'default',
      'image'
    ) as $file_type){
      if(@isset($presets['file']['containers'][$file_type]['presets'][$image_style_to_link_to_media]) && is_array($presets['file']['containers'][$file_type]['presets'][$image_style_to_link_to_media])){
        array_unshift($presets['file']['containers'][$file_type]['presets'][$image_style_to_link_to_media], array(
          'name' => 'linkToMedia',
          'settings' => array()
        ));
      }
    }
  }
  $presets['file']['containers']['image']['styles']['square_thumbnail'] = array(
    'default preset' => 'linked_square_thumbnail',
    'preset' => 'linked_square_thumbnail'
  );
  $presets['file']['containers']['audio']['styles']['square_thumbnail'] = array(
    'default preset' => 'original',
    'preset' => 'linked_square_thumbnail'
  );
  $presets['file']['containers']['video']['styles']['square_thumbnail'] = array(
    'default preset' => 'original',
    'preset' => 'linked_square_thumbnail'
  );
  $presets['file']['containers']['default']['styles']['square_thumbnail'] = array(
    'default preset' => 'linked_square_thumbnail'
  );
}