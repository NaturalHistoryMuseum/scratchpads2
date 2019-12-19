<?php

/**
 * Implements hook_process_page().
 */
function scratchpads_process_page(&$variables) {
  // You can use process hooks to modify the variables before they are passed to
  // the theme function or template file.
}

/**
 * Implements hook_process_region().
 */
function scratchpads_process_region(&$vars)
{
  $theme = alpha_get_theme();
  switch ($vars['elements']['#region']) {
    case 'content':
      if (isset($theme->page['page']['subtitle'])) {
        $vars['subtitle'] = $theme->page['page']['subtitle'];
      } else {
        $vars['subtitle'] = NULL;
      }
      if (isset($theme->page['page']['title'])) {
        $vars['title'] = $theme->page['page']['title'];
      }
      break;
  }
}
