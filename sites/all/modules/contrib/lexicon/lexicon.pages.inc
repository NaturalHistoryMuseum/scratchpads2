<?php
/**
 * @file
 * Page callbacks for the Lexicon module
 */

/**
 * Lexicon page callback function.
 */
function _lexicon_page($letter = NULL) {
  $found_vid = NULL;
  $curr_path = ($_GET['q']);
  $scroll_enabled = variable_get('lexicon_local_links_scroll', 0);

  $path = drupal_get_path('module', 'lexicon');

  // Add the JavaScript required for the scroll effect if enabled.
  if ($scroll_enabled == 1) {
    drupal_add_js($path . '/js/jquery.scrollTo-min.js');
    drupal_add_js($path . '/js/jquery.localscroll-min.js');
    drupal_add_js($path . '/js/lexicon.js');
  }

  $vids = variable_get('lexicon_vids', array());

  // Get the vocabulary-id for the vocabulary which the page callback is called
  // for by comparing the current path to the path that is configured for each
  // Lexicon.
  foreach ($vids as $vid) {
    $tmp_path = variable_get('lexicon_path_' . $vid, 'lexicon/' . $vid);
    if (strpos($curr_path, $tmp_path) !== FALSE) {
      $found_vid = $vid;
    }
  }

  // Check the argument and derive the letter from it if it is correct.
  if ($letter != NULL) {
    if (drupal_strlen($letter) != 8 || drupal_substr($letter, 0, 7) != 'letter_') {
      return MENU_NOT_FOUND;
    }
    else {
      $letter = drupal_substr($letter, 7, 1);
    }
  }

  $voc = taxonomy_vocabulary_load($found_vid);
  // Set the active menu to be "primary-links" to make the breadcrumb work.
  // By default the active menu would be "navigation", causing only
  // "Home" > $node->title to be shown.
  menu_set_active_menu_names('primary-links');

  return _lexicon_overview($voc, $letter);
}

/**
 * Lexicon overview function that creates all the data end renders the output
 * through the various theme templates.
 */
