<?php
/* double-commented to avoid conflict with svn
 */

/**
 * @file
 *   Highly experimental attempt to hook into the undocumented rdf.module
 * and provide taxonomy term data
 */

module_load_include('inc', 'rdf', 'rdf.pages');

/**
 * Similar to rdf_rdf_export_node()
 */
function taxonomy_xml_rdf_export_term($term) {
  $term = is_numeric($term) ? taxonomy_term_load($term) : $term;
  $data = taxonomy_xml_term_as_rdf_data($term);
  rdf_export($data, 'term-' . $term->tid);
}

/**
 * When asked for a RDF representation of a taxonomy term, return a merge of
 * both the extrinsic RDF statements that have been made about that term (using
 * the RDF repository-triplestore and cannonic taxonomy/term/n notation) and
 * also the Drupal internal fields that make up that term.
 *
 * As a bonus, also list the children and parents of this term.
 */
function taxonomy_xml_term_as_rdf_data($term) {
  // Expect an object, but the menu callback may have just provided the tid
  $term = is_numeric($term) ? taxonomy_term_load($term) : $term;

  $subject_uri  = url('taxonomy/term/' . $term->tid, array('absolute' => TRUE));
  $subject_curi = 'term:' . $term->tid;
  $subject      = rdf_is_valid_curie($subject_curi) ? rdf_qname_to_uriref($subject_curi) : rdf_uri($subject_curi);

  $data = rdf_query($subject_uri);

  // $data is something called  RDF_QueryIterator extends AppendIterator ??
  // I have no idea how to add things to that. Flatten that (we were about to do that anyway)
  // and add more statements
  $data = rdf_normalize($data);

  dpm($term);

  // Get normal internal $term values from Drupal
  // and represent them as RDF statements
  $statements = array(
    'rdf:type' => rdf_qname_to_uriref('rdfs:Class'),
    'rdfs:label' => rdf_literal($term->name),
    'rdfs:isDefinedBy' => rdf_uri( url('taxonomy/vocabulary/' . $term->vid, array('absolute' => TRUE))),
  );
  if (!empty($term->description)) {
    $statements['rdfs:comment'] = rdf_literal($term->description);
  }
  foreach ((array) $term->parent as $ptid) {
    $statements['rdfs:subClassOf'][$ptid] = rdf_uri( url('taxonomy/term/' . $ptid, array('absolute' => TRUE)));
  }
  foreach ((array) $term->children as $ctid => $child) {
    $statements['rdfs:superClassOf'][$ctid] = rdf_uri( url('taxonomy/term/' . $ctid, array('absolute' => TRUE)));
  }
  foreach ((array) $term->synonyms as $i => $synonym) {
    $statements['owl:equivalentClass'][$i] = rdf_literal($synonym);
  }
  foreach ((array) $term->related as $i => $seealso) {
    $statements['rdfs:seeAlso'][$i] = rdf_uri( url('taxonomy/term/' . $seealso, array('absolute' => TRUE)));
  }

  $data = convert_statements_to_rdf_data($subject_uri, $statements);
  return $data;
}


/**
 * Return an RDF representation of the requested vocab.
 * 
 * Include references to all the top-level child terms.
 * 
 */
