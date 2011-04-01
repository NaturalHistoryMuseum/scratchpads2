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
module_load_include('inc', 'biblio', 'biblio.pages');

/**
 * @param $element
 * @return unknown_type
 */
function theme_biblio_coin_button($element) {
  return '<a href="/biblio/regen_coins"><input type="button"  name="' . $element['#name'] . '" value="' . $element['#value'] . '"  /></a>';
}

/**
 * @param $openURL
 * @return unknown_type
 */
function theme_biblio_openurl($variables) {
  $open_url = $variables['openURL'];
  $openurl_image = variable_get('biblio_openurlimage', '');
  if ($openurl_image != '') {
    $output .= '<div class="biblio-openurl-image"><a href="' . $open_url . '"target=_blank><img border="0" src="' . $openurl_image . '"/></a></div>';
  }
  else {
    $output .= '<div class="biblio-openurl-text"><a href="' . $open_url . '" target=_blank>' . t('Find It Via OpenURL!') . '</a></div>';
  }
  return $output;
}

/**
 * @param $node
 * @return unknown_type
 */
function biblio_openURL($node) {
  global $user;
  $open_url = '';
  // Copyright:          Matthias Steffens <mailto:refbase@extracts.de> and the file's
  //                     original author.
  // Original Author:    Richard Karnesky <mailto:karnesky@gmail.com>  //
  // Adapted for biblio: Ron Jerome
  //  global $open_url_resolver; // these variables are defined in 'ini.inc.php'
  //  global $hostInstitutionAbbrevName;
  $open_url_resolver = !empty($user->data['biblio_baseopenurl']) ? $user->data['biblio_baseopenurl'] : '';
  if (empty($open_url_resolver)) $open_url_resolver = variable_get('biblio_baseopenurl', '');
  if (!empty($open_url_resolver)) {
    $co = biblio_contextObject($node);

    $sid = "biblio:" . variable_get('site_name', 'Drupal');
    $user_sid = !empty($user->data['biblio_openurl_sid']) ? $user->data['biblio_openurl_sid'] : '';
    $co["sid"] = !empty($user_sid) ? $user_sid : variable_get('biblio_openurl_sid', $sid);

    $open_url = $open_url_resolver;
    if (!preg_match("/\?/", $open_url_resolver)) {
      $open_url .= "?";
    }
    else {
      $open_url .= "&amp;";
    }
    $open_url .= "ctx_ver=Z39.88-2004";
    foreach ($co as $coKey => $coValue) {
      $coKey = preg_replace("/rft./", "", $coKey);
      $coKey = preg_replace("/au[0-9]*/", "au", $coKey);
      $open_url .= "&amp;" . $coKey . "=" . rawurlencode($coValue);
    }
  }
  return $open_url;
}


/**
 * DEPRECIATED! this was the original output format which is not to flexable it will be removed
 * TODO: remove this function
 * @param $node
 * @param $base
 * @param $style
 * @return unknown_type
 */
function theme_biblio_long($node, $base = 'biblio', $style = 'classic') {
  if (module_exists('popups')) {
     popups_add_popups();
  }
  if (empty ($node->biblio_coins))
    biblio_coins_generate($node);
  $output .= filter_xss($node->biblio_coins, array('span'));
  $layout = variable_get('biblio_node_layout', 'orig');
  if (variable_get('biblio_node_layout', 'orig') == 'ft' && strlen(trim($node->body)) && user_access('view full text')) {
    $output .= '<div class="biblio-head">' . theme('biblio_style', $node, $base, $style) . '</div>';
    $annotation_field = variable_get('biblio_annotations', 'none');
    if ($annotation_field != 'none' && $node-> $annotation_field) {
      $output .= '<div class="biblio-annotation">';
      $output .= filter_xss($node->$annotation_field, biblio_get_allowed_tags());
      $output .= '</div>';
    }
    $output .= check_markup($node->body);
    if (biblio_access('export', $node)) {
      $output .= theme('biblio_export_links', array('node' => $node));
    }
    return $output;
  }
  foreach ((array)$node->biblio_contributors as $auth) {
    if ($auth['auth_category'] == 1) {
      $authors[] = theme('biblio_author_link', array('author' => $auth));
    }
  }
  $authors = implode('; ', (array)$authors);
  $openurl_base = variable_get('biblio_baseopenurl', '');
  if ($openurl_base)
    $output .= theme('biblio_openurl', array('openURL' => biblio_openurl($node, $openurl_base)));
  $output .= '<div class="biblio_type"><h3>' . t("Publication Type") . ":</h3> $node->biblio_type_name</div>\n";
  $output .= '<div class="biblio_authors"><h3>' . t("Authors") . ':</h3> ' . $authors . "</div>\n";
  $output .= '<div class="biblio_source"><h3>' . t("Source") . ': </h3> ';
  $source = NULL;
  if ($node->biblio_secondary_title)
    $source .= check_plain($node->biblio_secondary_title);
  if ($node->biblio_publisher) {
    $source .= $source ? ", " : "";
    $source .= check_plain($node->biblio_publisher);
  }
  if ($node->biblio_volume) {
    $source .= $source ? ", " : "";
    $source .= t('Volume') . ' ' . check_plain($node->biblio_volume);
  }
  if ($node->biblio_issue) {
    $source .= $source ? ", " : "";
    $source .= t('Issue') . ' ' . check_plain($node->biblio_issue);
  }
  if ($node->biblio_number) {
    $source .= $source ? ", " : "";
    $source .= t('Number') . ' ' . check_plain($node->biblio_number);
  }
  if ($node->biblio_place_published) {
    $source .= $source ? ", " : "";
    $source .= check_plain($node->biblio_place_published);
  }
  if ($node->biblio_pages) {
    $source .= $source ? ", " : "";
    $source .= 'p.' . check_plain($node->biblio_pages);
  }
  if (isset ($node->biblio_year)) {
    $node->biblio_year = _biblio_text_year($node->biblio_year);
    $source .= ' (' . check_plain($node->biblio_year) . ')';
  }
  $output .= "$source</div>\n";
  if ($node->biblio_isbn)
    $output .= '<h3>' . t("ISBN") . ':</h3> ' . check_plain($node->biblio_isbn) . "\n";
  if ($node->biblio_call_number)
    $output .= '<h3>' . t("Call Number") . ':</h3> ' . check_plain($node->biblio_call_number) . "\n";
  if ($node->biblio_accession_number)
    $output .= '<h3>' . t("Accession Number") . ':</h3> ' . check_plain($node->biblio_accession_number) . "\n";
  if ($node->biblio_other_number)
    $output .= '<h3>' . t("Other Number") . ':</h3> ' . check_plain($node->biblio_other_number) . "\n";
  if ($node->biblio_url) {
    $attrib = (variable_get('biblio_links_target_new_window', FALSE)) ? array('target' => '_blank') : array();
    $output .= '<h3>' . t("URL") . ':</h3>' . l($node->biblio_url, $node->biblio_url, $attrib) . "\n";
  }
  if (!empty($node->biblio_keywords)) {
    $output .= '<h3>' . t("Keywords") . ':</h3> ' . _biblio_keyword_links( $node->biblio_keywords, $base) . "\n";
  }
  if ($node->biblio_abst_e)
    $output .= '<h3>' . t("Abstract") . ':</h3> ' . check_markup($node->biblio_abst_e) . "\n";
  if ($node->biblio_abst_f)
    $output .= '<p>' . check_markup($node->biblio_abst_f) . "\n";
  if ($node->biblio_notes)
    $output .= '<h3>' . t("Notes") . ': </h3>' . check_markup($node->biblio_notes) . "\n";
  if ( strlen(trim($node->body)) && user_access('view full text') ) {
       $output .= '<h3>' . t('Full Text') . ':</h3>' . check_markup($node->body);
  }

  return $output;
}
/**
 * @param $node
 * @param $base
 * @param $teaser
 * @return unknown_type
 */
