<?php

/**
 * @file
 * 
 * Description of the tinytax.theme.inc file
 */
/**
 * tinytax_branch theme function
 */
function theme_tinytax_branch($variables){
  // Add the CSS and JS
  drupal_add_css(drupal_get_path('module', 'tinytax') . '/css/tinytax.css');
  drupal_add_js(drupal_get_path('module', 'tinytax') . '/js/tinytax.js');
  // Get the terms to render
  $terms = taxonomy_get_tree($variables['vid'], $variables['tid'], 1, TRUE);
  $output = '<ul>';
  foreach($terms as $term){
    $output .= theme('tinytax_term', array(
      'term' => $term,
      'open_tids' => $variables['open_tids']
    ));
  }
  return $output . '</ul>';
}

/**
 * tinytax_term theme function
 */
function theme_tinytax_term($variables){
  $output = '<li>';
  $output .= $variables['term']->name;
  if(in_array($variables['term']->tid, $variables['open_tids']) && count(taxonomy_get_children($variables['term']->tid))){ // Check here for open!
    $output .= theme('tinytax_branch', array(
      'open_tids' => $variables['open_tids'],
      'tid' => $variables['term']->tid,
      'vid' => $variables['term']->vid
    ));
  }
  return $output . '</li>';
}