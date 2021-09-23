<?php

/**
 * @file
 * Extend Linkit with taxonomy links.
 */

/**
 * Build the default node query.
 */
function _linkit_taxonomy_create_base_query($profile, $result_tokens) {
  // Build default query
  $query = db_select('taxonomy_term_data', 't')
    ->fields('t', array('tid', 'name'))
    ->addTag('term_access');

  // Not all tokens are supported by Linkit.
  // Here is a list of valid tokens
  // [term:tid]
  // [term:name]
  // [term:vocabulary] (vid, name, [default : name])
  // [term:parent]
  if (isset($result_tokens['term'])) {
    foreach ($result_tokens['term'] AS $token => $token_value) {
      switch ($token) {
        case 'vocabulary':
          $query->addField('t', 'vid');
          break;
      }
    }

    // There can be tokens that is chained so we will not find them in the switch statement above.
    if (token_find_with_prefix($result_tokens['term'], 'vocabulary')) {
      $query->addField('t', 'vid');
    }
  }

  if ($profile->data['taxonomy']['group_by_vocabulary']) {
    $fields = $query->getFields();
    // We need the vid to sort on, if its not yet included in the query, we
    // have to inlcude it.
    if (!isset($fields['vid'])) {
      $query->addField('t', 'vid');
    }
    $query->orderBy('t.vid', 'ASC');
  }

  // Add the default sort.
  $query->orderBy('t.name', 'ASC');

  return $query;
}

/**
 * Build the group string for the term.
 */
function _linkit_taxonomy_build_group($profile, $term) {
  if ($profile->data['taxonomy']['group_by_vocabulary']) {
    $vocabulary = taxonomy_vocabulary_load($term->vid);
    return t('Terms (%vocabulary_name)', array('%vocabulary_name' => check_plain($vocabulary->name)));
  }
  else {
    return t('Terms');
  }
}

/**
 * The autocomplete callback function for the Linkit taxonomy plugin.
 */
function _linkit_taxonomy_autocomplete($string, $profile) {
  $matches = array();

  $result_description = check_plain($profile->data['taxonomy']['result_description']);

  // Build a list of all token-like patterns that appear in the text.
  $result_tokens = token_scan($result_description);

  $query = _linkit_taxonomy_create_base_query($profile, $result_tokens);
  $query->condition('t.name' , '%' . db_like($string) . '%' , 'LIKE')
        ->addTag('linkit_taxonomy_autocomplete');
  $result = $query->execute();

  foreach ($result as $term) {
    $uri = entity_uri('taxonomy_term', taxonomy_term_load($term->tid));
    $matches[] = array(
      'title' => $term->name,
      'description' => token_replace($result_description, array(
        'term' => $term,
      )),
      'path' => url($uri['path'], array('alias' => TRUE)),
      'group' => _linkit_taxonomy_build_group($profile, $term),
    );
  }
  return  $matches;
}

/**
 * The path info callback function for the Linkit taxonomy plugin.
 *
 * If the path given is a taxonomy term, then return information about that term.
 *
 * @see linkit.api.php
 */
function _linkit_taxonomy_path_info($path_info, $profile) {
  // Define the default taxonomy URI's first argument (arg(0)).
  $taxonomy_uri_array = array('taxonomy', 'forum');

  if (!isset($path_info['system_path'])) {
    return FALSE;
  }

  // Check if path is referring to a taxonomy
  if (in_array(arg(0, $path_info['system_path']), $taxonomy_uri_array)) {
    $result_description = check_plain($profile->data['taxonomy']['result_description']);

    // Build a list of all token-like patterns that appear in the text.
    $result_tokens = token_scan($result_description);

    // We need to know where the tid is.
    switch ($path_info['system_path']) {
      case 'taxonomy':
        // URI = taxonomy/term/[tid]
        $arg_i = 2;
        break;
      case 'forum':
        // URI = forum/[tid]
        $arg_i = 1;
        break;
    }

    // Build the base query.
    $query = _linkit_taxonomy_create_base_query($profile, $result_tokens);
    $query->condition('t.tid', arg($arg_i, $path_info['system_path']));
    $query_result = $query->execute()->fetch();

    if ($query_result) {
      $result = array(
        'title' => check_plain($query_result->name),
        'description' => token_replace($result_description, array(
          'term' => $query_result,
        )),
      );
      return $result;
    }

  }
  return FALSE;
}