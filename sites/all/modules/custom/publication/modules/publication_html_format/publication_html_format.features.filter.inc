<?php
/**
 * @file
 * publication_html_format.features.filter.inc
 */

/**
 * Implements hook_filter_default_formats().
 */
function publication_html_format_filter_default_formats() {
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
          'filter_url_length' => '72',
        ),
      ),
      'filter_html' => array(
        'weight' => '-48',
        'status' => '1',
        'settings' => array(
          'allowed_html' => '<div> <iframe> <hr> <i> <a> <em> <strong> <cite> <blockquote> <code> <ul> <ol> <li> <dl> <dt> <dd> <table> <thead> <tbody> <tr> <th> <td> <caption> <img> <pre> <p> <sub> <sup> <br> <h1> <h2> <h3> <h4> <h5> <h6> <u>',
          'filter_html_help' => 1,
          'filter_html_nofollow' => 0,
        ),
      ),
      'filter_autop' => array(
        'weight' => '-47',
        'status' => '1',
        'settings' => array(),
      ),
      'filter_htmlcorrector' => array(
        'weight' => '-46',
        'status' => '1',
        'settings' => array(),
      ),
      'media_filter' => array(
        'weight' => '-45',
        'status' => '1',
        'settings' => array(),
      ),
      'emptyparagraphkiller' => array(
        'weight' => '-44',
        'status' => '1',
        'settings' => array(),
      ),
    ),
  );

  return $formats;
}