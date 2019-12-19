<?php

/**
 * @file
 * Contains the theme settings form elements for the scripts extension.
 */

/**
 * Implements hook_extension_EXTENSION_theme_settings_form_alter().
 */
function omega_extension_assets_settings_form($element, &$form, $form_state) {
  $tokens = array(
    ':core' => '<code>:core</code><div class="description">' . t('Matches the root (core) modules, themes or misc folder.') . '</div>',
    ':contrib' => '<code>:contrib</code><div class="description">' . t('Matches all valid contrib module destinations (sites/all/modules, profiles/{profile}/modules, sites/{site}/modules).') . '</div>',
    ':current-theme' => '<code>:current-theme</code><div class="description">' . t('Matches the directory of the active (current) theme.') . '</div>',
    ':base-theme' => '<code>:base-theme</code><div class="description">' . t('Matches the directory of each base theme that belongs to the theme trail of the active theme.') . '</div>',
  );

  $element['omega_namespace_tokens'] = array(
    '#type' => 'fieldset',
    '#title' => t('Namespace tokens'),
  );

  $element['omega_namespace_tokens']['help'] = array(
    '#theme' => 'item_list',
    '#items' => $tokens,
    '#attributes' => array(
      'id' => 'omega-assets-namespace-tokens',
      'class' => 'omega-assets-namespace-tokens',
    ),
  );

  $element['omega_js_footer'] = array(
    '#type' => 'textarea',
    '#title' => t('Move JS files to the footer'),
    '#description' => t("Enter one file per line. The '*' character is a wildcard. Each path is relative to a module, theme, profile or theme engine (e.g. if you installed the Views module sites/all/modules the path 'views/*' would automatically be mapped to 'sites/all/modules/views/*') or the Drupal root if it can't be mapped to one of the aforementioned namespaces. Alternatively, you can use one of the provided <a href=\"#omega-namespace-tokens\">namespace tokens</a> as path prefixes."),
    // The paths are stored in an array.
    '#default_value' => implode("\n", (array) omega_theme_get_setting('omega_js_footer', array())),
  );

  $element['omega_css_exclude'] = array(
    '#type' => 'textarea',
    '#title' => t('Exclude CSS files'),
    '#description' => t("Enter one file per line. The '*' character is a wildcard. Each path is relative to a module, theme, profile or theme engine (e.g. if you installed the Views module sites/all/modules the path 'views/*' would automatically be mapped to 'sites/all/modules/views/*') or the Drupal root if it can't be mapped to one of the aforementioned namespaces. Alternatively, you can use one of the provided <a href=\"#omega-namespace-tokens\">namespace tokens</a> as path prefixes."),
    // The paths are stored in an array.
    '#default_value' => implode("\n", (array) omega_theme_get_setting('omega_css_exclude', array())),
  );

  $element['omega_js_exclude'] = array(
    '#type' => 'textarea',
    '#title' => t('Exclude JS files'),
    '#description' => t("Enter one file per line. The '*' character is a wildcard. Each path is relative to a module, theme, profile or theme engine (e.g. if you installed the Views module sites/all/modules the path 'views/*' would automatically be mapped to 'sites/all/modules/views/*') or the Drupal root if it can't be mapped to one of the aforementioned namespaces. Alternatively, you can use one of the provided <a href=\"#omega-namespace-tokens\">namespace tokens</a> as path prefixes."),
    // The paths are stored in an array.
    '#default_value' => implode("\n", (array) omega_theme_get_setting('omega_js_exclude', array())),
  );

  $element['omega_libraries'] = array(
    '#type' => 'container',
    '#tree' => TRUE,
  );

  $libraries = omega_theme_get_setting('omega_libraries', array());
  foreach (omega_theme_libraries_info() as $library => $info) {
    $key = strtolower($info['package']);
    if (!isset($element['omega_libraries'][$key])) {
      $element['omega_libraries'][$key] = array(
        '#type' => 'fieldset',
        '#title' => t(filter_xss_admin(ucfirst($info['package']))),
      );
    }

    $element['omega_libraries'][$key][$library]['status'] = array(
      '#type' => 'checkbox',
      '#title' => !isset($info['vendor']) ? $info['name'] : (isset($info['vendor url']) ? t('@library by <a href="!url">@vendor</a>', array(
        '@library' => $info['name'],
        '@vendor' => $info['vendor'],
        '!url' => $info['vendor url'],
      )) : t('@library by @vendor', array(
        '@library' => $info['name'],
        '@vendor' => $info['vendor'],
      ))),
      '#description' => $info['description'],
      '#default_value' => !empty($libraries[$library]['status']),
      '#parents' => array('omega_libraries', $library, 'status'),
    );

    if (!omega_library_path($library)) {
      $element['omega_libraries'][$key][$library]['missing'] = array(
        '#type' => 'item',
        '#title' => t('Missing library files'),
        '#markup' => t("The library's directory (@directory) could not be found.", array(
          '@directory' => "libraries/$library",
        )),
        '#prefix' => '<div class="omega-assets-missing-files">',
        '#suffix' => '</div>',
      );
    }

    $options = array();
    // Some libraries might want to provide special options.
    if (isset($info['options form']) && function_exists($info['options form'])) {
      // If a library options form callback exists and actually returns some
      // options for this library append conditionally show them underneath
      // the library checkbox with #states.
      $info['options form']($options, $form, $form_state, $library, $info);
    }

    if (!empty($info['variants'])) {
      $options['variant'] = array(
        '#type' => 'radios',
        '#title' => t('Variant'),
        '#options' => array('_default' => t('Default')),
        '#default_value' => !empty($libraries[$library]['variant']) ? $libraries[$library]['variant'] : '_default',
        '#parents' => array('omega_libraries', $library, 'variant'),
        '#weight' => -10,
      );

      $options['variant']['_default'] = array(
        '#description' => t('The default variant generally is a minified version of the library and provides the optimal production solution in most cases.'),
      );

      foreach ($info['variants'] as $variant => $variant_info) {
        $options['variant']['#options'][$variant] = $variant_info['name'];

        if (!empty($variant_info['description'])) {
          $options['variant'][$variant] = array(
            '#description' => $variant_info['description'],
          );
        }
      }
    }

    if (!empty($options)) {
      $element['omega_libraries'][$key][$library]['options'] = array(
        '#type' => 'fieldset',
        '#title' => t('Options'),
        '#states' => array(
          'visible' => array(
            ':input[name="omega_libraries[' . $library . '][status]"]' => array('checked' => TRUE),
          ),
        ),
        '#parents' => array('omega_libraries', $library),
        '#tree' => TRUE,
        '#attributes' => array('class' => array('omega-asset-options')),
        '#collapsible' => TRUE,
        '#collapsed' => TRUE,
      ) + $options;
    }
  }

  // We need a custom submit handler to store the CSS and JS paths as arrays.
  $form['#submit'][] = 'omega_extension_assets_theme_settings_form_submit';

  return $element;
}

