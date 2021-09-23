<?php

/**
 * @file
 * Chicago Manual of Style.
 *
 * Original File:       ./cite/styles/cite_Chicago.php
 * Original Author(s):  Matthias Steffens <mailto:refbase@extracts.de> and
 *                      Richard Karnesky <mailto:karnesky@gmail.com>
 *
 * This is a citation style file (which must reside within the 'cite/styles/' sub-directory of your refbase root directory). It contains a
 * version of the 'citeRecord()' function that outputs a reference list from selected records according to the citation style documented
 * in the "Chicago Manual of Style" (2003), and Kate Turabian's "Manual for Writer's of Term Papers, Theses, and Dissertations" (1996)
 *
 * Modified for use in biblio by Ron Jerome.
 */

/**
 * Get the style information.
 *
 * @return
 *   The name of the style
 */
function biblio_style_chicago_info() {
  return array(
    'chicago' => 'Chicago',
  );
}

/**
 *
 */
function biblio_style_chicago_author_options() {
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
    'numberOfAuthorsTriggeringEtAl' => 10,
    // 14.
    'includeNumberOfAuthors' => 10,
    // 15.
    'customStringAfterFirstAuthors' => ' et al.',
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
function biblio_style_chicago($node) {
  module_load_include('inc', 'biblio', '/includes/biblio.contributors');
  $output = '';
  $markupPatternsArray = array(
    "italic-prefix" => '<i>',
    "italic-suffix" => '<\/i>',
    "endash" => '-',
  );
  $author_options = biblio_style_chicago_author_options();
  $authors = $editor = '';
  $primary_authors = biblio_get_contributor_category($node->biblio_contributors, 1);
  $editors = biblio_get_contributor_category($node->biblio_contributors, 2);
  if (!empty($primary_authors)) {
    $authors = theme('biblio_format_authors', array('contributors' => $primary_authors, 'options' => $author_options));
  }
  // $editors = theme('biblio_format_authors', array('contributors' => $node->biblio_contributors[2], 'options' => $author_options, 'inline' => $inline));
  // if (empty($authors)) $authors = theme('biblio_authors', $node->biblio_contributors[5], 'mla', 5, $inline);  // if no authors substitute corp author if available
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
        $output .= '<span class="biblio-title-chicago">';
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
      // Publication (= journal) name.
      if (!empty($node->biblio_secondary_title)) {
        $output .= " <i>$node->biblio_secondary_title</i>";
      }

      // If there's no full journal name, we'll use the abbreviated journal name
      // abbreviated journal name.
      elseif (!empty($node->biblio_alternate_title)) {
        $output .= " <i>$node->biblio_alternate_title</i>";
      }

      // Volume.
      if (!empty($node->biblio_volume)) {
        $output .= " " . $node->biblio_volume;
      }

      // Issue.
      if (!empty($node->biblio_issue)) {
        $output .= ", no. " . $node->biblio_issue;
      }

      // Year.
      if (!empty($node->biblio_year)) {
        $output .= " (" . $node->biblio_year . ")";
      }

      // Pages.
      if (!empty($node->biblio_pages)) {
        // Only add ": " if either year, volume, issue, abbrev_journal or publication isn't empty.
        if (!empty($node->biblio_year) || !empty($node->biblio_volume) || !empty($node->biblio_issue) || !empty($node->biblio_alternate_title) || !empty($node->biblio_secondary_title)) {
          $output .= ": ";
        }

        // Function 'formatPageInfo()' is defined in 'cite.inc.php'.
        $output .= theme('biblio_page_number', array('orig_page_info' => $node->biblio_pages));
      }

      // This record refers to an online article.
      if (isset($node->online_publication)) {
        // Append an optional string (given in 'online_citation') plus the current date and the DOI (or URL):
        $today = date("F j, Y");

        // online_citation.
        if (!empty($node->online_citation)) {
          // Only add "," if either year, volume, issue, abbrev_journal or publication isn't empty.
          if (!empty($node->biblio_year) || !empty($node->biblio_volume) || !empty($node->biblio_issue) || !empty($node->biblio_alternate_title) || !empty($node->biblio_secondary_title)) {
            if (empty($node->biblio_pages)) {
              // Print instead of pages.
              $output .= ":";
            }
            else {
              // Append to pages.
              $output .= ",";
            }
          }

          $output .= " " . $node->online_citation;
        }

        // Doi.
        if (!empty($node->doi)) {
          // Only add "." if online_citation isn't empty, or else if either year, volume, issue, abbrev_journal or publication isn't empty.
          if (!empty($node->online_citation) or (empty($node->online_citation) and (!empty($node->biblio_year) || !empty($node->biblio_volume) || !empty($node->biblio_issue) || !empty($node->biblio_alternate_title) || !empty($node->biblio_secondary_title)))) {
            // NOTE: some Chicago examples (e.g. <http://www.lib.berkeley.edu/instruct/guides/chicago-turabianstyle.pdf>) use a comma here (not sure what's correct)
            $output .= ".";
          }

          if ($encodeHTML) {
            $output .= " " . encodeHTML("http://dx.doi.org/" . $node->doi) . " (accessed " . $today . ")";
          }
          else {
            $output .= " " . "http://dx.doi.org/" . $node->doi . " (accessed " . $today . ")";
          }
        }
        // Url.
        elseif (!empty($node->biblio_url)) {
          // Only add "." if online_citation isn't empty, or else if either year, volume, issue, abbrev_journal or publication isn't empty.
          if (!empty($node->online_citation) or (empty($node->online_citation) and (!empty($node->biblio_year) || !empty($node->biblio_volume) || !empty($node->biblio_issue) || !empty($node->biblio_alternate_title) || !empty($node->biblio_secondary_title)))) {
            // See note for doi.
            $output .= ".";
          }

          if ($encodeHTML) {
            $output .= " " . encodeHTML($node->biblio_url) . " (accessed " . $today . ")";
          }
          else {
            $output .= " " . $node->biblio_url . " (accessed " . $today . ")";
          }
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

        $output .= '"<i>';
        $output .= '<span class="biblio-title-chicago">';
        $url = biblio_get_title_url_info($node);
        $output .= l($node->title, $url['link'], $url['options']);
        $output .= "</span>";
        $output .= '</i>';
        if (!preg_match("/[?!.]$/", $node->title)) {
          $output .= ".";
        }
        $output .= '"';
      }

      $publication = preg_replace("/[ \r\n]*\(Eds?:[^\)\r\n]*\)/", "", $node->biblio_secondary_title);
      // Publication.
      if (!empty($publication)) {
        $output .= " In <i>$publication</i>";
      }

      // From here on we'll assume that at least either the 'author' or the 'title' field did contain some contents
      // if this is not the case, the output string will begin with a space. However, any preceding/trailing whitespace will be removed at the cleanup stage (see below)
      // Editor.
      if (!empty($editors)) {
        $editor_options = array(
          'BetweenAuthorsDelimStandard'     => ', ',
          'BetweenAuthorsDelimLastAuthor'   => ' and ',
          'AuthorsInitialsDelimFirstAuthor' => ' ',
          'AuthorsInitialsDelimStandard'    => ' ',
          'betweenInitialsDelim'            => '. ',
          'initialsBeforeAuthorFirstAuthor' => TRUE,
          'initialsBeforeAuthorStandard'    => TRUE,
          'shortenGivenNames'               => FALSE,
          'numberOfAuthorsTriggeringEtAl'   => 10,
          'includeNumberOfAuthors'          => 10,
          'customStringAfterFirstAuthors'   => ' et al.',
          'encodeHTML'                      => TRUE,
        );

        $editor = theme('biblio_format_authors', array('contributors' => $editors, 'options' => $editor_options));

        $output .= ", edited by " . $editor;
      }

      // Pages.
      if (!empty($node->biblio_pages)) {
        // Function 'formatPageInfo()' is defined in 'cite.inc.php'.
        $output .= ", " . theme('biblio_page_number', array('orig_page_info' => $node->biblio_pages));
      }

      // Edition.
      if (!empty($node->biblio_edition) && !preg_match("/^(1|1st|first|one)( ed\.?| edition)?$/i", $node->biblio_edition)) {
        if (!preg_match("/[?!.][ \"<i>]*$/", $output)) {
          $output .= ".";
        }

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

        if (!preg_match("/( ed\.?| edition)$/i", $node->biblio_edition)) {
          $editionSuffix .= " ed.";
        }

        $output .= " " . $node->biblio_edition . $editionSuffix;
      }

      // Volume.
      if (!empty($node->biblio_volume)) {
        if (!preg_match("/[?!.][ \"<i>]*$/", $output)) {
          $output .= ".";
        }

        $output .= " Vol. " . $node->biblio_volume;
      }

      // If there's either a full or an abbreviated series title.
      if (!empty($node->biblio_alternate_title) or !empty($node->biblio_tertiary_title)) {
        if (!preg_match("/[?!.][ \"<i>]*$/", $output)) {
          $output .= ".";
        }

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
          $output .= " ";
        }

        // Series volume.
        if (!empty($node->biblio_volume)) {
          $output .= $node->biblio_volume;
        }

        // If (!empty($node->biblio_issue))      // series issue (I'm not really sure if -- for this cite style -- the series issue should be rather omitted here)
        //       $output .= ", no. " . $node->biblio_issue; // is it correct to format series issues similar to journal article issues?
      }

      if (!preg_match("/[?!.][ \"<i>]*$/", $output)) {
        $output .= ".";
      }

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

      // Year.
      if (!empty($node->biblio_year)) {
        $output .= ", " . $node->biblio_year;
      }

      if (!preg_match("/\. *$/", $output)) {
        $output .= ".";
      }

      break;

    // All other types.
    default:
      // @todo
      // @code
      //        if (preg_match("[ \r\n]*\(ed\)", $node->author)) // single editor
      //          $author = $author . ", ed";
      //        elseif (preg_match("[ \r\n]*\(eds\)", $node->author)) // multiple editors
      //          $author = $author . ", eds";.
      // @endcode
      // Author.
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
        $url = biblio_get_title_url_info($node);
        if (!empty($authors)) {
          $output .= " ";
        }

        // Thesis.
        if (!empty($node->thesis)) {
          $output .= '<span class="biblio-title-chicago">';
          $output .= '"' . l($node->title, $url['link'], $url['options']);
          $output .= "</span>";
          if (!preg_match("/[?!.]$/", $node->title)) {
            $output .= ".";
          }
          $output .= '"';
        }
        // Not a thesis.
        else {
          $output .= '<i>';
        }
        $output .= '<span class="biblio-title-chicago">';
        $output .= l($node->title, $url['link'], $url['options']);
        $output .= "</span>";
        $output .= '</i>';
      }
      $publication = preg_replace("/[ \r\n]*\(Eds?:[^\)\r\n]*\)/", "", $node->biblio_secondary_title);
      // Publication.
      if (!empty($publication)) {
        $output .= " In <i>$publication</i>";
      }

      // Editor (if different from author, see note above regarding the check for ' (ed)' or ' (eds)')
      if (!empty($editors)) {
        $editor_options = array(
          'BetweenAuthorsDelimStandard'     => ', ',
          'BetweenAuthorsDelimLastAuthor'   => ' and ',
          'AuthorsInitialsDelimFirstAuthor' => ' ',
          'AuthorsInitialsDelimStandard'    => ' ',
          'betweenInitialsDelim'            => '. ',
          'initialsBeforeAuthorFirstAuthor' => TRUE,
          'initialsBeforeAuthorStandard'    => TRUE,
          'shortenGivenNames'               => FALSE,
          'numberOfAuthorsTriggeringEtAl'   => 10,
          'includeNumberOfAuthors'          => 10,
          'customStringAfterFirstAuthors'   => ' et al.',
          'encodeHTML'                      => TRUE,
        );

        $editor = theme('biblio_format_authors', array('contributors' => $editors, 'options' => $editor_options));
        $output .= ", Edited by " . $editor;
      }

      // Edition.
      if (!empty($node->biblio_edition) && !preg_match("/^(1|1st|first|one)( ed\.?| edition)?$/i", $node->biblio_edition)) {
        if (!preg_match("/[?!.][ \"<i>]*$/", $output)) {
          $output .= ".";
        }

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

        if (!preg_match("/( ed\.?| edition)$/i", $node->biblio_edition)) {
          $editionSuffix .= " ed.";
        }

        $output .= " " . $node->biblio_edition . $editionSuffix;
      }

      // Volume.
      if (!empty($node->biblio_volume)) {
        if (!preg_match("/[?!.][ \"<i>]*$/", $output)) {
          $output .= ".";
        }

        $output .= " Vol. " . $node->biblio_volume;
      }

      // If there's either a full or an abbreviated series title.
      if (!empty($node->biblio_alternate_title) or !empty($node->biblio_tertiary_title)) {
        if (!preg_match("/[?!.][ \"<i>]*$/", $output)) {
          $output .= ".";
        }

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
          $output .= " ";
        }

        // Series volume.
        if (!empty($node->biblio_volume)) {
          $output .= $node->biblio_volume;
        }

        // If (!empty($node->biblio_issue))      // series issue (I'm not really sure if -- for this cite style -- the series issue should be rather omitted here)
        //       $output .= ", no. " . $node->biblio_issue; // is it correct to format series issues similar to journal article issues?
      }

      // Thesis.
      if (!empty($node->thesis)) {
        if (!preg_match("/[?!.][ \"" . $markupPatternsArray["italic-suffix"] . "]*$/", $output)) {
          $output .= ".";
        }

        $output .= " " . $node->thesis;
        $output .= ", " . $node->biblio_publisher;
      }
      // Not a thesis.
      else {
        if (!preg_match("/[?!.][ \"" . $markupPatternsArray["italic-suffix"] . "]*$/", $output)) {
          $output .= ".";
        }

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

      // If ($node->online_publication == "yes") // this record refers to an online article
      //      {
      //        // append an optional string (given in 'online_citation') plus the current date and the DOI (or URL):
      //
      //        $today = date("F j, Y");
      //
      //        if (!empty($node->online_citation))      // online_citation
      //        {
      //          if (!preg_match("/\. *$/", $output))  $output .= ".";
      //
      //          $output .= " " . $node->online_citation;
      //        }
      //
      //        if (!empty($node->doi))      // doi
      //        {
      //          if (!preg_match("/\. *$/", $output)) $output .= ".";
      //
      //          if ($encodeHTML)
      //          $output .= " " . encodeHTML("http://dx.doi.org/" . $node->doi) . " (accessed " . $today . ")";
      //          else
      //          $output .= " " . "http://dx.doi.org/" . $node->doi . " (accessed " . $today . ")";
      //        }
      //        elseif (!empty($node->biblio_url))      // url
      //        {
      //          if (!preg_match("/\. *$/", $output)) $output .= ".";
      //
      //          if ($encodeHTML)
      //          $output .= " " . encodeHTML($node->biblio_url) . " (accessed " . $today . ")";
      //          else
      //          $output .= " " . $node->biblio_url . " (accessed " . $today . ")";
      //        }
      //
      //      }.
      if (!preg_match("/\. *$/", $output)) {
        $output .= ".";
      }
      break;
  }

  return filter_xss($output, biblio_get_allowed_tags());
}
