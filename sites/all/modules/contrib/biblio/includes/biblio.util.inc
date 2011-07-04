<?php
function biblio_normalize_title($title) {
  $stop_words = 'a,an,the,is,on';
  $stop_words = explode(',', variable_get('biblio_stop_words', $stop_words));
  if (!@preg_match('/\pL/u', 'a')) {
    // probably a broken PCRE library
    $title = trim(_strip_punctuation($title));
  }
  else {
    // Unicode safe filter for the value
    $title = trim(_strip_punctuation_utf8($title));
  }

  $title = trim(_strip_punctuation($title));

  if (function_exists('mb_regex_encoding')) {
    mb_regex_encoding("utf-8");
    $title_words = mb_split(' +', $title);
  }
  else {
    $title_words = explode(' ', $title);
  }

  while (array_search(drupal_strtolower($title_words[0]), $stop_words) !== FALSE) {
    array_shift($title_words);
  }

  return drupal_substr(implode(' ', $title_words), 0, 64);
}
function biblio_coins($node) {
  // Copyright:          Matthias Steffens <mailto:refbase@extracts.de> and the file's
  //                     original author.
  // Original Author:    Richard Karnesky <mailto:karnesky@gmail.com>  //
  // Adapted for biblio: Ron Jerome
  // fmt_info (type)
  $fmt = "info:ofi/fmt:kev:mtx:";
  // 'dissertation' is compatible with the 1.0 spec, but not the 0.1 spec
  if ($node->biblio_type == 108) {
    $fmt .= "dissertation";
  }
  elseif ($node->biblio_type == 102) {
    $fmt .= "journal";
  }
  elseif ($node->biblio_type == 100 || $node->biblio_type == 101) {
    $fmt .= "book";
  }
  // 'dc' (dublin core) is compatible with the 1.0 spec, but not the 0.1 spec.
  // We default to this, as it is the most generic type.
  else {
   $fmt .= "dc";
  }

  $co = biblio_contextObject($node);
  $coins = "ctx_ver=Z39.88-2004&amp;rft_val_fmt=" . urlencode($fmt);
  foreach ($co as $coKey => $coValue) {
    // 'urlencode()' differs from 'rawurlencode() (i.e., RFC1738 encoding)
    // in that spaces are encoded as plus (+) signs
    $coKey = preg_replace("/au[0-9]*/", "au", $coKey);
    $coins .= "&amp;" . $coKey . "=" . urlencode($coValue);
  }
  $coinsSpan = "<span class=\"Z3988\" title=\"" . $coins . "\"></span>";
  return $coinsSpan;
}

