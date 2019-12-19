<?php

/**
 * @file
 * Contains the theme function override for 'system_themes_page'.
 */

/**
 * Returns HTML for the Appearance page.
 *
 * @ingroup themeable
 */
function omega_system_themes_page($variables) {
  $theme_groups = $variables['theme_groups'];

  $output = '<div id="system-themes-page">';

  foreach ($variables['theme_group_titles'] as $state => $title) {
    if (!count($theme_groups[$state])) {
      // Skip this group of themes if no theme is there.
      continue;
    }
    // Start new theme group.
    $output .= '<div class="system-themes-list system-themes-list--' . $state . ' clearfix"><h2>' . $title . '</h2>';

    foreach ($theme_groups[$state] as $theme) {
      // Theme the screenshot.
      if ($theme->screenshot) {
        $theme->screenshot['attributes']['class'] = str_replace('screenshot', 'theme-selector__screenshot', $theme->screenshot['attributes']['class']);
        $screenshot = theme('image', $theme->screenshot);
      }
      else {
        $screenshot = '<div class="theme-selector__screenshot theme-selector__screenshot--missing">' . t('No Screenshot') . '</div>';
      }

      // Localize the theme description.
      $description = t($theme->info['description']);

      // Style theme info.
      $notes = count($theme->notes) ? ' (' . implode(', ', $theme->notes) . ')' : '';
      $theme->classes[] = 'theme-selector';
      $theme->classes[] = 'theme-selector--' . $state;
      if ($theme->is_default) {
        $theme->classes = str_replace('theme-default', 'theme-selector--default', $theme->classes);
      }
      $theme->classes[] = 'clearfix';
      $output .= '<div class="' . implode(' ', $theme->classes) . '">' . $screenshot . '<div class="theme-selector__info"><h3>' . $theme->info['name'] . ' ' . (isset($theme->info['version']) ? $theme->info['version'] : '') . $notes . '</h3><div class="theme-description">' . $description . '</div>';

      // Make sure to provide feedback on compatibility.
      if (!empty($theme->incompatible_core)) {
        $output .= '<div class="theme-selector__incompatible">' . t('This version is not compatible with Drupal !core_version and should be replaced.', array(
            '!core_version' => DRUPAL_CORE_COMPATIBILITY,
          )) . '</div>';
      }
      elseif (!empty($theme->incompatible_php)) {
        if (substr_count($theme->info['php'], '.') < 2) {
          $theme->info['php'] .= '.*';
        }
        $output .= '<div class="theme-selector__incompatible">' . t('This theme requires PHP version @php_required and is incompatible with PHP version !php_version.', array(
            '@php_required' => $theme->info['php'],
            '!php_version' => phpversion(),
          )) . '</div>';
      }
      else {
        $output .= theme('links', array(
          'links' => $theme->operations,
          'attributes' => array(
            'class' => array('theme-selector__operations', 'clearfix'),
          ),
        ));
      }
      $output .= '</div></div>';
    }
    $output .= '</div>';
  }
  $output .= '</div>';

  return $output;
}
