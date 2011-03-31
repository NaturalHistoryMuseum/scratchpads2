<?php

/**
* @file
* Views hooks implemented for the Webform module.
*/

function webform_views_data() {
  /**
   * Submissions table definitions.
   */
  $data['webform_submissions']['table']['group'] = t('Webform submissions');
  $data['webform_submissions']['table']['base'] = array(
    'field' => 'sid',
    'title' => t('Webform submissions'),
    'help' => t('Submissions grenerated from Webform forms.'),
  );

  // sid
  $data['webform_submissions']['sid'] = array(
    'title' => t('Sid'),
    'help' => t('The submission ID of the submission.'),
    'field' => array(
      'handler' => 'views_handler_field_numeric',
      'click sortable' => TRUE,
    ),
    'filter' => array(
      'title' => t('Sid'),
      'handler' => 'views_handler_filter_numeric',
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
  );

  // nid
  $data['webform_submissions']['nid'] = array(
    'title' => t('Node'),
    'help' => t('The node these submissions were generated from.'),
    'relationship' => array(
      'base' => 'node',
      'handler' => 'views_handler_relationship',
      'label' => t('Node'),
      'skip base' => array('node', 'node_revisions'),
    ),
  );

  // uid
  $data['webform_submissions']['uid'] = array(
    'title' => t('User'),
    'help' => t('The users who submitted these submissions.'),
    'relationship' => array(
      'base' => 'users',
      'handler' => 'views_handler_relationship',
      'label' => t('User'),
      'skip base' => array('users', 'node', 'node_revisions'),
    ),
  );

  // is_draft
  $data['webform_submissions']['is_draft'] = array(
    'title' => t('Draft'),
    'help' => t('Whether or not the submission is a draft.'),
    'field' => array(
      'handler' => 'webform_handler_field_is_draft',
      'click sortable' => TRUE,
    ),
    'filter' => array(
      'handler' => 'webform_handler_filter_is_draft',
    ),
    'sort' => array(
       'handler' => 'views_handler_sort',
    ),
  );

  // submitted
  $data['webform_submissions']['submitted'] = array(
    'title' => t('Submitted'),
    'help' => t('The date this submission was submitted.'),
    'field' => array(
      'handler' => 'views_handler_field_date',
      'click sortable' => TRUE,
    ),
    'filter' => array(
      'title' => t('Submitted'),
      'handler' => 'views_handler_filter_date',
    ),
    'sort' => array(
      'handler' => 'views_handler_sort_date',
    ),
  );

  // remote_addr
  $data['webform_submissions']['remote_addr'] = array(
    'title' => t('Remote address'),
    'help' => t('The remote IP address of the user that submitted this submission.'),
    'field' => array(
      'handler' => 'views_handler_field',
      'click sortable' => TRUE,
    ),
    'filter' => array(
      'title' => t('Remote address'),
      'handler' => 'views_handler_filter_string',
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
  );

  // view link
  $data['webform_submissions']['view_submission'] = array(
    'field' => array(
      'title' => t('View link'),
      'help' => t('Provide a simple link to view the submission.'),
      'handler' => 'webform_handler_field_submission_link',
      'link_type' => 'view',
    ),
  );

  // edit link
  $data['webform_submissions']['edit_submission'] = array(
    'field' => array(
      'title' => t('Edit link'),
      'help' => t('Provide a simple link to edit the submission.'),
      'handler' => 'webform_handler_field_submission_link',
      'link_type' => 'edit',
    ),
  );

  // delete link
  $data['webform_submissions']['delete_submission'] = array(
    'field' => array(
      'title' => t('Delete link'),
      'help' => t('Provide a simple link to delete the submission.'),
      'handler' => 'webform_handler_field_submission_link',
      'link_type' => 'delete',
    ),
  );

  return $data;
}

/**
 * Implementation of hook_views_data_alter().
 */
function webform_views_data_alter(&$data) {
  // Submission count (node).
  $data['node']['webform_submission_count_node'] = array(
    'field' => array(
      'title' => t('Webform submission count'),
      'help' => t('The number of webform submissions on this node.'),
      'handler' => 'webform_handler_field_submission_count',
      'count_type' => 'node',
    ),
  );

  // Submission count (user).
  $data['users']['webform_submission_count_user'] = array(
    'field' => array(
      'title' => t('Webform submission count'),
      'help' => t('The number of webform submissions for this user.'),
      'handler' => 'webform_handler_field_submission_count',
      'count_type' => 'users',
    ),
  );

  // Link for editing the webform.
  $data['node']['webform_edit'] = array(
    'field' => array(
      'title' => t('Webform edit link'),
      'help' => t('Provide a simple link to edit the webform components and configuration.'),
      'handler' => 'webform_handler_field_node_link_edit',
    ),
  );

  // Link for viewing webform results.
  $data['node']['webform_results'] = array(
    'field' => array(
      'title' => t('Webform results link'),
      'help' => t('Provide a simple link to view the results of a webform.'),
      'handler' => 'webform_handler_field_node_link_results',
    ),
  );

  // Webform form content.
  $data['node']['webform_form_body'] = array(
    'field' => array(
      'title' => t('Webform form body'),
      'help' => t('The Webform form body display for this node.'),
      'handler' => 'webform_handler_field_form_body',
    ),
  );
}

/**
 * Implementation of hook_views_handlers().
 */
function webform_views_handlers() {
  return array(
    'info' => array(
      'path' => drupal_get_path('module', 'webform') . '/views',
    ),
    'handlers' => array(
      'webform_handler_field_submission_link' => array(
        'parent' => 'views_handler_field',
        'file' => 'webform_handler_field_submission_link.inc',
      ),
      'webform_handler_field_submission_count' => array(
        'parent' => 'views_handler_field',
        'file' => 'webform_handler_field_submission_count.inc',
      ),
      'webform_handler_field_node_link_edit' => array(
        'parent' => 'views_handler_field_node_link',
      ),
      'webform_handler_field_node_link_results' => array(
        'parent' => 'views_handler_field_node_link',
      ),
      'webform_handler_field_form_body' => array(
        'parent' => 'views_handler_field',
        'file' => 'webform_handler_field_form_body.inc',
      ),
      'webform_handler_field_is_draft' => array(
        'parent' => 'views_handler_field',
        'file' => 'webform_handler_field_is_draft.inc',
      ),
      'webform_handler_filter_is_draft' => array(
        'parent' => 'views_handler_filter_in_operator',
        'file' => 'webform_handler_filter_is_draft.inc',
      ),
    ),
  );
}