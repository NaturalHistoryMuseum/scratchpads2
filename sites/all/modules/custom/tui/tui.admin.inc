<?php

/**
 * Callback for the tree editor form.
 */
function tui_editor_form($vocabulary){
  $tids = array();
  $all_tids = array();
  module_load_include('tree.inc', 'tui');
  $tree = tui_get_tree($vocabulary->vid, $tids, $all_tids);
  echo theme('page', theme('tui_page', $tree, $vocabulary, $all_tids), variable_get('tui_show_side_bars', TRUE));
}


/**
 * Callback for the menu item admin/content/taxonomy/edit/tui/%vid
 */
function tui_admin_vocabulary_edit($vocabulary){
  $tids = array();
//  if(arg(6)){
//    $tids = explode(',', arg(6));
//  }
  $all_tids = array();
  module_load_include('tree.inc', 'tui');
  $tree = tui_get_tree($vocabulary->vid, $tids, $all_tids);
  return theme('tui_page', $tree, $vocabulary, $all_tids);
}

/**
 * TUI settings page
 */
function tui_admin_settings(){
  $form['tui_show_side_bars'] = array(
    '#type' => 'checkbox',
    '#title' => 'Show sidebars',
    '#default_value' => variable_get('tui_show_side_bars', TRUE),
    '#description' => 'Uncheck this box to hide the sidebars on the taxonomy editor page, providing more room for the editor interface.'
  );
  return system_settings_form($form);
}

/**
 * Theme for above callback.
 */
