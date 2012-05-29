<?php

/**
 * @file
 * scratchpads_registry_stats.features.field.inc
 */
/**
 * Implements hook_field_default_fields().
 */
function scratchpads_registry_stats_field_default_fields(){
  $fields = array();
  // Exported field: 'node-entity_data-field_label'
  $fields['node-entity_data-field_label'] = array(
    'field_config' => array(
      'active' => '1',
      'cardinality' => '1',
      'deleted' => '0',
      'entity_types' => array(),
      'field_name' => 'field_label',
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
      'settings' => array(
        'max_length' => '255'
      ),
      'translatable' => '0',
      'type' => 'text'
    ),
    'field_instance' => array(
      'bundle' => 'entity_data',
      'default_value' => NULL,
      'deleted' => '0',
      'description' => '',
      'display' => array(
        'default' => array(
          'label' => 'above',
          'module' => 'text',
          'settings' => array(),
          'type' => 'text_default',
          'weight' => 1
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
      'field_name' => 'field_label',
      'label' => 'Label',
      'required' => 1,
      'settings' => array(
        'linkit' => array(
          'enable' => 0,
          'insert_plugin' => ''
        ),
        'text_processing' => '0',
        'user_register_form' => FALSE
      ),
      'widget' => array(
        'active' => 1,
        'module' => 'text',
        'settings' => array(
          'size' => '60'
        ),
        'type' => 'text_textfield',
        'weight' => '-3'
      )
    )
  );
  // Exported field: 'node-entity_data-field_machine_name'
  $fields['node-entity_data-field_machine_name'] = array(
    'field_config' => array(
      'active' => '1',
      'cardinality' => '1',
      'deleted' => '0',
      'entity_types' => array(),
      'field_name' => 'field_machine_name',
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
      'settings' => array(
        'max_length' => '255'
      ),
      'translatable' => '0',
      'type' => 'text'
    ),
    'field_instance' => array(
      'bundle' => 'entity_data',
      'default_value' => NULL,
      'deleted' => '0',
      'description' => '',
      'display' => array(
        'default' => array(
          'label' => 'above',
          'module' => 'text',
          'settings' => array(),
          'type' => 'text_default',
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
      'field_name' => 'field_machine_name',
      'label' => 'Machine name',
      'required' => 1,
      'settings' => array(
        'linkit' => array(
          'enable' => 0,
          'insert_plugin' => ''
        ),
        'text_processing' => '0',
        'user_register_form' => FALSE
      ),
      'widget' => array(
        'active' => 1,
        'module' => 'text',
        'settings' => array(
          'size' => '60'
        ),
        'type' => 'text_textfield',
        'weight' => '-4'
      )
    )
  );
  // Exported field: 'node-entity_data-field_quantity'
  $fields['node-entity_data-field_quantity'] = array(
    'field_config' => array(
      'active' => '1',
      'cardinality' => '1',
      'deleted' => '0',
      'entity_types' => array(),
      'field_name' => 'field_quantity',
      'foreign keys' => array(),
      'indexes' => array(),
      'module' => 'number',
      'settings' => array(),
      'translatable' => '0',
      'type' => 'number_integer'
    ),
    'field_instance' => array(
      'bundle' => 'entity_data',
      'default_value' => array(
        0 => array(
          'value' => '0'
        )
      ),
      'deleted' => '0',
      'description' => '',
      'display' => array(
        'default' => array(
          'label' => 'above',
          'module' => 'number',
          'settings' => array(
            'decimal_separator' => '.',
            'prefix_suffix' => TRUE,
            'scale' => 0,
            'thousand_separator' => ' '
          ),
          'type' => 'number_integer',
          'weight' => 2
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
      'field_name' => 'field_quantity',
      'label' => 'Quantity',
      'required' => 1,
      'settings' => array(
        'max' => '',
        'min' => '',
        'prefix' => '',
        'suffix' => '',
        'user_register_form' => FALSE
      ),
      'widget' => array(
        'active' => 0,
        'module' => 'number',
        'settings' => array(),
        'type' => 'number',
        'weight' => '-2'
      )
    )
  );
  // Exported field: 'node-entity_data-field_scratchpad'
  $fields['node-entity_data-field_scratchpad'] = array(
    'field_config' => array(
      'active' => '1',
      'cardinality' => '1',
      'deleted' => '0',
      'entity_types' => array(),
      'field_name' => 'field_scratchpad',
      'foreign keys' => array(
        'nid' => array(
          'columns' => array(
            'nid' => 'nid'
          ),
          'table' => 'node'
        )
      ),
      'indexes' => array(
        'nid' => array(
          0 => 'nid'
        )
      ),
      'module' => 'node_reference',
      'settings' => array(
        'referenceable_types' => array(
          'biblio' => 0,
          'entity_data' => 0,
          'media_gallery' => 0,
          'organization' => 0,
          'page' => 0,
          'scratchpad' => 'scratchpad',
          'spm' => 0,
          'user_data' => 0
        ),
        'view' => array(
          'args' => array(),
          'display_name' => '',
          'view_name' => ''
        )
      ),
      'translatable' => '0',
      'type' => 'node_reference'
    ),
    'field_instance' => array(
      'bundle' => 'entity_data',
      'default_value' => NULL,
      'deleted' => '0',
      'description' => '',
      'display' => array(
        'default' => array(
          'label' => 'above',
          'module' => 'node_reference',
          'settings' => array(),
          'type' => 'node_reference_default',
          'weight' => 3
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
      'field_name' => 'field_scratchpad',
      'label' => 'Scratchpad',
      'required' => 1,
      'settings' => array(
        'user_register_form' => FALSE
      ),
      'widget' => array(
        'active' => 1,
        'module' => 'node_reference',
        'settings' => array(
          'autocomplete_match' => 'contains',
          'autocomplete_path' => 'node_reference/autocomplete',
          'size' => '60'
        ),
        'type' => 'node_reference_autocomplete',
        'weight' => '-1'
      )
    )
  );
  // Exported field: 'node-endpoint-field_scratchpad'
  $fields['node-endpoint-field_scratchpad'] = array(
    'field_config' => array(
      'active' => '1',
      'cardinality' => '1',
      'deleted' => '0',
      'entity_types' => array(),
      'field_name' => 'field_scratchpad',
      'foreign keys' => array(
        'nid' => array(
          'columns' => array(
            'nid' => 'nid',
          ),
          'table' => 'node',
        ),
      ),
      'indexes' => array(
        'nid' => array(
          0 => 'nid',
        ),
      ),
      'module' => 'node_reference',
      'settings' => array(
        'referenceable_types' => array(
          'article' => 0,
          'endpoint' => 0,
          'entity_data' => 0,
          'organization' => 0,
          'page' => 0,
          'scratchpad' => 'scratchpad',
          'user_data' => 0,
        ),
        'view' => array(
          'args' => array(),
          'display_name' => '',
          'view_name' => '',
        ),
      ),
      'translatable' => '0',
      'type' => 'node_reference',
    ),
    'field_instance' => array(
      'bundle' => 'endpoint',
      'default_value' => NULL,
      'deleted' => '0',
      'description' => '',
      'display' => array(
        'default' => array(
          'label' => 'above',
          'module' => 'node_reference',
          'settings' => array(),
          'type' => 'node_reference_default',
          'weight' => 3,
        ),
        'teaser' => array(
          'label' => 'above',
          'settings' => array(),
          'type' => 'hidden',
          'weight' => 0,
        ),
      ),
      'entity_type' => 'node',
      'field_name' => 'field_scratchpad',
      'label' => 'Scratchpad',
      'required' => 0,
      'settings' => array(
        'user_register_form' => FALSE,
      ),
      'widget' => array(
        'active' => 1,
        'module' => 'node_reference',
        'settings' => array(
          'autocomplete_match' => 'contains',
          'autocomplete_path' => 'node_reference/autocomplete',
          'size' => '60',
        ),
        'type' => 'node_reference_autocomplete',
        'weight' => 0,
      ),
    ),
  );
  // Exported field: 'node-organization-body'
  $fields['node-organization-body'] = array(
    'field_config' => array(
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
      'bundle' => 'organization',
      'default_value' => NULL,
      'deleted' => '0',
      'description' => '',
      'display' => array(
        'default' => array(
          'label' => 'hidden',
          'module' => 'text',
          'settings' => array(),
          'type' => 'text_default',
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
          'weight' => 0
        )
      ),
      'entity_type' => 'node',
      'field_name' => 'body',
      'label' => 'Description',
      'required' => 0,
      'settings' => array(
        'display_summary' => 1,
        'text_processing' => '1',
        'user_register_form' => FALSE
      ),
      'widget' => array(
        'active' => 1,
        'module' => 'text',
        'settings' => array(
          'rows' => '20',
          'summary_rows' => 5
        ),
        'type' => 'text_textarea_with_summary',
        'weight' => '-4'
      )
    )
  );
  // Exported field: 'node-organization-field_homepage'
  $fields['node-organization-field_homepage'] = array(
    'field_config' => array(
      'active' => '1',
      'cardinality' => '1',
      'deleted' => '0',
      'entity_types' => array(),
      'field_name' => 'field_homepage',
      'foreign keys' => array(),
      'indexes' => array(),
      'module' => 'link',
      'settings' => array(
        'attributes' => array(
          'class' => '',
          'rel' => '',
          'target' => 'default'
        ),
        'display' => array(
          'url_cutoff' => 80
        ),
        'enable_tokens' => 1,
        'title' => 'optional',
        'title_maxlength' => 128,
        'title_value' => '',
        'url' => 0
      ),
      'translatable' => '0',
      'type' => 'link_field'
    ),
    'field_instance' => array(
      'bundle' => 'organization',
      'default_value' => NULL,
      'deleted' => '0',
      'description' => '',
      'display' => array(
        'default' => array(
          'label' => 'above',
          'module' => 'link',
          'settings' => array(),
          'type' => 'link_default',
          'weight' => 2
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
      'field_name' => 'field_homepage',
      'label' => 'Homepage',
      'required' => 0,
      'settings' => array(
        'attributes' => array(
          'class' => '',
          'configurable_title' => 0,
          'rel' => '',
          'target' => 'default',
          'title' => ''
        ),
        'display' => array(
          'url_cutoff' => '80'
        ),
        'enable_tokens' => 1,
        'linkit' => array(
          'enable' => 0,
          'insert_plugin' => ''
        ),
        'title' => 'none',
        'title_maxlength' => '128',
        'title_value' => '',
        'url' => 0,
        'user_register_form' => FALSE,
        'validate_url' => 1
      ),
      'widget' => array(
        'active' => 0,
        'module' => 'link',
        'settings' => array(),
        'type' => 'link_field',
        'weight' => 0
      )
    )
  );
  // Exported field: 'node-organization-field_uuid'
  $fields['node-organization-field_uuid'] = array(
    'field_config' => array(
      'active' => '1',
      'cardinality' => '1',
      'deleted' => '0',
      'entity_types' => array(),
      'field_name' => 'field_uuid',
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
      'settings' => array(
        'max_length' => '36'
      ),
      'translatable' => '0',
      'type' => 'text'
    ),
    'field_instance' => array(
      'bundle' => 'organization',
      'default_value' => NULL,
      'deleted' => '0',
      'description' => '',
      'display' => array(
        'default' => array(
          'label' => 'above',
          'module' => 'text',
          'settings' => array(),
          'type' => 'text_default',
          'weight' => 1
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
      'field_name' => 'field_uuid',
      'label' => 'UUID',
      'required' => 1,
      'settings' => array(
        'linkit' => array(
          'enable' => 0,
          'insert_plugin' => ''
        ),
        'text_processing' => '0',
        'user_register_form' => FALSE
      ),
      'widget' => array(
        'active' => 1,
        'module' => 'text',
        'settings' => array(
          'size' => '36'
        ),
        'type' => 'text_textfield',
        'weight' => '-2'
      )
    )
  );
  // Exported field: 'node-scratchpad-body'
  $fields['node-scratchpad-body'] = array(
    'field_config' => array(
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
      'bundle' => 'scratchpad',
      'default_value' => NULL,
      'deleted' => '0',
      'description' => '',
      'display' => array(
        'default' => array(
          'label' => 'hidden',
          'module' => 'text',
          'settings' => array(),
          'type' => 'text_default',
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
          'weight' => 0
        )
      ),
      'entity_type' => 'node',
      'field_name' => 'body',
      'label' => 'Description',
      'required' => 0,
      'settings' => array(
        'display_summary' => 1,
        'text_processing' => '1',
        'user_register_form' => FALSE
      ),
      'widget' => array(
        'active' => 1,
        'module' => 'text',
        'settings' => array(
          'rows' => '20',
          'summary_rows' => 5
        ),
        'type' => 'text_textarea_with_summary',
        'weight' => '-4'
      )
    )
  );
  // Exported field: 'node-scratchpad-field_homepage'
  $fields['node-scratchpad-field_homepage'] = array(
    'field_config' => array(
      'active' => '1',
      'cardinality' => '1',
      'deleted' => '0',
      'entity_types' => array(),
      'field_name' => 'field_homepage',
      'foreign keys' => array(),
      'indexes' => array(),
      'module' => 'link',
      'settings' => array(
        'attributes' => array(
          'class' => '',
          'rel' => '',
          'target' => 'default'
        ),
        'display' => array(
          'url_cutoff' => 80
        ),
        'enable_tokens' => 1,
        'title' => 'optional',
        'title_maxlength' => 128,
        'title_value' => '',
        'url' => 0
      ),
      'translatable' => '0',
      'type' => 'link_field'
    ),
    'field_instance' => array(
      'bundle' => 'scratchpad',
      'default_value' => NULL,
      'deleted' => '0',
      'description' => '',
      'display' => array(
        'default' => array(
          'label' => 'above',
          'module' => 'link',
          'settings' => array(),
          'type' => 'link_default',
          'weight' => 1
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
      'field_name' => 'field_homepage',
      'label' => 'Homepage',
      'required' => 1,
      'settings' => array(
        'attributes' => array(
          'class' => '',
          'configurable_title' => 0,
          'rel' => '',
          'target' => 'default',
          'title' => ''
        ),
        'display' => array(
          'url_cutoff' => '80'
        ),
        'enable_tokens' => 1,
        'linkit' => array(
          'enable' => 0,
          'insert_plugin' => ''
        ),
        'title' => 'none',
        'title_maxlength' => '128',
        'title_value' => '',
        'url' => 0,
        'user_register_form' => FALSE,
        'validate_url' => 1
      ),
      'widget' => array(
        'active' => 0,
        'module' => 'link',
        'settings' => array(),
        'type' => 'link_field',
        'weight' => '-3'
      )
    )
  );
  // Exported field: 'node-scratchpad-field_organization'
  $fields['node-scratchpad-field_organization'] = array(
    'field_config' => array(
      'active' => '1',
      'cardinality' => '1',
      'deleted' => '0',
      'entity_types' => array(),
      'field_name' => 'field_organization',
      'foreign keys' => array(
        'nid' => array(
          'columns' => array(
            'nid' => 'nid'
          ),
          'table' => 'node'
        )
      ),
      'indexes' => array(
        'nid' => array(
          0 => 'nid'
        )
      ),
      'module' => 'node_reference',
      'settings' => array(
        'referenceable_types' => array(
          'biblio' => 0,
          'media_gallery' => 0,
          'organization' => 'organization',
          'page' => 0,
          'scratchpad' => 0,
          'spm' => 0
        ),
        'view' => array(
          'args' => array(),
          'display_name' => '',
          'view_name' => ''
        )
      ),
      'translatable' => '0',
      'type' => 'node_reference'
    ),
    'field_instance' => array(
      'bundle' => 'scratchpad',
      'default_value' => NULL,
      'deleted' => '0',
      'description' => '',
      'display' => array(
        'default' => array(
          'label' => 'above',
          'module' => 'node_reference',
          'settings' => array(),
          'type' => 'node_reference_default',
          'weight' => 3
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
      'field_name' => 'field_organization',
      'label' => 'Organization',
      'required' => 1,
      'settings' => array(
        'user_register_form' => FALSE
      ),
      'widget' => array(
        'active' => 1,
        'module' => 'options',
        'settings' => array(),
        'type' => 'options_select',
        'weight' => '-1'
      )
    )
  );
  // Exported field: 'node-scratchpad-field_uuid'
  $fields['node-scratchpad-field_uuid'] = array(
    'field_config' => array(
      'active' => '1',
      'cardinality' => '1',
      'deleted' => '0',
      'entity_types' => array(),
      'field_name' => 'field_uuid',
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
      'settings' => array(
        'max_length' => '36'
      ),
      'translatable' => '0',
      'type' => 'text'
    ),
    'field_instance' => array(
      'bundle' => 'scratchpad',
      'default_value' => NULL,
      'deleted' => '0',
      'description' => '',
      'display' => array(
        'default' => array(
          'label' => 'above',
          'module' => 'text',
          'settings' => array(),
          'type' => 'text_default',
          'weight' => 2
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
      'field_name' => 'field_uuid',
      'label' => 'UUID',
      'required' => 1,
      'settings' => array(
        'linkit' => array(
          'enable' => 0,
          'insert_plugin' => ''
        ),
        'text_processing' => '0',
        'user_register_form' => FALSE
      ),
      'widget' => array(
        'active' => 1,
        'module' => 'text',
        'settings' => array(
          'size' => '36'
        ),
        'type' => 'text_textfield',
        'weight' => '-2'
      )
    )
  );
  // Exported field: 'node-user_data-field_active_in_last_28_days'
  $fields['node-user_data-field_active_in_last_28_days'] = array(
    'field_config' => array(
      'active' => '1',
      'cardinality' => '1',
      'deleted' => '0',
      'entity_types' => array(),
      'field_name' => 'field_active_in_last_28_days',
      'foreign keys' => array(),
      'indexes' => array(),
      'module' => 'number',
      'settings' => array(),
      'translatable' => '0',
      'type' => 'number_integer'
    ),
    'field_instance' => array(
      'bundle' => 'user_data',
      'default_value' => array(
        0 => array(
          'value' => '0'
        )
      ),
      'deleted' => '0',
      'description' => '',
      'display' => array(
        'default' => array(
          'label' => 'above',
          'module' => 'number',
          'settings' => array(
            'decimal_separator' => '.',
            'prefix_suffix' => TRUE,
            'scale' => 0,
            'thousand_separator' => ' '
          ),
          'type' => 'number_integer',
          'weight' => 2
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
      'field_name' => 'field_active_in_last_28_days',
      'label' => 'Active in last 28 days',
      'required' => 1,
      'settings' => array(
        'max' => '',
        'min' => '',
        'prefix' => '',
        'suffix' => '',
        'user_register_form' => FALSE
      ),
      'widget' => array(
        'active' => 0,
        'module' => 'number',
        'settings' => array(),
        'type' => 'number',
        'weight' => '3'
      )
    )
  );
  // Exported field: 'node-user_data-field_active_in_last_7_days'
  $fields['node-user_data-field_active_in_last_7_days'] = array(
    'field_config' => array(
      'active' => '1',
      'cardinality' => '1',
      'deleted' => '0',
      'entity_types' => array(),
      'field_name' => 'field_active_in_last_7_days',
      'foreign keys' => array(),
      'indexes' => array(),
      'module' => 'number',
      'settings' => array(),
      'translatable' => '0',
      'type' => 'number_integer'
    ),
    'field_instance' => array(
      'bundle' => 'user_data',
      'default_value' => array(
        0 => array(
          'value' => '0'
        )
      ),
      'deleted' => '0',
      'description' => '',
      'display' => array(
        'default' => array(
          'label' => 'above',
          'module' => 'number',
          'settings' => array(
            'decimal_separator' => '.',
            'prefix_suffix' => TRUE,
            'scale' => 0,
            'thousand_separator' => ' '
          ),
          'type' => 'number_integer',
          'weight' => 3
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
      'field_name' => 'field_active_in_last_7_days',
      'label' => 'Active in last 7 days',
      'required' => 0,
      'settings' => array(
        'max' => '',
        'min' => '',
        'prefix' => '',
        'suffix' => '',
        'user_register_form' => FALSE
      ),
      'widget' => array(
        'active' => 0,
        'module' => 'number',
        'settings' => array(),
        'type' => 'number',
        'weight' => '2'
      )
    )
  );
  // Exported field: 'node-user_data-field_last_login'
  $fields['node-user_data-field_last_login'] = array(
    'field_config' => array(
      'active' => '1',
      'cardinality' => '1',
      'deleted' => '0',
      'entity_types' => array(),
      'field_name' => 'field_last_login',
      'foreign keys' => array(),
      'indexes' => array(),
      'module' => 'date',
      'settings' => array(
        'granularity' => array(
          'day' => 'day',
          'hour' => 'hour',
          'minute' => 'minute',
          'month' => 'month',
          'second' => 0,
          'year' => 'year'
        ),
        'repeat' => 0,
        'timezone_db' => '',
        'todate' => '',
        'tz_handling' => 'none'
      ),
      'translatable' => '0',
      'type' => 'datestamp'
    ),
    'field_instance' => array(
      'bundle' => 'user_data',
      'deleted' => '0',
      'description' => '',
      'display' => array(
        'default' => array(
          'label' => 'above',
          'module' => 'date',
          'settings' => array(
            'format_type' => 'long',
            'fromto' => 'both',
            'multiple_from' => '',
            'multiple_number' => '',
            'multiple_to' => '',
            'show_repeat_rule' => 'show'
          ),
          'type' => 'date_default',
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
      'field_name' => 'field_last_login',
      'label' => 'Last login',
      'required' => 1,
      'settings' => array(
        'default_value' => 'now',
        'default_value2' => 'blank',
        'default_value_code' => '',
        'default_value_code2' => '',
        'user_register_form' => FALSE
      ),
      'widget' => array(
        'active' => 1,
        'module' => 'date',
        'settings' => array(
          'increment' => 15,
          'input_format' => 'Y-m-d H:i:s',
          'input_format_custom' => '',
          'label_position' => 'above',
          'repeat_collapsed' => 0,
          'text_parts' => array(),
          'year_range' => '-3:+3'
        ),
        'type' => 'date_text',
        'weight' => '1'
      )
    )
  );
  // Exported field: 'node-user_data-field_scratchpad'
  $fields['node-user_data-field_scratchpad'] = array(
    'field_config' => array(
      'active' => '1',
      'cardinality' => '1',
      'deleted' => '0',
      'entity_types' => array(),
      'field_name' => 'field_scratchpad',
      'foreign keys' => array(
        'nid' => array(
          'columns' => array(
            'nid' => 'nid'
          ),
          'table' => 'node'
        )
      ),
      'indexes' => array(
        'nid' => array(
          0 => 'nid'
        )
      ),
      'module' => 'node_reference',
      'settings' => array(
        'referenceable_types' => array(
          'biblio' => 0,
          'entity_data' => 0,
          'media_gallery' => 0,
          'organization' => 0,
          'page' => 0,
          'scratchpad' => 'scratchpad',
          'spm' => 0,
          'user_data' => 0
        ),
        'view' => array(
          'args' => array(),
          'display_name' => '',
          'view_name' => ''
        )
      ),
      'translatable' => '0',
      'type' => 'node_reference'
    ),
    'field_instance' => array(
      'bundle' => 'user_data',
      'default_value' => NULL,
      'deleted' => '0',
      'description' => '',
      'display' => array(
        'default' => array(
          'label' => 'above',
          'module' => 'node_reference',
          'settings' => array(),
          'type' => 'node_reference_default',
          'weight' => 4
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
      'field_name' => 'field_scratchpad',
      'label' => 'Scratchpad',
      'required' => 1,
      'settings' => array(
        'user_register_form' => FALSE
      ),
      'widget' => array(
        'active' => 1,
        'module' => 'node_reference',
        'settings' => array(
          'autocomplete_match' => 'contains',
          'autocomplete_path' => 'node_reference/autocomplete',
          'size' => '60'
        ),
        'type' => 'node_reference_autocomplete',
        'weight' => '5'
      )
    )
  );
  // Exported field: 'node-endpoint-field_endpoint_type'
  $fields['node-endpoint-field_endpoint_type'] = array(
    'field_config' => array(
      'active' => '1',
      'cardinality' => '1',
      'deleted' => '0',
      'entity_types' => array(),
      'field_name' => 'field_endpoint_type',
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
      'settings' => array(
        'max_length' => '255'
      ),
      'translatable' => '0',
      'type' => 'text'
    ),
    'field_instance' => array(
      'bundle' => 'endpoint',
      'default_value' => NULL,
      'deleted' => '0',
      'description' => '',
      'display' => array(
        'default' => array(
          'label' => 'above',
          'module' => 'text',
          'settings' => array(),
          'type' => 'text_default',
          'weight' => 2
        ),
        'teaser' => array(
          'label' => 'above',
          'settings' => array(),
          'type' => 'hidden',
          'weight' => 0
        )
      ),
      'entity_type' => 'node',
      'field_name' => 'field_endpoint_type',
      'label' => 'Endpoint type',
      'required' => 1,
      'settings' => array(
        'text_processing' => '0',
        'user_register_form' => FALSE
      ),
      'widget' => array(
        'active' => 1,
        'module' => 'text',
        'settings' => array(
          'size' => '60'
        ),
        'type' => 'text_textfield',
        'weight' => '-2'
      )
    )
  );
  // Exported field: 'node-endpoint-field_endpoint_url'
  $fields['node-endpoint-field_endpoint_url'] = array(
    'field_config' => array(
      'active' => '1',
      'cardinality' => '1',
      'deleted' => '0',
      'entity_types' => array(),
      'field_name' => 'field_endpoint_url',
      'foreign keys' => array(),
      'indexes' => array(),
      'module' => 'link',
      'settings' => array(
        'attributes' => array(
          'class' => '',
          'rel' => '',
          'target' => 'default'
        ),
        'display' => array(
          'url_cutoff' => 80
        ),
        'enable_tokens' => 1,
        'title' => 'optional',
        'title_maxlength' => 128,
        'title_value' => '',
        'url' => 0
      ),
      'translatable' => '0',
      'type' => 'link_field'
    ),
    'field_instance' => array(
      'bundle' => 'endpoint',
      'default_value' => NULL,
      'deleted' => '0',
      'description' => '',
      'display' => array(
        'default' => array(
          'label' => 'above',
          'module' => 'link',
          'settings' => array(),
          'type' => 'link_default',
          'weight' => 1
        ),
        'teaser' => array(
          'label' => 'above',
          'settings' => array(),
          'type' => 'hidden',
          'weight' => 0
        )
      ),
      'entity_type' => 'node',
      'field_name' => 'field_endpoint_url',
      'label' => 'Endpoint URL',
      'required' => 0,
      'settings' => array(
        'attributes' => array(
          'class' => '',
          'configurable_title' => 0,
          'rel' => '',
          'target' => 'default',
          'title' => ''
        ),
        'display' => array(
          'url_cutoff' => '80'
        ),
        'enable_tokens' => 1,
        'title' => 'none',
        'title_maxlength' => '128',
        'title_value' => '',
        'url' => 0,
        'user_register_form' => FALSE,
        'validate_url' => 1
      ),
      'widget' => array(
        'active' => 0,
        'module' => 'link',
        'settings' => array(),
        'type' => 'link_field',
        'weight' => '-3'
      )
    )
  );
  // Exported field: 'node-endpoint-field_id'
  $fields['node-endpoint-field_id'] = array(
    'field_config' => array(
      'active' => '1',
      'cardinality' => '1',
      'deleted' => '0',
      'entity_types' => array(),
      'field_name' => 'field_id',
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
      'settings' => array(
        'max_length' => '255'
      ),
      'translatable' => '0',
      'type' => 'text'
    ),
    'field_instance' => array(
      'bundle' => 'endpoint',
      'default_value' => NULL,
      'deleted' => '0',
      'description' => '',
      'display' => array(
        'default' => array(
          'label' => 'above',
          'module' => 'text',
          'settings' => array(),
          'type' => 'text_default',
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
      'field_name' => 'field_id',
      'label' => 'ID',
      'required' => 1,
      'settings' => array(
        'text_processing' => '0',
        'user_register_form' => FALSE
      ),
      'widget' => array(
        'active' => 1,
        'module' => 'text',
        'settings' => array(
          'size' => '60'
        ),
        'type' => 'text_textfield',
        'weight' => '-4'
      )
    )
  );
  // Translatables
  // Included for use with string extractors like potx.
  t('Endpoint URL');
  t('Endpoint type');
  t('ID');
  t('Active in last 28 days');
  t('Active in last 7 days');
  t('Description');
  t('Homepage');
  t('Label');
  t('Last login');
  t('Machine name');
  t('Organization');
  t('Quantity');
  t('Scratchpad');
  t('UUID');
  return $fields;
}
