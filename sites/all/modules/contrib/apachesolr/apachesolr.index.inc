<?php

/**
 * @file
 *   Functions used when indexing content to Apache Solr.
 */


/**
 * Send up to $limit entities of each type into the index.
 */
function apachesolr_index_entities($env_id, $limit) {
  $entities_processed = 0;
  foreach (entity_get_info() as $entity_type => $info) {
    // With each pass through the callback, retrieve the next group of nids.
    $rows = apachesolr_index_get_entities_to_index($env_id, $entity_type, $limit);
    $documents = array();
    foreach ($rows as $row) {
      $documents = array_merge($documents, apachesolr_index_entity_to_documents($row, $env_id));
    }

    // When there are no documents to send we do not want to execute the
    // indexing function either
    if (!empty($documents)) {
      $indexed = apachesolr_index_send_to_solr($env_id, $documents);
      if ($indexed !== FALSE && $indexed > 0) {
        $entities_processed += count($rows);
        $last_row = end($rows);
        apachesolr_set_last_index_position($env_id, $entity_type, $last_row->changed, $last_row->entity_id);
        apachesolr_set_last_index_updated($env_id, REQUEST_TIME);
      }
    }
  }
  return $entities_processed;
}

/**
 * Helper function for modules implementing hook_search's 'status' op.
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
    $query = db_select($table, 'asn')->condition('asn.status', 1)->condition('asn.bundle', $bundles);
    $total += $query->countQuery()->execute()->fetchField();

    // Get $last_entity_id and $last_change.
    extract(apachesolr_get_last_index_position($env_id, $entity_type));
    // Find the next batch of entities to index for this entity type.  Note that
    // for ordering we're grabbing the oldest first and then ordering by ID so
    // that we get a definitive order.
    $query = db_select($table, 'aie')
      ->condition('aie.status', 1)
      ->condition('aie.bundle', $bundles)
      ->condition(db_or()
        ->condition('aie.changed', $last_changed, '>')
        ->condition(db_and()
          ->condition('aie.changed', $last_changed, '=')
          ->condition('aie.entity_id', $last_entity_id, '>')))
      ->orderBy('aie.entity_id', 'ASC');


    if ($table == 'apachesolr_index_entities') {
      // Other, entity-specific tables don't need this condition.
      $query->condition('aie.entity_type', $entity_type);
    }
    $remaining += $query->countQuery()->execute()->fetchField();
  }
  return array('remaining' => $remaining, 'total' => $total);
}

/**
 * Worker callback for apachesolr_index_entities.
 *
 * @see apachesolr_index_nodes() for the old-skool version.
 * @return array of documents that need to be sent to the index of solr
 */
function apachesolr_index_entity_to_documents($item, $env_id) {

  // Always build the content for the index as an anonynmous user to avoid
  // exposing restricted fields and such.
  // @todo Uncomment these lines when we're done debugging, since they break dpm().
  global $user;
  drupal_save_session(FALSE);
  $saved_user = $user;
  // Should indexing take place using anon ( default )
  // or as another user
  $uid = variable_get('apachesolr_index_user', 0);
  if($uid == 0) {
    $user = drupal_anonymous_user();
  }
  else {
    $user = user_load($uid);
  }
  // Pull out all of our pertinent data.
  $entity_type = $item->entity_type;

  // TRUE on reset to bypass static caching and not blow out our memory limit.
  $entity = entity_load($entity_type, array($item->entity_id), array(), TRUE);
  $entity = $entity ? reset($entity) : FALSE;

  if (empty($entity)) {
    // If the object failed to load, just stop.
    return FALSE;
  }

  list($entity_id, $vid, $bundle) = entity_extract_ids($entity_type, $entity);

  // Create a new document, and do the bare minimum on it.
  $document = _apachesolr_index_process_entity_get_document($entity, $entity_type);

  //Get the callback array to add stuff to the document
  $callbacks = apachesolr_entity_get_callback($entity_type, 'document callback');
  $documents = array();
  foreach ($callbacks as $callback) {
    // Call a type-specific callback to add stuff to the document.
    $documents = array_merge($documents, $callback($document, $entity, $entity_type, $env_id));
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

    // Add additional indexing based on the body of each record.
    apachesolr_index_add_tags_to_document($document, $document->content);
  }

  // Now allow modules to alter each other's additions for maximum flexibility.
  drupal_alter('apachesolr_index_documents', $documents, $entity, $entity_type, $env_id);

  // Restore the user.
  $user = $saved_user;
  drupal_save_session(TRUE);

  return $documents;
}

