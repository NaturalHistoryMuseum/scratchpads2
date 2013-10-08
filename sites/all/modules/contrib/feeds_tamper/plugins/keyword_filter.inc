<?php

/**
 * @file
 * Filter based on a list of words/phrases.
 */

$plugin = array(
  'form' => 'feeds_tamper_keyword_filter_form',
  'callback' => 'feeds_tamper_keyword_filter_callback',
  'validate' => 'feeds_tamper_keyword_filter_validate',
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
    '#default_value' => isset($settings['word_boundaries']) ? $settings['word_boundaries'] : FALSE,
    '#description' => t('If checked, then "book" will match "book" but not "bookcase".'),
  );
  $form['exact'] = array(
    '#type' => 'checkbox',
    '#title' => t('Exact'),
    '#default_value' => isset($settings['exact']) ? $settings['exact'] : FALSE,
    '#description' => t('If checked, then "book" will only match "book". This will override the "Respect word boundaries" setting above.'),
  );
  $form['case_sensitive'] = array(
    '#type' => 'checkbox',
    '#title' => t('Case sensitive'),
    '#default_value' => isset($settings['case_sensitive']) ? $settings['case_sensitive'] : FALSE,
    '#description' => t('If checked, then "book" will match "book" but not "Book" or "BOOK".'),
  );
  $form['invert'] = array(
    '#type' => 'checkbox',
    '#title' => t('Invert filter'),
    '#default_value' => isset($settings['invert']) ? $settings['invert'] : FALSE,
    '#description' => t('Inverting the filter will %remove items with the specified text.', array('%remove' => 'remove')),
  );
  return $form;
}

function feeds_tamper_keyword_filter_validate(&$settings) {
  global $multibyte;
  $is_multibyte = ($multibyte == UNICODE_MULTIBYTE) ? TRUE : FALSE;

  $settings['words'] = str_replace("\r", '', $settings['words']);
  $settings['word_list'] = explode("\n", $settings['words']);
  $settings['word_list'] = array_map('trim', $settings['word_list']);
  $settings['regex'] = FALSE;

  if (!empty($settings['exact']) || $settings['word_boundaries']) {
    foreach ($settings['word_list'] as &$word) {
      if (!empty($settings['exact'])) {
        $word = '/^\Q' . $word . '\E$/u';
      }
      elseif ($settings['word_boundaries']) {
        // Word boundaries can only match a word with letters at the end.
        if (!preg_match('/^\w(.*\w)?$/u', $word)) {
          form_set_error('settings][words', t('Search text must begin and end with a letter, number, or underscore to use the %option option.<br />%word', array('%option' => 'Respect word boundaries', '%word' => $word)));
        }
        $word = '/\b\Q' . $word . '\E\b/u';
      }
      if (!$settings['case_sensitive']) {
        $word .= 'i';
      }
    }
    $settings['regex'] = TRUE;
  }

  elseif (!$settings['word_boundaries'] && $settings['case_sensitive']) {
    $settings['func'] = $is_multibyte ? 'mb_strpos' : 'strpos';
  }
  elseif (!$settings['word_boundaries'] && !$settings['case_sensitive']) {
    $settings['func'] = $is_multibyte ? 'mb_stripos' : 'stripos';
  }
}

function feeds_tamper_keyword_filter_callback($result, $item_key, $element_key, &$field, $settings, $source) {
  if ($settings['regex']) {
    foreach ($settings['word_list'] as $word) {
      if (preg_match($word, $field) > 0) {
        if (!$settings['invert']) {
          return;
        }
        unset($result->items[$item_key]);
        return;
      }
    }
  }
  else {
    foreach ($settings['word_list'] as $word) {
      if ($settings['func']($field, $word) !== FALSE) {
        if (!$settings['invert']) {
          return;
        }
        unset($result->items[$item_key]);
        return;
      }
    }
  }
  if (!$settings['invert']) {
    unset($result->items[$item_key]);
  }
}
