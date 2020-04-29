<?php

/**
 * @file
 * Functions related to Apache Solr indexing operations.
 */

/**
 * Processes all index queues associated with the passed environment.
 *
 * An environment usually indexes one or more entity types. Each entity type
 * stores its queue in a database table that is defined in the entity type's
 * info array. This function processes N number of items in each queue table,
 * where N is the limit passed as the second argument.
 *
 * The indexing routine allows developers to selectively bypass indexing on a
 * per-entity basis by implementing the following hooks:
 * - hook_apachesolr_exclude()
 * - hook_apachesolr_ENTITY_TYPE_exclude()
 *
 * @param string $env_id
 *   The machine name of the environment.
 * @param int $limit
 *   The number of items to process per queue table. For example, if there are
 *   two entities that are being indexed in this environment and they each have
 *   their own queue table, setting a limit of 50 will send a maximum number of
 *   100 documents to the Apache Solr server.
 *
 * @return int
 *   The total number of documents sent to the Apache Solr server for indexing.
 *
 * @see apachesolr_index_get_entities_to_index()
 * @see apachesolr_index_entity_to_documents()
 * @see apachesolr_index_send_to_solr()
 */
function apachesolr_index_entities($env_id, $limit) {
  $documents_submitted = 0;

  try {
    // Get the $solr object
    $solr = apachesolr_get_solr($env_id);
    // If there is no server available, don't continue.
    if (!$solr->ping(variable_get('apachesolr_ping_timeout', 4))) {
      throw new Exception(t('No Solr instance available during indexing.'));
    }
  }
  catch (Exception $e) {
    apachesolr_log_exception($env_id, $e);
    return FALSE;
  }

  foreach (entity_get_info() as $entity_type => $info) {
    // With each pass through the callback, retrieve the next group of nids.
    $rows = apachesolr_index_get_entities_to_index($env_id, $entity_type, $limit);
    // If there are none for this entity type - ignore it and go to the next
    // entity type.
    if (!count($rows)) {
      continue;
    }
    $documents = array();
    foreach ($rows as $row) {
      $row_documents = apachesolr_index_entities_document($row, $entity_type, $env_id);
      // TODO argument #1 is not an array
      $documents = array_merge($documents, $row_documents);
    }

    $indexed = apachesolr_index_send_to_solr($env_id, $documents);
    if ($indexed !== FALSE) {
      $documents_submitted += count($documents);
      // Check who's the last in line
      $last_row = end($rows);

      // set our last position to the entity id and changed value so we can
      // keep track where we left off
      if (!empty($last_row->changed) && !empty($last_row->entity_id)) {
        apachesolr_set_last_index_position($env_id, $entity_type, $last_row->changed, $last_row->entity_id);
      }
      else {
        $message = 'Environment @env_id: Failure recording indexing progress. Last entity id processed: %entity_id with timestamp %last_changed';
        $variables = array(
          '@env_id' => $env_id,
          '%entity_id' => $last_row->entity_id,
          '%last_changed' => $last_row->changed,
        );
        // Add it to watchdog
        watchdog('Apache Solr', $message, $variables, WATCHDOG_ERROR);
      }
      apachesolr_set_last_index_updated($env_id, REQUEST_TIME);
    }
  }
  return $documents_submitted;
}

/**
 * Convert a certain entity from the apachesolr index table to a set of documents. 1 entity
 * can be converted in multiple documents if the apachesolr_index_entity_to_documents decides to do so.
 *
 * @param array $row
 *   A row from the indexing table
 * @param string $entity_type
 *   The type of the entity
 * @param string $env_id
 *   The machine name of the environment.
 *
 * @return array of ApacheSolrDocument(s)
 */
function apachesolr_index_entities_document($row, $entity_type, $env_id) {
  $documents = array();
  if (!empty($row->status)) {
    // Let any module exclude this entity from the index.
    $build_document = TRUE;
    foreach (module_implements('apachesolr_exclude') as $module) {
      $exclude = module_invoke($module, 'apachesolr_exclude', $row->entity_id, $entity_type, $row, $env_id);
      // If the hook returns TRUE we should exclude the entity
      if (!empty($exclude)) {
        $build_document = FALSE;
      }
    }
    foreach (module_implements('apachesolr_' . $entity_type . '_exclude') as $module) {
      $exclude = module_invoke($module, 'apachesolr_' . $entity_type . '_exclude', $row->entity_id, $row, $env_id);
      // If the hook returns TRUE we should exclude the entity
      if (!empty($exclude)) {
        $build_document = FALSE;
      }
    }
    if ($build_document) {
      // TODO argument #2 is not an array
      $documents = array_merge($documents, apachesolr_index_entity_to_documents($row, $env_id));
    }
  }
  else {
    // Delete the entity from our index if the status callback returned 0
    apachesolr_remove_entity($env_id, $row->entity_type, $row->entity_id);
  }
  // Clear entity cache for this specific entity
  entity_get_controller($row->entity_type)->resetCache(array($row->entity_id));
  return $documents;
}
/**
 * Returns the total number of documents that are able to be indexed and the
 * number of documents left to be indexed.
 *
 * This is a helper function for modules that implement hook_search_status().
 *
 * @param string $env_id
 *   The machine name of the environment.
 *
 * @return array
 *   An associative array with the key-value pairs:
 *   - remaining: The number of items left to index.
 *   - total: The total number of items to index.
 *
 * @see hook_search_status()
 */
function apachesolr_index_status($env_id) {
  $remaining = 0;
  $total = 0;

  foreach (entity_get_info() as $entity_type => $info) {
    $bundles = apachesolr_get_index_bundles($env_id, $entity_type);
    if (empty($bundles)) {
      continue;
    }

    $table = apachesolr_get_indexer_table($entity_type);
    $query = db_select($table, 'aie')
      ->condition('aie.status', 1)
      ->condition('aie.bundle', $bundles)
      ->addTag('apachesolr_index_' . $entity_type);

    $total += $query->countQuery()->execute()->fetchField();

    $query = _apachesolr_index_get_next_set_query($env_id, $entity_type);
    $remaining += $query->countQuery()->execute()->fetchField();
  }
  return array('remaining' => $remaining, 'total' => $total);
}

