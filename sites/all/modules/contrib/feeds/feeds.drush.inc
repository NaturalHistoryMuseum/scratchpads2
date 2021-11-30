<?php

/**
 * @file
 * Drush commands for Feeds module.
 */

/**
 * The default maximum number of feeds to list or import.
 *
 * @var int
 */
define('DRUSH_FEEDS_DEFAULT_LIMIT', 2147483647);

/**
 * Implements hook_drush_command().
 */
function feeds_drush_command() {
  $items = array();

  $items['feeds-list-importers'] = array(
    'description' => 'Get a list of all Feeds importers in the system.',
    'aliases' => array('feeds'),
  );

  $items['feeds-list-feeds'] = array(
    'description' => 'List all feed sources.',
    'arguments' => array(
      'importer' => 'The name of the Feeds importer whose instances will be listed. Optional.',
    ),
    'examples' => array(
      'drush feeds-list-feeds' => 'List all instances of all feeds.',
      'drush feeds-list-feeds rss_feed' => 'List all feed sources of the rss_feed importer.',
      'drush feeds-list-feeds --limit=3' => 'Only list the first three feed sources.',
    ),
    'options' => array(
      'limit' => 'Limit the number of feeds to show in the list. Optional.',
    ),
    'aliases' => array('feeds-lf'),
  );

  $items['feeds-import'] = array(
    'description' => 'Imports a feed.',
    'arguments' => array(
      'importer' => 'The name of the Feeds importer that will be refreshed. Mandatory.',
    ),
    'options' => array(
      'nid' => '(optional) The nid of the Feeds importer that will be imported.',
      'file' => '(optional) The file to import. Bypasses the configured fetcher and does *not* update the source configuration.',
      'url' => '(optional) The URL to import. Bypasses the configured fetcher and does *not* update the source configuration.',
      'stdin' => '(optional) Read the file to import from stdin. Bypasses the configured fetcher and does *not* update the source configuration.',
    ),
    'examples' => array(
      'drush feeds-import my_importer' => "Import items with the importer 'my_importer'.",
      'drush feeds-import my_importer --nid=2' => "Import items with the importer 'my_importer' for feed node 2.",
    ),
    'aliases' => array('feeds-im'),
  );

  $items['feeds-import-all'] = array(
    'description' => 'Import all instances of feeds of the given type.',
    'arguments' => array(
      'importer' => 'The name of the Feeds importer that will be refreshed. Omitting the importer will cause all instances of all feeds to be imported.',
    ),
    'examples' => array(
      'drush feeds-import-all' => 'Import all instances of all feeds.',
      'drush feeds-import-all my_importer' => "Import all instances of the importer 'my_importer'.",
      'drush feeds-import-all my_importer --limit=10' => "Import 10 instances of the feed 'my_importer'.",
    ),
    'options' => array(
      'limit' => 'Limit the number of feeds to import. Optional.',
    ),
    'aliases' => array('feeds-ia', 'feeds-im-all'),
  );

  $items['feeds-clear'] = array(
    'description' => 'Delete all items from a feed.',
    'arguments' => array(
      'importer' => 'The name of the Feeds importer that will be cleared. Mandatory.',
    ),
    'options' => array(
      'nid' => 'The ID of the Feed node that will be cleared. Required if the importer is attached to a content type.',
    ),
    'examples' => array(
      'drush feeds-clear my_importer' => "Deletes all items imported with the importer 'my_importer'. The importer must use the standalone import form.",
      'drush feeds-clear my_importer --nid=2' => "Deletes all items imported with the importer 'my_importer' for the feed node with ID 2. The importer must be attached to a content type.",
    ),
  );

  $items['feeds-enable'] = array(
    'description' => 'Enables one or more Feeds importers.',
    'arguments' => array(
      'importers' => 'A space delimited list of Feeds importers. Mandatory.',
    ),
    'examples' => array(
      'drush feeds-enable importer_1 importer_2' => "Enable Feeds importers 'importer_1' and 'importer_2'.",
    ),
    'aliases' => array('feeds-en'),
  );

  $items['feeds-disable'] = array(
    'description' => 'Disable one or more Feeds importers.',
    'arguments' => array(
      'importers' => 'A space delimited list of Feeds importers. Mandatory.',
    ),
    'examples' => array(
      'drush feeds-disable importer_1 importer_2' => "Disable Feeds importers 'importer_1' and 'importer_2'.",
    ),
    'aliases' => array('feeds-dis'),
  );

  $items['feeds-delete'] = array(
    'description' => 'Deletes one or more Feeds importers.',
    'arguments' => array(
      'importers' => 'A space delimited list of Feeds importers. Mandatory.',
    ),
    'examples' => array(
      'drush feeds-delete importer_1 importer_2' => "Delete Feeds importers 'importer_1' and 'importer_2'.",
    ),
  );

  $items['feeds-revert'] = array(
    'description' => 'Reverts one or more Feeds importers.',
    'arguments' => array(
      'importers' => 'A space delimited list of Feeds importers. Mandatory.',
    ),
    'examples' => array(
      'drush feeds-revert importer_1 importer_2' => "Revert Feeds importers 'importer_1' and 'importer_2'.",
    ),
  );

  return $items;
}

