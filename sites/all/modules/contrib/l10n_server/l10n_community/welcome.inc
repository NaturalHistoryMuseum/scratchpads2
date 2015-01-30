<?php

/**
 * @file
 *   Introduction page for an l10n_community setup.
 */

/**
 * Introduction page for an l10n_community setup.
 *
 * This page is based on the admin page layout from system.module,
 * so we are using classes and concepts from there. This might not
 * fit perfectly with all listings, so I bended some stuff more to comply.
 */
function l10n_community_welcome_page() {
  if (!variable_get('l10n_community_stats_enabled', TRUE)) {
    return t('Statistics for projects have been disabled.');
  }
  elseif ($project = l10n_community_get_highlighted_project()) {

    // Similar to l10n_community_language_progress_for_project() with simpler
    // output more suitable for a welcoming overview.
    module_load_include('inc', 'l10n_community', 'pages');
    drupal_set_title(t('Translations'));

    $languages = l10n_community_get_languages();
    $num_source = l10n_community_get_string_count('project', $project->pid);
    $string_counts = l10n_community_get_string_count('languages', $project->pid);

    $table_rows = array();
    foreach ($languages as $langcode => $language) {
      if (empty($language->plurals)) {
        $table_rows[] = array(
          array(
            'data' => t($language->name),
            'sortdata' => t($language->name),
            'class' => array('rowhead'),
          ),
          array(
            'data' => t('Uninitialized plural formula. Please set up the plural formula in <a href="@language-config">the langauge configuration</a> or alternatively <a href="@import-url">import a valid interface translation</a> for Drupal in this language.', array('@import-url' => url('admin/structure/translate/import'), '@language-config' => url('admin/config/language'))),
            'class' => array('error'),
          ),
          array('data' => ''),
        );
      }
      else {
        $stats = l10n_community_get_stats($langcode);
        $progress = array_shift(theme(
                      'l10n_community_progress_columns',
                      array(
                        'sum' => $num_source,
                        'translated' => @$string_counts[$langcode]['translations'],
                        'has_suggestion' => @$string_counts[$langcode]['suggestions']
                      )
                    ));
        $table_rows[] = array(
          array(
            'data' => l(t($language->name), 'translate/languages/' . $langcode),
            'sortdata' => t($language->name),
            'class' => array('rowhead'),
          ),
          array(
            'data' => $progress,
            'sortdata' => ($num_source == 0 ? 0 : round(@$string_counts[$langcode]['translations'] / $num_source * 100, 2)),
          ),
          array(
            'data' => $stats['users'],
            'sortdata' => $stats['users'],
          ),
        );
      }
    }

    if (!empty($_GET['sort']) && !empty($_GET['order'])) {
      usort($table_rows, 'l10n_server_sort_by_column_language');
    }

    $header = array(
      array('data' => t('Language'), 'field' => 'language', 'class' => array('rowhead')),
      array('data' => t('@project progress', array('@project' => $project->title)), 'field' => 'progress'),
      array('data' => t('Contributors'), 'field' => 'contributors'),
    );
    $output = theme(
      'table',
      array(
        'header' => $header,
        'rows' => $table_rows,
        'attributes' => array('class' => array('l10n-community-overview l10n-community-highlighted'))
      ));

    $projects = l10n_server_get_projects();
    $project_count = count($projects);
    if ($project_count > 1) {
      $output .= '<p>' . format_plural($project_count, '@project is just one of the <a href="@projects">@count projects translated on this server</a>. Check them out.', '@project is just one of the <a href="@projects">@count projects translated on this server</a>. Check them out.', array('@project' => $project->title, '@projects' => url('translate/projects'))) . '<p>';
    }

  }
  else {
    $output = t('No highlighted project set.');
  }
  return $output;
}

/**
 * Form definition for jumping to languages or projects
 *
 * @see l10n_community_block_view
 */
