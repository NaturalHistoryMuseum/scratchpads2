<?php
/* double-commented to avoid conflict with svn
 */

/**
 * @file
 *   Include routines for the original XML parsing and taxonomy/term creation.
 * 
 * @TODO this format is largely deprecated as it's not at all portable, possibly
 * not even between Drupal versions.
 * As such, some features may be missing or untested.
 */

/**
 * sub-hook 
 * @see taxonomy_xml_HOOK_format_info()
 * 
 * Returns info about this syntax
 */
function taxonomy_xml_xml_format_info() {
  return array(
    'description' => "This format is Drupal-only. It closely matches the internal data structure, but is not portable outside of Drupal without work.",
  );  
}

/**
 * Return an XML representation of a taxonomy.
 *
 * @param $vid
 *   Which vocabulary to generate the tree for.
 *
 * @param $parent
 *   The term ID under which to generate the tree. If 0, generate the tree
 *   for the entire vocabulary.
 *
 * @param $depth
 *   Internal use only.
 *
 * @param $max_depth
 *   The number of levels of the tree to return. Leave NULL to return all levels.
 *
 * @return
 *   The text of an XML document.
 */
function taxonomy_xml_xml_create($vid, $parent = 0, $depth = -1, $max_depth = NULL) {
  $output = "<?xml version=\"1.0\" standalone=\"no\"?>\n";
  $output .= "<!DOCTYPE taxonomy SYSTEM \"taxonomy.dtd\">\n";
  $tree = taxonomy_get_tree($vid, $parent, $max_depth, $depth);
  if ($tree) {
    $vocabulary = taxonomy_vocabulary_load($vid);
    $output .= "<vocabulary>\n";
    // dump all entity properties as named XML tags
    foreach ($vocabulary as $key => $value) {
      if (is_array($value)) {
        #$output .= "<$key>" . check_plain(implode(',', $value)) . "</$key>";
      }
      else {
        $output .= "<$key>" . check_plain($value) . "</$key>";
      }
    }
    foreach ($tree as $term) {

      #$synonyms = taxonomy_get_synonyms($term->tid);
      $output .= "<term>";
      foreach ($term as $key => $value) {
        if ($key == 'parents') {
          foreach ($value as $parent) {
            $output .= "<parent>" . check_plain($parent) . "</parent>";
          }
        }
        else {
          $output .= "<$key>" . check_plain($value) . "</$key>";
        }
      }
      if (! empty($synonyms)) {
        $output .= "<synonyms>";
        $output .= implode("\n", $synonyms);
        $output .= "</synonyms>";
      }
      $output .= "</term>";
    }
    $output .= "</vocabulary>\n";
  }
  return $output;
}

/**
 * Call-back function used by the XML parser.
 */
function taxonomy_xml_element_start($parser, $name, $attributes) {
  global $_tx_term, $_tx_element, $_tx_tag;

  switch ($name) {
    case 'vocabulary':
      $_tx_element = $name;
      break;
    case 'term':
      $_tx_element = $name;
      $_tx_term += 1;
  }
  $_tx_tag = $name;
}

/**
 * Call-back function used by the XML parser.
 */
function taxonomy_xml_element_end($parser, $name) {
  global $_tx_element;

  switch ($name) {
    case 'vocabulary':
    case 'term':
      $_tx_element = '';
  }
}

/**
 * Call-back function used by the XML parser.
 */
function taxonomy_xml_element_data($parser, $data) {
  global $_tx_vocabulary, $_tx_element, $_tx_terms, $_tx_term, $_tx_tag;

  switch ($_tx_element) {
    case 'term':
      if ($_tx_tag == 'parent') {
        if (trim($data)) {
          $_tx_terms[$_tx_term][$_tx_tag][] = $data;
        }
      }
      else {
        @$_tx_terms[$_tx_term][$_tx_tag] .= trim($data);
      }
      break;
    default:
      $_tx_vocabulary[$_tx_tag] = isset($_tx_vocabulary[$_tx_tag]) ? $_tx_vocabulary[$_tx_tag] .= trim($data) : trim($data);
      // (PHP strict is boring for lazy coders.)
  }
}

/**
 * Initiate the parser on the custom XML schema.
 *
 * This uses the XML callback parser with tag callbacks.
 *
 * @param $data XML string
 *
 * @param $vid will either be an existing vid that the data is to be merged into
 * (only the data, not the vocab definition) or
 * TAXONOMY_XML_DETERMINED_BY_SOURCE_FILE - which means the vid is to be
 * determined from the source file. If so it may become modified by reference.
 *
 * @return an array of new/modified term objects.
 *
 * @see taxonomy_xml_element_start()
 * @see taxonomy_xml_element_end()
 * @see taxonomy_xml_element_data()
 *
 *
 * @todo ONLY EXPERIMENTALLY UPGRADED to D7 - no testing!
 */
