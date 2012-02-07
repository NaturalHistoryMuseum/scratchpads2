<?php

/**
 * @file
 * Node implementations of the page title hooks
 */


/**
 * Implementation of hook_page_title_alter().
 */
function node_page_title_alter(&$title) {
  $menu_item = menu_get_item();
  // Test if this is a node page.
  if ( !strncmp($menu_item['path'], 'node/%', 6) &&
       ($node = menu_get_object()) &&
       !empty($node->page_title) &&
       variable_get('page_title_type_' . $node->type . '_showfield', 0)) {
    // If the node has a custom page title and the node type is configured
    // to have a custom page title (ie, it's not a leftover from a previous
    // setting), then use it.
    $title = $node->page_title;
  }
}


/**
 * Implementation of hook_page_title_pattern_alter().
 */
function node_page_title_pattern_alter(&$pattern, &$types) {
  $menu_item = menu_get_item();
  // Test if this is a node page.
  if ( !strncmp($menu_item['path'], 'node/%', 6) &&
       $node = menu_get_object() ) {
    $types['node'] = $node;

    // If the node has any taxonomy, grab the first term for use in tokens.
    // TODO: Handle multiple terms? Only pass specific terms per content type?
    // In Drupal 7, terms are no longer in $node->taxomomy. We need to grab all
    // taxonomy_term_reference fields for this bundle and attach them.
    // TODO: Should this be in page_title.taxonomy.inc?!
    $fields = field_info_fields();
    foreach ($fields as $field_name => $field) {
      if ( $field['type'] == 'taxonomy_term_reference' &&
          !empty($field['bundles']['node']) &&
          in_array($node->type, $field['bundles']['node']) &&
          isset($node->{$field_name}[$node->language][0]) ) {
        // Get the term instance.
        $instance = $node->{$field_name}[$node->language][0];
        if (isset($instance['taxonomy_term'])) {
          $types['term'] = $instance['taxonomy_term'];
        break;
        }
        elseif (isset($instance['tid'])) {
          $types['term'] = taxonomy_term_load($instance['tid']);
          break;
        }
      }
    }

    $pattern = variable_get('page_title_type_' . $types['node']->type, '');
  }
}


/**
 * Implementation of hook_page_title_settings().
 */
function node_page_title_settings() {
  $settings = array();

  $types = node_type_get_types();
  foreach ($types as $type) {
    $settings['page_title_type_' . $type->type] = array(
      'label' => 'Content Type - %type',
      'label arguments' => array('%type' => $type->name),
      'scopes' => array('global', 'node', 'term', 'vocabulary'),
      'show field' => TRUE,
      'description' => 'This pattern will be used for all %type node-type pages',
      'description arguments' => array('%type' => $type->name),
    );
  }

  return $settings;
}
