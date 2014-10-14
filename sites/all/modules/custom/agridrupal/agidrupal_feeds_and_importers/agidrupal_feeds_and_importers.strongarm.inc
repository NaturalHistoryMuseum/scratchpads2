<?php
/**
 * @file
 * agidrupal_feeds_and_importers.strongarm.inc
 */

/**
 * Implements hook_strongarm().
 */
function agidrupal_feeds_and_importers_strongarm() {
  $export = array();

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'comment_agris_authors_feed';
  $strongarm->value = '2';
  $export['comment_agris_authors_feed'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'comment_agris_authors_upload';
  $strongarm->value = '2';
  $export['comment_agris_authors_upload'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'comment_agris_conferences_feed';
  $strongarm->value = '2';
  $export['comment_agris_conferences_feed'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'comment_agris_corporate_feed';
  $strongarm->value = '2';
  $export['comment_agris_corporate_feed'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'comment_agris_dlio_upload';
  $strongarm->value = '2';
  $export['comment_agris_dlio_upload'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'comment_agris_journals_feed';
  $strongarm->value = '2';
  $export['comment_agris_journals_feed'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'comment_agris_publishers_feed';
  $strongarm->value = '2';
  $export['comment_agris_publishers_feed'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'comment_anonymous_agris_authors_feed';
  $strongarm->value = 0;
  $export['comment_anonymous_agris_authors_feed'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'comment_anonymous_agris_authors_upload';
  $strongarm->value = 0;
  $export['comment_anonymous_agris_authors_upload'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'comment_anonymous_agris_conferences_feed';
  $strongarm->value = 0;
  $export['comment_anonymous_agris_conferences_feed'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'comment_anonymous_agris_corporate_feed';
  $strongarm->value = 0;
  $export['comment_anonymous_agris_corporate_feed'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'comment_anonymous_agris_dlio_upload';
  $strongarm->value = 0;
  $export['comment_anonymous_agris_dlio_upload'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'comment_anonymous_agris_journals_feed';
  $strongarm->value = 0;
  $export['comment_anonymous_agris_journals_feed'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'comment_anonymous_agris_publishers_feed';
  $strongarm->value = 0;
  $export['comment_anonymous_agris_publishers_feed'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'comment_anonymous_books_feed';
  $strongarm->value = 0;
  $export['comment_anonymous_books_feed'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'comment_anonymous_xmlimport_dlios';
  $strongarm->value = 0;
  $export['comment_anonymous_xmlimport_dlios'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'comment_books_feed';
  $strongarm->value = '2';
  $export['comment_books_feed'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'comment_controls_agris_authors_feed';
  $strongarm->value = '3';
  $export['comment_controls_agris_authors_feed'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'comment_controls_agris_authors_upload';
  $strongarm->value = '3';
  $export['comment_controls_agris_authors_upload'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'comment_controls_agris_conferences_feed';
  $strongarm->value = '3';
  $export['comment_controls_agris_conferences_feed'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'comment_controls_agris_corporate_feed';
  $strongarm->value = '3';
  $export['comment_controls_agris_corporate_feed'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'comment_controls_agris_dlio_upload';
  $strongarm->value = '3';
  $export['comment_controls_agris_dlio_upload'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'comment_controls_agris_journals_feed';
  $strongarm->value = '3';
  $export['comment_controls_agris_journals_feed'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'comment_controls_agris_publishers_feed';
  $strongarm->value = '3';
  $export['comment_controls_agris_publishers_feed'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'comment_controls_books_feed';
  $strongarm->value = '3';
  $export['comment_controls_books_feed'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'comment_controls_xmlimport_dlios';
  $strongarm->value = '3';
  $export['comment_controls_xmlimport_dlios'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'comment_default_mode_agris_authors_feed';
  $strongarm->value = 1;
  $export['comment_default_mode_agris_authors_feed'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'comment_default_mode_agris_authors_upload';
  $strongarm->value = 1;
  $export['comment_default_mode_agris_authors_upload'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'comment_default_mode_agris_conferences_feed';
  $strongarm->value = 1;
  $export['comment_default_mode_agris_conferences_feed'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'comment_default_mode_agris_corporate_feed';
  $strongarm->value = 1;
  $export['comment_default_mode_agris_corporate_feed'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'comment_default_mode_agris_dlio_upload';
  $strongarm->value = 1;
  $export['comment_default_mode_agris_dlio_upload'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'comment_default_mode_agris_journals_feed';
  $strongarm->value = 1;
  $export['comment_default_mode_agris_journals_feed'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'comment_default_mode_agris_publishers_feed';
  $strongarm->value = 1;
  $export['comment_default_mode_agris_publishers_feed'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'comment_default_mode_books_feed';
  $strongarm->value = 1;
  $export['comment_default_mode_books_feed'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'comment_default_mode_xmlimport_dlios';
  $strongarm->value = 1;
  $export['comment_default_mode_xmlimport_dlios'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'comment_default_order_agris_authors_feed';
  $strongarm->value = '1';
  $export['comment_default_order_agris_authors_feed'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'comment_default_order_agris_authors_upload';
  $strongarm->value = '1';
  $export['comment_default_order_agris_authors_upload'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'comment_default_order_agris_conferences_feed';
  $strongarm->value = '1';
  $export['comment_default_order_agris_conferences_feed'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'comment_default_order_agris_corporate_feed';
  $strongarm->value = '1';
  $export['comment_default_order_agris_corporate_feed'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'comment_default_order_agris_dlio_upload';
  $strongarm->value = '1';
  $export['comment_default_order_agris_dlio_upload'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'comment_default_order_agris_journals_feed';
  $strongarm->value = '1';
  $export['comment_default_order_agris_journals_feed'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'comment_default_order_agris_publishers_feed';
  $strongarm->value = '1';
  $export['comment_default_order_agris_publishers_feed'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'comment_default_order_books_feed';
  $strongarm->value = '1';
  $export['comment_default_order_books_feed'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'comment_default_order_xmlimport_dlios';
  $strongarm->value = '1';
  $export['comment_default_order_xmlimport_dlios'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'comment_default_per_page_agris_authors_feed';
  $strongarm->value = '50';
  $export['comment_default_per_page_agris_authors_feed'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'comment_default_per_page_agris_authors_upload';
  $strongarm->value = '50';
  $export['comment_default_per_page_agris_authors_upload'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'comment_default_per_page_agris_conferences_feed';
  $strongarm->value = '50';
  $export['comment_default_per_page_agris_conferences_feed'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'comment_default_per_page_agris_corporate_feed';
  $strongarm->value = '50';
  $export['comment_default_per_page_agris_corporate_feed'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'comment_default_per_page_agris_dlio_upload';
  $strongarm->value = '50';
  $export['comment_default_per_page_agris_dlio_upload'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'comment_default_per_page_agris_journals_feed';
  $strongarm->value = '50';
  $export['comment_default_per_page_agris_journals_feed'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'comment_default_per_page_agris_publishers_feed';
  $strongarm->value = '50';
  $export['comment_default_per_page_agris_publishers_feed'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'comment_default_per_page_books_feed';
  $strongarm->value = '50';
  $export['comment_default_per_page_books_feed'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'comment_default_per_page_xmlimport_dlios';
  $strongarm->value = '50';
  $export['comment_default_per_page_xmlimport_dlios'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'comment_form_location_agris_authors_feed';
  $strongarm->value = '0';
  $export['comment_form_location_agris_authors_feed'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'comment_form_location_agris_authors_upload';
  $strongarm->value = '0';
  $export['comment_form_location_agris_authors_upload'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'comment_form_location_agris_conferences_feed';
  $strongarm->value = '0';
  $export['comment_form_location_agris_conferences_feed'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'comment_form_location_agris_corporate_feed';
  $strongarm->value = '0';
  $export['comment_form_location_agris_corporate_feed'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'comment_form_location_agris_dlio_upload';
  $strongarm->value = '0';
  $export['comment_form_location_agris_dlio_upload'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'comment_form_location_agris_journals_feed';
  $strongarm->value = '0';
  $export['comment_form_location_agris_journals_feed'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'comment_form_location_agris_publishers_feed';
  $strongarm->value = '0';
  $export['comment_form_location_agris_publishers_feed'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'comment_form_location_books_feed';
  $strongarm->value = '0';
  $export['comment_form_location_books_feed'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'comment_form_location_xmlimport_dlios';
  $strongarm->value = '0';
  $export['comment_form_location_xmlimport_dlios'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'comment_preview_agris_authors_feed';
  $strongarm->value = 2;
  $export['comment_preview_agris_authors_feed'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'comment_preview_agris_authors_upload';
  $strongarm->value = 2;
  $export['comment_preview_agris_authors_upload'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'comment_preview_agris_conferences_feed';
  $strongarm->value = 2;
  $export['comment_preview_agris_conferences_feed'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'comment_preview_agris_corporate_feed';
  $strongarm->value = 2;
  $export['comment_preview_agris_corporate_feed'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'comment_preview_agris_dlio_upload';
  $strongarm->value = 2;
  $export['comment_preview_agris_dlio_upload'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'comment_preview_agris_journals_feed';
  $strongarm->value = 2;
  $export['comment_preview_agris_journals_feed'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'comment_preview_agris_publishers_feed';
  $strongarm->value = 2;
  $export['comment_preview_agris_publishers_feed'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'comment_preview_books_feed';
  $strongarm->value = 2;
  $export['comment_preview_books_feed'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'comment_preview_xmlimport_dlios';
  $strongarm->value = 2;
  $export['comment_preview_xmlimport_dlios'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'comment_subject_field_agris_authors_feed';
  $strongarm->value = '1';
  $export['comment_subject_field_agris_authors_feed'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'comment_subject_field_agris_authors_upload';
  $strongarm->value = '1';
  $export['comment_subject_field_agris_authors_upload'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'comment_subject_field_agris_conferences_feed';
  $strongarm->value = '1';
  $export['comment_subject_field_agris_conferences_feed'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'comment_subject_field_agris_corporate_feed';
  $strongarm->value = '1';
  $export['comment_subject_field_agris_corporate_feed'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'comment_subject_field_agris_dlio_upload';
  $strongarm->value = '1';
  $export['comment_subject_field_agris_dlio_upload'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'comment_subject_field_agris_journals_feed';
  $strongarm->value = '1';
  $export['comment_subject_field_agris_journals_feed'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'comment_subject_field_agris_publishers_feed';
  $strongarm->value = '1';
  $export['comment_subject_field_agris_publishers_feed'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'comment_subject_field_books_feed';
  $strongarm->value = '1';
  $export['comment_subject_field_books_feed'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'comment_subject_field_xmlimport_dlios';
  $strongarm->value = '1';
  $export['comment_subject_field_xmlimport_dlios'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'comment_xmlimport_dlios';
  $strongarm->value = '0';
  $export['comment_xmlimport_dlios'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'field_bundle_settings_node__agris_authors_feed';
  $strongarm->value = array(
    'extra_fields' => array(
      'form' => array(
        'body' => array(
          'weight' => 0,
        ),
      ),
      'display' => array(),
    ),
    'view_modes' => array(),
  );
  $export['field_bundle_settings_node__agris_authors_feed'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'field_bundle_settings_node__agris_authors_upload';
  $strongarm->value = array(
    'extra_fields' => array(
      'form' => array(
        'body' => array(
          'weight' => 0,
        ),
      ),
      'display' => array(),
    ),
    'view_modes' => array(),
  );
  $export['field_bundle_settings_node__agris_authors_upload'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'field_bundle_settings_node__agris_conferences_feed';
  $strongarm->value = array(
    'extra_fields' => array(
      'form' => array(
        'body' => array(
          'weight' => 0,
        ),
      ),
      'display' => array(),
    ),
    'view_modes' => array(),
  );
  $export['field_bundle_settings_node__agris_conferences_feed'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'field_bundle_settings_node__agris_corporate_feed';
  $strongarm->value = array(
    'extra_fields' => array(
      'form' => array(
        'body' => array(
          'weight' => 0,
        ),
      ),
      'display' => array(),
    ),
    'view_modes' => array(),
  );
  $export['field_bundle_settings_node__agris_corporate_feed'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'field_bundle_settings_node__agris_dlio_upload';
  $strongarm->value = array(
    'extra_fields' => array(
      'form' => array(
        'body' => array(
          'weight' => 0,
        ),
      ),
      'display' => array(),
    ),
    'view_modes' => array(),
  );
  $export['field_bundle_settings_node__agris_dlio_upload'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'field_bundle_settings_node__agris_journals_feed';
  $strongarm->value = array(
    'extra_fields' => array(
      'form' => array(
        'body' => array(
          'weight' => 0,
        ),
      ),
      'display' => array(),
    ),
    'view_modes' => array(),
  );
  $export['field_bundle_settings_node__agris_journals_feed'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'field_bundle_settings_node__agris_publishers_feed';
  $strongarm->value = array(
    'extra_fields' => array(
      'form' => array(
        'body' => array(
          'weight' => 0,
        ),
      ),
      'display' => array(),
    ),
    'view_modes' => array(),
  );
  $export['field_bundle_settings_node__agris_publishers_feed'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'field_bundle_settings_node__books_feed';
  $strongarm->value = array(
    'extra_fields' => array(
      'form' => array(
        'body' => array(
          'weight' => 0,
        ),
      ),
      'display' => array(),
    ),
    'view_modes' => array(),
  );
  $export['field_bundle_settings_node__books_feed'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'field_bundle_settings_node__xmlimport_dlios';
  $strongarm->value = array(
    'extra_fields' => array(
      'form' => array(
        'body' => array(
          'weight' => 0,
        ),
      ),
      'display' => array(),
    ),
    'view_modes' => array(),
  );
  $export['field_bundle_settings_node__xmlimport_dlios'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'language_content_type_agris_authors_feed';
  $strongarm->value = '0';
  $export['language_content_type_agris_authors_feed'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'language_content_type_agris_authors_upload';
  $strongarm->value = '0';
  $export['language_content_type_agris_authors_upload'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'language_content_type_agris_conferences_feed';
  $strongarm->value = '0';
  $export['language_content_type_agris_conferences_feed'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'language_content_type_agris_corporate_feed';
  $strongarm->value = '0';
  $export['language_content_type_agris_corporate_feed'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'language_content_type_agris_dlio_upload';
  $strongarm->value = '0';
  $export['language_content_type_agris_dlio_upload'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'language_content_type_agris_journals_feed';
  $strongarm->value = '0';
  $export['language_content_type_agris_journals_feed'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'language_content_type_agris_publishers_feed';
  $strongarm->value = '0';
  $export['language_content_type_agris_publishers_feed'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'language_content_type_books_feed';
  $strongarm->value = '0';
  $export['language_content_type_books_feed'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'language_content_type_xmlimport_dlios';
  $strongarm->value = '0';
  $export['language_content_type_xmlimport_dlios'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'node_options_agris_authors_feed';
  $strongarm->value = array(
    0 => 'status',
  );
  $export['node_options_agris_authors_feed'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'node_options_agris_authors_upload';
  $strongarm->value = array(
    0 => 'status',
    1 => 'promote',
  );
  $export['node_options_agris_authors_upload'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'node_options_agris_conferences_feed';
  $strongarm->value = array(
    0 => 'status',
    1 => 'promote',
  );
  $export['node_options_agris_conferences_feed'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'node_options_agris_corporate_feed';
  $strongarm->value = array(
    0 => 'status',
    1 => 'promote',
  );
  $export['node_options_agris_corporate_feed'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'node_options_agris_dlio_upload';
  $strongarm->value = array(
    0 => 'status',
    1 => 'promote',
  );
  $export['node_options_agris_dlio_upload'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'node_options_agris_journals_feed';
  $strongarm->value = array(
    0 => 'status',
    1 => 'promote',
  );
  $export['node_options_agris_journals_feed'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'node_options_agris_publishers_feed';
  $strongarm->value = array(
    0 => 'status',
    1 => 'promote',
  );
  $export['node_options_agris_publishers_feed'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'node_options_books_feed';
  $strongarm->value = array(
    0 => 'status',
    1 => 'promote',
  );
  $export['node_options_books_feed'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'node_options_xmlimport_dlios';
  $strongarm->value = array(
    0 => 'status',
  );
  $export['node_options_xmlimport_dlios'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'node_preview_agris_authors_feed';
  $strongarm->value = 1;
  $export['node_preview_agris_authors_feed'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'node_preview_agris_authors_upload';
  $strongarm->value = 1;
  $export['node_preview_agris_authors_upload'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'node_preview_agris_conferences_feed';
  $strongarm->value = 1;
  $export['node_preview_agris_conferences_feed'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'node_preview_agris_corporate_feed';
  $strongarm->value = 1;
  $export['node_preview_agris_corporate_feed'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'node_preview_agris_dlio_upload';
  $strongarm->value = 1;
  $export['node_preview_agris_dlio_upload'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'node_preview_agris_journals_feed';
  $strongarm->value = 1;
  $export['node_preview_agris_journals_feed'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'node_preview_agris_publishers_feed';
  $strongarm->value = 1;
  $export['node_preview_agris_publishers_feed'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'node_preview_books_feed';
  $strongarm->value = 1;
  $export['node_preview_books_feed'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'node_preview_xmlimport_dlios';
  $strongarm->value = 1;
  $export['node_preview_xmlimport_dlios'] = $strongarm;

  return $export;
}
