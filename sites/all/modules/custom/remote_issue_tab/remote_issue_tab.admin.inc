<?php

/**
 * Form for selecting the provider for the issue quque
 */
function remote_issue_tab_admin_form($form, &$form_state)
{
  // Todo: Make this include automatic
    module_load_include('inc', 'remote_issue_tab', 'remote_issue_tab.github');

    $providers = module_invoke_all('remote_issue_provider');

    $form['provider_options'] = array(
    '#type' => 'value',
    '#value' => $providers
    );
    $form['provider'] = array(
    '#title' => t('Remote issue provider'),
    '#type' => 'select',
    '#description' => "The service that provides your issue queue.",
    '#options' => $form['provider_options']['#value'],
    '#default_value' => variable_get('remote_issue_tab_provider', 'github')
    );
    $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save')
    );

    return $form;
}

/**
 * Save the issue queue provider
 */
function remote_issue_tab_admin_form_submit($form, &$form_state)
{
    variable_set('remote_issue_tab_provider', $form_state['values']['provider']);
}
