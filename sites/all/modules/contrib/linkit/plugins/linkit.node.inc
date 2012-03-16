<?php

/**
 * @file
 * Extend Linkit with node links.
 */

/**
 * Build the default node query.
 */
function _linkit_node_create_base_query($profile, $result_tokens) {
  // Build default query
  $query = db_select('node', 'n')
    ->fields('n', array('nid', 'title'))
    ->addTag('node_access');

  // Not all tokens are supported by Linkit.
  // Here is a list of valid tokens
  // [node:nid]
  // [node:vid]
  // [node:tnid]
  // [node:type]
  // [node:type-name]
  // [node:title]
  // [node:language]
  // [node:created] (short, medium, long, since, raw, custom, [default : medium])
  // [node:changed] (short, medium, long, since, raw, custom, [default : medium])
  // [node:author]  (Can use the user tokens, [default : name])

  if (isset($result_tokens['node'])) {
    foreach ($result_tokens['node'] as $token => $token_value) {
      switch ($token) {
        case 'vid':
          $query->addField('n', 'vid');
          break;
        case 'tnid':
          $query->addField('n', 'tnid');
          break;
        case 'type':
        case 'type-name';
          $query->addField('n', 'type');
          break;
        case 'language':
          $query->addField('n', 'language');
          break;
        case 'author':
          $query->addField('n', 'uid');
          $query->addField('u', 'name', 'name');
          $query->leftJoin('users', 'u', 'u.uid = n.uid');
          break;
        case 'created':
          $query->addField('n', 'created');
          break;
        case 'changed':
          $query->addField('n', 'changed');
          break;
      }
    }

    // This tokens can be chained so we cant use them in the foreach loop above.
    if (token_find_with_prefix($result_tokens['node'], 'author')) {
      $query->addField('n', 'uid');
    }

    if (token_find_with_prefix($result_tokens['node'], 'created')) {
      $query->addField('n', 'created');
    }

    if (token_find_with_prefix($result_tokens['node'], 'changed')) {
      $query->addField('n', 'changed');
    }
  }

  // Content type check
  if ($allowed_content_types = array_filter($profile->data['node']['content_types'])) {
    $query->condition('n.type', $allowed_content_types, 'IN');
  }

  // Status condition.
  if (!isset($profile->data['node']['include_unpublished'])) {
    $query->condition('n.status', 1);
  }

  if ($profile->data['node']['group_by_content_type']) {
    $fields = $query->getFields();
    // We need the type to sort on, if its not yet included in the query, we
    // have to inlcude it.
    if (!isset($fields['type'])) {
      $query->addField('n', 'type');
    }
    $query->orderBy('n.type', 'ASC');
  }

  // Add the default sort.
  $query->orderBy('n.title', 'ASC');

  return $query;
}

/**
 * Build the group string for the node.
 */
function _linkit_node_build_group($profile, $node) {
  $group = t('Content');
  if ($profile->data['node']['group_by_content_type']) {
    $type_name = node_type_get_name($node);
    $group .= ' · ' . check_plain($type_name);
  }
  return $group;
}

/**
 * The autocomplete callback function for the Linkit node plugin.
 */
function _linkit_node_autocomplete($string, $profile) {
  $matches = array();

  $result_description = check_plain($profile->data['node']['result_description']);

  // Build a list of all token-like patterns that appear in the text.
  $result_tokens = token_scan($result_description);

  // Build the base query.
  $query = _linkit_node_create_base_query($profile, $result_tokens);
  $query->condition('n.title', '%' . db_like($string) . '%', 'LIKE')
        ->addTag('linkit_node_autocomplete');
  $result = $query->execute();

  foreach ($result as $node) {
    $matches[] = array(
      'title' => $node->title,
      'description' => token_replace($result_description, array(
        'node' => $node,
      )),
      // We have to set alias to TRUE as we don't want an alias back.
      'path' => url('node/' . $node->nid, array('alias' => TRUE)),
      'group' => _linkit_node_build_group($profile, $node),
    );
  }

  return $matches;
}

/**
 * The path info callback function for the Linkit node plugin.
 *
 * If the path given is a node item, then return information about that node.
 *
 * @see linkit.api.php
 */
function _linkit_node_path_info($path_info, $profile) {
  // Check if path is referring to a node
  if (isset($path_info['system_path']) && arg(0, $path_info['system_path']) == 'node' && is_numeric(arg(1, $path_info['system_path']))) {

    $result_description = check_plain($profile->data['node']['result_description']);

    // Build a list of all token-like patterns that appear in the text.
    $result_tokens = token_scan($result_description);

    // Build the base query.
    $query = _linkit_node_create_base_query($profile, $result_tokens);
    $query->condition('n.nid', arg(1, $path_info['system_path']));
    $query_result = $query->execute()->fetch();

    if ($query_result) {
      $result = array(
        'title' => check_plain($query_result->title),
        'description' => token_replace($result_description, array(
          'node' => $query_result,
        )),
      );
      return $result;
    }
  }
  return FALSE;
}
