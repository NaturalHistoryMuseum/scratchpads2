<?php

/**
 * Implementation of hook_node_info().
 */
function migrate_example_baseball_node_info() {
  $items = array(
    'migrate_example_baseball' => array(
      'name' => t('migrate_example_baseball'),
      'base' => 'node_content',
      'description' => t('A baseball box score'),
      'has_title' => '1',
      'title_label' => t('Title'),
      'help' => '',
    ),
  );
  return $items;
}
