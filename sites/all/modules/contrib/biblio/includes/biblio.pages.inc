<?php
/**
 *
 *   Copyright (C) 2006-2008  Ron Jerome
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
 ****************************************************************************/


function biblio_view_node($nid) {
  drupal_goto('node/' . (int)$nid, NULL, NULL, 301); // set a 301 response code
}
/**
 * @return unknown_type
 */
function biblio_db_search($arg_list = array()) {
  $nids = array();
  $saved_filters = array();

  $uri = drupal_parse_url(request_uri());
  $arg_list = array_merge($arg_list, $uri['query']);
  if(isset($_SESSION['biblio_filter']) && !empty($_SESSION['biblio_filter'])) {
    $saved_filters = &$_SESSION['biblio_filter'];
  }
  foreach ($saved_filters as $filter) {
    $type = key($filter);
    $value = $filter[$type];
    $arg_list['f'][$type] = $value;
  }

  // Drupal search? It returns an array of search results. We store the nids
  // of the result nodes and make them a "where n.nid in {..}" filter.
  // After installing the filter, we can go on as usual.

  // When called manually, search takes one parameter, i.e.
  // biblio_db_search('search', 'bla blu')
  $search = array_keys($arg_list, 'search');
  if (!empty($search)) {
    $keys = $arg_list[$search+1];

    // Special case: if search is activated via URL, i.e., biblio/search/...,
    // we reset the search session filter. Two searches are not combinable.
    $base =  variable_get('biblio_base', 'biblio');
    if (preg_match('+' . $base . '/search/+', $_GET['q'])) {
      $_SESSION['biblio_filter'] = array();
    }
  }
  else {
    $keys = _get_biblio_search_filter();
  }

/*  if ($keys) {
    // Only search in biblio nodes. If we use a SESSION filter and have a list of
    // nids stored there, don't re-search. List is reset when submitting a new search.
    if ($search !== FALSE || !_get_biblio_search_filter('nodelist')) {
      if ($result = biblio_build_search_query($keys)) {
        $node_list = '';
        while ($nid = db_result($result)) {
          $node_list .= $nid. ",";
        }
        // No node search result. Make sure we find nothing, too. Node -1 does not exist.
        if (empty($node_list)) $node_list = '-1';

        // Store as SESSION filter or argument list.
        // When called as function argument it takes only one parameter, i.e.
        // biblio_db_search('search', 'bla blu') and we must insert the node
        // list inbetween.
        if ($search !== FALSE) {
          array_splice($arg_list, $search + 1, 0, $node_list);
        }
        else {
          $_SESSION['biblio_filter'] = array(array('search', rtrim($node_list, ','), $keys));
        }
      }
      // Wrong query (too short, only negative words etc.). Warning has been issued.
      else $_SESSION['biblio_filter'] = array();
    }
  }
*/
  $inline = in_array('inline', $arg_list);
  $inline = in_array('profile', $arg_list)?'profile':$inline;

  $_SESSION['last_biblio_query'] = $arg_list;

  $arg_list['page_limit'] = variable_get('biblio_rowsperpage', 25);

  $query_info = biblio_build_query($arg_list);

  if ($query_info['rss']['feed']) {
    biblio_filter_feed($query_info['query'], $query_info['query_terms'], $query_info['rss']);
  }
  else{
    //$count = db_result(db_query($query_info['count_query'],$query_info['query_terms']));
    $nodes = array();
    //$result = pager_query($query_info['query'], variable_get('biblio_rowsperpage', 25),0,$query_info['count_query'],$query_info['query_terms']);
    $query_info['filter_line'] = _biblio_filter_info_line($query_info['args']);

    foreach ($query_info['query'] as $node) {
      $nids[] = $node->nid;
      if (isset($node->biblio_year)) unset($node->biblio_year);
      $extras[$node->nid] = $node;
    }
    if (count($nids)) {
      $nodes = node_load_multiple($nids);

      foreach ($nodes as $nid => $node) {
        $nodes[$nid] = (object)array_merge((array)$node, (array)$extras[$nid]);
      }
    }
  }

  return biblio_show_results($nodes, $query_info, $inline);
}


/*
 * biblio_db_search builds the SQL query which will be used to
 * select and order "biblio" type nodes.  The query results are
 * then passed to biblio_show_results for output
 *
 *
 */
