<?php
/* double-commented to avoid conflict with svn
 */

/**
 * @file
 *   Include routines for RDFS parsing and taxonomy/term creation.
 * 
 * THIS IS OLDER CODE, for backwards support and choice.
 * rdf_format has been updated to use the more configurable D7 'bundles' and
 * 'rdf_mapping'
 * 
 * RDF here is based on the W3C examples (using RDFS), an alternative to the
 * Drupal7 preferred SKOS version.
 * 
 * This format provides an export only, The generic RDF importer can take either
 * flavour of RDF
 * 
 * @author dman http://coders.co.nz
 *
 *
 */

module_load_include('inc', 'taxonomy_xml', 'rdf_utils');


// Constants for rules when recursing
// When dumping a term, don't list child terms
define('TAXONOMY_XML_NO_CHILDREN', 0);
// When dumping a term, Just list child term URIs
define('TAXONOMY_XML_CHILDREN_REF_ONLY', 1);
// When dumping a term, Fully describe immediate child terms (URI ref under them)
define('TAXONOMY_XML_CHILDREN_DETAILS', 3);
// When dumping a term, Fully describe all child terms
define('TAXONOMY_XML_CHILDREN_RECURSIVE', 4);



/**
 * sub-hook 
 * @see taxonomy_xml_HOOK_format_info()
 * 
 * Returns info about this syntax
 */
function taxonomy_xml_rdfs_format_info() {
  return array(
    'description' => "RDFS is based on the examples from W3C, predating SKOS support. It models terms as classes and subclasses within an OWL ontology.",
  );  
}


/**
 * Return an XML/RDF document representing this vocab
 * 
 * Uses PHP DOM to create DOM document and nodes.
 *
 * We use namespaces carefully here, although it may create wordy output if the
 * DOM is not optimizing the declarations for us. Still, best to be explicit, it
 * would seem.
 *
 * The URI used to refer to other resources is based on the source document
 * location, eg
 * http://this.server/taxonomy/term/{tid}
 *
 * Preamble should look something like:
 *
 * <rdf:RDF xmlns:rdf ="http://www.w3.org/1999/02/22-rdf-syntax-ns#"
 *   xmlns: rdfs="http://www.w3.org/2000/01/rdf-schema#"
 *   xmlns: owl="http://www.w3.org/2002/07/owl#"
 *
 */
function taxonomy_xml_rdfs_create($vid, $parent = 0, $depth = -1, $max_depth = NULL) {
  $vocabulary = taxonomy_vocabulary_load($vid);

  $domcontainer = taxonomy_xml_rdf_document();
  $dom = $domcontainer->ownerDocument;
  #dpm(array(domcontainer => $domcontainer, dom => $dom));
  
  // Use our OWN rdf mapping!
  

  // define the vocab
  taxonomy_xml_rdfs_add_vocab($domcontainer, $vocabulary);

  // and more details?

  // Now start adding terms.
  // They are listed as siblings, not children of the ontology
  $tree = taxonomy_get_tree($vocabulary->vid, $parent, $max_depth, $depth);
  taxonomy_xml_rdfs_add_terms($domcontainer, $tree);

  $result = $dom->savexml();

  // Minor layout tweak for readability
  $result = preg_replace('|(<[^<]*/[^>]*>)|', "$1\n", $result);
  $result = preg_replace('|><|', ">\n<", $result);
  # dpm($result);
  return $result;
}


/**
 * Create a vocabulary definition (just the def, not its terms) and insert it
 * into the given document element.
 *
 * @param $domcontainer an XML dom document, modified by ref.
 * @param $vocabulary a vocab object
 */
