<?php
// Constants
DEFINE('EOLAPI_URL', 'http://eol.org/api/');

/**
 * This will create a new EOL search entity and return it.  If an identical
 * search entity already exists, then that is updated (or simply returned if
 * it is younger than $age).
 */
function eolapi_search($tid, $age = 604800){ // Seconds in one week.
  $term = taxonomy_term_load($tid);
  try{
    $row = db_select('eolapi', 'e')->condition('label', $term->name)->condition('type', 'search')->fields('e')->orderBy('changed', 'DESC')->execute()->fetch();
    if($row){
      // Note, we do a seperate load to ensure other modules can alter this eolapi
      // if they so wish.
      if(REQUEST_TIME - $age < $row->changed){return entity_load_single('eolapi', $row->eid);}
    }
    // Check first that the service is up. Else we set a warning message.
    if(file_get_contents(EOLAPI_URL . 'ping')){
      $data = file_get_contents(EOLAPI_URL . 'search/' . fullescape(urlencode($term->name)) . '.json?exact=1');
      if($data){
        $data = json_decode($data);
        if($data){
          // Search has worked, we should update the current $row (if set), or
          // save a new eolapi entity
          if($row){
            $eolapi = entity_load_single('eolapi', $row->eid);
          }else{
            // New eolapi data object
            $eolapi = new stdClass();
            $eolapi->type = 'search';
            $eolapi->label = $term->name;
          }
          $eolapi->data = serialize($data);
          $eolapi->eolapi_taxonomy = array(
            'und' => array(
              array(
                'tid' => $tid
              )
            )
          );
          entity_save('eolapi', $eolapi);
          return $eolapi;
        }
      }else{
        return FALSE;
      }
    }else{
      if($row){
        return entity_load_single('eolapi', $row->eid);
      }else{
        return FALSE;
      }
    }
  }
  catch(Exception $e){
    watchdog('eolapi', 'Search failed for: %search', array(
      '%search' => $term->name
    ), WATCHDOG_ERROR);
    return FALSE;
  }
}

/**
 * Get 
 */
function eolapi_pages($id, $age = 604800){
  try{
    // Mus musculus - 328450
    $row = db_select('eolapi', 'e')->condition('label', $id)->condition('type', 'pages')->fields('e')->orderBy('changed', 'DESC')->execute()->fetch();
    if($row){
      // Note, we do a seperate load to ensure other modules can alter this eolapi
      // if they so wish.
      if(REQUEST_TIME - $age < $row->changed){return entity_load_single('eolapi', $row->eid);}
    }
    // Check first that the service is up. Else we set a warning message.
    if(file_get_contents(EOLAPI_URL . 'ping')){
      // We get the full amount of data that we can.
      $data = file_get_contents(EOLAPI_URL . 'pages/' . $id . '.json?images=75&videos=75&text=75&subject=all&licenses=all&details=1&vetted=' . variable_get('eolapi_trusted_content', 1) . '&version=1.0');
      if($data){
        $data = json_decode($data);
        if($data){
          // Search has worked, we should update the current $row (if set), or
          // save a new eolapi entity
          if($row){
            $eolapi = entity_load_single('eolapi', $row->eid);
          }else{
            // New eolapi data object
            $eolapi = new stdClass();
            $eolapi->type = 'pages';
            $eolapi->label = $id;
          }
          $eolapi->data = serialize($data);
          $eolapi->eolapi_taxonomy = array(
            'und' => array(
              array(
                'tid' => 1
              )
            )
          );
          entity_save('eolapi', $eolapi);
          return $eolapi;
        }
      }else{
        // Error here.  We simply return FALSE;
        return FALSE;
      }
    }else{
      //drupal_set_message('Search failed', 'error')
      if($row){
        return entity_load_single('eolapi', $row->eid);
      }else{
        return FALSE;
      }
    }
  }
  catch(Exception $e){
    watchdog('eolapi', 'Retrieval of data failed for: %id', array(
      '%id' => $id
    ), WATCHDOG_ERROR);
    return FALSE;
  }
}

/**
 * Function taken from http://www.php.net/manual/en/function.urlencode.php#96394
 */
function fullescape($in){
  $in = str_replace('+', '%20', $in);
  $in = str_replace('_', '%5F', $in);
  $in = str_replace('.', '%2E', $in);
  $in = str_replace('-', '%2D', $in);
  return $in;
} 