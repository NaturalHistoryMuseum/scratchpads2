<?php

/**
 * @file
 * To do views declarations.
 */

/**
 * Implementation of hook_views_handlers().
 */
function footnotes_views_views_handlers() {
  return array(
    'handlers' => array(
      'footnotes_views_handler_field' => array(
        'parent' => 'views_handler_field_markup',
      ),
      'footnotes_views_handler_field_body_content' => array(
        'parent' => 'footnotes_views_handler_field',
      ),
      'footnotes_views_handler_field_body_footnotes' => array(
        'parent' => 'footnotes_views_handler_field',
      ),
    ),
  );
}

/**
 * Implementation of hook_views_data().
 */
function footnotes_views_views_data() {
  $data = array();

  // node view extensions

  $data['node_revisions']['body_content'] = array(
    'group' => t('Node'),
    'title' => t('Body content'),
    'help' => t('The content of the body without the footnotes.'),
    'field' => array(
      'field' => 'body',
      'handler' => 'footnotes_views_handler_field_body_content',
      'format' => 'format',
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_string',
    ),
  );

  $data['node_revisions']['body_footnotes'] = array(
    'group' => t('Node'),
    'title' => t('Body footnotes'),
    'help' => t('The footnotes alone, without body.'),
    'field' => array(
      'field' => 'body',
      'handler' => 'footnotes_views_handler_field_body_footnotes',
      'format' => 'format',
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_string',
    ),
  );

  $data['node_revisions']['teaser_content'] = array(
    'group' => t('Node'),
    'title' => t('Teaser content'),
    'help' => t('The content of the teaser without the footnotes.'),
    'field' => array(
      'field' => 'teaser',
      'handler' => 'footnotes_views_handler_field_body_content',
      'format' => 'format',
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_string',
    ),
  );

  $data['node_revisions']['teaser_footnotes'] = array(
    'group' => t('Node'),
    'title' => t('Teaser footnotes'),
    'help' => t('The footnotes alone, without the teaser.'),
    'field' => array(
      'field' => 'teaser',
      'handler' => 'footnotes_views_handler_field_body_footnotes',
      'format' => 'format',
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_string',
    ),
  );

  return $data;
}

// vim: ts=2 sw=2 et syntax=php
