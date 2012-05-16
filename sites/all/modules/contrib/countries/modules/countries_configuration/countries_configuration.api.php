<?php

/**
 * @file
 * An overview of the hooks that this module provides.
 */

/**
 * Form settings for integrating with the countries configuration module.
 *
 * This API allows multiple forms per module.
 */
function hook_countries_configuration_options() {
  $items = array(
    // This should be an unique key for this option. To avoid conflicts with
    // other modules, use, or prefix with, the modules name.
    'address' => array(
      // This provides the tab title when editing the form.
      'title' => t('Addresses'),
      // This should provide the form of elements that you want to save.
      'form callback' => 'example_address_country_admin_form',
      // Optional: Provides a better title for the page edit.
      'title callback' => 'example_address_country_admin_form_title',
      // Required: Used when no data exists or when resetting the country data.
      'default values' => array(
        'labels' => array(),
      ),
      // Optional: Includes this title before any callbacks are executed.
      'file' => 'address.admin.inc',
      // Optional: included file path if not in the base module directory.
      'file path' => drupal_get_path('module', 'address') . '/includes',
      // Optional: Provides a help section to forms.
      'help' => t('This form allows you to set country specific configuration options for all address fields. Leave these blank to use the field defaults. Available address components are determined by the field settings.'),
    ),
  );
  $components = address_field_components();
  $items['address']['default values']['labels'] += array_combine(array_keys($components), array_fill(0, count($components), ''));

  return $items;
}

/**
 * An alter hook for hook_countries_configuration_options().
 *
 * Provides a hook into dynamically changing the settings provided by
 * hook_countries_configuration_options() in relation to a country.
 *
 * @param array $values
 *   The values with the default values loaded.
 * @param string $name
 *   The machine name given to this country configuration set.
 * @param array $info
 *   Additional info. Keyed elements are:
 *     country       - the country object or iso2 code.
 *     is_new        - flag to check if any data is stored for this country.
 *     load_defaults - flag to load default values.
 *                     This is used by the field settings to ensure that no
 *                     defaults are loaded into the Field UI area.
 */
function hook_countries_configuration_options_alter(&$values, $name, $info) {
  if ($name == 'address' && !$info['is_new']) {
    if ($overrides = address_country_details($info['country'])) {
      $values['labels'] = $overrides['labels'] + $values['labels'];
    }
  }
}

/**
 * An example form callback provided by hook_countries_configuration_options().
 *
 * @param object $country
 *   The country that is being edited.
 * @param array $values
 *   The existing values from the database or from the default settings.
 * @param array $form
 *   A reference to the base form if required.
 *   You can use this to append your own form data that is not maintained by
 *   this module. You will of course need to provide submit handles, et al, to
 *   maintain this data yourself.
 *
 * @return array
 *   The form structure of the additional form fields.
 */
function example_address_country_admin_form($country, $values, &$form) {
  foreach (address_field_components() as $key => $info) {
    $form['labels'][$key] = array(
      '#type' => 'textfield',
      '#title' => t('Label for @title', array('@title' => $info['name'])),
      '#default_value' => $values['labels'][$key],
    );
  }

  return $form;
}

/**
 * An example title callback provided by hook_countries_configuration_options().
 *
 * @param object $country
 *   The country that is being edited.
 *
 * @return string
 *   The page title to display.
 *   This is run through check_plain() so there is no need to escape the name.
 */
function example_address_country_admin_form_title($country) {
  return t('Edit !name address options', array('!name' => $country->name));
}

/**
 * An example implementing how you would use the data saved.
 *
 * @param object $country
 *   The country object to configure.
 */
function example_address_country_configuration_usage($country) {
  // Depreciated: This will log a warning in the watchdog table and will be
  // removed in future versions.
  // $settings = countries_load_configuration_options($country, 'address');
  $settings = countries_configuration($country, 'address');
  drupal_set_message(t('Editting the %region for %country', array(
    '%region' => $settings['labels']['region'],
    '%country' => $country->name,
  )));
}
