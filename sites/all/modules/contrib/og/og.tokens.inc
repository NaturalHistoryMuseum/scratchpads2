<?php

/**
 * @file
 * Builds placeholder replacement tokens for Organic groups related data.
 */

/**
 * Implements hook_token_info().
 */
function og_token_info() {
  $types['group'] = array(
    'name' => t('Organic groups'),
    'description' => t('Tokens related to Organic groups.'),
    'needs-data' => 'group',
  );

  $types['group-manager'] = array(
    'name' => t('Group manager'),
    'description' => t('Tokens related to the group manager user.'),
    'type' => 'user',
  );

  $group['gid'] = array(
    'name' => t('Group ID'),
    'description' => t("The unique ID of the group."),
  );

  $group['label'] = array(
    'name' => t('Label'),
    'description' => t("The label (i.e. name) of the group."),
  );

  $group['url'] = array(
    'name' => t("URL"),
    'description' => t("The URL of the group."),
  );

  $group['created'] = array(
    'name' => t("Created"),
    'description' => t("The date the user account was created."),
    'type' => 'date',
  );

  return array(
    'types' => $types,
    'tokens' => array('group' => $group),
  );
}

/**
 * Implements hook_tokens().
 */
function og_tokens($type, $tokens, array $data = array(), array $options = array()) {
  $url_options = array('absolute' => TRUE);
  if (isset($options['language'])) {
    $url_options['language'] = $options['language'];
    $language_code = $options['language']->language;
  }
  else {
    $language_code = NULL;
  }
  $sanitize = !empty($options['sanitize']);

  $replacements = array();
  if ($type == 'group' && !empty($data['group'])) {
    $group = $data['group'];
    foreach ($tokens as $name => $original) {
      switch ($name) {
        // Basic user account information.
        case 'gid':
          $replacements[$original] = !empty($group->gid) ? $group->gid : t('not yet assigned');
          break;

        case 'label':
          $replacements[$original] = og_label($group->gid, $sanitize);
          break;

        case 'url':
          $entity = $group->getEntity();
          $uri = entity_uri($group->entity_type, $entity);
          $replacements[$original] = !empty($uri['path']) ? url($uri['path'], $url_options) : t('not yet assigned');
          break;

        case 'created':
          // In the case of user_presave the created date may not yet be set.
          $replacements[$original] = !empty($group->created) ? format_date($group->created, 'medium', '', NULL, $language_code) : t('not yet created');
          break;
      }
    }

    if ($registered_tokens = token_find_with_prefix($tokens, 'created')) {
      $replacements += token_generate('date', $registered_tokens, array('date' => $account->created), $options);
    }
  }

  if ($type == 'group-manager' && !empty($data['group-manager'])) {
    $replacements += token_generate('user', $tokens, array('user' => $data['group-manager']), $options);
  }

  return $replacements;
}
