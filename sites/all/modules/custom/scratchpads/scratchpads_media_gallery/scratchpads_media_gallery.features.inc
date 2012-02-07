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
      'base' => 'node_content',
      'description' => t('Upload media, or select media already uploaded to display together in a gallery.'),
      'has_title' => '1',
      'title_label' => t('Title'),
      'help' => ''
    )
  );
  return $items;
}

/**
 * Implements hook_ctools_plugin_api().
 */
function scratchpads_media_gallery_ctools_plugin_api(){
  list($module, $api) = func_get_args();
  if($module == "file_entity" && $api == "file_default_displays"){return array(
      "version" => "1"
    );}
}

/**
 * Implements hook_styles_default_styles().
 */
function scratchpads_media_gallery_styles_default_styles(){
  $styles = array();
  // Exported style: scratchpads_media_gallery_thumbnail
  $styles['file']['styles']['scratchpads_media_gallery_thumbnail'] = array(
    'label' => 'scratchpads_media_gallery_thumbnail',
    'description' => '',
    'preset_info' => array(
      'image' => array(
        'scratchpads_media_gallery_thumbnail' => array(
          'default preset' => 'original',
          'preset' => 'linked_square_thumbnail'
        )
      ),
      'audio' => array(
        'scratchpads_media_gallery_thumbnail' => array(
          'default preset' => 'original',
          'preset' => 'linked_square_thumbnail'
        )
      ),
      'video' => array(
        'scratchpads_media_gallery_thumbnail' => array(
          'default preset' => 'original',
          'preset' => 'linked_square_thumbnail'
        )
      ),
      'default' => array(
        'scratchpads_media_gallery_thumbnail' => array(
          'default preset' => 'original'
        )
      )
    )
  );
  return $styles;
}

/**
 * Implements hook_styles_default_presets_alter().
 */
function scratchpads_media_gallery_styles_default_presets_alter(&$presets){
  $styles = styles_default_styles();
  if($styles['file']['styles']['scratchpads_media_gallery_thumbnail']['storage'] == STYLES_STORAGE_DEFAULT){
    $presets['file']['containers']['image']['styles']['scratchpads_media_gallery_thumbnail'] = array(
      'default preset' => 'original',
      'preset' => 'linked_square_thumbnail'
    );
    $presets['file']['containers']['audio']['styles']['scratchpads_media_gallery_thumbnail'] = array(
      'default preset' => 'original',
      'preset' => 'linked_square_thumbnail'
    );
    $presets['file']['containers']['video']['styles']['scratchpads_media_gallery_thumbnail'] = array(
      'default preset' => 'original',
      'preset' => 'linked_square_thumbnail'
    );
    $presets['file']['containers']['default']['styles']['scratchpads_media_gallery_thumbnail'] = array(
      'default preset' => 'linked_square_thumbnail'
    );
  }
}
