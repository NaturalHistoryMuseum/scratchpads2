<?php 

/**
 * Theme function for an empty results page.
 */
function theme_ecoint_serach($variables) {
  $rows = array();
  foreach($variables['results'] as $result){
    $ecoint_node = node_load($result['node']->entity_id);
    if($ecoint_node){
      print_r($ecoint_node);exit;
    }
  }
  return theme('table', array(
      'header' => array(
          array(
              'data' => t('Authors'),
              'field' => '',
              'sort' => 'asc'
          ),
          array(
              'data' => t('Year'),
              'field' => '',
              'sort' => ''
          ),
          array(
              'data' => t('Title'),
              'field' => '',
              'sort' => ''
          )
      ),
      'rows' => $rows,
      'attributes' => array(
          'class' => array(
              'biblio'
          )
      )
  )) . theme('pager', array(
      'tags' => NULL
  ));
}
 
function theme_ecoint_search_empty($variables){
  return t('<p>Either no literature content has been added to this site, or it has not yet been indexed. Indexing can take up to one hour, so please check back later.</p>');
}