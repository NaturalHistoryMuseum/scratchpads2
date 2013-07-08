<?php

/**
 * @file
 *   Administrative functions for localization community UI.
 */

// = Settings form callback ====================================================

/**
 * Settings form callback.
 */
function l10n_community_settings_form($form, &$form_state) {
  $projects = l10n_server_get_projects();

  $form = array();
  $form['l10n_community_project_per_page'] = array(
    '#title' => t('Number of projects to display per page'),
    '#description' => t('Number of projects displayed per page in overviews. It is not advised to change this setting on live sites, as it changes what is displayed on each page, so links will break to existing project listings.'),
    '#type' => 'select',
    '#options' => drupal_map_assoc(array(4, 8, 10, 12, 14, 16, 18, 20, 22, 24, 26, 28, 30, 32, 34, 36, 38, 40)),
    '#default_value' => variable_get('l10n_community_project_per_page', 10),
  );
  $form['l10n_community_stats_enabled'] = array(
    '#title' => t('Enable statistics for projects'),
    '#description' => t('You can disable this option in case of heavy load on your server.'),
    '#type' => 'checkbox',
    '#default_value' => variable_get('l10n_community_stats_enabled', TRUE),
  );
  $form['l10n_community_import_user'] = array(
    '#title' => t('Account associated with bulk imports'),
    '#description' => t('The account associated with imports of Gettext files which have been translated by a team, so assigning the current user importing them would not be appropriate. Suggested username for this account is <em>Multiple contributors</em> to indicate the team nature.'),
    '#type' => 'textfield',
    '#default_value' => variable_get('l10n_community_import_user', ''),
    '#autocomplete_path' => 'user/autocomplete',
  );
  $form['l10n_community_highlighted_project'] = array(
    '#title' => t('Highlight project on the welcome page'),
    '#description' => t('Pick a project which will be highlighted on the welcome page, so progress of all translation teams can be seen instantly.'),
    '#default_value' => variable_get('l10n_community_highlighted_project', ''),
  );
  if (($count = count($projects)) <= 30) {
    // Radio box widget for as much as 5 projects, select widget for 5-30 projects.
    $form['l10n_community_highlighted_project']['#type'] = ($count <= 5 ? 'radios' : 'select');
    $form['l10n_community_highlighted_project']['#options'] = array('' => t('None'));
    foreach ($projects as $project) {
      // title used to conform to the autocomplete behavior.
      $form['l10n_community_highlighted_project']['#options'][$project->title] = $project->title;
    }
  }
  else {
    // Autocomplete field for more then 30 projects.
    $form['l10n_community_highlighted_project'] += array(
      '#type' => 'textfield',
      '#autocomplete_path' => 'translate/projects/autocomplete',
    );
    $form['l10n_community_highlighted_project']['#description'] .= ' ' . t('Can be set to empty string to not pick any project.');
  }
  return system_settings_form($form);
}
