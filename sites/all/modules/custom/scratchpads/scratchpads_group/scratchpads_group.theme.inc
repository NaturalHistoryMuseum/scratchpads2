<?php

/**
 * Theme function for an empty results page.
 */
function theme_scratchpads_group_empty($variables){
  return t('<p>Either no content has been added to this group, or it has not yet been indexed. Indexing can take up to one hour, so please check back later.</p>');
}
