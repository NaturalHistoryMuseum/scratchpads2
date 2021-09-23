<?php

/**
 * @file
 * User pages and forms.
 */

function legal_page() {
  global $language;
  $conditions = legal_get_conditions($language->language);
  $output     = '';

  switch (variable_get('legal_display', '0')) {
    case 0: // Scroll Box.
      $output = nl2br(strip_tags($conditions['conditions']));
      break;
    case 1: // CSS Scroll Box with HTML.
    case 2: // HTML.
    case 3: // Page Link.
      $output = filter_xss_admin($conditions['conditions']);
      break;
  }

  return $output;
}
