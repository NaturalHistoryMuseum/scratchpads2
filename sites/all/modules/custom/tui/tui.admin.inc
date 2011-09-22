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
 * Sort stuff
 */
function tui_sort_callback(){
  $results = array();
  if(isset($_POST['parent_change'])){
    $term = taxonomy_term_load($_POST['parent_change']['tid']);
    if($term){
      $term->parent = isset($_POST['parent_change']['parent']) ? $_POST['parent_change']['parent'] : 0;
      taxonomy_term_save($term);
    } else {
      $results[] = ajax_command_html('#tui-tree-form', '<div class="messages error"><h2 class="element-invisible">Error message</h2>'.t('There has been an error.  Please reload this page.').'</div>');
      watchdog('femail', 'Attempted to move a nonexistent term');
    }
  }
  if(isset($_POST['sort_change'])){
    $tids = explode(",", $_POST['sort_change']['tids']);
    $weight = 0;
    foreach($tids as $tid){
      $term = taxonomy_term_load($tid);
      if($term){
        $term->weight = $weight;
        taxonomy_term_save($term);
        $weight++;
      } else {
        $results[] = ajax_command_html('#tui-tree-form', '<div class="messages error"><h2 class="element-invisible">Error message</h2>'.t('One of the terms being sorted appears to have been deleted.  Please reload the page.').'</div>');
        watchdog('femail', 'Attempted to move a nonexistent term');        
      }
    }
  }
  print ajax_render($results);
  exit();
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
 * Autocomplete callback
 */
/**
 * Helper function for autocompletion
 */
function tui_autocomplete($vid, $search = ''){
  $query = db_select('taxonomy_term_data', 't');
  $query->addTag('translatable');
  $query->addTag('term_access');
  // Select rows that match by term name.
  $tags_return = $query->fields('t', array(
    'tid',
    'name'
  ))->condition('t.vid', $vid)->condition('t.name', db_like($search) . '%', 'LIKE')->range(0, 10)->execute()->fetchAllKeyed();
  $term_matches = array();
  foreach($tags_return as $tid => $name){
    $n = $name;
    // Term names containing commas or quotes must be wrapped in quotes.
    if(strpos($name, ',') !== FALSE || strpos($name, '"') !== FALSE){
      $n = '"' . str_replace('"', '""', $name) . '"';
    }
    $term_matches[$n] = check_plain($name);
  }
  drupal_json_output($term_matches);
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
function tui_form_callback($vocabulary, $action, $tids = ''){
  //sleep(100);
  switch($action){
    case 'edit-form':
      // We can only show one form
      $tids = explode(',', $tids);
      $term = taxonomy_term_load($tids[0]);
      if($term){
        module_load_include('admin.inc', 'taxonomy');
        print ajax_render(array(
          ajax_command_html('#tui-form-container', drupal_render(drupal_get_form('taxonomy_form_term', $term)))
        ));
      }else{
        print ajax_render(array(
          ajax_command_html('#tui-tree-form', '<div class="messages error"><h2 class="element-invisible">Error message</h2>'.t('You are trying to edit a term that has been deleted.  Please reload this page.').'</div>'),
          ajax_command_invoke('#tui-tree-form', 'slideDown')
        ));
      }
      exit();
    case 'delete-form':
      print ajax_render(array(
        ajax_command_html('#tui-tree-form', drupal_render(drupal_get_form('tui_taxonomy_form_term_delete', $tids))),
        ajax_command_invoke('#tui-tree-form', 'show')
      ));
      exit();
    case 'search-form':
      print ajax_render(array(
        ajax_command_html('#tui-tree-form', drupal_render(drupal_get_form('tui_search_form', $tids))),
        ajax_command_invoke('#tui-tree-form', 'show')
      ));
      exit();
    case 'add-form':
      module_load_include('admin.inc', 'taxonomy');
      print ajax_render(array(
        ajax_command_html('#tui-form-container', drupal_render(drupal_get_form('taxonomy_form_term', array(), $vocabulary))),
      ));
      exit();
      
  }
}

/**
 * TUI autocomplete search form
 */
function tui_search_form($form, &$form_state, $vid){
  $ajax = array(
    'path' => 'tui/ajax',
    'callback' => 'tui_ajax_blank',
    'wrapper' => 'tui-tree-form',
    'effect' => 'slide'
  );
  return array(
    'tui-search-input' => array(
      '#autocomplete_path' => 'tui/autocomplete/' . $vid,
      '#size' => FALSE,
      '#type' => 'textfield',
      '#title' => ''
    ),
    'tui-vid' => array(
      '#value' => $vid,
      '#type' => 'value'
    ),
    'tui-search-input-button' => array(
      '#type' => 'submit',
      '#value' => 'Search',
      '#ajax' => $ajax
    ),
    'tui-cancel-input-button' => array(
      '#type' => 'button',
      '#value' => 'Cancel',
      '#ajax' => $ajax
    )
  );
}

/**
 * search submit
 */
function tui_search_form_submit(&$form, &$form_state){
  $results = db_select('taxonomy_term_data', 't')->fields('t', array(
    'tid'
  ))->condition('t.name', db_like($form_state['values']['tui-search-input']), 'LIKE')->execute();
  $highlight_tids = array();
  $open_tids = array();
  foreach($results as $row){
    $parents = taxonomy_get_parents_all($row->tid);
    array_shift($parents);
    foreach($parents as $parent){
      $open_tids[] = $parent->tid;
    }
    $highlight_tids[] = $row->tid;
  }
  $html = theme('tui_branch', array(
    'vocabulary' => taxonomy_vocabulary_load($form_state['values']['tui-vid']),
    'tids' => $open_tids,
    'highlight_tids' => $highlight_tids
  ));
  //$('#tui-tree-subcontainer').animate({scrollTop:$(element).position().top+$('#tui-tree-subcontainer').scrollTop()-50}, 500);
  $form_state['tui-ajax'] = array(
    ajax_command_html('#tui-tree-subcontainer', $html),
    ajax_command_invoke('#tui-tree', 'resize')
  );
}

/**
 * tui_taxonomy_form_term_delete
 */
function tui_taxonomy_form_term_delete($form, $form_state, $tids = ''){
  $tids = explode(",", $tids);
  $terms = taxonomy_term_load_multiple($tids);
  $term_names = array();
  $tids = array();
  foreach($terms as $term){
    $term_names[] = $term->name;
    $tids[] = $term->tid;
  }
  $ajax = array(
    'path' => 'tui/ajax',
    'callback' => 'tui_ajax_blank',
    'wrapper' => 'tui-tree-form',
    'effect' => 'slide'
  );
  return array(
    '#ajax' => $ajax,
    'you_sure' => array(
      '#markup' => t('Are you sure you would like to delete the highlighted terms and their children?') . theme('item_list', array(
        'items' => $term_names
      ))
    ),
    'tids' => array(
      '#type' => 'value',
      '#value' => $tids
    ),
    'cancel' => array(
      '#type' => 'button',
      '#title' => t('Cancel'),
      '#value' => t('Cancel'),
      '#ajax' => $ajax
    ),
    'submit' => array(
      '#type' => 'submit',
      '#title' => t('Delete'),
      '#value' => t('Delete'),
      '#ajax' => $ajax
    )
  );
}

/**
 * tui_taxonomy_form_term_delete_submit()
 */
function tui_taxonomy_form_term_delete_submit(&$form, &$form_state){
  // We have passed validation, lets delete the terms.
  foreach($form_state['values']['tids'] as $tid){
    taxonomy_term_delete($tid);
  }
  $form_state['tui-ajax'] = array(
    ajax_command_remove('.tui-highlight')
  );
}