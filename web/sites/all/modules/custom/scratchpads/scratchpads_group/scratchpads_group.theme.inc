<?php

/**
 *
 */
function theme_scratchpads_group($variables){
  $entity_ids = array();
  $content_array = array();
  $group_node = FALSE;
  if(isset($_GET['f'])){
    foreach($_GET['f'] as $facet){
      $facet = explode(':', $facet);
      if($facet[0] == 'im_group_audience'){
        $group_node = og_load($facet[1]);
        $group_node = node_load($group_node->etid);
      }
    }
  }
  if($group_node){
    $content = entity_view('node', array(
      $group_node
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
 * Theme function for an empty results page.
 */
function theme_scratchpads_group_empty($variables){
  return t('<p>Either no content has been added to this group, or it has not yet been indexed. Indexing can take up to one hour, so please check back later.</p>');
}
