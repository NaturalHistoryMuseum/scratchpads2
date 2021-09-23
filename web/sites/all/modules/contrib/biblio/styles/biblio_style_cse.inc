<?php

/**
 * @file
 * Council of Science Editors (CSE) style.
 */

/**
 * Get the style information.
 *
 * @return string[]
 *   The name of the style keyed with the abbreviation.
 */
function biblio_style_cse_info() {
  return array(
    'cse' => 'Council of Science Editors (CSE)',
  );
}

/**
 * Return an array of author options.
 *
 * @return array
 *   The author options array.
 */
function biblio_style_cse_author_options() {
  $author_options = array(
    // 4.
    'BetweenAuthorsDelimStandard'     => ', ',
    // 5.
    'BetweenAuthorsDelimLastAuthor'   => ', ',
    // 7.
    'AuthorsInitialsDelimFirstAuthor' => ' ',
    // 8.
    'AuthorsInitialsDelimStandard'    => ' ',
    // 9.
    'betweenInitialsDelim'            => '',
    // 10.
    'initialsBeforeAuthorFirstAuthor' => FALSE,
    // 11.
    'initialsBeforeAuthorStandard'    => FALSE,
    // 12.
    'shortenGivenNames'               => TRUE,
    // 13.
    'numberOfAuthorsTriggeringEtAl'   => 10,
    // 14.
    'includeNumberOfAuthors'          => 10,
    // 15.
    'customStringAfterFirstAuthors'   => ' et al.',
    'encodeHTML'                      => TRUE,
  );
  return $author_options;
}

/**
 * Apply a bibliographic style to the node.
 *
 * @param object $node
 *   An object containing the node data to render.
 *
 * @return string
 *   The styled biblio entry.
 */
function biblio_style_cse($node) {
  module_load_include('inc', 'biblio', '/includes/biblio.contributors');
  $output = $authors = '';
  if (!empty($node->biblio_contributors)) {
    $author_options = biblio_style_cse_author_options();
    $primary_authors = biblio_get_contributor_category($node->biblio_contributors, 1);
    $editors = biblio_get_contributor_category($node->biblio_contributors, 2);
    $corp_authors = biblio_get_contributor_category($node->biblio_contributors, 5);
    if (!empty($primary_authors)) {
      $authors = theme('biblio_format_authors', array('contributors' => $primary_authors, 'options' => $author_options));
    }
    // If no authors substitute corp author if available.
    if (empty($authors) && !empty($corp_authors)) {
      foreach ($corp_authors as $rank => $author) {
        $authors .= (empty($authors)) ? '' : ', ';
        $authors .= (variable_get('biblio_author_links', 1)) ? theme('biblio_author_link', array('author' => $author)) : $author['name'];

      }
    }
  }
  if (empty($authors)) {
    // Use anonymous if we still have nothing.
    $authors = '[' . t('Anonymous') . ']';
  }
  if (!empty($node->biblio_citekey) && (variable_get('biblio_display_citation_key', 0))) {
    $output .= '[' . check_plain($node->biblio_citekey) . '] ';
  }
  $output .= '<span class="biblio-authors">' . $authors . "</span>.&nbsp; \n";

  switch ($node->biblio_type) {
    default:
      if (isset($node->biblio_year)) {
        $output .= check_plain($node->biblio_year) . ".&nbsp;&nbsp;";
      }
      $output .= '<span class="biblio-title">';
      $url = biblio_get_title_url_info($node);
      $output .= l($node->title, $url['link'], $url['options']);
      // If the title ends in a question mark, don't put a period after it.
      $output .= (strpos($node->title, '?')) ? " </span>" : ". </span>";
      $output .= (!empty($node->biblio_secondary_title)) ? check_plain($node->biblio_secondary_title) . '. ' : '';
      $output .= (!empty($node->biblio_volume)) ? check_plain($node->biblio_volume) : '';
      $output .= (!empty($node->biblio_issue)) ? '(' . check_plain($node->biblio_issue) . ')' : '';
      $output .= (!empty($node->biblio_pages)) ? ':' . str_replace(" ", "", check_plain($node->biblio_pages)) . '.' : '';
  }

  return filter_xss($output, biblio_get_allowed_tags());
}
