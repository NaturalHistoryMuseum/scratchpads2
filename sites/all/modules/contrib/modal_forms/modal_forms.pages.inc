<?php
/**
 * @file
 * Page callbacks for the modal_forms module.
 */

/*
 * A modal user login callback.
 */
function modal_forms_login($js = NULL) {
  // Fall back if $js is not set.
  if (!$js) {
    return drupal_get_form('user_login');
  }

  ctools_include('modal');
  ctools_include('ajax');
  $form_state = array(
    // 'title' => t('Log in'),
    'ajax' => TRUE,
  );

  $output = ctools_modal_form_wrapper('user_login', $form_state);
  if (!empty($form_state['executed'])) {
    // We'll just overwrite the form output if it was successful.
    $output = array();
    ctools_add_js('ajax-responder');
    $output[] = ctools_modal_command_dismiss(t('Login success'));
    if (isset($_GET['destination'])) {
      $output[] = ctools_ajax_command_redirect($_GET['destination']);
    }
    elseif(module_exists('login_destination')) {
      $destination = login_destination_get_destination('login');
      $output[] = ctools_ajax_command_redirect($destination['path']);
    }
    else {
      $output[] = ctools_ajax_command_reload();
    }
  }
  print ajax_render($output);
}

/**
 * A modal user register callback.
 */
function modal_forms_register($js = NULL) {
  // Fall back if $js is not set.
  if (!$js) {
    return drupal_get_form('user_register_form');
  }

  ctools_include('modal');
  ctools_include('ajax');
  $form_state = array(
    // 'title' => t('Create new account'),
    'ajax' => TRUE,
  );
  $output = ctools_modal_form_wrapper('user_register_form', $form_state);
  if (!empty($form_state['executed'])) {
    // We'll just overwrite the form output if it was successful.
    $output = array();
    ctools_add_js('ajax-responder');
    if (isset($_GET['destination'])) {
      $output[] = ctools_ajax_command_redirect($_GET['destination']);
    }
    elseif(module_exists('login_destination')) {
      $destination = login_destination_get_destination('login');
      $output[] = ctools_ajax_command_redirect($destination['path']);
    }
    else {
      $output[] = ctools_ajax_command_reload();
    }
  }
  print ajax_render($output);
}

/**
 * A modal user password callback.
 */
function modal_forms_password($js = NULL) {
  module_load_include('inc', 'user', 'user.pages');

  // Fall back if $js is not set.
  if (!$js) {
    return drupal_get_form('user_pass');
  }

  ctools_include('modal');
  ctools_include('ajax');
  $form_state = array(
    // 'title' => t('Request new password'),
    'ajax' => TRUE,
  );
  $output = ctools_modal_form_wrapper('user_pass', $form_state);
  if (!empty($form_state['executed'])) {
    // We'll just overwrite the form output if it was successful.
    $output = array();
    ctools_add_js('ajax-responder');
    if (isset($_GET['destination'])) {
      $output[] = ctools_ajax_command_redirect($_GET['destination']);
    }
    else {
      $output[] = ctools_ajax_command_reload();
    }
  }
  print ajax_render($output);
}

/**
 * A modal contact callback.
 */
function modal_forms_contact($js = NULL) {
  module_load_include('inc', 'contact', 'contact.pages');

  // Fall back if $js is not set.
  if (!$js) {
    return drupal_get_form('contact_site_form');
  }

  ctools_include('modal');
  ctools_include('ajax');
  $form_state = array(
    // 'title' => t('Contact'),
    'ajax' => TRUE,
  );
  $output = ctools_modal_form_wrapper('contact_site_form', $form_state);
  if (!empty($form_state['executed'])) {
    // We'll just overwrite the form output if it was successful.
    $output = array();
    ctools_add_js('ajax-responder');
    if (isset($_GET['destination'])) {
      $output[] = ctools_ajax_command_redirect($_GET['destination']);
    }
    else {
      $output[] = ctools_ajax_command_reload();
    }
  }
  print ajax_render($output);
}

/**
 * A modal comment callback.
 */
