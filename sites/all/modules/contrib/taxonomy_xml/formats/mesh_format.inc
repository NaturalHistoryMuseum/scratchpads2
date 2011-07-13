<?php
/* double-commented to avoid conflict with svn
 */

/**
 * @file
 *   Include routines for the Medical Subject Headings Schema [MeSH] as
 * used by http://www.nlm.nih.gov/mesh/
 *
 * @author dman http://coders.co.nz
 *
 * TODO: lots, I imagine.
 * A write-version
 * A better way to handle relationships, as MeSH does not have any references,
 * just constructs its tree heirachy via a naming scheme.
 */

define('TAXONOMY_XML_MESH_NS', 'http://www.nlm.nih.gov/mesh/' );

/**
 * Reads a XML file and creates the term definitions found in it.
 *
 * Implimentation of the taxonomy_xml_HOOK_parse() callback.
 *
 *
 * @param $data XML string representing the MeSH file to be parsed
 * @param $vid Vocabulary ID the terms are to be created under. Passed by ref as
 * it may get set when using 'defined by source'
 * @param $url The source URL of the TCS doc. Used to create URIs from document
 * IDs
 *
 * @return An array of the terms created in this parsing process.
 *
 */
function taxonomy_xml_mesh_parse(&$data, &$vid = 0, $url = '') {
  #drupal_set_message(t("Importing from provided XML data file %url.", array('%url' => $url)));

  if ($vid == 0) {
    // We've been asked to use the vocab described in the source file.
    #drupal_set_message(t("No vocabulary specified in the form, using a default 'MeSH' one."));
    // Create a placeholder, use that
    $vocabulary = _taxonomy_xml_get_vocabulary_placeholder('MeSH');
    $vid = $vocabulary->vid;
  }
  else {
    // Else using a form-selected vocob.
    $vocabulary = taxonomy_vocabulary_load($vid);
  }

  if (! $vocabulary) {
    drupal_set_message("Problem retrieving vocabulary $vid to use. This is fatal", 'error');
    return;
  }

  $xmldoc = new domdocument;

  // Use the DOM, not the parser, it's quicker (to code)
  if (! $xmldoc->loadxml($data) ) {
    trigger_error("Failed to parse in xml source. [$xmlfile]", E_USER_WARNING);
    return;
  }

  // Scan for 'DescriptorRecord' which are our prime elements
  $xp = new DomXPath($xmldoc);

  // NEED a namespace when the default namespace is declared.
  $fakenamespace = FALSE; // set as an option during development - may be unwanted

  if ($fakenamespace) {
    $prefix = "mesh:";
    $xp->registerNameSpace('mesh', TAXONOMY_XML_MESH_NS);
  }
  else {
    $prefix = '';
  }

  $query = "//${prefix}DescriptorRecord";
  $concepts = $xp->query($query);

  if (! $concepts->length) {
    drupal_set_message('No DescriptorRecords found in this doc. Namespace problems? Wrong format?', 'error');
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
      'vid' => $vid,
    );

    // Find the id of this descriptor and other stuff from this node
    foreach ($concept->childNodes as $child) {
      if ($child->nodeName == 'DescriptorUI') {
        $term->id = trim($child->nodeValue);
      }
      if ($child->nodeName == 'DescriptorName') {
        $term->name = trim($child->nodeValue);
      }
      if ($child->nodeName == 'TreeNumberList') {
        $term->TreeNumberList = array();
        foreach ($child->childNodes as $treenumber) {
          if ($treenumber->nodeName == 'TreeNumber') {
            $term->TreeNumberList[] = trim($treenumber->nodeValue);
          }
        }
      }
    }

    if (! $term->id) {
      $term->id = $concept->getAttribute('id');
    }

    $term->guid = $url;

    // Try to find a desciption. Use the ScopeNote of the Preferred concept. Seems to be the most useful
    $notes = $xp->query("${prefix}ConceptList/${prefix}Concept[@PreferredConceptYN='Y']/${prefix}ScopeNote", $concept);
    if (! empty($notes)) {
      foreach ($notes as $note) {
        $term->description = trim($note->nodeValue);
      }
    }

    $synonyms = $xp->query("//${prefix}TermList/${prefix}Term/${prefix}String", $concept);
    foreach ($synonyms as $synonym) {
      $term->synonym_array = trim($synonym->nodeValue);
      $term->predicates[TAXONOMY_XML_HAS_SYNONYM][] = trim($synonym->nodeValue);
    }
    $term->relationships = array();

    // Parents and children are NOT given in normal MeSH syntax.
    // They should be :-(. I run on a cooked version that has added rdf relationships where needed.
    // Find parents. Store them in an array for later linking
    $parents = $xp->query("rdfs:subClassOf", $concept);
    foreach ($parents as $rel) {
      $reltype = TAXONOMY_XML_PARENT;
      // sorry hazy code here. Can't recall if we index on url or name (sometimes both)
      $reftarget = ($rel->getAttribute('rdf:resource')) ? $rel->getAttribute('rdf:resource') : $rel->textContent;
      $refname = $rel->textContent ? $rel->textContent : $reftarget;

      //
      // Large problem
      // When importing subtrees
      // - that contain children with multiple parents
      // - where one of the parents is OUTSIDE the current subtree
      // We do NOT want to instantiate it, as it creates broken twigs
      //
      // The only way to guess which is the true parent and which is the step-parent
      // is to see which one already exists, as it (should) have ben created
      // at an earlir time.
      // For this we need to do a database lookup (which we would do below anyway)
      //
      // See if we know it
      if (isset($terms[$reftarget]) ) {
        $term->predicates[$reltype][$reftarget] = $reftarget;
        // Fine, it must be kosher. no problem.
      }
      else {
        // or can find it
        $target_term = _taxonomy_xml_get_term_placeholder($refname, $vid);
        if (empty($target_term->tid)) {
          // It does not exist. Therefore don't even make a placeholder, just discard it
          // Later runs WILL establish the connection when both items exist, so this hole can be safely patched.
          #drupal_set_message("Discarding deadbeat dad - '$refname' is never around, so disowning it as a parent of {$term->name}");
        }
        else {
          $term->predicates[$reltype][$reftarget] = $reftarget;
          // Add it to the current array, seeing as we have it now.
          $terms[$reftarget] = &$target_term;
# don't need rough name lookups in MeSH
#          $terms[$refname] = &$target_term;
        }
        unset($target_term); // careful when using handles!
      }

    }
    // Find children. Store them in an array for later linking
    $children = $xp->query("wn:hyponym", $concept);
    foreach ($children as $rel) {
      $reltype = TAXONOMY_XML_CHILD;
      $reftarget = ($rel->getAttribute('rdf:resource')) ? $rel->getAttribute('rdf:resource') : $rel->textContent;
      $term->predicates[$reltype][$reftarget] = $reftarget;
      // Note the targets we will need to ensue we know about later
      $refname = $rel->textContent ? $rel->textContent : $reftarget;
      $term->relationships[$reftarget] = $refname;
    }
    // Let other hooks do their own logic with the data on save.
    $term->xml = $xmldoc->saveXML($concept);

    #dpm(array("found a taxonConcept ". $term->name ." " . $term->id, $term));

    // Add this term to our list, indexed as best we can.

    // If we were loading a remote file, and the file contains only one Concept, then the file URI represents the concept.
    // This is not strictly precise enough - it should be the #ID inside the doc, but this is the way the current web services work.
    // Index this thing by unique URI by choice, URI may include the #id as an anchor also
    if ($url && (count($concepts) == 1)) {
      $terms[$url] = &$terms[$term->id];
    }
    else {
      $terms[$term->id] = $term;
    }

    // Fill in gaps in the terms array
    // Ensure we can find the subject terms of any predicates we just found;
    // relationships is a temp array containing refs to both parents and children - just for looping
    foreach ((array) $term->relationships as $reftarget => $refname) {
      if (! isset($terms[$reftarget]) ) {
        $target_term = _taxonomy_xml_get_term_placeholder($refname, $vid);
        #if (! $target_term->tid) {
        #  drupal_set_message("We will have to make a new placeholder term called $refname to support its mention from within {$term->name}");
        #}
        $terms[$reftarget] = &$target_term;
# don't need this
#        $terms[$refname] = &$target_term;
        unset($target_term); // careful when using handles!
      }
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
    if (!empty($term->taxonomy_xml_presaved)) {
      continue;
    }

    if (!isset($term->guid)) {
      $term->guid = $identifier;
    }
    if (!isset($term->vid)) {
      $term->vid = $vid;
    }

    // Translate the predicate statements into the syntax we need
    taxonomy_xml_canonicize_predicates($term);

    // Data is now massaged and referring to itself correctly,
    // Start creating terms so we can retrieve term ids

    // Ensure name is valid
    if (empty($term->name)) {
      $term->name = taxonomy_xml_shortname($identifier);
      drupal_set_message(t("Problem, we were unable to find a specific label for the term referred to as %guid. Guessing that %name will be good enough.", array('%guid' => $term->guid, '%name' => $term->name)));
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
    #dpm(array("Assembled term data, almost ready to save ". ($term->tid? "over existing term":"newly made-up term" ) => $term));

    $status = taxonomy_term_save($term);

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
