<?php
/**
 * @file
 * dummy.apachesolr_search_defaults.inc
 */

/**
 * Implements hook_apachesolr_search_default_searchers().
 */
function ecoint_apachesolr_search_default_searchers() {
  $export = array();

  $searcher = new stdClass();
  $searcher->disabled = FALSE; /* Edit this to true to make a default searcher disabled initially */
  $searcher->api_version = 3;
  $searcher->page_id = 'ecological_interactions';
  $searcher->label = 'Ecological Interactions';
  $searcher->description = '';
  $searcher->search_path = 'interactions';
  $searcher->page_title = 'Ecological Interactions';
  $searcher->env_id = 'solr';
  $searcher->settings = array(
    'fq' => array(
      0 => 'bundle:ecological_interactions',
      1 => 'entity_type:node',
    ),
    'apachesolr_search_custom_enable' => 1,
    'apachesolr_search_search_type' => 'custom',
    'apachesolr_search_per_page' => 10,
    'apachesolr_search_spellcheck' => 1,
    'apachesolr_search_search_box' => 0,
    'apachesolr_search_allow_user_input' => 0,
    'apachesolr_search_browse' => 'results',
  );
  $export['ecological_interactions'] = $searcher;

  return $export;
}