function biblio_contextObject($node) {
  // Copyright:          Matthias Steffens <mailto:refbase@extracts.de> and the file's
  //                     original author.
  // Original Author:    Richard Karnesky <mailto:karnesky@gmail.com>  //
  // Adapted for biblio: Ron Jerome
  global $base_url;
  $i = 0;
    // $openurl_base = variable_get('biblio_baseopenurl', '');
  $co = array();
  // rfr_id
//  $co["rfr_id"] = "info:sid/". ereg_replace("http://", "", $base_url);
//  // genre (type)
//  if (isset($node->biblio_type)) {
//    if ($node->biblio_type == 102)
//    $co["rft.genre"] = "article";
//    elseif ($node->biblio_type == 101) $co["rft.genre"] = "bookitem";
//    elseif ($node->biblio_type == 100) $co["rft.genre"] = "book";
//    elseif ($node->biblio_type == "Journal") $co["rft.genre"] = "journal";
//  }
  // atitle, btitle, title (title, publication)
  if (($node->biblio_type == 102) || ($node->biblio_type == 101)) {
    if (!empty($node->title)) $co["rft.atitle"] = check_plain($node->title);
    if (!empty($node->biblio_secondary_title)) {
      $co["rft.title"] = check_plain($node->biblio_secondary_title);
      if ($node->biblio_type == 101)
      $co["rft.btitle"] = check_plain($node->biblio_secondary_title);
    }
  }
  elseif (!empty($node->title)) {
   $co["rft.title"] = check_plain($node->title);
  }
  if (($node->biblio_type == 100) && (!empty($node->biblio_secondary_title))) $co["rft.btitle"] = check_plain($node->biblio_secondary_title);
  // stitle (abbrev_journal)
  if (!empty($node->biblio_short_title)) $co["rft.stitle"] = check_plain($node->biblio_short_title);
  // series (series_title)
  if (!empty($node->biblio_tertiary_title)) $co["rft.series"] = check_plain($node->biblio_tertiary_title);
  // issn
  if (!empty($node->biblio_issn)) $co["rft.issn"] = check_plain($node->biblio_issn);
  // isbn
  if (!empty($node->biblio_isbn)) $co["rft.isbn"] = check_plain($node->biblio_isbn);
  // date (year)
  if (!empty($node->biblio_year)) $co["rft.date"] = check_plain($node->biblio_year);
  // volume
  if (!empty($node->biblio_volume)) $co["rft.volume"] = check_plain($node->biblio_volume);
  // issue
  if (!empty($node->biblio_issue)) $co["rft.issue"] = check_plain($node->biblio_issue);
  // spage, epage, tpages (pages)
  // NOTE: lifted from modsxml.inc.php--should throw some into a new include file
  if (!empty($node->biblio_pages)) {
    if (preg_match("/[0-9] *- *[0-9]/", $node->biblio_pages)) {
      list ($pagestart, $pageend) = preg_split('/\s*[-]\s*/', $node->biblio_pages);
      if ($pagestart < $pageend) {
        $co["rft.spage"] = check_plain($pagestart);
        $co["rft.epage"] = check_plain($pageend);
      }
    }
    elseif ($node->biblio_type == 100) { //"Book Whole") {
      $pagetotal = preg_replace('/^(\d+)\s*pp?\.?$/', "\\1", $node->biblio_pages);
      $co["rft.tpages"] = check_plain($pagetotal);
    }
    else {
      $co["rft.spage"] = check_plain($node->biblio_pages);
    }
  }
  // aulast, aufirst, author (author)
  if (!empty($node->biblio_contributors)) {
    if (!empty($node->biblio_contributors[0]['lastname'])) {
      $co["rft.aulast"] = check_plain($node->biblio_contributors[0]['lastname']);
    }
    if (!empty($node->biblio_contributors[0]['firstname'])) {
      $co["rft.aufirst"] = check_plain($node->biblio_contributors[0]['firstname']);
    }
    elseif (!empty($node->biblio_contributors[0]['initials'])) {
      $co["rft.auinit"] = check_plain($node->biblio_contributors[0]['initials']);
    }
    for($i = 1; $i < count($node->biblio_contributors); $i++) {
      $author = $node->biblio_contributors[$i];
      if ($author['auth_category'] == 1) {
        if (!empty($author['lastname'])) {
          $au = $author['lastname'];
          if (!empty($author['firstname']) || !empty($author['initials'])) $au .= ", ";
        }
        if (!empty($author['firstname'])) {
          $au .= $author['firstname'];
        }
        elseif (!empty($author['initials'])) {
          $au .= $author['initials'];
        }
        if (!empty($au)) $co["rft.au" . $i] = $au;
      }
    }
  }
  // pub (publisher)
  if (!empty($node->biblio_publisher)) $co["rft.pub"] = check_plain($node->biblio_publisher);
  // place
  if (!empty($node->biblio_place_published)) $co["rft.place"] = check_plain($node->biblio_place_published);
  // id (doi, url)
  if (!empty($node->biblio_doi)) {
    $co["rft_id"] = "info:doi/" . check_plain($node->biblio_doi);
  }
//  elseif (!empty($node->biblio_url)) {
//    $co["rft_id"] = $node->biblio_url;
//  }

  return $co;
}

