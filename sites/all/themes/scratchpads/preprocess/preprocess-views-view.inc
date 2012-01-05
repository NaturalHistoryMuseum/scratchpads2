<?php

function scratchpads_alpha_preprocess_views_view(&$vars){
  // TODO - remove this: why isn't scratchpads_preprocess_views_view__block being called directly?
  if(isset($vars['theme_hook_suggestion'])){
    $func = 'scratchpads_alpha_preprocess_' . $vars['theme_hook_suggestion'];
    if(function_exists($func)){
      $func($vars);
    }
  }
}

function scratchpads_alpha_preprocess_views_view__block(&$vars){
  if(!empty($vars['css_class'])){
    $vars['classes_array'] = array_diff($vars['classes_array'], array(
      $vars['css_class']
    ));
  }
}