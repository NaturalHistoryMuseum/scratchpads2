<?php

function entity_admin_views_default_views(){
  $views = array();
  foreach(entity_admin_entity_get_info() as $entity_type => $entity_info){
    if(isset($entity_info['type of'])){
      $view = new view();
      $view->name = 'entity_admin_' . $entity_type;
      $view->description = '';
      $view->tag = 'entity admin';
      $view->base_table = $entity_info['base table'];
      $view->human_name = $entity_info['label'];
      $view->core = 7;
      $view->api_version = '3.0';
      $view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */
      /* Display: Master */
      $handler = $view->new_display('default', 'Master', 'default');
      $handler->display->display_options['title'] = $entity_info['label'];
      $handler->display->display_options['use_ajax'] = TRUE;
      $handler->display->display_options['access']['type'] = 'none';
      $handler->display->display_options['cache']['type'] = 'none';
      $handler->display->display_options['query']['type'] = 'views_query';
      $handler->display->display_options['query']['options']['query_comment'] = FALSE;
      $handler->display->display_options['exposed_form']['type'] = 'basic';
      $handler->display->display_options['pager']['type'] = 'full';
      $handler->display->display_options['pager']['options']['items_per_page'] = '50';
      $handler->display->display_options['style_plugin'] = 'table';
      $handler->display->display_options['style_options']['columns'] = array(
        'views_bulk_operations' => 'views_bulk_operations'
      );
      $handler->display->display_options['style_options']['default'] = '-1';
      $handler->display->display_options['style_options']['info'] = array(
        'views_bulk_operations' => array(
          'align' => '',
          'separator' => '',
          'empty_column' => 0
        )
      );
      $handler->display->display_options['style_options']['override'] = 1;
      $handler->display->display_options['style_options']['sticky'] = 0;
      $handler->display->display_options['style_options']['empty_table'] = 0;
      /* Field: Bulk operations: Character project */
      $handler->display->display_options['fields']['views_bulk_operations']['id'] = 'views_bulk_operations';
      $handler->display->display_options['fields']['views_bulk_operations']['table'] = $entity_info['base table'];
      $handler->display->display_options['fields']['views_bulk_operations']['field'] = 'views_bulk_operations';
      $handler->display->display_options['fields']['views_bulk_operations']['alter']['alter_text'] = 0;
      $handler->display->display_options['fields']['views_bulk_operations']['alter']['make_link'] = 0;
      $handler->display->display_options['fields']['views_bulk_operations']['alter']['absolute'] = 0;
      $handler->display->display_options['fields']['views_bulk_operations']['alter']['external'] = 0;
      $handler->display->display_options['fields']['views_bulk_operations']['alter']['replace_spaces'] = 0;
      $handler->display->display_options['fields']['views_bulk_operations']['alter']['trim_whitespace'] = 0;
      $handler->display->display_options['fields']['views_bulk_operations']['alter']['nl2br'] = 0;
      $handler->display->display_options['fields']['views_bulk_operations']['alter']['word_boundary'] = 1;
      $handler->display->display_options['fields']['views_bulk_operations']['alter']['ellipsis'] = 1;
      $handler->display->display_options['fields']['views_bulk_operations']['alter']['more_link'] = 0;
      $handler->display->display_options['fields']['views_bulk_operations']['alter']['strip_tags'] = 0;
      $handler->display->display_options['fields']['views_bulk_operations']['alter']['trim'] = 0;
      $handler->display->display_options['fields']['views_bulk_operations']['alter']['html'] = 0;
      $handler->display->display_options['fields']['views_bulk_operations']['element_label_colon'] = 1;
      $handler->display->display_options['fields']['views_bulk_operations']['element_default_classes'] = 1;
      $handler->display->display_options['fields']['views_bulk_operations']['hide_empty'] = 0;
      $handler->display->display_options['fields']['views_bulk_operations']['empty_zero'] = 0;
      $handler->display->display_options['fields']['views_bulk_operations']['hide_alter_empty'] = 1;
      $handler->display->display_options['fields']['views_bulk_operations']['vbo']['entity_load_capacity'] = '10';
      $handler->display->display_options['fields']['views_bulk_operations']['vbo']['operations'] = array(
        'action::views_bulk_operations_delete_item' => array(
          'selected' => 1,
          'use_queue' => 0,
          'skip_confirmation' => 0,
          'override_label' => 0,
          'label' => ''
        ),
        'action::system_message_action' => array(
          'selected' => 0,
          'use_queue' => 0,
          'skip_confirmation' => 0,
          'override_label' => 0,
          'label' => ''
        ),
        'action::views_bulk_operations_script_action' => array(
          'selected' => 0,
          'use_queue' => 0,
          'skip_confirmation' => 0,
          'override_label' => 0,
          'label' => ''
        ),
        'action::views_bulk_operations_modify_action' => array(
          'selected' => 1,
          'use_queue' => 0,
          'skip_confirmation' => 0,
          'override_label' => 0,
          'label' => '',
          'settings' => array(
            'show_all_tokens' => 1,
            'display_values' => array(
              '_all_' => '_all_'
            )
          )
        ),
        'action::views_bulk_operations_argument_selector_action' => array(
          'selected' => 0,
          'skip_confirmation' => 0,
          'override_label' => 0,
          'label' => '',
          'settings' => array(
            'url' => ''
          )
        ),
        'action::system_goto_action' => array(
          'selected' => 0,
          'use_queue' => 0,
          'skip_confirmation' => 0,
          'override_label' => 0,
          'label' => ''
        ),
        'action::system_send_email_action' => array(
          'selected' => 0,
          'use_queue' => 0,
          'skip_confirmation' => 0,
          'override_label' => 0,
          'label' => ''
        )
      );
      $handler->display->display_options['fields']['views_bulk_operations']['vbo']['enable_select_all_pages'] = 1;
      $handler->display->display_options['fields']['views_bulk_operations']['vbo']['display_type'] = '0';
      $handler->display->display_options['fields']['views_bulk_operations']['vbo']['display_result'] = 1;
      $handler->display->display_options['fields']['views_bulk_operations']['vbo']['merge_single_action'] = 0;
      $handler->display->display_options['fields']['views_bulk_operations']['vbo']['force_single'] = 0;
      // An array of fields to add
      $fields = array();
      // Does the entity have a label?
      if($label_key = entity_admin_entity_label_key($entity_info)){
        /* Field: Entity: Link */
        $handler->display->display_options['fields']['view_link']['id'] = 'view_link';
        $handler->display->display_options['fields']['view_link']['table'] = $entity_info['base table'];
        $handler->display->display_options['fields']['view_link']['field'] = 'view_link';
        $handler->display->display_options['fields']['view_link']['label'] = $label_key;
        $handler->display->display_options['fields']['view_link']['exclude'] = TRUE;
        $handler->display->display_options['fields']['view_link']['element_label_colon'] = 1;
        $handler->display->display_options['fields']['view_link']['element_default_classes'] = 1;
        $handler->display->display_options['fields']['view_link']['hide_empty'] = 0;
        $handler->display->display_options['fields']['view_link']['empty_zero'] = 0;
        $handler->display->display_options['fields']['view_link']['hide_alter_empty'] = 1;
        /* Field: Entity: Label */
        $handler->display->display_options['fields'][$label_key]['id'] = $label_key;
        $handler->display->display_options['fields'][$label_key]['table'] = $entity_info['base table'];
        $handler->display->display_options['fields'][$label_key]['field'] = $label_key;
        $handler->display->display_options['fields'][$label_key]['label'] = $label_key;
        $handler->display->display_options['fields'][$label_key]['alter']['alter_text'] = 0;
        $handler->display->display_options['fields'][$label_key]['alter']['make_link'] = 1;
        $handler->display->display_options['fields'][$label_key]['alter']['path'] = '[view_link]';
        $handler->display->display_options['fields'][$label_key]['alter']['absolute'] = 0;
        $handler->display->display_options['fields'][$label_key]['alter']['external'] = 0;
        $handler->display->display_options['fields'][$label_key]['alter']['replace_spaces'] = 0;
        $handler->display->display_options['fields'][$label_key]['alter']['trim_whitespace'] = 0;
        $handler->display->display_options['fields'][$label_key]['alter']['nl2br'] = 0;
        $handler->display->display_options['fields'][$label_key]['alter']['word_boundary'] = 1;
        $handler->display->display_options['fields'][$label_key]['alter']['ellipsis'] = 1;
        $handler->display->display_options['fields'][$label_key]['alter']['more_link'] = 0;
        $handler->display->display_options['fields'][$label_key]['alter']['strip_tags'] = 0;
        $handler->display->display_options['fields'][$label_key]['alter']['trim'] = 0;
        $handler->display->display_options['fields'][$label_key]['alter']['html'] = 0;
        $handler->display->display_options['fields'][$label_key]['element_label_colon'] = 1;
        $handler->display->display_options['fields'][$label_key]['element_default_classes'] = 1;
        $handler->display->display_options['fields'][$label_key]['hide_empty'] = 0;
        $handler->display->display_options['fields'][$label_key]['empty_zero'] = 0;
        $handler->display->display_options['fields'][$label_key]['hide_alter_empty'] = 1;
        $handler->display->display_options['style_options']['columns'][$label_key] = $label_key;
        $handler->display->display_options['style_options']['info'][$label_key] = array(
          'sortable' => 1,
          'default_sort_order' => 'asc',
          'align' => '',
          'separator' => '',
          'empty_column' => 0
        );
        // Add an exposed filter for the label
        /* Filter criterion: Artist: Label */
        $handler->display->display_options['filters'][$label_key]['id'] = $label_key;
        $handler->display->display_options['filters'][$label_key]['table'] = $entity_info['base table'];
        $handler->display->display_options['filters'][$label_key]['field'] = $label_key;
        $handler->display->display_options['filters'][$label_key]['operator'] = 'contains';
        $handler->display->display_options['filters'][$label_key]['exposed'] = TRUE;
        $handler->display->display_options['filters'][$label_key]['expose']['operator_id'] = $label_key . '_op';
        $handler->display->display_options['filters'][$label_key]['expose']['label'] = $label_key;
        $handler->display->display_options['filters'][$label_key]['expose']['operator'] = $label_key . '_op';
        $handler->display->display_options['filters'][$label_key]['expose']['identifier'] = $label_key;
        $handler->display->display_options['filters'][$label_key]['expose']['required'] = 0;
        $handler->display->display_options['filters'][$label_key]['expose']['multiple'] = FALSE;
      }
      // Add type column
      if(isset($entity_info['entity keys']['bundle'])){
        $fields[$entity_info['entity keys']['bundle']] = array(
          'label' => $entity_info['entity keys']['bundle']
        );
      }
      // Add any fields defined in property info
      $property_info = entity_get_property_info();
      if(isset($property_info[$entity_type]['properties'])){
        $properties = array_diff_key($property_info[$entity_type]['properties'], array_fill_keys(array_filter($entity_info['entity keys']), true));
        if(count($properties)){
          foreach($properties as $property_name => $property){
            if($property['type'] == 'user'){
              $handler->display->display_options['relationships']['uid']['id'] = 'uid';
              $handler->display->display_options['relationships']['uid']['table'] = $entity_info['base table'];
              $handler->display->display_options['relationships']['uid']['field'] = 'uid';
              $handler->display->display_options['relationships']['uid']['required'] = 0;
              $fields['name'] = array(
                'label' => $property['label'],
                'relationship' => 'uid',
                'base table' => 'users'
              );
            }elseif($property['type'] == 'date'){
              $fields[$property_name] = array(
                'label' => $property['label']
              );
              // Sort by created date if available
              if($property_name == 'created'){
                /* Sort criterion: Character project: Date created */
                $handler->display->display_options['sorts'][$property_name]['id'] = $property_name;
                $handler->display->display_options['sorts'][$property_name]['table'] = $entity_info['base table'];
                $handler->display->display_options['sorts'][$property_name]['field'] = $property_name;
                $handler->display->display_options['sorts'][$property_name]['order'] = 'DESC';
              }
            }
          }
        }
      }
      if(count($fields)){
        // Add entity key fields
        foreach($fields as $field_id => $field_info){
          $handler->display->display_options['fields'][$field_id]['id'] = $field_id;
          $handler->display->display_options['fields'][$field_id]['table'] = isset($field_info['base table']) ? $field_info['base table'] : $entity_info['base table'];
          if(isset($field_info['relationship'])){
            $handler->display->display_options['fields'][$field_id]['relationship'] = $field_info['relationship'];
          }
          $handler->display->display_options['fields'][$field_id]['field'] = $field_id;
          $handler->display->display_options['fields'][$field_id]['label'] = $field_info['label'];
          $handler->display->display_options['fields'][$field_id]['alter']['alter_text'] = 0;
          $handler->display->display_options['fields'][$field_id]['alter']['make_link'] = 0;
          $handler->display->display_options['fields'][$field_id]['alter']['absolute'] = 0;
          $handler->display->display_options['fields'][$field_id]['alter']['external'] = 0;
          $handler->display->display_options['fields'][$field_id]['alter']['replace_spaces'] = 0;
          $handler->display->display_options['fields'][$field_id]['alter']['trim_whitespace'] = 0;
          $handler->display->display_options['fields'][$field_id]['alter']['nl2br'] = 0;
          $handler->display->display_options['fields'][$field_id]['alter']['word_boundary'] = 1;
          $handler->display->display_options['fields'][$field_id]['alter']['ellipsis'] = 1;
          $handler->display->display_options['fields'][$field_id]['alter']['more_link'] = 0;
          $handler->display->display_options['fields'][$field_id]['alter']['strip_tags'] = 0;
          $handler->display->display_options['fields'][$field_id]['alter']['trim'] = 0;
          $handler->display->display_options['fields'][$field_id]['alter']['html'] = 0;
          $handler->display->display_options['fields'][$field_id]['element_label_colon'] = 1;
          $handler->display->display_options['fields'][$field_id]['element_default_classes'] = 1;
          $handler->display->display_options['fields'][$field_id]['hide_empty'] = 0;
          $handler->display->display_options['fields'][$field_id]['empty_zero'] = 0;
          $handler->display->display_options['fields'][$field_id]['hide_alter_empty'] = 1;
          $handler->display->display_options['style_options']['columns'][$field_id] = $field_id;
          $handler->display->display_options['style_options']['info'][$field_id] = array(
            'sortable' => 1,
            'default_sort_order' => 'asc',
            'align' => '',
            'separator' => '',
            'empty_column' => 0
          );
        }
      }
      // Operation links
      /* Field: Entity: Edit link */
      $handler->display->display_options['fields']['edit_link']['id'] = 'edit_link';
      $handler->display->display_options['fields']['edit_link']['table'] = $entity_info['base table'];
      $handler->display->display_options['fields']['edit_link']['field'] = 'edit_link';
      $handler->display->display_options['fields']['edit_link']['label'] = 'Operations';
      $handler->display->display_options['fields']['edit_link']['alter']['alter_text'] = 0;
      $handler->display->display_options['fields']['edit_link']['alter']['make_link'] = 0;
      $handler->display->display_options['fields']['edit_link']['alter']['absolute'] = 0;
      $handler->display->display_options['fields']['edit_link']['alter']['replace_spaces'] = 0;
      $handler->display->display_options['fields']['edit_link']['alter']['trim_whitespace'] = 0;
      $handler->display->display_options['fields']['edit_link']['alter']['nl2br'] = 0;
      $handler->display->display_options['fields']['edit_link']['alter']['word_boundary'] = 1;
      $handler->display->display_options['fields']['edit_link']['alter']['ellipsis'] = 1;
      $handler->display->display_options['fields']['edit_link']['alter']['more_link'] = 0;
      $handler->display->display_options['fields']['edit_link']['alter']['strip_tags'] = 0;
      $handler->display->display_options['fields']['edit_link']['alter']['trim'] = 0;
      $handler->display->display_options['fields']['edit_link']['alter']['html'] = 0;
      $handler->display->display_options['fields']['edit_link']['element_label_colon'] = 1;
      $handler->display->display_options['fields']['edit_link']['element_default_classes'] = 1;
      $handler->display->display_options['fields']['edit_link']['hide_empty'] = 0;
      $handler->display->display_options['fields']['edit_link']['empty_zero'] = 0;
      $handler->display->display_options['fields']['edit_link']['hide_alter_empty'] = 1;
      $handler->display->display_options['style_options']['columns']['edit_link'] = 'edit_link';
      /* Field: Entity: Delete link */
      $handler->display->display_options['fields']['delete_link']['id'] = 'delete_link';
      $handler->display->display_options['fields']['delete_link']['table'] = $entity_info['base table'];
      $handler->display->display_options['fields']['delete_link']['field'] = 'delete_link';
      $handler->display->display_options['fields']['delete_link']['label'] = '';
      $handler->display->display_options['fields']['delete_link']['alter']['alter_text'] = 0;
      $handler->display->display_options['fields']['delete_link']['alter']['make_link'] = 0;
      $handler->display->display_options['fields']['delete_link']['alter']['absolute'] = 0;
      $handler->display->display_options['fields']['delete_link']['alter']['replace_spaces'] = 0;
      $handler->display->display_options['fields']['delete_link']['alter']['trim_whitespace'] = 0;
      $handler->display->display_options['fields']['delete_link']['alter']['nl2br'] = 0;
      $handler->display->display_options['fields']['delete_link']['alter']['word_boundary'] = 1;
      $handler->display->display_options['fields']['delete_link']['alter']['ellipsis'] = 1;
      $handler->display->display_options['fields']['delete_link']['alter']['more_link'] = 0;
      $handler->display->display_options['fields']['delete_link']['alter']['strip_tags'] = 0;
      $handler->display->display_options['fields']['delete_link']['alter']['trim'] = 0;
      $handler->display->display_options['fields']['delete_link']['alter']['html'] = 0;
      $handler->display->display_options['fields']['delete_link']['element_label_colon'] = FALSE;
      $handler->display->display_options['fields']['delete_link']['element_default_classes'] = 1;
      $handler->display->display_options['fields']['delete_link']['hide_empty'] = 0;
      $handler->display->display_options['fields']['delete_link']['empty_zero'] = 0;
      $handler->display->display_options['fields']['delete_link']['hide_alter_empty'] = 1;
      $handler->display->display_options['style_options']['columns']['delete_link'] = 'edit_link';
      /* Display: Page */
      $handler = $view->new_display('page', 'Page', 'page');
      $handler->display->display_options['path'] = $entity_info['admin ui']['path'];
      $handler->display->display_options['menu']['type'] = 'tab';
      $handler->display->display_options['menu']['title'] = isset($entity_info['plural label']) ? $entity_info['plural label'] : $entity_info['label'] . 's';
      $handler->display->display_options['menu']['description'] = t('!label content', array(
        '!label' => $entity_info['label']
      ));
      $handler->display->display_options['menu']['weight'] = '0';
      $handler->display->display_options['menu']['context'] = 0;
    }
    $views[$view->name] = $view;
  }
  return $views;
}