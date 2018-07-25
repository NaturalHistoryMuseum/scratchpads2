<?php
/**
 * @file
 * Provides an inline cell editor
 */
// Plugin definition
$plugin = array(
  'title' => t('Collapsible taxonomy filter'),
  'description' => t('A simple collapsible filter.'),
  'js' => array(
    'file' => 'slickgrid.filters.js',
    'class' => 'Slick.Filter.Collapsible'
  ),
  'field_types' => array(
    'taxonomy_term_reference_link',
    'taxonomy_term_reference_plain'
  )
);