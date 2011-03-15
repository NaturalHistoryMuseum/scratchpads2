<?php

/**
 * @file
 * 
 * All the theme functions that this module uses.  These can easily be
 * overridden by a theme/other module if wanted.
 */

/**
 * Main theme that creates the whole block content.
 */
function theme_remote_issue_tab_block($variables = array()){
  return print_r($variables, 1);
  
  // As creating the tab is very processor heavy (due to the number of 
  // encryptions were performing), we'll cache the footer, by user, for 15 mins
  // Note, we cache by user due to the fact that the user details are embeded in
  // each of the links.
  $fid = (integer)variable_get('remote_issue_tab_feed', 0);
  $key = variable_get('remote_issue_tab_uwho_client_key', FALSE);
  $url = variable_get('remote_issue_tab_uwho_url', FALSE);
  if($variables['feed'] && is_integer($variables['feed']) && $key && $url){
    // Load the required file
    module_load_include('pages.inc', 'aggregator');
    // This defaults to 20, which is more than enough for us!
    // Note, including fid in the SQL should be safe here, as we've ensured that
    // fid is an integer above.
    $items = aggregator_feed_items_load('SELECT * FROM {aggregator_item} WHERE fid = ' . $variables['feed'] . ' ORDER BY timestamp DESC, iid DESC', $fid);
    if(!$key && !$url){return '';}
    $data = '<div id="remote-issue-tab">
  <div class="content">
    <div id="open-close">
      <p>' . variable_get('remote_issue_tab_tab_title', t('Issues') . '...') . '</p>
    </div>
    <div class="subcontent">
      <div class="new-items">
        <h1>' . uwho_client_block_content($key, $url, variable_get('remote_issue_tab_redirect_path', FALSE), t('Search for/Report an issue')) . '</h1>
      </div>
      <div class="items">
        ' . theme('remote_issue_tab_feed_items', $items, $key, $url) . '
      </div>
      <div class="footer">
        ' . variable_get('remote_issue_tab_footer', '') . '
      </div>
    </div>
  </div>
</div>';
    cache_set('remote_issue_tab_footer_' . $user->uid, $data, 'cache', time() + variable_get('remote_issues_tab_cache_time', 900));
    return $data;
  }
  

}



/**
 * theme function for items
 */
function theme_remote_issue_tab_feed_items($items = NULL, $key, $url){
  // We only want 10 items, so we'll count
  $content = '';
  if(count($items)){
    $item_count = 0;
    $content = '<h1>Recent Issues:</h1><ul>';
    foreach($items as $item){
      if($item_count == 10){
        break;
      }
      $content .= theme('remote_issue_tab_feed_item', $item, $item_count, $key, $url);
      $item_count++;
    }
    $content .= '</ul>';
  }else{
    return t('There are no issues to display.  You may need to <a href="!agg_url">update</a> the issue feed.', array(
      '!agg_url' => url('admin/content/aggregator/update/' . variable_get('remote_issue_tab_feed', 0))
    ));
  }
  return $content;
}

/**
 * theme function for a single item
 */
function theme_remote_issue_tab_feed_item($item = NULL, $item_count, $key, $url){
  // Note, we remove the "read more" link, as it could prevent users being
  // logged in by uwho.
  return '<li class="' . ($item_count % 2 ? 'odd' : 'even') . ($item_count ? '' : ' first') . '"><div><h3>' . uwho_client_block_content($key, $url, $item->link, $item->title) . '</h3><p>' . preg_replace('/<a href[^>]*>read more<\/a>/', '', $item->description) . '</p></div></li>';
}

// FIXME - Remove me!!
function uwho_client_block_content(){
  return '';
}