<?php

/**
 * Theme function for a Scratchpads search page.
 */
function theme_scratchpads_media_gallery($variables){
  $entity_ids = array();
  foreach($variables['results'] as $result){
    $entity_ids[] = $result['fields']['entity_id'];
  }
  $media_entities = entity_load('file', $entity_ids);
  $content_array = entity_view('file', $media_entities, 'gallery');
  return '<div class="scratchpads_media_gallery clearfix">' . drupal_render($content_array) . '</div>';
}

/**
 * Theme an image/video/audo file for a gallery.
 */
function theme_scratchpads_media_gallery_entity($variables){
  switch($variables['entity']->type){
    case 'image':
      $content = theme('image_style', array(
        'style_name' => 'sp_square_thumbnail',
        'path' => substr($variables['entity']->uri, 9),
        'alt' => '',
        'title' => '',
        'attributes' => array()
      ));
  }
  if($variables['gallery_view']){
    $content = '<div style="float:left; border:solid 1px black; margin:3px; padding:1px;">' . $content . '</div>';
  }
  return $content;
}