/**
 * Index an array of documents to solr.
 *
 * @return number indexed, or FALSE on failure.
 */
function apachesolr_index_send_to_solr($env_id, $documents) {
  try {
    // Get the $solr object
    $solr = apachesolr_get_solr($env_id);
    // If there is no server available, don't continue.
    if (!$solr->ping(variable_get('apachesolr_ping_timeout', 4))) {
      throw new Exception(t('No Solr instance available during indexing.'));
    }
  }
  catch (Exception $e) {
    watchdog('Apache Solr', nl2br(check_plain($e->getMessage())), NULL, WATCHDOG_ERROR);
    return FALSE;
  }
  // Do not index when we do not have any documents to send
  if (empty($documents)) {
    return FALSE;
  }

  // Send the document off to Solr.
  watchdog('Apache Solr', 'Adding @count documents.', array('@count' => count($documents)));
  try {
    $docs_chunk = array_chunk($documents, 20);
    foreach ($docs_chunk as $docs) {
      $solr->addDocuments($docs);
    }
    watchdog('Apache Solr', 'Indexing succeeded on @count documents', array(
      '@count' => count($documents),
    ), WATCHDOG_INFO);
    return count($documents);
  }
  catch (Exception $e) {
    if (!empty($docs)) {
      foreach ($docs as $doc) {
        $eids[] = $doc->entity_type . '/' . $doc->entity_id;
      }
    }
    watchdog('Apache Solr', 'Indexing failed on one of the following entity ids: @eids <br /> !message', array(
      '@eids' => implode(', ', $eids),
      '!message' => nl2br(strip_tags($e->getMessage())),
    ), WATCHDOG_ERROR);
    return FALSE;
  }
}

/**
 * Extract HTML tag contents from $text and add to boost fields.
 *
 * $text must be stripped of control characters before hand.
 */
