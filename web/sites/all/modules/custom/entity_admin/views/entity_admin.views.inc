<?php

/**
 * Implements hook_views_data_alter().
 */
function entity_admin_views_data_alter(&$data){
  foreach(entity_admin_entity_get_info() as $entity_type => $entity_info){
    $entity_type = $entity_info['base table'];
    $data[$entity_type]['table']['base']['query class'] = 'entity_admin_query';
    $data[$entity_type]['view_link'] = array(
      'field' => array(
        'title' => t('View'),
        'help' => t('Provide a simple link to the %entity_label content.', array(
          '%entity_label' => $entity_type
        )),
        'handler' => 'entity_admin_views_handler_field_link'
      )
    );
    $data[$entity_type]['edit_link'] = array(
      'field' => array(
        'title' => t('Edit link'),
        'help' => t('Provide an edit link to the %entity_label content.', array(
          '%entity_label' => $entity_type
        )),
        'handler' => 'entity_admin_views_handler_field_link_edit'
      )
    );
    $data[$entity_type]['delete_link'] = array(
      'field' => array(
        'title' => t('Delete link'),
        'help' => t('Provide a delete link to the %entity_label content.', array(
          '%entity_label' => $entity_type
        )),
        'handler' => 'entity_admin_views_handler_field_link_delete'
      )
    );
  }
}

/**
 * Implements hook_views_plugins().
 */
function entity_admin_views_plugins(){
  $plugins = array(
    'query' => array(
      'entity_admin_query' => array(
        'title' => t('EntityAdminFieldQuery'),
        'help' => t('Uses EntityAdminFieldQuery (without entity_load) for querying entities and fields.'),
        'handler' => 'entity_admin_views_plugin_query'
      )
    )
  );
  return $plugins;
}
