<?php
/**
 * @file
 * agridrupal_views_rdf_oai.features.inc
 */

/**
 * Implements hook_ctools_plugin_api().
 */
function agridrupal_views_rdf_oai_ctools_plugin_api($module = NULL, $api = NULL) {
  if ($module == "arc2_store" && $api == "arc2_store") {
    return array("version" => "1");
  }
  if ($module == "strongarm" && $api == "strongarm") {
    return array("version" => "1");
  }
}

/**
 * Implements hook_views_api().
 */
function agridrupal_views_rdf_oai_views_api($module = NULL, $api = NULL) {
  return array("api" => "3.0");
}

/**
 * Implements hook_node_info().
 */
function agridrupal_views_rdf_oai_node_info() {
  $items = array(
    'agrepository' => array(
      'name' => t('Ag Repository'),
      'base' => 'node_content',
      'description' => t('This content type is for defining your repository\'s properties, especially for implementing special protocols / outputs for exchanging data.
For instance, it can be used to store the name of your repository, its OAI identifier, the available metadata formats, your Agris Center code (ARN) etc.
PLEASE ONLY CREATE ONE NODE OF THIS TYPE. ONLY THE FIRST NODE OF THIS TYPE WILL BE CONSIDERED ANYWAY.'),
      'has_title' => '1',
      'title_label' => t('Name'),
      'help' => '',
    ),
  );
  return $items;
}

/**
 * Implements hook_default_search_api_index().
 */
function agridrupal_views_rdf_oai_default_search_api_index() {
  $items = array();
  $items['node_rdf_index'] = entity_import('search_api_index', '{
    "name" : "Node RDF index",
    "machine_name" : "node_rdf_index",
    "description" : null,
    "server" : "rdf_indexing_server",
    "item_type" : "node",
    "options" : {
      "fields" : {
        "nid" : { "type" : "integer" },
        "type" : { "type" : "string" },
        "title" : { "type" : "text" },
        "language" : { "type" : "string" },
        "created" : { "type" : "date" },
        "changed" : { "type" : "date" },
        "author" : { "type" : "integer", "entity_type" : "user" },
        "field_ag_post_date" : { "type" : "date" },
        "field_ag_telephone" : { "type" : "text" },
        "field_ag_address" : { "type" : "text" },
        "field_ag_acronym" : { "type" : "text" },
        "field_ag_year" : { "type" : "date" },
        "field_ag_agris" : { "type" : "list\\u003Cinteger\\u003E", "entity_type" : "taxonomy_term" },
        "field_ag_email" : { "type" : "text" },
        "field_ag_agrovoc" : { "type" : "list\\u003Cinteger\\u003E", "entity_type" : "taxonomy_term" },
        "field_ag_institution_type" : { "type" : "integer", "entity_type" : "taxonomy_term" },
        "field_ag_lastname" : { "type" : "text" },
        "field_ag_name" : { "type" : "text" },
        "field_ag_degree" : { "type" : "text" },
        "field_ag_gender" : { "type" : "string" },
        "field_ag_study_field" : { "type" : "text" },
        "field_ag_position" : { "type" : "list\\u003Ctext\\u003E" },
        "field_ag_duties" : { "type" : "list\\u003Ctext\\u003E" },
        "field_ag_pub_title" : { "type" : "list\\u003Ctext\\u003E" },
        "field_ag_network" : { "type" : "list\\u003Cstring\\u003E" },
        "field_ag_network_id" : { "type" : "list\\u003Ctext\\u003E" },
        "field_ag_institution" : { "type" : "list\\u003Cinteger\\u003E", "entity_type" : "node" },
        "field_ag_deadline" : { "type" : "date" },
        "field_ag_employer" : { "type" : "text" },
        "field_ag_mispelling" : { "type" : "list\\u003Ctext\\u003E" },
        "field_ag_expert_record" : { "type" : "integer", "entity_type" : "node" },
        "field_ag_affiliation" : { "type" : "text" },
        "field_ag_number" : { "type" : "text" },
        "field_ag_volume" : { "type" : "text" },
        "field_ag_edition" : { "type" : "text" },
        "field_ag_isbn" : { "type" : "text" },
        "field_ag_issn" : { "type" : "text" },
        "field_ag_chkdatepub" : { "type" : "integer" },
        "field_ag_aproxdatepub" : { "type" : "text" },
        "field_ag_role" : { "type" : "list\\u003Cstring\\u003E" },
        "field_ag_link_to_jissue" : { "type" : "string" },
        "field_ag_rolecorp" : { "type" : "list\\u003Cstring\\u003E" },
        "field_ag_orderc" : { "type" : "list\\u003Cinteger\\u003E" },
        "field_ag_locator" : { "type" : "list\\u003Ctext\\u003E" },
        "field_ag_pagination" : { "type" : "text" },
        "field_ag_form" : { "type" : "string" },
        "field_ag_access" : { "type" : "string" },
        "field_ag_phlocation" : { "type" : "list\\u003Ctext\\u003E" },
        "field_ag_source" : { "type" : "list\\u003Ctext\\u003E" },
        "field_ag_publishplace" : { "type" : "text" },
        "field_ag_booknumber" : { "type" : "text" },
        "field_ag_notes" : { "type" : "text" },
        "field_ag_eissn" : { "type" : "text" },
        "field_ag_journal_issue" : { "type" : "integer", "entity_type" : "node" },
        "field_ag_author" : { "type" : "list\\u003Cinteger\\u003E", "entity_type" : "node" },
        "field_ag_corauthor" : { "type" : "list\\u003Cinteger\\u003E", "entity_type" : "node" },
        "field_ag_journal" : { "type" : "integer", "entity_type" : "node" },
        "field_ag_book" : { "type" : "integer", "entity_type" : "node" },
        "field_ag_confname" : { "type" : "integer", "entity_type" : "node" },
        "field_ag_publisher" : { "type" : "integer", "entity_type" : "node" },
        "field_ag_type" : { "type" : "integer", "entity_type" : "taxonomy_term" },
        "field_ag_acces" : { "type" : "integer", "entity_type" : "taxonomy_term" },
        "field_ag_subtitle" : { "type" : "list\\u003Ctext\\u003E" },
        "field_ag_datepub" : { "type" : "date" },
        "field_ag_abstract" : { "type" : "list\\u003Ctext\\u003E" },
        "field_pages" : { "type" : "text" },
        "field_ag_auth_order" : { "type" : "list\\u003Cinteger\\u003E" },
        "field_ag_institution_record" : { "type" : "integer", "entity_type" : "node" },
        "field_ag_conflocation" : { "type" : "text" },
        "search_api_language" : { "type" : "string" }
      },
      "data_alter_callbacks" : {
        "search_api_alter_bundle_filter" : {
          "status" : 1,
          "weight" : "-10",
          "settings" : { "default" : "0", "bundles" : { "agperiodical" : "agperiodical" } }
        },
        "search_api_alter_language_control" : {
          "status" : 0,
          "weight" : "0",
          "settings" : { "lang_field" : "", "languages" : [] }
        },
        "search_api_alter_node_access" : { "status" : 0, "weight" : "0", "settings" : [] },
        "search_api_alter_node_status" : { "status" : 0, "weight" : "0", "settings" : [] },
        "search_api_alter_add_hierarchy" : { "status" : 0, "weight" : "0", "settings" : { "fields" : [] } },
        "search_api_alter_add_viewed_entity" : { "status" : 0, "weight" : "0", "settings" : { "mode" : "full" } },
        "search_api_alter_add_url" : { "status" : 0, "weight" : "0", "settings" : [] },
        "search_api_alter_add_aggregation" : { "status" : 0, "weight" : "0", "settings" : [] },
        "rdf_indexer_alter_entity_public" : { "status" : 0, "weight" : "0", "settings" : [] }
      },
      "processors" : {
        "search_api_case_ignore" : {
          "status" : 0,
          "weight" : "0",
          "settings" : { "fields" : {
              "title" : true,
              "field_ag_telephone" : true,
              "field_ag_address" : true,
              "field_ag_acronym" : true,
              "field_ag_email" : true,
              "field_ag_lastname" : true,
              "field_ag_name" : true,
              "field_ag_degree" : true,
              "field_ag_study_field" : true,
              "field_ag_position" : true,
              "field_ag_duties" : true,
              "field_ag_pub_title" : true,
              "field_ag_network_id" : true,
              "field_ag_employer" : true,
              "field_ag_mispelling" : true,
              "field_ag_affiliation" : true,
              "field_ag_number" : true,
              "field_ag_volume" : true,
              "field_ag_edition" : true,
              "field_ag_isbn" : true,
              "field_ag_issn" : true,
              "field_ag_aproxdatepub" : true,
              "field_ag_locator" : true,
              "field_ag_pagination" : true,
              "field_ag_phlocation" : true,
              "field_ag_source" : true,
              "field_ag_publishplace" : true,
              "field_ag_booknumber" : true,
              "field_ag_notes" : true,
              "field_ag_eissn" : true,
              "field_ag_subtitle" : true,
              "field_ag_abstract" : true,
              "field_pages" : true,
              "field_ag_conflocation" : true
            }
          }
        },
        "search_api_html_filter" : {
          "status" : 0,
          "weight" : "10",
          "settings" : {
            "fields" : {
              "title" : true,
              "field_ag_telephone" : true,
              "field_ag_address" : true,
              "field_ag_acronym" : true,
              "field_ag_email" : true,
              "field_ag_lastname" : true,
              "field_ag_name" : true,
              "field_ag_degree" : true,
              "field_ag_study_field" : true,
              "field_ag_position" : true,
              "field_ag_duties" : true,
              "field_ag_pub_title" : true,
              "field_ag_network_id" : true,
              "field_ag_employer" : true,
              "field_ag_mispelling" : true,
              "field_ag_affiliation" : true,
              "field_ag_number" : true,
              "field_ag_volume" : true,
              "field_ag_edition" : true,
              "field_ag_isbn" : true,
              "field_ag_issn" : true,
              "field_ag_aproxdatepub" : true,
              "field_ag_locator" : true,
              "field_ag_pagination" : true,
              "field_ag_phlocation" : true,
              "field_ag_source" : true,
              "field_ag_publishplace" : true,
              "field_ag_booknumber" : true,
              "field_ag_notes" : true,
              "field_ag_eissn" : true,
              "field_ag_subtitle" : true,
              "field_ag_abstract" : true,
              "field_pages" : true,
              "field_ag_conflocation" : true
            },
            "title" : 0,
            "alt" : 1,
            "tags" : "h1 = 5\\r\\nh2 = 3\\r\\nh3 = 2\\r\\nstrong = 2\\r\\nb = 2\\r\\nem = 1.5\\r\\nu = 1.5"
          }
        },
        "search_api_tokenizer" : {
          "status" : 0,
          "weight" : "20",
          "settings" : {
            "fields" : {
              "title" : true,
              "field_ag_telephone" : true,
              "field_ag_address" : true,
              "field_ag_acronym" : true,
              "field_ag_email" : true,
              "field_ag_lastname" : true,
              "field_ag_name" : true,
              "field_ag_degree" : true,
              "field_ag_study_field" : true,
              "field_ag_position" : true,
              "field_ag_duties" : true,
              "field_ag_pub_title" : true,
              "field_ag_network_id" : true,
              "field_ag_employer" : true,
              "field_ag_mispelling" : true,
              "field_ag_affiliation" : true,
              "field_ag_number" : true,
              "field_ag_volume" : true,
              "field_ag_edition" : true,
              "field_ag_isbn" : true,
              "field_ag_issn" : true,
              "field_ag_aproxdatepub" : true,
              "field_ag_locator" : true,
              "field_ag_pagination" : true,
              "field_ag_phlocation" : true,
              "field_ag_source" : true,
              "field_ag_publishplace" : true,
              "field_ag_booknumber" : true,
              "field_ag_notes" : true,
              "field_ag_eissn" : true,
              "field_ag_subtitle" : true,
              "field_ag_abstract" : true,
              "field_pages" : true,
              "field_ag_conflocation" : true
            },
            "spaces" : "[^[:alnum:]]",
            "ignorable" : "[\\u0027]"
          }
        },
        "search_api_stopwords" : {
          "status" : 0,
          "weight" : "30",
          "settings" : {
            "fields" : {
              "title" : true,
              "field_ag_telephone" : true,
              "field_ag_address" : true,
              "field_ag_acronym" : true,
              "field_ag_email" : true,
              "field_ag_lastname" : true,
              "field_ag_name" : true,
              "field_ag_degree" : true,
              "field_ag_study_field" : true,
              "field_ag_position" : true,
              "field_ag_duties" : true,
              "field_ag_pub_title" : true,
              "field_ag_network_id" : true,
              "field_ag_employer" : true,
              "field_ag_mispelling" : true,
              "field_ag_affiliation" : true,
              "field_ag_number" : true,
              "field_ag_volume" : true,
              "field_ag_edition" : true,
              "field_ag_isbn" : true,
              "field_ag_issn" : true,
              "field_ag_aproxdatepub" : true,
              "field_ag_locator" : true,
              "field_ag_pagination" : true,
              "field_ag_phlocation" : true,
              "field_ag_source" : true,
              "field_ag_publishplace" : true,
              "field_ag_booknumber" : true,
              "field_ag_notes" : true,
              "field_ag_eissn" : true,
              "field_ag_subtitle" : true,
              "field_ag_abstract" : true,
              "field_pages" : true,
              "field_ag_conflocation" : true
            },
            "file" : "",
            "stopwords" : "but\\r\\ndid\\r\\nthe this that those\\r\\netc"
          }
        }
      }
    },
    "enabled" : "1",
    "read_only" : "0",
    "rdf_mapping" : []
  }');
  $items['taxonomy_rdf_index'] = entity_import('search_api_index', '{
    "name" : "taxonomy rdf index",
    "machine_name" : "taxonomy_rdf_index",
    "description" : null,
    "server" : "rdf_indexing_server",
    "item_type" : "taxonomy_term",
    "options" : {
      "fields" : {
        "tid" : { "type" : "integer" },
        "name" : { "type" : "text" },
        "description" : { "type" : "text" },
        "url" : { "type" : "uri" },
        "field_agrovoc_uri" : { "type" : "text" },
        "search_api_language" : { "type" : "string" }
      },
      "data_alter_callbacks" : {
        "search_api_alter_bundle_filter" : {
          "status" : 0,
          "weight" : "-10",
          "settings" : { "default" : "1", "bundles" : [] }
        },
        "search_api_alter_add_hierarchy" : { "status" : 0, "weight" : "0", "settings" : { "fields" : [] } },
        "search_api_alter_language_control" : {
          "status" : 0,
          "weight" : "0",
          "settings" : { "lang_field" : "", "languages" : [] }
        },
        "search_api_alter_add_viewed_entity" : { "status" : 0, "weight" : "0", "settings" : { "mode" : "full" } },
        "search_api_alter_add_aggregation" : { "status" : 0, "weight" : "0", "settings" : [] },
        "search_api_alter_add_url" : { "status" : 0, "weight" : "0", "settings" : [] },
        "rdf_indexer_alter_entity_public" : { "status" : 0, "weight" : "0", "settings" : [] }
      },
      "processors" : {
        "search_api_case_ignore" : {
          "status" : 0,
          "weight" : "0",
          "settings" : { "fields" : { "name" : true, "description" : true, "field_agrovoc_uri" : true } }
        },
        "search_api_html_filter" : {
          "status" : 0,
          "weight" : "10",
          "settings" : {
            "fields" : { "name" : true, "description" : true, "field_agrovoc_uri" : true },
            "title" : 0,
            "alt" : 1,
            "tags" : "h1 = 5\\r\\nh2 = 3\\r\\nh3 = 2\\r\\nstrong = 2\\r\\nb = 2\\r\\nem = 1.5\\r\\nu = 1.5"
          }
        },
        "search_api_tokenizer" : {
          "status" : 0,
          "weight" : "20",
          "settings" : {
            "fields" : { "name" : true, "description" : true, "field_agrovoc_uri" : true },
            "spaces" : "[^[:alnum:]]",
            "ignorable" : "[\\u0027]"
          }
        },
        "search_api_stopwords" : {
          "status" : 0,
          "weight" : "30",
          "settings" : {
            "fields" : { "name" : true, "description" : true, "field_agrovoc_uri" : true },
            "file" : "",
            "stopwords" : "but\\r\\ndid\\r\\nthe this that those\\r\\netc"
          }
        }
      }
    },
    "enabled" : "1",
    "read_only" : "0",
    "rdf_mapping" : []
  }');
  return $items;
}

/**
 * Implements hook_default_search_api_server().
 */
function agridrupal_views_rdf_oai_default_search_api_server() {
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
