<?php

/**
 * @file
 * The theme system, which controls the output of views slideshow.
 *
 * This just adds a wrapper div to the slideshow.
 */

function _views_slideshow_preprocess_views_slideshow(&$vars) {
  $options = $vars['options'];
  $vars['skin'] = 'default';
  $vars['slideshow'] = '';
  $main_frame_module = $options['slideshow_type'];

  if (empty($main_frame_module)) {
    // Get all slideshow types.
    $slideshows = module_invoke_all('views_slideshow_slideshow_info');

    if ($slideshows) {
      foreach ($slideshows as $slideshow_id => $slideshow_info) {
        $main_frame_module = $slideshow_id;
        break;
      }
    }
  }
  
  // Make sure the main slideshow settings are defined before building the
  // slideshow.
  if (empty($main_frame_module)) {
    drupal_set_message(t('No main frame module is enabled for views slideshow.'), 'error');
  }
  elseif (empty($options[$main_frame_module])) {
    drupal_set_message(t('The options for !module does not exists.', array('!module' => $main_frame_module)), 'error');
  }
  else {
    $settings = $options[$main_frame_module];
    $view = $vars['view'];
    $rows = $vars['rows'];
    $num_divs = count($rows);
    $vss_id = $view->name . '-' . $view->current_display;

    // Give each slideshow a unique id if there are more than one on the page.
    static $instances = array();
    if (isset($instances[$vss_id])) {
      $instances[$vss_id]++;
      $vss_id .= "_" . $instances[$vss_id];
    }
    else {
      $instances[$vss_id] = 1;
    }

    // Building our default methods.
    $methods = array(
      'goToSlide' => array(),
      'nextSlide' => array(),
      'pause' => array(),
      'play' => array(),
      'previousSlide' => array(),
      'transitionBegin' => array(),
      'transitionEnd' => array(),
    );

    // Pull all widget info and slideshow info and merge them together.
    $widgets = module_invoke_all('views_slideshow_widget_info');
    $slideshows = module_invoke_all('views_slideshow_slideshow_info');
    $addons = array_merge($widgets, $slideshows);

    // Loop through all the addons and call their methods if needed.
    foreach ($addons as $addon_id => $addon_info) {
      foreach ($addon_info['accepts'] as $imp_key => $imp_value) {
        if (is_array($imp_value)) {
          $methods[$imp_key][] = preg_replace('/_(.?)/e',"strtoupper('$1')", $addon_id);
        }
        else {
          $methods[$imp_value][] = preg_replace('/_(.?)/e',"strtoupper('$1')", $addon_id);
        }
      }
    }

    $js_settings = array(
      'viewsSlideshow' => array(
        $vss_id => array(
          'methods' => $methods,
          'paused' => 0,
        )
      )
    );
    drupal_add_js($js_settings, 'setting');

    /**
     * Process Skins
     */
    $skin_info = array();
    if (isset($options['skin_info'])) {
      $skin_info = $options['skin_info'];
    }

    // Make sure $skin_info has all the values.
    $skin_info += array(
      'class' => 'default',
      'name' => t('Untitled skin'),
      'module' => 'views_slideshow',
      'path' => '',
      'stylesheets' => array(),
    );

    $vars['skin'] = $skin_info['class'];

    // Enqueue any stylesheets set for the skin on this view are added.
    $skin_path = drupal_get_path('module', $skin_info['module']);
    if ($skin_info['path']) {
      $skin_path .= '/' . $skin_info['path'];
    }

    // Add stylesheet.
    if (!empty($skin_info['stylesheets'])) {
      foreach ($skin_info['stylesheets'] as $stylesheet) {
        drupal_add_css($skin_path . '/' . $stylesheet);
      }
    }

    /**
     * Process Widgets
     */

    // Build weights
    for ($i = 1; $i <= count($widgets); $i++) {
      $weight['top'][$i] = '';
      $weight['bottom'][$i] = '';
    }

    foreach ($widgets as $widget_id => $widget_name) {
      // Put our widgets in the right location.
      if ($options['widgets']['top'][$widget_id]['enable']) {
        $widget_weight = ($options['widgets']['top'][$widget_id]['weight'] > count($widgets)) ? count($widgets) : $options['widgets']['top'][$widget_id]['weight'];
        $weight['top'][$widget_weight][] = $widget_id;
      }

      if ($options['widgets']['bottom'][$widget_id]['enable']) {
        $widget_weight = ($options['widgets']['bottom'][$widget_id]['weight'] > count($widgets)) ? count($widgets) : $options['widgets']['bottom'][$widget_id]['weight'];
        $weight['bottom'][$widget_weight][] = $widget_id;
      }
    }

    // Build our widgets
    foreach ($weight as $location => $order) {
      $vars[$location . '_widget_rendered'] = '';
      foreach ($order as $order_num => $widgets) {
        if (is_array($widgets)) {
          foreach ($widgets as $widget_id) {
            $vars[$widget_id . '_' . $location] = theme($widget_id . '_widget_render', array('vss_id' => $vss_id, 'view' => $view, 'settings' => $options['widgets'][$location][$widget_id], 'location' => $location, 'rows' => $rows));
            $vars[$location . '_widget_rendered'] .= $vars[$widget_id . '_' . $location];
          }
        }
      }
    }

    /**
     * Process Slideshow
     */
    $slides = theme($main_frame_module . '_main_frame', array('vss_id' => $vss_id, 'view' => $view, 'settings' => $settings, 'rows' => $rows));
    $vars['slideshow'] = theme('views_slideshow_main_section', array('vss_id' => $vss_id, 'slides' => $slides, 'plugin' => $main_frame_module));
  }
}

