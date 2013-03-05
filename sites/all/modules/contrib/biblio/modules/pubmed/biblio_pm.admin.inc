<?php
/*
 * @file biblio_pm.admin.inc
 *
 */

/**
 * @return multitype:string NULL multitype:NULL  multitype:multitype:multitype:boolean
 */
function biblio_pm_settings_form() {
  $form = array();

  $form['pm_options'] = array(
    '#type' => 'fieldset',
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
    '#group'     => 'biblio_settings',
    '#title' => t('PubMed'),
    '#description' => t('Please select the action to be performed by the PubMed module when it detects changes to an existing entry.'),
    '#weight' => 105,
  );
  $form['pm_options']['biblio_pm_dup_action'] = array(
    '#type' => 'radios',
    '#title' => t('Actions'),
    '#default_value' => variable_get('biblio_pm_dup_action', 'newrev'),
    '#options'  => array(
      'newrev'  => t('Accept and create a new revision of the existing node.'),
      'replace' => t('Accept and replace the existing node.'),
      'reject'  => t('Reject and keep the existing node.'),
    )
  );
  $form['pm_options']['biblio_pm_auto_update'] = array(
      '#type' => 'checkbox',
      '#title' => t('Automatically check for updates via CRON'),
      '#return_value' => 1,
      '#default_value' => variable_get('biblio_pm_auto_update', 0),
      '#description' => t('Entries which were orginally downloaded from PubMed will be periodically checked and updated if the source record has changed.')
  );
  $form['pm_options']['pm_cron'] = array(
      '#type' => 'fieldset',
      '#collapsible' => FALSE,
      '#collapsed' => FALSE,
//      '#group'     => 'pm_options',
      '#title' => t('CRON Settings'),
  );
  $form['pm_options']['pm_cron']['biblio_pm_update_interval'] = array(
      '#type' => 'select',
      '#title' => t('PubMed update frequency'),
      '#default_value' => variable_get('biblio_pm_update_interval', 3600),
      '#options' => array(
          0 => t('Every CRON run'),
          3600 => t('Hourly'),
          86400 => t('Daily'),
          604800 => t('Weeekly'),
      ),
      '#description' => t('How frequently should we check for updates.'),
      '#states' => array(
          'invisible' => array(
              'input[name="biblio_pm_auto_update"]' => array('checked' => FALSE),
          ),
      ),
  );
  $form['pm_options']['pm_cron']['biblio_pm_update_limit'] = array(
      '#type' => 'select',
      '#title' => t('Number of items to check per CRON run'),
      '#default_value' => variable_get('biblio_pm_update_limit', 100),
      '#options' => array(
          10 => 10,
          20 => 20,
          50 => 50,
          100 => 100,
          500 => 500,
      ),
      '#description' => t('The maximum number of items updated in each pass of a cron maintenance task. If necessary, reduce the number of items to prevent timeouts and memory errors while updating.'),
      '#states' => array(
          'invisible' => array(
              'input[name="biblio_pm_auto_update"]' => array('checked' => FALSE),
          ),
      ),
  );
  $form['pm_options']['pm_cron']['biblio_pm_age_limit'] = array(
      '#type' => 'select',
      '#title' => t('Length of time to wait before re-checking for updates'),
      '#default_value' => variable_get('biblio_pm_age_limit', 2419200),
      '#options' => array(
          86400 => t('1 Day'),
          604800 => t('1 Week'),
          2419200 => t('1 Month'),
          29030400 => t('1 Year'),
      ),
      '#description' => t('Wait this long before checking for updates on a given node again.'),
      '#states' => array(
          'invisible' => array(
              'input[name="biblio_pm_auto_update"]' => array('checked' => FALSE),
          ),
      ),
  );


   return ($form);
}