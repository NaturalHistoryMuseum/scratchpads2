<?php

/**
 * Callback for the publish page for a given publication
 * 
 */
function publication_publish_page($publication) {
  $output = "";
  if (!$publication->published) {
    $output = drupal_get_form($publication->type . '_publication_new_form', $publication);
    if (!$output) {
      $output = array(
        '#type' => 'markup',
        '#markup' => '<div>' . t('This publication has not been published') . '</div>'
      ); 
    }
  } else {
    $output = drupal_get_form($publication->type . '_publication_action_form', $publication);
    if (!$output) {
      $output = array(
        '#type' => 'markup',
        '#markup' => '<div>' . t('This publication has been published') . '</div>'
      );
    }
  }
  
  return $output;
}