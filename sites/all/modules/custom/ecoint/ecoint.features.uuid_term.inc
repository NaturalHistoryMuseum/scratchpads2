<?php
/**
 * @file
 * ecoint.features.uuid_term.inc
 */

/**
 * Implements hook_uuid_features_default_terms().
 */
function ecoint_uuid_features_default_terms() {
  $terms = array();

  $terms[] = array(
    'revision_id' => '9',
    'log' => NULL,
    'timestamp' => '1465229177',
    'uid' => '1',
    'name' => 'parasite of',
    'description' => '',
    'format' => 'plain_text',
    'weight' => '0',
    'uuid' => '46f4c73e-36e3-4acb-95f1-c397b6302484',
    'vocabulary_machine_name' => 'ecological_interactions',
    'field_ontology_term_url_reverse_' => array(
      'und' => array(
        0 => array(
          'value' => 'http://purl.obolibrary.org/obo/RO_0002445',
          'format' => NULL,
          'safe_value' => 'http://purl.obolibrary.org/obo/RO_0002445',
        ),
      ),
    ),
    'field_ontology_url' => array(
      'und' => array(
        0 => array(
          'value' => 'http://purl.obolibrary.org/obo/RO_0002444',
          'format' => NULL,
          'safe_value' => 'http://purl.obolibrary.org/obo/RO_0002444',
        ),
      ),
    ),
  );
  return $terms;
}
