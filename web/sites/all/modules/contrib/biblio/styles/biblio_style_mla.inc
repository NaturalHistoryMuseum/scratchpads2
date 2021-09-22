<?php

/**
 * @file
 * MLA style.
 *
 * Original File:       ./cite/styles/cite_MLA.php
 * Original Repository: https://refbase.svn.sourceforge.net/svnroot/refbase/trunk/cite/styles/cite_MLA.php $
 * Original Author(s):  Richard Karnesky <mailto:karnesky@gmail.com> and
 *                      Matthias Steffens <mailto:refbase@extracts.de>
 *
 * Modified for use in biblio by Ron Jerome.
 */

/**
 * Get the style information.
 *
 * @return
 *   The name of the style
 */
function biblio_style_mla_info() {
  return array(
    'mla' => 'Modern Language Association (MLA)',
  );
}

/**
 *
 */
function biblio_style_mla_author_options() {
  $author_options = array(
    // 4.
    'BetweenAuthorsDelimStandard' => ', ',
    // 5.
    'BetweenAuthorsDelimLastAuthor' => ', and ',
    // 7.
    'AuthorsInitialsDelimFirstAuthor' => ', ',
    // 8.
    'AuthorsInitialsDelimStandard' => ' ',
    // 9.
    'betweenInitialsDelim' => '. ',
    // 10.
    'initialsBeforeAuthorFirstAuthor' => FALSE,
    // 11.
    'initialsBeforeAuthorStandard' => TRUE,
    // 12.
    'shortenGivenNames' => FALSE,
    // 13.
    'numberOfAuthorsTriggeringEtAl' => 3,
    // 14.
    'includeNumberOfAuthors' => 1,
    // 15.
    'customStringAfterFirstAuthors' => ', et al.',
    'encodeHTML' => TRUE,
  );

  return $author_options;
}

/**
 * Apply a bibliographic style to the node.
 *
 * @param $node
 *   An object containing the node data to render
 *
 * @return
 *   The styled biblio entry
 */
