/**
 * Theme function for an empty results page.
 */
function theme_ecoint_serach($variables) {
  return (serialize($variables));
}
 
function theme_ecoint_search_empty($variables){
  return t('<p>Either no literature content has been added to this site, or it has not yet been indexed. Indexing can take up to one hour, so please check back later.</p>');
}