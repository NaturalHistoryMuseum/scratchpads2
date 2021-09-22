<?php

/**
 * Theme for the TUI page
 */
function theme_tui_page($variables){
  if(is_null($variables['vocabulary'])){return array(
      '#markup' => ''
    );}
  return array(
    '#attached' => array(
      'js' => array(
        array(
          'data' => array(
            'tui' => array(
              'callback' => url('admin/structure/taxonomy/' . $variables['vocabulary']->machine_name . '/hierarchy/callback'),
              'form_callback' => url('admin/structure/taxonomy/' . $variables['vocabulary']->machine_name . '/hierarchy/form_callback'),
              'sort_callback' => url('tui/sort')
            )
          ),
          'type' => 'setting'
        ),
        array(
          'data' => drupal_get_path('module', 'tui') . '/js/tui.js'
        )
      ),
      'css' => array(
        drupal_get_path('module', 'tui') . '/css/tui.css'
      ),
      'library' => array(
        array(
          'tui',
          'ui.nestedSortable'
        ),
        array(
          'system',
          'drupal.ajax'
        ),
        array(
          'system',
          'ui.resizable'
        ),
        array(
          'system',
          'ui.dialog'
        )
      )
    ),
    '#markup' => '<div id="tui">
	<div id="tui-tree">
	  <div id="tui-tree-container">
      <div id="tui-tree-links">
        <p>
          ' . l('<img id="tui-add" src="' . base_path() . drupal_get_path('module', 'tui') . '/css/images/add.png" alt="' . t('Add term') . '" title="' . t('Add term') . '"/>', 'admin/structure/taxonomy/' . $variables['vocabulary']->machine_name . '/hierarchy/form_callback/add-form/', array(
      'html' => TRUE,
      'attributes' => array(
        'class' => 'use-ajax',
        'id' => 'tui-add-link'
      )
    )) . '
          <img id="tui-delete" data-url="' . url('admin/structure/taxonomy/' . $variables['vocabulary']->machine_name . '/hierarchy/form_callback/') . 'delete-form/" src="' . base_path() . drupal_get_path('module', 'tui') . '/css/images/delete.png" alt="' . t('Delete term') . '" title="' . t('Delete term') . '"/>
          <!-- <img id="tui-undo" src="' . base_path() . drupal_get_path('module', 'tui') . '/css/images/undo.png" alt="' . t('Undo move') . '" title="' . t('Undo move') . '"/> -->
          ' . (module_exists('silver-FIXME') ? l('<img id="tui-import" src="' . base_path() . drupal_get_path('module', 'tui') . '/css/images/import.png" alt="' . t('Import') . '" title="' . t('Import') . '"/>', 'i', array(
      'html' => TRUE,
      'attributes' => array(
        'target' => '_blank'
      )
    )) : '') . '
          <!-- <img id="tui-link" src="' . base_path() . drupal_get_path('module', 'tui') . '/css/images/link.png" alt="' . t('Link to this page') . '" title="' . t('Link to this page') . '"/> -->
          <img id="tui-reset" data-url="' . url('admin/structure/taxonomy/' . $variables['vocabulary']->machine_name . '/hierarchy/form_callback/') . 'reset-form/' . $variables['vocabulary']->vid . '" src="' . base_path() . drupal_get_path('module', 'tui') . '/css/images/reset.png" alt="' . t('Reset sort') . '" title="' . t('Reset the sorting of all terms to alphabetical') . '"/>
          <img id="tui-search" data-url="' . url('admin/structure/taxonomy/' . $variables['vocabulary']->machine_name . '/hierarchy/form_callback/') . 'search-form/' . $variables['vocabulary']->vid . '" src="' . base_path() . drupal_get_path('module', 'tui') . '/css/images/search.png" alt="' . t('Search') . '" title="' . t('Search') . '"/>
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
  	<div id="tui-name-live"><h2>&nbsp;</h2></div>
  	<div id="tui-form-container">
      <div id="tui-form-noform">' . tui_help('admin/structure/taxonomy/%', array(
      3 => $variables['vocabulary']->machine_name
    )) . '</div>
  	</div>
  </div>
  <div style="clear:both;">&nbsp;</div>
</div>'
  );
}

/**
 * tinytax_branch theme function
 */
function theme_tui_branch($variables){
  if(is_null($variables['vocabulary'])){
    if(!$variables['tid']){
      return '';
    }else{
      $term = taxonomy_term_load($variables['tid']);
      if($term){
        $variables['vocabulary'] = taxonomy_vocabulary_load($term->vid);
      }else{
        return '';
      }
    }
  }
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
  if(is_null($variables['term'])){return '';}
  $parents = taxonomy_get_parents($variables['term']->tid);
  $data = '';
  foreach($parents as $parent){
    $data .= ' data-tui-child-of="' . $parent->tid . '"';
  }
  if(!is_array($variables['tids'])){
    $variables['tids'] = array();
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
  return '<li' . $data . ' class="' . $class . '">' . theme('tui_term_name', array(
    'term' => $variables['term'],
    'highlight_tids' => $variables['highlight_tids'],
    'vocabulary' => $vocabulary
  )) . (in_array($variables['term']->tid, $variables['tids']) ? theme('tui_branch', array(
    'vocabulary' => $variables['vocabulary'],
    'tids' => $variables['tids'],
    'tid' => $variables['term']->tid,
    'highlight_tids' => $variables['highlight_tids']
  )) : '') . '</li>';
}

/**
 * Theme just the name (required for updating the tree).
 */
function theme_tui_term_name($variables){
  if(!is_array($variables['highlight_tids'])){
    $variables['highlight_tids'] = array();
  }
  if(is_object($variables['term'])){
    $query = array();
    if(isset($_GET['destination'])){
      $query = array(
        'destination' => $_GET['destination']
      );
    }
    return '<div' . (in_array($variables['term']->tid, $variables['highlight_tids']) ? ' class="tui-highlight"' : '') . '><span>&nbsp;</span>' . check_plain($variables['term']->name) . l('edit', 'admin/structure/taxonomy/' . $variables['vocabulary']->machine_name . '/hierarchy/form_callback/edit-form/' . $variables['term']->tid, array(
      'attributes' => array(
        'class' => 'edit-term-link use-ajax',
        'data-tui-this-term' => $variables['term']->tid
      ),
      'query' => $query
    )) . ' ' . l('view', 'taxonomy/term/' . $variables['term']->tid, array(
      'attributes' => array(
        'class' => 'edit-term-link view'
      )
    )) . '</div>';
  }
}