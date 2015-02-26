<?php

/**
 * @file
 * Admin page callbacks for the Countries module.
 */

/**
 * Menu callback; Displays a list of all countries.
 */
function countries_admin_overview() {
  $header = array();
  $header[] = array('data' => t('Name'), 'field' => 'c.name', 'sort' => 'asc');
  $header[] = array('data' => t('ISO alpha-2 code'), 'field' => 'c.iso2');
  $columns = variable_get('countries_admin_overview_columns',
    array(
      'iso3' => t('ISO alpha-3 code'),
      'numcode' => t('ISO numeric-3 code'),
      'continent' => t('Continent'),
      'official_name' => t('Official name'))
  );
  foreach ($columns as $key => $title) {
    $header[] = array('data' => $title, 'field' => 'c.' . $key);
  }
  $header[] = array('data' => t('Status'), 'field' => 'c.enabled');

  $header[] = array('data' => t('Operations'));
  $query = db_select('countries_country', 'c')->extend('PagerDefault')->extend('TableSort');

  $result = $query
    ->fields('c')
    ->orderByHeader($header)
    ->limit(50)
    ->execute();

  $destination = drupal_get_destination();

  $rows = array();
  // Note that additional id's are added for testing.
  foreach ($result as $country) {
    $row = array();
    $row[] = l($country->name, 'admin/config/regional/countries/' . $country->iso2, array('query' => $destination));
    $row[] = array(
      'data' => country_property($country, 'iso2'),
      'id' => $country->iso2 . '-iso2',
    );
    foreach ($columns as $key => $title) {
      $row[] = array(
        'data' => country_property($country, $key, ''),
        'id' => $country->iso2 . '-' . $key,
      );
    }
    $row[] = array(
      'data' => country_property($country, 'enabled'),
      'id' => $country->iso2 . '-enabled',
    );
    $operations = l(t('edit'), 'admin/config/regional/countries/' . $country->iso2, array('query' => $destination));
    if (module_exists('countries_regions')) {
      $count = countries_regions_count($country);
      $operations .= '&nbsp;&nbsp;&nbsp;' . l(t('!count regions', array('!count' => $count)), 'admin/config/regional/countries/' . $country->iso2 . '/regions', array('query' => $destination));
    }
    if (!country_is_locked($country)) {
      $operations .= '&nbsp;&nbsp;&nbsp;' . l(t('delete'), 'admin/config/regional/countries/' . $country->iso2 . '/delete', array('query' => $destination));
    }
    $row[] = $operations;
    $rows[] = $row;
  }
  if (empty($rows)) {
    $rows[] = array(
      array('data' => t('No countries are available.'), 'colspan' => count($header)),
    );
  }

  $build['countries_table']  = array(
    '#theme' => 'table',
    '#header' => $header,
    '#rows' => $rows,
  );
  $build['countries_pager'] = array('#theme' => 'pager');

  return $build;
}

/**
 * Menu callback; Display a country form.
 */
function countries_admin_page($country = NULL) {
  // Menu callbacks and local actions do not have page titles.
  if (isset($country)) {
    drupal_set_title(t('Edit country %title', array('%title' => $country->name)), PASS_THROUGH);
  }
  else {
    drupal_set_title(t('Add country'), PASS_THROUGH);
    $country = country_create();
  }
  return drupal_get_form('countries_admin_form', $country);
}

/**
 * Generate a country form.
 *
 * @ingroup forms
 * @see countries_admin_form_validate()
 * @see countries_admin_form_submit()
 */