function theme_biblio_tabular($variables) {
  $node = $variables['node'];
  $base = $variables['base'];
  static $citeproc;

  if (module_exists('popups')) {
     popups_add_popups();
  }
  $tid = $node->biblio_type;
  $fields = _biblio_get_field_information($node->biblio_type, TRUE);
  $rows[] = array(
    array('data' => t('Title'), 'class' => array('biblio-row-title')),
    array('data' => filter_xss($node->title, biblio_get_allowed_tags()))
  );
  $rows[] = array(
    array('data' => t('Publication Type'), 'class' => array('biblio-row-title')),
    array('data' => $node->biblio_type_name)
  );

  if ($node->biblio_url) {
    $attrib = (variable_get('biblio_links_target_new_window', FALSE)) ? array('target' => '_blank') : array();
    $node->biblio_url = l($node->biblio_url, $node->biblio_url, $attrib);
  }
  if ($node->biblio_doi) {
    $doi_url = '';
    $attrib = (variable_get('biblio_links_target_new_window', FALSE)) ? array('target' => '_blank') : array();
    if ( ($doi_start = strpos($node->biblio_doi, '10.')) !== FALSE) {
      $doi = substr($node->biblio_doi, $doi_start);
      $doi_url .= 'http://dx.doi.org/' . $doi;
    }
    $node->biblio_doi = l($node->biblio_doi, $doi_url, $attrib);
  }

  foreach ($fields as $key => $row) {
    // handling the contributor categories like any other field orders them correctly by weight
    if ($row['type'] == 'contrib_widget' && $node->biblio_contributors->has_category($row['fid']) ) {
      $data = biblio_format_authors($node->biblio_contributors->get_category($row['fid']));
    }
    elseif (empty ($node->$row['name']) || $row['name'] == 'biblio_coins') {
      continue;
    }
    else {
      switch ($row['name']) {
        case 'biblio_keywords' :
         $data = _biblio_keyword_links($node->$row['name'], $base);
          break;
        case 'biblio_url' :
        case 'biblio_doi' :
          // check_plain is not need on these since they have gone through
          // the l() function which does a check_plain
          $data = $node-> $row['name'];
          break;
        default:
          if ($row['type'] == 'textarea') {
//FIXME where is format stored now???
            $data = check_markup($node->$row['name']);
          }
          else {
            $data = check_plain($node->$row['name']);
          }
      }
    }
    $rows[] = array(
      array(
        'data' => t($row['title']),
        'class' => array('biblio-row-title')
      ),
      array(
        'data' => $data
      )
    );
  }
//FIXME figure out where the node body stuff is now...
//  if (strlen(trim($node->body)) && user_access('view full text')) {
//    $rows[] = array(
//        array('data' => t('Full Text'),  'valign' => 'top'),
//        array('data' =>  check_markup($node->body, $node->format, FALSE))
//    );
//
//  }
  $output = '<div id="biblio-node">';
  $output .= filter_xss($node->biblio_coins, array('span'));
  $header = array();
  $output .= theme('table', array('header' => $header, 'rows' => $rows));
  $output .= '</div>';

  return $output;
}

