<?php
/* double-commented to avoid conflict with svn
 */

/**
 * @file
 *   Include routines for the Taxon Concepts Schema as used by "the
 * Encyclopedia of Life" original XML parsing and taxonomy/term creation. and
 * others.
 */

/**
 * sub-hook 
 * @see taxonomy_xml_HOOK_format_info()
 * 
 * Returns info about this syntax
 */
function taxonomy_xml_tcs_format_info() {
  return array(
    'description' => "The Taxon Concept Schema is used in Life Sciences to notate biological families of living things.",
  );  
}


/**
 * Reads a TCS file and creates the term definitions found in it.
 *
 * Implimentation of the taxonomy_xml_HOOK_parse() callback.
 *
 * This passes through several times, first finding all TaxonConcepts mentioned
 * in the Doc, then finding if they refer to each other, then creating or
 * retrieving and merging existing term data, then looping again to ensure they
 * each point at each other. At no point can we assume to have all term items in
 * memory, it just tries to instantiate them incrementally, depending on the
 * information available.
 *
 * @param $data
 *   XML string representing the TCS file to be parsed
 * @param $vid
 *   Vocabulary ID the terms are to be created under
 * @param $url
 *   The source URL of the TCS doc. Used to create URIs from document IDs
 *
 * @return
 *   An  array of the terms created in this parsing process.
 */
