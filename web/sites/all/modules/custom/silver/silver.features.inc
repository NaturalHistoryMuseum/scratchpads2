<?php

/**
 * Implementation of hook_ctools_plugin_api().
 */
function silver_ctools_plugin_api(){
  list($module, $api) = func_get_args();
  if($module == "feeds" && ($api == "feeds_importer_default" || $api == 'plugins')){return array(
      "version" => "1"
    );}
  if($module == "feeds_tamper" && $api == "feeds_tamper_default"){return array(
      "version" => "2"
    );}
}
