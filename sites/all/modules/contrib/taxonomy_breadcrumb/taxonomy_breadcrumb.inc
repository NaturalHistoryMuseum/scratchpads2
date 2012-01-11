<?php

/**
 * @file
 * helper functions for taxonomy_breadcrumb
 */


/**
 * Finds all terms associated with a node.
 * This is a D7 Replacement for Drupal 6 taxonomy_node_get_terms.
 */
function taxonomy_breadcrumb_node_get_terms($node, $key = 'tid') {
  static $terms;
  if (!isset($terms[$node->vid][$key])) {
    $query = db_select('taxonomy_index', 'r');
    $t_alias = $query->join('taxonomy_term_data', 't', 'r.tid = t.tid');
    $v_alias = $query->join('taxonomy_vocabulary', 'v', 't.vid = v.vid');
    $query->fields( $t_alias );
    $query->condition("r.nid", $node->nid);
    $result = $query->execute();
    $terms[$node->vid][$key] = array();
    foreach ($result as $term) {
      $terms[$node->vid][$key][$term->$key] = $term;
    }
  }
  return $terms[$node->vid][$key];
}

/**
 * Return lightest term for a given node.
 *
 * @param $node
 *   The node object.
 *
 * @ return
 *   The lightest term object associated with the node.
 */
function _taxonomy_breadcrumb_node_get_lightest_term($node) {
  $terms = taxonomy_breadcrumb_node_get_terms($node);
  if (!empty($terms)) {
    if (count($terms) > 1) {
      foreach ($terms as $term) {
        // Only consider terms in the lightest vocabulary.
        if (!isset($vid)) {
          $vid = $term->vid;
        }
        elseif ($term->vid != $vid) {
          continue;
        }
        // If the term has parents, the weight of the term is the weight of the lightest parent.
        $parents = taxonomy_get_parents_all($term->tid);
        $depth = count($parents);
        if ($depth > 0) {
          $parent = array_pop($parents);
          $weight = $parent->weight;
        }
        else {
          $weight = $term->weight;
        }
        if ((isset($lweight) && ($weight < $lweight)) || !isset($lweight)) {
          $lterm = $term;
          $lweight = $weight;
          $ldepth = $depth;
        }
        elseif (isset($lweight) && ($weight == $lweight)) {
          // If the node has multiple child terms with the same parent, choose the child with the greatest depth.
          if ($depth > $ldepth) {
            $lterm = $term;
            $ldepth = $depth;
          }
          elseif ($depth == $ldepth) {
            // If the terms have the same depth, pick the term with the lightest weight.
            $lterm = ($lterm->weight < $term->weight) ? $lterm : $term;
          }
        }
      }
      return $lterm;
    }
    else {
      return array_pop($terms);
    }
  }
}

/**
 * Return the administrator defined vocabulary path for a given vocabulary
 * ($vid).  If a path doesn't exist, NULL is returned.
 */
function _taxonomy_breadcrumb_get_vocabulary_path($vid) {
  $path = db_query("SELECT path FROM {taxonomy_breadcrumb_vocabulary} WHERE vid = :vid", array(':vid' => $vid))->fetchField();
  return $path;
}

/**
 * Return the administrator defined term path for a given term ($tid).
 * If a path doesn't exist, NULL is returned.
 */
function _taxonomy_breadcrumb_get_term_path($tid) {
  $path = db_query("SELECT path FROM {taxonomy_breadcrumb_term} WHERE tid = :tid", array(':tid' => $tid))->fetchField();
  return $path;
}

/**
 * If the current drupal path (q=) is /node/nid, generate the breadcrumb trail
 * based on nid.
 */
function _taxonomy_breadcrumb_generate_breadcrumb($tid, $is_term_page = FALSE) {
  $term = taxonomy_term_load($tid);

  // Generate the HOME breadcrumb.
  $home_text = variable_get('taxonomy_breadcrumb_home', t('Home'));
  if ($home_text != '') {
    $breadcrumb[] = l($home_text, NULL);
  }
  // Generate the VOCABULARY breadcrumb.
  $vocabulary_path = _taxonomy_breadcrumb_get_vocabulary_path($term->vid);
  if ($vocabulary_path != NULL) {
    $vocabulary = taxonomy_vocabulary_load($term->vid);
    $breadcrumb[] = l(_taxonomy_breadcrumb_tt("taxonomy:vocabulary:$term->tid:name", $vocabulary->name), $vocabulary_path);
  }

  // Generate the TERM breadcrumb.
  $parent_terms = array_reverse(taxonomy_get_parents_all($tid));
  foreach ($parent_terms as $parent_term) {
    $term_path = _taxonomy_breadcrumb_get_term_path($parent_term->tid);
    if ($term_path == NULL) {
      $uri =  taxonomy_term_uri($parent_term);
      $term_path = $uri['path'];
    }
    if ($term_path == '<none>') {
      continue;
    }

    $term_title = $parent_term->name;

    // Use the SYNONYM instead of TERM, if we want to.
    // if (variable_get('taxonomy_breadcrumb_use_synonym', FALSE)) {
    // TODO The taxonomy synonym functionality has been removed.
    // TODO Is there a way to do this?
    //  $synonyms = array() /*taxonomy_get_synonyms($parent_term->tid)*/;
    //  if (!empty($synonyms)) {
    //    $term_title = $synonyms[0];
    //  }
    // }
    // Do not create links to own self if we are on a taxonomy/term page.
    if ($is_term_page && $parent_term->tid == $tid) {
      $breadcrumb[] = check_plain(_taxonomy_breadcrumb_tt("taxonomy:term:$parent_term->tid:name", $term_title));
    }
    else {
      $breadcrumb[] = l(_taxonomy_breadcrumb_tt("taxonomy:term:$parent_term->tid:name", $term_title), $term_path);
    }
  }

  // Optionally remove the current TERM from end of breadcrumb trail.
  if (!variable_get('taxonomy_breadcrumb_show_current_term', TRUE) && !is_null($breadcrumb)) {
    array_pop($breadcrumb);
  }
  return $breadcrumb;
}

/**
 * Helper function for when i18ntaxonomy module is not installed.
 */
function _taxonomy_breadcrumb_tt($string_id, $default, $language = NULL) {
  return function_exists('tt') ? tt($string_id, $default, $language) : $default;
}
