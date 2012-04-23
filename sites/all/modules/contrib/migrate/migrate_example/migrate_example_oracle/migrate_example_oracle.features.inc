<?php

/**
 * Implementation of hook_node_info().
 */
function migrate_example_oracle_node_info() {
  $items = array(
    'migrate_example_oracle' => array(
      'name' => t('Migrate example - oracle'),
      'base' => 'node_content',
      'description' => t('Example and test fodder for migration directly from an Oracle database.'),
      'has_title' => '1',
      'title_label' => t('Title'),
      'help' => '',
    ),
  );
  return $items;
}
