<?php

/**
 * @file
 * Holds Openlayers test views
 */

/**
 * Implements hook_views_default_views().
 */
function _openlayers_test_views_default_views() {
  // Only provide view if everything is already setup properly
  if (module_exists('content') && module_exists('content_copy')) {
    $field = content_fields('field_openlayers_test_wkt', 'openlayers_test_type');
    if (!empty($field)) {
      $view = new view;
      $view->name = 'openlayers_test_view';
      $view->description = 'OpenLayers Test View';
      $view->tag = '';
      $view->view_php = '';
      $view->base_table = 'node';
      $view->is_cacheable = FALSE;
      $view->api_version = 2;
      $view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */
      $handler = $view->new_display('default', 'OpenLayers Test View Default Display', 'default');
      $handler->override_option('fields', array(
        'field_openlayers_test_wkt_openlayers_wkt' => array(
          'label' => '',
          'alter' => array(
            'alter_text' => 0,
            'text' => '',
            'make_link' => 0,
            'path' => '',
            'link_class' => '',
            'alt' => '',
            'prefix' => '',
            'suffix' => '',
            'target' => '',
            'help' => '',
            'trim' => 0,
            'max_length' => '',
            'word_boundary' => 1,
            'ellipsis' => 1,
            'strip_tags' => 0,
            'html' => 0,
          ),
          'empty' => '',
          'hide_empty' => 0,
          'empty_zero' => 0,
          'link_to_node' => 0,
          'label_type' => 'none',
          'format' => 'openlayers_wkt',
          'multiple' => array(
            'group' => TRUE,
            'multiple_number' => '',
            'multiple_from' => '',
            'multiple_reversed' => FALSE,
          ),
          'exclude' => 0,
          'id' => 'field_openlayers_test_wkt_openlayers_wkt',
          'table' => 'node_data_field_openlayers_test_wkt',
          'field' => 'field_openlayers_test_wkt_openlayers_wkt',
          'relationship' => 'none',
        ),
        'teaser' => array(
          'label' => '',
          'alter' => array(
            'alter_text' => 0,
            'text' => '',
            'make_link' => 0,
            'path' => '',
            'link_class' => '',
            'alt' => '',
            'prefix' => '',
            'suffix' => '',
            'target' => '',
            'help' => '',
            'trim' => 0,
            'max_length' => '',
            'word_boundary' => 1,
            'ellipsis' => 1,
            'strip_tags' => 0,
            'html' => 0,
          ),
          'empty' => '',
          'hide_empty' => 0,
          'empty_zero' => 0,
          'exclude' => 0,
          'id' => 'teaser',
          'table' => 'node_revisions',
          'field' => 'teaser',
          'relationship' => 'none',
        ),
        'title' => array(
          'label' => '',
          'alter' => array(
            'alter_text' => 0,
            'text' => '',
            'make_link' => 0,
            'path' => '',
            'link_class' => '',
            'alt' => '',
            'prefix' => '',
            'suffix' => '',
            'target' => '',
            'help' => '',
            'trim' => 0,
            'max_length' => '',
            'word_boundary' => 1,
            'ellipsis' => 1,
            'strip_tags' => 0,
            'html' => 0,
          ),
          'empty' => '',
          'hide_empty' => 0,
          'empty_zero' => 0,
          'link_to_node' => 1,
          'exclude' => 0,
          'id' => 'title',
          'table' => 'node',
          'field' => 'title',
          'relationship' => 'none',
        ),
      ));
      $handler->override_option('filters', array(
        'type' => array(
          'operator' => 'in',
          'value' => array(
            'openlayers_test_type' => 'openlayers_test_type',
          ),
          'group' => '0',
          'exposed' => FALSE,
          'expose' => array(
            'operator' => FALSE,
            'label' => '',
          ),
          'id' => 'type',
          'table' => 'node',
          'field' => 'type',
          'relationship' => 'none',
        ),
      ));
      $handler->override_option('access', array(
        'type' => 'none',
      ));
      $handler->override_option('cache', array(
        'type' => 'none',
      ));
      $handler->override_option('title', 'OpenLayers Test View Layer');
      $handler->override_option('items_per_page', 200);
      $handler->override_option('style_plugin', 'openlayers_data');
      $handler->override_option('style_options', array(
        'grouping' => '',
        'data_source' => array(
          'value' => array(
            'openlayers_wkt' => 'openlayers_wkt',
          ),
          'other_lat' => 'field_openlayers_test_wkt_openlayers_wkt',
          'other_lon' => 'field_openlayers_test_wkt_openlayers_wkt',
          'openlayers_wkt' => 'field_openlayers_test_wkt_openlayers_wkt',
          'other_top' => 'field_openlayers_test_wkt_openlayers_wkt',
          'other_right' => 'field_openlayers_test_wkt_openlayers_wkt',
          'other_bottom' => 'field_openlayers_test_wkt_openlayers_wkt',
          'other_left' => 'field_openlayers_test_wkt_openlayers_wkt',
          'name_field' => 'title',
          'description_field' => 'teaser',
        ),
      ));
      $handler = $view->new_display('openlayers', 'OpenLayers Test View OpenLayers Display', 'openlayers_1');
      $handler->override_option('row_plugin', '');
      $handler->override_option('displays', array());

      // Return
      $views['openlayers_test_view'] = $view;
      return $views;
    }
  }
}