/**
 * Implements hook_drush_help().
 */
function feeds_drush_help($section) {
  switch ($section) {
    case 'drush:feeds-list-importers':
      return dt('Show a list of available Feeds importers with information about them.');

    case 'drush:feeds-list-feeds':
      return dt("List all feed sources. You can limit the number of feed sources to display by setting the option '--limit'.");

    case 'drush:feeds-import':
      $help = dt("Import items from a feed. Follow the command with the importer name to import items with. If the importer is attached to a content type, specify also the feed node with the option '--nid'.");
      $help .= "\n" . dt("Note that the options '--file', '--stdin' and '--url' temporary bypass the configured fetcher and do *not* update the source configuration. For example, if a file was uploaded for the feed source, that file will remain there even when you specify a different file using the '--file' option. Same story applies for when importing from a url. If you omit these options, the last stored source will be used.");
      return $help;

    case 'drush:feeds-import-all':
      return dt('Import items from all feeds. Optionally specify the importer name to import all feeds for.');

    case 'drush:feeds-clear':
      return dt("Delete all items from a feed. Follow the command with the importer name to delete items from. If the importer is attached to a content type, specify also the feed node with the option '--nid'.");

    case 'drush:feeds-enable':
      return dt('Enable the specified Feeds importers. Follow the command with a space delimited list of importer names.');

    case 'drush:feeds-disable':
      return dt('Disable the specified Feeds importers. Follow the command with a space delimited list of importer names.');

    case 'drush:feeds-delete':
      return dt('Delete the specified Feeds importers. Follow the command with a space delimited list of importer names.');

    case 'drush:feeds-revert':
      return dt('Revert the specified Feeds importers. Follow the command with a space delimited list of importer names.');
  }
}

/**
 * Prints a list of all Feeds importers.
 */
function drush_feeds_list_importers() {
  if (!$importers = feeds_importer_load_all(TRUE)) {
    drush_print(dt('No importers available.'));
    return;
  }

  $rows = array();

  $rows[] = array(
    dt('Name'),
    dt('Description'),
    dt('Attached to'),
    dt('Status'),
    dt('State'),
  );

  foreach ($importers as $importer) {
    if ($importer->export_type == EXPORT_IN_CODE) {
      $state = dt('Default');
    }
    elseif ($importer->export_type == EXPORT_IN_DATABASE) {
      $state = dt('Normal');
    }
    elseif ($importer->export_type == (EXPORT_IN_CODE | EXPORT_IN_DATABASE)) {
      $state = dt('Overridden');
    }

    $rows[] = array(
      $importer->config['name'] . ' (' . $importer->id . ')',
      $importer->config['description'],
      $importer->config['content_type'] ? dt(node_type_get_name($importer->config['content_type'])) : dt('none'),
      $importer->disabled ? dt('Disabled') : dt('Enabled'),
      $state,
    );
  }

  drush_print_table($rows, TRUE);
}

/**
 * Lists all feeds.
 *
 * @param string $importer_id
 *   (optional) The importer id.
 */
