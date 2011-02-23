<?php

/**
 * @file
 * 
 * The file defines two simple functions which store the fields associated with
 * the two different content types "Specimen/Observation" & "Location"
 */
/**
 * Returns the fields associated with the "Specimen/Observation" content type.
 * 
 * Note, the following field definitions were created using the standard fields
 * module, and then exported using the features module.
 */
/*
      'islandgroup' => array('type' => 'varchar', 'length' => 255),
      'island' => array('type' => 'varchar', 'length' => 255),
      'stateprovince' => array('type' => 'varchar', 'length' => 255),
      'county' => array('type' => 'varchar', 'length' => 255),
      'locality' => array('type' => 'varchar', 'length' => 255),
      'geodeticdatum' => array('type' => 'varchar', 'length' => 255),
      'verbatimcoordinatesystem' => array('type' => 'varchar', 'length' => 255),
      'georeferenceprotocol' => array('type' => 'varchar', 'length' => 255),
      'coordinateuncertainty' => array('type' => 'varchar', 'length' => 255),
      'georeferenceremarks' => array('type' => 'varchar', 'length' => 255),
      'minelevation' => array('type' => 'int', 'length' => 5),
      'maxelevation' => array('type' => 'int', 'length' => 5),
      'mindepth' => array('type' => 'int', 'length' => 5),
      'maxdepth' => array('type' => 'int', 'length' => 5)
 */
