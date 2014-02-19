<?php
$plugin = array(
  'title' => t('Character editor taxon/specimen filter'),
  'description' => t('Character Editor filter which provides both collapsible filter and text input filter for the taxon/specimen column'),
  'js' => array(
    'file' => 'charactereditor.filters.js',
    'class' => 'Slick.Filter.CharacterTaxonFilter'
  ),
  'field_types' => array(
    'character_entity_field'
  )
);