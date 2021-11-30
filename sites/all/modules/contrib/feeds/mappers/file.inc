<?php

/**
 * @file
 * On behalf implementation of Feeds mapping API for file.module and
 * image.module.
 */

/**
 * Flag for dealing with existing files: Replace the existing file if it is
 * different. Do nothing if the new file is exactly the same as the existing
 * file.
 */
define('FEEDS_FILE_EXISTS_REPLACE_DIFFERENT', 3);

/**
 * Flag for dealing with existing files: If the new file is different, rename
 * it by appending a number until the name is unique. Do nothing if the new file
 * is exactly the same as the existing file.
 */
define('FEEDS_FILE_EXISTS_RENAME_DIFFERENT', 4);

/**
 * Flag for dealing with existing files: Do nothing if a file with the same name
 * already exists.
 */
define('FEEDS_FILE_EXISTS_SKIP', 5);

/**
 * Implements hook_feeds_processor_targets().
 */
function file_feeds_processor_targets($entity_type, $bundle_name) {
  $targets = array();

  foreach (field_info_instances($entity_type, $bundle_name) as $name => $instance) {
    $info = field_info_field($name);

    if (in_array($info['type'], array('file', 'image'))) {
      $targets[$name . ':uri'] = array(
        'name' => t('@label: URI', array('@label' => $instance['label'])),
        'callback' => 'file_feeds_set_target',
        'description' => t('The URI of the @label field.', array('@label' => $instance['label'])),
        'real_target' => $name,
        'summary_callbacks' => array('file_feeds_summary_callback'),
        'form_callbacks' => array('file_feeds_form_callback'),
      );

      // Keep the old target name for backwards compatibility, but hide it from
      // the UI.
      $targets[$name] = $targets[$name . ':uri'];
      $targets[$name]['deprecated'] = TRUE;

      if ($info['type'] == 'image') {
        $targets[$name . ':alt'] = array(
          'name' => t('@label: Alt', array('@label' => $instance['label'])),
          'callback' => 'file_feeds_set_target',
          'description' => t('The alt tag of the @label field.', array('@label' => $instance['label'])),
          'real_target' => $name,
        );
        $targets[$name . ':title'] = array(
          'name' => t('@label: Title', array('@label' => $instance['label'])),
          'callback' => 'file_feeds_set_target',
          'description' => t('The title of the @label field.', array('@label' => $instance['label'])),
          'real_target' => $name,
        );
      }
      elseif ($info['type'] === 'file') {
        $targets[$name . ':description'] = array(
          'name' => t('@label: Description', array('@label' => $instance['label'])),
          'callback' => 'file_feeds_set_target',
          'description' => t('The description of the @label field.', array('@label' => $instance['label'])),
          'real_target' => $name,
        );
      }
    }
  }

  return $targets;
}

/**
 * Callback for mapping file fields.
 */
function file_feeds_set_target(FeedsSource $source, $entity, $target, array $values, array $mapping) {
  $language = $mapping['language'];
  $mapping += array('file_exists' => FILE_EXISTS_RENAME);

  // Add default of uri for backwards compatibility.
  list($field_name, $sub_field) = explode(':', $target . ':uri');
  $info = field_info_field($field_name);

  if ($sub_field == 'uri') {

    foreach ($values as $k => $v) {
      if (!($v instanceof FeedsEnclosure)) {
        if (!empty($v) && is_string($v)) {
          $values[$k] = new FeedsEnclosure($v, file_get_mimetype($v));
        }
        else {
          // Set the value for FALSE rather than remove it to keep our deltas
          // correct.
          $values[$k] = FALSE;
        }
      }
    }

    if ($entity instanceof Entity) {
      $entity_type = $entity->entityType();
      $bundle = $entity->bundle();
    }
    else {
      $entity_type = $source->importer->processor->entityType();
      $bundle = $source->importer->processor->bundle();
    }
    $instance_info = field_info_instance($entity_type, $field_name, $bundle);

    // Determine file destination.
    // @todo This needs review and debugging.
    $data = array();
    if (!empty($entity->uid)) {
      $data[$entity_type] = $entity;
    }

    $destination = file_field_widget_uri($info, $instance_info, $data);
  }

  // Populate entity.
  $field = isset($entity->$field_name) ? $entity->$field_name : array($language => array());
  $delta = 0;
  foreach ($values as $v) {
    if ($info['cardinality'] == $delta) {
      break;
    }

    if (!isset($field[$language][$delta])) {
      $field[$language][$delta] = array();
    }

    switch ($sub_field) {
      case 'alt':
      case 'title':
      case 'description':
        $field[$language][$delta][$sub_field] = $v;
        break;

      case 'uri':
        $skip = FALSE;
        if ($v) {
          if ($mapping['file_exists'] == FEEDS_FILE_EXISTS_SKIP) {
            if (file_exists($destination . '/' . basename($v->getValue()))) {
              $skip = TRUE;
            }
            else {
              // We already know the file doesn't exist so we don't have to
              // worry about anything being renamed, but we do need a valid
              // replace value for file_save().
              $mapping['file_exists'] = FILE_EXISTS_RENAME;
            }
          }
          if ($mapping['file_exists'] == FEEDS_FILE_EXISTS_REPLACE_DIFFERENT) {
            if (file_exists($destination . '/' . basename($v->getValue())) && file_feeds_file_compare($v->getValue(), $destination . '/' . basename($v->getValue()))) {
              $skip = TRUE;
            }
            else {
              // Either the file doesn't exist or it does and it's different.
              $mapping['file_exists'] = FILE_EXISTS_REPLACE;
            }
          }
          if ($mapping['file_exists'] == FEEDS_FILE_EXISTS_RENAME_DIFFERENT) {
            if (file_exists($destination . '/' . basename($v->getValue())) && file_feeds_file_compare($v->getValue(), $destination . '/' . basename($v->getValue()))) {
              $skip = TRUE;
            }
            else {
              // Either the file doesn't exist or it does and it's different.
              $mapping['file_exists'] = FILE_EXISTS_RENAME;
            }
          }
          if ($skip) {
            // Create a new dummy feeds enclosure where the value is the file
            // already in the file system (it will be skipped by getFile()).
            $mapping['file_exists'] = FEEDS_FILE_EXISTS_SKIP;
            $existing_path = $destination . '/' . basename($v->getValue());
            $v = new FeedsEnclosure($existing_path, file_get_mimetype($existing_path));
          }
          try {
            $v->setAllowedExtensions($instance_info['settings']['file_extensions']);
            $field[$language][$delta] += (array) $v->getFile($destination, $mapping['file_exists']);
            // @todo: Figure out how to properly populate this field.
            $field[$language][$delta]['display'] = 1;
          }
          catch (Exception $e) {
            watchdog('feeds', check_plain($e->getMessage()));
          }
        }
        break;
    }

    $delta++;
  }

  $entity->$field_name = $field;
}

