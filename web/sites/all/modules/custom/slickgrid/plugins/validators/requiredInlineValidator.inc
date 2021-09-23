<?php
/**
 * @file
 * Provides an inline cell editor
 */
// Plugin definition
$plugin = array(
  'title' => t('Required inline cell'),
  'description' => t('Validate required inline cells.'),
  'js' => array(
    'file' => 'slickgrid.validators.js',
  ),
  'field_types' => array(
    'text_default',
    'number_integer'
  ),
);