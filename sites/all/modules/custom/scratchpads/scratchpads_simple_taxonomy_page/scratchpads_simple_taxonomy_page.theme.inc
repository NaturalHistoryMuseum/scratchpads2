<?php

/**
 * 
 */
function theme_scratchpads_simple_taxonomy_page($variables){
  $entity_ids = array();
  $content_array = array();
  foreach($variables['results'] as $result){
    $entities = entity_load($result['entity_type'], array($result['fields']['entity_id']));
    $content_array[] = entity_view($result['entity_type'], $entities, 'teaser');
  }
  return print_r($content_array, 1);
  return drupal_render($content_array);
}

/**
 * 
 */
function theme_scratchpads_simple_taxonomy_page_empty(){
  return t('<p>Either no media has been !uploaded to this site, or it has not yet been indexed. Indexing can take up to one hour, so please check back later.</p>', array(
    '!uploaded' => user_access('create media_gallery content') ? l(t('uploaded'), 'node/add/media-gallery') : t('uploaded')
  ));
}