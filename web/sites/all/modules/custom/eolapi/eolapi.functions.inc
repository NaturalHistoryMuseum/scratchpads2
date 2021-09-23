<?php

/**
 * URL of the EOL API.
 */
DEFINE('EOLAPI_URL', 'https://eol.org/api/');
/**
 * Version of the EOL API to use
 */
DEFINE('EOLAPI_VERSION', '1.0');
/**
 * Timeout for requests to the EOL API.
 */
DEFINE('EOLAPI_TIMEOUT', 10);

/**
 * This will create a new EOL search entity and return it.  If an identical
 * search entity already exists, then that is updated (or simply returned if
 * it is younger than $age).
 */
function eolapi_search($tid, $age = 604800)
{ // Seconds in one week.
  $term = taxonomy_term_load($tid);
  if ($term) {
    $search_results = eolapi_get('search', $term->name, $tid, 'exact=1', 604800);
    if ($search_results) {
      $data = unserialize($search_results->data);
      foreach ($data->results as $result) {
        $page = eolapi_page($result->id, $tid);
        // break;
      }
      return $search_results;
    }
  }
  return false;
}

/**
 * Get an EOL API page.
 */
function eolapi_page($id, $tid, $age = 604800)
{ // Seconds in one week
  $page = eolapi_get('pages', $id, $tid, 'images_per_page=75&videos_per_page=75&text_per_page=75&subjects=all&licenses=all&details=true&vetted=' . variable_get('eolapi_trusted_content', 0) . '&version=1.0', $age);
  // We then process the page, and get all the required objects into the
  // database.
  $data = unserialize($page->data);
  foreach ($data->taxonConcept->dataObjects as $dataobject) {
    // We use the dataType as the bundle name, and save the entity form that.
    eolapi_data_objects($dataobject, $tid);
  }
  // And finally return the page.
  return $page;
}

/**
 * Gen an EOL API Data object.
 */
