<?php
// Constants
DEFINE('EOLAPI_URL', 'http://eol.org/api/');

/**
 * This will create a new EOL search entity and return it.  If an identical
 * search entity already exists, then that is updated (or simply returned if
 * it is younger than $age).
 */
function eolapi_search($search, $age = 604800){ // Seconds in one week.
  $search = trim($search);
  try{
    $row = db_select('eolapi', 'e')->condition('label', $search)->condition('type', 'search')->fields('e')->orderBy('changed', 'DESC')->execute()->fetch();
    if($row){
      // Note, we do a seperate load to ensure other modules can alter this eolapi
      // if they so wish.
      if(REQUEST_TIME - $age < $row->changed){return entity_load_single('eolapi', $row->eid);}
    }
    // Check first that the service is up. Else we set a warning message.
    if(file_get_contents(EOLAPI_URL . 'ping')){
      $data = file_get_contents(EOLAPI_URL . 'search/' . fullescape(urlencode($search)) . '.json?exact=1');
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
            $eolapi->label = $search;
          }
          $eolapi->data = serialize($data);
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
    watchdog('eolapi', 'Search failed for: %search', array(
      '%search' => $search
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