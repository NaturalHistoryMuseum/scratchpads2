<?php

/**
 * Plugin definition.
 */
$plugin = array(
  'name' => t('Group'),
  'access callback' => 'og_migrate_7000_access',
  'migrate callback' => 'og_migrate_og_migrate_upgrade_group',
);


/**
 * OG Migrate callback; Upgrade group nodes.
 */
function og_migrate_og_migrate_upgrade_group(&$context) {
  if (empty($context['sandbox'])) {
    $context['sandbox']['progress'] = 0;
    $context['sandbox']['current_node'] = 0;
    $context['sandbox']['last_vid'] = 0;
    $context['sandbox']['last'] = 0;

    // Calculate max items.
    $query = db_select('node_revision', 'nr');
    $query->innerJoin('node', 'n', 'n.vid = nr.vid');
    $query->innerJoin('d6_og', 'og', 'n.nid = og.nid');
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
  $query->innerJoin('d6_og', 'og', 'n.nid = og.nid');
  $query
    ->fields('nr', array('nid', 'vid'))
    ->fields('n', array('type', 'created'))
    ->fields('og', array('upgrade_id'))
    ->condition('nr.vid', $context['sandbox']['last_vid'], '>')
    ->condition('og.upgrade_id', $context['sandbox']['last'], '>')
    ->orderBy('nr.vid', 'ASC')
    ->range(0, $batch_size);
  $revisions = $query->execute();

  // Holds the node IDs with their earliest revision and timestamp, to create a
  // group entity with the earliest timestamp.
  $nids = array();
  foreach ($revisions as $revision) {
    // Check node isn't already a group.
    if (!og_get_group('node', $revision->nid)) {
      // Create a dummy node object.
      $node = (object) array(
        'nid' => $revision->nid,
        'vid' => $revision->vid,
        'type' => $revision->type,
      );

      if (empty($nids[$revision->nid]) || $nids[$revision->nid] > $revision->created) {
        $nids[$revision->nid] = $revision->created;
      }

      // Set field values.
      $node->{OG_GROUP_FIELD}[LANGUAGE_NONE][0]['value'] = TRUE;

      field_attach_update('node', $node);
    }

    $context['sandbox']['last_vid'] = $revision->vid;
    $context['sandbox']['last'] = $revision->upgrade_id;
    $context['sandbox']['progress']++;
  }

  // Update group enteties with the correct "created" timestamp. The group
  // enteties were already created in the above field_attach_update(), but we
  // didn't have a chance alter it.
  if ($nids) {
    foreach ($nids as $nid => $created) {
      // Make sure the created value wasn't altered already.
      if (($group = og_get_group('node', $nid, TRUE)) && $group->created != $created) {
        $group->created = $created;
        $group->save();
      }
    }
  }

  if ($context['sandbox']['progress'] != $context['sandbox']['max']) {
    $context['finished'] = $context['sandbox']['progress'] / $context['sandbox']['max'];
  }
}