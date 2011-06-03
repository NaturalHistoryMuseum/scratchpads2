<?php
/**
 * @file
 * Provides an inline cell editor
 */
// Plugin definition
$plugin = array(
  'title' => t('Collapsible filter'),
  'description' => t('A simple collapsible filter.'),
  'js' => array(
    'file' => 'slickgrid.filters.js',
    'class' => 'Slick.Filter.Collapsible'
  )
);