function drush_feeds_list_feeds($importer_id = NULL) {
  if (!$limit = drush_get_option('limit')) {
    $limit = DRUSH_FEEDS_DEFAULT_LIMIT;
  }

  $header = array(
    'importer_id' => dt('Importer ID'),
    'feed_nid' => dt('Feed NID'),
    'feed_title' => dt('Feed title'),
    'imported' => dt('Last imported'),
    'source' => dt('Feed source'),
    'process_in_background' => dt('Process in background'),
  );

  $rows = array();
  $nids = array();

  foreach (_drush_feeds_get_all($importer_id, $limit) as $feed) {
    $feed_config = feeds_source($feed->id, $feed->feed_nid)->importer->getConfig();

    $rows[] = array(
      'importer_id' => $feed->id,
      'feed_nid' => $feed->feed_nid,
      'feed_title' => '',
      'imported' => $feed->imported ? date('Y-m-d H:i:s', $feed->imported) : dt('Never'),
      'source' => is_scalar($feed->source) ? $feed->source : gettype($feed->source),
      'process_in_background' => !empty($feed_config['process_in_background']) ? dt('Yes') : dt('No'),
    );

    // Collect node ID's to find titles for.
    if ($feed->feed_nid) {
      $nids[] = $feed->feed_nid;
    }
  }

  // Find titles for feed nodes.
  if (count($nids)) {
    $nodes = db_select('node')
      ->fields('node', array('nid', 'title'))
      ->condition('nid', $nids)
      ->execute()
      ->fetchAllKeyed();

    foreach ($rows as &$row) {
      $nid = $row['feed_nid'];
      if ($nid && isset($nodes[$nid])) {
        $row['feed_title'] = $nodes[$nid];
      }
    }
  }

  // Check if there were any results.
  if (count($rows) == 0) {
    if (empty($importer_id)) {
      drush_print(dt('There are no feed sources.'));
    }
    else {
      drush_print(dt("No feed sources exists for importer '@importer_id'.", array(
        '@importer_id' => $importer_id,
      )));
    }
    return FALSE;
  }

  // Create table.
  $table = array_merge(array($header), $rows);

  drush_print_table($table, TRUE);
}

/**
 * Imports a given importer/source.
 *
 * @param string $importer_id
 *   (optional) The importer id to filter on.
 */
