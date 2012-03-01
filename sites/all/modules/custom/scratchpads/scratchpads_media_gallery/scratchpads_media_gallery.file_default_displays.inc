<?php

/**
 * @file
 * scratchpads_media_gallery_styles.file_default_displays.inc
 */
/**
 * Implements hook_file_default_displays().
 * 
 * Here we export the two essential displays (colorbox, and square_thumbnail)
 * for each file type (image, video, audio). 
 */
function scratchpads_media_gallery_file_default_displays(){
  $export = array();
  // Image --------------------------------------------------------------------
  // thumbnail
  $file_display = new stdClass();
  $file_display->api_version = 1;
  $file_display->name = 'image__file_styles_square_thumbnail__file_field_styles_file_square_thumbnail';
  $file_display->weight = -100;
  $file_display->status = TRUE;
  $file_display->settings = '';
  $export['image__file_styles_square_thumbnail__file_field_styles_file_square_thumbnail'] = $file_display;
  // colorbox
  $file_display = new stdClass();
  $file_display->api_version = 1;
  $file_display->name = 'image__file_styles_colorbox__file_field_styles_file_large';
  $file_display->weight = -100;
  $file_display->status = TRUE;
  $file_display->settings = '';
  $export[$file_display->name] = $file_display;
  // Video ---------------------------------------------------------------------
  // thumbnail
  $file_display = new stdClass();
  $file_display->api_version = 1;
  $file_display->name = 'video__file_styles_square_thumbnail__file_field_mediaelement_link_video';
  $file_display->weight = -100;
  $file_display->status = TRUE;
  $file_display->settings = array(
    'width' => 85,
    'height' => 85
  );
  $export[$file_display->name] = $file_display;
  // colorbox
  $file_display = new stdClass();
  $file_display->api_version = 1;
  $file_display->name = 'video__file_styles_colorbox__file_field_mediaelement_video';
  $file_display->weight = -100;
  $file_display->status = TRUE;
  $file_display->settings = array(
    'controls' => 1,
    'download_link' => 1,
    'download_text' => t('Download'),
    'width' => 640,
    'height' => 385
  );
  $export[$file_display->name] = $file_display;
  // Audio ---------------------------------------------------------------------
  // thumbnail
  $file_display = new stdClass();
  $file_display->api_version = 1;
  $file_display->name = 'audio__file_styles_square_thumbnail__file_field_mediaelement_link_audio';
  $file_display->weight = -100;
  $file_display->status = TRUE;
  $file_display->settings = array(
    'width' => 85,
    'height' => 85
  );
  $export[$file_display->name] = $file_display;
  // colorbox
  $file_display = new stdClass();
  $file_display->api_version = 1;
  $file_display->name = 'audio__file_styles_colorbox__file_field_mediaelement_audio';
  $file_display->weight = -100;
  $file_display->status = TRUE;
  $file_display->settings = array(
    'controls' => 1,
    'download_link' => 1,
    'download_text' => t('Download'),
    'width' => 640,
    'height' => 385
  );
  $export[$file_display->name] = $file_display;
  return $export;
}
