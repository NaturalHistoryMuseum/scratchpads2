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
  return l(theme('image_style', array(
    'style_name' => 'square_thumbnail',
    'path' => substr($variables['entity']->eolapi_image['und'][0]['uri'], 9),
    'width' => $variables['entity']->eolapi_image['und'][0]['width'],
    'height' => $variables['entity']->eolapi_image['und'][0]['height'],
    'alt' => (isset($data->title) ? $data->title : ''),
    'title' => (isset($data->title) ? $data->title : ''),
    'attributes' => array()
  )), 'file/' . $variables['entity']->eolapi_image[LANGUAGE_NONE][0]['fid'], array(
    'html' => TRUE
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
  $agents = array();
  if(isset($data->agents)){
    foreach($data->agents as $agent){
      if(isset($agent->homepage)){
        $agents[] = l($agent->full_name, $agent->homepage);
      }else{
        if(substr($agent->full_name, 0, 4) == 'http'){
          $agents[] = l($agent->full_name, $agent->full_name);
        }else{
          $agents[] = $agent->full_name;
        }
      }
    }
  }
  $rows = array(
    array(
      t('License'),
      isset($data->license) ? l($data->license, $data->license) : t('All rights reserved')
    ),
    array(
      t('Rights/Author'),
      isset($data->rights) ? $data->rights : (count($agents) ? implode(", ", $agents) : t('No rights holder'))
    ),
    array(
      t('Source'),
      isset($data->source) ? l($data->source, $data->source) : t('No source database.')
    )
  );
  return '<div class="eolapi-text">
  ' . (isset($data->title) ? '<div class="field-label">' . $data->title . ':</div>' : '') . '
  	<div class="eolapi-text-data">' . check_markup($data->description, 'filtered_html') . '
  		' . ((isset($data->references) && count($data->references)) ? theme('item_list', array(
    'items' => $data->references
  )) : '') . '
  		<p class="eolapi-text-rights">' . theme('table', array(
    'rows' => $rows
  )) . '</p>
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