function taxonomy_xml_tcs_parse(&$data, $vid = 0, $url = '') {
  #drupal_set_message(t("Importing from provided TCS data file %url.", array('%url' => $url)));

  if ($vid == 0) {
    // We've been asked to use the vocab described in the source file.
    // However TCS files do not define a vocab
    drupal_set_message(t("No vocabulary specified in the form, using a default one."));
    // Create a placeholder, use that
    $vocabulary = _taxonomy_xml_get_vocabulary_placeholder('Taxa');
    $vid = $vocabulary->vid;
  }
  else {
    // Else using a form-selected vocob.
    $vocabulary = taxonomy_vocabulary_load($vid);
  }
  $xmldoc = new domdocument;

  // Use the DOM, not the parser, it's quicker (to code)
  if (! $xmldoc->loadxml($data) ) {
    trigger_error("Failed to parse in xml source. [$xmlfile]", E_USER_WARNING);
    return;
  }

  // Scan for 'taxonconcepts' which are our prime elements
  $xp = new DomXPath($xmldoc);

  // NEED a namespace when the default namespace is declared.
  $fakenamespace = TRUE; // set as an option during development - may be unwanted

  if ($fakenamespace) {
    $prefix = "tcs:";
    $xp->registerNameSpace('tcs', TAXONOMY_XML_TCS_NS);
  }
  else {
    $prefix = '';
  }

  $query = "//${prefix}TaxonConcept";
  $concepts = $xp->query($query);

  if (! $concepts->length) {
    drupal_set_message('No TaxonConcepts found in this doc. Namespace problems? Wrong format?', 'error');
  }

  //
  // BEGIN the first loop, finding terms in this document
  //
  // Remembering all terms is memory-intensive, but may be more efficient in batch jobs.
  // Use a static list where possible. EXPERIMENTAL
  $terms = & taxonomy_xml_current_terms();

  #dpm(array("About to start analyzing a data doc $url, known terms are: " => $terms));

  foreach ($concepts as $concept) {
    // Start constructing a (new?) term
    $term = (object) array(
      'predicates' => array(),
    );

    // Concepts must have IDs (?)
    // But is it a local numeric id or a URI? Depends on what we are given.
    // In any case remember it and compare this value with later references to it.
    $term->id = $concept->getAttribute('id');

    // The URIs being used in the current data service are a bit vague.
    // As a TCS file may contain more than one 'TaxonConcept',
    // we can't really just point at the service URL to identify it.
    // It should really have an ID in the string.
    # $term->URI = $url . '#' . $concept->getAttribute('id');
    $term->uri = $url;

    $termnames = $xp->query("${prefix}Name", $concept);
    foreach ($termnames as $termname) {
      $term->name = $termname->textContent;
      // TCS doesn't provide names so much as references to names. Make a note.
      $term->namerefs[] = $termname->getAttribute('ref');
    }

    // Find ALL its relationships. Store them in an array for later linking
    $relationships = $xp->query("${prefix}TaxonRelationships/${prefix}TaxonRelationship", $concept);

    foreach ($relationships as $rel) {
      $reltype = $rel->getAttribute('type');
      $reltags =  $xp->query("${prefix}ToTaxonConcept", $rel);
      foreach ($reltags as $reltag) {
        // Following the spec, a reference may be by id, by name, or both.
        // Trust the ref-id where possible, otherwise lookup the name
        $reftarget = ($reltag->getAttribute('ref')) ? $reltag->getAttribute('ref') : $reltag->textContent;
        // $term->relationships array is here for other storage mechanisms (term_relations) to investigate and serialize if they can
        $term->relationships[$reltype][$reftarget] = $reftarget;
        $term->predicates[$reltype][$reftarget] = $reftarget;
      }
    }

    // TCS has a number of extra metadata fields which we want, BUT a clean Drupal install
    // does not have any way of serializing it.
    // IF we want to save that data, it must be massaged into saveable fields
    // by another module eg the new experimental eol.module
    // that grabs the data on hook_node_presave() and saves it using taxonomy_enhancer.
    //
    // This import module will attempt to NOT make any judgements at this
    // stage about how that data shall be stored.
    //
    // It could be possible to pass a handle on the full XML node,
    // but that proved way too intensive on memory.
    // Instead let the term know the xml that it came from. Other parsers can take it from there.
    //
    // Let other hooks do their own logic with the data on save.

    $term->xml = $xmldoc->saveXML($concept);
    #dpm(simplexml_import_dom($concept));

    // TCS stores its name information in another node. This concept needs to
    // know it later. Without getting into too much logic here, we'll see if
    // we can find the data and append it here.
    // Again, no analysis is done, just the xml is attached to the $term object
    foreach ($term->namerefs as $nameref) {
      // In theory, namerefs may point to TaxonNames in other documents
      // We are not expected to support that yet.
      $taxon_names = $xp->query("//${prefix}TaxonName[@id='$nameref']");
      foreach ($taxon_names as $taxon_name) {
        $term->taxon_names[$nameref] = $xmldoc->saveXML($taxon_name);
      }
    }

    #dpm(array("found a taxonConcept ". $term->name ." " . $term->id, $term));
    // Add this term to our list, indexed as best we can.
    $terms[$term->id] = $term;

    // If we were loading a remote file, and the file contains only one Concept, then the file URI represents the concept.
    // This is not strictly precise enough - it should be the #ID inside the doc, but this is the way the current web services work.
    // Index this thing as a duplicate handle so we can find it easily if asked for the URL
    if ($url && (count($concepts) == 1)) {
      $terms[$url] = &$terms[$term->id];
    }
  }

  #dpm(array("initialized placeholder terms from the input, now to flesh them out and structure them" => $terms));

  // The first placeholder term definitions are set up.
  // They may want to refer to each other, So now scan the refs for known referees
  // and actually create them so we have tids to link.
  //
  // $terms list may also include pre-existing terms, included for cross-reference and linking

  foreach ($terms as $identifier => &$term) {
    // Skip duplicates (some dupes may exist due to the use of handles)
    if ($term->taxonomy_xml_presaved) {
      continue;
    }

    if (!isset($term->uri)) {
      $term->uri = $identifier;
    }

    // Translate the predicate statements into the syntax we need
    taxonomy_xml_canonicize_predicates($term);

    // Data is now massaged and referring to itself correctly,
    // Start creating terms so we can retrieve term ids

    // Ensure name is valid
    if (! $term->name) {
      drupal_set_message(t("Problem, we were unable to find a specific label for the term referred to as %uri. Guessing that %name will be good enough.", array('%uri' => $term->URI, '%name' => $term->name)));
    }

    #dpm(array("Looking for an existing definition or making a placeholder for " => $term));

    // See if a definition already exists in the DB. Build on that. Otherwise start setting up a new prototype $term object.
    // This does a get by name. If we had a better GUID to lookup, should try that instead
    $existing_term = _taxonomy_xml_get_term_placeholder($term->name, $vid);
    // Merge the old term objects properties into this one. Really just want its tid, but there may be more info I should not lose.
    // Our new input takes precedence over older data
    foreach ((array) $existing_term as $key => $value) {
      if (! isset($term->$key)) {
        $term->$key = $value;
      }
    }
    //
    // The term object is now as tidy as it can be as a self-contained entity.
    //

    #dpm(array("Assembled term data and other, now saving" => $term));

    $save_term = (array) $term;
    $status = taxonomy_term_save($term /* TODO Term object replaces array $save_term */);

    // Re-retrieve the new term definition, just in case anything extra happened to it during processing
    $new_term = taxonomy_xml_get_term_by_name_from_vocab($term->name, $term->vid);
    if (! $new_term) {
      drupal_set_message(t("It seems like we failed to create and retrieve a term called %term_name", array('%term_name' => $term->name)), 'error');
    }
    // Merge retrieved values back over our main definition so the handles are up-to-date
    //
    // There seems to be a lot of data copying back and forth,
    // even though we are using handles, but this is because we are
    // temporarily retaining more information in the $term object than
    // the system knows how to handle.
    foreach ((array) $new_term as $key => $value) {
      $term->$key = $value;
    }

    if ( $status == SAVED_NEW ) {
      // Just remember this is fresh - for useful feedback messages.
      $term->taxonomy_xml_new_term = TRUE;
    }

    // It's possible that not all the referenced items were available in the current document/loop
    // Add referred items to the import queue for later processing
    taxonomy_xml_add_all_children_to_queue($term);
    $term->taxonomy_xml_presaved = TRUE; // A flag to avoid double-processing
  } // end term-construction loop;

  #dpm(array('created a bunch of terms, now they need relations set.' => $terms));

  taxonomy_xml_set_term_relations($terms);

  #dpm(array('After re-linking, we now have all terms set' => $terms));

  return $terms;
}


