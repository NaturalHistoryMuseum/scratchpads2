<?php

/**
* Plugin definition.
*/
$plugin = array(
  'title' => t('OG Content create links'),
  'description' => t('Crafted links to create content (nodes) for a given group.'),
  'required context' => new ctools_context_required(t('Group'), 'entity:group'),
  'category' => t('Organic groups'),
);

/**
 * Render callback.
 */
function og_node_create_links_content_type_render($subtype, $conf, $args, $context) {
  if (empty($context->data)) {
    return FALSE;
  }

  $group = $context->data;
  $links = og_node_create_links($group->gid);
  if (!$links) {
    return FALSE;
  }

  $module = 'og';
  $block = new stdClass();
  $block->module = $module;
  $block->title = t('Content create links');

  $output = '';
  $output .= '<div class="' . str_replace('_', '-', $subtype) . '-wrapper">';

  $output .= drupal_render($links);

  $output .= '</div>';
  $block->content = $output;
  return $block;
}

function og_node_create_links_content_type_edit_form($form, &$form_state) {
  // provide a blank form so we have a place to have context setting.
  return $form;
}