function drush_feeds_import($importer_id = NULL) {
  if (!strlen($importer_id)) {
    drush_set_error(dt("Please specify the importer to import items with. If the importer is attached to a content type, specify also the feed node with the option '--nid'."));
    return FALSE;
  }

  if (!$feed_nid = drush_get_option('nid')) {
    $feed_nid = 0;
  }

  try {
    $source = feeds_source($importer_id, $feed_nid)->existing();
  }
  catch (FeedsNotExistingException $e) {
    $importer = feeds_importer_load($importer_id);
    if (!$importer) {
      drush_set_error(dt("The importer '@importer' does not exist.", array(
        '@importer' => $importer_id,
      )));
      return FALSE;
    }
    elseif (!$importer->isEnabled()) {
      drush_set_error(dt("The importer '@importer' is not enabled. You can enable it with the command '@command'.", array(
        '@importer' => $importer_id,
        '@command' => 'drush feeds-enable ' . $importer_id,
      )));
      return FALSE;
    }

    if ($feed_nid == 0) {
      // Check if the importer is a standalone importer.
      if ($importer->config['content_type']) {
        drush_set_error(dt("The importer '@importer' is attached to a content type. Please specify the feed node to import items for with the option '--nid'. To show a list of available feed nodes for this importer, use 'drush feeds-list-feeds @importer'.", array(
          '@importer' => $importer_id,
        )));
        return FALSE;
      }
      elseif (drush_get_option('file') || drush_get_option('url') || drush_get_option('stdin')) {
        // Create a new source.
        $source = feeds_source($importer_id);
        $source->save();
      }
    }
    elseif (!$importer->config['content_type']) {
      $message = dt("The importer '@importer' is not attached to a content type. Do you want to import items for this importer?", array(
        '@importer' => $importer_id,
      ));

      if (!drush_confirm($message)) {
        return drush_log(dt('Aborting.'), 'ok');
      }
      else {
        drush_set_option('nid', 0);
        // Avoid asking for confirmation twice.
        drush_set_option('feeds_import_skip_confirm', 1);
        return drush_feeds_import($importer_id);
      }
    }

    if (empty($source)) {
      // Only abort at this point when no source object exists. It can exist
      // when the importer is not attached to a content type and a file, url or
      // stdin is supplied.
      if ($feed_nid == 0) {
        drush_set_error(dt("No source exists yet for the importer '@importer'. There is nothing to import. You can import from a file or url using the options '--file' or '--url' or go to /import/@importer to configure the source to import. See 'drush help feeds-import' for more information.", array(
          '@importer' => $importer_id,
        )));
        return FALSE;
      }
      else {
        drush_set_error(dt("There is no feed node with ID @nid for importer '@importer'. To show a list of available feed nodes for this importer, use 'drush feeds-list-feeds @importer'.", array(
          '@importer' => $importer_id,
          '@nid' => $feed_nid,
        )));
        return FALSE;
      }
    }
  }

  // Propose confirmation message.
  $messages = array();
  $vars = array(
    '@importer' => $importer_id,
    '@feed_nid' => $feed_nid,
  );
  if ($feed_nid) {
    $messages[] = dt("Items will be imported with the importer '@importer' for the feed node @feed_nid.", $vars);
  }
  else {
    $messages[] = dt("Items will be imported with the importer '@importer'.", $vars);
  }

  $result = NULL;
  if ($filename = drush_get_option('file')) {
    $filepath = _drush_feeds_find_file($filename);
    if (!is_file($filepath)) {
      drush_set_error(dt("The file '@file' does not exist.", array('@file' => $filename)));
      return FALSE;
    }
    else {
      $filepath = realpath($filepath);
    }
    $result = new FeedsFileFetcherResult($filepath);

    $messages[] = dt("The items will be imported from the file '@file'.", array(
      '@file' => $filepath,
    ));
  }
  elseif ($url = drush_get_option('url')) {
    $result = new FeedsHTTPFetcherResult($url);

    $messages[] = dt("The items will be imported from the url '@url'.", array(
      '@url' => $url,
    ));
  }
  elseif (drush_get_option('stdin')) {
    $result = new FeedsFetcherResult(file_get_contents('php://stdin'));

    $messages[] = dt('The items will be imported from stdin.');
  }

  // Only ask for confirmation if it wasn't already asked before. See above.
  if (!drush_get_option('feeds_import_skip_confirm')) {
    $messages[] = dt('Do you really want to continue?');
    $message = implode(' ', $messages);
    if (!drush_confirm($message)) {
      return drush_log(dt('Aborting.'), 'ok');
    }
  }

  // Start the import!
  if ($result) {
    try {
      $source->pushImport($result);
    }
    catch (Exception $e) {
      drush_set_error($e->getMessage());
      return FALSE;
    }
  }
  else {
    _drush_feeds_create_import_batch($importer_id, $feed_nid);
    drush_backend_batch_process();
    return;
  }
}

/**
 * Imports all feeds.
 *
 * @param string $importer_id
 *   (optional) The importer id to filter on.
 */
function drush_feeds_import_all($importer_id = NULL) {
  if (!$limit = drush_get_option('limit')) {
    $limit = DRUSH_FEEDS_DEFAULT_LIMIT;
  }

  // Set flag for whether or not executing the batch. When all importers are not
  // not confirmed there are no batches set and in that case there are no
  // batches to process.
  $execute_batch = FALSE;

  foreach (_drush_feeds_get_all($importer_id, $limit) as $feed) {
    if (!isset($feed->source) || !strlen($feed->source)) {
      continue;
    }

    try {
      $source = feeds_source($feed->id, $feed->feed_nid)->existing();
    }
    catch (FeedsNotExistingException $e) {
      continue;
    }

    // Compose messages.
    $vars = array(
      '@importer_id' => $source->id,
      '@feed_nid' => $source->feed_nid,
    );
    if ($source->feed_nid) {
      $feed_node = node_load($source->feed_nid);
      if ($feed_node) {
        $vars['@title'] = $feed_node->title;
      }
      else {
        drush_set_error(dt('The feed node @feed_nid (@importer_id) does not exist.', $vars));
        continue;
      }
      $confirm_message = dt("Do you want to import items with the importer '@importer_id' for the feed '@title'?", $vars);
      $skip_message = dt("Skipping importer '@importer_id' for feed '@title'.", $vars);
    }
    else {
      $confirm_message = dt("Do you want to import items with the importer '@importer_id'?", $vars);
      $skip_message = dt("Skipping importer '@importer_id'.", $vars);
    }

    if (drush_confirm($confirm_message)) {
      _drush_feeds_create_import_batch($feed->id, $feed->feed_nid);
      $execute_batch = TRUE;
    }
    else {
      drush_log($skip_message, 'ok');
    }
  }

  if ($execute_batch) {
    drush_backend_batch_process();
  }
}

