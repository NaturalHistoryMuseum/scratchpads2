<?php
// $ID:  $
/**
 * @file
 *   Experimental library for working with RDF within taxonomy_xml
 * 
 * Created on Apr 6, 2010
 *
 * @author 'dman' Dan Morrison http://coders.co.nz/
 */

define('TAXONOMY_XML_RDF_NS', 'http://www.w3.org/1999/02/22-rdf-syntax-ns#');
define('TAXONOMY_XML_TYPE', TAXONOMY_XML_RDF_NS . 'type');
define('TAXONOMY_XML_UNTYPED', 'UNTYPED');

define('TAXONOMY_XML_RDFS_NS', 'http://www.w3.org/2000/01/rdf-schema#');

// See  http://www.w3.org/2004/12/q/doc/rdf-labels.html
define('TAXONOMY_XML_CONTENTLABEL_NS', 'http://www.w3.org/2004/12/q/contentlabel#');
define('TAXONOMY_XML_CATEGORY', TAXONOMY_XML_CONTENTLABEL_NS . 'Category');

// OWL - Web Ontology Language - Formalized Meaning and Logic
define('TAXONOMY_XML_OWL_NS', 'http://www.w3.org/2002/07/owl#');

define('TAXONOMY_XML_W3C_WN', 'http://www.w3.org/2006/03/wn/wn20/');
define('TAXONOMY_XML_W3C_WN_SCHEMA', TAXONOMY_XML_W3C_WN . 'schema/');

// Dublin Core - Metadata standards
define('TAXONOMY_XML_DC_NS', 'http://purl.org/dc/elements/1.1/');
// Simple Knowledge Organization System - Structural information management
define('TAXONOMY_XML_SKOS_NS', 'http://www.w3.org/2004/02/skos/core#');
// Taxonomic Database Working Group - Biodiversity Information Standards (LSIDs etc)
define('TAXONOMY_XML_TDWG_NS', 'http://rs.tdwg.org/ontology/voc/Collection#');

// Freebase data
define('TAXONOMY_XML_FB_NS', 'http://rdf.freebase.com/ns/');


/**
 * Given a D7 entity with its 'rdf_mapping' available, create and fill in data
 * on an RDF/XML node in an XML document.
 * 
 * Fully supports namespaces, even though internally the RDF mapping only uses
 * CURIE notation.
 * 
 * @param an Drupal entity. Should contain $entity->rdf_mapping instructions.
 * @param handle on an XML Document to build the element within.
 * @param $domcontainer element to add this structure to. While it's not
 * required, alloing the DOM constructor here to add early will assist in
 * reducing namespace clutter!
 * @returns an XML node containing the data. Does not actually add the element
 * to the DOM
 */
function rdf_entity_to_xml($entity, $XMLDoc, $domcontainer = NULL) {
  if (empty($entity->rdf_mapping)) {
    trigger_error("No rdf mapping loaded on this entity");
    return FALSE;
  }
  if (empty($XMLDoc)) {
    $XMLDoc = new DOMDocument();
  }
  static $ns;
  if (empty($ns)) {
    $ns = module_invoke_all('rdf_namespaces');
  }
  
  $mapping = $entity->rdf_mapping;
  /*
   * rdf_mapping = array(
   *   rdftype => array( 0 => 'skos:Collection'),
   *   name    => array(
   *     predicates      => array(0 =>'rdfs: label'),
   *   )
   * )
   */

  $rdftype = reset($mapping['rdftype']); // is a soft CURIE, eg 'skos:Collection'
  // Need to know the real URI before adding it to a strict DOM
  $ns_id = fully_qualified_from_curie($rdftype);
  $entity_node = $XMLDoc->createElementNS($ns_id['ns'], $rdftype);
  
  if (! $entity_node) {
    trigger_error("Failed to create an XML node '$rdftype' on the XML document");
    return FALSE;
  }
  if (! empty($domcontainer)) {
    $domcontainer->appendchild($entity_node);
  }
  
  foreach ($mapping as $property_name => $mapping_details) {
    if (empty($mapping_details['predicates'])) continue;
    // The value of this predicate will be the corresponding property on this object
    if (! isset($entity->$property_name)) {
      watchdog('taxonomy_xml', 'Needs work. The rdf mapping wanted to know the value of the property %property_name but the entity did not hve a corresponding value set. '. __FUNCTION__, array('%property_name' => $property_name), WATCHDOG_DEBUG);
      continue;
    }

    // Skip some elements for now. 
    // Some mapping needs special treatment!
    // Elements with 'type' rel or rev actually link to resources.
    // I THINK the rdf_mapping 'callback' may help here, but I've not seen it used correctly yet
    // eg
    //  'skos:member',
    //  'skos:broader',
    //  'skos:narrower',
    if (! empty($mapping_details['type']) && ($mapping_details['type'] == 'rel' || $mapping_details['type'] == 'rev')) continue;

    // That should have filtered out non-string or numeric values (arrays)
    // so we know the value is now useful
    $property_value = trim($entity->$property_name);


    foreach ($mapping_details['predicates'] as $predicate) {
      $ns_id = fully_qualified_from_curie($predicate);
      if (empty($property_value)) {
        continue;
      }
      if (! is_array($property_value)) {
        // assume it's a list. tighter code.
        $property_value = array($property_value);
      }
      foreach ($property_value as $property_val) {
        $property_node = $entity_node->appendChild( $XMLDoc->createElementNS($ns_id['ns'], $predicate));
        $property_node->appendChild($XMLDoc->createTextNode($property_val));
      }
    }
  }
  return $entity_node;
}