function theme_tui_page($taxonomy_tree, $vocabulary, $all_tids){
  drupal_add_css(drupal_get_path('module', 'tui') . '/lib/bt/jquery.bt.css');
  drupal_add_css(drupal_get_path('module', 'tui') . '/tui.css');
  drupal_add_js(drupal_get_path('module', 'tui') . '/tui.js');
  drupal_add_js(drupal_get_path('module', 'tui') . '/lib/bt/jquery.bt.js');
  drupal_set_html_head('<!--[if IE]><script type="text/javascript" src="' . base_path() . drupal_get_path('module', 'tui') . '/lib/explorercanvas/excanvas.js"></script><![endif]-->');
  drupal_add_js(drupal_get_path('module', 'tui') . '/lib/explorercanvas/excanvas.js');
  drupal_add_js(array(
    'tui' => array(
      'opentids' => $all_tids,
      'vocabulary' => arg(5),
      'callbacks' => array(
        'form' => url('tui/getform', array(
          'absolute' => TRUE
        )),
        'full_tree' => url('tui/gettree', array(
          'absolute' => TRUE
        )),
        'tree' => url('tui/getchildren', array(
          'absolute' => TRUE
        )),
        'move' => url('tui/moveterm', array(
          'absolute' => TRUE
        )),
        'page' => url('admin/content/taxonomy/edit/tui/' . arg(5), array(
          'absolute' => TRUE
        )),
        'dlete' => url('tui/deleteterm', array(
          'absolute' => TRUE
        )),
        'nextorprevious' => url('tui/nextorprevious', array(
          'absolute' => TRUE
        )),
        'search' => url('tui/search', array(
          'absolute' => TRUE
        )),
        'undo' => url('tui/undo', array(
          'absolute' => TRUE
        )),
        'restoresort' => url('tui/restoresort', array(
          'absolute' => TRUE
        ))
      ),
      'dialog' => array(
        'dlete' => array(
          'title' => t('Delete?'),
          'content' => '<p>' . t('Are you sure you want to delete') . '<br/> <span id="tui-dialog-term-name"></span>?</p>'
        )
      )
    )
  ), 'setting');
  // Add the JS required.
  variable_set('jquery_update_compression_type', 'none');
  jquery_ui_add(array(
    'ui.tabs',
    'ui.draggable',
    'ui.dialog',
    'ui.droppable',
    'ui.resizable',
    'effects.highlight'
  ));
  drupal_add_css(drupal_get_path('module', 'jquery_ui') . '/jquery.ui/themes/smoothness/ui.all.css');
  // We add the form, so that all the extras (collapse.js etc) are added.  We 
  // then hide the form.
  module_load_include('admin.inc', 'taxonomy');
  drupal_set_title(t('Editing %vocabulary', array(
    '%vocabulary' => $vocabulary->name
  )));
  return '<div id="tui">
  <div id="tui-tree">
    <div id="tui-tree-container">
      <div id="tui-tree-links">
        <p>
          <img id="tui-previous" src="' . base_path() . drupal_get_path('module', 'tui') . '/images/previous.png" alt="' . t('Previous') . '" title="' . t('Previous') . '"/>
          <img id="tui-add" src="' . base_path() . drupal_get_path('module', 'tui') . '/images/add.png" alt="' . t('Add term') . '" title="' . t('Add term') . '"/>
          <img id="tui-delete" src="' . base_path() . drupal_get_path('module', 'tui') . '/images/delete.png" alt="' . t('Delete term') . '" title="' . t('Delete term') . '"/>
          <img id="tui-resort" src="' . base_path() . drupal_get_path('module', 'tui') . '/images/resort.png" alt="' . t('Sort&nbsp;children&nbsp; alphabetically') . '" title="' . t('Sort&nbsp;children&nbsp;alphabetically') . '"/>
          <img id="tui-undo" src="' . base_path() . drupal_get_path('module', 'tui') . '/images/undo.png" alt="' . t('Undo move') . '" title="' . t('Undo move') . '"/>
          <img id="tui-import" src="' . base_path() . drupal_get_path('module', 'tui') . '/images/import.png" alt="' . t('Import') . '" title="' . t('Import') . '"/>
          <img id="tui-link" src="' . base_path() . drupal_get_path('module', 'tui') . '/images/link.png" alt="' . t('Link to this page') . '" title="' . t('Link to this page') . '"/>
          <img id="tui-search" src="' . base_path() . drupal_get_path('module', 'tui') . '/images/search.png" alt="' . t('Search') . '" title="' . t('Search') . '"/>
          <img id="tui-next" src="' . base_path() . drupal_get_path('module', 'tui') . '/images/next.png" alt="' . t('Next') . '" title="' . t('Next') . '"/>
        </p>
        <div id="tui-search-form-container">
          <div class="tui-modal"></div>
          <div class="tui-form">' . drupal_get_form('tui_search_form', arg(5)) . '</div>
        </div>
        <div id="tui-link-box">
          <div id="tui-link-subbox">
            <p>' . t('Copy and paste the following link into an email, or webpage') . '</p>
            <textarea id="tui-link-back">' . url('admin/content/taxonomy/edit/tui/' . arg(5) . '/' . arg(6), array(
    'absolute' => TRUE
  )) . '</textarea>
          </div>
        </div>
      </div>
      <div id="tui-tree-subcontainer">
        ' . theme('tui_tree', $taxonomy_tree) . '
      </div>
    </div>
    <!-- <div id="tui-tree-container">
      <div id="tui-tree-links">
      </div>
      <div id="tui-tree-subcontainer">
      </div>
    </div> -->
  </div>
  <div id="tui-form">
    <div>
      <h2 id="tui-name-editing"></h2>
    </div>
    <div id="tui-form-container">
      <div id="tui-form-noform">' . tui_help('admin/content/taxonomy/edit/tui', array()) . '</div>
      <div class="hidden">' . drupal_get_form('taxonomy_form_term', $vocabulary, array()) . '</div>
    </div>
  </div>
  <div style="clear:both"/></div>
</div>';
}

/**
 * TUI autocomplete search form
 */
function tui_search_form(&$form_statge, $vid){
  return array(
    'tui-search-input' => array(
      '#autocomplete_path' => 'tui/autocomplete/' . $vid,
      '#size' => FALSE,
      '#type' => 'textfield',
      '#title' => '',
      '#attributes' => array(
        'onkeypress' => 'return Drupal.tui.search_return_press(event)'
      )
    ),
    'tui-search-input-button' => array(
      '#type' => 'button',
      '#value' => '',
      '#attributes' => array(
        'onclick' => 'return Drupal.tui.search_submit()'
      )
    )
  );
}