function theme_biblio_authors($contributors, $style = 'classic', $cat = 1, $inline = FALSE, $glue = ', ') {
  if (empty($contributors)) return; // t('No author information available');
  $author_style_function = '_' . $style . '_format_author';
  $author_links = variable_get('biblio_author_links', 1);
  $base  = variable_get('biblio_base', 'biblio');
  foreach ($contributors as $key => $author) {
    if (!empty($author['name']) && !isset($author['lastname'])) {
      $author = biblio_parse_author($author, $cat); // this is needed for form preview to fill in all fields
    }
    if (strlen($author['name'])) {
      $format = $author_style_function($author);
      if ($author_links) $format = theme('biblio_author_link', array('author' => $format, 'authorID' => $author['cid'], 'base' => $base, 'inline' => $inline));
      // add the auth_type as css id to allow later formatting
      $author_array[] = '<span id="' . $author['auth_type'] . '">' . $format . '</span>';
    }
  }
  if (empty($author_array)) return '';

  $output  = '<span class="biblio_authors">';
  $output .= implode($glue, $author_array);
  $output .= '</span>';
  return $output;
}
function _biblio_get_latin1_regex() {
  $alnum = "[:alnum:]ÄÅÁÀÂÃÇÉÈÊËÑÖØÓÒÔÕÜÚÙÛÍÌÎÏÆäåáàâãçéèêëñöøóòôõüúùûíìîïæÿß";

  // Matches ISO-8859-1 letters:
  $alpha = "[:alpha:]ÄÅÁÀÂÃÇÉÈÊËÑÖØÓÒÔÕÜÚÙÛÍÌÎÏÆäåáàâãçéèêëñöøóòôõüúùûíìîïæÿß";

  // Matches ISO-8859-1 control characters:
  $cntrl = "[:cntrl:]";

  // Matches ISO-8859-1 dashes & hyphens:
  $dash = "-–";

  // Matches ISO-8859-1 digits:
  $digit = "[\d]";

  // Matches ISO-8859-1 printing characters (excluding space):
  $graph = "[:graph:]ÄÅÁÀÂÃÇÉÈÊËÑÖØÓÒÔÕÜÚÙÛÍÌÎÏÆäåáàâãçéèêëñöøóòôõüúùûíìîïæÿß";

  // Matches ISO-8859-1 lower case letters:
  $lower = "[:lower:]äåáàâãçéèêëñöøóòôõüúùûíìîïæÿß";

  // Matches ISO-8859-1 printing characters (including space):
  $print = "[:print:]ÄÅÁÀÂÃÇÉÈÊËÑÖØÓÒÔÕÜÚÙÛÍÌÎÏÆäåáàâãçéèêëñöøóòôõüúùûíìîïæÿß";

  // Matches ISO-8859-1 punctuation:
  $punct = "[:punct:]";

  // Matches ISO-8859-1 whitespace (separating characters with no visual representation):
  $space = "[\s]";

  // Matches ISO-8859-1 upper case letters:
  $upper = "[:upper:]ÄÅÁÀÂÃÇÉÈÊËÑÖØÓÒÔÕÜÚÙÛÍÌÎÏÆ";

  // Matches ISO-8859-1 "word" characters:
  $word = "_[:alnum:]ÄÅÁÀÂÃÇÉÈÊËÑÖØÓÒÔÕÜÚÙÛÍÌÎÏÆäåáàâãçéèêëñöøóòôõüúùûíìîïæÿß";

  // Defines the PCRE pattern modifier(s) to be used in conjunction with the above variables:
  // More info: <http://www.php.net/manual/en/reference.pcre.pattern.modifiers.php>
  $patternModifiers = "";

  return array($alnum, $alpha, $cntrl, $dash, $digit, $graph, $lower,
       $print, $punct, $space, $upper, $word, $patternModifiers);

}
/*
 * Helper function for theme_biblio_format_authors() and theme_biblio_page_number()
 */
function _biblio_get_utf8_regex() {

    // Matches Unicode letters & digits:
  $alnum = "\p{Ll}\p{Lu}\p{Lt}\p{Lo}\p{Nd}"; // Unicode-aware equivalent of "[:alnum:]"

  // Matches Unicode letters:
  $alpha = "\p{Ll}\p{Lu}\p{Lt}\p{Lo}"; // Unicode-aware equivalent of "[:alpha:]"

  // Matches Unicode control codes & characters not in other categories:
  $cntrl = "\p{C}"; // Unicode-aware equivalent of "[:cntrl:]"

  // Matches Unicode dashes & hyphens:
  $dash = "\p{Pd}";

  // Matches Unicode digits:
  $digit = "\p{Nd}"; // Unicode-aware equivalent of "[:digit:]"

  // Matches Unicode printing characters (excluding space):
  $graph = "^\p{C}\t\n\f\r\p{Z}"; // Unicode-aware equivalent of "[:graph:]"

  // Matches Unicode lower case letters:
  $lower = "\p{Ll}\p{M}"; // Unicode-aware equivalent of "[:lower:]"

  // Matches Unicode printing characters (including space):
  $print = "\P{C}"; // same as "^\p{C}", Unicode-aware equivalent of "[:print:]"

  // Matches Unicode punctuation (printing characters excluding letters & digits):
  $punct = "\p{P}"; // Unicode-aware equivalent of "[:punct:]"

  // Matches Unicode whitespace (separating characters with no visual representation):
  $space = "\t\n\f\r\p{Z}"; // Unicode-aware equivalent of "[:space:]"

  // Matches Unicode upper case letters:
  $upper = "\p{Lu}\p{Lt}"; // Unicode-aware equivalent of "[:upper:]"

  // Matches Unicode "word" characters:
  $word = "_\p{Ll}\p{Lu}\p{Lt}\p{Lo}\p{Nd}"; // Unicode-aware equivalent of "[:word:]" (or "[:alnum:]" plus "_")

  // Defines the PCRE pattern modifier(s) to be used in conjunction with the above variables:
  // More info: <http://www.php.net/manual/en/reference.pcre.pattern.modifiers.php>
  $patternModifiers = "u"; // the "u" (PCRE_UTF8) pattern modifier causes PHP/PCRE to treat pattern strings as UTF-8

  return array($alnum, $alpha, $cntrl, $dash, $digit, $graph, $lower,
       $print, $punct, $space, $upper, $word, $patternModifiers);
}

function _biblio_get_regex_patterns() {
  // Checks if PCRE is compiled with UTF-8 and Unicode support
  if (!@preg_match('/\pL/u', 'a')) {
    // probably a broken PCRE library
    return _biblio_get_latin1_regex();
  }
  else {
    // Unicode safe filter for the value
    return _biblio_get_utf8_regex();
  }
}

