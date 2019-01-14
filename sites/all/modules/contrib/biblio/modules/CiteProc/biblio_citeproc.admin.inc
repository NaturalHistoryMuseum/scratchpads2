<?php
function biblio_citeproc_style_manager_form($form, &$form_state) {
  $form = array();
  $options = array();

  $cache = cache_get('biblio_citeproc_styles');

  if (!$cache || $cache->expire < time()) {
    if (!($style_zip_file = variable_get('biblio_citeproc_styles_zip_file', FALSE))) {
      $style_zip_file = _get_zip_from_github();
    }

    if ($style_zip_file) {
      $file = drupal_realpath($style_zip_file->uri);
      $options = _get_csl_list_from_zip($file);
    }

    if (!empty($options)) {
      //expire 30 days from now
      $expire = time() + 2592000;
      cache_set('biblio_citeproc_styles', $options, 'cache', $expire);
    }
  }
  else {
    $options = $cache->data;
  }


  $form['available_styles'] = array(
    '#type' => 'select',
    '#title' => t('Available styles'),
    '#size' => 15,
    '#multiple' => TRUE,
    '#description' => t('Choose the styles you would like to download and install.'),
  );

  $form['install'] = array(
    '#type'        => 'submit',
    '#value'       => '<--',
    '#description' => t('Install the selected styles from GitHub'),
  );
  $form['remove'] = array(
    '#type'        => 'submit',
    '#value'       => '-->',
    '#description' => t('Un-install the selected styles'),
  );
  $form['default'] = array(
    '#type'  => 'submit',
    '#value' => t('Set as site default'),
    '#submit' => array('biblio_citeproc_set_site_default'),
  );
  $form['update_installed'] = array(
    '#type'  => 'submit',
    '#value' => t('Update installed styles'),
    '#submit' => array('biblio_citeproc_update_installed'),
  );
  $form['update_available'] = array(
    '#type'  => 'submit',
    '#value' => t('Update available styles'),
    '#submit' => array('biblio_citeproc_update_available'),
  );
  $form['edit'] = array(
    '#type'  => 'submit',
    '#value' => t('Edit selected'),
    '#submit' => array('biblio_citeproc_edit_selected'),
  );
//   $form['install_all'] = array(
//     '#type'     => 'submit',
//     '#value'    => t('Install all')
//   );

  $form['#attributes']['enctype'] = 'multipart/form-data';

  $form['import_csl_file'] = array(
        '#type' => 'file',
        '#title' => t('Import Local CSL file'),
        '#default_value' => '',
        '#size' => 60
  );
  $form['import'] = array(
      '#type'   => 'submit',
      '#value'  => t('Import'),
      '#submit' => array('biblio_citeproc_csl_file_import_submit'),
  );

  $result = db_select('biblio_citeproc_styles', 'csl')
    ->fields('csl', array('filename', 'title', 'id', 'sha1', 'title', 'summary', 'changed', 'updated'))
    ->orderBy('filename', 'ASC')
    ->execute();

  $details = array();
  $titles = array();
  foreach ($result as $style) {
    $details[$style->filename] = $style;
    $titles[] = $style->title;
  }

  // now remove the installed titles from the available titles list
  $options = array_diff($options, $titles);
  $form['available_styles']['#options'] = $options;

  $form['installed_styles'] = array(
    '#type' => 'select',
    '#title' => t('Installed styles'),
    '#size' => 15,
    '#options' => biblio_get_styles(),
    '#multiple' => TRUE,
    '#description' => t('Currently installed styles.'),
  );

  $form['current_default'] = array(
    '#markup' => empty($details) ? '' : $details[variable_get('biblio_citeproc_style', 'ieee.csl')]->title,
  );

  $form['current_summary'] = array(
    '#markup' => empty($details) ? '' : $details[variable_get('biblio_citeproc_style', 'ieee.csl')]->summary,
  );

  $timestamp = $details[variable_get('biblio_citeproc_style', 'ieee.csl')]->updated;
  $updated = $timestamp ? ' ('. t('Last updated:') . ' ' . format_date($timestamp, 'medium') . ')' : '';

  $form['current_update'] = array(
    '#markup' => $updated,
  );

  return $form;
}

