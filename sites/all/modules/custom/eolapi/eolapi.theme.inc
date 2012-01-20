<?php

/**
 * Still image full
 */
function theme_eolapi_stillimage_full($variables){
  $data = unserialize($variables['entity']->data);
  return theme('image_style', array(
    'style_name' => 'large',
    'path' => substr($variables['entity']->eolapi_image['und'][0]['uri'], 9),
    'width' => $variables['entity']->eolapi_image['und'][0]['width'],
    'height' => $variables['entity']->eolapi_image['und'][0]['height'],
    'alt' => $data->title,
    'title' => $data->title,
    'attributes' => array()
  )) . (isset($data->description) ? $data->description : '');
}

/**
 * Still image teaser
 */
function theme_eolapi_stillimage_teaser($variables){
  $data = unserialize($variables['entity']->data);
  return theme('image_style', array(
    'style_name' => 'sp_square_thumbnail',
    'path' => substr($variables['entity']->eolapi_image['und'][0]['uri'], 9),
    'width' => $variables['entity']->eolapi_image['und'][0]['width'],
    'height' => $variables['entity']->eolapi_image['und'][0]['height'],
    'alt' => (isset($data->title) ? $data->title : ''),
    'title' => (isset($data->title) ? $data->title : ''),
    'attributes' => array()
  ));
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
  drupal_add_js(drupal_get_path('module', 'eolapi') . '/js/eolapi.js');
  return '<div class="eolapi-text">
  ' . (isset($data->title) ? '<h1>' . $data->title . '</h1>' : '') . '
  <div class="eolapi-text-hidden-block">
  	<div>' . check_markup($data->description, 'filtered_html') . '</div>
  	<hr/>
  	<div>
  		' . ((isset($data->references) && count($data->references)) ? theme('item_list', array(
    'items' => $data->references
  )) : '') . '
  		<p>' . (isset($data->rights) ? $data->rights : t('No rights holder')) . '<br/>' . (isset($data->source) ? l($data->source, $data->source) : t('No source database.')) . '</p>
  	</div>
  </div>  
</div>';
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
      return '<object data="' . $data->mediaURL . '" height="100" type="application/x-shockwave-flash" width="100"></object>';
      break;
  }
}