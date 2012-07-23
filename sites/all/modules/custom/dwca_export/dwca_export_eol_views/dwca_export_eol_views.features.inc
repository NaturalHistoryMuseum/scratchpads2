<?php

/**
 * @file
 * dwca_export_eol_views.features.inc
 */
/**
 * Implements hook_views_api().
 */
function dwca_export_eol_views_views_api(){
  list($module, $api) = func_get_args();
  if($module == "views" && $api == "views_default"){return array(
      "version" => "3.0"
    );}
}