function countries_admin_form($form, &$form_state, $country) {
  $form['cid'] = array(
    '#type' => 'value',
    '#value' => $country->cid,
  );
  $form['name'] = array(
    '#type' => 'textfield',
    '#title' => t('Name'),
    '#default_value' => $country->name,
    '#description' => t('Specify an unique name for this country.'),
    '#required' => TRUE,
    '#maxlength' => 95,
  );
  $locked = country_is_locked($country);
  $form['iso2'] = array(
    '#type' => 'textfield',
    '#title' => t('ISO alpha-2 code'),
    '#default_value' => $country->iso2,
    '#required' => TRUE,
    '#maxlength' => 2,
    '#disabled' => $locked,
  );
  if ($locked) {
    $form['iso2']['#description'] = t('Core country ISO alpha-2 codes are not editable.');
  }
  else {
    $form['iso2']['#description'] = t('Specify an unique alpha-2 code for this country. This is used as the key to this country, changing it may result in data loss.');
  }
  $form['iso3'] = array(
    '#type' => 'textfield',
    '#title' => t('ISO alpha-3 code'),
    '#default_value' => $country->iso3,
    '#description' => t('Specify an unique ISO alpha-3 code for this country.'),
    '#required' => FALSE,
    '#maxlength' => 3,
  );
  $form['official_name'] = array(
    '#type' => 'textfield',
    '#title' => t('Official name'),
    '#default_value' => $country->official_name,
    '#description' => t('Specify an unique official name for this country.'),
    '#required' => FALSE,
    '#maxlength' => 127,
  );
  $form['numcode'] = array(
    '#type' => 'textfield',
    '#title' => t('ISO numeric-3 code'),
    '#default_value' => empty($country->numcode) ? '' : $country->numcode,
    '#description' => t('Specify an unique ISO numeric-3 code for this country.'),
    '#required' => FALSE,
    '#maxlength' => 5,
  );
  $form['continent'] = array(
    '#type' => 'select',
    '#title' => t('Continent'),
    '#options' => countries_get_continents(),
    '#default_value' => $country->continent,
    '#required' => TRUE,
  );
  $form['enabled'] = array(
    '#type' => 'checkbox',
    '#title' => t('Status'),
    '#default_value' => $country->enabled,
    '#description' => t('Disabling a country should removing it from all country listings, with the exclusion of views and fields define by the Countries module. These will allow you to choose the status per field.'),
  );

  if (!empty($country->iso2)) {
    $form['#country'] = $country;
  }

  field_attach_form('country', $country, $form, $form_state);
  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save'),
    '#weight' => 100,
  );
  return $form;
}

/**
 * Validate country form submissions.
 */
function countries_admin_form_validate($form, &$form_state) {
  $country = (object) $form_state['values'];

  // Added to provide a better UI experience. Decide to keep or drop.
  if (!empty($country->iso2) && $existing = country_load($country->iso2)) {
    // New country, the ISO alpha-2 must not be used.
    if (empty($form['#country'])) {
      form_set_error('iso2',
        t('Another country was found with this ISO alpha-2 code; !link',
        array(
          '!link' => l(countries_t($existing),
          'admin/config/regional/countries/' . $existing->iso2),
        )));
      return;
    }
    elseif ($existing->iso2 != $form['#country']->iso2) {
      form_set_error('iso2', t('Another country was found with this ISO alpha-2 code; !link',
        array('!link' => l(countries_t($existing), 'admin/config/regional/countries/' . $existing->iso2))));
      return;
    }
  }
  if (country_validate($country)) {
    $form_state['values'] = (array) $country;
  }
  else {
    foreach ($country->_errors as $property => $error_message) {
      form_set_error($property, $error_message);
    }
  }
}

/**
 * Process country form submissions.
 */
function countries_admin_form_submit($form, &$form_state) {
  $country = (object) $form_state['values'];
  entity_form_submit_build_entity('country', $country, $form, $form_state);

  if (country_save($country) == SAVED_UPDATED) {
    $message = t('The country %country has been updated.', array('%country' => $country->name));
  }
  else {
    $message = t('Added country %country.', array('%country' => $country->name));
  }
  drupal_set_message($message);
  $form_state['redirect'] = 'admin/config/regional/countries';
}

/**
 * Menu callback; confirm deletion of a country.
 *
 * @ingroup forms
 * @see countries_admin_delete_submit()
 */
