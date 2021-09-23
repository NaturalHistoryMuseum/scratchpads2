<?php

/**
 * @file
 *
 * All the theme functions that this module uses.  These can easily be
 * overridden by a theme/other module if wanted.
 */
/**
 * Main theme that creates the whole block content.
 */
function theme_remote_issue_tab_block($variables = array()){
  // Add the CSS & js - defined in remote_issue_tab_library() so we can add the ui.tabs dependency
  drupal_add_library('remote_issue_tab', 'remote_issue_tab');

  foreach ($variables['external_scripts'] as $script) {
    drupal_add_js($script, "external");
  }

  foreach ($variables['scripts'] as $script) {
    drupal_add_js($script);
  }

  drupal_add_js(array('remote_issue_tab' => array(
    'fetch_url' => $variables['fetch_url'],
    'provider_module' => $variables['provider_module']
  )), 'setting');

  return '<div id="remote-issue-tab">
 <div>
  ' . theme_remote_issue_tab_create_issue($variables) . '
  <div class="header">'. $variables['header'] . '</div>
 </div>
 ' . theme_remote_issue_tab_list_issues($variables) . '
 <div class="footer">
  ' . $variables['footer'] . '
 </div>
</div>';
}

/**
 * Create issue-only theme
 */
function theme_remote_issue_tab_create_issue($variables = array())
{
  $form = drupal_get_form('remote_issue_tab_create_form');

    return '<h2>' . l(t('Report an issue'), $variables['html_link']) . '</h2>
  <div class="header">' . drupal_render($form) . '</div>';
}

/**
 * List issues-only theme
 */
function theme_remote_issue_tab_list_issues($variables = array())
{
    return '<div class="no-js">
  This module is not available in your browser.
  To browse or create issues please use the '. l('issue queue web page', $variables['html_link']) .'.
 </div>
 <div id="issuesTabs">
    <ul>
        <li><a href="#issuesTabOpen">Open</a></li>
        <li><a href="#issuesTabClosed">Closed</a></li>
    </ul>
    <div class="items" id="issuesTabOpen">
        <h2>Recently Opened Issues:</h2>
        <div class="loading">
        Loading issues...
        </div>
        <div class="empty">
        There are no issues to display.
        </div>
        <ul class="issues"></ul>
    </div>
    <div class="items" id="issuesTabClosed">
        <h2>Recently Closed Issues:</h2>
        <div class="loading">
        Loading issues...
        </div>
        <div class="empty">
        There are no issues to display.
        </div>
        <ul class="issues"></ul>    
    </div>
</div>';
}
