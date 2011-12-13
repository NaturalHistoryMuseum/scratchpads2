<?php

/**
 * @file
 *   Functions used when indexing content to Apache Solr.
 */

/**
 * Given a node, return a document representing that node.
 */
function apachesolr_node_to_document($node, $namespace) {
  $document = FALSE;
  // Let any module exclude this node from the index.
  $build_document = TRUE;
  foreach (module_implements('apachesolr_node_exclude') as $module) {
    $exclude = module_invoke($module, 'apachesolr_node_exclude', $node, $namespace);
    if (!empty($exclude)) {
      $build_document = FALSE;
    }
  }

  if ($build_document) {
    $document = new ApacheSolrDocument();
    $document->id = apachesolr_document_id($node->nid);
    $document->site = url(NULL, array('absolute' => TRUE));
    $document->hash = apachesolr_site_hash();
    $document->entity_type = 'node';
    $document->entity_id = $node->nid;
    $document->bundle = $node->type;
    $document->bundle_name = node_type_get_name($node);
    $document->label = apachesolr_clean_text($node->title);
    $path = 'node/' . $node->nid;
    $document->url = url($path, array('absolute' => TRUE));
    $document->path = $path;
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
      $output = drupal_get_path_alias($path, $language);
      if ($output && $output != $path) {
        $document->path_alias = $output;
      }
    }

    $document->ss_name = $node->name;
    // We want the name to ale be searchable for keywords.
    $document->tos_name = $node->name;

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

    // Handle fields including taxonomy.
    $indexed_fields = apachesolr_entity_fields('node');
    foreach ($indexed_fields as $index_key => $field_info) {
      $field_name = $field_info['field']['field_name'];
      // See if the node has fields that can be indexed
      if (isset($node->{$field_name})) {
        // Got a field.
        $function = $field_info['indexing_callback'];
        if ($function && is_callable($function)) {
          // NOTE: This function should always return an array.  One
          // node field may be indexed to multiple Solr fields.
          $fields = $function($node, $field_name, $index_key, $field_info);
          foreach ($fields as $field) {
            // It's fine to use this method also for single value fields.
            $document->setMultiValue($field['key'], $field['value']);
          }
        }
      }
    }

    // Index book module data.
    if (!empty($node->book['bid'])) {
      // Hard-coded - must change if apachesolr_index_key() changes.
      $document->is_book_bid = (int) $node->book['bid'];
    }
    apachesolr_add_tags_to_document($document, $text);

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
    if (isset($extra['comment'])) {
      $comments = $extra['comment'];
      unset($extra['comment']);
      $document->ts_comments = apachesolr_clean_text($comments);
      // @todo: do we want to reproduce apachesolr_add_tags_to_document() for comments?
    }
    // Use an omit-norms text field since this is generally going to be short; not
    // really a full-text field.
    $document->tos_content_extra = apachesolr_clean_text(implode(' ', $extra));

    // Let modules add to the document.
    foreach (module_implements('apachesolr_update_index') as $module) {
      $function = $module . '_apachesolr_update_index';
      $function($document, $node, $namespace);
    }
  }
  return $document;
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
 */
function apachesolr_fields_default_indexing_callback($entity, $field_name, $index_key, $field_info) {
  $fields = array();
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
        $function = 'apachesolr_clean_text';
    }
    foreach ($values as $fval) {
      $fields[] = array(
        'key' => $index_key,
        'value' => $function($fval['value']),
      );
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
 * hook_cron() helper to try to make {apachesolr_search_node} consistent with {node}.
 */
function apachesolr_cron_check_node_table() {
  // Check for unpublished content that wasn't deleted from the index.
  $query = db_select('apachesolr_search_node', 'asn')
    ->fields('n', array('nid', 'status'))
    ->where('asn.status <> n.status');
  $query->innerJoin('node', 'n', 'n.nid = asn.nid');
  $nodes = $query->execute()->fetchAllAssoc('nid');

  // Update or delete at most this many in each Solr query.
  $limit = variable_get('apachesolr_cron_mass_limit', 500);
  $node_lists = array_chunk($nodes, $limit, TRUE);

  foreach ($node_lists as $nodes) {
    watchdog('Apache Solr', 'On cron running apachesolr_nodeapi_mass_update() on nids @nids', array('@nids' => implode(',', array_keys($nodes))), WATCHDOG_WARNING);
    if (!apachesolr_nodeapi_mass_update($nodes)) {
      // Solr query failed - so stop trying.
      break;
    }
  }
  // Check for deleted content that wasn't deleted from the index.
  $query = db_select('apachesolr_search_node', 'asn')
    ->fields('asn', array('nid'))
    ->isNull('n.nid');
  $query->leftJoin('node', 'n', 'n.nid = asn.nid');
  $nodes = $query->execute()->fetchAllAssoc('nid');
  $node_lists = array_chunk($nodes, $limit, TRUE);

  foreach ($node_lists as $nodes) {
    watchdog('Apache Solr', 'On cron running apachesolr_nodeapi_mass_delete() on nids @nids', array('@nids' => implode(',', array_keys($nodes))), WATCHDOG_WARNING);
    if (!apachesolr_nodeapi_mass_delete($nodes)) {
      // Solr query failed - so stop trying.
      break;
    }
  }
}

function apachesolr_nodeapi_mass_update($nodes) {
  if (empty($nodes)) {
    return TRUE;
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
    $solr = apachesolr_get_solr();
    $solr->deleteByMultipleIds($unpublished_ids);
    apachesolr_index_set_last_updated(REQUEST_TIME);

      // There was no exception, so update the table.
    if ($published_ids) {
      db_update('apachesolr_search_node')->fields(array('changed' => REQUEST_TIME, 'status' => 1))->condition('nid', array_keys($published_ids), 'IN')->execute();
    }
    if ($unpublished_ids) {
      db_update('apachesolr_search_node')->fields(array('changed' => REQUEST_TIME, 'status' => 0))->condition('nid', array_keys($unpublished_ids), 'IN')->execute();
    }
    return TRUE;
  }
  catch (Exception $e) {
    watchdog('Apache Solr', nl2br(check_plain($e->getMessage())), NULL, WATCHDOG_ERROR);
    return FALSE;
  }
}

function apachesolr_nodeapi_mass_delete($nodes) {
  if (empty($nodes)) {
    return TRUE;
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
    $solr = apachesolr_get_solr();
    $solr->deleteByMultipleIds($ids);
    apachesolr_index_set_last_updated(REQUEST_TIME);
    // There was no exception, so update the table.
    db_delete('apachesolr_search_node')->condition('nid', $nids, 'IN')->execute();
    return TRUE;
  }
  catch (Exception $e) {
    watchdog('Apache Solr', nl2br(check_plain($e->getMessage())), NULL, WATCHDOG_ERROR);
    return FALSE;
  }
}