/**
 * Worker callback for apachesolr_index_entities().
 *
 * Loads and proccesses the entity queued for indexing and converts into one or
 * more documents that are sent to the Apache Solr server for indexing.
 *
 * The entity is loaded as the user specified in the "apachesolr_index_user"
 * system variable in order to prevent sentive data from being indexed and
 * displayed to underprivileged users in search results. The index user defaults
 * to a user ID of "0", which is the anonymous user.
 *
 * After the entity is loaded, it will be handed over to
 * apachesolr_convert_entity_to_documents() to be converted to an array via
 * the callback specified in the entity type's info array. The array that the
 * entity is converted to is the model of the document sent to the Apache Solr
 * server for indexing. This function allows developers to modify the document
 * by implementing the following hooks:
 * - apachesolr_index_document_build()
 * - apachesolr_index_document_build_ENTITY_TYPE()
 * - apachesolr_index_documents_alter()
 *
 * @param stdClass $item
 *   The data returned by the queue table containing:
 *   - entity_id: An integer containing the unique identifier of the entity, for
 *     example a node ID or comment ID.
 *   - entity_type: The unique identifier for the entity, i.e. "node", "file".
 *   - bundle: The machine-readable name of the bundle the passed entity is
 *     associated with.
 *   - status: The "published" status of the entity. The status will also be set
 *     to "0" when entity is deleted but the Apache Solr server is unavailable.
 *   - changed: A timestamp flagging when the entity was last modified.
 * @param string $env_id
 *   The machine name of the environment.
 *
 * @return array
 *   An associative array of documents that are sent to the Apache Solr server
 *   for indexing.
 *
 * @see apachesolr_index_nodes() for the old-skool version.
 */
function apachesolr_index_entity_to_documents($item, $env_id) {
  global $user;
  drupal_save_session(FALSE);
  $saved_user = $user;
  // build the content for the index as an anonymous user to avoid exposing restricted fields and such.
  // By setting a variable, indexing can take place as a different user
  $uid = variable_get('apachesolr_index_user', 0);
  if ($uid == 0) {
    $user = drupal_anonymous_user();
  }
  else {
    $user = user_load($uid);
  }
  // Pull out all of our pertinent data.
  $entity_type = $item->entity_type;

  // Entity cache will be reset at the end of the indexing algorithm, to use the cache properly whenever
  // the code does another entity_load
  $entity = entity_load($entity_type, array($item->entity_id));
  $entity = $entity ? reset($entity) : FALSE;

  if (empty($entity)) {
    // If the object failed to load, just stop.
    return FALSE;
  }

  $documents = apachesolr_convert_entity_to_documents($entity, $entity_type, $env_id);

  // Restore the user.
  $user = $saved_user;
  drupal_save_session(TRUE);

  return $documents;
}

/**
 * The given entity is converted to an array via the callback
 * specified in the entity type's info array. The array that the entity is
 * converted to is the model of the document sent to the Apache Solr server for
 * indexing. This function allows developers to modify the document by
 * implementing the following hooks:
 * - apachesolr_index_document_build()
 * - apachesolr_index_document_build_ENTITY_TYPE()
 * - apachesolr_index_documents_alter()
 *
 * This function's code has been isolated from
 * apachesolr_index_entity_to_documents() to a separate function to be re-used
 * by apachesolr_multilingual_apachesolr_index_documents_alter().
 *
 * @param object $entity
 *   The entity for which we want a document.
 * @param string $entity_type
 *   The type of entity we're processing.
 * @param string $env_id
 *   The machine name of the environment.
 *
 * @return array
 *   An associative array of documents that are sent to the Apache Solr server
 *   for indexing.
 */
function apachesolr_convert_entity_to_documents($entity, $entity_type, $env_id) {
  list($entity_id, $vid, $bundle) = entity_extract_ids($entity_type, $entity);

  // Create a new document, and do the bare minimum on it.
  $document = _apachesolr_index_process_entity_get_document($entity, $entity_type);

  //Get the callback array to add stuff to the document
  $document_callbacks = apachesolr_entity_get_callback($entity_type, 'document callback', $bundle);
  $documents = array();
  foreach ($document_callbacks as $document_callback) {
    // Call a type-specific callback to add stuff to the document.
    $documents = array_merge($documents, $document_callback($document, $entity, $entity_type, $env_id));
  }

  //do this for all possible documents that were returned by the callbacks
  foreach ($documents as $document) {
    // Call an all-entity hook to add stuff to the document.
    module_invoke_all('apachesolr_index_document_build', $document, $entity, $entity_type, $env_id);

    // Call a type-specific hook to add stuff to the document.
    module_invoke_all('apachesolr_index_document_build_' . $entity_type, $document, $entity, $env_id);

    // Final processing to ensure that the document is properly structured.
    // All records must have a label field, which is used for user-friendly labeling.
    if (empty($document->label)) {
      $document->label = '';
    }

    // All records must have a "content" field, which is used for fulltext indexing.
    // If we don't have one, enter an empty value.  This does mean that the entity
    // will not be fulltext searchable.
    if (empty($document->content)) {
      $document->content = '';
    }

    // All records must have a "teaser" field, which is used for abbreviated
    // displays when no highlighted text is available.
    if (empty($document->teaser)) {
      $document->teaser = truncate_utf8($document->content, 300, TRUE);
    }
  }

  // Now allow modules to alter each other's additions for maximum flexibility.

  // Hook to allow modifications of the retrieved results
  foreach (module_implements('apachesolr_index_documents_alter') as $module) {
    $function = $module . '_apachesolr_index_documents_alter';
    $function($documents, $entity, $entity_type, $env_id);
  }

  return $documents;
}

/**
 * Index an array of documents to solr.
 *
 * @param $env_id
 * @param array $documents
 *
 * @return bool|int number indexed, or FALSE on failure.
 * @throws Exception
 */
function apachesolr_index_send_to_solr($env_id, array $documents) {
  // Get the $solr object
  $solr = apachesolr_get_solr($env_id);

  // Do not index when we do not have any documents to send
  // Send TRUE because this is not an error
  if (empty($documents)) {
    return TRUE;
  }
  // Send the document off to Solr.
  $log_success = variable_get('apachesolr_watchdog_successes', TRUE);
  if ($log_success) {
    watchdog('Apache Solr', 'Environment @env_id: Adding @count documents.', array('@env_id' => $env_id, '@count' => count($documents)));
  }
  try {
    $docs_chunk = array_chunk($documents, 20);
    foreach ($docs_chunk as $docs) {
      $solr->addDocuments($docs);
    }
    if ($log_success) {
      watchdog('Apache Solr', 'Environment @env_id: Indexing succeeded on @count documents', array(
        '@env_id' => $env_id,
        '@count' => count($documents),
      ), WATCHDOG_INFO);
    }
    return count($documents);
  }
  catch (Exception $e) {
    if (!empty($docs)) {
      foreach ($docs as $doc) {
        $eids[] = $doc->entity_type . '/' . $doc->entity_id;
      }
    }
    watchdog('Apache Solr', 'Environment @env_id: Indexing failed on one of the following entity ids: @eids <br /> !message', array(
      '@env_id' => $env_id,
      '@eids' => implode(', ', $eids),
      '!message' => nl2br(strip_tags($e->getMessage())),
    ), WATCHDOG_ERROR);
    return FALSE;
  }
}

