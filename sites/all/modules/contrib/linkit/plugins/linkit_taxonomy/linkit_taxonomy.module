<?php 

/**
 * @file
 * Extend Linkit with taxonomy links.
 */


/**
 * Implements hook_linkit_load_plugins().
 */
function linkit_taxonomy_linkit_load_plugins($string) {
  $matches = array();

  $settings = variable_get('linkit_term', array());

  // Prevent "PHP notice: Undefined variable"
  _linkit_taxonomy_get_default_settings($settings);

  // Build default query
  $query = db_select('taxonomy_term_data', 't')
    ->fields('t', array('name', 'tid'))
    ->condition('t.name' , '%' . db_like($string) . '%' , 'LIKE')
    ->addTag('term_access');

  // Get terms
  $result = $query->execute();
  $i = 0;
  foreach ($result as $term) {
    $uri = entity_uri('taxonomy_term', taxonomy_term_load($term->tid));
    $matches['taxonomy'][$i] = array(
      'title' => $term->name,
      'path' => $uri['path'],
      'information' => array(
        'type' => 'Taxonomy',
      ),
    );
    // Add the taxonomy path
    if ($settings['display_settings']['show_parent']) {
      // Possible to find all parents to the root level, for now this isnt really pretty to put in
      // $parents = taxonomy_get_parents_all($term->tid);

      // The API says "Find all parents of a given term ID." but thats not true
      // It is only returning the very next parent
      $parents = taxonomy_get_parents($term->tid);
      
      if (count($parents)) {
        $current = current($parents);
        $matches['taxonomy'][$i]['information']['parent'] = $current->name;
      }
    } 
    $i++;
  }
  return  $matches;
}

/**
 * Implements hook_linkit_get_search_styled_link().
 */
function linkit_taxonomy_linkit_get_search_styled_link($string) {
  // Check to see that the link really is a term link
  // Backwards compatible with internal: links
  $escaped_string = str_replace('internal:', '', $string);
  $splitted_string = explode('/', $escaped_string);
  if ($splitted_string[0] != 'taxonomy' && $splitted_string[0] != 'forum') {
    return;
  }
  
  // This is a term link created with Linkit, try to grab the title and path now.
  $result = db_select('taxonomy_term_data', 't')
    ->fields('t', array('name'))
    ->condition('t.tid', $splitted_string[count($splitted_string)-1])
    ->addTag('term_access')
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
function linkit_taxonomy_linkit_info_plugins() {
  $return['linkit_taxonomy'] = array(
    'type' => 'taxonomy',
  );
  return $return;
}

/**
 * Prevent "PHP notice: Undefined variable" by merging the settings 
 * with the defule values
 */
function _linkit_taxonomy_get_default_settings(&$settings) {
  // Merge settings with the default settings
  $settings += array(
    'display_settings' => array(
      'show_parent' => 0,
    ), 
  );
}