<?php

/**
 * Plugin definition.
 */
$plugin = array(
  'title' => t('Content create links'),
  'description' => t('Crafted links to create content (nodes) for a given group.'),
  'required context' => new ctools_context_required(t('OG Group'), 'entity:group'),
  'category' => t('Organic groups'),
  'defaults' => array(
    'types' => array(),
  ),
);

/**
 * Render callback.
 */
function og_node_create_links_content_type_render($subtype, $conf, $args, $context) {
  if (empty($context->data)) {
    return FALSE;
  }

  $group = $context->data;
  $links = og_node_create_links($group->gid, '', !empty($conf['types']) ? $conf['types'] : NULL);
  if (!$links) {
    return FALSE;
  }

  $module = 'og';
  $block = new stdClass();
  $block->module = $module;
  $block->title = t('Content create links');

  $block->content = $links;
  return $block;
}

/**
 * Edit form.
 */
function og_node_create_links_content_type_edit_form($form, &$form_state) {
  $conf = $form_state['conf'];

  foreach (node_type_get_types() as $type) {
    if (og_is_group_content_type('node', $type->type)) {
      $options[$type->type] = check_plain($type->name);
    }
  }
  $form['types'] = array(
    '#title' => t('Restrict to content types'),
    '#type' => 'checkboxes',
    '#options' => $options,
    '#default_value' => $conf['types'],
    '#description' => t('If left empty, all possible content types are shown.'),
  );
  return $form;
}

/**
 * Edit form submit callback.
 */
function og_node_create_links_content_type_edit_form_submit($form, &$form_state) {
  $form_state['conf']['types'] = array_filter($form_state['values']['types']);
}