function biblio_build_query($arg_list) {
  global $user, $db_type;
  static $bcc = 0; //biblio_contributor (bc) count , increase for every invocation
  static $bkd = 0;
  static $tcc = 0; //term counter, increase for every invocation
  $inline = $rss_info['feed'] = FALSE;
  $joins = array();
  $args = array();
  $selects = array();
  $count_selects = array();
  $count_limit = '';


  if ($arg_list['page_limit'] > 0) {
    $query = db_select('node', 'n')->extend('PagerDefault');
    $query->limit($arg_list['page_limit']);
  }
  else {
    $query = db_select('node', 'n');
  }

  //add a tag of "node_access" to ensure that only nodes to which the user has access are retrieved
  $query->addTag('node_access');

  $query->addField('n', 'nid');
  $type_name = $query->addField('bt', 'name', 'biblio_type_name');
  $query->leftJoin('biblio', 'b', 'n.vid=b.vid');
  $query->innerJoin('biblio_types', 'bt', 'b.biblio_type=bt.tid');
  $query->distinct();

  // POSIX regular expression matching, case insensitive
  $match_op = ($db_type == 'pgsql') ? '~*' : 'RLIKE';
   // The following is to be used to compare titles
   // LOWER() is required for case insensitive sorting (at least, in PostgreSQL)
  $sort_title =" CASE
                    WHEN SUBSTR(n.title,1,1)='\"'   THEN LOWER(SUBSTR(n.title,2))
                    WHEN SUBSTR(n.title,1,1)='\''   THEN LOWER(SUBSTR(n.title,2))
                    WHEN SUBSTR(n.title,1,2)='A '   THEN LOWER(SUBSTR(n.title,3))
                    WHEN SUBSTR(n.title,1,3)='An '  THEN LOWER(SUBSTR(n.title,4))
                    WHEN SUBSTR(n.title,1,4)='The ' THEN LOWER(SUBSTR(n.title,5))
                    ELSE LOWER(n.title)
                 END ";

  $limit = '';
  if (variable_get('biblio_view_only_own', 0) ) {
    $limit .= " AND n.uid = $user->uid ";
  }

  if (!isset($arg_list['s'])) {
    $arg_list['s'] = variable_get('biblio_sort', 'year');
  }
  if (!isset($arg_list['o'])) {
    $arg_list['o'] = strtolower(variable_get('biblio_order', 'desc'));
  }

  if (!isset($_SESSION['biblio_filter']) || !is_array($_SESSION['biblio_filter'])) {
    $_SESSION['biblio_filter'] = array();
  }

  $session = &$_SESSION['biblio_filter'];

  if (!in_array('no_filters', $arg_list)) {
    foreach ($session as $filter) {
      $arg_list = array_merge($arg_list, $filter);
    }
  }
  switch ($arg_list['s']) {
    case 'type':
      //$sortby = "ORDER BY bt.name %s, b.biblio_year DESC ";
      $query->addField('n', 'title');
      $query->orderBy($type_name, $arg_list['o']);
      $query->orderBy($sort_title, $arg_list['o']);
            break;
    case 'title':
      $query->addField('n', 'title');
      $query->orderBy($sort_title, $arg_list['o']);

      break;
    case 'author':
      //$last_name = $query->addField('bcd', 'lastname');
      $query->innerJoin('biblio_contributor', 'bc', 'b.vid = bc.vid');
      $query->join('biblio_contributor_data', 'bcd', 'bc.cid = bcd.cid');
      $query->condition('bc.rank', 0);
      $query->addField('bcd', 'lastname');
      $query->orderBy('bcd.lastname', $arg_list['o']);
      // $query->condition('bc.auth_category', 1);
      break;
    case 'keyword': // added msh 070808
      $word = $query->addField('bkd', 'word', 'biblio_keyword');
      $query->orderBy($word, $arg_list['o']);
      $query->innerJoin('biblio_keyword', 'bk', 'b.vid = bk.vid');
      $query->innerJoin('biblio_keyword_data', 'bkd', 'bk.kid = bkd.kid');
      break;
    case 'year':
    default:
      $query->addField('b', 'biblio_year');
      $query->addField('b', 'biblio_date');
      $query->orderBy('biblio_year', $arg_list['o']);
      $query->orderBy('biblio_date', $arg_list['o']);
  } //end switch


  if (isset($arg_list['f']) && count($arg_list['f']) ) {
    $args = array();
    foreach ($arg_list['f'] as $type => $value) {
      switch ($type) {
        case 'no_filters':
          break;
        case 'and':
          $operator = " AND ";
          break;
        case 'or':
          $operator = " OR ";
          break;
        case 'inline':
          $inline = TRUE;
          break;
        case 'rss.xml':
          $rss_info['feed'] = TRUE;
          $count_limit = 'LIMIT ' . variable_get('biblio_rss_number_of_entries', 10);
          break;
        case 'profile':
          $inline = "profile";
          break;
        case 'term':
        case 'term_id':
          $query->innerJoin('taxonomy_index', "ti$tcc", "n.nid = ti$tcc.nid");
          if ($type == 'term') {
            $query->innerJoin('taxonomy_term_data', 'td', "ti$tcc.tid = td.tid");
            $query->condition('td.name', $value);
          }
          elseif ($type == 'term_id') {
            $query->condition("ti$tcc.tid", $value);
          }
          $tcc++;
          break;
        case 'tg':
          $query->where("substring(($sort_title),1 ,1) " . $match_op . " LOWER(:letter)", array(':letter' => $value));
          break;
        case 'ag': //selects entries whoose authors firstname starts with the letter provided
          $query->where(" UPPER(substring(bcd.lastname,1,1)) = :letter ", array(':letter' => $value));
          //$where['bc-rank'] = "bc.rank=0";
          $query->innerJoin('biblio_contributor', 'bc', 'b.vid = bc.vid');
          $query->innerJoin('biblio_contributor_data', 'bcd', 'bc.cid = bcd.cid');
          break;
        case 'cid':
        case 'aid':
          $bcc++;
          $query->innerJoin('biblio_contributor', "bc$bcc", "n.vid = bc$bcc.vid");
          $query->condition("bc$bcc.cid", $value);
          break;
        case 'author':
          $bcc++;
          if (array_search('bc', array_keys($query->getTables())) === FALSE) {
            $query->innerJoin('biblio_contributor', 'bc', 'n.vid = bc.vid');
          }
          if (is_numeric($value)) {
            $cids = db_query('SELECT cid FROM {biblio_contributor_data}
                              WHERE cid = :cid OR
                              (aka = (SELECT aka FROM {biblio_contributor_data} WHERE cid = :cdid) AND aka != 0)',
                              array(':cid' => $value, ':cdid' => $value));

            $cid_count = 0;
            $or = db_or();

            foreach ($cids as $cid) {
              $or->condition("bc.cid", $cid->cid);
              $cid_count++;
            }

            if ($cid_count == 0) {
              $query->condition("bc.cid", -1);
            }
            else {
              $query->condition($or);
            }

          }
          else {
            if (array_search('bcd', array_keys($query->getTables())) === FALSE) {
              $query->innerJoin('biblio_contributor_data', 'bcd', 'bcd.cid = bc.cid');
            }
            $query->condition('bcd.name', "[[:<:]]" . $value . "[[:>:]]", $match_op);
            $rss_info['title'] = t("Publications by @value", array('@value' => $value));
            $rss_info['description'] = t("These publications by %author are part of the works listed at %sitename", array('%author' => $value, '%sitename' => variable_get('site_name', 'Drupal')));
            $rss_info['link'] = '/author/' . $value;
          }
          break;
        case 'publisher':
          $query->condition('b.biblio_publisher', "[[:<:]]" . $value . "[[:>:]]", $match_op);
          break;
        case 'year':
          $query->condition('b.biblio_year', $value);
          break;
        case 'uid':
          $query->addField('n', 'uid');
          $query->condition('n.uid', $value);
          break;
        case 'keyword':
          $bkd++;
          if (array_search('bk', array_keys($query->getTables())) === FALSE) {
            $query->innerJoin('biblio_keyword', 'bk', 'n.vid = bk.vid');
          }
          if (is_numeric($value)) {
            $query->condition('bk.kid', $value);
          }
          else{
            if (array_search('bkd', array_keys($query->getTables())) === FALSE) {
              $query->innerJoin('biblio_keyword_data', 'bkd', 'bkd.kid = bk.kid');
            }
            if (strlen($value) == 1) {
              $query->condition('',  $value, 'SUBSTR(bkd.word, 1, 1) =');
            }
            else {
              $query->condition('bkd.word', "[[:<:]]" . $value . "[:>:]]", 'LIKE');
            }
            $rss_info['title'] = t("Keyword @value", array('@value' => $value));
            $rss_info['description'] = t("These publications, containing the keyword: %keyword, are part of the works listed at %sitename", array('%keyword' => $value, '%sitename' => variable_get('site_name', 'Drupal')));
            $rss_info['link'] = '/keyword/' . $value;
          }
          break;
        case 'citekey':
          $query->condition('b.biblio_citekey', $value);
          break;
        case 'type':
          $query->condition('b.biblio_type', $value);
          break;
        case 'search':
          $term = explode("?", array_shift($arg_list));
          $result_nids = split(',', $term[0]);
          $where[] = "n.nid in (" . db_placeholders($result_nids) . ")";
          foreach ($result_nids as $result_nid) {
            $terms[] = $result_nid;
            array_push($args, $type, $result_nid);
          }
          // Save search keyword to show in the filter list.
          $term = array_shift($arg_list);
          array_push($args, $type, $term);
          $operator = NULL;
          break;
        default:
          $fields = biblio_get_db_fields();
          if (in_array("biblio_$type", $fields)) {
            $query->condition("b.biblio_$type ", $value, 'LIKE');
          }
          break;
      }
    }
  }

  $result = $query->execute();
  return (array('query'      => $result,
               'args'        => $args,
              // 'sort_attrib' => $sort_attrib,
               'rss'         => $rss_info
  ));

}