/**
 * Creates a batch job for an import.
 *
 * @param string $importer_id
 *   The importer id.
 * @param int $feed_nid
 *   The feed node id.
 */
function _drush_feeds_create_import_batch($importer_id, $feed_nid) {
  $feed_node = FALSE;
  if ($feed_nid) {
    if (!$feed_node = node_load($feed_nid)) {
      drush_set_error(dt('The feed node @feed_nid (@importer_id) does not exist.', array('@importer_id' => $importer_id, '@feed_nid' => $feed_nid)));
      return FALSE;
    }
  }

  $title = $feed_node ? $feed_node->title . ' (' . $importer_id . ')' : $importer_id;

  drush_log(dt('Importing: @title', array('@title' => $title)), 'ok');

  $batch = array(
    'title' => '',
    'operations' => array(
      array('feeds_batch', array('import', $importer_id, $feed_nid)),
    ),
    'progress_message' => '',
  );

  batch_set($batch);
}

/**
 * Clears a Feeds importer.
 *
 * @param string $importer_id
 *   The importer id to clean.
 */
function drush_feeds_clear($importer_id = NULL) {
  if (!strlen($importer_id)) {
    drush_set_error(dt("Please specify the importer to delete all imported items from. If the importer is attached to a content type, specify also the feed node with the option '--nid'."));
    return FALSE;
  }

  if (!$feed_nid = drush_get_option('nid')) {
    $feed_nid = 0;
  }

  try {
    feeds_source($importer_id, $feed_nid)->existing();
  }
  catch (FeedsNotExistingException $e) {
    $importer = feeds_importer_load($importer_id);
    if (!$importer) {
      drush_set_error(dt("The importer '@importer' does not exist.", array(
        '@importer' => $importer_id,
      )));
      return FALSE;
    }
    elseif (!$importer->isEnabled()) {
      drush_set_error(dt("The importer '@importer' is not enabled. You can enable it with the command '@command'.", array(
        '@importer' => $importer_id,
        '@command' => 'drush feeds-enable ' . $importer_id,
      )));
      return FALSE;
    }

    if ($feed_nid == 0) {
      // Check if the importer is a standalone importer.
      if ($importer->config['content_type']) {
        drush_set_error(dt("The importer '@importer' is attached to a content type. Please specify the feed node to delete items from with the option '--nid'. To show a list of available feed nodes for this importer, use 'drush feeds-list-feeds @importer'.", array(
          '@importer' => $importer_id,
        )));
        return FALSE;
      }
    }
    elseif (!$importer->config['content_type']) {
      $message = dt("The importer '@importer' is not attached to a content type. Do you want to clear all items for this importer?", array(
        '@importer' => $importer_id,
      ));

      if (!drush_confirm($message)) {
        return drush_log(dt('Aborting.'), 'ok');
      }
      else {
        drush_set_option('nid', 0);
        // Avoid asking for confirmation twice.
        drush_set_option('feeds_clear_skip_confirm', 1);
        return drush_feeds_clear($importer_id);
      }
    }

    drush_set_error(dt("There is no feed node with ID @nid for importer '@importer'. To show a list of available feed nodes for this importer, use 'drush feeds-list-feeds @importer'.", array(
      '@importer' => $importer_id,
      '@nid' => $feed_nid,
    )));
    return FALSE;
  }

  // Only ask for confirmation if it wasn't already asked before. See above.
  if (!drush_get_option('feeds_clear_skip_confirm')) {
    if ($feed_nid) {
      $message = dt("All items imported with the importer '@importer' for the feed node @feed_nid will be deleted. Do you really want to continue?", array(
        '@importer' => $importer_id,
        '@feed_nid' => $feed_nid,
      ));
    }
    else {
      $message = dt("All items imported with the importer '@importer' will be deleted. Do you really want to continue?", array(
        '@importer' => $importer_id,
      ));
    }
    if (!drush_confirm($message)) {
      return drush_log(dt('Aborting.'), 'ok');
    }
  }

  $batch = array(
    'title' => dt('Clearing !importer', array('!importer' => $importer_id)),
    'operations' => array(
      array('feeds_batch', array('clear', $importer_id, $feed_nid)),
    ),
  );

  batch_set($batch);
  drush_backend_batch_process();
}

