<?php

/**
 * @file
 * All field related code.
 */

/**
 * Implements hook_field_info().
 */
function countries_field_info() {
  return array(
    'country' => array(
      'label' => t('Country'),
      'description' => t('This field stores a country reference in the database.'),
      'settings' => array(
        'countries' => array(),
        'continents' => array(),
        'enabled' => COUNTRIES_ENABLED,
        'size' => 5,
      ),
      'default_widget' => 'options_select',
      'default_formatter' => 'country_default',
      // Support hook_entity_property_info() from contrib "Entity API".
      'property_type' => 'country',
      'property_callbacks' => array('countries_entity_metadata_field_property_callback'),
    ),
  );
}

/**
 * @TODO: Function to keep compatibility with 7.x-2.1 version.
 * @see: #2293209 issue.
 */
function countries_field_info_property_callback(&$info, $entity_type, $field, $instance, $field_type) {
  countries_entity_metadata_field_property_callback($info, $entity_type, $field, $instance, $field_type);
}

/**
 * Callback to add in property info defaults per field instance.
 * @see entity_metadata_field_entity_property_info().
 */
function countries_entity_metadata_field_property_callback(&$info, $entity_type, $field, $instance, $field_type) {
  $name = $field['field_name'];
  $property = &$info[$entity_type]['bundles'][$instance['bundle']]['properties'][$name];
  entity_metadata_field_default_property_callback($info, $entity_type, $field, $instance, $field_type);
  $property['options list'] = 'entity_metadata_field_options_list';
}

/**
 * Implements hook_field_prepare_view().
 */
function countries_field_prepare_view($entity_type, $entities, $field, $instances, $langcode, &$items) {
  foreach ($entities as $id => $entity) {
    foreach ($items[$id] as $delta => $item) {
      if (!isset($item['country'])) {
        $items[$id][$delta]['value'] = $item['iso2'];
        if (!empty($item['iso2']) && $country = country_load($item['iso2'])) {
          $items[$id][$delta]['country'] = $country;
          $items[$id][$delta]['safe_value'] = check_plain($items[$id][$delta]['country']->name);
        }
        else {
          $items[$id][$delta]['country'] = FALSE;
          $items[$id][$delta]['safe_value'] = '';
        }
        // @todo: Remove post 7.x-2.x
        $items[$id][$delta]['safe'] = $items[$id][$delta]['safe_value'];
      }
    }
  }
}

/**
 * Implements hook_field_is_empty().
 */
function countries_field_is_empty($item, $field) {
  return empty($item['iso2']);
}

/**
 * Implements hook_field_formatter_info().
 */
function countries_field_formatter_info() {
  $formatters = array(
    'country_default' => array(
      'label' => t('Default'),
      'field types' => array('country'),
    ),
    'country_official' => array(
      'label' => t('Official name'),
      'field types' => array('country'),
    ),
    'country_alpha_2' => array(
      'label' => t('ISO alpha-2 code'),
      'field types' => array('country'),
    ),
    'country_alpha_3' => array(
      'label' => t('ISO alpha-3 code'),
      'field types' => array('country'),
    ),
    'country_number' => array(
      'label' => t('ISO numeric-3 code'),
      'field types' => array('country'),
    ),
    'country_continent' => array(
      'label' => t('Continent'),
      'field types' => array('country'),
    ),
    'country_continent_code' => array(
      'label' => t('Continent code'),
      'field types' => array('country'),
    ),
  );

  // Adds a formatter option for the country icon.
  if (module_exists('countryicons')) {
    $formatters['country_icon'] = array(
      'label' => t('Country Icon'),
      'field types' => array('country'),
      'settings' => array(
        'countryiconset' => '',
        'property' => '',
      ),
    );
  }

  return $formatters;
}

/**
 * Implements hook_field_formatter_settings_summary().
 */
function countries_field_formatter_settings_summary($field, $instance, $view_mode) {
  $display = $instance['display'][$view_mode];
  $settings = (array) $display['settings'];
  $settings += array(
    'countryiconset' => '',
    'property' => '',
  );
  $summary = '';
  if ($display['type'] == 'country_icon' && module_exists('countryicons')) {
    $version = function_exists('countryicons_api_version') ? countryicons_api_version() : '1.0';
    $options = array();
    foreach (countryicons_get_iconsets() as $iconset) {
      $iconset = (array) $iconset;
      $key = version_compare("$version", '2.0') >= 0 ? $iconset['key'] : $iconset['name'];
      $name = version_compare("$version", '2.0') >= 0 ? $iconset['name'] : $iconset['description'];
      if ('icon_' . $key == $settings['countryiconset']) {
        $summary = t('<strong>Icon:</strong> @country_icon_set', array(
          '@country_icon_set' => empty($name) ? $key : $name,
        ));
        break;
      }
      elseif ('sprite_' . $key == $settings['countryiconset']) {
        $summary = t('<strong>Icon sprite:</strong> @country_icon_set', array(
          '@country_icon_set' => empty($name) ? $key : $name,
        ));
        break;
      }
    }
    if (empty($summary)) {
      $summary = t('Missing icon or spirte plugin');
    }
    if (!empty($settings['property'])) {
      $properties = countries_core_properties();
      $properties['continent_code'] = t('Continent code');
      $summary .= '<br/>' . t('<strong>Suffix:</strong> %property', array('%property' => $properties[$settings['property']]));
    }

  }
  return $summary;
}

