<?php
// $Id: date_views.views.inc,v 1.3 2011/01/18 00:37:35 karens Exp $

/**
 * @file
 * Defines date-related Views data and plugins:
 * 
 * Date argument:
 *   A generic date argument that has an option to select one or more 
 *   Views date fields to filter on, automatically adds them to the view, 
 *   and then filters the view by the value of the selected field(s). 
 *   The flexible argument will accept and evaluate most ISO date
 *   and period formats, like 2009-05-01, 2008-W25, P1W. 
 * 
 * Date filter:
 *   A generic date filter that has an option to select a 
 *   Views date field to filter on, with a choice of a widget to use
 *   for the filter form and an option to set the default value to 
 *   a set date or something like 'now +90 days' . If the operator is 
 *   set to 'between' or 'not between' you can set a default value for
 *   both the from and to dates. 
 * 
 * Current date argument default
 *   Adds a default option to set the argument to the current date 
 *   when the argument is empty.
 * 
 * Date navigation attachment
 *   Navigation that can be attached to any display to create back/next 
 *   links by date, requires the date argument and uses the current
 *   date argument default to set a starting point for the view.
 */
/**
 * Implementation of hook_views_plugins
 */
function date_views_views_plugins() {
  $path = drupal_get_path('module', 'date_views');
  $views_path = drupal_get_path('module', 'views');
  module_load_include('inc', 'date_views', 'theme/theme');

  return array(
    'module' => 'date_views', // This just tells our themes are elsewhere.
    'display' => array(
      // Display plugin for date navigation.
      'date_nav' => array(
        'title' => t('Date browser'),
        'help' => t('Date back/next navigation to attach to other displays. Requires the Date argument.'),
        'handler' => 'date_plugin_display_attachment',
        'parent' => 'attachment',
        'path' => "$path/includes",
        'theme' => 'views_view',
        'use ajax' => TRUE,
        'admin' => t('Date browser'),
        'help topic' => 'date-browser',
      ),
    ),
    'style' => array(
      'parent' => array(
        // this isn't really a display but is necessary so the file can
        // be included.
        'no ui' => TRUE,
        'handler' => 'views_plugin_style',
        'path' => "$views_path/plugins",
        'theme file' => 'theme.inc',
        'theme path' => "$views_path/theme",
        'parent' => '',
      ),
      'date_nav' => array(
        'title' => t('Date browser style'),
        'help' => t('Creates back/next navigation.'),
        'handler' => 'date_navigation_plugin_style',
        'path' => "$path/includes",
        'parent' => 'parent',
        'theme' => 'date_navigation',
        'theme file' => 'theme.inc',
        'theme path' => "$path/theme",
        'uses row plugin' => FALSE,
        'uses fields' => FALSE,
        'uses options' => TRUE,
        'type' => 'date_nav', 
        'even empty' => TRUE,
      ),
    ),
  );
}

/**
 * Implementation of hook_views_data()
 */
function date_views_views_data() {
  $data = array();

  $tables = module_invoke_all('date_views_tables');

  foreach ($tables as $base_table) {
    // The flexible date argument.
    $data[$base_table]['date_argument'] = array(
      'group' => t('Date'),
      'title' => t('Date (!base_table)', array('!base_table' => $base_table)),
      'help' => t('Filter any Views !base_table date field by a date argument, using any common ISO date/period format (i.e. YYYY, YYYY-MM, YYYY-MM-DD, YYYY-W99, YYYY-MM-DD--P3M, P90D, etc). ', array('!base_table' => $base_table)),
      'argument' => array(
        'handler' => 'date_views_argument_handler',
        'empty field name' => t('Undated'),
        //'skip base' => $base_table,
      ),
    );  
    // The flexible date filter.
    $data[$base_table]['date_filter'] = array(
      'group' => t('Date'),
      'title' => t('Date (!base_table)', array('!base_table' => $base_table)),
      'help' => t('Filter any Views !base_table date field.', array('!base_table' => $base_table)),
      'filter' => array(
        'handler' => 'date_views_filter_handler',
        'empty field name' => t('Undated'),
        //'skip base' => $base_table,
      ),
    ); 
  }

  return $data;
}

