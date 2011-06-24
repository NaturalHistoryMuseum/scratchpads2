<?php
/**
 * @file
 * An example formflow plugin
 */
// Plugin definition
$plugin = array(
  'flow' => array(
    'title' => t('Example flow'),
    'description' => t('An example flow using ctools plugin.'),
    'path' => 'formflow/example',
    'show_trail' => 1,
    'show_back' => 1,
    'show_cancel' => 1,
    'steps' => array(
      array(
        'form_id' => 'ctools_ajax_sample_start',
        'title' => 'Ctools example'
      ),
      array(
        'form_id' => 'page_node_form',
        'title' => 'Page',
        'path' => 'node/add/page'
      )
    )
  )
);