/**
 * Enables a set of Feeds importers.
 */
function drush_feeds_enable() {
  $all = array_keys(feeds_importer_load_all(TRUE));
  $enabled = array_keys(feeds_importer_load_all());
  $missing = array_diff(func_get_args(), $all);
  $to_enable = array_diff(func_get_args(), $enabled, $missing);
  $already_enabled = array_intersect(func_get_args(), $enabled);

  if ($missing) {
    drush_print(dt('The following importers are missing: !importers', array('!importers' => implode(', ', $missing))));
  }

  if ($already_enabled) {
    drush_print(dt('The following importers are already enabled: !importers', array('!importers' => implode(', ', $already_enabled))));
  }

  if ($to_enable) {
    drush_print(dt('The following importers will be enabled: !importers', array('!importers' => implode(', ', $to_enable))));
  }
  elseif (count(func_get_args()) == 0) {
    return drush_set_error(dt('Please specify a space delimited list of importers to enable.'));
  }
  else {
    return drush_print(dt('There are no importers to enable.'));
  }

  if (!drush_confirm(dt('Do you really want to continue?'))) {
    return drush_log(dt('Aborting.'), 'ok');
  }

  $disabled = variable_get('default_feeds_importer', array());

  foreach ($to_enable as $importer_id) {
    unset($disabled[$importer_id]);
    drush_log(dt("The importer '!importer' has been enabled.", array('!importer' => $importer_id)), 'ok');
  }

  variable_set('default_feeds_importer', $disabled);
  feeds_cache_clear();
}

/**
 * Disables a set of Feeds importers.
 */
function drush_feeds_disable() {
  $all = array_keys(feeds_importer_load_all(TRUE));
  $enabled = array_keys(feeds_importer_load_all());
  $to_disable = array_intersect(func_get_args(), $enabled);
  $missing = array_diff(func_get_args(), $all);
  $already_disabled = array_diff(func_get_args(), $enabled, $missing);

  if ($missing) {
    drush_print(dt('The following importers are missing: !importers', array('!importers' => implode(', ', $missing))));
  }

  if ($already_disabled) {
    drush_print(dt('The following importers are already disabled: !importers', array('!importers' => implode(', ', $already_disabled))));
  }

  if ($to_disable) {
    drush_print(dt('The following importers will be disabled: !importers', array('!importers' => implode(', ', $to_disable))));
  }
  elseif (count(func_get_args()) == 0) {
    return drush_set_error(dt('Please specify a space delimited list of importers to disable.'));
  }
  else {
    return drush_print(dt('There are no importers to disable.'));
  }

  if (!drush_confirm(dt('Do you really want to continue?'))) {
    return drush_log(dt('Aborting.'), 'ok');
  }

  $disabled = variable_get('default_feeds_importer', array());
  foreach ($to_disable as $importer_id) {
    $disabled[$importer_id] = TRUE;
    drush_log(dt("The importer '!importer' has been disabled.", array('!importer' => $importer_id)), 'ok');
  }

  variable_set('default_feeds_importer', $disabled);
  feeds_cache_clear();
}

/**
 * Deletes a set of Feeds importers.
 */
function drush_feeds_delete() {
  $all = feeds_importer_load_all(TRUE);
  $to_delete = array_intersect_key($all, array_flip(func_get_args()));
  $missing = array_diff(func_get_args(), array_keys($all));
  $cant_delete = array();

  // Filter out default importers that are not overridden.
  foreach ($to_delete as $delta => $importer) {
    if ($importer->export_type === EXPORT_IN_CODE) {
      unset($to_delete[$delta]);
      $cant_delete[$importer->id] = $importer->id;
    }
  }

  if ($missing) {
    drush_print(dt('The following importers are missing: !importers', array('!importers' => implode(', ', $missing))));
  }

  if ($cant_delete) {
    drush_print(dt('The following importers cannot be deleted because they only exist in code: !importers', array('!importers' => implode(', ', array_keys($cant_delete)))));
  }

  if ($to_delete) {
    drush_print(dt('The following importers will be deleted: !importers', array('!importers' => implode(', ', array_keys($to_delete)))));
  }
  elseif (count(func_get_args()) == 0) {
    return drush_set_error(dt('Please specify a space delimited list of importers to delete.'));
  }
  else {
    return drush_print(dt('There are no importers to delete.'));
  }

  if (!drush_confirm(dt('Do you really want to continue?'))) {
    return drush_log(dt('Aborting.'), 'ok');
  }

  foreach ($to_delete as $importer) {
    $importer->delete();
    drush_log(dt("The importer '!importer' was deleted successfully.", array('!importer' => $importer->id)), 'ok');
  }

  feeds_cache_clear();
}

