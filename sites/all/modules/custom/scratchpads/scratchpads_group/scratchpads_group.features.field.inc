<?php

/**
 * @file
 * scratchpads_group.features.field.inc
 */
/**
 * Implements hook_field_default_fields().
 */
function scratchpads_group_field_default_fields($additional_content_type = FALSE){
  $fields = array();
  // Exported field: 'node-group-body'
  $fields['node-group-body'] = array(
    'field_config' => array(
      'locked' => '0',
      'active' => '1',
      'cardinality' => '1',
      'deleted' => '0',
      'entity_types' => array(
        0 => 'node'
      ),
      'field_name' => 'body',
      'foreign keys' => array(
        'format' => array(
          'columns' => array(
            'format' => 'format'
          ),
          'table' => 'filter_format'
        )
      ),
      'indexes' => array(
        'format' => array(
          0 => 'format'
        )
      ),
      'module' => 'text',
      'settings' => array(),
      'translatable' => '0',
      'type' => 'text_with_summary'
    ),
    'field_instance' => array(
      'bundle' => 'group',
      'default_value' => NULL,
      'deleted' => '0',
      'description' => '',
      'display' => array(
        'default' => array(
          'label' => 'hidden',
          'module' => 'text',
          'settings' => array(),
          'type' => 'text_default',
          'weight' => 1
        ),
        'entityconnectpreview' => array(
          'label' => 'above',
          'settings' => array(),
          'type' => 'hidden',
          'weight' => 0
        ),
        'linked_node' => array(
          'label' => 'above',
          'settings' => array(),
          'type' => 'hidden',
          'weight' => 0
        ),
        'teaser' => array(
          'label' => 'hidden',
          'module' => 'text',
          'settings' => array(
            'trim_length' => 600
          ),
          'type' => 'text_summary_or_trimmed',
          'weight' => 1
        )
      ),
      'entity_type' => 'node',
      'field_name' => 'body',
      'label' => 'Body',
      'required' => FALSE,
      'settings' => array(
        'display_summary' => TRUE,
        'text_processing' => 1,
        'user_register_form' => FALSE
      ),
      'widget' => array(
        'module' => 'text',
        'settings' => array(
          'rows' => 20,
          'summary_rows' => 5
        ),
        'type' => 'text_textarea_with_summary',
        'weight' => 1
      )
    )
  );
  // Exported field: 'node-group-group_access'
  $fields['node-group-group_access'] = array(
    'field_config' => array(
      'active' => '1',
      'cardinality' => '1',
      'deleted' => '0',
      'entity_types' => array(),
      'field_name' => 'group_access',
      'foreign keys' => array(),
      'indexes' => array(
        'value' => array(
          0 => 'value'
        )
      ),
      'module' => 'list',
      'no_ui' => TRUE,
      'settings' => array(
        'allowed_values' => array(
          0 => 'Public - accessible to all site users',
          1 => 'Private - accessible only to group members'
        ),
        'allowed_values_function' => ''
      ),
      'translatable' => '0',
      'type' => 'list_boolean'
    ),
    'field_instance' => array(
      'bundle' => 'group',
      'default_value' => array(
        0 => array(
          'value' => 0
        )
      ),
      'deleted' => '0',
      'description' => '',
      'display' => array(
        'default' => array(
          'label' => 'above',
          'module' => 'list',
          'settings' => array(),
          'type' => 'list_default',
          'weight' => 3
        ),
        'entityconnectpreview' => array(
          'label' => 'above',
          'settings' => array(),
          'type' => 'hidden',
          'weight' => 0
        ),
        'linked_node' => array(
          'label' => 'above',
          'settings' => array(),
          'type' => 'hidden',
          'weight' => 0
        ),
        'teaser' => array(
          'label' => 'above',
          'settings' => array(),
          'type' => 'hidden',
          'weight' => 0
        )
      ),
      'entity_type' => 'node',
      'field_name' => 'group_access',
      'label' => 'Group visibility',
      'required' => TRUE,
      'settings' => array(
        'user_register_form' => FALSE
      ),
      'view modes' => array(
        'full' => array(
          'label' => 'above',
          'type' => 'options_onoff'
        ),
        'teaser' => array(
          'label' => 'above',
          'type' => 'options_onoff'
        )
      ),
      'widget' => array(
        'module' => 'options',
        'settings' => array(),
        'type' => 'options_buttons',
        'weight' => 3
      ),
      'widget_type' => 'options_select'
    )
  );
  // Exported field: 'node-group-group_group'
  $fields['node-group-group_group'] = array(
    'field_config' => array(
      'locked' => '1',
      'active' => '1',
      'cardinality' => '1',
      'deleted' => '0',
      'entity_types' => array(),
      'field_name' => 'group_group',
      'foreign keys' => array(),
      'indexes' => array(
        'value' => array(
          0 => 'value'
        )
      ),
      'module' => 'list',
      'no_ui' => TRUE,
      'settings' => array(
        'allowed_values' => array(
          0 => 'Not a group',
          1 => 'Group'
        ),
        'allowed_values_function' => ''
      ),
      'translatable' => '0',
      'type' => 'list_boolean'
    ),
    'field_instance' => array(
      'bundle' => 'group',
      'default_value' => array(
        0 => array(
          'value' => 1
        )
      ),
      'deleted' => '0',
      'description' => 'Leave this box checked to ensure that this node is a \'Group\'.',
      'display' => array(
        'default' => array(
          'label' => 'above',
          'module' => 'og_ui',
          'settings' => array(),
          'type' => 'og_group_subscribe',
          'weight' => 0
        ),
        'entityconnectpreview' => array(
          'label' => 'above',
          'settings' => array(),
          'type' => 'hidden',
          'weight' => 0
        ),
        'linked_node' => array(
          'label' => 'above',
          'settings' => array(),
          'type' => 'hidden',
          'weight' => 0
        ),
        'teaser' => array(
          'label' => 'above',
          'settings' => array(),
          'type' => 'hidden',
          'weight' => 0
        )
      ),
      'display_label' => 1,
      'entity_type' => 'node',
      'field_name' => 'group_group',
      'label' => 'Group',
      'required' => 0,
      'settings' => array(
        'user_register_form' => FALSE
      ),
      'view modes' => array(
        'full' => array(
          'custom settings' => FALSE,
          'label' => 'Full',
          'type' => 'og_group_subscribe'
        ),
        'teaser' => array(
          'custom settings' => FALSE,
          'label' => 'Teaser',
          'type' => 'og_group_subscribe'
        )
      ),
      'widget' => array(
        'active' => 1,
        'module' => 'options',
        'settings' => array(
          'display_label' => 0
        ),
        'type' => 'options_onoff',
        'weight' => 0
      )
    )
  );
  // Find all the content types we want to add OG capability to
  $node_types = node_type_get_types();
  $file_type = entity_get_info('file');
  $types = array(
    'node' => array_keys($node_types),
    'file' => array_keys($file_type['bundles'])
  );
  if($additional_content_type){
    $types[$additional_content_type['entity_type']][] = $additional_content_type['bundle'];
  }
  foreach($types as $entity_type => $bundles){
    foreach($bundles as $bundle){
      if($entity_type == 'node' && $bundle == 'group'){
        continue;
      }
      // Exported field: 'node-xxx-group_audience'
      $fields[$entity_type . '-' . $bundle . '-group_audience'] = array(
        'field_config' => array(
          'locked' => '1',
          'active' => '1',
          'cardinality' => '-1',
          'deleted' => '0',
          'entity_types' => array(),
          'field_name' => 'group_audience',
          'foreign keys' => array(
            'og' => array(
              'columns' => array(
                'gid' => 'gid'
              ),
              'table' => 'og'
            )
          ),
          'indexes' => array(
            'gid' => array(
              0 => 'gid'
            )
          ),
          'module' => 'og',
          'no_ui' => TRUE,
          'settings' => array(),
          'translatable' => '0',
          'type' => 'group'
        ),
        'field_instance' => array(
          'bundle' => $bundle,
          'default_value' => NULL,
          'default_value_function' => 'og_field_audience_default_value',
          'deleted' => '0',
          'description' => '',
          'display' => array(
            'default' => array(
              'label' => 'above',
              'module' => 'og',
              'settings' => array(),
              'type' => 'og_list_default',
              'weight' => '1'
            ),
            'teaser' => array(
              'label' => 'hidden',
              'settings' => array(),
              'type' => 'hidden',
              'weight' => '0'
            )
          ),
          'entity_type' => $entity_type,
          'field_name' => 'group_audience',
          'label' => 'Groups audience',
          'required' => FALSE,
          'settings' => array(
            'user_register_form' => FALSE
          ),
          'view modes' => array(
            'full' => array(
              'custom settings' => FALSE,
              'label' => 'Full',
              'type' => 'og_list_default'
            ),
            'teaser' => array(
              'custom settings' => FALSE,
              'label' => 'Teaser',
              'type' => 'og_list_default'
            )
          ),
          'widget' => array(
            'module' => 'og',
            'settings' => array(
              'opt_group' => 'auto'
            ),
            'type' => 'group_audience',
            'weight' => -3
          ),
          'widget_type' => 'group_audience'
        )
      );
      // Exported field: 'node-xxx-group_content_access'
      $fields[$entity_type . '-' . $bundle . '-group_content_access'] = array(
        'field_config' => array(
          'locked' => '1',
          'active' => '1',
          'cardinality' => '1',
          'deleted' => '0',
          'entity_types' => array(),
          'field_name' => 'group_content_access',
          'foreign keys' => array(),
          'indexes' => array(
            'value' => array(
              0 => 'value'
            )
          ),
          'module' => 'list',
          'no_ui' => TRUE,
          'settings' => array(
            'allowed_values' => array(
              0 => 'Use group defaults',
              1 => 'Public - accessible to all site users',
              2 => 'Private - accessible only to group members'
            ),
            'allowed_values_function' => ''
          ),
          'translatable' => '0',
          'type' => 'list_integer'
        ),
        'field_instance' => array(
          'bundle' => $bundle,
          'default_value' => array(
            0 => array(
              'value' => 0
            )
          ),
          'deleted' => '0',
          'description' => '',
          'display' => array(
            'default' => array(
              'label' => 'above',
              'settings' => array(),
              'type' => 'hidden',
              'weight' => '2'
            ),
            'teaser' => array(
              'label' => 'hidden',
              'settings' => array(),
              'type' => 'hidden',
              'weight' => '0'
            )
          ),
          'entity_type' => $entity_type,
          'field_name' => 'group_content_access',
          'label' => 'Group content visibility',
          'required' => TRUE,
          'settings' => array(
            'user_register_form' => FALSE
          ),
          'view modes' => array(
            'full' => array(
              'label' => 'above',
              'type' => 'list_default'
            ),
            'teaser' => array(
              'label' => 'above',
              'type' => 'list_default'
            )
          ),
          'widget' => array(
            'module' => 'options',
            'settings' => array(),
            'type' => 'options_select',
            'weight' => -2
          ),
          'widget_type' => 'options_select'
        )
      );
    }
  }
  // Exported field: 'og_membership-og_membership_type_default-og_membership_request'
  $fields['og_membership-og_membership_type_default-og_membership_request'] = array(
    'field_config' => array(
      'locked' => '1',
      'active' => '1',
      'cardinality' => '1',
      'deleted' => '0',
      'entity_types' => array(
        0 => 'og_membership'
      ),
      'field_name' => 'og_membership_request',
      'foreign keys' => array(
        'format' => array(
          'columns' => array(
            'format' => 'format'
          ),
          'table' => 'filter_format'
        )
      ),
      'indexes' => array(
        'format' => array(
          0 => 'format'
        )
      ),
      'module' => 'text',
      'settings' => array(),
      'translatable' => '0',
      'type' => 'text_long'
    ),
    'field_instance' => array(
      'bundle' => 'og_membership_type_default',
      'default_value' => NULL,
      'deleted' => '0',
      'description' => 'This is the text a user may send to the group administrators.',
      'display' => array(
        'default' => array(
          'label' => 'above',
          'module' => 'text',
          'settings' => array(),
          'type' => 'text_default',
          'weight' => 0
        )
      ),
      'entity_type' => 'og_membership',
      'field_name' => 'og_membership_request',
      'label' => 'Request message',
      'required' => FALSE,
      'settings' => array(
        'text_processing' => 0,
        'user_register_form' => FALSE
      ),
      'widget' => array(
        'module' => 'text',
        'settings' => array(
          'rows' => 5
        ),
        'type' => 'text_textarea',
        'weight' => 0
      )
    )
  );
  // Exported field: 'user-user-group_audience'
  $fields['user-user-group_audience'] = array(
    'field_config' => array(
      'locked' => '1',
      'active' => '1',
      'cardinality' => '-1',
      'deleted' => '0',
      'entity_types' => array(),
      'field_name' => 'group_audience',
      'foreign keys' => array(
        'og' => array(
          'columns' => array(
            'gid' => 'gid'
          ),
          'table' => 'og'
        )
      ),
      'indexes' => array(
        'gid' => array(
          0 => 'gid'
        )
      ),
      'module' => 'og',
      'no_ui' => TRUE,
      'settings' => array(),
      'translatable' => '0',
      'type' => 'group'
    ),
    'field_instance' => array(
      'bundle' => 'user',
      'default_value' => NULL,
      'default_value_function' => 'og_field_audience_default_value',
      'deleted' => '0',
      'description' => '',
      'display' => array(
        'default' => array(
          'label' => 'above',
          'module' => 'og',
          'settings' => array(),
          'type' => 'og_list_default',
          'weight' => '0'
        ),
        'full' => array(
          'label' => 'above',
          'module' => 'og',
          'settings' => array(),
          'type' => 'og_list_default',
          'weight' => '0'
        )
      ),
      'entity_type' => 'user',
      'field_name' => 'group_audience',
      'label' => 'Groups audience',
      'required' => FALSE,
      'settings' => array(
        'user_register_form' => FALSE
      ),
      'view modes' => array(
        'full' => array(
          'custom settings' => FALSE,
          'label' => 'Full',
          'type' => 'og_list_default'
        ),
        'teaser' => array(
          'custom settings' => FALSE,
          'label' => 'Teaser',
          'type' => 'og_list_default'
        )
      ),
      'widget' => array(
        'module' => 'og',
        'settings' => array(
          'opt_group' => 'auto'
        ),
        'type' => 'group_audience',
        'weight' => 7
      ),
      'widget_type' => 'group_audience'
    )
  );
  // Translatables
  // Included for use with string extractors like potx.
  t('Body');
  t('Group');
  t('Group content visibility');
  t('Groups audience');
  t('Leave this box checked to ensure that this node is a \'Group\'.');
  t('Request message');
  t('This is the text a user may send to the group administrators.');
  return $fields;
}