/**
 * Implements hook_field_formatter_settings_form().
 */
function countries_field_formatter_settings_form($field, $instance, $view_mode, $form, &$form_state) {
  $display = $instance['display'][$view_mode];
  $settings = (array) $display['settings'];
  $element = array();

  if ($display['type'] == 'country_icon' && module_exists('countryicons')) {
    $settings += array(
      'countryiconset' => '',
      'property' => '',
    );

    $version = function_exists('countryicons_api_version') ? countryicons_api_version() : '1.0';
    $options = array();
    foreach (countryicons_get_iconsets() as $iconset) {
      $iconset = (array) $iconset;

      // The only diff between versions 1 & 2 of countryicons is the key value.
      $key = version_compare("$version", '2.0') >= 0 ? $iconset['key'] : $iconset['name'];
      $name = version_compare("$version", '2.0') >= 0 ? $iconset['name'] : $iconset['description'];
      $options['icon_' . $key] = t('Icon: @country_icon_set', array('@country_icon_set' => empty($name) ? $key : $name));
      if (!empty($iconset['css_sprite'])) {
        $options['sprite_' . $key] = t('Icon sprite: @country_icon_set', array('@country_icon_set' => empty($name) ? $key : $name));
      }
    }
    $element['countryiconset'] = array(
      '#type' => 'select',
      '#title' => t('Country icon source'),
      '#options' => $options,
      '#default_value' => $settings['countryiconset'],
    );
    $properties = countries_core_properties();
    $properties['continent_code'] = t('Continent code');
    $element['property'] = array(
      '#type' => 'select',
      '#title' => t('Icon suffix'),
      '#options' => array('' => t('-- None --')) + $properties,
      '#default_value' => $settings['property'],
    );
  }
  return $element;
}


/**
 * Implements hook_field_formatter_view().
 */
function countries_field_formatter_view($entity_type, $entity, $field, $instance, $langcode, $items, $display) {
  $element = array();
  $map = array(
    'country_official' => 'official_name',
    'country_alpha_2' => 'iso2',
    'country_alpha_3' => 'iso3',
    'country_number' => 'numcode',
    'country_continent' => 'continent',
    'country_continent_code' => 'continent_code',
  );
  foreach ($items as $delta => $item) {
    $country = $item['country'];
    if (empty($country)) {
      continue;
    }
    elseif (isset($map[$display['type']])) {
      $element[$delta]['#markup'] = country_property($country, $map[$display['type']]);
    }
    elseif ($display['type'] == 'country_icon') {
      $variables = array(
        'country' => $country,
        'settings' => $display['settings'],
      );
      $element[$delta]['#markup'] = theme('country_icon_adapter', $variables);
    }
    else {
      $element[$delta]['#markup'] = $item['safe_value'];
    }
  }
  return $element;
}

/**
 * Implements hook_field_widget_info().
 */
function countries_field_widget_info() {
  return array(
    'countries_continent' => array(
      'label' => t('Countries by continent'),
      'field types' => array('country'),
      'settings' => array(),
    ),
  );
}

/**
 * Implements hook_field_widget_info_alter().
 */
function countries_field_widget_info_alter(&$info) {
  if (isset($info['options_buttons'])) {
    $info['options_buttons']['field types'][] = 'country';
    $info['options_select']['field types'][] = 'country';
  }
}

/**
 * Implements hook_field_widget_form().
 */
