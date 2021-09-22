<?php

/**
 * @file
 * publication_html_format.features.inc
 */
/**
 * Implements hook_views_api().
 */
function publication_html_format_views_api(){
  list($module, $api) = func_get_args();
  if($module == "views" && $api == "views_default"){return array(
      "version" => "3.0"
    );}
}