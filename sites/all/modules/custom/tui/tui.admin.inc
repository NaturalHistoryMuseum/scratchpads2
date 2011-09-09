<?php

/**
 * @file
 */
/**
 * tui_form_hierarchy - Function to do stuff!
 */
function tui_hierarchy_page($vocabulary, $tids = FALSE, $highlight_tids = FALSE){
  $tids = _tui_get_tids_from_string($vocabulary, $highlight_tids ? $highlight_tids . "," . $tids : $tids);
  $highlight_tids = _tui_get_tids_from_string($vocabulary, $highlight_tids, FALSE);
  return theme('tui_page', array(
    'vocabulary' => $vocabulary,
    'tids' => $tids,
    'highlight_tids' => $highlight_tids
  ));
}

/**
 * Get array of tids from comma delimited string
 */
function _tui_get_tids_from_string($vocabulary, $string, $parents = TRUE){
  $tids = array();
  if($string){
    // Sanity check to ensure the tids are from the vocabulary.
    $tids = explode(",", $string);
    $results = db_select('taxonomy_term_data', 't')->fields('t', array(
      'tid'
    ))->condition('tid', $tids, 'IN')->condition('vid', $vocabulary->vid)->execute();
    $tids = array();
    foreach($results as $row){
      if($parents){
        // Get the parents of each term - that then gives us every term that
        // needs to be visible.
        $parents = taxonomy_get_parents_all($row->tid);
        // Unset the first one, as that doesn't actually need to be opened (by
        // opening the parent, we can see the child).
        unset($parents[0]);
        foreach($parents as $parent){
          $tids[] = $parent->tid;
        }
      }else{
        $tids[] = $row->tid;
      }
    }
  }
  return $tids;
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
    'vocabulary' => $variables['vocabulary'],
    'tids' => $variables['tids'],
    'highlight_tids' => $variables['highlight_tids']
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
    'vocabulary' => $vocabulary,
    'tid' => $term->tid
  )));
  exit();
}

/**
 * AJAX Callback to get a term form
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
  $terms = taxonomy_get_tree($variables['vocabulary']->vid, $variables['tid'], 1, TRUE);
  // Get the depth
  $depth = count(taxonomy_get_parents_all($variables['tid']));
  $output = '<ol>';
  foreach($terms as $term){
    $output .= theme('tui_term', array(
      'vocabulary' => $variables['vocabulary'],
      'term' => $term,
      'tids' => $variables['tids'],
      'highlight_tids' => $variables['highlight_tids']
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
  $data .= ' data-tui-this-term="' . $variables['term']->tid . '"';
  // Calculate class based on children
  $class = 'tui-no-children';
  if(count(taxonomy_get_children($variables['term']->tid, $variables['term']->vid))){
    if(in_array($variables['term']->tid, $variables['tids'])){
      $class = 'tui-has-children tui-open';
    } else {
      $class = 'tui-has-children tui-closed tui-never-opened';
    }
  }
  $vocabulary = taxonomy_vocabulary_load($variables['term']->vid);
  return '<li' . $data . ' class="' . $class . '"><div' . (in_array($variables['term']->tid, $variables['highlight_tids']) ? ' class="tui-highlight"' : '') . '><span>&nbsp;</span>' . check_plain($variables['term']->name) . l('edit', 'admin/structure/taxonomy/' . $vocabulary->machine_name . '/hierarchy/form_callback/' . $variables['term']->tid, array(
    'attributes' => array(
      'class' => 'edit-term-link use-ajax',
      'data-tui-this-term' => $variables['term']->tid
    )
  )) . '</div>' . (in_array($variables['term']->tid, $variables['tids']) ? theme('tui_branch', array(
    'vocabulary' => $variables['vocabulary'],
    'tids' => $variables['tids'],
    'tid' => $variables['term']->tid,
    'highlight_tids' => $variables['highlight_tids']
  )) : '') . '</li>';
}