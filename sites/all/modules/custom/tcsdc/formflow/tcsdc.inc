<?php
/**
 * Taxon Concept Schema - Form flow.
 */
$plugin = array(
  'flow' => array(
    'title' => t('Taxon Concept Schema Import'),
		'name' => 'tcsdc_import',
    'description' => t('Download and convert TCS from an external resource.'),
    'path' => 'admin/structure/taxonomy/import',
    'show_trail' => 1,
    'show_back' => 1,
    'show_cancel' => 1,
    'steps' => array(
      array(
        'form_id' => 'tcsdc_primary_form',
        'title' => 'Select TCS'
      ),
      array(
        'form_id' => 'page_node_form',
        'title' => 'Page'
      )
    )
  )
);