/**
 * Set up an RDF document preamble.
 * Returns a document, also sets the passed handle to the RDF node that content
 * should land in
 *
 */
function taxonomy_xml_rdf_document() {
  $dom = new domdocument('1.0', 'UTF-8');
  $ns = module_invoke_all('rdf_namespaces');

  $dom->appendchild($dom->createcomment(xmlentities("
    This file was created by Drupal taxonomy_xml import/export tool.
    http://drupal.org/project/taxonomy_xml
    ")
  ));
  $dom->appendchild($dom->createprocessinginstruction('xml-stylesheet', 'href="render-taxonomy-rdf.xsl" type="text/xsl"' ));

  $domcontainer = $dom->createelementns($ns['rdf'], 'rdf:RDF');
  $dom->appendchild($domcontainer);

  // Why can't I set more namespaces nicely?
  // By appending a namespaced att, the extra namespaces appear at the top.
  // Otherwise the appear everywhere. There must be a better way
  #  $domcontainer->setattributens($ns['rdfs'], 'rdfs:title', "Initializing namespace in PHP is hard" );
  #  $domcontainer->setattributens($ns['owl'], 'owl:hack', "Initializing namespace in PHP is hard" );
  #  $domcontainer->setattributens($ns['skos'], 'skos:hack', "Initializing namespace in PHP is hard" );

  // Magic, here's how! http://bytes.com/topic/net/answers/468470-how-declare-namespace-prefix-java#post1800600
  $domcontainer->setAttributeNS("http://www.w3.org/2000/xmlns/", "xmlns:rdfs", $ns['rdfs']);
  $domcontainer->setAttributeNS("http://www.w3.org/2000/xmlns/", "xmlns:owl", $ns['owl']);
  $domcontainer->setAttributeNS("http://www.w3.org/2000/xmlns/", "xmlns:skos", $ns['skos']);

  // One way to get namespaces to work right MAY BE by adding new
  // elements to their context/container asap, not by waiting until after further bits are added.
  // Building node structures first then addin gthem to the dom produces all the extra namespaces
  // But even that will not propogate namespaces up into containers where they are more useful
  return $domcontainer;
}

/**
 * Resolve shortnames back into namespaces
 */
function fully_qualified_from_curie($curie) {
  if (empty($ns)) {
    $ns = module_invoke_all('rdf_namespaces');
  }
  list($prefix, $id) = split(':', $curie);
  return array(
    'p' => $prefix,
    'id' => $id,
    'ns' => $ns[$prefix],
  );
}


/**
 * Return the path of the arc2 library
 * 
 * Semi-intelligent lookup of places I expect it to be
 */
function rdf_arc2_library_path() {
  $arc2_library_path = variable_get('arc2_library_path', '');
  if (is_readable($arc2_library_path)) {
    return $arc2_library_path;
  }
  // Try other spots?
  $possibilities = array(
    'sites/all/libraries/arc',
    dirname(__FILE__) . '/arc',
  );
  foreach ($possibilities as $arc2_library_path) {
    if (is_readable($arc2_library_path)) {
      variable_set('arc2_library_path', $arc2_library_path);
      return $arc2_library_path;
    }
  }
  return NULL;
}

/**
 * Loads the arc2 library from likely location
 */
function rdf_load_arc2() {
  if ( $arc2_library_path = rdf_arc2_library_path() ) {
    require_once $arc2_library_path . "/ARC2.php";
    return TRUE;
  }
  else {
    $message = 'ARC2 RDF Parser is unavailable. see !install';
    $strings = array('!install' => l('INSTALL.txt', drupal_get_path('module', 'taxonomy_xml') . '/INSTALL.txt') );
    drupal_set_message(t($message, $strings), 'error');
    watchdog('taxonomy_xml', $message, $strings, WATCHDOG_ERROR);
    return FALSE;
  }
}


/**
 * Check ARC RDF library is available
 *
 * (should look into starting to use chaostools to manage plugins?)
 *
 * For now, if it returns anything but an empty string, that means there's
 * a fail.
 */
function taxonomy_xml_rdf_requirements() {
  $requirements = array();
  $arc2_library_path = rdf_arc2_library_path();
  
  if (! is_readable($arc2_library_path . "/ARC2.php")) {
    $requirements['taxonomy_xml_rdf'] = array(
      'value' => t('ARC1 RDF Parser is unavailable.'),
      'severity' => 1, // REQUIREMENT_WARNING,
      'description' => t('
        See <a href="!install">INSTALL.txt</a>
        for the extra features that the external
        <a href="!arc">ARC library</a> can add
        if you download it to %path.
      ', array(
          '!arc' => 'http://arc.semsol.org/',
          '!install' => url( drupal_get_path('module', 'taxonomy_xml') . '/INSTALL.txt'),
          '%path' => $arc2_library_path,
        )
      ),
    );
  }
  return $requirements;
}