/**
 * Theme settings form submit handler for handling the PIE selectors.
 */
function omega_library_pie_selectors_submit($form, &$form_state) {
  $values = &$form_state['values']['omega_libraries']['pie'];

  // Generate a CSS file for the provided selectors if the inclusion method is
  // set to 'pie.htc' or 'pie.php'.
  $selectors = array();
  if (!empty($values['selectors'])) {
    // Sanitize the selectors.
    $selectors = array_map('trim', explode("\n", $values['selectors']));
    $selectors = array_unique($selectors);
  }

  // Save the selectors as an array.
  $parents = array('#parents' => array(
    'omega_libraries',
    'pie',
    'selectors'
  ));

  form_set_value($parents, $selectors, $form_state);

  // Delete stale CSS selector files.
  $theme = $form_state['build_info']['args'][0];
  file_unmanaged_delete("public://omega/$theme/pie-selectors.css");
  file_unmanaged_delete("public://omega/$theme/pie-selectors.js");
}

/**
 * CSS3PIE Library options form callback.
 */
function omega_library_pie_options_form(&$element, &$form, $form_state, $library, $info) {
  // Give the default variant a proper label.
  $element['variant']['#options']['_default'] = t('HTML Component (default)');

  // Pull the selectors from the theme settings.
  $settings = omega_theme_get_setting('omega_libraries', array());

  // The generated CSS file is stored in the public file system.
  $theme = $form_state['build_info']['args'][0];
  $destination = file_create_url('public://omega/' . $theme);

  $element['selectors'] = array(
    '#title' => t('Selectors'),
    '#type' => 'textarea',
    '#description' => t('You can use this textarea to define all the CSS rules that you want to apply the PIE behavior to. Define one CSS selector per line. The selectors submitted with this field get directly cached in a <a href="!css">.css</a> file (as well as a <a href="!js">.js</a> file) in the public file system. This file is then included on page load and triggers CSS3PIE.', array(
      '!css' => $destination . '/pie-selectors.css',
      '!js' => $destination . '/pie-selectors.js',
    )),
    '#default_value' => !empty($settings['pie']['selectors']) ? implode("\n", $settings['pie']['selectors']) : '',
  );

  // We need to provide a submit handler to create a CSS file for the
  // defined selectors and remove them from the theme settings array.
  $form['#submit'][] = 'omega_library_pie_selectors_submit';
}

/**
 * Form submit handler for the css theme settings extension.
 */
function omega_extension_assets_theme_settings_form_submit($form, &$form_state) {
  $values = $form_state['values'];

  // Explode and trim the values for the exclusion rules.
  foreach (array('omega_css_exclude', 'omega_js_exclude', 'omega_js_footer') as $item) {
    $exclude = array_filter(array_map('trim', explode("\n", $values[$item])));
    form_set_value(array('#parents' => array($item)), $exclude, $form_state);
  }
}
