<?php

/**
 * @file
 * Contains a pre-process hook for 'block'.
 */

/**
 * Implements hook_preprocess_block().
 */
function omega_preprocess_block(&$variables) {
  // Add BEM style classes to blocks.
  if (!empty($variables['block_html_id'])) {
    $variables['attributes_array']['id'] = $variables['block_html_id'];
    $variables['attributes_array']['class'][] = preg_replace('/^block-/', 'block--', $variables['block_html_id']);
  }

  $css_module = drupal_clean_css_identifier($variables['block']->module);
  $variables['attributes_array']['class'] = preg_replace('/^block-' . $css_module . '$/', 'block--' . $css_module, $variables['attributes_array']['class']);
  $variables['attributes_array']['class'] = preg_replace('/^block-menu$/', 'block--menu', $variables['attributes_array']['class']);

  $variables['title_attributes_array']['class'][] = 'block__title';

  $variables['content_attributes_array']['class'][] = 'block__content';

  // Add template suggestions to appropriate blocks.
  switch ($variables['block']->module) {
    case 'system':
      switch ($variables['block']->delta) {
        case 'help':
        case 'powered-by':
          break;

        case 'main':
          // Use a template with no wrapper for the page's main content.
          $variables['theme_hook_suggestions'][] = 'block__minimal';
          break;

        default:
          // Any other "system" block is a menu block and should use
          // block--nav.tpl.php
          $variables['theme_hook_suggestions'][] = 'block__nav';
          break;
      }
      break;

    case 'menu':
    case 'menu_block':
      // Use block--nav.tpl.php template.
      $variables['theme_hook_suggestions'][] = 'block__nav';
      break;
  }

  // Add Aria Roles via attributes.
  switch ($variables['block']->module) {
    case 'system':
      switch ($variables['block']->delta) {
        case 'main':
          // Note: the "main" role goes in the page.tpl, not here.
          break;

        case 'help':
        case 'powered-by':
          $variables['attributes_array']['role'] = 'complementary';
          break;

        default:
          // Any other "system" block is a menu block.
          $variables['attributes_array']['role'] = 'navigation';
          break;
      }
      break;

    case 'menu':
    case 'menu_block':
    case 'blog':
    case 'book':
    case 'comment':
    case 'forum':
    case 'shortcut':
    case 'statistics':
      $variables['attributes_array']['role'] = 'navigation';
      break;

    case 'search':
      $variables['attributes_array']['role'] = 'search';
      break;

    case 'help':
    case 'aggregator':
    case 'locale':
    case 'poll':
    case 'profile':
      $variables['attributes_array']['role'] = 'complementary';
      break;

    case 'node':
      switch ($variables['block']->delta) {
        case 'syndicate':
          $variables['attributes_array']['role'] = 'complementary';
          break;

        case 'recent':
          $variables['attributes_array']['role'] = 'navigation';
          break;
      }
      break;

    case 'user':
      switch ($variables['block']->delta) {
        case 'login':
          $variables['attributes_array']['role'] = 'form';
          break;

        case 'new':
        case 'online':
          $variables['attributes_array']['role'] = 'complementary';
          break;
      }
      break;
  }
}
