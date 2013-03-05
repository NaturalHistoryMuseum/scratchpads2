<?php
/**
* @file
* Contains all keyword related functions
*/

/**
 * Gets a keyword ID using keyword text as the key.
 *
 * @param $name
 * @return int Keyword ID
 */
function biblio_get_kid_by_name($name) {
  $kw = biblio_get_keyword_by_name($name);
  if ($kw) {
    return $kw->kid;
  }
}
/**
 * @param $name
 * @return array of keywords
 */
function biblio_get_keyword_by_name($name) {
  static $keywords = array();
  if (!$kid = array_search($name, $keywords)) {
    $term = db_query("SELECT kd.kid, kd.word, COUNT(*) as use_count FROM {biblio_keyword_data} kd
    								LEFT JOIN {biblio_keyword} bk on bk.kid = kd.kid
    								WHERE LOWER(kd.word) = LOWER(:name)
    								GROUP BY kd.kid, kd.word", array(':name' => trim($name)))->fetchObject();
    if ($term) {
      $keywords[$term->kid] = $term;
      return $keywords[$term->kid];
    }
    else {
      return FALSE;
    }
  }

  return $keywords[$kid];
}

/**
 * @param $kid
 * @return unknown_type
 */
function biblio_get_keyword_by_id($kid) {
  static $keywords = array();

  if (!isset($keywords[$kid])) {
    $keywords[$kid] = db_query('SELECT * FROM {biblio_keyword_data} WHERE kid = :kid', array(':kid' => $kid))->fetchObject();
  }

  return $keywords[$kid];

}

/**
 * Load keywords for a single node
 *
 * @param $vid
 * @return keywords for a given node->vid
 */
function biblio_load_keywords($vid) {
  $vids = (isset($vid) ? array($vid) : array());
  return biblio_load_keywords_multiple($vids);
}

/**
 * Load keywords for multiple nodes
 *
 * @param array $vids
 * @return multitype:
 */
function biblio_load_keywords_multiple($vids = array()) {
  $keywords = array();
  if (empty($vids)) return $keywords;

  $query = db_select('biblio_keyword', 'bk');
  $query->innerJoin('biblio_keyword_data', 'bkd', 'bk.kid = bkd.kid');
  $query->addField('bk', 'vid');
  $query->fields('bkd', array('kid', 'word'));
  $query->orderby('bk.vid');
  $query->orderby('bkd.word');
  if (count($vids) == 1) {
    $query->condition('bk.vid', $vids[0]);
  }
  else {
    $query->condition('bk.vid', $vids, 'IN');
  }
  $query->addMetaData('base_table', 'biblio_keyword');
  $query->addTag('node_access');
  $result = $query->execute();

  foreach ($result as $keyword) {
    $keywords[$keyword->vid][$keyword->kid] = $keyword->word;
  }

  return $keywords;
}
/**
 * Update the keyword database from the supplied node
 *
 * @param stdClass $node
 * @return
 *   An array of keyword ID's
 */
function biblio_update_keywords($node) {
  $kids = biblio_insert_keywords($node, TRUE);
  return $kids;
}

/**
 * Insert keywords into the database
 *
 * @param $node
 *   A node with keywords attached
 * @param $update
 *   Set to TRUE if you are updating an existing node
 * @return
 *   An array of keyword ID's from this node
 */
function biblio_insert_keywords($node, $update = FALSE) {
  $kw_vocab = variable_get('biblio_keyword_vocabulary', 0);
  $taxo_terms = $typed_keywords = array();
  $freetag_vocab = FALSE;
  if (!is_array($node->biblio_keywords)) {
    $typed_keywords = biblio_explode_keywords($node->biblio_keywords);
  }
  else {
    $typed_keywords = $node->biblio_keywords;
  }

  if ($update) {
        $and = db_and()->condition('nid', $node->nid)
                       ->condition('vid', $node->vid);
         db_delete('biblio_keyword')
            ->condition($and)
            ->execute();
  }

  if (empty($node->biblio_keywords)) return;

  $vocabularies = module_invoke('taxonomy', 'get_vocabularies');
  $vid = variable_get('biblio_keyword_vocabulary', 0);

  if (variable_get('biblio_keyword_freetagging', 0) && $vid) {
    $freetag_vocab = $vocabularies[variable_get('biblio_keyword_vocabulary', 0)];
  }

  if (isset($node->taxonomy) && is_array($node->taxonomy) && variable_get('biblio_copy_taxo_terms_to_keywords', 0)) { //add any taxonomy terms to our keyword list
    foreach ($node->taxonomy as $vid => $term) {
      if ($vid == 'copy_to_biblio' && $term == 0 ) {// don't copy if user overrides the default to copy, just set the $taxo_terms to an empty array and break out of the for loop
        $taxo_terms = array();
        break;
      }
      if (is_array($term) && !empty($term)) {
        foreach ($term as $tid) {
          if ($tid) {
            $term_obj = taxonomy_term_load($tid);
            $taxo_terms[$term_obj->tid] = $term_obj->name;
          }
        }
      }
      elseif ($term) {
        $term_obj = taxonomy_term_load($term);
        $taxo_terms[$term_obj->tid] = $term_obj->name;
      }
    }
  }

  $keywords = array_merge($typed_keywords, $taxo_terms);

  foreach ($keywords as $keyword) {
    $word = (is_object($keyword)) ? trim($keyword->word) : trim($keyword);
    if (!strlen(trim($word))) continue; //skip if we have a blank
    $kid = FALSE;
    // See if the term exists
    if ( ($kw = biblio_get_keyword_by_name($word)) ) {
      $kid = $kw->kid;
    }
    if (!$kid) {
      $kw = array('word' => trim($word));
      $status = biblio_save_keyword($kw);
      $kid = $kw['kid'];
    }
    // Defend against duplicate, differently cased tags
    if (!isset($inserted[$kid])) {
      db_merge('biblio_keyword')
        ->key(array('kid' => $kid, 'vid' => $node->vid))
        ->fields(
          array(
            'kid' => $kid,
            'nid' => $node->nid,
            'vid' => $node->vid
          ))->execute();
      $inserted[$kid] = TRUE;
    }
  }

  // now if we are saving keywords into a taxonomy freetagging vocabulary, then create the tags string and add it to the node object.
  $vocabularies = module_invoke('taxonomy', 'get_vocabularies');
  $vid          = variable_get('biblio_keyword_vocabulary', 0);
  $freetagging  = variable_get('biblio_keyword_freetagging', 0);

  if ($freetagging && $vid) {
    $freetag_vocab = $vocabularies[$vid];
    $term_refs     = biblio_get_term_ref_fields();
    $ft_field      = 'field_' . $freetag_vocab->machine_name . '_ref';
    if (!isset($term_refs[$ft_field])) {
      biblio_create_term_ref($freetag_vocab);
    }
  }


  if ($freetagging && $freetag_vocab ) {
    $tids = array();
    $lang = isset($freetag_vocab->language) ? $freetag_vocab->language : 'und';
    if (isset($node->{$ft_field}[$lang])) {
      foreach ($node->{$ft_field}[$lang] as $tag) {
        $tids[] = $tag['tid'];
      }
    }

    if (is_array($typed_keywords) && !empty($typed_keywords)) {
      $query = new EntityFieldQuery();
      $query
      ->entityCondition('entity_type', 'taxonomy_term')
      ->propertyCondition('vid', $vid)
      ->propertyCondition('name', $typed_keywords);

      $result = $query->execute();

      if (isset($result['taxonomy_term'])) {
        $existing_tids  = array_keys($result['taxonomy_term']);
        $existing_terms = taxonomy_term_load_multiple($existing_tids);
        $existing_words = array();
        foreach ($existing_terms as $term) {
          $existing_words[$term->name] = $term;
        }
      }
    }

    foreach ($typed_keywords as $kw) {
      if (isset($existing_words[$kw])) {
        $term = $existing_words[$kw];
      }
      else {
        $term = new stdClass();
        $term->vid = $freetag_vocab->vid;
        $term->name  = $kw;
        $term->vocabulary_machine_name = $freetag_vocab->machine_name;
        taxonomy_term_save($term);
      }
      if (!in_array($term->tid, $tids)) {
        $node->{$ft_field}[$lang][] = (array)$term;
      }
    }
  }

  return array_keys($inserted);
}
/**
 * @param $word
 * @return
 */
function biblio_save_keyword(&$keyword) {
  if (!empty($keyword['kid']) && $keyword['word']) {
    drupal_write_record('biblio_keyword_data', $keyword, 'kid');
    $status = SAVED_UPDATED;
  }
  else {
    drupal_write_record('biblio_keyword_data', $keyword);
    $status = SAVED_NEW;
  }

  return $status;
}

/**
 * @return multitype:
 */
function biblio_get_orphaned_keyword_ids() {
  $orphans = $active_kids = $all_kids = array();

  $query = db_select('biblio_keyword', 'bk');
  $active_kids = $query
    ->fields('bk', array('kid'))
    ->groupBy('kid')
    ->execute()
    ->fetchCol();

  $query = db_select('biblio_keyword_data', 'bkd');
  $all_kids = $query
    ->fields('bkd', array('kid'))
    ->execute()
    ->fetchCol();

  $orphans = array_diff($all_kids, $active_kids);

  return $orphans;
}

/**
 * Deletes orphaned keywords
 *
 * Deletion only occurs if the Drupal Variable "biblio_keyword_orphan_autoclean"
 * is true OR if the $force argument is TRUE
 *
 * @param  bool $force
 *   Forces deletion to occur even if autoclean is turned off
 * @return int $count
 *   The number of orphans removed
 */
function biblio_delete_orphan_keywords($force = FALSE) {
  $count = 0;

  if (variable_get('biblio_keyword_orphan_autoclean', 0) || $force) {
    $orphans = biblio_get_orphaned_keyword_ids();
    if (!empty($orphans)) {
      $count = db_delete('biblio_keyword_data')
        ->condition('kid', $orphans, 'IN')
        ->execute();
    }
  }
  return $count;
}

/**
 * Delete all keywords references for a given node.
 *
 * The actual keywords remain in the biblio_keyword_data table
 *
 * @param $node
 * @return
 *   The number of links removed
 */
function biblio_delete_keywords($node) {
  $count = db_delete('biblio_keyword')
            ->condition('nid', $node->nid)
            ->execute();
  return $count;
}

/**
 * Delete "node revision to keyword" links from the biblio_keyword table
 *
 * @param $node
 * @return
 *   The number of links removed
 */
function biblio_delete_revision_keywords($node) {
  return db_delete('biblio_keyword')
            ->condition('vid', $node->vid)
            ->execute();
}

/**
 * Delete multiple keywords.
 *
 * Removes from both the biblio_keyword and biblio_keyword_data tables
 * This will remove the keywords referenced by the supplied ID's from
 * ALL nodes which reference them.
 *
 * @param array $keywords
 *   An array of keyword id's to delete
 * @return
 *   The number of keywords deleted
 */
function biblio_delete_multiple_keywords($keywords) {
  $count = 0;
  foreach ($keywords as $kid) {
    $count += biblio_delete_keyword($kid);
  }
  return $count;
}
/**
 * Delete a keyword from both the biblio_keyword and biblio_keyword_data tables
 * This will remove the keyword referenced by the supplied ID from ALL nodes which reference them.
 *
 * @param $keyword_id
 *   The keyword id to delete
 * @return
 *   The number of keywords deleted (should always be one)
 */
function biblio_delete_keyword($keyword_id) {
  db_delete('biblio_keyword')
      ->condition('kid', $keyword_id)
      ->execute();

  return db_delete('biblio_keyword_data')
            ->condition('kid', $keyword_id)
            ->execute();
}

/**
 * @param unknown_type $string
 * @return multitype:string
 */
function biblio_explode_keywords($string) {
  $sep = variable_get('biblio_keyword_sep', ',');
  $regexp = '%(?:^|' . $sep . '\ *)("(?>[^"]*)(?>""[^"]* )*"|(?: [^"' . $sep . ']*))%x';
  preg_match_all($regexp, $string, $matches);
  $keyword_array = array_unique($matches[1]);

  $keywords = array();
  foreach ($keyword_array as $keyword) {
    // If a user has escaped a term (to demonstrate that it is a group,
    // or includes a comma or quote character), we remove the escape
    // formatting so to save the term into the database as the user intends.
    $keyword = trim(str_replace('""', '"', preg_replace('/^"(.*)"$/', '\1', $keyword)));
    if ($keyword != "") {
      $keywords[] = $keyword;
    }
  }
  return $keywords;
}
/**
 * @param unknown_type $keywords
 * @param unknown_type $sep
 * @return Ambigous <string, unknown>
 */
function biblio_implode_keywords($keywords, $sep = '') {

  if (empty($sep)) $sep = variable_get('biblio_keyword_sep', ',');
  $string = '';
  foreach ($keywords as $kid => $keyword) {
    $string .= strlen($string)?"$sep ":'';
    if (strpos($keyword, $sep) !== FALSE) {
      $string .= '"' . $keyword . '"';
    }
    else {
      $string .= $keyword;
    }
  }
  return $string;
}

function biblio_get_term_ref_fields() {
  $termfields = array();
  foreach (field_info_instances('node', 'biblio') as $instance) {
    $field = field_info_field_by_id($instance['field_id']);
    if ($field['type'] == 'taxonomy_term_reference') {
      $termfields[$field['field_name']] = $instance['label'];
    }
  }
  return $termfields;
}

function biblio_create_term_ref($vocabulary) {
  $field_name = 'field_' . $vocabulary->machine_name . '_ref';

  $field = array(
      'field_name' => $field_name,
      'type' => 'taxonomy_term_reference',
      'cardinality' => FIELD_CARDINALITY_UNLIMITED,
      'settings' => array(
          'allowed_values' => array(
              array(
                  'vocabulary' => $vocabulary->machine_name,
                  'parent' => 0
              ),
          ),
      ),
  );

  field_create_field($field);


  $instance = array(
      'field_name' => $field_name,
      'entity_type' => 'node',
      'label' => $vocabulary->name,
      'bundle' => 'biblio',
      'required' => FALSE,
      'widget' => array(
          'type' => 'options_select',
          'cardinality' => -1
      ),
      'display' => array(
          'default' => array('type' => 'hidden'),
          'teaser' => array('type' => 'hidden')
      )
  );

  field_create_instance($instance);
}