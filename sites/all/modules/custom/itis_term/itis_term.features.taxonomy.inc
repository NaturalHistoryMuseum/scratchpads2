<?php

/**
 * Implementation of hook_taxonomy_default_vocabularies().
 */
function itis_term_taxonomy_default_vocabularies() {
  return array(
    'itis_vocabulary' => array(
      'name' => 'ITIS Vocabulary',
      'machine_name' => 'itis_vocabulary',
      'description' => '',
      'hierarchy' => '0',
      'module' => 'taxonomy',
      'weight' => '0',
      'rdf_mapping' => array(
        'rdftype' => array(
          '0' => 'skos:ConceptScheme',
        ),
        'name' => array(
          'predicates' => array(
            '0' => 'dc:title',
          ),
        ),
        'description' => array(
          'predicates' => array(
            '0' => 'rdfs:comment',
          ),
        ),
      ),
    ),
  );
}
