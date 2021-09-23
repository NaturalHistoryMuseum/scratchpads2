<?php

/**
 * @file
 * AMA style.
 *
 * Original File:       ./cite/styles/cite_AMA.php
 * Original Author(s):  Matthias Steffens <mailto:refbase@extracts.de> and
 *                      Richard Karnesky <mailto:karnesky@gmail.com>
 *
 * This is a citation style file (which must reside within the 'cite/styles/' sub-directory of your refbase root directory). It contains a
 * version of the 'citeRecord()' function that outputs a reference list from selected records according to the citation style used by
 * the American Medical Association (AMA)
 *
 * based on 'cite_Chicago.php'
 *
 * @todo: - abstracts, conference proceedings, patents, reports
 *       - book/volume/report/etc titles should be formatted in heading caps
 *       - don't add a dot if the abbreviated journal (or series title) ends with a dot!
 *
 * Modified for use in biblio by Ron Jerome.
 */

/**
 * Get the style information.
 *
 * @return
 *   The name of the style
 */
function biblio_style_ama_info() {
  return array(
    'ama' => 'American Medical Association (AMA)',
  );
}

/**
 *
 */
function biblio_style_ama_author_options() {
  $author_options = array(
    'BetweenAuthorsDelimStandard'       => ', ',
    'BetweenAuthorsDelimLastAuthor'     => ', ',
    'AuthorsInitialsDelimFirstAuthor'   => ' ',
    'AuthorsInitialsDelimStandard'      => ' ',
    'betweenInitialsDelim'              => '',
    'initialsBeforeAuthorFirstAuthor'   => FALSE,
    'initialsBeforeAuthorStandard'      => FALSE,
    'shortenGivenNames'                 => TRUE,
    'numberOfAuthorsTriggeringEtAl'     => 6,
    'includeNumberOfAuthors'            => 3,
    'customStringAfterFirstAuthors'     => ', et al.',
    'encodeHTML'                        => TRUE,
  );

  return $author_options;
}

/**
 *
 */