function theme_biblio_citeproc_style_manager_form($variables) {
  $form = $variables['form'];
  $rows = array();
  $updated = drupal_render($form['current_update']);
  $updated = empty($updated) ? $updated : '</br>' . $updated;
  $rows[] = array(
            array('data' => t('Current default style:')),
            array('data' => '<b>' . drupal_render($form['current_default']) . '</b>'  .
                '</br><i>' . drupal_render($form['current_summary']) . '</i>' . $updated
                ),
  );
  $rows[] = array(
            array('data' => t('Example citation:')),
            array('data' => biblio_citeproc_example_citation()));
  $output = theme('table', array('rows' => $rows));
  $rows = array();
  $rows[] = array(
              array('data' => drupal_render($form['installed_styles']) ),
              array('data' => drupal_render($form['install']) . '<br>' . drupal_render($form['remove'])),
              array('data' => drupal_render($form['available_styles'])),
            );
  $rows[] = array(
              array('data' =>  drupal_render($form['default'])  . drupal_render($form['edit']) .drupal_render($form['update_installed'])),
              array('data' => ''),
              array('data' => drupal_render($form['update_available'])),
            );
  $rows[] = array(array('data' => drupal_render($form['import_csl_file']) . drupal_render($form['import']), 'colspan' => 3) );
  $output .= theme('table', array('rows' => $rows));

  $output .= drupal_render_children($form);
  return $output;

}
function biblio_citeproc_style_manager_form_validate($form, &$form_state) {
  if ($form_state['clicked_button']['#value'] == '<--' && count( $form_state['values']['available_styles'])) {
    if (count($form_state['values']['available_styles']) > 60) {
      form_error($form['available_styles'], t('You may not select more than 60 styles for installation at one time'));
    }
  }
 if ($form_state['clicked_button']['#value'] ==  t('Set as site default') && !count( $form_state['values']['installed_styles'])) {
   form_error($form['installed_styles'], t('You must select an installed style to set as the default.'));
 }

}
function biblio_citeproc_style_manager_form_submit($form, &$form_state) {
  if ($form_state['clicked_button']['#value'] == '<--' && count( $form_state['values']['available_styles'])) {
    if (!($style_zip_file = variable_get('biblio_citeproc_styles_zip_file', FALSE))) {
      $style_zip_file = _get_zip_from_github();
    }

    if (!$style_zip_file) {
      form_set_error('<--', t('Could not get the style files from GitHub'));
    }

    $file = drupal_realpath($style_zip_file->uri);
    $selected = $form_state['values']['available_styles'];
    _install_selected_from_zip($file, $selected);
  }

  if ($form_state['clicked_button']['#value'] == '-->' && count( $form_state['values']['installed_styles'])) {
    $selected = $form_state['values']['installed_styles'];
    _uninstall_selected($selected);
  }
}

function biblio_citeproc_edit_selected($form, &$form_state) {
  if (count( $form_state['values']['installed_styles'])) {
    $style = array_shift($form_state['values']['installed_styles']);
    $dest = drupal_get_destination();
    drupal_goto('admin/config/content/biblio/citeproc/styles/' . $style . '/edit');
  }
}

function biblio_citeproc_set_site_default($form, &$form_state) {
  if (count( $form_state['values']['installed_styles']) == 1) {
    $def = array_shift($form_state['values']['installed_styles']);
    variable_set('biblio_citeproc_style', $def);
  }
  else {
    form_set_error('installed_styles', t('You may only select one style when setting the default'));
  }
}

function biblio_citeproc_update_installed($form, &$form_state) {
  $batch_op = array(
      'title' => t('Updating all installed styles from the main GitHub repository'),
      'operations' => array(
          array('_get_zip_from_github', array()),
          array('biblio_citeproc_update_installed_batch', array()),
      ),
      'progressive' => TRUE,
      'finished' => 'biblio_citeproc_update_installed_finished',
      'init_message' => t('Downloading file...'),
      'progress_message' => t('Updating styles...'),
      'file' => './' . drupal_get_path('module', 'biblio_citeproc') . '/biblio_citeproc.admin.inc'
  );
  batch_set($batch_op);
}

