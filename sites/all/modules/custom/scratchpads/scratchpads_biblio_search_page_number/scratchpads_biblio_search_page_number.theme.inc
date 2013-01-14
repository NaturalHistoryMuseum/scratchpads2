<?php

/**
 * Theme function for a Scratchpads search page.
 */
function theme_scratchpads_biblio_search_page_number($variables){
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
        isset($biblio_node->biblio_section) ? $biblio_node->biblio_section : '',
        biblio_format_authors($biblio_node->biblio_contributors),
        $biblio_node->biblio_year,
        l(scratchpads_tweaks_translate_after_check_plain(check_plain($biblio_node->title)), 'node/' . $biblio_node->nid, array(
          'html' => TRUE
        ))
      );
    }
  }
  if($reset_author_links){
    variable_set('biblio_author_links', $reset_author_links);
  }
  return theme('table', array(
    'header' => array(
      array(
        'data' => t('Page'),
        'field' => '',
        'sort' => 'asc'
      ),
      array(
        'data' => t('Authors'),
        'field' => '',
        'sort' => ''
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