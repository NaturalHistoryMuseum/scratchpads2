<?php

/**
 * @file
 * Provide views data and handlers for the Taxonomy revision module.
 */

/**
 * Implements hook_views_data().
 */
function taxonomy_revision_views_data() {

  $data['taxonomy_term_data']['table']['join'] = array(
    'taxonomy_term_data_revision' => array(
      'left_field' => 'tid',
      'field' => 'tid',
      'type' => 'INNER',
    ),
  );

  $data['taxonomy_term_data_revision']['table']['group']  = t('Taxonomy term revision');
  $data['taxonomy_term_data_revision']['table']['base'] = array(
    'field' => 'revision_id',
    'title' => t('Taxonomy term revision'),
    'help' => t('Taxonomy term revisions are a history of changes to taxonomy terms.'),
  );

  // For other base tables, explain how we join.
  $data['taxonomy_term_data_revision']['table']['join'] = array(
    'terms' => array(
      'left_field' => 'revision_id',
      'field' => 'revision_id',
    ),
  );

  // Uid field for user revision.
  $data['taxonomy_term_data_revision']['uid'] = array(
    'title' => t('User'),
    'help' => t('Relate a taxonomy term revision to the user who created the revision.'),
    'relationship' => array(
      'handler' => 'views_handler_relationship',
      'base' => 'users',
      'base field' => 'uid',
      'label' => t('Revision user'),
    ),
  );

  $data['taxonomy_term_data_revision']['revision_id'] = array(
    'title' => t('Revision id'),
    'help' => t('The revision ID of the taxonomy term revision.'),
    'field' => array(
      'click sortable' => TRUE,
    ),
    // Information for accepting a nid as an argument.
    'argument' => array(
      'handler' => 'views_handler_argument_taxonomy_term_revision_id',
      'click sortable' => TRUE,
      'numeric' => TRUE,
    ),
    // Information for accepting a uid as a filter.
    'filter' => array(
      'handler' => 'views_handler_filter_numeric',
    ),
    // Information for sorting on a uid.
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
    'relationship' => array(
      'handler' => 'views_handler_relationship',
      'base' => 'terms',
      'base field' => 'revision_id',
      'title' => t('Taxonomy term'),
      'label' => t('Get the actual term from a taxonomy term revision.'),
    ),
  );

  $data['taxonomy_term_data_revision']['tid'] = array(
    'title' => t('Tid'),
    'help' => t('The tid field of the taxonomy term revisions table'),
    'relationship' => array(
      'handler' => 'views_handler_relationship',
      'base' => 'terms',
      'base field' => 'tid',
      'title' => t('Taxonomy term'),
      'label' => t('Get all revisions from an user.'),
    ),
  );

  $data['taxonomy_term_data_revision']['name'] = array(
    'title' => t('Name'),
    'help' => t('The user name.'),
     // Information for displaying a title as a field.
    'field' => array(
      'field' => 'name',
      'handler' => 'views_handler_field_taxonomy_term_revision',
      'click sortable' => TRUE,
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_string',
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_string',
    ),
  );

  $data['taxonomy_term_data_revision']['log'] = array(
    'title' => t('Log message'),
    'help' => t('The log message entered when the revision was created.'),
    'field' => array(
      'handler' => 'views_handler_field_xss',
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_string',
    ),
  );

  $data['taxonomy_term_data_revision']['timestamp'] = array(
    'title' => t('Created date'),
    'help' => t('The date the taxonomy term revision was created.'),
    'field' => array(
      'handler' => 'views_handler_field_date',
      'click sortable' => TRUE,
    ),
    'sort' => array(
      'handler' => 'views_handler_sort_date',
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_date',
    ),
  );

  $data['taxonomy_term_data_revision']['revert_revision'] = array(
    'field' => array(
      'real field' => 'tid',
      'title' => t('Revert link'),
      'help' => t('Provide a simple link to revert to the revision.'),
      'handler' => 'views_handler_field_taxonomy_term_revision_link_revert',
    ),
  );

  $data['taxonomy_term_data_revision']['delete_revision'] = array(
    'field' => array(
      'real field' => 'tid',
      'title' => t('Delete link'),
      'help' => t('Provide a simple link to delete the taxonomy term revision.'),
      'handler' => 'views_handler_field_taxonomy_term_revision_link_delete',
    ),
  );

  return $data;
}
