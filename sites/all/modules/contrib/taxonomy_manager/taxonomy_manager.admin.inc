<?php

/**
 * 
 * @file
 * Taxonomy Manager Admin
 * 
 * contains all forms and saving function for the Taxonomy Manager
 * 
 */
 
include_once DRUPAL_ROOT . "/modules/taxonomy/taxonomy.admin.inc";


/**
 * list of vocabularies, which link to Taxonomy Manager interface
 */
function taxonomy_manager_voc_list() {
  $output = l(t('Add new vocabulary'), 'admin/structure/taxonomy/add/vocabulary') .' | ';
  $output .= l(t('Edit vocabulary settings'), 'admin/structure/taxonomy') .'<br /><br />';
  
  $vocabularies = taxonomy_get_vocabularies();
  $voc_list = array();
  
  foreach ($vocabularies as $vocabulary) {
    $voc_list[] = l($vocabulary->name, 'admin/structure/taxonomy_manager/voc/'. $vocabulary->machine_name);
  }
  if (!count($voc_list)) {
    $voc_list[] = t('No Vocabularies available');
  }
  $output .= theme('item_list', array('items' => $voc_list, 'title' => t('Vocabularies')));
  return $output;
}


/**
 * defines forms for taxonomy manager interface
 * 
 * @param $voc vocabulary
 * @param $tid a term id, if not 0, displays term editing from for given tid on right side
 * @param $search_string a string to filter root level terms
 */
function taxonomy_manager_form($form, &$form_state, $voc, $tid = 0, $filter = NULL) {
  // Check for confirmation forms.
  if (isset($form_state['confirm_delete'])) {
    return taxonomy_manager_term_confirm_delete($form, $form_state, $voc->vid);
  }
  // By default use tid from term data form, except it is submitted from the load-tid helper and the tree needs to be refreshed.
  if (isset($form_state['values']['tid']) && !(isset($form_state['triggering_element']['#value']) && $form_state['triggering_element']['#value'] == "load term" && $form_state['values']['load-tid-refresh-tree'])) {
    $tid = $form_state['values']['tid'];
  }
  else if (isset($form_state['triggering_element']['#value']) && $form_state['triggering_element']['#value'] == "load term" && $form_state['values']['load-tid-refresh-tree']) {
    $tid = $form_state['values']['load-tid'];
  }
	   
  $module_path = drupal_get_path('module', 'taxonomy_manager') .'/';
  
  $form['#attached']['css'][] = $module_path .'css/taxonomy_manager.css';
  $form['#attached']['js'][] = $module_path .'js/hideForm.js';
  $form['#attached']['js'][] = $module_path .'js/updateWeight.js';
  $form['#attached']['js'][] = $module_path .'js/termData.js';
  
  $form['#attached']['js'][] = array('data' => array('termData' => array('url' => url("admin/structure/taxonomy_manager/termdata/edit/". $voc->vid))), 'type' => 'setting');
  $form['#attached']['js'][] = array('data' => array('updateWeight' => array('up' => 'edit-weight-up', 'down' => 'edit-weight-down', 'url' => url('admin/structure/taxonomy_manager/weight/'), 'disable_mouseover' => variable_get('taxonomy_manager_disable_mouseover', 0))), 'type' => 'setting');
  $form['#attached']['js'][] = array('data' => array('TMAjaxThrobber' => array('add' => TRUE)), 'type' => 'setting');
  $form['#attached']['js'][] = array('data' => array('taxonomy_manager' => array('modulePath' => (url($module_path) == $module_path) ? $module_path : (base_path() . $module_path))), 'type' => 'setting');
  
  drupal_set_title(t("Taxonomy Manager - %voc_name", array("%voc_name" => $voc->name)), PASS_THROUGH);

  $form['voc'] = array('#type' => 'value', "#value" => $voc);

  // Helper forms for loading the term data form.
  // When a term link in the tree view gets clicked, the term id is written into the 
  // load-tid textfield and aftwards the load-tid-submit button gets clicked, which 
  // triggers the ajax system.
  $form['load-tid'] = array(
    '#type' => 'textfield',
    '#prefix' => '<div class="taxonomy-manager-term-load-helper">',
  );
  $form['load-tid-refresh-tree'] = array(
    '#type' => 'checkbox',
  );
  $form['load-tid-submit'] = array(
    '#type' => 'submit',
    '#value' => 'load term',
    '#submit' => array('taxonomy_manager_load_tid_submit'),
    '#ajax' => array(
      'callback' => 'taxonomy_manager_term_data_form_ajax_callback',
      'method' => 'replaceWith',
      'event' => 'click',
      'wrapper' => 'taxonomy-term-data-replace',
    ),
    '#suffix' => '</div>',
  );
  
  if (_taxonomy_manager_voc_is_empty($voc->vid)) {
    $text = t('No terms available');
    $form['text'] = array( 
      '#markup' => $text,
    );
    taxonomy_manager_add_form($form, $voc, FALSE);
    return $form;
  }
  $form_state['cache'] = TRUE;
  
  $form['taxonomy']['#tree'] = TRUE;
  
  $form['taxonomy']['manager'] = array( 
    '#type' => 'fieldset',
    '#title' => check_plain($voc->name),
    '#weight' => 10,
    '#tree' => TRUE,
  );
  
  $form['taxonomy']['manager']['top'] = array(
    '#markup' => '', 
    '#prefix' => '<div class="taxonomy-manager-tree-top">', 
    '#suffix' => '</div>',
  );
  
  if (module_exists('i18ntaxonomy')) {
    if (i18ntaxonomy_vocabulary($voc->vid) == I18N_TAXONOMY_TRANSLATE) {
      if ($tid) {
        $language = _taxonomy_manager_term_get_lang($tid); 
      }
      else {
        $lang = language_default();
        $language = $lang->language;
      }
      $form['taxonomy']['manager']['top']['language'] = array(
        '#type' => 'select',
        '#title' => t('Language'),
        '#default_value' => $language,
        '#options' => array('' => t('All'), 'no language' => t('no language')) + locale_language_list('name'),
        '#attributes' => array('class' => 'language-selector'),
      );
    }
  }
 
  $form['taxonomy']['manager']['top']['size'] = array('#markup' => '<div class="taxonomy-manager-tree-size">'. theme("image", array('path' =>$module_path ."images/grippie.png", 'alt' => t("Resize tree"), 'title' => t("Resize tree"), 'attributes' => array('class' => "div-grippie"))) .'</div>');
  
  if (isset($form_state['values']['double_tree_values']['left_tree_expand_terms']) && count($form_state['values']['double_tree_values']['left_tree_expand_terms'])) {
    $tids_to_expand = $form_state['values']['double_tree_values']['left_tree_expand_terms'];
  }
  else if (isset($form_state['values']['searched_terms']) && count($form_state['values']['searched_terms'])) {
    $tids_to_expand = $form_state['values']['searched_terms'];
  }
  else {
    $tids_to_expand = $tid;
  }

  $form['taxonomy']['manager']['tree'] = array( 
    '#type' => 'taxonomy_manager_tree', 
    '#vid' => $voc->vid, 
    '#pager' => TRUE,
    '#search_string' => ($tid) ? NULL : $filter,
    '#language' => isset($language) ? $language : '',
    '#terms_to_expand' => $tids_to_expand,
    '#terms_to_highlight' => $tids_to_expand,
  );
  
  $form['toolbar'] = array( 
    '#type' => 'fieldset', 
    '#title' => t('Toolbar'),
    '#attributes' => array('id' => 'taxonomy-manager-toolbar'),
  );
  
  $form['toolbar']['weight_up'] = array( 
    //'#type' => 'button',
    '#attributes' => array('class' => array('taxonomy-manager-buttons')), 
    '#value' => t('Up'),
    '#theme' => 'no_submit_button',
    '#prefix' => '<div id="taxonomy-manager-toolbar-buttons">',
  );
  
  $form['toolbar']['weight-down'] = array( 
    //'#type' => 'button', 
    '#attributes' => array('class' => array('taxonomy-manager-buttons')),
    '#value' => t('Down'),
    '#theme' => 'no_submit_button',
  );
  
  $form['toolbar']['wrapper'] = array( 
    '#type' => 'markup', 
    '#markup' => '<div id="taxonomy-manager-toolbar-throbber"></div><div class="clear"></div>', 
    '#weight' => 20,
    '#prefix' => '</div>',
  );
  
  taxonomy_manager_add_form($form, $voc);
  
  taxonomy_manager_confirm_delete($form, $voc);
  
  taxonomy_manager_term_merge_form($form, $voc);
  
  taxonomy_manager_move_form($form, $voc);
  
  taxonomy_manager_export_form($form, $voc);
  
  taxonomy_manager_double_tree_settings_form($form, $voc);
  
  taxonomy_manager_search_form($form, $voc);
  
  // A term clicked in tree-view and submitted through load-tid helper, 
  // generate term data form.
  // Moved this code snippet to the bottom here to prevent needless 
  // expanding of the tree for this tid.
  if (isset($form_state['values']['load-tid'])) {
    $tid = $form_state['values']['load-tid'];
  }
  taxonomy_manager_term_data_form($form, $form_state, $tid, $voc->vid);
  
  return $form;
}

