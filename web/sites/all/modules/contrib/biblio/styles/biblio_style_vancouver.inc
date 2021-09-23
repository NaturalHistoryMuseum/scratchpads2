<?php

/**
 * @file
 * The Vancouver style.
 *
 * Original File: ./cite/styles/cite_Vancouver.php.
 * Original Author(s):  Matthias Steffens <mailto:refbase@extracts.de>.
 * This Vancouver style was modeled after these resources:
 * <http://www.library.uq.edu.au/training/citation/vancouv.pdf>
 * <http://library.curtin.edu.au/research_and_information_skills/referencing/vancouver.pdf>
 * <http://www.icmje.org/index.html> citing: <https://www.nlm.nih.gov/citingmedicine/>
 * <https://www.nlm.nih.gov/bsd/uniform_requirements.html>
 * <http://library.sun.ac.za//eng/help/infolit2002/bibvancouver.htm>
 * Based on 'cite_AMA.php'
 * NOTES: - In the Vancouver style, the reference list is arranged numerically in the order in which references are cited in the text.
 *          This isn't currently handled by this style
 *        - For conference proceedings, you'll currently need to add the place & date of the conference in the proceedings title field
 *          (e.g. "Proceedings of the 5th Germ Cell Tumour Conference; 2001 Sep 13-15; Leeds, UK").
 * @todo: - abstracts, newspaper/magazine articles, patents & reports?
 *       - arrange references numerically
 *       - for newspaper articles, only the beginning page number of an article should be included (see: <https://www.ncbi.nlm.nih.gov/books/bv.fcgi?rid=citmed.section.41496#41607>)
 *       - where to put (and how to format) editors of whole books that also have an author?
 *       - see also inline comments labeled with @todo (and NOTE)
 * Modified for use in biblio by Ron Jerome.
 */

/**
 * Get the style information.
 *
 * @return
 *   The name of the style
 */
function biblio_style_vancouver_info() {
  return array(
    'vancouver' => 'Vancouver',
  );
}

/**
 *
 */
function biblio_style_vancouver_author_options() {
  $author_options = array(
    'BetweenAuthorsDelimStandard'       => ', ',
    'BetweenAuthorsDelimLastAuthor'     => ', ',
    'AuthorsInitialsDelimFirstAuthor'   => ' ',
    'AuthorsInitialsDelimStandard'      => ' ',
    'betweenInitialsDelim'              => '',
    'initialsBeforeAuthorFirstAuthor'   => FALSE,
    'initialsBeforeAuthorStandard'      => FALSE,
    'numberOfInitialsToKeep'            => 2,
    'shortenGivenNames'                 => TRUE,
    'numberOfAuthorsTriggeringEtAl'     => 6,
    'includeNumberOfAuthors'            => 6,
    'customStringAfterFirstAuthors'     => ', et al.',
    'encodeHTML'                        => TRUE,
  );

  return $author_options;
}

/**
 *
 */
