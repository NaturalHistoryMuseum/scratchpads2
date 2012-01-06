<?php

/**
 * Still image full
 */
function theme_eolapi_stillimage_full($variables){
  $data = unserialize($variables['entity']->data);
  return '<img src="' . $data->eolThumbnailURL . '"/>';
}

/**
 * Still image teaser
 */
function theme_eolapi_stillimage_teaser($variables){
  // Download the original image so that we can perform image_styles magic on
  // it.
  $data = unserialize($variables['entity']->data);
  return theme('image_style', array(
    'style_name' => 'square_thumbnail',
    'path' => substr($variables['entity']->eolapi_image['und'][0]['uri'], 9),
    'width' => $variables['entity']->eolapi_image['und'][0]['width'],
    'height' => $variables['entity']->eolapi_image['und'][0]['height'],
    'alt' => $data->title,
    'title' => $data->title,
    'attributes' => array()
  )) . (isset($data->description) ? $data->description : '');
}

/**
 * Text full
 */
function theme_eolapi_text_full($variables){
  $data = unserialize($variables['entity']->data);
  return $data->description;
}

/**
 * Text teaser
 */
function theme_eolapi_text_teaser($variables){
  $data = unserialize($variables['entity']->data);
  return $data->description;
}

/**
 * YouTube full
 */
function theme_eolapi_movingimage_full($variables){
  $data = unserialize($variables['entity']->data);
  switch($data->mimeType){
    case 'video/x-flv':
      return '<object data="' . $data->mediaURL . '" height="420" type="application/x-shockwave-flash" width="540"></object>';
      break;
  }
}

/**
 * YouTube teaser
 */
function theme_eolapi_movingimage_teaser($variables){
  $data = unserialize($variables['entity']->data);
  switch($data->mimeType){
    case 'video/x-flv':
      return '<object data="' . $data->mediaURL . '" height="220" type="application/x-shockwave-flash" width="270"></object>';
      break;
  }
}