<?php

/**
 * @file
 */

/**
 * @param $aid
 *
 * @return
 */
function biblio_get_linked_contributors($aid) {
  $related = &drupal_static(__FUNCTION__);

  if (!isset($related[$aid])) {
    $aka = db_query('SELECT aka FROM {biblio_contributor_data} WHERE cid = :cid', array(':cid' => $aid))->fetchField();
    if ($aka == 0) {
      $query = db_select('biblio_contributor_data', 'bcd');
      $cids = $query->fields('bcd', array('cid'))
        ->condition(db_and()->condition('bcd.aka', $aid, '=')->condition('bcd.alt_form', 0, '='))
        ->execute()
        ->fetchCol();
    }
    else {
      $query = db_select('biblio_contributor_data', 'bcd');
      $and = db_and()->condition('bcd.aka', $aka, '=')->condition('bcd.alt_form', 0, '=')->condition('bcd.cid', $aid, '<>');
      $cids = $query->fields('bcd', array('cid'))
        ->condition(db_or()->condition('bcd.cid', $aka)->condition('bcd.aka', $aid)->condition($and))
        ->execute()
        ->fetchCol();
    }
    $related[$aid] = $cids;
  }
  return $related[$aid];
}

/**
 *
 */
function biblio_get_contributor($aid) {
  $contributor = &drupal_static(__FUNCTION__);

  if (!isset($contributor[$aid])) {
    $contributor[$aid] = db_query('SELECT * FROM {biblio_contributor_data} WHERE cid = :cid', array(':cid' => $aid))->fetchObject();
  }

  return $contributor[$aid];
}

/**
 * @param array $contributors
 * @param int $category
 *
 * @return array $authors
 */
function biblio_get_contributor_category($contributors, $category) {
  $authors = array();
  foreach ($contributors as $author) {
    if ($author['auth_category'] == $category) {
      $authors[] = $author;
    }
  }
  return $authors;
}

/**
 * Get an author id from the database using the "name" field as a key.
 * If the "cid" value is negative, this means we found an "alternate" form of
 * the name, which should have an "aka" value which points to the preferred form.
 *
 * @param string $name
 *
 * @return int
 */
function biblio_get_cid_by_name($name) {
  $cid = 0;
  $auth = biblio_get_contributor_by_name($name);
  if ($auth) {
    if ($auth->cid > 0) {
      $cid = $auth->cid;
    }
    if ($auth->aka > 0 && $auth->alt_form > 0) {
      $cid = $auth->aka;
    }
  }
  return $cid;
}

/**
 * @param $name
 *
 * @return unknown
 */
function biblio_get_contributor_by_name($name) {
  $contributors = &drupal_static(__FUNCTION__);
  $name = trim($name);
  if (!isset($contributors[$name]) || $contributors[$name] === FALSE) {
    $query = db_select('biblio_contributor_data', 'bcd');
    $contributors[$name] = $query->fields('bcd')->condition('bcd.name', $name)->execute()->fetchObject();
  }

  return $contributors[$name];
}

/**
 * @param $vid
 *
 * @return Ambigous <NULL>
 */
function biblio_get_first_contributor($vid) {
  static $contributor = array();
  if (!isset($contributor[$vid])) {
    $query = db_select('biblio_contributor', 'bc');
    $query->join('biblio_contributor_data', 'bcd', 'bc.cid=bcd.cid');
    $query->fields('bcd');
    $query->condition('bc.vid', $vid);
    $query->condition('bc.rank', 0);
    $contributor[$vid] = $query->execute()->fetchObject();
  }

  return $contributor[$vid];
}

/**
 * @param $vid
 *
 * @return
 */
function biblio_load_contributors($vid) {
  $vids = (isset($vid) ? array($vid) : array());
  return biblio_load_contributors_multiple($vids);
}

/**
 * @param $vids
 * @param $auth_category
 *
 * @return multitype:|Ambigous <multitype:, array>
 */
