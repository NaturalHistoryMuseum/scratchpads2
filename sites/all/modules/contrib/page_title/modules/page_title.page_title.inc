<?php

/**
 * @file
 * Page Title implementations of the page title hooks
 */


/**
 * Implementation of hook_page_title_alter().
 */
function page_title_page_title_alter(&$title) {
 // If nothing above set a title, give the legacy function a chance to act
  if (empty($title)) {
    $title = page_title_set_title();
  }

  // If we still have no title, fall back to the title provided by Drupal Core
  if (empty($title)) {
    $title = strip_tags(drupal_get_title());
  }
}


/**
 * Implementation of hook_page_title_pattern_alter().
 */
function page_title_page_title_pattern_alter(&$pattern, &$types) {
  // If frontpage, then use the frontpage pattern and set the title.
  if (drupal_is_front_page()) {
    // Get the frontpage pattern
    $settings = page_title_get_settings();
    $pattern = variable_get('page_title_front', $settings['page_title_front']['default']);
  }
}


/**
 * Implementation of hook_page_title_settings().
 */
function page_title_page_title_settings() {
  return array(
    'page_title_default' => array(
      'label' => 'Default',
      'scopes' => array('global'),
      'required' => TRUE,
      'show field' => FALSE,
      'description' => 'This pattern will be used as a <em>fallback</em> (ie, when no other pattern is defined)',
      'weight' => -50,
      'default' => '[current-page:page-title] | [site:name]',
    ),
    'page_title_front' => array(
      'label' => 'Frontpage',
      'scopes' => array('global'),
      'show field' => FALSE,
      'description' => 'This pattern will be used for the site frontpage',
      'weight' => -49,
      'default' => '[site:name] | [site:slogan]',
    ),
    'page_title_pager_pattern' => array(
      'label' => 'Pager Suffix',
      'scopes' => array('global'),
      'show field' => FALSE,
      'description' => 'This pattern will be appended to a page title for any given page with a pager on it',
      'weight' => -48,
    ),
  );
}
