<?php

/**
 * @file
 * Theme & preprocess functions for the Views Slideshow: cycle module.
 */

/**
 * Views Slideshow: Theme the main frame wrapper.
 *
 * @ingroup themeable
 */

function _views_slideshow_cycle_preprocess_views_slideshow_cycle_main_frame(&$vars) {
  $settings = $vars['settings'];
  $rows = $vars['rows'];
  $view = $vars['view'];
  $vss_id = $vars['vss_id'];

  // Cast the strings into int or bool as necessary.
  $new_settings = array();
  foreach ($settings as $key => $value) {
    if (is_string($value)) {
      if (stristr($value, "\n")) {
        $value = str_replace("\n", ' ', $value);
      }
      $value = trim($value);

      if (is_numeric($value)) {
        $value = (int)$value;
      }
      elseif (strtolower($value) == 'true') {
        $value = TRUE;
      }
      elseif (strtolower($value) == 'false') {
        $value = FALSE;
      }
    }

    $new_settings[$key] = $value;
  }

  $settings = array_merge(
    array(
      'num_divs' => sizeof($rows),
      'id_prefix' => '#views_slideshow_cycle_main_',
      'div_prefix' => '#views_slideshow_cycle_div_',
      'vss_id' => $vss_id,
    ),
    $new_settings
  );

  // We need to go through the current js setting values to make sure the one we
  // want to add is not already there. If it is already there then append -[num]
  // to the id to make it unique.
  $slideshow_count = 1;
  $current_settings = drupal_add_js();
  foreach ($current_settings['settings']['data'] AS $current_setting) {
    if (isset($current_setting['viewsSlideshowCycle'])) {
      $current_keys = array_keys($current_setting['viewsSlideshowCycle']);
      if (stristr($current_keys[0], '#views_slideshow_cycle_main_' . $vss_id)) {
        $slideshow_count++;
      }
    }
  }

  if ($slideshow_count > 1) {
    $vss_id .= '-' . $slideshow_count;
    $settings['vss_id'] = $vss_id;
  }
  
  // Don't load javascript unless libraries module is present.
  if (module_exists('libraries')) {
    // Load jQuery Cycle
    if ($cycle_path = _views_slideshow_cycle_library_path()) {
      drupal_add_js($cycle_path);
    }

    // Load Json2
    $json_path = libraries_get_path('json2');
    if (!empty($json_path) && file_exists($json_path . '/json2.js')) {
      drupal_add_js($json_path . '/json2.js');
    }

    // Load our cycle js
    $module_path = drupal_get_path('module', 'views_slideshow_cycle');
    drupal_add_js($module_path . '/js/views_slideshow_cycle.js');
  }

  // Load our cycle css
  drupal_add_css($module_path . '/views_slideshow_cycle.css', 'file');

  drupal_add_js(array('viewsSlideshowCycle' => array('#views_slideshow_cycle_main_' . $vss_id => $settings)), 'setting');
  
  // Add hover intent library
  if ($settings['pause']) {
    if (module_exists('libraries')) {
      // Load jQuery hoverIntent
      $hoverIntent_path = libraries_get_path('jquery.hoverIntent');
      if (!empty($hoverIntent_path) && file_exists($hoverIntent_path . '/jquery.hoverIntent.js')) {
        drupal_add_js($hoverIntent_path . '/jquery.hoverIntent.js');
      }
    }
  }

  // Add the slideshow elements.
  $vars['classes_array'][] = 'views_slideshow_cycle_teaser_section';

  $styles = '';
  if (isset($view->display_handler->display->display_options['style_options']['views_slideshow_cycle'])) {
    $styles = $view->display_handler->display->display_options['style_options']['views_slideshow_cycle'];
  }

  $styles_default = '';
  if (isset($view->display['default']->display_options['style_options']['views_slideshow_cycle'])) {
    $styles_default = $view->display['default']->display_options['style_options']['views_slideshow_cycle'];
  }

  // Retrive the number of items per frame
  if (isset($styles['items_per_slide']) && $styles['items_per_slide'] > 0) {
    $items_per_slide = $styles['items_per_slide'];
  }
  elseif (isset($styles_default['items_per_slide']) && $styles_default['items_per_slide'] > 0) {
    $items_per_slide = $styles_default['items_per_slide'];
  }
  else {
    $items_per_slide = 1;
  }

  $vars['items_per_slide'] = $items_per_slide;

  $items = array();
  $slideshow_count = 0;
  $rendered_rows = '';
  foreach ($rows as $count => $item) {
    $items[] = $item;
    if (count($items) == $items_per_slide || $count == (count($rows)-1)) {
      $rendered_rows .= theme('views_slideshow_cycle_main_frame_row', array('vss_id' => $vss_id, 'items' => $items, 'count' => $slideshow_count, 'view' => $vars['view']));
      $items = array();
      $slideshow_count++;
    }
  }

  $vars['rendered_rows'] = $rendered_rows;
}

/**
 * Views Slideshow slideshow rows.
 *
 * @ingroup themeable
 */
function _views_slideshow_cycle_preprocess_views_slideshow_cycle_main_frame_row(&$vars) {
  $current = $vars['count'] + 1;
  $vars['classes_array'][] = 'views_slideshow_cycle_slide';
  $vars['classes_array'][] = 'views_slideshow_slide views-row-' . $current;

  if ($vars['count']) {
    $vars['classes_array'][] =  'views_slideshow_cycle_hidden';
  }
  $vars['classes_array'][] = ($vars['count'] % 2) ? 'views-row-even' : 'views-row-odd';

  $vars['rendered_items'] = '';
  foreach ($vars['items'] as $item_count => $item) {
    $vars['rendered_items'] .= theme('views_slideshow_cycle_main_frame_row_item', array('item' => $item, 'item_count' => $item_count, 'view' => $vars['view']));
  }
}

function _views_slideshow_cycle_preprocess_views_slideshow_cycle_main_frame_row_item(&$vars) {
  $vars['classes_array'][] = 'views-row views-row-' . $vars['item_count'];
  if (!$vars['item_count']) {
    $vars['classes_array'][] = 'views-row-first';
  }
  if ($vars['item_count'] % 2) {
    $vars['classes_array'][] = 'views-row-even';
  }
  else {
    $vars['classes_array'][] = 'views-row-odd';
  }
  
  /**
   * Support custom row classes.
   */
  if ($row_class = $vars['view']->style_plugin->get_row_class($vars['item_count'])) {
    $vars['classes_array'][] = $row_class;
  }
}
