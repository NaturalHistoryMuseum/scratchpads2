<?php

/**
 * @file
 * Page callbacks and functions.
 */

/**
 * The landing page callback.
 *
 * You are brought here if you submit a protected form without JavaScript
 * enabled.
 */
function antibot_landing_page() {
  $page = array();
  $page['message'] = array(
    '#type' => 'html_tag',
    '#tag' => 'div',
    '#attributes' => array(
      'class' => array('messages', 'error'),
    ),
    '#value' => t('You have reached this page because you submitted a form that required JavaScript to be enabled on your browser. This protection is in place to attempt to prevent automated submissions made on forms. Please return to the page that you came from and enable JavaScript on your browser before attempting to submit the form again.'),
  );
  return $page;
}
