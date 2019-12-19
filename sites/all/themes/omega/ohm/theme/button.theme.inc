<?php

/**
 * @file
 * Contains the theme function override for 'button'.
 */

/**
 * Returns HTML for a button form element.
 *
 * @ingroup themeable
 */
function ohm_button($variables) {
  $element = $variables['element'];
  $element['#attributes']['type'] = 'submit';
  element_set_attributes($element, array('id', 'name', 'value'));

  $element['#attributes']['class'][] = 'form-' . $element['#button_type'];
  $element['#attributes']['class'][] = 'button';
  if (!empty($element['#attributes']['disabled'])) {
    $element['#attributes']['class'][] = 'button--inactive';
  }

  // Catch some primary button functions.
  foreach (array('save', 'new', 'add', 'confirm', 'done', 'submit') as $primary) {
    if (isset($element['#id']) && substr_count($element['#id'], $primary)) {
      $element['#attributes']['class'][] = 'button--primary';
    }
  }

  // Catch some negative button functions.
  foreach (array('cancel', 'remove', 'delete') as $negative) {
    if (isset($element['#id']) && substr_count($element['#id'], $negative)) {
      $element['#attributes']['class'][] = 'button--negative';
    }
  }

  return '<input' . drupal_attributes($element['#attributes']) . ' />';
}
