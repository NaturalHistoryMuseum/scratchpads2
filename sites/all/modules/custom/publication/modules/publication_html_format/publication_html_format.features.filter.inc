<?php

/**
 * @file
 * publication_html_format.features.filter.inc
 */
/**
 * Implements hook_filter_default_formats().
 */
function publication_html_format_filter_default_formats(){
  $formats = array();
  // Exported format: Publication HTML
  $formats['publication_html'] = array(
    'format' => 'publication_html',
    'name' => 'Publication HTML',
    'cache' => '0',
    'status' => '1',
    'weight' => '0',
    'filters' => array(
      'filter_url' => array(
        'weight' => '-49',
        'status' => '1',
        'settings' => array(
          'filter_url_length' => '72'
        )
      ),
      'filter_autop' => array(
        'weight' => '-47',
        'status' => '1',
        'settings' => array()
      ),
      'filter_htmlcorrector' => array(
        'weight' => '-46',
        'status' => '1',
        'settings' => array()
      ),
      'media_filter' => array(
        'weight' => '-45',
        'status' => '1',
        'settings' => array()
      ),
      'emptyparagraphkiller' => array(
        'weight' => '-44',
        'status' => '1',
        'settings' => array()
      )
    )
  );
  return $formats;
}