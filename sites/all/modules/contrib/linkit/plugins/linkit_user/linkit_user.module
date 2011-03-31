<?php 

/**
 * @file
 * Extend Linkit with user links.
 */

/**
 * Implements hook_linkit_load_plugins().
 */
function linkit_user_linkit_load_plugins($string) {
  $matches = array();

  // Get users
  $result = db_select('users', 'u')
    ->fields('u', array('uid', 'name'))
    ->condition('u.name', '%' . db_like($string) . '%', 'LIKE')
    ->condition('u.status' , '1')
    ->execute();
  foreach ($result AS $user) {
    $matches['user'][] = array(
      'title' => $user->name,
      'path' => 'user/' . $user->uid,
      'information' => array(
        'type' => 'User',
      ),
    );

  }

  return $matches;
}

/**
 * Implements hook_linkit_get_search_styled_link().
 */
function linkit_user_linkit_get_search_styled_link($string) {
  // Check to see that the link really is a user link
  // Backwards compatible with internal: links
  $escaped_string = str_replace('internal:', '', $string);
  $splitted_string = explode('/', $escaped_string);
  if ($splitted_string[0] != 'user') {
    return;
  }

  // This is a node link created with Linkit, try to grab the title and path now.
  $result = db_select('users', 'u')
    ->fields('u', array('name'))
    ->condition('u.uid', $splitted_string[1])
    ->execute()
    ->fetchObject();

  // No reault was found
  if (!$result) {
    return;
  }

  return check_plain($result->name) . ' [path:' . $escaped_string . ']';
}

/**
 * Implements hook_linkit_info_plugins().
 * 
 * This is used by linkit_permissions
 */
function linkit_user_linkit_info_plugins() {
  $return['linkit_user'] = array(
    'type' => 'user',
  );
  return $return;
}