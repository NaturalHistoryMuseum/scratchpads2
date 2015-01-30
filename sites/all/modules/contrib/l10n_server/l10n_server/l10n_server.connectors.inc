<?php

/**
 * @file
 *   Administrative functions for localization server connectors.
 */

// = Main connector admin pages ================================================

/**
 * Main connector overview admin page.
 */
function l10n_server_connectors_overview() {
  $connectors = l10n_server_get_connectors();
  $sources = l10n_server_get_sources();
  if (empty($connectors) || empty($sources)) {
    drupal_set_message(t('No localization server connectors found.'), 'error');
    return '';
  }
  return drupal_get_form('l10n_server_admin_connectors_form', $connectors, $sources);
}

/**
 * Connector overview setup form.
 */
function l10n_server_admin_connectors_form($form, &$form_state, $connectors, $sources) {
  // Save for theming.
  $form['#connector_info'] = $connectors;
  $form['#source_info'] = $sources;

  // Build checkboxes to maintain enabled status.
  $form['connectors'] = array();
  foreach ($connectors as $name => $info) {
    foreach ($info['sources'] as $source) {

      // Build configure/scan links based on source info.
      $links = array();
      if (!empty($sources[$source]['form'])) {
        $links[] = l(t('configure'), 'admin/l10n_server/connectors/config/' . $name . '/' . $source);
      }
      else {
        $links[] = '';
      }

      if (!empty($sources[$source]['scan']) && variable_get('l10n_server_connector_' . $name . '_' . $source . '_enabled', 0)) {
        $links[] = l(t('scan'), 'admin/l10n_server/connectors/scan/' . $name . '/' . $source);
      }
      else {
        $links[] = '';
      }

      // Add checkbox and links.
      $form['connectors'][] = array(
        'l10n_server_connector_' . $name . '_' . $source . '_enabled' => array(
          '#title' => t('@title from @source', array('@title' => $info['title'], '@source' => $sources[$source]['title'])),
          '#type' => 'checkbox',
          '#default_value' => variable_get('l10n_server_connector_' . $name . '_' . $source . '_enabled', 0),
          '#l10n_server_source' => $source,
          '#l10n_server_connector' => $name,
        ),
        'l10n_server_connector_' . $name . '_' . $source . '_links' => array(
          '#type' => 'value',
          '#value' => $links,
        ),
      );
    }
  }
  $form = system_settings_form($form);
  $form['#theme'] = 'l10n_server_admin_connectors_form';
  return $form;
}

/**
 * Theme function for l10n_server_admin_connectors_form().
 */
function theme_l10n_server_admin_connectors_form($variables) {
  $form = $variables['form'];
  $rows = array();
  $max_operations = 1;
  foreach ($form['connectors'] as $id => &$elements) {
    $row = array('', '');
    if (is_array($elements)) {
      foreach ($elements as &$item) {
        if (!empty($item['#type']) && ($item['#type'] == 'checkbox')) {
          $row[1] = $item['#title'];
          unset($item['#title']);
          $row[0] = drupal_render($item);
        }
        elseif (!empty($item['#type']) && ($item['#type'] == 'value')) {
          $row = array_merge($row, $item['#value']);
          $max_operations = max($max_operations, count($item['#value']));
        }
      }
    }
    if (join('', $row) != '') {
      $rows[] = $row;
    }
  }

  // Put our overview into a table and add remaining form items.
  $headers = array(t('Enabled'), t('Connector'), array(
               'colspan' => $max_operations,
               'data' => t('Operations'),
             ));
  $output = theme('table', array('header' => $headers, 'rows' => $rows));
  $output .= drupal_render_children($form);
  return $output;
}

// = Connector configuration ===================================================

/**
 * Configuration page callback for arbitrary connector/source combinations.
 */