function _apachesolr_tags_to_index() {
  $tags_to_index = variable_get('apachesolr_tags_to_index', array(
    'h1' => 'tags_h1',
    'h2' => 'tags_h2_h3',
    'h3' => 'tags_h2_h3',
    'h4' => 'tags_h4_h5_h6',
    'h5' => 'tags_h4_h5_h6',
    'h6' => 'tags_h4_h5_h6',
    'u' => 'tags_inline',
    'b' => 'tags_inline',
    'i' => 'tags_inline',
    'strong' => 'tags_inline',
    'em' => 'tags_inline',
    'a' => 'tags_a'
  ));
  return $tags_to_index;
}

/**
 * Extract HTML tag contents from $text and add to boost fields.
 *
 * @param ApacheSolrDocument $document
 * @param string $text
 *   must be stripped of control characters before hand.
 *
 */
function apachesolr_index_add_tags_to_document(ApacheSolrDocument $document, $text) {
  $tags_to_index = _apachesolr_tags_to_index();

  // Strip off all ignored tags.
  $allowed_tags = '<' . implode('><', array_keys($tags_to_index)) . '>';
  $text = strip_tags($text, $allowed_tags);

  preg_match_all('@<(' . implode('|', array_keys($tags_to_index)) . ')[^>]*>(.*)</\1>@Ui', $text, $matches);
  foreach ($matches[1] as $key => $tag) {
    $tag = drupal_strtolower($tag);
    // We don't want to index links auto-generated by the url filter.
    if ($tag != 'a' || !preg_match('@(?:http://|https://|ftp://|mailto:|smb://|afp://|file://|gopher://|news://|ssl://|sslv2://|sslv3://|tls://|tcp://|udp://|www\.)[a-zA-Z0-9]+@', $matches[2][$key])) {
      if (!isset($document->{$tags_to_index[$tag]})) {
        $document->{$tags_to_index[$tag]} = '';
      }
      $document->{$tags_to_index[$tag]} .= ' ' . apachesolr_clean_text($matches[2][$key]);
    }
  }
}

/**
 * Returns a generic Solr document object for this entity.
 *
 * This function will do the basic processing for the document that is common
 * to all entities, but virtually all entities will need their own additional
 * processing.
 *
 * @param object $entity
 *   The entity for which we want a document.
 * @param string $entity_type
 *   The type of entity we're processing.
 * @return ApacheSolrDocument
 */
function _apachesolr_index_process_entity_get_document($entity, $entity_type) {
  list($entity_id, $vid, $bundle) = entity_extract_ids($entity_type, $entity);

  $document = new ApacheSolrDocument();

  // Define our url options in advance. This differs depending on the
  // language
  $languages = language_list();
  $url_options = array('absolute' => TRUE);
  if (isset($entity->language) && isset($languages[$entity->language])) {
    $url_options['language'] = $languages[$entity->language];
  }

  $document->id = apachesolr_document_id($entity_id, $entity_type);
  $document->site = url(NULL, $url_options);
  $document->hash = apachesolr_site_hash();

  $document->entity_id = $entity_id;
  $document->entity_type = $entity_type;
  $document->bundle = $bundle;
  $document->bundle_name = entity_bundle_label($entity_type, $bundle);

  if (empty($entity->language)) {
    // 'und' is the language-neutral code in Drupal 7.
    $document->ss_language = LANGUAGE_NONE;
  }
  else {
    $document->ss_language = $entity->language;
  }

  $path = entity_uri($entity_type, $entity);
  // A path is not a requirement of an entity
  if (!empty($path)) {
    $document->path = $path['path'];
    $document->url = url($path['path'], $path['options'] + $url_options);
    // Path aliases can have important information about the content.
    // Add them to the index as well.
    if (function_exists('drupal_get_path_alias')) {
      // Add any path alias to the index, looking first for language specific
      // aliases but using language neutral aliases otherwise.
      $output = drupal_get_path_alias($document->path, $document->ss_language);
      if ($output && $output != $document->path) {
        $document->path_alias = $output;
      }
    }
  }
  return $document;
}

/**
 * Returns an array of rows from a query based on an indexing environment.
 * @todo Remove the read only because it is not environment specific
 *
 * @param $env_id
 * @param $entity_type
 * @param $limit
 *
 * @return array list of row to index
 */
function apachesolr_index_get_entities_to_index($env_id, $entity_type, $limit) {
  $rows = array();
  if (apachesolr_environment_variable_get($env_id, 'apachesolr_read_only', APACHESOLR_READ_WRITE) == APACHESOLR_READ_ONLY) {
    return $rows;
  }
  $bundles = apachesolr_get_index_bundles($env_id, $entity_type);
  if (empty($bundles)) {
    return $rows;
  }

  // Get next batch of entities to index
  $query = _apachesolr_index_get_next_set_query($env_id, $entity_type);
  
  $query->range(0, $limit);
  $records = $query->execute();

  $status_callbacks = array();
  foreach ($records as $record) {
    if (!isset($status_callbacks[$record->bundle])) {
      $status_callbacks[$record->bundle] = apachesolr_entity_get_callback($entity_type, 'status callback', $record->bundle);
    }
    // Check status and status callbacks before sending to the index
    if (is_array($status_callbacks[$record->bundle])) {
      foreach ($status_callbacks[$record->bundle] as $status_callback) {
        if (is_callable($status_callback)) {
          // by placing $status in front we prevent calling any other callback
          // after one status callback returned false
          $record->status = $record->status && $status_callback($record->entity_id, $record->entity_type);
        }
      }
    }
    $rows[] = $record;
  }
  return $rows;
}

/**
 * Delete the whole index for an environment.
 *
 * @param string $env_id
 *   The machine name of the environment.
 * @param string $entity_type
 *   (optional) specify to remove just this entity_type from the index.
 * @param string $bundle
 *   (optional) also specify a bundle to remove just the bundle from
 *   the index.
 *
 * @return bool
 *   TRUE for success, FALSE if an error occured.
 */
