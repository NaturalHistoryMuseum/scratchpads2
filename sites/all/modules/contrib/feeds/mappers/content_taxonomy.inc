<?php

/**
 * @file
 * Enable the user of feeds to map taxonomy terms from the feed for the
 * current node to content_taxonomy CCK fields.
 */

/**
 * Implementation of hook_feeds_node_processor_targets_alter().
 *
 * @see FeedsNodeProcessor::getMappingTargets().
 */
function content_taxonomy_feeds_node_processor_targets_alter(&$targets, $content_type) {
  $info = content_types($content_type);
  $fields = array();
  if (isset($info['fields']) && count($info['fields'])) {
    foreach ($info['fields'] as $field_name => $field) {
      if (in_array($field['type'], array('content_taxonomy'))) {
        $name = isset($field['widget']['label']) ? $field['widget']['label'] : $field_name;
        $targets[$field_name] = array(
          'name' => check_plain($name),
          'callback' => 'content_taxonomy_feeds_set_target',
          'description' => t('The CCK @name field of the node (@type).', array('@name' => $name, '@type' => $field['type'])),
        );
      }
    }
  }
}

/**
 * Callback for mapping. Here is where the actual mapping happens.
 *
 * @param $node
 *   Reference to the node object we are working on.
 *
 * @param $vid
 *   The selected content_taxonomy CCK field.
 *
 * @param $terms
 *   Given terms as array. If a string is used, it is converted to an array
 *   using <code>taxonomy_terms_parse_string($terms)->tids</code>.
 *
 * @see taxonomy_terms_parse_string().
 *
 */
function content_taxonomy_feeds_set_target(&$node, $field_name, $terms) {
  static $fields = array();

  $field = content_fields($field_name, $node->type);

  // Parse string for multiple tags (comma separated)
  if (is_string($terms)) {
    $terms = explode(',', $terms);
  }

  // Return if there are no or empty terms.
  if (!is_array($terms) || empty($terms)) {
    return;
  }

  $tags = $field['widget']['type'] == 'content_taxonomy_autocomplete' && $field['widget']['new_terms'];
  $multiple = $field['widget']['multiple'];

  foreach ($terms as $k => $term_name) {
   $term_name = trim($term_name);
    if ($terms_found = content_taxonomy_get_term_by_name_vid($term_name, $field['vid'])) {
      // If any terms are found add them to the field by found tid.
      foreach ($terms_found as $term_found) {
        if (!is_array($node->$field_name)) $node->$field_name = array();
        array_push($node->$field_name, array('value' => $term_found['tid']));
        if ($multiple != 0 && count($node->$field_name) >= $multiple) {
          // If the vocab is not for multiple tags break after the first hit.
          break;
        }
      }
    }
    elseif ($tags) {
      // If the field is configured for free tagging, create a new term
      $edit = array('vid' => $field['vid'], 'name' => $term_name);
      if ($field['widget']['extra_parent']) {
        $edit['parent'] = $field['widget']['extra_parent'];
      }
      taxonomy_save_term($edit);
      if (!is_array($node->$field_name)) {
        $node->$field_name = array();
      }
      array_push($node->$field_name, array('value' => $edit['tid']));
    }
    if ($multiple != 0 && count($node->$field_name) >= $multiple) {
      // If the vocab is not for multiple tags break after a first term has been added.
      break;
    }
  }
}

/**
 * Try to map a string to an existing term by name and vocabulary id.
 *
 * Provides a case-insensitive and trimmed mapping, to maximize the
 * likelihood of a successful match limited by a vocabulary id.
 *
 * @param $name
 *   Name of the term to search for.
 *
 * @param $vid
 *   The vocabulary's ID.
 *
 * @return
 *   An array of matching term objects.
 */
function content_taxonomy_get_term_by_name_vid($name, $vid) {
  $db_result = db_query(db_rewrite_sql("SELECT t.tid, t.name FROM {term_data} t WHERE LOWER(t.name) = LOWER('%s') AND vid=%d", 't', 'tid'), trim($name), $vid);
  $result = array();
  while ($term = db_fetch_array($db_result)) {
    $result[] = $term;
  }
  return $result;
}
