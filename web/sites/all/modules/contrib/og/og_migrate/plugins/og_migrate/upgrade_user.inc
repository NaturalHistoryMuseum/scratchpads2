<?php

/**
 * Plugin definition.
 */
$plugin = array(
  'name' => t('User relation to group'),
  'access callback' => 'og_migrate_7000_access',
  'migrate callback' => 'og_migrate_og_migrate_upgrade_user',
  'dependencies' => array('upgrade_group'),
);

/**
 * OG Migrate callback; Upgrade user subscription to groups.
 */
function og_migrate_og_migrate_upgrade_user(&$context) {
  if (empty($context['sandbox'])) {
    $context['sandbox']['progress'] = 0;
    $context['sandbox']['last'] = 0;

    // Get the administrator role ID.
    $roles = array_flip(og_get_global_roles());
    $context['sandbox']['rid'] = $roles[OG_ADMINISTRATOR_ROLE];

    // Calculate max items.
    $query = db_select('d6_og_uid', 'ogu');
    $context['sandbox']['max'] = $query
      ->countQuery()
      ->execute()
      ->fetchField();

    if (!$context['sandbox']['max']) {
      // No data to process, so return.
      $context['finished'] = 1;
      return;
    }

    // It is probably already added, but make sure there is og-audience field
    // attached to the user.
    og_create_field(OG_AUDIENCE_FIELD, 'user', 'user');
  }

  if (!$context['sandbox']['max']) {
    // No data to process, so return.
    $context['finished'] = 1;
    return;
  }

  // Operate on users in batches.
  $batch_size = variable_get('og_batch_size', 200);

  $query = db_select('d6_og_uid', 'ogu');
  $query
    ->fields('ogu')
    ->condition('ogu.upgrade_id', $context['sandbox']['last'], '>')
    ->orderBy('ogu.upgrade_id', 'ASC')
    ->range(0, $batch_size);
  $records = $query->execute();

  foreach ($records as $record) {
    $group = og_get_group('node', $record->nid);
    $state = $record->is_active ? OG_STATE_ACTIVE : OG_STATE_PENDING;

    $account = user_load($record->uid);
    // Make sure entity isn't already associated with group.
    if (!og_is_member($group->gid, 'user', $account)) {
      $values = array('entity' => $account, 'state' => $state);
      og_group($group->gid, $values);
    }

    if ($record->is_admin) {
      og_role_grant($group->gid, $record->uid, $context['sandbox']['rid']);
    }
    $context['sandbox']['last'] = $record->upgrade_id;
    $context['sandbox']['progress']++;
    $context['message'] = t('Processing database rows @start - @end', array('@start' => $record->upgrade_id, '@end' => $record->upgrade_id + $batch_size));
  }

  if ($context['sandbox']['progress'] != $context['sandbox']['max']) {
    $context['finished'] = $context['sandbox']['progress'] / $context['sandbox']['max'];
  }
}