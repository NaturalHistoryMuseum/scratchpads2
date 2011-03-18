<?php
/* double-commented to avoid conflict with svn
 */

/**
 * @file
 * The workhorse processes for importing
 * taxonomies  
 */

/**
 * Fetch the named vocab if it exists
 *
 * Create and return a useful placeholder if not.
 *
 * @return
 *   The new vocab object.
 */
function _taxonomy_xml_get_vocabulary_placeholder($name) {
  if (empty($name)) {
    drupal_set_message("Cannot create a vocabulary with no name", 'error');
    return FALSE;
  }
  if (! ($vocabulary = taxonomy_xml_get_vocabulary_by_name($name))) {
    // Create new vocab
    $machine_name = substr(preg_replace('/[^a-z0-9]+/', '_', strtolower($name)), 0, 21);
    $vocabulary = (object) array(
      'name' => $name,
      'relations' => TRUE,
      'hierarchy' => 2,
      'machine_name' => $machine_name,
    );
    taxonomy_vocabulary_save($vocabulary);
    drupal_set_message(t('Created vocabulary #%vid: %vocabname to put these terms into. You probably want to <a href="!vocablink">go edit it now</a>.', array('%vocabname' => $vocabulary->name, '%vid' => $vocabulary->vid, '!vocablink' => url(TAXONOMY_XML_ADMIN . '/' . $vocabulary->machine_name . '/edit') )));
  }
  else {
    // A valid Vocab exists,
    // But still may need to add some fields...
  }
  
  // Ensure vocab will store our URI and extra values
  if (! taxonomy_xml_prepare_vocabulary($vocabulary) ) {
    drupal_set_message("Problem preparing vocabulary", 'error');
    dpm($vocabulary);
  }
  
  // The $vocabulary object will now have a vid
  return $vocabulary;
}

/**
 * Ensure a vocab will store our URI and extra values.
 * 
 * This adds a new 'guid' field and ressurects the missing 'synonym' field on
 * taxonomy terms.
 */
function taxonomy_xml_prepare_vocabulary(&$vocabulary) {
  if (! $vocabulary->machine_name) {
    drupal_set_message("Cannot prepare a vocabulary with no machine name", 'error');
    return FALSE;
  }
  if (! field_info_field('field_guid')) {
    // Create the generic, uninstanced field definition
    taxonomy_xml_create_guid_field();
  }
  if (! field_info_instance('taxonomy_term', 'field_guid', $vocabulary->machine_name)) {
    watchdog('taxonomy_xml', "Adding %field_type storage support to vocabulary %vocab_machinename.", array('%field_type' => 'URI', '%vocab_machinename' => $vocabulary->machine_name), WATCHDOG_INFO );
    taxonomy_xml_create_guid_instance($vocabulary->machine_name);
  }

  if (! field_info_field('field_synonym')) {
    taxonomy_xml_create_synonym_field();
  }
  if (! field_info_instance('taxonomy_term', 'field_synonym', $vocabulary->machine_name)) {
    watchdog('taxonomy_xml', "Adding %field_type storage support to vocabulary %vocab_machinename.", array('%field_type' => 'synonym', '%vocab_machinename' => $vocabulary->machine_name), WATCHDOG_INFO );
    taxonomy_xml_create_synonym_instance($vocabulary->machine_name);
  }
  watchdog('taxonomy_xml', "Prepared vocabulary %vocab_machinename with additional required fields.", array('%vocab_machinename' => $vocabulary->machine_name), WATCHDOG_INFO );
  return TRUE;
}

/**
 * Create a URI field that will be applied to our vocabularies so we can
 * identify their terms.
 * 
 * Uses field API CRUD
 */
function taxonomy_xml_create_guid_field() {
  $field = array(
    'field_name' => 'field_guid',
    'type' => 'text',
    'entity_type' => 'taxonomy_term',
    'module' => 'text',
    'cardinality' => 1,
    'translatable' => TRUE,
    'locked' => TRUE,
    'indexes' => array('format' => array('format')),
    'settings' => array(
      'max_length' => 255,
    ),
    'storage' => array(
      'type' => 'field_sql_storage',
      'settings' => array(),
    ),
  );
  return field_create_field($field);
}

/**
 * Create a field instance to attach to a vocab and store the URI in
 * 
 * Uses field API CRUD
 */
function taxonomy_xml_create_guid_instance($vocab_name, $field_name = 'field_guid') {
  $instance = array(
    'field_name' => $field_name,
    'entity_type' => 'taxonomy_term',
    'label' => 'GUID',
    'bundle' => $vocab_name,
    'description' => 'Unique global identifier for this term. Usually an URL or URI, but need not be resolvable.',
    'required' => FALSE,

    'widget' => array(
      'type' => 'text_textfield',
      'module' => 'text',
      'settings' => array(
        'size' => 60,
      ),
      'weight' => -1,
    ),

    'display' => array(
      'full' => array(
        'label' => 'hidden',
        'type' => 'text_default',
        'settings' => array(),
        'weight' => -1,
      ),
    ),
  );
  return field_create_instance($instance);
}

/**
 * Create a synonym field that will be applied to our vocabularies.
 * A replacement for the D6 synonym table.
 * 
 * Uses field API CRUD
 */
function taxonomy_xml_create_synonym_field() {
  $field = array(
    'field_name' => 'field_synonym',
    'type' => 'text',
    'module' => 'text',
    'cardinality' => 0,
    'locked' => TRUE,
  // defaults should be able to fill in the rest here?
/*
    'translatable' => TRUE,
    'indexes' => array('format' => array('format')),
    'settings' => array(
      'max_length' => 255,
    ),
    'storage' => array(
      'type' => 'field_sql_storage',
      'settings' => array(),
    ),
*/
  );
  return field_create_field($field);
}