function apachesolr_index_delete_index($env_id, $entity_type = NULL, $bundle = NULL) {
  if (apachesolr_index_env_is_readonly($env_id)) {
    apachesolr_index_report_readonly($env_id);
    return FALSE;
  }

  // Instantiate a new Solr object.
  try {
    $solr = apachesolr_get_solr($env_id);
    $query = '*:*';

    if (!empty($entity_type)) {
      if (!empty($bundle)) {
        $query = "(bundle:$bundle AND entity_type:$entity_type) OR sm_parent_entity_bundle:{$entity_type}-{$bundle}";
      }
      else {
        $query = "(entity_type:$entity_type)";
      }
    }
    elseif (!empty($bundle)) {
      $query = "(bundle:$bundle)";
    }

    // Allow other modules to modify the delete query.
    // For example, use the site hash so that you only delete this site's
    // content:  $query = 'hash:' . apachesolr_site_hash()
    drupal_alter('apachesolr_delete_by_query', $query);
    $solr->deleteByQuery($query);
    $solr->commit();

    // Log the query used for deletion.
    $log_success = variable_get('apachesolr_watchdog_successes', TRUE);
    if ($log_success) {
      watchdog('Apache Solr', 'Environment @env_id: Deleted documents from index with query @query', array('@env_id' => $env_id, '@query' => $query), WATCHDOG_INFO);
    }

    if (!empty($entity_type)) {
      $reindex_callback = apachesolr_entity_get_callback($entity_type, 'reindex callback');
      if (is_callable($reindex_callback)) {
        $reindex_callback($env_id, $bundle);
      }
    }
    else {
      apachesolr_index_mark_for_reindex($env_id);
    }

    apachesolr_set_last_index_updated($env_id, REQUEST_TIME);
  }
  catch (Exception $e) {
    apachesolr_log_exception($env_id, $e);
    return FALSE;
  }
  return TRUE;
}

/**
 * Internal function that identifies entities that are still due to be indexed.
 *
 * @param string $env_id Environment ID
 * @param string $entity_type
 *
 * @return SelectQuery
 */
function _apachesolr_index_get_next_set_query($env_id, $entity_type) {
  $table = apachesolr_get_indexer_table($entity_type);
  // Get $last_entity_id and $last_changed.
  $last_index_position = apachesolr_get_last_index_position($env_id, $entity_type);
  $bundles = apachesolr_get_index_bundles($env_id, $entity_type);

  $last_entity_id = $last_index_position['last_entity_id'];
  $last_changed = $last_index_position['last_changed'];

  // Find the next batch of entities to index for this entity type.  Note that
  // for ordering we're grabbing the oldest first and then ordering by ID so
  // that we get a definitive order.
  // Also note that we fetch ALL fields from the indexer table
  $query = db_select($table, 'aie')
    ->fields('aie')
    ->condition('aie.bundle', $bundles)
    ->condition('aie.status', 1)
    ->condition(db_or()
      ->condition('aie.changed', $last_changed, '>')
      // Tie breaker for entities that were changed at exactly
      // the same second as the last indexed entity
      ->condition(db_and()
        ->condition('aie.changed', $last_changed, '=')
        ->condition('aie.entity_id', $last_entity_id, '>')
      )
    )
    // It is important that everything is indexed in order of changed date and
    // then on entity_id because otherwise the conditions above will not match
    // correctly
    ->orderBy('aie.changed', 'ASC')
    ->orderBy('aie.entity_id', 'ASC')
    ->addTag('apachesolr_index_' . $entity_type);

  if ($table == 'apachesolr_index_entities') {
    // Other, entity-specific tables don't need this condition.
    $query->condition('aie.entity_type', $entity_type);
  }
  return $query;
}

/**
 * Delete from the index documents with the entity type and any of the excluded bundles.
 *
 * Also deletes all documents that have the entity type and bundle as a parent.
 *
 * @param string $env_id
 *   The machine name of the environment.
 * @param string $entity_type
 * @param array $excluded_bundles
 *
 * @return true on success, false on failure.
 */
function apachesolr_index_delete_bundles($env_id, $entity_type, array $excluded_bundles) {
  if (apachesolr_index_env_is_readonly($env_id)) {
    apachesolr_index_report_readonly($env_id);
    return FALSE;
  }

  // Remove newly omitted bundles.
  try {
    $solr = apachesolr_get_solr($env_id);
    foreach ($excluded_bundles as $bundle) {
      $query = "(bundle:$bundle AND entity_type:$entity_type) OR sm_parent_entity_bundle:{$entity_type}-{$bundle}";

      // Allow other modules to modify the delete query.
      // For example, use the site hash so that you only delete this site's
      // content:  $query = 'hash:' . apachesolr_site_hash()
      drupal_alter('apachesolr_delete_by_query', $query);
      $solr->deleteByQuery($query);

      // Log the query used for deletion.
      $log_success = variable_get('apachesolr_watchdog_successes', TRUE);
      if ($log_success) {
        watchdog('Apache Solr', 'Environment @env_id: Deleted documents from index with query @query', array('@env_id' => $env_id, '@query' => $query), WATCHDOG_INFO);
      }
    }
    if ($excluded_bundles) {
      $solr->commit();
    }
    return TRUE;
  }
  catch (Exception $e) {
    apachesolr_log_exception($env_id, $e);
    return FALSE;
  }
}

/**
 * Delete an entity from the index.
 *
 * Also deletes all documents that have the deleted document as a parent.
 *
 * @param string $env_id
 *   The machine name of the environment.
 * @param string $entity_type
 * @param string $entity_id
 *
 * @return true on success, false on failure.
 */
function apachesolr_index_delete_entity_from_index($env_id, $entity_type, $entity_id) {
  // Keep track of failures per environment.
  static $failed = array();
  if (!empty($failed[$env_id])) {
    return FALSE;
  }

  if (apachesolr_index_env_is_readonly($env_id)) {
    apachesolr_index_report_readonly($env_id);
    return FALSE;
  }

  try {
    $solr = apachesolr_get_solr($env_id);
    $document_id = apachesolr_document_id($entity_id, $entity_type);
    $query = "id:\"$document_id\" OR sm_parent_document_id:\"$document_id\"";
    $solr->deleteByQuery($query);
    // Log the query used for deletion.
    $log_success = variable_get('apachesolr_watchdog_successes', TRUE);
    if ($log_success) {
      watchdog('Apache Solr', 'Environment @env_id: Deleted documents from index with query @query', array('@env_id' => $env_id, '@query' => $query), WATCHDOG_INFO);
    }
    apachesolr_set_last_index_updated($env_id, REQUEST_TIME);
    return TRUE;
  }
  catch (Exception $e) {
    apachesolr_log_exception($env_id, $e);
    // Don't keep trying queries if they are failing for this environment.
    $failed[$env_id] = TRUE;
    return FALSE;
  }
}

/**
 * Mark a certain entity type for a specific environment for reindexing.
 *
 * @param $env_id
 * @param null $entity_type
 */
function apachesolr_index_mark_for_reindex($env_id, $entity_type = NULL) {
  $entities = entity_get_info();
  $node_types = node_type_get_names();
  foreach ($node_types as $node_type => $node_info) {
    if ($node_type == $entity_type) {
      if (isset($entities['node']['apachesolr']) && ($entities['node']['apachesolr']['indexable'])) {
        $reindex_callback = apachesolr_entity_get_callback('node', 'reindex callback');
        if (!empty($reindex_callback)) {
          call_user_func($reindex_callback, $env_id, $entity_type);
        }
      }
    }
  }
  foreach ($entities as $type => $entity_info) {
    if (($type == $entity_type) || ($entity_type == NULL)) {
      if (isset($entity_info['apachesolr']) && ($entity_info['apachesolr']['indexable'])) {
        $reindex_callback = apachesolr_entity_get_callback($type, 'reindex callback');
        if (!empty($reindex_callback)) {
          call_user_func($reindex_callback, $env_id);
        }
      }
    }
  }
  apachesolr_clear_last_index_position($env_id, $entity_type);
  cache_clear_all('*', 'cache_apachesolr', TRUE);
}

