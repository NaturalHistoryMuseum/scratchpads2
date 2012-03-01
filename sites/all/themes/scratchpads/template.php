<?php

/**
 * 
 * Implentation of hook_block_view_alter
 * Move the view class to the container
 * @param array $blocks
 * @param array $data
 */
function scratchpads_block_view_alter(&$data, $block){
  if(isset($data['content']) && $block->module == 'views'){
    // Move the view classes to the block classes
    if(preg_match('/(grid-[0-9])/', $data['content']['#markup'], $matches)){
      if(count($matches)){
        $data['content']['#markup'] = str_replace($matches[1], '', $data['content']['#markup']);
        $classes = array(
          $matches[1]
        );
        if(strpos($data['content']['#markup'], 'alpha')){
          str_replace('alpha', '', $data['content']['#markup']);
          $classes[] = 'alpha';
        }
        if(strpos($data['content']['#markup'], 'omega')){
          str_replace('omega', '', $data['content']['#markup']);
          $classes[] = 'omega';
        }
        $data['class'] = $classes;
      }
    }
  }
}

function scratchpads_contextual_links_view_alter(&$element, &$items){
  if(isset($element['#element']['#block'])){
    // Remove contextual links for some blocks
    switch($element['#element']['#block']->module){
      case 'search':
        switch($element['#element']['#block']->delta){
          case 'form':
            unset($element['#links']);
            break;
        }
        break;
      case 'scratchpads_colour':
      case 'scratchpads_blocks':
        unset($element['#links']);
        break;
    }
  }
}

function scratchpads_form_user_login_block_alter(&$form, &$form_state, $form_id){
  $form['links']['#weight'] = 100;
  $form['#suffix'] = l('Log in', 'user', array(
    'attributes' => array(
      'class' => array(
        'scratchpads-slide-toggle'
      )
    )
  ));
  $form['#validate'][] = 'scratchpads_user_login_form_validate';
  $form['#attributes']['style'] = 'display:none';
}

function scratchpads_user_login_form_validate(&$form, &$form_state){
  // If there's validation errors, we don't want to display
  if(form_get_errors()){
    unset($form['#attributes']['style']);
  }
}

function scratchpads_preprocess_breadcrumb(&$variables){
  if(count($variables['breadcrumb'])){
    $variables['breadcrumb'][] = drupal_get_title();
  }
}

/**
 * Implements hook_process_region().
 */
function scratchpads_process_region(&$vars){
  $theme = alpha_get_theme();
  switch($vars['elements']['#region']){
    case 'content':
      if(isset($theme->page['page']['subtitle'])){
        $vars['subtitle'] = $theme->page['page']['subtitle'];
      }else{
        $vars['subtitle'] = NULL;
      }
      if(isset($theme->page['page']['title'])){
        $vars['title'] = $theme->page['page']['title'];
      }
      break;
  }
}

/**
 * Implements hook_preprocess_region().
 */
function scratchpads_preprocess_region(&$vars){
  global $user;
  switch($vars['elements']['#region']){
    case 'secondary_menu':
      $links = array();
      if(module_exists('contact') && user_access('access site-wide contact form')){
        $links[] = array(
          'href' => 'contact',
          'title' => t("Contact us"),
          'attributes' => array(
            'class' => array(
              'contact-us'
            )
          )
        );
        $links[] = array(
          'href' => 'contact/report-abuse',
          'title' => t("Report abuse"),
          'attributes' => array(
            'class' => array(
              'report-abuse'
            )
          )
        );
      }
      if(user_is_logged_in()){
        $links[] = array(
          'href' => url('help.scratchpads.eu'),
          'title' => t("Help"),
          'attributes' => array(
            'class' => array(
              'help'
            )
          )
        );
      }
      $vars['links'] = theme('links', array(
        'links' => $links
      ));

      
      break;
  }
}

function scratchpads_form_search_block_form_alter(&$form, &$form_state, $form_id){
  $form['actions']['#weight'] = -10;
}

