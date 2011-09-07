<?php

/**
 * @file
 */
/**
 * tui_form_hierarchy - Function to do stuff!
 */
function tui_hierarchy_page($vocabulary){
  return theme('tui_page', array(
    'vocabulary' => $vocabulary
  ));
}

/**
 * Theme for the TUI page
 */
function theme_tui_page($variables){
  drupal_add_css(drupal_get_path('module','tui').'/css/tui.css');
  drupal_add_js(drupal_get_path('module', 'tui') . '/js/tui.js');
  drupal_add_js(array(
    'tui' => array(
      'callback' => url('tui')
    )
  ), 'setting');
  drupal_add_library('system', 'ui.sortable');
  // We're unable to use the same functions as the Tinytax module, due to the
  // fact that the Tinytax module uses Nested lists, which simply would not
  // work with this module.  We instead need to use a flat list to allow the
  // terms to be sorted easily.
  return '<div class="tui">' . theme('tui_branch', array(
    'vid' => $variables['vocabulary']->vid
  )) . '</div>';
}

/**
 * tinytax_branch theme function
 */
function theme_tui_branch($variables){
  // Get the terms to render
  $terms = taxonomy_get_tree($variables['vid'], $variables['tid'], 1, TRUE);
  // Get the depth
  $depth = count(taxonomy_get_parents_all($variables['tid']));
  $output = '';
  foreach($terms as $term){
    $output .= theme('tui_term', array(
      'term' => $term,
      'depth' => $depth
    ));
  }
  return $output;
}

/**
 * Theme a single term, depending on its depth within the taxonomy
 */
function theme_tui_term($variables){
  $parents = taxonomy_get_parents($variables['term']->tid);
  $data = '';
  foreach($parents as $parent){
    $data .= ' data-tui-child-of="' . $parent->tid . '"';
  }
  $data .= ' data-tui-this-term="' . $variables['term']->tid . '"';
  // Calculate class based on children
  $class = 'tui-no-children';
  if(count(taxonomy_get_children($variables['term']->tid, $variables['term']->vid))){
    $class = 'tui-has-children tui-closed tui-never-opened';
  }
  return "<p$data class=\"$class\" style=\"left-margin:" . ($variables['depth'] * 20) . 'px">' . $variables['term']->name . '</p>';
}