function countries_admin_delete($form, &$form_state, $country) {
  if (country_is_locked($country)) {
    drupal_set_message(t('Core countries defined by the system can not be deleted.'), 'error');
    drupal_goto('admin/config/regional/countries');
  }

  $form['#country'] = $country;

  return confirm_form($form,
    t('Are you sure you want to delete the country %country?', array('%country' => $country->name)),
    'admin/config/regional/countries',
    t('References that use this country will become invalid. This action cannot be undone.'),
    t('Delete'),
    t('Cancel')
  );
}

/**
 * Process countries delete form submission.
 */
function countries_admin_delete_submit($form, &$form_state) {
  $country = $form['#country'];
  country_delete($country->iso2);

  drupal_set_message(t('Deleted country %country.', array('%country' => $country->name)));
  $form_state['redirect'] = 'admin/config/regional/countries';
}

/**
 * Menu callback to update the database from the CSV file.
 */
function countries_admin_import_form($form, &$form_state) {
  $results = countries_csv_updates();

  if (count($results['inserts'])) {
    $form_state['inserts'] = $results['inserts'];
    $form['inserts'] = array(
      '#type' => 'checkboxes',
      '#title' => t('Select the countries to import'),
    );
    foreach ($results['inserts'] as $iso2 => $country) {
      $t_options = array(
        '%name' => $country->name,
        '%continent' => $country->continent,
        '%official_name' => $country->official_name,
        '%iso2' => $country->iso2,
        '%iso3' => $country->iso3,
        '%numcode' => theme('countries_number', array('country' => $country)),
        '%enabled' => $country->enabled ? t('Enabled') : t('Disabled'),
      );
      $form['inserts']['#options'][$iso2] = t('New country %name, %continent (%official_name): %iso2, %iso3, %numcode, %enabled', $t_options);
      $form['inserts']['#default_value'][$iso2] = $iso2;
    }
  }
  else {
    $form['inserts'] = array(
      '#type' => 'markup',
      '#markup' => '<div class="form-item">' . t('There are no new records to import.') . '</div>',
    );
  }
  if (count($results['updates'])) {
    $form_state['updates'] = $results['updates'];
    $form['updates'] = array(
      '#type' => 'checkboxes',
      '#title' => t('Select the countries and their properties to update'),
    );

    foreach ($results['updates'] as $iso2 => $changes) {
      $country = country_load($iso2);
      foreach ($changes as $key => $values) {
        $t_options = array(
          '%name' => $country->name,
          '%iso2' => $country->iso2,
          '%code' => $key,
          '%new' => $values['new'],
          '%old' => $values['old'],
        );
        if ($key == 'enabled') {
          if ($values['new']) {
            $form['updates']['#options'][$iso2 . '-' . $key] = t('%name (%iso2): Enable this country', $t_options);
          }
          else {
            $form['updates']['#options'][$iso2 . '-' . $key] = t('%name (%iso2): Disable this country', $t_options);
          }

        }
        else {
          $form['updates']['#options'][$iso2 . '-' . $key] = t('%name (%iso2): Change %code from "%old" to "%new"', $t_options);
        }
        $form['updates']['#default_value'][$iso2 . '-' . $key] = $iso2 . '-' . $key;
      }
    }
  }
  else {
    $form['updates'] = array(
      '#type' => 'markup',
      '#markup' => '<div class="form-item">' . t('There are no updates to import.') . '</div>',
    );
  }
  if (!empty($results['skipped'])) {
    $items = array();
    foreach ($results['skipped'] as $iso => $errors) {
      foreach ($errors as $error) {
        $items[] = t('@iso: !error', array('@iso' => $iso, '!error' => $error));
      }
    }
    $form['skipped'] = array(
      '#type' => 'markup',
      '#markup' => '<div class="form-item">'
          . t('The following errors were found. These records have been skipped: !errors',
          array('!errors' => theme('item_list', array('items' => $items)))) . '</div>',
    );
  }
  if (!empty($form_state['inserts']) || !empty($form_state['updates'])) {
    $form['submit'] = array(
      '#type' => 'submit',
      '#value' => t('Import'),
    );
  }
  return $form;
}

