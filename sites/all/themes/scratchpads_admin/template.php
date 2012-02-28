<?php

/**
 * Implements hook_js_alter().
 * 
 * We simply want to remove the rubik.js file, as it is no longer being used,
 * and causes an error.
 */
function scratchpads_admin_js_alter(&$javascript){
  unset($javascript['sites/all/themes/rubik/js/rubik.js']);
}

/**
 * Implements hook_theme_registry_alter().
 * 
 * Loop through the theme registry to remove rubik/templates/form-default, and
 * replace it with ours.
 */
function scratchpads_admin_theme_registry_alter(&$theme_registry){
  foreach($theme_registry as $key => $value){
    if(isset($value['path']) && $value['path'] == 'sites/all/themes/rubik/templates' && isset($value['template']) && $value['template'] == 'form-default'){
      $theme_registry[$key]['path'] = drupal_get_path('theme', 'scratchpads_admin');
    }
  }
}

/**
 * Postprocessor for theme('page').
 */
function scratchpads_admin_process_page(&$variables){
  // If help text exists, add the help shortcut icon
  if(isset($variables['page']['help']) && count($variables['page']['help'])){
    // Ensure the shortcut.css is available as we want to use some of the styles
    $path = drupal_get_path('theme', 'rubik');
    drupal_add_css($path . '/shortcut.css');
    $variables['title_suffix']['help'] = array(
      '#prefix' => '<div class="add-or-remove-shortcuts help-shortcut">',
      '#type' => 'link',
      '#title' => '<span class="icon"></span><span class="text">Help</span>',
      '#href' => request_path(),
      '#options' => array(
        'html' => TRUE
      ),
      '#suffix' => '</div>'
    );
  }
}

/**
 * Preprocessor for theme('page').
 */
function scratchpads_admin_preprocess_page(&$vars){
  rubik_preprocess_page($vars);
  _rubik_local_tasks($vars);
}

/**
 * Override of theme('breadcrumb').
 * rubik_breadcrumb doesn't use page title of current page if it's not a menu item (ie; a view)
 */
function scratchpads_admin_breadcrumb($vars){
  $output = '';
  // Add current page onto the end.
  if(!drupal_is_front_page()){
    if($item = menu_get_item()){
      if(!empty($item['title'])){
        $title = $item['title'];
      }
    }
    if(!isset($title)){
      $title = strip_tags(drupal_get_title());
    }
    $end = end($vars['breadcrumb']);
    if($end && strip_tags($end) !== $title){
      $vars['breadcrumb'][] = "<strong>" . check_plain($title) . "</strong>";
    }
  }
  // Optional: Add the site name to the front of the stack.
  if(!empty($vars['prepend'])){
    $site_name = empty($vars['breadcrumb']) ? "<strong>" . check_plain(variable_get('site_name', '')) . "</strong>" : l(variable_get('site_name', ''), '<front>', array(
      'purl' => array(
        'disabled' => TRUE
      )
    ));
    array_unshift($vars['breadcrumb'], $site_name);
  }
  $depth = 0;
  foreach($vars['breadcrumb'] as $link){
    $output .= "<span class='breadcrumb-link breadcrumb-depth-{$depth}'>{$link}</span>";
    $depth++;
  }
  return $output;
}