/**
 * biblio_show_results takes the query results from biblio_db_search and
 * adds some controls to the page then loops through the results applying
 * the selected style to each entry
 *
 * @param $result
 * @param $count
 * @param $attrib
 * @param $args
 * @param $inline
 * @return unknown_type
 */
function biblio_show_results($nodes, $query_info = array(), $inline = FALSE) {
  global $pager_total_items;
  $profile = FALSE;
//  $attrib = $query_info['sort_attrib'];
  $args   = $query_info['args'];
  $base =  variable_get('biblio_base', 'biblio');
  $style = biblio_get_style();
  if ($inline === 'profile') {
    $profile = TRUE;
    $inline  = FALSE;
  }
  if (module_exists('popups')) {
     popups_add_popups();
  }

  if (!$inline && !$profile) {

    if (variable_get('biblio_rss', 0)) {
      drupal_add_html_head_link(array(
        'rel' => 'alternate',
        'type' => 'application/rss+xml',
        'title' => variable_get('site_name', 'Drupal') . ' RSS',
        'href' => url("$base/rss.xml")
      ));
    }
    // Search box. Has same permissions as the filter tab.
    $content = '<div id="biblio-header" class="clear-block">';
    if (variable_get('biblio_search', 0) && user_access('show filter tab')) {
      $content .= drupal_render(drupal_get_form('biblio_search_form'));
    }
    if ( biblio_access('export')) {
      $content .= '<div class="biblio-export">' . theme('biblio_export_links', array('node' => NULL)) . '</div>';
    }
    else{
      $content .= '<div class="biblio-export">' . t('Found @count results', array('@count' => $pager_total_items[0])) . '</div><br />';
    }
    // Add some links to the top of the page to change the sorting/ordering...
    if (user_access('show sort links')) {
      $content .= _biblio_sort_tabs();
    }
    $content .= '</div>';

    if  (user_access('show filter tab')) {
      $content .=  $query_info['filter_line'];
    }
  }

  if ($inline === TRUE) print '<div class="biblio-inline">';

  if (isset($_GET['s'])) {
    if ( $_GET['s'] == 'title' ||
    $_GET['s'] == 'author' ||
    $_GET['s'] == 'keyword') {
      if (strpos($_GET['q'], 'ag') ||
      strpos($_GET['q'], 'tg') ||
      strpos($_GET['q'], 'keyword')) {
        $value = substr($_GET['q'], strrpos($_GET['q'], '/') + 1);
      }
      else {
        $value = '';
      }
      $content .= theme('biblio_alpha_line', array('type' => $_GET['s'], 'current' => $value, 'path' => ''));
    }
  }

  $count = 0;
  // Reset separator bar status for repeated calls to biblio_db_search.
  _biblio_category_separator_bar(NULL, TRUE);

  foreach ($nodes as $node) {
    $count++;
    if (is_array($node)) $node = (object)$node;
    if (variable_get('biblio_hide_bibtex_braces', 0)) $node->title = biblio_remove_brace($node->title);
    if (variable_get('biblio_fix_isi_links', 0)) biblio_fix_isi_links($node);


    // output separator bar if needed
    $content .= _biblio_category_separator_bar($node);

    $inline_links = ($inline && variable_get('biblio_inlinemode_in_links', 0)) ? TRUE : FALSE;

    $content .= theme('biblio_entry', array('node' => $node, 'base' => $base, 'style_name' => $style, 'inline' => $inline_links));

  } //end while
  if ($count) $content .= '</div><!-- end category-section -->';
  $content .= theme('pager', array('quantity' => variable_get('biblio_rowsperpage', 25)));
  if ($count == 0) {
    $content .= "<h3>" . t("No items found") . "</h3>";
    if (strstr($content, "Filters:")) {
      $content .= t('!modify_link or !remove_link your filters and try again.', array('!modify_link' => l(t('Modify'), "$base/filter"), '!remove_link' => l(t('remove'), "$base/filter/clear")));
    }
  }
  if ($profile === TRUE)  return $content;
  if ($inline === TRUE)   return $content . "</div>";
  if ($inline === FALSE)  {
    drupal_set_title(check_plain(variable_get('biblio_base_title', 'Biblio')));
    return $content;
  }

}