/**
 * The current element of the slideshow.
 *
 * @ingroup themeable
 */
function theme_views_slideshow_main_section($vars) {
  return '<div id="' . $vars['plugin'] . '_main_' . $vars['vss_id'] . '" class="' .  $vars['plugin'] . '_main views_slideshow_main">' . $vars['slides'] . '</div>';
}

/**
 * Views Slideshow: pager.
 *
 * @ingroup themeable
 */
function theme_views_slideshow_pager_widget_render($vars) {
  // Add javascript settings for the pager type.
  $js_vars = array(
    'viewsSlideshowPager' => array(
      $vars['vss_id'] => array(
        $vars['location'] => array(
          'type' => preg_replace('/_(.?)/e',"strtoupper('$1')", $vars['settings']['type']),
        ),
      ),
    ),
  );

  drupal_add_js($js_vars, 'setting');

  // Create some attributes
  $attributes['class'] = 'widget_pager widget_pager_' . $vars['location'];
  $attributes['id'] = 'widget_pager_' . $vars['location'] . '_' . $vars['vss_id'];
  return theme($vars['settings']['type'], array('vss_id' => $vars['vss_id'], 'view' => $vars['view'], 'settings' => $vars['settings'], 'location' => $vars['location'], 'attributes' => $attributes));
}

/**
 * Theme pager fields
 */
function _views_slideshow_preprocess_views_slideshow_pager_fields(&$vars) {
  // Build our javascript settings.
  $js_vars = array(
    'viewsSlideshowPagerFields' => array(
      $vars['vss_id'] => array(
        $vars['location'] => array(
          'activatePauseOnHover' => $vars['settings']['views_slideshow_pager_fields_hover'],
        ),
      ),
    ),
  );

  // Add the settings to the page.
  drupal_add_js($js_vars, 'setting');
  
  // Add hover intent library
  if ($vars['settings']['views_slideshow_pager_fields_hover']) {
    if (module_exists('libraries')) {
      // Load jQuery hoverIntent
      $hoverIntent_path = libraries_get_path('jquery.hoverIntent');
      if (!empty($hoverIntent_path) && file_exists($hoverIntent_path . '/jquery.hoverIntent.js')) {
        drupal_add_js($hoverIntent_path . '/jquery.hoverIntent.js');
      }
    }
  }

  $vars['classes_array'][] = $vars['attributes']['class'];
  $vars['widget_id'] = $vars['attributes']['id'];
  // Add our class to the wrapper.
  $vars['classes_array'][] = 'views_slideshow_pager_field';

  // Render all the fields unless there is only 1 slide and the user specified
  // to hide them when there is only one slide.
  $vars['rendered_field_items'] = '';
  if (empty($vars['settings']['hide_on_single_slide']) && count($vars['view']->result) > 1) {
    foreach ($vars['view']->result as $count => $node) {
      $rendered_fields = '';
      foreach ($vars['settings']['views_slideshow_pager_fields_fields'] as $field => $use) {
        if ($use !== 0 && is_object($vars['view']->field[$field])) {
          $rendered_fields .= theme('views_slideshow_pager_field_field', array('view' => $vars['view'], 'field' => $field, 'count' => $count));
        }
      }
      $vars['rendered_field_items'] .= theme('views_slideshow_pager_field_item', array('vss_id' => $vars['vss_id'], 'item' => $rendered_fields, 'count' => $count, 'location' => $vars['location']));
    }
  }
}

