<?php

/**
 * @file
 * Copyright (C) 2006-2011  Ron Jerome.
 */

/**
 *
 */
function biblio_view_node($nid) {
  $options = array();
  // Set a 301 response code.
  drupal_goto('node/' . (int) $nid, $options, 301);
}

/**
 *
 */
function biblio_arg_handler($arg_info) {
  $arg_list = array();
  if (count($arg_info['func_args']) == 1 && is_array($arg_info['func_args'][0])) {
    return $arg_info['func_args'][0];
  }

  while ($arg_info['func_args']) {
    $arg   = array_shift($arg_info['func_args']);
    $value = array_shift($arg_info['func_args']);
    if ($arg == 'sort') {
      $arg_list['s'] = $value;
    }
    elseif ($arg == 'order') {
      $arg_list['o'] = (strtolower($value) == 'desc') ? 'desc' : 'asc';
    }
    else {
      $arg_list['f'][$arg] = $value;
    }
  }
  $arg_list = array_merge_recursive($arg_list, $arg_info['uri']['query']);
  return $arg_list;
}

/**
 *
 */
function biblio_profile_page($user) {
  $arg_list = array();
  if (isset($user->data['biblio_contributor_id']) && $user->data['biblio_contributor_id'] > 0) {
    $arg_list = array(
      'f' => array(
        'author' => $user->data['biblio_contributor_id'],
        'profile' => TRUE,
      ),
    );
  }
  elseif (isset($user->data['biblio_lastname']) && !empty($user->data['biblio_lastname'])) {
    $arg_list = array(
      'f' => array(
        'author' => $user->data['biblio_lastname'],
        'profile' => TRUE,
      ),
    );
  }
  else {
    $arg_list = array(
      'f' => array(
        'uid' => $user->uid,
        'profile' => TRUE,
      ),
    );
  }
  $arg_list['page_limit'] = variable_get('biblio_rowsperpage', 25);

  list($nids, $extras, $rss_info) = biblio_build_query($arg_list);
  $render['biblio_page']['header'] = biblio_page_header($arg_list);

  $render['biblio_page']['profile'] = biblio_page_content($nids, $extras);

  return $render;
}

/**
 *
 */
function biblio_page() {
  $nodes = $nids = $extras = $rss_info = $info = $arg_list = $render = array();
  $base = variable_get('biblio_base', 'biblio');

  $info['func_args'] = func_get_args();
  $info['uri'] = drupal_parse_url(request_uri());
  $arg_list = biblio_arg_handler($info);
  $arg_list['page_limit'] = variable_get('biblio_rowsperpage', 25);

  list($nids, $extras, $rss_info) = biblio_build_query($arg_list);
  if ($rss_info['feed']) {
    biblio_filter_feed($rss_info, $nids);
    return;
  }

  if (variable_get('biblio_rss', 0)) {
    drupal_add_html_head_link(array(
      'rel' => 'alternate',
      'type' => 'application/rss+xml',
      'title' => variable_get('site_name', 'Drupal') . ' RSS',
      'href' => url("$base/rss.xml"),
    ));
  }

  drupal_set_title(t(check_plain(variable_get('biblio_base_title', 'Biblio'))));
  $filter = (isset($arg_list['f'])) ? array('f' => $arg_list['f']) : array();

  $render['biblio_page']['header'] = biblio_page_header($filter);
  $render['biblio_page']['content'] = biblio_page_content($nids, $extras);

  // If no content AND we have a filter give a hint on what to do...
  if (isset($render['biblio_page']['content']['section_0']) && !empty($render['biblio_page']['header']['filter_status'])) {
    $render['biblio_page']['content']['hint']['#markup'] = t('!modify_link or !remove_link your filters and try again.', array('!modify_link' => l(t('Modify'), "$base/filter"), '!remove_link' => l(t('remove'), "$base/filter/clear")));
  }

  return $render;
}

/**
 *
 */
function biblio_page_header($filter = array()) {
  $header = array();
  $header = array(
    '#prefix' => '<div id="biblio-header" class="clear-block">',
    '#suffix' => '</div>',
    '#weight' => -100,
  );

  // Search box. Has same permissions as the filter tab.
  if (variable_get('biblio_search', 0) && user_access('show filter tab')) {
    $header += array(
      'search_form' => drupal_get_form('biblio_search_form'),
    );
  }
  $header += array(
    'export_links' => array(
      '#prefix' => '<div class="biblio-export">',
      '#node'    => NULL,
      '#filter' => $filter,
      '#theme'  => 'biblio_export_links',
      '#suffix' => '</div>',
    ),
  );

  if (!biblio_access('export')) {
    global $pager_total_items;
    $header['export_links']['#markup'] = t('Found @count results', array('@count' => $pager_total_items[0]));
    unset($header['export_links']['#theme']);
  }
  // Add some links to the top of the page to change the sorting/ordering...
  if (user_access('show sort links')) {
    $header += array(
      'sort_links' => array(
        '#markup' => theme('biblio_sort_tabs'),
      ),
    );
  }
  $header += array(
    'filter_status' => array(
      '#prefix' => '<div class="biblio-filter-status">',
      '#suffix' => '</div>',
      '#markup' => _biblio_filter_info_line($filter),
    ),
  );
  if (isset($_GET['s'])) {
    if ($_GET['s'] == 'title' ||
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
      $header += array(
        'alpha_line' => array(
          '#prefix' => '<div class="biblio-alpha-line">',
          '#suffix' => '</div>',
          '#markup' => theme('biblio_alpha_line', array(
            'type' => $_GET['s'],
            'current' => $value,
            'path' => variable_get('biblio_base', 'biblio'),
          )
          ),
        ),
      );
    }
  }

  return $header;
}

