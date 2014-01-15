<?php
$plugin = array(
  'title' => t('Character Flag Editor'),
  'description' => t('Edit a character\'s flag - from code only'),
  'handler' => array(
    'class' => 'character_metadata_editor_handler'
  ),
  'field_types' => array_keys(field_info_formatter_types()), // Works for all fields
  'process' => 'slickgrid_plugin_modal_form_process',
  'error' => 'slickgrid_plugin_modal_form_error',
  'form_id' => 'slickgrid_editor_form'
); 