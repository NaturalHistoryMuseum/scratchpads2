<?php
/* double-commented to avoid conflict with svn
 */

/**
 * @file
 *   Include routines for RDF parsing and taxonomy/term creation.
 * 
 * RDF here is based on the W3C examples (using RDFS), but also incorporates
 * support for the SKOS Dialect as well.
 * 
 * Note the use of the word 'node' here almost always refers to XML nodes, not
 * Drupal nodes.
 * 
 * @author dman http://coders.co.nz
 *
 * 2009-09 Code to support bnodes (internal references to other nodes within an
 * RDF document) prompted by a patch contribution from by Remzi Celebi
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
function taxonomy_xml_rdf_format_info() {
  return array(
    'description' => "RDF is recommended for portability with external databases, although it is verbose and sometimes unreadable to humans. The RDF used here is based on Drupal 7 'rdf_mapping' used internally by entities and RDFa",
    'mime' => 'application/rdf+xml',
  );
}


/**
 * Return a list of 'types' of things that we may import as 'terms'
 * 
 * The RDF input may come in several flavours,
 * Resources of the following 'types' may be cast into taxonomy terms for our purposes.
 * That is, an rdf:Class is a Drupal:term
 *
 * Add to this list as needed as examples come from the wild
 */
function taxonomy_xml_rdf_term_types() {
  $term_types = array(
    TAXONOMY_XML_SKOS_NS . 'Concept',
    TAXONOMY_XML_RDF_NS . 'Property',
    TAXONOMY_XML_DC_NS . 'subject',
    TAXONOMY_XML_RDFS_NS . 'Class',
    TAXONOMY_XML_OWL_NS . 'Class',
    TAXONOMY_XML_W3C_WN_SCHEMA . 'Word',
    TAXONOMY_XML_W3C_WN_SCHEMA . 'NounWordSense',
    TAXONOMY_XML_W3C_WN_SCHEMA . 'NounSynset',
    TAXONOMY_XML_CONTENTLABEL_NS . 'Category',
    'urn:lsid:ubio.org:classificationbank',
    TAXONOMY_XML_FB_NS . 'common.topic', // A freebase core 'topic'
      // freebase 'topic' is a superclass of useful things like 'music.genre'
  );
  return $term_types;
}

/**
 * Return a list of types of things that may behave like 'vocabularies'
 * 
 * A Drupal 'vocabulary' is represented by an owl:Ontology
 * or other similar shaped constructs, like a SKOS ConceptScheme
 */
function taxonomy_xml_rdf_vocabulary_types() {
  $vocabulary_types = array(
    TAXONOMY_XML_SKOS_NS . 'ConceptScheme',
    TAXONOMY_XML_OWL_NS . 'Ontology', // eg SIOC
    TAXONOMY_XML_RDF_NS . 'Description',
    'http://www.w3.org/2001/12/Glossary',
    TAXONOMY_XML_TDWG_NS . 'Collection',
    # Resources that are of type fb:type_profile are often collections of 'topics'
    # thus, the are analogous to our 'vocabulary'
    TAXONOMY_XML_FB_NS . 'freebase.type_profile',
  );
  return $vocabulary_types;
}

/**
 * Read in RDF taxonomies and vocabularies. Create vocabs and terms as needed.
 *
 * See formats.html readme for information about the RDF input supported.
 *
 * Targets include:
 *   ICRA      Content Rating  http://www.icra.org/vocabulary/
 *   WordNet   Lexicon http: //wordnet.princeton.edu/
 *   SUMO      http://www. ontologyportal.org/
 *   Freebase
 *
 * ... and the ontologies found at http://www.schemaweb.info/ that implement
 * appropriate parts of the RDF Schema "rdfs" (eg Classes with subclassOf)
 *
 * This function takes care of the parsing of RDF syntax into attributes
 * (predicates). Actual term creation and logic is done by taxonomy_xml.module,
 * mostly in taxonomy_xml_rdf_make_term() taxonomy_xml_canonisize_predicates().
 *
 * @param $data the string containing XML/RDF
 * @param $vid int Vocab ID. May be modified by ref if this process creates a
 * new vocab to use.
 * @param $url optional source URL this RDF came from if needed to resolve GUIDs
 * etc. Cannot work for uploads.
 *
 * @return a list of resulting terms. FALSE on failure.
 */
