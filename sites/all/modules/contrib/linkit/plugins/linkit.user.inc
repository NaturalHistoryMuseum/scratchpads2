<?php

/**
 * @file
 * Extend Linkit with user links.
 */

/**
 * Build the default node query.
 */
function _linkit_user_create_base_query($profile, $result_tokens) {
  // Get users
  $query = db_select('users', 'u')
    ->fields('u', array('uid', 'name'))
    ->condition('u.status' , '1');

  // Not all tokens are supported by Linkit.
  // Here is a list of valid tokens
  // [user:uid]
  // [user:name]
  // [user:mail]
  // [user:last-login] (short, medium, long, since, raw, custom, [default : medium])
  // [user:created] (short, medium, long, since, raw, custom, [default : medium])
  if (isset($result_tokens['user'])) {
    foreach ($result_tokens['user'] AS $token => $token_value) {
      switch ($token) {
        case 'mail':
          $query->addField('u', 'mail');
          break;

        case 'last-login':
          $query->addField('u', 'login');
          break;

        case 'created':
          $query->addField('u', 'created');
          break;
      }
    }

    // There can be tokens that is chained so we will not find them in the switch statement above.
    if (token_find_with_prefix($result_tokens['user'], 'last-login')) {
      $query->addField('u', 'login');
    }

    if (token_find_with_prefix($result_tokens['user'], 'created')) {
      $query->addField('u', 'created');
    }
  }

  // Add the default sort.
  $query->orderBy('u.name', 'ASC');

  return $query;
}

/**
 * The autocomplete callback function for the Linkit user plugin.
 */
function _linkit_user_autocomplete($string, $profile) {
  $matches = array();

  $result_description = check_plain($profile->data['user']['result_description']);

  // Build a list of all token-like patterns that appear in the text.
  $result_tokens = token_scan($result_description);

  $query = _linkit_user_create_base_query($profile, $result_tokens);
  $query ->condition('u.name', '%' . db_like($string) . '%', 'LIKE')
    ->addTag('linkit_user_autocomplete');
  $result = $query->execute();

  foreach ($result AS $user) {
    $matches[] = array(
      'title' => $user->name,
      'description' => token_replace($result_description, array(
        'user' => $user,
      )),
      'path' => url('user/' . $user->uid, array('alias' => TRUE)),
      'group' => t('Users'),
    );
  }

  return $matches;
}

/**
 * The path info callback function for the Linkit user plugin.
 *
 * If the path given is a user, then return information about that user.
 *
 * @see linkit.api.php
 */
function _linkit_user_path_info($path_info, $profile) {
  // Check if path is referring to a user
  if (isset($path_info['system_path']) && arg(0, $path_info['system_path']) == 'user' && is_numeric(arg(1, $path_info['system_path']))) {
    $result_description = check_plain($profile->data['user']['result_description']);

    // Build a list of all token-like patterns that appear in the text.
    $result_tokens = token_scan($result_description);

    // Build the base query.
    $query = _linkit_user_create_base_query($profile, $result_tokens);
    $query->condition('u.uid', arg(1, $path_info['system_path']));
    $query_result = $query->execute()->fetch();

    if ($query_result) {
      $result = array(
        'title' => check_plain($query_result->name),
        'description' => token_replace($result_description, array(
          'user' => $query_result,
        )),
      );
      return $result;
    }
  }
  return FALSE;
}