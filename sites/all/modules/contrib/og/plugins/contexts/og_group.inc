<?php

/**
 * @file
 *
 * Plugin to provide an OG group context.
 */

/**
 * Plugins are described by creating a $plugin array which will be used
 * by the system that includes this file.
 */
$plugin = array(
  'title' => t("OG group"),
  'description' => t('A single group object.'),
  'context' => 'og_context_create_group',
  'edit form' => 'og_context_group_settings_form',
  'defaults' => array('type' => 'select', 'uid' => ''),
  'keyword' => 'group',
  'context name' => 'og_group',
  'convert list' => 'og_context_group_convert_list',
  'convert' => 'og_context_group_convert',
  'convert default' => 'gid',
  'js' => array('misc/autocomplete.js'),
);

/**
 * It's important to remember that $conf is optional here, because contexts
 * are not always created from the UI.
 */
function og_context_create_group($empty, $data = NULL, $conf = FALSE) {
  $context = new ctools_context('og_group');
  $context->plugin = 'og_group';

  if ($empty) {
    return $context;
  }

  if (!empty($data)) {
    $data = og_get_group('group', $data->gid);
  }

  if (!empty($data)) {
    $context->data     = $data;
    $context->title    = isset($data->label) ? $data->label : t('Group ID @gid', array('@gid' => $data['gid']));
    $context->argument = $data->gid;
    return $context;
  }
}

function og_context_group_settings_form($form, &$form_state) {
  $conf = $form_state['conf'];

  $form['og_group'] = array(
    '#title' => t('Select a group'),
    '#type' => 'select',
  );

  if (!empty($conf['gid'])) {
    $group = og_get_group('group', $conf['gid']);
    $entity = $group->getEntity();
    if ($entity) {
      $form['og_group']['#description'] = t('Currently set to !link', array('!link' => entity_uri($group->entity_type, $entity)));
    }
  }

  $form['gid'] = array(
    '#type' => 'value',
    '#value' => $conf['gid'],
  );

  return $form;
}

/**
 * Validate a user.
 */
function og_context_group_settings_form_validate($form, &$form_state) {
  if ($form_state['values']['type'] != 'select') {
    return;
  }

  // TODO: Validate the autocomplete
}

function og_context_group_settings_form_submit($form, &$form_state) {
  $form_state['conf']['gid'] = $form_state['values']['gid'];
}

/**
 * Provide a list of replacements.
 */
function og_context_group_convert_list() {
  $list = array(
    'gid' => t('Group ID'),
    'entity_type' => t('Entity type'),
    'entity_id' => t('Entity ID'),
  	'label' => t('Group label'),
  );
  // TODO: Add token support.
  return $list;
}

/**
 * Convert a context into a string.
 */
function og_context_group_convert($context, $type) {
  switch ($type) {
    case 'gid':
      return $context->data->gid;
    case 'entity_type':
      return $context->data->entity_type;
    case 'entity_id':
      return $context->data->etid;
    case 'label':
      return $context->data->label;
  }
  // TODO: Add token support.
}