/**
 *
 */
function biblio_page_content($nids = array(), $extras = array()) {
  $base = variable_get('biblio_base', 'biblio');
  $content = $raw_nodes = $nodes = array();
  $count = $section_id = 0;

  if (module_exists('popups')) {
    popups_add_popups();
  }

  if (count($nids)) {
    // $nids = array_unique($nids);
    $raw_nodes = node_load_multiple($nids);
    $langcode = $GLOBALS['language_content']->language;
    field_attach_prepare_view('node', $raw_nodes, 'biblio_list', $langcode);
    entity_prepare_view('node', $raw_nodes, $langcode);

    foreach ($nids as $key => $nid) {
      if (!empty($extras)) {
        $nodes[] = (object) array_merge((array) $raw_nodes[$nid], (array) $extras[$key]);
      }
      else {
        $nodes[] = $raw_nodes[$nid];
      }
    }

  }

  foreach ($nodes as $node) {
    $count++;
    if (is_array($node)) {
      $node = (object) $node;
    }
    if (variable_get('biblio_hide_bibtex_braces', 0)) {
      $node->title = biblio_remove_brace($node->title);
    }

    // Output new section if needed.
    if ($section = biblio_category_section($node)) {
      $section_id++;
      $content['section_' . $section_id] = $section;
    }

    $content['section_' . $section_id][] = biblio_entry($node);

  }

  $content['pager']['#markup'] = theme('pager');
  if ($count == 0) {
    $content['section_0']['#markup'] = "<h3>" . t("No items found") . "</h3>";
  }
  return $content;
}

/**
 *
 */
function biblio_entry($node) {
  $entry = array();
  $style = biblio_get_style();

  /*
  $select_box = array(
  '#type' => 'checkbox',
  '#return_value'  => $node->nid,
  '#default_value' => 0,
  '#attributes' => array('class' => array('biblio-export-selector'),)

  );
   */
  $prefix = '<div class="biblio-entry">';
  $suffix = '</div>';
  if (!$node->status) {
    $prefix .= '<div id="node-' . $node->nid . '" class="node node-unpublished">';
    $suffix .= '</div>';
  }
  // $prefix .= theme('checkbox', array('element' => $select_box));.
  $entry = array(
    '#prefix' => $prefix,
    '#suffix' => $suffix,
  );

  $entry['entry']['#markup'] = theme('biblio_style', array('node' => $node, 'style_name' => $style));

  $annotation_field = variable_get('biblio_annotations', 'none');
  if ($annotation_field != 'none' && $node->$annotation_field) {
    $entry['annotation'] = array(
      '#prefix' => '<div class="biblio-annotation">',
      '#markup' => filter_xss($node->$annotation_field, biblio_get_allowed_tags()),
      '#suffix' => '</div>',
    );
  }

  $openurl_base = variable_get('biblio_baseopenurl', '');

  if ($openurl_base) {
    $entry['openurl'] = array(
      '#markup' => theme('biblio_openurl', array('openURL' => biblio_openurl($node))),
    );
  }

  if (biblio_access('export')) {
    $base = variable_get('biblio_base', 'biblio');
    $entry['export_links'] = array(
      '#markup' => theme('biblio_export_links', array('node' => $node)),
    );
  }

  if (biblio_access('download', $node)) {
    // Add links to attached files (if any)
    $entry['download_links'] = array(
      '#markup' => theme('biblio_download_links', array('node' => $node)),
    );
  }

  return $entry;
}

/**
 * Biblio_db_search builds the SQL query which will be used to
 * select and order "biblio" type nodes.  The query results are
 * then passed to biblio_show_results for output.
 */
