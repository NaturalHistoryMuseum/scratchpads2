<?php

/**
 * @file
 * languagefield.views.inc
 */

/**
 * Implements hook_views_data().
 */
function languagefield_views_data() {
  $data['languages']['table']['group'] = t('Languages');

  $data['languages']['table']['base'] = array(
    'field' => 'language',
    'title' => t('Language'),
    'help' => t('Table containing site languages.'),
    'weight' => -10,
  );

  $data['languages']['table']['join'] = array(
    'node' => array(
      'left_field' => 'language',
      'field' => 'language',
    ),
    'user' => array(
      'left_field' => 'language',
      'field' => 'language',
    ),
    'url_alias' => array(
      'left_field' => 'language',
      'field' => 'language',
    ),
  );

  $data['languages']['language'] = array(
    'title' => t('Language code'),
    'help' => t("Language code, e.g. 'de' or 'en-US'."),
    'field' => array(
      'handler' => 'views_handler_field',
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
    'relationship' => array(
      'base' => 'node',
      'field' => 'language',
      'handler' => 'views_handler_relationship',
      'label' => t('Node'),
    ),
  );

  $data['languages']['name'] = array(
    'title' => t('Language name'),
    'help' => t('Language name in English.'),
    'field' => array(
      'handler' => 'views_handler_field',
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

  $data['languages']['native'] = array(
    'title' => t('Native language name'),
    'help' => t('Native language name.'),
    'field' => array(
      'handler' => 'views_handler_field',
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

  $data['languages']['direction'] = array(
    'title' => t('Language direction'),
    'help' => t('Direction of language.'),
    'field' => array(
      'handler' => 'views_handler_field_numeric',
      'click sortable' => TRUE,
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_numeric',
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
  );

  $data['languages']['enabled'] = array(
    'title' => t('Enabled'),
    'help' => t('Whether or not the language is enabled.'),
    'field' => array(
      'handler' => 'views_handler_field_boolean',
      'click sortable' => TRUE,
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_boolean_operator',
      'label' => t('Enabled'),
      'type' => 'yes-no',
      'use equal' => FALSE,
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
  );

  $data['languages']['plurals'] = array(
    'title' => t('Plurals'),
    'help' => t('Number of plural indexes in this language.'),
    'field' => array(
      'handler' => 'views_handler_field_numeric',
      'click sortable' => TRUE,
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_numeric',
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
  );

  $data['languages']['formula'] = array(
    'title' => t('Formula'),
    'help' => t('Plural formula in PHP code to evaluate to get plural indexes.'),
    'field' => array(
      'handler' => 'views_handler_field',
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

  $data['languages']['domain'] = array(
    'title' => t('Domain'),
    'help' => t('Domain to use for this language.'),
    'field' => array(
      'handler' => 'views_handler_field',
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

  $data['languages']['prefix'] = array(
    'title' => t('Path prefix'),
    'help' => t('Path prefix to use for this language.'),
    'field' => array(
      'handler' => 'views_handler_field',
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

  $data['languages']['weight'] = array(
    'title' => t('Weight'),
    'help' => t('Weight, used in lists of languages.'),
    'field' => array(
      'handler' => 'views_handler_field_numeric',
      'click sortable' => TRUE,
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_numeric',
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
  );

  $data['languages']['javascript'] = array(
    'title' => t('JavaScript'),
    'help' => t('Location of JavaScript translation file.'),
    'field' => array(
      'handler' => 'views_handler_field',
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

  return $data;
}

/**
 * Implements hook_views_data_alter().
 *
 * @param $data
 */
function languagefield_views_data_alter(&$data) {
  $languagefields = array();
  foreach (field_info_fields() as $field_name => $field) {
    if ($field['module'] == 'languagefield') {
      $languagefields[$field_name] = $field;
    }
  }
  foreach ($data as $field_name => $field) {
    foreach (array_keys($languagefields) as $name) {
      // Check on field storage type prevents warnings on non-sql field storage.
      if (isset($field[$name]) && $languagefields[$name]['storage']['type'] == 'field_sql_storage') {
        $title = empty($data[$field_name][$name]['title']) ? '' : $data[$field_name][$name]['title'] . ' ';
        $data[$field_name][$name]['help'] = $title . t('If you need more fields add a language relationship.');
      }
    }
  }
}

/**
 * Implements hook_field_views_data_alter().
 */
function languagefield_field_views_data_alter(&$result, &$field, &$module) {
  // This is a fix for languagefield module.
  if ($field['type'] == 'language_field') {
    $field_name = $field['field_name'] . '_value';

    foreach ($result as &$table_name) {
      if (isset($table_name[$field_name])) {
        if (isset($table_name[$field_name]['filter'])) {
          $table_name[$field_name]['filter']['handler'] = 'views_handler_filter_locale_language';
        }
        $table_name[$field_name]['relationship'] = array(
          'handler' => 'views_handler_relationship',
          'base' => 'languages',
          'base field' => 'language',
          'label' => t('Language from !field_name', array('!field_name' => $field['field_name'])),
          'help' => t('Provides access to all of the language properties.'),
          'languages' => array(
            'field' => $field['field_name'],
          ),
        );
      }
    }
  }
}
