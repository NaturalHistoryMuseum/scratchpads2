<?php

function dwcarchiver_drush_command(){
  return array(
    'dwcarchiver' => array(
      'callback' => 'dwcarchiver_drush_rebuild_archives',
      'description' => 'Rebuilds all DwC-A files'
    )
  );
}

function dwcarchiver_drush_rebuild_archives(){
  $archives = dwcarchiver_load_all();
  //array_shift($archives);
  foreach($archives as $archive){
    module_load_include('rebuild.inc', 'dwcarchiver');
    dwcarchiver_rebuild($archive, FALSE);
    drush_log('Rebuilt: ' . $archive->name, 'ok');
  }
}