function taxonomy_xml_rdf_parse(&$data, &$vid, $url = NULL) {
  // See if it's really a different file we need to parse
  @list($resource_url, $anchor) = split('#', $url);

  $index = taxonomy_xml_rdf_parse_data_into_index($data, $url);
#dpm($index);
  // If a specific ID was defined in the file, this means we just need to load
  // that one. This will help break things up for batches, and also allow us to
  // grab only sub-trees from big files.
  if (!empty($anchor)) {
    watchdog('taxonomy_xml', "
      We were only asked about #%anchor in this document.
      Reducing the data down to statements about that.",
      array('%anchor' => $anchor),
      WATCHDOG_DEBUG
    );
    $index = array($url => $index[$url]);
    if (empty($index)) {
      watchdog('taxonomy_xml',
        "Found no information about  %anchor in the document !resource_url",
        array('%anchor' => $anchor, '!resource_url' => l($resource_url, $resource_url)),
        WATCHDOG_WARNING
      );
      return NULL;
    }
  }

  $resources_by_type = taxonomy_xml_convert_index_to_sorted_objects($index);
  #dpm($index);

  // The resources are all initialized as data objects.
  // The predicates have NOT been flattened yet.
  // Resource types we expect to be dealing with are just vocabs and terms.

  // Debug only:  
  if (! $anchor ) {
    // Message is just noise if using anchors.
    watchdog('taxonomy_xml', "
      Found %count different <strong>kinds</strong> of resources
      in the named input : %types
      ",
      array(
        '%count' => count($resources_by_type),
        '%types' => join(', ', array_keys($resources_by_type))
      ),
      WATCHDOG_INFO
    );
  }

  // Debug only:  
  if (! empty($resources_by_type[TAXONOMY_XML_UNTYPED])) {
    // Just FYI, make a note about the quality of data found.
    // Do not complain about URLs - this is quite normal.
    watchdog('taxonomy_xml', "
      Found %count Unsorted (untyped) resources.
      An untyped entity is the subject of a statement,
      but I don't know what <em>type</em> of thing they are.
      Not sure what I'll do with these.
      They are just things that have had statements made about them ..
      that I don't recognise.
      Probably just extra data found in the input and ignored.
      <br/>ID was: %unknown",
      array(
        '%count' => count($resources_by_type[TAXONOMY_XML_UNTYPED]),
        '%unknown' => join(', ', array_keys($resources_by_type[TAXONOMY_XML_UNTYPED]))
      ),
      WATCHDOG_DEBUG
    );
  }

  // Debug only:  
  if (count($resources_by_type) == 0) {
    watchdog('taxonomy_xml', "
      It sure doesn't look like this is any useful sort of RDF source.
      Zero resource entities were parsed out of it.
      Probably need to do content-negotiation or something, 
      and check the validity of the file. Aborting.",
      array('%url' => ''),
      WATCHDOG_ERROR
    );
    return;
  }

  #dpm($resources_by_type);

  //  
  // Almost ready to build. 
  // Prepare destination VOCAB.
  ////

  $vocabulary_types = taxonomy_xml_rdf_vocabulary_types();
  if ($vid == TAXONOMY_XML_DETERMINED_BY_SOURCE_FILE) {
    // If the vid has already been set, we ignore vocab definitions found in the file

    // Scan the sorted objects for vocabulary definitions
    // Hopefully there's only one vocab per file, but loop anyway
    $vocabularies = array();
    foreach ($vocabulary_types as $vocabulary_type) {
      if (isset($resources_by_type[$vocabulary_type]) && is_array($resources_by_type[$vocabulary_type])) {
        foreach ($resources_by_type[$vocabulary_type] as $guid => &$vocabulary_handle) {
          $vocabularies[$guid] = &$vocabulary_handle;
        }
      }
    }
    drupal_set_message(t("Found %count resources to be used as vocabulary definitions", array('%count' => count($vocabularies))));

    if (! $vocabularies) {
      // Create a placeholder.
      $vocabularies[] = (object) array('name' => 'Imported Vocabulary');
    }
    $vid = taxonomy_xml_absorb_vocabulary_definitions($vocabularies);
    // $vocabularies now contains a keyed array of target vocabularies the terms may be put into
    // $vid is the default one (most common is one vocab per input file) to be used unless otherwise defined per-term.

    if (empty($vid)) {
      drupal_set_message(t("No vocabulary to add terms to, aborting."), 'error');
      return FALSE;
    }
  }
  else {
    // Else using a form-selected vocob.
    $vocabularies[$vid] = taxonomy_vocabulary_load($vid);
  }

  //
  // VOCAB set up, start on TERMS...
  ///
  
  // Note that when 'identifier' is used as a key here, it means the identifier
  // according to the source document - usually a URI. 
  // A term identifier is a string distinct from the local term id.

  // Gather the resources that will become terms.
  // Slightly long way (not using array_merge), as I need to merge indexed and by reference
  $terms = array();
  $term_types = taxonomy_xml_rdf_term_types();
  foreach ($term_types as $term_type) {
    // watchdog('taxonomy_xml', 'Adding all %term_type to the list of terms to be processed', array('%term_type' => $term_type), WATCHDOG_DEBUG);
    if (isset($resources_by_type[$term_type]) && is_array($resources_by_type[$term_type])) {
      foreach ($resources_by_type[$term_type] as $guid => &$term_handle) {
        // Grab a name/label early for debugging and indexing (Premature, forget it)
        // $term_handle->name = taxonomy_xml_shortname($guid);
        $terms[$guid] = &$term_handle;
      }
    }
  }

  // Some of the RDF documents I've been fed DO NOT DEFINE A TYPE for their primary subject.
  // Neither
  // http://www.ubio.org/authority/metadata.php nor
  // http://biocol.org/ nor
  // http://lsid.tdwg.org/
  // return RDF that says WHAT the data is. Those that use LSIDs have a type encoded in the Identifier itself :-/

  // I end up with a collection of data but no idea what it's really talking about.
  // But IF an entity is rdf:about="THIS URL" then we will take a leap and assume that is our target lump of data.
  // ... this worked for biocol input
  foreach ( (array) @$resources_by_type[TAXONOMY_XML_UNTYPED] as $identifier => $untyped_lump) {
    if ($identifier == $url) {
      // Looks like this was the specific thing we were looking for
      watchdog('taxonomy_xml', "Trying to import an <em>untyped</em> data object in the hopes that it is the term we asked for. This may be incorrect, but it's all the document gave us. We asked, and got: '%identifier' .", array('%identifier' => $identifier ), WATCHDOG_NOTICE);
      watchdog('taxonomy_xml', "Untyped data object (possibly wrong) '%identifier' = <pre>%data</pre> .", array('%identifier' => $identifier, '%data' => print_r($untyped_lump, 1) ), WATCHDOG_DEBUG);
      $terms[$identifier] = $untyped_lump;
    }
  }

  // Special case for Freebase.
  taxonomy_xml_rdf_process_freebase_vocab($resources_by_type, $vid);
  // Special case for freebase. sub-terms are listed, but point to the parent, not vice-versa
  taxonomy_xml_rdf_process_dbpedia($resources_by_type, $terms);

  if (! $anchor ) {
    // Shh.
    drupal_set_message(t("Found %count resources to be imported as terms into vocabulary %vid", array('%count' => count($terms), '%vid' => $vid)));
  }

  //
  // START MAKING TERMS
  //
  foreach ($terms as $guid => &$term) {
    #drupal_set_message(t("Reviewing term %identifier '%name' and analyzing its properties", array('%identifier' => $identifier, '%name' => $term->name)));
    if (empty($term)) {
      watchdog('taxonomy_xml', "An empty term '%guid' was in the array of terms to create. This should not have happened, fix the input upstream. Ignoring.", array('%guid' => $guid ), WATCHDOG_NOTICE);
      continue;
    }

    if (!isset($term->vid)) {
      // This is just a default fallback. Imported terms should really have already chosen their vid.
      $term->vid = $vid;
    }
    taxonomy_xml_set_term_guid($term, $guid);
    taxonomy_xml_rdf_make_term($term);
  }

  // Now the terms are all happily created, create their relationships
  // Couldn't do so until they had all been given tids.
  taxonomy_xml_set_term_relations($terms);
  // Note this will not yet affect terms that have been queued for later processing.
  // Such terms will need to attach themselves to the parent terms themselves.

  watchdog('taxonomy_xml', "After re-linking, we now have all terms set <pre>!data</pre>", array('!data' => $terms), WATCHDOG_INFO);
  
  return $terms;
}

