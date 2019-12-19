<?php

/**
 * @file
 * Contains a pre-process hook for 'html'.
 */

/**
 * Implements hook_preprocess_html().
 */
function omega_preprocess_html(&$variables, $hook) {
  // Return early, so the maintenance page does not call any of the code below.
  if ($hook != 'html') {
    return;
  }

  // Add $grddl_profile as link-tag.
  drupal_add_html_head_link(array(
    'rel' => 'profile',
    'href' => $variables['grddl_profile'],
  ));

  // Serialize RDF Namespaces into an RDFa 1.1 prefix attribute.
  if ($variables['rdf_namespaces']) {
    $prefixes = array();
    foreach (explode("\n  ", ltrim($variables['rdf_namespaces'])) as $namespace) {
      // Remove xlmns: and ending quote and fix prefix formatting.
      $prefixes[] = str_replace('="', ': ', substr($namespace, 6, -1));
    }
    $variables['rdf_namespaces'] = ' prefix="' . implode(' ', $prefixes) . '"';
  }

  // Classes for body element. Allows advanced theming based on context
  // (home page, node of certain type, etc.).
  if (!$variables['is_front']) {
    // Add unique class for each page. The default language is used to generate
    // the path and class names for localized sites.
    $path = drupal_get_path_alias($_GET['q'], language_default()->language);

    // Add unique class for each section of the website.
    list($section) = explode('/', $path, 2);
    $arg = explode('/', $_GET['q']);

    if ($arg[0] == 'node' && isset($arg[1])) {
      if ($arg[1] == 'add') {
        $section = 'node-add';
      }
      elseif (isset($arg[2]) && is_numeric($arg[1]) && ($arg[2] == 'edit' || $arg[2] == 'delete')) {
        $section = 'node-' . $arg[2];
      }
    }

    $variables['attributes_array']['class'][] = drupal_html_class('section-' . $section);
  }

  // Remove the sidebar classes (@see template_preprocess_html()).
  $variables['classes_array'] = array_diff($variables['classes_array'], array(
    'two-sidebars',
    'one-sidebar sidebar-first',
    'one-sidebar sidebar-second',
    'no-sidebars',
  ));

  // Show a warning when the Omega theme is the active theme.
  if ($GLOBALS['theme'] == 'omega' && omega_theme_get_setting('omega_enable_warning', TRUE)) {
    drupal_set_message(t('Omega is a base theme and should never be used directly. Please <a href="!documentation">create a sub-theme</a> instead. You can disable this message on the <a href="!settings">theme settings page</a>.', array(
      '!documentation' => 'https://drupal.org/node/2060747',
      '!settings' => url('admin/appearance/settings/omega'),
    )), 'warning');
  }

  // Add Javascript to remove the no-js class from <html>.
  drupal_add_js(drupal_get_path('theme', 'omega') . '/js/no-js.js', array('group' => JS_LIBRARY, 'every_page' => TRUE, 'weight' => -1));
}
