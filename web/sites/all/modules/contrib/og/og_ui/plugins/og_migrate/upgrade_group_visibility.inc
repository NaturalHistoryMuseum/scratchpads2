<?php

/**
 * Plugin definition.
 */
$plugin = array(
  'name' => t('Group visibility'),
  'description' => t('Upgrade group visibility (e.g. open, moderated, closed) from Organic groups 6.'),
  'access callback' => 'og_migrate_7000_access',
  'migrate callback' => 'og_ui_og_migrate_upgrade_group_visibility',
  'dependencies' => array('upgrade_group'),
);

/**
 * OG Migrate callback; Upgrade user subscription to groups.
 */
function og_ui_og_migrate_upgrade_group_visibility(&$context) {
  if (empty($context['sandbox'])) {
    $context['sandbox']['progress'] = 0;
    $context['sandbox']['last'] = 0;
    $context['sandbox']['last_vid'] = 0;
    $context['sandbox']['max'] = 0;

    // Check if there are groups defined.
    $group_types = array();
    foreach (node_type_get_types() as $type) {
      if (og_is_group_type('node', $type->type)) {
        $group_types[] = $type->type;

        // Add the "Group roles and permissions" field to the bundle.
        og_create_field(OG_DEFAULT_ACCESS_FIELD, 'node', $type->type);
      }
    }

    if ($group_types) {
      // Calculate max items.
      $query = db_select('d6_og', 'og');
      $context['sandbox']['max'] = $query
        ->countQuery()
        ->execute()
        ->fetchField();
    }

    if (!$context['sandbox']['max']) {
      // No data to process, so return.
      $context['finished'] = 1;
      return;
    }
  }

  // Operate in batches.
  $batch_size = variable_get('og_batch_size', 200);

  $query = db_select('node_revision', 'nr');
  $query->innerJoin('node', 'n', 'n.vid = nr.vid');
  $query->innerJoin('d6_og', 'og', 'n.nid = og.nid');
  $query
    ->fields('nr', array('nid', 'vid'))
    ->fields('n', array('type', 'created'))
    ->fields('og', array('og_selective', 'upgrade_id'))
    ->condition('nr.vid', $context['sandbox']['last_vid'], '>')
    ->condition('og.upgrade_id', $context['sandbox']['last'], '>')
    ->orderBy('nr.vid', 'ASC')
    ->range(0, $batch_size);
  $revisions = $query->execute();

  // Hold the node IDs so we can later change the permissions accordingly.
  $nids = array();
  foreach ($revisions as $revision) {
    // Create a dummy node object.
    $node = (object) array(
      'nid' => $revision->nid,
      'vid' => $revision->vid,
      'type' => $revision->type,
    );

    $nids[$revision->nid] = $revision->og_selective;

    // Set field values.
    $node->{OG_DEFAULT_ACCESS_FIELD}[LANGUAGE_NONE][0]['value'] = 1;

    field_attach_update('node', $node);

    $context['sandbox']['last_vid'] = $revision->vid;
    $context['sandbox']['last'] = $revision->upgrade_id;
    // Group content subscribed to group.
    $context['sandbox']['progress']++;
  }

  // Set the correct permissions.
  if ($nids) {
    foreach ($nids as $nid => $state) {
      if ($group = og_get_group('node', $nid)) {
        $anon_permissions = array();
        $auth_permissions = array('unsubscribe' => TRUE);
        switch ($state) {
          // Open
          case 0:
            $anon_permissions = array(
              'subscribe' => FALSE,
              'subscribe without approval' => TRUE,
            );
            break;

          // Moderated.
          case 1:
            $anon_permissions = array(
              'subscribe' => TRUE,
              'subscribe without approval' => FALSE,
            );
            break;

          // Invite only.
          case 2:
            $anon_permissions = array(
              'subscribe' => FALSE,
              'subscribe without approval' => FALSE,
            );
            break;

          // Closed.
          case 3:
            $anon_permissions = array(
              'subscribe' => FALSE,
              'subscribe without approval' => FALSE,
            );

            $auth_permissions = array('unsubscribe' => FALSE);
            break;
        }

        // Since the node is already a group we have to manually override
        // the roles, as og_group() isn't called.
        $roles = og_roles_override($group->gid);
        $roles = array_flip($roles);

        // Set the new permissions.
        og_role_change_permissions($roles[OG_ANONYMOUS_ROLE], $anon_permissions);
        og_role_change_permissions($roles[OG_AUTHENTICATED_ROLE], $auth_permissions);
      }
    }
  }

  if ($context['sandbox']['progress'] != $context['sandbox']['max']) {
    $context['finished'] = $context['sandbox']['progress'] / $context['sandbox']['max'];
  }
}