function darwincore_field_default_fields(){
  return array(
    'node-specimen_observation-field_basis_of_record' => array(
      'field_config' => array(
        'field_name' => 'field_basis_of_record',
        'indexes' => array(
          'value' => array(
            '0' => 'value'
          )
        ),
        'module' => 'list',
        'settings' => array(
          'allowed_values' => array(
            'Fossil Specimen' => 'Fossil Specimen',
            'Human Observation' => 'Human Observation',
            'Living Specimen' => 'Living Specimen',
            'Machine Observation' => 'Machine Observation',
            'Moving Image' => 'Moving Image',
            'Other Specimen' => 'Other Specimen',
            'Preserved Specimen' => 'Preserved Specimen',
            'Sound Recording' => 'Sound Recording',
            'Still Image' => 'Still Image'
          ),
          'allowed_values_function' => ''
        ),
        'type' => 'list_text'
      ),
      'field_instance' => array(
        'bundle' => 'specimen_observation',
        'default_value' => NULL,
        'description' => '',
        'display' => array(
          'default' => array(
            'label' => 'above',
            'module' => 'list',
            'settings' => array(),
            'type' => 'list_default',
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
        'field_name' => 'field_basis_of_record',
        'label' => 'Basis of record',
        'required' => 0,
        'settings' => array(
          'user_register_form' => FALSE
        ),
        'widget' => array(
          'module' => 'options',
          'settings' => array(),
          'type' => 'options_select',
          'weight' => '-2'
        )
      )
    ),
    'node-specimen_observation-field_catalogue_number' => array(
      'field_config' => array(
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
            '0' => 'format'
          )
        ),
        'module' => 'text',
        'settings' => array(
          'max_length' => '255'
        ),
        'type' => 'text'
      ),
      'field_instance' => array(
        'bundle' => 'specimen_observation',
        'default_value' => NULL,
        'description' => '',
        'display' => array(
          'default' => array(
            'label' => 'above',
            'module' => 'text',
            'settings' => array(),
            'type' => 'text_default',
            'weight' => 6
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
        'required' => 0,
        'settings' => array(
          'text_processing' => '0',
          'user_register_form' => FALSE
        ),
        'widget' => array(
          'module' => 'text',
          'settings' => array(
            'size' => '60'
          ),
          'type' => 'text_textfield',
          'weight' => '2'
        )
      )
    ),
    'node-specimen_observation-field_collection_code' => array(
      'field_config' => array(
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
            '0' => 'format'
          )
        ),
        'module' => 'text',
        'settings' => array(
          'max_length' => '255'
        ),
        'type' => 'text'
      ),
      'field_instance' => array(
        'bundle' => 'specimen_observation',
        'default_value' => NULL,
        'description' => '',
        'display' => array(
          'default' => array(
            'label' => 'above',
            'module' => 'text',
            'settings' => array(),
            'type' => 'text_default',
            'weight' => 5
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
        'required' => 0,
        'settings' => array(
          'text_processing' => '0',
          'user_register_form' => FALSE
        ),
        'widget' => array(
          'module' => 'text',
          'settings' => array(
            'size' => '60'
          ),
          'type' => 'text_textfield',
          'weight' => '1'
        )
      )
    ),
    'node-specimen_observation-field_collector' => array(
      'field_config' => array(
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
            '0' => 'uid'
          )
        ),
        'module' => 'user_reference',
        'settings' => array(
          'referenceable_roles' => array(
            '2' => 0,
            '3' => 0
          ),
          'referenceable_status' => array(
            '0' => 0,
            '1' => 0
          )
        ),
        'type' => 'user_reference'
      ),
      'field_instance' => array(
        'bundle' => 'specimen_observation',
        'default_value' => NULL,
        'description' => '',
        'display' => array(
          'default' => array(
            'label' => 'above',
            'module' => 'user_reference',
            'settings' => array(),
            'type' => 'user_reference_default',
            'weight' => 14
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
          'module' => 'options',
          'settings' => array(),
          'type' => 'options_select',
          'weight' => '10'
        )
      )
    ),
    'node-specimen_observation-field_collector_number' => array(
      'field_config' => array(
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
            '0' => 'format'
          )
        ),
        'module' => 'text',
        'settings' => array(
          'max_length' => '255'
        ),
        'type' => 'text'
      ),
      'field_instance' => array(
        'bundle' => 'specimen_observation',
        'default_value' => NULL,
        'description' => '',
        'display' => array(
          'default' => array(
            'label' => 'above',
            'module' => 'text',
            'settings' => array(),
            'type' => 'text_default',
            'weight' => 12
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
          'module' => 'text',
          'settings' => array(
            'size' => '60'
          ),
          'type' => 'text_textfield',
          'weight' => '8'
        )
      )
    ),
    'node-specimen_observation-field_count' => array(
      'field_config' => array(
        'field_name' => 'field_count',
        'indexes' => array(),
        'module' => 'number',
        'settings' => array(),
        'type' => 'number_integer'
      ),
      'field_instance' => array(
        'bundle' => 'specimen_observation',
        'default_value' => array(
          '0' => array(
            'value' => '1'
          )
        ),
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
            'weight' => 19
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
          'module' => 'number',
          'settings' => array(),
          'type' => 'number',
          'weight' => '15'
        )
      )
    ),
    'node-specimen_observation-field_date_collected' => array(
      'field_config' => array(
        'field_name' => 'field_date_collected',
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
        'type' => 'date'
      ),
      'field_instance' => array(
        'bundle' => 'specimen_observation',
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
            'weight' => 15
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
          'default_value' => 'now',
          'default_value2' => 'blank',
          'default_value_code' => '',
          'default_value_code2' => '',
          'user_register_form' => FALSE
        ),
        'widget' => array(
          'module' => 'date',
          'settings' => array(
            'increment' => '1',
            'input_format' => 'm/d/Y - H:i:s',
            'input_format_custom' => '',
            'label_position' => 'above',
            'repeat_collapsed' => 0,
            'text_parts' => array(),
            'year_range' => '-3:+3'
          ),
          'type' => 'date_popup',
          'weight' => '11'
        )
      )
    ),
    'node-specimen_observation-field_date_identified_day' => array(
      'field_config' => array(
        'field_name' => 'field_date_identified_day',
        'indexes' => array(
          'value' => array(
            '0' => 'value'
          )
        ),
        'module' => 'list',
        'settings' => array(
          'allowed_values' => array(
            '1' => '1',
            '2' => '2',
            '3' => '3',
            '4' => '4',
            '5' => '5',
            '6' => '6',
            '7' => '7',
            '8' => '8',
            '9' => '9',
            '10' => '10',
            '11' => '11',
            '12' => '12',
            '13' => '13',
            '14' => '14',
            '15' => '15',
            '16' => '16',
            '17' => '17',
            '18' => '18',
            '19' => '19',
            '20' => '20',
            '21' => '21',
            '22' => '22',
            '23' => '23',
            '24' => '24',
            '25' => '25',
            '26' => '26',
            '27' => '27',
            '28' => '28',
            '29' => '29',
            '30' => '30',
            '31' => '31'
          ),
          'allowed_values_function' => ''
        ),
        'type' => 'list_integer'
      ),
      'field_instance' => array(
        'bundle' => 'specimen_observation',
        'default_value' => NULL,
        'description' => '',
        'display' => array(
          'default' => array(
            'label' => 'above',
            'module' => 'list',
            'settings' => array(),
            'type' => 'list_default',
            'weight' => 11
          ),
          'teaser' => array(
            'label' => 'above',
            'settings' => array(),
            'type' => 'hidden',
            'weight' => 0
          )
        ),
        'entity_type' => 'node',
        'field_name' => 'field_date_identified_day',
        'label' => 'Day',
        'required' => 0,
        'settings' => array(
          'user_register_form' => FALSE
        ),
        'widget' => array(
          'module' => 'options',
          'settings' => array(),
          'type' => 'options_select',
          'weight' => '7'
        )
      )
    ),
    'node-specimen_observation-field_date_identified_month' => array(
      'field_config' => array(
        'field_name' => 'field_date_identified_month',
        'indexes' => array(
          'value' => array(
            '0' => 'value'
          )
        ),
        'module' => 'list',
        'settings' => array(
          'allowed_values' => array(
            '1' => 'January',
            '2' => 'February',
            '3' => 'March',
            '4' => 'April',
            '5' => 'May',
            '6' => 'June',
            '7' => 'July',
            '8' => 'August',
            '9' => 'September',
            '10' => 'October',
            '11' => 'November',
            '12' => 'December'
          ),
          'allowed_values_function' => ''
        ),
        'type' => 'list_integer'
      ),
      'field_instance' => array(
        'bundle' => 'specimen_observation',
        'default_value' => NULL,
        'description' => '',
        'display' => array(
          'default' => array(
            'label' => 'above',
            'module' => 'list',
            'settings' => array(),
            'type' => 'list_default',
            'weight' => 10
          ),
          'teaser' => array(
            'label' => 'above',
            'settings' => array(),
            'type' => 'hidden',
            'weight' => 0
          )
        ),
        'entity_type' => 'node',
        'field_name' => 'field_date_identified_month',
        'label' => 'Month',
        'required' => 0,
        'settings' => array(
          'user_register_form' => FALSE
        ),
        'widget' => array(
          'module' => 'options',
          'settings' => array(),
          'type' => 'options_select',
          'weight' => '6'
        )
      )
    ),
    'node-specimen_observation-field_date_identified_year' => array(
      'field_config' => array(
        'field_name' => 'field_date_identified_year',
        'indexes' => array(
          'value' => array(
            '0' => 'value'
          )
        ),
        'module' => 'list',
        'settings' => array(
          'allowed_values' => array(
            '2009' => '2009',
            '2010' => '2010',
            '2011' => '2011'
          ),
          'allowed_values_function' => ''
        ),
        'type' => 'list_integer'
      ),
      'field_instance' => array(
        'bundle' => 'specimen_observation',
        'default_value' => NULL,
        'description' => '',
        'display' => array(
          'default' => array(
            'label' => 'above',
            'module' => 'list',
            'settings' => array(),
            'type' => 'list_default',
            'weight' => 9
          ),
          'teaser' => array(
            'label' => 'above',
            'settings' => array(),
            'type' => 'hidden',
            'weight' => 0
          )
        ),
        'entity_type' => 'node',
        'field_name' => 'field_date_identified_year',
        'label' => 'Year',
        'required' => 0,
        'settings' => array(
          'user_register_form' => FALSE
        ),
        'widget' => array(
          'module' => 'options',
          'settings' => array(),
          'type' => 'options_select',
          'weight' => '5'
        )
      )
    ),
    'node-specimen_observation-field_genbank_number' => array(
      'field_config' => array(
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
            '0' => 'format'
          )
        ),
        'module' => 'text',
        'settings' => array(
          'max_length' => '255'
        ),
        'type' => 'text'
      ),
      'field_instance' => array(
        'bundle' => 'specimen_observation',
        'default_value' => NULL,
        'description' => '',
        'display' => array(
          'default' => array(
            'label' => 'above',
            'module' => 'text',
            'settings' => array(),
            'type' => 'text_default',
            'weight' => 20
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
        'label' => 'GenBank number',
        'required' => 0,
        'settings' => array(
          'text_processing' => '0',
          'user_register_form' => FALSE
        ),
        'widget' => array(
          'module' => 'text',
          'settings' => array(
            'size' => '60'
          ),
          'type' => 'text_textfield',
          'weight' => '16'
        )
      )
    ),
    'node-specimen_observation-field_identification_qualifier' => array(
      'field_config' => array(
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
            '0' => 'format'
          )
        ),
        'module' => 'text',
        'settings' => array(
          'max_length' => '255'
        ),
        'type' => 'text'
      ),
      'field_instance' => array(
        'bundle' => 'specimen_observation',
        'default_value' => NULL,
        'description' => '',
        'display' => array(
          'default' => array(
            'label' => 'above',
            'module' => 'text',
            'settings' => array(),
            'type' => 'text_default',
            'weight' => 7
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
          'module' => 'text',
          'settings' => array(
            'size' => '60'
          ),
          'type' => 'text_textfield',
          'weight' => '3'
        )
      )
    ),
    'node-specimen_observation-field_identified_by' => array(
      'field_config' => array(
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
            '0' => 'uid'
          )
        ),
        'module' => 'user_reference',
        'settings' => array(
          'referenceable_roles' => array(
            '2' => 0,
            '3' => 0
          ),
          'referenceable_status' => array(
            '0' => 0,
            '1' => 0
          )
        ),
        'type' => 'user_reference'
      ),
      'field_instance' => array(
        'bundle' => 'specimen_observation',
        'default_value' => NULL,
        'description' => '',
        'display' => array(
          'default' => array(
            'label' => 'above',
            'module' => 'user_reference',
            'settings' => array(),
            'type' => 'user_reference_default',
            'weight' => 8
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
          'module' => 'options',
          'settings' => array(),
          'type' => 'options_select',
          'weight' => '4'
        )
      )
    ),
    'node-specimen_observation-field_institution_code' => array(
      'field_config' => array(
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
            '0' => 'format'
          )
        ),
        'module' => 'text',
        'settings' => array(
          'max_length' => '255'
        ),
        'type' => 'text'
      ),
      'field_instance' => array(
        'bundle' => 'specimen_observation',
        'default_value' => NULL,
        'description' => '',
        'display' => array(
          'default' => array(
            'label' => 'above',
            'module' => 'text',
            'settings' => array(),
            'type' => 'text_default',
            'weight' => 4
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
        'required' => 0,
        'settings' => array(
          'text_processing' => '0',
          'user_register_form' => FALSE
        ),
        'widget' => array(
          'module' => 'text',
          'settings' => array(
            'size' => '60'
          ),
          'type' => 'text_textfield',
          'weight' => '0'
        )
      )
    ),
    'node-specimen_observation-field_lifestage' => array(
      'field_config' => array(
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
            '0' => 'format'
          )
        ),
        'module' => 'text',
        'settings' => array(
          'max_length' => '255'
        ),
        'type' => 'text'
      ),
      'field_instance' => array(
        'bundle' => 'specimen_observation',
        'default_value' => NULL,
        'description' => '',
        'display' => array(
          'default' => array(
            'label' => 'above',
            'module' => 'text',
            'settings' => array(),
            'type' => 'text_default',
            'weight' => 18
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
          'module' => 'text',
          'settings' => array(
            'size' => '60'
          ),
          'type' => 'text_textfield',
          'weight' => '14'
        )
      )
    ),
    'node-specimen_observation-field_notes' => array(
      'field_config' => array(
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
            '0' => 'format'
          )
        ),
        'module' => 'text',
        'settings' => array(),
        'type' => 'text_long'
      ),
      'field_instance' => array(
        'bundle' => 'specimen_observation',
        'default_value' => NULL,
        'description' => '',
        'display' => array(
          'default' => array(
            'label' => 'above',
            'module' => 'text',
            'settings' => array(),
            'type' => 'text_default',
            'weight' => 16
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
          'module' => 'text',
          'settings' => array(
            'rows' => '5'
          ),
          'type' => 'text_textarea',
          'weight' => '12'
        )
      )
    ),
    'node-specimen_observation-field_number' => array(
      'field_config' => array(
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
            '0' => 'format'
          )
        ),
        'module' => 'text',
        'settings' => array(
          'max_length' => '255'
        ),
        'type' => 'text'
      ),
      'field_instance' => array(
        'bundle' => 'specimen_observation',
        'default_value' => NULL,
        'description' => '',
        'display' => array(
          'default' => array(
            'label' => 'above',
            'module' => 'text',
            'settings' => array(),
            'type' => 'text_default',
            'weight' => 13
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
          'module' => 'text',
          'settings' => array(
            'size' => '60'
          ),
          'type' => 'text_textfield',
          'weight' => '9'
        )
      )
    ),
    'node-specimen_observation-field_other_catalogue_numbers' => array(
      'field_config' => array(
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
            '0' => 'format'
          )
        ),
        'module' => 'text',
        'settings' => array(),
        'type' => 'text_long'
      ),
      'field_instance' => array(
        'bundle' => 'specimen_observation',
        'default_value' => NULL,
        'description' => '',
        'display' => array(
          'default' => array(
            'label' => 'above',
            'module' => 'text',
            'settings' => array(),
            'type' => 'text_default',
            'weight' => 17
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
          'module' => 'text',
          'settings' => array(
            'rows' => '5'
          ),
          'type' => 'text_textarea',
          'weight' => '13'
        )
      )
    ),
    'node-specimen_observation-field_remarks' => array(
      'field_config' => array(
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
            '0' => 'format'
          )
        ),
        'module' => 'text',
        'settings' => array(),
        'type' => 'text_long'
      ),
      'field_instance' => array(
        'bundle' => 'specimen_observation',
        'default_value' => NULL,
        'description' => '',
        'display' => array(
          'default' => array(
            'label' => 'above',
            'module' => 'text',
            'settings' => array(),
            'type' => 'text_default',
            'weight' => 21
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
          'text_processing' => '0',
          'user_register_form' => FALSE
        ),
        'widget' => array(
          'module' => 'text',
          'settings' => array(
            'rows' => '5'
          ),
          'type' => 'text_textarea',
          'weight' => '17'
        )
      )
    ),
    'node-specimen_observation-field_sex' => array(
      'field_config' => array(
        'field_name' => 'field_sex',
        'indexes' => array(
          'value' => array(
            '0' => 'value'
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
        'type' => 'list_text'
      ),
      'field_instance' => array(
        'bundle' => 'specimen_observation',
        'default_value' => NULL,
        'description' => '',
        'display' => array(
          'default' => array(
            'label' => 'above',
            'module' => 'list',
            'settings' => array(),
            'type' => 'list_default',
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
        'field_name' => 'field_sex',
        'label' => 'Sex',
        'required' => 0,
        'settings' => array(
          'user_register_form' => FALSE
        ),
        'widget' => array(
          'module' => 'options',
          'settings' => array(),
          'type' => 'options_select',
          'weight' => '-3'
        )
      )
    ),
    'node-specimen_observation-field_type_status' => array(
      'field_config' => array(
        'field_name' => 'field_type_status',
        'indexes' => array(
          'value' => array(
            '0' => 'value'
          )
        ),
        'module' => 'list',
        'settings' => array(
          'allowed_values' => array(
            'Holotype' => 'Holotype',
            'Isotype' => 'Isotype',
            'Lectotype' => 'Lectotype',
            'Neotype' => 'Neotype',
            'Nontype' => 'Nontype',
            'Not Provided' => 'Not Provided',
            'Paralectotype' => 'Paralectotype',
            'Paratype' => 'Paratype',
            'Syntype' => 'Syntype'
          ),
          'allowed_values_function' => ''
        ),
        'type' => 'list_text'
      ),
      'field_instance' => array(
        'bundle' => 'specimen_observation',
        'default_value' => NULL,
        'description' => '',
        'display' => array(
          'default' => array(
            'label' => 'above',
            'module' => 'list',
            'settings' => array(),
            'type' => 'list_default',
            'weight' => 3
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
          'module' => 'options',
          'settings' => array(),
          'type' => 'options_select',
          'weight' => '-1'
        )
      )
    )
  );
}

/**
 * Returns the fields associated with the "Location" content type.
 */
function darwincorelocation_fields(){
  return array();
}