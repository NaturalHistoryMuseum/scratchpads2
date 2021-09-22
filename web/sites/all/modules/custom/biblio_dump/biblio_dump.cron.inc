<?php

/**
 * The meat of hook_cron().
 * 
 * We only save 50 entries per cron run to help prevent memory issues.
 */
function _biblio_dump_cron(){
  if(variable_get('biblio_dump_rebuild', FALSE)){
    // Check if we have nodes that need their bibtex building. If we do, we do
    // that first.
    $nids = variable_get('biblio_dump_nids', array());
    if(count($nids)){
      $count = 0;
      $query = db_insert('biblio_dump')->fields(array(
        'nid',
        'bibtex'
      ));
      $nids_to_delete_first = array();
      module_load_include('inc', 'biblio', 'includes/biblio.contributors');
      while(($nid = array_pop($nids)) != FALSE && $count < 50){
        $query->values(array(
          'nid' => $nid,
          'bibtex' => _biblio_bibtex_export(node_load($nid))
        ));
        $count++;
        $nids_to_delete_first[] = $nid;
      }
      db_delete('biblio_dump')->condition('nid', $nids_to_delete_first)->execute();
      $query->execute();
      variable_set('biblio_dump_nids', $nids);
    }else{
      // We're here, the biblio_dump table is populated, so we just need to
      // export it.
      $filename = variable_get('biblio_dump_filename', FALSE);
      if(!$filename){
        $filename = 'public://biblio_dump-' . md5(time()) . '.bib';
        variable_set('biblio_dump_filename', $filename);
      }
      // Blank the file
      file_put_contents($filename, '');
      $results = db_select('biblio_dump', 'b')->fields('b', array(
        'bibtex'
      ))->execute();
      foreach($results as $row){
        file_put_contents($filename, $row->bibtex, FILE_APPEND);
      }
      variable_set('biblio_dump_rebuild', FALSE);
    }
  }
}