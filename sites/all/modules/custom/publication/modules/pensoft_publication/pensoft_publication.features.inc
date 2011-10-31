<?php
/**
 * @file
 * pensoft_publication.features.inc
 */

/**
 * Implementation of hook_default_publication_type().
 */
function pensoft_publication_default_publication_type() {
  $items = array();
  $items['pensoft'] = entity_import('publication_type', '{
    "type" : "pensoft",
    "label" : "Pensoft",
    "weight" : "0",
    "rdf_mapping" : []
  }');
  return $items;
}