/**
 * FREEBASE only
 * 
 * If  we are reading a top-level topic type page
 * eg  http://www.freebase.com/tools/explore/music/genre
 * type = fb:type_profile
 * then it may contain a list of 'instances' which represent our desired
 * member terms.
 */
function taxonomy_xml_rdf_process_freebase_vocab(&$resources_by_type, $vid){
  $fb_vocab_type = TAXONOMY_XML_FB_NS . 'freebase.type_profile';
  if (empty($resources_by_type[$fb_vocab_type])) {
    return;
  }
  
  foreach ($resources_by_type[$fb_vocab_type] as $vocab_guid => $vocabulary) {
    if (empty($vocabulary->predicates)) {
      trigger_error("Something wrong with the vocabulary we are trying to process, it has no predicates");
      #dpm($vocabulary);
    }
    $instances = @$vocabulary->predicates['type.type.instance'];
    if (! empty($instances) ) {
      // I've got a list of URIs that represent terms, but not even a name for them
      // The system will still hopefully be able to work it out from just that.
      watchdog('taxonomy_xml', "
        FREEBASE: Each <em>instance</em> listed in a freebase <em>type profile</em>
        will be imported as a term.",
        array(), WATCHDOG_INFO
      );
      foreach ($instances as $term_guid) {
        $terms[$term_guid] = $placeholder_term = (object) array(
          'guid' => $term_guid,
          'vid' => $vid,
        );
        // Queue a full lookup of this item
        taxonomy_xml_add_term_to_batch_queue($placeholder_term);
        watchdog('taxonomy_xml',
          "Queuing a full retrieval of term !term_guid it for later retrieval and import",
          array(
            '!term_guid' => l($term_guid, $term_guid),
          )
          , WATCHDOG_INFO
        );
      } // loop over all term 'instances' mentioned by the vocab
    }
  
    // Extra diagnostic - freebase-specific
    $instance_count = $vocabulary->predicates['freebase.type_profile.instance_count'];
    if ($instance_count > count($instances) ) {
      watchdog('taxonomy_xml', "
        FREEBASE: The topic set definition claims there are %instance_count
        topic instances in the set, but I can see only %actual_count.
        Some data may be missing from this doc that I am unable to retrieve.
        ",
        array(
          '%instance_count' => reset($instance_count),
          '%actual_count' => count($instances),
        ), WATCHDOG_WARNING
      );
    }

    // Resources that are being processed as freebase vocabs are NOT also terms.
    // But the freenet schema labels topic sets as 'topics' themselves.
    // Unset this so as not to make a vocab definition a member of itself.
    unset($resources_by_type[TAXONOMY_XML_FB_NS . 'common.topic'][$vocab_uri]);
  }
}

