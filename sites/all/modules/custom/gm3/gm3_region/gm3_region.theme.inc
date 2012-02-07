<?php

/**
 * Region tools button
 */
function theme_gm3_region_button($variables){
  return '<div data-gm3-class="region" class="gm3-tools-button"><p>' . t('Region') . ' +</p></div>';
}

/**
 * Themes for field types.
 */
function theme_gm3_region_region_text($variables){
  return array(
    '#markup' => t('Region: %region_id', array(
      '%region_id' => $variables['data']['region_id']
    ))
  );
}