function biblio_load_contributors_multiple($vids = array(), $auth_category = 0) {
  $contributors = array();
  if (empty($vids)) {
    return $contributors;
  }

  $query = db_select('biblio_contributor', 'bc');
  $query->innerJoin('biblio_contributor_data', 'bcd', 'bcd.cid = bc.cid');
  $query->fields('bc');
  $query->fields('bcd');
  $query->orderby('bc.vid');
  $query->orderby('bc.rank');
  if (count($vids) == 1) {
    $query->condition('bc.vid', $vids[0]);
  }
  else {
    $query->condition('bc.vid', $vids, 'IN');
  }
  if ($auth_category) {
    $query->condition('bc.auth_category', $auth_category);
  }
  $query->addMetaData('base_table', 'biblio_contributor');
  $query->addTag('node_access');
  $result = $query->execute();

  foreach ($result as $creator) {
    $contributors[$creator->vid][] = (array) $creator;
  }

  return $contributors;
}

/**
 * Add separate author named "et al" to the end of the author array.
 *
 * @param $authors
 *   - author array to augment
 * @param $type
 *   - auth_type
 *
 * @return TRUE if author was added, FALSE if "etal" was already there
 */
function biblio_authors_add_etal(&$authors, $type) {
  $etal = "et al"; $max_rank = 0;
  // Et al author should be added only once per type.
  foreach ($authors as $author) {
    if ($author['auth_type'] != $type) {
      continue;
    }
    if ($author['name'] == $etal) {
      return FALSE;
    }
    $max_rank = max($max_rank, $author['rank']);
  }
  $authors[] = biblio_parse_author(array('name' => $etal, 'auth_type' => $type, 'lastname' => $etal, 'rank' => $max_rank + 1));
  return TRUE;
}

/**
 * @param $node
 *
 * @return unknown
 */
function biblio_delete_contributors($node) {
  $count = db_delete('biblio_contributor')
    ->condition('nid', $node->nid)
    ->execute();
  return $count;
}

/**
 * @param $node
 *
 * @return unknown
 */
function biblio_delete_contributors_revision($node) {
  $count = db_delete('biblio_contributor')
    ->condition('vid', $node->vid)
    ->execute();
  return $count;
}

/**
 * @param $cid
 */
function biblio_delete_contributor($cid) {
  db_delete('biblio_contributor')
    ->condition('cid', $cid)
    ->execute();

  return db_delete('biblio_contributor_data')
    ->condition('cid', $cid)
    ->execute();

}

/**
 * @param $cid
 * @param $vid
 */
function biblio_delete_contributor_revision($cid, $vid) {
  return db_delete('biblio_contributor')
    ->condition('cid', $cid)
    ->condition('vid', $vid)
    ->execute();

}

/**
 * Get the number of orphaned authors in the database.
 *
 * @return int
 */
function biblio_count_orphan_authors() {
  $cids = biblio_get_orphan_author_ids();
  return count($cids);
}

/**
 * Get an array of authors which are not associated with any biblio entires.
 *
 * @return array
 */
function biblio_get_orphan_authors() {
  $authors = array();
  $cids = biblio_get_orphan_author_ids();

  if (count($cids)) {
    $query = db_select('biblio_contributor_data', 'bcd');
    $result = $query->fields('bcd')
      ->condition('cid', $cids, 'IN')
      ->orderBy('lastname')
      ->execute();

    foreach ($result as $author) {
      $authors[] = $author;
    }
  }
  return $authors;
}

/**
 * Get an array of author id's which are not associated with any biblio entries.
 *
 * @return array
 */
function biblio_get_orphan_author_ids() {
  $orphans = array();
  $active_cids = array();
  $all_cids = array();

  $query = db_select('biblio_contributor', 'bc');
  $active_cids = $query
    ->fields('bc', array('cid'))
    ->groupBy('cid')
    ->execute()
    ->fetchCol();

  $query = db_select('biblio_contributor_data', 'bcd');
  $all_cids = $query
    ->fields('bcd', array('cid'))
    ->condition(db_and()->condition('bcd.cid', 0, '>')->condition('bcd.alt_form', 0, '='))
    ->execute()
    ->fetchCol();

  $orphans = array_diff($all_cids, $active_cids);

  return $orphans;
}

/**
 * @param $force
 */
