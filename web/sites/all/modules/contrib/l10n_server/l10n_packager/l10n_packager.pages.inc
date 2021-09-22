<?php

/**
 * @file
 *   Front end pages for translation packaging.
 */

/**
 * @todo Please document this function.
 * @see http://drupal.org/node/1354
 */
function l10n_packager_explore_downloads() {
  drupal_add_css(drupal_get_path('module', 'l10n_packager') . '/l10n_packager.css');
  drupal_add_css(drupal_get_path('module', 'l10n_community') . '/l10n_community.css');

  $projects = l10n_server_get_projects();
  $form = drupal_get_form('l10n_packager_download_project_form', $projects);

  // @todo: remove hard-wired Drupal default from here.
  if (empty($_GET['project'])) {
    if ($drupal = l10n_server_get_projects(array('uri' => 'drupal'))) {
      $project = $drupal;
    }
  }
  else {
    $project = l10n_server_get_projects(array('uri' => $_GET['project']));
  }
  if (empty($project)) {
    drupal_set_message(t('Requested project not found.'), 'warning');
    return $form;
  }

  // Set proper title for given project.
  drupal_set_title(t('@project translation downloads', array('@project' => $project->title)), PASS_THROUGH);

  $output = drupal_render($form);
  // Expire old entries and then try to look up our entry.
  cache_clear_all(NULL, 'cache_l10n_packager');
  $rendered = cache_get('download:' . $project->uri, 'cache_l10n_packager');
  // Not found, generate the list from our data.
  if (empty($rendered)) {
    $downloads = l10n_packager_show_downloads($project);
    // Store for later use.
    cache_set('download:' . $project->uri, $downloads, 'cache_l10n_packager', REQUEST_TIME + 60 * 60 * 24);
    $output .= $downloads;
  }
  else {
    $output .= $rendered->data;
  }

  return $output;
}

/**
 * Form callback. Allow user to pick project to show downloads for.
 */
function l10n_packager_download_project_form($form, &$form_state, $projects) {
  if (empty($form_state['values'])) {
    $form_state['values'] = array(
      // @todo: bad, bad hardcoding of Drupal as project.
      'project' => (isset($_GET['project']) && isset($projects[$_GET['project']]) ? $projects[$_GET['project']]->title : 'Drupal core'),
    );
  }
  $form['project'] = array(
    '#default_value' => $form_state['values']['project'],
    '#title' => t('Pick a project'),
  );
  if (($count = count($projects)) <= 30) {
    // Radio box widget for as much as 5 projects, select widget for 5-30 projects.
    $form['project']['#type'] = ($count <= 5 ? 'radios' : 'select');
    $form['project']['#options'] = array();
    foreach ($projects as $project) {
      // Title used to conform to the autocomplete behavior.
      $form['project']['#options'][$project->title] = $project->title;
    }
  }
  else {
    // Autocomplete field for more then 30 projects.
    $form['project'] += array(
      '#type' => 'textfield',
      '#autocomplete_path' => 'translate/project-autocomplete',
    );
  }
  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Show downloads'),
  );
  return $form;
}

/**
 * Form validation.
 */
function l10n_packager_download_project_form_validate($form, &$form_state) {
  if (!empty($form_state['values']['project'])) {
    if ($project = l10n_server_get_projects(array('uri' => l10n_community_project_uri_by_title($form_state['values']['project'])))) {
      $form_state['values']['uri'] = $project->uri;
    }
    else {
      form_set_error('project', t('Invalid project name.'));
    }
  }
}

/**
 * Form submission.
 */
function l10n_packager_download_project_form_submit($form, $form_state) {
  drupal_goto('translate/downloads', array('query' => array('project' =>  $form_state['values']['uri'])));
}

/**
 * Generate list of file downloads for given project.
 */
function l10n_packager_show_downloads($project) {
  $files = array();
  $branches = array();
  $result = db_query('SELECT r.*, pr.*, pr.checked as release_checked, lf.*, f.* FROM {l10n_server_release} r INNER JOIN {l10n_packager_release} pr ON r.rid = pr.rid INNER JOIN {l10n_packager_file} lf ON r.rid = lf.rid INNER JOIN {file_managed} f ON lf.fid = f.fid WHERE r.pid = :pid', array(':pid' => $project->pid));
  foreach ($result as $item) {
    // @todo: bad, bad harcoding of Drupal versioning schemes.
    $branch = preg_replace(($project->uri == 'drupal' ? '!^(\d\.)(.+)$!' : '!^(\d\.x-\d\.)(.+)$!'), '\1', $item->title) . 'x';
    $branches[$branch] = TRUE;
    $files[$item->language][$branch][$item->rid] = $item;
  }

  if (empty($branches)) {
    return t('No translation downloads found for %project.', array('%project' => $project->title));
  }

  ksort($branches);
  $languages = l10n_community_get_languages();
  $download_url = variable_get('l10n_packager_update_url', file_create_url(l10n_packager_directory()));

  $table = array();
  foreach ($languages as $langcode => $language) {
    $row = array();
    // Start off the row with the language name and code.
    $row[] = array(
      'data' => array(
        '#type' => 'link',
        '#title' => t($language->name),
        '#href' => 'translate/languages/' . $langcode,
      ));
    foreach (array_keys($branches) as $branch) {
      // Generate a cell for each major version.
      if (!empty($files[$langcode][$branch])) {
        krsort($files[$langcode][$branch]);
        $latest_item = array_shift($files[$langcode][$branch]);
        $cell = '<p>' . l($latest_item->title . ' (' . format_size($latest_item->filesize) . ')', $download_url . '/' . intval($branch) . '.x/' . $project->uri . '/' . $latest_item->filename) . '</p>';
        $cell .= '<p class="l10n-packager-meta">' . t('Generated: @generated', array('@generated' => format_date($latest_item->timestamp, 'custom', 'Y-M-d'))) . '</p>';
        $up_to_date = max($latest_item->checked, $latest_item->release_checked);
        if ($up_to_date > $latest_item->timestamp) {
          $cell .= '<p class="l10n-packager-meta">' . t('Up to date as of: @checked', array('@checked' => format_date($up_to_date, 'custom', 'Y-M-d'))) . '</p>';
        }
        $row[] = $cell;
      }
      else {
        $row[] = t('n/a');
      }
    }
    $table[] = $row;
  }

  $header = array_merge(array(t('Languages')), array_keys($branches));
  return theme('table', array('header' => $header, 'rows' => $table, 'attributes' => array('class' => array('l10n-packager-table'))));
}
