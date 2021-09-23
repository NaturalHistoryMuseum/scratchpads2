<?php

/**
 * @file
 * Hooks provided by Views Slideshow.
 */

/**
 * @defgroup vss_hooks Hooks
 * @{
 * Hooks for modules to implement to extend or modify Views Slideshow.
 *
 * Allows modules to create actual slideshow implementations or add
 * widgets such as pagers.
 *
 * @see contrib\views_slideshow_simple_pager\views_slideshow_simple_pager.module
 * @see https://api.drupal.org/api/drupal/includes%21module.inc/group/hooks/7.x
 */

/**
 * Define the type of the slideshow (eg.: cycle, imageflow, ddblock).
 *
 * @return array
 *   Associative array of slideshow type and its information.
 */
function hook_views_slideshow_slideshow_info() {
  $options = array(
    'views_slideshow_cycle' => array(
      'name' => t('Cycle'),
      'accepts' => array(
        'goToSlide',
        'nextSlide',
        'pause',
        'play',
        'previousSlide',
      ),
      'calls' => array(
        'transitionBegin',
        'transitionEnd',
        'goToSlide',
        'pause',
        'play',
        'nextSlide',
        'previousSlide',
      ),
    ),
  );
  return $options;
}

/**
 * Define form fields to be displayed in the views settings form.
 *
 * These fields would help configure your slideshow type.
 */
function hook_views_slideshow_slideshow_type_form(&$form, &$form_state, &$view) {
  $form['views_slideshow_cycle']['effect'] = array(
    '#type' => 'select',
    '#title' => t('Effect'),
    '#options' => $effects,
    '#default_value' => $view->options['views_slideshow_cycle']['effect'],
    '#description' => t('The transition effect that will be used to change between images. Not all options below may be relevant depending on the effect. !link', array('!link' => l(t('Follow this link to see examples of each effect.'), 'http://jquery.malsup.com/cycle/browser.html', array('attributes' => array('target' => '_blank'))))),
  );
}

/**
 * Set default values for options specified in hook_views_slideshow_type_form.
 *
 * @return array
 *   Associative array of slideshow type name and options.
 */
function hook_views_slideshow_option_definition() {
  $options['views_slideshow_cycle'] = array(
    'contains' => array(
      // Transition.
      'effect' => array('default' => 'fade'),
      'transition_advanced' => array('default' => 0),
      'timeout' => array('default' => 5000),
      'speed' => array('default' => 700),
      'delay' => array('default' => 0),
      'sync' => array('default' => 1),
      'random' => array('default' => 0),
    ),
  );
  return $options;
}

/**
 * Form validation callback for the slideshow settings.
 */
function hook_views_slideshow_options_form_validate(&$form, &$form_state, &$view) {
  if (!is_numeric($form_state['values']['style_options']['views_slideshow_cycle']['speed'])) {
    form_error($form['views_slideshow_cycle']['speed'], t('!setting must be numeric!', array('!setting' => 'Speed')));
  }
  if (!is_numeric($form_state['values']['style_options']['views_slideshow_cycle']['timeout'])) {
    form_error($form['views_slideshow_cycle']['timeout'], t('!setting must be numeric!', array('!setting' => 'Timeout')));
  }
  if (!is_numeric($form_state['values']['style_options']['views_slideshow_cycle']['remember_slide_days'])) {
    form_error($form['views_slideshow_cycle']['remember_slide_days'], t('!setting must be numeric!', array('!setting' => 'Slide days')));
  }
}

/**
 * Form submission callback for the slideshow settings.
 */
function hook_views_slideshow_options_form_submit($form, &$form_state) {
  // Act on option submission.
}

/**
 * Define slideshow skins to be available to the end user.
 */
function hook_views_slideshow_skin_info() {
  return array(
    'default' => array(
      'name' => t('Default'),
    ),
  );
}

/**
 * Define new widgets (pagers, controls, counters).
 *
 * Available events for accepts and calls
 *  - pause
 *  - play
 *  - nextSlide
 *  - previousSlide
 *  - goToSlide
 *  - transitionBegin
 *  - transitionEnd.
 *
 * @return array
 *   Array keyed by the widget names.
 */
function hook_views_slideshow_widget_info() {
  return array(
    'views_slideshow_pager' => array(
      'name' => t('Pager'),
      'accepts' => array(
        'transitionBegin' => array('required' => TRUE),
        'goToSlide' => array(),
        'previousSlide' => array(),
        'nextSlide' => array(),
      ),
      'calls' => array(
        'goToSlide',
        'pause',
        'play',
      ),
    ),
    'views_slideshow_controls' => array(
      'name' => t('Controls'),
      'accepts' => array(
        'pause' => array('required' => TRUE),
        'play' => array('required' => TRUE),
      ),
      'calls' => array(
        'nextSlide',
        'pause',
        'play',
        'previousSlide',
      ),
    ),
    'views_slideshow_slide_counter' => array(
      'name' => t('Slide Counter'),
      'accepts' => array(
        'transitionBegin' => array('required' => TRUE),
        'goToSlide' => array(),
        'previousSlide' => array(),
        'nextSlide' => array(),
      ),
      'calls' => array(),
    ),
  );
}

/**
 * Form fields to be added for a specific widget type.
 *
 * Example of a widget type would be views_slideshow_pager
 * or views_slideshow_slide_counter.
 */
function INSERT_WIDGET_TYPE_HERE_views_slideshow_widget_form_options(&$form, $form_state, $view, $defaults, $dependency) {
}

/**
 * Hook called by the pager widget to configure it, the fields that should be shown.
 */
function hook_views_slideshow_widget_pager_info($view) {
}

/**
 * Hook called by the pager widget to add form items.
 */
function INSERT_WIDGET_TYPE_HERE_views_slideshow_widget_pager_form_options(&$form, &$form_state, &$view, $defaults, $dependency) {
}

/**
 * Hook called by the controls widget to configure it, the fields that should be shown.
 */
function hook_views_slideshow_widget_controls_info($view) {
}

/**
 * Hook called by the controls widget to add form items.
 */
function INSERT_WIDGET_TYPE_HERE_views_slideshow_widget_controls_form_options(&$form, &$form_state, &$view, $defaults, $dependency) {
}

/**
 * @} End of "defgroup vss_hooks".
 */
