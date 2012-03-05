<?php

/**
 * Theme function for a Scratchpads search page.
 */
function theme_scratchpads_biblio_search($variables){
  module_load_include('inc', 'biblio', 'includes/biblio_theme');
  $reset_author_links = variable_get('biblio_author_links', 1);
  if($reset_author_links){
    variable_set('biblio_author_links', 0);
  }
  $rows = array();
  foreach($variables['results'] as $result){
    $biblio_node = node_load($result['node']->entity_id);
    if($biblio_node){
      $rows[] = array(
        biblio_format_authors($biblio_node->biblio_contributors),
        $biblio_node->biblio_year,
        l($biblio_node->title, 'node/' . $biblio_node->nid)
      );
    }
  }
  if($reset_author_links){
    variable_set('biblio_author_links', $reset_author_links);
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