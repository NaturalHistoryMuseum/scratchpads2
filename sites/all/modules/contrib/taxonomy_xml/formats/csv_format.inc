<?php
/* double-commented to avoid conflict with svn
 */

/**
 * @file
 *   Include  routines for CSV parsing and taxonomy/term creation.
 */

/**
 * Scan the input CSV file and create a taxonomy structure out of it.
 *
 * See the sample files for the expected format of the CSV
 *
 *
 * This scan process takes many rows of discrete 'statements' and combines them
 * into one interleaved description of many dependant terms. It does this in
 * three passes,
 * - The first to collect and enumerate the terms being used.
 * - The second to retrieve or create the terms.
 * - The third to link the dependancies together.
 *
 * The wording used in the source CSV may vary depending on your sources, add
 * extra terminology to the provided taxonomy_xml_relationship_synonyms()
 * function to adapt other words.
 */
function taxonomy_xml_csv_parse(&$data, $vid) {
  $output = '';

  // Unset the global variables before we use them:
  unset($GLOBALS['element'], $GLOBALS['term'], $GLOBALS['tag']);
  $terms         = array();
  $new_terms     = array();
  $skipped_terms = array();
  $vocabulary    = array();
  if ($vid) {
    $vocabulary = taxonomy_vocabulary_load($vid);
  }
  else {
    drupal_set_message(t('No vocab to import into. Either make one or choose one.'));
    return;
  }

  $inverses = array(
    TAXONOMY_XML_PARENT => TAXONOMY_XML_CHILD,
    TAXONOMY_XML_RELATED => TAXONOMY_XML_RELATED,
  );
  $inverses = array_merge($inverses, array_flip($inverses));

  $rows = explode("\n", $data);
  drupal_set_message(t('%rowcount rows of data', array('%rowcount' => count($rows))));

  // PHASE 1
  //
  // Enumerate all terms and their properties
  // This goes through all the input and sets up an array of placeholders for
  // the terms, before actually creating any.

  $predicate_synonyms = taxonomy_xml_relationship_synonyms();
  foreach ($rows as $row) {
    $triple = csv_string_to_array($row);
    if (count($triple) < 3) {
      # drupal_set_message("This line containes no triple: '$row'", 'error');
      continue;
    }
    $subject = trim($triple[0], '"');
    $predicate = $original_predicate = trim($triple[1], '"');
    $object = trim($triple[2], "\n\r\"");
    if (!$subject) {
      continue;
    }

    // Translate terminology synonyms to the real predicate, because the source data can be inconsistant
    if (isset($predicate_synonyms[$predicate])) {
      $predicate = $predicate_synonyms[$predicate];
    }

    unset($term); // As we are dealing with handles, be careful to avoid inadvertant re-use
    unset($other_term);
    $term = isset($terms[$subject]) ? $terms[$subject] : NULL;
    if (! $term) {
      // Start by looking for it
      $term = _taxonomy_xml_get_term_placeholder($subject, $vid);
      $terms[$subject] = &$term;
      // Created term placeholder, or have a handle on it
    }

    // Set its property as an array value. Allow duplicates, we will filter later
    if (! isset($term->predicates[$predicate]) || ! is_array($term->predicates[$predicate])) {
      $term->predicates[$predicate] = array();
    }
    $term->predicates[$predicate][] = $object;

    // Also set up reciprocal links with the opposite term.
    // We use reciprocals because we allow either broader or narrower terms, but don't require both.
    if (isset($inverses[$predicate])) {
      $inverse = $inverses[$predicate];
      // Ensure the other word exists. fetch it or make a placeholder
      $other_term = isset($terms[$object]) ? $terms[$object] : NULL;
      if (! $other_term) {
        $other_term = _taxonomy_xml_get_term_placeholder($object, $vid);
        $terms[$object] = &$other_term;
      }

      // Set the inverse property on it, referring back to the current subject.
      if (! isset($other_term->predicates[$inverse]) || ! is_array($other_term->predicates[$inverse])) {
        $other_term->predicates[$inverse] = array();
      }
      $other_term->predicates[$inverse][] = $subject;
    }
    else {
      // This predicate has no inverse, it's not a relationship, it's flat data
      switch ( $predicate ) {
        case TAXONOMY_XML_NAME:
          $term->name = $object;
          break;
        case TAXONOMY_XML_DESCRIPTION:
          // Multiple descriptions roll up into one big string.
          $term->description = $term->description ? $term->description . "\n" . $object : $object;
          break;
        case TAXONOMY_XML_HAS_SYNONYM:
          // This strong term also uses the weak one as a synonym
          $term->synonyms_array[] = $object;
          // Synonyms are just extra text labels
          break;
        case TAXONOMY_XML_SYNONYM_OF:
          // This weak term is just another word for the referred to one.
          // It's not really a full term. Do nothing now, tag the strong term later. It may not exist yet.
          break;
        default:
          drupal_set_message(("Not quite sure what '$original_predicate' ('$predicate') in '$row' means. You may add this term to the translation array in the module code to make it become useful."));
      }
    }
    $terms[$subject] = &$term;
  }
  unset($term);
  drupal_set_message(t("Processing statements about %count terms", array('%count' => count($terms)) ));

  # dpm(array('terms from data' => $terms));

  // Note the $terms array is all handles, not copies. Changes to them happen everywhere.


  // PHASE 2
  // Ordered all the input, go through and actually add terms to Drupal (if needed)
  //
  // Ensure a definition exists for them, Make one if needed, retrieve the id
  //
  foreach ($terms as $name => $term) {
    drupal_set_message(t("Processing term %name (%termname) %tid", array('%name' => $name, '%termname' => $term->name, '%tid' => isset($term->tid) ? $term->tid : 'new') ));
    if (! is_object($term)) {
      drupal_set_message("Having difficulty analyzing term info '$name':" . print_r($term, 1), 'error');
      // Bad data got this far. Ignore.
      continue;
    }
    $term->vid = $vocabulary->vid;

    // If the first pass was indexed on identifier, not name, we would not have retrieved it. Try again.
    if ($loaded_term = taxonomy_xml_get_term_by_name_from_vocab($term->name, $vid)) {
      // Found one by name this time, merge data with it and keep a handle on it
      foreach ($term as $att => $val) {
        $loaded_term->$att = $val;
      }
      $term = $loaded_term;
      $terms[$name] = $term;
    }

    if (empty($term->tid)) {
      if ((count($term->predicates) == 1) && isset($term->predicates[TAXONOMY_XML_SYNONYM_OF]) ) {
        // If a term was only listed to be a synonym, don't really make it.
        drupal_set_message(t("The term %name is just a synonym for %strong_term  - not a true term.", array('%name' => $term->name, '%strong_term' => print_r($term->predicates[TAXONOMY_XML_SYNONYM_OF], 1)) ));
        // Ensure the stronger term knows ...
        foreach ($term->predicates[TAXONOMY_XML_SYNONYM_OF] as $strong_term) {
          $terms[$strong_term]->synonyms_array[] = $term->name;
        }
        // And now it's attached to its stronger term, we can forget it.
        unset($terms[$name]);
      }
      else {
        // Make new term!
        #drupal_set_message(t("Did not find an existing entry for %termname - making a new one ", array('%termname' => $term->name) ));
        $term->synonyms = join("\n", array_unique((array) $term->synonyms_array) );
        taxonomy_term_save($term);
        // Even though $term was created and possibly modified by reference, it SHOULD still retain all the raw data we had it hold.
        // @todo unit test this
        $new_terms[] = $term->name;
      }
    }
    else {
      // Term already existed. Just make a note.
      $skipped_terms[] = $term->name;
    }
  }
  #dpm($terms);
  drupal_set_message(t('Created all %count needed terms, now linking them together.', array('%count' => count($terms))));

  // PHASE 3
  // Third time through, set the related terms and structure,
  // and save again

  taxonomy_xml_set_term_relations($terms);

  if ($new_terms) {
    drupal_set_message( t('Added term(s)') . ' <i>' . implode(', ', $new_terms) . '.</i> ' );
  }
  else {
    drupal_set_message( t('No new terms added.') );
  }
  if ($skipped_terms) {
    drupal_set_message( t('Did not need to re-create %skipped_count duplicate/existing term(s)', array('%skipped_count' => count($skipped_terms))) );
  }

  return $terms;
}

/**
 * Given a CSV string that may or may not contain quoted values,
 * Split it into an array of values.
 */
function csv_string_to_array($str) {
  if (drupal_substr($str, 0, 1) == '#' || drupal_substr($str, 0, 1) == ';') {
    return;
  }
  $expr = "/,(?=(?:[^\"]*\"[^\"]*\")*(?![^\"]*\"))/";
  $results = preg_split($expr, $str);
  $results = array_map('trim', $results);
  return preg_replace("/^\"(.*)\"$/", "$1", $results);
}
