<?php

/**
 * @file
 * IEEE style.
 *
 * A style closer to IEEE format
 * by Jeffrey Dwoskin
 * See http://standards.ieee.org/guides/style/section7.html#992
 */

/**
 * Implements biblio_style_STYLE_info().
 */
function biblio_style_ieee_info() {
  return array(
    'ieee' => 'Institute of Electrical and Electronics Engineers (IEEE)',
  );
}

/**
 * Implements biblio_style_STYLE_author_options().
 */
function biblio_style_ieee_author_options() {
  $author_options = array(
    // 4.
    'BetweenAuthorsDelimStandard'     => ', ',
    // 5.
    'BetweenAuthorsDelimLastAuthor'   => ', and ',
    // 7.
    'AuthorsInitialsDelimFirstAuthor' => ', ',
    // 8.
    'AuthorsInitialsDelimStandard'    => ' ',
    // 9.
    'betweenInitialsDelim'            => '. ',
    // 10.
    'initialsBeforeAuthorFirstAuthor' => FALSE,
    // 11.
    'initialsBeforeAuthorStandard'    => TRUE,
    // 12.
    'shortenGivenNames'               => TRUE,
    // 13.
    'numberOfAuthorsTriggeringEtAl'   => 10,
    // 14.
    'includeNumberOfAuthors'          => 10,
    // 15.
    'customStringAfterFirstAuthors'   => ', et al.',
    'encodeHTML'                      => TRUE,
  );
  return $author_options;
}

/**
 * Implements biblio_style_STYLE().
 */
function biblio_style_ieee($node) {
  module_load_include('inc', 'biblio', '/includes/biblio.contributors');
  $output = '';
  $author_options = biblio_style_ieee_author_options();
  $primary_authors = isset($node->biblio_contributors) ? biblio_get_contributor_category($node->biblio_contributors, 1) : NULL;
  if (!empty($primary_authors)) {
    $authors = theme('biblio_format_authors', array('contributors' => $primary_authors, 'options' => $author_options));
  }
  if (!empty($node->biblio_citekey) && (variable_get('biblio_display_citation_key', 0))) {
    $output .= '[' . check_plain($node->biblio_citekey) . '] ';
  }

  $output .= isset($authors) ? '<span class="biblio-authors">' . $authors . '</span>, ' : '';

  $url = biblio_get_title_url_info($node);
  if (!empty($node->biblio_secondary_title)) {
    $output .= '<span class="biblio-title">&quot;';
    $output .= l($node->title, $url['link'], $url['options']);
    $output .= '&quot;</span>, ';
    $output .= '<i>' . check_plain($node->biblio_secondary_title) . '</i>';
  }
  else {
    $output .= '<span class="biblio-title"><i>';
    $output .= l($node->title, $url['link'], $url['options']);
    $output .= '</i></span>';
  }
  if (!empty($node->biblio_edition)) {
    $output .= ', ' . check_plain($node->biblio_edition);
  }
  if (!empty($node->biblio_volume)) {
    $output .= ', vol. ' . check_plain($node->biblio_volume);
  }
  if (!empty($node->biblio_issue)) {
    $output .= ', issue ' . check_plain($node->biblio_issue);
  }
  if (!empty($node->biblio_number)) {
    $output .= ', no. ' . check_plain($node->biblio_number);
  }
  if (!empty($node->biblio_place_published)) {
    $output .= ', ' . check_plain($node->biblio_place_published);
  }
  if (!empty($node->biblio_publisher)) {
    $output .= (check_plain($node->biblio_place_published)) ? ', ' : ': ';
    $output .= check_plain($node->biblio_publisher);
  }
  // If a single page instead of a range, should use 'p.' instead of 'pp.';
  // ignoring.
  if (!empty($node->biblio_pages)) {
    $output .= ', pp. ' . check_plain($node->biblio_pages);
  }
  // If it is a book, year should go before pages instead; ignoring for
  // non-books, should also include month of publication (e.g. "Mar. 2006").
  // Use date instead of year if available.
  if (!empty($node->biblio_date)) {
    $output .= ', ' . check_plain($node->biblio_date);
  }
  if ((!empty($node->biblio_year) && !empty($node->biblio_date) && !strstr($node->biblio_date, $node->biblio_year)) || (!empty($node->biblio_year) && empty($node->biblio_date))) {
    $output .= ', ' . check_plain($node->biblio_year);
  }
  $output .= ".\n";

  return filter_xss($output, biblio_get_allowed_tags());
}