function biblio_build_query($arg_list) {
  global $user;
  // biblio_contributor (bc) count , increase for every invocation.
  static $bcc = 0;
  static $bkd = 0;
  // Term counter, increase for every invocation.
  static $tcc = 0;
  $rss_info['feed'] = FALSE;
  $rss_info['title'] = variable_get('biblio_base_title', 'Biblio');
  $rss_info['link'] = '';
  $rss_info['description'] = '';

  if ($arg_list['page_limit'] > 0) {
    $query = db_select('node', 'n')->extend('PagerDefault');
    $query->limit($arg_list['page_limit']);
  }
  else {
    $query = db_select('node', 'n');
  }

  // Add a tag of "node_access" to ensure that only nodes to which the user has access are retrieved.
  $query->addTag('node_access');

  $query->addField('n', 'nid');
  $type_name = $query->addField('bt', 'name', 'biblio_type_name');
  $query->leftJoin('biblio', 'b', 'n.vid=b.vid');
  $query->innerJoin('biblio_types', 'bt', 'b.biblio_type=bt.tid');
  // $query->distinct();
  // POSIX regular expression matching, case insensitive.
  $match_op = (db_driver() == 'pgsql') ? '~*' : 'RLIKE';

  $limit = '';
  if (variable_get('biblio_view_only_own', 0)) {
    $limit .= " AND n.uid = $user->uid ";
  }

  if (isset($arg_list['s']) || isset($arg_list['sort'])) {
    $sort = isset($arg_list['s']) ? $arg_list['s'] : $arg_list['sort'];
    unset($arg_list['s']);
    unset($arg_list['sort']);
  }
  else {
    $sort = variable_get('biblio_sort', 'year');
  }

  if (isset($arg_list['o']) || isset($arg_list['order'])) {
    if (isset($arg_list['o'])) {
      $order = (strcasecmp($arg_list['o'], 'ASC')) ? 'DESC' : 'ASC';
      unset($arg_list['o']);
    }
    if (isset($arg_list['order'])) {
      $order = (strcasecmp($arg_list['order'], 'ASC')) ? 'DESC' : 'ASC';
      unset($arg_list['order']);
    }
  }
  else {
    $order = strtolower(variable_get('biblio_order', 'DESC'));
  }

  /*  if (!isset($_SESSION['biblio_filter']) || !is_array($_SESSION['biblio_filter'])) {
  $_SESSION['biblio_filter'] = array();
  }

  $session = &$_SESSION['biblio_filter'];

  if (!in_array('no_filters', $arg_list)) {
  foreach ($session as $filter) {
  $arg_list = array_merge($arg_list, $filter);
  }
  }
   */

  switch ($sort) {
    case 'type':
      // $sortby = "ORDER BY bt.name %s, b.biblio_year DESC ";.
      $query->addField('n', 'title');
      $query->orderBy($type_name, $order);
      $query->orderBy('biblio_sort_title', $order);
      break;

    case 'title':
      $query->addField('n', 'title');
      $query->orderBy('biblio_sort_title', $order);
      break;

    case 'author':
      // $last_name = $query->addField('bcd', 'lastname');.
      $query->innerJoin('biblio_contributor', 'bc', 'b.vid = bc.vid');
      $query->join('biblio_contributor_data', 'bcd', 'bc.cid = bcd.cid');
      $query->condition('bc.rank', 0);
      $query->addField('bcd', 'lastname');
      $query->orderBy('bcd.lastname', $order);
      // $query->condition('bc.auth_category', 1);.
      break;

    // Added msh 070808.
    case 'keyword':
      $word = $query->addField('bkd', 'word', 'biblio_keyword');
      $query->orderBy($word, $order);
      $query->innerJoin('biblio_keyword', 'bk', 'b.vid = bk.vid');
      $query->innerJoin('biblio_keyword_data', 'bkd', 'bk.kid = bkd.kid');
      break;

    case 'year':
    default:
      $query->addField('b', 'biblio_year');
      $query->addField('b', 'biblio_date');
      $query->orderBy('biblio_year', $order);
      $query->orderBy('biblio_sort_title');
  } //end switch

  if (isset($arg_list['f']) && count($arg_list['f'])) {
    $fields = biblio_get_db_fields();
    foreach ($arg_list['f'] as $type => $value) {
      $tables = array_keys($query->getTables());
      switch ($type) {
        case 'no_filters':
          break;

        case 'rss.xml':
          $rss_info['feed'] = TRUE;
          $query->limit(variable_get('biblio_rss_number_of_entries', 10));
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
          $query->where("UPPER(substring(biblio_sort_title,1 ,1)) = :letter", array(':letter' => $value));
          break;

        // Selects entries whoose authors firstname starts with the letter provided.
        case 'ag':
          $query->where(" UPPER(substring(bcd.lastname,1,1)) = :letter ", array(':letter' => $value));
          // $where['bc-rank'] = "bc.rank=0";.
          if ($sort != 'author') {
            if (array_search('bc', $tables) === FALSE) {
              $query->innerJoin('biblio_contributor', 'bc', 'n.vid = bc.vid');
            }
            $query->innerJoin('biblio_contributor_data', 'bcd', 'bc.cid = bcd.cid');
          }
          break;

        case 'cid':
        case 'aid':
          $bcc++;
          $query->innerJoin('biblio_contributor', "bc$bcc", "n.vid = bc$bcc.vid");
          $query->condition("bc$bcc.cid", $value);
          break;

        case 'author':
          module_load_include('inc', 'biblio', 'includes/biblio.contributors');
          $bcc++;
          if (array_search('bc', $tables) === FALSE) {
            $query->innerJoin('biblio_contributor', 'bc', 'n.vid = bc.vid');
          }
          if (is_numeric($value)) {
            $cids = biblio_get_linked_contributors($value);
            $cids[] = $value;
            $cid_count = 0;
            $or = db_or();

            foreach ($cids as $cid) {
              $or->condition("bc.cid", $cid);
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
            if (array_search('bcd', $tables) === FALSE) {
              $query->innerJoin('biblio_contributor_data', 'bcd', 'bcd.cid = bc.cid');
            }

            // [[:<:]], [[:>:]] These markers stand for word boundaries. They
            // match the beginning and end of words, respectively. A word is a
            // sequence of word characters that is not preceded by or followed
            // by word characters. A word character is an alphanumeric character
            // in the alnum class or an underscore (_).
            // https://dev.mysql.com/doc/refman/5.7/en/regexp.html
            $query->condition('bcd.name', "[[:<:]]" . preg_quote($value) . "[[:>:]]", $match_op);
            $rss_info['title'] = t("Publications by @value", array('@value' => $value));
            $rss_info['description'] = t("These publications by %author are part of the works listed at %sitename", array('%author' => $value, '%sitename' => variable_get('site_name', 'Drupal')));
            $rss_info['link'] = '/author/' . $value;
          }
          break;

        case 'publisher':
          $query->condition('b.biblio_publisher', "[[:<:]]" . preg_quote($value) . "[[:>:]]", $match_op);
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
          if (array_search('bk', $tables) === FALSE) {
            $query->innerJoin('biblio_keyword', 'bk', 'n.vid = bk.vid');
          }
          if (is_numeric($value)) {
            $query->condition('bk.kid', $value);
          }
          else {
            if (array_search('bkd', $tables) === FALSE) {
              $query->innerJoin('biblio_keyword_data', 'bkd', 'bkd.kid = bk.kid');
            }
            if (strlen($value) == 1) {
              // $query->condition('',  $value, 'SUBSTR(bkd.word, 1, 1) =');.
              $query->where(" UPPER(substring(bkd.word,1,1)) = :letter ", array(':letter' => $value));
            }
            else {
              $query->condition('bkd.word', "[[:<:]]" . preg_quote($value) . "[[:>:]]", 'LIKE');
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
          $search_nids = array();
          $search_nids = biblio_search_query($value);
          if (empty($search_nids)) {
            // If we didn't find anything, then add one value of -1 since there will never be a node id == -1.
            $search_nids[] = -1;
          }
          $query->condition('n.nid', $search_nids, 'IN');
          break;

        default:
          if (in_array("biblio_$type", $fields)) {
            $query->condition("b.biblio_$type ", $value, 'LIKE');
          }
          break;
      }
    }
  }

  // Show unpublished nodes to users with uid = 1 or  with 'Administer Biblio' permissions.
  if ($user->uid != 1 && !biblio_access('admin')) {
    $query->condition('n.status', 1);
  }

  $result = $query->execute();
  $nids = array();
  $extras = array();

  foreach ($result as $node) {
    $nids[] = $node->nid;
    if (isset($node->biblio_year)) {
      unset($node->biblio_year);
    }
    $extras[] = $node;
  }

  return array($nids, $extras, $rss_info);

}

/**
 *
 */
function _biblio_filter_info_line($args = array()) {
  $content = '';
  $filtercontent = '';
  $search_content = FALSE;
  $profile_content = FALSE;
  $base = variable_get('biblio_base', 'biblio');
  $session = !empty($_SESSION['biblio_filter']) ? $_SESSION['biblio_filter'] : array();
  // If there are any filters in place, print them at the top of the list.
  $uri = drupal_parse_url(request_uri());
  $uri['path'] = variable_get('biblio_base', 'biblio');
  $filters = isset($uri['query']['f']) ? $uri['query']['f'] : (isset($args['f']) ? $args['f'] : array());
  if (count($filters)) {
    $i = 0;
    foreach ($filters as $type => $value) {
      // Will $value be run through check_plain()?
      $check_plain = TRUE;
      if ($type == 'search') {
        $search_content = $value;
        continue;
      }
      if ($type == 'profile') {
        $profile_content = $value;
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
      if ($type == 'uid') {
        $user = user_load($value);
        $value = $user->name;
        $type = t("Drupal user");
      }
      if ($type == 'aid' || ($type == 'author' && is_numeric($value))) {
        module_load_include('inc', 'biblio', 'includes/biblio.contributors');
        $author = biblio_get_contributor($value);
        $check_plain = FALSE;
        $value = isset($author->name) ? check_plain($author->name) : t('Unknown Author');

        // Link to edit page for author.
        if (drupal_valid_path(variable_get('biblio_base', 'biblio') . '/authors/%/edit')) {
          $value .= _biblio_author_edit_links($author);
        }

        $type = t("Author");
      }
      if ($type == 'ag') {
        // return;.
        $type = t("First letter of last name");
      }
      if ($type == 'tg') {
        // return;.
        $type = t("First letter of title");
      }
      if ($type == 'type' && $value > 0) {
        if (($pub_type = db_query('SELECT t.* FROM {biblio_types} as t WHERE t.tid=:tid', array(':tid' => $value))->fetchObject())) {
          $value = drupal_ucfirst(_biblio_localize_type($pub_type->tid, $pub_type->name));
          $type = t("Type");
        }
      }
      $params = array('%a' => check_plain(ucwords($type)) , '!b' => $check_plain ? check_plain($value) : $value);
      $filtercontent .= ($i++ ? t(' <strong>and</strong> %a is <em class="placeholder">!b</em>', $params) : t('%a is <em class="placeholder">!b</em>', $params));
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
    elseif (!$search_content && !$profile_content) {
      $content .= '&nbsp;&nbsp;' . l('[' . t('Clear All Filters') . ']', "$base/filter/clear", $uri);
    }
    $content .= '</div>';
  }

  return $content;
}

/**
 *
 */
function _biblio_category_separator_bar($node, $reset = FALSE) {
  $_text = &drupal_static(__FUNCTION__, '');
  if ($reset) {
    $_text = &drupal_static(__FUNCTION__, '', TRUE);
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
      $title = $node->biblio_sort_title;
      $first = drupal_substr(drupal_ucfirst(ltrim($title)), 0, 1);
      if ($first != $_text) {
        if ($_text != '') {
          $content .= theme_biblio_end_category_section();
        }
        $_text = $first;
        $content .= theme_biblio_separator_bar($_text);
      }
      break;

    case 'author':
      if ((isset($node->biblio_contributors[0]['lastname'])) &&
      (drupal_substr(drupal_ucfirst(ltrim($node->biblio_contributors[0]['lastname'])), 0, 1) != $_text)) {
        if ($_text != '') {
          $content .= theme_biblio_end_category_section();
        }
        $_text = drupal_substr(drupal_ucfirst(ltrim($node->biblio_contributors[0]['lastname'])), 0, 1);
        $content .= theme_biblio_separator_bar($_text);
      }
      break;

    case 'type':
      if ($node->biblio_type_name != $_text) {
        if ($_text != '') {
          $content .= theme_biblio_end_category_section();
        }
        $_text = $node->biblio_type_name;
        // $name = db_result(db_query("SELECT name FROM {biblio_types} as t where t.tid=%d", $node->biblio_type)) ;.
        $content .= theme_biblio_separator_bar(_biblio_localize_type($node->biblio_type, $_text));
      }
      break;

    // Added msh 08 aug 07.
    case 'keyword':
      // $kw = array_shift($node->biblio_keyword);.
      $tok = $node->biblio_keyword;
      if (empty($tok)) {
        $tok = t("No Keywords");
      }
      if ($tok != $_text) {
        if ($_text != '') {
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
        if ($_text != '') {
          $content .= theme_biblio_end_category_section();
        }
        $_text = $node->biblio_year;
        $content .= theme_biblio_separator_bar($_text);
      }
  } //end switch
  return $content;
}

/**
 *
 */
function biblio_category_section($node, $reset = FALSE) {
  $_text = &drupal_static(__FUNCTION__, '');
  if ($reset) {
    $_text = &drupal_static(__FUNCTION__, '', TRUE);
    return;
  }

  $section = array();

  if (isset($_GET['s'])) {
    $sort = $_GET['s'];
  }
  else {
    $sort = variable_get('biblio_sort', 'year');
  }

  switch ($sort) {
    case 'title':
      $title = $node->biblio_sort_title;
      $first = drupal_substr(drupal_ucfirst(ltrim($title)), 0, 1);
      if ($first != $_text) {
        $_text = $first;
        $section['bar'] = biblio_section_bar($_text);
      }
      break;

    case 'author':
      if ((isset($node->biblio_contributors[0]['lastname'])) &&
      (drupal_substr(drupal_ucfirst(ltrim($node->biblio_contributors[0]['lastname'])), 0, 1) != $_text)) {
        $_text = drupal_substr(drupal_ucfirst(ltrim($node->biblio_contributors[0]['lastname'])), 0, 1);
        $section['bar'] = biblio_section_bar($_text);
      }
      break;

    case 'type':
      if ($node->biblio_type_name != $_text) {
        $_text = $node->biblio_type_name;
        // $name = db_result(db_query("SELECT name FROM {biblio_types} as t where t.tid=%d", $node->biblio_type)) ;.
        $section['bar'] = biblio_section_bar(_biblio_localize_type($node->biblio_type, $_text));
      }
      break;

    // Added msh 08 aug 07.
    case 'keyword':
      // $kw = array_shift($node->biblio_keyword);.
      $tok = $node->biblio_keyword;
      if (empty($tok)) {
        $tok = t("No Keywords");
      }
      if ($tok != $_text) {
        $_text = $tok;
        if ($_text != '') {
          $section['bar'] = biblio_section_bar($_text);
        }
      }
      break;

    case 'year':
    default:
      if ($node->biblio_year != $_text) {
        $_text = $node->biblio_year;
        $section['bar'] = biblio_section_bar($_text);
      }
  } //end switch
  if (!empty($section)) {
    $section += array(
      '#prefix' => '<div class="biblio-category-section">',
      '#suffix' => '</div>',
    );
  }
  return $section;
}

/**
 *
 */
function biblio_section_bar($text) {
  return array(
    '#prefix' => '<div class="biblio-separator-bar">',
    '#suffix' => '</div>',
    '#markup' => check_plain($text),
  );
}

/**
 * Add a search field on the main biblio page.
 */

/**
 * @param $form_state
 *
 * @return
 */
function biblio_search_form($form, &$form_state) {
  $base = variable_get('biblio_base', 'biblio');
  $searchform['biblio_search'] = array(
    '#prefix' => '<div class="container-inline biblio-search clear-block">',
    '#suffix' => '</div>',
  );
  $searchform['biblio_search']['keys'] = array(
    '#type' => 'textfield',
    '#title' => '',
    '#default_value' => '',
    '#size' => 25,
    '#maxlength' => 255,
  );
  $button_text = variable_get('biblio_search_button_text', 'Biblio search');
  $searchform['biblio_search']['submit'] = array(
    '#type' => 'submit',
    '#value' => t($button_text),
  );

  $form['search_form'] = array(
    '#type'   => 'fieldset',
    '#title'   => t('Search'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
    'searchform' => $searchform,
  );
  if (variable_get('biblio_search_form_filter', TRUE)) {
    $form['search_form']['filterform'] = biblio_form_filter();
  }

  return $form;
}

/**
 *
 */
function biblio_search_form_submit($form, &$form_state) {
  static $keys = '';
  $base = variable_get('biblio_base', 'biblio');
  $keys = trim($form_state['values']['keys']);
  if (!empty($keys)) {
    $uri = drupal_parse_url(request_uri());
    $uri['path'] = variable_get('biblio_base', 'biblio');
    $uri['query']['f']['search'] = $keys;
    $form_state['redirect'] = array($uri['path'], $uri);
  }

}

/**
 *
 */
function biblio_search_query($keys) {
  if (!empty($keys)) {
    $query = db_select('search_index', 'i', array('target' => 'slave'))
      ->extend('SearchQuery');
    // ->extend('PagerDefault');.
    $query->join('node', 'n', 'n.nid = i.sid');
    $query->condition('n.status', 1)
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
    $find = $query->execute();

    $nids = array();
    foreach ($find as $item) {
      $nids[] = $item->sid;
    }
    return $nids;
  }
}

/**
 * @param $arg
 *
 * @return
 */
function _get_biblio_search_filter($arg = 'keys') {
  if (variable_get('biblio_search', 0) &&
    !empty($_SESSION['biblio_filter']) &&
    is_array($_SESSION['biblio_filter']) &&
    is_array($_SESSION['biblio_filter'][0]) &&
    in_array('search', $_SESSION['biblio_filter'][0])
  ) {
    switch ($arg) {
      case 'keys':
        return $_SESSION['biblio_filter'][0][2];

      case 'nodelist':
        return $_SESSION['biblio_filter'][0][1];
    }
  }
}

/**
 *
 */
function _get_biblio_filters() {

  $fields = " b.biblio_year, t.name , t.tid ";
  $order = " b.biblio_year DESC";
  $taxo_fields = "td.name as termname, td.tid as taxid, v.name as vocab_name";
  $taxo_order = "vocab_name ASC, termname ASC";
  $table = "{node} as n  inner join {biblio} as b on n.vid=b.vid ";
  $join = "left join {biblio_types} as t on b.biblio_type = t.tid";
  $taxo_join = array("inner join {taxonomy_index} as ti on n.nid = ti.nid",
    "left join  {taxonomy_term_data} as td on ti.tid = td.tid",
    "left join  {taxonomy_vocabulary} as v on v.vid = td.vid",
  );

  $taxo_joins = implode(' ', $taxo_join);

  $result = db_query("SELECT $fields FROM $table $join ORDER BY $order");
  $authors = db_query("SELECT DISTINCT firstname, initials, lastname, bcd.cid
                       FROM {biblio_contributor_data} as bcd
                       INNER JOIN {biblio_contributor} as bc on bc.cid = bcd.cid
                       ORDER BY lastname ASC");
  $keywords = db_query("SELECT word, kid FROM {biblio_keyword_data} ORDER BY word ASC");
  $taxoresult = db_query("SELECT $taxo_fields FROM $table $taxo_joins ORDER BY $taxo_order");
  $pub_years['any'] = t('any');
  $pub_type['any'] = t('any');
  $pub_authors['any'] = t('any');
  $pub_keywords['any'] = t('any');
  $pub_taxo['any'] = t('any');
  foreach ($result as $option) {
    if (isset($option->biblio_year)) {
      $option->biblio_year = _biblio_text_year($option->biblio_year);
    }
    $pub_years[$option->biblio_year] = $option->biblio_year;
    $pub_type[$option->tid] = _biblio_localize_type($option->tid, $option->name);
  }

  foreach ($authors as $auth) {
    $pub_authors[$auth->cid] = $auth->lastname . ((!empty($auth->firstname) || !empty($auth->initials)) ? ', ' . $auth->firstname . ' ' . $auth->initials : '');
  }
  foreach ($keywords as $keyword) {
    $pub_keywords[$keyword->kid] = $keyword->word;
  }
  foreach ($taxoresult as $tax) {
    $pub_taxo["$tax->taxid"] = "$tax->vocab_name - $tax->termname";
  }
  $author_select  = isset($pub_authors) ? array('title' => t('Author'), 'options' => $pub_authors) : NULL;
  $years_select   = isset($pub_years) ? array('title' => t('Year'), 'options' => array_unique($pub_years)) : NULL;
  $type_select    = isset($pub_type) ? array('title' => t('Type'), 'options' => array_unique($pub_type)) : NULL;
  $tax_select     = isset($pub_taxo) ? array('title' => t('Term'), 'options' => array_unique($pub_taxo)) : NULL;
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
 * @return
 */
function biblio_form_filter() {
  $session = isset($_SESSION['biblio_filter']) ? $_SESSION['biblio_filter'] : array();
  $filters = _get_biblio_filters();

  $i = 0;
  $form['filters'] = array(
    '#type' => 'fieldset',
    '#title' => t('Show only items where'),
    '#theme' => 'exposed_filters__node',
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );

  foreach ($session as $filter) {
    $type = key($filter);
    $value = $filter[$type];
    // list($type, $value) = $filter;.
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
      '#default_value' => 'any',
    );
  }

  $form['filters']['status']['actions'] = array(
    '#type' => 'actions',
    '#attributes' => array('class' => array('container-inline')),
  );
  $form['filters']['status']['actions']['submit'] = array(
    '#type' => 'submit',
    '#value' => count($session) ? t('Refine') : t('Filter'),
    '#submit' => array('biblio_form_filter_submit'),
  );
  if (count($session)) {
    $form['filters']['status']['actions']['undo'] = array(
      '#type' => 'submit',
      '#value' => t('Undo'),
      '#submit' => array('biblio_form_filter_submit'),
    );
    $form['filters']['status']['actions']['reset'] = array(
      '#type' => 'submit',
      '#value' => t('Reset'),
      '#submit' => array('biblio_form_filter_submit'),
    );
  }

  return $form;
}

/**
 * @param $form
 * @param $form_state
 *
 * @return
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
        if (isset($form_state['values'][$filter]) && $form_state['values'][$filter] != 'any') {
          // Flatten the options array to accommodate hierarchical/nested options.
          $flat_options = form_options_flatten($filters[$filter]['options']);
          // Only accept valid selections offered on the dropdown, block bad input.
          if (isset($flat_options[$form_state['values'][$filter]])) {
            $_SESSION['biblio_filter'][] = array($filter => $form_state['values'][$filter]);
            $uri['query']['f'][$filter] = $form_state['values'][$filter];
          }
        }
      }
      $form_state['redirect'] = array($uri['path'], $uri);
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
 * @return
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
 *
 */
function biblio_author_page() {
  $path = drupal_get_path('module', 'biblio');
  drupal_add_js($path . '/misc/biblio.highlight.js', 'file');

  $uri = drupal_parse_url(request_uri());
  $filter = isset($uri['query']['f']['author']) ? $uri['query']['f']['author'] : '';
  $authors = _biblio_get_authors($filter);

  return _biblio_format_author_page($uri['path'], $filter, $authors);
}

/**
 *
 */
function _biblio_get_authors($filter = NULL) {
  global $user;
  $where = array();
  $authors = array();
  $where_clause = '';
  $output = '';

  if ($filter) {
    $filter = strtoupper($filter);
    $where['filter'] = "UPPER(SUBSTRING(lastname,1,1)) = :filter ";
    $header_ext = t(' (whose last name starts with the letter "@letter") ', array('@letter' => $filter));
  }
  else {
    $query_ext = NULL;
    $header_ext = NULL;
  }

  if (!biblio_access('edit_author')) {
    $where['access'] = 'n.status = 1 ';
  }//show only published entries to everyone except admin

  if (count($where)) {
    $where_clause = 'WHERE (' . implode(') AND (', $where) . ')';
  }

  $query = db_select('biblio_contributor_data', 'bcd')
    ->fields('bcd', array('lastname', 'firstname', 'alt_form'))
    ->groupBy('lastname')
    ->groupBy('firstname')
    ->groupBy('alt_form')
    ->having('COUNT(*) > 1');

  if ($filter) {
    $filter = strtoupper($filter);
    $query->where("UPPER(SUBSTRING(lastname,1,1)) = :filter ", array(':filter' => $filter));
  }
  $result = $query->execute();

  $suspects = array();
  foreach ($result as $author) {
    $suspects[] = $author->lastname;
  }

  $db_result = db_query('SELECT bd.cid, bd.drupal_uid, bd.name, bd.lastname,
                                bd.firstname, bd.prefix, bd.suffix,
                                bd.initials, bd.affiliation, bd.md5, bd.literal,
                                COUNT(*) AS cnt
                            FROM {biblio_contributor} b
                                 LEFT JOIN {biblio_contributor_data} bd ON b.cid = bd.cid
                                 INNER JOIN {node} n on n.vid = b.vid
                            ' . $where_clause . '
                            GROUP BY bd.cid, bd.drupal_uid, bd.name, bd.lastname,
                                     bd.firstname, bd.prefix, bd.suffix,
                                     bd.initials, bd.affiliation, bd.md5, bd.literal
                            ORDER BY  lastname ASC, SUBSTRING(firstname,1,1) ASC,
                            initials ASC', array(':filter' => $filter));

  foreach ($db_result as $author) {
    if (array_search($author->lastname, $suspects) !== FALSE) {
      $author->suspect = TRUE;
    }
    $authors[] = $author;
  }

  return $authors;
}

/**
 *
 */
function _biblio_format_author_page($path, $filter, $authors) {
  $header_ext = $checkbox = '';
  $header = array();

  if (biblio_access('edit_author')) {
    if (!empty($filter)) {
      $header_ext = ' ' . t('whose last name begins with the letter') . ': ' . $filter;
    }
    $checkbox = array(
      '#title' => t('Highlight possible duplicates'),
      '#type'  => 'checkbox',
      '#id'    => 'biblio-highlight',
    );
    $checkbox = '<div class="biblio-alpha-line">' . drupal_render($checkbox) . '</div>';
    $header = array(array('data' => t('There are a total of @count authors in the database!header_ext.', array('@count' => count($authors), '!header_ext' => $header_ext)), 'align' => 'center', 'colspan' => 3));
  }

  $rows[] = array(
    array(
      'data' =>
      theme('biblio_alpha_line', array('type' => 'authors', 'current' => $filter, 'path' => $path)) . $checkbox,
      'colspan' => 3,
    ),
  );
  if (count($authors)) {
    for ($i = 0; $i < count($authors); $i += 3) {
      $rows[] = array(array('data' => _biblio_format_author($authors[$i])),
      array('data' => isset($authors[$i + 1]) ? _biblio_format_author($authors[$i + 1]) : ''),
      array('data' => isset($authors[$i + 2]) ? _biblio_format_author($authors[$i + 2]) : ''),
      );
    }
  }

  return array(
    '#theme' => 'table',
    '#header' => $header,
    '#rows' => $rows,
  );
}

/**
 * Helper function to format the authors and add edit links if required.
 */
function _biblio_format_author($author) {
  if ($author->literal) {
    $name = $author->name;
  }
  else {
    $name = $author->lastname;
    $name .= (!empty($author->firstname)) ? ', ' . drupal_substr($author->firstname, 0, 1) . '.' :
             (!empty($author->initials) ? ', ' . $author->initials : '');
  }

  $uri['path'] = variable_get('biblio_base', 'biblio');
  $uri['query']['f']['author'] = $author->cid;
  $uri['attributes'] = array();

  if (isset($author->drupal_uid) && $author->drupal_uid > 0) {
    $uri['attributes'] += array('class' => array('biblio-local-author'));
  }
  if (variable_get('biblio_links_target_new_window', NULL)) {
    $uri['attributes'] += array('target' => '_blank');
    $uri['html'] = TRUE;
  }

  $name = l(trim($name), $uri['path'], $uri);

  // $format = biblio_format_authors(array($author));
  $name .= ' (' . $author->cnt . ') ' . ((biblio_access('edit_author')) ? _biblio_author_edit_links($author) : '');
  if (biblio_access('edit_author') && isset($author->suspect)) {
    $name = '<div class="suspect">' . $name . '</div>';
  }

  return $name;
}

/**
 * Return an HTML link to an author edit page.
 *
 * If current_path() is within the admin section of biblio, the link will be to
 * the admin version of the edit page.
 *
 * @param object $author
 *   The author object.
 *
 * @return string
 *   The HTML link.
 */
function _biblio_author_edit_links($author) {
  // Cache $path.
  static $path = NULL;
  if (empty($path)) {
    // When the page is an admin page, use the admin author edit page. Usually:
    // admin/config/content/biblio/author
    // admin/config/content/biblio/author/list.
    $search_path = 'admin/config/content/biblio/';
    if (substr(current_path(), 0, 28) === $search_path) {
      $path = 'admin/config/content/biblio/author/';
    }
    // Otherwise, use the public author edit path. Usually: $base/authors:
    else {
      $path = variable_get('biblio_base', 'biblio') . '/authors/';
    }
  }
  return l(' [' . t('edit') . ']', $path . $author->cid . "/edit");
}

/**
 *
 */
function biblio_keyword_page() {
  $uri = drupal_parse_url(request_uri());
  // $uri['path'] = variable_get('biblio_base', 'biblio');.
  $filter = isset($uri['query']['f']['keyword']) ? $uri['query']['f']['keyword'] : '';
  $keywords = _biblio_get_keywords($filter);
  return _biblio_format_keyword_page($uri, $filter, $keywords);
}

/**
 *
 */
function _biblio_get_keywords($filter = NULL) {
  global $user;
  $keywords = array();
  $where = array();
  $where_clause = '';
  if ($filter) {
    $filter = strtoupper($filter);
    $where[] = "UPPER(SUBSTRING(word,1,1)) = :filter ";
    $header_ext = t(' (which start with the letter "@letter") ', array('@letter' => $filter));
  }
  else {
    $query_ext = NULL;
    $header_ext = NULL;
  }

  if ($user->uid != 1) {
    $where[] = 'n.status = 1 ';
  }//show only published entries to everyone except admin

  if (count($where)) {
    $where_clause = count($where) > 1 ? 'WHERE (' . implode(') AND (', $where) . ')' : 'WHERE ' . $where[0];
  }

  $db_result = db_query('SELECT bkd.kid, bkd.word, COUNT(*) AS cnt
                         FROM {biblio_keyword} bk
                         LEFT JOIN {biblio_keyword_data} bkd ON bkd.kid = bk.kid
                         INNER JOIN {node} n ON n.vid = bk.vid
                         ' . $where_clause . '
                         GROUP BY bkd.kid, bkd.word
                         ORDER BY  word ASC', array(':filter' => $filter));

  foreach ($db_result as $keyword) {
    $keywords[] = $keyword;
  }
  return $keywords;
}

/**
 *
 */
function _biblio_format_keyword_page($uri, $filter, $keywords) {
  $rows[] = array(array('data' => theme('biblio_alpha_line', array('type' => 'keywords', 'current' => $filter, 'path' => $uri['path'])), 'colspan' => 3));
  for ($i = 0; $i < count($keywords); $i += 3) {
    $rows[] = array(array('data' => _biblio_format_keyword($uri, $keywords[$i])),
    array('data' => isset($keywords[$i + 1]) ? _biblio_format_keyword($uri, $keywords[$i + 1]) : ''),
    array('data' => isset($keywords[$i + 2]) ? _biblio_format_keyword($uri, $keywords[$i + 2]) : ''),
    );
  }
  // $header = array(array('data' => t('There are a total of @count keywords !header_ext in the database',array('@count' => count($keywords), '!header_ext' => $header_ext)), 'align' =>'center', 'colspan' => 3));.
  $output = theme('table', array('rows' => $rows));
  return $output;
}

/**
 *
 */
function _biblio_format_keyword($uri, $keyword) {
  $base                         = variable_get('biblio_base', 'biblio');
  $uri['path']                  = $base;
  $uri['query']['f']['keyword'] = $keyword->kid;
  $format                       = l(trim($keyword->word), $base, $uri);
  $format                      .= ' (' . $keyword->cnt . ') ';
  $destination                  = drupal_get_destination();
  $path                         = (ord(substr($_GET['q'], -1)) > 97) ? $_GET['q'] . "/" : substr($_GET['q'], 0, -1);
  $edit_link                    = ' [' . l(t('edit'), $path . $keyword->kid . "/edit", array('query' => $destination)) . '] ';
  $format                      .= (user_access('administer biblio')) ? $edit_link : '';

  return $format;
}