function biblio_format_authors($authors) {
  if (module_exists('biblio_citeproc')) {
    static $auth_proc;
    if (!isset($auth_proc)) {
      module_load_include('inc', 'biblio_citeproc', 'CSL');
      $csl = '<name form="long" name-as-sort-order="all" sort-separator=", " initialize-with="" delimiter=", " />';
      $csl_doc = new DOMDocument();
      $csl_doc->loadXML($csl);
      $auth_proc = new csl_rendering_element($csl_doc);
    }
    $data = $auth_proc->render($authors);
  }
  else {
    $style_name = biblio_get_style();
    $style_function = "biblio_style_$style_name" . "_author_options";
    if (!function_exists($style_function)) {
      module_load_include('inc', 'biblio', "/styles/biblio_style_$style_name");
    }
    $author_options = $style_function();
    $author_options['numberOfAuthorsTriggeringEtAl'] = 100; //set really high so we see all authors
    $data = theme('biblio_format_authors', array('contributors' => $authors, 'options' => $author_options));
  }

  return $data;
}
function theme_biblio_format_authors($variables) {
  $contributors = $variables['contributors'];
  $options      = $variables['options'];
  $inline       = $variables['inline'];

  if (empty($contributors)) return;
  list($alnum, $alpha, $cntrl, $dash, $digit, $graph, $lower,
       $print, $punct, $space, $upper, $word, $patternModifiers) = _biblio_get_regex_patterns();
  $base  = variable_get('biblio_base', 'biblio');
  $author_links = variable_get('biblio_author_links', 1);

  $authorCount = count($contributors); // check how many authors we have to deal with
  $output = ""; // this variable will hold the final author string
  $includeStringAfterFirstAuthor = FALSE;

  if (empty($options['numberOfAuthorsTriggeringEtAl']))  $options['numberOfAuthorsTriggeringEtAl'] = $authorCount;

  if (empty($options['includeNumberOfAuthors']))  $options['includeNumberOfAuthors'] = $authorCount;

  foreach ($contributors as $rank => $author) {
    if (empty($author['name'])) continue;
    if (!isset($author['lastname'])) {
      module_load_include('inc', 'biblio', 'biblio.contributors');
      $author = biblio_parse_author($author, $author['auth_type']); // this is needed for form preview to fill in all fields
    }

    if (!empty($author['firstname'])) {
      if ($options['shortenGivenNames']) {// if we're supposed to abbreviate given names
        // within initials, reduce all full first names (-> defined by a starting uppercase character, followed by one ore more lowercase characters)
        // to initials, i.e., only retain their first character
        $author['firstname'] = preg_replace("/([$upper])[$lower]+/$patternModifiers", '\\1', $author['firstname']);
        //$author['firstname'] = drupal_substr($author['firstname'], 0, 1);
        // the next line caused extra betweenInitialsDelim to appear in some circumstances
        //$author['firstname'] = preg_replace("/($space|$dash)?/$patternModifier", $options['betweenInitialsDelim'], $author['firstname']);
      }
    }
    if (isset($author['initials'])) {
      // within initials, remove any dots:
      $author['initials'] = preg_replace("/([$upper])\.+/$patternModifiers", "\\1", $author['initials']);
      //$author['initials'] = str_replace('.', '',  $author['initials']);

      // within initials, remove any spaces *between* initials:
      $author['initials'] = preg_replace("/(?<=[-$upper]) +(?=[-$upper])/$patternModifiers", "", $author['initials']);
      //$author['initials'] = str_replace(' ', '',  $author['initials']);

      // within initials, add a space after a hyphen, but only if ...
      if (preg_match('/ $/', $options['betweenInitialsDelim'])) { // ... the delimiter that separates initials ends with a space
        $author['initials'] = preg_replace("/-(?=[$upper])/$patternModifiers", "- ", $author['initials']);
      }
      // then, separate initials with the specified delimiter:
      $delim = $options['betweenInitialsDelim'];
      $author['initials'] = preg_replace("/([$upper])(?=[^$lower]+|$)/$patternModifiers", "\\1$delim", $author['initials']);

      $shortenInitials = (isset($options['numberOfInitialsToKeep'])) ? $options['numberOfInitialsToKeep'] : FALSE;
      if ($shortenInitials) $author['initials'] = drupal_substr($author['initials'], 0, $shortenInitials);

      if ($options['shortenGivenNames'] && !empty($author['firstname'])) {
        $author['firstname'] = $author['firstname'] . $options['betweenInitialsDelim'] . $author['initials'];
        if ($shortenInitials) $author['firstname'] = drupal_substr($author['firstname'], 0, $shortenInitials);
      }
      elseif (!empty($author['firstname'])) {
        $author['firstname'] = $author['firstname'] . ' ' . $author['initials'];
      }
      elseif (empty($author['firstname'])) {
        $author['firstname'] = $author['initials'];
      }
    }

    // if there is a surname prefix like "van", "von" etc, stick it back before the last name.
    if (!empty($author['prefix'])) $author['lastname'] = $author['prefix'] . ' ' . $author['lastname'];
    if (!empty($author['suffix'])) $author['lastname'] = $author['lastname'] . ', ' . $author['suffix'];

    if (!empty($author['firstname'])) {
      if ($rank == 0) { // -> first author
        if ($options['initialsBeforeAuthorFirstAuthor']) {
          $author['name'] = $author['firstname'] . $options['AuthorsInitialsDelimFirstAuthor'] . $author['lastname'];
        }
        else{
          $author['name'] = $author['lastname'] . $options['AuthorsInitialsDelimFirstAuthor'] . $author['firstname'];
        }
      }
      else { // $rank > 0 // -> all authors except the first one
        if ($options['initialsBeforeAuthorStandard']) {
          $author['name'] = $author['firstname'] . $options['AuthorsInitialsDelimStandard'] . $author['lastname'];
        }
        else{
          $author['name'] = $author['lastname'] . $options['AuthorsInitialsDelimStandard'] . $author['firstname'];
        }
      }
    }
    else {
      $author['name'] = $author['lastname'];
    }
    if ($author_links) {
      $author['name'] = theme('biblio_author_link', array('author' => $author));
    }
    else {
      $author['name'] = check_plain($author['name']);
    }
    // append this author to the final author string:
    if (($rank == 0) OR ($rank + 1) < $authorCount) {// -> first author, or (for multiple authors) all authors except the last one
      if ($rank == 0) { // -> first author
        $output .= $author['name'];
      }
      else {// -> for multiple authors, all authors except the first or the last one
        $output .= $options['BetweenAuthorsDelimStandard'] . $author['name'];
      }
      // we'll append the string in '$customStringAfterFirstAuthors' to the number of authors given in '$includeNumberOfAuthors' if the total number of authors is greater than the number given in '$numberOfAuthorsTriggeringEtAl':
      if ((($rank + 1) == $options['includeNumberOfAuthors']) AND ($authorCount > $options['numberOfAuthorsTriggeringEtAl'])) {
        if (preg_match("/__NUMBER_OF_AUTHORS__/", $options['customStringAfterFirstAuthors'])) {
          $customStringAfterFirstAuthors = preg_replace("/__NUMBER_OF_AUTHORS__/", ($authorCount - $options['includeNumberOfAuthors']), $options['customStringAfterFirstAuthors']); // resolve placeholder
        }
        $includeStringAfterFirstAuthor = TRUE;
        break;
      }
    }
    elseif (($authorCount > 1) AND (($rank + 1) == $authorCount)) {// -> last author (if multiple authors)
      $output .= $options['BetweenAuthorsDelimLastAuthor'] . $author['name'];
    }
  }

  // do some final clean up:
  //if ($options['encodeHTML'])
  //$output = encodeHTML($output); // HTML encode higher ASCII characters within the newly arranged author contents

  if ($includeStringAfterFirstAuthor) {
    $output .= $options['customStringAfterFirstAuthors']; // the custom string won't get HTML encoded so that it's possible to include HTML tags (such as '<i>') within the string
  }
  $output = preg_replace("/  +/", " ", $output); // remove double spaces (which occur e.g., when both, $betweenInitialsDelim & $newAuthorsInitialsDelim..., end with a space)
  $output = preg_replace("/ +([,.;:?!()]|$)/", "\\1", $output); // remove excess spaces before [,.;:?!()] and from the end of the author string

  return $output;
}

