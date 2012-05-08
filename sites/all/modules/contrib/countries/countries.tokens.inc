<?php

/**
 * @file
 * Builds placeholder replacement tokens for country-related data.
 */

/**
 * Implements hook_token_info().
 */
function countries_token_info() {
  $types['country'] = array(
    'name' => t('Countries'),
    'description' => t('Tokens related to individual countries.'),
    'needs-data' => 'country',
  );
  $types['default-country'] = array(
    'name' => t('Default country'),
    'description' => t('Tokens related to the sites default country.'),
    'type' => 'country',
  );

  $core_properties = countries_core_properties();
  $country['cid'] = array(
    'name' => t('Country ID'),
    'description' => t('The unique internal database ID of the country.'),
  );
  $country['name'] = array(
    'name' => $core_properties['name'],
    'description' => t('The name of the country.'),
  );
  $country['official-name'] = array(
    'name' => $core_properties['official_name'],
    'description' => t('The official name of the country.'),
  );
  $country['iso2'] = array(
    'name' => $core_properties['iso2'],
    'description' => t('The @property of the country.', array('@property' => $core_properties['iso2'])),
  );
  $country['iso3'] = array(
    'name' => $core_properties['iso3'],
    'description' => t('The @property of the country.', array('@property' => $core_properties['iso3'])),
  );
  $country['numcode'] = array(
    'name' => $core_properties['numcode'],
    'description' => t('The @property of the country.', array('@property' => $core_properties['numcode'])),
  );
  $country['continent'] = array(
    'name' => $core_properties['continent'],
    'description' => t('The @property of the country.', array('@property' => $core_properties['continent'])),
  );
  $country['continent-code'] = array(
    'name' => t('Continent code'),
    'description' => t('The Continent code of the country.'),
  );
  $country['enabled'] = array(
    'name' => $core_properties['enabled'],
    'description' => t('The @property of the country.', array('@property' => $core_properties['continent'])),
  );

  return array(
    'types' => $types,
    'tokens' => array('country' => $country),
  );
}

/**
 * Implements hook_tokens().
 */
function countries_tokens($type, $tokens, array $data = array(), array $options = array()) {
  $sanitize = !empty($options['sanitize']);
  $replacements = array();
  if ($type == 'country' && !empty($data['country'])) {
    $country = $data['country'];
    foreach ($tokens as $name => $original) {
      $replacements[$original] = country_property($country, str_replace('-', '_', $name), '', $sanitize);
    }
  }

  if ($type == 'default-country') {
    $country = country_load(variable_get('site_default_country', ''));
    $replacements += token_generate('country', $tokens, array('country' => $country), $options);
  }

  return $replacements;
}
