<?php

/**
 * @file
 * File hooks implemented by the File entity module.
 */

/**
 * Implements hook_file_presave().
 */
function file_entity_file_presave($file) {
  // Always ensure the filemime property is current.
  if (!empty($file->original) || empty($file->filemime)) {
    $file->filemime = file_get_mimetype($file->uri);
  }

  // Always update file type based on filemime.
  $file->type = file_get_type($file);

  field_attach_presave('file', $file);
}

/**
 * Implements hook_file_insert().
 */
function file_entity_file_insert($file) {
  // Ensure field data is saved since file_save() does not in Drupal 7.
  field_attach_insert('file', $file);

  // Clear any related field caches.
  file_entity_invalidate_field_caches($file);

  // Clear the page and block caches.
  cache_clear_all();
}

/**
 * Implement hook_file_update().
 */
function file_entity_file_update($file) {
  // Ensure field data is saved since file_save() does not in Drupal 7.
  field_attach_update('file', $file);

  // Clear any related field caches.
  file_entity_invalidate_field_caches($file);

  // Clear the page and block caches.
  cache_clear_all();
}

/**
 * Implements hook_file_delete().
 */
function file_entity_file_delete($file) {
  field_attach_delete('file', $file);

  // This is safe to call since the file's records from the usage table have
  // not yet been deleted.
  file_entity_invalidate_field_caches($file);

  // Clear the page and block caches.
  cache_clear_all();
}

/**
 * Implements hook_file_mimetype_mapping_alter().
 */
function file_entity_file_mimetype_mapping_alter(&$mapping) {
  // Fix the mime type mapping for ogg.
  // @todo Remove when http://drupal.org/node/1239376 is fixed in core (7.8).
  $new_mappings['ogg'] = 'audio/ogg';

  // Add support for m4v.
  // @todo Remove when http://drupal.org/node/1290486 is fixed in core (7.9).
  $new_mappings['m4v'] = 'video/x-m4v';

  // Add support for mka and mkv.
  // @todo Remove when http://drupal.org/node/1293140 is fixed in core.
  $new_mappings['mka'] = 'audio/x-matroska';
  $new_mappings['mkv'] = 'video/x-matroska';

  // Add support for weba, webm, and webp.
  // @todo Remove when http://drupal.org/node/1347624 is fixed in core.
  $new_mappings['weba'] = 'audio/webm';
  $new_mappings['webm'] = 'video/webm';
  $new_mappings['webp'] = 'image/webp';

  foreach ($new_mappings as $extension => $mime_type) {
    if (!in_array($mime_type, $mapping['mimetypes'])) {
      // If the mime type does not already exist, add it.
      $mapping['mimetypes'][] = $mime_type;
    }

    // Get the index of the mime type and assign the extension to that key.
    $index = array_search($mime_type, $mapping['mimetypes']);
    $mapping['extensions'][$extension] = $index;
  }
}

/**
 * Implements hook_file_operation_info().
 */
function file_entity_file_operation_info() {
  $info['delete'] = array(
    'label' => t('Delete selected files'),
    'callback' => 'file_entity_multiple_delete_confirm_operation',
    'confirm' => TRUE,
  );

  return $info;
}