function taxonomy_xml_rdfs_add_vocab(&$domcontainer, $vocabulary) {
  $dom = $domcontainer->ownerDocument;

dpm($vocabulary);

  // Describe the vocabulary itself
  $vocabnode = $dom->createelementns(TAXONOMY_XML_OWL_NS, 'owl:Ontology');
  $domcontainer->appendchild($vocabnode);

  // If this was a cannonic vocab, we would use a full URI as identifiers
  $vocabnode->setattributens(TAXONOMY_XML_RDF_NS, 'rdf:ID', 'vocabulary-' . $vocabulary->vid );
  $vocabnode->setattributens(TAXONOMY_XML_RDF_NS, 'rdf:about', url('taxonomy_xml/' . $vocabulary->vid . '/rdf', array('absolute' => TRUE) ) );

  $vocabnode->appendchild(
    $dom->createelementns(TAXONOMY_XML_RDFS_NS, 'rdfs:label', xmlentities($vocabulary->name))
  );

  if ($vocabulary->description) {
    $vocabnode->appendchild(
      $dom->createelementns(TAXONOMY_XML_RDFS_NS, 'rdfs:comment', xmlentities($vocabulary->description))
    );
  }

  $vocabnode->appendchild(
    $dom->createelementns(TAXONOMY_XML_OWL_NS, 'owl:versionInfo', xmlentities(format_date(REQUEST_TIME, 'long')))
  );
}

/**
 * Given a list of terms, append definitions of them to the passed DOM container
 *
 * Following w3c, SUMO and Wordnet examples (tho not any explicit instructions,
 * taxonomy terms are modelled as rdfs:Class objects structured using rdfs:
 * subClassOf statements.
 *
 * Sample from Wordnet:
 *
 * <Class rdf:about="http://xmlns.com/wordnet/1.6/Cat">
 *   <label>Cat  [ 1 ]</label>
 *   <comment>feline mammal usually having thick soft fur and being unable
 * to roar; domestic cats; wildcats</comment>
 *   <subClassOf>
 *     <Class rdf:about="http://xmlns.com/wordnet/1.6/Feline" />
 *   </subClassOf>
 * </Class>
 *
 * I'm copying that syntax.
 *
 * @param $termlist a FLAT array of all terms, internally cross-referenced to
 * each other defining the tree stucture
 */