function apachesolr_index_add_tags_to_document(&$document, $text) {
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

  // Strip off all ignored tags.
  $text = strip_tags($text, '<' . implode('><', array_keys($tags_to_index)) . '>');

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
 * @param stdClass $entity
 *   The entity for which we want a document.
 * @param string $entity_type
 *   The type of entity we're processing.
 * @return ApacheSolrDocument
 */
function _apachesolr_index_process_entity_get_document($entity, $entity_type) {
  list($entity_id, $vid, $bundle) = entity_extract_ids($entity_type, $entity);

  $document = new ApacheSolrDocument();

  $document->id = apachesolr_document_id($entity_id, $entity_type);
  $document->site = url(NULL, array('absolute' => TRUE));
  $document->hash = apachesolr_site_hash();

  $document->entity_id = $entity_id;
  $document->entity_type = $entity_type;
  $document->bundle = $bundle;
  $document->bundle_name = entity_bundle_label($entity_type, $bundle);

  $path = entity_uri($entity_type, $entity);
  // A path is not a requirement of an entity
  if (!empty($path)) {
    $document->path = $path['path'];
    $document->url = url($path['path'], $path['options'] + array('absolute' => TRUE));
  }
  if (empty($entity->language)) {
    // 'und' is the language-neutral code in Drupal 7.
    $document->language = LANGUAGE_NONE;
  }
  else {
    $document->language = $entity->language;
  }

  // Path aliases can have important information about the content.
  // Add them to the index as well.
  if (function_exists('drupal_get_path_alias')) {
    // Add any path alias to the index, looking first for language specific
    // aliases but using language neutral aliases otherwise.
    $output = drupal_get_path_alias($document->path, $document->language);
    if ($output && $output != $document->path) {
      $document->path_alias = $output;
    }
  }
  return $document;
}

/**
 * Returns an array of rows from a query based on an indexing environment.
 * @todo Remove the read only because it is not environment specific
 */
function apachesolr_index_get_entities_to_index($env_id, $entity_type, $limit) {
  $rows = array();
  if (variable_get('apachesolr_read_only', 0)) {
    return $rows;
  }
  $bundles = apachesolr_get_index_bundles($env_id, $entity_type);
  if (empty($bundles)) {
    return $rows;
  }

  $table = apachesolr_get_indexer_table($entity_type);
  // Get $last_entity_id and $last_change.
  extract(apachesolr_get_last_index_position($env_id, $entity_type));
  // Find the next batch of entities to index for this entity type.  Note that
  // for ordering we're grabbing the oldest first and then ordering by ID so
  // that we get a definitive order.
  $query = db_select($table, 'aie')
    ->fields('aie', array('entity_type', 'entity_id', 'changed'))
    ->condition('aie.status', 1)
    ->condition('aie.bundle', $bundles)
    ->condition(db_or()
      ->condition('aie.changed', $last_changed, '>')
      ->condition(db_and()
        ->condition('aie.changed', $last_changed, '=')
        ->condition('aie.entity_id', $last_entity_id, '>')))
    ->orderBy('aie.entity_id', 'ASC');

  if ($table == 'apachesolr_index_entities') {
    // Other, entity-specific tables don't need this condition.
    $query->condition('aie.entity_type', $entity_type);
  }
  $query->range(0, $limit);
  $records = $query->execute();

  foreach ($records as $record) {
    $rows[] = $record;
  }
  return $rows;
}

/**
 * Delete an index from an environment and/or a specific entity type
 * @param String $env_id
 * @param String $type
 */
function apachesolr_index_delete_index($env_id) {
  // Instantiate a new Solr object.
  try {
    $solr = apachesolr_get_solr($env_id);
    $query = '*:*';

    // Allow other modules to modify the delete query.
    // For example, use the site hash so that you only delete this site's
    // content:  $query = 'hash:' . apachesolr_site_hash()
    drupal_alter('apachesolr_delete_index', $query);
    $solr->deleteByQuery($query);
    $solr->commit();
    apachesolr_clear_last_index_position($env_id);
    apachesolr_set_last_index_updated($env_id, REQUEST_TIME);
  }
  catch (Exception $e) {
    watchdog('Apache Solr', nl2br(check_plain($e->getMessage())), NULL, WATCHDOG_ERROR);
  }
}

/**
 * Delete an entity from the indexer.
 */
function apachesolr_index_delete_entity_from_index($env_id, $entity_type, $entity) {
  static $failed = FALSE;
  if ($failed) {
    return FALSE;
  }
  try {
    list($id) = entity_extract_ids($entity_type, $entity);
    $solr = apachesolr_get_solr($env_id);
    $solr->deleteById(apachesolr_document_id($id, $entity_type));
    apachesolr_set_last_index_updated($env_id, REQUEST_TIME);
    return TRUE;
  }
  catch (Exception $e) {
    watchdog('Apache Solr', nl2br(check_plain($e->getMessage())), NULL, WATCHDOG_ERROR);
    // Don't keep trying queries if they are failing.
    $failed = TRUE;
    return FALSE;
  }
}

/**
 *
 * @param type $type
 * @return type
 * @todo Add Type support
 */
function apachesolr_index_mark_for_reindex($env_id, $entity_type = NULL) {
  foreach (entity_get_info() as $type => $entity_info) {
    if (($type == $entity_type) || ($entity_type == NULL)) {
      if ($entity_info['apachesolr']['indexable']) {
        $bundles = apachesolr_get_index_bundles($env_id, $type);
        $reindex_callback = '';
        if (!empty($bundles)) {
          $reindex_callback = apachesolr_entity_get_callback($type, 'reindex callback');
        }
        if (!empty($reindex_callback)) {
          if (!call_user_func($reindex_callback, $env_id)) {
            drupal_set_message(t('There was an error reindexing @entity_type.  Please consult the log for more information.', array('@entity_type' => $entity_info['label'])), 'error');
            return;
          }
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
 *   The Solr core for which to index entities.
 * @param string $entity_type
 *   The entity type to index.
 * @param array $bundles
 *   The machine names of the bundles to index.
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
    watchdog_exception('Apache Solr', $e);
  }
}

// This really should be in core, but it isn't yet.  When it gets added to core,
// we can remove this version.
// @see http://drupal.org/node/969180
if (!function_exists('entity_bundle_label')) {

/**
 * Returns the label of a bundle.
 *
 * @param $entity_type
 *   The entity type; e.g. 'node' or 'user'.
 * @param $entity
 *   The entity for which we want the human-readable label of its bundle.
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


/************************
 * The NODE entity indexing part
 ************************/

/**
 * Builds the node-specific information for a Solr document.
 *
 * @param ApacheSolrDocument $document
 *   The Solr document we are building up.
 * @param stdClass $entity
 *   The entity we are indexing.
 * @param string $entity_type
 *   The type of entity we're dealing with.
 */
function apachesolr_index_node_solr_document(ApacheSolrDocument $document, $node, $entity_type, $env_id) {
  // None of these get added unless they are explicitly in our schema.xml
  $document->label = apachesolr_clean_text($node->title);

  // Build the node body.
  $build = node_view($node, 'search_index');
  // Why do we need this?
  unset($build['#theme']);
  $text = drupal_render($build);
  $document->content = apachesolr_clean_text($text);
  if (isset($node->teaser)) {
    $document->teaser = apachesolr_clean_text($node->teaser);
  }
  else {
    $document->teaser = truncate_utf8($document->content, 300, TRUE);
  }
  // Path aliases can have important information about the content.
  // Add them to the index as well.
  if (function_exists('drupal_get_path_alias')) {
    // Add any path alias to the index, looking first for language specific
    // aliases but using language neutral aliases otherwise.
    $language = empty($node->language) ? NULL : $node->language;
    $path = 'node/' . $node->nid;
    $output = drupal_get_path_alias($path, $language);
    if ($output && $output != $path) {
      $document->path_alias = $output;
    }
  }

  $document->ss_name = $node->name;
  // We want the name to be searchable for keywords.
  $document->tos_name = $node->name;

  // Index formatted username so it can be searched and sorted on.
  $account = (object) array('uid' => $node->uid, 'name' => $node->name);
  $username = format_username($account);
  $document->ss_name_formatted = $username;
  $document->tos_name_formatted = $username;

  // Everything else uses dynamic fields
  $document->is_uid = $node->uid;
  $document->bs_status = $node->status;
  $document->bs_sticky = $node->sticky;
  $document->bs_promote = $node->promote;
  $document->is_tnid = $node->tnid;
  $document->bs_translate = $node->translate;
  if (empty($node->language)) {
    // 'und' is the language-neutral code in Drupal 7.
    $document->ss_language = LANGUAGE_NONE;
  }
  else {
    $document->ss_language = $node->language;
  }
  $document->ds_created = apachesolr_date_iso($node->created);
  $document->ds_changed = apachesolr_date_iso($node->changed);
  if (isset($node->last_comment_timestamp) && !empty($node->comment_count)) {
    $document->ds_last_comment_timestamp = apachesolr_date_iso($node->last_comment_timestamp);
    $document->ds_last_comment_or_change = apachesolr_date_iso(max($node->last_comment_timestamp, $node->changed));
  }
  else {
    $document->ds_last_comment_or_change = apachesolr_date_iso($node->changed);
  }
  $document->is_comment_count = isset($node->comment_count) ? $node->comment_count : 0;

  // Fetch extra data normally not visible, including comments.
  // We do this manually (with module_implements instead of node_invoke_nodeapi)
  // because we want a keyed array to come back. Only in this way can we decide
  // whether to index comments or not.
  $extra = array();
  $exclude_comments = in_array($node->type, variable_get('apachesolr_exclude_comments_types', array()), TRUE);

  if (!empty($extra)) {
    // Use an omit-norms text field since this is generally going to be short; not
    // really a full-text field.
    $document->tos_content_extra = apachesolr_clean_text(implode(' ', $extra));
  }

  $document->type_name = node_type_get_name($node);
  $document->created = apachesolr_date_iso($node->created);
  $document->changed = apachesolr_date_iso($node->changed);
  $last_change = (isset($node->last_comment_timestamp) && $node->last_comment_timestamp > $node->changed) ? $node->last_comment_timestamp : $node->changed;
  $document->last_comment_or_change = apachesolr_date_iso($last_change);
  $document->comment_count = isset($node->comment_count) ? $node->comment_count : 0;

  // We need to get the real username here, since it needs a full user object.
  // That means we can't do the format_username() call on the display side.
  $document->name = format_username(user_load($node->uid));

  // Fetch extra data normally not visible, including comments.
  // We do this manually (with module_implements instead of node_invoke_nodeapi)
  // because we want a keyed array to come back. Only in this way can we decide
  // whether to index comments or not.
  $extra = array();
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
  if (isset($extra['comment'])) {
    $comments = $extra['comment'];
    unset($extra['comment']);
    $document->ts_comments = apachesolr_clean_text($comments);
    // @todo: do we want to reproduce apachesolr_add_tags_to_document() for comments?
  }
  // Use an omit-norms text field since this is generally going to be short; not
  // really a full-text field.
  $document->tos_content_extra = apachesolr_clean_text(implode(' ', $extra));

  //  Generic usecase for future reference. Callbacks can
  //  allow you to send back multiple documents
  $documents = array();
  $documents[] = $document;
  return $documents;
}

/**
 * Reindexing callback for ApacheSolr, for nodes.
 * @param String $env_id
 * @return boolean
 */
function apachesolr_index_node_solr_reindex($env_id) {
  $indexer_table = apachesolr_get_indexer_table('node');
  $transaction = db_transaction();
  try {
    db_delete($indexer_table)
      ->condition('entity_type', 'node')
      ->execute();
    $select = db_select('node', 'n');
    $select->addExpression("'node'", 'entity_type');
    $select->addField('n', 'nid', 'entity_id');
    $select->addField('n', 'type', 'bundle');
    $select->addField('n', 'status', 'status');
    $select->addExpression(REQUEST_TIME, 'changed');
    $select->condition('n.type', apachesolr_get_index_bundles($env_id, 'node'), 'IN');

    $insert = db_insert($indexer_table)
      ->fields(array('entity_id', 'bundle', 'status', 'entity_type', 'changed'))
      ->from($select)
      ->execute();
  }
  catch (Exception $e) {
    $transaction->rollback();
    //drupal_set_message($e->getMessage(), 'error');
    watchdog_exception('Apache Solr', $e);
    return FALSE;
  }

  return TRUE;
}

/**
 * Status callback for ApacheSolr, for nodes.
 */
function apachesolr_index_node_status_callback($node, $type) {
  return $node->status;
}


/**
 * Callback that converts term_reference field into an array
 */
function apachesolr_term_reference_indexing_callback($node, $field_name, $index_key, $field_info) {
  // Keep ancestors cached
  $ancestors = &drupal_static(__FUNCTION__, array());

  $fields = array();
  $vocab_names = array();
  if (!empty($node->{$field_name}) && function_exists('taxonomy_get_parents_all')) {
    $field = $node->$field_name;
    list($lang, $items) = each($field);
    foreach ($items as $item) {
      // Triple indexing of tids lets us do effecient searches (on tid)
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
        $fields[] = array(
          'key' => $index_key,
          'value' => $ancestor->tid,
        );
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
 */
function apachesolr_fields_default_indexing_callback($entity, $field_name, $index_key, $field_info) {
  $fields = array();
  $numeric = TRUE;
  if (!empty($entity->{$field_name})) {
    $field = $entity->$field_name;
    list($lang, $values) = each($field);
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

      // Only store the first value of the field in a singular index
      if ($numeric && ($i == 0)) {
        $singular_field_info = $field_info;
        $singular_field_info['multiple'] = FALSE;
        $single_key = apachesolr_index_key($singular_field_info);
        $fields[] = array(
          'key' => $single_key,
          'value' => $function($values[$i]['value']),
        );
      }
    }
  }
  return $fields;
}

/**
 * This function is used during indexing to normalize the DATE and DATETIME
 * fields into the appropriate format for Apache Solr.
 */
function apachesolr_date_default_indexing_callback($entity, $field_name, $index_key, $field_info) {
  $fields = array();
  if (!empty($entity->{$field_name})) {
    $field = $entity->$field_name;
    list($lang, $values) = each($field);
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
 */
function apachesolr_datestamp_default_indexing_callback($entity, $field_name, $index_key, $field_info) {
  $fields = array();
  if (!empty($entity->{$field_name})) {
    // $fields may end up having two values; one for the start date
    // and one for the end date.
    $field = $entity->$field_name;
    list($lang, $values) = each($field);

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
 */
function apachesolr_nodereference_indexing_callback($entity, $field_name, $index_key, $field_info) {
  $fields = array();
  if (!empty($entity->{$field_name})) {
    $index_key = apachesolr_index_key($field_info);
    foreach ($entity->$field_name as $field_references) {
      foreach ($field_references as $reference) {
        if ($index_value = (!empty($reference['nid'])) ? $reference['nid'] : FALSE) {
          $fields[] = array(
            'key' => $index_key,
            'value' => $index_value,
          );
        }
      }
    }
  }
  return $fields;
}

/**
 *  Indexing callback for the user_reference module
 *  by the references module
 */
function apachesolr_userreference_indexing_callback($entity, $field_name, $index_key, $field_info) {
  $fields = array();
  if (!empty($entity->$field_name)) {
    $index_key = apachesolr_index_key($field_info);
    foreach ($entity->$field_name as $field_references) {
      foreach ($field_references as $reference) {
        if ($index_value = (isset($reference['uid']) && strlen($reference['uid'])) ? $reference['uid'] : FALSE) {
          $fields[] = array(
            'key' => $index_key,
            'value' => $index_value,
          );
        }
      }
    }
  }
  return $fields;
}

/**
 * Extract HTML tag contents from $text and add to boost fields.
 *
 * $text must be stripped of control characters before hand.
 */
function apachesolr_add_tags_to_document($document, $text) {
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

  // Strip off all ignored tags.
  $text = strip_tags($text, '<' . implode('><', array_keys($tags_to_index)) . '>');

  preg_match_all('@<(' . implode('|', array_keys($tags_to_index)) . ')[^>]*>(.*)</\1>@Ui', $text, $matches);
  foreach ($matches[1] as $key => $tag) {
    $tag = strtolower($tag);
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
 * Additional index utility functions
 */

/**
 * hook_cron() helper to try to make the index table consistent with their respective entity table.
 */
function apachesolr_index_node_check_table() {
  // Check for unpublished content that wasn't deleted from the index.
  $table = apachesolr_get_indexer_table('node');
  $query = db_select($table, 'aien')
    ->fields('n', array('nid', 'status'))
    ->where('aien.status <> n.status');
  $query->innerJoin('node', 'n', 'n.nid = aien.entity_id');
  $nodes = $query->execute()->fetchAllAssoc('nid');

  // Update or delete at most this many in each Solr query.
  $limit = variable_get('apachesolr_cron_mass_limit', 500);
  $node_lists = array_chunk($nodes, $limit, TRUE);

  foreach ($node_lists as $nodes) {
    watchdog('Apache Solr', 'On cron running apachesolr_nodeapi_mass_update() on nids @nids', array('@nids' => implode(',', array_keys($nodes))), WATCHDOG_WARNING);
    if (!apachesolr_index_nodeapi_mass_update($nodes, $table)) {
      // Solr query failed - so stop trying.
      break;
    }
  }
  // Check for deleted content that wasn't deleted from the index.
  $query = db_select($table, 'aien')
    ->fields('aien', array('entity_id'))
    ->isNull('n.nid');
  $query->leftJoin('node', 'n', 'n.nid = aien.entity_id');
  $nodes = $query->execute()->fetchAllAssoc('nid');
  $node_lists = array_chunk($nodes, $limit, TRUE);

  foreach ($node_lists as $nodes) {
    watchdog('Apache Solr', 'On cron running apachesolr_nodeapi_mass_delete() on nids @nids', array('@nids' => implode(',', array_keys($nodes))), WATCHDOG_WARNING);
    if (!apachesolr_index_nodeapi_mass_delete($nodes, $table)) {
      // Solr query failed - so stop trying.
      break;
    }
  }
}

/**
 * Mass Update nodes from the solr indexer table
 *
 * @param type $nodes
 * @param type $table
 * @return type
 */
function apachesolr_index_nodeapi_mass_update($nodes, $table = NULL) {
  if (empty($nodes)) {
    return TRUE;
  }
  if (empty($table)) {
    $table = apachesolr_get_indexer_table('node');
  }

  if (apachesolr_environment_variable_get(apachesolr_default_environment(), 'apachesolr_read_only', APACHESOLR_READ_WRITE) == APACHESOLR_READ_ONLY) {
    return TRUE;
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
    $env_id = apachesolr_default_environment();
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
    watchdog('Apache Solr', nl2br(check_plain($e->getMessage())), NULL, WATCHDOG_ERROR);
    return FALSE;
  }
}

/**
 * Mass delete nodes form the solr indexer tables.
 *
 * @param type $nodes
 * @param type $table
 * @return type
 */
function apachesolr_index_nodeapi_mass_delete($nodes, $table = NULL) {
  if (empty($nodes)) {
    return TRUE;
  }
  if (empty($table)) {
    $table = apachesolr_get_indexer_table('node');
  }

  if (apachesolr_environment_variable_get(apachesolr_default_environment(), 'apachesolr_read_only', APACHESOLR_READ_WRITE) == APACHESOLR_READ_ONLY) {
    return TRUE;
  }

  $ids = array();
  $nids = array();
  foreach ($nodes as $node) {
    $ids[] = apachesolr_document_id($node->nid);
    $nids[] = $node->nid;
  }
  try {
    $env_id = apachesolr_default_environment();
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
    watchdog('Apache Solr', nl2br(check_plain($e->getMessage())), NULL, WATCHDOG_ERROR);
    return FALSE;
  }
}