////////////////////////////////////////////////////////////////////////
// TCS Writer


define('TAXONOMY_XML_TCS_NS', 'http://www.tdwg.org/schemas/tcs/1.01');
define('TAXONOMY_XML_XSI_NS', 'http://www.w3.org/2001/XMLSchema-instance');
define('TAXONOMY_XML_TCS_SCHEMA', 'http://www.tdwg.org/schemas/tcs/1.01 http://tdwg.napier.ac.uk/TCS_1.01/v101.xsd');


/**
 * Return an XML/TCS document representing this vocab
 *
 * Uses PHP DOM to create DOM document and nodes.
 *
 * We use namespaces carefully here, although it may create wordy output if the
 * DOM is not optimizing the declarations for us. Still, best to be explicit, it
 * would seem.
 *
 * The URI used to refer to other resources is based on the source document
 * location, eg
 * http://this.server/taxonomy_xml/{vid}/rdf#{tid}
 *
 * Preamble should look something like:
 *
 * <?xml version="1.0" encoding="UTF-8"?>
 * <DataSet
 *  xmlns: xsi="http://www.w3.org/2001/XMLSchema-instance"
 *  xsi: schemaLocation="http://www.tdwg.org/schemas/tcs/1.01
 *  http: //tdwg.napier.ac.uk/TCS_1.01/v101.xsd"
 *  xmlns=" http://www.tdwg.org/schemas/tcs/1.01">
 *
 *
 */
function taxonomy_xml_tcs_create($vid, $parent = 0, $depth = -1, $max_depth = NULL) {
  $vocabulary = taxonomy_vocabulary_load($vid);

  $domcontainer = taxonomy_xml_tcs_document();
  $dom = $domcontainer->ownerDocument;

  // and more details?

  // Now start adding terms.
  // They are listed as siblings, not children of the ontology
  $tree = taxonomy_get_tree($vid, $parent, $max_depth, $depth);
  taxonomy_xml_add_terms_as_tcs($domcontainer, $tree);

  $result = $dom->savexml();

  // Minor layout tweak for readability
  $result = preg_replace('|(<[^<]*/[^>]*>)|', "$1\n", $result);
  $result = preg_replace('|><|', ">\n<", $result);
  #dpm($result);
  return $result;
}

/**
 * Set up a TCS document preamble.
 *
 * Returns a document, also sets the passed
 * handle to the RDF node that content should land in
 *
 * This format based on inspection of the TCS schema (TCS101) and sample
 * documents found at http://www.tdwg.org/standards/117/
 */
