<?php

/**
 * Theme function for a Scratchpads search page.
 */
function theme_scratchpads_biblio_search($variables){
  module_load_include('inc', 'biblio', 'includes/biblio_theme');
  $rows = array();
  foreach($variables['results'] as $result){
    $biblio_node = node_load($result['node']->entity_id);
    if($biblio_node){
      foreach($biblio_node->biblio_contributors as $key => $contributor){
        if(empty($contributor['firstname']) && empty($contributor['lastname'])){
          $biblio_node->biblio_contributors[$key]['literal'] = TRUE;
        }
        if($contributor['auth_category'] > 1){
          unset($biblio_node->biblio_contributors[$key]);
        }
      }
      $rows[] = array(
        biblio_format_authors($biblio_node->biblio_contributors),
        $biblio_node->biblio_year,
        l(scratchpads_tweaks_translate_after_check_plain(check_plain($biblio_node->title)), 'node/' . $biblio_node->nid, array(
          'html' => TRUE
        ))
      );
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

/**
 * Theme function for an empty results page.
 */
function theme_scratchpads_biblio_search_empty($variables){
  return t('<p>Either no literature content has been added to this site, or it has not yet been indexed. Indexing can take up to one hour, so please check back later.</p>');
}