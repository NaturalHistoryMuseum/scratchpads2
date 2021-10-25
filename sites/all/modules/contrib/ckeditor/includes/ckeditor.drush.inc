<?php

/**
 * CKEditor - The text editor for the Internet - http://ckeditor.com
 * Copyright (c) 2003-2013, CKSource - Frederico Knabben. All rights reserved.
 *
 * == BEGIN LICENSE ==
 *
 * Licensed under the terms of any of the following licenses of your
 * choice:
 *
 *  - GNU General Public License Version 2 or later (the "GPL")
 *    http://www.gnu.org/licenses/gpl.html
 *
 *  - GNU Lesser General Public License Version 2.1 or later (the "LGPL")
 *    http://www.gnu.org/licenses/lgpl.html
 *
 *  - Mozilla Public License Version 1.1 or later (the "MPL")
 *    http://www.mozilla.org/MPL/MPL-1.1.html
 *
 * == END LICENSE ==
 *
 * @file
 * Drush integration for the CKEditor module.
 */

/**
 * Implements hook_drush_command().
 */
function ckeditor_drush_command() {
  $items['ckeditor-download'] = array(
    'callback' => 'ckeditor_drush_download',
    'description' => dt('Downloads the required CKEditor library.'),
    'arguments' => array(
      'path' => dt('Optional. The path to the download folder. If omitted, Drush will use the default location (sites/all/libraries/ckeditor).'),
    ),
  );
  return $items;
}

/**
 * Downloads.
 */
function ckeditor_drush_download() {
  $args = func_get_args();
  $path = !empty($args[0]) ? $args[0] : drush_get_context('DRUSH_DRUPAL_ROOT') . '/sites/all/libraries/ckeditor';

  $zip_url = 'https://download.cksource.com/CKEditor/CKEditor/CKEditor%20' . CKEDITOR_LATEST . '/';
  $file_name = 'ckeditor_' . CKEDITOR_LATEST . '_full.zip';

  $file = drush_download_file($zip_url . $file_name, $file_name, 3600);
  if (empty($file)) {
    drush_log(dt('Drush was unable to download CKEditor !version to !path from the !url', array(
      '!version' => CKEDITOR_LATEST,
      '!path' => $path,
      '!url' => $zip_url . $file_name
    )), 'error');

    return FALSE;
  }

  $tmp = drush_tempdir();
  if (!drush_tarball_extract($file, $tmp)) {
    drush_log(dt('CKEditor was not extracted to !path.', array('!path' => $path)), 'error');

    return FALSE;
  }

  drush_move_dir($tmp . '/ckeditor', $path, TRUE);

  drush_log(dt('CKEditor !version was downloaded to !path.', array('!version' => CKEDITOR_LATEST, '!path' => $path)), 'success');

  return TRUE;
}

/**
 * Implements drush_MODULE_post_COMMAND().
 */
function drush_ckeditor_post_enable() {
  $modules = func_get_args();
  if (in_array('ckeditor', $modules) && !drush_get_option('skip')) {
    ckeditor_drush_download();
  }
}
