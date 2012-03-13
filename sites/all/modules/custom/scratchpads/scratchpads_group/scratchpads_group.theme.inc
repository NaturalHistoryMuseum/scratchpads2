<?php

/**
 * Theme function for a Scratchpads search page.
 */
function theme_scratchpads_group($variables){
  return print_r($variables, 1);
}

/**
 * Theme function for an empty results page.
 */
function theme_scratchpads_group_empty($variables){
  return t('<p>Either no media has been !uploaded to this site, or it has not yet been indexed. Indexing can take up to one hour, so please check back later.</p>', array(
    '!uploaded' => user_access('create media_gallery content') ? l(t('uploaded'), 'node/add/media-gallery') : t('uploaded')
  ));
}
