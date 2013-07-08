<?php

/**
 * @file
 *   Administration pages for translation packaging.
 */

/**
 * Settings form callback.
 */
function l10n_packager_settings_form($form, &$form_state) {
  $form['l10n_packager_cron'] = array(
    '#title' => t('Generate packages on every Drupal cron run'),
    '#type' => 'checkbox',
    '#default_value' => variable_get('l10n_packager_cron', 0),
  );
  $form['l10n_packager_directory'] = array(
    '#title' => t('Directory for generated packages'),
    '#description' => t('The directory on the local file system to use to store packages generated. Either relative to the Drupal installation directory or an absolute path on your file system. Drupal should have read and write access to the files and directories found there.'),
    '#type' => 'textfield',
    '#required' => TRUE,
    '#default_value' => l10n_packager_directory(),
    // Create directory by default if possible.
    '#after_build' => array('l10n_packager_admin_check_directory'),
  );
  $form['l10n_packager_update_url'] = array(
    '#title' => t('Root URL for translation downloads'),
    '#type' => 'textfield',
    '#default_value' => variable_get('l10n_packager_update_url', file_create_url(l10n_packager_directory())),
    '#description' => t('Root URL for the client to build file URLs and fetch updates. The public facing URL for the package directory defined above. Leave blank for not providing any.'),
  );
  $form['l10n_packager_filepath'] = array(
    '#title' => t('Path structure for generated packages'),
    '#type' => 'textfield',
    '#required' => TRUE,
    '#default_value' => variable_get('l10n_packager_filepath', L10N_PACKAGER_FILEPATH),
    '#description' => t("Available tokens are: %project, %release, %core, %version, %extra, %language."),
  );
  $form['l10n_packager_release_limit'] = array(
    '#title' => t('Number of releases to check at once'),
    '#description' => t('The number of releases to check on a manual or cron run.'),
    '#type' => 'select',
    '#options' => drupal_map_assoc(array(1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 15, 20, 30, 40, 50, 60, 70, 80, 90, 100)),
    '#default_value' => variable_get('l10n_packager_release_limit', 10),
  );
  $form['l10n_packager_file_limit'] = array(
    '#title' => t('Maximum number of files to package at once'),
    '#description' => t('The number of files to package on a manual or cron run.'),
    '#type' => 'select',
    '#options' => drupal_map_assoc(array(1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 15, 20, 30, 40, 50, 60, 70, 80, 90, 100)),
    '#default_value' => variable_get('l10n_packager_file_limit', 1),
  );
  // Logging settings
  $period = array(0 => t('Never')) + drupal_map_assoc(array(3600, 10800, 21600, 32400, 43200, 86400, 172800, 259200, 604800, 1209600, 2419200, 4838400, 9676800), 'format_interval');
  $form['l10n_packager_update'] = array(
    '#title' => t('Repackaging interval'),
    '#type' => 'select',
    '#options' => $period,
    '#default_value' => variable_get('l10n_packager_update', 0),
    '#description' => t('Time interval for the translations to be automatically repackaged.'),
  );

  $form = system_settings_form($form);

  // Rebuild metafile after submisson.
  $form['#submit'][] = 'l10n_packager_export_metafile';

  return $form;
}

/**
 * Check/create directory if not there already.
 */
function l10n_packager_admin_check_directory($form_element) {
  file_prepare_directory($form_element['#value'], FILE_CREATE_DIRECTORY, $form_element['#parents'][0]);
  return $form_element;
}

/**
 * Join two independent forms on one page.
 */
function l10n_packager_admin_repackage_page() {
  $projects = l10n_server_get_projects();

  $content = array();
  $content[] = drupal_get_form('l10n_packager_admin_projects_form', $projects);
  $content[] = drupal_get_form('l10n_packager_admin_languages_form');

  // @todo: make this project agnostic for others running l10n_server.
  if (!empty($projects['drupal'])) {
    $content[] = drupal_get_form('l10n_packager_admin_installer_form', $projects['drupal']);
  }

  return $content;
}

/**
 * Form callback. Repackage translations manually.
 */
function l10n_packager_admin_languages_form($form) {
  $form['languages'] = array(
    '#type' => 'fieldset',
    '#title' => t('List of languages'),
    '#description' => t('This list is rebuilt on every change to any languages, but you can force a rebuild here.'),
  );
  $form['languages']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Rebuild language list'),
    '#submit' => array('l10n_packager_export_metafile'),
  );
  return $form;
}

/**
 * Form callback. Repackage translations manually.
 */
function l10n_packager_admin_projects_form($form, &$form_state, $projects) {
  $form['projects'] = array(
    '#type' => 'fieldset',
    '#title' => t('Project translations'),
    '#description' => t('Repackage all translations of a single project or release.'),
  );

  $form['projects']['project'] = array(
    '#title' => t('Project'),
    '#required' => TRUE,
  );
  if (($count = count($projects)) <= 30) {
    // Radio box widget for as much as 5 projects, select widget for 5-30 projects.
    $form['projects']['project']['#type'] = ($count <= 5 ? 'radios' : 'select');
    $form['projects']['project']['#options'] = array();
    foreach ($projects as $project) {
      // Title used to conform to the autocomplete behavior.
      $form['projects']['project']['#options'][$project->title] = $project->title;
    }
  }
  else {
    // Autocomplete field for more then 30 projects.
    $form['projects']['project'] += array(
      '#type' => 'textfield',
      '#autocomplete_path' => 'translate/project-autocomplete',
    );
  }

  $form['projects']['release'] = array(
    '#title' => t('Release'),
    '#type' => 'textfield',
    '#description' => t('Optionally select a release name like <em>6.x-1.0-beta1</em> or a partial release name like <em>6.x%</em>.'),
  );
  $form['projects']['languages'] = array(
    '#type' => 'select',
    '#title' => t('Only for these languages'),
    '#multiple' => TRUE,
    '#default_value' => array(),
    '#options' => l10n_community_get_languages('name'),
    '#description' => t('Select none for all languages. Otherwise pick the languages you want repackaged.'),
  );

  $form['projects']['buttons']['repackage'] = array(
    '#type' => 'submit',
    '#value' => t('Repackage now'),
  );
  $form['projects']['buttons']['mark'] = array(
    '#type' => 'submit',
    '#value' => t('Mark for repackaging'),
  );
  return $form;
}

