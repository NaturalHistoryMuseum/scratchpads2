<?php

/**
 * Overview page for an EOL bundle.  This will allow users to clear the cache
 * and perform various other operations on the data.
 */
function eolapi_admin_page(){
  return array(
    'message' => array(
      '#markup' => '<p>' . t('Features coming soon!') . '</p>'
    )
  );
}

function eolapi_overview_page(){
  $object = new stdClass();
  $object->type = 'eolapi_search';
  $object->label = 'stuff';
  //eolapi_save($object);
  $data = eolapi_load(1);
  dpm($data);
  module_load_include('functions.inc', 'eolapi');
  $search_results = eolapi_search('Pediculus humanus');
  dpm($search_results);
  return array(
    'message' => array(
      '#markup' => '<p>' . t('Features coming soon!') . '</p>'
    )
  );
}