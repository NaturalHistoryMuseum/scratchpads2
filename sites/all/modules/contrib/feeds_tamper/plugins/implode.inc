<?php

$plugin = array(
  'form' => 'feeds_tamper_implode_form',
  'callback' => 'feeds_tamper_implode_callback',
  'validate' => 'feeds_tamper_implode_validate',
  'description' => 'feeds_tamper_implode_description',
  'name' => 'Implode',
  'multi' => 'direct',
  'single' => 'skip',
  'category' => 'List',
);

function feeds_tamper_implode_form($importer, $element_key, $settings) {
  $form = array();
  $form['glue'] = array(
    '#type' => 'textfield',
    '#title' => t('String glue'),
    '#default_value' => isset($settings['glue']) ? $settings['glue'] : ',',
    '#description' => t('Join array elements into a string. For example, array(\'a\', \'b\', \'c\') would become "a, b, c".
                        A space can be represented by %s, tabs by %t, and newlines by %n.'),
  );
  return $form;
}

function feeds_tamper_implode_validate(&$settings) {
  $settings['real_glue'] = str_replace(array('%s', '%t', '%n'), array(' ', "\t", "\n"), $settings['glue']);
}

function feeds_tamper_implode_description($settings) {
  return 'Convert this multi-valued item to a single valued item separated by: "' . $settings['real_glue'] . '".';
}

function feeds_tamper_implode_callback($result, $item_key, $element_key, &$field, $settings) {
  $field = implode($settings['real_glue'], $field);
}
