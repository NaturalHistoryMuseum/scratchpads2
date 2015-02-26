<?php

/**
 * @file
 * Devel Generate module integration.
 */

/**
 * Implements hook_devel_generate().
 */
function countries_devel_generate($object, $field, $instance, $bundle) {
	if (field_behaviors_widget('multiple values', $instance) == FIELD_BEHAVIOR_CUSTOM) {
		return devel_generate_multiple('_countries_devel_generate', $object, $field, $instance, $bundle);
	}
	else {
		return _countries_devel_generate($object, $field, $instance, $bundle);
	}
}

/**
 * Implements hook_devel_generate().
 */
function _countries_devel_generate($object, $field, $instance, $bundle) {
	$cache = &drupal_static(__FUNCTION__, array());
	if (!isset($cache[$field['field_name']])) {
		$countries = array();
		foreach (countries_options_list($field) as $iso2 => $name) {
			$countries[$iso2]['iso2'] = $iso2;
		}
		$cache[$field['field_name']] = $countries;
	}
	return $cache[$field['field_name']][array_rand($cache[$field['field_name']])];
}
