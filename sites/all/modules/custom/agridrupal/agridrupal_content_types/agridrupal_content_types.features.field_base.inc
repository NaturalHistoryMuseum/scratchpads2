<?php
/**
 * @file
 * agridrupal_content_types.features.field_base.inc
 */

/**
 * Implements hook_field_default_field_bases().
 */
function agridrupal_content_types_field_default_field_bases() {
  $field_bases = array();

  // Exported field_base: 'field_ag_abstract'
  $field_bases['field_ag_abstract'] = array(
    'active' => 1,
    'cardinality' => -1,
    'deleted' => 0,
    'entity_types' => array(),
    'field_name' => 'field_ag_abstract',
    'field_permissions' => array(
      'type' => 0,
    ),
    'foreign keys' => array(
      'format' => array(
        'columns' => array(
          'format' => 'format',
        ),
        'table' => 'filter_format',
      ),
    ),
    'indexes' => array(
      'format' => array(
        0 => 'format',
      ),
    ),
    'locked' => 0,
    'module' => 'text',
    'settings' => array(),
    'translatable' => 0,
    'type' => 'text_long',
  );

  // Exported field_base: 'field_ag_acces'
  $field_bases['field_ag_acces'] = array(
    'active' => 1,
    'cardinality' => 1,
    'deleted' => 0,
    'entity_types' => array(),
    'field_name' => 'field_ag_acces',
    'field_permissions' => array(
      'type' => 0,
    ),
    'foreign keys' => array(
      'tid' => array(
        'columns' => array(
          'tid' => 'tid',
        ),
        'table' => 'taxonomy_term_data',
      ),
    ),
    'indexes' => array(
      'tid' => array(
        0 => 'tid',
      ),
    ),
    'locked' => 0,
    'module' => 'taxonomy',
    'settings' => array(
      'allowed_values' => array(
        0 => array(
          'vocabulary' => 'vocabulary_5',
          'parent' => 0,
          'depth' => '',
        ),
      ),
      'options_list_callback' => 'i18n_taxonomy_allowed_values',
    ),
    'translatable' => 0,
    'type' => 'taxonomy_term_reference',
  );

  // Exported field_base: 'field_ag_access'
  $field_bases['field_ag_access'] = array(
    'active' => 1,
    'cardinality' => 1,
    'deleted' => 0,
    'entity_types' => array(),
    'field_name' => 'field_ag_access',
    'field_permissions' => array(
      'type' => 0,
    ),
    'foreign keys' => array(),
    'indexes' => array(
      'value' => array(
        0 => 'value',
      ),
    ),
    'locked' => 0,
    'module' => 'list',
    'settings' => array(
      'allowed_values' => array(
        0 => 'Not our Copyright',
        1 => 'Not on our behalf',
        2 => 'Our Copyright',
      ),
      'allowed_values_function' => '',
      'allowed_values_php' => '',
      'text_processing' => 0,
    ),
    'translatable' => 0,
    'type' => 'list_text',
    'type_name' => 'agdlios',
  );

  // Exported field_base: 'field_ag_acronym'
  $field_bases['field_ag_acronym'] = array(
    'active' => 1,
    'cardinality' => 1,
    'deleted' => 0,
    'entity_types' => array(),
    'field_name' => 'field_ag_acronym',
    'foreign keys' => array(
      'format' => array(
        'columns' => array(
          'format' => 'format',
        ),
        'table' => 'filter_format',
      ),
    ),
    'indexes' => array(
      'format' => array(
        0 => 'format',
      ),
    ),
    'locked' => 0,
    'module' => 'text',
    'settings' => array(
      'allowed_values' => '',
      'allowed_values_php' => '',
      'max_length' => 255,
      'text_processing' => 0,
    ),
    'translatable' => 0,
    'type' => 'text',
    'type_name' => 'aginstitution',
  );

  // Exported field_base: 'field_ag_address'
  $field_bases['field_ag_address'] = array(
    'active' => 1,
    'cardinality' => 1,
    'deleted' => 0,
    'entity_types' => array(),
    'field_name' => 'field_ag_address',
    'foreign keys' => array(
      'format' => array(
        'columns' => array(
          'format' => 'format',
        ),
        'table' => 'filter_format',
      ),
    ),
    'indexes' => array(
      'format' => array(
        0 => 'format',
      ),
    ),
    'locked' => 0,
    'module' => 'text',
    'settings' => array(
      'allowed_values' => '',
      'allowed_values_php' => '',
      'max_length' => '',
      'text_processing' => 0,
    ),
    'translatable' => 0,
    'type' => 'text_long',
    'type_name' => 'agprofile',
  );

  // Exported field_base: 'field_ag_agris'
  $field_bases['field_ag_agris'] = array(
    'active' => 1,
    'cardinality' => -1,
    'deleted' => 0,
    'entity_types' => array(),
    'field_name' => 'field_ag_agris',
    'field_permissions' => array(
      'type' => 0,
    ),
    'foreign keys' => array(
      'tid' => array(
        'columns' => array(
          'tid' => 'tid',
        ),
        'table' => 'taxonomy_term_data',
      ),
    ),
    'indexes' => array(
      'tid' => array(
        0 => 'tid',
      ),
    ),
    'locked' => 0,
    'module' => 'taxonomy',
    'settings' => array(
      'allowed_values' => array(
        0 => array(
          'vocabulary' => 'agris_subject_categories',
          'parent' => 0,
        ),
      ),
      'options_list_callback' => 'i18n_taxonomy_allowed_values',
    ),
    'translatable' => 0,
    'type' => 'taxonomy_term_reference',
  );

  // Exported field_base: 'field_ag_agrovoc'
  $field_bases['field_ag_agrovoc'] = array(
    'active' => 1,
    'cardinality' => -1,
    'deleted' => 0,
    'entity_types' => array(),
    'field_name' => 'field_ag_agrovoc',
    'field_permissions' => array(
      'type' => 0,
    ),
    'foreign keys' => array(
      'tid' => array(
        'columns' => array(
          'tid' => 'tid',
        ),
        'table' => 'taxonomy_term_data',
      ),
    ),
    'indexes' => array(
      'tid' => array(
        0 => 'tid',
      ),
    ),
    'locked' => 0,
    'module' => 'taxonomy',
    'settings' => array(
      'allowed_values' => array(
        0 => array(
          'vocabulary' => 'vocabulary_16',
          'parent' => 0,
        ),
        1 => array(
          'parent' => 0,
          'vocabulary' => 'vocabulary_16',
        ),
      ),
      'options_list_callback' => 'i18n_taxonomy_allowed_values',
    ),
    'translatable' => 0,
    'type' => 'taxonomy_term_reference',
  );

  // Exported field_base: 'field_ag_aproxdatepub'
  $field_bases['field_ag_aproxdatepub'] = array(
    'active' => 1,
    'cardinality' => 1,
    'deleted' => 0,
    'entity_types' => array(),
    'field_name' => 'field_ag_aproxdatepub',
    'field_permissions' => array(
      'type' => 0,
    ),
    'foreign keys' => array(
      'format' => array(
        'columns' => array(
          'format' => 'format',
        ),
        'table' => 'filter_format',
      ),
    ),
    'indexes' => array(
      'format' => array(
        0 => 'format',
      ),
    ),
    'locked' => 0,
    'module' => 'text',
    'settings' => array(
      'allowed_values' => '',
      'allowed_values_php' => '',
      'max_length' => 255,
      'text_processing' => 0,
    ),
    'translatable' => 0,
    'type' => 'text',
    'type_name' => 'agdlios',
  );

  // Exported field_base: 'field_ag_auth_order'
  $field_bases['field_ag_auth_order'] = array(
    'active' => 1,
    'cardinality' => -1,
    'deleted' => 0,
    'entity_types' => array(),
    'field_name' => 'field_ag_auth_order',
    'field_permissions' => array(
      'type' => 0,
    ),
    'foreign keys' => array(),
    'indexes' => array(),
    'locked' => 0,
    'module' => 'number',
    'settings' => array(),
    'translatable' => 0,
    'type' => 'number_integer',
  );

  // Exported field_base: 'field_ag_author'
  $field_bases['field_ag_author'] = array(
    'active' => 1,
    'cardinality' => -1,
    'deleted' => 0,
    'entity_types' => array(),
    'field_name' => 'field_ag_author',
    'field_permissions' => array(
      'type' => 0,
    ),
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
    'locked' => 0,
    'module' => 'node_reference',
    'settings' => array(
      'referenceable_types' => array(
        'ag_job_feed' => 0,
        'agauthor' => 'agauthor',
        'agconference' => 0,
        'agcorporate_body' => 0,
        'agdlios' => 0,
        'agevent' => 0,
        'ageventsfeed' => 0,
        'aginstitution' => 0,
        'agnews' => 0,
        'agnewsfeed' => 0,
        'agperiodical' => 0,
        'agprofile' => 0,
        'agrepository' => 0,
        'agris_authors_feed' => 0,
        'agris_authors_upload' => 0,
        'agris_conferences_feed' => 0,
        'agris_corporate_feed' => 0,
        'agris_dlio_upload' => 0,
        'agris_journals_feed' => 0,
        'agris_publishers_feed' => 0,
        'agrssevents' => 0,
        'agrssjob' => 0,
        'agrssnews' => 0,
        'agvacancy' => 0,
        'blog' => 0,
        'books_feed' => 0,
        'feed' => 0,
        'feed_fast' => 0,
        'feed_item' => 0,
        'forum' => 0,
        'group' => 0,
        'page' => 0,
        'story' => 0,
        'xmlimport_dlios' => 0,
      ),
      'view' => array(
        'args' => array(),
        'display_name' => 'references_1',
        'view_name' => 'refauthors',
      ),
    ),
    'translatable' => 0,
    'type' => 'node_reference',
    'type_name' => 'agdlios',
  );

  // Exported field_base: 'field_ag_book'
  $field_bases['field_ag_book'] = array(
    'active' => 1,
    'cardinality' => 1,
    'deleted' => 0,
    'entity_types' => array(),
    'field_name' => 'field_ag_book',
    'field_permissions' => array(
      'type' => 0,
    ),
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
    'locked' => 0,
    'module' => 'node_reference',
    'settings' => array(
      'referenceable_types' => array(
        'ag_job_feed' => 0,
        'agauthor' => 0,
        'agconference' => 0,
        'agcorporate_body' => 0,
        'agdlios' => 'agdlios',
        'agevent' => 0,
        'ageventsfeed' => 0,
        'aginstitution' => 0,
        'agnews' => 0,
        'agnewsfeed' => 0,
        'agprofile' => 0,
        'agrepository' => 0,
        'agris_authors_feed' => 0,
        'agris_authors_upload' => 0,
        'agris_conferences_feed' => 0,
        'agris_corporate_feed' => 0,
        'agris_dlio_upload' => 0,
        'agris_journals_feed' => 0,
        'agris_publishers_feed' => 0,
        'agrssevents' => 0,
        'agrssjob' => 0,
        'agrssnews' => 0,
        'agvacancy' => 0,
        'blog' => 0,
        'books_feed' => 0,
        'feed' => 0,
        'feed_fast' => 0,
        'feed_item' => 0,
        'forum' => 0,
        'group' => 0,
        'page' => 0,
        'story' => 0,
        'xmlimport_dlios' => 0,
      ),
      'view' => array(
        'args' => array(),
        'display_name' => 'references_1',
        'view_name' => 'refauthors',
      ),
    ),
    'translatable' => 0,
    'type' => 'node_reference',
    'type_name' => 'agdlios',
  );

  // Exported field_base: 'field_ag_booknumber'
  $field_bases['field_ag_booknumber'] = array(
    'active' => 1,
    'cardinality' => 1,
    'deleted' => 0,
    'entity_types' => array(),
    'field_name' => 'field_ag_booknumber',
    'field_permissions' => array(
      'type' => 0,
    ),
    'foreign keys' => array(
      'format' => array(
        'columns' => array(
          'format' => 'format',
        ),
        'table' => 'filter_format',
      ),
    ),
    'indexes' => array(
      'format' => array(
        0 => 'format',
      ),
    ),
    'locked' => 0,
    'module' => 'text',
    'settings' => array(
      'allowed_values' => '',
      'allowed_values_php' => '',
      'max_length' => 255,
      'text_processing' => 0,
    ),
    'translatable' => 0,
    'type' => 'text',
    'type_name' => 'agdlios',
  );

  // Exported field_base: 'field_ag_chkdatepub'
  $field_bases['field_ag_chkdatepub'] = array(
    'active' => 1,
    'cardinality' => 1,
    'deleted' => 0,
    'entity_types' => array(),
    'field_name' => 'field_ag_chkdatepub',
    'field_permissions' => array(
      'type' => 0,
    ),
    'foreign keys' => array(),
    'indexes' => array(
      'value' => array(
        0 => 'value',
      ),
    ),
    'locked' => 0,
    'module' => 'list',
    'settings' => array(
      'allowed_values' => array(
        0 => 'Yes',
      ),
      'allowed_values_function' => '',
      'allowed_values_php' => '',
      'max' => '',
      'min' => '',
      'prefix' => '',
      'suffix' => '',
    ),
    'translatable' => 0,
    'type' => 'list_integer',
    'type_name' => 'agdlios',
  );

  // Exported field_base: 'field_ag_confdate'
  $field_bases['field_ag_confdate'] = array(
    'active' => 1,
    'cardinality' => 1,
    'deleted' => 0,
    'entity_types' => array(),
    'field_name' => 'field_ag_confdate',
    'foreign keys' => array(),
    'indexes' => array(),
    'locked' => 0,
    'module' => 'date',
    'settings' => array(
      'granularity' => array(
        'day' => 'day',
        'month' => 'month',
        'year' => 'year',
      ),
      'repeat' => 0,
      'timezone_db' => 'Europe/Paris',
      'todate' => 'optional',
      'tz_handling' => 'none',
    ),
    'translatable' => 0,
    'type' => 'date',
    'type_name' => 'agconference',
  );

  // Exported field_base: 'field_ag_conflocation'
  $field_bases['field_ag_conflocation'] = array(
    'active' => 1,
    'cardinality' => 1,
    'deleted' => 0,
    'entity_types' => array(),
    'field_name' => 'field_ag_conflocation',
    'foreign keys' => array(
      'format' => array(
        'columns' => array(
          'format' => 'format',
        ),
        'table' => 'filter_format',
      ),
    ),
    'indexes' => array(
      'format' => array(
        0 => 'format',
      ),
    ),
    'locked' => 0,
    'module' => 'text',
    'settings' => array(
      'allowed_values' => '',
      'allowed_values_php' => '',
      'max_length' => 255,
      'text_processing' => 0,
    ),
    'translatable' => 0,
    'type' => 'text',
    'type_name' => 'agconference',
  );

  // Exported field_base: 'field_ag_confname'
  $field_bases['field_ag_confname'] = array(
    'active' => 1,
    'cardinality' => 1,
    'deleted' => 0,
    'entity_types' => array(),
    'field_name' => 'field_ag_confname',
    'field_permissions' => array(
      'type' => 0,
    ),
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
    'locked' => 0,
    'module' => 'node_reference',
    'settings' => array(
      'referenceable_types' => array(
        'ag_job_feed' => 0,
        'agauthor' => 0,
        'agconference' => 'agconference',
        'agcorporate_body' => 0,
        'agdlios' => 0,
        'agevent' => 0,
        'ageventsfeed' => 0,
        'aginstitution' => 0,
        'agnews' => 0,
        'agnewsfeed' => 0,
        'agprofile' => 0,
        'agrepository' => 0,
        'agris_authors_feed' => 0,
        'agris_authors_upload' => 0,
        'agris_conferences_feed' => 0,
        'agris_corporate_feed' => 0,
        'agris_dlio_upload' => 0,
        'agris_journals_feed' => 0,
        'agris_publishers_feed' => 0,
        'agrssevents' => 0,
        'agrssjob' => 0,
        'agrssnews' => 0,
        'agvacancy' => 0,
        'blog' => 0,
        'books_feed' => 0,
        'feed' => 0,
        'feed_fast' => 0,
        'feed_item' => 0,
        'forum' => 0,
        'group' => 0,
        'page' => 0,
        'story' => 0,
        'xmlimport_dlios' => 0,
      ),
      'view' => array(
        'args' => array(),
        'display_name' => '',
        'view_name' => '',
      ),
    ),
    'translatable' => 0,
    'type' => 'node_reference',
    'type_name' => 'agdlios',
  );

  // Exported field_base: 'field_ag_corauthor'
  $field_bases['field_ag_corauthor'] = array(
    'active' => 1,
    'cardinality' => -1,
    'deleted' => 0,
    'entity_types' => array(),
    'field_name' => 'field_ag_corauthor',
    'field_permissions' => array(
      'type' => 0,
    ),
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
    'locked' => 0,
    'module' => 'node_reference',
    'settings' => array(
      'referenceable_types' => array(
        'ag_job_feed' => 0,
        'agauthor' => 0,
        'agconference' => 0,
        'agcorporate_body' => 'agcorporate_body',
        'agdlios' => 0,
        'agevent' => 0,
        'ageventsfeed' => 0,
        'aginstitution' => 0,
        'agnews' => 0,
        'agnewsfeed' => 0,
        'agperiodical' => 0,
        'agprofile' => 0,
        'agrepository' => 0,
        'agris_authors_feed' => 0,
        'agris_authors_upload' => 0,
        'agris_conferences_feed' => 0,
        'agris_corporate_feed' => 0,
        'agris_dlio_upload' => 0,
        'agris_journals_feed' => 0,
        'agris_publishers_feed' => 0,
        'agrssevents' => 0,
        'agrssjob' => 0,
        'agrssnews' => 0,
        'agvacancy' => 0,
        'blog' => 0,
        'books_feed' => 0,
        'feed' => 0,
        'feed_fast' => 0,
        'feed_item' => 0,
        'forum' => 0,
        'group' => 0,
        'page' => 0,
        'story' => 0,
        'xmlimport_dlios' => 0,
      ),
      'view' => array(
        'args' => array(),
        'display_name' => '',
        'view_name' => '',
      ),
    ),
    'translatable' => 0,
    'type' => 'node_reference',
    'type_name' => 'agdlios',
  );

  // Exported field_base: 'field_ag_country'
  $field_bases['field_ag_country'] = array(
    'active' => 1,
    'cardinality' => 1,
    'deleted' => 0,
    'entity_types' => array(),
    'field_name' => 'field_ag_country',
    'field_permissions' => array(
      'type' => 0,
    ),
    'foreign keys' => array(),
    'indexes' => array(),
    'locked' => 0,
    'module' => 'get_list_from_file',
    'settings' => array(
      'file' => 'countries.csv',
    ),
    'translatable' => 0,
    'type' => 'get_list_from_file',
  );

  // Exported field_base: 'field_ag_date'
  $field_bases['field_ag_date'] = array(
    'active' => 1,
    'cardinality' => 1,
    'deleted' => 0,
    'entity_types' => array(),
    'field_name' => 'field_ag_date',
    'foreign keys' => array(),
    'indexes' => array(),
    'locked' => 0,
    'module' => 'date',
    'settings' => array(
      'granularity' => array(
        'day' => 'day',
        'month' => 'month',
        'year' => 'year',
      ),
      'repeat' => 0,
      'timezone_db' => '',
      'todate' => 'required',
      'tz_handling' => 'none',
    ),
    'translatable' => 0,
    'type' => 'date',
    'type_name' => 'agrssevents',
  );

  // Exported field_base: 'field_ag_datepub'
  $field_bases['field_ag_datepub'] = array(
    'active' => 1,
    'cardinality' => 1,
    'deleted' => 0,
    'entity_types' => array(),
    'field_name' => 'field_ag_datepub',
    'field_permissions' => array(
      'type' => 0,
    ),
    'foreign keys' => array(),
    'indexes' => array(),
    'locked' => 0,
    'module' => 'date',
    'settings' => array(
      'cache_count' => 4,
      'cache_enabled' => 0,
      'granularity' => array(
        'day' => 0,
        'hour' => 0,
        'minute' => 0,
        'month' => 'month',
        'second' => 0,
        'year' => 'year',
      ),
      'timezone_db' => '',
      'todate' => '',
      'tz_handling' => 'none',
    ),
    'translatable' => 0,
    'type' => 'datetime',
  );

  // Exported field_base: 'field_ag_deadline'
  $field_bases['field_ag_deadline'] = array(
    'active' => 1,
    'cardinality' => 1,
    'deleted' => 0,
    'entity_types' => array(),
    'field_name' => 'field_ag_deadline',
    'foreign keys' => array(),
    'indexes' => array(),
    'locked' => 0,
    'module' => 'date',
    'settings' => array(
      'granularity' => array(
        'day' => 'day',
        'month' => 'month',
        'year' => 'year',
      ),
      'repeat' => 0,
      'timezone_db' => '',
      'todate' => '',
      'tz_handling' => 'none',
    ),
    'translatable' => 0,
    'type' => 'date',
    'type_name' => 'agvacancy',
  );

  // Exported field_base: 'field_ag_degree'
  $field_bases['field_ag_degree'] = array(
    'active' => 1,
    'cardinality' => 1,
    'deleted' => 0,
    'entity_types' => array(),
    'field_name' => 'field_ag_degree',
    'foreign keys' => array(
      'format' => array(
        'columns' => array(
          'format' => 'format',
        ),
        'table' => 'filter_format',
      ),
    ),
    'indexes' => array(
      'format' => array(
        0 => 'format',
      ),
    ),
    'locked' => 0,
    'module' => 'text',
    'settings' => array(
      'allowed_values' => '',
      'allowed_values_php' => '',
      'max_length' => 255,
      'text_processing' => 0,
    ),
    'translatable' => 0,
    'type' => 'text',
    'type_name' => 'agprofile',
  );

  // Exported field_base: 'field_ag_degree_institution'
  $field_bases['field_ag_degree_institution'] = array(
    'active' => 1,
    'cardinality' => 1,
    'deleted' => 0,
    'entity_types' => array(),
    'field_name' => 'field_ag_degree_institution',
    'field_permissions' => array(
      'type' => 0,
    ),
    'foreign keys' => array(
      'format' => array(
        'columns' => array(
          'format' => 'format',
        ),
        'table' => 'filter_format',
      ),
    ),
    'indexes' => array(
      'format' => array(
        0 => 'format',
      ),
    ),
    'locked' => 0,
    'module' => 'text',
    'settings' => array(
      'allowed_values' => '',
      'allowed_values_php' => '',
      'max_length' => 255,
      'text_processing' => 0,
    ),
    'translatable' => 0,
    'type' => 'text',
    'type_name' => 'agprofile',
  );

  // Exported field_base: 'field_ag_duties'
  $field_bases['field_ag_duties'] = array(
    'active' => 1,
    'cardinality' => -1,
    'deleted' => 0,
    'entity_types' => array(),
    'field_name' => 'field_ag_duties',
    'field_permissions' => array(
      'type' => 0,
    ),
    'foreign keys' => array(
      'format' => array(
        'columns' => array(
          'format' => 'format',
        ),
        'table' => 'filter_format',
      ),
    ),
    'indexes' => array(
      'format' => array(
        0 => 'format',
      ),
    ),
    'locked' => 0,
    'module' => 'text',
    'settings' => array(
      'allowed_values' => '',
      'allowed_values_php' => '',
      'max_length' => '',
      'text_processing' => 0,
    ),
    'translatable' => 0,
    'type' => 'text_long',
    'type_name' => 'agprofile',
  );

  // Exported field_base: 'field_ag_edition'
  $field_bases['field_ag_edition'] = array(
    'active' => 1,
    'cardinality' => 1,
    'deleted' => 0,
    'entity_types' => array(),
    'field_name' => 'field_ag_edition',
    'foreign keys' => array(
      'format' => array(
        'columns' => array(
          'format' => 'format',
        ),
        'table' => 'filter_format',
      ),
    ),
    'indexes' => array(
      'format' => array(
        0 => 'format',
      ),
    ),
    'locked' => 0,
    'module' => 'text',
    'settings' => array(
      'allowed_values' => '',
      'allowed_values_php' => '',
      'max_length' => 255,
      'text_processing' => 0,
    ),
    'translatable' => 0,
    'type' => 'text',
    'type_name' => 'agdlios',
  );

  // Exported field_base: 'field_ag_eissn'
  $field_bases['field_ag_eissn'] = array(
    'active' => 1,
    'cardinality' => 1,
    'deleted' => 0,
    'entity_types' => array(),
    'field_name' => 'field_ag_eissn',
    'field_permissions' => array(
      'type' => 0,
    ),
    'foreign keys' => array(
      'format' => array(
        'columns' => array(
          'format' => 'format',
        ),
        'table' => 'filter_format',
      ),
    ),
    'indexes' => array(
      'format' => array(
        0 => 'format',
      ),
    ),
    'locked' => 0,
    'module' => 'text',
    'settings' => array(
      'allowed_values' => '',
      'allowed_values_php' => '',
      'max_length' => 255,
      'text_processing' => 0,
    ),
    'translatable' => 0,
    'type' => 'text',
    'type_name' => 'agdlios',
  );

  // Exported field_base: 'field_ag_email'
  $field_bases['field_ag_email'] = array(
    'active' => 1,
    'cardinality' => 1,
    'deleted' => 0,
    'entity_types' => array(),
    'field_name' => 'field_ag_email',
    'field_permissions' => array(
      'type' => 0,
    ),
    'foreign keys' => array(),
    'indexes' => array(),
    'locked' => 0,
    'module' => 'email',
    'settings' => array(),
    'translatable' => 0,
    'type' => 'email',
    'type_name' => 'agprofile',
  );

  // Exported field_base: 'field_ag_employer'
  $field_bases['field_ag_employer'] = array(
    'active' => 1,
    'cardinality' => 1,
    'deleted' => 0,
    'entity_types' => array(),
    'field_name' => 'field_ag_employer',
    'foreign keys' => array(
      'format' => array(
        'columns' => array(
          'format' => 'format',
        ),
        'table' => 'filter_format',
      ),
    ),
    'indexes' => array(
      'format' => array(
        0 => 'format',
      ),
    ),
    'locked' => 0,
    'module' => 'text',
    'settings' => array(
      'allowed_values' => '',
      'allowed_values_php' => '',
      'max_length' => 255,
      'text_processing' => 0,
    ),
    'translatable' => 0,
    'type' => 'text',
    'type_name' => 'agrssjob',
  );

  // Exported field_base: 'field_ag_form'
  $field_bases['field_ag_form'] = array(
    'active' => 1,
    'cardinality' => 1,
    'deleted' => 0,
    'entity_types' => array(),
    'field_name' => 'field_ag_form',
    'foreign keys' => array(),
    'indexes' => array(
      'value' => array(
        0 => 'value',
      ),
    ),
    'locked' => 0,
    'module' => 'list',
    'settings' => array(
      'allowed_values' => array(
        0 => 'CD-ROM',
        1 => 'DVD',
        2 => 'Printed resource',
        3 => 'Slides',
        4 => 'Videocassette/tape',
        5 => 'Web resource',
      ),
      'allowed_values_function' => '',
      'allowed_values_php' => '',
      'text_processing' => 0,
    ),
    'translatable' => 0,
    'type' => 'list_text',
    'type_name' => 'agdlios',
  );

  // Exported field_base: 'field_ag_gender'
  $field_bases['field_ag_gender'] = array(
    'active' => 1,
    'cardinality' => 1,
    'deleted' => 0,
    'entity_types' => array(),
    'field_name' => 'field_ag_gender',
    'foreign keys' => array(),
    'indexes' => array(
      'value' => array(
        0 => 'value',
      ),
    ),
    'locked' => 0,
    'module' => 'list',
    'settings' => array(
      'allowed_values' => array(
        'F' => 'Female',
        'M' => 'Male',
      ),
      'allowed_values_function' => '',
      'allowed_values_php' => '',
      'text_processing' => 0,
    ),
    'translatable' => 0,
    'type' => 'list_text',
    'type_name' => 'agprofile',
  );

  // Exported field_base: 'field_ag_institution'
  $field_bases['field_ag_institution'] = array(
    'active' => 1,
    'cardinality' => -1,
    'deleted' => 0,
    'entity_types' => array(),
    'field_name' => 'field_ag_institution',
    'field_permissions' => array(
      'type' => 0,
    ),
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
    'locked' => 0,
    'module' => 'node_reference',
    'settings' => array(
      'referenceable_types' => array(
        'ag_job_feed' => 0,
        'agauthor' => 0,
        'agconference' => 0,
        'agcorporate_body' => 0,
        'agdlios' => 0,
        'agevent' => 0,
        'ageventsfeed' => 0,
        'aginstitution' => 'aginstitution',
        'agnews' => 0,
        'agnewsfeed' => 0,
        'agprofile' => 0,
        'agrepository' => 0,
        'agris_authors_feed' => 0,
        'agris_authors_upload' => 0,
        'agris_conferences_feed' => 0,
        'agris_corporate_feed' => 0,
        'agris_dlio_upload' => 0,
        'agris_journals_feed' => 0,
        'agris_publishers_feed' => 0,
        'agrssevents' => 0,
        'agrssjob' => 0,
        'agrssnews' => 0,
        'agvacancy' => 0,
        'blog' => 0,
        'books_feed' => 0,
        'feed' => 0,
        'feed_fast' => 0,
        'feed_item' => 0,
        'forum' => 0,
        'group' => 0,
        'page' => 0,
        'story' => 0,
        'xmlimport_dlios' => 0,
      ),
      'view' => array(
        'args' => array(),
        'display_name' => '',
        'view_name' => '',
      ),
    ),
    'translatable' => 0,
    'type' => 'node_reference',
  );

  // Exported field_base: 'field_ag_institution_record'
  $field_bases['field_ag_institution_record'] = array(
    'active' => 1,
    'cardinality' => 1,
    'deleted' => 0,
    'entity_types' => array(),
    'field_name' => 'field_ag_institution_record',
    'field_permissions' => array(
      'type' => 0,
    ),
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
    'locked' => 0,
    'module' => 'node_reference',
    'settings' => array(
      'referenceable_types' => array(
        'ag_job_feed' => 0,
        'agauthor' => 0,
        'agconference' => 0,
        'agcorporate_body' => 0,
        'agdlios' => 0,
        'agevent' => 0,
        'ageventsfeed' => 0,
        'aginstitution' => 'aginstitution',
        'agnews' => 0,
        'agnewsfeed' => 0,
        'agprofile' => 0,
        'agrepository' => 0,
        'agris_authors_feed' => 0,
        'agris_authors_upload' => 0,
        'agris_conferences_feed' => 0,
        'agris_corporate_feed' => 0,
        'agris_dlio_upload' => 0,
        'agris_journals_feed' => 0,
        'agris_publishers_feed' => 0,
        'agrssevents' => 0,
        'agrssjob' => 0,
        'agrssnews' => 0,
        'agvacancy' => 0,
        'blog' => 0,
        'books_feed' => 0,
        'feed' => 0,
        'feed_fast' => 0,
        'feed_item' => 0,
        'forum' => 0,
        'group' => 0,
        'page' => 0,
        'story' => 0,
        'xmlimport_dlios' => 0,
      ),
      'view' => array(
        'args' => array(),
        'display_name' => '',
        'view_name' => '',
      ),
    ),
    'translatable' => 0,
    'type' => 'node_reference',
    'type_name' => 'agcorporate_body',
  );

  // Exported field_base: 'field_ag_institution_type'
  $field_bases['field_ag_institution_type'] = array(
    'active' => 1,
    'cardinality' => 1,
    'deleted' => 0,
    'entity_types' => array(),
    'field_name' => 'field_ag_institution_type',
    'field_permissions' => array(
      'type' => 0,
    ),
    'foreign keys' => array(
      'tid' => array(
        'columns' => array(
          'tid' => 'tid',
        ),
        'table' => 'taxonomy_term_data',
      ),
    ),
    'indexes' => array(
      'tid' => array(
        0 => 'tid',
      ),
    ),
    'locked' => 0,
    'module' => 'taxonomy',
    'settings' => array(
      'allowed_values' => array(
        0 => array(
          'vocabulary' => 'vocabulary_12',
          'parent' => 0,
          'depth' => '',
        ),
      ),
      'options_list_callback' => 'i18n_taxonomy_allowed_values',
    ),
    'translatable' => 0,
    'type' => 'taxonomy_term_reference',
  );

  // Exported field_base: 'field_ag_isbn'
  $field_bases['field_ag_isbn'] = array(
    'active' => 1,
    'cardinality' => 1,
    'deleted' => 0,
    'entity_types' => array(),
    'field_name' => 'field_ag_isbn',
    'field_permissions' => array(
      'type' => 0,
    ),
    'foreign keys' => array(
      'format' => array(
        'columns' => array(
          'format' => 'format',
        ),
        'table' => 'filter_format',
      ),
    ),
    'indexes' => array(
      'format' => array(
        0 => 'format',
      ),
    ),
    'locked' => 0,
    'module' => 'text',
    'settings' => array(
      'allowed_values' => '',
      'allowed_values_php' => '',
      'max_length' => 255,
      'text_processing' => 0,
    ),
    'translatable' => 0,
    'type' => 'text',
    'type_name' => 'agdlios',
  );

  // Exported field_base: 'field_ag_issn'
  $field_bases['field_ag_issn'] = array(
    'active' => 1,
    'cardinality' => 1,
    'deleted' => 0,
    'entity_types' => array(),
    'field_name' => 'field_ag_issn',
    'field_permissions' => array(
      'type' => 0,
    ),
    'foreign keys' => array(
      'format' => array(
        'columns' => array(
          'format' => 'format',
        ),
        'table' => 'filter_format',
      ),
    ),
    'indexes' => array(
      'format' => array(
        0 => 'format',
      ),
    ),
    'locked' => 0,
    'module' => 'text',
    'settings' => array(
      'allowed_values' => '',
      'allowed_values_php' => '',
      'max_length' => 255,
      'text_processing' => 0,
    ),
    'translatable' => 0,
    'type' => 'text',
    'type_name' => 'agdlios',
  );

  // Exported field_base: 'field_ag_journal'
  $field_bases['field_ag_journal'] = array(
    'active' => 1,
    'cardinality' => 1,
    'deleted' => 0,
    'entity_types' => array(),
    'field_name' => 'field_ag_journal',
    'field_permissions' => array(
      'type' => 0,
    ),
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
    'locked' => 0,
    'module' => 'node_reference',
    'settings' => array(
      'referenceable_types' => array(
        'ag_job_feed' => 0,
        'agauthor' => 0,
        'agconference' => 0,
        'agcorporate_body' => 0,
        'agdlios' => 0,
        'agevent' => 0,
        'ageventsfeed' => 0,
        'aginstitution' => 0,
        'agnews' => 0,
        'agnewsfeed' => 0,
        'agperiodical' => 'agperiodical',
        'agprofile' => 0,
        'agrepository' => 0,
        'agris_authors_feed' => 0,
        'agris_authors_upload' => 0,
        'agris_conferences_feed' => 0,
        'agris_corporate_feed' => 0,
        'agris_dlio_upload' => 0,
        'agris_journals_feed' => 0,
        'agris_publishers_feed' => 0,
        'agrssevents' => 0,
        'agrssjob' => 0,
        'agrssnews' => 0,
        'agvacancy' => 0,
        'blog' => 0,
        'books_feed' => 0,
        'feed' => 0,
        'feed_fast' => 0,
        'feed_item' => 0,
        'forum' => 0,
        'group' => 0,
        'page' => 0,
        'story' => 0,
        'xmlimport_dlios' => 0,
      ),
      'view' => array(
        'args' => array(),
        'display_name' => '',
        'view_name' => '',
      ),
    ),
    'translatable' => 0,
    'type' => 'node_reference',
    'type_name' => 'agdlios',
  );

  // Exported field_base: 'field_ag_journal_issue'
  $field_bases['field_ag_journal_issue'] = array(
    'active' => 1,
    'cardinality' => 1,
    'deleted' => 0,
    'entity_types' => array(),
    'field_name' => 'field_ag_journal_issue',
    'field_permissions' => array(
      'type' => 0,
    ),
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
    'locked' => 0,
    'module' => 'node_reference',
    'settings' => array(
      'referenceable_types' => array(
        'ag_job_feed' => 0,
        'agauthor' => 0,
        'agconference' => 0,
        'agcorporate_body' => 0,
        'agdlios' => 'agdlios',
        'agevent' => 0,
        'ageventsfeed' => 0,
        'aginstitution' => 0,
        'agnews' => 0,
        'agnewsfeed' => 0,
        'agprofile' => 0,
        'agrepository' => 0,
        'agris_authors_feed' => 0,
        'agris_authors_upload' => 0,
        'agris_conferences_feed' => 0,
        'agris_corporate_feed' => 0,
        'agris_dlio_upload' => 0,
        'agris_journals_feed' => 0,
        'agris_publishers_feed' => 0,
        'agrssevents' => 0,
        'agrssjob' => 0,
        'agrssnews' => 0,
        'agvacancy' => 0,
        'blog' => 0,
        'books_feed' => 0,
        'feed' => 0,
        'feed_fast' => 0,
        'feed_item' => 0,
        'forum' => 0,
        'group' => 0,
        'page' => 0,
        'story' => 0,
        'xmlimport_dlios' => 0,
      ),
      'view' => array(
        'args' => array(),
        'display_name' => '',
        'view_name' => '',
      ),
    ),
    'translatable' => 0,
    'type' => 'node_reference',
    'type_name' => 'agdlios',
  );

  // Exported field_base: 'field_ag_langsubt'
  $field_bases['field_ag_langsubt'] = array(
    'active' => 1,
    'cardinality' => -1,
    'deleted' => 0,
    'entity_types' => array(),
    'field_name' => 'field_ag_langsubt',
    'field_permissions' => array(
      'type' => 0,
    ),
    'foreign keys' => array(),
    'indexes' => array(),
    'locked' => 0,
    'module' => 'get_list_from_file',
    'settings' => array(
      'file' => 'languages-new.csv',
    ),
    'translatable' => 0,
    'type' => 'get_list_from_file',
  );

  // Exported field_base: 'field_ag_langtext'
  $field_bases['field_ag_langtext'] = array(
    'active' => 1,
    'cardinality' => -1,
    'deleted' => 0,
    'entity_types' => array(),
    'field_name' => 'field_ag_langtext',
    'field_permissions' => array(
      'type' => 0,
    ),
    'foreign keys' => array(),
    'indexes' => array(),
    'locked' => 0,
    'module' => 'get_list_from_file',
    'settings' => array(
      'file' => 'languages-new.csv',
    ),
    'translatable' => 0,
    'type' => 'get_list_from_file',
  );

  // Exported field_base: 'field_ag_langtitle'
  $field_bases['field_ag_langtitle'] = array(
    'active' => 1,
    'cardinality' => 1,
    'deleted' => 0,
    'entity_types' => array(),
    'field_name' => 'field_ag_langtitle',
    'field_permissions' => array(
      'type' => 0,
    ),
    'foreign keys' => array(),
    'indexes' => array(),
    'locked' => 0,
    'module' => 'get_list_from_file',
    'settings' => array(
      'file' => 'languages-new.csv',
    ),
    'translatable' => 0,
    'type' => 'get_list_from_file',
  );

  // Exported field_base: 'field_ag_language'
  $field_bases['field_ag_language'] = array(
    'active' => 1,
    'cardinality' => -1,
    'deleted' => 0,
    'entity_types' => array(),
    'field_name' => 'field_ag_language',
    'field_permissions' => array(
      'type' => 0,
    ),
    'foreign keys' => array(),
    'indexes' => array(),
    'locked' => 0,
    'module' => 'get_list_from_file',
    'settings' => array(
      'file' => 'languages-new.csv',
    ),
    'translatable' => 0,
    'type' => 'get_list_from_file',
  );

  // Exported field_base: 'field_ag_link_to_jissue'
  $field_bases['field_ag_link_to_jissue'] = array(
    'active' => 1,
    'cardinality' => 1,
    'deleted' => 0,
    'entity_types' => array(),
    'field_name' => 'field_ag_link_to_jissue',
    'field_permissions' => array(
      'type' => 0,
    ),
    'foreign keys' => array(),
    'indexes' => array(
      'value' => array(
        0 => 'value',
      ),
    ),
    'locked' => 0,
    'module' => 'list',
    'settings' => array(
      'allowed_values' => array(
        0 => 'Link to journal',
        1 => 'Link to journal issue',
      ),
      'allowed_values_function' => '',
      'allowed_values_php' => '',
      'text_processing' => 0,
    ),
    'translatable' => 0,
    'type' => 'list_text',
    'type_name' => 'agdlios',
  );

  // Exported field_base: 'field_ag_location'
  $field_bases['field_ag_location'] = array(
    'active' => 1,
    'cardinality' => 1,
    'deleted' => 0,
    'entity_types' => array(),
    'field_name' => 'field_ag_location',
    'foreign keys' => array(
      'format' => array(
        'columns' => array(
          'format' => 'format',
        ),
        'table' => 'filter_format',
      ),
    ),
    'indexes' => array(
      'format' => array(
        0 => 'format',
      ),
    ),
    'locked' => 0,
    'module' => 'text',
    'settings' => array(
      'allowed_values' => '',
      'allowed_values_php' => '',
      'max_length' => 255,
      'text_processing' => 0,
    ),
    'translatable' => 0,
    'type' => 'text',
    'type_name' => 'agvacancy',
  );

  // Exported field_base: 'field_ag_locator'
  $field_bases['field_ag_locator'] = array(
    'active' => 1,
    'cardinality' => -1,
    'deleted' => 0,
    'entity_types' => array(),
    'field_name' => 'field_ag_locator',
    'field_permissions' => array(
      'type' => 0,
    ),
    'foreign keys' => array(
      'format' => array(
        'columns' => array(
          'format' => 'format',
        ),
        'table' => 'filter_format',
      ),
    ),
    'indexes' => array(
      'format' => array(
        0 => 'format',
      ),
    ),
    'locked' => 0,
    'module' => 'text',
    'settings' => array(
      'allowed_values' => '',
      'allowed_values_php' => '',
      'max_length' => 255,
      'text_processing' => 0,
    ),
    'translatable' => 0,
    'type' => 'text',
    'type_name' => 'agdlios',
  );

  // Exported field_base: 'field_ag_network'
  $field_bases['field_ag_network'] = array(
    'active' => 1,
    'cardinality' => -1,
    'deleted' => 0,
    'entity_types' => array(),
    'field_name' => 'field_ag_network',
    'field_permissions' => array(
      'type' => 0,
    ),
    'foreign keys' => array(),
    'indexes' => array(
      'value' => array(
        0 => 'value',
      ),
    ),
    'locked' => 0,
    'module' => 'list',
    'settings' => array(
      'allowed_values' => array(
        1 => 'Facebook',
        2 => 'LinkedIn',
        3 => 'Twitter',
        4 => 'NING',
        5 => 'Google+',
        10 => 'Scopus',
        11 => ' ResearcherID',
        12 => 'ORCID',
        13 => 'Google Scholar',
        20 => 'Other',
      ),
      'allowed_values_function' => '',
      'allowed_values_php' => '',
    ),
    'translatable' => 0,
    'type' => 'list_text',
  );

  // Exported field_base: 'field_ag_network_id'
  $field_bases['field_ag_network_id'] = array(
    'active' => 1,
    'cardinality' => -1,
    'deleted' => 0,
    'entity_types' => array(),
    'field_name' => 'field_ag_network_id',
    'field_permissions' => array(
      'type' => 0,
    ),
    'foreign keys' => array(
      'format' => array(
        'columns' => array(
          'format' => 'format',
        ),
        'table' => 'filter_format',
      ),
    ),
    'indexes' => array(
      'format' => array(
        0 => 'format',
      ),
    ),
    'locked' => 0,
    'module' => 'text',
    'settings' => array(
      'max_length' => 255,
    ),
    'translatable' => 0,
    'type' => 'text',
  );

  // Exported field_base: 'field_ag_notes'
  $field_bases['field_ag_notes'] = array(
    'active' => 1,
    'cardinality' => 1,
    'deleted' => 0,
    'entity_types' => array(),
    'field_name' => 'field_ag_notes',
    'foreign keys' => array(
      'format' => array(
        'columns' => array(
          'format' => 'format',
        ),
        'table' => 'filter_format',
      ),
    ),
    'indexes' => array(
      'format' => array(
        0 => 'format',
      ),
    ),
    'locked' => 0,
    'module' => 'text',
    'settings' => array(
      'allowed_values' => '',
      'allowed_values_php' => '',
      'max_length' => '',
      'text_processing' => 0,
    ),
    'translatable' => 0,
    'type' => 'text_long',
    'type_name' => 'agdlios',
  );

  // Exported field_base: 'field_ag_number'
  $field_bases['field_ag_number'] = array(
    'active' => 1,
    'cardinality' => 1,
    'deleted' => 0,
    'entity_types' => array(),
    'field_name' => 'field_ag_number',
    'field_permissions' => array(
      'type' => 0,
    ),
    'foreign keys' => array(
      'format' => array(
        'columns' => array(
          'format' => 'format',
        ),
        'table' => 'filter_format',
      ),
    ),
    'indexes' => array(
      'format' => array(
        0 => 'format',
      ),
    ),
    'locked' => 0,
    'module' => 'text',
    'settings' => array(
      'allowed_values' => '',
      'allowed_values_php' => '',
      'max_length' => 255,
      'text_processing' => 0,
    ),
    'translatable' => 0,
    'type' => 'text',
    'type_name' => 'agdlios',
  );

  // Exported field_base: 'field_ag_orderc'
  $field_bases['field_ag_orderc'] = array(
    'active' => 1,
    'cardinality' => -1,
    'deleted' => 0,
    'entity_types' => array(),
    'field_name' => 'field_ag_orderc',
    'field_permissions' => array(
      'type' => 0,
    ),
    'foreign keys' => array(),
    'indexes' => array(),
    'locked' => 0,
    'module' => 'number',
    'settings' => array(
      'allowed_values' => array(),
      'allowed_values_php' => '',
      'max' => '',
      'min' => '',
      'prefix' => '',
      'suffix' => '',
    ),
    'translatable' => 0,
    'type' => 'number_integer',
    'type_name' => 'agdlios',
  );

  // Exported field_base: 'field_ag_pagination'
  $field_bases['field_ag_pagination'] = array(
    'active' => 1,
    'cardinality' => 1,
    'deleted' => 0,
    'entity_types' => array(),
    'field_name' => 'field_ag_pagination',
    'field_permissions' => array(
      'type' => 0,
    ),
    'foreign keys' => array(
      'format' => array(
        'columns' => array(
          'format' => 'format',
        ),
        'table' => 'filter_format',
      ),
    ),
    'indexes' => array(
      'format' => array(
        0 => 'format',
      ),
    ),
    'locked' => 0,
    'module' => 'text',
    'settings' => array(
      'allowed_values' => '',
      'allowed_values_php' => '',
      'max_length' => 255,
      'text_processing' => 0,
    ),
    'translatable' => 0,
    'type' => 'text',
    'type_name' => 'agdlios',
  );

  // Exported field_base: 'field_ag_period'
  $field_bases['field_ag_period'] = array(
    'active' => 1,
    'cardinality' => -1,
    'deleted' => 0,
    'entity_types' => array(),
    'field_name' => 'field_ag_period',
    'field_permissions' => array(
      'type' => 0,
    ),
    'foreign keys' => array(),
    'indexes' => array(),
    'locked' => 0,
    'module' => 'date',
    'settings' => array(
      'cache_count' => 4,
      'cache_enabled' => 0,
      'granularity' => array(
        'day' => 0,
        'hour' => 0,
        'minute' => 0,
        'month' => 'month',
        'second' => 0,
        'year' => 'year',
      ),
      'repeat' => 0,
      'timezone_db' => '',
      'todate' => 'required',
      'tz_handling' => 'none',
    ),
    'translatable' => 0,
    'type' => 'date',
    'type_name' => 'agprofile',
  );

  // Exported field_base: 'field_ag_personal_website'
  $field_bases['field_ag_personal_website'] = array(
    'active' => 1,
    'cardinality' => 1,
    'deleted' => 0,
    'entity_types' => array(),
    'field_name' => 'field_ag_personal_website',
    'field_permissions' => array(
      'type' => 0,
    ),
    'foreign keys' => array(),
    'indexes' => array(),
    'locked' => 0,
    'module' => 'link',
    'settings' => array(
      'attributes' => array(
        'class' => '',
        'rel' => '',
        'target' => 'default',
      ),
      'display' => array(
        'url_cutoff' => 80,
      ),
      'enable_tokens' => 1,
      'title' => 'optional',
      'title_maxlength' => 128,
      'title_value' => '',
      'url' => 0,
    ),
    'translatable' => 0,
    'type' => 'link_field',
  );

  // Exported field_base: 'field_ag_phlocation'
  $field_bases['field_ag_phlocation'] = array(
    'active' => 1,
    'cardinality' => -1,
    'deleted' => 0,
    'entity_types' => array(),
    'field_name' => 'field_ag_phlocation',
    'field_permissions' => array(
      'type' => 0,
    ),
    'foreign keys' => array(
      'format' => array(
        'columns' => array(
          'format' => 'format',
        ),
        'table' => 'filter_format',
      ),
    ),
    'indexes' => array(
      'format' => array(
        0 => 'format',
      ),
    ),
    'locked' => 0,
    'module' => 'text',
    'settings' => array(
      'allowed_values' => '',
      'allowed_values_php' => '',
      'max_length' => 255,
      'text_processing' => 0,
    ),
    'translatable' => 0,
    'type' => 'text',
    'type_name' => 'agdlios',
  );

  // Exported field_base: 'field_ag_position'
  $field_bases['field_ag_position'] = array(
    'active' => 1,
    'cardinality' => -1,
    'deleted' => 0,
    'entity_types' => array(),
    'field_name' => 'field_ag_position',
    'field_permissions' => array(
      'type' => 0,
    ),
    'foreign keys' => array(
      'format' => array(
        'columns' => array(
          'format' => 'format',
        ),
        'table' => 'filter_format',
      ),
    ),
    'indexes' => array(
      'format' => array(
        0 => 'format',
      ),
    ),
    'locked' => 0,
    'module' => 'text',
    'settings' => array(
      'allowed_values' => '',
      'allowed_values_php' => '',
      'max_length' => 255,
      'text_processing' => 0,
    ),
    'translatable' => 0,
    'type' => 'text',
    'type_name' => 'agprofile',
  );

  // Exported field_base: 'field_ag_post_date'
  $field_bases['field_ag_post_date'] = array(
    'active' => 1,
    'cardinality' => 1,
    'deleted' => 0,
    'entity_types' => array(),
    'field_name' => 'field_ag_post_date',
    'foreign keys' => array(),
    'indexes' => array(),
    'locked' => 0,
    'module' => 'date',
    'settings' => array(
      'granularity' => array(
        'day' => 'day',
        'hour' => 'hour',
        'minute' => 'minute',
        'month' => 'month',
        'second' => 'second',
        'year' => 'year',
      ),
      'repeat' => 0,
      'timezone_db' => 'UTC',
      'todate' => '',
      'tz_handling' => 'site',
    ),
    'translatable' => 0,
    'type' => 'datetime',
    'type_name' => 'agrssnews',
  );

  // Exported field_base: 'field_ag_profile_url'
  $field_bases['field_ag_profile_url'] = array(
    'active' => 1,
    'cardinality' => -1,
    'deleted' => 0,
    'entity_types' => array(),
    'field_name' => 'field_ag_profile_url',
    'field_permissions' => array(
      'type' => 0,
    ),
    'foreign keys' => array(),
    'indexes' => array(),
    'locked' => 0,
    'module' => 'link',
    'settings' => array(
      'attributes' => array(
        'class' => '',
        'rel' => '',
        'target' => 'default',
      ),
      'display' => array(
        'url_cutoff' => 80,
      ),
      'enable_tokens' => 1,
      'title' => 'optional',
      'title_maxlength' => 128,
      'title_value' => '',
      'url' => 0,
    ),
    'translatable' => 0,
    'type' => 'link_field',
  );

  // Exported field_base: 'field_ag_pub_title'
  $field_bases['field_ag_pub_title'] = array(
    'active' => 1,
    'cardinality' => -1,
    'deleted' => 0,
    'entity_types' => array(),
    'field_name' => 'field_ag_pub_title',
    'field_permissions' => array(
      'type' => 0,
    ),
    'foreign keys' => array(
      'format' => array(
        'columns' => array(
          'format' => 'format',
        ),
        'table' => 'filter_format',
      ),
    ),
    'indexes' => array(
      'format' => array(
        0 => 'format',
      ),
    ),
    'locked' => 0,
    'module' => 'text',
    'settings' => array(
      'allowed_values' => '',
      'allowed_values_php' => '',
      'max_length' => 255,
      'text_processing' => 0,
    ),
    'translatable' => 0,
    'type' => 'text',
    'type_name' => 'agprofile',
  );

  // Exported field_base: 'field_ag_publisher'
  $field_bases['field_ag_publisher'] = array(
    'active' => 1,
    'cardinality' => 1,
    'deleted' => 0,
    'entity_types' => array(),
    'field_name' => 'field_ag_publisher',
    'field_permissions' => array(
      'type' => 0,
    ),
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
    'locked' => 0,
    'module' => 'node_reference',
    'settings' => array(
      'referenceable_types' => array(
        'ag_job_feed' => 0,
        'agauthor' => 0,
        'agconference' => 0,
        'agcorporate_body' => 'agcorporate_body',
        'agdlios' => 0,
        'agevent' => 0,
        'ageventsfeed' => 0,
        'aginstitution' => 0,
        'agnews' => 0,
        'agnewsfeed' => 0,
        'agperiodical' => 0,
        'agprofile' => 0,
        'agrepository' => 0,
        'agris_authors_feed' => 0,
        'agris_authors_upload' => 0,
        'agris_conferences_feed' => 0,
        'agris_corporate_feed' => 0,
        'agris_dlio_upload' => 0,
        'agris_journals_feed' => 0,
        'agris_publishers_feed' => 0,
        'agrssevents' => 0,
        'agrssjob' => 0,
        'agrssnews' => 0,
        'agvacancy' => 0,
        'blog' => 0,
        'books_feed' => 0,
        'feed' => 0,
        'feed_fast' => 0,
        'feed_item' => 0,
        'forum' => 0,
        'group' => 0,
        'page' => 0,
        'story' => 0,
        'xmlimport_dlios' => 0,
      ),
      'view' => array(
        'args' => array(),
        'display_name' => 'references_1',
        'view_name' => 'refauthors',
      ),
    ),
    'translatable' => 0,
    'type' => 'node_reference',
    'type_name' => 'agdlios',
  );

  // Exported field_base: 'field_ag_publishplace'
  $field_bases['field_ag_publishplace'] = array(
    'active' => 1,
    'cardinality' => 1,
    'deleted' => 0,
    'entity_types' => array(),
    'field_name' => 'field_ag_publishplace',
    'field_permissions' => array(
      'type' => 0,
    ),
    'foreign keys' => array(
      'format' => array(
        'columns' => array(
          'format' => 'format',
        ),
        'table' => 'filter_format',
      ),
    ),
    'indexes' => array(
      'format' => array(
        0 => 'format',
      ),
    ),
    'locked' => 0,
    'module' => 'text',
    'settings' => array(
      'allowed_values' => '',
      'allowed_values_php' => '',
      'max_length' => 255,
      'text_processing' => 0,
    ),
    'translatable' => 0,
    'type' => 'text',
    'type_name' => 'agdlios',
  );

  // Exported field_base: 'field_ag_resource_url'
  $field_bases['field_ag_resource_url'] = array(
    'active' => 1,
    'cardinality' => 1,
    'deleted' => 0,
    'entity_types' => array(),
    'field_name' => 'field_ag_resource_url',
    'field_permissions' => array(
      'type' => 0,
    ),
    'foreign keys' => array(),
    'indexes' => array(),
    'locked' => 0,
    'module' => 'link',
    'settings' => array(
      'attributes' => array(
        'class' => '',
        'rel' => '',
        'target' => 'default',
      ),
      'display' => array(
        'url_cutoff' => 80,
      ),
      'enable_tokens' => 1,
      'title' => 'optional',
      'title_maxlength' => 128,
      'title_value' => '',
      'url' => 0,
    ),
    'translatable' => 0,
    'type' => 'link_field',
  );

  // Exported field_base: 'field_ag_role'
  $field_bases['field_ag_role'] = array(
    'active' => 1,
    'cardinality' => -1,
    'deleted' => 0,
    'entity_types' => array(),
    'field_name' => 'field_ag_role',
    'field_permissions' => array(
      'type' => 0,
    ),
    'foreign keys' => array(),
    'indexes' => array(
      'value' => array(
        0 => 'value',
      ),
    ),
    'locked' => 0,
    'module' => 'list',
    'settings' => array(
      'allowed_values' => array(
        0 => 'Author',
        1 => 'Compiler',
        2 => 'Contributor',
        3 => 'Editor',
      ),
      'allowed_values_function' => '',
      'allowed_values_php' => '',
      'text_processing' => 0,
    ),
    'translatable' => 0,
    'type' => 'list_text',
    'type_name' => 'agdlios',
  );

  // Exported field_base: 'field_ag_rolecorp'
  $field_bases['field_ag_rolecorp'] = array(
    'active' => 1,
    'cardinality' => -1,
    'deleted' => 0,
    'entity_types' => array(),
    'field_name' => 'field_ag_rolecorp',
    'field_permissions' => array(
      'type' => 0,
    ),
    'foreign keys' => array(),
    'indexes' => array(
      'value' => array(
        0 => 'value',
      ),
    ),
    'locked' => 0,
    'module' => 'list',
    'settings' => array(
      'allowed_values' => array(
        0 => 'Author',
        1 => 'Compiler',
        2 => 'Contributor',
        3 => 'Editor',
      ),
      'allowed_values_function' => '',
      'allowed_values_php' => '',
      'text_processing' => 1,
    ),
    'translatable' => 0,
    'type' => 'list_text',
    'type_name' => 'agdlios',
  );

  // Exported field_base: 'field_ag_source'
  $field_bases['field_ag_source'] = array(
    'active' => 1,
    'cardinality' => -1,
    'deleted' => 0,
    'entity_types' => array(),
    'field_name' => 'field_ag_source',
    'foreign keys' => array(
      'format' => array(
        'columns' => array(
          'format' => 'format',
        ),
        'table' => 'filter_format',
      ),
    ),
    'indexes' => array(
      'format' => array(
        0 => 'format',
      ),
    ),
    'locked' => 0,
    'module' => 'text',
    'settings' => array(
      'allowed_values' => '',
      'allowed_values_php' => '',
      'max_length' => '',
      'text_processing' => 0,
    ),
    'translatable' => 0,
    'type' => 'text_long',
    'type_name' => 'agdlios',
  );

  // Exported field_base: 'field_ag_source_url'
  $field_bases['field_ag_source_url'] = array(
    'active' => 1,
    'cardinality' => -1,
    'deleted' => 0,
    'entity_types' => array(),
    'field_name' => 'field_ag_source_url',
    'field_permissions' => array(
      'type' => 0,
    ),
    'foreign keys' => array(
      'format' => array(
        'columns' => array(
          'format' => 'format',
        ),
        'table' => 'filter_format',
      ),
    ),
    'indexes' => array(
      'format' => array(
        0 => 'format',
      ),
    ),
    'locked' => 0,
    'module' => 'text',
    'settings' => array(
      'allowed_values' => '',
      'allowed_values_php' => '',
      'max_length' => 255,
      'text_processing' => 0,
    ),
    'translatable' => 0,
    'type' => 'text',
    'type_name' => 'agrssnews',
  );

  // Exported field_base: 'field_ag_study_field'
  $field_bases['field_ag_study_field'] = array(
    'active' => 1,
    'cardinality' => 1,
    'deleted' => 0,
    'entity_types' => array(),
    'field_name' => 'field_ag_study_field',
    'foreign keys' => array(
      'format' => array(
        'columns' => array(
          'format' => 'format',
        ),
        'table' => 'filter_format',
      ),
    ),
    'indexes' => array(
      'format' => array(
        0 => 'format',
      ),
    ),
    'locked' => 0,
    'module' => 'text',
    'settings' => array(
      'allowed_values' => '',
      'allowed_values_php' => '',
      'max_length' => 255,
      'text_processing' => 0,
    ),
    'translatable' => 0,
    'type' => 'text',
    'type_name' => 'agprofile',
  );

  // Exported field_base: 'field_ag_subtitle'
  $field_bases['field_ag_subtitle'] = array(
    'active' => 1,
    'cardinality' => -1,
    'deleted' => 0,
    'entity_types' => array(),
    'field_name' => 'field_ag_subtitle',
    'field_permissions' => array(
      'type' => 0,
    ),
    'foreign keys' => array(
      'format' => array(
        'columns' => array(
          'format' => 'format',
        ),
        'table' => 'filter_format',
      ),
    ),
    'indexes' => array(
      'format' => array(
        0 => 'format',
      ),
    ),
    'locked' => 0,
    'module' => 'text',
    'settings' => array(
      'max_length' => 255,
    ),
    'translatable' => 0,
    'type' => 'text',
  );

  // Exported field_base: 'field_ag_tag'
  $field_bases['field_ag_tag'] = array(
    'active' => 1,
    'cardinality' => -1,
    'deleted' => 0,
    'entity_types' => array(),
    'field_name' => 'field_ag_tag',
    'foreign keys' => array(
      'tid' => array(
        'columns' => array(
          'tid' => 'tid',
        ),
        'table' => 'taxonomy_term_data',
      ),
    ),
    'indexes' => array(
      'tid' => array(
        0 => 'tid',
      ),
    ),
    'locked' => 0,
    'module' => 'taxonomy',
    'settings' => array(
      'allowed_values' => array(
        0 => array(
          'vocabulary' => 'vocabulary_13',
          'parent' => 0,
        ),
      ),
      'options_list_callback' => 'i18n_taxonomy_allowed_values',
    ),
    'translatable' => 0,
    'type' => 'taxonomy_term_reference',
    'type_name' => 'agnews',
  );

  // Exported field_base: 'field_ag_telephone'
  $field_bases['field_ag_telephone'] = array(
    'active' => 1,
    'cardinality' => 1,
    'deleted' => 0,
    'entity_types' => array(),
    'field_name' => 'field_ag_telephone',
    'foreign keys' => array(
      'format' => array(
        'columns' => array(
          'format' => 'format',
        ),
        'table' => 'filter_format',
      ),
    ),
    'indexes' => array(
      'format' => array(
        0 => 'format',
      ),
    ),
    'locked' => 0,
    'module' => 'text',
    'settings' => array(
      'allowed_values' => '',
      'allowed_values_php' => '',
      'max_length' => 255,
      'text_processing' => 0,
    ),
    'translatable' => 0,
    'type' => 'text',
    'type_name' => 'agprofile',
  );

  // Exported field_base: 'field_ag_type'
  $field_bases['field_ag_type'] = array(
    'active' => 1,
    'cardinality' => 1,
    'deleted' => 0,
    'entity_types' => array(),
    'field_name' => 'field_ag_type',
    'field_permissions' => array(
      'type' => 0,
    ),
    'foreign keys' => array(
      'tid' => array(
        'columns' => array(
          'tid' => 'tid',
        ),
        'table' => 'taxonomy_term_data',
      ),
    ),
    'indexes' => array(
      'tid' => array(
        0 => 'tid',
      ),
    ),
    'locked' => 0,
    'module' => 'taxonomy',
    'settings' => array(
      'allowed_values' => array(
        0 => array(
          'vocabulary' => 'vocabulary_1',
          'parent' => 0,
          'depth' => '',
        ),
      ),
      'options_list_callback' => 'i18n_taxonomy_allowed_values',
    ),
    'translatable' => 0,
    'type' => 'taxonomy_term_reference',
    'type_name' => 'agdlios',
  );

  // Exported field_base: 'field_ag_url_of_publication'
  $field_bases['field_ag_url_of_publication'] = array(
    'active' => 1,
    'cardinality' => -1,
    'deleted' => 0,
    'entity_types' => array(),
    'field_name' => 'field_ag_url_of_publication',
    'field_permissions' => array(
      'type' => 0,
    ),
    'foreign keys' => array(),
    'indexes' => array(),
    'locked' => 0,
    'module' => 'link',
    'settings' => array(
      'attributes' => array(
        'class' => '',
        'rel' => '',
        'target' => 'default',
      ),
      'display' => array(
        'url_cutoff' => 80,
      ),
      'enable_tokens' => 1,
      'title' => 'optional',
      'title_maxlength' => 128,
      'title_value' => '',
      'url' => 0,
    ),
    'translatable' => 0,
    'type' => 'link_field',
  );

  // Exported field_base: 'field_ag_volume'
  $field_bases['field_ag_volume'] = array(
    'active' => 1,
    'cardinality' => 1,
    'deleted' => 0,
    'entity_types' => array(),
    'field_name' => 'field_ag_volume',
    'field_permissions' => array(
      'type' => 0,
    ),
    'foreign keys' => array(
      'format' => array(
        'columns' => array(
          'format' => 'format',
        ),
        'table' => 'filter_format',
      ),
    ),
    'indexes' => array(
      'format' => array(
        0 => 'format',
      ),
    ),
    'locked' => 0,
    'module' => 'text',
    'settings' => array(
      'allowed_values' => '',
      'allowed_values_php' => '',
      'max_length' => 255,
      'text_processing' => 0,
    ),
    'translatable' => 0,
    'type' => 'text',
    'type_name' => 'agdlios',
  );

  // Exported field_base: 'field_ag_website_url'
  $field_bases['field_ag_website_url'] = array(
    'active' => 1,
    'cardinality' => 1,
    'deleted' => 0,
    'entity_types' => array(),
    'field_name' => 'field_ag_website_url',
    'field_permissions' => array(
      'type' => 0,
    ),
    'foreign keys' => array(),
    'indexes' => array(),
    'locked' => 0,
    'module' => 'link',
    'settings' => array(
      'attributes' => array(
        'class' => '',
        'rel' => '',
        'target' => 'default',
      ),
      'display' => array(
        'url_cutoff' => 80,
      ),
      'enable_tokens' => 1,
      'title' => 'optional',
      'title_maxlength' => 128,
      'title_value' => '',
      'url' => 0,
    ),
    'translatable' => 0,
    'type' => 'link_field',
  );

  // Exported field_base: 'field_ag_working_language'
  $field_bases['field_ag_working_language'] = array(
    'active' => 1,
    'cardinality' => -1,
    'deleted' => 0,
    'entity_types' => array(),
    'field_name' => 'field_ag_working_language',
    'field_permissions' => array(
      'type' => 0,
    ),
    'foreign keys' => array(),
    'indexes' => array(),
    'locked' => 0,
    'module' => 'get_list_from_file',
    'settings' => array(
      'file' => 'languages-new.csv',
    ),
    'translatable' => 0,
    'type' => 'get_list_from_file',
  );

  // Exported field_base: 'field_ag_year'
  $field_bases['field_ag_year'] = array(
    'active' => 1,
    'cardinality' => 1,
    'deleted' => 0,
    'entity_types' => array(),
    'field_name' => 'field_ag_year',
    'foreign keys' => array(),
    'indexes' => array(),
    'locked' => 0,
    'module' => 'date',
    'settings' => array(
      'granularity' => array(
        'year' => 'year',
      ),
      'repeat' => 0,
      'timezone_db' => 'Europe/Paris',
      'todate' => '',
      'tz_handling' => 'none',
    ),
    'translatable' => 0,
    'type' => 'date',
    'type_name' => 'agprofile',
  );

  // Exported field_base: 'field_pages'
  $field_bases['field_pages'] = array(
    'active' => 1,
    'cardinality' => 1,
    'deleted' => 0,
    'entity_types' => array(),
    'field_name' => 'field_pages',
    'field_permissions' => array(
      'type' => 0,
    ),
    'foreign keys' => array(
      'format' => array(
        'columns' => array(
          'format' => 'format',
        ),
        'table' => 'filter_format',
      ),
    ),
    'indexes' => array(
      'format' => array(
        0 => 'format',
      ),
    ),
    'locked' => 0,
    'module' => 'text',
    'settings' => array(
      'max_length' => 20,
    ),
    'translatable' => 0,
    'type' => 'text',
  );

  // Exported field_base: 'field_region'
  $field_bases['field_region'] = array(
    'active' => 1,
    'cardinality' => 1,
    'deleted' => 0,
    'entity_types' => array(),
    'field_name' => 'field_region',
    'foreign keys' => array(),
    'indexes' => array(),
    'locked' => 0,
    'module' => 'computed_field',
    'settings' => array(
      'code' => 'require_once \'sites/default/files/includes/class_fileFolder.php\';
global $locale;

$countries = array();
$file = new fileFolder(\'sites/default/files/txt/countries.csv\');
$records = $file->ArrayFromFile();
foreach ($records as $rec)
{
  $arr = explode(",", $rec);
  $code = $arr[0];
  if ($node->field_country[0][\'value\'] == $code)
    $node_field[0][\'value\'] = $arr[4];
}
if ($node_field[0][\'value\'] == "")
  $node_field[0][\'value\'] = "None";
',
      'data_default' => '',
      'data_length' => 20,
      'data_not_NULL' => 0,
      'data_sortable' => 1,
      'data_type' => 'varchar',
      'database' => array(
        'data_default' => NULL,
        'data_index' => FALSE,
        'data_length' => 32,
        'data_not_NULL' => FALSE,
        'data_precision' => 10,
        'data_scale' => 2,
        'data_size' => 'normal',
        'data_type' => 'varchar',
      ),
      'display_format' => '$display = $node_field_item[\'value\'];',
      'store' => 1,
    ),
    'translatable' => 0,
    'type' => 'computed',
    'type_name' => 'event',
  );

  // Exported field_base: 'taxonomy_vocabulary_3'
  $field_bases['taxonomy_vocabulary_3'] = array(
    'active' => 1,
    'cardinality' => -1,
    'deleted' => 0,
    'entity_types' => array(),
    'field_name' => 'taxonomy_vocabulary_3',
    'indexes' => array(
      'tid' => array(
        0 => 'tid',
      ),
    ),
    'locked' => 0,
    'module' => 'taxonomy',
    'settings' => array(
      'allowed_values' => array(
        0 => array(
          'vocabulary' => 'vocabulary_3',
          'parent' => 0,
        ),
      ),
      'options_list_callback' => 'i18n_taxonomy_allowed_values',
      'required' => FALSE,
    ),
    'translatable' => 0,
    'type' => 'taxonomy_term_reference',
  );

  // Exported field_base: 'upload'
  $field_bases['upload'] = array(
    'active' => 1,
    'cardinality' => -1,
    'deleted' => 0,
    'entity_types' => array(),
    'field_name' => 'upload',
    'field_permissions' => array(
      'type' => 0,
    ),
    'indexes' => array(
      'fid' => array(
        0 => 'fid',
      ),
    ),
    'locked' => 0,
    'module' => 'file',
    'settings' => array(
      'default_file' => 0,
      'display_default' => 1,
      'display_field' => 1,
      'uri_scheme' => 'public',
    ),
    'translatable' => 0,
    'type' => 'file',
  );

  return $field_bases;
}
