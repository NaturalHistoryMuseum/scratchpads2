<?php

$plugin = array(
  'form' => 'feeds_tamper_explode_form',
  'callback' => 'feeds_tamper_explode_callback',
  'validate' => 'feeds_tamper_explode_validate',
  'name' => 'Explode',
  'multi' => 'skip',
  'category' => 'List',
  'description' => 'feeds_tamper_explode_description',
);

function feeds_tamper_explode_form($importer, $element_key, $settings) {
  $form = array();
  $form['separator'] = array(
    '#type' => 'textfield',
    '#title' => t('String separator'),
    '#default_value' => isset($settings['separator']) ? $settings['separator'] : ',',
    '#description' => t('This will break up sequenced data into an array. For example, "a, b, c" would get broken up into the array(\'a\', \'b\', \'c\').
                        A space can be represented by %s, tabs by %t, and newlines by %n.'),
  );
  $form['limit'] = array(
    '#type' => 'textfield',
    '#title' => t('Limit'),
    '#default_value' => isset($settings['limit']) ? $settings['limit'] : '',
    '#description' => t('If limit is set and positive, the returned items will contain a maximum of limit with the last item containing the rest of string.
                        If limit is negative, all components except the last -limit are returned. If the limit parameter is zero, then this is treated as 1.'),
  );
  return $form;
}

function feeds_tamper_explode_validate(&$settings) {
  $settings['real_separator'] = str_replace(array('%s', '%t', '%n'), array(' ', "\t", "\n"), $settings['separator']);
  $settings['limit'] = trim($settings['limit']);
  if ($settings['limit'] == 0) {
    $settings['limit'] = 1;
  }
  if (!is_numeric($settings['limit'])) {
    form_set_error('explode][limit', t('Limit must be an integer.'));
  }
}

function feeds_tamper_explode_description($settings) {
  $num = !empty($settings['limit']) ? $settings['limit'] : '';
  $text = 'Break text into items at every "' . check_plain($settings['separator']) . '".';
  if ($settings['limit']) {
    $text .= ' Limit to ' . $settings['limit'] . ' items.';
  }
  return $text;
}

function feeds_tamper_explode_callback($result, $item_key, $element_key, &$field, $settings) {
  $field = explode($settings['real_separator'], $field);
}