/**
 * Create a field instance to attach to a vocab and store the synonyms in
 * 
 * Uses field API CRUD
 */
function taxonomy_xml_create_synonym_instance($vocab_name, $field_name = 'field_synonym') {
  $instance = array(
    'field_name' => $field_name,
    'entity_type' => 'taxonomy_term',
    'label' => 'Synonyms',
    'bundle' => $vocab_name,
    'description' => 'Alternative names for this term. Note that this data has no special meaning unless extended with other Drupal extensions.',
    'required' => FALSE,

    'widget' => array(
      'type' => 'text_textfield',
      'module' => 'text',
      'settings' => array(
        'size' => 60,
      ),
      'weight' => -1,
    ),

    'display' => array(
      'full' => array(
        'label' => 'hidden',
        'type' => 'text_default',
        'settings' => array(),
        'weight' => -1,
      ),
    ),

  );
  return field_create_instance($instance);
}

/**
 * Create Vocabulary definitions.
 *
 * Use the vocabs defined as resources in the input.
 *
 * @param
 *   $vocabularies    An array of vocabulary definition objects, extracted from
 * the XML. Modified with their deduced or new vid values by reference
 *
 * $vocabularies = array(
 *   '#vocabulary-3' => stdClass Object
 *     'name' => "Countries",
 *     'predicates'  => array(
 *       'description' => array( 0 => "A list of countries" ),
 *       'version'  => array( 0 => "2008-08-08" ),
 *     )
 *   )
 * )
 *
 * All 'predicates' will be compressed into properties. EG in the above example,
 * ['#vocabulary-3']['predicates']['description'][0] = "a list"
 * is flattened to
 * ['#vocabulary-3']['description'] = "a list"
 *
 * Either input format is fine.
 *
 */
function taxonomy_xml_absorb_vocabulary_definitions(&$vocabularies) {
  // See if we can match this definition against the given vid
  // - then on name.
  // If neither seems to exist, make a new one.
  $vocab_ids = array();
  if (is_array($vocabularies)) {
    #dpm(array("The vocabulary definition(s) found in the input file is ", $vocabularies));
    // There may be more than one vocab def per file, although this is unlikely

    // Diagnostics:
    if (count($vocabularies) > 1) {
      drupal_set_message(t("When importing, I found what looked like more than one vocabulary definition in the same resource. This could be confusing in batch jobs, but should be OK if each of the term items include an rdfs:isDefinedBy field. <pre>!object</pre>", array('!object' => print_r($vocabularies, 1))), 'warning' );
    }

    foreach ($vocabularies as $guid => &$vocab) {
      
      // Relabel any big namespaced predicates into common ones.
      // For vocabs this won't do much, but will still help consolidate descriptions and labels
      taxonomy_xml_canonicize_predicates($vocab, 'taxonomy_vocabulary');

      // Merge all predicate data into a simpler array, re-tagging the attributes as needed
      // - there's not a lot of metadata about vocabs we know how to import, but try anyway - for expansion
      if (! empty($vocab->predicates)) {
        taxonomy_xml_merge_predicates_into_attributes($vocab);
      }

      if (empty($vocab->name)) {
        drupal_set_message("We require a NAME to create a vocabulary. Vocabulary definition appeared to have no name. Using a label derived from the URI instead.", 'warning');
        // Make up a name based on the URI
        $vocab->name = taxonomy_xml_shortname($guid);
      }

      $target_vocab = NULL;
      // Deduce the given vocab definitions vid, if given as a value
      if (isset($vocab->vid)) {
        $vocab->internal_id = $vocab->vid;
        drupal_set_message(t("Found a vocabulary definition in the input, called $guid. vid=$vocab->internal_id"));

        // Try to maintain old Vocabulary IDs
        // Check if it's a good number to write into
        // If the input defines a vid BUT there is already a non-matching vocab with that number, we need a new number
        // If it DOES seem to match, we are safe
        $target_vocab = taxonomy_vocabulary_load($vocab->internal_id);
      }

      #dpm(array('loaded vocab' => $target_vocab, 'parsed vocab' => $vocab));
      if (! empty($target_vocab) && ($target_vocab->name == $vocab->name)) {
        // Looks like a great match
        $vocab->vid = $vocab->internal_id;
        drupal_set_message(t("Found matching target vocabulary '%vocab_name' vid=%vocab_vid", array('%vocab_name' => $vocab->name, '%vocab_vid' => $vocab->vid)));
      }
      else {
        if ($target_vocab) {
          drupal_set_message(t("The vocab ID given in the input file (%vocab_vid) conflicts with an existing vocabulary. We need a different ID... ", array('%vocab_vid' => $vocab->vid) ));
        }
        unset($vocab->vid);
        // Vocab with this id exists, but is called something else - Do not merge with it
        // Look for a match by name instead
        if ($target_vocab = taxonomy_xml_get_vocabulary_by_name($vocab->name)) {
          // Found a local vocab called the same as the input vocab. That's a good enough match for us.
          $vocab->vid = $target_vocab->vid;
          drupal_set_message(t("Found a target vocabulary already in the database, matching by name '%name' vid=%vid . This will be used, but not updated.", array('%name' => $vocab->name, '%vid' => $vocab->vid)));
        }
      }

      // Have we found a target vocab yet?
      if (empty($vocab->vid)) {
        // Make a brand new one from the imported definition
        // There is very little we can import from the given data?
        $vocab = _taxonomy_xml_get_vocabulary_placeholder($vocab->name);
        $vocab->description = $guid;

        // Built a vocabulary from input details. Now save it
        $status = taxonomy_vocabulary_save($vocab);
        $strings = array(
          '%name' => $vocab->name,
          '%description' => $vocab->description
        );
        if (!empty($vocab->vid)) {
          drupal_set_message(t("Made a new Drupal vocabulary definition from data found in the input. Vocab is called: '%name': %description ", $strings));
        }
        else {
          drupal_set_message(t("Failed to create a new vocabulary called: '%name' : %description \n This is fatal, aborting.", $strings), 'error');
          return FALSE;
        }
        #dpm($vocab);
      }
      $vocab_ids[$guid] = $vocab->vid;
    } // end looping through found vocabs
  } // vocabs exist
  else {
    drupal_set_message("The document provided no recognisible vocabulary definitions");
  }
  
#  // Dealing with multiples is currently in flux...
#  // If there are many, then they have a useful identifier.
#  // return an array matching their guid with the vid.
#  // else old behavior - return an int.
#  if (count($vocab_ids) > 1) {
#    return $vocab_ids;
#  }
$taxonomy_xml_vocabulary_ids = variable_get('taxonomy_xml_vocabulary_ids', array()) + $vocab_ids;
variable_set('taxonomy_xml_vocabulary_ids', $taxonomy_xml_vocabulary_ids);

  // This is the default (last found) vid. Probably should not be used, but we may have to make a guess.
  // Either an input file contains just one vocab - in which case this will be right,
  // or the input file contains multiple vocabularies - in which case the terms damn well ought to be tagged with which vocab to use.

  return reset($vocab_ids);
}


