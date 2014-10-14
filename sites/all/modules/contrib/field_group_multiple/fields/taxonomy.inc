<?php
/**
 * @file 
 * 
 * Override for taxonomy_field_widget_form
 */
function field_group_multiple_taxonomy_field_widget_form(&$form, &$form_state, $field, $instance, $langcode, $items, $delta, $element) {
  $_items = array();
  if (isset($items[$delta]) && isset($items[$delta]['tid'])) {
    $_items[$delta] = $items[$delta];
  }
  // TODO find solution for autocomplete
  return taxonomy_field_widget_form($form, $form_state, $field, $instance, $langcode, $_items, $delta, $element);
}