function biblio_delete_orphan_authors($force = FALSE) {
  if (variable_get('biblio_auto_orphaned_author_delete', 0) || $force) {
    $orphans = biblio_get_orphan_author_ids();
    if (!empty($orphans)) {
      db_delete('biblio_contributor_data')
        ->condition('cid', $orphans, 'IN')
        ->execute();
    }
  }
}

/**
 * @param $node
 *
 * @return Ambigous <success, boolean>
 */
function biblio_insert_contributors($node) {
  if (!empty($node->biblio_contributors)) {
    return _save_contributors($node->biblio_contributors, $node->nid, $node->vid);
  }
}

/**
 * @param $node
 */
function biblio_update_contributors($node) {
  if (!empty($node->biblio_contributors)) {
    _save_contributors($node->biblio_contributors, $node->nid, $node->vid, TRUE);
  }
  return;

}

/**
 * @param $author
 */
function biblio_save_contributor(&$author) {
  foreach ($author as $key => $value) {
    $author[$key] = trim($value);
  }
  return drupal_write_record('biblio_contributor_data', $author);
}

/**
 * @param $author
 *
 * @return bool
 */
function biblio_update_contributor(&$author) {
  if (!isset($author['cid'])) {
    return FALSE;
  }
  return drupal_write_record('biblio_contributor_data', $author, 'cid');
}

/**
 * @param $authors
 */
function _biblio_contributor_sort(&$authors) {
  foreach ($authors as $key => $author) {
    if (!isset($author['rank']) || empty($author['rank'])) {
      $authors[$key]['rank'] = $key;
    }
  }
  usort($authors, '_biblio_contributor_usort');
}

/**
 * @param $a
 * @param $b
 *
 * @return int
 */
function _biblio_contributor_usort($a, $b) {
  if (empty($a['name'])) {
    return 1;
  }
  if (empty($b['name'])) {
    return -1;
  }
  return ($a['rank'] < $b['rank']) ? -1 : 1;
}

/**
 * Save contributors to the database.
 *
 * @param $authors
 * @param $nid
 * @param $vid
 * @param $update
 *
 * @return success of database operations
 */
function _save_contributors(&$contributors, $nid, $vid, $update = FALSE) {
  $rank = 0;

  db_delete('biblio_contributor')
    ->condition(db_and()->condition('nid', $nid)->condition('vid', $vid))
    ->execute();

  // re-sort the authors by rank because the rank may have changed due to tabledrag on the input form.
  _biblio_contributor_sort($contributors);

  $name_parser = new HumanNameParser_Parser();

  foreach ($contributors as $key => $author) {
    if (!empty($author['name'])) {
      if (isset($author['cid']) && !empty($author['cid'])) {
        // Check to make sure the name wasn't changed
        // this should only happen via the node/add/biblio input form.
        $auth = biblio_get_contributor($author['cid']);
        if (!empty($auth) && isset($auth->name) && $auth->name != $author['name']) {
          // If the name has changed, NULL the cid so a new entry is created.
          $author['cid'] = NULL;
        }
        else {
          $contributors[$key] = array_merge($author, (array) $auth);
        }
      }
      // If we don't have a cid, lets see if we can find and exact match
      // to the name and use that cid.
      if (!isset($author['cid']) || empty($author['cid'])) {
        $auth = biblio_get_contributor_by_name($author['name']);
        if (!empty($auth) && isset($auth->cid)) {
          $author['cid'] = $auth->cid;
          $contributors[$key] = array_merge($author, (array) $auth);
        }
      }

      // If we still don't have a cid, then create a new entry in the biblio_contirbutor_data table.
      if (empty($author['cid'])) {
        try {
          $author = $name_parser->parseName($author);
        }
        catch (Exception $e) {
          $link = l('in node ' . $nid, 'node/' . $nid);
          $message = $e->getMessage() . ' ' . $link;
          drupal_set_message($message, 'error');
          watchdog('biblio', $message, array(), WATCHDOG_ERROR);
        }

        $contributors[$key] = $author;
        biblio_save_contributor($author);
      }

      // We should now have a cid, if not we are in big trouble...
      if (empty($author['cid'])) {
        // Throw error that author was not saved.
      }

      $link_array = array(
        'nid' => $nid,
        'vid' => $vid,
        'cid' => $author['cid'],
        'rank' => $rank++,
        'auth_type' => !empty($author['auth_type']) ? $author['auth_type'] : $author['auth_category'],
        'auth_category' => $author['auth_category'],
      );

      if (!drupal_write_record('biblio_contributor', $link_array)) {
        return FALSE;
      }
    }
  }
  // @todo check if it is necessary to reset aka here...
  // @code
  //  db_query("UPDATE {biblio_contributor_data} SET aka = cid WHERE aka = 0 OR aka IS NULL");
  //  db_update('biblio_contributor_data')
  //    ->fields(array('aka', )
  // @endcode
  // Successfully saved all contributors.
  return TRUE;
}

