<?php

function scratchpads_tweaks_clear_site_cache($form, $form_state){
  return (array(
    'clear_cache' => array(
      '#type' => 'fieldset',
      '#title' => t('Clear cache'),
      '#description' => "Clearing the site's cache may help to resolve issues with pages not displaying as expected.  Please note, clearing the site's cache will have a short term effect on the performance of the site.",
      'clear' => array(
        '#type' => 'submit',
        '#value' => t('Clear all caches')
      )
    )
  ));
}

/**
 * Submit callback; clear system caches.
 *
 * @ingroup forms
 */
function scratchpads_tweaks_clear_site_cache_submit($form, &$form_state){
  drupal_flush_all_caches();
  drupal_set_message(t('Caches cleared.'));
  $form_state['redirect'] = 'admin/config';
}