function theme_biblio_author_link($variables) {
  $author = $variables['author'];
//  $aid    = $variables['authorID'];
//  $base   = $variables['base'];
//  $inline = $variables['inline'];

  $uri = drupal_parse_url(request_uri());
  $uri['path'] = variable_get('biblio_base', 'biblio');

  $uri['query']['f']['author'] = $author['cid'];

//  if ($inline) {
//    $options['query']['inline'] = 'TRUE';
//  }

  return l(trim($author['name']), $uri['path'], $uri );

}

// Format page information:
//
// NOTES: - this function (and refbase in general) assumes following rules for the original formatting of page information in '$orig_page_info':
//          - single-page items are given as a page range with identical start & end numbers (e.g. "127-127")
//          - multi-page items are given as a page range where the end number is greater than the start number (e.g. "127-132")
//          - for multi-page items where only the start page is known, a hyphen is appended to the start page (e.g. "127-")
//          - total number of pages are given with a "pp" suffix (e.g. "498 pp"), see TODO
//          - the given page info is left as is if it does not match any of the above rules (e.g. a single page number is ambiguous since it
//            could mean a single page or the total number of pages)
//        - the function attempts to deal with page locators that contain letters (e.g. "A1 - A3" or "4a-4c") but, ATM, locator parts (e.g. "A1")
//          must contain at least one digit character & must not contain any whitespace
//
// TODO:  - should we only use Unicode-aware regex expressions (i.e. always use '$space', '$digit' or '$word' instead of ' ', '\d' or '\w', etc)?
//        - recognize & process total number of pages
//        - for '$shortenPageRangeEnd=TRUE', add support for page locators that contain letters (e.g. "A1 - A3" or "4a-4c")
function theme_biblio_page_number($variables) {
  $orig_page_info         = $variables['orig_page_info'];
  $page_range_delim       = $variables['page_range_delim'];
  $single_page_prefix     = $variables['single_page_prefix'];
  $page_range_prefix      = $variables['page_range_prefix'];
  $total_pages_prefix     = $variables['total_pages_prefix'];
  $single_page_suffix     = $variables['single_page_suffix'];
  $page_range_suffix      = $variables['page_range_prefix'];
  $total_pages_suffix     = $variables['total_pages_prefix'];
  $shorten_page_range_end = $variables['single_page_suffix'];

  list($alnum, $alpha, $cntrl, $dash, $digit, $graph, $lower,
       $print, $punct, $space, $upper, $word, $patternModifiers) = _biblio_get_regex_patterns();

  // Check original page info for any recognized page locators, and process them appropriately:
  if (preg_match("/\w*\d+\w* *[$dash]+ *(?:\w*\d+\w*)?/$patternModifiers", $orig_page_info)) {// the original page info contains a page range (like: "127-127", "127-132", "A1 - A3", "4a-4c", or "127-" if only start page given)
    // Remove any whitespace around dashes or hyphens that indicate a page range:
    $orig_page_info = preg_replace("/(\w*\d+\w*) *([$dash]+) *(\w*\d+\w*)?(?=[^\w\d]|$)/$patternModifiers", "\\1\\2\\3", $orig_page_info);

    // Split original page info into its functional parts:
    // NOTE: ATM, we simply split on any whitespace characters, then process all parts with page ranges
    //       (this will also reduce runs of whitespace to a single space)
    $partsArray = preg_split("/ +/", $orig_page_info);
    $partsCount = count($partsArray);

    for ($i=0; $i < $partsCount; $i++) {
      // Format parts with page ranges:
      // - single-page item:
      if (preg_match("/(\w*\d+\w*)[$dash]+\\1(?=[^\w\d]|$)/$patternModifiers", $partsArray[$i])) // this part contains a page range with identical start & end numbers (like: "127-127")
      $partsArray[$i] = preg_replace("/(\w*\d+\w*)[$dash]+\\1(?=[^\w\d]|$)/$patternModifiers", $single_page_prefix . "\\1" . $single_page_suffix, $partsArray[$i]);

      // - multi-page item:
      elseif (preg_match("/\w*\d+\w*[$dash]+(?:\w*\d+\w*)?(?=[^\w\d]|$)/$patternModifiers", $partsArray[$i])) {// this part contains a page range (like: "127-132", or "127-" if only start page given)
        // In case of '$shorten_page_range_end=TRUE', we abbreviate ending page numbers so that digits aren't repeated unnecessarily:
        if ($shorten_page_range_end AND preg_match("/\d+[$dash]+\d+/$patternModifiers", $partsArray[$i])) {// ATM, only digit-only page locators (like: "127-132") are supported
          // NOTE: the logic of this 'if' clause doesn't work if the original page info contains something like "173-190; 195-195" (where, for the first page range, '$endPage' would be "190;" and not "190")
          list($startPage, $endPage) = preg_split("/[$dash]+/$patternModifiers", $partsArray[$i]);

          $countStartPage = strlen($startPage);
          $countEndPage = strlen($endPage);

          if (($countStartPage == $countEndPage) AND ($startPage < $endPage)) {
            for ($j=0; $j < $countStartPage; $j++) {
              if (preg_match("/^" . substr($startPage, $j, 1) . "/", $endPage)) {// if the ending page number has a digit that's identical to the starting page number (at the same digit offset)
                $endPage = substr($endPage, 1); // remove the first digit from the remaining ending page number
              }
              else {
                break;
              }
            }
          }

          $partsArray[$i] = $page_range_prefix . $startPage . $page_range_delim . $endPage . $page_range_suffix;
        }
        else {// don't abbreviate ending page numbers:
          $partsArray[$i] = preg_replace("/(\w*\d+\w*)[$dash]+(\w*\d+\w*)?(?=[^\w\d]|$)/$patternModifiers", $page_range_prefix . "\\1" . $page_range_delim . "\\2" . $page_range_suffix, $partsArray[$i]);
        }
      }
    }

    $newPageInfo = join(" ", $partsArray); // merge again all parts
  }
  else {
    $newPageInfo = $orig_page_info; // page info is ambiguous, so we don't mess with it
  }

  return $newPageInfo;
}


