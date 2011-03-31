<?php
/*
 * @file biblio_pm.admin.inc
 *
 */

function biblio_pm_settings_form() {
  $form = array();
  
  $form['pm_options'] = array(
    '#type' => 'fieldset',
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
    '#group'     => 'biblio_settings',
    '#title' => t('PubMed'),
    '#description' => t('Please select the action to be performed by the PubMed module when it detects changes to an existing entry.')
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
   return ($form);
}