function scratchpads_preprocess_node(&$variables){
  if($variables['view_mode'] == 'linked_node'){
    $node_info = node_type_load($variables['type']);
    $variables['title'] = $node_info->name;
    $variables['display_submitted'] = false;
  }
}

function scratchpads_preprocess_field(&$variables, $hook){
  if(isset($variables['element']['#stripe'])){
    $variables['classes_array'][] = $variables['element']['#stripe'];
    $variables['classes_array'][] = 'clearfix';
  }
}

function scratchpads_biblio_tabular($variables){
  module_load_include('inc', 'biblio', '/includes/biblio.contributors');
  $node = $variables['node'];
  $base = $variables['base'];
  static $citeproc;
  if(module_exists('popups')){
    popups_add_popups();
  }
  $tid = $node->biblio_type;
  $fields = _biblio_get_field_information($node->biblio_type, TRUE);
  if(!isset($node->biblio_type_name) && isset($node->biblio_type)){ // needed for preview
    if(($pub_type = db_query('SELECT t.tid, t.name FROM {biblio_types} as t WHERE t.tid=:tid', array(
      ':tid' => $node->biblio_type
    ))->fetchObject())){
      $node->biblio_type_name = drupal_ucfirst(_biblio_localize_type($pub_type->tid, $pub_type->name));
    }
  }
  $rows[] = array(
    array(
      'data' => t('Publication Type:'),
      'class' => array(
        'biblio-row-title'
      )
    ),
    array(
      'data' => $node->biblio_type_name
    )
  );
  $attrib = (variable_get('biblio_links_target_new_window', FALSE)) ? array(
    'target' => '_blank'
  ) : array();
  $doi = '';
  if(!empty($node->biblio_doi)){
    $doi_url = '';
    if(($doi_start = strpos($node->biblio_doi, '10.')) !== FALSE){
      $doi = substr($node->biblio_doi, $doi_start);
      $doi_url .= 'http://dx.doi.org/' . $doi;
      $doi = l($doi, $doi_url, $attrib);
    }
  }
  foreach($fields as $key => $row){
    // handling the contributor categories like any other field orders them correctly by weight
    if($row['type'] == 'contrib_widget' && ($authors = biblio_get_contributor_category($node->biblio_contributors, $row['fid']))){
      $data = biblio_format_authors($authors);
    }elseif(empty($node->$row['name']) || $row['name'] == 'biblio_coins'){
      continue;
    }else{
      switch($row['name']){
        case 'biblio_keywords':
          $data = _biblio_keyword_links($node->$row['name'], $base);
          break;
        case 'biblio_url':
          $data = l($node->$row['name'], $node->$row['name'], $attrib);
          break;
        case 'biblio_doi':
          $data = $doi;
          break;
        default:
          if($row['type'] == 'text_format'){
            $data = check_markup($node->$row['name'], $node->biblio_formats[$row['name']]);
          }else{
            $data = check_plain($node->$row['name']);
          }
      }
    }
    $rows[] = array(
      array(
        'data' => t($row['title']) . ':',
        'class' => array(
          'biblio-row-title'
        )
      ),
      array(
        'data' => $data
      )
    );
  }
  if(isset($node->body) && !empty($node->body) && user_access('view full text')){
    $rows[] = array(
      array(
        'data' => t('Full Text'),
        'valign' => 'top'
      ),
      array(
        'data' => drupal_render(field_view_field('node', $node, 'body', array(
          'label' => 'hidden'
        )))
      )
    );
  }
  $output = '<div id="biblio-node">';
  $output .= filter_xss($node->biblio_coins, array(
    'span'
  ));
  $header = array();
  $output .= theme('table', array(
    'header' => $header,
    'rows' => $rows
  ));
  $output .= '</div>';
  return $output;
}

/**
 * Implements hook_preprocess_page().
 */
function scratchpads_preprocess_page(&$vars){
  if(isset($vars['tabs']) && empty($vars['tabs']['#primary'])){
    $vars['tabs'] = array();
  }
}
  