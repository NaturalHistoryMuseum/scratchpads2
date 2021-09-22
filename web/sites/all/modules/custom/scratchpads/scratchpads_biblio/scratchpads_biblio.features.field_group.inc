<?php
/**
 * @file
 * scratchpads_biblio.features.field_group.inc
 */

/**
 * Implements hook_field_group_default_field_groups().
 */
function scratchpads_biblio_field_group_default_field_groups() {
  $field_groups = array();

  // Exported field_group: 'node-biblio-default-group_container'
  $field_groups['node-biblio-default-group_container'] = array(
    'group_name' => 'group_container',
    'entity_type' => 'node',
    'bundle' => 'biblio',
    'mode' => 'default',
    'parent_name' => '',
    'label' => 'Container',
    'weight' => '4',
    'children' => array(
      0 => 'field_taxonomic_name',
    ),
    'format_type' => 'field_group_stripy_div',
    'format_settings' => array(
      'formatter' => 'open',
    ),
  );

  return $field_groups;
}