/**
 * Callback to restore the sort
 * 
 * FIXME - The following should probably have some sort of token, to ensure that
 * it has not been eroneously called.  This isn't too much of an issue though, 
 * as reordering of terms is unlikely to be a cracker's first target.
 */
function tui_restore_sort($tid_id){
  // Get the id from the tid HTML id
  $tid = array_pop(explode("-", $tid_id));
  if(is_numeric($tid)){
    db_query('UPDATE {term_data} SET weight = 0 WHERE tid IN (SELECT tid FROM {term_hierarchy} WHERE parent = %d)', $tid);
  }
  drupal_json(array());
  exit;
}
/**
 * Autocomplete for above form
 */
function tui_autocomplete($vid, $search){
  $matches = array();
  if($search != ''){
    $result = db_query_range(db_rewrite_sql("SELECT t.tid, t.name FROM {term_data} t WHERE t.vid = %d AND (LOWER(t.name) LIKE LOWER('%s%%') OR LOWER(t.name) LIKE LOWER('%% %s%%'))", 't', 'tid'), $vid, $search, $search, 0, 10);
    while($term = db_fetch_object($result)){
      $n = $term->name;
      // Commas and quotes in terms are special cases, so encode 'em.
      if(strpos($term->name, ',') !== FALSE || strpos($term->name, '"') !== FALSE){
        $n = '"' . str_replace('"', '""', $term->name) . '"';
      }
      // Get parents for a name, to append to the drop down
      $parents = taxonomy_get_parents_all($term->tid);
      if(count($parents) > 1){
        array_shift($parents);
        $parent_string = ' [';
        while($parents){
          $parent = array_shift($parents);
          $parent_string .= check_plain($parent->name);
          if($parents){
            $parent_string .= ' &raquo; ';
          }
        }
        $parent_string .= ']';
      }
      $matches[$term->tid . ":" . $n] = "<b>" . check_plain($term->name) . "</b>" . $parent_string;
    }
  }
  drupal_json($matches);
}

/**
 * TUI Search
 */
function tui_search($vid, $search = FALSE){
  // MESSY, please tidy (I'm too lazy to tidy this, in fact, I think I may go 
  // to the park and play on the swings and slide)!
  // Very similar to the above autocomplete function, but this time we're 
  // returning an array of tids, to allow the tree to be expanded!
  // First we check to see if the entered string is numeric, or if it has been
  // sent in the form "tid:name".
  if(is_numeric($search)){
    $term = taxonomy_get_term($search);
    if($term && $term->vid == $vid){
      drupal_json(array(
        $term->tid
      ));
      $returned_results = TRUE;
    }
  }elseif(strpos($search, ":")){
    $before_colon = substr($search, 0, strpos($search, ':'));
    if(is_numeric($before_colon)){
      $term = taxonomy_get_term($search);
      if($term && $term->vid == $vid){
        drupal_json(array(
          $term->tid
        ));
        $returned_results = TRUE;
      }
    }
  }
  if(!$returned_results){
    $matches = array();
    if($search != ''){
      $result = db_query(db_rewrite_sql("SELECT t.tid FROM {term_data} t WHERE t.vid = %d AND LOWER(t.name) = LOWER('%s')", 't', 'tid'), $vid, $search);
      while($row = db_fetch_array($result)){
        $matches[] = $row['tid'];
      }
    }
    drupal_json($matches);
  }
}

/**
 * Return the form to ajax
 */
function tui_get_term_form($tid){
  // Saving session when getting the form was preventing the Undo button from
  // working.  There shouldn't be any need to save the session when getting the
  // form anyway, so nae bother.
  session_save_session(FALSE);
  $tid_parts = explode('-', $tid);
  if($tid_parts[0] == 'new'){
    $vocabulary = taxonomy_vocabulary_load($tid_parts[1]);
    $term = array();
  }else{
    $term = (array)taxonomy_get_term($tid_parts[1]);
    $vocabulary = taxonomy_vocabulary_load($term['vid']);
  }
  module_load_include('admin.inc', 'taxonomy');
  echo drupal_get_form('taxonomy_form_term', $vocabulary, $term);
}

