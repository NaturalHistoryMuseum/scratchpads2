<?php

/**
 * @file
 * Rules integration.
 */

/**
 * Implements hook_rules_event_info().
 */
function feeds_rules_event_info() {
  // General events definitions.
  $info = array(
    'feeds_before_import' => array(
      'label' => t('Before importing feed'),
      'group' => t('Feeds'),
      'variables' => array('source' => array('type' => 'feeds_source', 'label' => 'Feeds source')),
    ),
    'feeds_after_import' => array(
      'label' => t('After importing feed'),
      'group' => t('Feeds'),
      'variables' => array('source' => array('type' => 'feeds_source', 'label' => 'Feeds source')),
    ),
  );
  // Per importer events definitions.
  $entity_info = entity_get_info();
  foreach (feeds_importer_load_all() as $importer) {
    $config = $importer->getConfig();
    $processor = feeds_plugin($config['processor']['plugin_key'], $importer->id);

    // It's possible to get FeedsMissingPlugin here which will break things
    // since it doesn't implement FeedsProcessor::entityType().
    if (!$processor instanceof FeedsProcessor) {
      continue;
    }

    $entity_type = $processor->entityType();
    $label = isset($entity_info[$entity_type]['label']) ? $entity_info[$entity_type]['label'] : $entity_type;

    $info['feeds_import_' . $importer->id] = array(
      'label' => t('Before saving an item imported via @name.', array('@name' => $importer->config['name'])),
      'group' => t('Feeds'),
      'variables' => array(
        $entity_type => array(
          'label' => t('Imported @label', array('@label' => $label)),
          'type' => $entity_type,
          'bundle' => $processor->bundle(),
          // Saving is handled by feeds anyway (unless the skip action is used).
          'skip save' => TRUE,
        ),
      ),
      'access callback' => 'feeds_rules_access_callback',
    );
  }
  return $info;
}

/**
 * Implements hook_rules_action_info().
 */
function feeds_rules_action_info() {
  return array(
    'feeds_import_feed' => array(
      'base' => 'feeds_action_import_feed',
      'label' => t('Execute feeds importer'),
      'parameter' => array(
        'importer' => array(
          'type' => 'text',
          'label' => t('Feeds importer'),
          'options list' => 'feeds_importer_list',
          'default mode' => 'input',
        ),
        'feed_nid' => array(
          'type' => 'node',
          'label' => t('Feed node'),
          'default mode' => 'input',
          'description' => t("The feed node, if the importer is attached to a content type. Put in '0' if the importer is not attached to a content type."),
        ),
      ),
      'group' => t('Feeds'),
      'access callback' => 'feeds_rules_access_callback',
    ),
    'feeds_skip_item' => array(
      'base' => 'feeds_action_skip_item',
      'label' => t('Skip import of feeds item'),
      'group' => t('Feeds'),
      'parameter' => array(
        'entity' => array('type' => 'entity', 'label' => t('The feeds import item to be marked as skipped')),
      ),
      'access callback' => 'feeds_rules_access_callback',
    ),
  );
}

/**
 * Implements hook_rules_data_info().
 */
function feeds_rules_data_info() {
  return array(
    'feeds_source' => array(
      'label' => t('Feeds source'),
      'group' => t('Feeds'),
      'wrap' => TRUE,
      'property info' => array(
        'id' => array(
          'label' => t('ID'),
          'type' => 'text',
          'description' => t("The machine readable name of the source importer."),
        ),
        'imported' => array(
          'label' => t('Date imported'),
          'type' => 'date',
          'description' => t("The date the source was last imported."),
        ),
        // @TODO: fetcher, parser, state ...
      ),
    ),
  );
}

/**
 * Rules action callback for "feeds_import_feed" action.
 *
 * @param string $importer_id
 *   ID of the importer.
 * @param object|null $feed_node
 *   The feed node, if found. Null otherwise.
 * @param array $params
 *   The raw parameters.
 */
function feeds_action_import_feed($importer_id, $feed_node, array $params) {
  $source = feeds_source($importer_id, $params['feed_nid']);
  try {
    $source->existing()->startImport();

    // Execute batch, if there is any. Set 'progressive' to false to prevent
    // batch from triggering a drupal_goto().
    $batch =& batch_get();
    if (!empty($batch)) {
      $batch['progressive'] = FALSE;
      batch_process();
    }
  }
  catch (FeedsNotExistingException $e) {
    // Ignore this kind of exception.
  }
  catch (Exception $e) {
    $source->log('import', $e->getMessage(), array(), WATCHDOG_ERROR);
  }
}

/**
 * Mark feeds import item as skipped.
 */
function feeds_action_skip_item($entity_wrapper) {
  $entity = $entity_wrapper->value();
  if (isset($entity->feeds_item)) {
    $entity->feeds_item->skip = TRUE;
  }
}

/**
 * Help callback for the skip action.
 */
function feeds_action_skip_item_help() {
  return t("This action allows skipping certain feed items during feeds processing, i.e. before an imported item is saved. Once this action is used on a item, the changes to the entity of the feed item are not saved.");
}

/**
 * List callback for selecting a Feeds importer.
 */
function feeds_importer_list() {
  $list = array();
  ctools_include('export');
  $configs = ctools_export_load_object('feeds_importer', 'all');
  foreach ($configs as $id => $config) {
    if (empty($config->disabled)) {
      $list[$id] = $config->config['name'];
    }
  }
  return $list;
}

/**
 * Access callback for the feeds rules integration.
 */
function feeds_rules_access_callback() {
  return user_access('administer feeds');
}
