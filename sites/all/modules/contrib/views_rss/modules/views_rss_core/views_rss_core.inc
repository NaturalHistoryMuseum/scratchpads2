<?php

/**
 * @file
 * Preprocess functions for Views RSS: Core Elements module.
 */

/**
 * Preprocess function for channel <title> element.
 */
function views_rss_core_preprocess_channel_title(&$variables) {
  $view_title = $variables['view']->get_title();
  $variables['elements'][0]['value'] = ($view_title) ? $view_title : variable_get('site_name', t('Drupal'));
}

/**
 * Preprocess function for channel <description> element.
 */
function views_rss_core_preprocess_channel_description(&$variables) {
  if (empty($variables['elements'][0]['value'])) {
    $variables['elements'][0]['value'] = variable_get('site_slogan', '');
  }
}

/**
 * Preprocess function for channel <link> element.
 */
function views_rss_core_preprocess_channel_link(&$variables) {
  $variables['elements'][0]['value'] = url('<front>', array('absolute' => TRUE));
}

/**
 * Preprocess function for channel <atom:link> element.
 */
function views_rss_core_preprocess_channel_atom_link(&$variables) {
  $url_options = array('absolute' => TRUE);
  $input = $variables['view']->get_exposed_input();
  if ($input) {
    $url_options['query'] = $input;
  }
  $url = url($variables['view']->get_url(), $url_options);
  $variables['elements'][0]['attributes'] = array(
    'rel' => 'self',
    'href' => $url,
  );
}

/**
 * Preprocess function for channel <category> element.
 */
function views_rss_core_preprocess_channel_category(&$variables) {
  // No value = no preprocessing.
  if (empty($variables['elements'][0]['value'])) {
    return;
  }
  $elements = array();
  $categories = explode(',', $variables['elements'][0]['value']);
  foreach ($categories as $category) {
    $elements[] = array(
      'key' => 'category',
      'value' => trim($category),
    );
  }
  $variables['elements'] = $elements;
}

/**
 * Preprocess function for channel <image> element.
 */
function views_rss_core_preprocess_channel_image(&$variables) {
  // No value = no preprocessing.
  if (empty($variables['elements'][0]['value'])) {
    return;
  }
  $path = $variables['elements'][0]['value'];
  // Get value of channel <title> element from its preprocess function.
  views_rss_core_preprocess_channel_title($variables);
  $title = $variables['elements'][0]['value'];
  // Get value of channel <title> element from its preprocess function.
  views_rss_core_preprocess_channel_link($variables);
  $link = $variables['elements'][0]['value'];
  // Create subelements array.
  $variables['elements'][0]['value'] = array(
    'url' => file_create_url($path),
    'title' => $title,
    'link' => $link,
  );
  // Try to get image description from website's mission.
  $site_slogan = variable_get('site_slogan', '');
  if (!empty($site_slogan)) {
    $variables['elements'][0]['value']['description'] = $site_slogan;
  }
  // Get image width and height.
  $image = image_load($path);
  if (!empty($image)) {
    $variables['elements'][0]['value']['width'] = $image->info['width'];
    $variables['elements'][0]['value']['height'] = $image->info['height'];
  }
}

/**
 * Preprocess function for channel <pubDate> and <lastBuildDate> elements.
 *
 * It will return values for date element providing that original Views query
 * was modified appropriately by views_rss_core_views_query_alter() by adding
 * new fields to SELECT clause retrieving object creation (for <pubDate>)
 * or modification timestamp (for <lastBuildDate>).
 */
function views_rss_core_preprocess_channel_date(&$variables) {
  if (count($variables['view']->result) > 0) {
    $max_date = 0;
    foreach ($variables['view']->result as $row) {
      $key = strtolower($variables['elements'][0]['key']);
      if (isset($row->$key) && $row->$key > $max_date) {
        $max_date = $row->$key;
      }
    }
    if ($max_date) {
      $variables['elements'][0]['value'] = date('r', $max_date);
    }
  }
}

/**
 * Preprocess function for channel <skipHours> and <skipDays> elements.
 */
function views_rss_core_preprocess_channel_skip(&$variables) {
  // No value = no preprocessing.
  if (empty($variables['elements'][0]['value'])) {
    return;
  }
  $elements = array();
  $skips = strip_tags($variables['elements'][0]['value']);
  if (!empty($skips)) {
    foreach (explode(',', $skips) as $skip_value) {
      $elements[] = array(
        'key' => ($variables['elements'][0]['key'] == 'skipHours') ? 'hour' : 'day',
        'value' => trim($skip_value),
      );
    }
  }
  $variables['elements'][0]['value'] = $elements;
}

/**
 * Preprocess function for channel <cloud> element.
 */
function views_rss_core_preprocess_channel_cloud(&$variables) {
  // No value = no preprocessing.
  if (empty($variables['elements'][0]['value'])) {
    return;
  }
  if ($url = parse_url($variables['elements'][0]['value'])) {
    $variables['elements'][0]['value'] = NULL;
    $variables['elements'][0]['attributes'] = array(
      'domain' => $url['host'],
      'port' => $url['port'],
      'path' => $url['path'],
      'registerProcedure' => $url['fragment'],
      'protocol' => $url['scheme'],
    );
  }
}

/**
 * Preprocess function for item <guid> element.
 */
function views_rss_core_preprocess_item_guid(&$variables) {
  // No value = no preprocessing.
  if (empty($variables['elements'][0]['value'])) {
    return;
  }
  $is_permalink = 'false';
  if (
    (!isset($variables['item']['views_rss_core']['link']) || empty($variables['item']['views_rss_core']['link']))
    && valid_url($variables['elements'][0]['value'], TRUE)
  ) {
    $is_permalink = 'true';
  }
  $variables['elements'][0]['attributes']['isPermaLink'] = $is_permalink;
}

/**
 * Preprocess function for item <source> element.
 */
function views_rss_core_preprocess_item_source(&$variables) {
  $url_options = array('absolute' => TRUE);
  $input = $variables['view']->get_exposed_input();
  if ($input) {
    $url_options['query'] = $input;
  }
  $url = url($variables['view']->get_url(), $url_options);

  $view_title = $variables['view']->get_title();
  $variables['elements'][0]['value'] = (!empty($view_title)) ? $view_title : variable_get('site_name', t('Drupal'));
  $variables['elements'][0]['attributes'] = array('url' => $url);
}