function biblio_coins_generate(& $node) {
  if (!isset($node->vid)) {
    $node->biblio_coins = biblio_coins($node);
    return;
  }
  if ($node) {
    $node->biblio_coins = biblio_coins($node);
    db_update('biblio')
      ->fields(array('biblio_coins' => $node->biblio_coins))
      ->condition('vid', $node->vid)
      ->execute();
  }
  else {
    $result = db_query("SELECT nr.*, b.*
                        FROM {node} AS n
                        LEFT JOIN {node_revision}  AS nr ON n.vid = nr.vid LEFT JOIN {biblio} AS b ON n.vid = b.vid
                        WHERE n.type = 'biblio'  ");

    foreach ($result as $node) {
      $node->biblio_coins = biblio_coins($node);
      db_update('biblio')
        ->fields(array('biblio_coins' => $node->biblio_coins))
        ->condition('vid', $node->vid)
        ->execute();
    }
    drupal_goto('');
  }
}

function _strip_punctuation($text) {
  return preg_replace("/[[:punct:]]/", '', $text);
}
/**
 * Copyright (c) 2008, David R. Nadeau, NadeauSoftware.com.
 * All rights reserved.
 *
 * Strip punctuation characters from UTF-8 text.
 *
 * Characters stripped from the text include characters in the following
 * Unicode categories:
 *
 *   Separators
 *   Control characters
 *  Formatting characters
 *  Surrogates
 *  Open and close quotes
 *  Open and close brackets
 *  Dashes
 *  Connectors
 *  Numer separators
 *  Spaces
 *  Other punctuation
 *
 * Exceptions are made for punctuation characters that occur withn URLs
 * (such as [ ] : ; @ & ? and others), within numbers (such as . , % # '),
 * and within words (such as - and ').
 *
 * Parameters:
 *   text    the UTF-8 text to strip
 *
 * Return values:
 *   the stripped UTF-8 text.
 *
 * See also:
 *   http://nadeausoftware.com/articles/2007/9/php_tip_how_strip_punctuation_characters_web_page
 */
function _strip_punctuation_utf8( $text )
{
  $urlbrackets    = '\[\]\(\)';
  $urlspacebefore = ':;\'_\*%@&?!' . $urlbrackets;
  $urlspaceafter  = '\.,:;\'\-_\*@&\/\\\\\?!#' . $urlbrackets;
  $urlall         = '\.,:;\'\-_\*%@&\/\\\\\?!#' . $urlbrackets;

  $specialquotes = '\'"\*<>';

  $fullstop      = '\x{002E}\x{FE52}\x{FF0E}';
  $comma         = '\x{002C}\x{FE50}\x{FF0C}';
  $arabsep       = '\x{066B}\x{066C}';
  $numseparators = $fullstop . $comma . $arabsep;

  $numbersign    = '\x{0023}\x{FE5F}\x{FF03}';
  $percent       = '\x{066A}\x{0025}\x{066A}\x{FE6A}\x{FF05}\x{2030}\x{2031}';
  $prime         = '\x{2032}\x{2033}\x{2034}\x{2057}';
  $nummodifiers  = $numbersign . $percent . $prime;

  return preg_replace(
    array(
    // Remove separator, control, formatting, surrogate,
    // open/close quotes.
      '/[\p{Z}\p{Cc}\p{Cf}\p{Cs}\p{Pi}\p{Pf}]/u',
    // Remove other punctuation except special cases
      '/\p{Po}(?<![' . $specialquotes .
        $numseparators . $urlall . $nummodifiers . '])/u',
    // Remove non-URL open/close brackets, except URL brackets.
      '/[\p{Ps}\p{Pe}](?<![' . $urlbrackets . '])/u',
    // Remove special quotes, dashes, connectors, number
    // separators, and URL characters followed by a space
      '/[' . $specialquotes . $numseparators . $urlspaceafter .
        '\p{Pd}\p{Pc}]+((?= )|$)/u',
    // Remove special quotes, connectors, and URL characters
    // preceded by a space
      '/((?<= )|^)[' . $specialquotes . $urlspacebefore . '\p{Pc}]+/u',
    // Remove dashes preceded by a space, but not followed by a number
      '/((?<= )|^)\p{Pd}+(?![\p{N}\p{Sc}])/u',
    // Remove consecutive spaces
      '/ +/',
    ),
    ' ',
    $text );
}

/**
 * Copyright (c) 2008, David R. Nadeau, NadeauSoftware.com.
 * All rights reserved.
 *
 * Strip symbol characters from UTF-8 text.
 *
 * Characters stripped from the text include characters in the following
 * Unicode categories:
 *
 *   Modifier symbols
 *   Private use symbols
 *   Math symbols
 *   Other symbols
 *
 * Exceptions are made for math symbols embedded within numbers (such as
 * + - /), math symbols used within URLs (such as = ~), units of measure
 * symbols, and ideograph parts.  Currency symbols are not removed.
 *
 * Parameters:
 *   text    the UTF-8 text to strip
 *
 * Return values:
 *   the stripped UTF-8 text.
 *
 * See also:
 *  http://nadeausoftware.com/articles/2007/09/php_tip_how_strip_symbol_characters_web_page
 */
function _strip_symbols( $text )
{
  $plus   = '\+\x{FE62}\x{FF0B}\x{208A}\x{207A}';
  $minus  = '\x{2012}\x{208B}\x{207B}';

  $units  = '\\x{00B0}\x{2103}\x{2109}\\x{23CD}';
  $units .= '\\x{32CC}-\\x{32CE}';
  $units .= '\\x{3300}-\\x{3357}';
  $units .= '\\x{3371}-\\x{33DF}';
  $units .= '\\x{33FF}';

  $ideo   = '\\x{2E80}-\\x{2EF3}';
  $ideo  .= '\\x{2F00}-\\x{2FD5}';
  $ideo  .= '\\x{2FF0}-\\x{2FFB}';
  $ideo  .= '\\x{3037}-\\x{303F}';
  $ideo  .= '\\x{3190}-\\x{319F}';
  $ideo  .= '\\x{31C0}-\\x{31CF}';
  $ideo  .= '\\x{32C0}-\\x{32CB}';
  $ideo  .= '\\x{3358}-\\x{3370}';
  $ideo  .= '\\x{33E0}-\\x{33FE}';
  $ideo  .= '\\x{A490}-\\x{A4C6}';

  return preg_replace(
    array(
    // Remove modifier and private use symbols.
      '/[\p{Sk}\p{Co}]/u',
    // Remove math symbols except + - = ~ and fraction slash
      '/\p{Sm}(?<![' . $plus . $minus . '=~\x{2044}])/u',
    // Remove + - if space before, no number or currency after
      '/((?<= )|^)[' . $plus . $minus . ']+((?![\p{N}\p{Sc}])|$)/u',
    // Remove = if space before
      '/((?<= )|^)=+/u',
    // Remove + - = ~ if space after
      '/[' . $plus . $minus . '=~]+((?= )|$)/u',
    // Remove other symbols except units and ideograph parts
      '/\p{So}(?<![' . $units . $ideo . '])/u',
    // Remove consecutive white space
      '/ +/',
    ),
    ' ',
    $text );
}
/**
 * Remove HTML tags, including invisible text such as style and
 * script code, and embedded objects.  Add line breaks around
 * block-level tags to prevent word joining after tag removal.
 */
function _strip_html_tags( $text )
{
    $text = preg_replace(
        array(
          // Remove invisible content
            '@<head[^>]*?>.*?</head>@siu',
            '@<style[^>]*?>.*?</style>@siu',
            '@<script[^>]*?.*?</script>@siu',
            '@<object[^>]*?.*?</object>@siu',
            '@<embed[^>]*?.*?</embed>@siu',
            '@<applet[^>]*?.*?</applet>@siu',
            '@<noframes[^>]*?.*?</noframes>@siu',
            '@<noscript[^>]*?.*?</noscript>@siu',
            '@<noembed[^>]*?.*?</noembed>@siu',
          // Add line breaks before and after blocks
            '@</?((address)|(blockquote)|(center)|(del))@iu',
            '@</?((div)|(h[1-9])|(ins)|(isindex)|(p)|(pre))@iu',
            '@</?((dir)|(dl)|(dt)|(dd)|(li)|(menu)|(ol)|(ul))@iu',
            '@</?((table)|(th)|(td)|(caption))@iu',
            '@</?((form)|(button)|(fieldset)|(legend)|(input))@iu',
            '@</?((label)|(select)|(optgroup)|(option)|(textarea))@iu',
            '@</?((frameset)|(frame)|(iframe))@iu',
        ),
        array(
            ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ',
            "\n\$0", "\n\$0", "\n\$0", "\n\$0", "\n\$0", "\n\$0",
            "\n\$0", "\n\$0",
        ),
        $text );
    return strip_tags( $text );
}