function taxonomy_manager_double_tree_form($form, &$form_state, $voc1, $voc2, $tid = 0, $filter = NULL) {
  // Check for confirmation forms.
  if (isset($form_state['confirm_delete'])) {
    return taxonomy_manager_term_confirm_delete($form, $form_state, $voc1->vid, $voc2->vid);
  }
  $module_path = drupal_get_path('module', 'taxonomy_manager') .'/';
  
  $form = taxonomy_manager_form($form, $form_state, $voc1, $tid, $filter);
  
  $form['disable'] = array(
    '#markup' => l(t('Disable Double Tree'), 'admin/structure/taxonomy_manager/voc/'. $voc1->machine_name),
    '#weight' => -100,
  );
    
  $form['voc2'] = array('#type' => 'value', "#value" => $voc2);
  
  $form['taxonomy2'] = array(
    '#tree' => TRUE,
  );
  
  $form['taxonomy2']['manager'] = array( 
    '#type' => 'fieldset',
    '#title' => check_plain($voc2->name),
    '#weight' => 10,
    '#tree' => TRUE,
  );
  
  $form['taxonomy2']['manager']['top'] = array(
    '#value' => '', 
    '#prefix' => '<div class="taxonomy-manager-tree-top">', 
    '#suffix' => '</div>',
  );
  
  if (module_exists('i18ntaxonomy')) {
    if (i18ntaxonomy_vocabulary($voc2->vid) == I18N_TAXONOMY_TRANSLATE) {
      if ($tid) {
        $language = _taxonomy_manager_term_get_lang($tid); 
      }
      else {
        $lang = language_default();
        $language = $lang->language;
      }
      $form['taxonomy2']['manager']['top']['language'] = array(
        '#type' => 'select',
        '#title' => t('Language'),
        '#default_value' => $language,
        '#options' => array('' => t('All'), 'no language' => t('no language')) + locale_language_list('name'),
        '#attributes' => array('class' => array('language-selector')),
      );
    }
  }
  
  $form['taxonomy2']['manager']['top']['size'] = array('#markup' => '<div class="taxonomy-manager-tree-size">'. theme("image", array('path' => $module_path ."images/grippie.png", 'alt' => t("Resize tree"), 'title' => t("Resize tree"), 'attributes' => array('class' => "div-grippie"))) .'</div>');
  
  if (isset($form_state['values']['double_tree_values']['right_tree_expand_terms']) && count($form_state['values']['double_tree_values']['right_tree_expand_terms'])) {
    $tids_to_expand = $form_state['values']['double_tree_values']['right_tree_expand_terms'];
  }
  else {
    $tids_to_expand = $tid;
  }
  
  $form['taxonomy2']['manager']['tree'] = array( 
    '#type' => 'taxonomy_manager_tree', 
    '#vid' => $voc2->vid, 
    '#pager' => TRUE,
    '#search_string' => ($tid) ? NULL : $filter,
    '#language' => isset($language) ? $language : '',
    '#terms_to_expand' => $tids_to_expand, //@todo check if term coming through ajax
    '#terms_to_highlight' => $tids_to_expand,
  );
  
  $form['double-tree']['operations'] = array(
    '#tree' => TRUE,
  );
  
  $form['double-tree']['operations']['move_right'] = array(
    '#type' => 'image_button',
    '#attributes' => array('title' => t('Move right')),
    '#src' => $module_path ."images/go-next.png",
    '#submit' => array('taxonomy_manager_double_tree_move_submit'),
    '#validate' => array('taxonomy_manager_double_tree_move_validate'),
    '#ajax' => array(
      'callback' => 'taxonomy_manager_double_tree_ajax_callback',
      'method' => 'replaceWith',
      'event' => 'click',
      'wrapper' => 'taxonomy-manager',
      'progress' => array('type' => ''),
    ),
    '#weight' => 20,
    '#prefix' => '<div class="taxonomy-manager-double-tree-operations-buttons">',
    '#suffix' => '</div>',
  );
  $form['double-tree']['operations']['move_left'] = array(
    '#type' => 'image_button',
    '#attributes' => array('title' => t('Move left')),
    '#src' => $module_path ."images/go-previous.png",
    '#submit' => array('taxonomy_manager_double_tree_move_submit'),
    '#validate' => array('taxonomy_manager_double_tree_move_validate'),
    '#ajax' => array(
      'callback' => 'taxonomy_manager_double_tree_ajax_callback',
      'method' => 'replaceWith',
      'event' => 'click',
      'wrapper' => 'taxonomy-manager',
      'progress' => array('type' => ''),
    ),
    '#weight' => 20,
    '#prefix' => '<div class="taxonomy-manager-double-tree-operations-buttons">',
    '#suffix' => '</div>',
  );

  //switch operations
  if ($voc1->vid != $voc2->vid) {
    $form['double-tree']['operations']['move_right']['#attributes']['title'] = t('Switch selected terms and its children to the right voc');
    $form['double-tree']['operations']['move_right']['#submit'] = array('taxonomy_manager_double_tree_switch_submit');
    $form['double-tree']['operations']['move_right']['#validate'] = array('taxonomy_manager_double_tree_switch_validate');
    
    $form['double-tree']['operations']['move_left']['#attributes']['title'] = t('Switch selected terms and its children to the left voc');
    $form['double-tree']['operations']['move_left']['#submit'] = array('taxonomy_manager_double_tree_switch_submit');
    $form['double-tree']['operations']['move_left']['#validate'] = array('taxonomy_manager_double_tree_switch_validate');
    
    // Copy buttons
    $form['double-tree']['operations']['copy_right'] = $form['double-tree']['operations']['move_right'];
    $form['double-tree']['operations']['copy_left'] = $form['double-tree']['operations']['move_left'];
    
    $form['double-tree']['operations']['copy_right']['#attributes']['title'] = t('Copy selected term names to the right voc.');
    $form['double-tree']['operations']['copy_right']['#src'] = $module_path ."images/copy-right.png";
    $form['double-tree']['operations']['copy_right']['#submit'] = array('taxonomy_manager_double_tree_copy_submit');
    $form['double-tree']['operations']['copy_right']['#validate'] = array('taxonomy_manager_double_tree_copy_validate');
    $form['double-tree']['operations']['copy_right']['#weight'] = 22;
    
    $form['double-tree']['operations']['copy_left']['#attributes']['title'] = t('Copy selected term names to the left voc.');
    $form['double-tree']['operations']['copy_left']['#src'] = $module_path ."images/copy-left.png";
    $form['double-tree']['operations']['copy_left']['#submit'] = array('taxonomy_manager_double_tree_copy_submit');
    $form['double-tree']['operations']['copy_left']['#validate'] = array('taxonomy_manager_double_tree_copy_validate');
    $form['double-tree']['operations']['copy_left']['#weight'] = 23;

  }
  else if (isset($language)) {
    $form['double-tree']['operations']['add_translation'] = array(
      '#type' => 'image_button',
      '#attributes' => array('title' => t('Add Translation')),
      '#src' => $module_path ."images/connect.png",
      '#prefix' => '<div class="taxonomy-manager-double-tree-operations-buttons">',
      '#suffix' => '</div>',
    );
  }

  return $form;
}

/**
 * form for searching terms
 */
