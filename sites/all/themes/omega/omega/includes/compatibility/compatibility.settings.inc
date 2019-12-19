<?php

/**
 * @file
 * Contains the theme settings form elements for the compatibility extension.
 */

/**
 * Implements hook_extension_EXTENSION_theme_settings_form_alter().
 */
function omega_extension_compatibility_settings_form($element, &$form, $form_state) {
  $element['omega_conditional_classes_html'] = array(
    '#type' => 'checkbox',
    '#title' => t('Add conditional classes for Internet Explorer'),
    '#description' => t('Adds conditional classes (for Internet Explorer) to the &lt;html&gt;.'),
    '#default_value' => omega_theme_get_setting('omega_conditional_classes_html', TRUE),
  );

  $element['omega_metatags'] = array(
    '#type' => 'fieldset',
    '#title' => t('Metatags and HTTP headers'),
  );

  $element['omega_metatags']['omega_apple_touch'] = array(
    '#type' => 'checkbox',
    '#title' => t('Apple touch icons'),
    '#description' => t('You can set an Apple Touch Icon for your website just like you can set a favorite icon.'),
    '#default_value' => omega_theme_get_setting('omega_apple_touch', TRUE),
  );

  $element['omega_metatags']['omega_cleartype'] = array(
    '#type' => 'checkbox',
    '#title' => t('Cleartype support for Windows'),
    '#description' => t('Mobile Internet Explorer allows us to activate ClearType technology for smoothing fonts for easy reading.'),
    '#default_value' => omega_theme_get_setting('omega_cleartype', TRUE),
  );

  $element['omega_metatags']['omega_handheld_friendly'] = array(
    '#type' => 'checkbox',
    '#title' => t('Handheld friendly'),
    '#description' => t('The HandheldFriendly meta-tag was used initially by older Palm and Blackberry models as well as browsers like AvantGo.'),
    '#default_value' => omega_theme_get_setting('omega_handheld_friendly', TRUE),
  );

  $element['omega_metatags']['omega_mobile_optimized'] = array(
    '#type' => 'checkbox',
    '#title' => t('Mobile Optimized'),
    '#description' => t('Microsoft introduced the MobileOptimized tag for the PocketPC.'),
    '#default_value' => omega_theme_get_setting('omega_mobile_optimized', TRUE),
  );

  $element['omega_metatags']['omega_viewport'] = array(
    '#type' => 'checkbox',
    '#title' => t('Viewport'),
    '#description' => t('This is more widely supported by modern smartphones, including but not limited to: iOS, Android, Palm Pre, Blackberry, Windows Phone 7.'),
    '#default_value' => omega_theme_get_setting('omega_viewport', TRUE),
  );

  $element['omega_metatags']['viewport_settings'] = array(
    '#type' => 'fieldset',
    '#title' => t('Viewport settings'),
    '#states' => array(
      'visible' => array(
        ':input[name="omega_viewport"]' => array('checked' => TRUE),
      ),
    ),
  );

  $options = array('_default' => t('Default'));
  foreach (range(0.1, 10, 0.1) as $value) {
    // Format the value to display with one decimal.
    $value = number_format($value, 1);
    $options[(string) $value] = $value;
  }

  $element['omega_metatags']['viewport_settings']['omega_viewport_initial_scale'] = array(
    '#type' => 'select',
    '#title' => t('Initial scale'),
    '#default_value' => omega_theme_get_setting('omega_viewport_initial_scale', '_default'),
    '#options' => $options,
    '#description' => t('The initial scaling of the page. Has to be in the range of the minimum- and maximum scale options.'),
  );

  $element['omega_metatags']['viewport_settings']['omega_viewport_user_scaleable'] = array(
    '#type' => 'checkbox',
    '#title' => t('User scalable'),
    '#default_value' => omega_theme_get_setting('omega_viewport_user_scaleable', TRUE),
    '#description' => t("Whether users are allowed to zoom in and out. Defaults to 'yes'. This is usually accomplished via multi-touch gestures on iOS and Android devices. You should only disallow zooming (uncheck this checkbox) if your mobile theme is very customized and presented with good typography and graphics for a reduced mobile size."),
  );

  $element['omega_metatags']['viewport_settings']['scaling_settings'] = array(
    '#type' => 'fieldset',
    '#title' => t('Scaling settings'),
    '#states' => array(
      'visible' => array(
        ':input[name="omega_viewport_user_scaleable"]' => array('checked' => TRUE),
      ),
    ),
  );

  $element['omega_metatags']['viewport_settings']['scaling_settings']['omega_viewport_minimum_scale'] = array(
    '#type' => 'select',
    '#title' => t('Minimum scale'),
    '#default_value' => omega_theme_get_setting('omega_viewport_minimum_scale', '_default'),
    '#options' => $options,
    '#description' => t('The minimum allowed scaling of the site.'),
  );

  $element['omega_metatags']['viewport_settings']['scaling_settings']['omega_viewport_maximum_scale'] = array(
    '#type' => 'select',
    '#title' => t('Maximum scale'),
    '#default_value' => omega_theme_get_setting('omega_viewport_maximum_scale', '_default'),
    '#options' => $options,
    '#description' => t('The maximum allowed scaling of the site.'),
  );

  $element['omega_metatags']['omega_chrome_edge'] = array(
    '#type' => 'checkbox',
    '#title' => t('Enable Chrome Frame and Internet Explorer Edge support'),
    '#description' => t('Google\'s Chrome Frame is an open source project for Internet Explorer 6, 7, 8, and 9 that allows those version of Internet Explorer to <a href="!link target="_blank">harness the power of Google Chrome\'s engine</a>. Edge mode tells Internet Explorer to display content in the highest mode available. With Internet Explorer 9, this is equivalent to IE9 mode. If a (hypothetical) future release of Internet Explorer supported a higher compatibility mode, pages set to edge mode would appear in the highest mode supported by that version. Those same pages would still appear in IE9 mode when viewed with Internet Explorer 9.', array('!link' => 'https://www.youtube.com/watch?v=sjW0Bchdj-w&feature=player_embedded"')),
    '#default_value' => omega_theme_get_setting('omega_chrome_edge', TRUE),
  );

  $element['omega_metatags']['chrome'] = array(
    '#type' => 'fieldset',
    '#title' => t('Google Chrome Frame settings'),
    '#states' => array(
      'visible' => array(
        'input[name="omega_chrome_edge"]' => array('checked' => TRUE),
      ),
    ),
  );

  $element['omega_metatags']['chrome']['omega_internet_explorer_support'] = array(
    '#type' => 'select',
    '#title' => t('Minimum supported Internet Explorer version'),
    '#description' => t('The minimum version number of Internet Explorer that you actively support. The Chrome Frame prompt will display for any version below this version number.'),
    '#default_value' => omega_theme_get_setting('omega_internet_explorer_support'),
    '#options' => array(
      '_none' => t('No Internet Explorer support'),
      '9' => t('Internet Explorer 10'),
      '8' => t('Internet Explorer 9'),
      '7' => t('Internet Explorer 8'),
      '6' => t('Internet Explorer 7'),
    ),
  );

  return $element;
}