function biblio_citeproc_update_available($form, &$form_state) {
  _get_zip_from_github();
}

function biblio_citeproc_csl_file_import_submit($form, &$form_state) {
  $validators = array(
      'file_validate_extensions' => array('csl xml'),
      'biblio_citeproc_validate_csl_file' => array()
  );

  if ($import_file = file_save_upload('import_csl_file', $validators)) {
    $csl = file_get_contents($import_file->uri);
//    if (biblio_citeproc_validate_csl($csl)) {
    _install_csl($import_file->filename, $csl);
//    }
  }

}
function _get_github_repo_tree($path = '') {
  $options = array();
  $tree = array();

  $tree_url = 'https://api.github.com/repos/citation-style-language/styles-distribution/contents';
    if (!empty($path)) {
      $tree_url .= '/' . $path;
    }

    $result=  drupal_http_request($tree_url);
    if ($result->code == 200) {
      $tree = json_decode($result->data);
    }
    else {
      $message = t('Attempt to get list of styles from GitHub resulted in an HTTP error: !code.', array('!code' => $result->code));

      $cache = cache_get('biblio_citeproc_styles');

      if ($cache) {
        $message .= ' ' . t('I will use cached data instead.');
        $mess_type = 'warning';
        $options = $cache->data;
      }
      else {
        $message .= ' ' . t('I have no cached data, so you will not be able to install new styles at this time.');
        $mess_type = 'error';
      }
      drupal_set_message(check_plain($message), $mess_type);
      return $options;
    }

    foreach ($tree as $file) {
      if ($file->type == 'file' && strstr($file->name, '.csl')) {
        $options[$file->path] =  basename($file->name);
      }
      elseif ($file->type == 'dir') {
        $options = array_merge($options, _get_github_repo_tree($file->name));
      }
    }
  return $options;
}

function _install_csl($name = NULL, $csl = NULL, $sha = NULL, $all = FALSE, $update = FALSE) {
  static  $installed = array();

  if (empty($installed)) {
    $result = db_select('biblio_citeproc_styles', 'csl')
    ->fields('csl', array('filename', 'id', 'sha1', 'title'))
    ->orderBy('filename', 'ASC')
    ->execute();

    $installed = array();
    foreach ($result as $style) {
      $installed[$style->id] = $style;
    }
  }

  $xml = simplexml_load_string($csl);

  if ($xml) {
    $parent = '';
    foreach ($xml->info->link as $link) {
      $attrs = $link->attributes();
      if (isset($attrs['rel']) && $attrs['rel'] == 'independent-parent') {
        $parent = (string)$attrs['href'];
      }
    }
    if (!$all && !$update && !empty($parent)) {
      $csl_file_contents = db_query("SELECT csl FROM {biblio_citeproc_styles} WHERE id = :parent", array(':parent' => $parent))->fetchField();
      if (!$csl_file_contents) {
        _install_csl_from_github(basename($parent) . '.csl');
      }
    }

    $sha1 = (isset($sha)) ? $sha : sha1($csl);

    $record  = array(
        'filename' => $name,
        'parent'   => $parent,
        'title'    => trim((string)$xml->info->title),
        'summary'  => (string)$xml->info->summary,
        'csl'      => $csl,
        'sha1'     => $sha1,
        'id'       => (string)$xml->info->id,
        'updated'  => time(),
        'changed'  => 0,
    );

    if (!array_key_exists($record['id'], $installed)) {
      db_insert('biblio_citeproc_styles')->fields($record)->execute();
      $installed[$record['id']] = TRUE;
      return 1;
    }
    elseif ($record['sha1'] != $installed[$record['id']]->sha1) {
      db_update('biblio_citeproc_styles')->condition('id', $record['id'])->fields($record)->execute();
      return 2;
    }
    elseif (($record['sha1'] == $installed[$record['id']]->sha1 && $update == FALSE)) {
      $message = t('The CSL file you supplied: !name, is already installed', array('!name' => $name));
      drupal_set_message(check_plain($message), 'warning');
    }
  }
  else {
    drupal_set_message(t('I could not parse the CSL provided as valid XML'), 'error');
  }
}