/**
 * Sets what bundles on the specified entity type should be indexed.
 *
 * @param string $env_id
 *   The machine name of the environment.
 * @param string $entity_type
 *   The entity type to index.
 * @param array $bundles
 *   The machine names of the bundles to index.
 *
 * @throws Exception
 */
function apachesolr_index_set_bundles($env_id, $entity_type, array $bundles) {
  $transaction = db_transaction();
  try {
    db_delete('apachesolr_index_bundles')
      ->condition('env_id', $env_id)
      ->condition('entity_type', $entity_type)
      ->execute();

    if ($bundles) {
      $insert = db_insert('apachesolr_index_bundles')
        ->fields(array('env_id', 'entity_type', 'bundle'));

      foreach ($bundles as $bundle) {
        $insert->values(array(
          'env_id' => $env_id,
          'entity_type' => $entity_type,
          'bundle' => $bundle,
        ));
      }
      $insert->execute();
    }
  }
  catch (Exception $e) {
    $transaction->rollback();
    // Re-throw the exception so we are aware of the failure.
    throw $e;
  }
}

// This really should be in core, but it isn't yet.  When it gets added to core,
// we can remove this version.
// @see http://drupal.org/node/969180
if (!function_exists('entity_bundle_label')) {

/**
 * Returns the label of a bundle.
 *
 * @param string $entity_type
 *   The entity type; e.g. 'node' or 'user'.
 * @param string $bundle_name
 *   The bundle for which we want the label from
 *
 * @return
 *   A string with the human-readable name of the bundle, or FALSE if not specified.
 */
function entity_bundle_label($entity_type, $bundle_name) {
  $labels = &drupal_static(__FUNCTION__, array());

  if (empty($labels)) {
    foreach (entity_get_info() as $type => $info) {
      foreach ($info['bundles'] as $bundle => $bundle_info) {
        $labels[$type][$bundle] = !empty($bundle_info['label']) ? $bundle_info['label'] : FALSE;
      }
    }
  }

  return $labels[$entity_type][$bundle_name];
}

}

/**
 * Builds the node-specific information for a Solr document.
 *
 * @param ApacheSolrDocument $document
 *   The Solr document we are building up.
 * @param object $node
 *   The entity we are indexing.
 * @param string $entity_type
 *   The type of entity we're dealing with.
 * @param string $env_id
 *   The type of entity we're dealing with.
 *
 * @return array A set of ApacheSolrDocument documents
 */
function apachesolr_index_node_solr_document(ApacheSolrDocument $document, $node, $entity_type, $env_id) {
  // None of these get added unless they are explicitly in our schema.xml
  $document->label = apachesolr_clean_text($node->title);

  // Build the node body.
  $language = !empty($node->language) ? $node->language : LANGUAGE_NONE;
  $build = node_view($node, 'search_index', $language);
  // Remove useless html crap out of the render.
  unset($build['#theme']);
  // Allow cache if it's present
  $build['#cache'] = true;
  // Render it into html
  $text = drupal_render($build);
  $document->content = apachesolr_clean_text($text);

  // Adding the teaser
  if (isset($node->teaser)) {
    $document->teaser = apachesolr_clean_text($node->teaser);
  }
  else {
    // If there is no node teaser we will have to generate the teaser
    // ourselves. We have to be careful to not leak the author and other
    // information that is normally also not visible.
    if (isset($node->body[$language][0]['safe_summary'])) {
      $document->teaser = apachesolr_clean_text($node->body[$language][0]['safe_summary']);
    }
    else {
      $document->teaser = truncate_utf8($document->content, 300, TRUE);
    }
  }

  // Author information
  if ($node->uid == 0 || strlen($node->name) == 0) {
    // @see user_validate_name(). !'0' === TRUE.
    $document->ss_name = '0';
  }
  else {
    $document->ss_name = $node->name;
    // We want the name to be searchable for keywords.
    $document->tos_name = $node->name;
  }
  // Index formatted username so it can be searched and sorted on.
  $account = (object) array('uid' => $node->uid, 'name' => $node->name);
  $username = format_username($account);
  $document->ss_name_formatted = $username;
  $document->tos_name_formatted = $username;
  $document->is_uid = $node->uid;
  $document->bs_status = $node->status;
  $document->bs_sticky = $node->sticky;
  $document->bs_promote = $node->promote;
  $document->is_tnid = $node->tnid;
  $document->bs_translate = $node->translate;

  // Timestamp of the node
  $document->ds_created = apachesolr_date_iso($node->created);
  $document->ds_changed = apachesolr_date_iso($node->changed);

  // Comment counts + time
  if (isset($node->last_comment_timestamp) && !empty($node->comment_count)) {
    $document->ds_last_comment_timestamp = apachesolr_date_iso($node->last_comment_timestamp);
    $document->ds_last_comment_or_change = apachesolr_date_iso(max($node->last_comment_timestamp, $node->changed));
    $document->is_comment_count = $node->comment_count;
  }
  else {
    $document->ds_last_comment_or_change = apachesolr_date_iso($node->changed);
  }

  // Fetch extra data normally not visible, including comments.
  // We do this manually (with module_implements instead of node_invoke_nodeapi)
  // because we want a keyed array to come back. Only in this way can we decide
  // whether to index comments or not.
  $extra = array();
  $excludes = variable_get('apachesolr_exclude_nodeapi_types', array());
  $exclude_nodeapi = isset($excludes[$node->type]) ? $excludes[$node->type] : array();

  foreach (module_implements('node_update_index') as $module) {
    // Invoke nodeapi if this module has not been excluded, for example,
    // exclude 'comment' for a type to skip indexing its comments.
    if (empty($exclude_nodeapi[$module])) {
      $function = $module . '_node_update_index';
      if ($output = $function($node)) {
        $extra[$module] = $output;
      }
    }
  }

  // Adding the text of the comments
  if (isset($extra['comment'])) {
    $comments = $extra['comment'];
    // Remove comments from the extra fields
    unset($extra['comment']);
    $document->ts_comments = apachesolr_clean_text($comments);
    // @todo: do we want to reproduce apachesolr_add_tags_to_document() for comments?
  }
  // If there are other extra fields, add them to the document
  if (!empty($extra)) {
    // Use an omit-norms text field since this is generally going to be short; not
    // really a full-text field.
    $document->tos_content_extra = apachesolr_clean_text(implode(' ', $extra));
  }

  // Add additional indexing based on the body of each record.
  apachesolr_index_add_tags_to_document($document, $text);

  //  Generic use case for future reference. Callbacks can
  //  allow you to send back multiple documents
  $documents = array();
  $documents[] = $document;
  return $documents;
}