function _biblio_sort_tabs() {
  global $base_path;
  $content = '';
  $sort_links = array();
  $tabs = variable_get('biblio_sort_tabs_style', 0);
  $uri = drupal_parse_url(request_uri());
  if (substr($uri['path'], 0, 1) == '/') $uri['path'] = substr($uri['path'], 1);

  if (isset($uri['query']['s'])) {
    $sort = $uri['query']['s'];
  }
  else {
    $sort = variable_get('biblio_sort', 'year');
  }
  if (isset($uri['query']['o'])) {
    $order = ($uri['query']['o'] == 'desc' ||$uri['query']['o'] == 'DESC') ? 'asc' : 'desc';
  }
  else {
    $order = strtolower(variable_get('biblio_order', 'desc'));
    $order = ($order == 'desc' || $order == 'DESC') ? 'asc' : 'desc';
  }

  $path = drupal_get_path('module', 'biblio');
  $order_arrow = ($order == 'asc') ? '<img src ="' . $base_path . $path . '/misc/arrow-asc.png" alt =" (Desc)" />'
                                   : '<img src ="' . $base_path . $path . '/misc/arrow-desc.png" alt = " (Asc)" />';
  $sort_links =  variable_get('biblio_sort_tabs', array('author' => 'author', 'title' => 'title', 'type' => 'type', 'year' => 'year', 'keyword' => 'keyword'));
  ksort($sort_links);
  $content .= $tabs ? '<ul class="tabs secondary ">' : '';

  foreach ($sort_links as $key => $title) {
    $uri['attributes'] = array("title" => t("Click a second time to reverse the sort order"));
    $uri['html'] = TRUE;
    $uri['text'] = t(ucfirst($title));
    if ($key === $title && $title == $sort) {
      $uri['query']['s'] = $title;
      $uri['query']['o'] = $order;
      $uri['attributes']['class'][] = "active";
      $uri['active'] = TRUE;
      $uri['pfx'] = ' [ ';
      $uri['sfx'] = '] ';
      $uri['arrow'] = $order_arrow;
      $content .= _biblio_sort_tab($uri, $tabs);

    }
    elseif ($key === $title ) {
      $uri['query']['s'] = $title;
      $uri['query']['o'] = $order;
      $uri['active'] = FALSE;
      $uri['pfx'] = ' ';
      $uri['sfx'] = ' ';
      $uri['arrow'] = '';
      $content .= _biblio_sort_tab($uri, $tabs);
    }

  }
  if (!$tabs) $content = t('Sort by') . ': ' . $content;
  $content .= $tabs ? '</ul>':'';

  return $content;
}

function _biblio_sort_tab($tab, $tabs = FALSE) {
  if ($tabs) {
    $text  = '<span class="a"><span class="b">' . $tab['text'] . $tab['arrow'] . '</span></span>';
    $class = ($tab['attributes']['class']) ? 'class="active"' : '';
    $link  = l($text, $tab['path'], $tab);
    return "<li $class >" . str_replace('class="active"', $class, $link) . '</li>';
  }
  else {
    return $tab['pfx'] . l($tab['text'], $tab['path'], $tab) . $tab['arrow'] . $tab['sfx'];
  }
  return;
}

function _biblio_filter_info_line($args) {
  $content = '';
  $filtercontent = '';
  $search_content = '';
  $base =  variable_get('biblio_base', 'biblio');
  $session = &$_SESSION['biblio_filter'];
  // if there are any filters in place, print them at the top of the list
  $uri = drupal_parse_url(request_uri());
  $filters = isset($uri['query']['f']) ? $uri['query']['f'] : array();
  if (count($filters)) {
    $i = 0;
    foreach ($filters as $type => $value) {
      if ($type == 'search') {
        $search_content = array_shift($args);
        continue;
      }
      if ($type == 'term_id') {
        $term = taxonomy_term_load($value);
        $value = $term->name;
        $type = t("Taxonomy term");
      }
      if ($type == 'keyword') {
        module_load_include('inc', 'biblio', 'includes/biblio.keywords');
        $type = t("Keyword");
        if (is_numeric($value)) {
          $term = biblio_get_keyword_by_id($value);
          if (isset($term->word)) {
            $value = $term->word;
          }
        }
        elseif (is_string($value) && strlen($value) == 1) {
          $type = t("First letter of keyword");
        }
      }
      if ($type == 'uid' ) {
        $user = user_load($value);
        $value = $user->name;
        $type = t("Drupal user");
      }
      if ($type == 'aid' || ($type == 'author' && is_numeric($value))) {
        module_load_include('inc', 'biblio', 'includes/biblio.contributors');
        $author = biblio_get_contributor($value);
        $value = $author->name;
        $type = t("Author");
      }
      if ($type == 'ag' ) {
        //return;
        $type = t("First letter of last name");
      }
      if ($type == 'tg' ) {
        //return;
        $type = t("First letter of title");
      }
      if ($type == 'type' && $value > 0) {
        if (($pub_type = db_query('SELECT t.* FROM {biblio_types} as t WHERE t.tid=:tid', array(':tid' => $value))->fetchObject())) {
          $value = drupal_ucfirst(_biblio_localize_type($pub_type->tid, $pub_type->name));
          $type = t("Type");
        }
      }
      $params = array('%a' =>  check_plain(ucwords($type)) , '%b' =>  check_plain($value) );
      $filtercontent .= ($i++ ? t('<em> and</em> <strong>%a</strong> is <strong>%b</strong>', $params) : t('<strong>%a</strong> is <strong>%b</strong>', $params)) ;
    }
    if ($search_content) {
      $content .= '<div class="biblio-current-filters"><b>' . t('Search results for') . '</b>';
      $content .= '<em> ' . check_plain($search_content) . '</em>';
      if ($filtercontent) {
        $content .= '<br><b>' . t('Filters') . ': </b>';
      }
    }
    else {
      $content .= '<div class="biblio-current-filters"><b>' . t('Filters') . ': </b>';
    }
    $content .= $filtercontent;

    $link_options = array();
    if (isset($_GET['s'])) {
      $link_options['query']['s'] = $_GET['s'];
    }
    if (isset($_GET['o'])) {
      $link_options['query']['o'] = $_GET['o'];
    }
    unset($uri['query']['f']);
    if ($search_content) {
      $content .= '&nbsp;&nbsp;' . l('[' . t('Reset Search') . ']', "$base/filter/clear", $link_options);
    }
    else {
      $content .= '&nbsp;&nbsp;' . l('[' . t('Clear All Filters') . ']', "$base/filter/clear", $uri);
    }
    $content .= '</div>';
  }

  return $content;
}

