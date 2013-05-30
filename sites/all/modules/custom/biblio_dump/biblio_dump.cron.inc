<?php

/**
 * The meat of hook_cron().
 */
function _biblio_dump_cron(){
  // Check if we need to create a new file.
  $filename = variable_get('biblio_dump_filename', FALSE);
  if($filename){
    if(file_exists($filename)){
      // Check the last edit/create time of a biblio node.
      $max_changed = db_select('node', 'n')->condition('type', 'biblio');
      $max_changed->addExpression('MAX(changed)', 'max_changed');
      $max_changed = $max_changed->execute()->fetchCol();
      $max_changed = array_pop($max_changed);
      if(filemtime($filename) > $max_changed){return;}
    }
    // We are here - we must need to rewrite the file.
    // Start by blanking the file.
    file_put_contents($filename, '');
    $results = db_select('node', 'n')->fields('n', array(
      'nid'
    ))->condition('type', 'biblio')->execute();
    foreach($results as $row){
      file_put_contents($filename, _biblio_bibtex_export(node_load($row->nid)), FILE_APPEND);
    }
  }else{
    module_load_include('install', 'biblio_dump');
    biblio_dump_install();
  }
}