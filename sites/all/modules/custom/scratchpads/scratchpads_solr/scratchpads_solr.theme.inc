<?php

/**
 * Theme function for a Scratchpads search page.
 */
function theme_scratchpads_solr_search($variables){
  $info = module_invoke_all('scratchpads_solr_info');
  drupal_alter('scratchpads_solr_info', $info);
  if(isset($info[arg(0)])){
    return theme($info[arg(0)], $variables);
  }else{
    return theme('search_results', $variables);
  }
}