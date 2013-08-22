<?php

/**
 * @file
 *   Localization packager module drush integration.
 */

/**
 * Implements hook_drush_command().
 *
 * @see drush_parse_command() for a list of recognized keys.
 *
 * @return
 *   An associative array describing your command(s).
 */
function l10n_packager_drush_command() {
  $items['l10n_packager'] = array(
    'callback' => 'l10n_packager_drush_update',
    'description' => "Check translations and refresh files for updated ones.",
  );
  return $items;
}

/**
 * Drush command callback
 */
function l10n_packager_drush_update() {
  list($checked, $updated, $time) = l10n_packager_check_updates();
  $vars = array(
    '!checkmax' => variable_get('l10n_packager_release_limit', 10),
    '!checked' => $checked,
    '!updated' => $updated,
    '!ms' => $time,
  );
  drush_print(dt("!ms ms for !checked releases/!updated files.", $vars));
}
