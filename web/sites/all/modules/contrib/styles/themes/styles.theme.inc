<?php

/**
 * @file styles.theme.inc
 * Theme and preprocess functions for the Styles project.
 */

function theme_styles_field_formatter($variables) {
  $formatter = $variables['element']['#formatter'];
  $variables['object'] = (object)$variables['object'];
  $output = '';
  if (preg_match('@^styles_(.*?)_(.*?)$@i', $formatter, $matches)) {
    $variables['field_type'] = $field_type = $matches[1];
    $variables['style_name'] = $style_name = $matches[2];
    $output = theme('styles', $variables);
  }
  return $output;
}

function template_preprocess_styles(&$variables) {
  // If we have a Styles object instance already, then honor that.
  // Otherwise, we'll build a new Styles object instance from our settings.
  $variables['instance'] = $variables['instance'] ? $variables['instance'] : styles_instance($variables);

  // Add the style name to the wrapper's classes array.
  $variables['instance']->arrayPush('classes', $variables['style_name']);

  if ($variables['instance']) {
    // Prefix and suffix are for the wrapper, such as div or span.
    $variables['prefix'] = $variables['instance']->getPrefix();
    $variables['suffix'] = $variables['instance']->getSuffix();

    // Render the output for the template file.
    $variables['output'] = $variables['instance']->display(TRUE);
  }
  else {
    // We have no instance, thus nothing to output.
    $variables['output'] = '';
  }
}