function l10n_community_pick_go($form, &$form_state) {
  $form = array();
  $languages = l10n_community_get_languages();
  $projects = l10n_server_get_projects();

  if (empty($form_state['values']['project'])) {
    if (!empty($_GET['project']) && isset($projects[$_GET['project']])) {
      // Set default project based on possible URL argument (eg. used on downloads page).
      $form_state['values']['project'] = $projects[$_GET['project']]->title;
    }
    else {
      // Set default if it was not defined.
      $form_state['values']['project'] = '';
    }
  }

  // Drop languages without a plural formula from the options.
  $language_list = array();
  foreach ($languages as $langcode => $language) {
    if (!empty($language->plurals)) {
      $language_list[$langcode] = t($language->name);
    }
  }

  if (!count($language_list) || !count($projects)) {
    $pconfig_tip = '';
    if (!module_exists('l10n_pconfig')) {
      $pconfig_tip = ' ' . t('The <a href="@pconfig">Plural formula configurator</a> module is recommended for plural formula setup.', array('@pconfig' => 'http://drupal.org/project/l10n_pconfig'));
    }
    $form['dummy'] = array(
      '#type' => 'markup',
      '#markup' => t('No languages with plural formulas defined or no projects scanned yet. Check the <a href="@languages">configuration of languages</a>, <a href="@connectors">list of connectors</a> and <a href="@projects">projects</a>.', array('@languages' => url('admin/config/regional/language'), '@connectors' => url('admin/l10n_server/connectors'), '@projects' => url('admin/l10n_server/projects'))) . $pconfig_tip,
    );
    return $form;
  }

  $default_language = '';
  if (module_exists('l10n_groups')) {
    // We are dealing with a groups based permission model.
    $form['groups'] = l10n_groups_pick_go();
    if (isset($form['groups']['#l10n_groups']) && is_array($form['groups']['#l10n_groups'])) {
      // Pick the first language as default. Most people are only member of one
      // language, so this should be a safe default.
      $default_language = current($form['groups']['#l10n_groups']);
    }
  }

  if (count($language_list) > 1) {
    // If more then one language.
    $form['langcode'] = array(
      // For three or less languages, display radio buttons, select otherwise.
      '#type' => (count($language_list) <= 3) ? 'radios' : 'select',
      '#title' => t('Pick a language'),
      '#options' => array('' => t('All')) + $language_list,
      '#description' => t('Alternatively you can <a href="@explore">explore languages</a>.', array('@explore' => url('translate/languages'))),
      '#default_value' => $default_language,
    );
  }
  else {
    // Only one language, store as form value.
    list($language_code, $language_name) = each($language_list);
    $form['langcode'] = array(
      '#type' => 'value',
      '#value' => $language_code,
    );
  }

  if (count($language_list) > 1) {
    $projects_title = l10n_community_access('browse translations') ? t('And/or pick a project') : t('Or pick a project');
  }
  else {
    $projects_title = t('Pick a project for @language translation', array('@language' => $language_name));
  }
  $form['project'] = array(
    '#title' => $projects_title,
    '#description' => t('Alternatively you can <a href="@explore">explore projects</a>.', array('@explore' => url('translate/projects'))),
    '#default_value' => $form_state['values']['project'],
  );
  if (($count = count($projects)) <= 30) {
    // Radio box widget for as much as 5 projects, select widget for 5-30 projects.
    $form['project']['#type'] = ($count <= 5 ? 'radios' : 'select');
    $form['project']['#options'] = array('' => t('All'));
    foreach ($projects as $project) {
      // title used to conform to the autocomplete behavior.
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

  // Either we have languages or projects, so we can jump.
  $form['submit'] = array(
    '#type' => 'submit',
    '#prefix' => '<div class="clearing"></div>',
    '#value' => t('Go there'),
  );
  return $form;
}

/**
 * Validation handler for l10n_community_pick_go()
 */
function l10n_community_pick_go_validate($form, &$form_state) {
  if (empty($form_state['values']['project']) && empty($form_state['values']['langcode'])) {
    // We require at least one to be selected to be effective.
    form_set_error('langcode', t('At least select a language or a project.'));
  }
}

/**
 * Submission handler for l10n_community_pick_go().
 *
 * Redirects the user to the selected project page with language selection
 * if both are provided, or to the project page with project only or the
 * language page with language only.
 */
function l10n_community_pick_go_submit($form, &$form_state) {
  if (!empty($form_state['values']['project']) && ($uri = l10n_community_project_uri_by_title($form_state['values']['project']))) {
    // Project surely selected, possibly with language too.
    if (!empty($form_state['values']['langcode'])) {
      if (l10n_community_access('browse translations')) {
        drupal_goto('translate/languages/' . $form_state['values']['langcode'] . '/translate', array('query' => array('project' => '\\' . $uri)));
      }
      else {
        // Both language and project set, no access to browse > go to project.
        drupal_goto('translate/projects/' . $uri);
      }
    }
    else {
      // Only project set.
      drupal_goto('translate/projects/' . $uri);
    }
  }
  elseif (!empty($form_state['values']['langcode'])) {
    // Only language set.
    drupal_goto('translate/languages/' . $form_state['values']['langcode']);
  }
}