/**
 * Applies a "style" function to a single node.
 *
 * @param $node A node
 * @param $base The base url for biblio (defaults to /biblio)
 * @param $style_name The name of the style to apply
 * @param $inline "inline" mode returns the raw HTML rather than letting drupal render the whole page.
 * @return A string containing the styled (HTML) node
 */
function theme_biblio_style($variables ) {
  $node       = $variables['node'];
  $base       = $variables['base'];
  $style_name = $variables['style_name'];
  $inline     = $variables['inline'];

  module_load_include('inc', 'biblio', "/styles/biblio_style_$style_name");

  $style_function = "biblio_style_$style_name";

  $styled_node = $style_function ($node, $base, $inline);

  if (empty ($node->biblio_coins)) {
    biblio_coins_generate($node);
  }
  return ($styled_node . filter_xss($node->biblio_coins, array('span')));
}

/**
 * @param $node
 * @param $base
 * @param $style
 * @param $inline
 * @return unknown_type
 */
function theme_biblio_entry($variables) {
  $node       = $variables['node'];
  $base       = $variables['base'];
  $style_name = $variables['style_name'];
  $inline     = $variables['inline'];

  $output  = "\n" . '<div class="biblio-entry">' . "\n" ;
  $output  .= '<div class="biblio-style-' . $style_name . '">' . "\n" ;
  if (!$node->status) {
    $output .= '<div id="node-' . $node->nid . '" class="node node-unpublished">';
  }

  // first add the styled entry...
  $output .= theme('biblio_style', array('node' => $node, 'base' => $base, 'style_name' => $style_name, 'inline' => $inline));

  // now add the various links
//  if ($node->biblio_abst_e) {
//    $output .= '<span class="biblio-abstract-link">';
//    $output .= l(" Abstract", "node/$node->nid") . "\n";
//    $output .= '</span>';
//  }
  $annotation_field = variable_get('biblio_annotations', 'none');
  if ($annotation_field != 'none' && $node-> $annotation_field) {
    $output .= '<div class="biblio-annotation">';
    $output .= filter_xss($node->$annotation_field, biblio_get_allowed_tags());
    $output .= '</div>';
  }
  $openurl_base = variable_get('biblio_baseopenurl', '');

  if ($openurl_base) {
    $output .= theme('biblio_openurl', array('openURL' => biblio_openurl($node)));
  }

  if (biblio_access('export', $node)) {
    $base = variable_get('biblio_base', 'biblio');
    $output .= theme('biblio_export_links', array('node' => $node));
  }

  if (biblio_access('download', $node)) {
    // add links to attached files (if any)
    $output .= theme('biblio_download_links', array('node' => $node));
  }
  if (!$node->status) {
    $output .= '</div>';
  }

  $output .= "\n</div></div>";

  return $output;
}

/**
 * @param $form
 * @return unknown_type
 */
function theme_biblio_filters($variables) {
  $form = $variables['form'];
  $output = '';
  if (isset($form['current'])) {
    $output .= '<ul>';
    foreach (element_children($form['current']) as $key) {
      $output .= '<li>' . drupal_render($form['current'][$key]) . '</li>';
    }
    $output .= '</ul>';
  }
  $output .= '<dl class="multiselect">' . (isset($form['current']) ? '<dt><em>' . t('and') . '</em> ' . t('where') . '</dt>' : '') . '<dd class="a">';
  foreach (element_children($form['filter']) as $key) {
    $output .= drupal_render($form['filter'][$key]);
  }
  $output .= '</dd>';
  $output .= '<dt>' . t('is') . '</dt><dd class="b">';
  foreach (element_children($form['status']) as $key) {
    $output .= drupal_render($form['status'][$key]);
  }
  $output .= '</dd>';
  $output .= '</dl>';
  $output .= '<div class="container-inline" id="node-buttons">' . drupal_render($form['buttons']) . '</div>';
  $output .= '<br class="clear" />';
  return $output;
}

/**
 * @param $form
 * @return unknown_type
 */
function theme_biblio_form_filter($variables) {
 $form = $variables['form'];

  $output .= '<div id="biblio-admin-filter">';
  $output .= drupal_render($form['filters']);
  $output .= '</div>';
  $output .= drupal_render($form);
  return $output;
}

/**
 * @param $form
 * @return unknown_type
 */
function theme_biblio_admin_types_edit_form($variables) {
  $form = $variables['form'];
  drupal_add_tabledrag('field-table', 'order', 'sibling', 'weight', NULL, NULL, FALSE);
  $base = variable_get('biblio_base', 'biblio');
  $tid = (!empty ($form['#parameters'][2])) ? $form['#parameters'][2] : FALSE;
  //  drupal_set_title($form['type_name'] ? $form['type_name']['#value'] : t('Common'));

  // build the table with all the fields if no $tid is given, or only the common
  // and customized fields if $tid is given
  $conf_table = array();
  foreach (element_children($form['configured_flds']) as $fld) {
    $form['configured_flds'][$fld]['weight']['#attributes']['class'] = array('weight');

    $conf_row = array();
    $conf_row[] = array('data' => drupal_render($form['configured_flds'][$fld]['name']));
    $conf_row[] = array('data' => drupal_render($form['configured_flds'][$fld]['title']));
    if (isset($form['configured_flds'][$fld]['auth_type'])) {
      $form['configured_flds'][$fld]['hint']['#size'] = 15;
      $conf_row[] = array('data' => drupal_render($form['configured_flds'][$fld]['hint']));
      $conf_row[] = array('data' => drupal_render($form['configured_flds'][$fld]['auth_type']));
    }
    else {
      $form['configured_flds'][$fld]['hint']['#size'] = ($tid ? 38: 36);
      $conf_row[] = array('data' => drupal_render($form['configured_flds'][$fld]['hint']), 'colspan' => array(2));
    }
    foreach (element_children($form['configured_flds'][$fld]['checkboxes']) as $oid) {
      if (is_array($form['configured_flds'][$fld]['checkboxes'])) {
        $conf_row[] = array(
          'data' => drupal_render($form['configured_flds'][$fld]['checkboxes'][$oid]),
          'title' => $oid);
      }
    }
    $conf_row[] = array('data' => drupal_render($form['configured_flds'][$fld]['weight']));
    $conf_table[] = array('data' => $conf_row, 'class' => array('draggable'));
  }
  if ($tid) {
    $header = array(t('Field Name'), t('Title'), t('Hint'), '', t('Visible'), t('Required'), t('Weight'));
  }
  else {
    $header = array(t('Field Name'), t('Default Title'), t('Hint'), '', t('Common'), t('Required'), t('Autocomplete'), t('Weight'));
  }
  $output = '<h2>' . drupal_render($form['top_message']) . '</h2>';
  $output .= drupal_render($form['help']);
  $output .= drupal_render($form['type_name']);
  $output .= drupal_render($form['options']);

  $output .= theme('table', array('header' => $header, 'rows' => $conf_table, 'attributes' => array('id' => 'field-table')));
  $output .= '<p><center>' . drupal_render($form['submit']) . '</center></p>';
  $output .= drupal_render_children($form);
  return $output;
}

