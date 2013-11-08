<?php

function scratchpads_pages_views_default_views(){
  $views = array();
  // Overide the default teaser views fro some content types - eg specimens display in a slickgrid
  // These should be disabled by default, and will be enabled if the option is set in the menu item
  $view = new view();
  $view->name = 'specimen_observation_page';
  $view->description = '';
  $view->tag = 'scratchpads-page';
  $view->base_table = 'node';
  $view->human_name = 'Scratchpads specimen observation page';
  $view->core = 7;
  $view->api_version = '3.0';
  $view->disabled = TRUE;
  /* Display: Master */
  $handler = $view->new_display('default', 'Master', 'default');
  $handler->display->display_options['title'] = 'Specimens';
  $handler->display->display_options['access']['type'] = 'perm';
  $handler->display->display_options['cache']['type'] = 'none';
  $handler->display->display_options['query']['type'] = 'views_query';
  $handler->display->display_options['query']['options']['query_comment'] = FALSE;
  $handler->display->display_options['exposed_form']['type'] = 'basic';
  $handler->display->display_options['pager']['type'] = 'some';
  $handler->display->display_options['pager']['options']['items_per_page'] = '0';
  $handler->display->display_options['pager']['options']['offset'] = '0';
  $handler->display->display_options['style_plugin'] = 'slickgrid';
  $handler->display->display_options['style_options']['grouping_field'] = '';
  $handler->display->display_options['style_options']['collapse_groups_by_default'] = 0;
  $handler->display->display_options['style_options']['enableColumnResize'] = 1;
  $handler->display->display_options['style_options']['enableColumnReorder'] = 1;
  $handler->display->display_options['style_options']['select_columns'] = 1;
  $handler->display->display_options['style_options']['asyncEditorLoading'] = 1;
  $handler->display->display_options['style_options']['forceFitColumns'] = 1;
  $handler->display->display_options['style_options']['pager'] = 0;
  $handler->display->display_options['style_options']['headerHeight'] = '42';
  $handler->display->display_options['style_options']['viewport_height'] = '500';
  $handler->display->display_options['style_options']['rowHeight'] = '30';
  $handler->display->display_options['style_options']['row_selection_checkbox'] = 0;
  $handler->display->display_options['style_options']['multi_edit'] = 0;
  $handler->display->display_options['style_options']['delete'] = 0;
  $handler->display->display_options['style_options']['clone'] = 0;
  $handler->display->display_options['style_options']['export_selected_rows'] = 0;
  $handler->display->display_options['style_options']['undo'] = 0;
  $handler->display->display_options['style_options']['add'] = '0';
  $handler->display->display_options['style_options']['autoEdit'] = 0;
  $handler->display->display_options['style_options']['columns'] = array();
  $handler->display->display_options['style_options']['tabs'] = '';
  $fields = field_info_instances('node', 'specimen_observation');
  $groups = field_group_info_groups('node', 'specimen_observation', 'form', true);
  usort($groups, 'scratchpads_species_usort_comparison');
  foreach($groups as $group){
    if(count(array_intersect($group->children, array_keys($fields)))){
      $handler->display->display_options['style_options']['tabs'] .= $group->label . PHP_EOL;
      foreach($group->children as $field){
        if(array_key_exists($field, $fields)){
          $handler->display->display_options['fields'][$field]['id'] = $field;
          $handler->display->display_options['fields'][$field]['table'] = 'field_data_' . $field;
          $handler->display->display_options['fields'][$field]['field'] = $field;
          $handler->display->display_options['fields'][$field]['label'] = $fields[$field]['label'];
          $handler->display->display_options['fields'][$field]['alter']['alter_text'] = 0;
          $handler->display->display_options['fields'][$field]['alter']['make_link'] = 0;
          $handler->display->display_options['fields'][$field]['alter']['absolute'] = 0;
          $handler->display->display_options['fields'][$field]['alter']['external'] = 0;
          $handler->display->display_options['fields'][$field]['alter']['replace_spaces'] = 0;
          $handler->display->display_options['fields'][$field]['alter']['trim_whitespace'] = 0;
          $handler->display->display_options['fields'][$field]['alter']['nl2br'] = 0;
          $handler->display->display_options['fields'][$field]['alter']['word_boundary'] = 1;
          $handler->display->display_options['fields'][$field]['alter']['ellipsis'] = 1;
          $handler->display->display_options['fields'][$field]['alter']['strip_tags'] = 0;
          $handler->display->display_options['fields'][$field]['alter']['trim'] = 0;
          $handler->display->display_options['fields'][$field]['alter']['html'] = 0;
          $handler->display->display_options['fields'][$field]['element_type'] = '0';
          $handler->display->display_options['fields'][$field]['element_label_type'] = 'strong';
          $handler->display->display_options['fields'][$field]['element_label_colon'] = 1;
          $handler->display->display_options['fields'][$field]['element_default_classes'] = 0;
          $handler->display->display_options['fields'][$field]['hide_empty'] = 0;
          $handler->display->display_options['fields'][$field]['empty_zero'] = 0;
          $handler->display->display_options['fields'][$field]['hide_alter_empty'] = 1;
          $handler->display->display_options['fields'][$field]['field_api_classes'] = 0;
          $handler->display->display_options['style_options']['columns'][$field] = array(
            'width' => 120,
            'sortable' => 1,
            'tab' => $group->label,
            'filter' => 'textFilter'
          );
          if($field == 'field_genbank_number'){
            $handler->display->display_options['fields'][$field]['type'] = 'darwincore_link_to_ncbi';
          }
        }
      }
    }
  }
  /* Field: Node: Link */
  $handler->display->display_options['fields']['view_node']['id'] = 'view_node';
  $handler->display->display_options['fields']['view_node']['table'] = 'views_entity_node';
  $handler->display->display_options['fields']['view_node']['field'] = 'view_node';
  $handler->display->display_options['fields']['view_node']['label'] = 'Specimen';
  $handler->display->display_options['fields']['view_node']['alter']['alter_text'] = 0;
  $handler->display->display_options['fields']['view_node']['alter']['make_link'] = 0;
  $handler->display->display_options['fields']['view_node']['alter']['absolute'] = 0;
  $handler->display->display_options['fields']['view_node']['alter']['replace_spaces'] = 0;
  $handler->display->display_options['fields']['view_node']['alter']['trim_whitespace'] = 0;
  $handler->display->display_options['fields']['view_node']['alter']['nl2br'] = 0;
  $handler->display->display_options['fields']['view_node']['alter']['word_boundary'] = 1;
  $handler->display->display_options['fields']['view_node']['alter']['ellipsis'] = 1;
  $handler->display->display_options['fields']['view_node']['alter']['strip_tags'] = 0;
  $handler->display->display_options['fields']['view_node']['alter']['trim'] = 0;
  $handler->display->display_options['fields']['view_node']['alter']['html'] = 0;
  $handler->display->display_options['fields']['view_node']['element_label_colon'] = 1;
  $handler->display->display_options['fields']['view_node']['element_default_classes'] = 1;
  $handler->display->display_options['fields']['view_node']['hide_empty'] = 0;
  $handler->display->display_options['fields']['view_node']['empty_zero'] = 0;
  $handler->display->display_options['fields']['view_node']['hide_alter_empty'] = 1;
  $handler->display->display_options['fields']['view_node']['text'] = 'View specimen';
  /* Sort criterion: Content: Title */
  $handler->display->display_options['sorts']['title']['id'] = 'title';
  $handler->display->display_options['sorts']['title']['table'] = 'node';
  $handler->display->display_options['sorts']['title']['field'] = 'title';
  /* Filter criterion: Content: Published */
  $handler->display->display_options['filters']['status']['id'] = 'status';
  $handler->display->display_options['filters']['status']['table'] = 'node';
  $handler->display->display_options['filters']['status']['field'] = 'status';
  $handler->display->display_options['filters']['status']['value'] = 1;
  $handler->display->display_options['filters']['status']['group'] = 0;
  $handler->display->display_options['filters']['status']['expose']['operator'] = FALSE;
  /* Filter criterion: Content: Type */
  $handler->display->display_options['filters']['type']['id'] = 'type';
  $handler->display->display_options['filters']['type']['table'] = 'node';
  $handler->display->display_options['filters']['type']['field'] = 'type';
  $handler->display->display_options['filters']['type']['value'] = array(
    'specimen_observation' => 'specimen_observation'
  );
  /* Display: XML Export */
  $handler = $view->new_display('views_data_export', 'XML Export', 'views_data_export_1');
  $handler->display->display_options['pager']['type'] = 'none';
  $handler->display->display_options['pager']['options']['offset'] = '0';
  $handler->display->display_options['style_plugin'] = 'views_data_export_xml';
  $handler->display->display_options['style_options']['provide_file'] = 0;
  $handler->display->display_options['style_options']['parent_sort'] = 0;
  $handler->display->display_options['style_options']['transform'] = 1;
  $handler->display->display_options['path'] = 'specimen_observation/xml';
  $handler->display->display_options['displays'] = array(
    'default' => 'default',
    'page_1' => 'page_1'
  );
  /* Display: CSV Export */
  $handler = $view->new_display('views_data_export', 'CSV Export', 'views_data_export_2');
  $handler->display->display_options['pager']['type'] = 'none';
  $handler->display->display_options['pager']['options']['offset'] = '0';
  $handler->display->display_options['style_plugin'] = 'views_data_export_csv';
  $handler->display->display_options['path'] = 'specimen_observation/csv';
  $handler->display->display_options['displays'] = array(
    'default' => 'default',
    'page_1' => 'page_1'
  );
  /* Display: Page */
  $handler = $view->new_display('page', 'Page', 'page_1');
  $handler->display->display_options['path'] = 'specimen_observation';
  $handler->display->display_options['menu']['type'] = 'normal';
  $handler->display->display_options['menu']['title'] = t("Specimens");
  $handler->display->display_options['menu']['name'] = PRIMARY_MENU_NAME;
  $handler->display->display_options['menu']['weight'] = 1;
  $views[$view->name] = $view;
  // Automatically generated teaser views
  $display_settings = scratchpads_pages_variable_get('scratchpads_pages_display');
  foreach(scratchpads_pages_get_node_types() as $type => $node_info){
    // Do we have any nodes of this content type?
    if(scratchpads_pages_node_exists_of_type($type)){
      if(module_exists($node_info->module) && (!isset($display_settings[$type]) || $display_settings[$type] == 'view')){
        $name = $type . '_page';
        // Have we already defined this view?
        if(isset($views[$name])){
          $views[$name]->disabled = FALSE;
          // If so, enable it	
        }else{ // If not, create it
          $view = new view();
          $view->name = $name;
          $view->description = '';
          $view->tag = 'scratchpads-page';
          $view->base_table = 'node';
          $view->human_name = 'Scratchpads ' . $node_info->name . ' page';
          $view->core = 7;
          $view->api_version = '3.0';
          $view->disabled = FALSE;
          /* Display: Master */
          $handler = $view->new_display('default', 'Master', 'default');
          $handler->display->display_options['title'] = scratchpads_pages_pluralise_node_name($node_info->name);
          $handler->display->display_options['access']['type'] = 'perm';
          $handler->display->display_options['cache']['type'] = 'none';
          $handler->display->display_options['query']['type'] = 'views_query';
          $handler->display->display_options['query']['options']['query_comment'] = FALSE;
          $handler->display->display_options['exposed_form']['type'] = 'basic';
          $handler->display->display_options['pager']['type'] = 'full';
          $handler->display->display_options['pager']['options']['items_per_page'] = '10';
          $handler->display->display_options['style_plugin'] = 'list';
          $handler->display->display_options['row_plugin'] = 'node';
          $handler->display->display_options['row_options']['links'] = FALSE;
          /* No results behavior: Global: Text area */
          $handler->display->display_options['empty']['area']['id'] = 'area';
          $handler->display->display_options['empty']['area']['table'] = 'views';
          $handler->display->display_options['empty']['area']['field'] = 'area';
          $handler->display->display_options['empty']['area']['label'] = 'Empty text';
          $handler->display->display_options['empty']['area']['empty'] = FALSE;
          $handler->display->display_options['empty']['area']['content'] = 'Sorry there is not yet any content of this type.';
          $handler->display->display_options['empty']['area']['format'] = 'filtered_html';
          $handler->display->display_options['empty']['area']['tokenize'] = 0;
          /* Field: Content: Title */
          $handler->display->display_options['fields']['title']['id'] = 'title';
          $handler->display->display_options['fields']['title']['table'] = 'node';
          $handler->display->display_options['fields']['title']['field'] = 'title';
          $handler->display->display_options['fields']['title']['label'] = '';
          $handler->display->display_options['fields']['title']['alter']['alter_text'] = 0;
          $handler->display->display_options['fields']['title']['alter']['make_link'] = 0;
          $handler->display->display_options['fields']['title']['alter']['absolute'] = 0;
          $handler->display->display_options['fields']['title']['alter']['word_boundary'] = 0;
          $handler->display->display_options['fields']['title']['alter']['ellipsis'] = 0;
          $handler->display->display_options['fields']['title']['alter']['strip_tags'] = 0;
          $handler->display->display_options['fields']['title']['alter']['trim'] = 0;
          $handler->display->display_options['fields']['title']['alter']['html'] = 0;
          $handler->display->display_options['fields']['title']['hide_empty'] = 0;
          $handler->display->display_options['fields']['title']['empty_zero'] = 0;
          $handler->display->display_options['fields']['title']['link_to_node'] = 1;
          /* Sort criterion: Content: Post date */
          $handler->display->display_options['sorts']['created']['id'] = 'created';
          $handler->display->display_options['sorts']['created']['table'] = 'node';
          $handler->display->display_options['sorts']['created']['field'] = 'created';
          $handler->display->display_options['sorts']['created']['order'] = 'DESC';
          /* Filter criterion: Content: Published */
          $handler->display->display_options['filters']['status']['id'] = 'status';
          $handler->display->display_options['filters']['status']['table'] = 'node';
          $handler->display->display_options['filters']['status']['field'] = 'status';
          $handler->display->display_options['filters']['status']['value'] = 1;
          $handler->display->display_options['filters']['status']['group'] = 0;
          $handler->display->display_options['filters']['status']['expose']['operator'] = FALSE;
          /* Filter criterion: Content: Type */
          $handler->display->display_options['filters']['type']['id'] = 'type';
          $handler->display->display_options['filters']['type']['table'] = 'node';
          $handler->display->display_options['filters']['type']['field'] = 'type';
          $handler->display->display_options['filters']['type']['value'] = array(
            $type => $type
          );
          /* Display: Page */
          $handler = $view->new_display('page', 'Page', 'page');
          $handler->display->display_options['path'] = $type;
          $handler->display->display_options['menu']['type'] = 'normal';
          $handler->display->display_options['menu']['title'] = scratchpads_pages_pluralise_node_name($node_info->name);
          $handler->display->display_options['menu']['name'] = PRIMARY_MENU_NAME;
          $handler->display->display_options['menu']['weight'] = 1;
          $views[$view->name] = $view;
        }
      }
    }
  }
  return $views;
}