function l10n_server_connector_configure($connector = NULL, $source = NULL) {
  $connectors = l10n_server_get_connectors();
  $sources = l10n_server_get_sources();

  if (!isset($connectors[$connector]) || !isset($sources[$source]) || empty($sources[$source]['form'])) {
    // We should be able to display a form here.
    return drupal_not_found();
  }

  // Include file to handle form if needed.
  if (!empty($sources[$source]['file'])) {
    include_once DRUPAL_ROOT . '/' . $sources[$source]['file'];
  }

  drupal_set_title(t('@title from @source connector configuration', array('@title' => $connectors[$connector]['title'], '@source' => $sources[$source]['title'])), PASS_THROUGH);

  // TODO $connector needs to have $form as its first parameter.
  return drupal_get_form($sources[$source]['form'], $connector, $source);
}

// = Built-in file connector configuration =====================================

/**
 * Settings form callback.
 */
function l10n_server_connectors_files_settings_form($form, &$form_state, $connector_name, $source_name) {
  $form['#connector_name'] = $connector_name;

  $form['l10n_server_connector_' . $connector_name . '_' . $source_name . '_directory'] = array(
    '#title' => t('Source directory'),
    '#description' => t('The directory on the local file system to be scanned for source data. Either relative to the Drupal site root or an absolute path on your file system. Drupal should have read access to the files and directories found there. Projects are identified based on subdirectory names or the first part of filenames if put directly in the root directory. Releases are identified based on the second part of package filenames. Examples: Fishbowl/fishbowl-1.2.tar.gz is from project "Fishbowl" in version 1.2, while campwizard-3.4.tar.gz is project "campwizard" in version 3.4.'),
    '#type' => 'textfield',
    '#required' => TRUE,
    '#default_value' => variable_get('l10n_server_connector_' . $connector_name . '_' . $source_name . '_directory', conf_path() . '/files/' . $connector_name),
    // Create directory by default if possible.
    '#after_build' => array('l10n_server_connectors_files_check_directory'),
  );
  $form['l10n_server_connector_' . $connector_name . '_' . $source_name . '_limit'] = array(
    '#title' => t('Number of releases to look at once'),
    '#description' => t('The number of releases to scan on a manual or cron run. Scanning is synchronous, so you need to wait while extraction and parsing of file content is underway.'),
    '#type' => 'select',
    '#options' => drupal_map_assoc(array(1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 12, 14, 16, 18, 20)),
    '#default_value' => variable_get('l10n_server_connector_' . $connector_name . '_' . $source_name . '_limit', 1),
  );
  $form['l10n_server_connector_' . $connector_name . '_' . $source_name . '_cron'] = array(
    '#title' => t('Run scanning on cron'),
    '#type' => 'checkbox',
    '#default_value' => variable_get('l10n_server_connector_' . $connector_name . '_' . $source_name . '_cron', 0),
    '#description' => t('It is advised to set up a regular cron run to parse new files, instead of hitting the Scan tab manually.'),
  );
  return system_settings_form($form);
}

/**
 * Check/create directory if not there already.
 */
function l10n_server_connectors_files_check_directory($form_element) {
  file_prepare_directory($form_element['#value'], FILE_CREATE_DIRECTORY, $form_element['#parents'][0]);
  return $form_element;
}

// = Source scanning ===========================================================

/**
 * Scan page callback for arbitrary connector/source combinations.
 */
function l10n_server_connector_scan($connector = NULL, $source = NULL) {
  $connectors = l10n_server_get_connectors();
  $sources = l10n_server_get_sources();

  if (!isset($connectors[$connector]) || !isset($sources[$source]) || empty($sources[$source]['scan']) || empty($connectors[$connector]['scan'])) {
    // We should be able to invoke a scanner callback here.
    return drupal_not_found();
  }

  // Include file to handle form if needed.
  if (!empty($connectors[$connector]['file'])) {
    include_once DRUPAL_ROOT . '/' . $connectors[$connector]['file'];
  }

  return call_user_func($connectors[$connector]['scan'], $source);
}