/**
 * Function that will be executed if the node bundles were updated.
 * Currently it does nothing, but it could potentially do something later on.
 *
 * @param $env_id
 * @param $existing_bundles
 * @param $new_bundles
 */
function apachesolr_index_node_bundles_changed($env_id, $existing_bundles, $new_bundles) {
  // Nothing to do for now.
}

/**
 * Reindexing callback for ApacheSolr, for nodes.
 *
 * @param string $env_id
 *   The machine name of the environment.
 * @param string|null $bundle
 *   (optional) The bundle type to reindex. If not used
 *   all bundles will be re-indexed.
 *
 * @return null
 *   returns NULL if the specified bundle is not in the indexable bundles list
 *
 * @throws Exception
 */
function apachesolr_index_node_solr_reindex($env_id, $bundle = NULL) {
  $indexer_table = apachesolr_get_indexer_table('node');
  $transaction = db_transaction();
  try {
    $indexable_bundles = apachesolr_get_index_bundles($env_id, 'node');

    if ($bundle && !empty($indexable_bundles) && !in_array($bundle, $indexable_bundles)) {
      // The bundle specified is not in the indexable bundles list.
      return NULL;
    }

    // Leave status 0 rows - those need to be
    // removed from the index later.
    $delete = db_delete($indexer_table);
    $delete->condition('status', 1);

    if (!empty($bundle)) {
      $delete->condition('bundle', $bundle);
    }
    elseif (!empty($indexable_bundles)) {
      $delete->condition('bundle', $indexable_bundles, 'IN');
    }

    $delete->execute();

    $select = db_select('node', 'n');
    $select->condition('status', 1);
    $select->addExpression("'node'", 'entity_type');
    $select->addField('n', 'nid', 'entity_id');
    $select->addField('n', 'type', 'bundle');
    $select->addField('n', 'status', 'status');
    $select->addExpression(REQUEST_TIME, 'changed');

    if ($bundle) {
      // Mark all nodes of the specified content type for reindexing.
      $select->condition('n.type', $bundle);
    }
    elseif (!empty($indexable_bundles)) {
      // Restrict reindex to content types in the indexable bundles list.
      $select->condition('n.type', $indexable_bundles, 'IN');
    }

    $insert = db_insert($indexer_table)
      ->fields(array('entity_id', 'bundle', 'status', 'entity_type', 'changed'))
      ->from($select)
      ->execute();
  }
  catch (Exception $e) {
    $transaction->rollback();
    throw $e;
  }
}

/**
 * Status callback for ApacheSolr, for nodes.
 * after indexing a certain amount of nodes
 *
 * @param $entity_id
 * @param $entity_type
 * @param $entity
 *   In the case where the status is being checked while the entity is being
 *   saved, this contains the full entity object. In other cases, it will be
 *   NULL.
 *
 * @return int
 *   The status of the node
 */
function apachesolr_index_node_status_callback($entity_id, $entity_type, $entity = NULL) {
  if ($entity === NULL) {
    $entity = entity_load($entity_type, array($entity_id));
    $entity = $entity ? reset($entity) : FALSE;
  }
  if (empty($entity)) {
    // If the object failed to load, just stop.
    return FALSE;
  }
  // Make sure we have an integer value.
  // Anything different from 1 becomes zero
  return ($entity->status == 1 ? 1 : 0);
}

/**
 * Callback that converts term_reference field into an array
 *
 * @param object $node
 * @param string $field_name
 * @param string $index_key
 * @param array $field_info
 * @return array $fields
 *   fields that will be indexed for this term reference
 */
function apachesolr_term_reference_indexing_callback($node, $field_name, $index_key, array $field_info) {
  // Keep ancestors cached
  $ancestors = &drupal_static(__FUNCTION__, array());

  $fields = array();
  $vocab_names = array();
  if (!empty($node->{$field_name}) && function_exists('taxonomy_get_parents_all')) {
    $field = $node->$field_name;
    $items = current($field);
    foreach ($items as $item) {
      // Triple indexing of tids lets us do efficient searches (on tid)
      // and do accurate per field or per-vocabulary faceting.

      // By including the ancestors to a term in the index we make
      // sure that searches for general categories match specific
      // categories, e.g. Fruit -> apple, a search for fruit will find
      // content categorized with apple.
      if (!isset($ancestors[$item['tid']])) {
        $ancestors[$item['tid']] = taxonomy_get_parents_all($item['tid']);
      }
      foreach ($ancestors[$item['tid']] as $ancestor) {
        // Index parent term against the field. Note that this happens
        // regardless of whether the facet is set to show as a hierarchy or not.
        // We would need a separate field if we were to index terms without any
        // hierarchy at all.
        // If the term is singular, then we cannot add another value to the
        // document as the field is single
        if ($field_info['multiple']) {
          $fields[] = array(
            'key' => $index_key,
            'value' => $ancestor->tid,
          );
        }
        $fields[] = array(
          'key' => 'tid',
          'value' => $ancestor->tid,
        );
        $fields[] = array(
          'key' => 'im_vid_' . $ancestor->vid,
          'value' => $ancestor->tid,
        );
        $name = apachesolr_clean_text($ancestor->name);
        $vocab_names[$ancestor->vid][] = $name;
        // We index each name as a string for cross-site faceting
        // using the vocab name rather than vid in field construction .
        $fields[] = array(
          'key' => 'sm_vid_' . apachesolr_vocab_name($ancestor->vid),
          'value' => $name,
        );
      }
    }
    // Index the term names into a text field for MLT queries and keyword searching.
    foreach ($vocab_names as $vid => $names) {
      $fields[] = array(
        'key' => 'tm_vid_' . $vid . '_names',
        'value' => implode(' ', $names),
      );
    }
  }
  return $fields;
}

/**
 * Helper function - return a safe (PHP identifier) vocabulary name.
 *
 * @param integer $vid
 * @return string
 */
function apachesolr_vocab_name($vid) {
  $names = &drupal_static(__FUNCTION__, array());

  if (!isset($names[$vid])) {
    $vocab_name = db_query('SELECT v.name FROM {taxonomy_vocabulary} v WHERE v.vid = :vid', array(':vid' => $vid))->fetchField();
    $names[$vid] = preg_replace('/[^a-zA-Z0-9_\x7f-\xff]/', '_', $vocab_name);
    // Fallback for names ending up all as '_'.
    $check = rtrim($names[$vid], '_');
    if (!$check) {
      $names[$vid] = '_' . $vid . '_';
    }
  }
  return $names[$vid];
}

