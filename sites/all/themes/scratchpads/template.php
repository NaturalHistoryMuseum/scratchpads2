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
    $explode = explode('-', $block->delta);
    if(count($explode) != 2){return;}
    list($name, $display_id) = $explode;
    // Load the view
    if($view = views_get_view($name)){
      if(isset($view->display[$display_id]->display_options['css_class'])){
        $data['class'] = array(
          $view->display[$display_id]->display_options['css_class']
        );
      }elseif(isset($view->display['default']->display_options['css_class'])){
        $data['class'] = array(
          $view->display['default']->display_options['css_class']
        );
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
  $form['#suffix'] = l('Log in', '', array(
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
      break;
  }
}