function _lexicon_overview($vocab, $letter = NULL) {
  $dest = drupal_get_destination();

  $vid = $vocab->vid;
  $path = variable_get('lexicon_path_' . $vid, 'lexicon/' . $vid);

  $current_let = '';

  $separate = variable_get('lexicon_separate_letters', FALSE);
  $page_per_letter = variable_get('lexicon_page_per_letter', FALSE);
  $show_description = variable_get('lexicon_show_description', FALSE);
  $link_to_term_page = variable_get('lexicon_link_to_term_page', FALSE);

  // If the Lexicon is configured to show one big list of terms, but there is a
  // letter in the argument, return 404.
  if (!$page_per_letter && $letter) {
    return MENU_NOT_FOUND;
  }

  // Set the title if the terms are displayed per letter instead of in one big
  // list if the Lexicon is configured to be split into multiple pages and there
  // is a letter argument.
  if ($page_per_letter && $letter) {
    drupal_set_title(t('@title beginning with @let', array(
      '@title' => variable_get('lexicon_title_' . $vid, $vocab->name),
      '@let' => drupal_strtoupper($letter),
    )));
  }

  // Load the entire vocabulary with entities.
  $tree = taxonomy_get_tree($vid, 0, NULL, TRUE);
  // Since the tree might not be sorted alphabetically sort it.
  uasort($tree, '_lexicon_tree_sort');

  $lexicon_alphabar = NULL;
  // If the overview is separated in sections per letter or the Lexicon is
  // displayed spread over multiple pages per letter create the alphabar.
  if ($separate || $page_per_letter) {
    $lexicon_alphabar = _lexicon_alphabar($vid, $tree);
  }

  $lexicon_overview_sections = array();
  $lexicon_introduction = NULL;

  // Check if the Lexicon is spread over multiple pages per letter and if a
  // letter argument is present.
  if (!$letter) {
    $introduction_text = variable_get('lexicon_introduction_' . $vid, NULL);
    // Display the introduction text if it is set in the configuration.
    if ($introduction_text['value'] != '') {
      $lexicon_introduction = t(check_markup($introduction_text['value'], $introduction_text['format']));
    }
  }
  if (!($page_per_letter && !$letter)) {
    $lexicon_overview_items = '';
    $lexicon_section = new stdClass();
    if ($tree) {
      $not_first = FALSE;
      // Build up the list by iterating through all terms within the vocabulary.
      foreach ($tree as $term) {
        // If terms should not be marked if a term has no description continue with the next term.
        if (!variable_get('lexicon_allow_no_description', FALSE) && empty($term->description)) {
          continue;
        }
        // If we're looking for a single letter, see if this is it.
        $term->let = drupal_strtolower(drupal_substr($term->name, 0, 1));
        // If there is no letter argument or the first letter of the term equals
        // the letter argument process the term.
        if ((!$letter) || $term->let == $letter) {
          // See if it's a new section.
          if ($term->let != $current_let) {
            if ($not_first) {
              if ($separate) {
                // Create the section output for the previous section.
                $lexicon_overview_sections[] = theme('lexicon_overview_section', array('lexicon_section' => $lexicon_section, 'lexicon_overview_items' => $lexicon_overview_items));
                // Clear the items to fill with the items of the new section.
                $lexicon_overview_items = '';
              }
            }
            if ($separate) {
              $lexicon_section->letter = $term->let;
              // Set the anchor id of the section used by the alphabar and
              // linked terms. The anchor is as meaningful as possible
              // ("letter"_$letter) for accessibility purposes.
              $lexicon_section->id = 'letter_' . $lexicon_section->letter;
            }
          }
          // Create the term output.
          $term_output = _lexicon_term_add_info($term);
          // Unset the description if it should not be shown.
          if (!$show_description) {
            unset($term_output->description);
            unset($term_output->safe_description);
          }
          if ($link_to_term_page) {
            $term_output->name = l($term_output->name, 'taxonomy/term/' . $term_output->tid);
            $term_output->safe_name = l($term_output->safe_name, 'taxonomy/term/' . $term_output->tid);
          }
          $lexicon_overview_items .= theme('lexicon_overview_item', array('term' => $term_output));
          // For future use
          // $lexicon_overview_items .= drupal_render(taxonomy_term_view($term_output, 'lexicon'));
          $current_let = $term->let;
          $not_first = TRUE;
        }
      }
      // Create a section without anchor and heading if the Lexicon is not
      // seperated into sections per letter or if there are no items to display.
      if (!$separate || $lexicon_overview_items == '') {
        $lexicon_section = NULL;
      }
      // Create the last section output.
      $lexicon_overview_sections[] = theme('lexicon_overview_section', array('lexicon_section' => $lexicon_section, 'lexicon_overview_items' => $lexicon_overview_items));
    }
  }

  $lexicon_overview = new stdClass();
  $lexicon_overview->voc_name = drupal_strtolower(_lexicon_create_valid_id($vocab->name));

  $lexicon_overview->description = filter_xss($vocab->description);
  $lexicon_overview->introduction = $lexicon_introduction;
  if ($separate && variable_get('lexicon_go_to_top_link', FALSE) == TRUE) {
    $lexicon_overview->go_to_top_link['name'] = t('Go to top');
    $lexicon_overview->go_to_top_link['fragment'] = variable_get('lexicon_go_to_top_link_fragment', 'top');
    $lexicon_overview->go_to_top_link['attributes'] = array(
      'class' => array('lexicon_go_to_top_link'),
    );
  }

  $output = array(
    'admin_links' => array(
      '#prefix' => '<div class="lexicon-admin-links">',
      '#links' => _lexicon_admin_links($vocab, $dest),
      '#theme' => 'links',
      '#suffix' => '</div>',
    ),
    'overview' => array(
      '#lexicon_overview' => $lexicon_overview,
      '#lexicon_alphabar' => $lexicon_alphabar,
      '#lexicon_overview_sections' => $lexicon_overview_sections,
      '#theme' => 'lexicon_overview',
    ),
  );

  return $output;
}

