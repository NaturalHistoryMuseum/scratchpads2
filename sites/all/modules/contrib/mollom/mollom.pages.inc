<?php

/**
 * @file
 * Various non-administration page callbacks for the mollom module.
 */

/**
 * AJAX callback to retrieve a CAPTCHA.
 *
 * @param $type
 *   The new CAPTCHA type to retrieve, e.g. 'image' or 'audio'.
 * @param $form_build_id
 *   The internal form build id of the form to update the CAPTCHA for.
 * @param $mollom_session_id
 *   The last known Mollom session id contained in the form.
 *
 * @return
 *   A JSON array containing:
 *   - content: The HTML markup for the new CAPTCHA.
 *   - session_id: The Mollom session id for the new CAPTCHA.
 *
 * @todo Add error handling.
 */
function mollom_captcha_js($type, $form_build_id, $mollom_session_id) {
  $captcha = mollom_get_captcha($type, array('session_id' => $mollom_session_id));

  // Update cached session id in the cached $form_state.
  // We rely on native form caching of Form API to store our Mollom session
  // data. When retrieving a new CAPTCHA through this JavaScript callback, the
  // cached $form_state is not updated and still contains the Mollom session
  // data that was known when rendering the form. Since above XML-RPC requests
  // may return a new Mollom session id for the new CAPTCHA, the user would not
  // be able to successfully complete the CAPTCHA, because we would try to
  // validate the user's response in combination with the old/previous session
  // id. Therefore, we need to update the session id in the cached $form_state.
  // @todo Replace the entire CAPTCHA switch/refresh with new AJAX framework
  //   functionality.
  if (!empty($captcha['response']['session_id'])) {
    if ($cache = cache_get('form_state_' . $form_build_id, 'cache_form')) {
      $form_state = $cache->data;
      $form_state['mollom']['response']['session_id'] = $captcha['response']['session_id'];
      cache_set('form_state_' . $form_build_id, $form_state, 'cache_form', REQUEST_TIME + 21600);
      // After successfully updating the cache, replace the original session id.
      $mollom_session_id = $captcha['response']['session_id'];
    }
  }

  // Return new content and new session_id via JSON.
  $data = array(
    'content' => $captcha['markup'],
    'session_id' => $mollom_session_id,
  );
  drupal_json_output($data);
  drupal_exit();
}

/**
 * Form builder for report to Mollom form.
 *
 * @param $entity
 *   The entity type of the data to report, e.g. 'node' or 'comment'.
 * @param $id
 *   The entity id the data belongs to. If 'session' is passed as $entity, then
 *   $id is assumed to be a Mollom session_id, as returned by Mollom servers,
 *   which should only be used to report session data that was not stored for an
 *   entity in the database (such as contact form submissions).
 */
function mollom_report_form($form, &$form_state, $entity, $id) {
  $form['entity'] = array(
    '#type' => 'value',
    '#value' => $entity,
  );
  $form['id'] = array(
    '#type' => 'value',
    '#value' => $id,
  );
  // @todo "Delete" does not work for reporting mails to Mollom. In D7+, this
  //   form should be used solely for mails, as other entities are reported
  //   through existing delete confirmation forms instead. Perhaps there should
  //   be a dedicated form for reporting mails, as they are not really
  //   compatible with any of the standard processes either way.
  $form = confirm_form($form,
    t('Are you sure you want to delete and report the content as inappropriate?'),
    '<front>',
    t('This action cannot be undone.'),
    t('Delete'), t('Cancel')
  );
  mollom_data_delete_form_alter($form, $form_state);
  return $form;
}

/**
 * Form submit handler for mollom_report_form().
 */
function mollom_report_form_submit($form, &$form_state) {
  if ($form_state['values']['confirm']) {
    $entity = $form_state['values']['entity'];
    $id = $form_state['values']['id'];

    // Load the Mollom session data.
    if ($entity == 'session') {
      $data = new stdClass;
      $data->session_id = $id;
    }
    else {
      $data = mollom_data_load($entity, $id);
    }

    // Send feedback to Mollom, if we have session data.
    if (!empty($data->session_id) && !empty($form_state['values']['mollom']['feedback'])) {
      if (_mollom_send_feedback($data->session_id, $form_state['values']['mollom']['feedback'])) {
        drupal_set_message(t('The content was successfully reported as inappropriate.'));
      }
    }

    // Delete the content. The callback should take care of proper deletion and
    // cache clearing on its own.
    foreach (mollom_form_list() as $form_id => $info) {
      if (!isset($info['entity']) || $info['entity'] != $entity) {
        continue;
      }
      // If there is a 'report delete callback', invoke it.
      if (isset($info['report delete callback']) && function_exists($info['report delete callback'])) {
        $function = $info['report delete callback'];
        $function($entity, $id);
        break;
      }
    }

    $form_state['redirect'] = '<front>';
  }
}
