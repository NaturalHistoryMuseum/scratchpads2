<?php

/**
 * @file
 * scratchpads_ecological_relations_block.views_default.inc
 */
/**
 * Implements hook_views_default_views().
 */
function scratchpads_ecological_relations_block_views_default_views(){
  $export = array();
  $view = new view();
  $view->name = 'species_relationships';
  $view->description = '';
  $view->tag = 'default';
  $view->base_table = 'taxonomy_term_data';
  $view->human_name = 'Species relationships';
  $view->core = 7;
  $view->api_version = '3.0';
  $view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */

  /* Display: Master */
    $handler = $view->new_display('default', 'Master', 'default');
  $handler->display->display_options['title'] = 'Ecological relationships';
  $handler->display->display_options['css_class'] = 'grid-8 alpha omega';
  $handler->display->display_options['use_more_always'] = FALSE;
  $handler->display->display_options['access']['type'] = 'perm';
  $handler->display->display_options['cache']['type'] = 'none';
  $handler->display->display_options['query']['type'] = 'views_query';
  $handler->display->display_options['exposed_form']['type'] = 'basic';
  $handler->display->display_options['pager']['type'] = 'none';
  $handler->display->display_options['pager']['options']['offset'] = '0';
  $handler->display->display_options['style_plugin'] = 'default';
  $handler->display->display_options['row_plugin'] = 'fields';
  $handler->display->display_options['row_options']['hide_empty'] = TRUE;
  /* Field: Taxonomy term: Ecological relationship */
  $handler->display->display_options['fields']['field_ecological_relationship']['id'] = 'field_ecological_relationship';
  $handler->display->display_options['fields']['field_ecological_relationship']['table'] = 'field_data_field_ecological_relationship';
  $handler->display->display_options['fields']['field_ecological_relationship']['field'] = 'field_ecological_relationship';
  $handler->display->display_options['fields']['field_ecological_relationship']['label'] = '';
  $handler->display->display_options['fields']['field_ecological_relationship']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['field_ecological_relationship']['hide_empty'] = TRUE;
  $handler->display->display_options['fields']['field_ecological_relationship']['empty_zero'] = TRUE;
  $handler->display->display_options['fields']['field_ecological_relationship']['type'] = 'ecorelation_add_endpoint_and_fields';
  $handler->display->display_options['fields']['field_ecological_relationship']['delta_offset'] = '0';
  $handler->display->display_options['fields']['field_ecological_relationship']['separator'] = '';
  /* Contextual filter: Taxonomy term: Term ID */
  $handler->display->display_options['arguments']['tid']['id'] = 'tid';
  $handler->display->display_options['arguments']['tid']['table'] = 'taxonomy_term_data';
  $handler->display->display_options['arguments']['tid']['field'] = 'tid';
  $handler->display->display_options['arguments']['tid']['default_action'] = 'default';
  $handler->display->display_options['arguments']['tid']['default_argument_type'] = 'taxonomy_tid';
  $handler->display->display_options['arguments']['tid']['summary']['number_of_records'] = '0';
  $handler->display->display_options['arguments']['tid']['summary']['format'] = 'default_summary';
  $handler->display->display_options['arguments']['tid']['summary_options']['items_per_page'] = '25';
  $handler->display->display_options['arguments']['tid']['validate']['type'] = 'taxonomy_term';
  $handler->display->display_options['arguments']['tid']['validate']['fail'] = 'empty';
  /* Display: Block */
  $handler = $view->new_display('block', 'Block', 'block');
  $handler->display->display_options['block_description'] = 'View: Species ecological relations';
  $export['species_relationships'] = $view;
  return $export;
}
