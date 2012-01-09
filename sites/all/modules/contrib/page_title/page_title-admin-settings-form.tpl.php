<?php

$form = &$variables['form'];

$rows = array();

foreach (element_children($form['pattern']) as $key) {
  $title = array(
    '#markup' => $form['pattern'][$key]['#title'],
    '#required' => $form['pattern'][$key]['#required'],
  );
  unset($form['patterns']['pattern'][$key]['#title']);

  $row = array(
    array('data' => render($title), 'class' => 'page-type'),
    array('data' => render($form['scope'][$key]), 'class' => 'scope'),
  );

  if (isset($form['showfield'][$key .'_showfield'])) {
    $row[] = array('data' => render($form['pattern'][$key]), 'class' => 'pattern');
    $row[] = array('data' => render($form['showfield'][$key .'_showfield']), 'class' => 'showfield');
  }
  else {
    $row[] = array('data' => render($form['pattern'][$key]), 'colspan' => 2, 'class' => 'pattern');
  }
  $rows[] = $row;
}

$headers = array(
  array('data' => t('Page Type'),   'class' => 'page-type'),
  array('data' => t('Token Scope'), 'class' => 'scope'),
  array('data' => t('Pattern'),     'class' => 'pattern'),
  array('data' => t('Show Field'),  'class' => 'showfield'),
);
drupal_add_css(drupal_get_path('module', 'page_title') .'/page_title.admin.css', array('type' => 'file', 'preprocess' => FALSE));

print theme('table', array('header' => $headers, 'rows' => $rows, 'attributed' => array('id' => 'page-title-settings')));
//print drupal_render_children($form);
