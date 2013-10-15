<?php

/**
 * @file
 *   Gettext export for localization community.
 */

/**
 * User interface for the translation export screen.
 */
function l10n_community_export_page($uri = NULL, $langcode = NULL) {
  $breadcrumb = array(
    l(t('Home'), NULL),
    l(t('Translate'), 'translate'),
  );

  // Include filters just so if user wants to quickly go back to translate or
  // browse forms, they can do that without loosing their filters.
  $filters = l10n_community_build_filter_values($_GET);
  l10n_community_add_url_modifiers($langcode, $filters);
  drupal_add_js(drupal_get_path('module', 'l10n_community') . '/l10n_community.js');

  // Set a matching title with the translation page.
  if (isset($langcode)) {
    $language = l10n_community_get_language($langcode);
    drupal_set_title(t('Export @language translations', array('@language' => t($language->name))), PASS_THROUGH);
    $breadcrumb[] = l(t('Explore languages'), 'translate/languages');
    $breadcrumb[] = l(t($language->name), 'translate/languages/' . $langcode);
  }
  else {
    $project = l10n_server_get_projects(array('uri' => $uri));
    drupal_set_title(t('Export @project translation templates', array('@project' => $project->title)), PASS_THROUGH);
    $breadcrumb[] = l(t('Explore projects'), 'translate/projects');
    $breadcrumb[] = l($project->title, 'translate/projects/' . $project->uri);
  }
  // Add missing breadcrumb.
  drupal_set_breadcrumb($breadcrumb);

  if (isset($project)) {
    module_load_include('inc', 'l10n_community', 'pages');
    if (!$num_source = l10n_community_get_string_count('project', $project->pid)) {
      drupal_set_message(t('No strings found to export in this project yet.'), 'warning');
      return '';
    }
  }

  return drupal_get_form('l10n_community_export_form', $uri, $langcode);
}

/**
 * Translation export form. This is multi-step with the project selection.
 */
function l10n_community_export_form($form, &$form_state, $uri = NULL, $langcode = NULL) {

  // When project was preset from path, disable the selector.
  $hide_projects = isset($uri);

  // Set initial form state, so we can consistently use that.
  $projects = l10n_server_get_projects();
  if (empty($form_state['values'])) {
    $form_state['values'] = array(
      'project' => isset($projects[$uri]) ? $projects[$uri]->title : (isset($_GET['project']) && isset($projects[$_GET['project']]) ? $projects[$_GET['project']]->title : ''),
      'release' => isset($_GET['release']) ? $_GET['release'] : 'all',
    );
  }

  $form = array();
  if ($hide_projects) {
    // This only happens if we have a project to export via $uri.
    // In this case, $uri is already verified via the menu system.
    $form['project'] = array(
      '#type' => 'value',
      '#value' => $form_state['values']['project'],
    );
  }
  else {
    $form['project'] = array(
      '#title' => t('Project'),
      '#required' => TRUE,
      '#default_value' => $form_state['values']['project'],
      '#ajax' => array(
        // 'event' missing, Drupal will apply the per-#type defaults.
        'callback' => 'l10n_community_ajax_releases',
        'wrapper' => 'l10n-server-releases',
        'effect' => 'fade',
      ),
    );
    if (($count = count($projects)) <= 30) {
      // Radio box widget for as much as 5 projects, select widget for 5-30 projects.
      $form['project']['#type'] = ($count <= 5 ? 'radios' : 'select');
      $form['project']['#options'] = array();
      foreach ($projects as $project) {
        // Title used to conform to the autocomplete behavior.
        $form['project']['#options'][$project->title] = $project->title;
      }
    }
    else {
      // Autocomplete field for more then 30 projects.
      $form['project'] += array(
        '#type' => 'textfield',
        '#autocomplete_path' => 'translate/project-autocomplete',
      );
    }
  }

  $release_options = array('all' => t('All releases merged'));
  if (!empty($form_state['values']['project'])) {
    $selected_project = $projects[l10n_community_project_uri_by_title($form_state['values']['project'])];
    $releases = l10n_server_get_releases($selected_project->uri);
    foreach ($releases as $rid => $this_release) {
      $release_options[$rid] = t('@release only', array('@release' => $this_release->title));
    }
  }
  if (!isset($release_options[$form_state['values']['release']])) {
    $form_state['values']['release'] = 'all';
  }
  $form['release'] = array(
    '#title' => t('Release'),
    '#required' => TRUE,
    '#type' => 'select',
    '#options' => $release_options,
    '#default_value' => $form_state['values']['release'],
    // Wrapper used to replace release options via AJAX.
    '#prefix' => '<div id="l10n-server-releases">',
    '#suffix' => '</div>',
  );

  if (isset($langcode)) {
    $form['langcode'] = array(
      '#type' => 'value',
      '#value' => $langcode,
    );

    // Only include the type selector if we are not presenting
    // a template export page (which does not have a language).
    $languages = l10n_community_get_languages();
    $form['translations'] = array(
      '#title' => t('Add @language translations', array('@language' => $languages[$langcode]->name)),
      '#type' => 'checkbox',
      '#default_value' => 1,
    );
    $form['suggestions'] = array(
      '#title' => t('Add @language suggestions', array('@language' => $languages[$langcode]->name)),
      '#type' => 'checkbox',
      '#description' => t('For untranslated strings, the first suggestion will be exported as a fuzzy translation. All other suggestions are exported in comments.'),
    );
    $form['verbose'] = array(
      '#title' => t('Verbose output'),
      '#type' => 'checkbox',
      '#description' => t('Verbose files are ideal for desktop editing, compact files are better for download size.'),
      '#default_value' => 1,
    );
  }

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => isset($langcode) ? t('Export Gettext file') : t('Export Gettext template'),
  );
  return $form;
}

