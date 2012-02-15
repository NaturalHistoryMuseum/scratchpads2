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