function _get_zip_from_github() {
  $zip_file = false;

  $zip_url = 'https://github.com/citation-style-language/styles-distribution/archive/master.zip';
  $destination = file_build_uri('Biblio-CiteProc-Styles.zip');
  $zip_file = system_retrieve_file($zip_url, $destination, TRUE, FILE_EXISTS_REPLACE);
  if ($zip_file) {
    $usage = file_usage_list($zip_file);
  }
  else {
    drupal_set_message("Error downloading styles", 'error');
    return $zip_file;
  }
  if (empty($usage)) {
    file_usage_add($zip_file, 'biblio_citeproc', 'csl', 0);
  }
  variable_set('biblio_citeproc_styles_zip_file', $zip_file);
  cache_clear_all('biblio_citeproc_styles', 'cache');
  return $zip_file;
}

function _install_csl_from_github($path, $update = FALSE) {
  $csl = '';
  $github_url = 'https://api.github.com/repos/citation-style-language/styles-distribution/contents/';
  $URL = $github_url . $path;
  $result = drupal_http_request($URL);
  if ($result->code == 200) {
    $file  = json_decode($result->data);
    switch ($file->encoding) {
      case 'base64':
        $csl = base64_decode($file->content);
        break;
    }
    _install_csl($file->name, $csl, $file->sha, FALSE, $update);
  }
  else {
    $message = t('Attempt to get style: %name from GitHub resulted in an HTTP error: !code.', array('%name' => $path, '!code' => $result->code));
    $mess_type = 'error';
    drupal_set_message(check_plain($message), $mess_type);
  }
  return;
}

function _get_csl_list_from_zip($filename) {
  $options = array();
  $za      = new ZipArchive();

  if ($za->open($filename) !== TRUE) {
    $message = t('Could not open zip file containing styles: @file', array('@file' => realpath($filename)));
    $message = check_plain($message);
    drupal_set_message($message, 'error');
    return $options;
  }

  $num_files = $za->numFiles;

  for ($i = 0; $i < $num_files; $i++) {
    $name = $za->getNameIndex($i);
    $name = basename($name);
    if (strstr($name, '.csl')) {
      $csl = $za->getFromIndex($i);
      $xml = simplexml_load_string($csl);
      if ($xml) {
        $options[$i] = trim((string)$xml->info->title);
      }
    }
  }

  $za->close();
  asort($options);
  return $options;
}

function _install_selected_from_zip($filename = '', $ids = array()) {
  $za = new ZipArchive();
  if ($za->open($filename) == TRUE) {
    foreach ($ids as $id) {
      $name = $za->getNameIndex($id);
      $name = basename($name);
      if (strstr($name, '.csl')) {
        $csl = $za->getFromIndex($id);
        _install_csl($name, $csl);
      }
    }
    $za->close();
  }
}

function _uninstall_selected($ids = array()) {
  $result = db_select('biblio_citeproc_styles', 'csl')
    ->fields('csl', array('id', 'filename', 'parent'))
    ->orderBy('filename', 'ASC')
    ->execute();

  foreach ($result as $csl) {
    $fp[$csl->filename] = $csl->parent;
    $fi[$csl->filename] = $csl->id;
  }

  foreach ($ids as $id) {
      db_delete('biblio_citeproc_styles')->condition('filename', $id)->execute();
      //if this is a dependent style, delete the parent style if no others are using it
      if (!empty($fp[$id])) {
        $parent = array_keys($fp, $fp[$id]);
        if ( count($parent) == 1) {
          db_delete('biblio_citeproc_styles')->condition('id', $fp[$id])->execute();
        }
      }
      //delete all the dependents
      $children = array_keys($fp, $fi[$id]);
      if (!empty($children)) {
         db_delete('biblio_citeproc_styles')->condition('filename', $children, 'IN')->execute();
      }
      if (variable_get('biblio_citeproc_style', 'ieee.csl') == $id) {
        variable_del('biblio_citeproc_style');
      }
    }

}

