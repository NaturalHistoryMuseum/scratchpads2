<?php

$plugin = array(
  'form' => 'feeds_tamper_keyword_filter_form',
  'callback' => 'feeds_tamper_keyword_filter_callback',
  'validate' => 'feeds_tamper_keyword_filter_validate',
  'description' => 'feeds_tamper_keyword_filter_description',
  'name' => 'Keyword filter',
  'multi' => 'skip',
  'category' => 'Filter',
);

function feeds_tamper_keyword_filter_form($importer, $element_key, $settings) {
  $form = array();
  $form['words'] = array(
    '#type' => 'textarea',
    '#title' => t('Words or phrases to filter on'),
    '#default_value' => isset($settings['words']) ? $settings['words'] : '',
    '#description' => t('A list of words/phrases that need to appear in the text.'),
  );
  $form['word_boundaries'] = array(
    '#type' => 'checkbox',
    '#title' => t('Respect word boundaries'),
    '#default_value' => isset($settings['word_boundaries']) ? $settings['word_boundaries'] : TRUE,
    '#description' => t('If checked, then "book" will match "book" but not "bookcase".'),
  );
  $form['case_sensitive'] = array(
    '#type' => 'checkbox',
    '#title' => t('Case sensitive'),
    '#default_value' => isset($settings['case_sensitive']) ? $settings['case_sensitive'] : FALSE,
    '#description' => t('If checked, then "book" will match "book" but not "Book" or "BOOK".')
  );
  return $form;
}

function feeds_tamper_keyword_filter_validate(&$settings) {
  $settings['words'] = str_replace("\r", '', $settings['words']);
  $settings['word_list'] = explode("\n", $settings['words']);
  $settings['word_list'] = array_map('trim', $settings['word_list']);
  $settings['regex'] = FALSE;

  if (!$settings['word_boundaries'] && $settings['case_sensitive']) {
    $settings['func'] = 'strpos';
  }
  else if (!$settings['word_boundaries'] && !$settings['case_sensitive']) {
    $settings['func'] = 'stripos';
  }
  else {
    foreach ($settings['word_list'] as &$word) {
      $word = '/\b' . $word . '\b/';

      if (!$settings['case_sensitive']) {
        $word .= 'i';
      }
    }
    $settings['regex'] = TRUE;
  }
}

function feeds_tamper_keyword_filter_description($settings) {
  $words = explode("\n", $settings['words']);
  $words = implode(', ', $words);
  $words = substr($words, 0, 15);
  return 'Filter items that do not contain one of "' . $words . '".';
}

function feeds_tamper_keyword_filter_callback($result, $item_key, $element_key, &$field, $settings) {
  if ($settings['regex']) {
    foreach ($settings['word_list'] as $word) {
      if (preg_match($word, $field) > 0) {
        return;
      }
    }
  }
  else {
    foreach ($settings['word_list'] as $word) {
      if ($settings['func']($field, $word) !== FALSE) {
        return;
      }
    }
  }
  unset($result->items[$item_key]);
}
