<?php

/**
 * @file
 * Provide diff field functions for the Countries module.
 */

/**
 * Diff field callback for parsing country field comparative values.
 */
function countries_field_diff_view($items, $context) {
  $field = $context['field'];
  $instance = $context['instance'];
  $settings = $context['settings'];

  $diff_items = array();
  foreach ($items as $delta => $item) {
    if (empty($item['country']) && !empty($item['iso2'])) {
      $item['country'] = country_load($item['iso2']);
    }

    $country = $item['country'];
    switch ($settings['format']) {
      case 'country_official':
        if (!empty($country->official_name)) {
          $diff_items[$delta] = $country->official_name;
        }
        else {
          $diff_items[$delta] = $country->name;
        }
        break;

      case 'country_alpha_2':
        $diff_items[$delta] = $country->iso2;
        break;

      case 'country_alpha_3':
        $diff_items[$delta] = empty($country->iso3) ? '---' : $country->iso3;
        break;

      case 'country_number':
        $diff_items[$delta] = theme('countries_number', array('country' => $country));
        break;

      case 'country_default':
      default:
        $diff_items[$delta] = $country->name;
        break;
    }
    if (!empty($settings['append_iso'])) {
      $diff_items[$delta] .= ' (' . $country->iso2 . ')';
    }
  }
  return $diff_items;
}

/**
 * Provide default field comparison options.
 */
function countries_field_diff_default_options($field_type) {
  return array(
    'format' => 'default',
    'append_iso' => 1,
  );
}

/**
 * Provide a form for setting the field comparison options.
 */
function countries_field_diff_options_form($field_type, $settings) {
  $options_form = array();
  $formats = array();
  foreach (countries_field_formatter_info() as $key => $info) {
    if (strpos($key, 'countryicons')) {
      continue;
    }
    $formats[$key] = $info['label'];
  }
  $formats['country_default'] = t('Country name');
  $options_form['format'] = array(
    '#type' => 'select',
    '#title' => t('Name format'),
    '#default_value' => $settings['format'],
    '#options' => $formats,
    '#required' => TRUE,
  );
  $options_form['append_iso'] = array(
    '#type' => 'checkbox',
    '#title' => t('Append ISO Alpha-3 Code'),
    '#default_value' => $settings['append_iso'],
  );
  return $options_form;
}
