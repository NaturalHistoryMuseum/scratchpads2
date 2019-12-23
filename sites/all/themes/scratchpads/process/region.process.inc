<?php

/**
 * Implements hook_process_region().
 */
function scratchpads_process_region(&$vars)
{
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
    case 'menu':

      $vars['main_menu'] = menu_main_menu();
      $vars['secondary_menu'] = menu_secondary_menu();
      break;
  }
}