function taxonomy_xml_tcs_document() {
  $dom = new domdocument('1.0', 'UTF-8');

  $dom->appendchild($dom->createcomment(htmlentities('
    This file was created by Drupal taxonomy_xml import/export tool.
    http://drupal.org/project/taxonomy_xml
    This format is based on inspection of the TCS schema (TCS101) and sample
    documents found at http://www.tdwg.org/standards/117/
    ')
  ));
  $dom->appendchild($dom->createprocessinginstruction('xml-stylesheet', 'href="render-taxonomy-tcs.xsl" type="text/xsl"' ));
  $domcontainer = $dom->createelementns(TAXONOMY_XML_TCS_NS, 'DataSet');
  $dom->appendchild($domcontainer);
  #$domcontainer->setnamespace(TAXONOMY_XML_TCS_NS, 'tcs');
  $domcontainer->setattribute("xmlns", TAXONOMY_XML_TCS_NS);

  // Why can't I set more namespaces?
  // By appending a namespaced att, the extra namespaces appear at the top.
  // Otherwise the appear everywhere. There must be a better way
  $domcontainer->setattributens(TAXONOMY_XML_XSI_NS, 'xsi:schemaLocation', TAXONOMY_XML_TCS_SCHEMA );

  // Note that one way to get namespaces to work right is by adding new
  // elements to their context asap, not by waiting until after further bits are added.
  return $domcontainer;
}


/**
 * Given a list of terms, append definitions of them to the passed DOM container
 *
 *
 * @param $termlist
 *   a FLAT array of all Drupal terms, internally cross- referenced to each
 * other defining the tree stucture
 */
function taxonomy_xml_add_terms_as_tcs(&$domcontainer, $termlist) {
  if (! $termlist) {
    return;
  }
  $dom = $domcontainer->ownerDocument;

  $termscontainer = $dom->createelementns(TAXONOMY_XML_TCS_NS, 'TaxonConcepts');
  $domcontainer->appendchild($termscontainer);

  foreach ($termlist as $term) {
    $termnode = $dom->createelementns(TAXONOMY_XML_TCS_NS, 'TaxonConcept');
    $termnode->setattributens(TAXONOMY_XML_TCS_NS, 'id', 'term-' . $term->tid );
    $termscontainer->appendchild($termnode);

    $propername = $dom->createelementns(TAXONOMY_XML_TCS_NS, 'Name', $term->name);
    $propername->setattributens(TAXONOMY_XML_TCS_NS, 'scientific', 'true');
    $propername->setattributens(TAXONOMY_XML_TCS_NS, 'ref', '?');
    $termnode->appendchild($propername);

/*
    $synonyms = taxonomy_get_synonyms($term->tid);
    foreach ((array) $synonyms as $synonym) {
      $synonymname = $dom->createelementns(TAXONOMY_XML_TCS_NS, 'Name', xmlentities($synonym) );
      $synonymname->setattributens(TAXONOMY_XML_TCS_NS, 'scientific', 'false');
      $termnode->appendchild($synonymname);
    }
*/
    $termrelations = $dom->createelementns(TAXONOMY_XML_TCS_NS, 'TaxonRelationships');
    $termnode->appendchild($termrelations);

    // Start adding relations
    foreach ((array) $term->parents as $relatedid) {
      $relatedlist = array();
      if ($relatedid) {
        $relatedlist[$relatedid] = $related =  taxonomy_term_load($relatedid);
        $relation_node = $dom->createelementns(TAXONOMY_XML_TCS_NS, 'TaxonRelationship');
        $relation_node->setattributens(TAXONOMY_XML_TCS_NS, 'type', 'is child taxon of' );
        $termrelations->appendchild($relation_node);
        $relation_reference = $dom->createelementns(TAXONOMY_XML_TCS_NS, 'ToTaxonConcept', xmlentities($related->name));
        $relation_reference->setattributens(TAXONOMY_XML_TCS_NS, 'ref', $related->tid );
        $relation_node->appendchild($relation_reference);
      }
    }
    # $termnode->appendchild($dom->createcomment(print_r($term, 1)));

    // workaround for large vocabs - extend runtime indefinately
    drupal_set_time_limit(10);
  }
  // Done all terms in list
}
