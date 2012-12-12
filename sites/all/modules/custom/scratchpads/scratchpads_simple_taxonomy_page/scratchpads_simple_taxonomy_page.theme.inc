<?php

/**
 * 
 */
function theme_scratchpads_simple_taxonomy_page($variables){
  $entity_ids = array();
  $content_array = array();
  $term = taxonomy_term_load(arg(2));
  if($term){
    $content = entity_view('taxonomy_term', array(
      $term
    ));
    $content_array[] = $content;
  }
  foreach($variables['results'] as $result){
    $entities = entity_load($result['entity_type'], array(
      $result['fields']['entity_id']
    ));
    $content = entity_view($result['entity_type'], $entities, 'teaser');
    $class = 'even';
    if(count($content_array) % 2){
      $class = 'odd';
    }
    $content['#prefix'] = '<div class="search-row-' . $class . '">';
    $content['#suffix'] = '</div>';
    $content_array[] = $content;
  }
  $content_array[] = array(
    '#theme' => 'pager',
    '#tags' => NULL
  );
  return drupal_render($content_array);
}

/**
 * 
 */
function theme_scratchpads_simple_taxonomy_page_empty(){
  $content_array = array();
  $term = taxonomy_term_load(arg(2));
  if($term){
    $content = entity_view('taxonomy_term', array(
      $term
    ));
    $content_array[] = $content;
  }
  $content_array[] = array('#markup' => t('<p style="margin-top:50px">There is currently no additional content associated with this term.</p>'));
  return drupal_render($content_array);
}