/**
 * Project field is mandatory.
 */
function l10n_community_export_form_validate($form, &$form_state) {
  if (!$project = l10n_community_project_uri_by_title($form_state['values']['project'])) {
    form_set_error('project', t('Invalid project selected.'));
    $form_state['values']['project'] = '';
  }
}

/**
 * Generate translation or template package on the fly based on
 * all details available and return the output via HTTP.
 */
function l10n_community_export_form_submit($form, &$form_state) {

  // This was validated to work in the validation code.
  $uri = !empty($form_state['values']['project']) ? l10n_community_project_uri_by_title($form_state['values']['project']) : NULL;
  if (empty($uri)) {
    form_set_error('project', t('Please select a project first.'));
    return;
  }

  if (empty($form_state['values']['release'])) {
    form_set_error('release', t('Please choose a release or opt to export for all releases.'));
    $form_state['redirect'] = array($_GET['q'], array('project' => $uri));
    return;
  }
  elseif ($form_state['values']['release'] != 'all') {
    $releases = l10n_server_get_releases($uri);
    if (!isset($releases[$form_state['values']['release']])) {
      form_set_error('release', t('Invalid release chosen.'));
      return;
    }
  }

  $language = NULL;
  if (isset($form_state['values']['langcode'])) {
    $languages = l10n_community_get_languages();
    $language = $languages[$form_state['values']['langcode']];
  }

  // Generate tarball or PO file and get file name.
  $export_result = l10n_community_export(
    $uri,
    ($form_state['values']['release'] == 'all' ? NULL : $form_state['values']['release']),
    $language,
    empty($form_state['values']['translations']),
    empty($form_state['values']['verbose']),
    FALSE,
    $form_state['values']['suggestions']
  );

  if (isset($export_result) && is_array($export_result)) {
    // If we got an array back from the export build, tear that into pieces.
    list($mime_type, $file_name, $serve_name, $sid_count) = $export_result;
    header('Content-Disposition: attachment; filename=' . $serve_name);
    header('Content-Type: ' . $mime_type);
    echo file_get_contents($file_name);
    unlink($file_name);
    die();
  }
  else {
    // Messages should already be recorded about any build errors.
    return;
  }
}

/**
 * Helper function to store the export string.
 */
