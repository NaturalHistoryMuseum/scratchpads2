<?php

/**
 * @file
 * Plugin to provide an argument handler for a country entity.
 */

/**
 * Plugins are described by creating a $plugin array which will be used
 * by the system that includes this file.
 */
$plugin = array(
  'title' => t("Country: ISO Code"),
  // keyword to use for %substitution
  'keyword' => 'country',
  'description' => t('Creates a country context from a ISO code argument. Accepts alpha-2, alpha-3 and numeric codes.'),
  'context' => 'countries_country_code_context',
  'placeholder form' => array(
    '#type' => 'textfield',
    '#description' => t('Enter the country ISO code for this argument.'),
  ),
);

/**
 * Discover if this argument gives us the term we crave.
 */
function countries_country_code_context($arg = NULL, $conf = NULL, $empty = FALSE) {
  // If unset it wants a generic, unfilled context.
  if ($empty) {
    return ctools_context_create_empty('entity:country');
  }

  // We can accept either a country entity or a pure nid.
  if (is_object($arg)) {
    return ctools_context_create('entity:country', $arg);
  }

  // A very generic country lookup is done here.
  // This will fallback to names, etc, but only ISO codes are really safe.
  if ($country = countries_country_lookup($arg)) {
    return ctools_context_create('entity:country', $country);
  }

  return FALSE;
}
