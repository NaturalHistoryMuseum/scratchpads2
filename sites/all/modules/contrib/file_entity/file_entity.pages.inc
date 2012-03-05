<?php
/**
 * @file
 * Supports file operations including View, Edit, and Delete.
 */

/**
 * Menu callback; view a single file entity.
 */
function file_entity_view_page($file) {
  // @todo Implement granular editorial access: http://drupal.org/node/696970.
  //   In the meantime, protect information about private files from being
  //   discovered by unprivileged users. File IDs are autoincrement, so one can
  //   attempt discovery by trying to access different media/ID paths. See also
  //   media_browser_list(). This logic potentially belongs within
  //   media_access(), but that would require extending that function's
  //   signature to accept a $file paramter, and this is temporary code anyway.
  if (!user_access('administer files') && (file_uri_scheme($file->uri) === 'private')) {
    return MENU_ACCESS_DENIED;
  }

  drupal_set_title($file->filename);

  $uri = entity_uri('file', $file);
  // Set the file path as the canonical URL to prevent duplicate content.
  drupal_add_html_head_link(array('rel' => 'canonical', 'href' => url($uri['path'], $uri['options'])), TRUE);
  // Set the non-aliased path as a default shortlink.
  drupal_add_html_head_link(array('rel' => 'shortlink', 'href' => url($uri['path'], array_merge($uri['options'], array('alias' => TRUE)))), TRUE);

  return file_view($file, 'full');
}

/**
 * Page callback: Form constructor for the file edit form.
 *
 * Path: file/%file/edit
 *
 * @param object $file
 *   A file object from file_load().
 *
 * @see file_entity_menu()
 *
 * @todo Rename this form to file_edit_form to ease into core.
 */
function file_entity_edit($form, &$form_state, $file) {
  drupal_set_title(t('<em>Edit @type</em> @title', array('@type' => $file->type, '@title' => $file->filename)), PASS_THROUGH);

  $form_state['file'] = $file;

  $form['#attributes']['class'][] = 'file-form';
  if (!empty($file->type)) {
    $form['#attributes']['class'][] = 'file-' . $file->type . '-form';
  }

  // Basic file information.
  // These elements are just values so they are not even sent to the client.
  foreach (array('fid', 'type', 'uid', 'timestamp') as $key) {
    $form[$key] = array(
      '#type' => 'value',
      '#value' => isset($file->$key) ? $file->$key : NULL,
    );
  }

  $form['filename'] = array(
    '#type' => 'textfield',
    '#title' => t('Name'),
    '#default_value' => $file->filename,
    '#required' => TRUE,
    '#maxlength' => 255,
    '#weight' => -10,
  );

  $form['preview'] = array(
    '#theme' => 'file_link',
    '#file' => $file,
    '#weight' => -5,
  );

  // Add the buttons.
  $form['actions'] = array('#type' => 'actions');
  $form['actions']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save'),
    '#weight' => 5,
    '#submit' => array('file_entity_edit_submit'),
  );
  $form['actions']['delete'] = array(
    '#type' => 'submit',
    '#value' => t('Delete'),
    '#weight' => 10,
    '#submit' => array('file_entity_edit_delete_submit'),
  );
  $form['actions']['cancel'] = array(
    '#type' => 'link',
    '#title' => t('Cancel'),
    '#href' => isset($_GET['destination']) ? $_GET['destination'] : 'file/' . $file->fid,
    '#weight' => 15,
  );

  field_attach_form('file', $file, $form, $form_state);

  return $form;
}

/**
 * Form validation handler for file_entity_edit().
 */
function file_entity_edit_validate($form, &$form_state) {
  entity_form_field_validate('file', $form, $form_state);
}

/**
 * Form submission handler for the 'Save' button for file_entity_edit().
 */
function file_entity_edit_submit($form, &$form_state) {
  $file = $form_state['file'];
  entity_form_submit_build_entity('file', $file, $form, $form_state);
  file_save($file);
  $form_state['redirect'] = 'file/' . $file->fid;
}

/**
 * Form submission handler for the 'Delete' button for file_entity_edit().
 */
