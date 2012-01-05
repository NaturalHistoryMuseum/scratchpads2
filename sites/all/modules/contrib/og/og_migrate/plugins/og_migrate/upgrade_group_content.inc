<?php

/**
 * Plugin definition.
 */
$plugin = array(
  'name' => t('Group content'),
  'description' => t('Upgrade group content'),
  'access callback' => 'og_migrate_7000_access',
  'migrate callback' => 'og_migrate_og_migrate_upgrade_group_content',
  'dependencies' => array('upgrade_group'),
);


/**
 * OG Migrate callback; Upgrade "group content" nodes.
 */
function og_migrate_og_migrate_upgrade_group_content(&$context) {
  if (empty($context['sandbox'])) {
    $context['sandbox']['progress'] = 0;
    $context['sandbox']['current_node'] = 0;
    $context['sandbox']['last_vid'] = 0;
    $context['sandbox']['last'] = 0;

    // Calculate max items.
    $query = db_select('node_revision', 'nr');
    $query->innerJoin('node', 'n', 'n.vid = nr.vid');
    $query->innerJoin('d6_og_ancestry', 'oga', 'n.nid = oga.nid');
    $context['sandbox']['max'] = $query
      ->countQuery()
      ->execute()
      ->fetchField();

    if (!$context['sandbox']['max']) {
      // No data to process, so return.
      $context['finished'] = 1;
      return;
    }

    og_migrate_create_fields();
  }

  // Operate on every revision of every node, in batches.
  $batch_size = variable_get('og_batch_size', 200);

  $query = db_select('node_revision', 'nr');
  $query->innerJoin('node', 'n', 'n.vid = nr.vid');
  $query->innerJoin('d6_og_ancestry', 'oga', 'n.nid = oga.nid');
  $query
    ->fields('nr', array('nid', 'vid'))
    ->fields('n', array('type', 'created'))
    ->fields('oga', array('upgrade_id', 'group_nid'))
    ->condition('nr.vid', $context['sandbox']['last_vid'], '>')
    ->condition('oga.upgrade_id', $context['sandbox']['last'], '>')
    ->orderBy('nr.vid', 'ASC')
    ->range(0, $batch_size);
  $revisions = $query->execute();

  foreach ($revisions as $revision) {
    $group = og_get_group('node', $revision->group_nid);

    $node = node_load($revision->nid, $revision->vid);
    // Make sure entity isn't already associated with group.
    if (!og_is_member($group->gid, 'node', $node)) {
      $values = array('entity type' => 'node', 'entity' => $node);
      og_group($group->gid, $values);
    }

    $context['sandbox']['last_vid'] = $revision->vid;
    $context['sandbox']['last'] = $revision->upgrade_id;
    // Group content subscribed to group.
    $context['sandbox']['progress']++;
  }
  if ($context['sandbox']['progress'] != $context['sandbox']['max']) {
    $context['finished'] = $context['sandbox']['progress'] / $context['sandbox']['max'];
  }
}
