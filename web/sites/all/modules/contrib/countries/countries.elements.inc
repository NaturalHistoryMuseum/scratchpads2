<?php

/**
 * @file
 * Defines all element related code for the countries module.
 *
 * @todo Find a better way of removing the process, value and validation
 * callbacks into this file without needing the includes in the main module
 * file.
 */

/**
 * Actual implementation hook_element_info().
 *
 * @see countries_element_info()
 */
function _countries_element_info() {
  $type['country'] = array(
    '#input' => TRUE,
    '#size' => 0,
    '#multiple' => FALSE,
    '#process' => array(
      'countries_country_expand',
      'form_process_select',
      'ajax_process_form',
    ),
    '#element_validate' => array('countries_country_element_validate'),
    '#theme' => 'select',
    '#theme_wrappers' => array('form_element'),
    // Filter based on enabled flag or continents to filter the options.
    // See countries_filter() for details.
    '#filters' => array(),
    // If empty, the default list is the system country list, which is the
    // list of all enabled countries, which runs through hook_countries_alter().
    // Otherwise, the module runs it's own country list based on the filters.
    '#options' => array(),
  );
  return $type;
}

/**
 * Helper function to determine the value for a country form element.
 *
 * @param array $element
 *   The form element whose value is being populated.
 * @param bool|string|array $input
 *   The incoming input to populate the form element. If this is FALSE,
 *   the element's default value should be returned.
 *
 * @return array|string
 *   The data that will appear in the $element_state['values'] collection
 *   for this element. Return nothing to use the default.
 */
function _form_type_country_value($element, $input = FALSE) {
  if ($input !== FALSE) {
    if (isset($element['#multiple']) && $element['#multiple']) {
      return (is_array($input)) ? array_values($input) : array();
    }
    else {
      return $input;
    }
  }
}

/**
 * Validate callback for the country FAPI element.
 */
function _countries_country_element_validate($element, &$form_state) {
  if (!isset($element['#cardinality'])) {
    return;
  }
  $values = array();
  if (!is_array($element['#value'])) {
    $element['#value'] = array_filter(array($element['#value']));
  }
  foreach (array_values($element['#value']) as $value) {
    $values[] = array('iso2' => $value);
  }
  if ($element['#cardinality'] >= 0 && count($values) > $element['#cardinality']) {
    $title = empty($element['#title']) ? t('Countries') : $element['#title'];
    form_error($element, t('%name field is restricted to %max countries.', array('%name' => $title, '%max' => $element['#cardinality'])));
  }
}

/**
 * Our process callback to expand the country FAPI element.
 */
function _countries_country_expand($element) {
  if (empty($element['#options'])) {
    if (empty($element['#filters'])) {
      $element['#filters']['enabled'] = COUNTRIES_ENABLED;
    }
    $element['#options'] = countries_get_countries('name');
  }
  $element['#options'] = countries_filter($element['#options'], $element['#filters']);

  // Allow other modules to update this list.
  countries_invoke_additional_countries_alter($element['#options']);

  // Ensure that this is set in case '#hide_empty' is used.
  if (empty($element['#required']) && !isset($element['#empty_value'])) {
    $element['#empty_value'] = '';
  }

  // Adds a hidden element style for support of the continent-country widget.
  if (!empty($element['#hide_empty'])) {
    // Only return the element if it's not empty.
    $count = count($element['#options']);
    if (!$count || ($count == 1 && isset($element['#options'][$element['#empty_value']]))) {
      $element += array(
        '#prefix' => '',
        '#suffix' => '',
      );
      $element['#prefix'] .= '<div style="display: none;">';
      $element['#suffix'] = '</div>' . $element['#suffix'];
    }
  }
  return $element;
}
