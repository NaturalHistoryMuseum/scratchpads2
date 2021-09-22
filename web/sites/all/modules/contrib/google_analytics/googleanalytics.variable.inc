<?php

/**
 * @file
 * Definition of variables for Variable API module.
 */

/**
 * Implements hook_variable_info().
 */
function googleanalytics_variable_info($options) {
  $variables['googleanalytics_account'] = array(
    'type' => 'string',
    'title' => t('Web Property ID', array(), $options),
    'default' => 'UA-',
    'description' => t('This ID is unique to each site you want to track separately, and is in the form of UA-xxxxxxx-yy. To get a Web Property ID, <a href="@analytics">register your site with Google Analytics</a>, or if you already have registered your site, go to your Google Analytics Settings page to see the ID next to every site profile. <a href="@webpropertyid">Find more information in the documentation</a>.', array('@analytics' => 'http://www.google.com/analytics/', '@webpropertyid' => url('https://developers.google.com/analytics/resources/concepts/gaConceptsAccounts', array('fragment' => 'webProperty'))), $options),
    'required' => TRUE,
    'group' => 'googleanalytics',
    'localize' => TRUE,
    'multidomain' => TRUE,
    'validate callback' => 'googleanalytics_validate_googleanalytics_account',
  );

  return $variables;
}

/**
 * Implements hook_variable_group_info().
 */
function googleanalytics_variable_group_info() {
  $groups['googleanalytics'] = array(
    'title' => t('Google Analytics'),
    'description' => t('Configure tracking behavior to get insights into your website traffic and marketing effectiveness.'),
    'access' => 'administer google analytics',
    'path' => array('admin/config/system/googleanalytics'),
  );

  return $groups;
}

/**
 * Validate Web Property ID variable.
 */
function googleanalytics_validate_googleanalytics_account($variable) {
  // Replace all type of dashes (n-dash, m-dash, minus) with the normal dashes.
  $variable['value'] = str_replace(array('–', '—', '−'), '-', $variable['value']);

  if (!preg_match('/^UA-\d{4,}-\d+$/', $variable['value'])) {
    return t('A valid Google Analytics Web Property ID is case sensitive and formatted like UA-xxxxxxx-yy.');
  }
}