/**
 * Special handling for dbpedia data
 * 
 * eg
 * http://dbpedia.org/page/Category:Rock_music_genres
 * 
 * When taking data from dbpedia, it does not list sub terms as 'narrower', it
 * instead lists all the subterms individually, and tags them as having the
 * parent term as 'broader'. This means the same thing, but the parent term does
 * not know about its children.
 * To support this, ensure that any resource (probably untyped) then has a
 * 'broader' property matching a current term id gets tagged as being a child of
 * it, and is present for being processed as a 'term'.
 */
function taxonomy_xml_rdf_process_dbpedia(&$resources_by_type, &$terms) {
  watchdog('taxonomy_xml', "Processing dbpedia special case", array(), WATCHDOG_INFO);
  foreach ($resources_by_type as $type => &$typedlist) {
    foreach ($typedlist as $guid => $resource) {
      if (isset($terms[$guid])) {
        // Already know this is a term
        continue;
      }
      // Should canonicize predicates here?
      if (isset($resource->predicates[TAXONOMY_XML_SKOS_NS .'broader'])) {
        // This is not a term, but is DOES have something else as a broader term
        // therefore it really is a term. (of unknown type)
        watchdog('taxonomy_xml', "Although not listed as a a term, %guid has something as a 'broader' parent. So it probably is a term after all. Adding it to the list", array('%guid' => $guid), WATCHDOG_INFO);
        $terms[$guid] = $resource;
      }
    }
  }
  return $terms;
}