/**
 * Submit handler.
 */
function countries_admin_import_form_submit($form, &$form_state) {
  $inserted = array();
  $updated = array();
  $skipped = array();
  if (!empty($form_state['inserts'])) {
    foreach (array_filter($form_state['values']['inserts']) as $iso2) {
      $country = $form_state['inserts'][$iso2];
      if ($duplicates = country_duplicate_field_check($country)) {
        $skipped[] = t('Skipped @iso2 due to duplicate fields in:<br />!duplicates',
            array('@iso2' => $iso2, '!duplicates' => implode('<br />', $duplicates)));
      }
      else {
        country_save($country, FALSE);
        $inserted[] = l($country->name, 'admin/config/regional/countries/' . $country->iso2);
      }
    }
  }
  if (count($inserted)) {
    drupal_set_message(t('The newly imported countries were: !countries', array('!countries' => implode('; ', $inserted))));
  }

  if (!empty($form_state['updates'])) {
    $changes = array();
    foreach (array_filter($form_state['values']['updates']) as $change) {
      list ($iso2, $key) = explode('-', $change);
      $changes[$iso2][$key] = $form_state['updates'][$iso2][$key]['new'];
    }
    foreach ($changes as $iso2 => $values) {
      $country = country_load($iso2);
      foreach ($values as $key => $value) {
        $country->{$key} = $value;
      }
      if ($duplicates = country_duplicate_field_check($country)) {
        $skipped[] = t('Skipped @iso2 due to duplicate fields in:<br />!duplicates',
            array('@iso2' => $iso2, '!duplicates' => implode('<br />', $duplicates)));
      }
      else {
        country_save($country, FALSE);
        $updated[] = l($country->name, 'admin/config/regional/countries/' . $country->iso2);
      }
    }
  }
  if (count($updated)) {
    drupal_set_message(t('The updated countries were: !countries', array('!countries' => implode('; ', $updated))));
  }

  if (empty($inserted) && empty($updated)) {
    drupal_set_message(t('No changes to the countries database were done.'));
  }
  else {
    countries_clear_caches();
  }

  if (!empty($skipped)) {
    foreach ($skipped as $warning) {
      drupal_set_message($warning, 'warning');
    }
  }
  $form_state['redirect'] = 'admin/config/regional/countries';
}

/**
 * Parses the given CSV file.
 */
function countries_csv_updates($file = NULL, $defaults = array()) {
  if (!$file) {
    $file = variable_get('countries_csv_datasource',
        drupal_get_path('module', 'countries') . '/countries.csv');
  }

  $defaults += array(
    'name' => '',
    'official_name' => '',
    'enabled' => 0,
    'iso2' => '',
    'iso3' => '',
    'numcode' => 0,
    'continent' => 'UN',
  );
  $countries = array();

  $inserts = array();
  $updates = array();
  $skipped = array();

  if ($handle = @fopen($file, "r")) {
    $headers = fgetcsv($handle, 1024, ",");
    while (($row = fgetcsv($handle, 1024, ",")) !== FALSE) {
      $country = new StdClass();
      $errors = array();
      foreach ($row as $index => $value) {
        if (!isset($defaults[$headers[$index]])) {
          continue;
        }
        $key = $headers[$index];
        $value = trim($value);
        // We need special processing for the enabled key.
        if ($key == 'enabled') {
          if (strlen($value) && $value != 'NULL') {
            $country->$key = empty($value) ? 0 : 1;
          }
        }
        elseif (empty($value) || $value == 'NULL') {
          // Skip setting defaults until we check existing countries later.
        }
        elseif ($error = countries_property_invalid($key, $value)) {
          $errors[] = $error;
        }
        else {
          if ($key == 'name' || $key == 'official_name') {
            // Enable translation of country names during importation.
            $value = t($value);
          }
          $country->$key = $value;
        }
      }

      if (!empty($country->iso2)) {
        // Validating the data as the source can not longer be trusted.
        if (empty($errors)) {
          $countries[$country->iso2] = $country;
        }
        else {
          $skipped[$country->iso2] = $errors;
        }
      }
    }
    fclose($handle);
  }

  foreach ($countries as $country) {
    if ($existing = country_load($country->iso2)) {
      foreach ($defaults as $key => $default_value) {
        // The only empty value we update is enabled.
        if (empty($country->{$key}) && $key != 'enabled') {
          continue;
        }
        if (!isset($existing->{$key})) {
          continue;
        }
        if (!isset($country->{$key})) {
          $country->{$key} = $existing ? $existing->{$key} : $default_value;
        }
        if ($key == 'name' || $key == 'official_name') {
          $existing->{$key} = t($existing->{$key});
        }
        if ((string) $existing->{$key} !== (string) $country->{$key}) {
          $updates[$country->iso2][$key] = array('new' => $country->{$key}, 'old' => $existing->{$key});
        }
      }
    }
    else {
      $country = (object) (((array) $country) + $defaults);
      $inserts[$country->iso2] = $country;
    }
  }
  return array(
    'inserts' => $inserts,
    'updates' => $updates,
    'skipped' => $skipped,
  );
}

