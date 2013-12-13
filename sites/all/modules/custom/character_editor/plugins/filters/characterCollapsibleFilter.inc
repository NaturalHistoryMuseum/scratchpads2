<?php
$plugin = array(
  'title' => t('Collapsible Character filter'),
  'description' => t('A simple collapsible filter for the character editor.'),
  'js' => array(
    'file' => 'charactereditor.filters.js',
    'class' => 'Slick.Filter.CharacterCollapsible'
  ),
  'field_types' => array(
    'character_entity_field'
  )
);