/**
 * Released through http://bibliophile.sourceforge.net under the GPL licence.
 * Do whatever you like with this -- some credit to the author(s) would be appreciated.
 *
 * A collection of PHP classes to manipulate bibtex files.
 *
 * If you make improvements, please consider contacting the administrators at bibliophile.sourceforge.net so that your improvements can be added to the release package.
 *
 * Mark Grimshaw 2004/2005
 * http://bibliophile.sourceforge.net
 *
 * 28/04/2005 - Mark Grimshaw.
 * Efficiency improvements.
 *
 * 11/02/2006 - Daniel Reidsma.
 * Changes to preg_matching to account for Latex characters in names such as {\"{o}}
 *
 * For a quick command-line test (php -f PARSECREATORS.php) after installation, uncomment these lines:
 * @code
 * $authors = "Mark \~N. Grimshaw and Bush III, G.W. & M. C. H{\\'a}mmer Jr. and von Frankenstein, Ferdinand Cecil, P.H. & Charles Louis Xavier Joseph de la Vallee P{\\\"{o}}ussin";
 * $creator = new PARSECREATORS();
 * $creatorArray = $creator->parse($authors);
 * print_r($creatorArray);
 * @endcode
 *
 * Create writer arrays from bibtex input.
 * 'author field can be (delimiters between authors are 'and' or '&'):
 * 1. <first-tokens> <von-tokens> <last-tokens>
 * 2. <von-tokens> <last-tokens>, <first-tokens>
 * 3. <von-tokens> <last-tokens>, <jr-tokens>, <first-tokens>
 */

/**
 * @param $author_array
 *
 * @return
 */
function biblio_parse_author($author_array, $cat = 0) {

  if (isset($author_array['auth_category']) && $author_array['auth_category'] == 5) {
    $author_array['firstname'] = '';
    $author_array['initials'] = '';
    $author_array['lastname'] = trim($author_array['name']);
    $author_array['prefix'] = '';
    $author_array['literal'] = 1;
  }
  else {
    $value = trim($author_array['name']);
    $appellation = $prefix = $surname = $firstname = $initials = '';
    $prefix = "";
    // Replace multiple white space by single space.
    $value = preg_replace("/\s{2,}/", ' ', $value);
    $author = explode(",", $value);
    $size = count($author);
    // No commas therefore something like Mark Grimshaw, Mark Nicholas Grimshaw, M N Grimshaw, Mark N. Grimshaw.
    if ($size == 1) {
      // Is complete surname enclosed in {...}, unless the string starts with a backslash (\) because then it is
      // probably a special latex-sign..
      // 2006.02.11 DR: in the last case, any NESTED curly braces should also be taken into account! so second
      // clause rules out things such as author="a{\"{o}}"
      // .
      if (preg_match("/(.*) {([^\\\].*)}/", $value, $matches) && !(preg_match("/(.*) {\\\.{.*}.*}/", $value, $matches2))) {
        $author = explode(" ", $matches[1]);
        $surname = $matches[2];
      }
      else {
        $author = explode(" ", $value);
        // Last of array is surname (no prefix if entered correctly)
        $surname = array_pop($author);
      }
    }
    // Something like Grimshaw, Mark or Grimshaw, Mark Nicholas  or Grimshaw, M N or Grimshaw, Mark N.
    elseif ($size == 2) {
      // First of array is surname (perhaps with prefix)
      list ($surname, $prefix) = _grabSurname(array_shift($author));
    }
    // If $size is 3, we're looking at something like Bush, Jr. III, George W.
    else {
      // Middle of array is 'Jr.', 'IV' etc.
      $appellation = implode(' ', array_splice($author, 1, 1));
      // First of array is surname (perhaps with prefix)
      list ($surname, $prefix) = _grabSurname(array_shift($author));
    }
    $remainder = implode(" ", $author);
    list ($firstname, $initials, $prefix2) = _grabFirstnameInitials($remainder);
    if (!empty($prefix2)) {
      $prefix .= $prefix2;
    }
    // var_dump($prefix);
    // $surname = $surname . ' ' . $appellation;.
    $author_array['firstname'] = trim($firstname);
    $author_array['initials'] = (strlen(trim($initials)) > 10) ? drupal_substr(trim($initials), 0, 10) : trim($initials);
    $author_array['lastname'] = trim($surname);
    $author_array['prefix'] = trim($prefix);
    $author_array['suffix'] = trim($appellation);
  }
  $author_array['md5'] = _md5sum($author_array);
  return $author_array;
}

