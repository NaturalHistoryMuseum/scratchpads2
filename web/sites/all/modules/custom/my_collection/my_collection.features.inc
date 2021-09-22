<?php
/**
 * @file
 * my_collection.features.inc
 */

/**
 * Implements hook_views_api().
 */
function my_collection_views_api() {
  list($module, $api) = func_get_args();
  if ($module == "views" && $api == "views_default") {
    return array("version" => "3.0");
  }
}

/**
 * Implements hook_flag_default_flags().
 */
function my_collection_flag_default_flags() {
  $flags = array();
  // Exported flag: "My collection".
  $flags['collection'] = array(
    'content_type' => 'node',
    'title' => 'My collection',
    'global' => '0',
    'types' => array(
      0 => 'blog',
      1 => 'location',
      2 => 'specimen_observation',
      3 => 'media_gallery',
      4 => 'spm',
      5 => 'forum',
      6 => 'biblio',
      7 => 'page',
      8 => 'webform',
    ),
    'flag_short' => 'add to collection',
    'flag_long' => 'Add this item to your collection',
    'flag_message' => 'Item has been added to your collection',
    'unflag_short' => '[X]',
    'unflag_long' => 'Remove item from your collection',
    'unflag_message' => 'Item was removed from your collection',
    'unflag_denied_text' => '',
    'link_type' => 'toggle',
    'roles' => array(
      'flag' => array(
        0 => '2',
        1 => '3',
        2 => '4',
        3 => '5',
      ),
      'unflag' => array(
        0 => '2',
        1 => '3',
        2 => '4',
        3 => '5',
      ),
    ),
    'show_on_page' => 1,
    'show_on_teaser' => 1,
    'show_on_form' => 1,
    'access_author' => '',
    'i18n' => 0,
    'api_version' => 2,
    'module' => 'my_collection',
    'locked' => array(
      0 => 'name',
    ),
  );
  // Exported flag: "My collection".
  $flags['collections_term'] = array(
    'content_type' => 'taxonomy_term',
    'title' => 'My collection',
    'global' => '0',
    'types' => array(
      0 => '5',
      1 => '9',
      2 => '1',
      3 => '2',
      4 => '3',
      5 => '28',
    ),
    'flag_short' => 'add to collection',
    'flag_long' => 'Add this item to your collection',
    'flag_message' => 'This item has been added to your collection',
    'unflag_short' => '[X]',
    'unflag_long' => 'Remove this item from your collection',
    'unflag_message' => 'This post has item removed from your collection',
    'unflag_denied_text' => '',
    'link_type' => 'toggle',
    'roles' => array(
      'flag' => array(
        0 => '2',
      ),
      'unflag' => array(
        0 => '2',
      ),
    ),
    'show_on_term_page' => 1,
    'api_version' => 2,
    'module' => 'my_collection',
    'locked' => array(
      0 => 'name',
    ),
  );
  return $flags;
}