/**
 * Reverts a set of feeds.
 */
function drush_feeds_revert() {
  $all = feeds_importer_load_all(TRUE);
  $missing = array_diff(func_get_args(), array_keys($all));
  $to_revert = array_intersect_key($all, array_flip(func_get_args()));
  $cant_revert = array();
  $cant_revert_db = array();

  // Filter out non-overridden importers.
  foreach ($to_revert as $delta => $importer) {
    if ($importer->export_type !== (EXPORT_IN_CODE | EXPORT_IN_DATABASE)) {
      unset($to_revert[$delta]);
      if ($importer->export_type == EXPORT_IN_DATABASE) {
        $cant_revert_db[$importer->id] = $importer->id;
      }
      else {
        $cant_revert[$importer->id] = $importer->id;
      }
    }
  }

  if ($missing) {
    drush_print(dt('The following importers are missing: !importers', array('!importers' => implode(', ', $missing))));
  }

  if ($cant_revert) {
    drush_print(dt('The following importers cannot be reverted because they are not overridden: !importers', array('!importers' => implode(', ', array_keys($cant_revert)))));
  }
  if ($cant_revert_db) {
    drush_print(dt('The following importers cannot be reverted because they only exist in the database: !importers', array('!importers' => implode(', ', array_keys($cant_revert_db)))));
  }

  if ($to_revert) {
    drush_print(dt('The following importers will be reverted: !importers', array('!importers' => implode(', ', array_keys($to_revert)))));
  }
  elseif (count(func_get_args()) == 0) {
    return drush_set_error(dt('Please specify a space delimited list of importers to revert.'));
  }
  else {
    return drush_print(dt('There are no importers to revert.'));
  }

  if (!drush_confirm(dt('Do you really want to continue?'))) {
    return drush_log(dt('Aborting.'), 'ok');
  }

  foreach ($to_revert as $importer) {
    $importer->delete();
    drush_log(dt("The importer '!importer' was reverted successfully.", array('!importer' => $importer->id)), 'ok');
  }

  feeds_cache_clear();
}

/**
 * Returns all feed instances filtered by an optional importer.
 *
 * @param string $importer_id
 *   (optional) The importer id.
 * @param int $limit
 *   (optional) The number of feeds to return.
 *
 * @return DatabaseStatementInterface
 *   A list of feeds objects.
 */
function _drush_feeds_get_all($importer_id = NULL, $limit = DRUSH_FEEDS_DEFAULT_LIMIT) {
  if (isset($importer_id)) {
    return db_query_range("SELECT * FROM {feeds_source} WHERE id = :importer ORDER BY imported ASC", 0, $limit, array(':importer' => $importer_id));
  }

  return db_query_range("SELECT * FROM {feeds_source} ORDER BY imported ASC", 0, $limit);
}

/**
 * Tries to find the specified file at several locations.
 *
 * @param string $filename
 *   The file to look for.
 *
 * @return string
 *   If found, the file that was found.
 *   NULL otherwise.
 */
function _drush_feeds_find_file($filename) {
  // If the full path to the file is specified, the file will be found right
  // away.
  if (is_file($filename)) {
    // Found!
    return $filename;
  }

  // Look for the file within the current active directory.
  $search = drush_cwd() . '/' . $filename;
  if (is_file($search)) {
    // Found!
    return $search;
  }

  // Look for the file within the directory Drupal is installed in.
  // bootstrap_drupal_root() sets the active directory to the Drupal root.
  $search = getcwd() . '/' . $filename;
  if (is_file($search)) {
    // Found!
    return $search;
  }
}
