<?php

/**
 * Callback for the publish page for a given publication
 * 
 */
function publication_publish_page($publication){
  $output = drupal_get_form($publication->type . '_publication_publish_form', $publication);
  if(!$output){
    $output = array(
      '#type' => 'markup',
      '#markup' => '<div>' . t('This publication type does not provide a publish page') . '</div>'
    );
  }
  return $output;
}