function taxonomy_xml_xml_parse(&$data, &$vid = 0) {
  global $_tx_terms, $_tx_vocabulary;

  // Unset the global variables before we use them:
  unset($GLOBALS['_tx_element'], $GLOBALS['_tx_term'], $GLOBALS['_tx_tag']);
  $_tx_terms = array();
  $_tx_vocabulary = array();

  ////////////////////////
  // Parse the data:
  //

  $xml_parser = drupal_xml_parser_create($data);
  xml_parser_set_option($xml_parser, XML_OPTION_CASE_FOLDING, FALSE); //
  xml_set_element_handler($xml_parser, 'taxonomy_xml_element_start', 'taxonomy_xml_element_end');
  xml_set_character_data_handler($xml_parser, 'taxonomy_xml_element_data');

  if (!xml_parse($xml_parser, $data, 1)) {
    watchdog('taxonomy_xml', 'Failed to parse XML file: %error at line %line.', array('%error' => xml_error_string(xml_get_error_code($xml_parser)), '%line' => xml_get_current_line_number($xml_parser)),  WATCHDOG_ERROR);
    drupal_set_message(t('Failed to parse file: %error at line %line.', array('%error' => xml_error_string(xml_get_error_code($xml_parser)), '%line' => xml_get_current_line_number($xml_parser))), 'error');
  }
  xml_parser_free($xml_parser);

  ////////////////////////
  // Define the vocabulary
  //

  // If an existing vocabulary has been chosen or has the same name as the vocabulary being added,
  // terms should be added to the existing vocabulary. Otherwise a new vocabulary should be created.

  if ($vid == TAXONOMY_XML_DETERMINED_BY_SOURCE_FILE) {
    // Asked to define vocabulary details from file data.
    $vid = taxonomy_xml_setup_vocabulary_from_data($_tx_vocabulary);
  }
  else {
    // We have an existing, valid vid. Load the existing vocab, and go from there
  }

  // If I can't load from the vid, something previous has gone wrong
  // and everything else will fail (and add orphaned terms), so abort.
  $vocabulary = (array) taxonomy_vocabulary_load($vid);
  if (empty($vocabulary['vid'])) {
    drupal_set_message(t("Failed to initialize vocabulary to put terms into. Vocabulary:%vid is an invalid ID. Aborting import.", array('%vid' => $vid)), 'error');
    return FALSE;
  }

  ////////////////////////
  // Start loading terms
  //

  // These collections are just for auditing:
  $modified_terms = array(); // names
  $new_terms = array(); // objects
  $skipped_terms = array(); // names

  // $new_tid is a list mapping from old IDs (defined in the file)
  // to new IDs (because the database needs new numbers when creating things
  // - to avoid conflicts and overrwrites)
  $new_tid = array();

  // Get the maximum depth of terms
  $term_depth = array();
  foreach ($_tx_terms as $term) {
    $term_depth[] = $term['depth'];
  }

  // Import terms in order of depth, no matter what order in the file
  // This is because we require parents to exist before children.
  for ($i = 0; $i <= max($term_depth); $i++) {
    // Loop breadth-first.
    foreach ($_tx_terms as $term_data) {
      if ($term_data['depth'] != $i) {
        continue;
      }
      $term_data['vid'] = $vocabulary['vid'];
      $term_data['old_tid'] = $term_data['tid'];
      unset($term_data['tid']);

      // Calculate parent link-ups
      if (isset($term_data['parent'])) {
        foreach ((array) $term_data['parent'] as $key => $value) {
          if ($value) {
            if (! isset($new_tid[$value]) ) {
              drupal_set_message("Input claims that {$term_data['name']} has a parent {$value} but that term doesn't exist (yet). tricky.");
            }
            else {
              $term_data['parent'][$key] = $new_tid[$value];
            }
          }
        }
      }
      $term_exists = FALSE;
      if (! variable_get('taxonomy_xml_duplicate', 0)) {
        // Retrieve pre-existing terms - by name - if possible
        $existing_terms = taxonomy_get_term_by_name($term_data['name']);
        if (count($existing_terms) > 0) {
          foreach ($existing_terms as $existing_term) {
            if ($existing_term->vid == $term_data['vid']) {
              $term_exists = TRUE;
              // Map the term tid from the imported XML file to the tid in term_data database table
              $new_tid[$term_data['old_tid']] = $existing_term->tid;
              $modified_terms[$existing_term->tid] = $existing_term;
              $skipped_terms[$existing_term->tid] = $existing_term->name;
            }
          }
        }
      }
      // If the term doesn't already exist in this vocabulary, add it.
      if (!$term_exists) {
        $term = (object) $term_data;
        taxonomy_term_save($term);
        // Map the term tid from the imported XML file to the tid in term_data database table
        $new_tid[$term_data['old_tid']] = $term->tid;
        $new_terms[$term->tid] = $term->name;
        $modified_terms[$term->tid] = taxonomy_term_load($term->tid);
      }
    }
  }

  ////////////////////////
  // Summarize results

  $output = t('Vocabulary %name: ', array('%name' => $vocabulary['name'])) . '<br/>';
  if ($new_terms) {
    $output .= t('<b>Added</b> term(s) %terms. ', array('%terms' => implode(', ', $new_terms)));
  }
  else {
    $output .= t('No terms added. ');
  }
  if (!empty($skipped_terms)) {
    $output .= "<br/>" . t('<b>Ignored</b> duplicate term(s) %terms. ', array('%terms' => implode(', ', $skipped_terms)));
  }
  drupal_set_message($output);

  return $modified_terms;
}

