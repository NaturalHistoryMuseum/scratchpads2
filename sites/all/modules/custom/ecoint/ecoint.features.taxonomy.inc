<?php

/**
 * @file
 * ecoint.features.taxonomy.inc
 */
/**
 * Implements hook_taxonomy_default_vocabularies().
 */
function ecoint_taxonomy_default_vocabularies(){
  return array(
    'ecological_interactions' => array(
      'name' => 'Ecological Interactions',
      'machine_name' => 'ecological_interactions',
      'description' => '',
      'hierarchy' => '0',
      'module' => 'taxonomy',
      'weight' => '0',
      'rdf_mapping' => array(
        'rdftype' => array(
          0 => 'skos:ConceptScheme'
        ),
        'name' => array(
          'predicates' => array(
            0 => 'dc:title'
          )
        ),
        'description' => array(
          'predicates' => array(
            0 => 'rdfs:comment'
          )
        )
      ),
      'show_synonym_data' => FALSE,
      'biological_classification' => 0,
      'use_scratchpads_species_pages' => 0
    ),
    'presence_status' => array(
      'name' => 'Presence Status',
      'machine_name' => 'presence_status',
      'description' => '',
      'hierarchy' => '0',
      'module' => 'taxonomy',
      'weight' => '0',
      'rdf_mapping' => array(
        'rdftype' => array(
          0 => 'skos:ConceptScheme'
        ),
        'name' => array(
          'predicates' => array(
            0 => 'dc:title'
          )
        ),
        'description' => array(
          'predicates' => array(
            0 => 'rdfs:comment'
          )
        )
      ),
      'show_synonym_data' => FALSE,
      'biological_classification' => 0,
      'use_scratchpads_species_pages' => 0
    )
  );
}
