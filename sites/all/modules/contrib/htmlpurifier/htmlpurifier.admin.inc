<?php

/**
 * @file
 * Administrative page callbacks for the HTML Purifier module.
 */

/**
 * Generate a form for configuring overall HTML Purifier settings.
 *
 * @ingroup forms
 * @see htmlpurifier_admin_settings_submit()
 */
function htmlpurifier_admin_settings($form, &$form_state) {
  $form['htmlpurifier_introduction'] = array(
    '#markup' => t('<p>This page contains global settings for all HTML Purifier enabled filters.  If you are looking for specific filter configuration options, check <a href="@format">the format configurations page</a> and select the specific format you would like to configure.</p>', array('@filter' => url('admin/config/content/formats'))),
  );
  $form['htmlpurifier_clear_cache'] = array(
    '#type' => 'submit',
    '#value' => t('Clear cache (Warning: Can result in performance degradation)'),
    '#submit' => array('_htmlpurifier_clear_cache')
  );

  $form = system_settings_form($form);
  $form['#submit'][] = 'htmlpurifier_admin_settings_submit';
  return $form;
}

/**
 * Submit handler for the HTML Purifier settings form.
 */
function htmlpurifier_admin_settings_submit($form, &$form_state) {
  // Resave all text formats so that the new cache settings for each format are
  // recorded.
  // TODO: There should be a better way to do this.
  foreach (filter_formats() as $format) {
    $format->filters = filter_list_format($format->format);
    foreach ($format->filters as &$filter) {
      $filter = (array) $filter;
    }
    filter_format_save($format);
  }
}