/**
 * Callback that converts list module field into an array
 * For every multivalued value we also add a single value to be able to
 * use the stats
 *
 * @param object $entity
 * @param string $field_name
 * @param string $index_key
 * @param array $field_info
 * @return array $fields
 */
function apachesolr_fields_default_indexing_callback($entity, $field_name, $index_key, array $field_info) {
  $fields = array();
  $numeric = TRUE;
  if (!empty($entity->{$field_name})) {
    $field = $entity->$field_name;
    $values = current($field);
    switch ($field_info['index_type']) {
      case 'integer':
      case 'half-int':
      case 'sint':
      case 'tint':
      case 'thalf-int':
      case 'boolean':
        $function = 'intval';
        break;
      case 'float':
      case 'double':
      case 'sfloat':
      case 'sdouble':
      case 'tfloat':
      case 'tdouble':
        $function = 'apachesolr_floatval';
        break;
      default:
        $numeric = FALSE;
        $function = 'apachesolr_clean_text';
    }
    for ($i = 0; $i < count($values); $i++) {
      $fields[] = array(
        'key' => $index_key,
        'value' => $function($values[$i]['value']),
      );
    }
    // Also store the first value of the field in a singular index for multi value fields
    if ($field_info['multiple'] && $numeric && !empty($values[0])) {
      $singular_field_info = $field_info;
      $singular_field_info['multiple'] = FALSE;
      $single_key = apachesolr_index_key($singular_field_info);
      $fields[] = array(
        'key' => $single_key,
        'value' => $function($values[0]['value']),
      );
    }
  }
  return $fields;
}

/**
 * This function is used during indexing to normalize the DATE and DATETIME
 * fields into the appropriate format for Apache Solr.
 *
 * @param object $entity
 * @param string $field_name
 * @param string $index_key
 * @param array $field_info
 * @return array $fields
 */
function apachesolr_date_default_indexing_callback($entity, $field_name, $index_key, array $field_info) {
  $fields = array();
  if (!empty($entity->{$field_name})) {
    $field = $entity->$field_name;
    $values = current($field);
    // Construct a Solr-ready date string in UTC time zone based on the field's date string and time zone.
    $tz = new DateTimeZone(isset($field['timezone']) ? $field['timezone'] : 'UTC');

    // $fields may end up having two values; one for the start date
    // and one for the end date.
    foreach ($values as $value) {
      if ($date = date_create($value['value'], $tz)) {
        $index_value = apachesolr_date_iso($date->format('U'));
        $fields[] = array(
          'key' => $index_key,
          'value' => $index_value,
        );
      }

      if (isset($value['value2'])) {
        if ($date = date_create($value['value2'], $tz)) {
          $index_value = apachesolr_date_iso($date->format('U'));
          $fields[] = array(
            // The value2 element is the end date. Therefore it gets indexed
            // into its own Solr field.
            'key' => $index_key . '_end',
            'value' => $index_value,
          );
        }
      }
    }
  }
  return $fields;
}

/**
 * This function is used during indexing to normalize the DATESTAMP fields
 * into the appropriate format for Apache Solr.
 *
 * @param object $entity
 * @param string $field_name
 * @param string $index_key
 * @param array $field_info
 * @return array $fields
 */
function apachesolr_datestamp_default_indexing_callback($entity, $field_name, $index_key, array $field_info) {
  $fields = array();
  if (!empty($entity->{$field_name})) {
    // $fields may end up having two values; one for the start date
    // and one for the end date.
    $field = $entity->$field_name;
    $values = current($field);

    foreach ($values as $value) {
      if (isset($value['value']) && $value['value'] != 0) {
        $index_value = apachesolr_date_iso($value['value']);
        $fields[] = array(
          'key' => $index_key,
          'value' => $index_value,
        );
      }
      if (isset($value['value2']) && $value['value'] != 0) {
        $index_value = apachesolr_date_iso($value['value2']);
        $fields[] = array(
          // The value2 element is the end date. Therefore it gets indexed
          // into its own Solr field.
          'key' => $index_key . '_end',
          'value' => $index_value,
        );
      }
    }
  }
  return $fields;
}

function apachesolr_floatval($value) {
  return sprintf('%0.20f', $value);
}

/**
 *  Indexing callback for the node_reference module
 *  by the references module
 *
 * @param object $entity
 * @param string $field_name
 * @param string $index_key
 * @param array $field_info
 * @return array $fields
 */
function apachesolr_nodereference_indexing_callback($entity, $field_name, $index_key, array $field_info) {
  $fields = array();
  // Druapl 7 core sets all fields to use LANGUAGE_NONE even if the entity
  // (e.g. node) is flagged as being in a specific language.
  if (!empty($entity->{$field_name}) && isset($entity->{$field_name}[LANGUAGE_NONE])) {
    $index_key = apachesolr_index_key($field_info);
    foreach ($entity->{$field_name}[LANGUAGE_NONE] as $reference) {
      if ($index_value = (!empty($reference['nid'])) ? $reference['nid'] : FALSE) {
        $fields[] = array(
          'key' => $index_key,
          'value' => $index_value,
        );
      }
    }
  }
  return $fields;
}

/**
 *  Indexing callback for the user_reference module
 *  by the references module
 *
 * @param object $entity
 * @param string $field_name
 * @param string $index_key
 * @param array $field_info
 * @return array $fields
 */
function apachesolr_userreference_indexing_callback($entity, $field_name, $index_key, array $field_info) {
  $fields = array();
  // Druapl 7 core sets all fields to use LANGUAGE_NONE even if the entity
  // (e.g. node) is flagged as being in a specific language.
  if (!empty($entity->{$field_name}) && isset($entity->{$field_name}[LANGUAGE_NONE])) {
    $index_key = apachesolr_index_key($field_info);
    foreach ($entity->{$field_name}[LANGUAGE_NONE] as $reference) {
      if ($index_value = (isset($reference['uid']) && strlen($reference['uid'])) ? $reference['uid'] : FALSE) {
        $fields[] = array(
          'key' => $index_key,
          'value' => $index_value,
        );
      }
    }
  }
  return $fields;
}

/**
 * Indexing callback for entityreference fields.
 *
 * @param object $entity
 * @param string $field_name
 * @param string $index_key
 * @param array $field_info
 * @return array $fields
 *
 */
