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
  $data = unserialize($variables['entity']->data);
  return '<img src="' . $data->eolThumbnailURL . '"/>';
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