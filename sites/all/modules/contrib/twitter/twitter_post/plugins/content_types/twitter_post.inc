<?php
/**
 * @file
 * Display the node form's Twitter fieldset in a Panels pane.
 */

$plugin = array(
  'single' => TRUE,
  'title' => t('Post to Twitter'),
  'icon' => 'icon_node_form.png',
  'description' => t('Optionally post a message to Twitter announcing the node.'),
  'required context' => new ctools_context_required(t('Form'), 'node_form'),
  'category' => t('Form'),
);

/**
 * CTools content type callback to output this pane.
 */
function twitter_post_twitter_post_content_type_render($subtype, $conf, $panel_args, &$context) {
  $block = new stdClass();

  $block->title = t('Post to Twitter');
  $block->module = 'twitter_post';
  $block->delta = 'twitter_post';

  if (isset($context->form)) {
    if (isset($context->form['twitter'])) {
      $block->content['twitter'] = $context->form['twitter'];
      unset($block->content['twitter']['#pre_render']);
      unset($block->content['twitter']['#theme_wrappers']);
      $block->content['twitter']['#type'] = '';

      // Set access to false on the original rather than removing so that
      // vertical tabs doesn't clone it. I think this is due to references.
      $context->form['twitter']['#access'] = FALSE;
    }
  }
  else {
    $block->content = t('Post to Twitter.');
  }
  return $block;
}

/**
 * CTools content type callback to get the title of this pane.
 */
function twitter_post_twitter_post_content_type_admin_title($subtype, $conf, $context) {
  if (isset($context->identifier)) {
    $message = t('"@s" node form "Post to Twitter" options', array('@s' => $context->identifier));
  }
  else {
    $message = t('Node form "Post to Twitter" options');
  }
  return $message;
}

/**
 * CTools content type callback to display an edit form for this pane.
 */
function twitter_post_twitter_post_content_type_edit_form($form, &$form_state) {
  // Provide a blank form so we have a place to have context setting.
  return $form;
}

/**
 * CTools content type callback to provide extra information about this pane.
 */
function twitter_post_twitter_post_content_type_admin_info($type, $subtype, $conf, $context = NULL) {
  $output = new stdClass();
  $output->title = t('Post to Twitter');
  $output->content = t('Shows the "Post to Twitter" fields on this node form, if the user has the necessary permission, and either they have added a Twitter account to their site account or there are some Twitter accounts added to the site which are marked as being "Global".');

  return $output;
}