/**
 * The setting is to initialize a vocabulary from the given settings (an array
 * of attributes).
 *
 * Depending on the state of pre-existing taxonomies, this will either create a
 * new vocab, or update an existing one.
 *
 * @param $edit
 *   array of vocabulary settings parsed from the data.
 *
 * @return the appropriate vocabulary id
 */
function taxonomy_xml_setup_vocabulary_from_data($edit) {
  // Massage/Parse vocabulary node types from csv value (old syntax) into the array we need
  if (!empty($edit['nodes']) && is_string($edit['nodes'])) {
    $node_types = explode(',', $edit['nodes']);
    if (!empty($node_types)) {
      $edit['nodes'] = array_combine($node_types, $node_types);
    }
  }

  // Potential conflict with vids?
  // See if the data does in fact define a vid
  if (isset($edit['vid'])) {
    $placeholder_vocabulary = (array) taxonomy_vocabulary_load($edit['vid']);

    // Does this vocab already exist?
    // Note that a failure to load a vocab does not return NULL, it returns an array with an actual empty item. Annoying
    if (empty($placeholder_vocabulary['vid'])) {
      drupal_set_message(t("
        Declared vocab ID:%data_vid does NOT yet exist.
        Need to make a new one, (cannot retain the internal IDs).",
        array('%data_vid' => $edit['vid']))
      );
      // Drupal database API will not allow me to override the VID when creating a new vocab.
      // Fair enough I guess. Make a brand new one.
      $placeholder_vocabulary = (array) _taxonomy_xml_get_vocabulary_placeholder($edit['name']);
      $vid = $placeholder_vocabulary['vid'];

      // Use other attributes defined in the source file (all except the vid)
      unset($edit['vid']);
      $placeholder_vocabulary = array_merge($placeholder_vocabulary, $edit);
      drupal_set_message(t("Applying imported settings onto vocabulary:%vid.", array('%vid' => $vid)) );
    }
    else {
      // TODO valid vocab found, but is it REALLY the same?
      drupal_set_message(t("
        Declared vocab ID:%data_vid DOES already exist.
        Terms will be imported there, but <a href='!settings_url'>existing vocabulary settings</a>
        will not be modified.",
        array(
          '%data_vid' => $edit['vid'],
          '!settings_url' => url('admin/content/taxonomy/edit/vocabulary/' . $edit['vid']),
        ))
      );
      // TODO sanity check to avoid inadvertant overwrites.
      // No change
      $vid = $placeholder_vocabulary['vid'];
    }
  }
  else {
    // Input did not define a vid. Make a new vocab with these new values
    // Should I try to match on *name* to find an earlier version?
    $placeholder_vocabulary = (array) _taxonomy_xml_get_vocabulary_placeholder($edit['name']);
    $vid = $placeholder_vocabulary['vid'];

    // Use other attributes defined in the source file (all except the vid)
    unset($edit['vid']);
    $placeholder_vocabulary = array_merge($placeholder_vocabulary, $edit);
    drupal_set_message(t("Applying imported settings onto vocabulary:%vid.", array('%vid' => $vid)) );
  }

  // Ensure the merged attributes are up to date.
  taxonomy_vocabulary_save($vocabulary /* TODO Vocabulary object replaces array $placeholder_vocabulary */);
  return $placeholder_vocabulary['vid'];
}
