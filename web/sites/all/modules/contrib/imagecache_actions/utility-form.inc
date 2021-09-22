<?php
/**
 * @file Utility form, conversion and rendering functions for image processes
 */

/**
 * Prepare a subform for displaying positioning fields
 *
 * Helper function to render a common element.
 */
function imagecache_actions_pos_form($action) {
  $defaults = array(
    'xpos' => 'center',
    'ypos' => 'center',
  );
  $action = array_merge($defaults, (array) $action);

  $form = array(
    #'#theme' => 'canvasactions_pos_form',
    'xpos' => array(
      '#type' => 'textfield',
      '#title' => t('X offset'),
      '#default_value' => $action['xpos'],
      '#size' => 6,
      '#description' => t('Enter an offset in pixels or use a keyword: <em>left</em>, <em>center</em>, or <em>right</em>.'),
      '#element_validate' => array('imagecache_actions_validate_number'),
    ),
    'ypos' => array(
      '#type' => 'textfield',
      '#title' => t('Y offset'),
      '#default_value' => $action['ypos'],
      '#size' => 6,
      '#description' => t('Enter an offset in pixels or use a keyword: <em>top</em>, <em>center</em>, or <em>bottom</em>.'),
      '#element_validate' => array('imagecache_actions_validate_number'),
    ),
  );
  return $form;
}

/**
 * Ensure the numbers are valid.
 *
 * Set blanks to zero, just so the status summary doesn't get odd blanks
 */
function imagecache_actions_validate_number(&$element, &$form_state) {
  if (empty($element['#value'])) {
    form_set_value($element, 0, $form_state);
  }
}

/**
 * @todo Please document this function.
 * @see https://drupal.org/node/1354
 */
function imagecache_actions_validate_alpha(&$element, &$form_status) {
  if (!is_numeric($element['#value']) || $element['#value'] < 1 || $element['#value'] > 100) {
    form_set_error(join('][', $element['#parents']), t('Opacity must be a number between 1 and 100.'));
  }
}

