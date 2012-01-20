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
  $content_array = entity_view('file', $media_entities, 'file_styles_scratchpads_media_gallery_thumbnail');
  foreach($content_array as $key => $value){
    if(is_numeric($key)){
      $content_array[$key]['#prefix'] = '<div class="grid-2">';
      $content_array[$key]['#suffix'] = '</div>';
    }
  }
  return drupal_render($content_array);
}