function apachesolr_entityreference_indexing_callback($entity, $field_name, $index_key, $field_info) {
  $fields = array();
  if (!empty($entity->{$field_name}) && array_key_exists(LANGUAGE_NONE, $entity->$field_name)) {

    // Gets entity type and index key. We need to prefix the ID with the entity
    // type so we know what entity we are dealing with in the mapping callback.
    $entity_type = $field_info['field']['settings']['target_type'];
    $index_key = apachesolr_index_key($field_info);

    // Iterates over all references and adds them to the fields.
    foreach ($entity->{$field_name}[LANGUAGE_NONE] as $reference) {
      if ($id = (!empty($reference['target_id'])) ? $reference['target_id'] : FALSE) {
        $fields[] = array(
          'key' => $index_key,
          'value' => $entity_type . ':' . $id,
        );
      }
    }
  }
  return $fields;
}

/**
 * hook_cron() helper to try to make the index table consistent with their
 * respective entity table.
 */
function apachesolr_index_node_check_table() {
  // Check for unpublished content that wasn't deleted from the index.
  $table = apachesolr_get_indexer_table('node');
  // We do not check more nodes than double the cron limit per time
  // Update or delete at most this many in each Solr query.
  $limit = variable_get('apachesolr_cron_mass_limit', 500);
  $query = db_select($table, 'aie')
    ->fields('n', array('nid', 'status'))
    ->where('aie.status <> n.status')
    ->range(0, ($limit * 2))
    ->addTag('apachesolr_index_node');
  $query->innerJoin('node', 'n', 'n.nid = aie.entity_id');
  $nodes = $query->execute()->fetchAllAssoc('nid');

  $node_lists = array_chunk($nodes, $limit, TRUE);
  foreach ($node_lists as $nodes) {
    watchdog('Apache Solr', 'On cron running apachesolr_nodeapi_mass_update() on nids @nids', array('@nids' => implode(',', array_keys($nodes))), WATCHDOG_NOTICE);
    if (!apachesolr_index_nodeapi_mass_update($nodes, $table)) {
      // Solr query failed - so stop trying.
      break;
    }
  }

  // Check for deleted content that wasn't deleted from the index.
  $query = db_select($table, 'aien')
    ->isNull('n.nid')
    ->range(0, ($limit*2));
  $query->addExpression('aien.entity_id', 'nid');
  $query->leftJoin('node', 'n', 'n.nid = aien.entity_id');
  $nodes = $query->execute()->fetchAllAssoc('nid');
  $node_lists = array_chunk($nodes, $limit, TRUE);

  foreach ($node_lists as $nodes) {
    watchdog('Apache Solr', 'On cron running apachesolr_nodeapi_mass_delete() on nids @nids', array('@nids' => implode(',', array_keys($nodes))), WATCHDOG_NOTICE);
    if (!apachesolr_index_nodeapi_mass_delete($nodes, $table)) {
      // Solr query failed - so stop trying.
      break;
    }
  }
}

/**
 * Mass Update nodes from the solr indexer table
 * TODO: This always assumes the default environment!
 *
 * @param array $nodes
 * @param string $table
 * @return boolean
 *   true if we mass updated, false if failed
 *
 */
function apachesolr_index_nodeapi_mass_update(array $nodes, $table = NULL) {
  if (empty($nodes)) {
    return TRUE;
  }
  if (empty($table)) {
    $table = apachesolr_get_indexer_table('node');
  }

  $env_id = apachesolr_default_environment();
  if (apachesolr_index_env_is_readonly($env_id)) {
    apachesolr_index_report_readonly($env_id);
    return FALSE;
  }

  $published_ids = array();
  $unpublished_ids = array();
  foreach ($nodes as $node) {
    if ($node->status) {
      $published_ids[$node->nid] = apachesolr_document_id($node->nid);
    }
    else {
      $unpublished_ids[$node->nid] = apachesolr_document_id($node->nid);
    }
  }
  try {
    $solr = apachesolr_get_solr($env_id);
    $solr->deleteByMultipleIds($unpublished_ids);
    apachesolr_set_last_index_updated($env_id, REQUEST_TIME);

      // There was no exception, so update the table.
    if ($published_ids) {
      db_update($table)
        ->fields(array('changed' => REQUEST_TIME, 'status' => 1))
        ->condition('entity_id', array_keys($published_ids), 'IN')
        ->execute();
    }
    if ($unpublished_ids) {
      db_update($table)
        ->fields(array('changed' => REQUEST_TIME, 'status' => 0))
        ->condition('entity_id', array_keys($unpublished_ids), 'IN')
        ->execute();
    }
    return TRUE;
  }
  catch (Exception $e) {
    apachesolr_log_exception($env_id, $e);
    return FALSE;
  }
}

/**
 * Mass delete nodes from the solr indexer tables.
 * TODO: This always assumes the default environment!
 *
 * @param array $nodes
 * @param string $table
 * @return boolean
 *   true if we mass updated, false if failed
 */
function apachesolr_index_nodeapi_mass_delete(array $nodes, $table = NULL) {
  if (empty($nodes)) {
    return TRUE;
  }
  if (empty($table)) {
    $table = apachesolr_get_indexer_table('node');
  }

  $env_id = apachesolr_default_environment();
  if (apachesolr_index_env_is_readonly($env_id)) {
    apachesolr_index_report_readonly($env_id);
    return FALSE;
  }

  $ids = array();
  $nids = array();
  foreach ($nodes as $node) {
    $ids[] = apachesolr_document_id($node->nid);
    $nids[] = $node->nid;
  }
  try {
    $solr = apachesolr_get_solr($env_id);
    $solr->deleteByMultipleIds($ids);
    apachesolr_set_last_index_updated($env_id, REQUEST_TIME);
    // There was no exception, so update the table.
    db_delete($table)
      ->condition('entity_id', $nids, 'IN')
      ->execute();
    return TRUE;
  }
  catch (Exception $e) {
    apachesolr_log_exception($env_id, $e);
    return FALSE;
  }
}

/**
 * Determine if an environment is in read-only mode.
 *
 * @param string $env_id
 *   An apachesolr environment id.
 *
 * @return bool
 *   TRUE if the environment is read-only.
 */
function apachesolr_index_env_is_readonly($env_id) {
  return (apachesolr_environment_variable_get($env_id, 'apachesolr_read_only', APACHESOLR_READ_WRITE) == APACHESOLR_READ_ONLY);
}

/**
 * Report attempts to change the index when an env is read-only.
 *
 * A flag can be set to disable these watchdog warnings by setting the system
 * variable 'apachesolr_report_readonly_to_watchdog' to FALSE.
 *
 * @param string $env_id
 *   An apachesolr environment id.
 */
function apachesolr_index_report_readonly($env_id) {
  $backtrace = debug_backtrace();
  if (variable_get('apachesolr_report_readonly_to_watchdog', TRUE)) {
    watchdog('Apache Solr', 'Environment @env_id: Trying to update the Solr index while the environment @env_id is read-only in function %function', array(
      '%function' => $backtrace[1]['function'],
      '@env_id' => $env_id
    ), WATCHDOG_WARNING);
  }
}