function taxonomy_xml_rdfs_add_terms(&$domcontainer, $termlist, $recursion_behaviour = TAXONOMY_XML_CHILDREN_REF_ONLY) {
  if (! $termlist) {
    return;
  }
  $dom = $domcontainer->ownerDocument;

  // Allow submission of a single term
  if (! is_array($termlist)) {
    $termlist = array($termlist);
  }

  // D7 hook_taxonomy_term_load actually takes an array, not a singular
  module_invoke_all('taxonomy_term_load', $termlist);

  foreach ($termlist as $term) {
    #dpm($term);
    // TODO - rewrite this hunk to use the new rdf entity rdf mappings!
    // eg with
    // $termnode = rdf_entity_to_xml($term, $dom);
    
    
    $termnode = $dom->createelementns(TAXONOMY_XML_RDFS_NS, 'rdfs:Class');
    $termnode->setattributens(TAXONOMY_XML_RDF_NS, 'rdf:ID', 'term-' . $term->tid );
    $domcontainer->appendchild($termnode);

    // Set either the local or (preferably) the cannonic remote URI as the elements
    // 'about' attribute.

    if ($uri = taxonomy_xml_get_term_uri($term)) {
      $termnode->setattributens(TAXONOMY_XML_RDF_NS, 'rdf:about', $uri );
    }
    else {
      $term_uri = taxonomy_term_uri($term);
      $path = url($term_uri['path'], array('absolute' => TRUE));
      // Why does that return an array now?
      $termnode->setAttributeNS(TAXONOMY_XML_RDF_NS, 'rdf:about', $path);
    }

    $termnode->appendchild(
      $dom->createelementns(TAXONOMY_XML_RDFS_NS, 'rdfs:label', xmlentities($term->name))
    );

    if ($term->description) {
      $termnode->appendchild(
        $dom->createelementns(TAXONOMY_XML_RDFS_NS, 'rdfs:comment', xmlentities($term->description) )
      );
    }

    $vocab_ref = $dom->createelementns(TAXONOMY_XML_RDFS_NS, 'rdfs:isDefinedBy');
    $vocab_ref->setattributens(TAXONOMY_XML_RDF_NS, 'rdf:resource', '#vocabulary-' . $term->vid );
    $termnode->appendchild($vocab_ref);

    /*
     * These went away in D7
    foreach ((array) taxonomy_get_related($term->tid) as $relatedid => $relatedterm) {
      $related_node = $dom->createelementns(TAXONOMY_XML_RDFS_NS, 'rdfs:seeAlso', xmlentities($relatedterm->name) );
      $related_node->setattributens(TAXONOMY_XML_RDF_NS, 'rdf:resource', '#term-' . $relatedid );
      $termnode->appendchild($related_node);
    }

    $synonyms = taxonomy_get_synonyms($term->tid);
    // TODO - figure out the right syntax for synonym
    // I'm using 'equivalentClass' ... although that's really intended for merging different vocabs.
    foreach ((array) $synonyms as $synonymname) {
      $synonymnode = $parent_node = $dom->createelementns(TAXONOMY_XML_OWL_NS, 'owl:equivalentClass', xmlentities($synonymname) );
      $termnode->appendchild($synonymnode);
    }
    */

    if (!empty($term->parents)) {
      foreach ((array) $term->parents as $parentid) {
        $parentlist = array();
        if ($parentid) {
          $parentlist[$parentid] = $parent =  taxonomy_term_load($parentid);
          $parent_node = $dom->createelementns(TAXONOMY_XML_RDFS_NS, 'rdfs:subClassOf', xmlentities($parent->name));
          $parent_node->setattributens(TAXONOMY_XML_RDF_NS, 'rdf:resource', '#term-' . $parentid );
          $termnode->appendchild($parent_node);
        }
      }
    }
    # dpm(array('adding term to rdf' => $term));
    #$termnode->appendchild($dom->createcomment(print_r($term, 1)));

    // Now add the children also
    switch ($recursion_behaviour) {
      case TAXONOMY_XML_NO_CHILDREN :
      break;
      case TAXONOMY_XML_CHILDREN_REF_ONLY :
        $max_depth = 1;
        $tree = taxonomy_get_tree($term->vid, $term->tid, $max_depth);
        foreach ($tree as $child) {
          $child_id = $child->tid;
          $child_node = $dom->createelementns(TAXONOMY_XML_RDFS_NS, 'rdfs:superClassOf', xmlentities($child->name));
          $child_node->setattributens(TAXONOMY_XML_RDF_NS, 'rdf:resource', '#term-' . $child_id );
          $termnode->appendchild($child_node);
        }
        
    }
      

    // workaround for large vocabs - extend runtime indefinately
    drupal_set_time_limit(10);
  }
  // Done all terms in list
}



/**
 * Return a single term as RDF. Header and all
 */
function taxonomy_xml_rdfs_export_term($term, $depth = -1, $max_depth = NULL) {
  if (is_numeric($term)) {
    $term = taxonomy_term_load($term);
  }

  // Load in all extra data ? All taken core of in D7?

  $domcontainer = taxonomy_xml_rdf_document();
  $dom = $domcontainer->ownerDocument;

  taxonomy_xml_add_terms_as_rdf($domcontainer, $term);

  // Now start adding child terms.
  // Should recurse according to rules set elsewhere (taxonomy_server)
  // If taxonomy_server is not present, then use default recursion rule.
  // They are listed as siblings, not children of the ontology
  $tree = module_invoke('taxonomy', 'get_tree', $term->vid, $term->tid, $max_depth, $depth);

  taxonomy_xml_add_terms_as_rdf($domcontainer, $tree);
  $result = $dom->savexml();

  // Minor layout tweak for readability
  $result = preg_replace('|(<[^<]*/[^>]*>)|', "$1\n", $result);
  $result = preg_replace('|><|', ">\n<", $result);
  # dpm($result);
  print $result;
  exit;
}
