<?php
// Constants
DEFINE('EOLAPI_URL', 'http://eol.org/api/');
DEFINE('EOLAPI_VERSION', '1.0');

/**
 * This will create a new EOL search entity and return it.  If an identical
 * search entity already exists, then that is updated (or simply returned if
 * it is younger than $age).
 */
function eolapi_search($tid, $age = 604800){ // Seconds in one week.
  $term = taxonomy_term_load($tid);
  if($term){
    $search_results = eolapi_get('search', $term->name, $tid, 'exact=1', 604800);
    if($search_results){
      $data = unserialize($search_results->data);
      foreach($data->results as $result){
        $page = eolapi_page($result->id, $tid);
      }
      return $search_results;
    }
  }
  return FALSE;
}

/**
 * Get an EOL API page.
 */
function eolapi_page($id, $tid, $age = 604800){ // Seconds in one week
  $page = eolapi_get('pages', $id, $tid, 'images=75&videos=75&text=75&subjects=all&licenses=all&details=1&vetted=' . variable_get('eolapi_trusted_content', 0) . '&version=1.0', $age);
  // We then process the page, and get all the required objects into the
  // database.
  $data = unserialize($page->data);
  foreach($data->dataObjects as $dataobject){
    // We use the dataType as the bundle name, and save the entity form that.
    eolapi_data_objects($dataobject, $tid);
  }
  // And finally return the page.
  return $page;
}

/**
 * Gen an EOL API Data object.
 */
function eolapi_data_objects($id, $tid, $age = 604800){
  if(is_object($id)){
    // We have been passed an object rather than an ID.  This has most likely
    // come from an EOL page, and therefore needs saving.
    // We tweak this object to look IDENTICAL to the object that we would
    // obtain if using the eolapi_get() function.
    $row = db_select('eolapi', 'e')->condition('label', $id->identifier)->fields('e')->orderBy('changed', 'DESC')->execute()->fetch();
    if($row){
      $eolapi = entity_load_single('eolapi', $row->eid);
    }else{
      // New eolapi data object
      $eolapi = new stdClass();
    }
    $eolapi->type = strtolower($id->dataType);
    if(substr($eolapi->type, 0, 4) == 'http'){
      $eolapi->type = array_pop(explode("/", $eolapi->type));
    }
    // We do something a little special for the text type.  This may throw up
    // issues with odd ordering in the future, so if it does, remove the
    // following.
    if($eolapi->type == 'text'){
      if(!isset($id->subject) && isset($id->dataRating)){
        // Add 10 to the rating
        $id->dataRating += 10;
      }
    }
    $eolapi->trusted = $id->vettedStatus == 'Trusted' ? 1 : 0;
    $eolapi->license = isset($id->license) ? $id->license : '';
    $eolapi->rating = $id->dataRating;
    $eolapi->label = $id->identifier;
    $eolapi->data = serialize($id);
    $lang = field_language('eolapi', $eolapi, 'eolapi_taxonomy');
    $eolapi->eolapi_taxonomy = array(
      $lang => array(
        array(
          'tid' => $tid
        )
      )
    );
    $eolapi->source = 'EOL';
    entity_save('eolapi', $eolapi);
  }else{
    $eolapi = eolapi_get('data_objects', $id, $tid);
  }
  //return $eolapi;
  // Check to see if this is an stillimage type, and if so, download the image
  // for it.
  if($eolapi->type == 'stillimage' && (!isset($eolapi->eolapi_image) || !count($eolapi->eolapi_image))){
    // Download the original image so that we can perform image_styles magic on
    // it.
    $data = unserialize($eolapi->data);
    $file = file_save_data(@file_get_contents($data->mediaURL));
    if(!$file->filesize){
      // We failed to download a file.  We should therefore delete this file, and
      // continue with the EOL preview.
      file_delete($file);
      $file = file_save_data(@file_get_contents($data->eolMediaURL));
    }
    $toolkit = image_get_toolkit();
    $image = new stdClass();
    $image->source = $file->uri;
    $image->toolkit = $toolkit;
    $details = image_toolkit_invoke('get_info', $image);
    $file = file_move($file, 'public://eolapi/' . $eolapi->eid . '-' . $eolapi->erid . '.' . $details['extension'], FILE_EXISTS_REPLACE);
    $file->title = '';
    $file->alt = '';
    $file->width = $details['width'];
    $file->height = $details['height'];
    $file->filename = $eolapi->eid . '-' . $eolapi->erid . '.' . $details['extension'];
    file_save($file);
    $lang = field_language('eolapi', $eolapi, 'eolapi_image');
    $eolapi->eolapi_image = array(
      $lang => array(
        (array)$file
      )
    );
    entity_save('eolapi', $eolapi);
  }
  return $eolapi;
}

/**
 * Gen an EOL API Data object.
 */
function eolapi_get($type, $id, $tid, $additional_query = '', $age = 604800){
  try{
    $query = db_select('eolapi', 'e')->condition('label', $id);
    if($type != 'data_objects'){
      $query->condition('type', $type);
    }
    $row = $query->fields('e')->orderBy('changed', 'DESC')->execute()->fetch();
    if($row){
      // Note, we do a seperate load to ensure other modules can alter this eolapi
      // if they so wish.
      if(REQUEST_TIME - $age < $row->changed){return entity_load_single('eolapi', $row->eid);}
    }
    // Check first that the service is up. Else we set a warning message.
    if(file_get_contents(EOLAPI_URL . 'ping')){
      // We get the full amount of data that we can.
      $data = file_get_contents(EOLAPI_URL . $type . '/' . EOLAPI_VERSION . '/' . fullescape(urlencode($id)) . '.json?' . $additional_query);
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
          }
          $eolapi->type = strtolower(($type == 'data_objects') ? $data->dataObjects[0]->dataType : $type);
          $eolapi->label = $id;
          $eolapi->data = serialize($data);
          if($type == 'data_objects'){
            $eolapi->trusted = $data->dataObjects[0]->vettedStatus == 'Trusted' ? 1 : 0;
            $eolapi->license = $data->dataObjects[0]->license;
            $eolapi->rating = $data->dataObjects[0]->dataRating;
            $eolapi->data = serialize($data->dataObjects[0]);
            $eolapi->source = 'EOL';
          }
          if(substr($eolapi->type, 0, 4) == 'http'){
            $eolapi->type = array_pop(explode("/", $eolapi->type));
          }
          $lang = field_language('eolapi', $eolapi, 'eolapi_taxonomy');
          $eolapi->eolapi_taxonomy = array(
            $lang => array(
              array(
                'tid' => $tid
              )
            )
          );
          $eolapi->source = 'EOL';
          entity_save('eolapi', $eolapi);
          return $eolapi;
        }
      }else{
        // Error here.  We simply return FALSE;
        return FALSE;
      }
    }else{
      drupal_set_message(t('We are currently experiencing problems with the EOL service.'), 'error', FALSE);
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