function _biblio_category_separator_bar($node, $reset = FALSE) {
  static $_text = '';
  if ($reset) {
    $_text = '';
    return;
  }
  $content = '';

  if (isset($_GET['s'])) {
    $sort = $_GET['s'];
  }
  else {
    $sort = variable_get('biblio_sort', 'year');
  }

  switch ($sort) {
    case 'title':
      $title = $node->title;
      $first = drupal_substr(drupal_ucfirst(ltrim($title)), 0, 1);
      if (drupal_substr(drupal_ucfirst(ltrim($title)), 0, 1) == '"' ) $first = drupal_ucfirst(drupal_substr(ltrim($title), 1, 1));
      if (drupal_substr(drupal_ucfirst(ltrim($title)), 0, 1) == "'" ) $first = drupal_ucfirst(drupal_substr(ltrim($title), 1, 1));
      if (drupal_substr(drupal_ucfirst(ltrim($title)), 0, 2) == "A " ) $first = drupal_ucfirst(drupal_substr(ltrim($title), 2, 1));
      if (drupal_substr(drupal_ucfirst(ltrim($title)), 0, 3) == "An " ) $first = drupal_ucfirst(drupal_substr(ltrim($title), 3, 1));
      if (drupal_substr(drupal_ucfirst(ltrim($title)), 0, 4) == "The " ) $first = drupal_ucfirst(drupal_substr(ltrim($title), 4, 1));
      if ( $first  != $_text) {
        if ($_text != '' ) {
          $content .= theme_biblio_end_category_section();
        }
        $_text =  $first ;
        $content .= theme_biblio_separator_bar($_text);
      }
      break;
    case 'author':
      if ($node->biblio_contributors->sort_letter &&
          $node->biblio_contributors->sort_letter != $_text) {
        if ($_text != '' ) {
          $content .= theme_biblio_end_category_section();
        }
        $_text = $node->biblio_contributors->sort_letter ;
        $content .= theme_biblio_separator_bar($_text);
      }
      break;
    case 'type':
      if ($node->biblio_type_name != $_text) {
        if ($_text != '' ) {
          $content .= theme_biblio_end_category_section();
        }
        $_text = $node->biblio_type_name;
        //      $name = db_result(db_query("SELECT name FROM {biblio_types} as t where t.tid=%d", $node->biblio_type)) ;
        $content .= theme_biblio_separator_bar(_biblio_localize_type($node->biblio_type, $_text));
      }
      break;
    case 'keyword':   // added msh 08 aug 07
      // $kw = array_shift($node->biblio_keyword);
      $tok = $node->biblio_keyword;
      if (empty($tok)) {
        $tok = t("No Keywords");
      }
      if ($tok != $_text) {
        if ($_text != '' ) {
          $content .= theme_biblio_end_category_section();
        }
        $_text = $tok;
        if ($_text != '') {
          $content .= theme_biblio_separator_bar($_text);
        }
      }
      break;
    case 'year':
    default:
      if ($node->biblio_year != $_text) {
        if ($_text != '' ) {
          $content .= theme_biblio_end_category_section();
        }
        $_text = $node->biblio_year;
        $content .= theme_biblio_separator_bar($_text);
      }
  } //end switch
  return $content;
}
function theme_biblio_separator_bar($text) {
  $content = "\n" . '<div class="biblio-separator-bar">' . check_plain($text) . "</div>\n";
  $content .= "\n" . '<div class ="biblio-category-section">';
  return $content;
}

function theme_biblio_end_category_section() {
  return "\n</div><!-- end category-section -->";
}

/**
 * Add a search field on the main biblio page.
 */
/**
 * @param $form_state
 * @return unknown_type
 */
function biblio_search_form($form, &$form_state) {
  $form['biblio_search'] = array(
    '#prefix' => '<div class="container-inline biblio-search clear-block">',
    '#suffix' => '</div>',
    );
  $form['biblio_search']['keys'] = array(
  '#type' => 'textfield',
    '#title' => '',
    '#default_value' => '',
    '#size' => 25,
    '#maxlength' => 255,
  );
  $button_text = variable_get('biblio_search_button_text', 'Biblio search');
  $form['biblio_search']['submit'] = array('#type' => 'submit', '#value' => t($button_text));
  $form['#cache'] = FALSE;
  return $form;
}

/**
 * Build the query following the do_search algorithm in search.module.
 * Unfortunately we cannot reuse anything from do_search as everything
 * is hard-coded :-(
 * @param $keys
 */
function biblio_build_search_query($keys = '') {
  if ($keys != '')  {
    $query = search_parse_query($keys);
    if ($query[2] == '') {
      form_set_error('keys', t('You must include at least one positive keyword with @count characters or more.', array('@count' => variable_get('minimum_word_size', 3))));
      return FALSE;
    }
    if ($query === NULL || $query[0] == '') return FALSE;

    $where = '(' . $query[2] . ')';
    $args = $query[3];
    if (!$query[5]) {
      $where .= " AND ($query[0])";
      $args = array_merge($args, $query[1]);
      $join = " INNER JOIN {search_dataset} d ON i.sid = d.sid AND i.type = d.type";
    }
    // The COUNT ensures that we get only nodes where "term1 AND term2"
    // match as we demand 2 matches. Note that this doesn't work when
    // using the partial word search patch.
    $args[] =$query[4];

    $query = "SELECT distinct(i.sid) FROM {search_index} i
              INNER JOIN {node} n ON n.nid = i.sid
              $join
              WHERE n.status = 1 AND (n.type = 'biblio')
              AND $where
              AND i.type = 'node'
              GROUP BY i.type, i.sid HAVING COUNT(*) >= %d";
    return db_query(db_rewrite_sql($query), $args);
  }
  return FALSE;
}


/**
 * When we submit a search, we revoke all current filters since search
 * and filtering are considered two different concepts things* conceptually.
 *
 * But we store the results as a filter (which is just a list of node ids that
 * matched the search request) so that we can reorder or export the search
 * results like with any other filter.  The filter has three components:
 * ('search',<list of node ids>,<search keywords>).
 * The second component, the filter value, is empty when submitting keywords.
 * In biblio_db_search we fill the second component with the list of nids
 * matching our keywords, as returned by node_search.  We store the keywords
 * only for showing them in "Search results for <keywords>".
 */
/**
 * @param $form
 * @param $form_state
 * @return unknown_type
 */
function biblio_search_form_submit($form, &$form_state) {
  static $keys = '';
}