function taxonomy_xml_rdf_export_vocabulary($vocabulary) {
  // Expect an object, but the menu callback may have just provided the id
  $vocabulary = is_numeric($vocabulary) ? taxonomy_vocabulary_load($vocabulary) : $vocabulary;

  // This URI may not resolve unless you are running taxonomy_server, but 
  // that's fine - it doesn't have to.
  $subject_uri  = url('taxonomy/vocabulary/' . $vocabulary->vid, array('absolute' => TRUE));
  $subject_curi = 'vocabulary:' . $vocabulary->vid;

dpm($vocabulary);

  $XMLDoc = new DOMDocument();
  $XMLRDF = $XMLDoc->createElementNS(TAXONOMY_XML_RDF_NS, "rdf:RDF" );
  $XMLDoc->appendChild($XMLRDF);
  
  $vocab_xml = rdf_entity_to_xml($vocabulary, $XMLDoc);
  $vocab_xml->setAttributeNS(TAXONOMY_XML_RDF_NS, 'rdf:about', $subject_uri);
  $XMLRDF->appendChild($vocab_xml);
  
  // Now add the terms in this vocabulary
  // They are listed as siblings, not children of the ontology
  // TODO work with max_depth
  $parent = 0;
  $depth = -1;
  $max_depth = NULL;
  $max_depth = 2;
  $tree = taxonomy_get_tree($vocabulary->vid, $parent, $max_depth, $depth);
  foreach ($tree as $term) {
    // need a full term load to retrieve the mapping and some extras
    $term = taxonomy_term_load($term->tid);
    if ( $term_xml = rdf_entity_to_xml($term, $XMLDoc) ) {
      $term_uri = taxonomy_term_uri($term);
      $path = url($term_uri['path'], array('absolute' => TRUE));
      // Why does that return an array now?
      $term_xml->setAttributeNS(TAXONOMY_XML_RDF_NS, 'rdf:about', $path);
      
      $vocab_xml->appendChild($term_xml);
    }
  }


return ('<textarea style="width:100%; height:300px">' . $XMLDoc->saveXML() . '</textarea>');

  $subject      = rdf_is_valid_curie($subject_curi) ? rdf_qname_to_uriref($subject_curi) : rdf_uri($subject_curi);

  $data = rdf_query($subject_uri);
  // $data is something called  RDF_QueryIterator extends AppendIterator ??
  // I have no idea how to add things to that. Flatten that (we were about to do that anyway)
  // and add more statements
  $data = rdf_normalize($data);

  #dpm($vocabulary);

  // Get normal internal $term values from Drupal
  // and represent them as RDF statements
  $statements = array(
    'rdf:type' => rdf_qname_to_uriref('owl:Ontology'),
    'rdfs:label' => rdf_literal($vocabulary->name),
  );
  if (!empty($vocabulary->description)) {
    $statements['rdfs:comment'] = rdf_literal($vocabulary->description);
  }
  $data = convert_statements_to_rdf_data($subject_uri, $statements);

  // Now add the terms in this vocabulary
  // They are listed as siblings, not children of the ontology
  // TODO work with max_depth
  $parent = 0;
  $depth = -1;
  $max_depth = NULL;
  $max_depth = 2;
  $tree = taxonomy_get_tree($vocabulary->vid, $parent, $max_depth, $depth);
  foreach ($tree as $term) {
    $term_data = taxonomy_xml_term_as_rdf_data($term);
    $data += $term_data;
  }
  rdf_export($data, 'vocabulary-' . $term->tid);
}

/**
 * Given $statements=array('rdfs:type' => 'thing', 'dc:title' => 'A THING');
 * return that in an objectified $data array that RDF can serialize better
 */
function convert_statements_to_rdf_data($subject_uri, $statements) {
  // Convert CURIE statements to full statement resource refs
  foreach ($statements as $predicate => $objects) {
    $predicate = rdf_is_valid_curie($predicate) ? rdf_qname_to_uriref($predicate) : rdf_uri($predicate);
    $objects = rdf_objects($objects);
    foreach ($objects as $key => $object) {
      $objects[$key] = rdf_is_valid_uri($object) ? rdf_uri($object) : $object;
    }
    // Add this data to the entity
    $data[$subject_uri][$predicate->uri] = $objects;
  }
  return $data;
}

////////////////////////////////////////////////////////////////////////////////
// I have no idea what this under here means - it's just bits pulled out to look at.

/* A schema, when produced by the system, looks something like: */

