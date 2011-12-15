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
  return file_view($file, 'full');
}

/**
 * Menu callback; presents the Media editing form.
 */
function file_entity_page_edit($file) {
  drupal_set_title(t('<em>Edit @type</em> @title', array('@type' => $file->type, '@title' => $file->filename)), PASS_THROUGH);
  return drupal_get_form('file_entity_edit', $file);
}

/**
 * Form builder: Builds the edit file form.
 */
function file_entity_edit($form, $form_state, $file) {
  $form_state['file'] = $file;
  field_attach_form('file', $file, $form, $form_state);

  $form['preview'] = array(
    '#theme' => 'file_link',
    '#file' => $file,
  );

  // Add the buttons.
  $form['actions'] = array('#type' => 'actions');
  $form['actions']['delete'] = array(
    '#type' => 'submit',
    '#value' => t('Delete'),
    '#weight' => 15,
    '#submit' => array('file_entity_delete_submit'),
  );

  $form['actions']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save'),
    '#weight' => 5,
    '#submit' => array('file_entity_edit_submit'),
  );

  // Add internal file properties needed by media_edit_validate().
  foreach (array('fid', 'type') as $key) {
    $form[$key] = array('#type' => 'value', '#value' => $file->$key);
  }

  return $form;
}

/**
 * Form validation handler for the file entity edit form.
 */
function file_entity_edit_validate($form, &$form_state) {
  entity_form_field_validate('file', $form, $form_state);
}

/**
 * Form submit handler for the media submit form.
 */
function file_entity_edit_submit($form, &$form_state) {
  $file = $form_state['file'];
  entity_form_submit_build_entity('file', $file, $form, $form_state);
  file_save($file);
  $form_state['redirect'] = 'file/' . $file->fid;
}

/**
 * Menu callback; shows delete confirmation form.
 */
function file_entity_page_delete($file) {
  drupal_set_title(t('<em>Delete @type</em> @title', array('@type' => $file->type, '@title' => $file->filename)), PASS_THROUGH);
  // Don't bother showing the form if the item is in use, since we won't allow
  // them to delete it anyway.
  $references = file_usage_list($file);
  if (!empty($references)) {
    return t('The file %title is in use and cannot be deleted.', array('%title' => $file->filename));
  }
  else {
    $files = array($file->fid => $file->fid);
    return drupal_get_form('file_entity_multiple_delete_confirm', $files, 'file/' . $file->fid);
  }
}

/**
 * Confirm form for the request to delete files.
 *
 * @param array $files
 *   An array of file_ids to delete.
 */
function file_entity_multiple_delete_confirm($form, &$form_state, $files, $redirect_path = NULL) {
  $form['files'] = array('#tree' => TRUE);
  $form['file_titles'] = array('#theme' => 'item_list');

  $files = file_load_multiple($files);
  foreach ($files as $fid => $file) {
    $form['files'][$fid] = array(
      '#type' => 'value',
      '#value' => $fid,
    );
    $form['file_titles']['#items'][] = check_plain($file->filename);
  }
  $form['operation'] = array('#type' => 'hidden', '#value' => 'delete');

  $confirm_question = format_plural(count($files),
                                  'Are you sure you want to delete this file?',
                                  'Are you sure you want to delete these files?');

  return confirm_form($form,
    $confirm_question,
    $redirect_path,
    t('This action cannot be undone.'),
    t('Delete'),
    t('Cancel'));
}

/**
 * Attempt to delete files and notify the user of the result.
 */
function file_entity_multiple_delete_confirm_submit($form, &$form_state) {
  if ($form_state['values']['confirm']) {
    $files = file_load_multiple(array_keys($form_state['values']['files']));
    foreach ($files as $fid => $file) {
      $result = file_delete($file);
      if (is_array($result)) {
        drupal_set_message(t('The file @title is in use and cannot be deleted.', array('@title' => $file->filename)), 'warning');
      }
      elseif (!$result) {
        drupal_set_message(t('The file @title was not deleted due to an error.', array('@title' => $file->filename)), 'error');
      }
      else {
        $message = t('File @title was deleted', array('@title' => $file->filename));
        $form_state['redirect'] = user_access('administer files') ? 'admin/content/file' : '<front>';
        watchdog('file', $message);
        drupal_set_message($message);
      }
    }
  }
}

/**
 * Form submit handler for the Delete button on the media edit form.
 */
function file_entity_delete_submit($form, &$form_state) {
  $fid = $form_state['values']['fid'];
  $destination = array();
  if (isset($_GET['destination'])) {
    $destination = drupal_get_destination();
    unset($_GET['destination']);
  }
  $form_state['redirect'] = array('file/' . $fid . '/delete', array('query' => $destination));
}