/**
 * Views Slideshow: pager item.
 *
 * @ingroup themeable
 */
function _views_slideshow_preprocess_views_slideshow_pager_field_item(&$vars) {
  $vars['classes_array'][] = 'views_slideshow_pager_field_item';
  if (!$vars['count']) {
    $vars['classes_array'][] = 'views_slideshow_active_pager_field_item';
  }
  $vars['classes_array'][] = ($vars['count'] % 2) ? 'views-row-even' : 'views-row-odd';
}

/**
 * Views Slideshow: Controls.
 *
 * @inggroup themeable
 */
function theme_views_slideshow_controls_widget_render($vars) {
  // Add javascript settings for the controls type.
  $js_vars = array(
    'viewsSlideshowControls' => array(
      $vars['vss_id'] => array(
        $vars['location'] => array(
          'type' => preg_replace('/_(.?)/e',"strtoupper('$1')", $vars['settings']['type']),
        ),
      ),
    ),
  );

  drupal_add_js($js_vars, 'setting');

  $output = '';
  if (empty($vars['settings']['hide_on_single_slide']) && count($vars['rows']) > 1) {
    $output = theme($vars['settings']['type'], array('vss_id' => $vars['vss_id'], 'view' => $vars['view'], 'settings' => $vars['settings'], 'location' => $vars['location'], 'rows' => $vars['rows']));
  }

  return $output;
}

/**
 * The slideshow controls.
 *
 * @ingroup themeable
 */
function _views_slideshow_preprocess_views_slideshow_controls_text(&$vars) {
  $module_path = drupal_get_path('module', 'views_slideshow');
  drupal_add_css($module_path . '/views_slideshow_controls_text.css', array('type' => 'file'));

  $vars['classes_array'][] = 'views_slideshow_controls_text';

  $vars['rendered_control_previous'] = theme('views_slideshow_controls_text_previous', array('vss_id' => $vars['vss_id'], 'view' => $vars['view'], 'settings' => $vars['settings']));

  $vars['rendered_control_pause'] = theme('views_slideshow_controls_text_pause', array('vss_id' => $vars['vss_id'], 'view' => $vars['view'], 'settings' => $vars['settings']));

  $vars['rendered_control_next'] = theme('views_slideshow_controls_text_next', array('vss_id' => $vars['vss_id'], 'view' => $vars['view'], 'settings' => $vars['settings']));
}

/**
 * Views Slideshow: "previous" control.
 *
 * @ingroup themeable
 */
function _views_slideshow_preprocess_views_slideshow_controls_text_previous(&$vars) {
  $vars['classes_array'][] = 'views_slideshow_controls_text_previous';
}

/**
 * Views Slideshow: "pause" control.
 *
 * @ingroup themeable
 */
function _views_slideshow_preprocess_views_slideshow_controls_text_pause(&$vars) {
  $vars['classes_array'][]  = 'views_slideshow_controls_text_pause';
  $vars['start_text'] = t('Pause');
}

/**
 * Views Slideshow: "next" control.
 *
 * @ingroup themeable
 */
function _views_slideshow_preprocess_views_slideshow_controls_text_next(&$vars) {
  $vars['classes_array'][] = 'views_slideshow_controls_text_next';
}

/**
 * Views Slideshow: Slide Counter.
 *
 * @inggroup themeable
 */
function theme_views_slideshow_slide_counter_widget_render($vars) {
  return theme('views_slideshow_slide_counter', array('vss_id' => $vars['vss_id'], 'view' => $vars['view'], 'settings' => $vars['settings'], 'location' => $vars['location'], 'rows' => $vars['rows']));
}

/**
 * Views Slideshow: slide counter.
 */
function _views_slideshow_preprocess_views_slideshow_slide_counter(&$vars) {
  $vars['classes_array'][] = 'views_slideshow_slide_counter';
}
