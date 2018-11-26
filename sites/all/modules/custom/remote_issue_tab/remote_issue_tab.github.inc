<?php

// Todo: Hook into requirements to make sure the github class is installed

/**
 * Implements hook_remote_issue_provider
 * We return an array of provider names, keyed by an identifier we will use in function names
 */
function remote_issue_tab_remote_issue_provider()
{
    return array(
    'github' => 'GitHub'
    );
}

/**
 * Implements hook_PROVIDER_admin_form
 * Any extra settings we may need for the github provider - auth key and repo
 */
function remote_issue_tab_github_admin_form($form, &$form_state)
{
    $form['repository'] = array(
    '#type' => 'textfield',
    '#title' => t('Repository owner/name'),
    '#required' => true,
    '#value' => variable_get('remote_issue_tab_github_repository')
    );
    $form['auth_key'] = array(
    '#type' => 'textfield',
    '#title' => t('Github Authentication Key'),
    '#required' => true,
    '#value' => variable_get('remote_issue_tab_github_auth_key')
    );
    $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save')
    );

    return $form;
}

// Todo: Add verify handler for these settings - user should exist & have access to repo

/**
 * Submit handler for github settings form
 */
function remote_issue_tab_github_admin_form_submit($form, &$form_state)
{
    $repository = $form_state['values']['repository'];
    $auth_key = $form_state['values']['auth_key'];

    variable_set('remote_issue_tab_github_repository', $repository);
    variable_set('remote_issue_tab_github_auth_key', $auth_key);
}

/**
 * Implements hook_PROVIDER_create_issue
 * Where we submit the values to github
 * $values = values submitted by form
 * By default it contains issue_title and issue_body
 * Returns true on success or false on failure
 */
function remote_issue_tab_github_create_issue($values)
{
    $auth_token = variable_get('remote_issue_tab_github_auth_key');
    $repo = variable_get('remote_issue_tab_github_repository');
    $title = $values['issue_title'];
    $body = $values['issue_body'];

    list($repo_owner, $repo_name) = explode('/', $repo);

    require_once libraries_get_path('vendor') . '/autoload.php';

    $client = new \Github\Client();
    $client->authenticate($auth_token, null, \Github\Client::AUTH_HTTP_TOKEN);

    try {
        $issue = $client->api('issue')->create($repo_owner, $repo_name, array(
        'title' => $title,
        'body' => $body
        ));

        module_invoke_all('github_create_issue', $issue);
    } catch (Exception $e) {
      //echo $e->getMessage();
        return false;
    }

    return true;
}
