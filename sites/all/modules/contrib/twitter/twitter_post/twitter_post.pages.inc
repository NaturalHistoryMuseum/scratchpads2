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
    '#type' => 'checkboxes',
    '#title' => t('Node types'),
    '#options' => node_type_get_names(),
    '#default_value' => variable_get('twitter_post_types', array('story' => 'story', 'blog' => 'blog')),
  );

  $form['twitter_post_default_format'] = array(
    '#type' => 'textfield',
    '#title' => t('Default format string'),
    '#maxlength' => 140,
    '#description' => t('The given text will be posted to twitter.com. You can use !url, !url-alias, !tinyurl, !title, and !user as replacement text.'),
    '#default_value' => variable_get('twitter_post_default_format', 'New post: !title !tinyurl'),
  );

  return system_settings_form($form);
}
