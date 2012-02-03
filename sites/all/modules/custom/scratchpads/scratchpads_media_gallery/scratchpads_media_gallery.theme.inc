<?php

/**
 * Theme function for a Scratchpads search page.
 */
function theme_scratchpads_media_gallery($variables){
  if($nid = scratchpads_media_gallery_is_gallery_node()){
    $gallery_node = node_load($nid);
    // Try to set the body text
    $body_array = field_view_field('node', $gallery_node, 'body');
    $body_array['#label_display'] = 'hidden';
    $body_text = drupal_render($body_array);
    // Specifics for the omega theme.
    try{
      $alpha_get_theme = drupal_static('alpha_get_theme');
      drupal_set_title($gallery_node->title);
      $alpha_get_theme['scratchpads']->page['title'] = drupal_set_title();
      $alpha_get_theme['scratchpads']->page['breadcrumb'] = theme('breadcrumb', array(
        'breadcrumb' => drupal_get_breadcrumb()
      ));
    }
    catch(Exception $e){
      ; // Do nothing, we're probably not using the Omega/Scratchpads theme.
    }
  }
  $entity_ids = array();
  foreach($variables['results'] as $result){
    $entity_ids[] = $result['fields']['entity_id'];
  }
  $media_entities = entity_load('file', $entity_ids);
  $content_array = entity_view('file', $media_entities, 'file_styles_square_thumbnail');
  foreach($content_array as $key => $array){
    if(is_numeric($key) && is_array($array)){
      if(!isset($array['file']['#file'])){
        $content_array[$key]['file']['#file'] = $media_entities[$key];
      }
    }
  }
  // Add the body text if we have it
  if(isset($body_text)){
    $content_array = array_merge(array(
      'body_text' => array(
        '#markup' => $body_text
      )
    ), $content_array);
  }
  return drupal_render($content_array);
}