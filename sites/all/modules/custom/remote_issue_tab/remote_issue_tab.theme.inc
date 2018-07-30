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
  $fid = $variables['feed'];
  if($fid){
    // Add the CSS & JS
    drupal_add_css(drupal_get_path('module', 'remote_issue_tab') . '/css/remote_issue_tab.css');
    drupal_add_js(drupal_get_path('module', 'remote_issue_tab') . '/js/remote_issue_tab.js');
    // Load the required file
    module_load_include('pages.inc', 'aggregator');
    // This defaults to 20, which is more than enough for us!
    // Note, including fid in the SQL should be safe here, as we've ensured that
    // fid is an integer above.
    $items = remote_issue_tab_aggregator_feed_items_load('source', (object)array(
      'fid' => $fid
    ));
    return '<div id="remote-issue-tab">
 <div>
  <h2>'. l( t('Search for/Report an issue'), _remote_issue_tab_feed_link($fid) ) . '</h2>
  <div class="header">'. $variables['header'] . '</div>
 </div>
 <div class="items">
  ' . theme('remote_issue_tab_feed_items', array(
      'items' => $items,
      'feed' => $fid
    )) . '
 </div>
 <div class="footer">
  ' . $variables['footer'] . '
 </div>
</div>';
  }else{
    // Something is wrong, we'll return content that is effectively a warning
    // message telling the user to set the settings (stop being a dickhead, and
    // actually read the instructions).
    return t('Please ensure you have <a href="!agg_url">configured this block</a> before enabling it.', array(
      '!agg_url' => url('admin/structure/block/manage/remote_issue_tab/0/configure')
    ));
  }
}

// Get the html link for the selected feed
function _remote_issue_tab_feed_link($fid) {
  $result = db_select('aggregator_feed', 'a')->fields('a')->condition('fid', $fid)->execute()->fetchAssoc();
  return $result['link'];
}

// Copy of aggregator_feed_items_load, but without the pager shite!
function remote_issue_tab_aggregator_feed_items_load($type, $data = NULL){
  $items = array();
  switch($type){
    case 'sum':
      $query = db_select('aggregator_item', 'i');
      $query->join('aggregator_feed', 'f', 'i.fid = f.fid');
      $query->fields('i');
      $query->addField('f', 'title', 'ftitle');
      $query->addField('f', 'link', 'flink');
      break;
    case 'source':
      $query = db_select('aggregator_item', 'i');
      $query->fields('i')->condition('i.fid', $data->fid);
      break;
    case 'category':
      $query = db_select('aggregator_category_item', 'c');
      $query->leftJoin('aggregator_item', 'i', 'c.iid = i.iid');
      $query->leftJoin('aggregator_feed', 'f', 'i.fid = f.fid');
      $query->fields('i')->condition('cid', $data['cid']);
      $query->addField('f', 'title', 'ftitle');
      $query->addField('f', 'link', 'flink');
      break;
  }
  $result = $query->orderBy('i.timestamp', 'DESC')->orderBy('i.iid', 'DESC')->execute();
  foreach($result as $item){
    $item->categories = db_query('SELECT c.title, c.cid FROM {aggregator_category_item} ci LEFT JOIN {aggregator_category} c ON ci.cid = c.cid WHERE ci.iid = :iid ORDER BY c.title', array(
      ':iid' => $item->iid
    ))->fetchAll();
    $items[] = $item;
  }
  return $items;
}

/**
 * theme function for items
 */
function theme_remote_issue_tab_feed_items($variables = array()){
  // We only want 10 items, so we'll count
  $content = '';
  if(count($variables['items'])){
    $item_count = 0;
    $content = '<h2>Recent Issues:</h2><ul>';
    foreach($variables['items'] as $item){
      if($item_count == 10){
        break;
      }
      $content .= theme('remote_issue_tab_feed_item', array(
        'item' => $item,
        'item_count' => $item_count
      ));
      $item_count++;
    }
    $content .= '</ul>';
  }else{
    return t('There are no issues to display.  You may need to <a href="!agg_url">update</a> the issue feed.', array(
      '!agg_url' => url('admin/config/services/aggregator/update/' . $variables['feed'])
    ));
  }
  return $content;
}

/**
 * theme function for a single item
 */
function theme_remote_issue_tab_feed_item($variables = array()){
  // Note, we capitalise &lti&gt; tags to ensure they don't get converted into
  // real tags.
  return '<li class="' . ($variables['item_count'] % 2 ? 'odd' : 'even') . ($variables['item_count'] ? '' : ' first') . '"><div><h3>' . l($variables['item']->title, $variables['item']->link) . '</h3><div class="remote_issue_item">' . strtr($variables['item']->description, array(
    '&lt;i&gt;' => '&LT;I&GT;',
    '&lt;em&gt;' => '&LT;EM&GT',
    '&lt;/i&gt;' => '&lt;/I&GT;',
    '&lt;/em&gt;' => '&lt;/EM&GT;'
  )) . '</div></div></li>';
}
