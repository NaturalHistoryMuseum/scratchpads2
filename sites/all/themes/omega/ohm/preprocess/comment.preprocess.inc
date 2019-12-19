<?php

/**
 * @file
 * Contains a pre-process hook for 'comment'.
 */

/**
 * Implements hook_preprocess_TEMPLATE().
 */
function ohm_preprocess_comment(&$variables) {
  $variables['attributes_array']['class'][] = 'clearfix';

  $created = format_interval(REQUEST_TIME - $variables['comment']->created);
  $changed = format_interval(REQUEST_TIME - $variables['comment']->changed);

  $variables['created'] = t('Posted @date ago', array('@date' => $created));
  $variables['changed'] = t('Posted @date ago', array('@date' => $changed));
}