function theme_biblio_download_links($variables) {
  $node = $variables['node'];
  $files = '';
  if (!empty ($node->files) && count($node->files) > 0 && user_access('view uploaded files')) {
    $files .= '<span class="biblio_file_links">';
    $files .= '&nbsp;' . t('Download') . ':&nbsp;';
    $file_count = 0;
    foreach ($node->files as $file) {
      if ($file->list) {
        $file_count++;
        $href = file_create_url($file->uri);
        $text = $file->description ? $file->description : $file->filename;
        $files .= l($text, $href) . '&nbsp;(' . format_size($file->filesize) . ')';
      }
    }
    $files .= '</span>';
  }
  if (module_exists('filefield')) { // now lets get any CCK FileField files...
    $fields = filefield_get_field_list('biblio');
    foreach ($fields as $field_name => $field) {
      if (filefield_view_access($field_name)) {
        $field_files = filefield_get_node_files($node, $field_name);
        if ($field_files) {
          foreach ($field_files as $file) {
            if ($file['fid']) {
              $files .= theme('filefield_file', $file);
            }
          }
        }
      }
    }
  }

  return $files;
}

/**
 * Creates a group of links for the various export functions
 * @param $nid the node id to export (if omitted, all nodes in the current view will be exported
 * @return an un-ordered list of class "biblio-export-buttons"
 */
function theme_biblio_export_links($variables) {
  global $pager_total_items;
  $node = $variables['node'];
  $links = array();
  $output = '';

  if (biblio_access('export', $node)) {
    $show_link = variable_get('biblio_lookup_links', array('google' => TRUE));
    $lookup_links = module_invoke_all('biblio_lookup_link', $node);
    if ($show_link['google'] && !empty($node)) {
      $lookup_links['biblio_google_scholar'] = theme_google_scholar_link($node);
    }
    $nid = (isset($node->nid)) ? $node->nid : NULL;
    $export_links = module_invoke_all('biblio_export_link', $nid);
    $links = array_merge($lookup_links, $export_links);
  }
  if (empty($node) && !empty($links)) {
    $output = t('Export @count results', array('@count' => $pager_total_items[0])) . ': ';
  }
  return $output . theme('links', array('links' => $links, 'attributes' => array('class' => "biblio-export-buttons")));

}


function theme_google_scholar_link($node) {
  $query = array();

  $query['btnG'] = 'Search+Scholar';
  $query['as_q'] = '"' . str_replace(array(' ', '(', ')'), array('+'), $node->title) . '"'; // as_q = all the words
  if (isset($node->biblio_contributors->data[0]['lastname'])) {
    $query['as_sauthors'] = $node->biblio_contributors->data[0]['lastname'];
  }
  $query['as_occt'] = 'any';
  $query['as_epq'] = ''; // exact phrase
  $query['as_oq'] = ''; // at least one of the words
  $query['as_eq'] = ''; // without the words
  $query['as_publication'] = ''; // published in
  $query['as_ylo'] = ''; // lower date in date range
  $query['as_yhi'] = ''; // upper date in date range
  $query['as_sdtAAP'] = 1; //Search articles in all subject areas
  $query['as_sdtp'] = 1; //include patents

  $attrs = array('title' => t("Click to search Google Scholar for this entry"));
  if (variable_get('biblio_links_target_new_window', null)){
    $attrs = array_merge($attrs, array('target'=>'_blank'));
  }

  return array(
        'title'      => t('Google Scholar'),
        'href'       => 'http://scholar.google.com/scholar',
        'attributes' => $attrs,
        'query'      => $query,
  );
}

/**
 * @param $form
 * @return unknown_type
 */
function theme_biblio_contributors($variables) {
  $form = $variables['form'];
  $rows = array();
  $headers = array('', t('Name'), t('Category'), t('Role'), t('Weight'));
  drupal_add_tabledrag($form['#id'], 'order', 'sibling', 'rank');

  foreach (element_children($form) as $key) {
    // No need to print the field title every time.
//    unset($form[$key]['name']['#title'], $form[$key]['auth_type']['#title'], $form[$key]['auth_category']['#title']);
    // Add class to group weight fields for drag and drop.
    $form[$key]['rank']['#attributes']['class'] = array('rank');

    // Build the table row.
    $row = array('');
    $row[] = array('data' => drupal_render($form[$key]['name']),
                   'class' => array('biblio-contributor'));
    $row[] = array('data' => drupal_render($form[$key]['auth_category']),
                     'class' => array('biblio-contributor-category'));
    $row[] = array('data' => drupal_render($form[$key]['auth_type']),
                     'class' => array('biblio-contributor-type'));
    $row[] = drupal_render($form[$key]['rank']);
    $rows[] = array('data' => $row, 'class' => array('draggable'));
  }
  $output = theme('table', array('header' => $headers, 'rows' => $rows, 'attributes' => array('id' => $form['#id'])));
  //$output .= drupal_render_children($form);
  return $output;
}

