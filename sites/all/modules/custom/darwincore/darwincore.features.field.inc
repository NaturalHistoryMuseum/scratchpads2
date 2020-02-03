<?php

/**
 * @file
 * darwincore.features.field.inc
 */
/**
 * Implements hook_field_default_fields().
 */
function darwincore_field_default_fields(){
  $fields = array();
  // Exported field: 'node-specimen_observation-field_media'
  $fields['node-specimen_observation-field_media'] = array(
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
      'bundle' => 'specimen_observation',
      'deleted' => '0',
      'description' => 'Select media to link them to this specimen/observation. Selected media will appear as links at the bottom of the page. Once media is selected, it may be edited, or removed from this specimen/observation.',
      'display' => array(
        'default' => array(
          'label' => 'above',
          'module' => 'file_entity',
          'settings' => array(
            'file_view_mode' => 'file_styles_square_thumbnail'
          ),
          'type' => 'file_rendered',
          'weight' => '1'
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
          'allowed_schemes' => array(),
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
  // Exported field: 'node-location-field_continent_or_ocean'
  $fields['node-location-field_continent_or_ocean'] = array(
    'field_config' => array(
      'locked' => '1',
      'active' => '1',
      'cardinality' => '1',
      'deleted' => '0',
      'entity_types' => array(),
      'field_name' => 'field_continent_or_ocean',
      'foreign keys' => array(),
      'indexes' => array(
        'value' => array(
          0 => 'value'
        )
      ),
      'module' => 'list',
      'settings' => array(
        'allowed_values' => array(
          'Africa' => 'Africa',
          'Antarctic' => 'Antarctic',
          'Asia-Temperate' => 'Asia-Temperate',
          'Asia-Tropical' => 'Asia-Tropical',
          'Atlantic Ocean' => 'Atlantic Ocean',
          'Australasia' => 'Australasia',
          'Europe' => 'Europe',
          'Indian Ocean' => 'Indian Ocean',
          'North-Central Pacific' => 'North-Central Pacific',
          'Northern America' => 'Northern America',
          'Northwestern Pacific' => 'Northwestern Pacific',
          'Pacific' => 'Pacific',
          'South-Central Pacific' => 'South-Central Pacific',
          'Southern America' => 'Southern America',
          'Southwestern Pacific' => 'Southwestern Pacific'
        ),
        'allowed_values_function' => ''
      ),
      'translatable' => '1',
      'type' => 'list_text'
    ),
    'field_instance' => array(
      'bundle' => 'location',
      'default_value' => NULL,
      'deleted' => '0',
      'description' => 'The full, unabbreviated name of the island group from which the catalogued item was collected.',
      'display' => array(
        'default' => array(
          'label' => 'above',
          'module' => 'list',
          'settings' => array(),
          'type' => 'list_default',
          'weight' => '2'
        ),
        'linked_node' => array(
          'label' => 'above',
          'module' => 'list',
          'settings' => array(),
          'type' => 'list_default',
          'weight' => '2'
        ),
        'teaser' => array(
          'label' => 'above',
          'settings' => array(),
          'type' => 'hidden',
          'weight' => 2
        )
      ),
      'entity_type' => 'node',
      'field_name' => 'field_continent_or_ocean',
      'label' => 'Continent/Ocean',
      'required' => 0,
      'settings' => array(
        'user_register_form' => FALSE
      ),
      'widget' => array(
        'active' => 1,
        'module' => 'options',
        'settings' => array(),
        'type' => 'options_select',
        'weight' => '2'
      )
    )
  );
  // Exported field: 'node-location-field_coordinate_system'
  $fields['node-location-field_coordinate_system'] = array(
    'field_config' => array(
      'locked' => '1',
      'active' => '1',
      'cardinality' => '1',
      'deleted' => '0',
      'entity_types' => array(),
      'field_name' => 'field_coordinate_system',
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
      'translatable' => '1',
      'type' => 'text'
    ),
    'field_instance' => array(
      'bundle' => 'location',
      'default_value' => NULL,
      'deleted' => '0',
      'description' => 'The name of the system in which the verbatim geographic coordinates were recorded. Examples: "decimal degrees", "degrees minutes seconds", "degrees decimal minutes", "UTM"',
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
          'module' => 'text',
          'settings' => array(),
          'type' => 'text_default',
          'weight' => '11'
        ),
        'teaser' => array(
          'label' => 'above',
          'settings' => array(),
          'type' => 'hidden',
          'weight' => 11
        )
      ),
      'entity_type' => 'node',
      'field_name' => 'field_coordinate_system',
      'label' => 'Verbatim Coordinate System',
      'required' => 0,
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
        'weight' => '10'
      )
    )
  );
  // Exported field: 'node-location-field_coordinate_uncertainty'
  $fields['node-location-field_coordinate_uncertainty'] = array(
    'field_config' => array(
      'locked' => '1',
      'active' => '1',
      'cardinality' => '1',
      'deleted' => '0',
      'entity_types' => array(),
      'field_name' => 'field_coordinate_uncertainty',
      'foreign keys' => array(),
      'indexes' => array(),
      'module' => 'number',
      'settings' => array(),
      'translatable' => '1',
      'type' => 'number_integer'
    ),
    'field_instance' => array(
      'bundle' => 'location',
      'default_value' => NULL,
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
          'weight' => '9'
        ),
        'linked_node' => array(
          'label' => 'above',
          'module' => 'number',
          'settings' => array(
            'decimal_separator' => '.',
            'prefix_suffix' => TRUE,
            'scale' => 0,
            'thousand_separator' => ' '
          ),
          'type' => 'number_integer',
          'weight' => '9'
        ),
        'teaser' => array(
          'label' => 'above',
          'settings' => array(),
          'type' => 'hidden',
          'weight' => 9
        )
      ),
      'entity_type' => 'node',
      'field_name' => 'field_coordinate_uncertainty',
      'label' => 'Coordinate uncertainty in meters',
      'required' => 0,
      'settings' => array(
        'max' => '',
        'min' => '',
        'prefix' => '',
        'suffix' => ' m',
        'user_register_form' => FALSE
      ),
      'widget' => array(
        'active' => 0,
        'module' => 'number',
        'settings' => array(),
        'type' => 'number',
        'weight' => '8'
      )
    )
  );
  // Exported field: 'node-location-field_country'
  $fields['node-location-field_country'] = array(
    'field_config' => array(
      'locked' => '1',
      'active' => '1',
      'cardinality' => '1',
      'deleted' => '0',
      'entity_types' => array(),
      'field_name' => 'field_country',
      'foreign keys' => array(),
      'indexes' => array(
        'iso2' => array(
          0 => 'iso2'
        )
      ),
      'module' => 'countries',
      'settings' => array(
        'continents' => array(),
        'enabled' => '1',
        'size' => 5
      ),
      'translatable' => '0',
      'type' => 'country'
    ),
    'field_instance' => array(
      'bundle' => 'location',
      'default_value' => NULL,
      'deleted' => '0',
      'description' => 'The name of the country or major administrative unit in which the Location occurs. Recommended best practice is to use a controlled vocabulary such as the Getty Thesaurus of Geographic Names.',
      'display' => array(
        'default' => array(
          'label' => 'above',
          'module' => 'countries',
          'settings' => array(),
          'type' => 'country_default',
          'weight' => '3'
        ),
        'linked_node' => array(
          'label' => 'above',
          'module' => 'countries',
          'settings' => array(),
          'type' => 'country_default',
          'weight' => '3'
        ),
        'teaser' => array(
          'label' => 'above',
          'module' => 'countries',
          'settings' => array(),
          'type' => 'country_default',
          'weight' => '3'
        )
      ),
      'entity_type' => 'node',
      'field_name' => 'field_country',
      'label' => 'Country',
      'required' => FALSE,
      'settings' => array(
        'user_register_form' => FALSE
      ),
      'widget' => array(
        'module' => 'options',
        'settings' => array(),
        'type' => 'options_select',
        'weight' => '3'
      )
    )
  );
  // Exported field: 'node-location-field_county'
  $fields['node-location-field_county'] = array(
    'field_config' => array(
      'locked' => '1',
      'active' => '1',
      'cardinality' => '1',
      'deleted' => '0',
      'entity_types' => array(),
      'field_name' => 'field_county',
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
      'translatable' => '1',
      'type' => 'text'
    ),
    'field_instance' => array(
      'bundle' => 'location',
      'default_value' => NULL,
      'deleted' => '0',
      'description' => 'The full, unabbreviated name of the county, shire, or municipality (i.e., the next smaller political region than StateProvince) from which the catalogued item was collected.',
      'display' => array(
        'default' => array(
          'label' => 'above',
          'module' => 'text',
          'settings' => array(),
          'type' => 'text_default',
          'weight' => '7'
        ),
        'linked_node' => array(
          'label' => 'above',
          'module' => 'text',
          'settings' => array(),
          'type' => 'text_default',
          'weight' => '7'
        ),
        'teaser' => array(
          'label' => 'above',
          'settings' => array(),
          'type' => 'hidden',
          'weight' => 7
        )
      ),
      'entity_type' => 'node',
      'field_name' => 'field_county',
      'label' => 'County',
      'required' => 0,
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
        'weight' => '6'
      )
    )
  );
  // Exported field: 'node-location-field_geodetic_datum'
  $fields['node-location-field_geodetic_datum'] = array(
    'field_config' => array(
      'locked' => '1',
      'active' => '1',
      'cardinality' => '1',
      'deleted' => '0',
      'entity_types' => array(),
      'field_name' => 'field_geodetic_datum',
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
      'translatable' => '1',
      'type' => 'text'
    ),
    'field_instance' => array(
      'bundle' => 'location',
      'default_value' => NULL,
      'deleted' => '0',
      'description' => 'The geodetic datum to which the latitude and longitude refer. If not known, use "not recorded".',
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
          'module' => 'text',
          'settings' => array(),
          'type' => 'text_default',
          'weight' => '10'
        ),
        'teaser' => array(
          'label' => 'above',
          'settings' => array(),
          'type' => 'hidden',
          'weight' => 10
        )
      ),
      'entity_type' => 'node',
      'field_name' => 'field_geodetic_datum',
      'label' => 'Geodetic datum',
      'required' => 0,
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
        'weight' => '9'
      )
    )
  );
  // Exported field: 'node-location-field_georeference_protocol'
  $fields['node-location-field_georeference_protocol'] = array(
    'field_config' => array(
      'locked' => '1',
      'active' => '1',
      'cardinality' => '1',
      'deleted' => '0',
      'entity_types' => array(),
      'field_name' => 'field_georeference_protocol',
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
      'translatable' => '1',
      'type' => 'text'
    ),
    'field_instance' => array(
      'bundle' => 'location',
      'default_value' => NULL,
      'deleted' => '0',
      'description' => 'A reference to the methods used for determining the coordinates and uncertainties. Example: "http://manisnet.org/GeorefGuide.html".',
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
          'module' => 'text',
          'settings' => array(),
          'type' => 'text_default',
          'weight' => '12'
        ),
        'teaser' => array(
          'label' => 'above',
          'settings' => array(),
          'type' => 'hidden',
          'weight' => 0
        )
      ),
      'entity_type' => 'node',
      'field_name' => 'field_georeference_protocol',
      'label' => 'Georeference protocol',
      'required' => 0,
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
        'weight' => '11'
      )
    )
  );
  // Exported field: 'node-location-field_georeference_remarks'
  $fields['node-location-field_georeference_remarks'] = array(
    'field_config' => array(
      'locked' => '1',
      'active' => '1',
      'cardinality' => '1',
      'deleted' => '0',
      'entity_types' => array(),
      'field_name' => 'field_georeference_remarks',
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
      'bundle' => 'location',
      'default_value' => NULL,
      'deleted' => '0',
      'description' => 'Comments about the spatial description determination, explaining assumptions made in addition or opposition to the those formalized in the method referred to in GeoreferenceProtocol.',
      'display' => array(
        'default' => array(
          'label' => 'above',
          'module' => 'text',
          'settings' => array(),
          'type' => 'text_default',
          'weight' => '13'
        ),
        'linked_node' => array(
          'label' => 'above',
          'module' => 'text',
          'settings' => array(),
          'type' => 'text_default',
          'weight' => '13'
        ),
        'teaser' => array(
          'label' => 'above',
          'settings' => array(),
          'type' => 'hidden',
          'weight' => 13
        )
      ),
      'entity_type' => 'node',
      'field_name' => 'field_georeference_remarks',
      'label' => 'Georeference remarks',
      'required' => 0,
      'settings' => array(
        'text_processing' => '0',
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
  // Exported field: 'node-location-field_island_group'
  $fields['node-location-field_island_group'] = array(
    'field_config' => array(
      'locked' => '1',
      'active' => '1',
      'cardinality' => '1',
      'deleted' => '0',
      'entity_types' => array(),
      'field_name' => 'field_island_group',
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
      'translatable' => '1',
      'type' => 'text'
    ),
    'field_instance' => array(
      'bundle' => 'location',
      'default_value' => NULL,
      'deleted' => '0',
      'description' => 'The full, unabbreviated name of the island group from which the catalogued item was collected.',
      'display' => array(
        'default' => array(
          'label' => 'above',
          'module' => 'text',
          'settings' => array(),
          'type' => 'text_default',
          'weight' => '5'
        ),
        'linked_node' => array(
          'label' => 'above',
          'module' => 'text',
          'settings' => array(),
          'type' => 'text_default',
          'weight' => '5'
        ),
        'teaser' => array(
          'label' => 'above',
          'settings' => array(),
          'type' => 'hidden',
          'weight' => 0
        )
      ),
      'entity_type' => 'node',
      'field_name' => 'field_island_group',
      'label' => 'Island group',
      'required' => 0,
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
        'weight' => '4'
      )
    )
  );
  // Exported field: 'node-location-field_island'
  $fields['node-location-field_island'] = array(
    'field_config' => array(
      'locked' => '1',
      'active' => '1',
      'cardinality' => '1',
      'deleted' => '0',
      'entity_types' => array(),
      'field_name' => 'field_island',
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
      'translatable' => '1',
      'type' => 'text'
    ),
    'field_instance' => array(
      'bundle' => 'location',
      'default_value' => NULL,
      'deleted' => '0',
      'description' => 'The full, unabbreviated name of the island from which the catalogued item was collected.',
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
          'module' => 'text',
          'settings' => array(),
          'type' => 'text_default',
          'weight' => '4'
        ),
        'teaser' => array(
          'label' => 'above',
          'settings' => array(),
          'type' => 'hidden',
          'weight' => 4
        )
      ),
      'entity_type' => 'node',
      'field_name' => 'field_island',
      'label' => 'Island',
      'required' => 0,
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
        'weight' => '4'
      )
    )
  );
  // Exported field: 'node-location-field_locality'
  $fields['node-location-field_locality'] = array(
    'field_config' => array(
      'locked' => '1',
      'active' => '1',
      'cardinality' => '1',
      'deleted' => '0',
      'entity_types' => array(),
      'field_name' => 'field_locality',
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
      'bundle' => 'location',
      'default_value' => NULL,
      'deleted' => '0',
      'description' => 'A description of the locality from which the catalogued item was collected, sans geographic information provided in other geographic fields.',
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
          'module' => 'text',
          'settings' => array(),
          'type' => 'text_default',
          'weight' => '8'
        ),
        'teaser' => array(
          'label' => 'above',
          'settings' => array(),
          'type' => 'hidden',
          'weight' => 8
        )
      ),
      'entity_type' => 'node',
      'field_name' => 'field_locality',
      'label' => 'Locality',
      'required' => 0,
      'settings' => array(
        'text_processing' => '0',
        'user_register_form' => FALSE
      ),
      'widget' => array(
        'active' => 1,
        'module' => 'text',
        'settings' => array(
          'rows' => '5'
        ),
        'type' => 'text_textarea',
        'weight' => '7'
      )
    )
  );
  // Exported field: 'node-location-field_map'
  $fields['node-location-field_map'] = array(
    'field_config' => array(
      'locked' => '1',
      'active' => '1',
      'cardinality' => '1',
      'deleted' => '0',
      'entity_types' => array(),
      'field_name' => 'field_map',
      'foreign keys' => array(),
      'indexes' => array(
        'latitude' => array(
          0 => 'latitude'
        ),
        'longitude' => array(
          0 => 'longitude'
        )
      ),
      'module' => 'gm3_field',
      'settings' => array(),
      'translatable' => '0',
      'type' => 'gm3_point'
    ),
    'field_instance' => array(
      'bundle' => 'location',
      'default_value' => NULL,
      'deleted' => '0',
      'description' => '',
      'display' => array(
        'default' => array(
          'label' => 'above',
          'module' => 'gm3_field',
          'settings' => array(
            'display_convex_hull' => FALSE,
            'point_radius_field' => 'field_coordinate_uncertainty'
          ),
          'type' => 'gm3_entity_map',
          'weight' => '1'
        ),
        'linked_node' => array(
          'label' => 'above',
          'module' => 'gm3_field',
          'settings' => array(
            'display_convex_hull' => FALSE,
            'point_radius_field' => 'field_coordinate_uncertainty'
          ),
          'type' => 'gm3_entity_map',
          'weight' => '1'
        ),
        'entityconnectpreview' => array(
          'label' => 'above',
          'module' => 'gm3_field',
          'settings' => array(
            'display_convex_hull' => FALSE,
            'point_radius_field' => 'field_coordinate_uncertainty'
          ),
          'type' => 'gm3_entity_map',
          'weight' => '1'
        ),
        'teaser' => array(
          'label' => 'above',
          'settings' => array(),
          'type' => 'hidden',
          'weight' => 0
        )
      ),
      'entity_type' => 'node',
      'field_name' => 'field_map',
      'label' => 'Map',
      'required' => 0,
      'settings' => array(
        'user_register_form' => FALSE
      ),
      'widget' => array(
        'active' => 0,
        'module' => 'gm3_field',
        'settings' => array(),
        'type' => 'gm3_point_gm3',
        'weight' => '2'
      )
    )
  );
  // Exported field: 'node-location-field_max_depth'
  $fields['node-location-field_max_depth'] = array(
    'field_config' => array(
      'locked' => '1',
      'active' => '1',
      'cardinality' => '1',
      'deleted' => '0',
      'entity_types' => array(),
      'field_name' => 'field_max_depth',
      'foreign keys' => array(),
      'indexes' => array(),
      'module' => 'number',
      'settings' => array(),
      'translatable' => '1',
      'type' => 'number_integer'
    ),
    'field_instance' => array(
      'bundle' => 'location',
      'default_value' => NULL,
      'deleted' => '0',
      'description' => 'The maximum or actual depth at which the collection or observation was made. Use positive values for locations below the surface. Examples: 10 (for a depth of up to 10m). 100 (for a depth between 50m and 100m).',
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
          'weight' => '17'
        ),
        'linked_node' => array(
          'label' => 'above',
          'module' => 'number',
          'settings' => array(
            'decimal_separator' => '.',
            'prefix_suffix' => TRUE,
            'scale' => 0,
            'thousand_separator' => ' '
          ),
          'type' => 'number_integer',
          'weight' => '17'
        ),
        'teaser' => array(
          'label' => 'above',
          'settings' => array(),
          'type' => 'hidden',
          'weight' => 17
        )
      ),
      'entity_type' => 'node',
      'field_name' => 'field_max_depth',
      'label' => 'Maximum depth',
      'required' => 0,
      'settings' => array(
        'max' => '',
        'min' => '',
        'prefix' => '',
        'suffix' => ' m',
        'user_register_form' => FALSE
      ),
      'widget' => array(
        'active' => 0,
        'module' => 'number',
        'settings' => array(),
        'type' => 'number',
        'weight' => '17'
      )
    )
  );
  // Exported field: 'node-location-field_max_elevation'
  $fields['node-location-field_max_elevation'] = array(
    'field_config' => array(
      'locked' => '1',
      'active' => '1',
      'cardinality' => '1',
      'deleted' => '0',
      'entity_types' => array(),
      'field_name' => 'field_max_elevation',
      'foreign keys' => array(),
      'indexes' => array(),
      'module' => 'number',
      'settings' => array(),
      'translatable' => '1',
      'type' => 'number_integer'
    ),
    'field_instance' => array(
      'bundle' => 'location',
      'default_value' => NULL,
      'deleted' => '0',
      'description' => 'The maximum distance in meters above (positive) or below sea level of the collecting locality.',
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
          'weight' => '14'
        ),
        'linked_node' => array(
          'label' => 'above',
          'module' => 'number',
          'settings' => array(
            'decimal_separator' => '.',
            'prefix_suffix' => TRUE,
            'scale' => 0,
            'thousand_separator' => ' '
          ),
          'type' => 'number_integer',
          'weight' => '14'
        ),
        'teaser' => array(
          'label' => 'above',
          'settings' => array(),
          'type' => 'hidden',
          'weight' => 0
        )
      ),
      'entity_type' => 'node',
      'field_name' => 'field_max_elevation',
      'label' => 'Maximum elevation',
      'required' => 0,
      'settings' => array(
        'max' => '',
        'min' => '',
        'prefix' => '',
        'suffix' => ' m',
        'user_register_form' => FALSE
      ),
      'widget' => array(
        'active' => 0,
        'module' => 'number',
        'settings' => array(),
        'type' => 'number',
        'weight' => '14'
      )
    )
  );
  // Exported field: 'node-location-field_min_depth'
  $fields['node-location-field_min_depth'] = array(
    'field_config' => array(
      'locked' => '1',
      'active' => '1',
      'cardinality' => '1',
      'deleted' => '0',
      'entity_types' => array(),
      'field_name' => 'field_min_depth',
      'foreign keys' => array(),
      'indexes' => array(),
      'module' => 'number',
      'settings' => array(),
      'translatable' => '1',
      'type' => 'number_integer'
    ),
    'field_instance' => array(
      'bundle' => 'location',
      'default_value' => NULL,
      'deleted' => '0',
      'description' => 'The minimum or actual depth at which the collection or observation was made. Use positive values for locations below the surface. Examples: 0 (for a depth of up to 10m). 50 (for a depth between 50m and 100m).',
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
          'weight' => '16'
        ),
        'linked_node' => array(
          'label' => 'above',
          'module' => 'number',
          'settings' => array(
            'decimal_separator' => '.',
            'prefix_suffix' => TRUE,
            'scale' => 0,
            'thousand_separator' => ' '
          ),
          'type' => 'number_integer',
          'weight' => '16'
        ),
        'teaser' => array(
          'label' => 'above',
          'settings' => array(),
          'type' => 'hidden',
          'weight' => 16
        )
      ),
      'entity_type' => 'node',
      'field_name' => 'field_min_depth',
      'label' => 'Minimum depth',
      'required' => 0,
      'settings' => array(
        'max' => '',
        'min' => '',
        'prefix' => '',
        'suffix' => ' m',
        'user_register_form' => FALSE
      ),
      'widget' => array(
        'active' => 0,
        'module' => 'number',
        'settings' => array(),
        'type' => 'number',
        'weight' => '16'
      )
    )
  );
  // Exported field: 'node-location-field_min_elevation'
  $fields['node-location-field_min_elevation'] = array(
    'field_config' => array(
      'locked' => '1',
      'active' => '1',
      'cardinality' => '1',
      'deleted' => '0',
      'entity_types' => array(),
      'field_name' => 'field_min_elevation',
      'foreign keys' => array(),
      'indexes' => array(),
      'module' => 'number',
      'settings' => array(),
      'translatable' => '1',
      'type' => 'number_integer'
    ),
    'field_instance' => array(
      'bundle' => 'location',
      'default_value' => NULL,
      'deleted' => '0',
      'description' => 'The minimum distance in meters above (positive) or below sea level of the collecting locality.',
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
          'weight' => '15'
        ),
        'linked_node' => array(
          'label' => 'above',
          'module' => 'number',
          'settings' => array(
            'decimal_separator' => '.',
            'prefix_suffix' => TRUE,
            'scale' => 0,
            'thousand_separator' => ' '
          ),
          'type' => 'number_integer',
          'weight' => '15'
        ),
        'teaser' => array(
          'label' => 'above',
          'settings' => array(),
          'type' => 'hidden',
          'weight' => 15
        )
      ),
      'entity_type' => 'node',
      'field_name' => 'field_min_elevation',
      'label' => 'Minimum elevation',
      'required' => 0,
      'settings' => array(
        'max' => '',
        'min' => '',
        'prefix' => '',
        'suffix' => ' m',
        'user_register_form' => FALSE
      ),
      'widget' => array(
        'active' => 0,
        'module' => 'number',
        'settings' => array(),
        'type' => 'number',
        'weight' => '15'
      )
    )
  );
  // Exported field: 'node-location-field_state_province'
  $fields['node-location-field_state_province'] = array(
    'field_config' => array(
      'locked' => '1',
      'active' => '1',
      'cardinality' => '1',
      'deleted' => '0',
      'entity_types' => array(),
      'field_name' => 'field_state_province',
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
      'translatable' => '1',
      'type' => 'text'
    ),
    'field_instance' => array(
      'bundle' => 'location',
      'default_value' => NULL,
      'deleted' => '0',
      'description' => 'The full, unabbreviated name of the state, province, or region (i.e., the next smaller political region than Country) from which the catalogued item was collected.',
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
          'module' => 'text',
          'settings' => array(),
          'type' => 'text_default',
          'weight' => '6'
        ),
        'teaser' => array(
          'label' => 'above',
          'settings' => array(),
          'type' => 'hidden',
          'weight' => 6
        )
      ),
      'entity_type' => 'node',
      'field_name' => 'field_state_province',
      'label' => 'State/Province',
      'required' => 0,
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
        'weight' => '5'
      )
    )
  );
  // Exported field: 'node-specimen_observation-field_basis_of_record'
  $fields['node-specimen_observation-field_basis_of_record'] = array(
    'field_config' => array(
      'locked' => '1',
      'active' => '1',
      'cardinality' => '1',
      'deleted' => '0',
      'entity_types' => array(),
      'field_name' => 'field_basis_of_record',
      'foreign keys' => array(),
      'indexes' => array(
        'value' => array(
          0 => 'value'
        )
      ),
      'module' => 'list',
      'settings' => array(
        'allowed_values' => array(
          'FossilSpecimen' => 'Fossil Specimen',
          'HumanObservation' => 'Human Observation',
          'LivingSpecimen' => 'Living Specimen',
          'MachineObservation' => 'Machine Observation',
          'PreservedSpecimen' => 'Preserved Specimen',
          'NomenclaturalChecklist' => 'Nomenclatural Checklist',
          'Sample' => 'Sample',
          'Occurrence' => 'Occurrence'
        ),
        'allowed_values_function' => ''
      ),
      'translatable' => '1',
      'type' => 'list_text'
    ),
    'field_instance' => array(
      'bundle' => 'specimen_observation',
      'default_value' => NULL,
      'deleted' => '0',
      'description' => 'A descriptive term indicating whether the record represents an object or observation.',
      'display' => array(
        'default' => array(
          'label' => 'above',
          'module' => 'list',
          'settings' => array(),
          'type' => 'list_default',
          'weight' => '1'
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
      'field_name' => 'field_basis_of_record',
      'label' => 'Basis of record',
      'required' => 1,
      'settings' => array(
        'user_register_form' => FALSE
      ),
      'widget' => array(
        'active' => 1,
        'module' => 'options',
        'settings' => array(),
        'type' => 'options_select',
        'weight' => '3'
      )
    )
  );
  // Exported field: 'node-specimen_observation-field_catalogue_number'
  $fields['node-specimen_observation-field_catalogue_number'] = array(
    'field_config' => array(
      'locked' => '1',
      'active' => '1',
      'cardinality' => '1',
      'deleted' => '0',
      'entity_types' => array(),
      'field_name' => 'field_catalogue_number',
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
      'translatable' => '1',
      'type' => 'text'
    ),
    'field_instance' => array(
      'bundle' => 'specimen_observation',
      'default_value' => NULL,
      'deleted' => '0',
      'description' => 'The catalogue number/identifier for this specimen',
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
      'field_name' => 'field_catalogue_number',
      'label' => 'Catalogue number',
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
        'weight' => '6'
      )
    )
  );
  // Exported field: 'node-specimen_observation-field_collection_code'
  $fields['node-specimen_observation-field_collection_code'] = array(
    'field_config' => array(
      'locked' => '1',
      'active' => '1',
      'cardinality' => '1',
      'deleted' => '0',
      'entity_types' => array(),
      'field_name' => 'field_collection_code',
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
      'translatable' => '1',
      'type' => 'text'
    ),
    'field_instance' => array(
      'bundle' => 'specimen_observation',
      'default_value' => NULL,
      'deleted' => '0',
      'description' => 'The code (or acronym) identifying the collection within the institution in which the organism record is catalogued.',
      'display' => array(
        'default' => array(
          'label' => 'above',
          'module' => 'text',
          'settings' => array(),
          'type' => 'text_default',
          'weight' => '3'
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
      'field_name' => 'field_collection_code',
      'label' => 'Collection code',
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
        'weight' => '5'
      )
    )
  );
  // Exported field: 'node-specimen_observation-field_collector'
  $fields['node-specimen_observation-field_collector'] = array(
    'field_config' => array(
      'locked' => '1',
      'active' => '1',
      'cardinality' => '-1',
      'deleted' => '0',
      'entity_types' => array(),
      'field_name' => 'field_collector',
      'foreign keys' => array(
        'uid' => array(
          'columns' => array(
            'uid' => 'uid'
          ),
          'table' => 'users'
        )
      ),
      'indexes' => array(
        'uid' => array(
          0 => 'uid'
        )
      ),
      'module' => 'user_reference',
      'settings' => array(
        'referenceable_roles' => array(
          2 => 0,
          3 => 0
        ),
        'referenceable_status' => array(
          0 => 0,
          1 => 0
        ),
        'view' => array(
          'args' => array(),
          'display_name' => 'references_1',
          'view_name' => 'users_references'
        )
      ),
      'translatable' => '1',
      'type' => 'user_reference'
    ),
    'field_instance' => array(
      'bundle' => 'specimen_observation',
      'default_value' => NULL,
      'deleted' => '0',
      'description' => 'The name(s) of the collector(s) of the original data for the cataloged item.',
      'display' => array(
        'default' => array(
          'label' => 'above',
          'module' => 'scratchpads_user_full_name',
          'settings' => array(
            'show_name_as_link' => 1,
            'separator_option' => ', '
          ),
          'type' => 'scratchpads_user_full_name',
          'weight' => '5'
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
      'field_name' => 'field_collector',
      'label' => 'Collector',
      'required' => 0,
      'settings' => array(
        'user_register_form' => FALSE
      ),
      'widget' => array(
        'active' => 1,
        'module' => 'user_reference',
        'settings' => array(
          'autocomplete_match' => 'contains',
          'autocomplete_path' => 'user_reference/autocomplete',
          'size' => '60'
        ),
        'type' => 'user_reference_autocomplete',
        'weight' => '9'
      )
    )
  );
  // Exported field: 'node-specimen_observation-field_collector_number'
  $fields['node-specimen_observation-field_collector_number'] = array(
    'field_config' => array(
      'locked' => '1',
      'active' => '1',
      'cardinality' => '1',
      'deleted' => '0',
      'entity_types' => array(),
      'field_name' => 'field_collector_number',
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
      'translatable' => '1',
      'type' => 'text'
    ),
    'field_instance' => array(
      'bundle' => 'specimen_observation',
      'default_value' => NULL,
      'deleted' => '0',
      'description' => 'An identifying string applied to the object or observation at the time of collection. Serves as a link between field notes and the object or observation.',
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
      'field_name' => 'field_collector_number',
      'label' => 'Collector number',
      'required' => 0,
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
        'weight' => '7'
      )
    )
  );
  // Exported field: 'node-specimen_observation-field_count'
  $fields['node-specimen_observation-field_count'] = array(
    'field_config' => array(
      'locked' => '1',
      'active' => '1',
      'cardinality' => '1',
      'deleted' => '0',
      'entity_types' => array(),
      'field_name' => 'field_count',
      'foreign keys' => array(),
      'indexes' => array(),
      'module' => 'number',
      'settings' => array(),
      'translatable' => '1',
      'type' => 'number_integer'
    ),
    'field_instance' => array(
      'bundle' => 'specimen_observation',
      'default_value' => NULL,
      'deleted' => '0',
      'description' => 'The number of individuals present in the lot or container. Not to be used for observations.',
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
      'field_name' => 'field_count',
      'label' => 'Count',
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
        'weight' => '14'
      )
    )
  );
  // Exported field: 'node-specimen_observation-field_date_collected'
  $fields['node-specimen_observation-field_date_collected'] = array(
    'field_config' => array(
      'locked' => '1',
      'active' => '1',
      'cardinality' => '1',
      'deleted' => '0',
      'entity_types' => array(),
      'field_name' => 'field_date_collected',
      'foreign keys' => array(),
      'indexes' => array(),
      'module' => 'date',
      'settings' => array(
        'granularity' => array(
          'day' => 'day',
          'month' => 'month',
          'year' => 'year'
        ),
        'repeat' => 0,
        'timezone_db' => '',
        'todate' => 'optional',
        'tz_handling' => 'none'
      ),
      'translatable' => '1',
      'type' => 'date'
    ),
    'field_instance' => array(
      'bundle' => 'specimen_observation',
      'deleted' => '0',
      'description' => 'The earliest and latest date-time (Common Era calendar) in a date-time period during which an organism or group of organisms was collected or observed. If the event is recorded as occurring at a single date-time, populate only the  Earliest Date Collected field',
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
          'weight' => '7'
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
      'field_name' => 'field_date_collected',
      'label' => 'Date collected',
      'required' => 0,
      'settings' => array(
        'default_format' => 'medium',
        'default_value' => 'blank',
        'default_value2' => 'blank',
        'default_value_code' => '',
        'default_value_code2' => '',
        'user_register_form' => FALSE
      ),
      'widget' => array(
        'active' => 1,
        'module' => 'date',
        'settings' => array(
          'increment' => 1,
          'input_format' => 'Y/m/d - H:i:s',
          'input_format_custom' => '',
          'label_position' => 'above',
          'repeat_collapsed' => 0,
          'text_parts' => array(),
          'year_range' => '-3:+3'
        ),
        'type' => 'date_text',
        'weight' => '10'
      )
    )
  );
  // Exported field: 'node-specimen_observation-field_date_identified'
  $fields['node-specimen_observation-field_date_identified'] = array(
    'field_config' => array(
      'locked' => '1',
      'active' => '1',
      'cardinality' => '1',
      'deleted' => '0',
      'entity_types' => array(),
      'field_name' => 'field_date_identified',
      'foreign keys' => array(),
      'indexes' => array(),
      'module' => 'date',
      'settings' => array(
        'granularity' => array(
          'day' => 'day',
          'month' => 'month',
          'year' => 'year'
        ),
        'repeat' => 0,
        'timezone_db' => '',
        'todate' => 'optional',
        'tz_handling' => 'none'
      ),
      'translatable' => '1',
      'type' => 'date'
    ),
    'field_instance' => array(
      'bundle' => 'specimen_observation',
      'deleted' => '0',
      'description' => 'The date in the Common Era calendar in which the object or observation was identified as being a member of the taxon given in the Scientific Name.  If dates are uncertain, use a range of dates (i.e. if you know the specimen was identified in 1982, use 1982/1/1 to 1982/12/31).',
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
          'weight' => '5'
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
      'field_name' => 'field_date_identified',
      'label' => 'Date identified',
      'required' => 0,
      'settings' => array(
        'default_format' => 'medium',
        'default_value' => 'blank',
        'default_value2' => 'blank',
        'default_value_code' => '',
        'default_value_code2' => '',
        'user_register_form' => FALSE
      ),
      'widget' => array(
        'active' => 1,
        'module' => 'date',
        'settings' => array(
          'increment' => 1,
          'input_format' => 'Y/m/d - H:i:s',
          'input_format_custom' => '',
          'label_position' => 'above',
          'repeat_collapsed' => 0,
          'text_parts' => array(),
          'year_range' => '-3:+3'
        ),
        'type' => 'date_text',
        'weight' => '6'
      )
    )
  );
  // Exported field: 'node-specimen_observation-field_genbank_number'
  $fields['node-specimen_observation-field_genbank_number'] = array(
    'field_config' => array(
      'locked' => '1',
      'active' => '1',
      'cardinality' => '1',
      'deleted' => '0',
      'entity_types' => array(),
      'field_name' => 'field_genbank_number',
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
      'translatable' => '1',
      'type' => 'text'
    ),
    'field_instance' => array(
      'bundle' => 'specimen_observation',
      'default_value' => NULL,
      'deleted' => '0',
      'description' => '',
      'display' => array(
        'default' => array(
          'label' => 'above',
          'module' => 'text',
          'settings' => array(),
          'type' => 'darwincore_link_to_ncbi',
          'weight' => '7'
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
      'field_name' => 'field_genbank_number',
      'label' => 'GenBank number(s)',
      'description' => t('Enter a comma separated list of numbers. For example: FN554775, FR694013'),
      'required' => 0,
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
        'weight' => '15'
      )
    )
  );
  // Exported field: 'node-specimen_observation-field_identification_qualifier'
  $fields['node-specimen_observation-field_identification_qualifier'] = array(
    'field_config' => array(
      'locked' => '1',
      'active' => '1',
      'cardinality' => '1',
      'deleted' => '0',
      'entity_types' => array(),
      'field_name' => 'field_identification_qualifier',
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
      'translatable' => '1',
      'type' => 'text'
    ),
    'field_instance' => array(
      'bundle' => 'specimen_observation',
      'default_value' => NULL,
      'deleted' => '0',
      'description' => 'A standard term to qualify the identification of the organism when doubts have arisen as to its identity. Examples: "cf.", "aff.", "subspecies in question"',
      'display' => array(
        'default' => array(
          'label' => 'above',
          'module' => 'text',
          'settings' => array(),
          'type' => 'text_default',
          'weight' => '3'
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
      'field_name' => 'field_identification_qualifier',
      'label' => 'Identification qualifier',
      'required' => 0,
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
        'weight' => '4'
      )
    )
  );
  // Exported field: 'node-specimen_observation-field_identified_by'
  $fields['node-specimen_observation-field_identified_by'] = array(
    'field_config' => array(
      'locked' => '1',
      'active' => '1',
      'cardinality' => '1',
      'deleted' => '0',
      'entity_types' => array(),
      'field_name' => 'field_identified_by',
      'foreign keys' => array(
        'uid' => array(
          'columns' => array(
            'uid' => 'uid'
          ),
          'table' => 'users'
        )
      ),
      'indexes' => array(
        'uid' => array(
          0 => 'uid'
        )
      ),
      'module' => 'user_reference',
      'settings' => array(
        'referenceable_roles' => array(
          2 => 0,
          3 => 0
        ),
        'referenceable_status' => array(
          0 => 0,
          1 => 0
        ),
        'view' => array(
          'args' => array(),
          'display_name' => 'references_1',
          'view_name' => 'users_references'
        )
      ),
      'translatable' => '1',
      'type' => 'user_reference'
    ),
    'field_instance' => array(
      'bundle' => 'specimen_observation',
      'default_value' => NULL,
      'deleted' => '0',
      'description' => 'The name(s) of the person(s) who applied the ScientificName to the object or observation. Example: James Earl Jones.',
      'display' => array(
        'default' => array(
          'label' => 'above',
          'module' => 'scratchpads_user_full_name',
          'settings' => array(
            'show_name_as_link' => 1,
            'separator_option' => ', '
          ),
          'type' => 'scratchpads_user_full_name',
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
      'field_name' => 'field_identified_by',
      'label' => 'Identified by',
      'required' => 0,
      'settings' => array(
        'user_register_form' => FALSE
      ),
      'widget' => array(
        'active' => 1,
        'module' => 'user_reference',
        'settings' => array(
          'autocomplete_match' => 'contains',
          'autocomplete_path' => 'user_reference/autocomplete',
          'size' => '60'
        ),
        'type' => 'user_reference_autocomplete',
        'weight' => '5'
      )
    )
  );
  // Exported field: 'node-specimen_observation-field_institution_code'
  $fields['node-specimen_observation-field_institution_code'] = array(
    'field_config' => array(
      'locked' => '1',
      'active' => '1',
      'cardinality' => '1',
      'deleted' => '0',
      'entity_types' => array(),
      'field_name' => 'field_institution_code',
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
      'translatable' => '1',
      'type' => 'text'
    ),
    'field_instance' => array(
      'bundle' => 'specimen_observation',
      'default_value' => NULL,
      'deleted' => '0',
      'description' => 'The code (or acronym) identifying the institution administering the collection in which the organism record is cataloged. No global registry exists for institutional codes; use the code that is "standard" at your institution or in your discipline.',
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
      'field_name' => 'field_institution_code',
      'label' => 'Institution code',
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
        'weight' => '4'
      )
    )
  );
  // Exported field: 'node-specimen_observation-field_lifestage'
  $fields['node-specimen_observation-field_lifestage'] = array(
    'field_config' => array(
      'locked' => '1',
      'active' => '1',
      'cardinality' => '1',
      'deleted' => '0',
      'entity_types' => array(),
      'field_name' => 'field_lifestage',
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
      'translatable' => '1',
      'type' => 'text'
    ),
    'field_instance' => array(
      'bundle' => 'specimen_observation',
      'default_value' => NULL,
      'deleted' => '0',
      'description' => 'The age class or life stage of the biological individual represented by the catalogued object or observation. Examples: "adult", "mature", "juvenile", "eft", "nymph", "seedling", "seed", "egg"',
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
      'field_name' => 'field_lifestage',
      'label' => 'Lifestage',
      'required' => 0,
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
        'weight' => '13'
      )
    )
  );
  // Exported field: 'node-specimen_observation-field_location'
  $fields['node-specimen_observation-field_location'] = array(
    'field_config' => array(
      'locked' => '1',
      'active' => '1',
      'cardinality' => '1',
      'deleted' => '0',
      'entity_types' => array(),
      'field_name' => 'field_location',
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
          'article' => 0,
          'location' => 'location',
          'page' => 0,
          'specimen_observation' => 0
        ),
        'view' => array(
          'args' => array(),
          'display_name' => '',
          'view_name' => ''
        )
      ),
      'translatable' => '1',
      'type' => 'node_reference'
    ),
    'field_instance' => array(
      'bundle' => 'specimen_observation',
      'default_value' => NULL,
      'deleted' => '0',
      'description' => 'Select a location that this specimen was collected from.',
      'display' => array(
        'default' => array(
          'label' => 'hidden',
          'module' => 'node_reference',
          'settings' => array(
            'node_reference_view_mode' => 'linked_node'
          ),
          'type' => 'node_reference_node',
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
      'field_name' => 'field_location',
      'label' => 'Location',
      'required' => 0,
      'settings' => array(
        'user_register_form' => FALSE
      ),
      'widget' => array(
        'active' => 1,
        'module' => 'options',
        'settings' => array(),
        'type' => 'options_select',
        'weight' => '18'
      )
    )
  );
  // Exported field: 'node-specimen_observation-field_notes'
  $fields['node-specimen_observation-field_notes'] = array(
    'field_config' => array(
      'locked' => '1',
      'active' => '1',
      'cardinality' => '1',
      'deleted' => '0',
      'entity_types' => array(),
      'field_name' => 'field_notes',
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
      'bundle' => 'specimen_observation',
      'default_value' => NULL,
      'deleted' => '0',
      'description' => 'Notes taken in the field for the catalogued item.',
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
      'field_name' => 'field_notes',
      'label' => 'Field notes',
      'required' => 0,
      'settings' => array(
        'text_processing' => '0',
        'user_register_form' => FALSE
      ),
      'widget' => array(
        'active' => 1,
        'module' => 'text',
        'settings' => array(
          'rows' => '5'
        ),
        'type' => 'text_textarea',
        'weight' => '11'
      )
    )
  );
  // Exported field: 'node-specimen_observation-field_number'
  $fields['node-specimen_observation-field_number'] = array(
    'field_config' => array(
      'locked' => '1',
      'active' => '1',
      'cardinality' => '1',
      'deleted' => '0',
      'entity_types' => array(),
      'field_name' => 'field_number',
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
      'translatable' => '1',
      'type' => 'text'
    ),
    'field_instance' => array(
      'bundle' => 'specimen_observation',
      'default_value' => NULL,
      'deleted' => '0',
      'description' => 'An identifying string applied to a set of material that resulted from a single collecting event.',
      'display' => array(
        'default' => array(
          'label' => 'above',
          'module' => 'text',
          'settings' => array(),
          'type' => 'text_default',
          'weight' => '9'
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
      'field_name' => 'field_number',
      'label' => 'Field number',
      'required' => 0,
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
        'weight' => '8'
      )
    )
  );
  // Exported field: 'node-specimen_observation-field_other_catalogue_numbers'
  $fields['node-specimen_observation-field_other_catalogue_numbers'] = array(
    'field_config' => array(
      'locked' => '1',
      'active' => '1',
      'cardinality' => '1',
      'deleted' => '0',
      'entity_types' => array(),
      'field_name' => 'field_other_catalogue_numbers',
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
      'bundle' => 'specimen_observation',
      'default_value' => NULL,
      'deleted' => '0',
      'description' => 'A list of previous or alternative fully qualified catalog numbers for the same object or observation, whether in the current collection or in any other.  One per line',
      'display' => array(
        'default' => array(
          'label' => 'above',
          'module' => 'text',
          'settings' => array(),
          'type' => 'text_default',
          'weight' => '9'
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
      'field_name' => 'field_other_catalogue_numbers',
      'label' => 'Other catalogue numbers',
      'required' => 0,
      'settings' => array(
        'text_processing' => '0',
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
  // Exported field: 'node-specimen_observation-field_remarks'
  $fields['node-specimen_observation-field_remarks'] = array(
    'field_config' => array(
      'locked' => '1',
      'active' => '1',
      'cardinality' => '1',
      'deleted' => '0',
      'entity_types' => array(),
      'field_name' => 'field_remarks',
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
      'bundle' => 'specimen_observation',
      'default_value' => NULL,
      'deleted' => '0',
      'description' => 'General comments about the specimen that others may find useful.',
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
      'field_name' => 'field_remarks',
      'label' => 'Remarks',
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
        'weight' => '17'
      )
    )
  );
  // Exported field: 'node-specimen_observation-field_sex'
  $fields['node-specimen_observation-field_sex'] = array(
    'field_config' => array(
      'locked' => '1',
      'active' => '1',
      'cardinality' => '1',
      'deleted' => '0',
      'entity_types' => array(),
      'field_name' => 'field_sex',
      'foreign keys' => array(),
      'indexes' => array(
        'value' => array(
          0 => 'value'
        )
      ),
      'module' => 'list',
      'settings' => array(
        'allowed_values' => array(
          'Female' => 'Female',
          'Hermaphrodite' => 'Hermaphrodite',
          'Indeterminate' => 'Indeterminate',
          'Male' => 'Male',
          'Multiple' => 'Multiple',
          'Transitional' => 'Transitional',
          'Undetermined' => 'Undetermined',
          'Unknown' => 'Unknown'
        ),
        'allowed_values_function' => ''
      ),
      'translatable' => '1',
      'type' => 'list_text'
    ),
    'field_instance' => array(
      'bundle' => 'specimen_observation',
      'default_value' => NULL,
      'deleted' => '0',
      'description' => '',
      'display' => array(
        'default' => array(
          'label' => 'above',
          'module' => 'list',
          'settings' => array(),
          'type' => 'list_default',
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
      'field_name' => 'field_sex',
      'label' => 'Sex',
      'required' => 0,
      'settings' => array(
        'user_register_form' => FALSE
      ),
      'widget' => array(
        'active' => 1,
        'module' => 'options',
        'settings' => array(),
        'type' => 'options_select',
        'weight' => '12'
      )
    )
  );
  // Exported field: 'node-specimen_observation-field_taxonomic_name'
  $fields['node-specimen_observation-field_taxonomic_name'] = array(
    'field_config' => field_info_field('field_taxonomic_name'),
    'field_instance' => array(
      'bundle' => 'specimen_observation',
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
      'required' => 0,
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
  // Exported field: 'node-specimen_observation-field_type_status'
  $fields['node-specimen_observation-field_type_status'] = array(
    'field_config' => array(
      'locked' => '1',
      'active' => '1',
      'cardinality' => '1',
      'deleted' => '0',
      'entity_types' => array(),
      'field_name' => 'field_type_status',
      'foreign keys' => array(),
      'indexes' => array(
        'value' => array(
          0 => 'value'
        )
      ),
      'module' => 'list',
      'settings' => array(
        'allowed_values' => array(
          'allolectotype' => 'allolectotype',
          'alloneotype' => 'alloneotype',
          'allotype' => 'allotype',
          'cotype' => 'cotype',
          'epitype' => 'epitype',
          'exepitype' => 'exepitype',
          'exholotype' => 'exholotype',
          'exisotype' => 'exisotype',
          'exlectotype' => 'exlectotype',
          'exneotype' => 'exneotype',
          'exparatype' => 'exparatype',
          'exsyntype' => 'exsyntype',
          'extype' => 'extype',
          'hapantotype' => 'hapantotype',
          'holotype' => 'holotype',
          'iconotype' => 'iconotype',
          'isolectotype' => 'isolectotype',
          'isoneotype' => 'isoneotype',
          'isosyntype' => 'isosyntype',
          'isotype' => 'isotype',
          'lectotype' => 'lectotype',
          'neotype' => 'neotype',
          'notatype' => 'notatype',
          'originalmaterial' => 'originalmaterial',
          'paralectotype' => 'paralectotype',
          'paraneotype' => 'paraneotype',
          'paratype' => 'paratype',
          'plastoholotype' => 'plastoholotype',
          'plastoisotype' => 'plastoisotype',
          'plastolectotype' => 'plastolectotype',
          'plastoneotype' => 'plastoneotype',
          'plastoparatype' => 'plastoparatype',
          'plastosyntype' => 'plastosyntype',
          'plastotype' => 'plastotype',
          'secondarytype' => 'secondarytype',
          'supplementarytype' => 'supplementarytype',
          'syntype' => 'syntype',
          'topotype' => 'topotype',
          'type' => 'type',
          'typeSpecies' => 'typeSpecies',
          'typeGenus' => 'typeGenus'
        ),
        'allowed_values_function' => ''
      ),
      'translatable' => '1',
      'type' => 'list_text'
    ),
    'field_instance' => array(
      'bundle' => 'specimen_observation',
      'default_value' => NULL,
      'deleted' => '0',
      'description' => '',
      'display' => array(
        'default' => array(
          'label' => 'above',
          'module' => 'list',
          'settings' => array(),
          'type' => 'list_default',
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
      'field_name' => 'field_type_status',
      'label' => 'Type status',
      'required' => 0,
      'settings' => array(
        'user_register_form' => FALSE
      ),
      'widget' => array(
        'active' => 1,
        'module' => 'options',
        'settings' => array(),
        'type' => 'options_select',
        'weight' => '2'
      )
    )
  );
  // Translatables
  // Included for use with string extractors like potx.
  t('A description of the locality from which the catalogued item was collected, sans geographic information provided in other geographic fields.');
  t('A descriptive term indicating whether the record represents an object or observation.');
  t('A list of previous or alternative fully qualified catalog numbers for the same object or observation, whether in the current collection or in any other.  One per line');
  t('A reference to the methods used for determining the coordinates and uncertainties. Example: "http://manisnet.org/GeorefGuide.html".');
  t('A standard term to qualify the identification of the organism when doubts have arisen as to its identity. Examples: "cf.", "aff.", "subspecies in question"');
  t('An identifying string applied to a set of material that resulted from a single collecting event.');
  t('An identifying string applied to the object or observation at the time of collection. Serves as a link between field notes and the object or observation.');
  t('Basis of record');
  t('Catalogue number');
  t('Collection code');
  t('Collector');
  t('Collector number');
  t('Comments about the spatial description determination, explaining assumptions made in addition or opposition to the those formalized in the method referred to in GeoreferenceProtocol.');
  t('Continent/Ocean');
  t('Coordinate uncertainty in meters');
  t('Count');
  t('Country');
  t('County');
  t('Date collected');
  t('Date identified');
  t('Field notes');
  t('Field number');
  t('GenBank number(s)');
  t('General comments about the specimen that others may find useful.');
  t('Geodetic datum');
  t('Georeference protocol');
  t('Georeference remarks');
  t('Identification qualifier');
  t('Identified by');
  t('Institution code');
  t('Island');
  t('Lifestage');
  t('Locality');
  t('Location');
  t('Map');
  t('Maximum depth');
  t('Maximum elevation');
  t('Minimum depth');
  t('Minimum elevation');
  t('Notes taken in the field for the catalogued item.');
  t('Other catalogue numbers');
  t('Remarks');
  t('Select a location that this specimen was collected from.');
  t('Sex');
  t('State/Province');
  t('Taxonomic name');
  t('The age class or life stage of the biological individual represented by the catalogued object or observation. Examples: "adult", "mature", "juvenile", "eft", "nymph", "seedling", "seed", "egg"');
  t('The catalogue number/identifier for this specimen');
  t('The code (or acronym) identifying the collection within the institution in which the organism record is catalogued.');
  t('The code (or acronym) identifying the institution administering the collection in which the organism record is cataloged. No global registry exists for institutional codes; use the code that is "standard" at your institution or in your discipline.');
  t('The date in the Common Era calendar in which the object or observation was identified as being a member of the taxon given in the Scientific Name.  If dates are uncertain, use a range of dates (i.e. if you know the specimen was identified in 1982, use 1982/1/1 to 1982/12/31).');
  t('The earliest and latest date-time (Common Era calendar) in a date-time period during which an organism or group of organisms was collected or observed. If the event is recorded as occurring at a single date-time, populate only the  Earliest Date Collected field');
  t('The full name of the lowest level taxon to which the organism has been identified in the most recent accepted determination, specified as precisely as possible');
  t('The full, unabbreviated name of the county, shire, or municipality (i.e., the next smaller political region than StateProvince) from which the catalogued item was collected.');
  t('The full, unabbreviated name of the island from which the catalogued item was collected.');
  t('The full, unabbreviated name of the island group from which the catalogued item was collected.');
  t('The full, unabbreviated name of the state, province, or region (i.e., the next smaller political region than Country) from which the catalogued item was collected.');
  t('The geodetic datum to which the latitude and longitude refer. If not known, use "not recorded".');
  t('The maximum distance in meters above (positive) or below sea level of the collecting locality.');
  t('The maximum or actual depth at which the collection or observation was made. Use positive values for locations below the surface. Examples: 10 (for a depth of up to 10m). 100 (for a depth between 50m and 100m).');
  t('The minimum distance in meters above (positive) or below sea level of the collecting locality.');
  t('The minimum or actual depth at which the collection or observation was made. Use positive values for locations below the surface. Examples: 0 (for a depth of up to 10m). 50 (for a depth between 50m and 100m).');
  t('The name of the country or major administrative unit in which the Location occurs. Recommended best practice is to use a controlled vocabulary such as the Getty Thesaurus of Geographic Names.');
  t('The name of the system in which the verbatim geographic coordinates were recorded. Examples: "decimal degrees", "degrees minutes seconds", "degrees decimal minutes", "UTM"');
  t('The name(s) of the collector(s) of the original data for the cataloged item.');
  t('The name(s) of the person(s) who applied the ScientificName to the object or observation. Example: James Earl Jones.');
  t('The number of individuals present in the lot or container. Not to be used for observations.');
  t('Type status');
  t('Verbatim Coordinate System');
  return $fields;
}
