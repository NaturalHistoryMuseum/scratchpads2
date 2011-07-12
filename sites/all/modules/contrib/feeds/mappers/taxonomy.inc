<?php

/**
 * @file
 * Mapper that exposes a node's taxonomy vocabularies as mapping targets.
 */

/**
 * Implementation of hook_feeds_parser_sources_alter().
 */
function taxonomy_feeds_parser_sources_alter(&$sources, $content_type) {
  if (!empty($content_type)) {
    foreach (taxonomy_get_vocabularies($content_type) as $vocabulary) {
      $sources['parent:taxonomy:'. taxonomy_vocabulary_id($vocabulary)] = array(
        'name' => t('Feed node: Taxonomy: @vocabulary', array('@vocabulary' => $vocabulary->name)),
        'description' => t('Taxonomy terms from feed node in given vocabulary.'),
        'callback' => 'taxonomy_feeds_get_source',
      );
    }
  }
}

/**
 * Callback, returns taxonomy from feed node.
 */
function taxonomy_feeds_get_source(FeedsImportBatch $batch, $key) {
  if ($node = node_load($batch->feed_nid)) {
    $terms = taxonomy_node_get_terms($node);
    $vocabulary = taxonomy_get_vocabulary(str_replace('parent:taxonomy:', '', $key));
    $result = array();
    foreach ($terms as $tid => $term) {
      if ($term->vid == $vocabulary->vid) {
        $result[] = new FeedsTermElement($term);
      }
    }
    return $result;
  }
}

/**
 * Implementation of hook_feeds_node_processor_targets_alter().
 *
 * @see FeedsNodeProcessor::getMappingTargets().
 */
function taxonomy_feeds_node_processor_targets_alter(&$targets, $content_type) {
  foreach (taxonomy_get_vocabularies($content_type) as $vocabulary) {
    $description = t('The @name vocabulary of the node. If this is a "Tags" vocabulary, any new terms will be created on import. Otherwise only existing terms will be used. If this is not a "Tags" vocabulary and not a "Multiple select" vocabulary, only the first available term will be created. See !settings.', array('@name' => $vocabulary->name, '!settings' => l(t('vocabulary settings'), 'admin/content/taxonomy/edit/vocabulary/'. $vocabulary->vid, array('query' => 'destination='. $_GET['q']))));

    $targets['taxonomy:'. taxonomy_vocabulary_id($vocabulary)] = array(
      'name' => t('Taxonomy: @name', array('@name' => $vocabulary->name)),
      'callback' => 'taxonomy_feeds_set_target',
      'description' => check_plain($description),
      'real_target' => 'taxonomy',
    );
  }
}

/**
 * Callback for mapping. Here is where the actual mapping happens.
 *
 * @param $node
 *   Reference to the node object we are working on.
 *
 * @param $key
 *   A key as added to the $targets array by
 *   taxonomy_feeds_node_processor_targets_alter().
 *
 * @param $terms
 *   Given terms as array.
 *
 * Add the given terms to the node object so the taxonomy module can add them
 * on node_save().
 */
function taxonomy_feeds_set_target(&$node, $key, $terms) {

  // Return if there are no terms.
  if (empty($terms)) {
    return;
  }

  // Load target vocabulary to check, if it has the "tags" flag.
  $vocabulary = taxonomy_get_vocabulary(str_replace('taxonomy:', '', $key));

  // Cast a given single string to an array so we can use it.
  if (!is_array($terms)) {
    $terms = array($terms);
  }

  if ($vocabulary->tags) {
    foreach ($terms as $k => $v) {
      // Make sure there aren't any terms with a comma (=tag delimiter) in it.
      $terms[$k] = preg_replace('/\s*,\s*/', ' ', $v);
    }
    // Simply add a comma separated list to the node for a "tags" vocabulary.
    $terms = array_merge($terms, drupal_explode_tags($node->taxonomy['tags'][$vocabulary->vid]));
    $node->taxonomy['tags'][$vocabulary->vid] = implode(',', $terms);
  }
  else {
    foreach ($terms as $term) {
      if ($term instanceof FeedsTermElement) {
        $node->taxonomy[$term->tid] = (object)$term;
      }
      // Check if a term already exists.
      elseif ($terms_found = taxonomy_get_term_by_name_vid($term, $vocabulary->vid)) {
        // If any terms are found add them to the node's taxonomy by found tid.
        foreach ($terms_found AS $term_found) {
          $node->taxonomy[$term_found->tid] = $term_found;
          if (!$vocabulary->multiple) {
            break;
          }
        }
      }
      // If the vocab is not for multiple tags break after the first hit.
      if (!$vocabulary->multiple) {
        break;
      }
    }
  }
}

/**
 * Try to map a string to an existing term by name and vocabulary id.
 *
 * Provides a case-insensitive and trimmed mapping, to maximize the likelihood
 * of a successful match limited by a vocabulary id.
 *
 * @param $name
 *   Name of the term to search for.
 *
 * @param $vid
 *   The vocabulary's ID.
 *
 * @return
 *   An array of matching term objects.
 */
function taxonomy_get_term_by_name_vid($name, $vid) {
  $db_result = db_query(db_rewrite_sql("SELECT t.tid, t.name FROM {term_data} t WHERE LOWER(t.name) = LOWER('%s') AND t.vid = %d", 't', 'tid'), trim($name), $vid);
  $result = array();
  while ($term = db_fetch_object($db_result)) {
    $result[] = $term;
  }
  return $result;
}

/**
 * Look up a vocabulary by vid or module name.
 *
 * @param $id
 *   A module name or a numeric vocabulary id.
 *
 * @return
 *   An object of type stdClass that represents a vocabulary.
 */
function taxonomy_get_vocabulary($id) {
  static $vocabularies;
  if (!isset($vocabularies[$id])) {
    foreach (taxonomy_get_vocabularies() as $vocabulary) {
      if ($vocabulary->vid == $id) {
        $vocabularies[$id] = $vocabulary;
        break;
      }
      elseif ($vocabulary->module == $id) {
        $vocabularies[$id] = $vocabulary;
        break;
      }
    }
  }
  return $vocabularies[$id];
}

/**
 * Return the vocabulary identifier, the vocabulary's vid or module.
 *
 * @return
 *   Vocabulary's module name if it is a features vocabulary (= exportable),
 *   vocabulary's vid otherwise.
 */
function taxonomy_vocabulary_id($vocabulary) {
  if (strpos($vocabulary->module, 'features_') === 0) {
    return $vocabulary->module;
  }
  return $vocabulary->vid;
}
