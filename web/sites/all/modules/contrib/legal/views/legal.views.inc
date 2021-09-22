<?php

/**
 * @file
 * Views handler and plugin definitions for the legal module.
 */

/**
 * Implements hook_views_data().
 */
function legal_views_data() {
  $data                                     = array();
  $data['legal_accepted']['table']['group'] = t('Legal');

  $data['legal_accepted']['table']['join']['users'] = array(
    'left_field' => 'uid',
    'field'      => 'uid',
  );

  $data['users']['legal_accepted_rel'] = array(
    'real field'   => 'uid',
    'title'        => t('accepted user'),
    'group'        => t('Legal'),
    'help'         => t('An accepted user'),
    'relationship' => array(
      'handler'            => 'views_handler_relationship',
      'base'               => 'legal_accepted',
      'base field'         => 'uid',
      'label'              => t('Legal'),
      'relationship field' => 'uid',
    ),
  );

  $data['legal_accepted']['legal_conditions_rel'] = array(
    'group'        => t('Legal'),
    'title'        => t('Legal Conditions relationship'),
    'help'         => t('Create a relationship to a legal conditions and legal accepted.'),
    'relationship' => array(
      'handler'            => 'views_handler_relationship',
      'base'               => 'legal_conditions',
      'base field'         => 'tc_id',
      'label'              => t('Legal'),
      'relationship field' => 'tc_id',
    ),
  );

  $data['legal_accepted']['version'] = array(
    'title'    => t('Version'),
    'help'     => t('Version ID accepted.'),
    'field'    => array(
      'handler'        => 'views_handler_field',
      'click sortable' => TRUE,
    ),
    'filter'   => array(
      'handler'     => 'views_handler_filter_numeric',
      'allow empty' => TRUE,
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_numeric',
    ),
    'sort'     => array(
      'handler' => 'views_handler_sort',
    ),
  );

  $data['legal_accepted']['revision'] = array(
    'title'    => t('Revision'),
    'help'     => t('Revision ID accepted.'),
    'field'    => array(
      'handler'        => 'views_handler_field',
      'click sortable' => TRUE,
    ),
    'filter'   => array(
      'handler'     => 'views_handler_filter_numeric',
      'allow empty' => TRUE,
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_numeric',
    ),
    'sort'     => array(
      'handler' => 'views_handler_sort',
    ),
  );

  if (module_exists('locale')) {
    $data['legal_accepted']['language'] = array(
      'title'    => t('Language'),
      'help'     => t('Language of T&C accepted by user.'),
      'field'    => array(
        'handler'        => 'views_handler_field_locale_language',
        'click sortable' => TRUE,
      ),
      'filter'   => array(
        'handler'     => 'views_handler_filter_locale_language',
        'allow empty' => TRUE,
      ),
      'argument' => array(
        'handler' => 'views_handler_argument_locale_language',
      ),
      'sort'     => array(
        'handler' => 'views_handler_sort',
      ),
    );
  }

  $data['legal_accepted']['uid'] = array(
    'title'        => t('User ID'),
    'help'         => t('User ID of user that accepted.'),
    'field'        => array(
      'handler'        => 'views_handler_field_user',
      'click sortable' => TRUE,
    ),
    'filter'       => array(
      'handler'     => 'views_handler_filter_user_name',
      'allow empty' => TRUE,
    ),
    'argument'     => array(
      'handler' => 'views_handler_argument_user_uid',
    ),
    'sort'         => array(
      'handler' => 'views_handler_sort',
    ),
    'relationship' => array(
      'base'       => 'users',
      'base_field' => 'uid',
      'handler'    => 'views_handler_relationship',
      'label'      => t('Acceptances to user relationship'),
    ),
  );

  $data['legal_accepted']['accepted'] = array(
    'title'  => t('Time Accepted'),
    'help'   => t('Time and date user accepted T&Cs.'),
    'field'  => array(
      'handler'        => 'views_handler_field_date',
      'click sortable' => TRUE,
    ),
    'filter' => array(
      'handler'     => 'views_handler_filter_date',
      'allow empty' => TRUE,
    ),
    'sort'   => array(
      'handler' => 'views_handler_sort_date',
    ),
  );

  $data['legal_conditions'] = array(
    'version'    => array(
      'title'    => t('Version'),
      'help'     => t('Version of T&Cs.'),
      'field'    => array(
        'handler'        => 'views_handler_field',
        'click sortable' => TRUE,
      ),
      'filter'   => array(
        'handler'     => 'views_handler_filter_numeric',
        'allow empty' => TRUE,
      ),
      'argument' => array(
        'handler' => 'views_handler_argument_numeric',
      ),
      'sort'     => array(
        'handler' => 'views_handler_sort',
      ),
    ),
    'revision'   => array(
      'title'    => t('Revision'),
      'help'     => t('Revision of T&Cs.'),
      'field'    => array(
        'handler'        => 'views_handler_field',
        'click sortable' => TRUE,
      ),
      'filter'   => array(
        'handler'     => 'views_handler_filter_numeric',
        'allow empty' => TRUE,
      ),
      'argument' => array(
        'handler' => 'views_handler_argument_numeric',
      ),
      'sort'     => array(
        'handler' => 'views_handler_sort',
      ),
    ),
    'conditions' => array(
      'title'    => t('Terms & Conditions'),
      'help'     => t('Text of T&Cs.'),
      'field'    => array(
        'handler'        => 'views_handler_field',
        'click sortable' => TRUE,
      ),
      'filter'   => array(
        'handler'     => 'views_handler_filter_string',
        'allow empty' => TRUE,
      ),
      'argument' => array(
        'handler' => 'views_handler_argument_string',
      ),
      'sort'     => array(
        'handler' => 'views_handler_sort',
      ),
    ),
    'date'       => array(
      'title'  => t('Date Added'),
      'help'   => t('Date T&Cs where added'),
      'field'  => array(
        'handler'        => 'views_handler_field_date',
        'click sortable' => TRUE,
      ),
      'filter' => array(
        'handler'     => 'views_handler_filter_date',
        'allow empty' => TRUE,
      ),
      'sort'   => array(
        'handler' => 'views_handler_sort_date',
      ),
    ),
    'extras'     => array(
      'title'    => t('Additional Checkboxes'),
      'help'     => t('Additional terms of T&Cs'),
      'field'    => array(
        'handler'        => 'legal_views_handler_field_unserialized_list',
        'click sortable' => TRUE,
      ),
      'filter'   => array(
        'handler'     => 'views_handler_filter_string',
        'allow empty' => TRUE,
      ),
      'argument' => array(
        'handler' => 'views_handler_argument_string',
      ),
      'sort'     => array(
        'handler' => 'views_handler_sort',
      ),
    ),
    'changes'    => array(
      'title'    => t('Changes'),
      'help'     => t('Explanation of changes to T&Cs'),
      'field'    => array(
        'handler'        => 'legal_views_handler_field_exploded_list',
        'click sortable' => TRUE,
      ),
      'filter'   => array(
        'handler'     => 'views_handler_filter_string',
        'allow empty' => TRUE,
      ),
      'argument' => array(
        'handler' => 'views_handler_argument_string',
      ),
      'sort'     => array(
        'handler' => 'views_handler_sort',
      ),
    ),
    'table'      => array(
      'group' => t('Legal'),
      'base'  => array(
        'field'    => 'tc_id',
        'title'    => t('Legal terms and conditions'),
        'help'     => 'Versions of the terms and conditions text',
        'weight'   => 10,
        'database' => 'default',
      ),
    ),
  );

  if (module_exists('locale')) {

    $data['legal_conditions']['language'] = array(
      'title'    => t('Language'),
      'help'     => t('Language of T&Cs.'),
      'field'    => array(
        'handler'        => 'views_handler_field_locale_language',
        'click sortable' => TRUE,
      ),
      'filter'   => array(
        'handler'     => 'views_handler_filter_locale_language',
        'allow empty' => TRUE,
      ),
      'argument' => array(
        'handler' => 'views_handler_argument_locale_language',
      ),
      'sort'     => array(
        'handler' => 'views_handler_sort',
      ),
    );
  }

  return $data;
}

/**
 * Implements hook_views_handlers();
 */
function legal_views_handlers() {
  return array(
    'info'     => array(
      'path' => drupal_get_path('module', 'legal') . '/views',
    ),
    'handlers' => array(
      'legal_views_handler_field_unserialized_list' => array(
        'parent' => 'views_handler_field',
      ),
      'legal_views_handler_field_exploded_list'     => array(
        'parent' => 'views_handler_field',
      ),
    ),
  );
}
