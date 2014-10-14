<?php
/**
 * @file
 * agridrupal_views_rdf_oai.rules_defaults.inc
 */

/**
 * Implements hook_default_rules_configuration().
 */
function agridrupal_views_rdf_oai_default_rules_configuration() {
  $items = array();
  $items['rules_agrovoc_uri_on_term_creation'] = entity_import('rules_config', '{ "rules_agrovoc_uri_on_term_creation" : {
      "LABEL" : "Fill AGROVOC URI for taxonomy term in AGROVOC taxonomy on creation",
      "PLUGIN" : "reaction rule",
      "TAGS" : [ "agrovoc", "taxonomy" ],
      "REQUIRES" : [ "rules", "php", "taxonomy" ],
      "ON" : [ "taxonomy_term_insert" ],
      "IF" : [
        { "entity_has_field" : { "entity" : [ "term" ], "field" : "field_agrovoc_uri" } }
      ],
      "DO" : [
        { "entity_fetch" : {
            "USING" : { "type" : "taxonomy_term", "id" : [ "term:tid" ] },
            "PROVIDE" : { "entity_fetched" : { "termentity_fetched" : "term-entity" } }
          }
        },
        { "php_eval" : { "code" : "\\/\\/ It saves the entity again if the URI is still empty (because the URI computed field is not filled on first save)\\r\\n$uri = $termentity_fetched-\\u003Efield_agrovoc_uri[\\u0022und\\u0022][0][\\u0027value\\u0027];\\r\\nif ($uri == \\u0022\\u0022) {\\r\\n  entity_save(\\u0022taxonomy_term\\u0022, $termentity_fetched);\\r\\n}" } }
      ]
    }
  }');
  $items['rules_check_journal_during_import'] = entity_import('rules_config', '{ "rules_check_journal_during_import" : {
      "LABEL" : "Check_journal_during_import",
      "PLUGIN" : "reaction rule",
      "REQUIRES" : [ "rules" ],
      "ON" : [ "node_presave" ],
      "IF" : [
        { "node_is_of_type" : { "node" : [ "node" ], "type" : { "value" : { "agdlios" : "agdlios" } } } },
        { "AND" : [] },
        { "NOT data_is_empty" : { "data" : [ "node:field-ag-journal" ] } }
      ],
      "DO" : [ { "data_set" : { "data" : [ "node:field-ag-type" ], "value" : "9" } } ]
    }
  }');
  $items['rules_replace_title_with_alternative_when_is_missing'] = entity_import('rules_config', '{ "rules_replace_title_with_alternative_when_is_missing" : {
      "LABEL" : "replace_title_with_alternative_when_is_missing",
      "PLUGIN" : "reaction rule",
      "WEIGHT" : "2",
      "REQUIRES" : [ "rules" ],
      "ON" : [ "node_presave" ],
      "IF" : [
        { "node_is_of_type" : { "node" : [ "node" ], "type" : { "value" : { "agdlios" : "agdlios" } } } },
        { "AND" : [] },
        { "data_is_empty" : { "data" : [ "node:title" ] } }
      ],
      "DO" : [
        { "data_set" : { "data" : [ "node:title" ], "value" : [ "node:field-ag-subtitle:0" ] } }
      ]
    }
  }');
  $items['rules_set_document_type_to_other_when_is_missing'] = entity_import('rules_config', '{ "rules_set_document_type_to_other_when_is_missing" : {
      "LABEL" : "set_document_type_to other_when_is_missing",
      "PLUGIN" : "reaction rule",
      "WEIGHT" : "1",
      "REQUIRES" : [ "rules" ],
      "ON" : [ "node_presave" ],
      "IF" : [
        { "node_is_of_type" : { "node" : [ "node" ], "type" : { "value" : { "agdlios" : "agdlios" } } } },
        { "AND" : [] },
        { "data_is_empty" : { "data" : [ "node:field-ag-type" ] } }
      ],
      "DO" : [ { "data_set" : { "data" : [ "node:field-ag-type" ], "value" : "14" } } ]
    }
  }');
  return $items;
}
