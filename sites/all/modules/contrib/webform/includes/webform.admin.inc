<?php

/**
 * @file
 * Administration pages provided by Webform module.
 */

/**
 * Menu callback for admin/config/content/webform.
 */
function webform_admin_settings() {
  module_load_include('inc', 'webform', 'includes/webform.export');

  $form['components'] = array(
    '#type' => 'fieldset',
    '#title' => t('Available components'),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
    '#description' => t('These are the available field types for your installation of Webform. You may disable any of these components by unchecking its corresponding box. Only checked components will be available in existing or new webforms.'),
  );

  // Add each component to the form:
  $form['components'] = array('#tree' => TRUE);
  $component_types = webform_components(TRUE);
  foreach ($component_types as $key => $component) {
    $form['components'][$key] = array(
      '#title' => $component['label'],
      '#description' => $component['description'],
      '#type' => 'checkbox',
      '#return_value' => 1,
      '#default_value' => $component['enabled'],
    );
  }

  $form['email'] = array(
    '#type' => 'fieldset',
    '#title' => t('Default e-mail values'),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
  );

  $form['email']['webform_default_from_address']  = array(
    '#type' => 'textfield',
    '#title' => t('From address'),
    '#default_value' => webform_variable_get('webform_default_from_address'),
    '#description' => t('The default sender address for emailed webform results; often the e-mail address of the maintainer of your forms.'),
  );

  $form['email']['webform_default_from_name']  = array(
    '#type' => 'textfield',
    '#title' => t('From name'),
    '#default_value' => webform_variable_get('webform_default_from_name'),
    '#description' => t('The default sender name which is used along with the default from address.'),
  );

  $form['email']['webform_default_subject']  = array(
    '#type' => 'textfield',
    '#title' => t('Default subject'),
    '#default_value' => webform_variable_get('webform_default_subject'),
    '#description' => t('The default subject line of any e-mailed results.'),
  );

  $form['email']['webform_email_replyto']  = array(
    '#type' => 'checkbox',
    '#title' => t('Use Reply-To header'),
    '#default_value' => webform_variable_get('webform_email_replyto'),
    '#description' => t('Sends all e-mail from the domain of the default address above and sets the "Reply-To" header to the actual sender. Helps prevent e-mail from being flagged as spam.'),
  );

  $form['email']['webform_email_html_capable']  = array(
    '#type' => 'checkbox',
    '#title' => t('HTML mail system'),
    '#default_value' => webform_variable_get('webform_email_html_capable'),
    '#description' => t('Whether the mail system configured for webform is capable of sending mail in HTML format.'),
  );

  $form['email']['webform_default_format']  = array(
    '#type' => 'radios',
    '#title' => t('Format'),
    '#options' => array(
      0 => t('Plain text'),
      1 => t('HTML'),
    ),
    '#default_value' => webform_variable_get('webform_default_format'),
    '#description' => t('The default format for new e-mail settings. Webform e-mail options take precedence over the settings for system-wide e-mails configured in MIME mail.'),
    '#states' => array(
      'visible' => array(
        ':input[name="webform_email_html_capable"]' => array('checked' => TRUE),
      ),
    ),
  );

  $form['email']['webform_format_override']  = array(
    '#type' => 'radios',
    '#title' => t('Format override'),
    '#options' => array(
      0 => t('Per-webform configuration of e-mail format'),
      1 => t('Send all e-mails in the default format'),
    ),
    '#default_value' => webform_variable_get('webform_format_override'),
    '#description' => t('Force all webform e-mails to be sent in the default format.'),
    '#states' => array(
      'visible' => array(
        ':input[name="webform_email_html_capable"]' => array('checked' => TRUE),
      ),
    ),
  );

  $form['progressbar'] = array(
    '#type' => 'fieldset',
    '#title' => t('Progress bar'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );
  $form['progressbar']['webform_progressbar_style']  = array(
    '#type' => 'checkboxes',
    '#title' => t('Progress bar style'),
    '#options' => array(
      'progressbar_bar' => t('Show progress bar'),
      'progressbar_page_number' => t('Show page number as number of completed (i.e. Page 1 of 10)'),
      'progressbar_percent' => t('Show percentage completed (i.e. 10%)'),
      'progressbar_pagebreak_labels' => t('Show page labels from page break components'),
      'progressbar_include_confirmation' => t('Include confirmation page in progress bar'),
    ),
    '#default_value' => webform_variable_get('webform_progressbar_style'),
    '#description' => t('Choose how the progress bar should be displayed for multi-page forms.'),
  );
  $form['progressbar']['webform_progressbar_label_first'] = array(
    '#type' => 'textfield',
    '#title' => t('First page label'),
    '#default_value' => webform_variable_get('webform_progressbar_label_first'),
    '#maxlength' => 255,
  );
  $form['progressbar']['webform_progressbar_label_confirmation'] = array(
    '#type' => 'textfield',
    '#title' => t('Confirmation page label'),
    '#default_value' => webform_variable_get('webform_progressbar_label_confirmation'),
    '#maxlength' => 255,
  );

  $form['advanced'] = array(
    '#type' => 'fieldset',
    '#title' => t('Advanced options'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
    '#weight' => 20,
  );

  $form['advanced']['webform_search_index']  = array(
    '#type' => 'checkbox',
    '#checked_value' => 1,
    '#title' => t('Include webform forms in search index'),
    '#default_value' => webform_variable_get('webform_search_index'),
    '#description' => t('When selected, all Webform nodes will have their form components indexed by the search engine.'),
    '#access' => module_exists('search'),
  );

  $form['advanced']['webform_tracking_mode']  = array(
    '#type' => 'radios',
    '#title' => t('Track anonymous users by:'),
    '#options' => array(
      'cookie' => t('Cookie only (least strict)'),
      'ip_address' => t('IP address only'),
      'strict' => t('Both cookie and IP address (most strict)'),
    ),
    '#default_value' => webform_variable_get('webform_tracking_mode'),
    '#description' => t('<a href="http://www.wikipedia.org/wiki/HTTP_cookie">Cookies</a> can be used to help prevent the same user from repeatedly submitting a webform. Limiting by IP address is more effective against repeated submissions, but may result in unintentional blocking of users sharing the same address. Logged-in users are always tracked by their user ID and are not affected by this option.'),
  );

  $form['advanced']['webform_email_address_format'] = array(
    '#type' => 'radios',
    '#title' => t('E-mail address format'),
    '#options' => array(
      'long' => t('Long format: "Example Name" &lt;name@example.com&gt;'),
      'short' => t('Short format: name@example.com'),
    ),
    '#default_value' => webform_variable_get('webform_email_address_format'),
    '#description' => t('Most servers support the "long" format which will allow for more friendly From addresses in e-mails sent. However many Windows-based servers are unable to send in the long format. Change this option if experiencing problems sending e-mails with Webform.'),
  );

  $form['advanced']['webform_export_format'] = array(
    '#type' => 'radios',
    '#title' => t('Default export format'),
    '#options' => webform_export_list(),
    '#default_value' => webform_variable_get('webform_export_format'),
  );

  $form['advanced']['webform_csv_delimiter']  = array(
    '#type' => 'select',
    '#title' => t('Default export delimiter'),
    '#description' => t('This is the delimiter used in the CSV/TSV file when downloading Webform results. Using tabs in the export is the most reliable method for preserving non-latin characters. You may want to change this to another character depending on the program with which you anticipate importing results.'),
    '#default_value' => webform_variable_get('webform_csv_delimiter'),
    '#options' => array(
      ','  => t('Comma (,)'),
      '\t' => t('Tab (\t)'),
      ';'  => t('Semicolon (;)'),
      ':'  => t('Colon (:)'),
      '|'  => t('Pipe (|)'),
      '.'  => t('Period (.)'),
      ' '  => t('Space ( )'),
    ),
  );

  $form['advanced']['webform_export_wordwrap'] = array(
    '#type' => 'radios',
    '#title' => t('Export word-wrap'),
    '#options' => array(
      '0' => t('Only text containing return characters'),
      '1' => t('All text'),
    ),
    '#default_value' => webform_variable_get('webform_export_wordwrap'),
    '#description' => t('Some export formats, such as Microsoft Excel, support word-wrapped text cells.'),
  );

  $form['advanced']['webform_submission_access_control']  = array(
    '#type' => 'radios',
    '#title' => t('Submission access control'),
    '#options' => array(
      '1' => t('Select the user roles that may submit each individual webform'),
      '0' => t('Disable Webform submission access control'),
    ),
    '#default_value' => webform_variable_get('webform_submission_access_control'),
    '#description' => t('By default, the configuration form for each webform allows the administrator to choose which roles may submit the form. You may want to allow users to always submit the form if you are using a separate node access module to control access to webform nodes themselves.'),
  );

  $form['advanced']['webform_email_select_max'] = array(
    '#type' => 'textfield',
    '#title' => t("Select email mapping limit"),
    '#default_value' => webform_variable_get('webform_email_select_max'),
    '#description' => t('When mapping emails addresses to a select component, limit the choice to components with less than the amount of options indicated. This is to avoid flooding the email settings form. '),
  );

  $form = system_settings_form($form);
  $form['#theme'] = 'webform_admin_settings';
  array_unshift($form['#submit'], 'webform_admin_settings_submit');

  return $form;
}

/**
 * Submit handler for the webform_admin_settings() form.
 */
function webform_admin_settings_submit($form, &$form_state) {
  $disabled_components = array();
  foreach ($form_state['values']['components'] as $name => $enabled) {
    if (!$enabled) {
      $disabled_components[] = $name;
    }
  }
  // Update $form_state and let system_settings_form_submit() handle saving.
  $form_state['values']['webform_disabled_components'] = $disabled_components;
  unset($form_state['values']['components']);

  // Trim out empty options in the progress bar options.
  $form_state['values']['webform_progressbar_style'] = array_keys(array_filter($form_state['values']['webform_progressbar_style']));
}

/**
 * Theme the output of the webform_admin_settings() form.
 */
function theme_webform_admin_settings($variables) {
  $form = $variables['form'];

  // Format the components into a table.
  foreach (element_children($form['components']) as $key) {
    $row = array();
    $row[] = $form['components'][$key]['#title'];
    $row[] = $form['components'][$key]['#description'];
    $form['components'][$key]['#title'] = NULL;
    $form['components'][$key]['#description'] = NULL;
    $row[] = array('data' => drupal_render($form['components'][$key]), 'align' => 'center');
    $rows[] = $row;
  }
  $header = array(t('Name'), t('Description'), array('data' => t('Enabled'), 'class' => array('checkbox')));

  // Create the table inside the form.
  $form['components']['table'] = array(
    '#theme' => 'table',
    '#header' => $header,
    '#rows' => $rows,
  );

  return drupal_render_children($form);
}

/**
 * Menu callback for admin/content/webform. Displays all webforms on the site.
 */
function webform_admin_content() {
  // Determine whether views or hard-coded tables should be used for the webforms table.
  if (!webform_variable_get('webform_table')) {
    $view = views_get_view('webform_webforms');
    return $view->preview('default');
  }

  $query = db_select('webform', 'w');
  $query->join('node', 'n', 'w.nid = n.nid');
  $query->fields('n');
  $nodes = $query->execute()->fetchAllAssoc('nid');
  return theme('webform_admin_content', array('nodes' => $nodes));
}

/**
 * Create a comma-separate list of content types that are webform enabled.
 */
function webform_admin_type_list() {
  $webform_types = webform_node_types();
  $webform_type_list = '';
  $webform_type_count = count($webform_types);
  foreach ($webform_types as $n => $type) {
    $webform_type_list .= l(node_type_get_name($type), 'node/add/' . str_replace('_', '-', $type));
    if ($n + 1 < $webform_type_count) {
      $webform_type_list .= $webform_type_count == 2 ? ' ' : ', ';
    }
    if ($n + 2 == $webform_type_count) {
      $webform_type_list .= t('or') . ' ';
    }
  }

  return $webform_type_list;
}

/**
 * Generate a list of all webforms avaliable on this site.
 */
function theme_webform_admin_content($variables) {
  $nodes = $variables['nodes'];
  $header = array(
    t('Title'),
    array('data' => t('View'), 'colspan' => '4'),
    array('data' => t('Operations'), 'colspan' => '3')
  );

  $rows = array();
  foreach ($nodes as $node) {
    $rows[] = array(
      l($node->title, 'node/' . $node->nid),
      l(t('Submissions'), 'node/' . $node->nid . '/webform-results'),
      l(t('Analysis'), 'node/' . $node->nid . '/webform-results/analysis'),
      l(t('Table'), 'node/' . $node->nid . '/webform-results/table'),
      l(t('Download'), 'node/' . $node->nid . '/webform-results/download'),
      node_access('update', $node) ? l(t('Edit'), 'node/' . $node->nid . '/edit') : '',
      node_access('update', $node) ? l(t('Components'), 'node/' . $node->nid . '/webform') : '',
      user_access('delete all webform submissions') ? l(t('Clear'), 'node/' . $node->nid . '/webform-results/clear') : '',
    );
  }

  if (empty($rows)) {
    $webform_types = webform_node_types();
    if (empty($webform_types)) {
      $message = t('Webform is currently not enabled on any content types.') . ' ' . t('Visit the <a href="!url">Webform settings</a> page and enable Webform on at least one content type.', array('!url' => url('admin/config/content/webform')));
    }
    else {
      $webform_type_list = webform_admin_type_list();
      $message = t('There are currently no webforms on your site. Create a !types piece of content.', array('!types' => $webform_type_list));
    }

    $rows[] = array(
      array('data' => $message, 'colspan' => 7),
    );
  }

  return theme('table', array('header' => $header, 'rows' => $rows));
}