/**
 * This function creates a string of letters (A - Z), which
 * depending on the sorting are either linked to author or title
 * filters i.e. clicking on the A when in the listing is sorted by
 * authors will bring up a list of all the entries where the first
 * character of the primary authors last name is "A"
 *
 * @param $type either "author or title"
 * @return a chunk of HTML code as described above
 */
function theme_biblio_alpha_line($variables) {
  $type    = $variables['type'];
  $current = $variables['current'];
  $path    = $variables['path'];
  $options = array();
  $base = variable_get('biblio_base', 'biblio');
  $all = '';
  $uri = drupal_parse_url(request_uri());
  if (substr($uri['path'], 0, 1) == '/') $uri['path'] = substr($uri['path'], 1);
  $all_uri = $uri;
  switch ($type) {
    case 'authors':
      $filter = 'author';
      $current = isset($uri['query']['f']['author']) ? $uri['query']['f']['author'] : '';
      unset($all_uri['query']['f']['author']);
      break;
    case 'keywords':
    case 'keyword':
      $filter = 'keyword';
      $current = isset($uri['query']['f']['keyword']) ? $uri['query']['f']['keyword'] : '';
      unset($all_uri['query']['f']['keyword']);
      break;
    case 'author':
      $current = isset($uri['query']['f']['ag']) ? $uri['query']['f']['ag'] : '';
      $filter = 'ag';
      if (!isset($uri['query']['s'])) $uri['query']['s'] = 'author';
      unset($all_uri['query']['f']['ag']);
      break;
    case 'title':
      $current = isset($uri['query']['f']['tg']) ? $uri['query']['f']['tg'] : '';
      $filter = 'tg';
      unset($all_uri['query']['f']['tg']);
      break;
    default:
  }
  $output = '<div class="biblio-alpha-line">';
//TODO   for Cyrillic use 1040 to 1071... e.g. &#1068;

  for ($i = 65; $i <= 90; $i++) {
    if ($i == ord($current)) {
      $output .= '<b>[' . chr($i) . ']</b>&nbsp;';
    }
    else{
      $uri['query']['f'][$filter] =  chr($i);
      $output .= l(chr($i), $uri['path'], $uri) . '&nbsp;';
    }
  }
  if ($current) {
    if (empty($all_uri['query']['f'])) unset($all_uri['query']['f']);
    $output .= '&nbsp;&nbsp;' . '[' . l(t('Show ALL'), $all_uri['path'], $all_uri) . ']';
  }
  $output .= '</div>';
  return $output;
}

/**
 * Themes the author editing form
 *
 * @param $form
 * @return rendered form
 */
function theme_biblio_admin_author_edit_form($variables) {
  $form = $variables['form'];
  $rows = array();
  $rows[] = array(
            array('data' => drupal_render($form['prefix'])),
            array('data' => drupal_render($form['firstname'])),
            array('data' => drupal_render($form['initials'])),
            array('data' => drupal_render($form['lastname'])),
            array('data' => drupal_render($form['suffix'])));
  $rows[] = array(array('data' => drupal_render($form['name']) . drupal_render($form['literal']), 'colspan' => 5));
  $rows[] = array(array('data' => drupal_render($form['affiliation']), 'colspan' => 5));
  $rows[] = array(array('data' => drupal_render($form['drupal_uid']), 'colspan' => 5));

  $output = theme('table', array('rows' => $rows));
  $output .= drupal_render($form['merge']);
  $output .= drupal_render($form['link']);
  $output .= drupal_render_children($form);
  return $output;

}

function theme_biblio_admin_orphans_form($variables) {
  $form = $variables['form'];
  // If there are any orphans, then $form['name'] contains a list of the author names
  $has_items = isset($form['name']) && is_array($form['name']);
  $select_header = $has_items ? theme('table_select_header_cell') : '';
  $header = array($select_header, t('Author Name'), t('Affiliation'));

  $output = '';
  $output .= drupal_render($form['submit']);
  if ($has_items) {
    foreach (element_children($form['name']) as $key) {
      $row = array();
      $row[] = drupal_render($form['authors'][$key]);
      $row[] = drupal_render($form['name'][$key]);
      $row[] = drupal_render($form['affiliation'][$key]);
      $rows[] = $row;
    }
  }
  else {
    $rows[] = array(array('data' => t('There are no orphaned authors in the database.'), 'colspan' => '3'));
  }
  $output .= theme('table', array('header' => $header, 'rows' => $rows));

  if (isset($form['pager']['#value'])) {
    $output .= drupal_render($form['pager']);
  }

//  $output .= drupal_render($form);

  return $output;
}
function theme_biblio_admin_keyword_orphans_form($variables) {

  $form = $variables['form'];
  // If there are any orphans, then $form['name'] contains a list of the author names

  $has_items = isset($form['keyword']) && is_array($form['keyword']);
  $select_header = $has_items ? theme('table_select_header_cell') : '';
  $header = array($select_header, t('Keyword'));

  $output = '';
  $output .= drupal_render($form['submit']);
  if ($has_items) {
    foreach (element_children($form['keyword']) as $key) {
      $row = array();
      $row[] = drupal_render($form['keywords'][$key]);
      $row[] = drupal_render($form['keyword'][$key]);
      $rows[] = $row;
    }
  }
  else {
    $rows[] = array(array('data' => t('There are no orphaned keywords in the database.'), 'colspan' => '2'));
  }
  $output .= theme('table', array('header' => $header, 'rows' => $rows));

  if ($form['pager']['#value']) {
 //   $output .= drupal_render($form['pager']);
  }

//  $output .= drupal_render($form);

  return $output;
}
function _biblio_keyword_links($keywords, $base='biblio') {
  $uri = drupal_parse_url(request_uri());
  $base      = variable_get('biblio_base', 'biblio');
  $uri['path'] = $base;

  $html = "";
  if (!is_array($keywords)) {
    require_once(drupal_get_path('module', 'biblio') . '/includes/biblio.keywords.inc');
    $keywords = biblio_explode_keywords($keywords);
  }

  $sep = variable_get('biblio_keyword_sep', ',');
  foreach ($keywords as $kid => $keyword ) {
    $uri['query']['f']['keyword'] = $kid;
    $linked_keywords [] = l(trim($keyword), $base , $uri);
  }
  return implode("$sep ", $linked_keywords);
}