function file_entity_edit_delete_submit($form, &$form_state) {
  $fid = $form_state['values']['fid'];
  $destination = array();
  if (isset($_GET['destination'])) {
    $destination = drupal_get_destination();
    unset($_GET['destination']);
  }
  $form_state['redirect'] = array('file/' . $fid . '/delete', array('query' => $destination));
}

/**
 * Page callback: Form constructor for the file deletion confirmation form.
 *
 * Path: file/%file/delete
 *
 * @param object $file
 *   A file object from file_load().
 *
 * @see file_entity_menu()
 */
function file_delete_form($form, &$form_state, $file) {
  $form_state['file'] = $file;

  $form['fid'] = array(
    '#type' => 'value',
    '#value' => $file->fid,
  );

  $description = t('This action cannot be undone.');
  if ($references = file_usage_list($file)) {
    $description .= ' ' . t('This file is currently in use and may cause problems if deleted.');
  }

  return confirm_form($form,
    t('Are you sure you want to delete the file %title?', array(
      '%title' => entity_label('file', $file),
    )),
    'file/' . $file->fid,
    $description,
    t('Delete')
  );
}

/**
 * Form submission handler for file_delete_form().
 */
function file_delete_form_submit($form, &$form_state) {
  if ($form_state['values']['confirm'] && $file = file_load($form_state['values']['fid'])) {
    // Use file_delete_multiple() rather than file_delete() since we want to
    // avoid unwanted validation and usage checking.
    file_delete_multiple(array($file->fid));
    $args = array(
      '@type' => file_type_get_name($file),
      '%title' => entity_label('file', $file),
    );
    watchdog('file', '@type: deleted %title.', $args);
    drupal_set_message(t('@type %title has been deleted.', $args));
  }

  $form_state['redirect'] = '<front>';
}

/**
 * Form constructor for file deletion confirmation form.
 *
 * @param array $files
 *   An array of file objects.
 *
 * @todo Rename this form to file_delete_form to ease into core.
 */
function file_multiple_delete_form($form, &$form_state, array $files) {
  $form['files'] = array(
    '#prefix' => '<ul>',
    '#suffix' => '</ul>',
    '#tree' => TRUE,
  );

  $files_have_usage = FALSE;
  foreach ($files as $fid => $file) {
    $title = entity_label('file', $file);
    $usage = file_usage_list($file);
    if (!empty($usage)) {
      $files_have_usage = TRUE;
      $title = t('@title (in use)', array('@title' => $title));
    }
    else {
      $title = check_plain($title);
    }
    $form['files'][$fid] = array(
      '#type' => 'hidden',
      '#value' => $fid,
      '#prefix' => '<li>',
      '#suffix' => $title . "</li>\n",
    );
  }

  $form['operation'] = array(
    '#type' => 'hidden',
    '#value' => 'delete',
  );

  $description = t('This action cannot be undone.');
  if ($files_have_usage) {
    $description .= ' ' . t('Some of the files are currently in use and may cause problems if deleted.');
  }

  return confirm_form(
    $form,
    format_plural(count($files), 'Are you sure you want to delete this file?', 'Are you sure you want to delete these files?'),
    'admin/content/file',
    $description,
    t('Delete')
  );
}

/**
 * Form submission handler for file_multiple_delete_form().
 */
function file_multiple_delete_form_submit($form, &$form_state) {
  if ($form_state['values']['confirm'] && $fids = array_keys($form_state['values']['files'])) {
    file_delete_multiple($fids);
    $count = count($fids);
    watchdog('file', 'Deleted @count files.', array('@count' => $count));
    drupal_set_message(format_plural($count, 'Deleted one file.', 'Deleted @count files.'));
  }
  $form_state['redirect'] = 'admin/content/file';
}


/**
 * Page callback for the file edit form.
 *
 * @deprecated
 *   Use drupal_get_form('file_entity_edit')
 */
function file_entity_page_edit($file) {
  return drupal_get_form('file_entity_edit', $file);
}

/**
 * Page callback for the file deletion confirmation form.
 *
 * @deprecated
 *   Use drupal_get_form('file_delete_form')
 */
function file_entity_page_delete($file) {
  return drupal_get_form('file_delete_form');
}
