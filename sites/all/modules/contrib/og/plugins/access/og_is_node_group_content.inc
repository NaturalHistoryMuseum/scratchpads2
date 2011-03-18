<?php


/**
 * @file
 * Plugin to provide access control based on existence of a specified context.
 */

$plugin = array(
  'title' => t("OG: Node is a group content"),
  'description' => t('Control access by whether or not a node is of type "Group".'),
  'callback' => 'og_is_node_group_content_check',
  'settings form' => 'og_is_node_group_content_settings',
  'summary' => 'og_is_node_group_content_summary',
  'required context' => new ctools_context_required(t('Node'), 'node'),
  'defaults' => array('is_group' => TRUE),
);

/**
 * Settings form
 */
function og_is_node_group_content_settings($form, &$form_state, $conf) {
  $form['settings']['is_group'] = array(
    '#type' => 'radios',
    '#description' => t('Check to see if the node is of type "Group".'),
    '#options' => array(TRUE => t('Is "Group content"'), FALSE => t('Is not "Group content"')),
    '#default_value' => $conf['is_group'],
  );
  return $form;
}

/**
 * Check for access.
 */
function og_is_node_group_content_check($conf, $context) {
  // As far as I know there should always be a context at this point, but this
  // is safe.
  if (empty($context) || empty($context->data) || empty($context->data->type)) {
    return FALSE;
  }
  
  return og_is_group_content_type('node', $context->data->type);
}

/**
 * Provide a summary description based upon the specified context
 */
function og_is_node_group_content_summary($conf, $context) {
  if (!empty($conf['is_group'])) {
    return t('@identifier exists', array('@identifier' => $context->identifier));
  }
  else {
    return t('@identifier does not exist', array('@identifier' => $context->identifier));
  }
}
