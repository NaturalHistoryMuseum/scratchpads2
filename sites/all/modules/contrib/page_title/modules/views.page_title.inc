<?php

/**
 * @file
 * Views implementations of the page title hooks
 */


/**
 * Implementation of hook_page_title_alter().
 */
function views_page_title_pattern_alter(&$pattern, &$types) {
  $menu = menu_get_item();

  if ($menu['page_callback'] == 'views_page') {
    // Get the args, name and display_id
    $args = $menu['page_arguments'];
    $name = array_shift($args);
    $display_id = array_shift($args);

    // Get the active page view
    $view = views_get_page_view();

    // If there is no view - then return - this helps protect from errors below
    if (!$view) return;

    // If there are args for this view, process to see if this argument has a SPECIFIC page title pattern
    if (!empty($args) && !empty($view->argument)) {
      // Grab the argument handlers
      $h = $view->argument;

      // Splice the arguments and get the key for the current arg.
      $h = array_shift(array_splice($h, count($args)-1, 1));

      // Get the Page Title Pattern from the options array for this handler
      $pattern = isset($h->options['page_title_pattern']) ? $h->options['page_title_pattern'] : $pattern;

      // If a page title pattern was found AND it contains a "%", assume there are placeholders and apply the %1, %2 type placeholder replacement.
      if (strpos($pattern, '%') !== FALSE) {
        // Grab the pre-built substitutions
        $subs = $view->build_info['substitutions'];

        // Apply any subs to the pattern
        $pattern = str_replace(array_keys($subs), $subs, $pattern);
      }
    }
    // This is a view with no args provided, or the specific arg has no title - lets use the base display title
    elseif ($view->display_handler->display->display_plugin == 'page_with_page_title') {
      $pattern = $view->display_handler->get_option('page_title_pattern');
    }
  }
}