/**
 * @param $creator
 *
 * @return
 */
function _md5sum($creator) {
  $string = $creator['firstname'] . $creator['initials'] . $creator['prefix'] . $creator['lastname'];
  $string = str_replace(' ', '', drupal_strtolower($string));
  return md5($string);
}

/**
 * Grab firstname and initials which may be of form "A.B.C." or "A. B. C. " or " A B C " etc.
 *
 * @param $remainder
 *
 * @return
 */
function _grabFirstnameInitials($remainder) {
  $prefix = array();
  $firstname = $initials = '';
  $array = explode(" ", $remainder);
  foreach ($array as $value) {
    $first_char = drupal_substr($value, 0, 1);
    if ((ord($first_char) >= 97) && (ord($first_char) <= 122)) {
      $prefix[] = $value;
    }
    elseif (preg_match("/[a-zA-Z]{2,}/", trim($value))) {
      $firstname_array[] = trim($value);
    }
    else {
      $initials_array[] = trim(str_replace(".", " ", trim($value)));
    }
  }
  if (isset($initials_array)) {
    $initials = implode(" ", $initials_array);
  }
  if (isset($firstname_array)) {
    $firstname = implode(" ", $firstname_array);
  }
  if (!empty($prefix)) {
    $prefix = implode(" ", $prefix);
  }
  return array($firstname, $initials, $prefix);
}

/**
 * Surname may have title such as 'den', 'von', 'de la' etc. - characterised by first character lowercased.  Any
 * uppercased part means lowercased parts following are part of the surname (e.g. Van den Bussche)
 *
 * @param $input
 *
 * @return
 */
function _grabSurname($input) {
  $no_prefix = FALSE;
  $surname = FALSE;
  $prefix = FALSE;

  $surname_array = explode(" ", $input);

  foreach ($surname_array as $value) {
    $first_char = substr($value, 0, 1);
    if (!$no_prefix && (ord($first_char) >= 97) && (ord($first_char) <= 122)) {
      $prefix[] = $value;
    }
    else {
      $surname[] = $value;
      $no_prefix = TRUE;
    }
  }
  if (!empty($surname)) {
    $surname = implode(" ", $surname);
  }
  if (!empty($prefix)) {
    $prefix = implode(" ", $prefix);
  }
  return array($surname, $prefix);
}

/**
 * @return
 */
function _loadMD5() {
  static $md5   = array();
  static $count = 0;
  $db_count     = db_query("SELECT COUNT(*) FROM {biblio_contributor_data}")->fetchField();
  // Refresh the cached data as some new authors may have been added or removed.
  if ($db_count != $count) {
    $count = $db_count;
    $md5 = array();
    $result = db_query('SELECT md5,cid  FROM {biblio_contributor_data} ');
    foreach ($result as $row) {
      $md5[$row->cid] = $row->md5;
    }
  }
  return (count($md5)) ? $md5 : NULL;
}