function _install_all_from_zip(&$context = NULL) {

   $zipname = $context['results']['zipname'];

  if (!empty($zipname)) {
    //variable_del('github_zip');
    $zip = zip_open($zipname);
    $za = new ZipArchive();

    if ($za->open($zipname) !== TRUE) {
      $message = t('Could not open zip file containing styles: @file', array('@file' => realpath($zipname)));
      $message = check_plain($message);
      drupal_set_message($message, 'error');
      return;
    }
    if (empty($context['sandbox'])) {
     $context['sandbox']['progress'] = 0;
     $context['results']['install_count'] = 0;
    }

    $num_files = $za->numFiles;
    $start = $context['sandbox']['progress'];
    $end = min(($start+50), $num_files);

    for ($i = $start; $i < $end; $i++) {
      $name = $za->getNameIndex($i);
      $name = basename($name);
      if (strstr($name, '.csl')) {
        $csl = $za->getFromIndex($i);
        _install_csl($name, $csl, NULL, TRUE);
        $context['results']['install_count']++;
      }
      $context['sandbox']['progress']++;
    }
    $za->close();

    if ($context['sandbox']['progress'] != $num_files) {
      $context['finished'] = $context['sandbox']['progress'] / $num_files;
    }
  }
}

function _csl_import_batch_finished($success, $results, $operations) {
  $zipname = variable_get('github_zip', '');
  file_unmanaged_delete($zipname);
  variable_del('github_zip');
}

function biblio_citeproc_example_citation() {
  global $language;
  $contributors = array(
    0 => array(
      'lastname' => 'Oneauth',
      'firstname' => 'Joe',
      'initials' => 'A',
      'auth_category' => 1,
      'cid' => -1),
    1 => array(
      'lastname' => 'Twoauth',
      'firstname' => 'John',
      'initials' => 'B',
      'auth_category' => 2,
      'cid' => -2),
  );
  $node = new stdClass();
  $node->nid = -1;
  $node->title                  = 'This is a fantastic title.';
  $node->biblio_contributors    = $contributors;
  $node->biblio_type            = 100;
  $node->biblio_year            = 2010;
  $node->biblio_volume          = 1;
  $node->biblio_issue           = 2;
  $node->biblio_secondary_title = 'Journal of Fantastic Articles';
  $node->biblio_pages           = '424-31';
  $node->biblio_coins           = '';
  $now = time();
  $node->biblio_access_date = array(
    format_date($now, 'custom', 'Y'),
    format_date($now, 'custom', 'n'),
    format_date($now, 'custom', 'j'),
  );
  return theme_biblio_citeproc_style(array('node' => $node));

}

function biblio_citeproc_csl_editor($form, &$form_state, $style) {

  $csl = db_query('SELECT id,parent,csl FROM {biblio_citeproc_styles} WHERE filename = :id', array(':id' => $style))->fetchObject();
  if (!isset($csl->csl)) {
    drupal_set_message(t('Biblio-CiteProc could not fetch the style file: @csl_id from the database. Check your CiteProc settings.', array('@csl_id' => $style)), 'error');
    return;
  }
  if (!empty($csl->parent)) {
    $csl = db_query("SELECT id,csl FROM {biblio_citeproc_styles} WHERE id = :id", array(':id' => $csl->parent))->fetchObject();

  }
  if (isset($csl->csl)) {
    $csl_file_contents = $csl->csl;
  }

  $form['editor'] = array(
      '#title'         => t('Editing %style', array('%style' => $style)),
      '#type'          => 'text_format',
      '#rows'          => 40,
      '#format'        => 'csl',
      '#default_value' => $csl_file_contents,
  );
  $form['save'] = array(
      '#value' => t('Save'),
      '#type'  => 'submit',
  );
  $form['cancel'] = array(
      '#value' => t('Cancel'),
      '#type'  => 'submit',
  );
  $form['style'] = array(
      '#value' => $style,
      '#type'  => 'hidden',
  );
  $form['id'] = array(
      '#value' => $csl->id,
      '#type'  => 'hidden',
  );

  return $form;
}
function biblio_citeproc_csl_editor_validate($form, &$form_state) {
  if ($form_state['triggering_element']['#value'] == t('Save')) {
    $csl = $form_state['values']['editor']['value'];
    $valid = biblio_citeproc_validate_csl($csl);
    if (!empty($valid)) {
      form_set_error('editor', $valid[0]);
    }
    else {
      $form_state['values']['editor']['value'] = $csl;
    }
  }
}
function biblio_citeproc_csl_editor_submit($form, &$form_state) {
  $form_state['redirect'] = 'admin/config/content/biblio/citeproc/styles';

  if ($form_state['triggering_element']['#value'] == t('Save')) {
    $csl = $form_state['values']['editor']['value'];
    $id  = $form_state['values']['id'];

    $record = array(
        'id'      => $id,
        'csl'     => $csl,
        'sha1'    => sha1($csl),
        'changed' => time(),
        'updated' => time(),
    );
    drupal_write_record('biblio_citeproc_styles', $record, 'id');
  }

}

