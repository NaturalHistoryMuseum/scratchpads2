<?php

/**
 * @file
 * API documentation for Automatic Entity Label module.
 */

/**
 * Provide post-processing of auto generated titles (labels).
 *
 * @param array $titles
 *   Array of titles keyed by langcode.
 * @param object $entity
 *   The entity that the titles are from.
 *
 * @see auto_entitylabel_set_title
 */
function hook_auto_entitylabel_title_alter(&$titles, $entity) {
  // Trim the title.
  foreach ($titles as $k => $v) {
    $titles[$k] = trim($v);
  }
}