/**
 * Theme the taxonomy form
 */
function tui_taxonomy_form_pre_render($form){
  $tab_html = '<div id="tabs"><ul>';
  // Specially for the term_node module
  if(isset($form['term_node']['node_form'])){
    foreach($form['term_node']['node_form'] as $key => $value){
      if(is_array($value) && isset($value['#type']) && $value['#type'] == 'fieldset'){
        $form[$key] = $form['term_node']['node_form'][$key];
        unset($form['term_node']['node_form'][$key]);
      }
    }
    if(count($form['term_node']['node_form']) == count(element_properties($form['term_node']['node_form']))){
      unset($form['term_node']);
    }
  }
  if(isset($form['group_scientificname']) && isset($form['identification']['parent_message'])){
    $form['group_scientificname']['parent_message'] = $form['identification']['parent_message'];
  }
  if($form['identification']['description']['#type'] == 'hidden'){
    $form['description'] = $form['identification']['description'];
    $form['name'] = $form['identification']['name'];
    unset($form['identification']);
  }
  $num_tabs = 0;
  foreach($form as $key => $value){
    if(isset($value['#type']) && $value['#type'] == 'fieldset'){
      $num_tabs++;
    }
  }
  if($num_tabs > 5){
    $class = ' class="tui-squeeze-tabs"';
  }else if($num_tabs == 1){
    $class = ' class="tui-hide-tabs"';
  }
  foreach($form as $key => $value){
    if(isset($value['#type']) && $value['#type'] == 'fieldset' && $key != 'term_node'){
      $tab_html .= '<li' . $class . ' title="' . $value['#title'] . '"><a href="#fragment-' . $key . '"><span>';
      if($class && strlen($value['#title']) > 8){
        $tab_html .= substr($value['#title'], 0, 5) . '...';
      }else{
        $tab_html .= $value['#title'];
      }
      $tab_html .= '</span></a></li>';
      $form[$key]['#prefix'] = '<div id="fragment-' . $key . '">';
      $form[$key]['#suffix'] = '</div>';
      $form[$key]['#collapsible'] = FALSE;
      $form[$key]['#collapsed'] = FALSE;
    }
  }
  if(isset($form['term_node'])){
    $tab_html .= '<li' . $class . ' title="' . $form['term_node']['#title'] . '"><a href="#fragment-term_node"><span>';
    if($class && strlen($form['term_node']['#title']) > 8){
      $tab_html .= substr($form['term_node']['#title'], 0, 5) . '...';
    }else{
      $tab_html .= $form['term_node']['#title'];
    }
    $tab_html .= '</span></a></li>';
    $form['term_node']['#prefix'] = '<div id="fragment-term_node">';
    $form['term_node']['#suffix'] = '</div>';
    $form['term_node']['#collapsible'] = FALSE;
    $form['term_node']['#collapsed'] = FALSE;    
  }
  $tab_html .= '</ul>';
  if(isset($form['group_scientificname']) && isset($form['identification']['parent_message'])){
    $form['group_scientificname']['parent_message'] = $form['identification']['parent_message'];
    unset($form['identification']['parent_message']);
  }
  // No fucking clue why the following is required.  Why is the weight of the
  // tabs markup element being ignored.  OH WELL!!! FIXME
  $temp_form = array(
    'tabs' => array(
      '#weight' => -1000,
      '#value' => $tab_html
    )
  );
  // Uuuugh, horrible. silly Drupal.
  foreach($form as $key => $value){
    $temp_form[$key] = $value;
  }
  $form = $temp_form;
  // Just tweak the buttons, as they can sometimes split the tabs and the 
  // content of the tab.
  $form['submit']['#weight'] = 1000;
  unset($form['delete']);
  return $form;
}
