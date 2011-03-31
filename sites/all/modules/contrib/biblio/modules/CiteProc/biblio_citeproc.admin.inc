<?php
function biblio_citeproc_style_manager_form($form, &$form_state) {
  $form = array();
  module_load_include('php', 'biblio_citeproc', '/Github/Autoloader');
  Github_Autoloader::register();
  $github = new Github_Client();
  $branches = $github->getRepoApi()->getRepoBranches('citation-style-language', 'styles');
  $tree = $github->getObjectApi()->listBlobs('citation-style-language', 'styles', $branches['master']);

  $tree = array_flip($tree);
  $options = array();
  foreach ($tree as $sha => $file) {
    if(strstr($file, '.csl')) {
      $options[$sha] =  basename($file);
    }
  }
  $form['remote_names'] = array(
    '#type' => 'value',
    '#value' => $options,
  );

  asort($options);
  $form['avialable_styles'] = array(
    '#type' => 'select',
    '#title' => t('Available styles'),
    '#size' => 15,
    '#multiple' => TRUE,
    '#description' => t('Choose the styles you would like to download and install.'),
  );

  $form['install'] = array(
    '#type' => 'submit',
    '#value' => t('<--')
  );
  $form['remove'] = array(
    '#type' => 'submit',
    '#value' => t('-->')
  );
  $form['default'] = array(
    '#type' => 'submit',
    '#value' => t('Set as site default')
  );
  $form['install_all'] = array(
    '#type' => 'submit',
    '#value' => t('Install all')
  );

  $result = db_select('biblio_citeproc_styles', 'csl')
    ->fields('csl', array('filename', 'id', 'sha1', 'title'))
    ->orderBy('filename', 'ASC')
    ->execute();

  $titles = array();
  $installed = array();
  foreach ($result as $style) {
    $installed[$style->id] = $style->filename;
    $titles[$style->filename] = $style->title;

    if (($sha = array_search($style->filename, $options))) {
      unset($options[$sha]);
    }
  }
  $form['avialable_styles']['#options'] = $options;

  $form['installed_styles'] = array(
    '#type' => 'select',
    '#title' => t('Installed styles'),
    '#size' => 15,
    '#options' => biblio_get_styles(),
    '#multiple' => TRUE,
    '#description' => t('Currently installed styles.'),
  );

  $form['current_default'] = array(
    '#markup' => $titles[variable_get('biblio_citeproc_style', 'cse.csl')],
  );

  return $form;
}
function theme_biblio_citeproc_style_manager_form($variables) {
  $form = $variables['form'];
  $rows = array();
  $rows[] = array(
            array('data' => drupal_render($form['installed_styles']). '<br>' . drupal_render($form['default'])),
            array('data' => drupal_render($form['install']) . '<br>' . drupal_render($form['remove'])),
            array('data' => drupal_render($form['avialable_styles']). '<br>' . drupal_render($form['install_all'])),
            );
  $rows[] = array(
            array('data' => t('Current default style') . ': <b>' . drupal_render($form['current_default']) . '</b>', 'colspan' => array(3)),

            );
  $output = theme('table', array('rows' => $rows));
  $output .= drupal_render_children($form);
  return $output;

}
function biblio_citeproc_style_manager_form_validate($form, &$form_state) {
  if ($form_state['clicked_button']['#value'] == '<--' && count( $form_state['values']['avialable_styles'])) {
    if (count($form_state['values']['avialable_styles']) > 60) {
      $message = t('You may not select more than 60 styles for installation at one time');
      form_error($form['avialable_styles'], $message);
    }
  }
 if ($form_state['clicked_button']['#value'] ==  t('Set as site default') && !count( $form_state['values']['installed_styles'])) {
   form_error($form['installed_styles'], t('You must select an installed style to set as the default.'));
 }

}
function biblio_citeproc_style_manager_form_submit($form, &$form_state) {
  if ($form_state['clicked_button']['#value'] == '<--' && count( $form_state['values']['avialable_styles'])) {
    module_load_include('php', 'biblio_citeproc', '/Github/Autoloader');
    Github_Autoloader::register();
    $github = new Github_Client();
    foreach ( $form_state['values']['avialable_styles'] as $sha) {
      $csl = $github->getObjectApi()->getRawData('citation-style-language', 'styles', $sha);
      _install_csl($form_state['values']['remote_names'][$sha], $csl, $sha);
    }
  }

  if ($form_state['clicked_button']['#value'] == '-->' && count( $form_state['values']['installed_styles'])) {
    foreach ($form_state['values']['installed_styles'] as $id) {
      db_delete('biblio_citeproc_styles')->condition('filename', $id)->execute();
    }
  }

  if ($form_state['clicked_button']['#value'] == t('Set as site default') &&
      count( $form_state['values']['installed_styles'])) {
    $def = array_shift($form_state['values']['installed_styles']);
    variable_set('biblio_citeproc_style', $def);
  }

  if ($form_state['clicked_button']['#value'] == t('Install all')) {
    _install_from_zip();
  }

}
function _install_csl($name, $csl, $sha = NULL) {
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

  $parent = '';
  foreach($xml->info->link as $link) {
    $attrs = $link->attributes();
    if (isset($attrs['rel']) && $attrs['rel'] == 'independent-parent') {
      $parent = (string)$attrs['href'];
    }
  }

  $sha1 = (isset($sha)) ? $sha : sha1($csl);

  $record  = array (
    'filename' => $name,
    'parent'	 => $parent,
    'title'    => (string)$xml->info->title,
    'summary'  => (string)$xml->info->summary,
    'csl'      => $csl,
    'sha1'     => $sha1,
    'id'       => (string)$xml->info->id,
  );

  if (!array_key_exists($record['id'], $installed)) {
    db_insert('biblio_citeproc_styles')->fields($record)->execute();
    $installed[$record['id']] = TRUE;
  }
  elseif ($record['sha1'] != $installed[$record['id']]->sha1) {
    db_update('biblio_citeproc_styles')->condition('id', $record['id'])->fields($record)->execute();
  }
}
function _install_from_zip() {
  $zip_url = 'http://github.com/citation-style-language/styles/zipball/master';
  $zip_file = system_retrieve_file($zip_url);

  $zip = zip_open(drupal_realpath($zip_file));
  if(!is_resource($zip)){
    $message = t('Could not open zip file containing styles: !file', array('!file' => drupal_realpath($zip_file)));
    drupal_set_message($message, 'error');
    return;
  }
  if ($zip) {
    while ($zip_entry = zip_read($zip)) {
      $name =  zip_entry_name($zip_entry);
      $name = basename($name);
      if (strstr($name, '.csl')  && zip_entry_open($zip, $zip_entry, "r")) {
        $csl = zip_entry_read($zip_entry, zip_entry_filesize($zip_entry));
        _install_csl($name, $csl);
        zip_entry_close($zip_entry);
      }
    }
    zip_close($zip);
  }
}