<?php

/**
 * @file
 * Registers Omega layouts as Panels layouts.
 */

/**
 * Implementation of hook_LAYOUT_panels_layouts().
 */
function omega_omega_panels_layouts() {
  // We cannot use the 'path' property here and anywhere in our layouts declared
  // in omega_panels_get_sublayouts() because panels uses it for both, the icon
  // and the include file in panels_theme() which we don't want. It's something
  // that works for things like 'flexible' layouts but not when the child plugin
  // mechanism is only used for an alternative discovery logic as in this
  // case.
  return array(
    'title' => t('Omega'),
    'category' => t('Omega page layouts'),
    'description' => t('Omega page layouts to be used as Panels Everywhere site templates.'),
    'get child' => 'omega_panels_get_sublayout',
    'get children' => 'omega_panels_get_sublayouts',
    'theme' => 'omega_panels',
    'admin theme' => 'omega_panels_admin',
    'regions' => array(),
  );
}

/**
 * Callback to retrieve a single Omega layout.
 */
function omega_panels_get_sublayout($plugin, $layout, $sublayout) {
  $layouts = omega_panels_get_sublayouts($plugin, $layout);
  if (isset($layouts["$layout:$sublayout"])) {
    return $layouts["$layout:$sublayout"];
  }
}

/**
 * Callback to retrieve all Omega layouts.
 */
function omega_panels_get_sublayouts($plugin, $layout) {
  $path = drupal_get_path('theme', 'omega');
  require_once "$path/includes/omega.inc";

  $layouts = array();
  if ($items = omega_layouts_info('omega')) {
    foreach ($items as $name => $info) {
      // Compute the relative path to the actual layout.
      $relative = _omega_relative_path("$path/panels/layouts/omega", $info['path']);

      $layouts["$layout:$name"] = array(
        'name' => "$layout:$name",
        'title' => $info['info']['name'],
        'omega_layout' => $name,
      ) + $plugin;

      // Panels calls the preview image 'icon'.
      if (isset($info['info']['preview'])) {
        $layouts["$layout:$name"]['icon'] = "$relative{$info['info']['preview']}";
      }

      // Layouts may provide 'admin css' files.
      if (isset($info['info']['admin css'])) {
        $layouts["$layout:$name"]['admin css'] = "$relative{$info['info']['admin css']}";
      }

      // Various optional elements from the .info file of the layout.
      foreach (array('regions', 'category', 'description') as $key) {
        if (isset($info['info'][$key])) {
          $layouts["$layout:$name"][$key] = $info['info'][$key];
        }
      }
    }
  }
  return $layouts;
}

/**
 * Implements hook_preprocess_omega_panels_admin().
 */
function template_preprocess_omega_panels_admin(&$variables) {
  _omega_panels_layout_preprocess($variables);
}

/**
 * Theme function for rendering an Omega layout on the Panels admin UI.
 */
function theme_omega_panels_admin(&$variables) {
  // Clean up the theme hook suggestion so we don't end up in an infinite loop.
  unset($variables['theme_hook_suggestion'], $variables['theme_hook_suggestions']);

  $hook = str_replace('-', '_', $variables['omega_layout']['template']);
  return theme($hook, $variables);
}

/**
 * Theme function for rendering an Omega layout.
 */
function theme_omega_panels(&$variables) {
  $path = drupal_get_path('theme', 'omega');
  require_once "$path/template.php";

  return theme_omega_page_layout($variables);
}

/**
 * Implements hook_preprocess_omega_panels().
 */
function template_preprocess_omega_panels(&$variables) {
  $path = drupal_get_path('theme', 'omega');
  require_once "$path/includes/omega.inc";

  _omega_panels_layout_preprocess($variables);

  $layout = $variables['omega_layout'];
  $page = &$variables['page'];

  // Wrap every page item in a region template.
  foreach ($layout['info']['regions'] as $region => $name) {
    if (!empty($variables['page'][$region])) {
      $page[$region] = array(
        '#region' => $region,
        '#theme_wrappers' => array('region'),
        '#children' => $page[$region],
      );
    }
  }

  // Place dummy blocks in each region if the 'demo regions' setting is active
  // to force regions to be rendered.
  if (omega_extension_enabled('development') && omega_theme_get_setting('omega_demo_regions', TRUE) && user_access('administer site configuration')) {
    foreach ($layout['info']['regions'] as $region => $name) {
      if (empty($page[$region])) {
        $page[$region]['#theme_wrappers'] = array('region');
        $page[$region]['#region'] = $region;
      }

      $page[$region]['#name'] = $name;
      $page[$region]['#debug'] = TRUE;
    }
  }
}

/**
 * Helper function for preprocessing an Omega layout in Panels.
 */
function _omega_panels_layout_preprocess(&$variables) {
  $layouts = omega_layouts_info();
  $variables['omega_layout'] = $layouts[$variables['layout']['omega_layout']];

  // Place a reference to the 'content' array so that the variables are
  // accessible through both, the default page.tpl.php accessor and the panels
  // layout accessor.
  $variables['page'] = &$variables['content'];

  // Provide some harmless default variables that would otherwise be added by
  // template_preprocess_page().
  $variables['base_path'] = base_path();
  $variables['front_page'] = url();
  $variables['language'] = $GLOBALS['language'];
  $variables['language']->dir = $GLOBALS['language']->direction ? 'rtl' : 'ltr';

  // In order to not render any of these items we simply set them as an empty
  // string.
  $variables['feed_icons'] = '';
  $variables['logo'] = '';
  $variables['main_menu'] = '';
  $variables['secondary_menu'] = '';
  $variables['action_links'] = '';
  $variables['site_name'] = '';
  $variables['site_slogan'] = '';
  $variables['tabs'] = '';
  $variables['breadcrumb'] = '';
  $variables['title'] = '';
  $variables['messages'] = '';
}

/**
 * Computes the relative path between two absolute paths.
 *
 * @param string $from
 *   An absolute path.
 * @param string $to
 *   An absolute path.
 *
 * @return string
 *   The relative path between $from and $to.
 */
function _omega_relative_path($from, $to) {
  // Some compatibility fixes for Windows paths.
  $from = is_dir($from) ? rtrim($from, '\/') . '/' : $from;
  $to = is_dir($to) ? rtrim($to, '\/') . '/' : $to;
  $from = str_replace('\\', '/', $from);
  $to = str_replace('\\', '/', $to);
  $from = explode('/', $from);
  $to = explode('/', $to);
  $relative = $to;

  foreach ($from as $depth => $dir) {
    // Find first non-matching directory.
    if ($dir === $to[$depth]) {
      // Ignore this directory.
      array_shift($relative);
    }
    else {
      // Get number of remaining dirs to $from.
      $remaining = count($from) - $depth;
      if ($remaining > 1) {
        // Add traversals up to first matching directory.
        $padding = (count($relative) + $remaining - 1) * -1;
        $relative = array_pad($relative, $padding, '..');
        break;
      }
      else {
        $padding[0] = './' . $relative[0];
      }
    }
  }

  return implode('/', $relative);
}
