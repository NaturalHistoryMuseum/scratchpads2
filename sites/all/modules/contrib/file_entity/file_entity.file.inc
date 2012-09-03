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

  // Get and store image dimensions.
  // @todo We should fetch image dimensions in file_entity_file_presave and
  // then save them in this hook.
  file_entity_image_dimensions($file, TRUE);
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

  // Reset the image dimensions for a file.
  // @todo We should fetch image dimensions in file_entity_file_presave and
  // then save them in this hook.
  file_entity_image_dimensions($file, TRUE);

  if ($file->type == 'image' && module_exists('image')) {
    // If the image dimensions have changed, update any image field references
    // to this file and flush image style derivatives.
    if ($file->image_dimensions != $file->original->image_dimensions) {
      _file_entity_update_image_field_dimensions($file);
    }

    // Flush image style derivatives whenever an image is updated.
    image_path_flush($file->uri);
  }
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

  // Delete image dimensions from the {image_dimensions} table
  db_query('DELETE FROM {image_dimensions} WHERE fid = :fid', array(':fid' => $file->fid));
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

/**
 * Implements hook_file_load().
 */
function file_entity_file_load($files) {
  // Load images dimensions already in the {image_dimensions} table.
  $result = db_query('SELECT * FROM {image_dimensions} id WHERE id.fid IN (:fids)', array(':fids' => array_keys($files)));
  foreach ($result as $record) {
    $files[$record->fid]->image_dimensions = array(
      'width' => $record->width,
      'height' => $record->height,
    );
  }
  // Retrieve any missing images dimensions.
  foreach ($files as $file) {
    file_entity_image_dimensions($file, FALSE);
  }
}

/**
 * Retrieve the dimensions of an image file and store them in the
 * {image dimensions} table.
 *
 * @param $file
 *   A file object.
 *
 * @param $force
 *   TRUE if the image dimensions should always be loaded from the actual file
 *   even if $file->image_dimensions is already set.
 *
 * @return
 *   The image dimensions as an array with the 'width' and 'height' properties.
 *   The array is also added to $file as its image_dimensions property. If the
 *   image dimensions cannot be read, the 'width' and 'height' properties will
 *   be NULL. If $file is either empty or not an image file, FALSE is returned.
 */
function file_entity_image_dimensions($file, $force = FALSE) {
  // Prevent PHP notices when trying to read empty files.
  // @see http://drupal.org/node/681042
  if (!$file->filesize) {
    return;
  }

  // Do not bother proceeding if this file does not have an image mime type.
  if (strpos($file->filemime, 'image/') !== 0) {
    return;
  }

  // Return the existing $file->image_dimensions unless a reload is forced.
  if (!$force && isset($file->image_dimensions)) {
    return $file->image_dimensions;
  }

  // We have a non-empty image file.
  $image_info = image_get_info($file->uri);
  if ($image_info) {
    $file->image_dimensions = array(
      'width' => $image_info['width'],
      'height' => $image_info['height'],
    );
    db_merge('image_dimensions')
      ->key(array('fid' => $file->fid))
      ->fields(array(
        'width' => $file->image_dimensions['width'],
        'height' => $file->image_dimensions['height'],
      ))
      ->execute();
  }
  else {
    // Fallback to NULL values.
    $file->image_dimensions = array(
      'width' => NULL,
      'height' => NULL,
    );
  }
}

/**
 * Update the image dimensions stored in any image fields for a file.
 *
 * @param object $file
 *   A file object that is an image.
 *
 * @see http://drupal.org/node/1448124
 */
function _file_entity_update_image_field_dimensions($file) {
  // Find all image field enabled on the site.
  $image_fields = array();
  foreach (field_info_fields() as $field) {
    if ($field['type'] == 'image') {
      $image_fields[] = $field['field_name'];
    }
  }

  foreach ($image_fields as $image_field) {
    $query = new EntityFieldQuery();
    $query->fieldCondition($image_field, 'fid', $file->fid);
    $results = $query->execute();

    foreach ($results as $entity_type => $entities) {
      $entities = entity_load($entity_type, array_keys($entities));
      foreach ($entities as $entity) {
        foreach ($entity->{$image_field} as $langcode => $items) {
          foreach ($items as $delta => $item) {
            if ($item['fid'] == $file->fid) {
              $entity->{$image_field}[$langcode][$delta]['width'] = $file->image_dimensions['width'];
              $entity->{$image_field}[$langcode][$delta]['height'] = $file->image_dimensions['height'];
            }
          }
        }

        // Ensure that file_field_update() will not trigger additional usage.
        unset($entity->revision);
        field_attach_update($entity_type, $entity);
      }
    }
  }
}
