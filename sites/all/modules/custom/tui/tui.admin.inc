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
  drupal_add_css(drupal_get_path('module', 'tui') . '/css/tui.css');
  drupal_add_js(drupal_get_path('module', 'tui') . '/js/tui.js');
  drupal_add_js(array(
    'tui' => array(
      'callback' => url('admin/structure/taxonomy/' . $variables['vocabulary']->machine_name . '/hierarchy/callback'),
      'form_callback' => url('admin/structure/taxonomy/' . $variables['vocabulary']->machine_name . '/hierarchy/form_callback')
    )
  ), 'setting');
  drupal_add_library('tui', 'ui.nestedSortable');
  drupal_add_library('system', 'drupal.ajax');
  return '
<div id="tui">
	<div id="tui-tree">
		<div id="tui-tree-container">' . theme('tui_branch', array(
    'vid' => $variables['vocabulary']->vid
  )) . '
  	</div>
  </div>
  <div id="tui-form">
  	<div id="tui-form-container">
      <div id="tui-form-noform">' . tui_help('admin/content/taxonomy/edit/tui', array()) . '</div>
  	</div>
  	<h2 id="tui-name-editing">Name being edited</h2>
  </div>
  <div style="clear:both;">&nbsp;</div>
</div>';
}

/**
 * JSON Callback to get branches
 */
function tui_hierarchy_page_callback($vocabulary, $term){
  echo json_encode(theme('tui_branch', array(
    'vid' => $vocabulary->vid,
    'tid' => $term->tid
  )));
  exit();
}

/**
 * JSON Callback to get a term form
 */
function tui_form_callback($vocabulary, $term){
  module_load_include('admin.inc', 'taxonomy');
  print ajax_render(array(
    ajax_command_html('#tui-form-container', drupal_render(drupal_get_form('taxonomy_form_term', $term)))
  ));
  exit();
}

/**
 * tinytax_branch theme function
 */
function theme_tui_branch($variables){
  // Get the terms to render
  $terms = taxonomy_get_tree($variables['vid'], $variables['tid'], 1, TRUE);
  // Get the depth
  $depth = count(taxonomy_get_parents_all($variables['tid']));
  $output = '<ol>';
  foreach($terms as $term){
    $output .= theme('tui_term', array(
      'term' => $term,
      'depth' => $depth
    ));
  }
  return $output . '</ol>';
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
  $data .= ' data-tui-this-term="' . $variables['term']->tid . '" data-tui-depth="' . $variables['depth'] . '"';
  // Calculate class based on children
  $class = 'tui-no-children';
  if(count(taxonomy_get_children($variables['term']->tid, $variables['term']->vid))){
    $class = 'tui-has-children tui-closed tui-never-opened';
  }
  $vocabulary = taxonomy_vocabulary_load($variables['term']->vid);
  return '<li' . $data . ' class="' . $class . '"><div><span>&nbsp;</span>' . check_plain($variables['term']->name) . l('edit', 'admin/structure/taxonomy/' . $vocabulary->machine_name . '/hierarchy/form_callback/' . $variables['term']->tid, array(
    'attributes' => array(
      'class' => 'edit-term-link use-ajax',
      'data-tui-this-term' => $variables['term']->tid
    )
  )) . '</li>';
}