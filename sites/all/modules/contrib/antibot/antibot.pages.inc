<?php

/**
 * The landing page you are brought to if you submit a protected form
 * without Javascript enabled.
 */
function antibot_landing_page() {
  $page = array();
  $page['message'] = array(
    '#type' => 'html_tag',
    '#tag' => 'div',
    '#attributes' => array(
      'class' => array('messages', 'error'),
    ),
    '#value' => t('You have reached this page because you submitted a form that required Javascript to be enabled on your browser. This protection is in place to attempt to prevent automated submissions made on forms. Please return to the page that you came from and enable Javascript on your browser before attempting to submit the form again.'),
  );
  $page['return'] = array(
    '#type' => 'item',
    '#markup' => l(t('Click here to go back'), $_SERVER['HTTP_REFERER']),
    '#access' => (bool) strlen($_SERVER['HTTP_REFERER']),
  );
  return $page;
}