/**
 * Mapping configuration summary callback for file targets.
 */
function file_feeds_summary_callback($mapping, $target, $form, $form_state) {
  $mapping += array('file_exists' => FILE_EXISTS_RENAME);
  switch ($mapping['file_exists']) {
    case FILE_EXISTS_REPLACE:
      return t('Replace existing files');

    case FILE_EXISTS_RENAME:
      return t('Rename if file exists');

    case FEEDS_FILE_EXISTS_REPLACE_DIFFERENT:
      return t('Replace only if file exists, but is different');

    case FEEDS_FILE_EXISTS_RENAME_DIFFERENT:
      return t('Rename only if file exists, but is different');

    case FEEDS_FILE_EXISTS_SKIP:
      return t('Skip if file exists');
  }
}

/**
 * Mapping configuration form callback for file targets.
 */
function file_feeds_form_callback($mapping, $target, $form, $form_state) {
  $description = array(
    '#theme' => 'item_list',
    '#items' => array(
      t('Rename: files whose name is already in use are renamed.'),
      t('Replace: files on the site with the same name are replaced.'),
      t('Replace only if different: files on the site with the same name are replaced only if the file to import is different, in other cases the file will not be imported. Works only if the file to import is locally accessible.'),
      t('Rename only if different: files on the site with the same name are renamed only if the file to import is different, in other cases the file will not be imported. Works only if the file to import is locally accessible.'),
      t('Skip existing: files whose name is already in use are not imported.'),
    ),
  );

  return array(
    'file_exists' => array(
      '#type' => 'select',
      '#title' => t('Replacement method'),
      '#default_value' => !empty($mapping['file_exists']) ? $mapping['file_exists'] : FILE_EXISTS_RENAME,
      '#options' => array(
        FILE_EXISTS_RENAME => t('Rename'),
        FILE_EXISTS_REPLACE => t('Replace'),
        FEEDS_FILE_EXISTS_REPLACE_DIFFERENT => t('Replace only if different'),
        FEEDS_FILE_EXISTS_RENAME_DIFFERENT => t('Rename only if different'),
        FEEDS_FILE_EXISTS_SKIP => t('Skip existing'),
      ),
      '#description' => t('New files are always copied. Files that have a name that is already in use on the site are handled based on this setting.') . drupal_render($description) . t('Note that this setting has no effect when using the File (Field) Paths module.'),
    ),
  );
}

/**
 * Compares two files to determine if they are the same.
 *
 * @param string $file1
 *   The path to the first file to compare.
 *
 * @param string $file2
 *   The path to the second file to compare.
 *
 * @return bool
 *   TRUE if the files are the same.
 *   FALSE otherwise.
 */
function file_feeds_file_compare($file1, $file2) {
  // If the file size is different then assume they are different files.
  // However, remote files may return FALSE from filesize() so only compare
  // file sizes if both values are not empty.
  $filesize1 = filesize($file1);
  $filesize2 = filesize($file2);
  if ($filesize1 !== FALSE && $filesize2 !== FALSE && $filesize1 !== $filesize2) {
    return FALSE;
  }

  // File sizes are the same so check md5 hash of files.
  if (md5_file($file1) != md5_file($file2)) {
    return FALSE;
  }

  return TRUE;
}
