<?php

/**
 * Search on the Catalogue of Life
 */
function scratchpads_col_search($search){
  // Do the query, and return the results.
  // We look for exact matches first, if we find any, we return them.
  $id = db_select('scratchpads_col_terms', 't')->condition('Term_name', $search)->fields('t', array(
    'taxon_id',
    'Term_name'
  ))->execute()->fetchAllKeyed();
  if(!$id){
    // We didn't find any exact matches, so we look for names that START with
    // the provided string
    $id = db_select('scratchpads_col_terms', 't')->condition('Term_name', $search . '%', 'LIKE')->fields('t', array(
      'taxon_id',
      'Term_name'
    ))->execute()->fetchAllKeyed();
  }
  if(!$id){
    // We didn't find any exact matches, so we look for names that START with
    // the provided string
    $id = db_select('scratchpads_col_terms', 't')->condition('Term_name', '%' . $search . '%', 'LIKE')->fields('t', array(
      'taxon_id',
      'Term_name'
    ))->execute()->fetchAllKeyed();
  }
  return $id;
}

/**
 * Return a MySQL result set which can be parsed through. Note, the headers are
 * based on the column names in the Query.
 */
function scratchpads_col_download($id, $page = 0, $limit = 50000){
  $folder = 'public://scratchpads_col';
  if(file_prepare_directory($folder, FILE_CREATE_DIRECTORY)){
    $filename = "{$folder}/{$id}-{$page}.csv";
    if(!file_exists($filename)){
      $csvf = fopen($filename, 'a');
      $row = db_select('scratchpads_col_terms', 'l')->condition('taxon_id', $id)->fields('l', array(
        'rgt',
        'lft'
      ))->execute()->fetchAssoc();
      $lft = $row['lft'];
      $rgt = $row['rgt'];
      $this_results = db_query("
    SELECT
      `Term_name` AS `Term name`,
      '' AS `Parent Term Name`,
      `Term_description` AS `Term description`,
      `GUID`,
      `Parent_GUID` AS `Parent GUID`,
      '' AS `Associated accepted name (Name)`,
      '' AS `Associated accepted name (TID)`,
      '' AS `Associated accepted name (GUID)`,
      `Vernacular_names_COLON__Vernacular_name` AS `Vernacular names: Vernacular name`,
      `Vernacular_names_COLON__Language` AS `Vernacular names: Language`,
      '' AS `Vernacular names: Source (GUID)`,
      '' AS `Vernacular names: Source (NID)`,
      '' AS `Vernacular names: Source (Title)`,
      '' AS `Vernacular names: Area from which the vernacular name usage originates`,
      '' AS `Vernacular names: Vernacular name remarks`,
      `Authors`,
      '' AS `Page number`,
      `Rank`,
      '' AS `Reference (GUID)`,
      '' AS `Reference (NID)`,
      '' AS `Reference (Title)`,
      '' AS `Unacceptability Reason`
    FROM
      scratchpads_col_terms
    WHERE lft >= :lft AND rgt <= :rgt  ORDER BY lft LIMIT " . ($page * $limit) . ", {$limit};", array(
        ':lft' => $lft,
        ':rgt' => $rgt
      ));
      $done_header = FALSE;
      $i = 0;
      while(($row = $this_results->fetchAssoc()) != FALSE){
        $i++;
        $name_parts = explode(' ', $row['Term name']);
        for($i = 1; $i <= 4; $i++){
          $row['Unit name ' . $i] = isset($name_parts[$i - 1]) ? $name_parts[$i - 1] : '';
        }
        if(!$done_header){
          fputcsv($csvf, array_keys($row));
          $done_header = TRUE;
        }
        fputcsv($csvf, $row);
      }
      if($i < $limit){
        // Need to work out how many results we actually need, and possibly also the
        // offset.
        $total = db_select('scratchpads_col_terms', 's')->condition('lft', $lft, '>=')->condition('rgt', $rgt, '<=')->countQuery()->execute()->fetchField();
        $offset = ($page * $limit) - $total;
        if($offset < 0){
          $offset = 0;
        }
        $limit = $limit - $i;
        $this_results = db_query("SELECT
      `Term_name` AS `Term name`,
      '' AS `Parent Term Name`,
      '' AS `Term description`,
      '' AS `GUID`,
      `Parent_GUID` AS `Parent GUID`,
      '' AS `Associated accepted name (Name)`,
      '' AS `Associated accepted name (TID)`,
      `Associated_accepted_name__OPEN_GUID_CLOSE_` AS `Associated accepted name (GUID)`,
      '' AS `Vernacular names: Vernacular name`,
      '' AS `Vernacular names: Language`,
      '' AS `Vernacular names: Source (GUID)`,
      '' AS `Vernacular names: Source (NID)`,
      '' AS `Vernacular names: Source (Title)`,
      '' AS `Vernacular names: Area from which the vernacular name usage originates`,
      '' AS `Vernacular names: Vernacular name remarks`,
      `Authors`,
      '' AS `Page number`,
      `Rank`,
      '' AS `Reference (GUID)`,
      '' AS `Reference (NID)`,
      '' AS `Reference (Title)`,
      '' AS `Unacceptability Reason`
    FROM
      scratchpads_col_synonyms
    WHERE lft >= :lft AND rgt <= :rgt ORDER BY `Term_name` LIMIT $offset, $limit", array(
          ':lft' => $lft,
          ':rgt' => $rgt
        ));
        $i = 0;
        while(($row = $this_results->fetchAssoc()) != FALSE){
          $i++;
          $name_parts = explode(' ', $row['Term name']);
          for($i = 1; $i <= 4; $i++){
            $row['Unit name ' . $i] = isset($name_parts[$i - 1]) ? $name_parts[$i - 1] : '';
          }
          if(!$done_header){
            fputcsv($csvf, array_keys($row));
            $done_header = TRUE;
          }
          fputcsv($csvf, $row);
        }
      }
      fclose($csvf);
    }
    header('Content-Type: text/csv');
    header('Content-Disposition: attachment;filename="classification-' . $page . '.csv"');
    readfile($filename);
  }
}