function biblio_search_query($keys) {
  if (empty($keys)) {
    $keys = $form_state['values']['keys'];


    $query = db_select('search_index', 'i', array('target' => 'slave'))->extend('SearchQuery')->extend('PagerDefault');
    $query->join('node', 'n', 'n.nid = i.sid');
    $query
    ->condition('n.status', 1)
    ->addTag('node_access')
    ->searchExpression($keys, 'node');

    // Insert special keywords.
    $query->setOption('type', 'n.type');
    $query->setOption('language', 'n.language');
    if ($query->setOption('term', 'ti.tid')) {
      $query->join('taxonomy_index', 'ti', 'n.nid = ti.nid');
    }
    // Only continue if the first pass query matches.
    if (!$query->executeFirstPass()) {
      return array();
    }

    // Add the ranking expressions.
    _node_rankings($query);

    // Load results.
    $find = $query
    ->limit(10)
    ->execute();
    $results = array();
    foreach ($find as $item) {
      // Build the node body.
      $results[] = node_load($item->sid);
    }
    return biblio_show_results($results);
  }

}

  /**
 * @param $arg
 * @return unknown_type
 */
function _get_biblio_search_filter($arg = 'keys') {
  if (variable_get('biblio_search', 0) &&
    !empty($_SESSION['biblio_filter']) &&
    is_array($_SESSION['biblio_filter']) &&
    is_array($_SESSION['biblio_filter'][0]) &&
    in_array('search', $_SESSION['biblio_filter'][0])
  ) {
    switch ($arg) {
      case 'keys': return $_SESSION['biblio_filter'][0][2]; break;
      case 'nodelist': return $_SESSION['biblio_filter'][0][1]; break;
    }
  }
}


function _get_biblio_filters() {

  $fields = " b.biblio_year, t.name , t.tid ";
  $order = " b.biblio_year DESC";
  $taxo_fields = "td.name as termname, td.tid as taxid, v.name as vocab_name";
  $taxo_order = "vocab_name ASC, termname ASC";
  $table = "{node} as n  inner join {biblio} as b on n.vid=b.vid ";
  $join = "left join {biblio_types} as t on b.biblio_type = t.tid";
  $taxo_join = array("inner join {taxonomy_index} as ti on n.nid = ti.nid",
                     "left join  {taxonomy_term_data} as td on ti.tid = td.tid",
                     "left join  {taxonomy_vocabulary} as v on v.vid = td.vid");

  $taxo_joins = implode(' ', $taxo_join);

  $result = db_query("SELECT $fields FROM $table $join ORDER BY $order");
  $authors = db_query("SELECT firstname, initials, lastname, cid FROM {biblio_contributor_data} ORDER BY lastname ASC");
  $keywords = db_query("SELECT word, kid FROM {biblio_keyword_data} ORDER BY word ASC");
  $taxoresult = db_query("SELECT $taxo_fields FROM $table $taxo_joins ORDER BY $taxo_order");
  $pub_years['[any]'] = t('any');
  $pub_type['[any]']  = t('any');
  $pub_authors['[any]']  = t('any');
  $pub_keywords['[any]']  = t('any');
  $pub_taxo['[any]']  = t('any');
  foreach ($result as $option) {
    if (isset ($option->biblio_year)) {
      $option->biblio_year = _biblio_text_year($option->biblio_year);
    }
    $pub_years[$option->biblio_year] = $option->biblio_year;
    $pub_type[$option->tid] = _biblio_localize_type($option->tid, $option->name);
  }

  foreach ($authors as $auth) {
    $pub_authors[$auth->cid] = $auth->lastname . ((!empty($auth->firstname) || !empty($auth->initials))?', ' . $auth->firstname . ' ' . $auth->initials :'');
  }
  foreach ($keywords as $keyword) {
    $pub_keywords[$keyword->kid] = $keyword->word;
  }
  foreach ($taxoresult as $tax) {
    $pub_taxo["$tax->taxid"] = "$tax->vocab_name - $tax->termname";
  }
  $author_select = isset($pub_authors) ? array('title' => t('Author'), 'options' => $pub_authors) : NULL;
  $years_select  = isset($pub_years) ? array('title' => t('Year'), 'options' => array_unique($pub_years)) : NULL;
  $type_select   = isset($pub_type)  ? array('title' => t('Type'), 'options' => array_unique($pub_type))  : NULL;
  $tax_select    = isset($pub_taxo)  ? array('title' => t('Term'), 'options' => array_unique($pub_taxo))  : NULL;
  $keyword_select = isset($pub_keywords) ? array('title' => t('Keyword'), 'options' => $pub_keywords) : NULL;

  $filters = array(
    'author'    => $author_select,
    'type'      => $type_select,
    'term_id'   => $tax_select,
    'year'      => $years_select,
    'keyword'   => $keyword_select,
  );

  return $filters;
}

/**
 * @return unknown_type
 */
function biblio_form_filter() {
  $session = $_SESSION['biblio_filter'];
  $session = is_array($session) ? $session : array();
  $filters = _get_biblio_filters();

  $i = 0;
  $form['filters'] = array(
    '#type' => 'fieldset',
    '#title' => t('Show only items where'),
    '#theme' => 'exposed_filters__node',
  );
  foreach ($session as $filter) {
    $type = key($filter);
    $value = $filter[$type];
   // list($type, $value) = $filter;
    if ($type == 'search') {
      $session = array();
      break;
    }
    if ($type == 'category') {
      // Load term name from DB rather than search and parse options array.
      $value = module_invoke('taxonomy', 'get_term', $value);
      $value = $value->name;
    }
    else {
      $value = $filters[$type]['options'][$value];
    }
    $t_args = array('%property' => $filters[$type]['title'], '%value' => $value);
    if ($i++) {
      $form['filters']['current'][] = array('#markup' => t('and where %property is %value', $t_args));
    }
    else {
      $form['filters']['current'][] = array('#markup' => t('where %property is %value', $t_args));
    }
    if (in_array($type, array('type', 'language'))) {
      // Remove the option if it is already being filtered on.
      unset($filters[$type]);
    }
  }

  $form['filters']['status'] = array(
    '#type' => 'container',
    '#attributes' => array('class' => array('clearfix')),
    '#prefix' => ($i ? '<div class="additional-filters">' . t('and where') . '</div>' : ''),
  );
  $form['filters']['status']['filters'] = array(
    '#type' => 'container',
    '#attributes' => array('class' => array('filters')),
  );
  foreach ($filters as $key => $filter) {
    $form['filters']['status']['filters'][$key] = array(
      '#type' => 'select',
      '#options' => $filter['options'],
      '#title' => $filter['title'],
      '#default_value' => '[any]',
    );
  }

  $form['filters']['status']['actions'] = array(
    '#type' => 'actions',
    '#attributes' => array('class' => array('container-inline')),
  );
  $form['filters']['status']['actions']['submit'] = array(
    '#type' => 'submit',
    '#value' => count($session) ? t('Refine') : t('Filter'),
  );
  if (count($session)) {
    $form['filters']['status']['actions']['undo'] = array('#type' => 'submit', '#value' => t('Undo'));
    $form['filters']['status']['actions']['reset'] = array('#type' => 'submit', '#value' => t('Reset'));
  }

  drupal_add_js('misc/form.js');

  return $form;
}


