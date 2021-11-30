<?php
/**
 * @file
 *
 * Page callbacks for Twitter Post module.
 */

/**
 * Settings form callback
 */
function twitter_post_admin_settings($form, &$form_state) {
  $form['twitter_post_types'] = array(
    '#title' => t('Node types'),
    '#type' => 'checkboxes',
    '#description' => t('Choose which node types should support posting to Twitter.'),
    '#options' => node_type_get_names(),
    '#default_value' => variable_get('twitter_post_types', array()),
  );

  $form['twitter_post_default_format'] = array(
    '#type' => 'textfield',
    '#title' => t('Default format string'),
    '#maxlength' => 140,
    '#description' => t('The given text will be used as a template for posting to Twitter.com. ' .
                        'The following token replacements are available: !url, !url-alias, !tinyurl, !title, and !user'),
    '#default_value' => variable_get('twitter_post_default_format', 'New post: !title !tinyurl'),
  );

  $form['twitter_post_default_value'] = array(
    '#type' => 'checkbox',
    '#title' => t('Post to Twitter by default'),
    '#default_value' => variable_get('twitter_post_default_value', 0),
    '#description' => t('When active, tweets will be automatically posted when the node is first published.'),
  );

  $form['twitter_post_on_node_update'] = array(
    '#type' => 'checkbox',
    '#title' => t('Post to Twitter on node update'),
    '#description' => t('When active, tweets will be posted to Twitter on node update; this can lead to apparently duplicate tweets, so may not be wanted.'),
    '#default_value' => variable_get('twitter_post_on_update', 0),
    '#states' => array(
      'visible' => array(
        'input[name="twitter_post_default_value"]' => array('checked' => TRUE),
      ),
    ),
  );

  return system_settings_form($form);
}
