<?php

/**
 * Implements hook_preprocess_region().
 */
function scratchpads_preprocess_region(&$vars)
{
  global $user;
  switch ($vars['elements']['#region']) {
    case 'secondary_menu':
      $links = array();
      if (module_exists('scratchpads_contact') && user_access('access site-wide contact form')) {
        $categories = scratchpads_contact_get_categories();
        foreach ($categories as $category) {
          $links[] = array(
            'href' => 'contact/' . $category->cid,
            'title' => $category->category
          );
        }
      }
      if (user_is_logged_in()) {
        $links[] = array(
          'href' => url('help.scratchpads.eu'),
          'title' => t("Help"),
          'attributes' => array(
            'target' => '_blank'
          )
        );
      }
      $vars['links'] = theme('links', array(
        'links' => $links
      ));
      break;
  }
}