/*

 <rdfs:Class rdf:about="http://taxonomy.drupal6.gadget/rdf/schema/term">
 <rdf:type rdf:resource="http://www.w3.org/2000/01/rdf-schema#Class"/>
 <rdfs:isDefinedBy rdf:resource="http://taxonomy.drupal6.gadget/rdf/schema/"/>
 <rdfs:subClassOf rdf:resource="http://www.w3.org/2000/01/rdf-schema#Resource"/>
 <rdfs:label xml:lang="en">Taxonomy term</rdfs:label>
 </rdfs:Class>

 <rdf:Property rdf:about="http://taxonomy.drupal6.gadget/rdf/schema/term#tid">
 <rdf:type rdf:resource="http://www.w3.org/1999/02/22-rdf-syntax-ns#Property"/>
 <rdfs:isDefinedBy rdf:resource="http://taxonomy.drupal6.gadget/rdf/schema/"/>
 <rdfs:domain rdf:resource="http://taxonomy.drupal6.gadget/rdf/schema/term"/>
 <rdfs:range rdf:resource="http://www.w3.org/2000/01/rdf-schema#Literal"/>
 <rdfs:label xml:lang="en">ID</rdfs:label>
 </rdf:Property>

 * */


/**
 * Implementation of hook_rdf_classes().
 *
 * This returns schema details describing the properties that make up the idea
 * of a drupal term.
 *
 * Much of it is boilerplated on by rdf_schema_load_schema_class()
 */
function taxonomy_xml_rdf_classes() {
  return array(
    'vocabulary' => array(
      'title'   => t('Taxonomy vocabulary'),
      'module'  => 'taxonomy',
      'table'   => 'vocabulary',
      'query'   => 'SELECT vid FROM {vocabulary}',
      // NOTE: Drupal vocabularies don't actually have dereferenceable URIs
      'uri'     => 'taxonomy/vocabulary/%vid',
      'enabled' => TRUE, // TODO
    ),
    'term' => array(
      'title'   => t('Taxonomy term'),
      'module'  => 'taxonomy',
      'table'   => 'term_data',
      'query'   => 'SELECT tid FROM {term_data}',
      'uri'     => 'taxonomy/term/%tid',
      'enabled' => TRUE, // TODO
    ),
  );
}
/**
 * Implementation of hook_rdf_resources().
 *
 * ... which is entirely undocumented :(
 * in rdf.module it appears to be intended return a list of entity definitions
 * as found in rdf.schema.inc
 */
function taxonomy_xml_rdf_resources($context) {
  switch ($context) {
    case NULL:
      return array(); // FIXME
      //return array_merge(rdf_schema_get_resources(), rdf_schema_get_classes());
    case RDF_SITE_URI:
      return array(RDF_SITE_URI => new RDF_QueryCallback('rdf_load_site'));
      //return rdf_schema_get_resources(); // rdf_schema.module
    case RDF_SCHEMA_URI:
      return rdf_schema_get_classes();
  }
}


function taxonomy_xml_rdf_schema_load_term($term) {
  $term = is_object($term) ? $term : taxonomy_term_load((int) $term);

  $statements = array(
    rdf::type         => rdf_uri(rdf_qname_to_uri('drupal:term')),
    'term:tid'        => (int) $term->tid,
    'term:vid'        => (int) $term->vid,
    'term:name'      => $term->name,
    'term:body'       => $term->description,
  );

  // Get normal internal $term values from Drupal
  // and represent them as RDF statements
  $statements = array(
    'rdf:type' => rdf_qname_to_uriref('rdfs:Class'),
    'rdfs:label' => rdf_literal($term->name),
    'rdfs:isDefinedBy' => rdf_uri( url('taxonomy/vocabulary/' . $term->vid, array('absolute' => TRUE))),
  );
  if (!empty($term->description)) {
    $statements['rdfs:comment'] = rdf_literal($term->description);
  }
  foreach ((array) $term->parent as $ptid) {
    $statements['rdfs:subClassOf'][$ptid] = rdf_uri( url('taxonomy/term/' . $ptid, array('absolute' => TRUE)));
  }
  foreach ((array) $term->synonyms as $i => $synonym) {
    $statements['owl:equivalentClass'][$i] = rdf_literal($synonym);
  }
  foreach ((array) $term->related as $i => $seealso) {
    $statements['rdfs:seeAlso'][$i] = rdf_uri( url('taxonomy/term/' . $seealso, array('absolute' => TRUE)));
  }

  return $data;
}
