<?php

/**
 * Implements hook_file_default_displays().
 */
function scratchpads_file_colorboxed_file_default_displays(){
  $export = array();
  return $export;
  $file_display = new stdClass();
  $file_display->api_version = 1;
  $file_display->name = 'audio__file_styles_colorbox__file_field_styles_file_colorbox';
  $file_display->weight = 0;
  $file_display->status = TRUE;
  $file_display->settings = '';
  $export['audio__file_styles_colorbox__file_field_styles_file_colorbox'] = $file_display;
  $file_display = new stdClass();
  $file_display->api_version = 1;
  $file_display->name = 'image__file_styles_colorbox__file_field_styles_file_large';
  $file_display->weight = 0;
  $file_display->status = TRUE;
  $file_display->settings = '';
  $export['image__file_styles_colorbox__file_field_styles_file_large'] = $file_display;
  $file_display = new stdClass();
  $file_display->api_version = 1;
  $file_display->name = 'video__file_styles_large__file_field_styles_file_large';
  $file_display->weight = 0;
  $file_display->status = TRUE;
  $file_display->settings = '';
  $export['video__file_styles_large__file_field_styles_file_large'] = $file_display;
  return $export;
}