function biblio_style_mla($node) {
  module_load_include('inc', 'biblio', '/includes/biblio.contributors');
  $output = $authors = $editor = '';
  $author_options = biblio_style_mla_author_options();
  $primary_authors = biblio_get_contributor_category($node->biblio_contributors, 1);
  $editors = biblio_get_contributor_category($node->biblio_contributors, 2);
  // $corp_authors = biblio_get_contributor_category($node->biblio_contributors, 5);.
  if (!empty($primary_authors)) {
    $authors = theme('biblio_format_authors', array('contributors' => $primary_authors, 'options' => $author_options));
  }
  // If (empty($authors)) $authors = theme('biblio_authors', $node->biblio_contributors->get_category(5), 'mla', 5, $inline);  // if no authors substitute corp author if available
  // if (empty($authors)) $authors = '[' . t('Anonymous') . ']';  // use anonymous if we still have nothing.
  // $output .= '<span class="biblio-authors">' . $authors . "</span>.&nbsp; \n";.
  if (!empty($node->biblio_citekey)&&(variable_get('biblio_display_citation_key', 0))) {
    $output .= '[' . check_plain($node->biblio_citekey) . '] ';
  }

  switch ($node->biblio_type) {
    // Journal Article.
    case 102:
      // Newspaper Article.
    case 105:
      // Magazine Article.
    case 106:
      if (!empty($authors)) {
        $output .= '<span class="biblio-authors">';
        $output .= $authors;
        if (!preg_match("/\. *$/", $authors)) {
          $output .= ".";
        }
        $output .= '</span>';
      }

      // Title.
      if (!empty($node->title)) {
        if (!empty($authors)) {
          $output .= " ";
        }
        $output .= '"';
        $output .= '<span class="biblio-title-mla">';
        $url = biblio_get_title_url_info($node);
        $output .= l($node->title, $url['link'], $url['options']);
        $output .= "</span>";
        if (!preg_match("/[?!.]$/", $node->title)) {
          $output .= ".";
        }
        $output .= '"';
      }

      // From here on we'll assume that at least either the 'author' or the 'title' field did contain some contents
      // if this is not the case, the output string will begin with a space. However, any preceding/trailing whitespace will be removed at the cleanup stage (see below)
      // Abbreviated journal name.
      if (!empty($node->biblio_alternate_title)) {
        $output .= " " . '<i>' . $node->biblio_alternate_title . '</i>';

        // If there's no abbreviated journal name, we'll use the full journal name.
      }
      // Publication (= journal) name.
      elseif (!empty($node->biblio_secondary_title)) {
        $output .= " " . '<i>' . $node->biblio_secondary_title . '</i>';
      }
      // Volume.
      if (!empty($node->biblio_volume)) {
        if (!empty($node->biblio_alternate_title) || !empty($node->biblio_secondary_title)) {
          $output .= ".";
        }
        $output .= " " . $node->biblio_volume;
      }
      // Issue.
      if (!empty($node->biblio_issue)) {
        $output .= "." . $node->biblio_issue;
      }
      // Year.
      if (!empty($node->biblio_year)) {
        $output .= " (" . $node->biblio_year . ")";
      }
      // @todo Do something with the online pubs section.
      // This record refers to an online article.
      if (FALSE /*$node->online_publication == "yes"*/) {
        // Instead of any pages info (which normally doesn't exist for online publications) we append
        // an optional string (given in 'online_citation') plus the current date and the DOI (or URL):
        $today = date("j M. Y");

        // online_citation.
        if (!empty($node->online_citation)) {
          // Only add ":" if either volume, issue, abbrev_journal or publication isn't empty.
          if (!empty($node->biblio_volume) || !empty($node->biblio_issue) || !empty($node->biblio_alternate_title) || !empty($node->biblio_secondary_title)) {
            $output .= ":";
          }

          $output .= " " . $node->online_citation;
        }

        // Doi.
        if (!empty($node->doi)) {
          // Only add "." if online_citation isn't empty, or else if either volume, issue, abbrev_journal or publication isn't empty.
          if (!empty($node->online_citation) or (empty($node->online_citation) and (!empty($node->biblio_volume) || !empty($node->biblio_issue) || !empty($node->biblio_alternate_title) || !empty($node->biblio_secondary_title)))) {
            $output .= ".";
          }

          if ($encodeHTML) {
            $output .= " " . $today . encodeHTML(" <http://dx.doi.org/" . $node->doi . ">");
          }
          else {
            $output .= " " . $today . " <http://dx.doi.org/" . $node->doi . ">";
          }
        }
        // Url.
        elseif (!empty($node->url)) {
          // Only add "." if online_citation isn't empty, or else if either volume, issue, abbrev_journal or publication isn't empty.
          if (!empty($node->online_citation) or (empty($node->online_citation) and (!empty($node->biblio_volume) || !empty($node->biblio_issue) || !empty($node->biblio_alternate_title) || !empty($node->biblio_secondary_title)))) {
            $output .= ".";
          }

          if ($encodeHTML) {
            $output .= " " . $today . encodeHTML(" <" . $node->url . ">");
          }
          else {
            $output .= " " . $today . " <" . $node->url . ">";
          }
        }

      }
      // $node->online_publication == "no" -> this record refers to a printed article, so we append any pages info instead:
      else {
        // Pages.
        if (!empty($node->biblio_pages)) {
          if (!empty($node->biblio_year) ||
          !empty($node->biblio_volume) ||
          !empty($node->biblio_issue) ||
          !empty($$node->biblio_alternate_title) ||
          // Only add ": " if either volume, issue, abbrev_journal or publication isn't empty.
          !empty($node->biblio_secondary_title)) {
            $output .= ": ";
          }
          // @todo: $output .= formatPageInfo($node->biblio_pages, $markupPatternsArray["endash"]); // function 'formatPageInfo()' is defined in 'cite.inc.php'.
          $output .= theme('biblio_page_number', array('orig_page_info' => $node->biblio_pages));
        }
      }

      if (!preg_match("/\. *$/", $output)) {
        $output .= ".";
      }

      break;

    // Book Chapter.
    case 101:
      // Conference Paper.
    case 103:
      if (!empty($authors)) {
        $output .= '<span class="biblio-authors">';
        $output .= $authors;
        if (!preg_match("/\. *$/", $authors)) {
          $output .= ".";
        }
        $output .= '</span>';
      }
      // Title.
      if (!empty($node->title)) {
        if (!empty($authors)) {
          $output .= " ";
        }

        // $output .= '"<i>' ;.
        $output .= '<span class="biblio-title-mla">';
        $url = biblio_get_title_url_info($node);
        $output .= l($node->title, $url['link'], $url['options']);
        $output .= "</span>";
        // $output .= '</i>';.
        if (!preg_match("/[?!.]$/", $node->title)) {
          $output .= ".";
        }
        $output .= '"';
      }

      $publication = preg_replace("/[ \r\n]*\(Eds?:[^\)\r\n]*\)/", "", $node->biblio_secondary_title);
      // Publication.
      if (!empty($publication)) {
        $output .= " <i>$publication</i>";
      }

      // From here on we'll assume that at least either the 'author' or the 'title' field did contain some contents
      // if this is not the case, the output string will begin with a space. However, any preceding/trailing whitespace will be removed at the cleanup stage (see below)
      // Editor.
      if (!empty($editors)) {
        $editor_options = array(
          // 4.
          'BetweenAuthorsDelimStandard' => ', ',
          // 5.
          'BetweenAuthorsDelimLastAuthor' => ', and ',
          // 7.
          'AuthorsInitialsDelimFirstAuthor' => ' ',
          // 8.
          'AuthorsInitialsDelimStandard' => ' ',
          // 9.
          'betweenInitialsDelim' => '. ',
          // 10.
          'initialsBeforeAuthorFirstAuthor' => TRUE,
          // 11.
          'initialsBeforeAuthorStandard' => TRUE,
          // 12.
          'shortenGivenNames' => FALSE,
          // 13.
          'numberOfAuthorsTriggeringEtAl' => 3,
          // 14.
          'includeNumberOfAuthors' => 1,
          // 15.
          'customStringAfterFirstAuthors' => ', et al.',
          'encodeHTML' => TRUE,
        );
        $editor = theme('biblio_format_authors', array('contributors' => $editors, 'options' => $editor_options));
        _period_if_needed($output);

        // There are at least two editors (separated by ';')
        if (count($editors) > 1) {
          $output .= " Eds. " . $editor;
        }
        // there's only one editor (or the editor field is malformed with multiple editors but missing ';' separator[s])
        else {
          $output .= " Ed. " . $editor;
        }
      }

      // Edition.
      if (!empty($node->biblio_edition) && !preg_match("/^(1|1st|first|one)( ed\.?| edition)?$/i", $node->biblio_edition)) {
        _period_if_needed($output);

        // If the edition field contains a number of up to three digits, we assume it's an edition number (such as "2nd ed.")
        if (preg_match("/^\d{1,3}$/", $node->biblio_edition)) {
          if ($node->biblio_edition == "2") {
            $editionSuffix = "nd";
          }
          elseif ($node->biblio_edition == "3") {
            $editionSuffix = "rd";
          }
          else {
            $editionSuffix = "th";
          }
        }
        else {
          $editionSuffix = "";
        }

        if (preg_match("/^(Rev\.?|Revised)( ed\.?| edition)?$/i", $node->biblio_edition)) {
          $node->biblio_edition = "Rev.";
        }
        elseif (preg_match("/^(Abr\.?|Abridged)( ed\.?| edition)?$/i", $node->biblio_edition)) {
          $node->biblio_edition = "Abr.";
        }
        if (!preg_match("/( ed\.?| edition)$/i", $node->biblio_edition)) {
          $editionSuffix .= " ed.";
        }
        $output .= " " . $node->biblio_edition . $editionSuffix;
      }

      // Volume.
      if (!empty($node->biblio_volume)) {
        _period_if_needed($output);
        $output .= " Vol. " . $node->biblio_volume;
      }

      // If there's either a full or an abbreviated series title.
      if (!empty($node->biblio_alternate_title) || !empty($node->biblio_tertiary_title)) {
        _period_if_needed($output);
        $output .= " ";

        if (!empty($node->biblio_alternate_title)) {
          // Abbreviated series title.
          $output .= $node->biblio_alternate_title;
        }
        // If there's no abbreviated series title, we'll use the full series title instead:
        elseif (!empty($node->biblio_tertiary_title)) {
          // Full series title.
          $output .= $node->biblio_tertiary_title;
        }
        if (!empty($node->biblio_volume)||!empty($node->biblio_issue)) {
          $output .= ", ";
        }
        // Series volume.
        if (!empty($node->biblio_volume)) {
          $output .= $node->biblio_volume;
        }
        // Series issue (I'm not really sure if -- for this cite style -- the series issue should be rather omitted here)
        if (!empty($node->biblio_issue)) {
          // Is it correct to format series issues similar to journal article issues?
          $output .= "." . $node->biblio_issue;
        }
      }

      _period_if_needed($output);
      // Place.
      if (!empty($node->biblio_place_published)) {
        $output .= " " . $node->biblio_place_published;
      }
      // Publisher.
      if (!empty($node->biblio_publisher)) {
        if (!empty($node->biblio_place_published)) {
          $output .= ":";
        }
        $output .= " " . $node->biblio_publisher . ", ";
      }

      // Year.
      if (!empty($node->biblio_year)) {
        $output .= " " . $node->biblio_year . ". ";
      }
      _period_if_needed($output);
      // Pages.
      if (!empty($node->biblio_pages)) {
        $output .= theme('biblio_page_number', array('orig_page_info' => $node->biblio_pages));
      }
      _period_if_needed($output);
      // If (!preg_match("/\. *$/", $output))  $output .= ".";.
      break;

    // All other types.
    default:

      // Author.
      if (!empty($authors)) {
        $output .= '<span class="biblio-authors">';
        $output .= $authors;
        if (!preg_match("/\.*$/", $authors)) {
          $output .= ".";
        }
        $output .= '</span>';
      }

      // Title.
      if (!empty($node->title)) {
        $url = biblio_get_title_url_info($node);
        if (!empty($authors)) {
          $output .= " ";
        }

        // Thesis.
        if (!empty($node->thesis)) {
          $output .= '<span class="biblio-title-mla">';
          $output .= '"' . l($node->title, $url['link'], $url['options']);
          $output .= '</span>';
          if (!preg_match("/[?!.]$/", $node->title)) {
            $output .= ".";
          }
          $output .= '"';
        }
        // Not a thesis.
        else {
          $output .= '<i>';
        }
        $output .= '<span class="biblio-title-mla">';
        $output .= l($node->title, $url['link'], $url['options']);
        $output .= '</span>';
        $output .= '</i>';
      }

      // Editor (if different from author, see note above regarding the check for ' (ed)' or ' (eds)')
      if (!empty($editors)) {
        $editor_options = array(
          // 4.
          'BetweenAuthorsDelimStandard' => ', ',
          // 5.
          'BetweenAuthorsDelimLastAuthor' => ', and ',
          // 7.
          'AuthorsInitialsDelimFirstAuthor' => ' ',
          // 8.
          'AuthorsInitialsDelimStandard' => ' ',
          // 9.
          'betweenInitialsDelim' => '. ',
          // 10.
          'initialsBeforeAuthorFirstAuthor' => TRUE,
          // 11.
          'initialsBeforeAuthorStandard' => TRUE,
          // 12.
          'shortenGivenNames' => FALSE,
          // 13.
          'numberOfAuthorsTriggeringEtAl' => 3,
          // 14.
          'includeNumberOfAuthors' => 1,
          // 15.
          'customStringAfterFirstAuthors' => ', et al.',
          'encodeHTML' => TRUE,
        );
        $editor = theme('biblio_format_authors', array('contributors' => $editors, 'options' => $editor_options));
        _period_if_needed($output);

        // There are at least two editors (separated by ';')
        if (count($editors) > 1) {
          $output .= " Eds. " . $editor;
        }
        // there's only one editor (or the editor field is malformed with multiple editors but missing ';' separator[s])
        else {
          $output .= " Ed. " . $editor;
        }
      }

      // Edition.
      if (!empty($node->biblio_edition) && !preg_match("/^(1|1st|first|one)( ed\.?| edition)?$/i", $node->biblio_edition)) {
        _period_if_needed($output);

        // If the edition field contains a number of up to three digits, we assume it's an edition number (such as "2nd ed.")
        if (preg_match("/^\d{1,3}$/", $node->biblio_edition)) {
          if ($node->biblio_edition == "2") {
            $editionSuffix = "nd";
          }
          elseif ($node->biblio_edition == "3") {
            $editionSuffix = "rd";
          }
          else {
            $editionSuffix = "th";
          }
        }
        else {
          $editionSuffix = "";
        }

        if (preg_match("/^(Rev\.?|Revised)( ed\.?| edition)?$/i", $node->biblio_edition)) {
          $node->biblio_edition = "Rev.";
        }
        elseif (preg_match("/^(Abr\.?|Abridged)( ed\.?| edition)?$/i", $node->biblio_edition)) {
          $node->biblio_edition = "Abr.";
        }
        if (!preg_match("/( ed\.?| edition)$/i", $node->biblio_edition)) {
          $editionSuffix .= " ed.";
        }
        $output .= " " . $node->biblio_edition . $editionSuffix;
      }

      // Volume.
      if (!empty($node->biblio_volume)) {
        _period_if_needed($output);
        $output .= " Vol. " . $node->biblio_volume;
      }

      // If there's either a full or an abbreviated series title.
      if (!empty($node->biblio_alternate_title) or !empty($node->biblio_secondary_title)) {
        _period_if_needed($output);
        $output .= " ";

        if (!empty($node->biblio_alternate_title)) {
          // Abbreviated series title.
          $output .= $node->biblio_alternate_title;
        }
        // If there's no abbreviated series title, we'll use the full series title instead:
        elseif (!empty($node->biblio_secondary_title)) {
          // Full series title.
          $output .= $node->biblio_secondary_title;
        }
        if (!empty($node->biblio_volume)||!empty($node->biblio_issue)) {
          $output .= ", ";
        }
        // Series volume.
        if (!empty($node->biblio_volume)) {
          $output .= $node->biblio_volume;
        }
        // Series issue (I'm not really sure if -- for this cite style -- the series issue should be rather omitted here)
        if (!empty($node->biblio_issue)) {
          // Is it correct to format series issues similar to journal article issues?
          $output .= "." . $node->biblio_issue;
        }
      }

      // Thesis (unpublished dissertation)
      if (!empty($node->thesis)) {
        // @todo: a published dissertation needs to be formatted differently!
        //       see e.g. example at: <http://web.csustan.edu/english/reuben/pal/append/AXI.HTML>
        _period_if_needed($output);

        // @todo: I've also seen MLA examples that separate thesis name, name of institution and year by dots. ?:-|
        //       Also, do we need to use the abbreviation "Diss." instead of "Ph.D. thesis"? What about other thesis types then?
        //       see e.g. <http://www.english.uiuc.edu/cws/wworkshop/writer_resources/citation_styles/mla/unpublished_diss.htm>
        $output .= " " . $node->thesis;
        $output .= ", " . $node->biblio_publisher;
      }
      // Not a thesis.
      else {
        _period_if_needed($output);

        // Place.
        if (!empty($node->biblio_place_published)) {
          $output .= " " . $node->biblio_place_published;
        }
        // Publisher.
        if (!empty($node->biblio_publisher)) {
          if (!empty($node->biblio_place_published)) {
            $output .= ":";
          }
          $output .= " " . $node->biblio_publisher;
        }
      }

      // Year.
      if (!empty($node->biblio_year)) {
        $output .= ", " . $node->biblio_year;
      }

      // This record refers to an online article.
      if (isset($node->online_publication) && $node->online_publication == "yes") {
        $today = date("j M. Y");

        // online_citation.
        if (!empty($node->online_citation)) {
          if (!preg_match("/\. *$/", $output)) {
            $output .= ".";
          }

          $output .= " " . $node->online_citation;
        }

        // Doi.
        if (!empty($node->doi)) {
          if (!preg_match("/\. *$/", $output)) {
            $output .= ".";
          }

          if ($encodeHTML) {
            $output .= " " . $today . encodeHTML(" <http://dx.doi.org/" . $node->doi . ">");
          }
          else {
            $output .= " " . $today . " <http://dx.doi.org/" . $node->doi . ">";
          }
        }
        // Url.
        elseif (!empty($node->url)) {
          if (!preg_match("/\. *$/", $output)) {
            $output .= ".";
          }

          if ($encodeHTML) {
            $output .= " " . $today . encodeHTML(" <" . $node->url . ">");
          }
          else {
            $output .= " " . $today . " <" . $node->url . ">";
          }
        }
      }

      if (!preg_match("/\. *$/", $output)) {
        $output .= ".";
      }
      break;
  }

  return filter_xss($output, biblio_get_allowed_tags());
}

/**
 *
 */
function _period_if_needed(&$output) {
  if (!preg_match("/[?!.][ \"" . '<\/i>' . "]*$/", $output)) {
    $output .= ".";
  }
}