/**
 * Invoke the ARC parser on the given data.
 *
 * Uses some minor caching if the base $url is the same.
 * If the requested base Uguidis the same as the previous one, you'll get a
 * cached version, but those data objects are not held onto in a true cache
 * array.
 * This will be optimal for one big file being called all the time (an all-in-
 * one taxonomy), and NOT fill up with crud if lots of different files are
 * requested once (as happens when spidering).
 * 
 * @return An indexed set of triples
 */
function taxonomy_xml_rdf_parse_data_into_index($data, $url) {
  static $old_index, $old_url;
  if (!empty($url) && $url == $old_url) {
    // re-using parser cache
    return $old_index;
  }
  watchdog('taxonomy_xml', "Parsing RDF", array(), WATCHDOG_INFO);

  // Use ARC parser
  if (! rdf_load_arc2()) {
    watchdog('taxonomy_xml', "ARC2 Parser was unavailable", array(), WATCHDOG_ERROR);
    return FALSE;
  }
  
  $parser = ARC2::getRDFParser();
  $base = $url;
  $parser->parse($base, $data);
  #dpm($parser->getTriples());

  $index = $parser->getSimpleIndex();
  // GetSimpleIndex flattens multiple values, eg different language labels for the same concept.
  // Cannot retrieve that info  if we use this method

  #dpm($index);
  // @todo bug in arc? attributes - eg href - nested in rdf seem to get damaged.
  // may be only when tagged as XMLLiteral rdf:parseType="Literal"
  
  if (! is_array($index)) {
    drupal_set_message(t("Problem parsing input %message", array('%message' => $index)), 'error');
    return;
  }
  watchdog('taxonomy_xml', "
    %count data objects (subjects) found in the source RDF doc",
    array('%count' => count($index)),
    WATCHDOG_INFO
  );
  #dpm($index);

  // Caching
  $old_url = $url;
  $old_index = $index;
  return $index;
}

/**
 * Sort a list of data objects into groups by 'type'
 * 
 * Arc2 indexing has done most of the flattening for us, we just need to throw
 * these things into different bags.
 * 
 * Ensure that objects have a 'name' and a 'type'
 */
function taxonomy_xml_convert_index_to_sorted_objects(&$index) {
  $resources_by_type = array();

  foreach ($index as $subject_guid => $values) {
    // A proto-resource object stores all the statements 
    // in an array called 'predicates' that we will inspect later.
    $subject = (object) array('predicates' => $values);
    
    // This remote URI is the key we use for real indexing 
    // when matching up children and parents.
    $subject->identifier = $subject_guid;
    
    // 'predicates' given to us from ARC2 are full namespaced URIs
    // yet through the rest of the code, we prefer to use the 
    // predicates 'shortname' (without namespace) most of the time.
    // Remember to flatten them if comparing with cannonic types later.
    
    #// Add a flattened version of all data here also!
    #foreach ($values as $predicate => $objects) {
    #  $subject->predicates[taxonomy_xml_rdf_shortname($predicate)] = $objects;
    #}
    # no, wait for later

    // We need to know that a thing has a name for later
    if (isset($values[TAXONOMY_XML_NAME])) {
      $subject->name = reset($values[TAXONOMY_XML_NAME]);
    }
    else {
      // Guess a shortname based on URI
      # $subject->name = taxonomy_xml_shortname($subject_guid);
      # premature, if set here it takes priority
      // A better, more accurate name may be deduced later on during 
      // synonym collapsing
    }
    
    if (isset($values[TAXONOMY_XML_TYPE])) {
      // Types may be multiple. Don't know why or how, but they may.
      // (see http://rdfs.org/sioc/types for one such)
      // In any case, it's always an array when we see it
      // To deal with twosies, place the same thing in two places by reference
      unset($handle);
      $handle = $subject; // copy
      foreach($values[TAXONOMY_XML_TYPE] as $type) {
        $subject->type = $type;
        $resources_by_type[$type][$subject_guid] = &$handle;
      }
      unset($handle);
      
    }
    else {
      // No idea what this is, remember it anyway.
      $resources_by_type[TAXONOMY_XML_UNTYPED][$subject_guid] = $subject;
    }
  }
  #dpm($resources_by_type);
  return $resources_by_type;
}


/**
 * Create the placeholder and fill in the values for this term - NOT its
 * relationships yet.
 */
