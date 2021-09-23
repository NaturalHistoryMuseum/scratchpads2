<?php

/**
 * @file
 * Theme & preprocess functions for the Views Slideshow: cycle module.
 */

/**
 * Views Slideshow: Theme the main frame wrapper.
 *
 * @param array $vars
 *   Theme variables.
 *
 * @ingroup vss_theme
 */
function template_preprocess_views_slideshow_cycle_main_frame(&$vars) {
  $settings = $vars['settings'];
  $rows = $vars['rows'];
  $view = $vars['view'];
  $vss_id = $vars['vss_id'];

  // Cast the strings into int or bool as necessary.
  $new_settings = array();
  foreach ($settings as $key => $value) {
    if (is_string($value)) {
      $value = str_replace("\n", ' ', $value);

      $value = trim($value);

      if (is_numeric($value)) {
        $value = (int) $value;
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
      'num_divs' => count($rows),
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
  foreach ($current_settings['settings']['data'] as $current_setting) {
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

  $module_path = drupal_get_path('module', 'views_slideshow_cycle');

  // Don't load JavaScript unless libraries module is present.
  if (module_exists('libraries')) {
    // Load jQuery Cycle.
    if ($cycle_path = _views_slideshow_cycle_library_path()) {
      drupal_add_js($cycle_path);
    }

    // Load Json2.
    $json_path = libraries_get_path('json2');
    if (!empty($json_path) && file_exists($json_path . '/json2.js')) {
      drupal_add_js($json_path . '/json2.js');
    }

    // Add hover intent library.
    if ($settings['pause']) {
      $hoverIntent_path = libraries_get_path('jquery.hoverIntent');
      if (!empty($hoverIntent_path) && file_exists($hoverIntent_path . '/jquery.hoverIntent.js')) {
        drupal_add_js($hoverIntent_path . '/jquery.hoverIntent.js');
      }
    }

    // Add jquery easing library.
    if (strstr($settings['advanced_options'], '"eas')) {
      $easing_path = libraries_get_path('jquery.easing');
      if (!empty($easing_path) && file_exists($easing_path . '/jquery.easing.1.3.js')) {
        drupal_add_js($easing_path . '/jquery.easing.1.3.js');
      }
    }

    // Load our cycle js.
    drupal_add_js($module_path . '/js/views_slideshow_cycle.js');

    // Load the pause library.
    if (!empty($settings['pause_in_middle']) && $pause_path = _views_slideshow_cycle_pause_library_path()) {
      drupal_add_js($pause_path);
    }
  }

  // Load our cycle css.
  drupal_add_css($module_path . '/views_slideshow_cycle.css', 'file');

  drupal_add_js(array('viewsSlideshowCycle' => array('#views_slideshow_cycle_main_' . $vss_id => $settings)), 'setting');

  // Add the slideshow elements.
  $vars['classes_array'][] = 'views_slideshow_cycle_teaser_section';

  $styles = array();
  if (isset($view->display_handler->display->display_options['style_options']['views_slideshow_cycle'])) {
    $styles = $view->display_handler->display->display_options['style_options']['views_slideshow_cycle'];
  }

  $styles_default = array();
  if (isset($view->display['default']->display_options['style_options']['views_slideshow_cycle'])) {
    $styles_default = $view->display['default']->display_options['style_options']['views_slideshow_cycle'];
  }

  // Retrieve the number of items per frame.
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

  $widgets = $vars['options']['widgets'];
  if (isset($widgets['top']['views_slideshow_pager']['enable']) && $widgets['top']['views_slideshow_pager']['enable']) {
    $aria = "aria-labelledby='views_slideshow_pager_field_item_top_${vss_id}_";
  }
  elseif (isset($widgets['bottom']['views_slideshow_pager']['enable']) && $widgets['bottom']['views_slideshow_pager']['enable']) {
    $aria = "aria-labelledby='views_slideshow_pager_field_item_bottom_${vss_id}_";
  }
  else {
    $aria = "";
  }


  $items = array();
  $slideshow_count = 0;
  $rendered_rows = '';
  $last_slide = ceil(count($rows) / $items_per_slide);
  foreach ($rows as $count => $item) {
    $items[] = $item;
    if (count($items) == $items_per_slide || $count == (count($rows) - 1)) {
      $slide = array(
        'vss_id' => $vss_id,
        'items' => $items,
        'count' => $slideshow_count,
        'view' => $vars['view'],
        'last_slide' => $last_slide,
      );
      if ($aria) {
        $slide['aria'] = $aria . $slideshow_count . "'";
      }
      else {
        $slide['aria'] = '';
      }
      $rendered_rows .= theme(views_theme_functions('views_slideshow_cycle_main_frame_row', $vars['view'], $vars['view']->display[$vars['view']->current_display]), $slide);
      $items = array();
      $slideshow_count++;
    }
  }

  $vars['rendered_rows'] = $rendered_rows;
}

/**
 * Views Slideshow slideshow rows.
 *
 * @param array $vars
 *   Theme variables.
 *
 * @ingroup vss_theme
 */
function template_preprocess_views_slideshow_cycle_main_frame_row(&$vars) {
  $current = $vars['count'] + 1;
  $vars['classes_array'][] = 'views_slideshow_cycle_slide';
  $vars['classes_array'][] = 'views_slideshow_slide views-row-' . $current;

  if ($vars['count']) {
    $vars['classes_array'][] = 'views_slideshow_cycle_hidden';
    if ($current == $vars['last_slide']) {
      $vars['classes_array'][] = 'views-row-last';
    }
  }
  else {
    $vars['classes_array'][] = 'views-row-first';
  }
  $vars['classes_array'][] = ($vars['count'] % 2) ? 'views-row-even' : 'views-row-odd';

  $vars['rendered_items'] = '';
  foreach ($vars['items'] as $item_count => $item) {
    $vars['rendered_items'] .= theme(views_theme_functions('views_slideshow_cycle_main_frame_row_item', $vars['view'], $vars['view']->display[$vars['view']->current_display]), array(
      'item' => $item,
      'item_count' => $item_count,
      'count' => $vars['count'],
      'view' => $vars['view'],
      'length' => count($vars['items']),
    ));
  }
}

/**
 * Implements hook_preprocess_views_slideshow_cycle_main_frame_row_item().
 *
 * @param array $vars
 *   Theme variables.
 *
 * @ingroup vss_theme
 */
function template_preprocess_views_slideshow_cycle_main_frame_row_item(&$vars) {
  $vars['classes_array'][] = 'views-row views-row-' . $vars['item_count'];
  $vars['classes_array'][] = ($vars['item_count'] % 2) ? 'views-row-even' : 'views-row-odd';
  if ($vars['count'] == 0) {
    $vars['classes_array'][] = 'views-row-first';
  }
  elseif ($vars['count'] == $vars['length'] - 1) {
    $vars['classes_array'][] = 'views-row-last';
  }

  // Support custom row classes.
  if ($row_class = $vars['view']->style_plugin->get_row_class($vars['count'])) {
    $vars['classes_array'][] = $row_class;
  }
}

/**
 * Implements hook_preprocess_views_slideshow_pager_fields().
 *
 * @param array $vars
 *   Theme variables.
 *
 * @ingroup vss_theme
 */
function views_slideshow_cycle_preprocess_views_slideshow_pager_fields(&$vars) {
  $slide_count = count($vars['view']->result);
  $items_per_slide = $vars['view']->style_options['views_slideshow_cycle']['items_per_slide'];

  if ($vars['view']->style_options['slideshow_type'] == 'views_slideshow_cycle') {
    $slide_count = $slide_count / $items_per_slide;
    $slide_count = ceil($slide_count);
  }

  $vars['rendered_field_items'] = '';
  if (empty($vars['settings']['hide_on_single_slide']) || $slide_count > 1) {
    foreach ($vars['view']->result as $count => $node) {
      if ($count < $slide_count) {
        $rendered_fields = '';
        foreach ($vars['settings']['views_slideshow_pager_fields_fields'] as $field => $use) {
          if ($use !== 0 && is_object($vars['view']->field[$field])) {
            $rendered_fields .= theme(views_theme_functions('views_slideshow_pager_field_field', $vars['view'], $vars['view']->display[$vars['view']->current_display]), array(
              'view' => $vars['view'],
              'field' => $field,
              'count' => $count,
            ));
          }
        }
        $vars['rendered_field_items'] .= theme(views_theme_functions('views_slideshow_pager_field_item', $vars['view'], $vars['view']->display[$vars['view']->current_display]), array(
          'vss_id' => $vars['vss_id'],
          'item' => $rendered_fields,
          'count' => $count,
          'location' => $vars['location'],
          'view' => $vars['view'],
        ));
      }
    }
  }
}

/**
 * Implements hook_preprocess_views_slideshow_slide_counter().
 *
 * @param array $vars
 *   Theme variables.
 *
 * @ingroup vss_theme
 */
function views_slideshow_cycle_preprocess_views_slideshow_slide_counter(&$vars) {
  if ($vars['view']->style_options['slideshow_type'] == 'views_slideshow_cycle') {
    $vars['slide_count'] = $vars['slide_count'] / $vars['view']->style_options['views_slideshow_cycle']['items_per_slide'];
    $vars['slide_count'] = ceil($vars['slide_count']);
  }
}

/**
 * Backwards comparability wrapper.
 *
 * @param array $vars
 *   Theme variables.
 *
 * @deprecated Removed in 3.5 when the hook_theme() implementation was fixed.
 *
 * @see template_preprocess_views_slideshow_cycle_main_frame().
 *
 * @ingroup vss_theme
 */
function _views_slideshow_cycle_preprocess_views_slideshow_cycle_main_frame(&$vars) {
  template_preprocess_views_slideshow_cycle_main_frame($vars);
}

/**
 * Backwards compatibility wrapper.
 *
 * @param array $vars
 *   Theme variables.
 *
 * @deprecated Removed in 3.5 when the hook_theme() implementation was fixed.
 *
 * @see template_preprocess_views_slideshow_cycle_main_frame_row().
 *
 * @ingroup vss_theme
 */
function _views_slideshow_cycle_preprocess_views_slideshow_cycle_main_frame_row(&$vars) {
  template_preprocess_views_slideshow_cycle_main_frame_row($vars);
}

/**
 * Backwards compatibility wrapper.
 *
 * @param array $vars
 *   Theme variables.
 *
 * @deprecated Removed in 3.5 when the hook_theme() implementation was fixed.
 *
 * @see template_preprocess_views_slideshow_cycle_main_frame_row_item().
 *
 * @ingroup vss_theme
 */
function _views_slideshow_cycle_preprocess_views_slideshow_cycle_main_frame_row_item(&$vars) {
  template_preprocess_views_slideshow_cycle_main_frame_row_item($vars);
}
