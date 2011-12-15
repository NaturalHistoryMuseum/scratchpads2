<?php
function biblio_citeproc_style_manager_form($form, &$form_state) {
  $form = array();
  $options = array();

  if ($has_curl = function_exists('curl_init')) {
    $cache = cache_get('github_csl_repo');
    if (!$cache || $cache->expire < time()) {
      module_load_include('php', 'biblio_citeproc', '/Github/Autoloader');
      Github_Autoloader::register();
      $github = new Github_Client();
      $branches = $github->getRepoApi()->getRepoBranches('citation-style-language', 'styles');
      $tree = $github->getObjectApi()->listBlobs('citation-style-language', 'styles', $branches['master']);

      $tree = array_flip($tree);
      $options = array();
      foreach ($tree as $sha => $file) {
        if (strstr($file, '.csl')) {
          $options[$sha] =  basename($file);
        }
      }
      if (!empty($options)) {
        $expire = time()+86400; //one day from now
        cache_set('github_csl_repo', $options, 'cache', $expire);
      }
    }
    else {
      $options = $cache->data;
    }
  }

  if (!$has_curl) {
    $message = t('Additional styles cannot be '
        . 'installed because the PHP <a href="@curl_url">cURL</a> library is '
        . 'not available.',
        array('@curl_url' => 'http://www.php.net/manual/en/book.curl.php'));
    drupal_set_message($message, 'warning');
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
    '#disabled' => !$has_curl,
    '#description' => t('Choose the styles you would like to download and install.'),
  );

  $form['install'] = array(
    '#type' => 'submit',
    '#disabled' => !$has_curl,
    '#value' => t('<--')
  );
  $form['remove'] = array(
    '#type' => 'submit',
    '#disabled' => !$has_curl,
    '#value' => t('-->')
  );
  $form['default'] = array(
    '#type' => 'submit',
    '#value' => t('Set as site default')
  );
  $form['install_all'] = array(
    '#type' => 'submit',
    '#disabled' => !$has_curl,
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
    '#markup' => empty($titles) ? '' : $titles[variable_get('biblio_citeproc_style', 'cse.csl')],
  );

  return $form;
}
function theme_biblio_citeproc_style_manager_form($variables) {
  $form = $variables['form'];
  $rows = array();
  $rows[] = array(
            array('data' => t('Current default style')),
            array('data' => '<b>' . drupal_render($form['current_default']) . '</b>'),
            );
  $rows[] = array(
            array('data' => t('Example citation:')),
            array('data' => biblio_citeproc_example_citation()));
  $output = theme('table', array('rows' => $rows));
  $rows = array();
  $rows[] = array(
            array('data' => drupal_render($form['installed_styles']) . '<br>' . drupal_render($form['default'])),
            array('data' => drupal_render($form['install']) . '<br>' . drupal_render($form['remove'])),
            array('data' => drupal_render($form['avialable_styles']) . '<br>' . drupal_render($form['install_all'])),
            );
  $output .= theme('table', array('rows' => $rows));

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
    $batch_op = array(
      'title' => t('Importing all styles from GitHub repository'),
      'operations' => array(
        array('_get_zip_from_github', array()),
        array('_install_from_zip', array()),
      ),
      'progressive' => TRUE,
      'finished' => '_csl_import_batch_finished',
      'init_message' => t('Downloading file...'),
      'progress_message' => t('Saving styles...'),
      'file' => './' . drupal_get_path('module', 'biblio_citeproc') . '/biblio_citeproc.admin.inc'
    );
    batch_set($batch_op);
    batch_process('admin/config/content/biblio/styles');

  }

}
function _install_csl($name, $csl, $sha = NULL, $all = FALSE) {
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
  foreach ($xml->info->link as $link) {
    $attrs = $link->attributes();
    if (isset($attrs['rel']) && $attrs['rel'] == 'independent-parent') {
      $parent = (string)$attrs['href'];
    }
  }
  if (!$all && !empty($parent)) {
    $csl_file_contents = db_query("SELECT csl FROM biblio_citeproc_styles WHERE id = :parent", array(':parent' => $parent))->fetchField();
    if (!$csl_file_contents) {
      drupal_set_message(t('You do not have the parent style file: !parent_id installed. You must install !parent_id before you can use !csl_id', array('!parent_id' => basename($parent), '!csl_id' => $name)), 'error');
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
  );

  if (!array_key_exists($record['id'], $installed)) {
    db_insert('biblio_citeproc_styles')->fields($record)->execute();
    $installed[$record['id']] = TRUE;
  }
  elseif ($record['sha1'] != $installed[$record['id']]->sha1) {
    db_update('biblio_citeproc_styles')->condition('id', $record['id'])->fields($record)->execute();
  }
}

function _get_zip_from_github(&$context = NULL) {
  $zip_url = 'https://github.com/citation-style-language/styles/zipball/master';
  $zip_file = system_retrieve_file($zip_url);

  $filename = drupal_realpath($zip_file);
  if (!empty($filename)) {
    variable_set('github_zip', $filename);
  }
}
function _install_from_zip(&$context = NULL) {

  $destination = variable_get('github_zip', '');

  if (!empty($destination)) {
    $zip = zip_open($destination);
    $za = new ZipArchive();

    if ($za->open($destination) !== TRUE) {
      $message = t('Could not open zip file containing styles: !file', array('!file' => realpath($destination)));
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
  $destination = variable_get('github_zip', '');
  file_unmanaged_delete($destination);
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
      'auth_category' => 1,
      'cid' => -2),
  );
  $node = new stdClass();
  $node->nid = -1;
  $node->title                  = 'This is a fantasic title.';
  $node->biblio_contributors    = $contributors;
  $node->biblio_type            = 102;
  $node->biblio_year            = 2010;
  $node->biblio_volume          = 1;
  $node->biblio_issue           = 2;
  $node->biblio_secondary_title = 'Journal of Fantastic Articles';
  $node->biblio_pages           = '	424-31';
  $node->biblio_coins           = '';
  return theme_biblio_citeproc_style(array('node' => $node));

}
function biblio_citeproc_csl_editor() {
//  <script type="text/javascript" src="js/ext/jquery-1.4.js"></script>
  drupal_add_js(drupal_get_path('module', 'biblio_citeproc') . 'js/ext/jquery-color.js');
  drupal_add_js(drupal_get_path('module', 'biblio_citeproc') . '/js/ext/GLR/GLR.js');
  drupal_add_js(drupal_get_path('module', 'biblio_citeproc') . '/js/ext/GLR/GLR.messenger.js');
  drupal_add_js(drupal_get_path('module', 'biblio_citeproc') . '/js/loc/xmlEditor.js');
  drupal_add_css(drupal_get_path('module', 'biblio_citeproc') . '/css/main.css');

}