/**
 * Function that builds up the alphabar that is displayed at the top of the
 * Lexicon overview page.
 */
function _lexicon_alphabar($vid, &$tree) {
  $path = variable_get('lexicon_path_' . $vid, 'lexicon/' . $vid);
  $page_per_letter = variable_get('lexicon_page_per_letter', FALSE);

  if (variable_get('lexicon_suppress_unused', FALSE)) {
    // Just make it empty; it will be filled in below.
    $letters = array();
  }
  else {
    // Create the array of characters to use for the alphabar.
    $lets = array_merge(variable_get('lexicon_alphabet', range('a', 'z')), variable_get('lexicon_digits', range('0', '9')));
    $letters = drupal_map_assoc($lets);
  }

  // For each term in the vocabulary get the first letter and put it in the
  // array with the correct link.
  foreach ($tree as $key => $term) {
    // If terms should not be marked if a term has no description continue with the next term.
    if (!variable_get('lexicon_allow_no_description', FALSE) && empty($term->description)) {
      continue;
    }
    $term->let = drupal_strtolower(drupal_substr($term->name, 0, 1));
    // If the Lexicon is split up in seperate pages per letter the link must
    // refer to the appropriate page.
    if ($page_per_letter) {
      $letters[$term->let] = l($term->let, $path . '/letter_' . $term->let, array(
        'attributes' => array(
          'class' => array(
            'lexicon-item',
          ),
        ),
      ));
    }
    // If the Lexicon is displayed with all letters on one overview then the
    // link must refer to an anchor.
    else {
      $letters[$term->let] = l($term->let, $path, array(
        'fragment' => 'letter_' . $term->let,
        'attributes' => array(
          'class' => array(
            'lexicon-item',
          ),
        ),
      ));
    }
  }

  $lexicon_alphabar = new stdClass();
  $lexicon_alphabar->separator = ' ' . variable_get('lexicon_alphabar_separator', '|') . ' ';
  $lexicon_alphabar->instructions = check_plain(variable_get('lexicon_alphabar_instruction', _lexicon_alphabar_instruction_default()));
  $lexicon_alphabar->letters = $letters;

  return theme('lexicon_alphabar', array('lexicon_alphabar' => $lexicon_alphabar));
}

/**
 * Lexicon admin links function. Returns an array of admin links if the user has
 * the appropriate permissions.
 */
function _lexicon_admin_links($vocabulary, $destination) {
  $links = array();
  if (user_access('administer taxonomy')) {
    $links['lexicon_add_term'] = array(
      'title' => t('Add term'),
      'href' => 'admin/structure/taxonomy/' . $vocabulary->machine_name . '/add/term',
    );
    $links['lexicon_edit'] = array(
      'title' => t('Edit @name', array('@name' => drupal_strtolower(check_plain($vocabulary->name)))),
      'href' => 'admin/structure/taxonomy/' . $vocabulary->machine_name . '/edit',
      'query' => $destination,
    );
  }

  if (user_access('administer filters')) {
    $links['lexicon_admin'] = array(
      'title' => t('Lexicon settings'),
      'href' => 'admin/config/system/lexicon',
      'query' => $destination,
    );
  }

  return $links;
}

/**
 * Sort callback function to sort vocabulary trees alphabetically on term name.
 */
function _lexicon_tree_sort($a, $b) {
  if (drupal_strtolower($a->name) == drupal_strtolower($b->name)) {
    return 0;
  }
  else {
    if (drupal_strtolower($a->name) < drupal_strtolower($b->name)) {
      return -1;
    }
    else {
      return 1;
    }
  }
}