function _l10n_community_export_string_files(&$po_data, $uri, $language, $template, $export_string, $compact = FALSE, $suggestions = FALSE) {
  $output = '';

  if (!empty($export_string)) {

    // Location comments are constructed in fileone:1,2,5; filetwo:123,537
    // format, where the numbers represent the line numbers of source
    // occurances in the respective source files.
    $comment = array();
    foreach ($export_string['comment'] as $path => $lines) {
      $comment[] = preg_replace('!(^[^/]+/)!', '', $path) . ':' . join(',', array_keys($lines));
    }
    $comment = '#: ' . join('; ', $comment) . "\n";
    if (!$compact) {
      $output = $comment;
    }

    $fuzzy = FALSE;
    if ($suggestions) {
      $all_suggestions = array_keys($export_string['suggestions']);
      // Export information about suggestions if inclusion was requested.
      if ($export_string['has_suggestion']) {
        // If we had suggestions, add comment to let reviewers know.
        $output .= count($all_suggestions) > 1 ? "# Suggestions on the localization server:\n" : "# Suggestion on the localization server:\n";
      }
      if (empty($export_string['translation']) && !empty($all_suggestions)) {
        // If no translation, make the translation the first identified suggestion
        // and mark the translation fuzzy (so it keeps to be a suggestion on
        // reimport).
        $export_string['translation'] = array_shift($all_suggestions);
        $fuzzy = TRUE;
      }
      if (!empty($all_suggestions)) {
        if (strpos($export_string['value'], "\0")) {
          foreach ($all_suggestions as $i => $suggestion) {
            // Format the suggestions in a readable format, if plurals.
            $all_suggestions[$i] = str_replace("\0", ' / ', $suggestion);
          }
        }
        // Add all other suggestions as comment lines. Multiline suggestions will
        // appear on multiple lines, people need to figure these out manually.
        $output .= '# ' . str_replace("\n", "\n# ", join("\n", $all_suggestions)) . "\n";
      }
    }
    if ($fuzzy) {
      $output .= "#, fuzzy\n";
    }

    if (strpos($export_string['value'], "\0") !== FALSE) {
      // This is a string with plural variants.
      list($singular, $plural) = explode("\0", $export_string['value']);
      if (!empty($export_string['context'])) {
        $output .= 'msgctxt ' . _l10n_community_export_string($export_string['context']);
      }
      $output .= 'msgid ' . _l10n_community_export_string($singular) . 'msgid_plural ' . _l10n_community_export_string($plural);
      if (!$template && !empty($export_string['translation'])) {
        // Export translations we have.
        foreach (explode("\0", $export_string['translation']) as $id => $value) {
          $output .= 'msgstr[' . $id . '] ' . _l10n_community_export_string($value);
        }
      }
      elseif (isset($language)) {
        // Empty msgstrs based on plural formula for language. Could be
        // a plural without translation or a template generated for a
        // specific language.
        for ($pi = 0; $pi < $language->plurals; $pi++) {
          $output .= 'msgstr[' . $pi . '] ""' . "\n";
        }
      }
      else {
        // Translation template without language, assume two msgstrs.
        $output .= 'msgstr[0] ""' . "\n";
        $output .= 'msgstr[1] ""' . "\n";
      }
    }
    else {
      // Simple string (and possibly translation pair).
      if (!empty($export_string['context'])) {
        $output .= 'msgctxt ' . _l10n_community_export_string($export_string['context']);
      }
      $output .= 'msgid ' . _l10n_community_export_string($export_string['value']);
      if (!empty($export_string['translation'])) {
        $output .= 'msgstr ' . _l10n_community_export_string($export_string['translation']);
      }
      else {
        $output .= 'msgstr ""' . "\n";
      }
    }

    if (empty($po_data)) {
      $po_data = array(
        'file' => '',
        'changed' => 0,
        'revisions' => array(),
      );
    }

    // Add to existing file storage.
    $po_data['file'] .= $output;
    if (!$compact) {
      $po_data['file'] .= "\n";
    }
    if (!$template) {
      $po_data['changed'] = max($po_data['changed'], $export_string['changed']);
    }
    $po_data['revisions'] = array_unique(array_merge($po_data['revisions'], array_keys($export_string['revisions'])));
  }
}

/**
 * Generates the PO(T) files contents and wrap them in a tarball for a given
 * project.
 *
 * @param $uri
 *   Project URI.
 * @param $release
 *   Release number (rid) to generate tarball for, or NULL to generate
 *   with all releases considered.
 * @param $language
 *   Language object.
 * @param $template
 *   TRUE if templates should be exported, FALSE if translations.
 * @param $compact
 *   A compact export will skip outputting the comments, superfluous
 *   newlines, empty translations and the list of files. TRUE or FALSE.
 * @param $installer
 *   Whether we should only export the translations needed for the installer
 *   and not those needed for the runtime site.
 */
