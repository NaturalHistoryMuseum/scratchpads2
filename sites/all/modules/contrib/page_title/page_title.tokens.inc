<?php

/**
 * @file
 * Builds placeholder replacement tokens for page titles.
 */

/**
 * Implements hook_token_info().
 */
function page_title_token_info() {
  $info['tokens']['current-page']['page-title'] = array(
    'name' => t('Page Title'),
    'description' => t('The title of the current page, as processed by the Page Title module.'),
  );

  return $info;
}

/**
 * Implements hook_token_values().
 */
function page_title_tokens($type, $tokens, array $data = array(), array $options = array()) {
  $replacements = array();
  $sanitize = !empty($options['sanitize']);

  if ($type == 'current-page') {
    foreach ($tokens as $name => $original) {
      switch ($name) {
        case 'page-title':
          $replacements[$original] = page_title_get_title(!$sanitize);
          break;
      }
    }
  }

  return $replacements;
}