/**
 * @param $form
 * @param $form_state
 * @return unknown_type
 */
function biblio_form_filter_submit($form, &$form_state) {
  // If the search filter was set, remove it now.
  if (_get_biblio_search_filter()) {
    $_SESSION['biblio_filter'] = array();
  }
  $op = $form_state['values']['op'];
  $filters = _get_biblio_filters();
  switch ($op) {
    case t('Filter'):
    case t('Refine'):
        $uri = drupal_parse_url(request_uri());
        $uri['path'] = variable_get('biblio_base', 'biblio');
      foreach ($filters as $filter => $options) {
        if (isset($form_state['values'][$filter]) && $form_state['values'][$filter] != '[any]') {
          // Flatten the options array to accommodate hierarchical/nested options.
          $flat_options = form_options_flatten($filters[$filter]['options']);
          // Only accept valid selections offered on the dropdown, block bad input.
          if (isset($flat_options[$form_state['values'][$filter]])) {
            $_SESSION['biblio_filter'][] = array($filter => $form_state['values'][$filter]);
            $uri['query']['f'][$filter] = $form_state['values'][$filter];
          }
        }
      }
      drupal_goto($uri['path'], $uri);
      $base =  variable_get('biblio_base', 'biblio');
      drupal_goto($base);

      if (isset($form_state['values']['filter'])) {
        $uri = drupal_parse_url(request_uri());
        $uri['path'] = variable_get('biblio_base', 'biblio');
//        $filter = $form_state['values']['filter'];
//
//        // Flatten the options array to accommodate hierarchical/nested options.
//        if (isset($filters[$filter]['options'])) {
//          $flat_options = form_options_flatten($filters[$filter]['options']);
//        }
        foreach ($form_state['values']['filter'] as $filter => $value) {
          if (!empty($value)) {
          }
        }
 //       if (isset($flat_options[$form_state['values'][$filter]]) ) {
 //         $_SESSION['biblio_filter'][] = array($filter, $form_state['values'][$filter]);
 //         $base =  variable_get('biblio_base', 'biblio');

  //      }
      }
      break;
    case t('Undo'):
      array_pop($_SESSION['biblio_filter']);
      break;
    case t('Reset'):
      $_SESSION['biblio_filter'] = array();
      break;
  }
}

/**
 * @param $user
 * @param $profile
 * @return unknown_type
 */
function biblio_get_user_pubs($user, $profile='', $nofilters='') {
  $args = array();
  if (isset($user->data['biblio_contributor_id']) && $user->data['biblio_contributor_id'] > 0 ) {
    $args += array(
      'f' => array(
        'author' => $user->data['biblio_contributor_id'],
      ),
    );
  }
  elseif (isset($user->data['biblio_lastname']) && !empty($user->data['biblio_lastname'])) {
    $args += array(
      'f' => array(
        'author' => $user->data['biblio_lastname'],
      ),
    );
  }
  else {
    $args += array(
      'f' => array(
        'uid' => $user->uid
      ),
    );
  }

  $args += array('profile' => 1);

  return  biblio_db_search($args);;
}

/**
 * @param $node
 * @return unknown_type
 */
function biblio_view_inline(&$node) {
  $style = biblio_get_style();
  $base =  variable_get('biblio_base', 'biblio');
  $layout = variable_get('biblio_node_layout', 'tabular');
  $theme = ($layout == 'tabular') ? 'biblio_tabular' : 'biblio_long';
  $output  = '<div class="biblio-inline">';
  $output .= theme($theme, array('node' => $node, 'base' => $base, 'style' => $style));
  $output .= '</div>';
  return $output;
}

/**
 * @return unknown_type
 */
function biblio_citekey_view() {
  $citekey = arg(2);
  $nid = db_query("SELECT nid FROM {biblio} WHERE biblio_citekey = :citekey ORDER BY vid DESC", array(':citekey' => $citekey))->fetchObject();
  if ($nid->nid > 0) {
    $node = node_load($nid->nid);
    return node_page_view($node);
  }
  else {
    return t("Sorry, citekey @cite not found", array('@cite' => $citekey));
  }

}

/**
 * @param $keywords
 * @param $base
 * @return unknown_type
 */

function biblio_author_page() {
  $uri = drupal_parse_url(request_uri());
  $filter = isset($uri['query']['f']['author']) ? $uri['query']['f']['author'] : '';
  $authors = _biblio_get_authors($filter);
  return _biblio_format_author_page($filter, $authors);
}