function biblio_style_vancouver($node) {
  module_load_include('inc', 'biblio', '/includes/biblio.contributors');
  $output = $authors = $editor = '';
  $markupPatternsArray = array(
    "italic-prefix" => '<i>',
    "italic-suffix" => '<\/i>',
    "endash" => '-',
  );
  $author_options = biblio_style_vancouver_author_options();
  $primary_authors = biblio_get_contributor_category($node->biblio_contributors, 1);
  $editors = biblio_get_contributor_category($node->biblio_contributors, 2);
  // $corp_authors = biblio_get_contributor_category($node->biblio_contributors, 5);.
  if (!empty($primary_authors)) {
    $authors = theme('biblio_format_authors', array('contributors' => $primary_authors, 'options' => $author_options));
  }
  // $editors = theme('biblio_format_authors', array('contributors' => $node->biblio_contributors->get_category(2), 'options' => $author_options, 'inline' => $inline));
  // if (empty($authors)) $authors = theme('biblio_authors', $node->biblio_contributors->get_category(5), 'mla', 5, $inline);  // if no authors substitute corp author if available
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
        $output .= '<span class="biblio-title-vancouver">';
        $url = biblio_get_title_url_info($node);
        $output .= l($node->title, $url['link'], $url['options']);
        $output .= "</span>";
        if (!preg_match("/[?!.]$/", $node->title)) {
          $output .= ".";
        }
      }

      // From here on we'll assume that at least either the 'author' or the 'title' field did contain some contents
      // if this is not the case, the output string will begin with a space. However, any preceding/trailing whitespace will be removed at the cleanup stage (see below)
      // Abbreviated journal name.
      if (!empty($node->biblio_alternate_title)) {
        // No punctuation marks are used in the abbreviated journal name, just spaces (@todo: smarten regex pattern)
        $output .= " " . preg_replace("/\./", "", $node->biblio_alternate_title);
      }

      // If there's no abbreviated journal name, we'll use the full journal name instead:
      // publication (= journal) name.
      elseif (!empty($node->biblio_secondary_title)) {
        $output .= " " . $node->biblio_secondary_title;
      }

      // This record refers to an online publication.
      if (isset($node->online_publication)) {
        // NOTE: some of the above mentioned resources use "[serial online]", "[serial on the Internet]" or just "[online]" instead.
        $output .= " [Internet]";
      }

      // NOTE: the formatting of year/volume/issue is meant for journal articles (@todo: newspaper/magazine articles)
      // year.
      if (!empty($node->biblio_year)) {
        $output .= ". " . $node->biblio_year;
      }

      // Append the current date if this record refers to an online publication.
      if (isset($node->online_publication)) {
        $output .= " [cited " . date("Y M j") . "]";
      }

      if (!empty($node->biblio_volume) || !empty($node->biblio_issue)) {
        $output .= ";";
      }

      // Volume (=month)
      if (!empty($node->biblio_volume)) {
        $output .= $node->biblio_volume;
      }

      // Issue (=day)
      if (!empty($node->biblio_issue)) {
        $output .= "(" . $node->biblio_issue . ")";
      }

      // Pages.
      if (!empty($node->biblio_pages)) {
        // Only add ": " if either year, volume, issue, abbrev_journal or publication isn't empty.
        if (!empty($node->biblio_year) || !empty($node->biblio_volume) || !empty($node->biblio_issue) || !empty($node->biblio_alternate_title) || !empty($node->biblio_secondary_title)) {
          $output .= ":";
        }

        $output .= theme('biblio_page_number', array(
          'orig_page_info' => $node->biblio_pages,
          'page_range_delim' => $markupPatternsArray["endash"],
          'shorten_page_range_end' => TRUE,
        // Function 'formatPageInfo()' is defined in 'cite.inc.php'.
        ));
      }

      // This record refers to an online publication.
      if (isset($node->online_publication)) {
        // Append an optional string (given in 'online_citation') plus the DOI (or URL):
        // online_citation.
        if (!empty($node->online_citation)) {
          // Only add ":" or "," if either year, volume, issue, abbrev_journal or publication isn't empty.
          if (!empty($node->biblio_year) || !empty($node->biblio_volume) || !empty($node->biblio_issue) || !empty($node->biblio_alternate_title) || !empty($node->biblio_secondary_title)) {
            if (empty($node->biblio_pages)) {
              // Print instead of pages.
              $output .= ":";
            }
            // Append to pages (@todo: not sure whether this is correct)
            else {
              $output .= ";";
            }
          }

          $output .= $node->online_citation;
        }

        // Doi OR url.
        if (!empty($node->doi) || !empty($node->url)) {
          // Only add "." if online_citation isn't empty, or else if either year, volume, issue, abbrev_journal or publication isn't empty.
          if (!empty($node->online_citation) or (empty($node->online_citation) and (!empty($node->biblio_year) || !empty($node->biblio_volume) || !empty($node->biblio_issue) || !empty($node->biblio_alternate_title) || !empty($node->biblio_secondary_title)))) {
            $output .= ".";
          }

          // NOTE: some of the above mentioned resources use "Available from: URL:http://..." instead.
          $output .= " Available from: " . $markupPatternsArray["underline-prefix"];

          // Doi.
          if (!empty($node->doi)) {
            $uri = "http://dx.doi.org/" . $node->doi;
          }
          // Url.
          else {
            $uri = $node->url;
          }

          if ($encodeHTML) {
            $output .= encodeHTML($uri);
          }
          else {
            $output .= $uri;
          }

          $output .= $markupPatternsArray["underline-suffix"];
        }
      }

      if (!preg_match("/\. *$/", $output) and (!isset($node->online_publication))) {
        // NOTE: the examples in the above mentioned resources differ wildly w.r.t. whether the closing period should be omitted for online publications.
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
        $output .= '<span class="biblio-title-vancouver">';
        $url = biblio_get_title_url_info($node);
        $output .= l($node->title, $url['link'], $url['options']);
        $output .= "</span>";
        if (!preg_match("/[?!.]$/", $node->title)) {
          $output .= ".";
        }
      }

      // Editor.
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
          'includeNumberOfAuthors'            => 6,
          'customStringAfterFirstAuthors'     => ' et al.',
          'encodeHTML'                        => TRUE,
        );

        $editor = theme('biblio_format_authors', array('contributors' => $editors, 'options' => $editor_options));

        $output .= " In: " . $editor . ", ";
        // There are at least two editors (separated by ';')
        if (count($editors) > 1) {
          $output .= "editors";
          // there's only one editor (or the editor field is malformed with multiple editors but missing ';' separator[s])
        }
        else {
          $output .= "editor";
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

        $output .= " " . $publication;
      }

      // Volume.
      if (!empty($node->biblio_volume)) {
        if (!preg_match("/[?!.][ \"" . $markupPatternsArray["italic-suffix"] . "]*$/", $output)) {
          $output .= ".";
        }

        // @todo: not sure whether this is correct.
        $output .= " Vol " . $node->biblio_volume;
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
        // // for places in the USA, format any two-letter postal code for the state (i.e. ensure upper case & wrap in parens, eg. "Boca Raton (FL)"):
        //        if (preg_match("/(.+?)[$punct$space]+($uspsStateAbbreviations)[$punct$space]*$/i$patternModifiers", $node->biblio_place_published))
        //          $output .= " " . preg_replace("/(.+?)[$punct$space]+($uspsStateAbbreviations)[$punct$space]*$/ie$patternModifiers", "'\\1 ('.strtoupper('\\2').')'", $node->biblio_place_published);
        //        else.
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
        $output .= ". " . theme_biblio_page_number($node->biblio_pages, $markupPatternsArray["endash"], "p. ", "p. ", "", "", "", "", TRUE);
      }
      $output .= ". " . theme('biblio_page_number', array(
        'orig_page_info' => $node->biblio_pages,
        'page_range_delim' => $markupPatternsArray["endash"],
        'single_page_prefix' => "p. ",
        'page_range_prefix' => "p. ",
        'shorten_page_range_end' => TRUE,
      // Function 'formatPageInfo()' is defined in 'cite.inc.php'.
      ));

      // If there's either a full or an abbreviated series title.
      if (!empty($node->biblio_alternate_title) or !empty($node->biblio_tertiary_title)) {
        if (!preg_match("/[?!.][ \"" . $markupPatternsArray["italic-suffix"] . "]*$/", $output)) {
          $output .= ".";
        }

        $output .= " (";

        // Abbreviated series title.
        if (!empty($node->biblio_alternate_title)) {
          // No punctuation marks are used in the abbreviated series title, just spaces (@todo: smarten regex pattern)
          $output .= preg_replace("/\./", "", $node->biblio_alternate_title);
        }

        // If there's no abbreviated series title, we'll use the full series title instead:
        // full series title.
        elseif (!empty($node->biblio_tertiary_title)) {
          $output .= $node->biblio_tertiary_title;
        }

        if (!empty($node->biblio_volume)||!empty($node->biblio_issue)) {
          $output .= "; ";
        }

        // Series volume.
        if (!empty($node->biblio_volume)) {
          $output .= "vol " . $node->biblio_volume;
        }

        if (!empty($node->biblio_volume) && !empty($node->biblio_issue)) {
          // @todo: not sure whether this is correct.
          $output .= "; ";
        }

        // Series issue (I'm not really sure if -- for this cite style -- the series issue should be rather omitted here)
        if (!empty($node->biblio_issue)) {
          // Since a series volume should be prefixed with "vol", is it correct to prefix series issues with "no"?
          $output .= "no " . $node->biblio_issue;
        }

        $output .= ")";
      }

      if (!preg_match("/\. *$/", $output)) {
        $output .= ".";
      }

      break;

    // All other types.
    default:
      // @todo.
      // @code
      //        if (preg_match("[ \r\n]*\(ed\)", $node->author)) // single editor
      //          $author = $author . ", ed";
      //        elseif (preg_match("[ \r\n]*\(eds\)", $node->author)) // multiple editors
      //          $author = $author . ", eds";
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

        // @todo: book/volume/report/etc titles should be formatted in heading caps, however, this doesn't yet work correctly if the publication title contains HTML entities.
        $output .= '<span class="biblio-title-vancouver">';
        $url = biblio_get_title_url_info($node);
        $output .= l($node->title, $url['link'], $url['options']);
        $output .= "</span>";
      }
      // For software, add software label.
      if ($node->type == "Software") {
        $output .= " [computer program]";
      }

      // This record refers to an online publication (online theses will be handled further down below)
      if (isset($node->online_publication) and empty($node->thesis)) {
        // NOTE: some of the above mentioned resources use "[monograph online]", "[monograph on the Internet]" or just "[online]" instead.
        $output .= " [Internet]";
      }

      // Volume.
      if (!empty($node->biblio_volume) and ($node->type != "Software")) {
        if (!preg_match("/[?!.][ \"" . $markupPatternsArray["italic-suffix"] . "]*$/", $output)) {
          $output .= ".";
        }

        // @todo: not sure whether this is correct.
        $output .= " Vol " . $node->biblio_volume;
      }

      // Edition.
      if (!empty($node->biblio_edition)) {
        if (!preg_match("/[?!.][ \"" . $markupPatternsArray["italic-suffix"] . "]*$/", $output)) {
          $output .= ".";
        }

        // Software edition (=version)
        if ($node->type == "Software") {
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
          $output .= ", editors";
          // there's only one editor (or the editor field is malformed with multiple editors but missing ';' separator[s])
        }
        else {
          $output .= ", editor";
        }
      }

      // Thesis.
      if (!empty($node->thesis)) {
        // @todo: do we need to use the term "[dissertation]" instead of "[Ph.D. thesis]", etc? What about other thesis types then?
        $output .= " [" . $node->thesis;

        // This record refers to an online thesis.
        if (isset($node->online_publication)) {
          $output .= " on the Internet]";
        }
        else {
          $output .= "]";
        }
      }

      if (!preg_match("/[?!.][ \"" . $markupPatternsArray["italic-suffix"] . "]*$/", $output)) {
        $output .= ".";
      }

      // Place.
      if (!empty($node->biblio_place_published)) {
        // // for places in the USA, format any two-letter postal code for the state (i.e. ensure upper case & wrap in parentheses, eg. "Boca Raton (FL)"):
        //        if (preg_match("/(.+?)[$punct$space]+($uspsStateAbbreviations)[$punct$space]*$/i$patternModifiers", $node->biblio_place_published))
        //          $output .= " " . preg_replace("/(.+?)[$punct$space]+($uspsStateAbbreviations)[$punct$space]*$/ie$patternModifiers", "'\\1 ('.strtoupper('\\2').')'", $node->biblio_place_published);
        //        else.
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

      // Year.
      if (!empty($node->biblio_year)) {
        $output .= " " . $node->biblio_year;
      }

      // For software, volume (=month) and issue (=day) information is printed after the year (@todo: not sure whether this is correct)
      if ($node->type == "Software") {
        // Volume (=month)
        if (!empty($node->biblio_volume)) {
          $output .= " " . $node->biblio_volume;
        }

        // Issue (=day)
        if (!empty($node->biblio_issue)) {
          $output .= " " . $node->biblio_issue;
        }
      }

      // Append the current date if this record refers to an online publication.
      if (isset($node->online_publication)) {
        $output .= " [cited " . date("Y M j") . "]";
      }

      // If there's either a full or an abbreviated series title.
      if (!empty($node->biblio_alternate_title) or !empty($node->biblio_tertiary_title)) {
        if (!preg_match("/[?!.][ \"" . $markupPatternsArray["italic-suffix"] . "]*$/", $output)) {
          $output .= ".";
        }

        $output .= " (";

        // Abbreviated series title.
        if (!empty($node->biblio_alternate_title)) {
          // No punctuation marks are used in the abbreviated series title, just spaces (@todo: smarten regex pattern)
          $output .= preg_replace("/\./", "", $node->biblio_alternate_title);
        }

        // If there's no abbreviated series title, we'll use the full series title instead:
        // full series title.
        elseif (!empty($node->biblio_tertiary_title)) {
          $output .= $node->biblio_tertiary_title;
        }

        if (!empty($node->biblio_volume)||!empty($node->biblio_issue)) {
          $output .= "; ";
        }

        // Series volume.
        if (!empty($node->biblio_volume)) {
          $output .= "vol " . $node->biblio_volume;
        }

        if (!empty($node->biblio_volume) && !empty($node->biblio_issue)) {
          // @todo: not sure whether this is correct.
          $output .= "; ";
        }

        // Series issue (I'm not really sure if -- for this cite style -- the series issue should be rather omitted here)
        if (!empty($node->biblio_issue)) {
          // Since a series volume should be prefixed with "vol", is it correct to prefix series issues with "no"?
          $output .= "no " . $node->biblio_issue;
        }

        $output .= ")";
      }

      // This record refers to an online publication, or a computer program/software.
      if (isset($node->online_publication) || $node->type == "Software") {
        // Append an optional string (given in 'online_citation') plus the DOI (or URL):
        // online_citation.
        if (!empty($node->online_citation)) {
          if (!preg_match("/\. *$/", $output)) {
            $output .= ".";
          }

          $output .= $node->online_citation;
        }

        // Doi OR url.
        if (!empty($node->doi) || !empty($node->url)) {
          if (!preg_match("/\. *$/", $output)) {
            $output .= ".";
          }

          // NOTE: some of the above mentioned resources use "Available from: URL:http://..." instead.
          $output .= " Available from: " . $markupPatternsArray["underline-prefix"];

          // Doi.
          if (!empty($node->doi)) {
            $uri = "http://dx.doi.org/" . $node->doi;
          }
          // Url.
          else {
            $uri = $node->url;
          }

          if ($encodeHTML) {
            $output .= encodeHTML($uri);
          }
          else {
            $output .= $uri;
          }

          $output .= $markupPatternsArray["underline-suffix"];
        }
      }

      if (!preg_match("/\. *$/", $output) and (!isset($node->online_publication)) and ($node->type != "Software")) {
        // NOTE: the examples in the above mentioned resources differ wildly w.r.t. whether the closing period should be omitted for online publications.
        $output .= ".";
      }
      break;
  }

  return filter_xss($output, biblio_get_allowed_tags());
}
