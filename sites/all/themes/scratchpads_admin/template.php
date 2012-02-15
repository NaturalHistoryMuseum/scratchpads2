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