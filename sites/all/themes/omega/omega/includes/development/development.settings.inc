<?php

/**
 * @file
 * Contains the theme settings form elements for the development extension.
 */

/**
 * Implements hook_extension_EXTENSION_theme_settings_form_alter().
 */
function omega_extension_development_settings_form($element, &$form, $form_state) {
  $element['omega_enable_export_reminder'] = array(
    '#type' => 'checkbox',
    '#title' => t('Show a reminder if the theme settings are not fully exported'),
    '#description' => t("You can export your theme settings with the Drush command <code>drush omega-export</code>. If you don't want to be reminded of this whenever your theme settings are not fully exported you can disable the warning message by unticking this checkbox."),
    '#default_value' => omega_theme_get_setting('omega_enable_export_reminder', TRUE),
    '#weight' => -20,
  );

  $element['omega_rebuild_theme_registry'] = array(
    '#type' => 'checkbox',
    '#title' => t('Rebuild the theme registry on every page load'),
    '#description' => t('While creating new templates and theme overrides the theme registry needs to be rebuilt. Note: This has a high (negative) performance impact and has to be deactivated once your website goes into production.'),
    '#default_value' => omega_theme_get_setting('omega_rebuild_theme_registry', FALSE),
  );

  $element['omega_rebuild_aggregates'] = array(
    '#type' => 'checkbox',
    '#title' => t('Rebuild CSS and JS aggregates on every page load'),
    '#description' => t('This can be useful for debugging your CSS or JS with activated aggregation. Note: This has a high (negative) performance impact and has to be deactivated once your website goes into production.'),
    '#default_value' => omega_theme_get_setting('omega_rebuild_aggregates', FALSE),
  );

  $element['omega_browser_width_indicator'] = array(
    '#type' => 'checkbox',
    '#title' => t('Browser width indicator'),
    '#description' => t('Adds a small box at the bottom of the browser window that displays the current width of the browser window. This can be very useful when writing media queries for a responsive website.'),
    '#default_value' => omega_theme_get_setting('omega_browser_width_indicator', FALSE),
  );

  $element['omega_livereload'] = array(
    '#type' => 'checkbox',
    '#title' => t('Enable LiveReload'),
    '#description' => t('<a href="http://livereload.com/">LiveReload</a> monitors changes in the file system. As soon as you save a file, it is preprocessed as needed,
      and the browser is refreshed. Even cooler, when you change a CSS file or an image, the browser is updated instantly without
      reloading the page. After enabling this features <a href="https://github.com/livereload/livereload-js">livereload.js</a> will
      be added to your pages which will then communicate with your LiveReload server.'),
    '#default_value' => omega_theme_get_setting('omega_livereload', TRUE),
  );

  $element['omega_livereload_settings'] = array(
    '#type' => 'fieldset',
    '#title' => t('LiveReload settings'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
    '#states' => array(
      'visible' => array(
        'input[name="omega_livereload"]' => array('checked' => TRUE),
      ),
    ),
  );

  $host = omega_theme_get_setting('omega_livereload_host', 'localhost');
  $port = omega_theme_get_setting('omega_livereload_port', '35729');
  $element['omega_livereload_settings']['omega_livereload_script'] = array(
    '#type' => 'textfield',
    '#title' => t('Script URL'),
    '#description' => t("The URL from which to load the script. When running LiveReload through Guard or Grunt the
      script can be retrieved from wherever Guard or Grunt are running."),
    '#default_value' => omega_theme_get_setting('omega_livereload_script', "http://$host:$port/livereload.js"),
    '#size' => 10,
  );

  $element['omega_livereload_settings']['omega_livereload_host'] = array(
    '#type' => 'textfield',
    '#title' => t('Host'),
    '#description' => t("The host at which the LiveReload server is running. Do not add any protocol prefixes (e.g. 'http://')."),
    '#default_value' => $host,
    '#size' => 30,
  );

  $element['omega_livereload_settings']['omega_livereload_port'] = array(
    '#type' => 'textfield',
    '#title' => t('Port'),
    '#description' => t('The port through which the LiveReload server can be reached. By default livereload.js tries to connect
      to port 35729 which is also the default for LiveReload server. Therefore, please only specify a custom port if the
      LiveReload server is running on a different port.'),
    '#default_value' => $port,
    '#size' => 10,
  );

  $regions = system_region_list($GLOBALS['theme_key'], REGIONS_VISIBLE);
  $regions = module_exists('dashboard') ? array_diff_key($regions, array_flip(dashboard_regions())) : $regions;
  if ($regions) {
    $element['omega_demo_regions'] = array(
      '#type' => 'checkbox',
      '#title' => t('Region demo mode'),
      '#description' => t('Forces all regions to be rendered, regardless of whether they are empty or not places a label with their name on them.'),
      '#default_value' => omega_theme_get_setting('omega_demo_regions', TRUE),
    );

    $element['omega_demo_regions_list'] = array(
      '#type' => 'fieldset',
      '#title' => t('Select demo regions'),
      '#collapsible' => TRUE,
      '#collapsed' => TRUE,
      '#states' => array(
        'visible' => array(
          'input[name="omega_demo_regions"]' => array('checked' => TRUE),
        ),
      ),
    );

    $element['omega_demo_regions_list']['omega_demo_regions_list'] = array(
      '#type' => 'checkboxes',
      '#title' => t('Demo regions'),
      '#options' => $regions,
      '#default_value' => omega_theme_get_setting('omega_demo_regions_list', array_keys($regions)),
    );
  }

  return $element;
}

