<?php
/**
 * @file
 * agidrupal_feeds_and_importers.feeds_tamper_default.inc
 */

/**
 * Implements hook_feeds_tamper_default().
 */
function agidrupal_feeds_and_importers_feeds_tamper_default() {
  $export = array();

  $feeds_tamper = new stdClass();
  $feeds_tamper->disabled = FALSE; /* Edit this to true to make a default feeds_tamper disabled initially */
  $feeds_tamper->api_version = 2;
  $feeds_tamper->id = '-xpathparser_22-trimj';
  $feeds_tamper->importer = '';
  $feeds_tamper->source = 'xpathparser:22';
  $feeds_tamper->plugin_id = 'trim';
  $feeds_tamper->settings = array(
    'mask' => '',
    'side' => 'trim',
  );
  $feeds_tamper->weight = 0;
  $feeds_tamper->description = 'Trim';
  $export['-xpathparser_22-trimj'] = $feeds_tamper;

  $feeds_tamper = new stdClass();
  $feeds_tamper->disabled = FALSE; /* Edit this to true to make a default feeds_tamper disabled initially */
  $feeds_tamper->api_version = 2;
  $feeds_tamper->id = '-xpathparser_28-replace_type';
  $feeds_tamper->importer = '';
  $feeds_tamper->source = 'xpathparser:28';
  $feeds_tamper->plugin_id = 'find_replace';
  $feeds_tamper->settings = array(
    'find' => 'KV',
    'replace' => 'Journal Article',
    'case_sensitive' => 1,
    'word_boundaries' => 1,
    'whole' => 1,
    'regex' => TRUE,
    'regex_find' => '/^KV$/',
  );
  $feeds_tamper->weight = 0;
  $feeds_tamper->description = 'Find replace KV with Journal Article';
  $export['-xpathparser_28-replace_type'] = $feeds_tamper;

  $feeds_tamper = new stdClass();
  $feeds_tamper->disabled = FALSE; /* Edit this to true to make a default feeds_tamper disabled initially */
  $feeds_tamper->api_version = 2;
  $feeds_tamper->id = '-xpathparser_9-find_replace_regex';
  $feeds_tamper->importer = '';
  $feeds_tamper->source = 'xpathparser:9';
  $feeds_tamper->plugin_id = 'find_replace_regex';
  $feeds_tamper->settings = array(
    'find' => '/^$/',
    'replace' => 'en',
    'limit' => '1',
    'real_limit' => '1',
  );
  $feeds_tamper->weight = 0;
  $feeds_tamper->description = 'Find replace REGEX';
  $export['-xpathparser_9-find_replace_regex'] = $feeds_tamper;

  $feeds_tamper = new stdClass();
  $feeds_tamper->disabled = FALSE; /* Edit this to true to make a default feeds_tamper disabled initially */
  $feeds_tamper->api_version = 2;
  $feeds_tamper->id = 'ag_books_feed-xpathparser_1-find_replace_regex';
  $feeds_tamper->importer = 'ag_books_feed';
  $feeds_tamper->source = 'xpathparser:1';
  $feeds_tamper->plugin_id = 'find_replace_regex';
  $feeds_tamper->settings = array(
    'find' => '/\\n/',
    'replace' => '',
    'limit' => '',
    'real_limit' => -1,
  );
  $feeds_tamper->weight = 2;
  $feeds_tamper->description = 'Find replace REGEX';
  $export['ag_books_feed-xpathparser_1-find_replace_regex'] = $feeds_tamper;

  $feeds_tamper = new stdClass();
  $feeds_tamper->disabled = FALSE; /* Edit this to true to make a default feeds_tamper disabled initially */
  $feeds_tamper->api_version = 2;
  $feeds_tamper->id = 'ag_books_feed-xpathparser_1-trim';
  $feeds_tamper->importer = 'ag_books_feed';
  $feeds_tamper->source = 'xpathparser:1';
  $feeds_tamper->plugin_id = 'trim';
  $feeds_tamper->settings = array(
    'mask' => '',
    'side' => 'trim',
  );
  $feeds_tamper->weight = 3;
  $feeds_tamper->description = 'Trim';
  $export['ag_books_feed-xpathparser_1-trim'] = $feeds_tamper;

  $feeds_tamper = new stdClass();
  $feeds_tamper->disabled = FALSE; /* Edit this to true to make a default feeds_tamper disabled initially */
  $feeds_tamper->api_version = 2;
  $feeds_tamper->id = 'ag_books_feed-xpathparser_1-truncate_to_255';
  $feeds_tamper->importer = 'ag_books_feed';
  $feeds_tamper->source = 'xpathparser:1';
  $feeds_tamper->plugin_id = 'truncate_text';
  $feeds_tamper->settings = array(
    'num_char' => '255',
    'ellipses' => 0,
  );
  $feeds_tamper->weight = 4;
  $feeds_tamper->description = 'Truncate to 255';
  $export['ag_books_feed-xpathparser_1-truncate_to_255'] = $feeds_tamper;

  $feeds_tamper = new stdClass();
  $feeds_tamper->disabled = FALSE; /* Edit this to true to make a default feeds_tamper disabled initially */
  $feeds_tamper->api_version = 2;
  $feeds_tamper->id = 'ag_books_feed-xpathparser_2-find_replace_regex';
  $feeds_tamper->importer = 'ag_books_feed';
  $feeds_tamper->source = 'xpathparser:2';
  $feeds_tamper->plugin_id = 'find_replace_regex';
  $feeds_tamper->settings = array(
    'find' => '/\\n/',
    'replace' => '',
    'limit' => '',
    'real_limit' => -1,
  );
  $feeds_tamper->weight = 5;
  $feeds_tamper->description = 'Find replace REGEX';
  $export['ag_books_feed-xpathparser_2-find_replace_regex'] = $feeds_tamper;

  $feeds_tamper = new stdClass();
  $feeds_tamper->disabled = FALSE; /* Edit this to true to make a default feeds_tamper disabled initially */
  $feeds_tamper->api_version = 2;
  $feeds_tamper->id = 'ag_books_feed-xpathparser_2-trim';
  $feeds_tamper->importer = 'ag_books_feed';
  $feeds_tamper->source = 'xpathparser:2';
  $feeds_tamper->plugin_id = 'trim';
  $feeds_tamper->settings = array(
    'mask' => '255',
    'side' => 'trim',
  );
  $feeds_tamper->weight = 6;
  $feeds_tamper->description = 'Trim';
  $export['ag_books_feed-xpathparser_2-trim'] = $feeds_tamper;

  $feeds_tamper = new stdClass();
  $feeds_tamper->disabled = FALSE; /* Edit this to true to make a default feeds_tamper disabled initially */
  $feeds_tamper->api_version = 2;
  $feeds_tamper->id = 'ag_books_feed-xpathparser_2-truncate_to_255';
  $feeds_tamper->importer = 'ag_books_feed';
  $feeds_tamper->source = 'xpathparser:2';
  $feeds_tamper->plugin_id = 'truncate_text';
  $feeds_tamper->settings = array(
    'num_char' => '255',
    'ellipses' => 0,
  );
  $feeds_tamper->weight = 7;
  $feeds_tamper->description = 'Truncate to 255';
  $export['ag_books_feed-xpathparser_2-truncate_to_255'] = $feeds_tamper;

  $feeds_tamper = new stdClass();
  $feeds_tamper->disabled = FALSE; /* Edit this to true to make a default feeds_tamper disabled initially */
  $feeds_tamper->api_version = 2;
  $feeds_tamper->id = 'ag_books_feed-xpathparser_4-find_replace_regex';
  $feeds_tamper->importer = 'ag_books_feed';
  $feeds_tamper->source = 'xpathparser:4';
  $feeds_tamper->plugin_id = 'find_replace_regex';
  $feeds_tamper->settings = array(
    'find' => '/.*/',
    'replace' => 'Book',
    'limit' => '1',
    'real_limit' => '1',
  );
  $feeds_tamper->weight = 0;
  $feeds_tamper->description = 'Find replace REGEX';
  $export['ag_books_feed-xpathparser_4-find_replace_regex'] = $feeds_tamper;

  $feeds_tamper = new stdClass();
  $feeds_tamper->disabled = FALSE; /* Edit this to true to make a default feeds_tamper disabled initially */
  $feeds_tamper->api_version = 2;
  $feeds_tamper->id = 'ag_jobs_feed_importer-xpathparser_0-trim';
  $feeds_tamper->importer = 'ag_jobs_feed_importer';
  $feeds_tamper->source = 'xpathparser:0';
  $feeds_tamper->plugin_id = 'trim';
  $feeds_tamper->settings = array(
    'mask' => '',
    'side' => 'trim',
  );
  $feeds_tamper->weight = 1;
  $feeds_tamper->description = 'Trim';
  $export['ag_jobs_feed_importer-xpathparser_0-trim'] = $feeds_tamper;

  $feeds_tamper = new stdClass();
  $feeds_tamper->disabled = FALSE; /* Edit this to true to make a default feeds_tamper disabled initially */
  $feeds_tamper->api_version = 2;
  $feeds_tamper->id = 'ag_jobs_feed_importer-xpathparser_0-truncate';
  $feeds_tamper->importer = 'ag_jobs_feed_importer';
  $feeds_tamper->source = 'xpathparser:0';
  $feeds_tamper->plugin_id = 'truncate_text';
  $feeds_tamper->settings = array(
    'num_char' => '255',
    'ellipses' => 0,
  );
  $feeds_tamper->weight = 2;
  $feeds_tamper->description = 'Truncate';
  $export['ag_jobs_feed_importer-xpathparser_0-truncate'] = $feeds_tamper;

  $feeds_tamper = new stdClass();
  $feeds_tamper->disabled = FALSE; /* Edit this to true to make a default feeds_tamper disabled initially */
  $feeds_tamper->api_version = 2;
  $feeds_tamper->id = 'agris_ap_pre_importer_conferences-xpathparser_0-find_replace_regex';
  $feeds_tamper->importer = 'agris_ap_pre_importer_conferences';
  $feeds_tamper->source = 'xpathparser:0';
  $feeds_tamper->plugin_id = 'find_replace_regex';
  $feeds_tamper->settings = array(
    'find' => '/\\n/',
    'replace' => '',
    'limit' => '1',
    'real_limit' => '1',
  );
  $feeds_tamper->weight = 2;
  $feeds_tamper->description = 'Replace newline with space';
  $export['agris_ap_pre_importer_conferences-xpathparser_0-find_replace_regex'] = $feeds_tamper;

  $feeds_tamper = new stdClass();
  $feeds_tamper->disabled = FALSE; /* Edit this to true to make a default feeds_tamper disabled initially */
  $feeds_tamper->api_version = 2;
  $feeds_tamper->id = 'agris_ap_pre_importer_conferences-xpathparser_0-trim';
  $feeds_tamper->importer = 'agris_ap_pre_importer_conferences';
  $feeds_tamper->source = 'xpathparser:0';
  $feeds_tamper->plugin_id = 'trim';
  $feeds_tamper->settings = array(
    'mask' => '',
    'side' => 'trim',
  );
  $feeds_tamper->weight = 4;
  $feeds_tamper->description = 'Trim';
  $export['agris_ap_pre_importer_conferences-xpathparser_0-trim'] = $feeds_tamper;

  $feeds_tamper = new stdClass();
  $feeds_tamper->disabled = FALSE; /* Edit this to true to make a default feeds_tamper disabled initially */
  $feeds_tamper->api_version = 2;
  $feeds_tamper->id = 'agris_ap_pre_importer_conferences-xpathparser_0-truncate';
  $feeds_tamper->importer = 'agris_ap_pre_importer_conferences';
  $feeds_tamper->source = 'xpathparser:0';
  $feeds_tamper->plugin_id = 'truncate_text';
  $feeds_tamper->settings = array(
    'num_char' => '255',
    'ellipses' => 0,
  );
  $feeds_tamper->weight = 3;
  $feeds_tamper->description = 'Truncate';
  $export['agris_ap_pre_importer_conferences-xpathparser_0-truncate'] = $feeds_tamper;

  $feeds_tamper = new stdClass();
  $feeds_tamper->disabled = FALSE; /* Edit this to true to make a default feeds_tamper disabled initially */
  $feeds_tamper->api_version = 2;
  $feeds_tamper->id = 'agris_ap_pre_importer_conferences-xpathparser_1-find_replace_regex';
  $feeds_tamper->importer = 'agris_ap_pre_importer_conferences';
  $feeds_tamper->source = 'xpathparser:1';
  $feeds_tamper->plugin_id = 'find_replace_regex';
  $feeds_tamper->settings = array(
    'find' => '/\\n/',
    'replace' => '',
    'limit' => '1',
    'real_limit' => '1',
  );
  $feeds_tamper->weight = 1;
  $feeds_tamper->description = 'Replace newline with space';
  $export['agris_ap_pre_importer_conferences-xpathparser_1-find_replace_regex'] = $feeds_tamper;

  $feeds_tamper = new stdClass();
  $feeds_tamper->disabled = FALSE; /* Edit this to true to make a default feeds_tamper disabled initially */
  $feeds_tamper->api_version = 2;
  $feeds_tamper->id = 'agris_ap_pre_importer_conferences-xpathparser_1-trim';
  $feeds_tamper->importer = 'agris_ap_pre_importer_conferences';
  $feeds_tamper->source = 'xpathparser:1';
  $feeds_tamper->plugin_id = 'trim';
  $feeds_tamper->settings = array(
    'mask' => '',
    'side' => 'trim',
  );
  $feeds_tamper->weight = 3;
  $feeds_tamper->description = 'Trim';
  $export['agris_ap_pre_importer_conferences-xpathparser_1-trim'] = $feeds_tamper;

  $feeds_tamper = new stdClass();
  $feeds_tamper->disabled = FALSE; /* Edit this to true to make a default feeds_tamper disabled initially */
  $feeds_tamper->api_version = 2;
  $feeds_tamper->id = 'agris_ap_pre_importer_conferences-xpathparser_1-truncate';
  $feeds_tamper->importer = 'agris_ap_pre_importer_conferences';
  $feeds_tamper->source = 'xpathparser:1';
  $feeds_tamper->plugin_id = 'truncate_text';
  $feeds_tamper->settings = array(
    'num_char' => '255',
    'ellipses' => 0,
  );
  $feeds_tamper->weight = 2;
  $feeds_tamper->description = 'Truncate';
  $export['agris_ap_pre_importer_conferences-xpathparser_1-truncate'] = $feeds_tamper;

  $feeds_tamper = new stdClass();
  $feeds_tamper->disabled = FALSE; /* Edit this to true to make a default feeds_tamper disabled initially */
  $feeds_tamper->api_version = 2;
  $feeds_tamper->id = 'agris_ap_pre_importer_corporate_authors-xpathparser_0-find_replace_regex';
  $feeds_tamper->importer = 'agris_ap_pre_importer_corporate_authors';
  $feeds_tamper->source = 'xpathparser:0';
  $feeds_tamper->plugin_id = 'find_replace_regex';
  $feeds_tamper->settings = array(
    'find' => '/\\n/',
    'replace' => '',
    'limit' => '1',
    'real_limit' => '1',
  );
  $feeds_tamper->weight = 2;
  $feeds_tamper->description = 'Replace newline with space';
  $export['agris_ap_pre_importer_corporate_authors-xpathparser_0-find_replace_regex'] = $feeds_tamper;

  $feeds_tamper = new stdClass();
  $feeds_tamper->disabled = FALSE; /* Edit this to true to make a default feeds_tamper disabled initially */
  $feeds_tamper->api_version = 2;
  $feeds_tamper->id = 'agris_ap_pre_importer_corporate_authors-xpathparser_0-trim';
  $feeds_tamper->importer = 'agris_ap_pre_importer_corporate_authors';
  $feeds_tamper->source = 'xpathparser:0';
  $feeds_tamper->plugin_id = 'trim';
  $feeds_tamper->settings = array(
    'mask' => '',
    'side' => 'trim',
  );
  $feeds_tamper->weight = 4;
  $feeds_tamper->description = 'Trim';
  $export['agris_ap_pre_importer_corporate_authors-xpathparser_0-trim'] = $feeds_tamper;

  $feeds_tamper = new stdClass();
  $feeds_tamper->disabled = FALSE; /* Edit this to true to make a default feeds_tamper disabled initially */
  $feeds_tamper->api_version = 2;
  $feeds_tamper->id = 'agris_ap_pre_importer_corporate_authors-xpathparser_0-truncate';
  $feeds_tamper->importer = 'agris_ap_pre_importer_corporate_authors';
  $feeds_tamper->source = 'xpathparser:0';
  $feeds_tamper->plugin_id = 'truncate_text';
  $feeds_tamper->settings = array(
    'num_char' => '255',
    'ellipses' => 0,
  );
  $feeds_tamper->weight = 3;
  $feeds_tamper->description = 'Truncate';
  $export['agris_ap_pre_importer_corporate_authors-xpathparser_0-truncate'] = $feeds_tamper;

  $feeds_tamper = new stdClass();
  $feeds_tamper->disabled = FALSE; /* Edit this to true to make a default feeds_tamper disabled initially */
  $feeds_tamper->api_version = 2;
  $feeds_tamper->id = 'agris_ap_pre_importer_corporate_authors-xpathparser_1-find_replace_regex';
  $feeds_tamper->importer = 'agris_ap_pre_importer_corporate_authors';
  $feeds_tamper->source = 'xpathparser:1';
  $feeds_tamper->plugin_id = 'find_replace_regex';
  $feeds_tamper->settings = array(
    'find' => '/\\n/',
    'replace' => '',
    'limit' => '1',
    'real_limit' => '1',
  );
  $feeds_tamper->weight = 1;
  $feeds_tamper->description = 'Replace newline with space';
  $export['agris_ap_pre_importer_corporate_authors-xpathparser_1-find_replace_regex'] = $feeds_tamper;

  $feeds_tamper = new stdClass();
  $feeds_tamper->disabled = FALSE; /* Edit this to true to make a default feeds_tamper disabled initially */
  $feeds_tamper->api_version = 2;
  $feeds_tamper->id = 'agris_ap_pre_importer_corporate_authors-xpathparser_1-trim';
  $feeds_tamper->importer = 'agris_ap_pre_importer_corporate_authors';
  $feeds_tamper->source = 'xpathparser:1';
  $feeds_tamper->plugin_id = 'trim';
  $feeds_tamper->settings = array(
    'mask' => '',
    'side' => 'trim',
  );
  $feeds_tamper->weight = 3;
  $feeds_tamper->description = 'Trim';
  $export['agris_ap_pre_importer_corporate_authors-xpathparser_1-trim'] = $feeds_tamper;

  $feeds_tamper = new stdClass();
  $feeds_tamper->disabled = FALSE; /* Edit this to true to make a default feeds_tamper disabled initially */
  $feeds_tamper->api_version = 2;
  $feeds_tamper->id = 'agris_ap_pre_importer_corporate_authors-xpathparser_1-truncate';
  $feeds_tamper->importer = 'agris_ap_pre_importer_corporate_authors';
  $feeds_tamper->source = 'xpathparser:1';
  $feeds_tamper->plugin_id = 'truncate_text';
  $feeds_tamper->settings = array(
    'num_char' => '255',
    'ellipses' => 0,
  );
  $feeds_tamper->weight = 2;
  $feeds_tamper->description = 'Truncate';
  $export['agris_ap_pre_importer_corporate_authors-xpathparser_1-truncate'] = $feeds_tamper;

  $feeds_tamper = new stdClass();
  $feeds_tamper->disabled = FALSE; /* Edit this to true to make a default feeds_tamper disabled initially */
  $feeds_tamper->api_version = 2;
  $feeds_tamper->id = 'agris_ap_pre_importer_journals-xpathparser_0-find_replace_regex';
  $feeds_tamper->importer = 'agris_ap_pre_importer_journals';
  $feeds_tamper->source = 'xpathparser:0';
  $feeds_tamper->plugin_id = 'find_replace_regex';
  $feeds_tamper->settings = array(
    'find' => '/\\n/',
    'replace' => '',
    'limit' => '1',
    'real_limit' => '1',
  );
  $feeds_tamper->weight = 2;
  $feeds_tamper->description = 'Replace newline with space';
  $export['agris_ap_pre_importer_journals-xpathparser_0-find_replace_regex'] = $feeds_tamper;

  $feeds_tamper = new stdClass();
  $feeds_tamper->disabled = FALSE; /* Edit this to true to make a default feeds_tamper disabled initially */
  $feeds_tamper->api_version = 2;
  $feeds_tamper->id = 'agris_ap_pre_importer_journals-xpathparser_0-trim';
  $feeds_tamper->importer = 'agris_ap_pre_importer_journals';
  $feeds_tamper->source = 'xpathparser:0';
  $feeds_tamper->plugin_id = 'trim';
  $feeds_tamper->settings = array(
    'mask' => '',
    'side' => 'trim',
  );
  $feeds_tamper->weight = 4;
  $feeds_tamper->description = 'Trim';
  $export['agris_ap_pre_importer_journals-xpathparser_0-trim'] = $feeds_tamper;

  $feeds_tamper = new stdClass();
  $feeds_tamper->disabled = FALSE; /* Edit this to true to make a default feeds_tamper disabled initially */
  $feeds_tamper->api_version = 2;
  $feeds_tamper->id = 'agris_ap_pre_importer_journals-xpathparser_0-truncate';
  $feeds_tamper->importer = 'agris_ap_pre_importer_journals';
  $feeds_tamper->source = 'xpathparser:0';
  $feeds_tamper->plugin_id = 'truncate_text';
  $feeds_tamper->settings = array(
    'num_char' => '255',
    'ellipses' => 0,
  );
  $feeds_tamper->weight = 3;
  $feeds_tamper->description = 'Truncate';
  $export['agris_ap_pre_importer_journals-xpathparser_0-truncate'] = $feeds_tamper;

  $feeds_tamper = new stdClass();
  $feeds_tamper->disabled = FALSE; /* Edit this to true to make a default feeds_tamper disabled initially */
  $feeds_tamper->api_version = 2;
  $feeds_tamper->id = 'agris_ap_pre_importer_journals-xpathparser_1-find_replace_regex';
  $feeds_tamper->importer = 'agris_ap_pre_importer_journals';
  $feeds_tamper->source = 'xpathparser:1';
  $feeds_tamper->plugin_id = 'find_replace_regex';
  $feeds_tamper->settings = array(
    'find' => '/\\n/',
    'replace' => '',
    'limit' => '1',
    'real_limit' => '1',
  );
  $feeds_tamper->weight = 1;
  $feeds_tamper->description = 'Replace newline with space	';
  $export['agris_ap_pre_importer_journals-xpathparser_1-find_replace_regex'] = $feeds_tamper;

  $feeds_tamper = new stdClass();
  $feeds_tamper->disabled = FALSE; /* Edit this to true to make a default feeds_tamper disabled initially */
  $feeds_tamper->api_version = 2;
  $feeds_tamper->id = 'agris_ap_pre_importer_journals-xpathparser_1-trim';
  $feeds_tamper->importer = 'agris_ap_pre_importer_journals';
  $feeds_tamper->source = 'xpathparser:1';
  $feeds_tamper->plugin_id = 'trim';
  $feeds_tamper->settings = array(
    'mask' => '',
    'side' => 'trim',
  );
  $feeds_tamper->weight = 3;
  $feeds_tamper->description = 'Trim';
  $export['agris_ap_pre_importer_journals-xpathparser_1-trim'] = $feeds_tamper;

  $feeds_tamper = new stdClass();
  $feeds_tamper->disabled = FALSE; /* Edit this to true to make a default feeds_tamper disabled initially */
  $feeds_tamper->api_version = 2;
  $feeds_tamper->id = 'agris_ap_pre_importer_journals-xpathparser_1-truncate';
  $feeds_tamper->importer = 'agris_ap_pre_importer_journals';
  $feeds_tamper->source = 'xpathparser:1';
  $feeds_tamper->plugin_id = 'truncate_text';
  $feeds_tamper->settings = array(
    'num_char' => '255',
    'ellipses' => 0,
  );
  $feeds_tamper->weight = 2;
  $feeds_tamper->description = 'Truncate';
  $export['agris_ap_pre_importer_journals-xpathparser_1-truncate'] = $feeds_tamper;

  $feeds_tamper = new stdClass();
  $feeds_tamper->disabled = FALSE; /* Edit this to true to make a default feeds_tamper disabled initially */
  $feeds_tamper->api_version = 2;
  $feeds_tamper->id = 'agris_ap_pre_importer_journals-xpathparser_3-truncate';
  $feeds_tamper->importer = 'agris_ap_pre_importer_journals';
  $feeds_tamper->source = 'xpathparser:3';
  $feeds_tamper->plugin_id = 'truncate_text';
  $feeds_tamper->settings = array(
    'num_char' => '5',
    'ellipses' => 0,
  );
  $feeds_tamper->weight = 1;
  $feeds_tamper->description = 'Truncate';
  $export['agris_ap_pre_importer_journals-xpathparser_3-truncate'] = $feeds_tamper;

  $feeds_tamper = new stdClass();
  $feeds_tamper->disabled = FALSE; /* Edit this to true to make a default feeds_tamper disabled initially */
  $feeds_tamper->api_version = 2;
  $feeds_tamper->id = 'agris_ap_pre_importer_publishers-xpathparser_0-find_replace_regex';
  $feeds_tamper->importer = 'agris_ap_pre_importer_publishers';
  $feeds_tamper->source = 'xpathparser:0';
  $feeds_tamper->plugin_id = 'find_replace_regex';
  $feeds_tamper->settings = array(
    'find' => '/\\n/',
    'replace' => '',
    'limit' => '1',
    'real_limit' => '1',
  );
  $feeds_tamper->weight = 2;
  $feeds_tamper->description = 'Replace newline with space';
  $export['agris_ap_pre_importer_publishers-xpathparser_0-find_replace_regex'] = $feeds_tamper;

  $feeds_tamper = new stdClass();
  $feeds_tamper->disabled = FALSE; /* Edit this to true to make a default feeds_tamper disabled initially */
  $feeds_tamper->api_version = 2;
  $feeds_tamper->id = 'agris_ap_pre_importer_publishers-xpathparser_0-trim';
  $feeds_tamper->importer = 'agris_ap_pre_importer_publishers';
  $feeds_tamper->source = 'xpathparser:0';
  $feeds_tamper->plugin_id = 'trim';
  $feeds_tamper->settings = array(
    'mask' => '',
    'side' => 'trim',
  );
  $feeds_tamper->weight = 4;
  $feeds_tamper->description = 'Trim';
  $export['agris_ap_pre_importer_publishers-xpathparser_0-trim'] = $feeds_tamper;

  $feeds_tamper = new stdClass();
  $feeds_tamper->disabled = FALSE; /* Edit this to true to make a default feeds_tamper disabled initially */
  $feeds_tamper->api_version = 2;
  $feeds_tamper->id = 'agris_ap_pre_importer_publishers-xpathparser_0-truncate';
  $feeds_tamper->importer = 'agris_ap_pre_importer_publishers';
  $feeds_tamper->source = 'xpathparser:0';
  $feeds_tamper->plugin_id = 'truncate_text';
  $feeds_tamper->settings = array(
    'num_char' => '255',
    'ellipses' => 0,
  );
  $feeds_tamper->weight = 3;
  $feeds_tamper->description = 'Truncate';
  $export['agris_ap_pre_importer_publishers-xpathparser_0-truncate'] = $feeds_tamper;

  $feeds_tamper = new stdClass();
  $feeds_tamper->disabled = FALSE; /* Edit this to true to make a default feeds_tamper disabled initially */
  $feeds_tamper->api_version = 2;
  $feeds_tamper->id = 'agris_ap_pre_importer_publishers-xpathparser_1-find_replace_regex';
  $feeds_tamper->importer = 'agris_ap_pre_importer_publishers';
  $feeds_tamper->source = 'xpathparser:1';
  $feeds_tamper->plugin_id = 'find_replace_regex';
  $feeds_tamper->settings = array(
    'find' => '/\\n/',
    'replace' => '',
    'limit' => '1',
    'real_limit' => '1',
  );
  $feeds_tamper->weight = 1;
  $feeds_tamper->description = 'Replace newline with space';
  $export['agris_ap_pre_importer_publishers-xpathparser_1-find_replace_regex'] = $feeds_tamper;

  $feeds_tamper = new stdClass();
  $feeds_tamper->disabled = FALSE; /* Edit this to true to make a default feeds_tamper disabled initially */
  $feeds_tamper->api_version = 2;
  $feeds_tamper->id = 'agris_ap_pre_importer_publishers-xpathparser_1-trim';
  $feeds_tamper->importer = 'agris_ap_pre_importer_publishers';
  $feeds_tamper->source = 'xpathparser:1';
  $feeds_tamper->plugin_id = 'trim';
  $feeds_tamper->settings = array(
    'mask' => '',
    'side' => 'trim',
  );
  $feeds_tamper->weight = 3;
  $feeds_tamper->description = 'Trim';
  $export['agris_ap_pre_importer_publishers-xpathparser_1-trim'] = $feeds_tamper;

  $feeds_tamper = new stdClass();
  $feeds_tamper->disabled = FALSE; /* Edit this to true to make a default feeds_tamper disabled initially */
  $feeds_tamper->api_version = 2;
  $feeds_tamper->id = 'agris_ap_pre_importer_publishers-xpathparser_1-truncate';
  $feeds_tamper->importer = 'agris_ap_pre_importer_publishers';
  $feeds_tamper->source = 'xpathparser:1';
  $feeds_tamper->plugin_id = 'truncate_text';
  $feeds_tamper->settings = array(
    'num_char' => '255',
    'ellipses' => 0,
  );
  $feeds_tamper->weight = 2;
  $feeds_tamper->description = 'Truncate';
  $export['agris_ap_pre_importer_publishers-xpathparser_1-truncate'] = $feeds_tamper;

  $feeds_tamper = new stdClass();
  $feeds_tamper->disabled = FALSE; /* Edit this to true to make a default feeds_tamper disabled initially */
  $feeds_tamper->api_version = 2;
  $feeds_tamper->id = 'agris_apxml_Import-xpathparser_0-find_replace_regex';
  $feeds_tamper->importer = 'agris_apxml_Import';
  $feeds_tamper->source = 'xpathparser:0';
  $feeds_tamper->plugin_id = 'find_replace_regex';
  $feeds_tamper->settings = array(
    'find' => '/\\n/',
    'replace' => '',
    'limit' => '1',
    'real_limit' => '1',
  );
  $feeds_tamper->weight = 3;
  $feeds_tamper->description = 'Replace newline with space';
  $export['agris_apxml_Import-xpathparser_0-find_replace_regex'] = $feeds_tamper;

  $feeds_tamper = new stdClass();
  $feeds_tamper->disabled = FALSE; /* Edit this to true to make a default feeds_tamper disabled initially */
  $feeds_tamper->api_version = 2;
  $feeds_tamper->id = 'agris_apxml_Import-xpathparser_0-trimtitle';
  $feeds_tamper->importer = 'agris_apxml_Import';
  $feeds_tamper->source = 'xpathparser:0';
  $feeds_tamper->plugin_id = 'trim';
  $feeds_tamper->settings = array(
    'mask' => '',
    'side' => 'trim',
  );
  $feeds_tamper->weight = 4;
  $feeds_tamper->description = 'Trimtitle';
  $export['agris_apxml_Import-xpathparser_0-trimtitle'] = $feeds_tamper;

  $feeds_tamper = new stdClass();
  $feeds_tamper->disabled = FALSE; /* Edit this to true to make a default feeds_tamper disabled initially */
  $feeds_tamper->api_version = 2;
  $feeds_tamper->id = 'agris_apxml_Import-xpathparser_0-truncate';
  $feeds_tamper->importer = 'agris_apxml_Import';
  $feeds_tamper->source = 'xpathparser:0';
  $feeds_tamper->plugin_id = 'truncate_text';
  $feeds_tamper->settings = array(
    'num_char' => '255',
    'ellipses' => 0,
  );
  $feeds_tamper->weight = 5;
  $feeds_tamper->description = 'Truncate';
  $export['agris_apxml_Import-xpathparser_0-truncate'] = $feeds_tamper;

  $feeds_tamper = new stdClass();
  $feeds_tamper->disabled = FALSE; /* Edit this to true to make a default feeds_tamper disabled initially */
  $feeds_tamper->api_version = 2;
  $feeds_tamper->id = 'agris_apxml_Import-xpathparser_2-find_replace_regex';
  $feeds_tamper->importer = 'agris_apxml_Import';
  $feeds_tamper->source = 'xpathparser:2';
  $feeds_tamper->plugin_id = 'find_replace_regex';
  $feeds_tamper->settings = array(
    'find' => '/\\n/',
    'replace' => '',
    'limit' => '1',
    'real_limit' => '1',
  );
  $feeds_tamper->weight = 0;
  $feeds_tamper->description = 'Replace newline with space';
  $export['agris_apxml_Import-xpathparser_2-find_replace_regex'] = $feeds_tamper;

  $feeds_tamper = new stdClass();
  $feeds_tamper->disabled = FALSE; /* Edit this to true to make a default feeds_tamper disabled initially */
  $feeds_tamper->api_version = 2;
  $feeds_tamper->id = 'agris_apxml_Import-xpathparser_2-trim';
  $feeds_tamper->importer = 'agris_apxml_Import';
  $feeds_tamper->source = 'xpathparser:2';
  $feeds_tamper->plugin_id = 'trim';
  $feeds_tamper->settings = array(
    'mask' => '',
    'side' => 'trim',
  );
  $feeds_tamper->weight = 1;
  $feeds_tamper->description = 'Trim';
  $export['agris_apxml_Import-xpathparser_2-trim'] = $feeds_tamper;

  $feeds_tamper = new stdClass();
  $feeds_tamper->disabled = FALSE; /* Edit this to true to make a default feeds_tamper disabled initially */
  $feeds_tamper->api_version = 2;
  $feeds_tamper->id = 'agris_apxml_Import-xpathparser_2-truncate';
  $feeds_tamper->importer = 'agris_apxml_Import';
  $feeds_tamper->source = 'xpathparser:2';
  $feeds_tamper->plugin_id = 'truncate_text';
  $feeds_tamper->settings = array(
    'num_char' => '255',
    'ellipses' => 0,
  );
  $feeds_tamper->weight = 2;
  $feeds_tamper->description = 'Truncate';
  $export['agris_apxml_Import-xpathparser_2-truncate'] = $feeds_tamper;

  $feeds_tamper = new stdClass();
  $feeds_tamper->disabled = FALSE; /* Edit this to true to make a default feeds_tamper disabled initially */
  $feeds_tamper->api_version = 2;
  $feeds_tamper->id = 'agris_apxml_Import-xpathparser_21-trim_journal';
  $feeds_tamper->importer = 'agris_apxml_Import';
  $feeds_tamper->source = 'xpathparser:21';
  $feeds_tamper->plugin_id = 'trim';
  $feeds_tamper->settings = array(
    'mask' => '',
    'side' => 'trim',
  );
  $feeds_tamper->weight = 1;
  $feeds_tamper->description = 'Trim_journal';
  $export['agris_apxml_Import-xpathparser_21-trim_journal'] = $feeds_tamper;

  $feeds_tamper = new stdClass();
  $feeds_tamper->disabled = FALSE; /* Edit this to true to make a default feeds_tamper disabled initially */
  $feeds_tamper->api_version = 2;
  $feeds_tamper->id = 'agris_apxml_Import-xpathparser_21-truncate';
  $feeds_tamper->importer = 'agris_apxml_Import';
  $feeds_tamper->source = 'xpathparser:21';
  $feeds_tamper->plugin_id = 'truncate_text';
  $feeds_tamper->settings = array(
    'num_char' => '255',
    'ellipses' => 0,
  );
  $feeds_tamper->weight = 0;
  $feeds_tamper->description = 'Truncate';
  $export['agris_apxml_Import-xpathparser_21-truncate'] = $feeds_tamper;

  $feeds_tamper = new stdClass();
  $feeds_tamper->disabled = FALSE; /* Edit this to true to make a default feeds_tamper disabled initially */
  $feeds_tamper->api_version = 2;
  $feeds_tamper->id = 'agris_apxml_Import-xpathparser_22-trim';
  $feeds_tamper->importer = 'agris_apxml_Import';
  $feeds_tamper->source = 'xpathparser:22';
  $feeds_tamper->plugin_id = 'trim';
  $feeds_tamper->settings = array(
    'mask' => '',
    'side' => 'trim',
  );
  $feeds_tamper->weight = 1;
  $feeds_tamper->description = 'Trim';
  $export['agris_apxml_Import-xpathparser_22-trim'] = $feeds_tamper;

  $feeds_tamper = new stdClass();
  $feeds_tamper->disabled = FALSE; /* Edit this to true to make a default feeds_tamper disabled initially */
  $feeds_tamper->api_version = 2;
  $feeds_tamper->id = 'agris_apxml_Import-xpathparser_22-truncate';
  $feeds_tamper->importer = 'agris_apxml_Import';
  $feeds_tamper->source = 'xpathparser:22';
  $feeds_tamper->plugin_id = 'truncate_text';
  $feeds_tamper->settings = array(
    'num_char' => '255',
    'ellipses' => 0,
  );
  $feeds_tamper->weight = 0;
  $feeds_tamper->description = 'Truncate';
  $export['agris_apxml_Import-xpathparser_22-truncate'] = $feeds_tamper;

  $feeds_tamper = new stdClass();
  $feeds_tamper->disabled = FALSE; /* Edit this to true to make a default feeds_tamper disabled initially */
  $feeds_tamper->api_version = 2;
  $feeds_tamper->id = 'agris_apxml_Import-xpathparser_24-trim';
  $feeds_tamper->importer = 'agris_apxml_Import';
  $feeds_tamper->source = 'xpathparser:24';
  $feeds_tamper->plugin_id = 'trim';
  $feeds_tamper->settings = array(
    'mask' => 'trimcorporate',
    'side' => 'trim',
  );
  $feeds_tamper->weight = 1;
  $feeds_tamper->description = 'Trim';
  $export['agris_apxml_Import-xpathparser_24-trim'] = $feeds_tamper;

  $feeds_tamper = new stdClass();
  $feeds_tamper->disabled = FALSE; /* Edit this to true to make a default feeds_tamper disabled initially */
  $feeds_tamper->api_version = 2;
  $feeds_tamper->id = 'agris_apxml_Import-xpathparser_24-truncate';
  $feeds_tamper->importer = 'agris_apxml_Import';
  $feeds_tamper->source = 'xpathparser:24';
  $feeds_tamper->plugin_id = 'truncate_text';
  $feeds_tamper->settings = array(
    'num_char' => '255',
    'ellipses' => 0,
  );
  $feeds_tamper->weight = 0;
  $feeds_tamper->description = 'Truncate';
  $export['agris_apxml_Import-xpathparser_24-truncate'] = $feeds_tamper;

  $feeds_tamper = new stdClass();
  $feeds_tamper->disabled = FALSE; /* Edit this to true to make a default feeds_tamper disabled initially */
  $feeds_tamper->api_version = 2;
  $feeds_tamper->id = 'agris_apxml_Import-xpathparser_25-find_replace_regex';
  $feeds_tamper->importer = 'agris_apxml_Import';
  $feeds_tamper->source = 'xpathparser:25';
  $feeds_tamper->plugin_id = 'find_replace_regex';
  $feeds_tamper->settings = array(
    'find' => '/\\n/',
    'replace' => '',
    'limit' => '1',
    'real_limit' => '1',
  );
  $feeds_tamper->weight = 2;
  $feeds_tamper->description = 'Replace newline with space';
  $export['agris_apxml_Import-xpathparser_25-find_replace_regex'] = $feeds_tamper;

  $feeds_tamper = new stdClass();
  $feeds_tamper->disabled = FALSE; /* Edit this to true to make a default feeds_tamper disabled initially */
  $feeds_tamper->api_version = 2;
  $feeds_tamper->id = 'agris_apxml_Import-xpathparser_25-trimconference';
  $feeds_tamper->importer = 'agris_apxml_Import';
  $feeds_tamper->source = 'xpathparser:25';
  $feeds_tamper->plugin_id = 'trim';
  $feeds_tamper->settings = array(
    'mask' => '',
    'side' => 'trim',
  );
  $feeds_tamper->weight = 3;
  $feeds_tamper->description = 'Trimconference';
  $export['agris_apxml_Import-xpathparser_25-trimconference'] = $feeds_tamper;

  $feeds_tamper = new stdClass();
  $feeds_tamper->disabled = FALSE; /* Edit this to true to make a default feeds_tamper disabled initially */
  $feeds_tamper->api_version = 2;
  $feeds_tamper->id = 'agris_apxml_Import-xpathparser_25-truncate';
  $feeds_tamper->importer = 'agris_apxml_Import';
  $feeds_tamper->source = 'xpathparser:25';
  $feeds_tamper->plugin_id = 'truncate_text';
  $feeds_tamper->settings = array(
    'num_char' => '255',
    'ellipses' => 0,
  );
  $feeds_tamper->weight = 4;
  $feeds_tamper->description = 'Truncate';
  $export['agris_apxml_Import-xpathparser_25-truncate'] = $feeds_tamper;

  $feeds_tamper = new stdClass();
  $feeds_tamper->disabled = FALSE; /* Edit this to true to make a default feeds_tamper disabled initially */
  $feeds_tamper->api_version = 2;
  $feeds_tamper->id = 'agris_apxml_Import-xpathparser_26-trimbook';
  $feeds_tamper->importer = 'agris_apxml_Import';
  $feeds_tamper->source = 'xpathparser:26';
  $feeds_tamper->plugin_id = 'trim';
  $feeds_tamper->settings = array(
    'mask' => '',
    'side' => 'trim',
  );
  $feeds_tamper->weight = 1;
  $feeds_tamper->description = 'Trimbook';
  $export['agris_apxml_Import-xpathparser_26-trimbook'] = $feeds_tamper;

  $feeds_tamper = new stdClass();
  $feeds_tamper->disabled = FALSE; /* Edit this to true to make a default feeds_tamper disabled initially */
  $feeds_tamper->api_version = 2;
  $feeds_tamper->id = 'agris_apxml_Import-xpathparser_27-trimpub';
  $feeds_tamper->importer = 'agris_apxml_Import';
  $feeds_tamper->source = 'xpathparser:27';
  $feeds_tamper->plugin_id = 'trim';
  $feeds_tamper->settings = array(
    'mask' => '',
    'side' => 'trim',
  );
  $feeds_tamper->weight = 1;
  $feeds_tamper->description = 'Trimpub';
  $export['agris_apxml_Import-xpathparser_27-trimpub'] = $feeds_tamper;

  $feeds_tamper = new stdClass();
  $feeds_tamper->disabled = FALSE; /* Edit this to true to make a default feeds_tamper disabled initially */
  $feeds_tamper->api_version = 2;
  $feeds_tamper->id = 'agris_apxml_Import-xpathparser_27-truncate';
  $feeds_tamper->importer = 'agris_apxml_Import';
  $feeds_tamper->source = 'xpathparser:27';
  $feeds_tamper->plugin_id = 'truncate_text';
  $feeds_tamper->settings = array(
    'num_char' => '255',
    'ellipses' => 0,
  );
  $feeds_tamper->weight = 0;
  $feeds_tamper->description = 'Truncate';
  $export['agris_apxml_Import-xpathparser_27-truncate'] = $feeds_tamper;

  $feeds_tamper = new stdClass();
  $feeds_tamper->disabled = FALSE; /* Edit this to true to make a default feeds_tamper disabled initially */
  $feeds_tamper->api_version = 2;
  $feeds_tamper->id = 'agris_apxml_Import-xpathparser_28-YV_replace';
  $feeds_tamper->importer = 'agris_apxml_Import';
  $feeds_tamper->source = 'xpathparser:28';
  $feeds_tamper->plugin_id = 'find_replace';
  $feeds_tamper->settings = array(
    'find' => 'YV',
    'replace' => 'Journal Article',
    'case_sensitive' => 0,
    'word_boundaries' => 0,
    'whole' => 1,
    'regex' => TRUE,
    'regex_find' => '/^YV$/i',
  );
  $feeds_tamper->weight = 14;
  $feeds_tamper->description = 'Converts YV Agris code into Journal article';
  $export['agris_apxml_Import-xpathparser_28-YV_replace'] = $feeds_tamper;

  $feeds_tamper = new stdClass();
  $feeds_tamper->disabled = FALSE; /* Edit this to true to make a default feeds_tamper disabled initially */
  $feeds_tamper->api_version = 2;
  $feeds_tamper->id = 'agris_apxml_Import-xpathparser_28-execute_php_code';
  $feeds_tamper->importer = 'agris_apxml_Import';
  $feeds_tamper->source = 'xpathparser:28';
  $feeds_tamper->plugin_id = 'php';
  $feeds_tamper->settings = array(
    'php' => 'switch ($field) {
    case "K":
        return "Conference Proceedings";
        break;
    case "E":
        return "Journal Article";
        break;
    case "V":
          return "Journal Article";
        break;
   case "KV":
        return "Journal Article";
        break;
   case "YV":
        return "Journal Article";
        break;
   default:
        return "Other";
        break;
}

',
  );
  $feeds_tamper->weight = 9;
  $feeds_tamper->description = 'Converts not recognized Agris codes into Other';
  $export['agris_apxml_Import-xpathparser_28-execute_php_code'] = $feeds_tamper;

  $feeds_tamper = new stdClass();
  $feeds_tamper->disabled = FALSE; /* Edit this to true to make a default feeds_tamper disabled initially */
  $feeds_tamper->api_version = 2;
  $feeds_tamper->id = 'agris_apxml_Import-xpathparser_28-replace_type';
  $feeds_tamper->importer = 'agris_apxml_Import';
  $feeds_tamper->source = 'xpathparser:28';
  $feeds_tamper->plugin_id = 'find_replace';
  $feeds_tamper->settings = array(
    'find' => 'KV',
    'replace' => 'Journal Article',
    'case_sensitive' => 0,
    'word_boundaries' => 0,
    'whole' => 1,
    'regex' => TRUE,
    'regex_find' => '/^KV$/i',
  );
  $feeds_tamper->weight = 13;
  $feeds_tamper->description = 'Converts KV Agris code into Journal article';
  $export['agris_apxml_Import-xpathparser_28-replace_type'] = $feeds_tamper;

  $feeds_tamper = new stdClass();
  $feeds_tamper->disabled = FALSE; /* Edit this to true to make a default feeds_tamper disabled initially */
  $feeds_tamper->api_version = 2;
  $feeds_tamper->id = 'agris_apxml_Import-xpathparser_32-convert_case';
  $feeds_tamper->importer = 'agris_apxml_Import';
  $feeds_tamper->source = 'xpathparser:32';
  $feeds_tamper->plugin_id = 'convert_case';
  $feeds_tamper->settings = array(
    'mode' => '1',
  );
  $feeds_tamper->weight = 0;
  $feeds_tamper->description = 'Convert case';
  $export['agris_apxml_Import-xpathparser_32-convert_case'] = $feeds_tamper;

  $feeds_tamper = new stdClass();
  $feeds_tamper->disabled = FALSE; /* Edit this to true to make a default feeds_tamper disabled initially */
  $feeds_tamper->api_version = 2;
  $feeds_tamper->id = 'agris_apxml_Import-xpathparser_34-find_replace_regex';
  $feeds_tamper->importer = 'agris_apxml_Import';
  $feeds_tamper->source = 'xpathparser:34';
  $feeds_tamper->plugin_id = 'find_replace_regex';
  $feeds_tamper->settings = array(
    'find' => '/\\n/',
    'replace' => '',
    'limit' => '1',
    'real_limit' => '1',
  );
  $feeds_tamper->weight = 5;
  $feeds_tamper->description = 'Replace newline with space';
  $export['agris_apxml_Import-xpathparser_34-find_replace_regex'] = $feeds_tamper;

  $feeds_tamper = new stdClass();
  $feeds_tamper->disabled = FALSE; /* Edit this to true to make a default feeds_tamper disabled initially */
  $feeds_tamper->api_version = 2;
  $feeds_tamper->id = 'agris_apxml_Import-xpathparser_34-trimsource';
  $feeds_tamper->importer = 'agris_apxml_Import';
  $feeds_tamper->source = 'xpathparser:34';
  $feeds_tamper->plugin_id = 'trim';
  $feeds_tamper->settings = array(
    'mask' => '',
    'side' => 'trim',
  );
  $feeds_tamper->weight = 6;
  $feeds_tamper->description = 'Trimsource';
  $export['agris_apxml_Import-xpathparser_34-trimsource'] = $feeds_tamper;

  $feeds_tamper = new stdClass();
  $feeds_tamper->disabled = FALSE; /* Edit this to true to make a default feeds_tamper disabled initially */
  $feeds_tamper->api_version = 2;
  $feeds_tamper->id = 'agris_apxml_Import-xpathparser_34-truncate';
  $feeds_tamper->importer = 'agris_apxml_Import';
  $feeds_tamper->source = 'xpathparser:34';
  $feeds_tamper->plugin_id = 'truncate_text';
  $feeds_tamper->settings = array(
    'num_char' => '255',
    'ellipses' => 0,
  );
  $feeds_tamper->weight = 7;
  $feeds_tamper->description = 'Truncate';
  $export['agris_apxml_Import-xpathparser_34-truncate'] = $feeds_tamper;

  $feeds_tamper = new stdClass();
  $feeds_tamper->disabled = FALSE; /* Edit this to true to make a default feeds_tamper disabled initially */
  $feeds_tamper->api_version = 2;
  $feeds_tamper->id = 'agris_apxml_Import-xpathparser_35-set_default_value';
  $feeds_tamper->importer = 'agris_apxml_Import';
  $feeds_tamper->source = 'xpathparser:35';
  $feeds_tamper->plugin_id = 'default_value';
  $feeds_tamper->settings = array(
    'default_value' => '0',
  );
  $feeds_tamper->weight = 0;
  $feeds_tamper->description = 'Set default value';
  $export['agris_apxml_Import-xpathparser_35-set_default_value'] = $feeds_tamper;

  $feeds_tamper = new stdClass();
  $feeds_tamper->disabled = FALSE; /* Edit this to true to make a default feeds_tamper disabled initially */
  $feeds_tamper->api_version = 2;
  $feeds_tamper->id = 'agris_apxml_Import-xpathparser_36-convert_case';
  $feeds_tamper->importer = 'agris_apxml_Import';
  $feeds_tamper->source = 'xpathparser:36';
  $feeds_tamper->plugin_id = 'convert_case';
  $feeds_tamper->settings = array(
    'mode' => '1',
  );
  $feeds_tamper->weight = 0;
  $feeds_tamper->description = 'Convert case';
  $export['agris_apxml_Import-xpathparser_36-convert_case'] = $feeds_tamper;

  $feeds_tamper = new stdClass();
  $feeds_tamper->disabled = FALSE; /* Edit this to true to make a default feeds_tamper disabled initially */
  $feeds_tamper->api_version = 2;
  $feeds_tamper->id = 'agris_apxml_Import-xpathparser_38-execute_php_code';
  $feeds_tamper->importer = 'agris_apxml_Import';
  $feeds_tamper->source = 'xpathparser:38';
  $feeds_tamper->plugin_id = 'php';
  $feeds_tamper->settings = array(
    'php' => '$s= strpos($field,"(");
$e =strpos($field,")");
return sbstr($field,$s,$e);',
  );
  $feeds_tamper->weight = 0;
  $feeds_tamper->description = 'find number in field ags:citationNumber';
  $export['agris_apxml_Import-xpathparser_38-execute_php_code'] = $feeds_tamper;

  $feeds_tamper = new stdClass();
  $feeds_tamper->disabled = FALSE; /* Edit this to true to make a default feeds_tamper disabled initially */
  $feeds_tamper->api_version = 2;
  $feeds_tamper->id = 'agris_apxml_Import-xpathparser_39-find_replace_regex';
  $feeds_tamper->importer = 'agris_apxml_Import';
  $feeds_tamper->source = 'xpathparser:39';
  $feeds_tamper->plugin_id = 'find_replace_regex';
  $feeds_tamper->settings = array(
    'find' => '/\\n/',
    'replace' => '',
    'limit' => '1',
    'real_limit' => '1',
  );
  $feeds_tamper->weight = 0;
  $feeds_tamper->description = 'Replace newline with space';
  $export['agris_apxml_Import-xpathparser_39-find_replace_regex'] = $feeds_tamper;

  $feeds_tamper = new stdClass();
  $feeds_tamper->disabled = FALSE; /* Edit this to true to make a default feeds_tamper disabled initially */
  $feeds_tamper->api_version = 2;
  $feeds_tamper->id = 'agris_apxml_Import-xpathparser_39-truncate';
  $feeds_tamper->importer = 'agris_apxml_Import';
  $feeds_tamper->source = 'xpathparser:39';
  $feeds_tamper->plugin_id = 'truncate_text';
  $feeds_tamper->settings = array(
    'num_char' => '255',
    'ellipses' => 0,
  );
  $feeds_tamper->weight = 1;
  $feeds_tamper->description = 'Truncate to 255';
  $export['agris_apxml_Import-xpathparser_39-truncate'] = $feeds_tamper;

  $feeds_tamper = new stdClass();
  $feeds_tamper->disabled = FALSE; /* Edit this to true to make a default feeds_tamper disabled initially */
  $feeds_tamper->api_version = 2;
  $feeds_tamper->id = 'agris_apxml_Import-xpathparser_40-execute_php_code';
  $feeds_tamper->importer = 'agris_apxml_Import';
  $feeds_tamper->source = 'xpathparser:40';
  $feeds_tamper->plugin_id = 'php';
  $feeds_tamper->settings = array(
    'php' => '$s=strpos($field,"(");
return substr($field,1,$s);',
  );
  $feeds_tamper->weight = 0;
  $feeds_tamper->description = 'find volume in ags:citationNumber';
  $export['agris_apxml_Import-xpathparser_40-execute_php_code'] = $feeds_tamper;

  $feeds_tamper = new stdClass();
  $feeds_tamper->disabled = FALSE; /* Edit this to true to make a default feeds_tamper disabled initially */
  $feeds_tamper->api_version = 2;
  $feeds_tamper->id = 'agris_apxml_Import-xpathparser_46-find_replace_regex';
  $feeds_tamper->importer = 'agris_apxml_Import';
  $feeds_tamper->source = 'xpathparser:46';
  $feeds_tamper->plugin_id = 'find_replace_regex';
  $feeds_tamper->settings = array(
    'find' => '/\\n/',
    'replace' => '',
    'limit' => '1',
    'real_limit' => '1',
  );
  $feeds_tamper->weight = 0;
  $feeds_tamper->description = 'Replace newline with space';
  $export['agris_apxml_Import-xpathparser_46-find_replace_regex'] = $feeds_tamper;

  $feeds_tamper = new stdClass();
  $feeds_tamper->disabled = FALSE; /* Edit this to true to make a default feeds_tamper disabled initially */
  $feeds_tamper->api_version = 2;
  $feeds_tamper->id = 'agris_apxml_Import-xpathparser_46-trim';
  $feeds_tamper->importer = 'agris_apxml_Import';
  $feeds_tamper->source = 'xpathparser:46';
  $feeds_tamper->plugin_id = 'trim';
  $feeds_tamper->settings = array(
    'mask' => '',
    'side' => 'trim',
  );
  $feeds_tamper->weight = 2;
  $feeds_tamper->description = 'Trim';
  $export['agris_apxml_Import-xpathparser_46-trim'] = $feeds_tamper;

  $feeds_tamper = new stdClass();
  $feeds_tamper->disabled = FALSE; /* Edit this to true to make a default feeds_tamper disabled initially */
  $feeds_tamper->api_version = 2;
  $feeds_tamper->id = 'agris_apxml_Import-xpathparser_46-truncate';
  $feeds_tamper->importer = 'agris_apxml_Import';
  $feeds_tamper->source = 'xpathparser:46';
  $feeds_tamper->plugin_id = 'truncate_text';
  $feeds_tamper->settings = array(
    'num_char' => '255',
    'ellipses' => 0,
  );
  $feeds_tamper->weight = 1;
  $feeds_tamper->description = 'Truncate to 255';
  $export['agris_apxml_Import-xpathparser_46-truncate'] = $feeds_tamper;

  $feeds_tamper = new stdClass();
  $feeds_tamper->disabled = FALSE; /* Edit this to true to make a default feeds_tamper disabled initially */
  $feeds_tamper->api_version = 2;
  $feeds_tamper->id = 'agris_apxml_Import-xpathparser_47-find_replace_regex';
  $feeds_tamper->importer = 'agris_apxml_Import';
  $feeds_tamper->source = 'xpathparser:47';
  $feeds_tamper->plugin_id = 'find_replace_regex';
  $feeds_tamper->settings = array(
    'find' => '/\\n/',
    'replace' => '',
    'limit' => '1',
    'real_limit' => '1',
  );
  $feeds_tamper->weight = 0;
  $feeds_tamper->description = 'Replace newline with space';
  $export['agris_apxml_Import-xpathparser_47-find_replace_regex'] = $feeds_tamper;

  $feeds_tamper = new stdClass();
  $feeds_tamper->disabled = FALSE; /* Edit this to true to make a default feeds_tamper disabled initially */
  $feeds_tamper->api_version = 2;
  $feeds_tamper->id = 'agris_apxml_Import-xpathparser_47-trim';
  $feeds_tamper->importer = 'agris_apxml_Import';
  $feeds_tamper->source = 'xpathparser:47';
  $feeds_tamper->plugin_id = 'trim';
  $feeds_tamper->settings = array(
    'mask' => '',
    'side' => 'trim',
  );
  $feeds_tamper->weight = 2;
  $feeds_tamper->description = 'Trim';
  $export['agris_apxml_Import-xpathparser_47-trim'] = $feeds_tamper;

  $feeds_tamper = new stdClass();
  $feeds_tamper->disabled = FALSE; /* Edit this to true to make a default feeds_tamper disabled initially */
  $feeds_tamper->api_version = 2;
  $feeds_tamper->id = 'agris_apxml_Import-xpathparser_47-truncate';
  $feeds_tamper->importer = 'agris_apxml_Import';
  $feeds_tamper->source = 'xpathparser:47';
  $feeds_tamper->plugin_id = 'truncate_text';
  $feeds_tamper->settings = array(
    'num_char' => '255',
    'ellipses' => 0,
  );
  $feeds_tamper->weight = 1;
  $feeds_tamper->description = 'Truncate';
  $export['agris_apxml_Import-xpathparser_47-truncate'] = $feeds_tamper;

  $feeds_tamper = new stdClass();
  $feeds_tamper->disabled = FALSE; /* Edit this to true to make a default feeds_tamper disabled initially */
  $feeds_tamper->api_version = 2;
  $feeds_tamper->id = 'agris_apxml_Import-xpathparser_48-find_replace_regex';
  $feeds_tamper->importer = 'agris_apxml_Import';
  $feeds_tamper->source = 'xpathparser:48';
  $feeds_tamper->plugin_id = 'find_replace_regex';
  $feeds_tamper->settings = array(
    'find' => '/\\n/',
    'replace' => '',
    'limit' => '1',
    'real_limit' => '1',
  );
  $feeds_tamper->weight = 0;
  $feeds_tamper->description = 'Replace newline with space';
  $export['agris_apxml_Import-xpathparser_48-find_replace_regex'] = $feeds_tamper;

  $feeds_tamper = new stdClass();
  $feeds_tamper->disabled = FALSE; /* Edit this to true to make a default feeds_tamper disabled initially */
  $feeds_tamper->api_version = 2;
  $feeds_tamper->id = 'agris_apxml_Import-xpathparser_48-trim';
  $feeds_tamper->importer = 'agris_apxml_Import';
  $feeds_tamper->source = 'xpathparser:48';
  $feeds_tamper->plugin_id = 'trim';
  $feeds_tamper->settings = array(
    'mask' => '',
    'side' => 'trim',
  );
  $feeds_tamper->weight = 2;
  $feeds_tamper->description = 'Trim';
  $export['agris_apxml_Import-xpathparser_48-trim'] = $feeds_tamper;

  $feeds_tamper = new stdClass();
  $feeds_tamper->disabled = FALSE; /* Edit this to true to make a default feeds_tamper disabled initially */
  $feeds_tamper->api_version = 2;
  $feeds_tamper->id = 'agris_apxml_Import-xpathparser_48-truncate';
  $feeds_tamper->importer = 'agris_apxml_Import';
  $feeds_tamper->source = 'xpathparser:48';
  $feeds_tamper->plugin_id = 'truncate_text';
  $feeds_tamper->settings = array(
    'num_char' => '255',
    'ellipses' => 0,
  );
  $feeds_tamper->weight = 1;
  $feeds_tamper->description = 'Truncate';
  $export['agris_apxml_Import-xpathparser_48-truncate'] = $feeds_tamper;

  $feeds_tamper = new stdClass();
  $feeds_tamper->disabled = FALSE; /* Edit this to true to make a default feeds_tamper disabled initially */
  $feeds_tamper->api_version = 2;
  $feeds_tamper->id = 'agris_apxml_Import-xpathparser_49-find_replace_regex';
  $feeds_tamper->importer = 'agris_apxml_Import';
  $feeds_tamper->source = 'xpathparser:49';
  $feeds_tamper->plugin_id = 'find_replace_regex';
  $feeds_tamper->settings = array(
    'find' => '/\\n/',
    'replace' => '',
    'limit' => '1',
    'real_limit' => '1',
  );
  $feeds_tamper->weight = 0;
  $feeds_tamper->description = 'Replace newline with space';
  $export['agris_apxml_Import-xpathparser_49-find_replace_regex'] = $feeds_tamper;

  $feeds_tamper = new stdClass();
  $feeds_tamper->disabled = FALSE; /* Edit this to true to make a default feeds_tamper disabled initially */
  $feeds_tamper->api_version = 2;
  $feeds_tamper->id = 'agris_apxml_Import-xpathparser_49-trim';
  $feeds_tamper->importer = 'agris_apxml_Import';
  $feeds_tamper->source = 'xpathparser:49';
  $feeds_tamper->plugin_id = 'trim';
  $feeds_tamper->settings = array(
    'mask' => '',
    'side' => 'trim',
  );
  $feeds_tamper->weight = 2;
  $feeds_tamper->description = 'Trim';
  $export['agris_apxml_Import-xpathparser_49-trim'] = $feeds_tamper;

  $feeds_tamper = new stdClass();
  $feeds_tamper->disabled = FALSE; /* Edit this to true to make a default feeds_tamper disabled initially */
  $feeds_tamper->api_version = 2;
  $feeds_tamper->id = 'agris_apxml_Import-xpathparser_49-truncate';
  $feeds_tamper->importer = 'agris_apxml_Import';
  $feeds_tamper->source = 'xpathparser:49';
  $feeds_tamper->plugin_id = 'truncate_text';
  $feeds_tamper->settings = array(
    'num_char' => '255',
    'ellipses' => 0,
  );
  $feeds_tamper->weight = 1;
  $feeds_tamper->description = 'Truncate';
  $export['agris_apxml_Import-xpathparser_49-truncate'] = $feeds_tamper;

  $feeds_tamper = new stdClass();
  $feeds_tamper->disabled = FALSE; /* Edit this to true to make a default feeds_tamper disabled initially */
  $feeds_tamper->api_version = 2;
  $feeds_tamper->id = 'agris_apxml_Import-xpathparser_50-find_replace_regex';
  $feeds_tamper->importer = 'agris_apxml_Import';
  $feeds_tamper->source = 'xpathparser:50';
  $feeds_tamper->plugin_id = 'find_replace_regex';
  $feeds_tamper->settings = array(
    'find' => '/\\n/',
    'replace' => '',
    'limit' => '1',
    'real_limit' => '1',
  );
  $feeds_tamper->weight = 0;
  $feeds_tamper->description = 'Replace newline with space';
  $export['agris_apxml_Import-xpathparser_50-find_replace_regex'] = $feeds_tamper;

  $feeds_tamper = new stdClass();
  $feeds_tamper->disabled = FALSE; /* Edit this to true to make a default feeds_tamper disabled initially */
  $feeds_tamper->api_version = 2;
  $feeds_tamper->id = 'agris_apxml_Import-xpathparser_50-trim';
  $feeds_tamper->importer = 'agris_apxml_Import';
  $feeds_tamper->source = 'xpathparser:50';
  $feeds_tamper->plugin_id = 'trim';
  $feeds_tamper->settings = array(
    'mask' => '',
    'side' => 'trim',
  );
  $feeds_tamper->weight = 2;
  $feeds_tamper->description = 'Trim';
  $export['agris_apxml_Import-xpathparser_50-trim'] = $feeds_tamper;

  $feeds_tamper = new stdClass();
  $feeds_tamper->disabled = FALSE; /* Edit this to true to make a default feeds_tamper disabled initially */
  $feeds_tamper->api_version = 2;
  $feeds_tamper->id = 'agris_apxml_Import-xpathparser_50-truncate';
  $feeds_tamper->importer = 'agris_apxml_Import';
  $feeds_tamper->source = 'xpathparser:50';
  $feeds_tamper->plugin_id = 'truncate_text';
  $feeds_tamper->settings = array(
    'num_char' => '255',
    'ellipses' => 0,
  );
  $feeds_tamper->weight = 1;
  $feeds_tamper->description = 'Truncate';
  $export['agris_apxml_Import-xpathparser_50-truncate'] = $feeds_tamper;

  $feeds_tamper = new stdClass();
  $feeds_tamper->disabled = FALSE; /* Edit this to true to make a default feeds_tamper disabled initially */
  $feeds_tamper->api_version = 2;
  $feeds_tamper->id = 'agris_apxml_Import-xpathparser_51-find_replace_regex';
  $feeds_tamper->importer = 'agris_apxml_Import';
  $feeds_tamper->source = 'xpathparser:51';
  $feeds_tamper->plugin_id = 'find_replace_regex';
  $feeds_tamper->settings = array(
    'find' => '/^(19|20)(\\d{2})$/',
    'replace' => '\\1\\2-01',
    'limit' => '1',
    'real_limit' => '1',
  );
  $feeds_tamper->weight = 0;
  $feeds_tamper->description = 'Find replace add month';
  $export['agris_apxml_Import-xpathparser_51-find_replace_regex'] = $feeds_tamper;

  $feeds_tamper = new stdClass();
  $feeds_tamper->disabled = FALSE; /* Edit this to true to make a default feeds_tamper disabled initially */
  $feeds_tamper->api_version = 2;
  $feeds_tamper->id = 'agris_apxml_Import-xpathparser_52-convert_case';
  $feeds_tamper->importer = 'agris_apxml_Import';
  $feeds_tamper->source = 'xpathparser:52';
  $feeds_tamper->plugin_id = 'convert_case';
  $feeds_tamper->settings = array(
    'mode' => '1',
  );
  $feeds_tamper->weight = 0;
  $feeds_tamper->description = 'Convert case';
  $export['agris_apxml_Import-xpathparser_52-convert_case'] = $feeds_tamper;

  $feeds_tamper = new stdClass();
  $feeds_tamper->disabled = FALSE; /* Edit this to true to make a default feeds_tamper disabled initially */
  $feeds_tamper->api_version = 2;
  $feeds_tamper->id = 'agris_apxml_Import-xpathparser_53-convert_case';
  $feeds_tamper->importer = 'agris_apxml_Import';
  $feeds_tamper->source = 'xpathparser:53';
  $feeds_tamper->plugin_id = 'convert_case';
  $feeds_tamper->settings = array(
    'mode' => '1',
  );
  $feeds_tamper->weight = 0;
  $feeds_tamper->description = 'Convert lower case';
  $export['agris_apxml_Import-xpathparser_53-convert_case'] = $feeds_tamper;

  $feeds_tamper = new stdClass();
  $feeds_tamper->disabled = FALSE; /* Edit this to true to make a default feeds_tamper disabled initially */
  $feeds_tamper->api_version = 2;
  $feeds_tamper->id = 'agris_apxml_Import-xpathparser_54-convert_case';
  $feeds_tamper->importer = 'agris_apxml_Import';
  $feeds_tamper->source = 'xpathparser:54';
  $feeds_tamper->plugin_id = 'convert_case';
  $feeds_tamper->settings = array(
    'mode' => '1',
  );
  $feeds_tamper->weight = 0;
  $feeds_tamper->description = 'Convert case';
  $export['agris_apxml_Import-xpathparser_54-convert_case'] = $feeds_tamper;

  $feeds_tamper = new stdClass();
  $feeds_tamper->disabled = FALSE; /* Edit this to true to make a default feeds_tamper disabled initially */
  $feeds_tamper->api_version = 2;
  $feeds_tamper->id = 'agris_apxml_Import-xpathparser_55-convert_case';
  $feeds_tamper->importer = 'agris_apxml_Import';
  $feeds_tamper->source = 'xpathparser:55';
  $feeds_tamper->plugin_id = 'convert_case';
  $feeds_tamper->settings = array(
    'mode' => '1',
  );
  $feeds_tamper->weight = 0;
  $feeds_tamper->description = 'Convert case';
  $export['agris_apxml_Import-xpathparser_55-convert_case'] = $feeds_tamper;

  $feeds_tamper = new stdClass();
  $feeds_tamper->disabled = FALSE; /* Edit this to true to make a default feeds_tamper disabled initially */
  $feeds_tamper->api_version = 2;
  $feeds_tamper->id = 'agris_apxml_Import-xpathparser_56-find_replace_regex';
  $feeds_tamper->importer = 'agris_apxml_Import';
  $feeds_tamper->source = 'xpathparser:56';
  $feeds_tamper->plugin_id = 'find_replace_regex';
  $feeds_tamper->settings = array(
    'find' => '/^(19|20)(\\d{2})$/',
    'replace' => '\\1\\2-01',
    'limit' => '1',
    'real_limit' => '1',
  );
  $feeds_tamper->weight = 0;
  $feeds_tamper->description = 'Find replace REGEX';
  $export['agris_apxml_Import-xpathparser_56-find_replace_regex'] = $feeds_tamper;

  $feeds_tamper = new stdClass();
  $feeds_tamper->disabled = FALSE; /* Edit this to true to make a default feeds_tamper disabled initially */
  $feeds_tamper->api_version = 2;
  $feeds_tamper->id = 'agris_apxml_Import-xpathparser_57-find_replace_regex';
  $feeds_tamper->importer = 'agris_apxml_Import';
  $feeds_tamper->source = 'xpathparser:57';
  $feeds_tamper->plugin_id = 'find_replace_regex';
  $feeds_tamper->settings = array(
    'find' => '/\\n/',
    'replace' => '',
    'limit' => '1',
    'real_limit' => '1',
  );
  $feeds_tamper->weight = 0;
  $feeds_tamper->description = 'replace newline with space';
  $export['agris_apxml_Import-xpathparser_57-find_replace_regex'] = $feeds_tamper;

  $feeds_tamper = new stdClass();
  $feeds_tamper->disabled = FALSE; /* Edit this to true to make a default feeds_tamper disabled initially */
  $feeds_tamper->api_version = 2;
  $feeds_tamper->id = 'agris_apxml_Import-xpathparser_57-trim';
  $feeds_tamper->importer = 'agris_apxml_Import';
  $feeds_tamper->source = 'xpathparser:57';
  $feeds_tamper->plugin_id = 'trim';
  $feeds_tamper->settings = array(
    'mask' => '',
    'side' => 'trim',
  );
  $feeds_tamper->weight = 1;
  $feeds_tamper->description = 'Trim';
  $export['agris_apxml_Import-xpathparser_57-trim'] = $feeds_tamper;

  $feeds_tamper = new stdClass();
  $feeds_tamper->disabled = FALSE; /* Edit this to true to make a default feeds_tamper disabled initially */
  $feeds_tamper->api_version = 2;
  $feeds_tamper->id = 'agris_apxml_Import-xpathparser_57-truncate';
  $feeds_tamper->importer = 'agris_apxml_Import';
  $feeds_tamper->source = 'xpathparser:57';
  $feeds_tamper->plugin_id = 'truncate_text';
  $feeds_tamper->settings = array(
    'num_char' => '255',
    'ellipses' => 0,
  );
  $feeds_tamper->weight = 2;
  $feeds_tamper->description = 'Truncate';
  $export['agris_apxml_Import-xpathparser_57-truncate'] = $feeds_tamper;

  $feeds_tamper = new stdClass();
  $feeds_tamper->disabled = FALSE; /* Edit this to true to make a default feeds_tamper disabled initially */
  $feeds_tamper->api_version = 2;
  $feeds_tamper->id = 'agris_apxml_Import-xpathparser_58-execute_php_code';
  $feeds_tamper->importer = 'agris_apxml_Import';
  $feeds_tamper->source = 'xpathparser:58';
  $feeds_tamper->plugin_id = 'php';
  $feeds_tamper->settings = array(
    'php' => '$pos=strpos($field,"v");
$posT=strpos($field,"(");
$r=substr($field,$pos+2,($posT-1)-($pos+1));
return $r;',
  );
  $feeds_tamper->weight = 0;
  $feeds_tamper->description = 'extract_volume';
  $export['agris_apxml_Import-xpathparser_58-execute_php_code'] = $feeds_tamper;

  $feeds_tamper = new stdClass();
  $feeds_tamper->disabled = FALSE; /* Edit this to true to make a default feeds_tamper disabled initially */
  $feeds_tamper->api_version = 2;
  $feeds_tamper->id = 'agris_apxml_Import-xpathparser_59-execute_php_code';
  $feeds_tamper->importer = 'agris_apxml_Import';
  $feeds_tamper->source = 'xpathparser:59';
  $feeds_tamper->plugin_id = 'php';
  $feeds_tamper->settings = array(
    'php' => '$pos=strpos($field,"(");
$posT=strpos($field,")");
$r=substr($field,$pos+1,($posT-1)- $pos);
return $r;',
  );
  $feeds_tamper->weight = 0;
  $feeds_tamper->description = 'extract_number';
  $export['agris_apxml_Import-xpathparser_59-execute_php_code'] = $feeds_tamper;

  $feeds_tamper = new stdClass();
  $feeds_tamper->disabled = FALSE; /* Edit this to true to make a default feeds_tamper disabled initially */
  $feeds_tamper->api_version = 2;
  $feeds_tamper->id = 'agris_apxml_Import-xpathparser_66-find_replace_regex';
  $feeds_tamper->importer = 'agris_apxml_Import';
  $feeds_tamper->source = 'xpathparser:66';
  $feeds_tamper->plugin_id = 'find_replace_regex';
  $feeds_tamper->settings = array(
    'find' => '/\\n/',
    'replace' => '',
    'limit' => '1',
    'real_limit' => '1',
  );
  $feeds_tamper->weight = 2;
  $feeds_tamper->description = 'Replace newline with space';
  $export['agris_apxml_Import-xpathparser_66-find_replace_regex'] = $feeds_tamper;

  $feeds_tamper = new stdClass();
  $feeds_tamper->disabled = FALSE; /* Edit this to true to make a default feeds_tamper disabled initially */
  $feeds_tamper->api_version = 2;
  $feeds_tamper->id = 'agris_apxml_Import-xpathparser_66-trim';
  $feeds_tamper->importer = 'agris_apxml_Import';
  $feeds_tamper->source = 'xpathparser:66';
  $feeds_tamper->plugin_id = 'trim';
  $feeds_tamper->settings = array(
    'mask' => '',
    'side' => 'trim',
  );
  $feeds_tamper->weight = 3;
  $feeds_tamper->description = 'Trim';
  $export['agris_apxml_Import-xpathparser_66-trim'] = $feeds_tamper;

  $feeds_tamper = new stdClass();
  $feeds_tamper->disabled = FALSE; /* Edit this to true to make a default feeds_tamper disabled initially */
  $feeds_tamper->api_version = 2;
  $feeds_tamper->id = 'agris_apxml_Import-xpathparser_66-truncate';
  $feeds_tamper->importer = 'agris_apxml_Import';
  $feeds_tamper->source = 'xpathparser:66';
  $feeds_tamper->plugin_id = 'truncate_text';
  $feeds_tamper->settings = array(
    'num_char' => '255',
    'ellipses' => 0,
  );
  $feeds_tamper->weight = 4;
  $feeds_tamper->description = 'Truncate';
  $export['agris_apxml_Import-xpathparser_66-truncate'] = $feeds_tamper;

  $feeds_tamper = new stdClass();
  $feeds_tamper->disabled = FALSE; /* Edit this to true to make a default feeds_tamper disabled initially */
  $feeds_tamper->api_version = 2;
  $feeds_tamper->id = 'agris_apxml_Import-xpathparser_67-find_replace_regex';
  $feeds_tamper->importer = 'agris_apxml_Import';
  $feeds_tamper->source = 'xpathparser:67';
  $feeds_tamper->plugin_id = 'find_replace_regex';
  $feeds_tamper->settings = array(
    'find' => '/\\n/',
    'replace' => '',
    'limit' => '1',
    'real_limit' => '1',
  );
  $feeds_tamper->weight = 2;
  $feeds_tamper->description = 'Replace newline with space';
  $export['agris_apxml_Import-xpathparser_67-find_replace_regex'] = $feeds_tamper;

  $feeds_tamper = new stdClass();
  $feeds_tamper->disabled = FALSE; /* Edit this to true to make a default feeds_tamper disabled initially */
  $feeds_tamper->api_version = 2;
  $feeds_tamper->id = 'agris_apxml_Import-xpathparser_67-trim';
  $feeds_tamper->importer = 'agris_apxml_Import';
  $feeds_tamper->source = 'xpathparser:67';
  $feeds_tamper->plugin_id = 'trim';
  $feeds_tamper->settings = array(
    'mask' => '',
    'side' => 'trim',
  );
  $feeds_tamper->weight = 3;
  $feeds_tamper->description = 'Trim';
  $export['agris_apxml_Import-xpathparser_67-trim'] = $feeds_tamper;

  $feeds_tamper = new stdClass();
  $feeds_tamper->disabled = FALSE; /* Edit this to true to make a default feeds_tamper disabled initially */
  $feeds_tamper->api_version = 2;
  $feeds_tamper->id = 'agris_apxml_Import-xpathparser_67-truncate';
  $feeds_tamper->importer = 'agris_apxml_Import';
  $feeds_tamper->source = 'xpathparser:67';
  $feeds_tamper->plugin_id = 'truncate_text';
  $feeds_tamper->settings = array(
    'num_char' => '255',
    'ellipses' => 0,
  );
  $feeds_tamper->weight = 4;
  $feeds_tamper->description = 'Truncate';
  $export['agris_apxml_Import-xpathparser_67-truncate'] = $feeds_tamper;

  $feeds_tamper = new stdClass();
  $feeds_tamper->disabled = FALSE; /* Edit this to true to make a default feeds_tamper disabled initially */
  $feeds_tamper->api_version = 2;
  $feeds_tamper->id = 'agris_apxml_Import-xpathparser_69-find_replace_regex';
  $feeds_tamper->importer = 'agris_apxml_Import';
  $feeds_tamper->source = 'xpathparser:69';
  $feeds_tamper->plugin_id = 'find_replace_regex';
  $feeds_tamper->settings = array(
    'find' => '/\\n/',
    'replace' => '',
    'limit' => '1',
    'real_limit' => '1',
  );
  $feeds_tamper->weight = 2;
  $feeds_tamper->description = 'Replace newline with space';
  $export['agris_apxml_Import-xpathparser_69-find_replace_regex'] = $feeds_tamper;

  $feeds_tamper = new stdClass();
  $feeds_tamper->disabled = FALSE; /* Edit this to true to make a default feeds_tamper disabled initially */
  $feeds_tamper->api_version = 2;
  $feeds_tamper->id = 'agris_apxml_Import-xpathparser_69-trim';
  $feeds_tamper->importer = 'agris_apxml_Import';
  $feeds_tamper->source = 'xpathparser:69';
  $feeds_tamper->plugin_id = 'trim';
  $feeds_tamper->settings = array(
    'mask' => '',
    'side' => 'trim',
  );
  $feeds_tamper->weight = 3;
  $feeds_tamper->description = 'Trim';
  $export['agris_apxml_Import-xpathparser_69-trim'] = $feeds_tamper;

  $feeds_tamper = new stdClass();
  $feeds_tamper->disabled = FALSE; /* Edit this to true to make a default feeds_tamper disabled initially */
  $feeds_tamper->api_version = 2;
  $feeds_tamper->id = 'agris_apxml_Import-xpathparser_69-truncate';
  $feeds_tamper->importer = 'agris_apxml_Import';
  $feeds_tamper->source = 'xpathparser:69';
  $feeds_tamper->plugin_id = 'truncate_text';
  $feeds_tamper->settings = array(
    'num_char' => '255',
    'ellipses' => 0,
  );
  $feeds_tamper->weight = 4;
  $feeds_tamper->description = 'Truncate';
  $export['agris_apxml_Import-xpathparser_69-truncate'] = $feeds_tamper;

  $feeds_tamper = new stdClass();
  $feeds_tamper->disabled = FALSE; /* Edit this to true to make a default feeds_tamper disabled initially */
  $feeds_tamper->api_version = 2;
  $feeds_tamper->id = 'agris_apxml_Import-xpathparser_70-find_replace_regex';
  $feeds_tamper->importer = 'agris_apxml_Import';
  $feeds_tamper->source = 'xpathparser:70';
  $feeds_tamper->plugin_id = 'find_replace_regex';
  $feeds_tamper->settings = array(
    'find' => '/\\n/',
    'replace' => '',
    'limit' => '1',
    'real_limit' => '1',
  );
  $feeds_tamper->weight = 2;
  $feeds_tamper->description = 'Replace newline with space';
  $export['agris_apxml_Import-xpathparser_70-find_replace_regex'] = $feeds_tamper;

  $feeds_tamper = new stdClass();
  $feeds_tamper->disabled = FALSE; /* Edit this to true to make a default feeds_tamper disabled initially */
  $feeds_tamper->api_version = 2;
  $feeds_tamper->id = 'agris_apxml_Import-xpathparser_70-trim';
  $feeds_tamper->importer = 'agris_apxml_Import';
  $feeds_tamper->source = 'xpathparser:70';
  $feeds_tamper->plugin_id = 'trim';
  $feeds_tamper->settings = array(
    'mask' => '',
    'side' => 'trim',
  );
  $feeds_tamper->weight = 3;
  $feeds_tamper->description = 'Trim';
  $export['agris_apxml_Import-xpathparser_70-trim'] = $feeds_tamper;

  $feeds_tamper = new stdClass();
  $feeds_tamper->disabled = FALSE; /* Edit this to true to make a default feeds_tamper disabled initially */
  $feeds_tamper->api_version = 2;
  $feeds_tamper->id = 'agris_apxml_Import-xpathparser_70-truncate';
  $feeds_tamper->importer = 'agris_apxml_Import';
  $feeds_tamper->source = 'xpathparser:70';
  $feeds_tamper->plugin_id = 'truncate_text';
  $feeds_tamper->settings = array(
    'num_char' => '255',
    'ellipses' => 0,
  );
  $feeds_tamper->weight = 4;
  $feeds_tamper->description = 'Truncate';
  $export['agris_apxml_Import-xpathparser_70-truncate'] = $feeds_tamper;

  $feeds_tamper = new stdClass();
  $feeds_tamper->disabled = FALSE; /* Edit this to true to make a default feeds_tamper disabled initially */
  $feeds_tamper->api_version = 2;
  $feeds_tamper->id = 'agris_apxml_Import-xpathparser_71-find_replace_regex';
  $feeds_tamper->importer = 'agris_apxml_Import';
  $feeds_tamper->source = 'xpathparser:71';
  $feeds_tamper->plugin_id = 'find_replace_regex';
  $feeds_tamper->settings = array(
    'find' => '/\\n/',
    'replace' => '',
    'limit' => '1',
    'real_limit' => '1',
  );
  $feeds_tamper->weight = 2;
  $feeds_tamper->description = 'Replace newline with space';
  $export['agris_apxml_Import-xpathparser_71-find_replace_regex'] = $feeds_tamper;

  $feeds_tamper = new stdClass();
  $feeds_tamper->disabled = FALSE; /* Edit this to true to make a default feeds_tamper disabled initially */
  $feeds_tamper->api_version = 2;
  $feeds_tamper->id = 'agris_apxml_Import-xpathparser_71-trim';
  $feeds_tamper->importer = 'agris_apxml_Import';
  $feeds_tamper->source = 'xpathparser:71';
  $feeds_tamper->plugin_id = 'trim';
  $feeds_tamper->settings = array(
    'mask' => '',
    'side' => 'trim',
  );
  $feeds_tamper->weight = 3;
  $feeds_tamper->description = 'Trim';
  $export['agris_apxml_Import-xpathparser_71-trim'] = $feeds_tamper;

  $feeds_tamper = new stdClass();
  $feeds_tamper->disabled = FALSE; /* Edit this to true to make a default feeds_tamper disabled initially */
  $feeds_tamper->api_version = 2;
  $feeds_tamper->id = 'agris_apxml_Import-xpathparser_71-truncate';
  $feeds_tamper->importer = 'agris_apxml_Import';
  $feeds_tamper->source = 'xpathparser:71';
  $feeds_tamper->plugin_id = 'truncate_text';
  $feeds_tamper->settings = array(
    'num_char' => '255',
    'ellipses' => 0,
  );
  $feeds_tamper->weight = 4;
  $feeds_tamper->description = 'Truncate';
  $export['agris_apxml_Import-xpathparser_71-truncate'] = $feeds_tamper;

  $feeds_tamper = new stdClass();
  $feeds_tamper->disabled = FALSE; /* Edit this to true to make a default feeds_tamper disabled initially */
  $feeds_tamper->api_version = 2;
  $feeds_tamper->id = 'agris_apxml_Import-xpathparser_73-execute_php_code';
  $feeds_tamper->importer = 'agris_apxml_Import';
  $feeds_tamper->source = 'xpathparser:73';
  $feeds_tamper->plugin_id = 'php';
  $feeds_tamper->settings = array(
    'php' => '$pos=strpos($field,"p");
$r=trim(substr($field,$pos+2,strlen($field)-$pos+1));
return $r;',
  );
  $feeds_tamper->weight = 0;
  $feeds_tamper->description = 'extract_pages';
  $export['agris_apxml_Import-xpathparser_73-execute_php_code'] = $feeds_tamper;

  $feeds_tamper = new stdClass();
  $feeds_tamper->disabled = FALSE; /* Edit this to true to make a default feeds_tamper disabled initially */
  $feeds_tamper->api_version = 2;
  $feeds_tamper->id = 'agris_apxml_Import-xpathparser_9-find_replace_regex';
  $feeds_tamper->importer = 'agris_apxml_Import';
  $feeds_tamper->source = 'xpathparser:9';
  $feeds_tamper->plugin_id = 'find_replace_regex';
  $feeds_tamper->settings = array(
    'find' => '/<^$>/',
    'replace' => 'en',
    'limit' => '1',
    'real_limit' => '1',
  );
  $feeds_tamper->weight = 3;
  $feeds_tamper->description = 'Find replace REGEX';
  $export['agris_apxml_Import-xpathparser_9-find_replace_regex'] = $feeds_tamper;

  $feeds_tamper = new stdClass();
  $feeds_tamper->disabled = FALSE; /* Edit this to true to make a default feeds_tamper disabled initially */
  $feeds_tamper->api_version = 2;
  $feeds_tamper->id = 'agris_apxml_Import_xpathparser_11_convert_to_lowercase';
  $feeds_tamper->importer = 'agris_apxml_Import';
  $feeds_tamper->source = 'xpathparser:11';
  $feeds_tamper->plugin_id = 'convert_case';
  $feeds_tamper->settings = array(
    'mode' => '1',
  );
  $feeds_tamper->weight = 2;
  $feeds_tamper->description = 'Convert text to lowercase.';
  $export['agris_apxml_Import_xpathparser_11_convert_to_lowercase'] = $feeds_tamper;

  $feeds_tamper = new stdClass();
  $feeds_tamper->disabled = FALSE; /* Edit this to true to make a default feeds_tamper disabled initially */
  $feeds_tamper->api_version = 2;
  $feeds_tamper->id = 'agris_apxml_Import_xpathparser_26_truncate_text_to_255_characters';
  $feeds_tamper->importer = 'agris_apxml_Import';
  $feeds_tamper->source = 'xpathparser:26';
  $feeds_tamper->plugin_id = 'truncate_text';
  $feeds_tamper->settings = array(
    'num_char' => '255',
    'ellipses' => 0,
  );
  $feeds_tamper->weight = 0;
  $feeds_tamper->description = 'Truncate to 255 characters.';
  $export['agris_apxml_Import_xpathparser_26_truncate_text_to_255_characters'] = $feeds_tamper;

  $feeds_tamper = new stdClass();
  $feeds_tamper->disabled = FALSE; /* Edit this to true to make a default feeds_tamper disabled initially */
  $feeds_tamper->api_version = 2;
  $feeds_tamper->id = 'agris_apxml_Import_xpathparser_34_make_unique';
  $feeds_tamper->importer = 'agris_apxml_Import';
  $feeds_tamper->source = 'xpathparser:34';
  $feeds_tamper->plugin_id = 'unique';
  $feeds_tamper->settings = FALSE;
  $feeds_tamper->weight = 8;
  $feeds_tamper->description = 'Make the values unique.';
  $export['agris_apxml_Import_xpathparser_34_make_unique'] = $feeds_tamper;

  $feeds_tamper = new stdClass();
  $feeds_tamper->disabled = FALSE; /* Edit this to true to make a default feeds_tamper disabled initially */
  $feeds_tamper->api_version = 2;
  $feeds_tamper->id = 'agris_apxml_Import_xpathparser_3_convert_to_lowercase';
  $feeds_tamper->importer = 'agris_apxml_Import';
  $feeds_tamper->source = 'xpathparser:3';
  $feeds_tamper->plugin_id = 'convert_case';
  $feeds_tamper->settings = array(
    'mode' => '1',
  );
  $feeds_tamper->weight = 0;
  $feeds_tamper->description = 'Convert text to lowercase.';
  $export['agris_apxml_Import_xpathparser_3_convert_to_lowercase'] = $feeds_tamper;

  $feeds_tamper = new stdClass();
  $feeds_tamper->disabled = FALSE; /* Edit this to true to make a default feeds_tamper disabled initially */
  $feeds_tamper->api_version = 2;
  $feeds_tamper->id = 'agris_apxml_Import_xpathparser_8_convert_to_lowercase';
  $feeds_tamper->importer = 'agris_apxml_Import';
  $feeds_tamper->source = 'xpathparser:8';
  $feeds_tamper->plugin_id = 'convert_case';
  $feeds_tamper->settings = array(
    'mode' => '1',
  );
  $feeds_tamper->weight = 0;
  $feeds_tamper->description = 'Convert text to lowercase.';
  $export['agris_apxml_Import_xpathparser_8_convert_to_lowercase'] = $feeds_tamper;

  $feeds_tamper = new stdClass();
  $feeds_tamper->disabled = FALSE; /* Edit this to true to make a default feeds_tamper disabled initially */
  $feeds_tamper->api_version = 2;
  $feeds_tamper->id = 'agris_apxml_Import_xpathparser_9_convert_to_lowercase';
  $feeds_tamper->importer = 'agris_apxml_Import';
  $feeds_tamper->source = 'xpathparser:9';
  $feeds_tamper->plugin_id = 'convert_case';
  $feeds_tamper->settings = array(
    'mode' => '1',
  );
  $feeds_tamper->weight = 2;
  $feeds_tamper->description = 'Convert text to lowercase.';
  $export['agris_apxml_Import_xpathparser_9_convert_to_lowercase'] = $feeds_tamper;

  $feeds_tamper = new stdClass();
  $feeds_tamper->disabled = FALSE; /* Edit this to true to make a default feeds_tamper disabled initially */
  $feeds_tamper->api_version = 2;
  $feeds_tamper->id = 'books_feed_xpathparser_1_truncate_text_to_255_characters';
  $feeds_tamper->importer = 'books_feed';
  $feeds_tamper->source = 'xpathparser:1';
  $feeds_tamper->plugin_id = 'truncate_text';
  $feeds_tamper->settings = array(
    'num_char' => '255',
    'ellipses' => 0,
  );
  $feeds_tamper->weight = 0;
  $feeds_tamper->description = 'Truncate to 255 characters.';
  $export['books_feed_xpathparser_1_truncate_text_to_255_characters'] = $feeds_tamper;

  $feeds_tamper = new stdClass();
  $feeds_tamper->disabled = FALSE; /* Edit this to true to make a default feeds_tamper disabled initially */
  $feeds_tamper->api_version = 2;
  $feeds_tamper->id = 'books_feed_xpathparser_2_truncate_text_to_255_characters';
  $feeds_tamper->importer = 'books_feed';
  $feeds_tamper->source = 'xpathparser:2';
  $feeds_tamper->plugin_id = 'truncate_text';
  $feeds_tamper->settings = array(
    'num_char' => '255',
    'ellipses' => 0,
  );
  $feeds_tamper->weight = 0;
  $feeds_tamper->description = 'Truncate to 255 characters.';
  $export['books_feed_xpathparser_2_truncate_text_to_255_characters'] = $feeds_tamper;

  $feeds_tamper = new stdClass();
  $feeds_tamper->disabled = FALSE; /* Edit this to true to make a default feeds_tamper disabled initially */
  $feeds_tamper->api_version = 2;
  $feeds_tamper->id = 'convert_type';
  $feeds_tamper->importer = 'agris_apxml_Import';
  $feeds_tamper->source = 'xpathparser:28';
  $feeds_tamper->plugin_id = 'find_replace';
  $feeds_tamper->settings = array(
    'find' => 'K',
    'replace' => 'Conference Proceedings',
    'case_sensitive' => 0,
    'word_boundaries' => 0,
    'whole' => 0,
    'regex' => FALSE,
    'func' => 'str_ireplace',
  );
  $feeds_tamper->weight = 10;
  $feeds_tamper->description = 'Converts K Agris code into Conference Proceedings';
  $export['convert_type'] = $feeds_tamper;

  $feeds_tamper = new stdClass();
  $feeds_tamper->disabled = FALSE; /* Edit this to true to make a default feeds_tamper disabled initially */
  $feeds_tamper->api_version = 2;
  $feeds_tamper->id = 'convert_type_v';
  $feeds_tamper->importer = 'agris_apxml_Import';
  $feeds_tamper->source = 'xpathparser:28';
  $feeds_tamper->plugin_id = 'find_replace';
  $feeds_tamper->settings = array(
    'find' => 'V',
    'replace' => 'Journal Article',
    'case_sensitive' => 0,
    'word_boundaries' => 0,
    'whole' => 1,
    'regex' => TRUE,
    'regex_find' => '/^V$/i',
  );
  $feeds_tamper->weight = 11;
  $feeds_tamper->description = 'Converts V Agris code into Journal Article';
  $export['convert_type_v'] = $feeds_tamper;

  $feeds_tamper = new stdClass();
  $feeds_tamper->disabled = FALSE; /* Edit this to true to make a default feeds_tamper disabled initially */
  $feeds_tamper->api_version = 2;
  $feeds_tamper->id = 'date_add_month';
  $feeds_tamper->importer = 'agris_apxml_Import';
  $feeds_tamper->source = 'xpathparser:30';
  $feeds_tamper->plugin_id = 'find_replace_regex';
  $feeds_tamper->settings = array(
    'description' => '',
    'find' => '/^(19|20)(\\d{2})$/',
    'replace' => '\\1\\2-01',
    'limit' => '1',
    'real_limit' => '1',
  );
  $feeds_tamper->weight = 0;
  $feeds_tamper->description = '';
  $export['date_add_month'] = $feeds_tamper;

  $feeds_tamper = new stdClass();
  $feeds_tamper->disabled = FALSE; /* Edit this to true to make a default feeds_tamper disabled initially */
  $feeds_tamper->api_version = 2;
  $feeds_tamper->id = 'dlio_pre_importer_authors-xpathparser_2-find_replace_regex';
  $feeds_tamper->importer = 'dlio_pre_importer_authors';
  $feeds_tamper->source = 'xpathparser:2';
  $feeds_tamper->plugin_id = 'find_replace_regex';
  $feeds_tamper->settings = array(
    'find' => '/\\n/',
    'replace' => '',
    'limit' => '1',
    'real_limit' => '1',
  );
  $feeds_tamper->weight = 2;
  $feeds_tamper->description = 'Replace newline with space';
  $export['dlio_pre_importer_authors-xpathparser_2-find_replace_regex'] = $feeds_tamper;

  $feeds_tamper = new stdClass();
  $feeds_tamper->disabled = FALSE; /* Edit this to true to make a default feeds_tamper disabled initially */
  $feeds_tamper->api_version = 2;
  $feeds_tamper->id = 'dlio_pre_importer_authors-xpathparser_2-trim';
  $feeds_tamper->importer = 'dlio_pre_importer_authors';
  $feeds_tamper->source = 'xpathparser:2';
  $feeds_tamper->plugin_id = 'trim';
  $feeds_tamper->settings = array(
    'mask' => '',
    'side' => 'trim',
  );
  $feeds_tamper->weight = 4;
  $feeds_tamper->description = 'Trim';
  $export['dlio_pre_importer_authors-xpathparser_2-trim'] = $feeds_tamper;

  $feeds_tamper = new stdClass();
  $feeds_tamper->disabled = FALSE; /* Edit this to true to make a default feeds_tamper disabled initially */
  $feeds_tamper->api_version = 2;
  $feeds_tamper->id = 'dlio_pre_importer_authors-xpathparser_2-truncate';
  $feeds_tamper->importer = 'dlio_pre_importer_authors';
  $feeds_tamper->source = 'xpathparser:2';
  $feeds_tamper->plugin_id = 'truncate_text';
  $feeds_tamper->settings = array(
    'num_char' => '255',
    'ellipses' => 0,
  );
  $feeds_tamper->weight = 3;
  $feeds_tamper->description = 'Truncate';
  $export['dlio_pre_importer_authors-xpathparser_2-truncate'] = $feeds_tamper;

  $feeds_tamper = new stdClass();
  $feeds_tamper->disabled = FALSE; /* Edit this to true to make a default feeds_tamper disabled initially */
  $feeds_tamper->api_version = 2;
  $feeds_tamper->id = 'dlio_pre_importer_authors-xpathparser_3-find_replace_regex';
  $feeds_tamper->importer = 'dlio_pre_importer_authors';
  $feeds_tamper->source = 'xpathparser:3';
  $feeds_tamper->plugin_id = 'find_replace_regex';
  $feeds_tamper->settings = array(
    'find' => '/,(.*)/',
    'replace' => '',
    'limit' => '1',
    'real_limit' => '1',
  );
  $feeds_tamper->weight = 3;
  $feeds_tamper->description = 'remove first name';
  $export['dlio_pre_importer_authors-xpathparser_3-find_replace_regex'] = $feeds_tamper;

  $feeds_tamper = new stdClass();
  $feeds_tamper->disabled = FALSE; /* Edit this to true to make a default feeds_tamper disabled initially */
  $feeds_tamper->api_version = 2;
  $feeds_tamper->id = 'dlio_pre_importer_authors-xpathparser_3-findreplaceregex';
  $feeds_tamper->importer = 'dlio_pre_importer_authors';
  $feeds_tamper->source = 'xpathparser:3';
  $feeds_tamper->plugin_id = 'find_replace_regex';
  $feeds_tamper->settings = array(
    'find' => '/\\n/',
    'replace' => '',
    'limit' => '1',
    'real_limit' => '1',
  );
  $feeds_tamper->weight = 1;
  $feeds_tamper->description = 'Replace newline with space';
  $export['dlio_pre_importer_authors-xpathparser_3-findreplaceregex'] = $feeds_tamper;

  $feeds_tamper = new stdClass();
  $feeds_tamper->disabled = FALSE; /* Edit this to true to make a default feeds_tamper disabled initially */
  $feeds_tamper->api_version = 2;
  $feeds_tamper->id = 'dlio_pre_importer_authors-xpathparser_3-trim';
  $feeds_tamper->importer = 'dlio_pre_importer_authors';
  $feeds_tamper->source = 'xpathparser:3';
  $feeds_tamper->plugin_id = 'trim';
  $feeds_tamper->settings = array(
    'mask' => '',
    'side' => 'trim',
  );
  $feeds_tamper->weight = 4;
  $feeds_tamper->description = 'Trim';
  $export['dlio_pre_importer_authors-xpathparser_3-trim'] = $feeds_tamper;

  $feeds_tamper = new stdClass();
  $feeds_tamper->disabled = FALSE; /* Edit this to true to make a default feeds_tamper disabled initially */
  $feeds_tamper->api_version = 2;
  $feeds_tamper->id = 'dlio_pre_importer_authors-xpathparser_3-truncate';
  $feeds_tamper->importer = 'dlio_pre_importer_authors';
  $feeds_tamper->source = 'xpathparser:3';
  $feeds_tamper->plugin_id = 'truncate_text';
  $feeds_tamper->settings = array(
    'num_char' => '255',
    'ellipses' => 0,
  );
  $feeds_tamper->weight = 2;
  $feeds_tamper->description = 'Truncate';
  $export['dlio_pre_importer_authors-xpathparser_3-truncate'] = $feeds_tamper;

  $feeds_tamper = new stdClass();
  $feeds_tamper->disabled = FALSE; /* Edit this to true to make a default feeds_tamper disabled initially */
  $feeds_tamper->api_version = 2;
  $feeds_tamper->id = 'dlio_pre_importer_authors-xpathparser_4-find_replace_regex';
  $feeds_tamper->importer = 'dlio_pre_importer_authors';
  $feeds_tamper->source = 'xpathparser:4';
  $feeds_tamper->plugin_id = 'find_replace_regex';
  $feeds_tamper->settings = array(
    'find' => '/\\n/',
    'replace' => '',
    'limit' => '1',
    'real_limit' => '1',
  );
  $feeds_tamper->weight = 1;
  $feeds_tamper->description = 'Replace newline with space';
  $export['dlio_pre_importer_authors-xpathparser_4-find_replace_regex'] = $feeds_tamper;

  $feeds_tamper = new stdClass();
  $feeds_tamper->disabled = FALSE; /* Edit this to true to make a default feeds_tamper disabled initially */
  $feeds_tamper->api_version = 2;
  $feeds_tamper->id = 'dlio_pre_importer_authors-xpathparser_4-trim';
  $feeds_tamper->importer = 'dlio_pre_importer_authors';
  $feeds_tamper->source = 'xpathparser:4';
  $feeds_tamper->plugin_id = 'trim';
  $feeds_tamper->settings = array(
    'mask' => '',
    'side' => 'trim',
  );
  $feeds_tamper->weight = 3;
  $feeds_tamper->description = 'Trim';
  $export['dlio_pre_importer_authors-xpathparser_4-trim'] = $feeds_tamper;

  $feeds_tamper = new stdClass();
  $feeds_tamper->disabled = FALSE; /* Edit this to true to make a default feeds_tamper disabled initially */
  $feeds_tamper->api_version = 2;
  $feeds_tamper->id = 'dlio_pre_importer_authors-xpathparser_4-truncate';
  $feeds_tamper->importer = 'dlio_pre_importer_authors';
  $feeds_tamper->source = 'xpathparser:4';
  $feeds_tamper->plugin_id = 'truncate_text';
  $feeds_tamper->settings = array(
    'num_char' => '255',
    'ellipses' => 0,
  );
  $feeds_tamper->weight = 2;
  $feeds_tamper->description = 'Truncate';
  $export['dlio_pre_importer_authors-xpathparser_4-truncate'] = $feeds_tamper;

  $feeds_tamper = new stdClass();
  $feeds_tamper->disabled = FALSE; /* Edit this to true to make a default feeds_tamper disabled initially */
  $feeds_tamper->api_version = 2;
  $feeds_tamper->id = 'dlio_pre_importer_authors-xpathparser_5-execute_php_code';
  $feeds_tamper->importer = 'dlio_pre_importer_authors';
  $feeds_tamper->source = 'xpathparser:5';
  $feeds_tamper->plugin_id = 'php';
  $feeds_tamper->settings = array(
    'php' => '$arr=explode(",", $field);
if (count($arr)==3){
$field=$arr[1];
}else
{$field=""}
return $field;',
  );
  $feeds_tamper->weight = 1;
  $feeds_tamper->description = 'remove last name and affiliation';
  $export['dlio_pre_importer_authors-xpathparser_5-execute_php_code'] = $feeds_tamper;

  $feeds_tamper = new stdClass();
  $feeds_tamper->disabled = FALSE; /* Edit this to true to make a default feeds_tamper disabled initially */
  $feeds_tamper->api_version = 2;
  $feeds_tamper->id = 'dlio_pre_importer_authors-xpathparser_5-find_replace_regex';
  $feeds_tamper->importer = 'dlio_pre_importer_authors';
  $feeds_tamper->source = 'xpathparser:5';
  $feeds_tamper->plugin_id = 'find_replace_regex';
  $feeds_tamper->settings = array(
    'find' => '/\\n/',
    'replace' => '',
    'limit' => '1',
    'real_limit' => '1',
  );
  $feeds_tamper->weight = 1;
  $feeds_tamper->description = 'Replace newline with spaceFind replace REGEX';
  $export['dlio_pre_importer_authors-xpathparser_5-find_replace_regex'] = $feeds_tamper;

  $feeds_tamper = new stdClass();
  $feeds_tamper->disabled = FALSE; /* Edit this to true to make a default feeds_tamper disabled initially */
  $feeds_tamper->api_version = 2;
  $feeds_tamper->id = 'dlio_pre_importer_authors-xpathparser_5-findreplaceregex';
  $feeds_tamper->importer = 'dlio_pre_importer_authors';
  $feeds_tamper->source = 'xpathparser:5';
  $feeds_tamper->plugin_id = 'find_replace_regex';
  $feeds_tamper->settings = array(
    'find' => '/(.*),(\\s?)/',
    'replace' => '',
    'limit' => '1',
    'real_limit' => '1',
  );
  $feeds_tamper->weight = 3;
  $feeds_tamper->description = 'remove last name';
  $export['dlio_pre_importer_authors-xpathparser_5-findreplaceregex'] = $feeds_tamper;

  $feeds_tamper = new stdClass();
  $feeds_tamper->disabled = FALSE; /* Edit this to true to make a default feeds_tamper disabled initially */
  $feeds_tamper->api_version = 2;
  $feeds_tamper->id = 'dlio_pre_importer_authors-xpathparser_5-trim';
  $feeds_tamper->importer = 'dlio_pre_importer_authors';
  $feeds_tamper->source = 'xpathparser:5';
  $feeds_tamper->plugin_id = 'trim';
  $feeds_tamper->settings = array(
    'mask' => '',
    'side' => 'trim',
  );
  $feeds_tamper->weight = 2;
  $feeds_tamper->description = 'Trim';
  $export['dlio_pre_importer_authors-xpathparser_5-trim'] = $feeds_tamper;

  $feeds_tamper = new stdClass();
  $feeds_tamper->disabled = FALSE; /* Edit this to true to make a default feeds_tamper disabled initially */
  $feeds_tamper->api_version = 2;
  $feeds_tamper->id = 'dlio_pre_importer_authors-xpathparser_5-truncate';
  $feeds_tamper->importer = 'dlio_pre_importer_authors';
  $feeds_tamper->source = 'xpathparser:5';
  $feeds_tamper->plugin_id = 'truncate_text';
  $feeds_tamper->settings = array(
    'num_char' => '255',
    'ellipses' => 0,
  );
  $feeds_tamper->weight = 0;
  $feeds_tamper->description = 'Truncate';
  $export['dlio_pre_importer_authors-xpathparser_5-truncate'] = $feeds_tamper;

  $feeds_tamper = new stdClass();
  $feeds_tamper->disabled = FALSE; /* Edit this to true to make a default feeds_tamper disabled initially */
  $feeds_tamper->api_version = 2;
  $feeds_tamper->id = 'dlio_pre_importer_authors-xpathparser_6-execute_php_code';
  $feeds_tamper->importer = 'dlio_pre_importer_authors';
  $feeds_tamper->source = 'xpathparser:6';
  $feeds_tamper->plugin_id = 'php';
  $feeds_tamper->settings = array(
    'php' => '$arr=explode(",", $field);
if (count($arr)==3){ 
    $field=$arr[2];}
elseif (count($arr)==2) 
    {$field=$arr[1];}
elseif (count($arr)>3)
 {$field=$arr[2];}

return $field;',
  );
  $feeds_tamper->weight = 2;
  $feeds_tamper->description = 'remove last name and first name';
  $export['dlio_pre_importer_authors-xpathparser_6-execute_php_code'] = $feeds_tamper;

  $feeds_tamper = new stdClass();
  $feeds_tamper->disabled = FALSE; /* Edit this to true to make a default feeds_tamper disabled initially */
  $feeds_tamper->api_version = 2;
  $feeds_tamper->id = 'dlio_pre_importer_authors-xpathparser_6-trim';
  $feeds_tamper->importer = 'dlio_pre_importer_authors';
  $feeds_tamper->source = 'xpathparser:6';
  $feeds_tamper->plugin_id = 'trim';
  $feeds_tamper->settings = array(
    'mask' => '',
    'side' => 'trim',
  );
  $feeds_tamper->weight = 1;
  $feeds_tamper->description = 'Trim';
  $export['dlio_pre_importer_authors-xpathparser_6-trim'] = $feeds_tamper;

  $feeds_tamper = new stdClass();
  $feeds_tamper->disabled = FALSE; /* Edit this to true to make a default feeds_tamper disabled initially */
  $feeds_tamper->api_version = 2;
  $feeds_tamper->id = 'force_type_to_journal';
  $feeds_tamper->importer = 'agris_ap_pre_importer_journals';
  $feeds_tamper->source = 'xpathparser:3';
  $feeds_tamper->plugin_id = 'find_replace_regex';
  $feeds_tamper->settings = array(
    'find' => '/.*/',
    'replace' => 'Journal',
    'limit' => '1',
    'real_limit' => '1',
  );
  $feeds_tamper->weight = 2;
  $feeds_tamper->description = '';
  $export['force_type_to_journal'] = $feeds_tamper;

  $feeds_tamper = new stdClass();
  $feeds_tamper->disabled = FALSE; /* Edit this to true to make a default feeds_tamper disabled initially */
  $feeds_tamper->api_version = 2;
  $feeds_tamper->id = 'import_type';
  $feeds_tamper->importer = 'books_feed';
  $feeds_tamper->source = 'xpathparser:4';
  $feeds_tamper->plugin_id = 'find_replace_regex';
  $feeds_tamper->settings = array(
    'description' => '',
    'find' => '/.*/',
    'replace' => 'Book',
    'limit' => '1',
    'real_limit' => '1',
  );
  $feeds_tamper->weight = 0;
  $feeds_tamper->description = '';
  $export['import_type'] = $feeds_tamper;

  $feeds_tamper = new stdClass();
  $feeds_tamper->disabled = FALSE; /* Edit this to true to make a default feeds_tamper disabled initially */
  $feeds_tamper->api_version = 2;
  $feeds_tamper->id = 'type_letter_E_convert';
  $feeds_tamper->importer = 'agris_apxml_Import';
  $feeds_tamper->source = 'xpathparser:28';
  $feeds_tamper->plugin_id = 'find_replace';
  $feeds_tamper->settings = array(
    'find' => 'E',
    'replace' => 'Journal article',
    'case_sensitive' => 0,
    'word_boundaries' => 0,
    'whole' => 1,
    'regex' => TRUE,
    'regex_find' => '/^E$/i',
  );
  $feeds_tamper->weight = 12;
  $feeds_tamper->description = 'Converts E Agris code into Journal article';
  $export['type_letter_E_convert'] = $feeds_tamper;

  return $export;
}
