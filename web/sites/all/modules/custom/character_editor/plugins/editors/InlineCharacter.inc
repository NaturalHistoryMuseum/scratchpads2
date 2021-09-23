<?php
/**
 * @file
 * Provides an inline cell editor
 */
// Plugin definition
$plugin = array(
  'title' => t('Inline character'),
  'description' => t('An inline character editor.'),
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