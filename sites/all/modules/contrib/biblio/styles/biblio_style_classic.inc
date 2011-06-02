<?php

/**
 * Get the style information
 *
 * @return
 *   The name of the style
 */
function biblio_style_classic_info() {
  return array(
    'classic' => 'Classic - This is the original biblio style'
  );
}
function biblio_style_classic_author_options() {
  $author_options = array(
    'BetweenAuthorsDelimStandard'     =>  ', ',      //4
    'BetweenAuthorsDelimLastAuthor'   =>  ', and ',    //5
    'AuthorsInitialsDelimFirstAuthor' =>  ', ',      //7
    'AuthorsInitialsDelimStandard'    =>  ' ',       //8
    'betweenInitialsDelim'            =>  '. ',      //9
    'initialsBeforeAuthorFirstAuthor' =>  FALSE,     //10
    'initialsBeforeAuthorStandard'    =>  FALSE,      //11
    'shortenGivenNames'               =>  FALSE,      //12
    'numberOfAuthorsTriggeringEtAl'   =>  10,        //13
    'includeNumberOfAuthors'          =>  10,        //14
    'customStringAfterFirstAuthors'   =>  ', et al.',//15
    'encodeHTML'                      =>  TRUE
  );
  return $author_options;
}

/**
 * Apply a bibliographic style to the node
 *
 *
 * @param $node
 *   An object containing the node data to render
 * @return
 *   The styled biblio entry
 */
function biblio_style_classic($node) {
  module_load_include('inc', 'biblio', '/includes/biblio.contributors');
  $output = '';
  $author_options = biblio_style_classic_author_options();
  $authors = $editor = '';
  $primary_authors = biblio_get_contributor_category($node->biblio_contributors, 1);
  if(!empty($primary_authors)) {
    $authors = theme('biblio_format_authors', array('contributors' => $primary_authors, 'options' => $author_options));
  }
  if (!empty ($node->biblio_citekey)&&(variable_get('biblio_display_citation_key',0))) {
    $output .= '[' . check_plain($node->biblio_citekey) . '] ';
  }
  $output .= '<span class="biblio-title">';
  $url = biblio_get_title_url_info($node);
  $output .= l($node->title, $url['link'], $url['options']);
  $output .= "</span>, \n";
  $output .= '<span class="biblio-authors">' . $authors . "</span> \n";
  if ($node->biblio_secondary_title) {
    $output .= ', ' . check_plain($node->biblio_secondary_title);
  }
  if ($node->biblio_date)
    $output .= ', ' . check_plain($node->biblio_date);
  if ($node->biblio_volume)
    $output .= ', Volume ' . check_plain($node->biblio_volume);
  if ($node->biblio_issue)
    $output .= ', Issue ' . check_plain($node->biblio_issue);
  if ($node->biblio_number)
    $output .= ', Number ' . check_plain($node->biblio_number);
  if ($node->biblio_place_published)
    $output .= ', ' . check_plain($node->biblio_place_published);
  if ($node->biblio_pages)
    $output .= ', p.' . check_plain($node->biblio_pages);
  if (isset ($node->biblio_year)) {
    $output .= ', (' . check_plain($node->biblio_year) . ")\n";
  }
  return filter_xss($output, biblio_get_allowed_tags());

}

function _classic_format_author($author) {
  $format = $author['prefix'] . ' ' . $author['lastname'] . ' ';
  $format .= !empty ($author['firstname']) ? ' ' . drupal_substr($author['firstname'], 0, 1) : '';
  $format .= !empty ($author['initials']) ? str_replace(' ', '', $author['initials']) : '';
  return $format;
}