function eolapi_data_objects($id, $tid, $age = 604800)
{


  // Convert dataobject into the eolapi entity
  $row = db_select('eolapi', 'e')->condition('label', $id->identifier)->fields('e')->orderBy('changed', 'DESC')->execute()->fetch();
  if ($row) {
    $eolapi = entity_load_single('eolapi', $row->eid);
  } else {
    // New eolapi data object
    $eolapi = new stdClass();
  }
  $eolapi->type = strtolower($id->dataType);
  if (substr($eolapi->type, 0, 4) == 'http') {
    $eolapi->type = array_pop(explode("/", $eolapi->type));
  }
  // We do something a little special for the text type.  This may throw up
  // issues with odd ordering in the future, so if it does, remove the
  // following.
  if ($eolapi->type == 'text') {
    if (!isset($id->subject) && isset($id->dataRating)) {
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

  // Check to see if this is an stillimage type, and if so, download the image
  // for it.
  if ($eolapi->type == 'stillimage' && (!isset($eolapi->eolapi_image) || !count($eolapi->eolapi_image))) {
    // Download the original image so that we can perform image_styles magic on
    // it.
    $data = unserialize($eolapi->data);
    // Create a file object by downloading the original image. We then delete
    // the file and replace the URI with the original URI. Note, we do this so
    // that we can get the width and height of the image (not sure if they're
    // actually required).
    $request = drupal_http_request($data->mediaURL, array(
      'timeout' => EOLAPI_TIMEOUT
    ));

    if ($request->code == 200) {
      $file = file_save_data($request->data);
      $toolkit = image_get_toolkit();
      $image = new stdClass();
      $image->source = $file->uri;
      $image->toolkit = $toolkit;
      $details = image_toolkit_invoke('get_info', $image);
      // Because Drupal is a little stupid when it comes to mime types, we need
      // to calculate the mime here.
      if (function_exists('finfo_open')) {
        $finfo = finfo_open(FILEINFO_MIME_TYPE);
        $filemime = finfo_file($finfo, $file->uri);
        $file->filemime = $filemime;
        finfo_close($finfo);
      }
      // file_unmanaged_delete($file->uri);
      // $file->uri = $data->mediaURL;
      $file->alt = '';
      $file->width = $details['width'];
      $file->height = $details['height'];
      $file->filename = basename($file->uri);
      try {
        file_save($file);
      } catch (Exception $e) {
        // Clear up all traces of the file, so the files table doesn't
        // Get populated with cruft
        file_delete($file, $force = true);
        echo ($e->getMessage());
        watchdog(WATCHDOG_ERROR, $e->getMessage());
        return false;
      }

      entity_save('eolapi', $eolapi);

      $lang = field_language('eolapi', $eolapi, 'eolapi_image');
      $eolapi->eolapi_image = array(
        $lang => array(
          (array)$file
        )
      );

    } else {
      // The download of the image failed.
      return false;
    }
  }
  entity_save('eolapi', $eolapi);
  return $eolapi;
}

function eolapi_is_service_available()
{
  $url = EOLAPI_URL . 'ping/' . EOLAPI_VERSION . '.json';
  $ping_request = drupal_http_request($url, array(
    'timeout' => EOLAPI_TIMEOUT
  ));
  if ($ping_request->code == 200) {
    $data = json_decode($ping_request->data);
    return $data->response->message == 'Success';
  }
  return false;
}

function curl_request($url)
{
  $ch = curl_init($url);
  curl_setopt($ch, CURLOPT_RETURNTRANSFER, true);
  curl_setopt($ch, CURLOPT_CONNECTTIMEOUT, EOLAPI_TIMEOUT);
  curl_setopt($ch, CURLOPT_HEADER, 0);
  $response = new stdClass();
  $response->data = curl_exec($ch);
  $response->code = curl_getinfo($ch, CURLINFO_HTTP_CODE);
  curl_close($ch);
  return $response;
}


/**
 * Gen an EOL API Data object.
 */
function eolapi_get($type, $id, $tid, $additional_query = '', $age = 604800)
{

  try {
    $query = db_select('eolapi', 'e')->condition('label', $id);
    if ($type != 'data_objects') {
      $query->condition('type', $type);
    }
    $row = $query->fields('e')->orderBy('changed', 'DESC')->execute()->fetch();
    if ($row) {
      // Note, we do a seperate load to ensure other modules can alter this eolapi
      // if they so wish.
      if (REQUEST_TIME - $age < $row->changed) {
        return entity_load_single('eolapi', $row->eid);
      }
    }

    if (eolapi_is_service_available()) {

      if ($type == 'search') {
        $url = EOLAPI_URL . $type . '/' . EOLAPI_VERSION . '.json?q=' . urlencode($id) . '&' . $additional_query;
      } else {
        $url = EOLAPI_URL . $type . '/' . EOLAPI_VERSION . '/' . fullescape(urlencode($id)) . '.json?' . $additional_query;
      }

      // We get the full amount of data that we can.
      $request = curl_request($url);

      if ($request->code == 200) {
        if (($data = json_decode($request->data)) != false) {
          // Search has worked, we should update the current $row (if set), or
          // save a new eolapi entity
          if ($row) {
            $eolapi = entity_load_single('eolapi', $row->eid);
          } else {
            // New eolapi data object
            $eolapi = new stdClass();
          }
          $eolapi->type = strtolower(($type == 'data_objects') ? $data->dataObjects[0]->dataType : $type);
          $eolapi->label = $id;
          $eolapi->data = serialize($data);
          if ($type == 'data_objects') {
            $eolapi->trusted = $data->dataObjects[0]->vettedStatus == 'Trusted' ? 1 : 0;
            $eolapi->license = $data->dataObjects[0]->license;
            $eolapi->rating = $data->dataObjects[0]->dataRating;
            $eolapi->data = serialize($data->dataObjects[0]);
            $eolapi->source = 'EOL';
          }
          if (substr($eolapi->type, 0, 4) == 'http') {
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
      } else {
        // Error here.  We simply return FALSE;
        return false;
      }
    } else {
      drupal_set_message(t('We are currently experiencing problems with the EOL service.'), 'error', false);
      if ($row) {
        return entity_load_single('eolapi', $row->eid);
      } else {
        return false;
      }
    }
  } catch (Exception $e) {
    watchdog('eolapi', 'Retrieval of data failed for: %id', array(
      '%id' => $id
    ), WATCHDOG_ERROR);
    return false;
  }
}

/**
 * Function taken from http://www.php.net/manual/en/function.urlencode.php#96394
 */
function fullescape($in)
{
  $in = str_replace('+', '%20', $in);
  $in = str_replace('_', '%5F', $in);
  $in = str_replace('.', '%2E', $in);
  $in = str_replace('-', '%2D', $in);
  return $in;
}