function biblio_style_ama($node) {
  module_load_include('inc', 'biblio', '/includes/biblio.contributors');
  $output = '';
  $markupPatternsArray = array(
    "italic-prefix" => "<i>",
    "italic-suffix" => "<\/i>",
    "endash" => '-',
  );
  $author_options = biblio_style_ama_author_options();
  $authors = biblio_get_contributor_category($node->biblio_contributors, 1);
  $editors = biblio_get_contributor_category($node->biblio_contributors, 2);

  if (!empty($authors)) {
    $authors = theme('biblio_format_authors', array('contributors' => $authors, 'options' => $author_options));
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
        $output .= "</span>";
      }

      // Title.
      if (!empty($node->title)) {
        if (!empty($authors)) {
          $output .= " ";
        }
        $output .= '"';
        $output .= '<span class="biblio-title-ama">';
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
        $output .= " <i>" . $node->biblio_alternate_title . "</i>";
      }

      // If there's no abbreviated journal name, we'll use the full journal name
      // publication (= journal) name.
      elseif (!empty($node->biblio_secondary_title)) {
        $output .= " <i>" . $node->biblio_secondary_title . "</i>";
      }

      // For newspaper articles (and magazine articles if w/o volume number), volume (=month) and issue (=day) information is printed before the year.
      if ($node->biblio_type == 105 || $node->biblio_type == 106 and !preg_match("/^\d+$/", $node->biblio_volume)) {
        // Volume (=month)
        if (!empty($node->biblio_volume)) {
          $output .= ". " . $node->biblio_volume;
        }

        // Issue (=day)
        if (!empty($node->biblio_issue)) {
          $output .= " " . $node->biblio_issue;
        }

        // Year.
        if (!empty($node->biblio_year)) {
          $output .= ", " . $node->biblio_year;
        }
      }
      // Journal article (or a magazine article with volume numbers)
      else {
        // Year.
        if (!empty($node->biblio_year)) {
          $output .= ". " . $node->biblio_year;
        }

        if (!empty($node->biblio_volume) || !empty($node->biblio_issue)) {
          $output .= ";";
        }

        // Volume.
        if (!empty($node->biblio_volume)) {
          $output .= $node->biblio_volume;
        }

        // Issue.
        if (!empty($node->biblio_issue)) {
          $output .= "(" . $node->biblio_issue . ")";
        }
      }

      // Pages.
      if (!empty($node->biblio_pages)) {
        // Only add ": " if either year, volume, issue, abbrev_journal or publication isn't empty.
        if (!empty($node->biblio_year) || !empty($node->biblio_volume) || !empty($node->biblio_issue) || !empty($node->biblio_alternate_title) || !empty($node->biblio_secondary_title)) {
          $output .= ":";
        }

        // Function 'formatPageInfo()' is defined in 'cite.inc.php'.
        $output .= theme('biblio_page_number', array('orig_page_info' => $node->biblio_pages, 'page_range_delim' => $markupPatternsArray["endash"]));
      }

      // This record refers to an online article.
      if (isset($node->online_publication)) {
        // Append an optional string (given in 'online_citation') plus the current date and the DOI (or URL):
        $today = date("F j, Y");

        // online_citation.
        if (!empty($row['online_citation'])) {
          // Only add ":" or "," if either year, volume, issue, abbrev_journal or publication isn't empty.
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

          $output .= $row['online_citation'];
        }

        // Doi.
        if (!empty($row['doi'])) {
          // Only add "." if online_citation isn't empty, or else if either year, volume, issue, abbrev_journal or publication isn't empty.
          if (!empty($row['online_citation']) or (empty($row['online_citation']) and (!empty($node->biblio_year) || !empty($node->biblio_volume) || !empty($node->biblio_issue) || !empty($node->biblio_alternate_title) || !empty($node->biblio_secondary_title)))) {
            $output .= ".";
          }

          if ($encodeHTML) {
            $output .= " " . encodeHTML("http://dx.doi.org/" . $row['doi']) . ". Accessed " . $today;
          }
          else {
            $output .= " " . "http://dx.doi.org/" . $row['doi'] . ". Accessed " . $today;
          }
        }
        // Url.
        elseif (!empty($row['url'])) {
          // Only add "." if online_citation isn't empty, or else if either year, volume, issue, abbrev_journal or publication isn't empty.
          if (!empty($row['online_citation']) or (empty($row['online_citation']) and (!empty($node->biblio_year) || !empty($node->biblio_volume) || !empty($node->biblio_issue) || !empty($node->biblio_alternate_title) || !empty($node->biblio_secondary_title)))) {
            $output .= ".";
          }

          if ($encodeHTML) {
            $output .= " " . encodeHTML($row['url']) . ". Accessed " . $today;
          }
          else {
            $output .= " " . $row['url'] . ". Accessed " . $today;
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
        $output .= '<span class="biblio-title-ama">';
        $url = biblio_get_title_url_info($node);
        $output .= l($node->title, $url['link'], $url['options']);
        $output .= "</span>";
        $output .= '</i>';
        if (!preg_match("/[?!.]$/", $node->title)) {
          $output .= ".";
        }
        $output .= '"';
      }

      if (!empty($editors)) {
        $editor_options = array(
          'BetweenAuthorsDelimStandard'       => ', ',
          'BetweenAuthorsDelimLastAuthor'     => ', ',
          'AuthorsInitialsDelimFirstAuthor'   => ' ',
          'AuthorsInitialsDelimStandard'      => ' ',
          'betweenInitialsDelim'              => '',
          'initialsBeforeAuthorFirstAuthor'   => FALSE,
          'initialsBeforeAuthorStandard'      => FALSE,
          'shortenGivenNames'                 => TRUE,
          'numberOfAuthorsTriggeringEtAl'     => 6,
          'includeNumberOfAuthors'            => 3,
          'customStringAfterFirstAuthors'     => ' et al.',
          'encodeHTML'                        => TRUE,
        );

        $editor = theme('biblio_format_authors', array('contributors' => $editors, 'options' => $editor_options));

        $output .= " In: " . $editor . ", ";
        // There are at least two editors (separated by ';')
        if (count($editors) > 1) {
          $output .= "eds";
          // there's only one editor (or the editor field is malformed with multiple editors but missing ';' separator[s])
        }
        else {
          $output .= "ed";
        }
      }

      $publication = preg_replace("/[ \r\n]*\(Eds?:[^\)\r\n]*\)/", "", $node->biblio_secondary_title);
      // Publication.
      if (!empty($publication)) {
        if (!preg_match("/[?!.] *$/", $output)) {
          $output .= ".";
        }

        if (empty($editor)) {
          $output .= " In:";
        }

        // @todo: container titles should be formatted in heading caps, however, this doesn't yet work correctly if the publication title contains HTML entities.
        $output .= " <i>" . $publication . "</i>";
        // $output .= " " . $markupPatternsArray["italic-prefix"] . changeCase("heading", $publication) . $markupPatternsArray["italic-suffix"]; // function 'changeCase()' is defined in 'include.inc.php'.
      }

      // Volume.
      if (!empty($node->biblio_volume)) {
        if (!preg_match("/[?!.][ \"" . $markupPatternsArray["italic-suffix"] . "]*$/", $output)) {
          $output .= ".";
        }

        $output .= " Vol. " . $node->biblio_volume;
      }

      // Edition.
      if (!empty($node->biblio_edition) && !preg_match("/^(1|1st|first|one)( ed\.?| edition)?$/i", $node->biblio_edition)) {
        if (!preg_match("/[?!.][ \"" . $markupPatternsArray["italic-suffix"] . "]*$/", $output)) {
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

      // Year.
      if (!empty($node->biblio_year)) {
        $output .= "; " . $node->biblio_year;
      }

      // Pages.
      if (!empty($node->biblio_pages)) {
        // Function 'formatPageInfo()' is defined in 'cite.inc.php'.
        $output .= ":" . theme('biblio_page_number', $node->biblio_pages, $markupPatternsArray["endash"]);
      }

      // If there's either a full or an abbreviated series title.
      if (!empty($node->biblio_alternate_title) or !empty($node->biblio_tertiary_title)) {
        if (!preg_match("/[?!.][ \"" . $markupPatternsArray["italic-suffix"] . "]*$/", $output)) {
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

        // Series issue (I'm not really sure if -- for this cite style -- the series issue should be rather omitted here)
        if (!empty($node->biblio_issue)) {
          // Is it correct to format series issues similar to journal article issues?
          $output .= "(" . $node->biblio_issue . ")";
        }
      }

      if (!preg_match("/\. *$/", $output)) {
        $output .= ".";
      }

      break;

    // All other types.
    default:
      // @todo
      // @code
      //        if (ereg("[ \r\n]*\(ed\)", $node->author)) // single editor
      //          $author = $author . ", ed";
      //        elseif (ereg("[ \r\n]*\(eds\)", $node->author)) // multiple editors
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
        if (!empty($authors)) {
          $output .= " ";
        }

        $output .= '<i>';
        // @todo: book/volume/report/etc titles should be formatted in heading caps, however, this doesn't yet work correctly if the publication title contains HTML entities.
        $output .= '<span class="biblio-title-ama">';
        $url = biblio_get_title_url_info($node);
        $output .= l($node->title, $url['link'], $url['options']);
        $output .= "</span>";
        $output .= '</i>';
      }
      // For software, add software label.
      if ($node->biblio_type == "Software") {
        $output .= " [computer program]";
      }

      // Volume.
      if (!empty($node->biblio_volume) and ($node->biblio_type != "Software")) {
        if (!preg_match("/[?!.][ \"" . $markupPatternsArray["italic-suffix"] . "]*$/", $output)) {
          $output .= ".";
        }

        $output .= " Vol. " . $node->biblio_volume;
      }

      // Edition.
      if (!empty($node->biblio_edition)) {
        if (!preg_match("/[?!.][ \"" . $markupPatternsArray["italic-suffix"] . "]*$/", $output)) {
          $output .= ".";
        }

        // Software edition (=version)
        if ($row['type'] == "Software") {
          $output .= " Version " . $node->biblio_edition;
        }
        // Edition.
        elseif (!preg_match("/^(1|1st|first|one)( ed\.?| edition)?$/i", $node->biblio_edition)) {
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
      }

      // Editor (if different from author, see note above regarding the check for ' (ed)' or ' (eds)')
      if (!empty($editors)) {
        $editor_options = array(
          'BetweenAuthorsDelimStandard'     => ', ',
          'BetweenAuthorsDelimLastAuthor'   => ', ',
          'AuthorsInitialsDelimFirstAuthor' => ' ',
          'AuthorsInitialsDelimStandard'    => ' ',
          'betweenInitialsDelim'            => '',
          'initialsBeforeAuthorFirstAuthor' => FALSE,
          'initialsBeforeAuthorStandard'    => FALSE,
          'shortenGivenNames'               => TRUE,
          'numberOfAuthorsTriggeringEtAl'   => 6,
          'includeNumberOfAuthors'          => 3,
          'customStringAfterFirstAuthors'   => ' et al.',
          'encodeHTML'                      => TRUE,
        );

        $editor = theme('biblio_format_authors', array('contributors' => $editors, 'options' => $editor_options));
        if (!preg_match("/[?!.][ \"" . $markupPatternsArray["italic-suffix"] . "]*$/", $output)) {
          $output .= ".";
        }

        $output .= " " . $editor;
        // There are at least two editors (separated by ';')
        if (count($editors) > 1) {
          $output .= ", eds";
          // there's only one editor (or the editor field is malformed with multiple editors but missing ';' separator[s])
        }
        else {
          $output .= ", ed";
        }
      }

      // Thesis.
      if (!empty($row['thesis'])) {
        // @todo: do we need to use the term "[dissertation]" instead of "[Ph.D. thesis]", etc? What about other thesis types then?
        $output .= " [" . $row['thesis'] . "]";
      }

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

      $output .= ";";

      // For software, volume (=month) and issue (=day) information is printed before the year (similar to newspaper articles)
      if ($node->biblio_type == "Software") {
        // Volume (=month)
        if (!empty($node->biblio_volume)) {
          $output .= " " . $node->biblio_volume;
        }

        // Issue (=day)
        if (!empty($node->biblio_issue)) {
          $output .= " " . $node->biblio_issue;
        }

        $output .= ",";
      }

      // Year.
      if (!empty($node->biblio_year)) {
        $output .= " " . $node->biblio_year;
      }

      // If there's either a full or an abbreviated series title.
      if (!empty($node->biblio_alternate_title) or !empty($node->biblio_tertiary_title)) {
        if (!preg_match("/[?!.][ \"" . $markupPatternsArray["italic-suffix"] . "]*$/", $output)) {
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

        // Series issue (I'm not really sure if -- for this cite style -- the series issue should be rather omitted here)
        if (!empty($node->biblio_issue)) {
          // Is it correct to format series issues similar to journal article issues?
          $output .= "(" . $node->biblio_issue . ")";
        }
      }

      // If ($row['online_publication'] == "yes" || $row['type'] == "Software") // this record refers to an online article, or a computer program/software
      //      {
      //        // append an optional string (given in 'online_citation') plus the current date and the DOI (or URL):
      //
      //        $today = date("F j, Y");
      //
      //        if (!empty($row['online_citation']))      // online_citation
      //        {
      //          if (!preg_match("/\. *$/", $output)) $output .= ".";
      //
      //          $output .= $row['online_citation'];
      //        }
      //
      //        if (!empty($row['doi']))      // doi
      //        {
      //          if (!preg_match("/\. *$/", $output))  $output .= ".";
      //
      //          if ($encodeHTML)
      //          $output .= " " . encodeHTML("http://dx.doi.org/" . $row['doi']) . ". Accessed " . $today;
      //          else
      //          $output .= " " . "http://dx.doi.org/" . $row['doi'] . ". Accessed " . $today;
      //        }
      //        elseif (!empty($row['url']))      // url
      //        {
      //          if (!preg_match("/\. *$/", $output))  $output .= ".";
      //
      //          if ($encodeHTML)
      //          $output .= " " . encodeHTML($row['url']) . ". Accessed " . $today;
      //          else
      //          $output .= " " . $row['url'] . ". Accessed " . $today;
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
