<?php

/**
 * @file
 * scratchpads_media_gallery_styles.file_default_displays.inc
 */
/**
 * Implements hook_file_default_displays().
 */
function scratchpads_media_gallery_file_default_displays(){
  $export = array();
  $file_display = new stdClass();
  $file_display->api_version = 1;
  $file_display->name = 'audio__file_styles_scratchpads_media_gallery_thumbnail__file_field_styles_file_scratchpads_media_gallery_thumbnail';
  $file_display->weight = 0;
  $file_display->status = TRUE;
  $file_display->settings = '';
  $export['audio__file_styles_scratchpads_media_gallery_thumbnail__file_field_styles_file_scratchpads_media_gallery_thumbnail'] = $file_display;
  $file_display = new stdClass();
  $file_display->api_version = 1;
  $file_display->name = 'image__file_styles_scratchpads_media_gallery_thumbnail__file_field_styles_file_scratchpads_media_gallery_thumbnail';
  $file_display->weight = 0;
  $file_display->status = TRUE;
  $file_display->settings = '';
  $export['image__file_styles_scratchpads_media_gallery_thumbnail__file_field_styles_file_scratchpads_media_gallery_thumbnail'] = $file_display;
  $file_display = new stdClass();
  $file_display->api_version = 1;
  $file_display->name = 'video__file_styles_scratchpads_media_gallery_thumbnail__file_field_styles_file_scratchpads_media_gallery_thumbnail';
  $file_display->weight = 0;
  $file_display->status = TRUE;
  $file_display->settings = '';
  $export['video__file_styles_scratchpads_media_gallery_thumbnail__file_field_styles_file_scratchpads_media_gallery_thumbnail'] = $file_display;
  return $export;
}
