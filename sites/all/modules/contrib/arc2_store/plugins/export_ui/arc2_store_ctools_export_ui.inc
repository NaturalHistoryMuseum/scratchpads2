<?php

/**
 * @file
 * CTools Export UI plugin for arc2 store settings.
 */

$plugin = array(
  'schema' => 'arc2_store_settings',

  'menu' => array(
    'menu prefix' => 'admin/config/services',
    'menu item' => 'arc2_store',
    'menu title' => 'ARC2 store settings',
    'menu description' => 'Configure ARC2 stores.',
  ),

  'title singular' => t('store'),
  'title plural' => t('stores'),
  'title singular proper' => t('ARC2 store'),
  'title plural proper' => t('ARC2 stores'),

  // Define the names of the functions that provide the add/edit forms.
  'form' => array(
    'settings' => 'arc2_store_export_ui_form',
  ),

  'handler' => array(
    'class' => 'arc2_store_export_ui',
    'parent' => 'ctools_export_ui',
  ),
);
