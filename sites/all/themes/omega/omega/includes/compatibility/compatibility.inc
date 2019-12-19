<?php

/**
 * @file
 * Main extension file for the 'compatibility' extension.
 */

/**
 * Implements hook_extension_EXTENSION_registry_alter().
 */
function omega_extension_compatibility_theme_registry_alter(&$registry) {
  $registry['html']['preprocess functions'][] = 'omega_extension_compatibility_preprocess_html';

  if (omega_theme_get_setting('omega_apple_touch', TRUE)) {
    // The regex finds all files following certain naming conventions.
    $mask = '/^apple-touch-icon(-precomposed)?(-([0-9]+x[0-9]+))?\.png$/';

    // Loop over all themes in the trail in reverse (starting with the current
    // theme) and use the touch icons of the first theme we find. Note: There
    // is no inheritance, meaning that as soon as a theme includes a touch icon
    // any potential touch icons from the base theme are ignored entirely.
    foreach (array_reverse(omega_theme_trail()) as $theme => $name) {
      $path = drupal_get_path('theme', $theme);

      // Apple touch icons have to be in the root directory of the theme for
      // them to be discoverable.
      if ($files = file_scan_directory($path, $mask, array('recurse' => FALSE))) {
        foreach ($files as $file) {
          $matches = array();

          // Run the filename through the regex once more picking up the
          // sub-matches in order to find out the dimensions of the touch icon
          // and whether it's preprocessed or not.
          preg_match($mask, $file->filename, $matches);

          // Cache the array of apple touch icons.
          $registry['html']['apple-touch-icon'][$file->uri] = array(
            'uri' => $file->uri,
            'precomposed' => !empty($matches[1]),
            'sizes' => !empty($matches[3]) ? $matches[3] : FALSE,
          );
        }

        // Break out of the loop because we found at least one touch icon.
        break;
      }
    }
  }
}

/**
 * Implements hook_extension_EXTENSION_preproces_html().
 */
function omega_extension_compatibility_preprocess_html(&$variables) {
  // Add the 'HandheldFriendly' meta tag.
  if (omega_theme_get_setting('omega_handheld_friendly', TRUE)) {
    $meta = array(
      '#tag' => 'meta',
      '#attributes' => array('name' => 'HandheldFriendly', 'content' => 'true'),
    );

    drupal_add_html_head($meta, 'omega-handheld-friendly');
  }

  // Add a the 'MobileOptimized' meta tag.
  if (omega_theme_get_setting('omega_mobile_optimized', TRUE)) {
    $meta = array(
      '#tag' => 'meta',
      '#attributes' => array('name' => 'MobileOptimized', 'content' => 'width'),
    );

    drupal_add_html_head($meta, 'omega-mobile-optimized');
  }

  // Add the 'cleartype' meta tag.
  if (omega_theme_get_setting('omega_cleartype', TRUE)) {
    $meta = array(
      '#tag' => 'meta',
      '#attributes' => array('http-equiv' => 'cleartype', 'content' => 'on'),
    );

    drupal_add_html_head($meta, 'omega-cleartype');
  }

  // Add the 'viewport' meta tag.
  if (omega_theme_get_setting('omega_viewport', TRUE)) {
    $content = array('width=device-width');

    // Add the 'initial-scale' property if configured.
    if (($value = omega_theme_get_setting('omega_viewport_initial_scale')) && $value != '_default') {
      $content[] = "initial-scale=$value";
    }

    // The minimum-scale and maximum-scale properties are ignored if
    // user-scalable is set to 'no'.
    if (omega_theme_get_setting('omega_viewport_user_scaleable', TRUE) == FALSE) {
      $content[] = 'user-scalable=no';
    }
    else {
      // Set the minimum-scale and maximum-scale properties if specified.
      foreach (array('minimum', 'maximum') as $type) {
        if (($value = omega_theme_get_setting("omega_viewport_{$type}_scale", '_default')) && $value != '_default') {
          $content[] = "$type-scale=$value";
        }
      }
    }

    $meta = array(
      '#tag' => 'meta',
      '#attributes' => array(
        'name' => 'viewport',
        'content' => implode(', ', $content),
      ),
      // The 'viewport' metatag should be rendered after the 'HandheldFriendly'
      // metatag.
      '#weight' => 10,
    );

    drupal_add_html_head($meta, 'omega-viewport');
  }

  // Add Apple touch icons.
  if (omega_theme_get_setting('omega_apple_touch', TRUE)) {
    $registry = theme_get_registry(FALSE);

    if (!empty($registry['html']['apple-touch-icon'])) {
      // Iterate over the array of touch icons and attributes.
      foreach ($registry['html']['apple-touch-icon'] as $icon) {
        $meta = array(
          '#tag' => 'link',
          '#attributes' => array(
            'rel' => 'apple-touch-icon' . (!empty($icon['precomposed']) ? '-precomposed' : ''),
            'href' => file_create_url($icon['uri']),
          ),
        );

        if (!empty($icon['sizes'])) {
          $meta['#attributes']['sizes'] = $icon['sizes'];
        }

        // Add the touch icon to the head using a unique key.
        $key = 'omega-touch-' . (!empty($icon['precomposed']) ? '-precomposed' : '') . (!empty($icon['sizes']) ? '-' . $icon['uri'] : '');
        drupal_add_html_head($meta, $key);
      }
    }
  }

  // Send X-UA-Compatible HTTP header to force IE to use the most recent
  // rendering engine or use Chrome's frame rendering engine if available.
  if (omega_theme_get_setting('omega_chrome_edge', TRUE)) {
    $supported = omega_theme_get_setting('omega_internet_explorer_support', '_none');

    if (drupal_get_http_header('X-UA-Compatible') === NULL) {
      // Only add the header if it hasn't been added before.
      drupal_add_http_header('X-UA-Compatible', 'IE=edge,chrome=' . ($supported && $supported != '_none' ? 'IE' . $supported : '1'));
    }
  }
}
