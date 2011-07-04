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
  $has_children = count(taxonomy_get_children($variables['term']->tid));
  $output = '<li>';
  if($has_children){
    // Plus or minus.
    if(in_array($variables['term']->tid, $variables['open_tids'])){
      // Minus
      $output .= theme('image', array(
        'path' => file_create_url(drupal_get_path('module', 'tinytax') . '/images/minus.gif'),
        'alt' => t('Close'),
        'title' => t('Close'),
        'attributes' => array(
          'class' => 'click'
        )
      ));
    }else{
      // Plus
      $output .= theme('image', array(
        'path' => file_create_url(drupal_get_path('module', 'tinytax') . '/images/plus.gif'),
        'alt' => t('Open'),
        'title' => t('Open'),
        'attributes' => array(
          'class' => 'click'
        )
      ));
    }
  }else{
    // Leaf
    $output .= theme('image', array(
      'path' => file_create_url(drupal_get_path('module', 'tinytax') . '/images/leaf.gif')
    ));
  }
  $output .= $variables['term']->name;
  if(in_array($variables['term']->tid, $variables['open_tids']) && $has_children){ // Check here for open!
    $output .= theme('tinytax_branch', array(
      'open_tids' => $variables['open_tids'],
      'tid' => $variables['term']->tid,
      'vid' => $variables['term']->vid
    ));
  }
  return $output . '</li>';
}