/**
 *  Identify all potential date/timestamp fields and cache the data.
 */
function date_views_fields($base = 'node', $reset = FALSE) {
  static $fields = array();
  $empty = array('name' => array(), 'alias' => array());
  module_load_include('inc', 'date_views', 'includes/date_views_fields');

  if (empty($fields[$base]) || $reset) {
    $cid = 'date_views_fields_' . $base;
    if (!$reset && $cached = cache_get($cid, 'cache_views')) {
      $fields[$base] = $cached->data;
    }
    else {
      $fields[$base] = _date_views_fields($base);
    }
  }
  // Make sure that empty values will be arrays in he expected format.
  return !empty($fields) && !empty($fields[$base]) ? $fields[$base] : $empty;
}

/**
 * Central function for setting up the right timezone values
 * in the SQL date handler.
 * 
 * The date handler will use this information to decide if the
 * database value needs a timezone conversion.
 * 
 * In Views, we will always be comparing to a local date value,
 * so the goal is to convert the database value to the right
 * value to compare to the local value.
 */
function date_views_set_timezone(&$date_handler, &$view, $field) {
  switch ($field['tz_handling']) {
    case 'date' :
      $date_handler->db_timezone = 'UTC';
      $date_handler->local_timezone_field = $field['timezone_field'];
      $date_handler->offset_field = $field['offset_field'];
      break;
    case 'none':
      $date_handler->db_timezone = date_default_timezone();
      $date_handler->local_timezone = date_default_timezone();
      break;
    case 'utc':
      $date_handler->db_timezone = 'UTC';
      $date_handler->local_timezone = 'UTC';
      break;
    default :
      $date_handler->db_timezone = 'UTC';
      $date_handler->local_timezone = date_default_timezone();
      break;
  }
}

function date_views_querystring($view, $extra_params = array()) {
  $query_params = array_merge($_GET, $extra_params);
  // Allow NULL params to be removed from the query string.
  foreach ($extra_params AS $key => $value) {
    if (!isset($value)) {
      unset($query_params[$key]);
    }
  }
  // Filter the special "q" and "view" variables out of the query string.
  $exclude = array('q');
  // If we don't explicitly add a value for "view", filter it out.
  if (empty($extra_params['view'])) {
    $exclude[] = 'view';
  }
  $query = drupal_http_build_query(drupal_get_query_parameters($query_params, $exclude));
  // To prevent an empty query string from adding a "?" on to the end of a URL,
  // we return NULL.
  return !empty($query) ? $query : NULL;
}

/**
 * Identify the base url of the page,
 * needed when the calendar is embedded so we
 * don't set the url to the calendar url.
 */
function date_views_page_url($view) {
  if ($view->build_type == 'page') {
    return date_views_real_url($view, $view->date_info->real_args);
  }
  else {
    $block_identifier = isset($view->date_info->block_identifier) ? $view->date_info->block_identifier : 'mini';
    return url($_GET['q'], date_views_querystring($view, array($block_identifier => NULL)), NULL, TRUE);
  }
}

/**
 * Figure out what the URL of the calendar view we're currently looking at is.
 */
function date_views_real_url($view, $args) {
  if (empty($args)) {
    return $view->date_info->url;
  }
  // Add non-calendar arguments to the base url.
  $parts = explode('/', $view->date_info->url);
  $bump = 0;
  foreach ($parts as $delta => $part) {
    // If one of the args is buried in the url, add it here and adjust
    // the delta values we'll compare the calendar arg positions to.
    if (substr($part, 0, 1) == '$') {
      $parts[$delta] = array_shift($args);
      $bump++;
    }
  }
  foreach ($args as $delta => $arg) {
    if (!in_array($delta + $bump, calendar_arg_positions($view)) && !empty($arg)) {
      array_push($parts, $arg);
    }
  }
  return implode('/', $parts);
}