function l10n_community_export($uri, $release = NULL, $language = NULL, $template = TRUE, $compact = FALSE, $installer = FALSE, $suggestions = FALSE) {
  $project = l10n_server_get_projects(array('uri' => $uri));

  $query = db_select('l10n_server_file', 'f');
  $query->innerJoin('l10n_server_line', 'l', 'f.fid = l.fid');
  $query->innerJoin('l10n_server_string', 's', 'l.sid = s.sid');
  $query
    ->fields('s', array('sid', 'value', 'context'))
    ->fields('f', array('location', 'revision'))
    ->fields('l', array('lineno', 'type'))
    ->condition('f.pid', $project->pid)
    ->orderBy('s.sid');

  if (!$template) {
    // Join differently based on compact method, so we can skip strings without
    // translation for compact method export.
    $translation_join = ($compact) ? 'innerJoin' : 'leftJoin';

    // Improve the query for templates
    $query->leftJoin('l10n_server_status_flag', 'st',
      'st.sid = s.sid AND st.language = :language',
      array(':language' => $language->language));
    $query->$translation_join('l10n_server_translation', 't',
      's.sid = t.sid AND t.language = :language AND t.is_active = 1',
      array(':language' => $language->language));
    $query
      ->fields('t', array('translation', 'is_suggestion'))
      ->fields('st', array('has_suggestion'))
      ->orderBy('t.is_suggestion')
      ->orderBy('t.time_entered', 'DESC');

    // Installer strings are POTX_STRING_INSTALLER or POTX_STRING_BOTH.
    if ($installer) {
      $query->condition('type', array(0,1), 'IN');
    }

    // Only include suggestions if requested, otherwise filter out.
    if (!$suggestions) {
      $query->condition('t.is_suggestion', '0');
    }
  }

  if (isset($release)) {
    // Release restriction.
    $query->condition('f.rid', $release);
    $releases = l10n_server_get_releases($uri);
    $release = $releases[$release];
  }

  $previous_sid = $sid_count = 0;
  $export_string = $po_data = array();

  $result = $query->execute();
  foreach ($result as $string) {
    if ($string->sid != $previous_sid) {
      // New string in the stream.
      _l10n_community_export_string_files($po_data, $uri, $language, $template, $export_string, $compact, $suggestions);

      // Now fill in the new string values.
      $previous_sid = $string->sid;
      $export_string = array(
        'comment' => array(),
        'value' => $string->value,
        'context' => $string->context,
        'translation' => (!empty($string->translation) && !$string->is_suggestion) ? $string->translation : '',
        'suggestions' => array(),
        'revisions' => array(),
        'changed' => isset($string->time_approved) ? $string->time_approved : 0,
        'type' => $string->type,
        'has_suggestion' => @$string->has_suggestion,
      );
      $sid_count++;
    }
    else {
      // Existing string but with new location information.
      if ($export_string['type'] != 0 && $export_string['type'] != $string->type) {
        // Elevate string type if it is not already 0 (POTX_STRING_BOTH), and
        // the currently found string type is different to the previously found.
        $export_string['type'] = 0;
      }
    }
    // Uniquely collected, so we use array keys for speed.
    $export_string['comment'][$string->location][$string->lineno] = 1;
    $export_string['revisions'][$string->revision] = 1;
    if ($string->is_suggestion) {
      $export_string['suggestions'][$string->translation] = 1;
    }
  }
  if ($previous_sid > 0) {
    // Store the last string.
    _l10n_community_export_string_files($po_data, $uri, $language, $template, $export_string, $compact, $suggestions);
    $sid_count++;
  }

  if (empty($po_data)) {
    // No strings were found.
    if (isset($release)) {
      $message = t('There are no strings in the %release release of %project to export.', array('%project' => $project->title, '%release' => $release->title));
    }
    else {
      $message = t('There are no strings in any releases of %project to export.', array('%project' => $project->title));
    }
    // Message to the user.
    drupal_set_message($message);
    // Message to watchdog for possible automated packaging.
    watchdog('l10n_community', $message);
    return NULL;
  }

  // Generate a 'unique' temporary filename for this package.
  $tempfile = tempnam(file_directory_temp(), 'l10n_community-' . $uri);

  if (!$compact) {
    if (count($po_data['revisions']) == 1) {
      $file_list = '# Generated from file: ' . $po_data['revisions'][0] . "\n";
    }
    else {
      $file_list = '# Generated from files:' . "\n#  " . join("\n#  ", $po_data['revisions']) . "\n";
    }
  }
  else {
    $file_list = '';
  }

  $release_title = $project->title . ' (' . (isset($release) ? $release->title : 'all releases') . ')';
  if (!$template) {
    $header = '# ' . $language->name . ' translation of ' . $release_title . "\n";
    $header .= "# Copyright (c) " . date('Y') . ' by the ' . $language->name . " translation team\n";
    $header .= $file_list;
    $header .= "#\n";
    $header .= "msgid \"\"\n";
    $header .= "msgstr \"\"\n";
    $header .= "\"Project-Id-Version: " . $release_title . "\\n\"\n";
    $header .= "\"POT-Creation-Date: " . date("Y-m-d H:iO") . "\\n\"\n";
    // Use date placeholder, if we have no date information (no translation here yet).
    $header .= "\"PO-Revision-Date: " . (!empty($po_data['changed']) ? date("Y-m-d H:iO", $po_data['changed']) : 'YYYY-mm-DD HH:MM+ZZZZ') . "\\n\"\n";
    $header .= "\"Language-Team: " . $language->name . "\\n\"\n";
    $header .= "\"MIME-Version: 1.0\\n\"\n";
    $header .= "\"Content-Type: text/plain; charset=utf-8\\n\"\n";
    $header .= "\"Content-Transfer-Encoding: 8bit\\n\"\n";
    if ($language->formula && $language->plurals) {
      $header .= "\"Plural-Forms: nplurals=" . $language->plurals . "; plural=" . strtr($language->formula, array('$' => '')) . ";\\n\"\n";
    }
  }
  else {
    $language_title = (isset($language) ? $language->name : 'LANGUAGE');
    $header = "# " . $language_title . " translation of " . $release_title . "\n";
    $header .= "# Copyright (c) " . date('Y') . "\n";
    $header .= $file_list;
    $header .= "#\n";
    $header .= "msgid \"\"\n";
    $header .= "msgstr \"\"\n";
    $header .= "\"Project-Id-Version: " . $release_title . "\\n\"\n";
    $header .= "\"POT-Creation-Date: " . date("Y-m-d H:iO") . "\\n\"\n";
    $header .= "\"PO-Revision-Date: YYYY-mm-DD HH:MM+ZZZZ\\n\"\n";
    $header .= "\"Language-Team: " . $language_title . "\\n\"\n";
    $header .= "\"MIME-Version: 1.0\\n\"\n";
    $header .= "\"Content-Type: text/plain; charset=utf-8\\n\"\n";
    $header .= "\"Content-Transfer-Encoding: 8bit\\n\"\n";
    if (isset($language) && $language->formula && $language->plurals) {
      $header .= "\"Plural-Forms: nplurals=" . $language->plurals . "; plural=" . strtr($language->formula, array('$' => '')) . ";\\n\"\n";
    }
    else {
      $header .= "\"Plural-Forms: nplurals=INTEGER; plural=EXPRESSION;\\n\"\n";
    }
  }
  // Write to file directly. We should only do this once.
  $fh = fopen($tempfile, 'w');
  fwrite($fh, $header . "\n" . $po_data['file']);
  fclose($fh);

  // Output a single PO(T) file.
  return array('text/plain', $tempfile, $uri . '-' . (isset($release) ? $release->title : 'all') . (isset($language) ? '.' . $language->language : '') . ($template ? '.pot' : '.po'), $sid_count);
}

