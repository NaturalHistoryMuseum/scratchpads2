<?php
/**
 *   biblio.module for Drupal
 *
 *   Copyright (C) 2006-2011  Ron Jerome
 *
 *   This program is free software; you can redistribute it and/or modify
 *   it under the terms of the GNU General Public License as published by
 *   the Free Software Foundation; either version 2 of the License, or
 *   (at your option) any later version.
 *
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details.
 *
 *   You should have received a copy of the GNU General Public License along
 *   with this program; if not, write to the Free Software Foundation, Inc.,
 *   51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 *
 */

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
 * @param $node
 * @return unknown_type
 */
function biblio_load_keywords($vid) {
  $vids = (isset($vid) ? array($vid) : array());
  return biblio_load_keywords_multiple($vids);
}

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
  if (empty($node->biblio_keywords)) return;
  $kw_vocab = variable_get('biblio_keyword_vocabulary', 0);
  $freetagging = variable_get('biblio_keyword_freetagging', 0);
  $taxo_terms = $typed_keywords = array();
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
  $vocabularies = module_invoke('taxonomy', 'get_vocabularies', 'biblio');
  if (variable_get('biblio_keyword_freetagging', 0)) {
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

  if ($freetagging && $freetag_vocab ) {
    $tids = array();
    $ft_field = 'field_' . $freetag_vocab->machine_name;
    $lang = isset($freetag_vocab->language) ? $freetag_vocab->language : 'und';
    if (isset($node->{$ft_field}[$lang])) {
      foreach($node->{$ft_field}[$lang] as $tag) {
        $tids[] = $tag['tid'];
      }
    }
    foreach ($typed_keywords as $kw) {
      if ($possibilities = taxonomy_term_load_multiple(array(), array('name' => trim($kw), 'vid' => $freetag_vocab->vid))) {
        $term = array_pop($possibilities);
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

function biblio_delete_orphan_keywords($force = FALSE) {
  if (variable_get('biblio_keyword_orphan_autoclean', 0) || $force) {
      $query = db_select('biblio_keyword', 'bk');
    $active_kids = $query
      ->fields('bk', array('kid'))
      ->groupBy('kid')
      ->execute()
      ->fetchCol();

    $query = db_select('biblio_keyword_data', 'bkd');
    $all_kids = $query
      ->fields('bkd', array('kid'))
      ->groupBy('kid')
      ->execute()
      ->fetchCol();

    $orphans = array_diff($all_kids, $active_kids);

    if (!empty($orphans)) {
      db_delete('biblio_keyword_data')
        ->condition('kid', $orphans, 'IN')
        ->execute();
    }
  }
  return;
}
/**
 * Delete all keywords references from a given node, but the actual keywords remain in the biblio_keyword_data table
 * Also remove orphan keywords if this option is enabled.  Orphan keywords
 * are keywords which remain in the biblio_keyword_data table but are not refferenced
 * by any nodes through the biblio_keyword table.
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
 * Delete multiple keywords from both the biblio_keyword and biblio_keyword_data tables
 * This will remove the keywords referenced by the supplied ID's from ALL nodes which reference them.
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