<?php
/**
 * @file
 * agridrupal_content_types.features.inc
 */

/**
 * Implements hook_ctools_plugin_api().
 */
function agridrupal_content_types_ctools_plugin_api($module = NULL, $api = NULL) {
  if ($module == "arc2_store" && $api == "arc2_store") {
    return array("version" => "1");
  }
  if ($module == "custom_formatters" && $api == "custom_formatters") {
    return array("version" => "2");
  }
  if ($module == "field_group" && $api == "field_group") {
    return array("version" => "1");
  }
  if ($module == "strongarm" && $api == "strongarm") {
    return array("version" => "1");
  }
}

/**
 * Implements hook_views_api().
 */
function agridrupal_content_types_views_api($module = NULL, $api = NULL) {
  return array("api" => "3.0");
}

/**
 * Implements hook_node_info().
 */
function agridrupal_content_types_node_info() {
  $items = array(
    'agauthor' => array(
      'name' => t('Ag Author'),
      'base' => 'node_content',
      'description' => t('This content type is for storing information about Authors of the documents in your repository: storing information on authors in a dedicated content type allows to maintain a reference / authority list.'),
      'has_title' => '1',
      'title_label' => t('Title'),
      'help' => '',
    ),
    'agconference' => array(
      'name' => t('Ag Conference'),
      'base' => 'node_content',
      'description' => t('This content type is for storing information about Conferences to which conference documents in your repository relate: storing information on conferences in a dedicated content type allows to maintain a reference / authority list.'),
      'has_title' => '1',
      'title_label' => t('Name'),
      'help' => '',
    ),
    'agcorporate_body' => array(
      'name' => t('Ag Corporate body'),
      'base' => 'node_content',
      'description' => t('This content type is for storing information about corporate authors of the documents in your repository: storing information on corporate authors in a dedicated content type allows to maintain a reference / authority list.'),
      'has_title' => '1',
      'title_label' => t('Name'),
      'help' => '',
    ),
    'agdlios' => array(
      'name' => t('Ag Document'),
      'base' => 'node_content',
      'description' => t('This content type is for Document-Like-Information-Objects: generically, documents and resources that have a title, an author and a topic.
If you manage a repository, all resources in your repository will be of this type.'),
      'has_title' => '1',
      'title_label' => t('Title'),
      'help' => '',
    ),
    'agevent' => array(
      'name' => t('Ag_Event'),
      'base' => 'node_content',
      'description' => t('This content type is for storing information on events. Country, start date and end date should always be provided for events. Events will appear on the homepage if the news and events block is on the homepage, and under "Our events". Events are exposed in an Ag-Event AP compliant feed that can be submitted to AgriFeeds as an event feed.'),
      'has_title' => '1',
      'title_label' => t('Title'),
      'help' => '',
    ),
    'aginstitution' => array(
      'name' => t('Ag Institution'),
      'base' => 'node_content',
      'description' => t('Profiles of institutions / organizations in the field of agriculture.
Users registering their expert profiles can also add records to the directory of Institutions and link them from their profile.'),
      'has_title' => '1',
      'title_label' => t('Name'),
      'help' => '',
    ),
    'agnews' => array(
      'name' => t('Ag News'),
      'base' => 'node_content',
      'description' => t('This content type is for storing news and announcements that will appear on the homepage and in the "News" section.'),
      'has_title' => '1',
      'title_label' => t('Title'),
      'help' => '',
    ),
    'agperiodical' => array(
      'name' => t('Ag_Periodical'),
      'base' => 'node_content',
      'description' => t('This content type is for periodical/journals.
'),
      'has_title' => '1',
      'title_label' => t('Title'),
      'help' => '',
    ),
    'agprofile' => array(
      'name' => t('Ag Expert profile'),
      'base' => 'node_content',
      'description' => t('Profiles of experts in different areas of agricultural research.
This content type is configured as "content profile", so each Expert profile is associated to a user and each user can only create his/her own expert profile and only have one.'),
      'has_title' => '1',
      'title_label' => t('Full name'),
      'help' => '',
    ),
    'agrssevents' => array(
      'name' => t('Ag RSS event'),
      'base' => 'node_content',
      'description' => t('Items from an RSS feed of Events.
DO NOT SUBMIT MANUALLY, IT MUST BE HARVESTED.'),
      'has_title' => '1',
      'title_label' => t('Title'),
      'help' => '',
    ),
    'agrssnews' => array(
      'name' => t('Ag RSS news item'),
      'base' => 'node_content',
      'description' => t('Items from an RSS feed of news.
DO NOT SUBMIT MANUALLY, IT MUST BE HARVESTED.'),
      'has_title' => '1',
      'title_label' => t('Title'),
      'help' => '',
    ),
  );
  return $items;
}

/**
 * Implements hook_default_search_api_server().
 */
function agridrupal_content_types_default_search_api_server() {
  $items = array();
  $items['rdf_indexing_server'] = entity_import('search_api_server', '{
    "name" : "RDF indexing server",
    "machine_name" : "rdf_indexing_server",
    "description" : "",
    "class" : "rdf_indexer_service",
    "options" : { "store_id" : "agridrupal" },
    "enabled" : "1",
    "rdf_mapping" : []
  }');
  return $items;
}