/**
 * Helper function to validate a core country property.
 */
function countries_property_invalid($property, &$value) {
  static $schema = NULL;
  if (!isset($schema)) {
    $schema = drupal_get_schema('countries_country');
  }

  $value = trim($value);
  switch ($property) {
    case 'iso2':
      $value = drupal_strtoupper($value);
      if (preg_match('/[^A-Z]/', $value) || drupal_strlen($value) != 2) {
        return t('ISO alpha-2 code must contain 2 letters between A and Z. %value was found.', array('%value' => $value));
      }
      break;

    case 'iso3':
      $value = drupal_strtoupper($value);
      if (preg_match('/[^A-Z]/', $value) || drupal_strlen($value) != 3) {
        return t('ISO alpha-3 code must contain 3 letters between A and Z. %value was found.', array('%value' => $value));
      }
      break;

    case 'numcode':
      if (!empty($value) && (preg_match('/[^0-9]/', $value) || ($value > 999 || $value < 0))) {
        return t('ISO numeric-3 code must be a number between 1 and 999. %value was found.', array('%value' => $value));
      }
      break;

    default:
      if (isset($schema['fields'][$property])) {
        $field_schema = $schema['fields'][$property];
        if (isset($field_schema['length']) && $field_schema['length'] < drupal_strlen($value)) {
          $core_properties = countries_core_properties();
          return t('!property must be less than !count characters.',
              array('!property' => $core_properties[$property], '!count' => $field_schema['length']));
        }
      }
      break;
  }
  return FALSE;
}

/**
 * Helper function to check for duplicates.
 */
function country_duplicate_field_check($country) {
  $duplicates = array();
  $cid = empty($country->cid) ? FALSE : $country->cid;
  foreach (array('name', 'official_name', 'iso2', 'iso3', 'numcode') as $property) {

    if (drupal_strlen($country->$property)) {
      if ($property == 'numcode' && empty($country->$property)) {
        continue;
      }

      $query = db_select('countries_country', 'c')
          ->fields('c', array('iso2'));
      if ($cid) {
        $query->condition('cid', $cid, '!=');
      }
      if ($property == 'official_name' || $property == 'name') {
        $db_or = db_or();
        $db_or->condition('official_name', $country->$property)
            ->condition('name', $country->$property);
        $query->condition($db_or);
      }
      else {
        $query->condition($property, $country->$property);
      }

      $value = $query->execute()->fetchColumn();
      if (!empty($value)) {
        $conflict = country_load($value);
        $duplicates[$property] = t('The <em>@value</em> is already used by <em>@country</em>',
            array(
              '@value' => country_property($country, $property),
              '@country' => country_property($conflict, 'name'),
            ));
      }
    }
  }
  return empty($duplicates) ? FALSE : $duplicates;
}