/**
 * Print out a string on multiple lines
 */
function _l10n_community_export_string($str) {
  $stri = addcslashes($str, "\0..\37\\\"");
  $parts = array();

  // Cut text into several lines
  while ($stri != "") {
    $i = strpos($stri, "\\n");
    if ($i === FALSE) {
      $curstr = $stri;
      $stri = "";
    }
    else {
      $curstr = substr($stri, 0, $i + 2);
      $stri = substr($stri, $i + 2);
    }
    $curparts = explode("\n", _l10n_community_export_wrap($curstr, 70));
    $parts = array_merge($parts, $curparts);
  }

  // Multiline string
  if (count($parts) > 1) {
    return "\"\"\n\"" . implode("\"\n\"", $parts) . "\"\n";
  }
  // Single line string
  elseif (count($parts) == 1) {
    return "\"$parts[0]\"\n";
  }
  // No translation
  else {
    return "\"\"\n";
  }
}

/**
 * Custom word wrapping for Portable Object (Template) files.
 */
function _l10n_community_export_wrap($str, $len) {
  $words = explode(' ', $str);
  $ret = array();

  $cur = "";
  $nstr = 1;
  while (count($words)) {
    $word = array_shift($words);
    if ($nstr) {
      $cur = $word;
      $nstr = 0;
    }
    elseif (strlen("$cur $word") > $len) {
      $ret[] = $cur . " ";
      $cur = $word;
    }
    else {
      $cur = "$cur $word";
    }
  }
  $ret[] = $cur;

  return implode("\n", $ret);
}
