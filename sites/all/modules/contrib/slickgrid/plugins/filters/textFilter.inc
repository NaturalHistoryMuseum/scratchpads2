<?php
/**
 * @file
 * Provides an inline cell editor
 */
// Plugin definition
$plugin = array(
  'title' => t('Text filter'),
  'description' => t('A simple text filter.'),
  'js' => array(
    'file' => 'slickgrid.filters.js',
    'class' => 'Slick.Filter.Text'
  ),
);