/**
 * Convert aliased predicates into common ones.
 *
 * Given a term with a collection of named predicate relations, convert those
 * into canonic (known, defined) terms. This involves some duplication as the
 * original and true names are both packed into the $term->predicates array.
 * Only the true names are looked at later however.
 * 
 * This operated by reference on the object it's given.
 * It assumes it's processing a term, but the same logic holds for vocab
 * entities also.
 * 
 * @param $entity type is just a hint to avoid irrelevant warnings if we are
 * 'canonisizing' a vocab definition instead.
 */
function taxonomy_xml_canonicize_predicates(&$term, $entity_type = 'taxonomy_term') {
  // Translate the predicate statements into what we need
  if (empty($term->predicates )) {
    $term->predicates = array();
  }

  // Integrity Assertion
  if (empty($term->vid) && $entity_type == 'taxonomy_term') {
    watchdog('taxonomy_xml', '
      Processing a term with no vocabulary ID, this is unexpected. 
      Terms should have a vid set by this point. 
      I do not require the vid here, but all term objects should have had it 
      set before reaching this point. 
      <pre>!term</pre>
    ', array('!term' => print_r($term, 1)), WATCHDOG_WARNING);
  }


  // $predicate_synonyms is a translation array to match synonyms from various syntaxes with Drupal concepts
  $predicate_synonyms = taxonomy_xml_relationship_synonyms();

  foreach ($term->predicates as $predicate => $values) {
    $original_predicate = $predicate;

    // First translate misc terminology synonyms to the cannonic predicate I use everywhere
    // This allows us to interpret several XML dialects at once

    if (isset($predicate_synonyms[$predicate]) && $cannonic = $predicate_synonyms[$predicate]) {
      $predicate = $cannonic;
    }
    else {
      // Predicates may still be namespaced at this point. 
      // Remove the namespace and see if 
      // the shortname matches one of our known types
      $shortname = taxonomy_xml_shortname($predicate);
      if (isset($predicate_synonyms[$shortname]) && $cannonic = $predicate_synonyms[$shortname]) {
        $predicate = $cannonic;
      }
    }

    # drupal_set_message(t("Applying '$predicate' ($predicate) value of ". print_r($values, 1) ." found in $guid"));
    switch ($predicate) {
      case TAXONOMY_XML_DESCRIPTION:
        $term->description = taxonomy_xml_get_literal_string($values);
        break;
      case TAXONOMY_XML_NAME:
        // In the (hopefully never) case that a term has, eg, both a 'name'
        // and a 'title' set, and different, we may have conflicts to resolve.
        // Pre-empt this here by noting both, but revisit if we can come up
        // with better logic, eg ordering priority of synonyms.
        $val = taxonomy_xml_get_literal_string($values);
        if (isset($term->name) && $val != $term->name) {
          $term->name .= ' (' . $val . ')';
        }
        else {
          $term->name = $val;
        }
        break;
      case TAXONOMY_XML_PARENT:
        foreach ($values as $i => $target_guid) {
          $term->predicates[$predicate][$i] = $target_guid;
          $strings = array(
            '%predicate' => $predicate,
            '%subject' => isset($term->name) ? $term->name : $term->guid,
            '%target_guid' => $target_guid,
          );
        }
        break;
      case TAXONOMY_XML_RELATED:
      case TAXONOMY_XML_CHILD:
        // A term relationship.
        // Translate each referred item from URI to its label or handle,
        // and save to be linked in later
        foreach ($values as $i => $target_guid) {
          $term->predicates[$predicate][$i] = $target_guid;
        }
        break;
      case TAXONOMY_XML_HAS_SYNONYM:
        $term->synonyms_array = isset($term->synonyms_array) ? array_merge($term->synonyms_array, $values) : $values;
        $term->synonyms = implode("\n", array_unique($term->synonyms_array) );
        break;
      case TAXONOMY_XML_IN_VOCABULARY:
        /* currently not used very much - more than one vocab per input file is rare
         // This term need to be in the vocabulary referred to by this URI
         // check our known vocabs to see if they are recognised
         // Do we know a vocab with an ID matching this 'isdefinedby' value?
         dpm(array('looking for vocab' => $values));
         @see taxonomy_xml_absorb_vocabulary_definitions()
         where we make a temporary global note of vocab guid->vid relations for 
         this purpose.
         */
         $taxonomy_xml_vocabulary_ids = variable_get('taxonomy_xml_vocabulary_ids', array());
         #watchdog('taxonomy_xml', 'looking for vocab '. join(', ', $values) .' '. print_r($taxonomy_xml_vocabulary_ids, 1), array(), WATCHDOG_INFO);
         foreach ($values as $value) { 
           // Probably just one...
           // but IF it's named and IF the name is valid and we've seen it beforem
           if (isset($taxonomy_xml_vocabulary_ids[$value])) {
             // I know this vocab!
             $term->vid = $taxonomy_xml_vocabulary_ids[$value];
             #watchdog('taxonomy_xml', "I found that the identified vocab %vocab_id is vocab %vocab_num", array('%vocab_id' => $value, '%vocab_num' => $term->vid), WATCHDOG_INFO);
           }
         }
         
         
        break;
      case 'type':
        // These are already done. Ignore
      case TAXONOMY_XML_UNUSED:
        // Explicitly ignore these, 
        // Unset and discard them in fact!
        unset($term->predicates[$original_predicate]);
        break;
      case TAXONOMY_XML_OTHER_PREDICATE:
        // These ones we'll try to save as RDF statements, attached to the term URI
        foreach ($values as $value) {
          $term->rdf[] = array(
            'subject' => NULL,
            'predicate' => $original_predicate,
            'object' => $value
          );
        }
        watchdog('taxonomy_xml', "
          Found a useful predicate '<b>%predicate</b> = %value'.
          Making a note of it for pure-RDF storage.
          ",
          array(
            '%predicate' => "$predicate ($original_predicate)",
            '%subject' => isset($term->name) ? $term->name : $term->guid,
            '%value' => $value,
          ),
          WATCHDOG_DEBUG
        );
        break;
      default:
        // A valid, but unrecognised statement was found when flattening the input
        watchdog('taxonomy_xml', "
          Dunno what to do with '<b>%predicate</b>'.
          Subject '%subject' has value(s) = <pre>!values</pre>
          A later content type may absorb this info,
          but it's not a core term property.",
          array(
            '%predicate' => $predicate,
            '%subject' => isset($term->name) ? $term->name : $term->guid,
            '!values' => print_r($values, 1)
          ),
          WATCHDOG_INFO
        );
    }

  }
  if (!empty($term->guid)) {
    taxonomy_xml_set_term_guid($term, $term->guid);
  }
  #dpm(array(__FUNCTION__ => $term, 'transformed predicates are' => $term->predicates));
}


/**
 * Queue up an import action.
 *
 * If the currently processing term refers to other terms by URI, set up a
 * job to retrieve them recursively later.
 *
 * For all unknown $term->predicates[TAXONOMY_XML_CHILD] URIs, add a job to the
 * batch queue.
 *
 * Helper function for parser routines
 *
 * @see taxonomy_xml_add_term_to_batch_queue()
 */
function taxonomy_xml_add_all_children_to_queue($term) {
  #dpm(array('Queuing children of ' => $term));
  if (variable_get('taxonomy_xml_recurse_down', TRUE) && !empty($term->predicates[TAXONOMY_XML_CHILD]) ) {
    // Add child items to the import queue.
    $children = $term->predicates[TAXONOMY_XML_CHILD];
    #dpm(array('Queuing children' => $children));

    foreach ((array) $children as $child_ref) {

      // Check that it looks like a valid URL we can request
      $scheme = "unknown";
      if (valid_url($child_ref)) {
        // The ref is a URI.
        // but LSID identifiers pass that test :)
        $url_parts = @parse_url($child_ref);
        $scheme = isset($url_parts['scheme']) ? $url_parts['scheme'] : 'no scheme';
      }
      if (isset($url_parts['host']) && $url_parts['host'] == '_') {
        // BEWARE, RDF bnodes ("_:123") may look like URIs
        // Ignore them
        continue;
      }

      if ($scheme == 'http') {

        // Check we don't know it already.
        if ($found_term = taxonomy_xml_get_term_by_guid($child_ref, $term->vid)) {
          watchdog(
            'taxonomy_xml',
            "
              While processing %term_name, found an existing local version of it. 
              !ref 
              Therefore NOT queuing it for further recursion again.
            ",
            array(
              '%term_name' => $term->name,
              '!ref' => l('#' . $found_term->tid, 'taxonomy/term/' . $found_term->tid)
            )
            , WATCHDOG_DEBUG
          );
          // This is cool, we have a handle on this term.
          // Make a note in the global list
          $terms = & taxonomy_xml_current_terms();
          $terms[$child_ref] = $found_term;
        }
        else {
          // Save the request as a batch job to do later. Our session queue will tuck this away and remember it.
          // Create a placeholder so at least we know where this item is being imported to
          // Beware memory. This should be lightweight, as the batch API seems to be treating it inefficiently.
          $placeholder_term = (object) array(
            'guid' => $child_ref,
            'parent' => array($term->tid => $term->tid),
            'vid' => $term->vid,
          );

          // Some data sources MAY supply a known name for this child, but that's too hard to extract
          // Trust the named resource will fill in the gaps, and just know it's a URI for now

          taxonomy_xml_add_term_to_batch_queue($placeholder_term);
          watchdog('taxonomy_xml', "
            While processing %term_name,
            Found a reference to child term !child_ref.
            I did not recognise that URI in this vocab %vid.
            Queuing it for later retrieval and import",
            array(
              '%term_name' => $term->name,
              '!child_ref' => l($child_ref, $child_ref),
              '%vid' => $term->vid,
            )
            , WATCHDOG_NOTICE
          );
        }
      }
      else {
        // The referred term is not a URI, nor do we recognise its identifier so far.
        // It's a dangling reference. What can we do?
        // Handle URI/GUID lookup services?
        //
        // @todo Should do this with a hook/service-callback rather than built into this module.

        // @todo - this uses a global, should use batch context info
        // Lets see if it fits the pattern that a lookup service expects
        if ($service_id = variable_get('taxonomy_xml_service_id', '')) {
          $services = taxonomy_xml_lookup_services(NULL, 'full');
          $service = $services[$service_id];
          $lookup_guid = taxonomy_xml_sub_placeholders_into_pattern($service['pattern'], array($service['identifier'] => $child_ref) );

          #drupal_set_message(t('Will use service lookup to find !child_ref', array('!child_ref' => l($child_ref, $lookup_guid), '%servicename' => $service['servicename'])));

          $placeholder_term = (object) array(
            'guid' => $lookup_guid,
            'parent' => array($term->tid => $term->tid),
          );
          taxonomy_xml_add_term_to_batch_queue($placeholder_term);
        }
        else {
          drupal_set_message(t('Cannot yet resolve non-URI references, and no resolver service is active. %child_ref', array('%child_ref' => $child_ref)));
        }
      }
    }
  }
}


/**
 * Merge all predicate data into a simpler array
 *
 * Re-tags the attributes as needed
 *
 * @param
 *   An   object containing a 'predicates' array. For each predicate, a
 * cannonically named attribute will be attached to the object.
 */
function taxonomy_xml_merge_predicates_into_attributes(&$object) {
  if (empty($object)) {
    return;
  }
  $predicate_synonyms = taxonomy_xml_relationship_synonyms();

  // Diagnostics:
  if (empty($object->predicates)) {
    watchdog('taxonomy_xml', "When importing an object, I found some data with no predicates at all. This is odd, but probably no big deal. <pre>!object</pre>", array('!object' => print_r($object, 1)), WATCHDOG_NOTICE );
    $object->predicates = array();
  }
  foreach ($object->predicates as $predicate => $vals) {
    $predicate = isset($predicate_synonyms[$predicate]) ? $predicate_synonyms[$predicate] : $predicate;
    // If there are multiple and we need just one, make a guess. Can't do much else
    $object->$predicate = taxonomy_xml_get_literal_string($vals, $predicate);
  }
  if (empty($object->description) && isset($object->{TAXONOMY_XML_DESCRIPTION})) {
    $object->description = $object->{TAXONOMY_XML_DESCRIPTION};
  }
  return $object;
}


/**
 * Either fetch the named term if it exists, or return a useful placeholder.
 *
 * The returned term has a 'synonyms_array' because that's easier to work with
 * than string concats in odd places.
 *
 * If $new = TRUE, then always return a fresh placeholder, do not attempt a
 * string name lookup.
 * Default is $new = FALSE, which attempts re-use of existing terms.
 */
function _taxonomy_xml_get_term_placeholder($name, $vid = 0, $new = FALSE) {
  #dpm("Getting placeholder '$name' vocab:$vid");
  if (! $new) {
    // Look for a pre-existing term by that name
    if ($name) {
      $term = taxonomy_xml_get_term_by_name_from_vocab($name, $vid);
    }
    else {
      // Assert input is OK. Just paranoia
      drupal_set_message(t("Asked to make a term with no name ... that can't be right. I refuse!"), 'error');
      return NULL;
    }
  }

  if (empty($term)) {
    $term = (object) array(
      'name' => $name,
      'vid'  => $vid,
      'description' => '',
      'weight' => 0,
      'predicates' => array(),
      'synonyms_array' => array(),
    );
  }
  else {
    #drupal_set_message(t("A term called '!name' already exists. We will just update information onto it.", array('!name' => l($term->name, TAXONOMY_XML_ADMIN . '/edit/term/' . $term->tid) )), 'trace');
  }
  return $term;
}


/**
 * Given a list of terms, set  the related-terms and structure, and save again
 *
 * Helper function for bulk processes.
 *
 * The terms are currently indexed by either URI or name. The reference arrays
 * refer to either the URI or name. Scan the current array for the objects
 * (terms) being linked to.
 *
 * Input would look (in part) like this:
 *
 * $terms = array(
 *   '#123' => (
 *      'name' => 'hotels',
 *      'tid' => 23,
 *      'predicates' => (
 *        'See Also' => ['#135', 'camping']
 *        'Broader Term' => ['accomodation']
 *      )
 *    )
 *   '#135' => ( 'name' => 'motels', 'tid' => 35 )
 *   '#145' => ( 'name' => 'camping', 'tid' => 37 )
 *   'accomodation' => ( 'name' => 'accomodation', 'tid' => 11 )
 * )
 *
 * The process will read the 'predicates', figure out what they mean, figure out
 * which other term is being referenced, and create properties on the term
 * object.
 *
 * And will return the term objects with appropriate Drupal attributes
 *
 *   '#123' => (
 *      'name' => 'hotels',
 *      'nid' => 23,
 *      'parent' => 11,
 *      'relations' => array(35, 37),
 *    )
 *
 * Note that the key need have no relation with the nid,
 * and may be a full string, which will work just as well.
 * The above shows an example of both,
 * although that would be rare in the one import.
 *
 *
 * Relationships cannot be created if the target term is not included in the
 * $terms list itself. If we are adding to an existing vocab, doing a partial
 * merge, the target terms should have already been retrieved from the database
 * and included in the complete list.
 *
 *
 * @param
 *   An    indexed array of existing taxonomy term objects, possibly referring
 * to each other by id. It's possible for the same term to be in the list twice,
 * under different keys, (indexed by tid, name or URL) but these should be
 * HANDLES on the same object by reference, so changes will stick.
 */
function taxonomy_xml_set_term_relations(&$terms) {
  watchdog('taxonomy_xml',  __FUNCTION__ . " Now connecting all known term relations and hierarchy links between this group of %count related terms. ", array('%count' => count($terms) ), WATCHDOG_INFO);

  $relationship_predicates = array(TAXONOMY_XML_PARENT, TAXONOMY_XML_CHILD, TAXONOMY_XML_RELATED);

  foreach ($terms as $guid => &$term) {
    if (empty($term)) {
      watchdog('taxonomy_xml', "An empty term '%guid' was in the array for relinking. This should not have happened, fix the input upstream. Ignoring.", array('%guid' => $guid ), WATCHDOG_NOTICE);
      continue;
    }

    // Avoid doing this again if we are stuck in a recursive loop,
    // batch, or working with duplicate handles
    if (isset($term->taxonomy_xml_linked)) {
      continue;
    }
    // The predicates (relationships) array may contain actual handles on terms, term ids,
    // or it may still contain URIs representing terms not yet loaded in this phase.
    // We need to resolve those external references into term handles (or at least tids) if possible.
    if (isset($term->predicates) && is_array($term->predicates)) {
      foreach ($term->predicates as $predicate => &$targets) {
        if (in_array($predicate, $relationship_predicates)) {

          // Keep a list just for logging
          $found_term_names = array();

          foreach ($targets as $targetid => &$target) {
            #dpm(t("Term %termname references %targetid as a %predicate", array('$termname' => $term->name, '%targetid' => $targetid, '%predicate' => $predicate )));

            // Here we first try to find the referred term in the list of recently-made terms
            if (! isset($terms[$targetid])) {
              #drupal_set_message(t("Referenced term %targetid seems unknown so far, need to try a bigger lookup for it", array('%targetid' => $targetid)));
              // taxonomy_enhancer.module, if available, may have more data about our terms. Hopefully including a GUID.
              if ($found_term = taxonomy_xml_get_term_by_guid($targetid, $term->vid)) {
                $terms[$targetid] = $found_term;
                $found_term_names[] = l($found_term->name, "term/{$term->tid}") . ' ' . l('#', $targetid);
              }
              else {
                if ($found_term = taxonomy_xml_get_term_by_guid($target, $term->vid)) {
                  $terms[$target] = $found_term;
                  $found_term_names[] = l($found_term->name, "term/{$term->tid}") . ' ' . l('#', $target);
                }
                else {
                  watchdog('taxonomy_xml', "When processing '%term_name', we so far have no knowledge of the referenced term - (%predicate) '%target' !targetid. It should be imported later and linked in.", array('%term_name' => $term->name, '%predicate' => $predicate, '%target' => $target, '!targetid' => l($targetid, $targetid)), WATCHDOG_DEBUG);
                  $found_term_names[] = $target;
                }
              }
            } // if not already loaded
            else {
              // already know about it
              $found_term_names[] = l($terms[$targetid]->name . " (#" . $terms[$targetid]->tid . ")", 'taxonomy/term/'. $terms[$targetid]->tid);
            }
          } // each referred term

          watchdog('taxonomy_xml',
            '%predicate relations of !term_name are !targets',
            array(
              '!term_name' => l($term->name, 'taxonomy/term/'. $term->tid),
              '%predicate' => $predicate,
              '!targets' => implode(', ', $found_term_names),
            ),
            WATCHDOG_INFO
          );

        } // Is a 'relationship' sort of predicate.
        #else {
        #    watchdog('taxonomy_xml', "$predicate is not a ". implode(',', $relationship_predicates));
        #}
      } // each type of predicate
    } // has predicates

    watchdog('taxonomy_xml',  __FUNCTION__ . " Prepared the list of all required referree terms. Now establishing the actual links.", array(), WATCHDOG_INFO);

    // Go through all and add relationships
    // Note that a modification was made by flagging $term->taxonomy_xml_relinked = TRUE;
    //
    // The linking earlier may have given us some duplicates if the source had redundant info, so filter for uniques
    if (isset($term->predicates[TAXONOMY_XML_PARENT]) && is_array($term->predicates[TAXONOMY_XML_PARENT])) {
      // Link in parent
      foreach (array_unique($term->predicates[TAXONOMY_XML_PARENT]) as $key => $othertermname) {
        if ($othertermname) {
          // Here we try to find the referred term in the list of recently-made terms

          if (isset($terms[$othertermname])) {
            $parent = $terms[$othertermname];
            if ($parent && isset($parent->tid)) {

              // Due to possible data inconsistencies (input from Freebase)
              // We need to take care to avoid infinite ancestry loops
              // Which Drupal doesn't handle.
              // It prevents that in the UI, not at the data level
              // So need to scan the ancestry tree to make sure we don't add this term
              // as a descendant of itself
              $ancestors = taxonomy_xml_get_term_ancestors($parent->tid);
              if (in_array($term->tid, array_keys($ancestors))) {
                watchdog('taxonomy_xml',
                  "Not setting %name as a descendant of itself. Avoiding a potential infinite loop.",
                  array('%name' => $term->name),
                  WATCHDOG_WARNING
                );
                continue;
              }

              global $_taxonomy_xml_current_doc;
              drupal_set_message(t("!name # %tid is a child of !parent # %ptid (<a href='!source' style='font-size:x-small'>source</a>)", array('!name' => l($term->name, "taxonomy/term/{$term->tid}/edit"), '%tid' => $term->tid, '!parent' => l($parent->name, "taxonomy/term/{$parent->tid}/edit"), '%ptid' => $parent->tid, '!source' => $_taxonomy_xml_current_doc)));

              // SPECIALNESS D7.
              // When an unparented term placeholder was created earlier, it was given parent=0 (root) by default.
              // If that happened, and we NOW are setting a real parent - that old relationship
              // is to be anulled. This may not always be intentional - but is likely the case
              // and produces the structure we actually expect.
              if (isset($term->parent[0])) {
                unset($term->parent[0]);
              }
              
              $term->parent[$parent->tid] = $parent->tid;
            }
          }
          else {
            #drupal_set_message(t("Couldn't find the parent identified as %termname for %name # %tid", array('%termname' => $othertermname, '%name' => $term->name, '%tid' => $term->tid) ));
          }
        }
      }
      $term->taxonomy_xml_relinked = TRUE;
    }
    #else{drupal_set_message(" $name ". $term->tid ." has no parent term");}

    // 'related' or 'synonyms' is no longer supported in D7
    // @todo ressurect it with the fieldable terms

    if (! empty($term->synonyms_array)) {
      $term->synonyms = implode("\n", array_unique($term->synonyms_array) );
      $term->taxonomy_xml_relinked = TRUE;
    }
    #dpm(array('Saving' => $term));
    $term->taxonomy_xml_linked = TRUE;

    // for efficiency, only re-save terms that really need it
    if ( !empty($term->taxonomy_xml_relinked) ) {

      // It may be premature to save this term if we don't know its parent yet,
      // The system will default to parent=0, which causes bad structure later on
      if (! isset($term->parent)) {
        watchdog('taxonomy_xml', __FUNCTION__ . " About to save a term with no parent, this could be a problem later. <pre>!term</pre>", array('!term' => print_r($term, 1)), WATCHDOG_INFO);
      }

      taxonomy_term_save($term);
    }
  }
}

/**
 * Return a list of all terms in the ancestry chain of the given term.
 *
 * List list includes all parents from multi-inheritance.
 *
 * Use this to check before setting up a parental chain that may loop
 *
 * @return
 *   array  keyed by tid, but in no specific order.
 * @see taxonomy_get_parents_all()
 */
function taxonomy_xml_get_term_ancestors($tid) {
  $parents = taxonomy_get_parents_all($tid);
  // index by tid
  $ancestors = array();
  foreach ($parents as $parent) {
    $ancestors[$parent->tid] = $parent;
  }
  return $ancestors;
}


/**
 * Manage batch queues by dividing them into recursive 'rounds'.
 *
 * This is required because the current implementation of batch processing isn't
 * actually as atomic as it looks, and it's easy to hit max_allowed_packets just
 * by adding things to the queue.
 *
 * Given a placeholder term item, make a note that it should be retrieved and
 * analysed when possible.
 *
 * @param $term a template term object, must include at least a URI that
 * indicates where the rest of the information should be sourced from.
 *
 * If no $term is given, this function RETURNS the remaining queue so far, in
 * the form of a batch configuration object that can be batch_set().
 *
 * Retrieving the queue empties the static list held here, so it can only be
 * done once.
 *
 * The returned queue will include a rider at the end of the operations that
 * will trigger a recursion if it finds that new terms have been added to this
 * list in the most recent round.
 *
 * @ingroup batch_operations
 */
function taxonomy_xml_add_term_to_batch_queue($term = NULL) {
  if ($term) {
    // Terms are added to the list queued by their individual uris.
    // This means that if the same term is queued twice, (as with multi-parents)
    // it only gets processed once (per pageload)
    $_SESSION['taxonomy_xml_batch_queue'][$term->guid] = array('taxonomy_xml_import_from_url', array($term));
    watchdog('taxonomy_xml', "Batch Queued %term for import later...", array('%term' => $term->guid), WATCHDOG_DEBUG);

    // To avoid overruns, ensure that batches are not too big
    if (count($_SESSION['taxonomy_xml_batch_queue']) >= TAXONOMY_XML_MAX_BATCH_SIZE) {
      batch_set(taxonomy_xml_add_term_to_batch_queue());
    }

  }
  else {
    // Called with no arg,
    // this means we want to return the queue so far, and flush it from here.
    if ( ! empty($_SESSION['taxonomy_xml_batch_queue']) ) {
      // Prepare a batch config
      $batch_settings = array(
        'finished' => 'taxonomy_xml_batch_import_finished',
        'title' => t('Processing all queued import requests.'),
        'init_message' => t('Starting Batch Taxonomy Import.'),
        'progress_message' => t('Processed @current out of @total. (May require further recursion)', array()),
        'error_message' => t('Batch Taxonomy Import has encountered an error.'),
        'file' => drupal_get_path('module', 'taxonomy_xml') . '/taxonomy_xml.process.inc',
      );

      // Queue up our ops, and flush them from here.
      $batch_settings['operations'] = $_SESSION['taxonomy_xml_batch_queue'];
      unset($_SESSION['taxonomy_xml_batch_queue']);

      drupal_set_message(t("Retrieving the next batch queue. %operations_count operations in this batch . ", array('%operations_count' => count($batch_settings['operations']))) );

      // The last thing each round should do is queue up the next round. Add this callback to the operations.
      $batch_settings['operations']['final'] = array('taxonomy_xml_batch_requeue_more', array());

      return $batch_settings;
    }
    else {
      // If the queue is empty, return NULL so the caller won't get confused by an empty batch
      return NULL;
    }
  }
}


/**
 * Import data from one URL. Function used by the batch operation
 *
 * $param $term_placeholder A term object that is expected to be provided with
 * at least a guid to go fetch data from. May have other context info (like the
 * parent attribute) already set. This data should be merged onto that which is
 * absorbed.
 *
 * @ingroup  batch_operations
 */
function taxonomy_xml_import_from_url($term_placeholder, &$context) {
  // TODO need to pass the per-service context around better than variable_get
  dpm($context);
  $format = variable_get('taxonomy_xml_format', 'rdf');

  $text = taxonomy_xml_cached_get_contents($term_placeholder->guid);
  if (empty($text)) {
    drupal_set_message(__FUNCTION__ . ' ' . t('Retrieved no content from URL %url. Returning failure.', array('%url' => $term_placeholder->guid)), 'error');
    return FALSE;
  }
  #drupal_set_message(t('Retrieved Queued URL %url. Now processing it.', array('%url' => $term_placeholder->guid)));

  // taxonomy_xml_cached_get_contents() used content negotiation.
  // @todo maybe add rdf-alternative awareness.
  // If we got given XHTML+RDFa, maybe that's useful too

  $vid = isset($term_placeholder->vid) ? $term_placeholder->vid : variable_get('taxonomy_xml_vid', 0);

  // Conditionally include and invoke the appropriate format library
  taxonomy_xml_load_format($format);

  $funcname = "taxonomy_xml_${format}_parse";
  if ( function_exists($funcname) ) {
    $terms = $funcname($text, $vid, $term_placeholder->guid);
    // $terms is an array, as one URL may produce several terms,
    // It also contains all the known terms we referred to this round, not just the new one.
  }
  else {
    watchdog('taxonomy_xml', 'Error loading expected parse function %funcname . This is pretty bad and wholly unexpeceted. The library %format_format must be broken?', array('%funcname' => $funcname, '%format' => $format), WATCHDOG_ERROR );
  }
  #dpm(array('placeholder was' => $term_placeholder, 'terms are' => $terms));

  $context['message'] = "Imported from " . $term_placeholder->guid;
  if (! empty($terms[$term_placeholder->guid])) {
    $this_term = $terms[$term_placeholder->guid];
    $context['results'][$this_term->tid] = $this_term->name;
    $context['message'] .= "<br/>Result: " . $this_term->name;
  }

  return $terms;
}


/**
 * Batch callback action that should happen at the end of each round of
 * processing.
 *
 * The last thing that happens in a batch 'round' is the next round gets added
 * to the queue.
 *
 * @ingroup  batch_operations
 */
function taxonomy_xml_batch_requeue_more(&$context) {
  if ($new_jobs = taxonomy_xml_add_term_to_batch_queue()) {
    $context['message'] = t("Finished one round of imports, but the process found still more to do. Restarting to process a further %remaining_count items.", array('%remaining_count' => count($new_jobs['operations'])));
    batch_set($new_jobs);
    watchdog('taxonomy_xml', $context['message']);
  }
}


/**
 * Finalization callback when a round is done.
 *
 * @ingroup batch_operations
 */
function taxonomy_xml_batch_import_finished($success, $results, $operations) {
  if ($success) {
    $message = t("Completed a batch round. %count items processed.", array('%count' => count($results)));
    foreach ($results as $tid => $term_name) {
      $message .= " " . l( $term_name, 'taxonomy/term/' . $tid);
    }
  }
  else {
    // An error occurred.
    // $operations contains the operations that remained unprocessed.
    $error_operation = reset($operations);
    $message = 'An error occurred while processing ' . $error_operation[0] . ' with arguments :' . print_r($error_operation[1], TRUE);
    watchdog('taxonomy_xml', "Batch error " . print_r(array($success, $results, $operations), 1), array(), WATCHDOG_ERROR );
  }
  watchdog('taxonomy_xml', $message, array(), WATCHDOG_INFO);
  drupal_set_message($message);
  #dpm(func_get_args());
}


/**
 * Replace URL patterns containing placeholders for data values.
 *
 * Used when invoking GET URL services
 *
 * Given a pattern like http://example.com/lookup?id=!id&rank=!rank and array
 * ('rank' => 'Genus', 'id' => 55596) those values will be placed into the URL.
 * Note that we add '!' before doing the sub, to avoid incorrect placements,
 * otherwise we'd just use strtr()
 *
 * @ingroup taxonomy_xml_services
 */
function taxonomy_xml_sub_placeholders_into_pattern($pattern, $values) {
  $subs = array();
  foreach ($values as $var => $val) {
    $subs['!' . $var] = $val;
  }
  return strtr($pattern, $subs);
}

/**
 * Make a request on a remote taxonomy server and process the response
 *
 * Remote services may be slow or unavailable, so we need to allow for that.
 *
 * @param
 *   $service  an array describing properties of the chosen service
 * @param
 *   $values  as submitted from a form, settings to invoke the service with.
 *
 * @ingroup taxonomy_xml_services
 */
function taxonomy_xml_invoke_service_request($service, $values) {
  switch ( $service['protocol'] ) {
    case 'URI':
      // Before running, need to make sure the preferences on the form
      // are in line with those set for the service -
      // ie, the format is selected right. Maybe more later?
      $values['format'] = $service['format'];

      $req = taxonomy_xml_sub_placeholders_into_pattern($service['pattern'], $values);

      # Was just:
      # $text = taxonomy_xml_cached_get_contents($req);
      # taxonomy_xml_invoke_import($text, $values, $req);

      // Now need to break it into steps, to try and keep the times manageable.

      // Prepare a batch config
      $batch_settings = array(
        'title' => t('Invoking a request on taxonomy server %servicename.', array('%servicename' => $service['name'])),
        'operations' => array(),
        // The last operation will be to see if any more jobs were queued in the meantime.
        // unlimited batch recursion.
        'finished' => 'taxonomy_xml_batch_import_finished',
        'file' => drupal_get_path('module', 'taxonomy_xml') . '/taxonomy_xml.process.inc',

      );

      // Break down the steps.
      // #1, Fetch a reponse from the URL
      $batch_settings['operations'][] = array('taxonomy_xml_cached_get_contents', array($req));

      // #2 That would have cached the response, so next time we open that file will be quicker
      $batch_settings['operations'][] = array('taxonomy_xml_invoke_import_on_url', array($req, $values));

      // Ensure that any pending jobs in the queue get found and done.
      $batch_settings['operations']['final'] = array('taxonomy_xml_batch_requeue_more', array());

      batch_set($batch_settings);
      break;

    default:
      drupal_set_message(t("Taxonomy server protocol %protocol is not yet supported", array('%protocol' => $service['protocol'])), 'warning');
  }
}

/**
 * Returns a HANDLE on the current working list of terms.
 *
 * Basically behaving like a global, so we can cache and share the working list.
 *
 * Remember to fetch the list by handle, eg $terms =&
 * taxonoomy_xml_current_terms() if you are planning on modifying the list.
 */
function &taxonomy_xml_current_terms() {
  static $terms;
  if (! isset($terms)) {
    $terms = array();
  }
  return $terms;
}
