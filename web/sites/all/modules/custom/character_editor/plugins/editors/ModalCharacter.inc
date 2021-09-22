<?php
/**
 * @file
 * Provides an inline cell editor
 */
// Plugin definition
$plugin = array(
  'title' => t('Modal character'),
  'description' => t('A modal character editor (used for text & DNA).'),
  'js' => array(
    'file' => 'character_editor.editors.js'
  ),
  'field_types' => array(
    'modal_character'
  ),
  'handler' => array(
    'class' => 'character_editor_handler'
  ),
  'form_id' => 'character_editor_state_form',
  'process' => 'character_editor_state_form_process'
);