function taxonomy_xml_rdf_make_term(&$term) {
  $identifier = taxonomy_xml_get_term_guid($term);
  # drupal_set_message(t("Reviewing term %identifier '%name' and analyzing its properties", array('%identifier' => $identifier, '%name' => @$term->name)));

  if (empty($identifier)) {
    watchdog('taxonomy_xml', "
      Attempting to make term, but no identifier is available. Can't do that. Skipping it. <pre>!term</pre>",
      array('!term' => print_r($term, 1)),
      WATCHDOG_ERROR
    );
    return NULL;
  }

  // When running in batch, children will have a hard time finding their
  // parents if they only know them by source-localized ID (probably a URI)
  // and the destination-taxonomy (here) HASN'T REMEMBERED THAT INFO.
  // Because taxonomy.module just doesn't.
  // We require some other method (fields on terms) to save that
  // metadata for us so the child can find its target later.
  // This is our 'identifier' - the REMOTE identifier not the local one.

  // Build term from data

  // Convert all input predicates into attributes on the object
  // the taxonomy.module will understand
  taxonomy_xml_canonicize_predicates($term);

  // Ensure name is valid, this is required.
  if (empty($term->name)) {
    // Fallback to a name, identifier derived (roughly) from the URI identifier - not always meaningful, but all we have in some contexts.
    $term->name = taxonomy_xml_shortname($identifier);

    if (empty($term->name)) {
      // Still not set?
      // This should be impossible - all subjects must have a URI or identifier
      // But who knows what wierdness the input gave us
      drupal_set_message(t("
        A term called %identifier didn't produce any readable name to use. "
        , array('%identifier' => $identifier)), 'error');
      watchdog('taxonomy_xml', "
        Invalid term object, not enough data : NO NAME <pre>!term</pre>",
        array('!term' => print_r($term, 1)),
        WATCHDOG_ERROR
      );
      return;
    }
    else {
      watchdog('taxonomy_xml', "
        We were unable to find a specific label for the term
        referred to as %identifier.
        Guessing that %name will be good enough.",
        array('%identifier' => $identifier, '%name' => $term->name),
        WATCHDOG_NOTICE
      );
      // Still, this causes problems if queuing data about terms that are not yet loaded
      // - such as those that are ONLY referenced by URI with no human name (Freenet)
      // Our munged names are temporary until the full data is retrieved.
    }
  }

  // See if a definition matching this terms name already exists in the DB.
  // Build on that.
  $force_new = variable_get('taxonomy_xml_duplicate', FALSE);
  $existing_term = _taxonomy_xml_get_term_placeholder($term->name, $term->vid, $force_new);

  // Merge the old term objects properties into this one. Really just want its tid, but there may be more info I should not lose.
  // New input takes precedence over older data. Old data just fills in the gaps.
  foreach ((array) $existing_term as $key => $value) {
    if (! isset($term->$key)) {
      $term->$key = $value;
    }
  }
  // The term object is now as tidy as it can be as a self-contained entity.
  # dpm($term);
  
  // It may be premature to save this term if we don't know its parent yet,
  // The system will default to parent=0, which causes bad structure later on
  if (! isset($term->parent)) {
    watchdog('taxonomy_xml', "About to save a term '%name' with no parent, this could be a problem later, but probably just means it's root-level", array('%name' => $term->name), WATCHDOG_INFO);
  }

  $status = taxonomy_term_save($term);
  // This object is being passed around as a handle, so I don't expect to have 
  // lost anything important from it

  if ( $status == SAVED_NEW ) {
    // Just remember this is fresh - for useful feedback messages.
    $term->taxonomy_xml_new_term = TRUE;
  }

  // It's possible that not all the referenced items were available in the current document/loop
  // Add referred items to the import queue for later processing
  taxonomy_xml_add_all_children_to_queue($term);
  $term->taxonomy_xml_presaved = TRUE; // A flag to avoid double-processing

  return $term;
} // end term-construction;




