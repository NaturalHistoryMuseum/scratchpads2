<?php

/**
 * @file
 * Starter template.php file for subthemes of Omega.
 */

/*
 * Add any conditional stylesheets you will need for this sub-theme.
 *
 * To add stylesheets that ALWAYS need to be included, you should add them to
 * your .info file instead. Only use this section if you are including
 * stylesheets based on certain conditions.
 */

/**
 * Implements hook_theme().
 */
function beta_theme(&$existing, $type, $theme, $path) {
  $hooks = array();
  
  return $hooks;
}

function beta_css_alter(&$css) {
  //krumo($css);
}