function _biblio_get_authors($filter = NULL) {
  global $user;
  $where = array();
  $authors = array();
  $where_clause = '';
  $output = '';
  $style_name = biblio_get_style();
  module_load_include('inc', 'biblio', "/styles/biblio_style_$style_name");

    $base = variable_get('biblio_base', 'biblio');
    $menu = menu_get_active_title();
    if ($menu == 'Authors') $path = $base . '/authors/';
    if ($menu == 'Biblio settings') $path = 'admin/config/biblio/author/list/';
    if ($filter) {
      $filter = strtoupper($filter);
      $where[] =  "UPPER(SUBSTRING(lastname,1,1)) = :filter ";
      $header_ext = t(' (whose last name starts with the letter "@letter") ', array('@letter' => $filter ));
    }
    else {
      $query_ext =  NULL;
      $header_ext = NULL;
    }
    if ($user->uid != 1 ) {
      $where[] = 'n.status = 1 ';
    }//show only published entries to everyone except admin

    if (count($where)) {
      $where_clause = count($where) > 1 ? 'WHERE (' . implode(') AND (', $where) . ')': 'WHERE ' . $where[0];
    }

    $db_result = db_query('SELECT bd.cid, bd.drupal_uid, bd.name, bd.lastname,
                              bd.firstname, bd.prefix, bd.suffix, bd.initials,
                              bd.affiliation, bd.md5, COUNT(*) AS cnt
                            FROM {biblio_contributor} b
                                 LEFT JOIN {biblio_contributor_data} bd ON b.cid = bd.cid
                                 LEFT JOIN {node} n on n.vid = b.vid
                            ' . $where_clause . '
                            GROUP BY bd.cid, bd.drupal_uid, bd.name, bd.lastname,
                                     bd.firstname, bd.prefix, bd.suffix,
                                     bd.initials, bd.affiliation, bd.md5
                            HAVING COUNT(*) > 0
                            ORDER BY  lastname ASC, SUBSTRING(firstname,1,1) ASC,
                            initials ASC', array(':filter' => $filter), array('fetch' => PDO::FETCH_ASSOC));

    foreach ($db_result as $author) {
      $authors[] = $author;
    }

    return $authors;
}

function _biblio_format_author_page($filter, $authors) {
  $rows[] = array(array('data' => theme('biblio_alpha_line', array('type' => 'authors', 'current' => $filter)), 'colspan' => 3));
  if (count($authors)) {
      for ($i=0; $i < count($authors); $i+=3) {
        $rows[] = array( array('data' => _biblio_format_author($authors[$i]) ),
        array('data' => isset($authors[$i+1])?_biblio_format_author($authors[$i+1]):'' ),
        array('data' => isset($authors[$i+2])?_biblio_format_author($authors[$i+2]):'' ));
      }
    }
  //$header = array(array('data' => t('There are a total of @count authors !header_ext in the database',array('@count' => count($authors), '!header_ext' => $header_ext)), 'align' =>'center', 'colspan' => 3));
  $output = theme('table', array('rows' => $rows));
  return $output;
}
/*
 * Helper function to format the authors and add edit links if required
 */
function _biblio_format_author($author) {
  static $author_options = array();
  $format = biblio_format_authors(array($author));
  $format .= ' (' . $author['cnt'] . ') ' . ((biblio_access('edit_author'))?_biblio_author_edit_links($author):'');
  return $format;
}

function _biblio_author_edit_links($author) {
  static $path = '';
  $destination = drupal_get_destination();
  if (empty($path)) {
    $path =  (ord(substr($_GET['q'], -1)) > 97) ? $_GET['q'] . "/" : substr($_GET['q'], 0, -1);
    $path = (strpos($path, 'list/')) ? str_replace('list/', '', $path) : $path;
  }
  return l(' [' . t('edit') . ']', $path . $author['cid'] . "/edit", array('query' => $destination) );
}

function biblio_keyword_page() {
  $uri = drupal_parse_url(request_uri());
  $filter = isset($uri['query']['f']['keyword']) ? $uri['query']['f']['keyword'] : '';
  $keywords = _biblio_get_keywords($filter);
  return _biblio_format_keyword_page($uri, $filter, $keywords);
}

function _biblio_get_keywords($filter = NULL) {
  global $user;
  $keywords = array();
  $where = array();
  $where_clause = '';
  if ($filter) {
    $filter = strtoupper($filter);
    $where[] =  "UPPER(SUBSTRING(word,1,1)) = :filter ";
    $header_ext = t(' (which start with the letter "@letter") ', array('@letter' => $filter ));
  }
  else {
    $query_ext =  NULL;
    $header_ext = NULL;
  }

  if ($user->uid != 1 ) {
    $where[] = 'n.status = 1 ';
  }//show only published entries to everyone except admin

  if (count($where)) {
    $where_clause = count($where) > 1 ? 'WHERE (' . implode(') AND (', $where) . ')': 'WHERE ' . $where[0];
  }

  $db_result = db_query('SELECT bkd.kid, bkd.word, COUNT(*) AS cnt
                         FROM {biblio_keyword} bk
                         LEFT JOIN {biblio_keyword_data} bkd ON bkd.kid = bk.kid
                         LEFT JOIN {node} n ON n.vid = bk.vid
                         '. $where_clause . '
                         GROUP BY bkd.kid, bkd.word HAVING COUNT(*) > 0
                         ORDER BY  word ASC', array(':filter' => $filter));

  foreach ($db_result as $keyword) {
    $keywords[] = $keyword;
  }
  return $keywords;
}

function _biblio_format_keyword_page($uri, $filter, $keywords) {
  $rows[] = array(array('data' => theme('biblio_alpha_line', array('type' => 'keywords', 'current' => $filter, 'path' => '')), 'colspan' => 3));
  for ($i=0; $i < count($keywords); $i+=3) {
    $rows[] = array( array('data' => _biblio_format_keyword($uri, $keywords[$i]) ),
    array('data' => isset($keywords[$i+1]) ? _biblio_format_keyword($uri, $keywords[$i+1]) : '' ),
    array('data' => isset($keywords[$i+2]) ? _biblio_format_keyword($uri, $keywords[$i+2]) : '' ));
  }
  //$header = array(array('data' => t('There are a total of @count keywords !header_ext in the database',array('@count' => count($keywords), '!header_ext' => $header_ext)), 'align' =>'center', 'colspan' => 3));
  $output = theme('table', array('rows' => $rows));
  return $output;
}
function _biblio_format_keyword($uri, $keyword) {
  $base      = variable_get('biblio_base', 'biblio');
  $uri['path'] = $base;
  $uri['query']['f']['keyword'] = $keyword->kid;
  $format    = l(trim($keyword->word), $base, $uri);
  $format   .= ' (' . $keyword->cnt . ') ' ;
  $destination = drupal_get_destination();
  $path =  (ord(substr($_GET['q'], -1)) > 97) ? $_GET['q'] . "/" : substr($_GET['q'], 0, -1);
  $edit_link = ' [' . l(t('edit'), $path . $keyword->kid . "/edit", array('query' => $destination)) . '] ';
  $format   .= (user_access('administer biblio')) ? $edit_link : '';

  return $format;
}
