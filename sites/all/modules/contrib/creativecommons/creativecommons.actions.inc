<?php
/**
 * @file
 * Example Action Implementation.
 *
 * This does not provide functional code, but is an illustration of how actions can be created for
 * Quiz.
 *
 * This file was created with the intent of providing
 * some documentation for a good starting point when creating actions for utilization with
 * the quiz module.  Most of this documentation was taken from http://api.drupal.org, the API
 * documentation provided by Drupal.  For more understanding of these functions and hooks, please
 * see that Drupal API documentation for further reading.
 * @file
 */


/**
 * Implements hook_action_info().
 * An action consists of two or three parts:
 * (1) an action definition (returned by this hook),
 * (2) a function which does the action
 *   (which by convention is named module + '_' + description of what the function does + '_action'),
 * (3) and an optional form definition function that defines a configuration form
 *   (which has the name of the action with '_form' appended to it.)
 *
 */
function creativecommons_action_info() {
  $info['creativecommons_set_license_action'] = array(
    'type' => 'node',
    'label' => t('Set Creative Commons license'),
    'configurable' => TRUE,
    'triggers' => array('any' => TRUE),
  );

  return $info;
}

/**
 * @todo Please document this function.
 * @see http://drupal.org/node/1354
 */
function creativecommons_set_license_action(&$node, $context) {
  $license = $context['license_object'];
  $license->save($node->nid, 'update');
}

/**
 * Form for configuring the 'set license' action.
 *
 * @param array $context
 * @return $form
 */
function creativecommons_set_license_action_form($context) {
  //$form = creativecommons_node_form(NULL, NULL);
  //return $form;
}

/**
 * @todo Please document this function.
 * @see http://drupal.org/node/1354
 */
function creativecommons_set_license_action_validate($context) {
  // FIXME do some validation
}

/**
 * Custom submit handler when creating a new action for a playing card.
 *
 * @param array $form
 * @param array $form_state
 * @return array
 */
function creativecommons_set_license_action_submit($form, $form_state) {
  $license_uri = $form_state['values']['creativecommons']['select_license_form']['cc_license_uri'];
  $metadata = $form_state['values']['creativecommons']['metadata'];
  $cc = new creativecommons_license($license_uri, $metadata);
  $context['license_object'] = $cc;

  return $context;
}