function taxonomy_manager_search_form(&$form, $voc) {
	$form['#attached']['js'][] = array(
    'data' => array('hideForm' => array(array(
      'show_button' => 'edit-search-show', 	 
	    'hide_button' => 'edit-search-cancel', 	 
	    'div' => 'edit-search'))), 
    'type' => 'setting');
  
  $form['toolbar']['search_show'] = array( 
    //'#type' => 'button',
    '#attributes' => array('class' => array('taxonomy-manager-buttons', 'search')), 
    '#value' => t('Search'), 
    '#theme' => 'no_submit_button',
  );
  
  $search_description = t("You can search directly for exisiting terms. 
      If your input doesn't match an existing term, it will be used for filtering root level terms (useful for non-hierarchical vocabularies).");
  
  $form['search'] = array(
    '#type' => 'fieldset',
    '#title' => t('Search'),
    '#attributes' => array('style' => 'display:none;', 'id' => 'edit-search'),
    '#description' => $search_description, 
    '#collapsed' => TRUE,
    '#tree' => TRUE, 
  );
  
  $form['search']['field'] = array( 
    '#type' => 'textfield', 
    '#title' => t('Search String'), 
    '#autocomplete_path' => 'taxonomy_manager/autocomplete/'. $voc->vid,
    //'#prefix' => '<div id="edit-find-field">', 
    //'#suffix' => '</div>', 
  );
  
  $search_options = array(
    //'synonyms' => t('Include synonyms'), // @todo implement synonyms feature
    'subtrees' => t('Search under selected terms'),
  );
  if (module_exists('i18ntaxonomy')) {
    if (i18ntaxonomy_vocabulary($voc->vid) == I18N_TAXONOMY_TRANSLATE) {
      $search_options['language'] = t('Search within selected language');
    }
  }
  $form['search']['options'] = array(
    '#type' => 'checkboxes',
    '#title' => t('Search options'),
    '#options' => $search_options,
  );
  
  $form['search']['button'] = array( 
    '#type' => 'submit', 
    '#attributes' => array('class' => array('taxonomy-manager-buttons', 'search')),
    '#value' => t('Search'),
    //'#suffix' => '<div class="clear"></div>',
  );
  
  $form['search']['cancel'] = array( 
    //'#type' => 'button', 
    '#value' => t('Cancel'),
    '#theme' => 'no_submit_button',
    '#attributes' => array('class' => array('taxonomy-manager-buttons', 'cancel')),  
  );
}

/**
 * confirmation form for deleting selected terms
 */
function taxonomy_manager_confirm_delete(&$form, $voc) {
  $form['#attached']['js'][] = array(
    'data' => array('hideForm' => array(array(
      'show_button' => 'edit-delete-confirm', 	 
      'hide_button' => 'edit-delete-cancel', 	 
      'div' => 'edit-delete'))), 
    'type' => 'setting');

  $form['toolbar']['delete_confirm'] = array( 
    //'#type' => 'button', 
    '#attributes' => array('class' => array('taxonomy-manager-buttons delete')),
    '#value' => t('Delete'),
    '#theme' => 'no_submit_button',
  );
  
  $form['delete'] = array(
    '#type' => 'fieldset',
    '#attributes' => array('style' => 'display:none;', 'id' => 'edit-delete'),
    '#tree' => TRUE,
    '#title' => t('Confirmation'),
  );
  
  $question = t('Are you sure you want to delete all selected terms? ');
  $info = t('Remember all term specific data will be lost. This action cannot be undone.');
  
  $form['delete']['text'] = array('#value' => "<b>". $question ."</b><br/>". $info);
  
  $options = array( 
    'delete_orphans' => t('Delete children of selected terms, if there are any'), 
  );
     
  $form['delete']['options'] = array( 
    '#type' => 'checkboxes', 
    '#title' => t('Options'), 
    '#options' => $options, 
  );
  
  $form['delete']['delete'] = array( 
    '#type' => 'submit', 
    '#value' => t('Delete'),
    '#attributes' => array('class' => array('taxonomy-manager-buttons', 'delete')), 
    '#submit' => array('taxonomy_manager_form_delete_submit'),
    '#validate' => array('taxonomy_manager_form_delete_validate'),
  );
  
  $form['delete']['cancel'] = array(
    //'#type' => 'button', 
    '#attributes' => array('class' => array('taxonomy-manager-buttons', 'cancel')),
    '#value' => t('Cancel'), 
    '#theme' => 'no_submit_button',  
  );
}

/**
 * form for adding terms
 */
function taxonomy_manager_add_form(&$form, $voc, $hide_form = TRUE) {
  $attributes = array();
  if ($hide_form) {
	  $form['#attached']['js'][] = array(
      'data' => array('hideForm' => array(array(
        'show_button' => 'edit-add-show', 	 
	      'hide_button' => 'edit-add-cancel', 	 
	      'div' => 'edit-add'))), 
      'type' => 'setting');
    $attributes = array('style' => 'display:none;', 'id' => 'edit-add');
    $form['toolbar']['add_show'] = array( 
      //'#type' => 'button',
      '#attributes' => array('class' => 'taxonomy-manager-buttons add'), 
      '#value' => t('Add'), 
      '#theme' => 'no_submit_button',
    );
  }
    
  $description = "";
  $description = t("If you have selected one or more terms in the tree view, the new terms are automatically children of those.");
  
  $form['add'] = array( 
    '#type' => 'fieldset', 
    '#tree' => TRUE,
    '#attributes' => $attributes,
    '#title' => t('Add new terms'),
    '#description' => $description,
  );
  
  for ($i=0; $i<3; $i++) {
    $form['add']['term'][$i] = array( 
      '#type' => 'textfield', 
    );
  }
  $form['add']['mass'] = array(
    '#type' => 'fieldset',
    '#tree' => TRUE,
    '#title' => t('Mass term import (with textarea)'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );
  $form['add']['mass']['mass_add'] = array(
    '#type' => 'textarea',
    '#title' => t('Terms'),
    '#description' => t('One term per line'),
    '#rows' => 10,
  );
  $form['add']['add'] = array( 
    '#type' => 'submit',
    '#attributes' => array('class' => array('taxonomy-manager-buttons', 'add')), 
    '#value' => t('Add'),
    '#submit' => array('taxonomy_manager_form_add_submit'),
  );
  $form['add']['cancel'] = array( 
    //'#type' => 'button', 
    '#value' => t('Cancel'),
    '#theme' => 'no_submit_button',
    '#attributes' => array('class' => array('taxonomy-manager-buttons', 'cancel')),  
  );
}


/**
 * Form for the Term merge module
 */
function taxonomy_manager_term_merge_form(&$form, $voc) {
  if (!module_exists('term_merge') || !user_access('merge terms')) {
    return;
  }
	$form['#attached']['js'][] = array(
    'data' => array('hideForm' => array(array(
      'show_button' => 'edit-term-merge-show',
	    'hide_button' => 'edit-term-merge-cancel',
	    'div' => 'edit-term-merge'))),
    'type' => 'setting');
      
  $description = t("This operation is based on the Term merge module. Selected terms get merged into the given one. Optionally the selected terms can be retained, otherwise they will be deleted.");

  $form['toolbar']['term_merge_show'] = array(
    //'#type' => 'button', 
    '#attributes' => array('class' => array('taxonomy-manager-buttons merge')),
    '#value' => t('Term merge'),
    '#theme' => 'no_submit_button', 
  );

  $form['term_merge'] = array(
    '#type' => 'fieldset',
    '#tree' => TRUE,
    '#attributes' => array('style' => 'display:none;', 'id' => 'edit-term-merge'),
    '#title' => t('Merging of terms'),
    '#description' => $description,
  );
  
  $form['term_merge']['dest_term'] = array(
    '#type' => 'textfield',
    '#title' => t('Destination term'),
    '#description' => t("Enter a unique term name or a term id with 'term-id:[tid]'"),
    '#required' => FALSE,
    '#autocomplete_path' => 'taxonomy_manager/autocomplete/'. $voc->vid,
  );

  $form['term_merge']['options'] = array(
    '#type' => 'checkboxes',
    '#title' => t('Options'),
    '#options' => array(
      'keep_merged' => t('Keep merged terms'),
    ),
  );
   
  $form['term_merge']['submit'] = array(
    '#type' => 'submit', 
    '#value' => t('Merge'),
    '#attributes' => array('class' => array('taxonomy-manager-buttons', 'merge')), 
    '#validate' => array('taxonomy_manager_form_term_merge_validate'),
    '#submit' => array('taxonomy_manager_form_term_merge_submit'),
  );
   
  $form['term_merge']['cancel'] = array(
    //'#type' => 'button', 
    '#value' => t('Cancel'),
    '#attributes' => array('class' =>  array('taxonomy-manager-buttons', 'cancel')), 
    '#theme' => 'no_submit_button', 
  );
}

/**
 * form for moving terms in hierarchies
 */
function taxonomy_manager_move_form(&$form, $voc) {
  $form['#attached']['js'][] = array(
    'data' => array('hideForm' => array(array(
      'show_button' => 'edit-move-show', 	 
	    'hide_button' => 'edit-move-cancel', 	 
	    'div' => 'edit-move'))), 
    'type' => 'setting');
    
  $description = t("You can change the parent of one or more selected terms. 
      If you leave the autocomplete field empty, the term will be a root term.");

  $form['toolbar']['move_show'] = array( 
    //'#type' => 'button', 
    '#value' => t('Move'),
    '#attributes' => array('class' => array('taxonomy-manager-buttons move')),
     '#theme' => 'no_submit_button', 
   );
  $form['move'] = array( 
    '#type' => 'fieldset',
    '#tree' => TRUE,
    '#attributes' => array('style' => 'display:none;', 'id' => 'edit-move'),
    '#title' => t('Moving of terms'),
    '#description' => $description,
  );
  
  $form['move']['parents'] = array(
    '#type' => 'textfield',
    '#title' => t('Parent term(s)'),
    '#description' => t("Enter a unique term name or a term id with 'term-id:[tid]'. Separate multiple parent terms with commas."),
    '#required' => FALSE,
    '#autocomplete_path' => 'taxonomy_manager/autocomplete/'. $voc->vid,
  );
  
  $options = array();
  $options['keep_old_parents'] = t('Keep old parents and add new ones (multi-parent). Otherwise old parents get replaced.');
  $form['move']['options'] = array( 
    '#type' => 'checkboxes', 
    '#title' => t('Options'), 
    '#options' => $options, 
  );
  
  $form['move']['submit'] = array( 
    '#type' => 'submit',
    '#attributes' => array('class' => array('taxonomy-manager-buttons', 'move')), 
    '#value' => t('Move'),
    '#validate' => array('taxonomy_manager_form_move_validate'), 
    '#submit' => array('taxonomy_manager_form_move_submit'),
  );
   
  $form['move']['cancel'] = array( 
    //'#type' => 'button', 
    '#value' => t('Cancel'),
    '#attributes' => array('class' =>  array('taxonomy-manager-buttons', 'cancel')),
    '#theme' => 'no_submit_button',  
  );
}

/**
 * form for exporting terms
 */
function taxonomy_manager_export_form(&$form, $voc) {
  $form['#attached']['js'][] = array(
    'data' => array('hideForm' => array(array(
      'show_button' => 'edit-export-show', 	 
	    'hide_button' => 'edit-export-cancel', 	 
	    'div' => 'edit-export'))), 
    'type' => 'setting');
  $form['#attached']['js'][] = drupal_get_path('module', 'taxonomy_manager') . '/js/csv_export.js';
  $form['#attached']['js'][] = array(
    'data' => array('exportCSV' => array('url' => url("admin/structure/taxonomy_manager/export"))), 
    'type' => 'setting');
  
  $form['toolbar']['export_show'] = array( 
    //'#type' => 'button', 
    '#attributes' => array('class' => array('taxonomy-manager-buttons export')),
    '#value' => t('CSV Export'), 
    '#theme' => 'no_submit_button', 
  );
  $form['export'] = array( 
    '#type' => 'fieldset',
    '#tree' => TRUE,
    '#attributes' => array('style' => 'display:none;', 'id' => 'edit-export'),
    '#title' => t('CSV Export'),
  );

  $form['export']['delimiter'] = array(
    '#type' => 'textfield',
    '#title' => t('Delimiter for CSV File'),
    '#required' => FALSE,
    '#default_value' => ";",
  );

  $options['whole_voc'] = t('Whole Vocabulary');
  $options['children'] = t('Child terms of a selected term');
  $options['root_terms'] = t('Root level terms only');

  $form['export']['options'] = array( 
    '#type' => 'radios', 
    '#title' => t('Terms to export'), 
    '#options' => $options, 
    '#default_value' => 'whole_voc',
    '#prefix' => '<div id="taxonomy_manager_export_options">',
    '#suffix' => '</div>',
  );
  
  $form['export']['depth'] = array(
    '#type' => 'textfield',
    '#title' => t('Depth of tree'),
    '#description' => t('The number of levels of the tree to export. Leave empty to return all levels.'),
  );
  
  $form['export']['csv'] = array(
    '#type' => 'textarea', 
    '#title' => t('Exported CSV'),
    '#description' => t('The generated code will appear here (per AJAX). You can copy and paste the code into a .csv file. The csv has following columns: voc id | term id | term name | description | parent id 1 | ... | parent id n'),
    '#rows' => 8,
  );
  
  $form['export']['submit'] = array( 
    '#type' => 'submit',
    '#attributes' => array('class' => array('taxonomy-manager-buttons', 'export')), 
    '#value' => t('Export now'),
    '#theme' => 'no_submit_button', 
  );
   
  $form['export']['cancel'] = array( 
    //'#type' => 'button', 
    '#value' => t('Cancel'),
    '#attributes' => array('class' =>  array('taxonomy-manager-buttons', 'cancel')),
    '#theme' => 'no_submit_button',  
  );
}

function taxonomy_manager_double_tree_settings_form(&$form, $voc) { 
	$form['#attached']['js'][] = array(
    'data' => array('hideForm' => array(array(
      'show_button' => 'edit-double-tree-show', 	 
	    'hide_button' => 'edit-double-tree-settings-cancel', 	 
	    'div' => 'edit-double-tree-settings'))), 
    'type' => 'setting');

  $form['toolbar']['double_tree_show'] = array( 
    //'#type' => 'button', 
    '#attributes' => array('class' => array('taxonomy-manager-buttons double-tree')),
    '#value' => t('Double Tree'), 
    '#theme' => 'no_submit_button', 
  );
  $form['double-tree-settings'] = array( 
    '#type' => 'fieldset',
    '#tree' => TRUE,
    '#attributes' => array('style' => 'display:none;', 'id' => 'edit-double-tree-settings'),
    '#title' => t('Double Tree Settings'),
    '#description' => t('Specify settings for second tree. Choose the same vocabulary if you want to move terms in the hierarchy or if you want to add new translations within a multilingual vocabulary. Choose a different vocabulary if you want to switch terms among these vocabularies.'),
  );
  
  $options = array();
  $vocs = taxonomy_get_vocabularies();
  foreach ($vocs as $v) {
    $options[$v->machine_name] = $v->name; 
  }
  $form['double-tree-settings']['voc2_machine_name'] = array(
    '#type' => 'select',
    '#title' => t('Vocabulary for second tree'),
    '#options' => $options,
    '#default_value' => $voc->vid,
  );
  
  $form['double-tree-settings']['submit'] = array( 
    '#type' => 'submit',
    '#attributes' => array('class' => array('taxonomy-manager-buttons', 'double-tree')), 
    '#value' => t('Enable Double Tree'),
    '#submit' => array('taxonomy_manager_form_double_tree_submit'),
  );
  if (arg(3) == "double-tree") {
    $form['double-tree-settings']['disable'] = array( 
      '#type' => 'submit',
      '#attributes' => array('class' => array('taxonomy-manager-buttons', 'double-tree-disable')), 
      '#value' => t('Disable Double Tree'),
      '#submit' => array('taxonomy_manager_form_double_tree_disable_submit'),
    );
  }
  $form['double-tree-settings']['cancel'] = array( 
    //'#type' => 'button', 
    '#value' => t('Cancel'),
    '#attributes' => array('class' =>  array('taxonomy-manager-buttons', 'cancel')),
    '#theme' => 'no_submit_button',  
  );
}

function taxonomy_manager_term_data_form(&$form, &$form_state, $tid = 0, $vid1, $vid2 = 0) {
  $module_path = drupal_get_path('module', 'taxonomy_manager') .'/';
  if ($tid) {
    $term = taxonomy_term_load($tid);

    //prevent that title of the fieldset is too long
    $title = check_plain($term->name);
    if (drupal_strlen($title) >= 33) {
      $title = drupal_substr($title, 0, 33) ."...";
    }
    $form['term_data'] = array( 
      '#type' => 'fieldset', 
      '#title' => $title ." (". $term->tid .")",
    );
    unset($form_state['term']);
    $form['term_data'] = taxonomy_form_term($form['term_data'], $form_state, $term);
    $form['term_data']['description']['#rows'] = 2;
    $form['term_data']['weight'] = $form['term_data']['relations']['weight'];
    unset($form['term_data']['relations']);
    unset($form['term_data']['actions']);

    $form['term_data']['parents'] = _taxonomy_manager_form_term_data_parents($term); 
    $form['term_data']['parents']['#weight'] = '52';
    
    $form['term_data']['close'] = array(
      '#markup' => '<div id="term-data-close"><span title="'. t('Close') .'">&nbsp;&nbsp;&nbsp;&nbsp;</span></div>',
      '#weight' => -100,
    );
    $link_img = theme("image", array('path' => $module_path ."images/link-small.png", 'alt' => t("link to term page")));
    $form['term_data']['link'] = array(
      '#markup' => '<br />'. l($link_img .'&nbsp;'. t('Go to the term page'), 'taxonomy/term/'. $term->tid, array('attributes' => array('rel' => 'tag', 'title' => strip_tags($term->description), 'target' => '_blank'), 'html' => TRUE)), 
      '#weight' => 56);
    
    $form['term_data']['vid'] = array('#type' => 'hidden', '#value' => $term->vid);
    $form['term_data']['tid'] = array('#type' => 'hidden', '#value' => $tid);
    
    $form['term_data']['save'] = array(
      '#type' => 'submit',
      '#value' => t('Save changes'),
      '#submit' => array('taxonomy_manager_term_data_form_submit'),
      '#validate' => array('taxonomy_form_term_validate'),
      '#attributes' => array('class' => array('taxonomy-manager-buttons', 'save')),
      '#ajax' => array(
        'callback' => 'taxonomy_manager_term_data_form_ajax_callback',
        'method' => 'replaceWith',
        'event' => 'click',
        'wrapper' => 'taxonomy-term-data-replace',
        'progress' => array('type' => ''),
      ),
      '#weight' => 20,
    );
  }
  
  $form['term_data']['#prefix'] = '<div id="taxonomy-term-data-replace">';
  $form['term_data']['#suffix'] = '</div>';
}

/**
 * helper function for generating tables with values and delete op and field for adding
 *
 * @param $term term object which is going to be displayed
 * @return an form array
 */
function _taxonomy_manager_form_term_data_parents($term) {
  $module_path = drupal_get_path('module', 'taxonomy_manager') .'/';
  
  $form['#tree'] = TRUE;
  $form['#theme'] = 'taxonomy_manager_term_data_extra';
  $form['headers'][] = array('#markup' => t('Parents'));
  $form['headers'][] = array('#markup' => '');
  $form['data'] = array();
  
  foreach (taxonomy_get_parents($term->tid) as $tid => $parent) {
    $extra_info = taxonomy_manager_tree_term_extra_info($parent);
    $form['data'][$tid][] = array(
      '#markup' => l($parent->name, 'admin/structure/taxonomy_manager/termdata/'. $parent->tid, array('attributes' => array('title' => $extra_info, 'class' => 'taxonomy-term-data-name-link'))),
      '#row-class' => 'taxonomy-term-data-name',
      '#row-id' => 'term-'. $tid,
      '#tree' => TRUE, 
    );
    $form['data'][$tid][0]['tid'] = array(
      '#type' => 'hidden',
      '#value' => $tid,
    );
    $form['data'][$tid]['remove'] = array(
      '#type' => 'image_button',
      '#attributes' => array('title' => t('Remove')),
      '#submit' => array('taxonomy_manager_term_data_form_submit_parents_remove'),
      '#validate' => array('taxonomy_manager_term_data_form_validate_parents_remove'),
      '#src' => $module_path ."images/list-remove.png",
      '#row-class' => 'taxonomy-term-data-operations',
    );
    $form['data'][$tid]['remove']['#ajax'] = array(
      'callback' => 'taxonomy_manager_term_data_form_ajax_callback',
      'progress' => array('type' => ''),
      'method' => 'replaceWith',
      'event' => 'click',
      'wrapper' => 'taxonomy-term-data-replace',
    );
  }

  $form['op'] = array();
  $form['op']['add'] = array( 
    '#type' => 'textfield',
    '#prefix' => '<div class="term-data-autocomplete">',
    '#autocomplete_path' => 'taxonomy_manager/autocomplete/'. $term->vid,
    '#suffix' => '</div>',
    '#size' => 35,
  );
  $form['op']['add_button'] = array( 
    '#type' => 'image_button',
    '#attributes' => array('title' => t('Add')),
    '#submit' => array('taxonomy_manager_term_data_form_submit_parents_add'),
    '#validate' => array('taxonomy_manager_term_data_form_validate_parents_add'),
    '#src' => $module_path ."images/list-add.png",
    '#row-class' => 'term-data-autocomplete-add',
    '#ajax' => array(
      'callback' => 'taxonomy_manager_term_data_form_ajax_callback',
      'progress' => array('type' => ''),
      'method' => 'replaceWith',
      'event' => 'click',
      'wrapper' => 'taxonomy-term-data-replace',
    ),
  );
  
  return $form;
}

/**
 * helper function for generating a table listing the translations 
 */
function _taxonomy_manager_form_term_data_translations($term, $translations, $header_type, $attr, $autocomplete = TRUE, $add = TRUE) {
  $module_path = drupal_get_path('module', 'taxonomy_manager') .'/';
  $rows = array();
  
  $form['#theme'] = 'taxonomy_manager_term_data_extra';
  $form['data'] = array();

  foreach ($translations as $tid => $value) {
    if (is_object($value)) {
      $name = $value->name;
      $id = $value->tid;
      $vid = $value->vid;
      $lang = $value->language;
      $trid = $value->trid;
      $extra_info = taxonomy_manager_tree_term_extra_info($value);
    }
    $form['data'][$id][] = array(
      '#value' => (isset($vid) && $vid > 0) ? l($name, 'admin/structure/taxonomy_manager/termdata/'. $id, array('attributes' => array('title' => $extra_info))) : check_plain($name),
      '#row-class' => 'taxonomy-term-data-name',
      '#row-id' => 'term-'. $id, 
    );
    $form['data'][$id][] = array(
      '#value' => check_plain(locale_language_name($lang)),
      '#row-class' => 'taxonomy-term-data-lang',
      '#row-id' => 'term-lang-'. $id, 
    );
    $form['data'][$id][] = array(
      '#value' => '<span class="'. $attr .'" title="'. t('Remove') .'">&nbsp;</span>',
      '#row-class' => 'taxonomy-term-data-operations',
    );
  }
  $form['headers'][] = array('#value' => $header_type);
  $form['headers'][] = array('#value' => t('Language'));
  $form['headers'][] = array('#value' => '');

  $form['op'] = array();
  if ($add) {
    $form['op']['add'] = array( 
      '#type' => 'textfield',
      '#prefix' => '<div class="term-data-autocomplete">',
      '#suffix' => '</div>',
      '#size' => 35,
    );
    $form['op']['lang'] = array( 
      '#type' => 'select',
      '#options' => array('' => '') + locale_language_list('name'),
      '#default value' => '',
    );
    if ($autocomplete) {
      $form['op']['add']['#autocomplete_path'] = 'taxonomy_manager/autocomplete/'. $term->vid;
    }
  
    $form['op']['add_button'] = array( 
      '#value' => '<span class="'. $attr .'" title="'. t('Add') .'">&nbsp;</span>', 
      '#prefix' => '<div class="term-data-autocomplete-add">',
      '#suffix' => '</div>',
    );
  }
  
  return $form;

}

/** 
 * submit handler for loading term data form
 */
function taxonomy_manager_load_tid_submit($form, &$form_state) {
  // Unset input, so that the term data form has fresh values.
  unset($form_state['input']);
  if ($form_state['values']['load-tid-refresh-tree']) {
    form_set_value(array('#parents' => array('tree-update')), array('update' => TRUE), $form_state);
  }
  $form_state['rebuild'] = TRUE;
}

/**
 * validates the form (only search button)
 **/
function taxonomy_manager_form_validate($form, &$form_state) {
  if ($form_state['clicked_button']['#value'] == t('Search') && empty($form_state['values']['search']['field'])) {
    form_set_error('search', t('Search field is empty'));
    $form_state['rebuild'] = TRUE;
  }
}

/**
 * submits the taxonomy manager form (only search button)
 **/
function taxonomy_manager_form_submit($form, &$form_state) {
  if ($form_state['values']['delete'] === TRUE) {
    return taxonomy_manager_term_confirm_delete_submit($form, $form_state);
  }
  $voc = $form_state['values']['voc'];
  if (isset($form_state['values']['voc2'])) {
    $voc2 = $form_state['values']['voc2'];
    $url_prefix = 'admin/structure/taxonomy_manager/double-tree/'. $voc->machine_name .'/'. $voc2->machine_name;
  }
  else {
    $url_prefix = 'admin/structure/taxonomy_manager/voc/'. $voc->machine_name;
  }
  
  $search_string = $form_state['values']['search']['field'];
  $terms = array();
  
  $include_synonyms = FALSE;
  $selected_tids = array();
  $language = "";
  
  // @todo
  /*if ($form_state['values']['search']['options']['synonyms']) {
    $include_synonyms = TRUE;
  }*/
  if ($form_state['values']['search']['options']['subtrees']) {
    $selected_tids = $form_state['values']['taxonomy']['manager']['tree']['selected_terms'];
  }
  if (isset($form_state['values']['search']['options']['language'])) {
    $language = $form_state['values']['taxonomy']['manager']['top']['language']; 
  }
  
  $terms = taxonomy_manager_autocomplete_search_terms($search_string, $voc->vid, $include_synonyms, $selected_tids, $language);
  if (count($terms) == 1) {
    $tid = $terms[0];
    drupal_set_message(t("Your search string matches exactly one term"));
    drupal_goto($url_prefix .'/'. $tid);
  }
  else if (count($terms) > 1) {
    $matched_text_list = array();
    foreach ($terms as $matched_tid) {
      $matched_term = taxonomy_term_load($matched_tid);
      $parents = array();
      foreach (taxonomy_get_parents($matched_tid) as $parent) {
        $parents[] = check_plain($parent->name);
      }
      $parent_text = count($parents) ? implode(' | ', $parents) ." › " : "";
      $matched_text_list[] = $parent_text ."". l($matched_term->name, $url_prefix .'/'. $matched_term->tid, array('attributes' => array('title' => taxonomy_manager_tree_term_extra_info($matched_term)))) ." (". $matched_term->tid . (!empty($matched_term->language) ? '-'. check_plain($matched_term->language) : '') . ")";
    }
    form_set_value($form, array('searched_terms' => $terms), $form_state);
    drupal_set_message(t("Your search string matches !count terms:", array('!count' => count($terms))) ."". theme('item_list', array('items' => $matched_text_list)));
    $form_state['rebuild'] = TRUE;
  }
  else {
    drupal_set_message(t("No match found. Filtering root level terms starting with @search_string.", array('@search_string' => $search_string)));
    drupal_set_message(l(t("Show unfiltered tree"), $url_prefix));
    drupal_goto($url_prefix .'/0/'. $search_string);
  }
}

/**
 * validates taxonomy manager double tree
 **/
function taxonomy_manager_double_tree_form_validate($form, &$form_state) {
  taxonomy_manager_form_validate($form, $form_state);
}

/**
 * submits the taxonomy manager double tree
**/
function taxonomy_manager_double_tree_form_submit($form, &$form_state) { 
  taxonomy_manager_form_submit($form, $form_state);
}

/**
 * Submit handler for adding terms
 */
function taxonomy_manager_form_add_submit($form, &$form_state) {
  $terms = array();
  $selected_tids = isset($form_state['values']['taxonomy']['manager']['tree']['selected_terms']) ? $form_state['values']['taxonomy']['manager']['tree']['selected_terms'] : array();

  foreach ($form_state['values']['add']['term'] as $value) {
    if (!empty($value)) {
      $terms[] = $value;
    }
  }
  if ($form_state['values']['add']['mass']['mass_add']) {
    foreach (explode ("\n", str_replace("\r", '', $form_state['values']['add']['mass']['mass_add'])) as $term) {
      if ($term) {
        $terms[] = $term;
      }
    }
  }
  $selected_tids = count($selected_tids) ? $selected_tids : 0;
  foreach ($terms as $name) {
    $term = new stdClass;
    $term->name = $name;
    $term->vid = $form_state['values']['voc']->vid;
    $term->parent = $selected_tids;
    taxonomy_term_save($term);
    if (module_exists('i18ntaxonomy')) {
      if (i18ntaxonomy_vocabulary($form_state['values']['voc']->vid) == I18N_TAXONOMY_TRANSLATE && $form_state['values']['taxonomy']['manager']['top']['language'] != "") {
        _i18ntaxonomy_term_set_lang($term['tid'], $form_state['values']['taxonomy']['manager']['top']['language']);
        $updated_lang = TRUE;
      }
    }
  }
  taxonomy_manager_update_voc($form_state['values']['voc']->vid, $selected_tids);

  if (isset($updated_lang) && $updated_lang == TRUE) {
    drupal_set_message(t("Saving terms to language @lang", array('@lang' => locale_language_name($form_state['values']['taxonomy']['manager']['top']['language']))));
  }
  drupal_set_message(t("Terms added: %terms", array('%terms' => implode(', ', $terms))));
}


/**
 * Validation handler for deleting terms
 */
function taxonomy_manager_form_delete_validate($form, &$form_state) {
  $selected_tids = array();
  $selected_tids = $form_state['values']['taxonomy']['manager']['tree']['selected_terms'];
  if (count($selected_tids) < 1) {
    form_set_error('delete', t("No terms for deleting selected"));
    $form_state['rebuild'] = TRUE;
  }
}



/**
 * Submit handler for deleting terms
 */
function taxonomy_manager_form_delete_submit($form, &$form_state) {
  $selected_tids = array();
  $selected_tids = $form_state['values']['taxonomy']['manager']['tree']['selected_terms'];
  if ($form_state['values']['delete'] === TRUE) {
    return taxonomy_manager_term_confirm_delete_submit($form, $form_state);
  }
  // Rebuild the form to confirm term deletion.
  $form_state['rebuild'] = TRUE;
  $form_state['confirm_delete'] = TRUE;
}


/**
 * Form builder for the term delete form.
 *
 */
function taxonomy_manager_term_confirm_delete($form, &$form_state, $vid, $vid2 = NULL) {
  $voc = taxonomy_vocabulary_load($vid);
  if (isset($vid2)) {
    $voc2 = taxonomy_vocabulary_load($vid2);
    $form['voc2'] = array('#type' => 'value', '#value' => $voc2);
    $url = 'admin/structure/taxonomy_manager/double-tree/'. $voc->machine_name .'/'. $voc2->machine_name;
  }
  else {
    $url = 'admin/structure/taxonomy_manager/voc/'. $voc->machine_name;
  }

  $selected = $form_state['values']['taxonomy']['manager']['tree']['selected_terms'];
  $form['selected_terms'] = array('#prefix' => '<ul>', '#suffix' => '</ul>', '#tree' => TRUE);
  foreach ($selected as $tid) {
    $term = taxonomy_term_load($tid);
    $form['selected_terms'][$tid] = array('#type' => 'hidden', '#value' => $tid, '#prefix' => '<li>', '#suffix' => check_plain($term->name) ."</li>\n");
  }

  $form['delete'] = array('#type' => 'value', '#value' => TRUE);
  $form['voc'] = array('#type' => 'value', '#value' => $voc);
  $form['options'] = array('#type' => 'value', '#value' => $form_state['values']['delete']['options']);
  $msg = !empty($form_state['values']['delete']['options']['delete_orphans']) ? t('Deleting a term will delete all its children if there are any. ') : '';
  $msg .= t('This action cannot be undone.');
  return confirm_form($form,
      t('Are you sure you want to delete the following terms: '),
      $url,
      $msg,
      t('Delete'),
      t('Cancel'));
}

/**
 * Submit handler to delete a term after confirmation.
 *
 */
function taxonomy_manager_term_confirm_delete_submit($form, &$form_state) {
  $info = taxonomy_manager_delete_terms($form_state['values']['selected_terms'], $form_state['values']['options']);
  $deleted_term_names = array();
  $remaining_child_term_names = array();

  foreach ($info['deleted_terms'] as $term) {
    $deleted_term_names[] = $term->name;
  }
  foreach ($info['remaining_child_terms'] as $term) {
    $remaining_child_term_names[] = $term->name;
  }
  
  if (isset($form_state['values']['voc2'])) {
    $form_state['redirect'] = 'admin/structure/taxonomy_manager/double-tree/'. $form_state['values']['voc']->machine_name .'/'. $form_state['values']['voc2']->machine_name;
  }
  else {
    $form_state['redirect'] = 'admin/structure/taxonomy_manager/voc/'. $form_state['values']['voc']->machine_name;
  }
  drupal_set_message(t("Deleted terms: %deleted_term_names", array('%deleted_term_names' => check_plain(implode(', ', $deleted_term_names)))));
  if (count($remaining_child_term_names)) {
    drupal_set_message(t("Remaining child terms with different parents: %remaining_child_term_names", array('%remaining_child_term_names' => check_plain(implode(', ', $remaining_child_term_names))))); 
  }
  return;
}



/**
 * Validation handler for moving terms
 */
function taxonomy_manager_form_move_validate($form, &$form_state) {
  $selected_tids = array();
  $selected_tids = $form_state['values']['taxonomy']['manager']['tree']['selected_terms'];
  $error_msg = "";
  if (count($selected_tids) < 1) {
    form_set_error('move', t("Please selected terms you want to move in the hierarchy"));
    $form_state['rebuild'] = TRUE;
  }
  else if (_taxonomy_manager_check_duplicates($form_state['values']['voc']->vid, $form_state['values']['move']['parents'], $error_msg)) {
    form_set_error('move', t("Warning: Your input matches with multiple terms, because of duplicated term names. Please enter a unique term name or the term id with 'term-id:[tid]'") ." (". $error_msg .").");
    $form_state['rebuild'] = TRUE;
  }
  
  $typed_parents = taxonomy_manager_autocomplete_tags_get_tids($form_state['values']['move']['parents'], $form_state['values']['voc']->vid, FALSE);

  $parents = array();
  foreach ($typed_parents as $parent_info) {
    $parents[(int) $parent_info['tid']] = (int) $parent_info['tid'];
  }

  if (!taxonomy_manager_check_circular_hierarchy($selected_tids, $parents)) {
    form_set_error('move', t('Invalid selection. The resulting hierarchy would contain circles, which is not allowed. A term cannot be a parent of itself.')); 
    $form_state['rebuild'] = TRUE;
  }
  else if (!taxonomy_manager_check_language($form_state['values']['voc']->vid, $selected_tids, $typed_parents)) {
    form_set_error('move', t('Terms must be of the same language')); 
    $form_state['rebuild'] = TRUE;
  }
}


/**
 * checks for circles in the hierarchy, e.g. 1 -> 2 -> 3 -> 1
 * a term can't contain itself as a parent
 *
 * returns TRUE if resulting hierarchy is valid, else FALSE 
 */
function taxonomy_manager_check_circular_hierarchy($tids, $new_parents_tids) {
  if (is_array($tids) && is_array($new_parents_tids)) {
    //directly same term
    foreach ($tids as $tid) {
      if (in_array($tid, $new_parents_tids)) {
        return FALSE;
      }  
    }
  
    //same term over more hierarchy levels
    $all_parents = array();
    foreach ($new_parents_tids as $parent_tid) {
      $parents = taxonomy_get_parents_all($parent_tid);
      foreach ($parents as $parent) {
        $all_parents[$parent->tid] = $parent->tid;
      }
    }
    foreach ($tids as $tid) {
      if (in_array($tid, $all_parents)) {
        return FALSE;
      } 
    }
  }
  return TRUE;
}

/**
 * checks if terms in move or merge operation are of the same language
 *
 * returns TRUE if operation allowed, else FALSE (different languages)
 */
function taxonomy_manager_check_language($vid, $selected_tids, $parents) {
  if (module_exists('i18ntaxonomy')) {
    if (count($parents) && count($selected_tids)) {
      $term = array_pop($parents);
      $lang = _taxonomy_manager_term_get_lang($term['tid']);
      if (i18ntaxonomy_vocabulary($vid) == I18N_TAXONOMY_TRANSLATE) {
        foreach ($parents as $parent) {
          if (_taxonomy_manager_term_get_lang($parent['tid']) != $lang) {
            return FALSE; 
          }
        }
        foreach ($selected_tids as $tid) {
          if (_taxonomy_manager_term_get_lang($tid) != $lang) {
            return FALSE;
          }
        } 
      }
    }
  }
  return TRUE;
}


/**
 * Submit handler for moving terms
 */
function taxonomy_manager_form_move_submit($form, $form_state) {
  $selected_tids = array();
  $selected_tids = $form_state['values']['taxonomy']['manager']['tree']['selected_terms'];
  $lang = isset($form_state['values']['taxonomy']['manager']['top']['language']) ? $form_state['values']['taxonomy']['manager']['top']['language'] : NULL;
  $typed_parents = taxonomy_manager_autocomplete_tags_get_tids($form_state['values']['move']['parents'], $form_state['values']['voc']->vid, TRUE, $lang);

  $parents = array();
  foreach ($typed_parents as $parent_info) {
    $parents[] = $parent_info['tid'];
  }
  
  if (count($parents) == 0) $parents[0] = 0; //if empty, delete all parents
  
  taxonomy_manager_move($parents, $selected_tids, $form_state['values']['move']['options']);
  
  if ($form_state['values']['move']['options']['keep_old_parents']) {
    $parents[] = 1; //++ parent count for hierarchy update (-> multi hierarchy)
  }
  taxonomy_manager_update_voc($form_state['values']['voc']->vid, $parents);
  
  $term_names_array = array();
  foreach ($selected_tids as $selected_tid) {
    $term = taxonomy_term_load($selected_tid);
    $term_names_array[] = $term->name; 
  }
  $term_names = implode(', ', $term_names_array);
  $parent_names = "";
  if (count($typed_parents) == 0) {
    $parent_names = t("root level");
  }
  else {
    $parent_names = $form_state['values']['move']['parents'];
  }
  drupal_set_message(t("Terms %term_names moved to %parent_names", array('%term_names' => $term_names, '%parent_names' => $parent_names)));
}


/**
 * Validation handler for validating terms
 */
function taxonomy_manager_form_term_merge_validate($form, &$form_state) {
  $selected_tids = array();
  $selected_tids = $form_state['values']['taxonomy']['manager']['tree']['selected_terms'];
  $dest_terms = array();
  $regexp = '%(?:^|,\ *)("(?>[^"]*)(?>""[^"]* )*"|(?: [^",]*))%x';
  preg_match_all($regexp, $form_state['values']['term_merge']['dest_term'], $matches);
  $dest_terms = $matches[1];
  $error_msg = "";
  $typed_terms = taxonomy_manager_autocomplete_tags_get_tids($form_state['values']['term_merge']['dest_term'], $form_state['values']['voc']->vid, FALSE);
  
  if (!is_array($dest_terms) || count($dest_terms) == 0 || empty($dest_terms[0])) {
    form_set_error('term_merge][dest_term', t('Please enter a name into "Destination term"'));
    $form_state['rebuild'] = TRUE;
  }
  else if (count($dest_terms) > 1) {
    form_set_error('term_merge][dest_term', t('Please only enter single names into "Destination term"'));
    $form_state['rebuild'] = TRUE;
  }
      
  if (count($selected_tids) < 1) {
    form_set_error('term_merge', t("Please selecte terms you want to merge"));
    $form_state['rebuild'] = TRUE;
  }
  else if (_taxonomy_manager_check_duplicates($form_state['values']['voc']->vid, $form_state['values']['term_merge']['dest_term'], $error_msg)) {
    form_set_error('term_merge', t("Warning: Your input matches with multiple terms, because of duplicated term names. Please enter a unique term name or the term id with 'term-id:[tid]'") ." (". $error_msg .").");
    $form_state['rebuild'] = TRUE;
  }
}

/**
 * Submit handler for term merge
 */
function taxonomy_manager_form_term_merge_submit($form, $form_state) {
  $selected_tids = array();
  $selected_tids = $form_state['values']['taxonomy']['manager']['tree']['selected_terms'];
  $dest_term_ids = taxonomy_manager_autocomplete_tags_get_tids($form_state['values']['term_merge']['dest_term'], $form_state['values']['voc']->vid, TRUE);
  $dest_term_id = array_shift($dest_term_ids);
  $dest_term = taxonomy_term_load($dest_term_id['tid']);

  $term_names_array = array();
  foreach ($selected_tids as $selected_tid) {
    $term = taxonomy_term_load($selected_tid);
    $term_names_array[] = $term->name; 
  }
  $term_names = implode($term_names_array, ', ');
  if (module_exists('term_merge')) {
    term_merge($selected_tids, $dest_term, $form_state['values']['term_merge']['options']['keep_merged']);
    drupal_set_message(t("Terms %term_names merged into %dest_term", array('%term_names' => $term_names, '%dest_term' => $dest_term->name)));
  }
  else {
    drupal_set_message(t('Term merge module not found'));
  }
}

/**
 * returns TRUE if term with same name exists more often
 */
function _taxonomy_manager_check_duplicates($vid, $autocomplete_value, &$msg) {
  $regexp = '%(?:^|,\ *)("(?>[^"]*)(?>""[^"]* )*"|(?: [^",]*))%x';
  preg_match_all($regexp, $autocomplete_value, $matches);
  foreach ($matches[1] as $match) {
   $terms = array();
   $terms = taxonomy_manager_autocomplete_tags_get_tids($match, $vid, FALSE);
   if (count($terms) > 1) {
     $tids = array();
     foreach ($terms as $t) {
       $term = taxonomy_term_load($t['tid']);
       $tids[] = $term->tid . (!empty($term->language) ? '-'. check_plain($term->language) : '');
     }
     $msg .= check_plain($match) .": ". implode(", ", $tids);
     return TRUE;
   }
  }
  return FALSE;
}

function taxonomy_manager_form_double_tree_submit($form, &$form_state) {
  drupal_goto('admin/structure/taxonomy_manager/double-tree/'. $form_state['values']['voc']->machine_name .'/'. $form_state['values']['double-tree-settings']['voc2_machine_name']);
}

function taxonomy_manager_form_double_tree_disable_submit($form, &$form_state) {
  drupal_goto('admin/structure/taxonomy_manager/voc/'. $form_state['values']['voc']->machine_name);
}

/**
 * Defines a settings form.
 */
function taxonomy_manager_settings($form) {
  $form['taxonomy_manager_disable_mouseover'] = array(
    '#type' => 'checkbox',
    '#title' => t('Disable mouse-over effect for terms (weights and direct link)'),
    '#default_value' => variable_get('taxonomy_manager_disable_mouseover', 0),
    '#description' => t('Disabeling this feature speeds up the Taxonomy Manager'),
  );
  $form['taxonomy_manager_pager_tree_page_size'] = array(
    '#type' => 'select',
    '#title' => t('Pager count'),
    '#options' => array(25 => 25, 50 => 50, 75 => 75, 100 => 100, 150 => 150, 200 => 200, 250 => 250, 300 => 300, 400 => 400, 500 => 500, 1000 => 1000, 2500 => 2500, 5000 => 5000, 10000 => 10000),
   '#default_value' => variable_get('taxonomy_manager_pager_tree_page_size', 50),
    '#description' => t('Select how many terms should be listed on one page. Huge page counts can slow down the Taxonomy Manager'),
  );
  return system_settings_form($form);
}


function taxonomy_manager_term_data_form_ajax_callback($form, $form_state) {
  $commands = array();
  $commands[] = ajax_command_insert(NULL, drupal_render($form['term_data']));
  $commands[] = ajax_command_after('#term-data-close', theme('status_messages'));

  // Update tree if necessary.
  if (isset($form_state['values']['tree-update'])) {
    if (isset($form_state['values']['tree-update']['update']) && $form_state['values']['tree-update']['update']) {
      $commands[] = ajax_command_insert('#taxonomy-manager .form-item-taxonomy-manager-tree', drupal_render($form['taxonomy']['manager']['tree']));
      if (isset($form['taxonomy2'])) {
        $commands[] = ajax_command_insert('#taxonomy-manager .form-item-taxonomy2-manager-tree', drupal_render($form['taxonomy2']['manager']['tree']));
      }
    }
    else if (isset($form_state['values']['tree-update']['update-name'])) {
       $selector = 'a.term-data-link-id-'. $form_state['values']['tree-update']['update-name']['tid'];
       $commands[] = ajax_command_html($selector, '<span>' . $form_state['values']['tree-update']['update-name']['name'] . '</span>');
    }
  }

  return array('#type' => 'ajax', '#commands' => $commands);
}


function taxonomy_manager_term_data_form_submit($form, &$form_state) {
  $old_term = taxonomy_term_load($form_state['values']['tid']);
  $term = taxonomy_form_term_submit_build_taxonomy_term($form, $form_state);
  if ($term->weight != $old_term->weight) {
    form_set_value(array('#parents' => array('tree-update')), array('update' => TRUE), $form_state); 
  }
  else if ($term->name != $old_term->name) {
    form_set_value(array('#parents' => array('tree-update')), array('update-name' => array('name' => $term->name, 'tid' => $term->tid)), $form_state);
  }

  taxonomy_term_save($term);
  drupal_set_message("Updated term data");
  $form_state['rebuild'] = TRUE;
}



function taxonomy_manager_term_data_form_validate_parents_add($form, &$form_state) {
  $term_data = $form_state['values'];  
  $term = taxonomy_term_load($term_data['tid']);
  
  $parent_names = $term_data['parents']['op']['add'];
  $typed_terms = taxonomy_manager_autocomplete_tags_get_tids($parent_names, $term->vid, FALSE);
  
  $error_msg = "";

  if ($parent_names == "") {
    form_set_error('parents][op][add', t("Parent field is required."));
  }
  else if (_taxonomy_manager_check_duplicates($term->vid, $parent_names, $error_msg)) {
    form_set_error('parents][op][add', t("Warning: Your input matches with multiple terms, because of duplicated term names. Please enter a unique term name or the term id with 'term-id:[tid]'" ." (!error_msg).", array('!error_msg' => $error_msg)));
  }
  else if(!taxonomy_manager_check_language($term->vid, array($term->tid), $typed_terms)) {
    form_set_error('parents][op][add', t("Terms must be of the same language."));
  }
  else {
    //validation for consistent hierarchy
    $parents = array();
    foreach ($typed_terms as $parent_info) {
      $parents[$parent_info['tid']] = $parent_info['tid'];
    }
    if (!taxonomy_manager_check_circular_hierarchy(array($term->tid => $term->tid), $parents)) {
      form_set_error('parents][op][add', t('Invalid parent. The resulting hierarchy would contain circles, which is not allowed. A term cannot be a parent of itself.'));
    }  
  }
}

function taxonomy_manager_term_data_form_submit_parents_add($form, &$form_state) {
  $term_data = $form_state['values'];  
  $term = taxonomy_term_load($term_data['tid']);
  
  $parent_names = $term_data['parents']['op']['add'];

  $typed_terms = taxonomy_manager_autocomplete_tags_get_tids($parent_names, $term->vid, TRUE); //insert terms

  foreach ($typed_terms as $parent_info) {
    db_insert('taxonomy_term_hierarchy')
      ->fields(array('tid', 'parent'))
      ->values(array('tid' => $term->tid, 'parent' => $parent_info['tid']))
      ->execute();
  }
  //remove root level entry
  db_delete('taxonomy_term_hierarchy')
    ->condition('tid', $term->tid)
    ->condition('parent', 0)
    ->execute();  
    
  taxonomy_term_save($term); //executes hooks and clears caches
  taxonomy_manager_update_voc($term->vid, taxonomy_get_parents($term->tid)); // update hierarchy settings, if necessary
  drupal_set_message(t('Updated hierarchy'));
  $form_state['rebuild'] = TRUE;
  form_set_value(array('#parents' => array('tree-update')), array('update' => TRUE), $form_state); 
}


function taxonomy_manager_term_data_form_validate_parents_remove($form, &$form_state) {
  $splits = explode('][', $form_state['triggering_element']['#name']);
  if (count($splits) == 3 && is_numeric($splits[1])) {
    form_set_value(array('#parents' => array('remove_parent_tid')), $splits[1], $form_state);
  }
  else {
    form_set_error('parents', t('Invalid term selection.'));
  }
}

function taxonomy_manager_term_data_form_submit_parents_remove($form, &$form_state) {
  $term = taxonomy_term_load($form_state['values']['tid']);
  
  $parent = $form_state['values']['remove_parent_tid'];
  $parents = taxonomy_get_parents($term->tid);
  if (count($parents) == 1) {
    //ensure that a term has a least parent 0
    db_delete('taxonomy_term_hierarchy')
      ->condition('tid', $term->tid)
      ->condition('parent', 0)
      ->execute();
    db_insert('taxonomy_term_hierarchy')
      ->fields(array('tid', 'parent'))
      ->values(array('tid' => $term->tid, 'parent' => 0))
      ->execute();
  }
  db_delete('taxonomy_term_hierarchy')
    ->condition('tid', $term->tid)
    ->condition('parent', $parent)
    ->execute();
  
  taxonomy_term_save($term); //executes hooks and clears caches
  drupal_set_message(t("Removed parent"));
  $form_state['rebuild'] = TRUE;
  form_set_value(array('#parents' => array('tree-update')), array('update' => TRUE), $form_state); 
}

function taxonomy_manager_double_tree_ajax_callback($form, $form_state) {
  $commands = array();
  $commands[] = ajax_command_prepend(NULL, theme('status_messages'));
  if ($form_state['values']['double_tree_values']['update_tree_left']) {
    $commands[] = ajax_command_insert('#taxonomy-manager .form-item-taxonomy-manager-tree', drupal_render($form['taxonomy']['manager']['tree']));
  }
  if ($form_state['values']['double_tree_values']['update_tree_right']) {
    $commands[] = ajax_command_insert('#taxonomy-manager .form-item-taxonomy2-manager-tree', drupal_render($form['taxonomy2']['manager']['tree']));
  }
  return array('#type' => 'ajax', '#commands' => $commands);
}

function taxonomy_manager_double_tree_move_validate($form, &$form_state) {
  $values = array();

  if (strpos($form_state['input']['_triggering_element_name'], 'right') !== FALSE) {
    //move right
    $values = array(
      'selected_parents' => $form_state['values']['taxonomy2']['manager']['tree']['selected_terms'], 
      'selected_terms' => $form_state['values']['taxonomy']['manager']['tree']['selected_terms'], 
      'selected_terms_direct_parents' => $form_state['values']['taxonomy']['manager']['tree']['selected_terms_direct_parents'],
      'left_tree_expand_terms' => $form_state['values']['taxonomy']['manager']['tree']['selected_terms_direct_parents'],
      'right_tree_expand_terms' => $form_state['values']['taxonomy2']['manager']['tree']['selected_terms'],
    );
  }
  else {
    //move left
    $values = array(
      'selected_parents' => $form_state['values']['taxonomy']['manager']['tree']['selected_terms'], 
      'selected_terms' => $form_state['values']['taxonomy2']['manager']['tree']['selected_terms'], 
      'selected_terms_direct_parents' => $form_state['values']['taxonomy2']['manager']['tree']['selected_terms_direct_parents'],
      'left_tree_expand_terms' => $form_state['values']['taxonomy']['manager']['tree']['selected_terms'],
      'right_tree_expand_terms' => $form_state['values']['taxonomy2']['manager']['tree']['selected_terms_direct_parents'],
    );
  }
  
  $values['update_tree_right'] = TRUE;
  $values['update_tree_left'] = TRUE;
  
  if (!count($values['selected_terms'])) {
    form_set_error('', t("No terms selected."));
  }
  else if (is_array($values['selected_parents']) && count($values['selected_parents'])) {
    $p_array = array();
    foreach ($values['selected_parents'] as $parent) {
      $p_array[$parent]['tid'] = $parent;
    }
    if (!taxonomy_manager_check_language($form_state['values']['voc']->vid, $values['selected_terms'], $p_array)) {
      form_set_error('', t("Terms must be of the same language."));
    }
    else if (!taxonomy_manager_check_circular_hierarchy($values['selected_terms'], $values['selected_parents'])) {
      form_set_error('', t('Invalid parent. The resulting hierarchy would contain circles, which is not allowed. A term cannot be a parent of itself.'));
    }
  }
  form_set_value(array('#parents' => array('double_tree_values')), $values, $form_state);
}

function taxonomy_manager_double_tree_move_submit($form, &$form_state) {
  $selected_terms = $form_state['values']['double_tree_values']['selected_terms'];
  $selected_parents = $form_state['values']['double_tree_values']['selected_parents'];
  $selected_terms_direct_parents = $form_state['values']['double_tree_values']['selected_terms_direct_parents'];
  
  $selected_terms_names = array();
  
  foreach ($selected_terms as $tid) {
    $selected_term = taxonomy_term_load($tid);
    $selected_terms_names[] = $selected_term->name;
    
    //reset all parents, except the direct parent in the tree
    $term_parents = taxonomy_get_parents($tid);
    $term_parents_array = array();
    $direct_parent = isset($selected_terms_direct_parents[$tid]) ? $selected_terms_direct_parents[$tid] : 0;
    foreach ($term_parents as $term_parent) {
      if ($direct_parent != $term_parent->tid) {
        $term_parents_array[$term_parent->tid] = $term_parent->tid; 
      } 
    }
    $selected_parent_names = array();
    if (count($selected_parents)) {
      foreach ($selected_parents as $parent) {
        $term = taxonomy_term_load($parent);
        $selected_parent_names[] = $term->name;
        $term_parents_array[$term->tid] = $term->tid;  
      }
    }
    if (count($term_parents_array) == 0) {
      $term_parents_array[0] = 0;
    }
    taxonomy_manager_move($term_parents_array, array($tid), array('keep_old_parents' => FALSE));
    taxonomy_manager_update_voc($form_state['values']['voc']->vid, $term_parents_array);  
  }
  
  $term_names = implode(', ', $selected_terms_names);
  
  if (count($selected_parents) == 0) {
    drupal_set_message(t("Removed current parent form terms %terms.", array('%terms' => $term_names)));
  }
  else {
    drupal_set_message(t("Terms %terms moved to parents %parents.", array('%terms' => $term_names, '%parents' => implode(', ', $selected_parent_names))));
  }
  $form_state['rebuild'] = TRUE;
}

function taxonomy_manager_double_tree_switch_validate($form, &$form_state) {
  $values = array();
  
  if (strpos($form_state['input']['_triggering_element_name'], 'right') !== FALSE) {
    //move right
    $values = array(
      'selected_parents' => $form_state['values']['taxonomy2']['manager']['tree']['selected_terms'], 
      'selected_terms' => $form_state['values']['taxonomy']['manager']['tree']['selected_terms'], 
      'left_tree_expand_terms' => $form_state['values']['taxonomy']['manager']['tree']['selected_terms_direct_parents'],
      'right_tree_expand_terms' => $form_state['values']['taxonomy']['manager']['tree']['selected_terms'],
      'voc1' => $form_state['values']['voc']->vid,
      'voc2' => $form_state['values']['voc2']->vid,
    );
  }
  else {
    //move left
    $values = array(
      'selected_parents' => $form_state['values']['taxonomy']['manager']['tree']['selected_terms'], 
      'selected_terms' => $form_state['values']['taxonomy2']['manager']['tree']['selected_terms'], 
      'left_tree_expand_terms' => $form_state['values']['taxonomy2']['manager']['tree']['selected_terms'],
      'right_tree_expand_terms' => $form_state['values']['taxonomy2']['manager']['tree']['selected_terms_direct_parents'],
      'voc1' => $form_state['values']['voc2']->vid,
      'voc2' => $form_state['values']['voc']->vid,
    );
  }
  $values['update_tree_right'] = TRUE;
  $values['update_tree_left'] = TRUE;
  
  if (!count($values['selected_terms'])) {
    form_set_error('', t("No terms selected."));
  }
  form_set_value(array('#parents' => array('double_tree_values')), $values, $form_state);
}

function taxonomy_manager_double_tree_copy_validate($form, &$form_state) {
  taxonomy_manager_double_tree_switch_validate($form, $form_state);
  $values = $form_state['values']['double_tree_values'];
  if (strpos($form_state['input']['_triggering_element_name'], 'right') !== FALSE) {
    $form_state['values']['double_tree_values']['update_tree_left'] = FALSE;
  }
  else {
    $form_state['values']['double_tree_values']['update_tree_right'] = FALSE;
  }
}

function taxonomy_manager_double_tree_copy_submit($form, &$form_state) {
  $selected_terms = $form_state['values']['double_tree_values']['selected_terms'];
  $selected_parents = $form_state['values']['double_tree_values']['selected_parents'];
  $voc1 = $form_state['values']['double_tree_values']['voc1'];
  $voc2 = $form_state['values']['double_tree_values']['voc2'];

  $voc1 = taxonomy_vocabulary_load($voc1);
  $voc2 = taxonomy_vocabulary_load($voc2);
  
  $new_tids = taxonomy_manager_copy($selected_terms, $voc1->vid, $voc2->vid, $selected_parents);
  if (count($new_tids)) {
    $new_tid = array_pop($new_tids);
    $form_state['values']['double_tree_values']['left_tree_expand_terms'] = array($new_tid);
    $form_state['values']['double_tree_values']['right_tree_expand_terms'] = array($new_tid); 
  }
  
  $selected_terms_names = array();
  foreach ($selected_terms as $tid) {
    $term = taxonomy_term_load($tid);
    $selected_terms_names[] = $term->name;
  }
  $selected_parent_names = array();
  if (count($selected_parents)) {
    foreach ($selected_parents as $parent) {
      $term = taxonomy_term_load($parent);
      $selected_parent_names[] = $term->name;
      $term_parents_array[$term->tid] = $term->tid;  
    }
  }
  $term_names = implode(', ', $selected_terms_names);
  if (count($selected_parents) == 0) {
    drupal_set_message(t("Copied %terms to vocabulary %voc.", array('%terms' => $term_names, '%voc' => $voc2->name)));
  }
  else {
    drupal_set_message(t("Copied %terms to vocabulary %voc under parents %parents.", array('%terms' => $term_names, '%voc' => $voc2->name, '%parents' => implode(', ', $selected_parent_names))));
  }
  
  $form_state['rebuild'] = TRUE;
}

function taxonomy_manager_double_tree_switch_submit($form, &$form_state) {
  $selected_terms = $form_state['values']['double_tree_values']['selected_terms'];
  $selected_parents = $form_state['values']['double_tree_values']['selected_parents'];
  $voc1 = $form_state['values']['double_tree_values']['voc1'];
  $voc2 = $form_state['values']['double_tree_values']['voc2'];

  $voc1 = taxonomy_vocabulary_load($voc1);
  $voc2 = taxonomy_vocabulary_load($voc2);
  
  taxonomy_manager_switch($selected_terms, $voc1->vid, $voc2->vid, $selected_parents);
  
  $selected_terms_names = array();
  foreach ($selected_terms as $tid) {
    $term = taxonomy_term_load($tid);
    $selected_terms_names[] = $term->name;
  }
  $selected_parent_names = array();
  if (count($selected_parents)) {
    foreach ($selected_parents as $parent) {
      $term = taxonomy_term_load($parent);
      $selected_parent_names[] = $term->name;
      $term_parents_array[$term->tid] = $term->tid;  
    }
  }
  $term_names = implode(', ', $selected_terms_names);
  if (count($selected_parents) == 0) {
    drupal_set_message(t("Terms %terms moved to vocabulary %voc.", array('%terms' => $term_names, '%voc' => $voc2->name)));
  }
  else {
    drupal_set_message(t("Terms %terms moved to vocabulary %voc under parents %parents.", array('%terms' => $term_names, '%voc' => $voc2->name, '%parents' => implode(', ', $selected_parent_names))));
  }
  
  $form_state['rebuild'] = TRUE;
}


/*function taxonomy_manager_doube_tree_edit_translate($params, &$msg, &$is_error_msg) {
  $msg = "Operation not yet implemented";
  $is_error_msg = TRUE;
  //TODO
  /*$term1 = taxonomy_term_load(array_pop($params['selected_terms']));
  $term2 = taxonomy_term_load(array_pop($params['selected_parents']));
  $vid = $term1->vid;
  
  if (module_exists('i18ntaxonomy')) {
    if (i18ntaxonomy_vocabulary($vid) == I18N_TAXONOMY_TRANSLATE) {
      if ($term1->language == $term2->language) {
        $msg = t("Selected terms are of the same language.");
        $is_error_msg = TRUE; 
      }
      else {
        $translations = i18ntaxonomy_term_get_translations(array('tid' => $term1->tid), FALSE);
        foreach ($translations as $translation) {
          if ($translation->language == $term2->language) {
            $msg = t('Translation for this language already exists.');
            $is_error_msg = TRUE;
            break; 
          }
        }
        $translations = i18ntaxonomy_term_get_translations(array('tid' => $term2->tid), FALSE);
        foreach ($translations as $translation) {
          if ($translation->language == $term1->language) {
            $msg = t('Translation for this language already exists.');
            $is_error_msg = TRUE;
            break; 
          }
        }
      }
    }
    else {
      $msg = t("This is not a multilingual vocabulary.");
      $is_error_msg = TRUE;
    }
  }
  else {
    $msg = t("Module i18ntaxonomy not enabled.");
    $is_error_msg = TRUE;
  }
  
  if (!$is_error_msg) {
    taxonomy_manager_add_translation($term1->tid, $term2->tid);
    $msg = t("Translation for %term2 - %term1 added.", array('%term2' => $term2->name, '%term1' => $term1->name)); 
  }
}*/



/**
 * adds translation between two terms
 */
function taxonomy_manager_add_translation($tid1, $tid2) {
  //TODO
  /*$trid1 = db_result(db_query("SELECT trid FROM {term_data} WHERE tid = %d", $tid1));
  $trid2 = db_result(db_query("SELECT trid FROM {term_data} WHERE tid = %d", $tid2));
  if ($trid1 == 0 && $trid2 == 0) {
    $trid = ((int)db_result(db_query('SELECT max(trid) FROM {term_data}'))) + 1;
    db_query('UPDATE {term_data} SET trid = %d WHERE tid = %d OR tid = %d', $trid, $tid1, $tid2);
  }
  else if ($trid1 != 0 && $trid2 == 0) {
    db_query('UPDATE {term_data} SET trid = %d WHERE tid = %d', $trid1, $tid2);
  }
  else if ($trid2 != 0 && $trid1 == 0) {
    db_query('UPDATE {term_data} SET trid = %d WHERE tid = %d', $trid2, $tid1);
  }
  else {
    $trid = max($trid1, $trid2); 
    db_query('UPDATE {term_data} SET trid = %d WHERE trid = %d OR trid = %d', $trid, $trid1, $trid2);
  }*/
}

/**
 * Duplicates a term to another voc
 */
function taxonomy_manager_copy($tids, $from_voc, $to_voc, $selected_parents) {
  $new_tids = array();
  foreach ($tids as $tid) {
    $term = taxonomy_term_load($tid);
    $new_term = clone $term;
    unset($new_term->tid);
    $new_term->parent = count($selected_parents) ? $selected_parents : 0;
    $new_term->vid = $to_voc;
    taxonomy_term_save($new_term);
    $new_tids[] = $new_term->tid;
  }
  taxonomy_manager_update_voc($to_voc, $selected_parents);
  taxonomy_terms_static_reset();
  return $new_tids;
}

/**
 * Changes vocabulary of given terms and its children
 * conflicts might be possible with multi-parent terms!
 */
function taxonomy_manager_switch($tids, $from_voc, $to_voc, $parents = array()) {  
  foreach ($tids as $tid) {
    //hook to inform modules about the changes
    module_invoke_all('taxonomy_manager_term', 'switch', $tid, $from_voc, $to_voc);
    
    $children = taxonomy_get_tree($from_voc, $tid);
    $terms_to_switch = array();
    foreach ($children as $child) {
      $terms_to_switch[] = $child->tid;
    }
    $terms_to_switch[] = $tid;

    db_update('taxonomy_term_data')
      ->fields(array('vid' => $to_voc))
      ->condition('tid', $terms_to_switch, 'IN')
      ->execute();
    
    taxonomy_terms_static_reset();
    
    //delete references to parents from the old voc
    foreach ($children as $child) {
      $term_parents = taxonomy_get_parents($child->tid);
      foreach ($term_parents as $term_parent) {
        if ($term_parent->vid != $to_voc) {
          db_delete('taxonomy_term_hierarchy')
            ->condition('tid', $child->tid)
            ->condition('parent', $term_parent->tid)
            ->execute();
        } 
      }
    }
   
    //set parent of the selected term
    if (!count($parents)) {
      $parents[0] = 0;
    }
    taxonomy_manager_move($parents, array($tid));
    taxonomy_manager_update_voc($to_voc, $parents);
  }
  //TODO invoke update hook!!
  taxonomy_terms_static_reset();
}
/**
 * checks if voc has terms
 *
 * @param $vid voc id
 * @return true, if terms already exists, else false
 */
function _taxonomy_manager_voc_is_empty($vid) {
  $has_rows = (bool) db_query_range("SELECT 1 FROM {taxonomy_term_data} t INNER JOIN {taxonomy_term_hierarchy} h ON t.tid = h.tid WHERE vid = :vid AND h.parent = 0", 0, 1, array(':vid' => $vid))->fetchField();
  return !$has_rows;
}


/**
 * deletes terms from the database
 * optional orphans (terms where parent get deleted) can be deleted as well
 * 
 * (difference to taxonomy_term_delete: deletion of orphans optional)
 *
 * @param $tids array of term id to delete
 * @param $options associative array with options
 *   if $options['delete_orphans'] is true, orphans get deleted
 */
function taxonomy_manager_delete_terms($tids, $options = array()) {
  $deleted_term = array();
  $remaining_child_terms = array();

  if (!is_array($tids)) array($tids);
  while (count($tids) > 0) {
    $orphans = array();
    foreach ($tids as $tid) {      
      if ($children = taxonomy_get_children($tid)) {
        foreach ($children as $child) {
          $parents = taxonomy_get_parents($child->tid);
          if ($options['delete_orphans']) {
            if (count($parents) == 1) {
              $orphans[$child->tid] = $child->tid;
            }
            else {
              $remaining_child_terms[$child->tid] = $child;
            }
          }
          else {
            $remaining_child_terms[$child->tid] = $child;
            db_delete('taxonomy_term_hierarchy')
              ->condition('tid', $child->tid)
              ->condition('parent', $tid)
              ->execute();
              
            if (count($parents) == 1) {
              $is_root = (bool) db_query_range("SELECT 1 FROM {taxonomy_term_hierarchy} h WHERE h.tid = :tid AND h.parent = 0", 0, 1, array(':tid' => $child->tid))->fetchField();
              if (!$is_root) {
                db_insert('taxonomy_term_hierarchy')
                  ->fields(array('tid', 'parent'))
                  ->values(array('tid' => $child->tid, 'parent' => 0))
                  ->execute();
              }
            }
          }
        }
      }
      
      $term = taxonomy_term_load($tid);
      $deleted_terms[] = $term;
      
      db_delete('taxonomy_term_data')
        ->condition('tid', $tid)
        ->execute();
      db_delete('taxonomy_term_hierarchy')
        ->condition('tid', $tid)
        ->execute();

      field_attach_delete('taxonomy_term', $term);
      module_invoke_all('taxonomy_term_delete', $term);

      // Reset cache after each deletion as it might affect parents/children information that is used later on.
      taxonomy_terms_static_reset();
    }
    $tids = $orphans;
  } 
  return array('deleted_terms' => $deleted_terms, 'remaining_child_terms' => $remaining_child_terms);
}


/**
 * moves terms in hierarchies to other parents
 *
 * @param $parents 
 *   array of parent term ids to where children can be moved
 *   array should only contain more parents if multi hiearchy enabled 
 *   if array contains 0, terms get placed to first (root) level
 * @param $children
 *   array of term ids to move
 * @param $options
 *   array of additional options for moving
 *   'keep_old_parents': if true, exisiting parents doesn't get deleted (only possible with multi hierarchies)
 */
function taxonomy_manager_move($parents, $children, $options = array()) {
  if (!is_array($parents)) array($parents);
  
  foreach ($children as $child) {
    if (!isset($options['keep_old_parents']) || !$options['keep_old_parents']) {
      db_delete('taxonomy_term_hierarchy')
        ->condition('tid', $child)
        ->execute();
    }
    foreach ($parents as $parent) {
      db_delete('taxonomy_term_hierarchy')
        ->condition('tid', $child)
        ->condition('parent', $parent)
        ->execute(); //prevent duplicated sql errors
      db_insert('taxonomy_term_hierarchy')
        ->fields(array('tid', 'parent'))
        ->values(array('tid' => $child, 'parent' => $parent))
        ->execute();
    }
  }
  taxonomy_terms_static_reset();
}




/**
 * helper function for getting out of term ids from autocomplete fields
 * non-exsiting terms get inserted autmatically
 * the input gets parsed for term names, optional a term id can be directly passed with prefixing the input with 'term-id:'
 *
 * @param $typed_input input string of form field
 * @param $vid vocabulary id
 * @param $insert_new TRUE if non-existing terms should be inserted
 * @param $lang define the language (optional)
 * @return array of term ids
 */
function taxonomy_manager_autocomplete_tags_get_tids($typed_input, $vid, $insert_new = TRUE, $lang = NULL) {
  $tids = array();
  
  $regexp = '%(?:^|,\ *)("(?>[^"]*)(?>""[^"]* )*"|(?: [^",]*))%x';
  preg_match_all($regexp, $typed_input, $matches);
  $typed_terms = array_unique($matches[1]);

  foreach ($typed_terms as $typed_term) {
    $typed_term = str_replace('""', '"', preg_replace('/^"(.*)"$/', '\1', $typed_term));
    $typed_term = trim($typed_term);
    if ($typed_term != "") { 
      if (substr($typed_term, 0, 8) == "term-id:") {
        $id = substr($typed_term, 8);
        $term = taxonomy_term_load($id);
        if ($term->vid == $vid) {
          $tids[$term->tid]['tid'] = $term->tid;
        }
      }
      else {
        $possibilities = taxonomy_get_term_by_name($typed_term);
        $typed_term_tid = NULL; // tid match if any.
        foreach ($possibilities as $possibility) {
          if ($possibility->vid == $vid && !($lang && $lang != $possibility->language)) {
            $typed_term_tid = $possibility->tid;
            $tids[$typed_term_tid]['tid'] = $typed_term_tid;
          }
        }

        if (!$typed_term_tid && $insert_new) {
          $edit = array('vid' => $vid, 'name' => $typed_term);
          $edit = new stdClass;
          $edit->name = $typed_term;
          $edit->vid = $vid;
          
          $status = taxonomy_term_save($edit);
          if (module_exists('i18ntaxonomy') && $lang != "") {
            _i18ntaxonomy_term_set_lang($edit->tid, $lang);
          }
          $typed_term_tid = $edit->tid;
          $tids[$typed_term_tid]['tid'] = $typed_term_tid;
          $tids[$typed_term_tid]['new'] = TRUE;
        }
      }
    }
  }
  return $tids;
}

/**
 * similar to taxonomy_manager_autocomplete_tags_get_tids, but used for searching terms
 * takes synonyms and selected subtrees into account
 *
 * @param $typed_input input string of form field
 * @param $vid vocabulary id
 * @param $include_synonyms TRUE if search should include synonyms
 * @param $parents array of parents
 * @return array of term ids
 */
function taxonomy_manager_autocomplete_search_terms($typed_input, $vid, $include_synonyms = FALSE, $parents = array(), $language = NULL) {
  $tids = array();
  
  if ($language != NULL && $language == "no language") {
    $language = "";
  }
  
  $regexp = '%(?:^|,\ *)("(?>[^"]*)(?>""[^"]* )*"|(?: [^",]*))%x';
  preg_match_all($regexp, $typed_input, $matches);
  $typed_terms = array_unique($matches[1]);

  foreach ($typed_terms as $typed_term) {
    $typed_term = str_replace('""', '"', preg_replace('/^"(.*)"$/', '\1', $typed_term));
    $typed_term = trim($typed_term);
    if ($typed_term == "") { 
      continue; 
    }
    //TODO
    /*if ($include_synonyms) {
      if ($language != NULL) {
        $search_sql = db_query("SELECT td.tid FROM {term_data} td LEFT JOIN {term_synonym} ts ON td.tid = ts.tid WHERE td.vid = %d AND (td.name = '%s' OR ts.name = '%s') AND td.language = '%s'", $vid, $typed_term, $typed_term, $language);
      }
      else {
        $search_sql = db_query("SELECT td.tid FROM {term_data} td LEFT JOIN {term_synonym} ts ON td.tid = ts.tid WHERE td.vid = %d AND (td.name = '%s' OR ts.name = '%s')", $vid, $typed_term, $typed_term);
      }
      
    }
    else {
      if ($language != NULL) {
        $search_sql = db_query("SELECT td.tid FROM {term_data} td WHERE td.vid = %d AND td.name = '%s' AND td.language = '%s'", $vid, $typed_term, $language);
      }
      else {
        $search_sql = db_query("SELECT td.tid FROM {term_data} td WHERE td.vid = %d AND td.name = '%s'", $vid, $typed_term);
      }
    }*/
    $terms = taxonomy_get_term_by_name($typed_term);
    foreach ($terms as $term) {
      if ($term->vid == $vid) {
        $tids[] = $term->tid;
      }
    }
    
    if (count($parents)) {
      $filtered_tids = array();
      foreach ($tids as $tid) {
        $parents_all = taxonomy_get_parents_all($tid);
        foreach ($parents_all as $key => $parent) {
          if (in_array($parent->tid, $parents)) {
            $filtered_tids[] = $tid;
            break; 
          } 
        }
      }
      $tids = $filtered_tids;
    }
    
  }
  return $tids;
}


/**
 * callback for updating weights
 * data send through AJAX, $_POST
 * $_POST[$tid] => $weight
 *
 */
function taxonomy_manager_update_weights() {
  $weights = $_POST;
  if (is_array($weights)) {
    foreach ($weights as $tid => $weight) {
      if (is_numeric($tid) && is_numeric($weight)) {
        $term = taxonomy_term_load($tid);
        $term->weight = $weight;
        taxonomy_term_save($term);
        //db_query("UPDATE {term_data} SET weight = %d WHERE tid = %d", $weight, $tid);
      }
    }
  }
  exit();
}

/** 
 * AJAX Callback that returns the CSV Output
 */
function taxonomy_manager_export() {
  $edit = $_POST;
  $depth = empty($edit['depth']) ? NULL : $edit['depth'];
  $output = taxonomy_manager_export_csv($edit['delimiter'], $edit['vid'], $edit['tid'], $depth, array($edit['option'] => TRUE));
  print $output;
  exit();
}

/** 
 * Generates the CVS Ouput
 */
function taxonomy_manager_export_csv($delimiter = ";", $vid, $selected_tid = 0, $depth = -1, $options = array()) {
  $output = "";
  $tree = taxonomy_manager_export_get_tree($vid, $selected_tid, $depth, $options);  
  foreach ($tree as $term) {
    $array = array();
    $array[] = '"'. $term->vid .'"';
    $array[] = '"'. $term->tid .'"';
    $array[] = '"'. $term->name .'"';
    $array[] = '"'. $term->description .'"';
    foreach ($term->parents as $parent) {
      $array[] = '"'. $parent .'"';
    }
    $output .= implode($delimiter, $array) ."\n";
  }
  drupal_json_output(array('csv' => $output));
}

/**
 * Helper for cvs export to get taxonomy tree
 */
function taxonomy_manager_export_get_tree($vid, $selected_tid, $depth, $options) {
  $tree = array();

  if (isset($options['whole_voc'])) {
    $tree = taxonomy_get_tree($vid, 0, $depth);
  }
  else if (isset($options['children']) && $selected_tid) {
    $tree = taxonomy_get_tree($vid, $selected_tid, $depth);
  }
  else if (isset($options['root_terms'])) {
    $tree = taxonomy_get_tree($vid, 0, 1);
  }
  return $tree;
}

/**
 * Helper function that updates the hierarchy settings of a voc
 */
function taxonomy_manager_update_voc($vid, $parents = array()) {
  $voc = taxonomy_vocabulary_load($vid);
  if ($voc->vid == $vid) {
    $current_hierarchy = count($parents);
    if ($current_hierarchy > 2) {
       $current_hierarchy = 2;
    }
    if ($current_hierarchy > $voc->hierarchy) {
      $voc->hierarchy = $current_hierarchy;
      taxonomy_vocabulary_save($voc);
    }
  }
}

/**
 * Retrieve a pipe delimited string of autocomplete suggestions (+synonyms)
 */
function taxonomy_manager_autocomplete_load($vid, $string = '') {  
  // The user enters a comma-separated list of tags. We only autocomplete the last tag.
  $array = drupal_explode_tags($string);

  // Fetch last tag
  $last_string = drupal_strtolower(trim(array_pop($array)));
  $matches = array();
  if ($last_string != '') {
    $query = db_select('taxonomy_term_data', 't');
    $query->addTag('translatable');
    $query->addTag('term_access');

    // Do not select already entered terms.
    if (!empty($tags_typed)) {
      $query->condition('t.name', $array, 'NOT IN');
    }
    // Select rows that match by term name.
    $tags_return = $query
      ->fields('t', array('tid', 'name'))
      ->condition('t.vid', $vid)
      ->condition('t.name', '%' . db_like($last_string) . '%', 'LIKE')
      ->range(0, 30)
      ->execute()
      ->fetchAllKeyed();

    $prefix = count($array) ? '"'. implode('", "', $array) .'", ' : '';
    
    $matches = array();
    foreach ($tags_return as $tid => $name) {
      $n = $name;
      // Term names containing commas or quotes must be wrapped in quotes.
      if (strpos($name, ',') !== FALSE || strpos($name, '"') !== FALSE) {
        $n = '"' . str_replace('"', '""', $name) . '"';
      }
      else {
        $matches[$prefix . $n] = check_plain($name);
      }
    }
  }

  drupal_json_output($matches);
}


/**
 * theme function for taxonomy manager form
 */
function theme_taxonomy_manager_form($variables) {
  $form = $variables['form'];
  $pager = theme('pager');
  $tree = drupal_render($form['taxonomy']);
  $term_data = drupal_render($form['term_data']);
  $top = drupal_render_children($form);
  $output = $top . $pager;
  $output .= '<div id="taxonomy-manager" class="admin clear-block">';
  $output .= '<div id="taxonomy-manager-tree-outer-div" class="left clear-block">';
  $output .= $tree;
  $output .= '</div>';
  $output .= '<div id="taxonomy-term-data" class="right clear-block">';
	$output .= $term_data;
	$output .= '</div>';
  $output .= '</div>';
  return $output;
}

/**
 * theme function for taxonomy manager form
 */
function theme_taxonomy_manager_double_tree_form($variables) {
  $form = $variables['form'];
  
  $pager = theme('pager');
  $tree1 = drupal_render($form['taxonomy']);
  $tree2 = drupal_render($form['taxonomy2']);
  $operations = drupal_render($form['double-tree']);
  $term_data = drupal_render($form['term_data']);
  $top = drupal_render_children($form);
  $output = $top . $pager;
  $output .= '<div id="taxonomy-manager" class="admin clear-block">';
  $output .= '<div id="taxonomy-manager-tree-outer-div" class="left clear-block">';
  $output .= $tree1;
  $output .= '</div>';
  
  $output .= '<div id="taxonomy-manager-double-tree-operations">';
  $output .= $operations;
  $output .= '</div>';
  
  $output .= '<div id="taxonomy2-manager-tree-outer-div" class="left clear-block">';
  $output .= $tree2;
  $output .= '</div>';
  $output .= '<div id="taxonomy-term-data" class="term-data-overlay">'; 	 
	$output .= $term_data; 	 
	$output .= '</div>';
  $output .= '</div>';

  return $output;
}



/**
 * themes a real button form type (no form submit)
 */
function theme_no_submit_button($variables) {
  $element = $variables['element'];
  /* Make sure not to overwrite classes.
  if (isset($element['#attributes']['class'])) {
    $element['#attributes']['class'][] = 'form-'. $element['#button_type'] .' '. $element['#attributes']['class'];
  }
  else {
    $element['#attributes']['class'][] = 'form-'. $element['#button_type'];
  }*/
  return '<input type="button" '. (empty($element['#name']) ? '' : 'name="'. $element['#name'] .'" ')  .'id="'. $element['#id'] .'" value="'. check_plain($element['#value']) .'" '. drupal_attributes($element['#attributes']) ." />\n";
}

/**
 * themes a image type button
 */
function theme_taxonomy_manager_image_button($element) {
  
  //TODO still used? 
  
  //Make sure not to overwrite classes
  if (isset($element['#attributes']['class'])) {
    $element['#attributes']['class'] = 'form-'. $element['#button_type'] .' '. $element['#attributes']['class'];
  }
  else {
    $element['#attributes']['class'] = 'form-'. $element['#button_type'];
  }

  // here the novelty begins: check if #button_type is normal submit button or image button
  $return_string = '<input ';
  if ($element['#button_type'] == 'image') {
    $return_string .= 'type="image" ';
  }
  else {
    $return_string .= 'type="submit" ';
  }
  $return_string .= (empty($element['#id']) ? '' : 'id="'. $element['#id'] .'" ');
  $return_string .= (empty($element['#name']) ? '' : 'name="'. $element['#name'] .'" ');
  $return_string .= 'value="'. check_plain($element['#value']) .'" ';
  $return_string .= drupal_attributes($element['#attributes']) ." />\n";
 
  return $return_string;

}

function theme_taxonomy_manager_term_data_extra($variables) {
  $element = $variables['element'];

  $rows = array();
  $headers = array();
  foreach (element_children($element['headers']) as $key) {
    if (is_array($element['headers'][$key])) {
      $headers[] = drupal_render($element['headers'][$key]);
    }
  }
  foreach (element_children($element['data']) as $tid) {
    $row = array();
    foreach (element_children($element['data'][$tid]) as $key) {
      if (is_array($element['data'][$tid][$key])) {
        $row[] = array(
          'data' => drupal_render($element['data'][$tid][$key]) . drupal_render($element['data'][$tid][$key]['tid']), 
          'class' => isset($element['data'][$tid][$key]['#row-class']) ? $element['data'][$tid][$key]['#row-class'] : '',
        );
      }
    }
    $rows[] = $row;
  }
  $row = array();
  foreach (element_children($element['op']) as $key) {
    if (is_array($element['op'][$key])) {
      //$row[] = drupal_render($element['op'][$key]);
      $row[] = array(
          'data' => drupal_render($element['op'][$key]),
          'class' => isset($element['op'][$key]['#row-class']) ? $element['op'][$key]['#row-class'] : '',
        );
    } 
  }
  $rows[] = $row;
  return theme('table', array('header' => $headers, 'rows' => $rows));
}
