<?php

// by Jeffrey Dwoskin
// A style closer to IEEE format
// see http://standards.ieee.org/guides/style/section7.html#992
/**
 * Get the style information
 *
 * @return
 *   The name of the style
 */
function biblio_style_ieee_info() {
  return array(
    'ieee' => 'Institute of Electrical and Electronics Engineers (IEEE)'
  );
}
function biblio_style_ieee_author_options() {
  $author_options = array(
    'BetweenAuthorsDelimStandard'     =>  ', ',      //4
    'BetweenAuthorsDelimLastAuthor'   =>  ', and ',    //5
    'AuthorsInitialsDelimFirstAuthor' =>  ', ',      //7
    'AuthorsInitialsDelimStandard'    =>  ' ',       //8
    'betweenInitialsDelim'            =>  '. ',      //9
    'initialsBeforeAuthorFirstAuthor' =>  FALSE,     //10
    'initialsBeforeAuthorStandard'    =>  TRUE,      //11
    'shortenGivenNames'               =>  TRUE,      //12
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
 * @param $base
 *   The base URL of the biblio module (defaults to /biblio)
 * @param $inline
 *   A logical value indicating if this is being rendered within the
 *   Drupal framwork (FALSE) or we are just passing back the html (TRUE)
 * @return
 *   The styled biblio entry
 */
function biblio_style_ieee($node, $base = 'biblio', $inline = FALSE) {
  $output = '';
  $author_options = biblio_style_ieee_author_options();
  if ($node->biblio_contributors->has_category(1)) {
    $authors = theme('biblio_format_authors', array('contributors' => $node->biblio_contributors->get_category(1), 'options' => $author_options, 'inline' => $inline));
  }
  if (!empty ($node->biblio_citekey)&&(variable_get('biblio_display_citation_key',0))) {
    $output .= '[' . check_plain($node->biblio_citekey) . '] ';
  }

  $output .= isset($authors) ? '<span class="biblio-authors">' . $authors . ", </span> \n" : '';
  switch ($node->biblio_type) {
    default :
      $url = biblio_get_title_url_info($node);
      if (!empty ($node->biblio_secondary_title)) {
        $output .= '<span class="biblio-title">&quot;';
        $output .= l($node->title, $url['link'], $url['options']);
          //$output .= $inline ? l("$node->title", "$base/viewinline/$node->nid") : l("$node->title", "node/$node->nid");
        $output .= "&quot;, </span> \n";
        $output .= '<i>' . check_plain($node->biblio_secondary_title) . '</i>';
      } else {
        $output .= '<span class="biblio-title"><i>';
        $output .= l($node->title, $url['link'], $url['options']);
        $output .= ", </i></span> \n";
      }
      if (!empty ($node->biblio_edition))
        $output .= ', ' . check_plain($node->biblio_edition);
      if (!empty ($node->biblio_volume))
        $output .= ', vol. ' . check_plain($node->biblio_volume);
      if (!empty ($node->biblio_issue))
        $output .= ', issue ' . check_plain($node->biblio_issue);
      if (!empty ($node->biblio_number))
        $output .= ', no. ' . check_plain($node->biblio_number);
      if (!empty ($node->biblio_place_published))
        $output .= ', ' . check_plain($node->biblio_place_published);
      if (!empty ($node->biblio_publisher)) {
        $output .= (check_plain($node->biblio_place_published)) ? ', ' : ': ';
        $output .= check_plain($node->biblio_publisher);
      }
      // if a single page instead of a range, should use 'p.' instead of 'pp.'  -- ignoring
      if (!empty ($node->biblio_pages))
        $output .= ', pp. ' . check_plain($node->biblio_pages);
      // if it is a book, year should go before pages instead -- ignoring
      // for non-books, should also include month of publication (e.g. "Mar. 2006") -- use date instead of year if available
      if (!empty ($node->biblio_date)) {
        $output .= ', ' . check_plain($node->biblio_date);
      }
      if ((!empty ($node->biblio_year) && !empty ($node->biblio_date) && !strstr($node->biblio_date, $node->biblio_year)) || (!empty ($node->biblio_year) && empty ($node->biblio_date))) {
        $output .= ', ' . check_plain($node->biblio_year);
      }
      $output .= ".\n";
      break; // generic
  }
  /*  if ($node->biblio_date) $output .= ', '. check_plain($node->biblio_date);

  */
  return filter_xss($output, biblio_get_allowed_tags());

}
function _ieee_format_author($author) {
  $format = $author['prefix'];
  $format .= !empty ($format) ? ' ' . $author['lastname'] . ' ' : $author['lastname'] . ' ';
  $format .= !empty ($author['firstname']) ? drupal_substr($author['firstname'], 0, 1) . '.' : '';
  $format .= !empty ($author['initials']) ? $author['initials'] . '.' : '';
  return $format;
}