function modal_forms_comment_reply($js = NULL, $node, $pid = NULL) {
  $output = array();
  $comment = array(
    'pid' => $pid,
    'nid' => $node->nid,
  );

  if (!$js) {
    return drupal_get_form('comment_node_' . $node->type . '_form', (object) $comment);
  }

  ctools_include('modal');
  ctools_include('ajax');

  $form_state = array(
    'build_info' => array(
      'args' => array(
        (object) $comment,
      ),
    ),
    // 'title' => t('Comment'),
    'ajax' => TRUE,
    're_render' => FALSE,
    'no_redirect' => TRUE,
  );

  // Should we show the reply box?
  if ($node->comment != COMMENT_NODE_OPEN) {
    drupal_set_message(t('This discussion is closed: you can\'t post new comments.'), 'error');
    drupal_goto('node/' . $node->nid);
  }
  else {
    $output = drupal_build_form('comment_node_' . $node->type . '_form', $form_state);
    // Remove output bellow the comment.
    unset($output['comment_output_below']);
  }

  if (!$form_state['executed'] || $form_state['rebuild']) {
    $output = ctools_modal_form_render($form_state, $output);
  }
  else {
    // We'll just overwrite the form output if it was successful.
    $output = array();
    // @todo: Uncomment once http://drupal.org/node/1587916 is fixed.
    //if (is_array($form_state['redirect'])) {
    //  list($path, $options) = $form_state['redirect'];
    //  $output[] = ctools_ajax_command_redirect($path, 0, $options);
    //}
    if (isset($_GET['destination'])) {
      $output[] = ctools_ajax_command_redirect($_GET['destination']);
    }
    else {
      $output[] = ctools_ajax_command_reload();
    }
  }

  print ajax_render($output);
}

/**
 * Modal display of the node's webform.
 *
 * @param $node
 *   A node object.
 */
function modal_forms_view_webform($js = NULL, $node) {
  $output = array();

  // Fall back if $js is not set.
  if (!$js) {
    return drupal_get_form('webform_client_form_' . $node->nid, $node, FALSE);
  }

  ctools_include('modal');
  ctools_include('ajax');

  if (empty($node->webform['components'])) {
    // No webform or no components.
    $output[] = ctools_modal_command_display(t('Webform'), t('No webform found.'));
    print ajax_render($output);
    exit;
  }

  // Get webform defaults.
  $title = check_plain($node->title);
  $text = check_markup($node->webform['confirmation'], $node->webform['confirmation_format'], '', TRUE);
  $form_state = array(
    'title' => $title,
    'ajax' => TRUE,
  );

  // Prevent webform redirect.
  $GLOBALS['conf']['webform_blocks']['client-block-' . $node->nid]['pages_block'] = 1;
  $node->webform_block = TRUE;

  // Pass required parameters.
  $form_state['build_info']['args'] = array($node, FALSE);
  $output = ctools_modal_form_wrapper('webform_client_form_' . $node->nid, $form_state);

  if (!empty($form_state['executed'])) {
    if (!isset($form_state['storage'])) {
      ctools_add_js('ajax-responder');
      $output[] = ctools_modal_command_display($title, $text . ctools_ajax_text_button(t('Close'), 'modal_forms/nojs/dismiss', t('Close')));
      // @todo Add support for redirect. Require some magic.
      // Copied from webform_client_form_submit().
      // $redirect_url = trim($node->webform['redirect_url']);
      // $redirect_url = _webform_filter_values($redirect_url, $node, NULL, NULL, FALSE, TRUE);
      // $output[] = ctools_ajax_command_redirect($redirect_url);
    }
    else {
      // We are on a multistep form step.
      $form_state['executed'] = array();
      $output = ctools_modal_form_wrapper('webform_client_form_' . $node->nid, $form_state);
    }
  }

  print ajax_render($output);
}

/**
 * Closes modal windows.
 */
function modal_forms_dismiss($js = NULL) {
  if (!$js) {
    // we should never be here out of ajax context
    return MENU_NOT_FOUND;
  }

  ctools_include('modal');
  ctools_include('ajax');

  $output = array(ctools_modal_command_dismiss());

  print ajax_render($output);
}