/**
 * Return an XML/RDF document representing this vocab
 *
 * I'd like to use ARC libraries, but it doesn't appear to include an RDF
 * serializer output method, only an input parser...
 *
 * Uses PHP DOM to create DOM document and nodes.
 *
 * We use namespaces carefully here, although it may create wordy output if the
 * DOM is not optimizing the declarations for us. Still, best to be explicit, it
 * would seem.
 *
 * The URI used to refer to other resources is based on the source document
 * location, eg
 * http://this.server/taxonomy/vocabulary/{vid}/rdf#{tid}
 *
 * Preamble should look something like:
 *
 * <rdf:RDF xmlns:rdf ="http://www.w3.org/1999/02/22-rdf-syntax-ns#"
 *   xmlns: rdfs="http://www.w3.org/2000/01/rdf-schema#"
 *   xmlns: owl="http://www.w3.org/2002/07/owl#"
 *
 * @return an XML document string.
 */
function taxonomy_xml_rdf_create($vocabulary, $parent = 0, $depth = -1, $max_depth = NULL) {
  $vocabulary = is_numeric($vocabulary) ? taxonomy_vocabulary_load($vocabulary) : $vocabulary;

  $domcontainer = taxonomy_xml_rdf_document();
  $dom = $domcontainer->ownerDocument;

  // define the vocab
  taxonomy_xml_rdf_add_vocab($domcontainer, $vocabulary);

  // Now start adding terms.
  // They are listed as siblings, not children of the ontology
  $tree = taxonomy_get_tree($vocabulary->vid, $parent, $max_depth, $depth);
  taxonomy_xml_rdf_add_terms($domcontainer, $tree);

  $result = $dom->savexml();

  // Minor layout tweak for readability
  // singletons go on their own lines
  $result = preg_replace('|(<[^<]*/>)|', "$1\n", $result);
  // nested tags go onto new lines.
  $result = preg_replace('|><|', ">\n<", $result);
  # dpm($result);
  return $result;
}


/**
 * Create a vocabulary definition (just the def, not its terms) and insert it
 * into the given document element.
 * 
 * No return, it acts on the DOM document directly.
 *
 * @param $domcontainer an XML dom document, modified by ref.
 * @param $vocabulary a vocab object
 */
function taxonomy_xml_rdf_add_vocab(&$domcontainer, $vocabulary) {
  $dom = $domcontainer->ownerDocument;
  $ns = module_invoke_all('rdf_namespaces');

  dpm($vocabulary);

  // Describe the vocabulary itself
  $vocabnode = rdf_entity_to_xml($vocabulary, $dom, $domcontainer);
  
  // If this was a cannonic vocab, we would use a full external URI as identifiers
  // But if it's our own, we get our own vocabulary path as a URI (or URN)
  $vocabnode->setattributens($ns['rdf'], 'rdf:about', taxonomy_xml_get_vocabulary_uri($vocabulary) );

  $vocabnode->appendchild(
    $dom->createelementns($ns['owl'], 'owl:versionInfo', xmlentities(format_date(REQUEST_TIME, 'long')))
  );
#  $domcontainer->appendchild($vocabnode);
}

/**
 * Given a list of terms, append definitions of them to the passed DOM container
 *
 * This term dump will directly reflect any rdf_mapping retrieved from
 * the Drupal 'entity' schema, which is based on SKOS.
 * 
 * <skos:Concept rdf:ID="term-1764" rdf:about="http://taxonomy.drupal7.
 * gadget/taxonomy/term/1764">
 * <rdfs:label>Corporate management (Internal)</rdfs:label>
 * <skos:prefLabel>Corporate management (Internal)</skos:prefLabel>
 * <skos:definition>
 *   Managing  the organisation's own corporate body
 * </skos:definition>
 * <skos:member>3</skos:member>
 * <skos:broader>1763</skos:broader>
 * </skos:Concept>
 * 
 * @param $termlist a FLAT array of all terms, internally cross-referenced to
 * each other defining the tree stucture
 * 
 * No return, it acts on the DOM document directly.
 */
