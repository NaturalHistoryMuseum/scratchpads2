<?php

/**
 * Implementation of hook_node_info().
 */
function migrate_extras_pathauto_node_info() {
  $items = array(
    'migrate_example_pathauto' => array(
      'name' => t('Migrate example - pathauto'),
      'base' => 'node_content',
      'description' => t('This content type is used for demonstrating and testing Migrate Example support for the Pathauto module.'),
      'has_title' => '1',
      'title_label' => t('Title'),
      'help' => '',
    ),
  );
  return $items;
}