/**
 * Repackage form validation
 */
function l10n_packager_admin_projects_form_validate($form, &$form_state) {
  if (!empty($form_state['values']['project'])) {
    if ($project = l10n_server_get_projects(array('uri' => l10n_community_project_uri_by_title($form_state['values']['project'])))) {
      $form_state['values']['pid'] = $project->pid;
    }
    else {
      form_set_error('project', t('Invalid project name.'));
    }
  }
  if (!empty($form_state['values']['release'])) {
    $query = db_select('l10n_server_release');
    $query->addExpression('COUNT(rid)');
    $query->condition('title', $form_state['values']['release'], 'LIKE');
    if (!empty($project)) {
      $query->condition('pid', $project->pid);
    }
    if (!$query->execute()->fetchField()) {
      form_set_error('release', t('Invalid release name.'));
    }
  }
  if (empty($form_state['values']['project']) && empty($form_state['values']['release'])) {
    form_set_error('project', t('You must select a project and optionally a release.'));
  }
}

/**
 * Repackage form submission
 */
function l10n_packager_admin_projects_form_submit($form, $form_state) {
  module_load_include('inc', 'l10n_packager');

  $op = $form_state['values']['op'];
  $languages = array_filter($form_state['values']['languages']);
  $pid = !empty($form_state['values']['pid']) ? $form_state['values']['pid'] : NULL;
  $release = !empty($form_state['values']['release']) ? $form_state['values']['release'] : NULL;

  // Build the query that will be used in different ways depending on the operation
  $query = db_select('l10n_server_release', 'r')->fields('r', array('rid'));
  if ($pid) {
    $query = $query->condition('pid', $pid);
  }
  if ($release) {
    $query = $query->condition('title', $release, 'LIKE');
  }

  // Now check results and run operations
  if ($op == t('Repackage now')) {
    $rids = $query->execute()->fetchCol();
    if (!empty($rids)) {
      $batch = l10n_packager_release_batch($rids, $languages);
      batch_set($batch);
    }
    else {
      drupal_set_message(t('No releases found for repackaging.'), 'error');
    }
  }
  elseif ($op == t('Mark for repackaging')) {
    $affected_rows = db_update('l10n_packager_release')
      ->fields(array(
          'updated' => 0,
          'checked' => 0,
          'status'  => L10N_PACKAGER_ACTIVE,
        ))
      ->condition('rid', $query, 'IN')
      ->execute();
    drupal_set_message(t("Marked %count releases for repackaging.", array('%count' => $affected_rows)));
  }
}

/**
 * Form callback. Package installer tarballs for Drupal core.
 *
 * The tarball created is offered for download, so the user can get it to their
 * install profile right away (given multiple files are generated).
 *
 * @todo
 *   Make it possible to reuse this for non-Drupal.org use, eg. by letting the
 *   user pick a project and a release.
 */
function l10n_packager_admin_installer_form($form, &$form_state, $project) {
  $form['installer'] = array(
    '#type' => 'fieldset',
    '#title' => t('Install profile packaging'),
    '#description' => t('Generate a set of .po files to be used in a Drupal 6 install profile from Drupal core.'),
  );

  $form['installer']['project'] = array(
    '#type' => 'value',
    '#value' => $project,
  );

  $releases = l10n_server_get_releases($project->uri);
  foreach ($releases as $rid => $this_release) {
    $release_options[$rid] = t('@release only', array('@release' => $this_release->title));
  }
  $form['installer']['release'] = array(
    '#title' => t('Release'),
    '#type' => count($release_options) <= 3 ? 'radios' : 'select',
    '#options' => $release_options,
    '#default_value' => isset($release) ? $release : 'all',
    '#required' => TRUE,
  );

  $form['installer']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Download an install profile package'),
  );
  return $form;
}

/**
 * Package an install profile tarball with .po files for all languages.
 */
function l10n_packager_admin_installer_form_submit($form, $form_state) {
  // l10n_community_requirements() makes sure there is a status
  // error if this is not installed.
  include_once 'Archive/Tar.php';

  module_load_include('inc', 'l10n_community', 'export');

  // Generate tgz file with all files added.
  $tempfile = tempnam(file_directory_temp(), 'l10n_community-' . $form_state['values']['project']->uri . '-installer');
  $tar = new Archive_Tar($tempfile, 'gz');

  $languages = l10n_community_get_languages();
  foreach ($languages as $language) {
    $output = l10n_community_export($form_state['values']['project']->uri, $form_state['values']['release'], $language, FALSE, TRUE, TRUE);
    // Add file to the tgz. We need to purely use the language code because
    // Drupal 6 uses that to identify the language.
    $tar->addString($language->language . '.po', file_get_contents($output[1]));
  }

  $releases = l10n_server_get_releases($form_state['values']['project']->uri);
  $release = $releases[$form_state['values']['release']];

  // Return compressed archive to user.
  header('Content-Disposition: attachment; filename=drupal-installer-' . $release->title . '.tgz');
  header('Content-Type: application/x-compressed');
  echo file_get_contents($tempfile);
  unlink($tempfile);
  die();
}
