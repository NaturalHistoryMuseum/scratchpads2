<?php
/**
 * @file
 * Provides an drop-down cell editor
 */
// Plugin definition
$plugin = array(
  'title' => t('Controlled character'),
  'description' => t('An drop down controlled character editor.'),
  'js' => array(
    'file' => 'character_editor.editors.js'
  ),
  'field_types' => array(
    'inline_character',
  ),
  'handler' => array(
    'class' => 'character_editor_handler'
  ),
  'form_id' => 'character_editor_state_form',
  'process' => 'character_editor_inline_cell_process'
);