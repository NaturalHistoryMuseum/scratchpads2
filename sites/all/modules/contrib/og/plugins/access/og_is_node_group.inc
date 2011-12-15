<?php


/**
 * @file
 * Plugin to provide access control based on existence of a specified context.
 */

$plugin = array(
  'title' => t("OG: Node is a group"),
  'description' => t('Control access by whether or not a node is of type "Group content".'),
  'callback' => 'og_is_node_group_check',
  'settings form' => 'og_is_node_group_settings',
  'summary' => 'og_is_node_group_summary',
  'required context' => new ctools_context_required(t('Node'), 'node'),
  'defaults' => array('is_group_content' => TRUE),
);

/**
 * Settings form
 */
function og_is_node_group_settings($form, &$form_state, $conf) {
  $form['settings']['is_group_content'] = array(
    '#type' => 'radios',
    '#description' => t('Check to see if the node is of type "Group content".'),
    '#options' => array(TRUE => t('Is "Group content"'), FALSE => t('Is not "Group content"')),
    '#default_value' => $conf['is_group_content'],
  );
  return $form;
}

/**
 * Check for access.
 */
function og_is_node_group_check($conf, $context) {
  // As far as I know there should always be a context at this point, but this
  // is safe.
  if (empty($context) || empty($context->data) || empty($context->data->nid)) {
    return FALSE;
  }

  return og_get_group('node', $context->data->nid);
}

/**
 * Provide a summary description based upon the specified context
 */
function og_is_node_group_summary($conf, $context) {
  if (!empty($conf['is_group_content'])) {
    return t('@identifier exists', array('@identifier' => $context->identifier));
  }
  else {
    return t('@identifier does not exist', array('@identifier' => $context->identifier));
  }
}