function taxonomy_xml_rdf_add_terms(&$domcontainer, $termlist, $recursion_behaviour = TAXONOMY_XML_CHILDREN_REF_ONLY) {
  if (! $termlist) {
    return;
  }
  $dom = $domcontainer->ownerDocument;
  $ns = module_invoke_all('rdf_namespaces');

  // Allow submission of a single term
  if (! is_array($termlist)) {
    $termlist = array($termlist);
  }

  // D7 hook_taxonomy_term_load actually takes an array, not a singular
  module_invoke_all('taxonomy_term_load', $termlist);
#dpm($termlist);
  foreach ($termlist as $term) {
    #dpm($term);
    // rdf_entity_to_xml does a direct mapping from data structure to XML,
    // so picks up most of the default values, using rdf_mapping

    // The term SHOULD have its entity mapping details attached to it by now.
    // didn't module_invoke_all do that?
    // If I have to do it myself : INEFFICIENCY HERE due to the full reload.
    if (empty($term->rdf_mapping)) {
      $term = taxonomy_term_load($term->tid); 
    }
    
    $termnode = rdf_entity_to_xml($term, $dom, $domcontainer);
    
    if (! $termnode) {
      watchdog('taxonomy_xml', "Failed to create an XML entry for term, <pre>!data</pre>", array('!data' => $term), WATCHDOG_ERROR);
      continue;
    }
    
    # Not allowed both an ID and an about
    #$termnode->setattributens($ns['rdf'], 'rdf:ID', 'term-' . $term->tid );

    #$domcontainer->appendchild($termnode);

    // Set either the local or (preferably) the cannonic remote URI as the elements
    // 'about' attribute.

    $guid = taxonomy_xml_get_term_guid($term);
    $termnode->setattributens($ns['rdf'], 'rdf:about', $guid );
    
    $vocab_ref = $dom->createelementns($ns['skos'], 'skos:member');
    $vocabulary = taxonomy_vocabulary_load($term->vid);
    $vocab_guid = taxonomy_xml_get_vocabulary_uri($term->vid);
    $vocab_ref->setattributens($ns['rdf'], 'rdf:resource', $vocab_guid);
    $vocab_ref->setattributens($ns['rdf'], 'rdf:value', xmlentities($vocabulary->name) );
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

    if (!empty($term->parent)) {
      foreach ((array) $term->parent as $parent_id) {
        $parentlist = array();
        if ($parent_id) {
          $parentlist[$parent_id] = $parent =  taxonomy_term_load($parent_id);
          #$parent_node = $dom->createelementns($ns['rdfs'], 'rdfs:subClassOf', xmlentities($parent->name));
          #$parent_node->setattributens($ns['rdf'], 'rdf:resource', '#term-' . $parentid );

          $parent_node = $dom->createelementns($ns['skos'], 'skos:broader');
          $parent_node->setattributens($ns['rdf'], 'rdf:resource', taxonomy_xml_get_term_guid($parent) );
          $parent_node->setattributens($ns['rdf'], 'rdf:value', xmlentities($parent->name) );
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
          #$child_node = $dom->createelementns(TAXONOMY_XML_RDFS_NS, 'rdfs:superClassOf', xmlentities($child->name));
          #$child_node->setattributens(TAXONOMY_XML_RDF_NS, 'rdf:resource', '#term-' . $child_id );
          $child_node = $dom->createelementns($ns['skos'], 'skos:narrower');
          $child_node->setattributens($ns['rdf'], 'rdf:resource', taxonomy_xml_get_term_guid($child) );
          $child_node->setattributens($ns['rdf'], 'rdf:value', xmlentities($child->name) );
          $termnode->appendchild($child_node);
        }
        
    }
      

    // workaround for large vocabs - extend runtime indefinately
    drupal_set_time_limit(10);
  }
  // Done all terms in list
}


/**
 * Return a term as RDF-XML
 * 
 * A sub-hook implementation of taxonomy_xml_{format}_create_term()
 * @see taxonomy_xml_export_term()
 * 
 * @param Either a term object, or a term id
 * @return an XML string.
 */
function taxonomy_xml_rdf_create_term($term, $depth = -1, $max_depth = NULL) {
  $term = is_numeric($term) ? taxonomy_term_load($term) : $term;

  // Load in all extra data ? All taken core of in D7?
  if (empty($term)) {
    watchdog('taxonomy_xml', "NULL term loaded <pre>!data</pre>", array('!data' => func_get_args()), WATCHDOG_ERROR);
    return FALSE;
  }

  $domcontainer = taxonomy_xml_rdf_document();
  $dom = $domcontainer->ownerDocument;

  // Although we were only asked for one term, child or parent terms may be mentioned when the entry is built
  taxonomy_xml_rdf_add_terms($domcontainer, $term);

  $result = $dom->savexml();

  // Minor layout tweak for readability
  $result = preg_replace('|(<[^<]*/[^>]*>)|', "$1\n", $result);
  $result = preg_replace('|><|', ">\n<", $result);
  # dpm($result);
  return $result;
}
