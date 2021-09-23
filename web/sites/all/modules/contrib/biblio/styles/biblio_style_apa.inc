<?php

/**
 * @file
 */

/**
 * Get the style information.
 *
 * @return
 *   The name of the style
 */
function biblio_style_apa_info() {
  return array(
    'apa' => 'American Psychological Association (APA)',
  );
}

/**
 *
 */
function biblio_style_apa_author_options() {
  $author_options = array(
    // 4.
    'BetweenAuthorsDelimStandard'     => ', ',
    // 5.
    'BetweenAuthorsDelimLastAuthor'   => ', & ',
    // 7.
    'AuthorsInitialsDelimFirstAuthor' => ', ',
    // 8.
    'AuthorsInitialsDelimStandard'    => ' ',
    // 9.
    'betweenInitialsDelim'            => '. ',
    // 10.
    'initialsBeforeAuthorFirstAuthor' => FALSE,
    // 11.
    'initialsBeforeAuthorStandard'    => FALSE,
    // 12.
    'shortenGivenNames'               => TRUE,
    // 13.
    'numberOfAuthorsTriggeringEtAl'   => 6,
    // 14.
    'includeNumberOfAuthors'          => 6,
    // 15.
    'customStringAfterFirstAuthors'   => ', et al.',
    'encodeHTML'                      => TRUE,
  );
  return $author_options;
}

/**
 * Apply a bibliographic style to the node.
 *
 * @param $node
 *   An object containing the node data to render
 *   The base URL of the biblio module (defaults to /biblio)
 *
 * @return
 *   The styled biblio entry
 */
function biblio_style_apa($node) {
  module_load_include('inc', 'biblio', '/includes/biblio.contributors');
  $output = '';
  $authors = '';
  $author_options = biblio_style_apa_author_options();
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
  if (empty($authors)) {
    // Use anonymous if we still have nothing.
    $authors = '[' . t('Anonymous') . ']';
  }
  if (!empty($node->biblio_citekey)&&(variable_get('biblio_display_citation_key', 0))) {
    $output .= '[' . check_plain($node->biblio_citekey) . '] ';
  }
  $output .= '<span class="biblio-authors">' . $authors . "</span> \n";
  $output .= (strrpos($authors, '.') == strlen($authors)) ? ".&nbsp;&nbsp;" : " ";
  switch ($node->biblio_type) {
    // Journal Article.
    case 1:
      // Conference Paper.
    case 2:
      // Are all.
    case 3:
    case 4:
    case 5:
    case 6:
    case 7:
    case 8:
    case 9:
    default:
      if (isset($node->biblio_year)) {
        $output .= "(" . check_plain($node->biblio_year) . ").&nbsp;&nbsp;";
      }
      $output .= '<span class="biblio-title">';
      $url = biblio_get_title_url_info($node);
      $output .= l($node->title, $url['link'], $url['options']);
      $output .= ". </span> \n";
      $output .= !empty($editors) ? '(' . theme('biblio_format_authors', array('contributors' => $editors, 'options' => $author_options)) . ', Ed.).' : "";
      $output .= ($node->biblio_secondary_title) ? '<u>' . check_plain($node->biblio_secondary_title) . '. ' : '<u>';
      $output .= ($node->biblio_volume) ? check_plain($node->biblio_volume) . ($node->biblio_issue ? '</u>(' . check_plain($node->biblio_issue) . '),&nbsp;' : ',</u> ') : '</u> ';

      // @code
      // $output .= ($node->biblio_issue) ? '('. check_plain($node->biblio_issue).')' :'';
      // @endcode

      $output .= ($node->biblio_pages) ? check_plain($node->biblio_pages) . '.' : '';
      break;
  }
  /*  if ($node->biblio_date) $output .= ', '. check_plain($node->biblio_date);
  if ($node->biblio_number) $output .= ', Number '. check_plain($node->biblio_number);

  if ($node->biblio_place_published) $output .= ', '. check_plain($node->biblio_place_published);
   */
  return filter_xss($output, biblio_get_allowed_tags());
}