function countries_field_widget_form(&$form, &$form_state, $field, $instance, $langcode, $items, $delta, $element) {
  if ($instance['widget']['type'] == 'countries_continent') {
    $field_name = $field['field_name'];
    $settings = $field['settings'];
    $required = !empty($element['#required']);
    $continents = countries_get_continents();

    if (!empty($settings['continents'])) {
      $continents = array_intersect_key($continents, $settings['continents']);
    }

    // Widget may be rendered individually, in a group or even a sub-form.
    $field_parents = empty($form['#parents']) ? array() : $form['#parents'];
    $parents = array_merge($field_parents, array($field_name, $langcode, $delta));
    $continent_parents = array_merge($parents, array('continent'));

    $iso2 = empty($items[$delta]['iso2']) ? '' : $items[$delta]['iso2'];
    $selected_continent = '';
    if (isset($form_state['values'])) {
      // This override the settings that are passed onto the options filter
      // query. This is not called when JScript is disabled.
      $selected_continent = drupal_array_get_nested_value($form_state['values'], $continent_parents);
      if (!$selected_continent) {
        // Fallback to the raw user post. A workaround for AJAX submissions.
        $user_data = drupal_array_get_nested_value($form_state['input'], $continent_parents);
        if (array_key_exists($user_data, $continents)) {
          $selected_continent = $user_data;
        }
      }
      // Do not override the continent settings when empty.
      if ($selected_continent) {
        $settings['continents'] = array($selected_continent);
      }
    }
    elseif (!empty($iso2) && $country = country_load($iso2)) {
      $selected_continent = $country->continent;
    }

    // Work out the best ID to exactly target the element.
    $wrapper_parents = array_merge($parents, array('iso-wrapper'));
    $wrapper_id = implode('-', $wrapper_parents);

    // Assist themers with floating DIVs.
    $element['#type'] = 'container';
    $element['#attributes']['class'][] = 'clearfix countries-continent-wrapper';
    $element['continent'] = array(
      '#type' => 'select',
      '#title' => t('Continent'),
      '#options' => $continents,
      '#empty_option' => t('-- None selected --'),
      '#empty_value' => 'none',
      '#default_value' => $selected_continent,
      '#required' => $required,
      '#ajax' => array(
        'callback' => '_ajax_countries_continent_widget_callback',
        'wrapper' => $wrapper_id,
      ),
    );
    $first = array_shift($continent_parents);
    $element['iso2'] = array(
      '#type' => 'country',
      '#title' => t('Country'),
      '#default_value' => $iso2,
      '#prefix' => '<div id="' . $wrapper_id . '" class="countries-ajax-wrapper">',
      '#suffix' => '</div>',
      '#empty_value' => '',
      '#hide_empty' => TRUE,
      '#filters' => $settings,
      '#required' => $required,
      '#states' => array(
        'visible' => array(
          ':input[name="' . $first . '[' . implode('][', $continent_parents) . ']"]' => array('!value' => 'none'),
        ),
      ),
    );
  }
  return $element;
}

/**
 * Returns the country element filtered by continent via AJAX.
 */
function _ajax_countries_continent_widget_callback($form, $form_state) {
  $parents = $form_state['triggering_element']['#array_parents'];
  array_pop($parents);
  $parents[] = 'iso2';
  $element = drupal_array_get_nested_value($form, $parents);

  return $element;
}

/**
 * Implements hook_options_list().
 */
function countries_options_list($field) {
  $function = !empty($field['settings']['options_list_callback']) ? $field['settings']['options_list_callback'] : 'countries_allowed_values';

  $field['settings'] += array('countries' => array());

  if (!isset($field['#filters'])) {
    $filters = array(
      'countries' => array_filter($field['settings']['countries']),
      'continents' => array_filter($field['settings']['continents']),
      'enabled' => $field['settings']['enabled'],
    );
    $field['#filters'] = $filters;
  }

  return $function($field);
}

/**
 * Returns a set of valid countries of a countries field.
 */
function countries_allowed_values($field) {
  $countries = countries_get_countries('name', $field['#filters']);

  // Allow other modules to update this list.
  countries_invoke_additional_countries_alter($countries);

  return $countries;
}

/**
 * Implements hook_field_settings_form().
 */
function countries_field_settings_form($field, $instance, $has_data) {
  $form = array();
  countries_filter_options_form($form, $field['settings']);
  return $form;
}

/**
 * Wrapper for the filter form options that are shared with other fields.
 */
function countries_filter_options_form(&$form, $settings) {
  $form['enabled'] = array(
    '#type' => 'radios',
    '#title' => t('Country status'),
    '#default_value' => isset($settings['enabled']) ? $settings['enabled'] : COUNTRIES_ENABLED,
    '#options' => array(
      COUNTRIES_ALL => t('Both'),
      COUNTRIES_ENABLED => t('Enabled countries only'),
      COUNTRIES_DISABLED => t('Disabled countries only'),
    ),
  );

  $form['countries'] = array(
    '#type' => 'select',
    '#title' => t('Filter by country'),
    '#default_value' => isset($settings['countries']) ? $settings['countries'] : array(),
    '#options' => countries_get_countries('name'),
    '#description' => t('If no countries are selected, this filter will not be used.'),
    '#size' => 10,
    '#multiple' => TRUE,
  );

  $form['continents'] = array(
    '#type' => 'checkboxes',
    '#title' => t('Filter by continent'),
    '#default_value' => isset($settings['continents']) ? $settings['continents'] : array(),
    '#options' => countries_get_continents(),
    '#description' => t('If no continents are selected, this filter will not be used.'),
    '#element_validate' => array('_countries_checkbox_filter'),
  );
}

/**
 * Helper function to filter empty options from a multi-checkbox field.
 */
function _countries_checkbox_filter($element, &$form_state) {
  form_set_value($element, $element['#value'], $form_state);
}