function biblio_citeproc_validate_csl_file($file) {
  if ($file->source == 'import_csl_file') {
    $csl = file_get_contents($file->uri);
    return biblio_citeproc_validate_csl($csl);
  }
}

function biblio_citeproc_validate_csl($csl) {
  $rng_schema = drupal_get_path('module', 'biblio_citeproc') . '/schema/csl.rng';
  $doc = new DOMDocument();
  $doc->loadXML($csl);
  $updated = $doc->getElementsByTagName('updated')->item(0);
  $updated->nodeValue = date(DATE_ATOM, time());
  $valid = $doc->relaxNGValidate($rng_schema);
  $csl = $doc->saveXML();
  return ($valid) ? array() : array(t('The supplied CSL file did not pass CSL 1.0 validation'));

}

function biblio_citeproc_update_installed_batch(&$context = NULL) {
  $style_zip_file = variable_get('biblio_citeproc_styles_zip_file', FALSE);
  $zipname = $style_zip_file ? drupal_realpath($style_zip_file->uri) : FALSE;

  if (empty($context['sandbox']['installed'])) {
    $context['sandbox']['installed'] = array();

    $result = db_select('biblio_citeproc_styles', 'csl')
    ->fields('csl', array('filename', 'id', 'sha1', 'title', 'parent', 'changed', 'updated'))
    ->orderBy('filename', 'ASC')
    ->execute();

    foreach ($result as $style) {
      $context['sandbox']['installed'][] = $style;
    }

    $context['sandbox']['progress'] = 0;
    $context['results']['update_count'] = 0;
    $context['results']['updated'] = array();
  }

  if (!empty($zipname)) {
    //variable_del('github_zip');
    $zip = zip_open($zipname);
    $za = new ZipArchive();

    if ($za->open($zipname) !== TRUE) {
      $message = t('Could not open zip file containing styles: @file', array('@file' => realpath($zipname)));
      $message = check_plain($message);
      drupal_set_message($message, 'error');
      $context['finished'] = 1;
      return;
    }

    $num_files = count($context['sandbox']['installed']);
    $start = $context['sandbox']['progress'];
    $end = min(($start+10), $num_files);

    for ($i = $start; $i < $end; $i++) {
      $name = $context['sandbox']['installed'][$i]->filename;
      $changed = $context['sandbox']['installed'][$i]->changed;
      if (($index = $za->locateName($name, ZIPARCHIVE::FL_NOCASE|ZIPARCHIVE::FL_NODIR))  !== FALSE) {
        if (($csl = $za->getFromIndex($index)) && !$changed) {
          $ret = _install_csl($name, $csl, NULL, NULL, TRUE);
          if ($ret == 2) {
            $context['results']['updated'][] = $name;
            $context['message'] = t('Updated') . ': ' . $name;
          }
        }
      }
      $context['sandbox']['progress']++;
    }
    $za->close();

    if ($context['sandbox']['progress'] != $num_files) {
      $context['finished'] = $context['sandbox']['progress'] / $num_files;
    }
  }
}

function biblio_citeproc_update_installed_finished($success, $results, $operations) {
  if ($success) {
    if (count($results['updated'])) {
      $message = format_plural(count($results['updated']), 'The following style was updated.', 'The following @count styles were updated.');
      drupal_set_message($message);
      foreach ($results['updated'] as $style) {
        drupal_set_message($style);
      }
    }
    else {
      $message = t('No updates were found, all styles are current.');
      drupal_set_message($message);
    }
  }
  else {
    drupal_set_message(t('Finished with an error.'));
  }
}
