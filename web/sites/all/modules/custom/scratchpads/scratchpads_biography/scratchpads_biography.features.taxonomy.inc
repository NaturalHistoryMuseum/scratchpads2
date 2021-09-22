<?php
/**
 * @file
 * scratchpads_biography.features.taxonomy.inc
 */

/**
 * Implements hook_taxonomy_default_vocabularies().
 */
function scratchpads_biography_taxonomy_default_vocabularies() {
  return array(
    'user_keywords' => array(
      'name' => 'User keywords',
      'machine_name' => 'user_keywords',
      'description' => 'Keywords',
      'hierarchy' => '0',
      'module' => 'taxonomy',
      'weight' => '0',
      'rdf_mapping' => array(
        'rdftype' => array(
          0 => 'skos:ConceptScheme',
        ),
        'name' => array(
          'predicates' => array(
            0 => 'dc:title',
          ),
        ),
        'description' => array(
          'predicates' => array(
            0 => 'rdfs:comment',
          ),
        ),
      ),
      'show_synonym_data' => FALSE,
      'biological_classification' => 0,
      'use_scratchpads_species_pages' => 0,
    ),
  );
}
