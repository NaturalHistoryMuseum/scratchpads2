<?php

/**
 * @file
 * spm.features.field.inc
 */
/**
 * Implements hook_field_default_fields().
 */
function spm_field_default_fields(){
  $fields = array();
  // Exported field: 'node-spm-field_taxonomic_name'
  $fields['node-spm-field_taxonomic_name'] = array(
    'field_config' => field_info_field('field_taxonomic_name'),
    'field_instance' => array(
      'bundle' => 'spm',
      'default_value' => NULL,
      'deleted' => '0',
      'description' => 'The full name of the lowest level taxon to which the organism has been identified in the most recent accepted determination, specified as precisely as possible',
      'display' => array(
        'default' => array(
          'label' => 'above',
          'module' => 'taxonomy_formatter',
          'settings' => array(
            'element_class' => '',
            'element_option' => '- None -',
            'links_option' => 1,
            'append_vocabulary' => 1,
            'separator_option' => ', ',
            'wrapper_class' => '',
            'wrapper_option' => '- None -'
          ),
          'type' => 'taxonomy_term_reference_csv',
          'weight' => '1000'
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
      'field_name' => 'field_taxonomic_name',
      'label' => 'Taxonomic name',
      'required' => 1,
      'settings' => array(
        'user_register_form' => FALSE
      ),
      'widget' => array(
        'active' => 0,
        'module' => 'taxonomy',
        'settings' => array(
          'autocomplete_path' => 'taxonomy/autocomplete',
          'size' => 60
        ),
        'type' => 'taxonomy_autocomplete',
        'weight' => '-3'
      )
    )
  );
  // Exported field: 'node-spm-field_spm_map'
  $fields['node-spm-field_spm_map'] = array(
    'field_config' => array(
      'locked' => '1',
      'active' => '1',
      'cardinality' => '-1',
      'deleted' => '0',
      'entity_types' => array(),
      'field_name' => 'field_spm_map',
      'foreign keys' => array(),
      'indexes' => array(
        'gm3_type' => array(
          0 => 'gm3_type'
        ),
        'latitude' => array(
          0 => 'latitude'
        ),
        'longitude' => array(
          0 => 'longitude'
        ),
        'region_id' => array(
          0 => 'region_id'
        )
      ),
      'module' => 'gm3_field',
      'settings' => array(
        'field_types' => array(
          'gm3_polygon' => 'gm3_polygon',
          'gm3_region' => 'gm3_region'
        ),
        'gm3_point' => 'Point',
        'gm3_polygon' => 'Polygon',
        'gm3_polyline' => 'Polyline',
        'gm3_rectangle' => 'Rectangle',
        'gm3_region' => 'Region'
      ),
      'translatable' => '0',
      'type' => 'gm3_combination'
    ),
    'field_instance' => array(
      'bundle' => 'spm',
      'default_value' => NULL,
      'deleted' => '0',
      'description' => '',
      'display' => array(
        'default' => array(
          'label' => 'above',
          'module' => 'gm3_field',
          'settings' => array(
            'display_convex_hull' => FALSE
          ),
          'type' => 'gm3_entity_map',
          'weight' => -38
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
      'field_name' => 'field_spm_map',
      'label' => 'Map',
      'required' => 0,
      'settings' => array(
        'user_register_form' => FALSE
      ),
      'widget' => array(
        'active' => 0,
        'module' => 'gm3_field',
        'settings' => array(),
        'type' => 'gm3_combination_gm3',
        'weight' => '-10'
      )
    )
  );
  // Exported field: 'node-spm-field_media'
  $fields['node-spm-field_media'] = array(
    'field_config' => array(
      'locked' => '1',
      'active' => '1',
      'cardinality' => '-1',
      'deleted' => '0',
      'entity_types' => array(),
      'field_name' => 'field_media',
      'foreign keys' => array(
        'fid' => array(
          'columns' => array(
            'fid' => 'fid'
          ),
          'table' => 'file_managed'
        )
      ),
      'indexes' => array(
        'fid' => array(
          0 => 'fid'
        )
      ),
      'module' => 'file',
      'settings' => array(
        'display_default' => 0,
        'display_field' => 0,
        'uri_scheme' => 'public'
      ),
      'translatable' => '0',
      'type' => 'file'
    ),
    'field_instance' => array(
      'bundle' => 'spm',
      'deleted' => '0',
      'description' => 'Select media to link them to this taxon description. Selected media will appear as links at the bottom of the page. Once media is selected, it may be edited, or removed from this taxon description.',
      'display' => array(
        'default' => array(
          'label' => 'above',
          'module' => 'file_entity',
          'settings' => array(
            'file_view_mode' => 'file_styles_square_thumbnail'
          ),
          'type' => 'file_rendered',
          'weight' => '100'
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
          'weight' => 100
        )
      ),
      'entity_type' => 'node',
      'field_name' => 'field_media',
      'label' => 'Media',
      'required' => 0,
      'settings' => array(
        'description_field' => 0,
        'file_directory' => '',
        'file_extensions' => FIELD_MEDIA_FILE_EXTENSIONS,
        'max_filesize' => '',
        'user_register_form' => FALSE
      ),
      'widget' => array(
        'active' => 1,
        'module' => 'media_multiselect',
        'settings' => array(
          'allowed_schemes' => array(
            'module' => 0,
            'public' => 'public'
          ),
          'allowed_types' => array(
            'audio' => 'audio',
            'default' => 0,
            'image' => 'image',
            'video' => 'video'
          ),
          'browser_plugins' => array(
            'media_default--media_browser_1' => 0,
            'media_default--media_browser_my_files' => 0,
            'upload' => 0
          ),
          'progress_indicator' => 'throbber'
        ),
        'type' => 'media_multiselect',
        'weight' => '2'
      )
    )
  );
  // Exported field: 'node-spm-field_associations'
  $fields['node-spm-field_associations'] = array(
    'field_config' => array(
      'locked' => '1',
      'active' => '1',
      'cardinality' => '1',
      'deleted' => '0',
      'entity_types' => array(),
      'field_name' => 'field_associations',
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
      'translatable' => '1',
      'type' => 'text_long'
    ),
    'field_instance' => array(
      'bundle' => 'spm',
      'default_value' => NULL,
      'deleted' => '0',
      'description' => 'Descriptions and lists of taxa that interact with the subject taxon. Includes explicit reference to the kind of ecological interaction: Predator/prey; host/parasite, pollinators, symbiosis, mutualism, commensalism; hybridisation, &hellip;',
      'display' => array(
        'default' => array(
          'label' => 'above',
          'module' => 'text',
          'settings' => array(),
          'type' => 'text_default',
          'weight' => '42'
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
      'field_name' => 'field_associations',
      'label' => 'Associations',
      'required' => 0,
      'settings' => array(
        'text_processing' => '1',
        'user_register_form' => FALSE
      ),
      'widget' => array(
        'active' => 1,
        'module' => 'text',
        'settings' => array(
          'rows' => '5'
        ),
        'type' => 'text_textarea',
        'weight' => '42'
      )
    )
  );
  // Exported field: 'node-spm-field_behavious'
  $fields['node-spm-field_behavious'] = array(
    'field_config' => array(
      'locked' => '1',
      'active' => '1',
      'cardinality' => '1',
      'deleted' => '0',
      'entity_types' => array(),
      'field_name' => 'field_behavious',
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
      'translatable' => '1',
      'type' => 'text_long'
    ),
    'field_instance' => array(
      'bundle' => 'spm',
      'default_value' => NULL,
      'deleted' => '0',
      'description' => 'Description of behaviour and behaviour patterns of an organism, including actions and reactions of organism in relation to its biotic and abiotic environment. Includes communication, perception, modes and mechanisms of locomotion, as well as long term strategies (except mating and reproductive strategies, covered under reproduction).',
      'display' => array(
        'default' => array(
          'label' => 'above',
          'module' => 'text',
          'settings' => array(),
          'type' => 'text_default',
          'weight' => '18'
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
      'field_name' => 'field_behavious',
      'label' => 'Behaviour',
      'required' => 0,
      'settings' => array(
        'text_processing' => '1',
        'user_register_form' => FALSE
      ),
      'widget' => array(
        'active' => 1,
        'module' => 'text',
        'settings' => array(
          'rows' => '5'
        ),
        'type' => 'text_textarea',
        'weight' => '18'
      )
    )
  );
  // Exported field: 'node-spm-field_biology'
  $fields['node-spm-field_biology'] = array(
    'field_config' => array(
      'locked' => '1',
      'active' => '1',
      'cardinality' => '1',
      'deleted' => '0',
      'entity_types' => array(),
      'field_name' => 'field_biology',
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
      'translatable' => '1',
      'type' => 'text_long'
    ),
    'field_instance' => array(
      'bundle' => 'spm',
      'default_value' => NULL,
      'deleted' => '0',
      'description' => 'An account of the biology of the taxon. E.g. behavior, reproduction, dispersal.',
      'display' => array(
        'default' => array(
          'label' => 'above',
          'module' => 'text',
          'settings' => array(),
          'type' => 'text_default',
          'weight' => '4'
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
      'field_name' => 'field_biology',
      'label' => 'Biology',
      'required' => 0,
      'settings' => array(
        'text_processing' => '1',
        'user_register_form' => FALSE
      ),
      'widget' => array(
        'active' => 1,
        'module' => 'text',
        'settings' => array(
          'rows' => '5'
        ),
        'type' => 'text_textarea',
        'weight' => '1'
      )
    )
  );
  // Exported field: 'node-spm-field_conservation_status'
  $fields['node-spm-field_conservation_status'] = array(
    'field_config' => array(
      'locked' => '1',
      'active' => '1',
      'cardinality' => '1',
      'deleted' => '0',
      'entity_types' => array(),
      'field_name' => 'field_conservation_status',
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
      'translatable' => '1',
      'type' => 'text_long'
    ),
    'field_instance' => array(
      'bundle' => 'spm',
      'default_value' => NULL,
      'deleted' => '0',
      'description' => 'A description of the likelihood of the species becoming extinct in the present day or in the near future. Population size is treated under Population Biology, and trends in population sizes are treated under Trends. However, this is the preferred element if an object includes all of these things and details about conservation listings.',
      'display' => array(
        'default' => array(
          'label' => 'above',
          'module' => 'text',
          'settings' => array(),
          'type' => 'text_default',
          'weight' => '6'
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
      'field_name' => 'field_conservation_status',
      'label' => 'Conservation status',
      'required' => 0,
      'settings' => array(
        'text_processing' => '1',
        'user_register_form' => FALSE
      ),
      'widget' => array(
        'active' => 1,
        'module' => 'text',
        'settings' => array(
          'rows' => '5'
        ),
        'type' => 'text_textarea',
        'weight' => '6'
      )
    )
  );
  // Exported field: 'node-spm-field_cyclicity'
  $fields['node-spm-field_cyclicity'] = array(
    'field_config' => array(
      'locked' => '1',
      'active' => '1',
      'cardinality' => '1',
      'deleted' => '0',
      'entity_types' => array(),
      'field_name' => 'field_cyclicity',
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
      'translatable' => '1',
      'type' => 'text_long'
    ),
    'field_instance' => array(
      'bundle' => 'spm',
      'default_value' => NULL,
      'deleted' => '0',
      'description' => 'Description of biorhythms, whether on the scale of seconds, hours, days, or seasons. Those states or conditions characterised by regular repetition in time. Could also cover phenomena such as chewing rates. Life cycles are treated in the Life Cycle term. Seasonal migration and reproduction are usually treated separately.',
      'display' => array(
        'default' => array(
          'label' => 'above',
          'module' => 'text',
          'settings' => array(),
          'type' => 'text_default',
          'weight' => '44'
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
      'field_name' => 'field_cyclicity',
      'label' => 'Cyclicity',
      'required' => 0,
      'settings' => array(
        'text_processing' => '1',
        'user_register_form' => FALSE
      ),
      'widget' => array(
        'active' => 1,
        'module' => 'text',
        'settings' => array(
          'rows' => '5'
        ),
        'type' => 'text_textarea',
        'weight' => '44'
      )
    )
  );
  // Exported field: 'node-spm-field_cytology'
  $fields['node-spm-field_cytology'] = array(
    'field_config' => array(
      'locked' => '1',
      'active' => '1',
      'cardinality' => '1',
      'deleted' => '0',
      'entity_types' => array(),
      'field_name' => 'field_cytology',
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
      'translatable' => '1',
      'type' => 'text_long'
    ),
    'field_instance' => array(
      'bundle' => 'spm',
      'default_value' => NULL,
      'deleted' => '0',
      'description' => 'Cell biology: formation, structure, organelles, and function of cells.',
      'display' => array(
        'default' => array(
          'label' => 'above',
          'module' => 'text',
          'settings' => array(),
          'type' => 'text_default',
          'weight' => '20'
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
      'field_name' => 'field_cytology',
      'label' => 'Cytology',
      'required' => 0,
      'settings' => array(
        'text_processing' => '1',
        'user_register_form' => FALSE
      ),
      'widget' => array(
        'active' => 1,
        'module' => 'text',
        'settings' => array(
          'rows' => '5'
        ),
        'type' => 'text_textarea',
        'weight' => '20'
      )
    )
  );
  // Exported field: 'node-spm-field_diagnostic_description'
  $fields['node-spm-field_diagnostic_description'] = array(
    'field_config' => array(
      'locked' => '1',
      'active' => '1',
      'cardinality' => '1',
      'deleted' => '0',
      'entity_types' => array(),
      'field_name' => 'field_diagnostic_description',
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
      'translatable' => '1',
      'type' => 'text_long'
    ),
    'field_instance' => array(
      'bundle' => 'spm',
      'default_value' => NULL,
      'deleted' => '0',
      'description' => 'Lists the features that distinguish this taxon from its closest relatives. May include but is not restricted to synapomorphies.',
      'display' => array(
        'default' => array(
          'label' => 'above',
          'module' => 'text',
          'settings' => array(),
          'type' => 'text_default',
          'weight' => '11'
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
      'field_name' => 'field_diagnostic_description',
      'label' => 'Diagnostic description',
      'required' => 0,
      'settings' => array(
        'text_processing' => '1',
        'user_register_form' => FALSE
      ),
      'widget' => array(
        'active' => 1,
        'module' => 'text',
        'settings' => array(
          'rows' => '5'
        ),
        'type' => 'text_textarea',
        'weight' => '14'
      )
    )
  );
  // Exported field: 'node-spm-field_diseases'
  $fields['node-spm-field_diseases'] = array(
    'field_config' => array(
      'locked' => '1',
      'active' => '1',
      'cardinality' => '1',
      'deleted' => '0',
      'entity_types' => array(),
      'field_name' => 'field_diseases',
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
      'translatable' => '1',
      'type' => 'text_long'
    ),
    'field_instance' => array(
      'bundle' => 'spm',
      'default_value' => NULL,
      'deleted' => '0',
      'description' => 'Description of diseases that the organism is subject to. Disease-causing organisms can also be listed under associations.',
      'display' => array(
        'default' => array(
          'label' => 'above',
          'module' => 'text',
          'settings' => array(),
          'type' => 'text_default',
          'weight' => '66'
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
      'field_name' => 'field_diseases',
      'label' => 'Diseases',
      'required' => 0,
      'settings' => array(
        'text_processing' => '1',
        'user_register_form' => FALSE
      ),
      'widget' => array(
        'active' => 1,
        'module' => 'text',
        'settings' => array(
          'rows' => '5'
        ),
        'type' => 'text_textarea',
        'weight' => '66'
      )
    )
  );
  // Exported field: 'node-spm-field_dispersal'
  $fields['node-spm-field_dispersal'] = array(
    'field_config' => array(
      'locked' => '1',
      'active' => '1',
      'cardinality' => '1',
      'deleted' => '0',
      'entity_types' => array(),
      'field_name' => 'field_dispersal',
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
      'translatable' => '1',
      'type' => 'text_long'
    ),
    'field_instance' => array(
      'bundle' => 'spm',
      'default_value' => NULL,
      'deleted' => '0',
      'description' => 'Description of the methods, circumstances, and timing of dispersal (includes both natal dispersal and interbreeding dispersal?)',
      'display' => array(
        'default' => array(
          'label' => 'above',
          'module' => 'text',
          'settings' => array(),
          'type' => 'text_default',
          'weight' => '22'
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
      'field_name' => 'field_dispersal',
      'label' => 'Dispersal',
      'required' => 0,
      'settings' => array(
        'text_processing' => '1',
        'user_register_form' => FALSE
      ),
      'widget' => array(
        'active' => 1,
        'module' => 'text',
        'settings' => array(
          'rows' => '5'
        ),
        'type' => 'text_textarea',
        'weight' => '27'
      )
    )
  );
  // Exported field: 'node-spm-field_distribution'
  $fields['node-spm-field_distribution'] = array(
    'field_config' => array(
      'locked' => '1',
      'active' => '1',
      'cardinality' => '1',
      'deleted' => '0',
      'entity_types' => array(),
      'field_name' => 'field_distribution',
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
      'translatable' => '1',
      'type' => 'text_long'
    ),
    'field_instance' => array(
      'bundle' => 'spm',
      'default_value' => NULL,
      'deleted' => '0',
      'description' => 'Covers ranges, e.g., a global range, or a narrower one; may be biogeographical, political or other (e.g., managed areas like conservencies); endemism; native or exotic; ref Darwin Core Geospatial extension. Does not include altitudinal distribution.',
      'display' => array(
        'default' => array(
          'label' => 'above',
          'module' => 'text',
          'settings' => array(),
          'type' => 'text_default',
          'weight' => '48'
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
      'field_name' => 'field_distribution',
      'label' => 'Distribution',
      'required' => 0,
      'settings' => array(
        'text_processing' => '1',
        'user_register_form' => FALSE
      ),
      'widget' => array(
        'active' => 1,
        'module' => 'text',
        'settings' => array(
          'rows' => '5'
        ),
        'type' => 'text_textarea',
        'weight' => '48'
      )
    )
  );
  // Exported field: 'node-spm-field_ecology'
  $fields['node-spm-field_ecology'] = array(
    'field_config' => array(
      'locked' => '1',
      'active' => '1',
      'cardinality' => '1',
      'deleted' => '0',
      'entity_types' => array(),
      'field_name' => 'field_ecology',
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
      'translatable' => '1',
      'type' => 'text_long'
    ),
    'field_instance' => array(
      'bundle' => 'spm',
      'default_value' => NULL,
      'deleted' => '0',
      'description' => '',
      'display' => array(
        'default' => array(
          'label' => 'above',
          'module' => 'text',
          'settings' => array(),
          'type' => 'text_default',
          'weight' => '50'
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
      'field_name' => 'field_ecology',
      'label' => 'Ecology',
      'required' => 0,
      'settings' => array(
        'text_processing' => '1',
        'user_register_form' => FALSE
      ),
      'widget' => array(
        'active' => 1,
        'module' => 'text',
        'settings' => array(
          'rows' => '5'
        ),
        'type' => 'text_textarea',
        'weight' => '50'
      )
    )
  );
  // Exported field: 'node-spm-field_evolution'
  $fields['node-spm-field_evolution'] = array(
    'field_config' => array(
      'locked' => '1',
      'active' => '1',
      'cardinality' => '1',
      'deleted' => '0',
      'entity_types' => array(),
      'field_name' => 'field_evolution',
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
      'translatable' => '1',
      'type' => 'text_long'
    ),
    'field_instance' => array(
      'bundle' => 'spm',
      'default_value' => NULL,
      'deleted' => '0',
      'description' => 'Description of the evolution of the taxon.',
      'display' => array(
        'default' => array(
          'label' => 'above',
          'module' => 'text',
          'settings' => array(),
          'type' => 'text_default',
          'weight' => '38'
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
      'field_name' => 'field_evolution',
      'label' => 'Evolution',
      'required' => 0,
      'settings' => array(
        'text_processing' => '1',
        'user_register_form' => FALSE
      ),
      'widget' => array(
        'active' => 1,
        'module' => 'text',
        'settings' => array(
          'rows' => '5'
        ),
        'type' => 'text_textarea',
        'weight' => '38'
      )
    )
  );
  // Exported field: 'node-spm-field_general_description'
  $fields['node-spm-field_general_description'] = array(
    'field_config' => array(
      'locked' => '1',
      'active' => '1',
      'cardinality' => '1',
      'deleted' => '0',
      'entity_types' => array(),
      'field_name' => 'field_general_description',
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
      'translatable' => '1',
      'type' => 'text_long'
    ),
    'field_instance' => array(
      'bundle' => 'spm',
      'default_value' => NULL,
      'deleted' => '0',
      'description' => 'A comprehensive description of the characteristics of the taxon. To be used primarily when many of the subject categories are treated together in one object, but at length. Taxon biology is to be used if a brief summary.',
      'display' => array(
        'default' => array(
          'label' => 'above',
          'module' => 'text',
          'settings' => array(),
          'type' => 'text_default',
          'weight' => '2'
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
      'field_name' => 'field_general_description',
      'label' => 'General description',
      'required' => 0,
      'settings' => array(
        'text_processing' => '1',
        'user_register_form' => FALSE
      ),
      'widget' => array(
        'active' => 1,
        'module' => 'text',
        'settings' => array(
          'rows' => '5'
        ),
        'type' => 'text_textarea',
        'weight' => '0'
      )
    )
  );
  // Exported field: 'node-spm-field_genetics'
  $fields['node-spm-field_genetics'] = array(
    'field_config' => array(
      'locked' => '1',
      'active' => '1',
      'cardinality' => '1',
      'deleted' => '0',
      'entity_types' => array(),
      'field_name' => 'field_genetics',
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
      'translatable' => '1',
      'type' => 'text_long'
    ),
    'field_instance' => array(
      'bundle' => 'spm',
      'default_value' => NULL,
      'deleted' => '0',
      'description' => 'Information on the genetics of the taxon, including karyotypes, barcoding status, whole genome sequencing status, ploidy.',
      'display' => array(
        'default' => array(
          'label' => 'above',
          'module' => 'text',
          'settings' => array(),
          'type' => 'text_default',
          'weight' => '22'
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
      'field_name' => 'field_genetics',
      'label' => 'Genetics',
      'required' => 0,
      'settings' => array(
        'text_processing' => '1',
        'user_register_form' => FALSE
      ),
      'widget' => array(
        'active' => 1,
        'module' => 'text',
        'settings' => array(
          'rows' => '5'
        ),
        'type' => 'text_textarea',
        'weight' => '22'
      )
    )
  );
  // Exported field: 'node-spm-field_growth'
  $fields['node-spm-field_growth'] = array(
    'field_config' => array(
      'locked' => '1',
      'active' => '1',
      'cardinality' => '1',
      'deleted' => '0',
      'entity_types' => array(),
      'field_name' => 'field_growth',
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
      'translatable' => '1',
      'type' => 'text_long'
    ),
    'field_instance' => array(
      'bundle' => 'spm',
      'default_value' => NULL,
      'deleted' => '0',
      'description' => 'Description of growth rates, allometries, parameters known to be predictive, morphometrics. Can also include hypotheses of paedomorphy or neoteny, etc.',
      'display' => array(
        'default' => array(
          'label' => 'above',
          'module' => 'text',
          'settings' => array(),
          'type' => 'text_default',
          'weight' => '24'
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
      'field_name' => 'field_growth',
      'label' => 'Growth',
      'required' => 0,
      'settings' => array(
        'text_processing' => '1',
        'user_register_form' => FALSE
      ),
      'widget' => array(
        'active' => 1,
        'module' => 'text',
        'settings' => array(
          'rows' => '5'
        ),
        'type' => 'text_textarea',
        'weight' => '24'
      )
    )
  );
  // Exported field: 'node-spm-field_habitat'
  $fields['node-spm-field_habitat'] = array(
    'field_config' => array(
      'locked' => '1',
      'active' => '1',
      'cardinality' => '1',
      'deleted' => '0',
      'entity_types' => array(),
      'field_name' => 'field_habitat',
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
      'translatable' => '1',
      'type' => 'text_long'
    ),
    'field_instance' => array(
      'bundle' => 'spm',
      'default_value' => NULL,
      'deleted' => '0',
      'description' => 'Includes realm (e.g Terrestrial etc) and climatic information (e.g Boreal); also includes requirements and tolerances; horizontal and vertical (altitudinal) distribution.',
      'display' => array(
        'default' => array(
          'label' => 'above',
          'module' => 'text',
          'settings' => array(),
          'type' => 'text_default',
          'weight' => '52'
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
      'field_name' => 'field_habitat',
      'label' => 'Habitat',
      'required' => 0,
      'settings' => array(
        'text_processing' => '1',
        'user_register_form' => FALSE
      ),
      'widget' => array(
        'active' => 1,
        'module' => 'text',
        'settings' => array(
          'rows' => '5'
        ),
        'type' => 'text_textarea',
        'weight' => '52'
      )
    )
  );
  // Exported field: 'node-spm-field_legislation'
  $fields['node-spm-field_legislation'] = array(
    'field_config' => array(
      'locked' => '1',
      'active' => '1',
      'cardinality' => '1',
      'deleted' => '0',
      'entity_types' => array(),
      'field_name' => 'field_legislation',
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
      'translatable' => '1',
      'type' => 'text_long'
    ),
    'field_instance' => array(
      'bundle' => 'spm',
      'default_value' => NULL,
      'deleted' => '0',
      'description' => 'Legal regulations or statutes relating to the taxon.',
      'display' => array(
        'default' => array(
          'label' => 'above',
          'module' => 'text',
          'settings' => array(),
          'type' => 'text_default',
          'weight' => '8'
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
      'field_name' => 'field_legislation',
      'label' => 'Legislation',
      'required' => 0,
      'settings' => array(
        'text_processing' => '1',
        'user_register_form' => FALSE
      ),
      'widget' => array(
        'active' => 1,
        'module' => 'text',
        'settings' => array(
          'rows' => '5'
        ),
        'type' => 'text_textarea',
        'weight' => '8'
      )
    )
  );
  // Exported field: 'node-spm-field_life_cycle'
  $fields['node-spm-field_life_cycle'] = array(
    'field_config' => array(
      'locked' => '1',
      'active' => '1',
      'cardinality' => '1',
      'deleted' => '0',
      'entity_types' => array(),
      'field_name' => 'field_life_cycle',
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
      'translatable' => '1',
      'type' => 'text_long'
    ),
    'field_instance' => array(
      'bundle' => 'spm',
      'default_value' => NULL,
      'deleted' => '0',
      'description' => 'Defines and describes obligatory developmental transformations. Includes metamorphosis, instars, gametophyte/embryophytes, transitions from sessile to mobile forms. Discusses timing. Morphology usually described in morphological descriptions.',
      'display' => array(
        'default' => array(
          'label' => 'above',
          'module' => 'text',
          'settings' => array(),
          'type' => 'text_default',
          'weight' => '54'
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
      'field_name' => 'field_life_cycle',
      'label' => 'Life cycle',
      'required' => 0,
      'settings' => array(
        'text_processing' => '1',
        'user_register_form' => FALSE
      ),
      'widget' => array(
        'active' => 1,
        'module' => 'text',
        'settings' => array(
          'rows' => '5'
        ),
        'type' => 'text_textarea',
        'weight' => '54'
      )
    )
  );
  // Exported field: 'node-spm-field_life_expectancy'
  $fields['node-spm-field_life_expectancy'] = array(
    'field_config' => array(
      'locked' => '1',
      'active' => '1',
      'cardinality' => '1',
      'deleted' => '0',
      'entity_types' => array(),
      'field_name' => 'field_life_expectancy',
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
      'translatable' => '1',
      'type' => 'text_long'
    ),
    'field_instance' => array(
      'bundle' => 'spm',
      'default_value' => NULL,
      'deleted' => '0',
      'description' => 'Any information on longevity, including The average period an organism can be expected to survive.',
      'display' => array(
        'default' => array(
          'label' => 'above',
          'module' => 'text',
          'settings' => array(),
          'type' => 'text_default',
          'weight' => '56'
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
      'field_name' => 'field_life_expectancy',
      'label' => 'Life expectancy',
      'required' => 0,
      'settings' => array(
        'text_processing' => '1',
        'user_register_form' => FALSE
      ),
      'widget' => array(
        'active' => 1,
        'module' => 'text',
        'settings' => array(
          'rows' => '5'
        ),
        'type' => 'text_textarea',
        'weight' => '56'
      )
    )
  );
  // Exported field: 'node-spm-field_look_alikes'
  $fields['node-spm-field_look_alikes'] = array(
    'field_config' => array(
      'locked' => '1',
      'active' => '1',
      'cardinality' => '1',
      'deleted' => '0',
      'entity_types' => array(),
      'field_name' => 'field_look_alikes',
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
      'translatable' => '1',
      'type' => 'text_long'
    ),
    'field_instance' => array(
      'bundle' => 'spm',
      'default_value' => NULL,
      'deleted' => '0',
      'description' => 'Other taxa that this taxon may be confused with. Useful for identification and comparison. Common in invasive species communities.',
      'display' => array(
        'default' => array(
          'label' => 'above',
          'module' => 'text',
          'settings' => array(),
          'type' => 'text_default',
          'weight' => '26'
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
      'field_name' => 'field_look_alikes',
      'label' => 'Look alikes',
      'required' => 0,
      'settings' => array(
        'text_processing' => '1',
        'user_register_form' => FALSE
      ),
      'widget' => array(
        'active' => 1,
        'module' => 'text',
        'settings' => array(
          'rows' => '5'
        ),
        'type' => 'text_textarea',
        'weight' => '26'
      )
    )
  );
  // Exported field: 'node-spm-field_management'
  $fields['node-spm-field_management'] = array(
    'field_config' => array(
      'locked' => '1',
      'active' => '1',
      'cardinality' => '1',
      'deleted' => '0',
      'entity_types' => array(),
      'field_name' => 'field_management',
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
      'translatable' => '1',
      'type' => 'text_long'
    ),
    'field_instance' => array(
      'bundle' => 'spm',
      'default_value' => NULL,
      'deleted' => '0',
      'description' => 'Describes techniques and goals used in management of species. May include management relative to a piece of legislation, e.g., a CITES list. [this is a change in the intent and will need to be considered by TDWG]',
      'display' => array(
        'default' => array(
          'label' => 'above',
          'module' => 'text',
          'settings' => array(),
          'type' => 'text_default',
          'weight' => '10'
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
      'field_name' => 'field_management',
      'label' => 'Management',
      'required' => 0,
      'settings' => array(
        'text_processing' => '1',
        'user_register_form' => FALSE
      ),
      'widget' => array(
        'active' => 1,
        'module' => 'text',
        'settings' => array(
          'rows' => '5'
        ),
        'type' => 'text_textarea',
        'weight' => '10'
      )
    )
  );
  // Exported field: 'node-spm-field_migration'
  $fields['node-spm-field_migration'] = array(
    'field_config' => array(
      'locked' => '1',
      'active' => '1',
      'cardinality' => '1',
      'deleted' => '0',
      'entity_types' => array(),
      'field_name' => 'field_migration',
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
      'translatable' => '1',
      'type' => 'text_long'
    ),
    'field_instance' => array(
      'bundle' => 'spm',
      'default_value' => NULL,
      'deleted' => '0',
      'description' => 'Description of the periodic movement of organisms from one locality to another (e.g., for breeding). Usually includes locality, timing, and hypothesized purpose.',
      'display' => array(
        'default' => array(
          'label' => 'above',
          'module' => 'text',
          'settings' => array(),
          'type' => 'text_default',
          'weight' => '58'
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
      'field_name' => 'field_migration',
      'label' => 'Migration',
      'required' => 0,
      'settings' => array(
        'text_processing' => '1',
        'user_register_form' => FALSE
      ),
      'widget' => array(
        'active' => 1,
        'module' => 'text',
        'settings' => array(
          'rows' => '5'
        ),
        'type' => 'text_textarea',
        'weight' => '58'
      )
    )
  );
  // Exported field: 'node-spm-field_molecular_biology'
  $fields['node-spm-field_molecular_biology'] = array(
    'field_config' => array(
      'locked' => '1',
      'active' => '1',
      'cardinality' => '1',
      'deleted' => '0',
      'entity_types' => array(),
      'field_name' => 'field_molecular_biology',
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
      'translatable' => '1',
      'type' => 'text_long'
    ),
    'field_instance' => array(
      'bundle' => 'spm',
      'default_value' => NULL,
      'deleted' => '0',
      'description' => 'Includes proteomic and biochemistry (e.g Toxicity). Genomic information is usually treated under genetics.',
      'display' => array(
        'default' => array(
          'label' => 'above',
          'module' => 'text',
          'settings' => array(),
          'type' => 'text_default',
          'weight' => '28'
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
      'field_name' => 'field_molecular_biology',
      'label' => 'Molecular biology',
      'required' => 0,
      'settings' => array(
        'text_processing' => '1',
        'user_register_form' => FALSE
      ),
      'widget' => array(
        'active' => 1,
        'module' => 'text',
        'settings' => array(
          'rows' => '5'
        ),
        'type' => 'text_textarea',
        'weight' => '28'
      )
    )
  );
  // Exported field: 'node-spm-field_morphology'
  $fields['node-spm-field_morphology'] = array(
    'field_config' => array(
      'locked' => '1',
      'active' => '1',
      'cardinality' => '1',
      'deleted' => '0',
      'entity_types' => array(),
      'field_name' => 'field_morphology',
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
      'translatable' => '1',
      'type' => 'text_long'
    ),
    'field_instance' => array(
      'bundle' => 'spm',
      'default_value' => NULL,
      'deleted' => '0',
      'description' => 'Description of the appearance of the taxon; e.g body plan, shape and color of external features, typical postures. May be referred to as or include habit, or anatomy.',
      'display' => array(
        'default' => array(
          'label' => 'above',
          'module' => 'text',
          'settings' => array(),
          'type' => 'text_default',
          'weight' => '30'
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
      'field_name' => 'field_morphology',
      'label' => 'Morphology',
      'required' => 0,
      'settings' => array(
        'text_processing' => '1',
        'user_register_form' => FALSE
      ),
      'widget' => array(
        'active' => 1,
        'module' => 'text',
        'settings' => array(
          'rows' => '5'
        ),
        'type' => 'text_textarea',
        'weight' => '30'
      )
    )
  );
  // Exported field: 'node-spm-field_phylogeny'
  $fields['node-spm-field_phylogeny'] = array(
    'field_config' => array(
      'locked' => '1',
      'active' => '1',
      'cardinality' => '1',
      'deleted' => '0',
      'entity_types' => array(),
      'field_name' => 'field_phylogeny',
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
      'translatable' => '1',
      'type' => 'text_long'
    ),
    'field_instance' => array(
      'bundle' => 'spm',
      'default_value' => NULL,
      'deleted' => '0',
      'description' => 'Description of phylogenetic and systematic treatments of the taxon.',
      'display' => array(
        'default' => array(
          'label' => 'above',
          'module' => 'text',
          'settings' => array(),
          'type' => 'text_default',
          'weight' => '40'
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
      'field_name' => 'field_phylogeny',
      'label' => 'Phylogeny',
      'required' => 0,
      'settings' => array(
        'text_processing' => '1',
        'user_register_form' => FALSE
      ),
      'widget' => array(
        'active' => 1,
        'module' => 'text',
        'settings' => array(
          'rows' => '5'
        ),
        'type' => 'text_textarea',
        'weight' => '40'
      )
    )
  );
  // Exported field: 'node-spm-field_physiology'
  $fields['node-spm-field_physiology'] = array(
    'field_config' => array(
      'locked' => '1',
      'active' => '1',
      'cardinality' => '1',
      'deleted' => '0',
      'entity_types' => array(),
      'field_name' => 'field_physiology',
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
      'translatable' => '1',
      'type' => 'text_long'
    ),
    'field_instance' => array(
      'bundle' => 'spm',
      'default_value' => NULL,
      'deleted' => '0',
      'description' => 'Description of physiological processes. Includes metabolic rates, and systems such as circulation, respiration, excretion, immunity, neurophysiology.',
      'display' => array(
        'default' => array(
          'label' => 'above',
          'module' => 'text',
          'settings' => array(),
          'type' => 'text_default',
          'weight' => '32'
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
      'field_name' => 'field_physiology',
      'label' => 'Physiology',
      'required' => 0,
      'settings' => array(
        'text_processing' => '1',
        'user_register_form' => FALSE
      ),
      'widget' => array(
        'active' => 1,
        'module' => 'text',
        'settings' => array(
          'rows' => '5'
        ),
        'type' => 'text_textarea',
        'weight' => '32'
      )
    )
  );
  // Exported field: 'node-spm-field_population_biology'
  $fields['node-spm-field_population_biology'] = array(
    'field_config' => array(
      'locked' => '1',
      'active' => '1',
      'cardinality' => '1',
      'deleted' => '0',
      'entity_types' => array(),
      'field_name' => 'field_population_biology',
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
      'translatable' => '1',
      'type' => 'text_long'
    ),
    'field_instance' => array(
      'bundle' => 'spm',
      'default_value' => NULL,
      'deleted' => '0',
      'description' => 'Includes abundance information (population size, density) and demographics (e.g. age stratification).',
      'display' => array(
        'default' => array(
          'label' => 'above',
          'module' => 'text',
          'settings' => array(),
          'type' => 'text_default',
          'weight' => '62'
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
      'field_name' => 'field_population_biology',
      'label' => 'Population biology',
      'required' => 0,
      'settings' => array(
        'text_processing' => '1',
        'user_register_form' => FALSE
      ),
      'widget' => array(
        'active' => 1,
        'module' => 'text',
        'settings' => array(
          'rows' => '5'
        ),
        'type' => 'text_textarea',
        'weight' => '62'
      )
    )
  );
  // Exported field: 'node-spm-field_procedures'
  $fields['node-spm-field_procedures'] = array(
    'field_config' => array(
      'locked' => '1',
      'active' => '1',
      'cardinality' => '1',
      'deleted' => '0',
      'entity_types' => array(),
      'field_name' => 'field_procedures',
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
      'translatable' => '1',
      'type' => 'text_long'
    ),
    'field_instance' => array(
      'bundle' => 'spm',
      'default_value' => NULL,
      'deleted' => '0',
      'description' => 'Deals with how you go about managing this taxon; what are the known threats to this taxon?',
      'display' => array(
        'default' => array(
          'label' => 'above',
          'module' => 'text',
          'settings' => array(),
          'type' => 'text_default',
          'weight' => '12'
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
      'field_name' => 'field_procedures',
      'label' => 'Procedures',
      'required' => 0,
      'settings' => array(
        'text_processing' => '1',
        'user_register_form' => FALSE
      ),
      'widget' => array(
        'active' => 1,
        'module' => 'text',
        'settings' => array(
          'rows' => '5'
        ),
        'type' => 'text_textarea',
        'weight' => '12'
      )
    )
  );
  // Exported field: 'node-spm-field_reproduction'
  $fields['node-spm-field_reproduction'] = array(
    'field_config' => array(
      'locked' => '1',
      'active' => '1',
      'cardinality' => '1',
      'deleted' => '0',
      'entity_types' => array(),
      'field_name' => 'field_reproduction',
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
      'translatable' => '1',
      'type' => 'text_long'
    ),
    'field_instance' => array(
      'bundle' => 'spm',
      'default_value' => NULL,
      'deleted' => '0',
      'description' => 'Description of reproductive physiology and behavior, including mating and life history variables. Includes cues, strategies, restraints, rates.',
      'display' => array(
        'default' => array(
          'label' => 'above',
          'module' => 'text',
          'settings' => array(),
          'type' => 'text_default',
          'weight' => '64'
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
      'field_name' => 'field_reproduction',
      'label' => 'Reproduction',
      'required' => 0,
      'settings' => array(
        'text_processing' => '1',
        'user_register_form' => FALSE
      ),
      'widget' => array(
        'active' => 1,
        'module' => 'text',
        'settings' => array(
          'rows' => '5'
        ),
        'type' => 'text_textarea',
        'weight' => '64'
      )
    )
  );
  // Exported field: 'node-spm-field_risk_statement'
  $fields['node-spm-field_risk_statement'] = array(
    'field_config' => array(
      'locked' => '1',
      'active' => '1',
      'cardinality' => '1',
      'deleted' => '0',
      'entity_types' => array(),
      'field_name' => 'field_risk_statement',
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
      'translatable' => '1',
      'type' => 'text_long'
    ),
    'field_instance' => array(
      'bundle' => 'spm',
      'default_value' => NULL,
      'deleted' => '0',
      'description' => 'Negative impacts on humans, communities. [This may also include impacts on ecosystems should the organism decline or be extirpated -- this is probably a change in intent from TDWG]',
      'display' => array(
        'default' => array(
          'label' => 'above',
          'module' => 'text',
          'settings' => array(),
          'type' => 'text_default',
          'weight' => '68'
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
      'field_name' => 'field_risk_statement',
      'label' => 'Risk statement',
      'required' => 0,
      'settings' => array(
        'text_processing' => '1',
        'user_register_form' => FALSE
      ),
      'widget' => array(
        'active' => 1,
        'module' => 'text',
        'settings' => array(
          'rows' => '5'
        ),
        'type' => 'text_textarea',
        'weight' => '68'
      )
    )
  );
  // Exported field: 'node-spm-field_size'
  $fields['node-spm-field_size'] = array(
    'field_config' => array(
      'locked' => '1',
      'active' => '1',
      'cardinality' => '1',
      'deleted' => '0',
      'entity_types' => array(),
      'field_name' => 'field_size',
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
      'translatable' => '1',
      'type' => 'text_long'
    ),
    'field_instance' => array(
      'bundle' => 'spm',
      'default_value' => NULL,
      'deleted' => '0',
      'description' => 'Average size, max, range; type of size (perimeter, length, volume, weight ...)',
      'display' => array(
        'default' => array(
          'label' => 'above',
          'module' => 'text',
          'settings' => array(),
          'type' => 'text_default',
          'weight' => '34'
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
      'field_name' => 'field_size',
      'label' => 'Size',
      'required' => 0,
      'settings' => array(
        'text_processing' => '1',
        'user_register_form' => FALSE
      ),
      'widget' => array(
        'active' => 1,
        'module' => 'text',
        'settings' => array(
          'rows' => '5'
        ),
        'type' => 'text_textarea',
        'weight' => '34'
      )
    )
  );
  // Exported field: 'node-spm-field_taxon_biology'
  $fields['node-spm-field_taxon_biology'] = array(
    'field_config' => array(
      'locked' => '1',
      'active' => '1',
      'cardinality' => '1',
      'deleted' => '0',
      'entity_types' => array(),
      'field_name' => 'field_taxon_biology',
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
      'translatable' => '1',
      'type' => 'text_long'
    ),
    'field_instance' => array(
      'bundle' => 'spm',
      'default_value' => NULL,
      'deleted' => '0',
      'description' => 'Summary or overview of all aspects of an organism\'s biology. [this may be a change in intent and need to be reviewed by TDWG]',
      'display' => array(
        'default' => array(
          'label' => 'above',
          'module' => 'text',
          'settings' => array(),
          'type' => 'text_default',
          'weight' => '36'
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
      'field_name' => 'field_taxon_biology',
      'label' => 'Taxon biology',
      'required' => 0,
      'settings' => array(
        'text_processing' => '1',
        'user_register_form' => FALSE
      ),
      'widget' => array(
        'active' => 1,
        'module' => 'text',
        'settings' => array(
          'rows' => '5'
        ),
        'type' => 'text_textarea',
        'weight' => '36'
      )
    )
  );
  // Exported field: 'node-spm-field_threats'
  $fields['node-spm-field_threats'] = array(
    'field_config' => array(
      'locked' => '1',
      'active' => '1',
      'cardinality' => '1',
      'deleted' => '0',
      'entity_types' => array(),
      'field_name' => 'field_threats',
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
      'translatable' => '1',
      'type' => 'text_long'
    ),
    'field_instance' => array(
      'bundle' => 'spm',
      'default_value' => NULL,
      'deleted' => '0',
      'description' => 'The threats to which this taxon is subject.',
      'display' => array(
        'default' => array(
          'label' => 'above',
          'module' => 'text',
          'settings' => array(),
          'type' => 'text_default',
          'weight' => '14'
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
      'field_name' => 'field_threats',
      'label' => 'Threats',
      'required' => 0,
      'settings' => array(
        'text_processing' => '1',
        'user_register_form' => FALSE
      ),
      'widget' => array(
        'active' => 1,
        'module' => 'text',
        'settings' => array(
          'rows' => '5'
        ),
        'type' => 'text_textarea',
        'weight' => '14'
      )
    )
  );
  // Exported field: 'node-spm-field_trends'
  $fields['node-spm-field_trends'] = array(
    'field_config' => array(
      'locked' => '1',
      'active' => '1',
      'cardinality' => '1',
      'deleted' => '0',
      'entity_types' => array(),
      'field_name' => 'field_trends',
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
      'translatable' => '1',
      'type' => 'text_long'
    ),
    'field_instance' => array(
      'bundle' => 'spm',
      'default_value' => NULL,
      'deleted' => '0',
      'description' => 'An indication of whether a population is stable, or increasing or decreasing.',
      'display' => array(
        'default' => array(
          'label' => 'above',
          'module' => 'text',
          'settings' => array(),
          'type' => 'text_default',
          'weight' => '16'
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
      'field_name' => 'field_trends',
      'label' => 'Trends',
      'required' => 0,
      'settings' => array(
        'text_processing' => '1',
        'user_register_form' => FALSE
      ),
      'widget' => array(
        'active' => 1,
        'module' => 'text',
        'settings' => array(
          'rows' => '5'
        ),
        'type' => 'text_textarea',
        'weight' => '16'
      )
    )
  );
  // Exported field: 'node-spm-field_trophic_strategy'
  $fields['node-spm-field_trophic_strategy'] = array(
    'field_config' => array(
      'locked' => '1',
      'active' => '1',
      'cardinality' => '1',
      'deleted' => '0',
      'entity_types' => array(),
      'field_name' => 'field_trophic_strategy',
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
      'translatable' => '1',
      'type' => 'text_long'
    ),
    'field_instance' => array(
      'bundle' => 'spm',
      'default_value' => NULL,
      'deleted' => '0',
      'description' => 'Summaries general nature of feeding interactions. For example, basic mode of nutrient uptake (autotrophy, heterotrophy, coprophagy, saprophagy), position in food network (top predator, primary producer, consumer), diet categorization (detritovore, omnivore, carnivore, herbivore). Specific lists of taxa are treated under associations (specifying predators or prey).',
      'display' => array(
        'default' => array(
          'label' => 'above',
          'module' => 'text',
          'settings' => array(),
          'type' => 'text_default',
          'weight' => '60'
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
      'field_name' => 'field_trophic_strategy',
      'label' => 'Trophic strategy',
      'required' => 0,
      'settings' => array(
        'text_processing' => '1',
        'user_register_form' => FALSE
      ),
      'widget' => array(
        'active' => 1,
        'module' => 'text',
        'settings' => array(
          'rows' => '5'
        ),
        'type' => 'text_textarea',
        'weight' => '60'
      )
    )
  );
  // Exported field: 'node-spm-field_uses'
  $fields['node-spm-field_uses'] = array(
    'field_config' => array(
      'locked' => '1',
      'active' => '1',
      'cardinality' => '1',
      'deleted' => '0',
      'entity_types' => array(),
      'field_name' => 'field_uses',
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
      'translatable' => '1',
      'type' => 'text_long'
    ),
    'field_instance' => array(
      'bundle' => 'spm',
      'default_value' => NULL,
      'deleted' => '0',
      'description' => 'Benefits for humans. ref Cook &#194;"Economic Botany&#194;" Can include ecosystem services. However, benefits to ecosystems not specific to humans are best treated under Risk statement (what happens when the organism is removed)',
      'display' => array(
        'default' => array(
          'label' => 'above',
          'module' => 'text',
          'settings' => array(),
          'type' => 'text_default',
          'weight' => '70'
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
      'field_name' => 'field_uses',
      'label' => 'Uses',
      'required' => 0,
      'settings' => array(
        'text_processing' => '1',
        'user_register_form' => FALSE
      ),
      'widget' => array(
        'active' => 1,
        'module' => 'text',
        'settings' => array(
          'rows' => '5'
        ),
        'type' => 'text_textarea',
        'weight' => '70'
      )
    )
  );
  // Translatables
  // Included for use with string extractors like potx.
  t('A comprehensive description of the characteristics of the taxon. To be used primarily when many of the subject categories are treated together in one object, but at length. Taxon biology is to be used if a brief summary.');
  t('A description of the likelihood of the species becoming extinct in the present day or in the near future. Population size is treated under Population Biology, and trends in population sizes are treated under Trends. However, this is the preferred element if an object includes all of these things and details about conservation listings.');
  t('An account of the biology of the taxon. E.g. behavior, reproduction, dispersal.');
  t('An indication of whether a population is stable, or increasing or decreasing.');
  t('Any information on longevity, including The average period an organism can be expected to survive.');
  t('Associations');
  t('Average size, max, range; type of size (perimeter, length, volume, weight ...)');
  t('Behaviour');
  t('Benefits for humans. ref Cook &#194;"Economic Botany&#194;" Can include ecosystem services. However, benefits to ecosystems not specific to humans are best treated under Risk statement (what happens when the organism is removed)');
  t('Biology');
  t('Cell biology: formation, structure, organelles, and function of cells.');
  t('Conservation status');
  t('Covers ranges, e.g., a global range, or a narrower one; may be biogeographical, political or other (e.g., managed areas like conservencies); endemism; native or exotic; ref Darwin Core Geospatial extension. Does not include altitudinal distribution.');
  t('Cyclicity');
  t('Cytology');
  t('Deals with how you go about managing this taxon; what are the known threats to this taxon?');
  t('Defines and describes obligatory developmental transformations. Includes metamorphosis, instars, gametophyte/embryophytes, transitions from sessile to mobile forms. Discusses timing. Morphology usually described in morphological descriptions.');
  t('Describes techniques and goals used in management of species. May include management relative to a piece of legislation, e.g., a CITES list. [this is a change in the intent and will need to be considered by TDWG]');
  t('Description of behaviour and behaviour patterns of an organism, including actions and reactions of organism in relation to its biotic and abiotic environment. Includes communication, perception, modes and mechanisms of locomotion, as well as long term strategies (except mating and reproductive strategies, covered under reproduction).');
  t('Description of biorhythms, whether on the scale of seconds, hours, days, or seasons. Those states or conditions characterised by regular repetition in time. Could also cover phenomena such as chewing rates. Life cycles are treated in the Life Cycle term. Seasonal migration and reproduction are usually treated separately.');
  t('Description of diseases that the organism is subject to. Disease-causing organisms can also be listed under associations.');
  t('Description of growth rates, allometries, parameters known to be predictive, morphometrics. Can also include hypotheses of paedomorphy or neoteny, etc.');
  t('Description of phylogenetic and systematic treatments of the taxon.');
  t('Description of physiological processes. Includes metabolic rates, and systems such as circulation, respiration, excretion, immunity, neurophysiology.');
  t('Description of reproductive physiology and behavior, including mating and life history variables. Includes cues, strategies, restraints, rates.');
  t('Description of the appearance of the taxon; e.g body plan, shape and color of external features, typical postures. May be referred to as or include habit, or anatomy.');
  t('Description of the evolution of the taxon.');
  t('Description of the methods, circumstances, and timing of dispersal (includes both natal dispersal and interbreeding dispersal?)');
  t('Description of the periodic movement of organisms from one locality to another (e.g., for breeding). Usually includes locality, timing, and hypothesized purpose.');
  t('Descriptions and lists of taxa that interact with the subject taxon. Includes explicit reference to the kind of ecological interaction: Predator&#194;/prey; host&#194;/parasite, pollinators, symbiosis, mutualism, commensalism; hybridisation, &#226;&#8364;&#166;');
  t('Diagnostic description');
  t('Diseases');
  t('Dispersal');
  t('Distribution');
  t('Ecology');
  t('Evolution');
  t('General description');
  t('Genetics');
  t('Growth');
  t('Habitat');
  t('Includes abundance information (population size, density) and demographics (e.g. age stratification).');
  t('Includes proteomic and biochemistry (e.g Toxicity). Genomic information is usually treated under genetics.');
  t('Includes realm (e.g Terrestrial etc) and climatic information (e.g Boreal); also includes requirements and tolerances; horizontal and vertical (altitudinal) distribution.');
  t('Information on the genetics of the taxon, including karyotypes, barcoding status, whole genome sequencing status, ploidy.');
  t('Legal regulations or statutes relating to the taxon.');
  t('Legislation');
  t('Life cycle');
  t('Life expectancy');
  t('Lists the features that distinguish this taxon from its closest relatives. May include but is not restricted to synapomorphies.');
  t('Look alikes');
  t('Management');
  t('Migration');
  t('Molecular biology');
  t('Morphology');
  t('Negative impacts on humans, communities. [This may also include impacts on ecosystems should the organism decline or be extirpated -- this is probably a change in intent from TDWG]');
  t('Other taxa that this taxon may be confused with. Useful for identification and comparison. Common in invasive species communities.');
  t('Phylogeny');
  t('Physiology');
  t('Population biology');
  t('Procedures');
  t('Reproduction');
  t('Risk statement');
  t('Size');
  t('Summaries general nature of feeding interactions. For example, basic mode of nutrient uptake (autotrophy, heterotrophy, coprophagy, saprophagy), position in food network (top predator, primary producer, consumer), diet categorization (detritovore, omnivore, carnivore, herbivore). Specific lists of taxa are treated under associations (specifying predators or prey).');
  t('Summary or overview of all aspects of an organism\'s biology. [this may be a change in intent and need to be reviewed by TDWG]');
  t('Taxon biology');
  t('The threats to which this taxon is subject.');
  t('Threats');
  t('Trends');
  t('Trophic strategy');
  t('Uses');
  t('Map');
  return $fields;
}
