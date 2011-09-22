<?php

/**
 * Theme for the TUI page
 */
function theme_tui_page($variables){
  drupal_add_css(drupal_get_path('module', 'tui') . '/css/tui.css');
  drupal_add_js(drupal_get_path('module', 'tui') . '/js/tui.js');
  drupal_add_js(array(
    'tui' => array(
      'callback' => url('admin/structure/taxonomy/' . $variables['vocabulary']->machine_name . '/hierarchy/callback'),
      'form_callback' => url('admin/structure/taxonomy/' . $variables['vocabulary']->machine_name . '/hierarchy/form_callback'),
      'sort_callback' => url('tui/sort')
    )
  ), 'setting');
  drupal_add_library('tui', 'ui.nestedSortable');
  drupal_add_library('system', 'drupal.ajax');
  drupal_add_library('system', 'ui.resizable');
  drupal_add_library('system', 'ui.dialog');
  $form_url = url('admin/structure/taxonomy/' . $variables['vocabulary']->machine_name . '/hierarchy/form_callback/');
  return '
<div id="tui">
	<div id="tui-tree">
	  <div id="tui-tree-container">
      <div id="tui-tree-links">
        <p>
          <img id="tui-add" src="' . base_path() . drupal_get_path('module', 'tui') . '/css/images/add.png" alt="' . t('Add term') . '" title="' . t('Add term') . '"/>
          <img id="tui-delete" data-url="' . $form_url . 'delete-form/" src="' . base_path() . drupal_get_path('module', 'tui') . '/css/images/delete.png" alt="' . t('Delete term') . '" title="' . t('Delete term') . '"/>
          <!-- <img id="tui-undo" src="' . base_path() . drupal_get_path('module', 'tui') . '/css/images/undo.png" alt="' . t('Undo move') . '" title="' . t('Undo move') . '"/> -->
          ' . (module_exists('silver-FIXME') ? l('<img id="tui-import" src="' . base_path() . drupal_get_path('module', 'tui') . '/css/images/import.png" alt="' . t('Import') . '" title="' . t('Import') . '"/>', 'i', array(
    'html' => TRUE,
    'attributes' => array(
      'target' => '_blank'
    )
  )) : '') . '
          <!-- <img id="tui-link" src="' . base_path() . drupal_get_path('module', 'tui') . '/css/images/link.png" alt="' . t('Link to this page') . '" title="' . t('Link to this page') . '"/> -->
          <img id="tui-search" data-url="' . $form_url . 'search-form/' . $variables['vocabulary']->vid . '" src="' . base_path() . drupal_get_path('module', 'tui') . '/css/images/search.png" alt="' . t('Search') . '" title="' . t('Search') . '"/>
          <a id="tui-click"></a>
        </p>
      </div>
      <div id="tui-tree-form">
      </div>
			<div id="tui-tree-subcontainer">' . theme('tui_branch', array(
    'vocabulary' => $variables['vocabulary'],
    'tids' => $variables['tids'],
    'highlight_tids' => $variables['highlight_tids']
  )) . '
  	</div></div>
  </div>
  <div id="tui-form">
  	<div id="tui-form-container">
      <div id="tui-form-noform">' . tui_help('admin/content/taxonomy/edit/tui', array()) . '</div>
  	</div>
  </div>
  <div style="clear:both;">&nbsp;</div>
</div>';
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
    }else{
      $class = 'tui-has-children tui-closed tui-never-opened';
    }
  }
  $vocabulary = taxonomy_vocabulary_load($variables['term']->vid);
  return '<li' . $data . ' class="' . $class . '"><div' . (in_array($variables['term']->tid, $variables['highlight_tids']) ? ' class="tui-highlight"' : '') . '><span>&nbsp;</span>' . check_plain($variables['term']->name) . l('edit', 'admin/structure/taxonomy/' . $vocabulary->machine_name . '/hierarchy/form_callback/edit-form/' . $variables['term']->tid, array(
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