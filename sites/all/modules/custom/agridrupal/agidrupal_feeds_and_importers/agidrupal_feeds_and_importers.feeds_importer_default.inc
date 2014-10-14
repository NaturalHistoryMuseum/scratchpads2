<?php
/**
 * @file
 * agidrupal_feeds_and_importers.feeds_importer_default.inc
 */

/**
 * Implements hook_feeds_importer_default().
 */
function agidrupal_feeds_and_importers_feeds_importer_default() {
  $export = array();

  $feeds_importer = new stdClass();
  $feeds_importer->disabled = FALSE; /* Edit this to true to make a default feeds_importer disabled initially */
  $feeds_importer->api_version = 1;
  $feeds_importer->id = 'ag_books_feed';
  $feeds_importer->config = array(
    'name' => 'Ag Books Feed',
    'description' => 'To import books',
    'fetcher' => array(
      'plugin_key' => 'FeedsHTTPFetcher',
      'config' => array(
        'auto_detect_feeds' => FALSE,
        'use_pubsubhubbub' => FALSE,
        'designated_hub' => '',
      ),
    ),
    'parser' => array(
      'plugin_key' => 'FeedsXPathParserXML',
      'config' => array(
        'sources' => array(
          'xpathparser:1' => 'dc:source[1]/text()',
          'xpathparser:2' => 'dc:source[1]/text()',
          'xpathparser:3' => '',
          'xpathparser:4' => 'dc:source[1]/text()',
        ),
        'rawXML' => array(
          'xpathparser:1' => 0,
          'xpathparser:2' => 0,
          'xpathparser:3' => 0,
          'xpathparser:4' => 0,
        ),
        'context' => '/ags:resources/ags:resource',
        'exp' => array(
          'errors' => 1,
          'tidy' => 0,
          'tidy_encoding' => '',
          'debug' => array(
            'context' => 0,
            'xpathparser:1' => 0,
            'xpathparser:2' => 0,
            'xpathparser:3' => 0,
            'xpathparser:4' => 0,
          ),
        ),
      ),
    ),
    'processor' => array(
      'plugin_key' => 'FeedsNodeProcessor',
      'config' => array(
        'input_format' => '2',
        'update_existing' => '0',
        'expire' => '-1',
        'mappings' => array(
          0 => array(
            'source' => 'xpathparser:1',
            'target' => 'guid',
            'unique' => 1,
          ),
          1 => array(
            'source' => 'xpathparser:2',
            'target' => 'title',
            'unique' => FALSE,
          ),
          2 => array(
            'source' => 'xpathparser:3',
            'target' => 'field_ag_isbn',
            'unique' => FALSE,
          ),
          3 => array(
            'source' => 'xpathparser:4',
            'target' => 'field_ag_type',
            'unique' => FALSE,
          ),
        ),
        'author' => '1',
        'authorize' => 0,
        'bundle' => 'agdlios',
      ),
    ),
    'content_type' => 'books_feed',
    'update' => 0,
    'import_period' => '-1',
    'expire_period' => 3600,
    'import_on_create' => 0,
  );
  $export['ag_books_feed'] = $feeds_importer;

  $feeds_importer = new stdClass();
  $feeds_importer->disabled = FALSE; /* Edit this to true to make a default feeds_importer disabled initially */
  $feeds_importer->api_version = 1;
  $feeds_importer->id = 'ag_events_feed_importer';
  $feeds_importer->config = array(
    'name' => 'Ag Events feed Importer',
    'description' => 'To import feeds using the Event AP metadata set',
    'fetcher' => array(
      'plugin_key' => 'FeedsHTTPFetcher',
      'config' => array(
        'auto_detect_feeds' => FALSE,
        'use_pubsubhubbub' => FALSE,
        'designated_hub' => '',
      ),
    ),
    'parser' => array(
      'plugin_key' => 'FeedsXPathParserXML',
      'config' => array(
        'sources' => array(
          'xpathparser:0' => 'title',
          'xpathparser:1' => 'description',
          'xpathparser:2' => 'ags:location/ags:locationCity',
          'xpathparser:3' => 'ags:location/ags:locationCountry',
          'xpathparser:4' => 'ags:dateStart',
          'xpathparser:5' => 'ags:dateEnd',
          'xpathparser:6' => 'link',
          'xpathparser:8' => 'guid',
        ),
        'rawXML' => array(
          'xpathparser:0' => 'xpathparser:0',
          'xpathparser:1' => 0,
          'xpathparser:2' => 0,
          'xpathparser:3' => 0,
          'xpathparser:4' => 0,
          'xpathparser:5' => 0,
          'xpathparser:6' => 0,
          'xpathparser:8' => 0,
        ),
        'context' => 'rss/channel/item',
        'exp' => array(
          'errors' => 0,
          'tidy' => 0,
          'tidy_encoding' => 'UTF8',
          'debug' => array(
            'context' => 0,
            'xpathparser:0' => 0,
            'xpathparser:1' => 0,
            'xpathparser:2' => 0,
            'xpathparser:3' => 0,
            'xpathparser:4' => 0,
            'xpathparser:5' => 0,
            'xpathparser:6' => 0,
            'xpathparser:8' => 0,
          ),
        ),
        'allow_override' => 1,
      ),
    ),
    'processor' => array(
      'plugin_key' => 'FeedsNodeProcessor',
      'config' => array(
        'input_format' => '1',
        'update_existing' => '1',
        'expire' => '-1',
        'mappings' => array(
          0 => array(
            'source' => 'xpathparser:0',
            'target' => 'title',
            'unique' => FALSE,
          ),
          1 => array(
            'source' => 'xpathparser:1',
            'target' => 'body',
            'unique' => FALSE,
          ),
          2 => array(
            'source' => 'xpathparser:2',
            'target' => 'field_ag_location',
            'unique' => FALSE,
          ),
          3 => array(
            'source' => 'xpathparser:3',
            'target' => 'field_ag_country',
            'unique' => FALSE,
          ),
          4 => array(
            'source' => 'xpathparser:4',
            'target' => 'field_ag_date:start',
            'unique' => FALSE,
          ),
          5 => array(
            'source' => 'xpathparser:5',
            'target' => 'field_ag_date:end',
            'unique' => FALSE,
          ),
          6 => array(
            'source' => 'xpathparser:6',
            'target' => 'field_ag_source_url',
            'unique' => FALSE,
          ),
          7 => array(
            'source' => 'xpathparser:8',
            'target' => 'guid',
            'unique' => 1,
          ),
        ),
        'author' => 0,
        'bundle' => 'agrssevents',
      ),
    ),
    'content_type' => 'ageventsfeed',
    'update' => 0,
    'import_period' => '10800',
    'expire_period' => 3600,
    'import_on_create' => 0,
    'process_in_background' => FALSE,
  );
  $export['ag_events_feed_importer'] = $feeds_importer;

  $feeds_importer = new stdClass();
  $feeds_importer->disabled = FALSE; /* Edit this to true to make a default feeds_importer disabled initially */
  $feeds_importer->api_version = 1;
  $feeds_importer->id = 'ag_jobs_feed_importer';
  $feeds_importer->config = array(
    'name' => 'Ag Jobs Feed Importer',
    'description' => 'Parses job feeds with elements from the Ag-Job AP',
    'fetcher' => array(
      'plugin_key' => 'FeedsHTTPFetcher',
      'config' => array(
        'auto_detect_feeds' => FALSE,
        'use_pubsubhubbub' => FALSE,
        'designated_hub' => '',
      ),
    ),
    'parser' => array(
      'plugin_key' => 'FeedsXPathParserXML',
      'config' => array(
        'sources' => array(
          'xpathparser:0' => 'title',
          'xpathparser:1' => 'description',
          'xpathparser:2' => 'ags:location/ags:locationCity',
          'xpathparser:3' => 'ags:location/ags:locationCountry',
          'xpathparser:6' => 'link',
          'xpathparser:8' => 'ags:dateEnd',
          'xpathparser:9' => 'guid',
        ),
        'rawXML' => array(
          'xpathparser:0' => 0,
          'xpathparser:1' => 0,
          'xpathparser:2' => 0,
          'xpathparser:3' => 0,
          'xpathparser:6' => 0,
          'xpathparser:8' => 0,
          'xpathparser:9' => 0,
        ),
        'context' => '/rss/channel/item',
        'exp' => array(
          'errors' => 0,
          'tidy' => 0,
          'tidy_encoding' => 'UTF8',
          'debug' => array(
            'context' => 0,
            'xpathparser:0' => 0,
            'xpathparser:1' => 0,
            'xpathparser:2' => 0,
            'xpathparser:3' => 0,
            'xpathparser:6' => 0,
            'xpathparser:8' => 0,
            'xpathparser:9' => 0,
          ),
        ),
        'allow_override' => 1,
      ),
    ),
    'processor' => array(
      'plugin_key' => 'FeedsNodeProcessor',
      'config' => array(
        'input_format' => '1',
        'update_existing' => '1',
        'expire' => '-1',
        'mappings' => array(
          0 => array(
            'source' => 'xpathparser:0',
            'target' => 'title',
            'unique' => FALSE,
          ),
          1 => array(
            'source' => 'xpathparser:1',
            'target' => 'body',
            'unique' => FALSE,
          ),
          2 => array(
            'source' => 'xpathparser:2',
            'target' => 'field_ag_location',
            'unique' => FALSE,
          ),
          3 => array(
            'source' => 'xpathparser:3',
            'target' => 'field_ag_country',
            'unique' => FALSE,
          ),
          4 => array(
            'source' => 'xpathparser:6',
            'target' => 'field_ag_source_url',
            'unique' => FALSE,
          ),
          5 => array(
            'source' => 'xpathparser:8',
            'target' => 'field_ag_deadline:start',
            'unique' => FALSE,
          ),
          6 => array(
            'source' => 'xpathparser:9',
            'target' => 'guid',
            'unique' => 1,
          ),
        ),
        'author' => 0,
        'bundle' => 'agrssjob',
      ),
    ),
    'content_type' => 'ag_job_feed',
    'update' => 0,
    'import_period' => '10800',
    'expire_period' => 3600,
    'import_on_create' => 0,
    'process_in_background' => FALSE,
  );
  $export['ag_jobs_feed_importer'] = $feeds_importer;

  $feeds_importer = new stdClass();
  $feeds_importer->disabled = FALSE; /* Edit this to true to make a default feeds_importer disabled initially */
  $feeds_importer->api_version = 1;
  $feeds_importer->id = 'ag_news_feed_importer';
  $feeds_importer->config = array(
    'name' => 'Ag News Feed Importer',
    'description' => 'Parses news feeds with some additional elements',
    'fetcher' => array(
      'plugin_key' => 'FeedsHTTPFetcher',
      'config' => array(
        'auto_detect_feeds' => FALSE,
        'use_pubsubhubbub' => FALSE,
        'designated_hub' => '',
      ),
    ),
    'parser' => array(
      'plugin_key' => 'FeedsXPathParserXML',
      'config' => array(
        'sources' => array(
          'xpathparser:0' => 'title',
          'xpathparser:1' => 'description',
          'xpathparser:6' => 'link',
          'xpathparser:7' => '',
          'xpathparser:8' => 'guid',
          'xpathparser:9' => 'pubDate',
        ),
        'rawXML' => array(
          'xpathparser:0' => 0,
          'xpathparser:1' => 0,
          'xpathparser:6' => 0,
          'xpathparser:7' => 0,
          'xpathparser:8' => 0,
          'xpathparser:9' => 0,
        ),
        'context' => '/rss/channel/item',
        'exp' => array(
          'errors' => 0,
          'debug' => array(
            'context' => 0,
            'xpathparser:0' => 0,
            'xpathparser:1' => 0,
            'xpathparser:6' => 0,
            'xpathparser:7' => 0,
            'xpathparser:8' => 0,
            'xpathparser:9' => 0,
          ),
        ),
      ),
    ),
    'processor' => array(
      'plugin_key' => 'FeedsNodeProcessor',
      'config' => array(
        'input_format' => '1',
        'update_existing' => '1',
        'expire' => '-1',
        'mappings' => array(
          0 => array(
            'source' => 'xpathparser:0',
            'target' => 'title',
            'unique' => FALSE,
          ),
          1 => array(
            'source' => 'xpathparser:1',
            'target' => 'body',
            'unique' => FALSE,
          ),
          2 => array(
            'source' => 'xpathparser:6',
            'target' => 'field_ag_source_url',
            'unique' => FALSE,
          ),
          3 => array(
            'source' => 'xpathparser:8',
            'target' => 'guid',
            'unique' => 1,
          ),
          4 => array(
            'source' => 'xpathparser:9',
            'target' => 'field_ag_post_date:start',
            'unique' => FALSE,
          ),
        ),
        'author' => 0,
        'authorize' => 0,
        'bundle' => 'agrssnews',
      ),
    ),
    'content_type' => 'agnewsfeed',
    'update' => 0,
    'import_period' => '10800',
    'expire_period' => 3600,
    'import_on_create' => 0,
  );
  $export['ag_news_feed_importer'] = $feeds_importer;

  $feeds_importer = new stdClass();
  $feeds_importer->disabled = FALSE; /* Edit this to true to make a default feeds_importer disabled initially */
  $feeds_importer->api_version = 1;
  $feeds_importer->id = 'agris_ap_dlio_upload_importer';
  $feeds_importer->config = array(
    'name' => 'Agris AP DLIO UPLOAD importer',
    'description' => 'This importer imports DLIO records from an Agris AP export by uploading the XML file',
    'fetcher' => array(
      'plugin_key' => 'FeedsFileFetcher',
      'config' => array(
        'direct' => FALSE,
      ),
    ),
    'parser' => array(
      'plugin_key' => 'FeedsXPathParserXML',
      'config' => array(
        'sources' => array(
          'xpathparser:0' => 'dc:title',
          'xpathparser:1' => 'dc:identifier[not(@*)]',
          'xpathparser:2' => 'dc:identifier[not(@*)]',
          'xpathparser:3' => 'dc:title/@xml:lang',
          'xpathparser:4' => 'ags:citation/ags:citationNumber',
          'xpathparser:5' => '',
          'xpathparser:6' => 'ags:citation/ags:citationNumber',
          'xpathparser:7' => 'ags:descriptionEdition',
          'xpathparser:8' => 'dc:language',
          'xpathparser:9' => '',
          'xpathparser:10' => 'dc:identifier[@scheme="ags:ISBN"]',
          'xpathparser:11' => 'dc:description/dcterms:abstract/@xml:lang',
          'xpathparser:12' => 'dc:description/dcterms:abstract/text()',
          'xpathparser:13' => 'dc:identifier[@scheme="ags:ISSN"]',
          'xpathparser:14' => 'dcterms:extent',
          'xpathparser:15' => 'dcterms:medium',
          'xpathparser:16' => 'ags:descriptionNotes',
          'xpathparser:17' => 'ags:availabilityLocation',
          'xpathparser:18' => 'ags:availabilityNumber',
          'xpathparser:19' => 'ags:citation/ags:citationNumber',
          'xpathparser:20' => 'dc:publisher/ags:publisherPlace',
          'xpathparser:21' => 'ags:citation/ags:citationTitle',
          'xpathparser:22' => 'dc:creator/ags:creatorPersonal',
          'xpathparser:23' => '',
          'xpathparser:24' => 'dc:creator/ags:creatorCorporate',
          'xpathparser:25' => 'ags:creatorConference',
          'xpathparser:26' => 'ags:citation/ags:citationTitle',
          'xpathparser:27' => 'dc:publisher/ags:publisherName',
          'xpathparser:28' => 'dc:type',
          'xpathparser:29' => 'dc:subject/ags:subjectClassification',
          'xpathparser:30' => 'dc:date/dcterms:dateIssued',
          'xpathparser:31' => 'dc:subject/ags:subjectThesaurus[@xml:lang="en"]',
          'xpathparser:32' => '',
        ),
        'rawXML' => array(
          'xpathparser:0' => 0,
          'xpathparser:1' => 0,
          'xpathparser:2' => 0,
          'xpathparser:3' => 0,
          'xpathparser:4' => 0,
          'xpathparser:5' => 0,
          'xpathparser:6' => 0,
          'xpathparser:7' => 0,
          'xpathparser:8' => 0,
          'xpathparser:9' => 0,
          'xpathparser:10' => 0,
          'xpathparser:11' => 0,
          'xpathparser:12' => 0,
          'xpathparser:13' => 0,
          'xpathparser:14' => 0,
          'xpathparser:15' => 0,
          'xpathparser:16' => 0,
          'xpathparser:17' => 0,
          'xpathparser:18' => 0,
          'xpathparser:19' => 0,
          'xpathparser:20' => 0,
          'xpathparser:21' => 0,
          'xpathparser:22' => 0,
          'xpathparser:23' => 0,
          'xpathparser:24' => 0,
          'xpathparser:25' => 0,
          'xpathparser:26' => 0,
          'xpathparser:27' => 0,
          'xpathparser:28' => 0,
          'xpathparser:29' => 0,
          'xpathparser:30' => 0,
          'xpathparser:31' => 0,
          'xpathparser:32' => 0,
        ),
        'context' => '/ags:resources/ags:resource',
        'exp' => array(
          'errors' => 0,
          'debug' => array(
            'context' => 0,
            'xpathparser:0' => 0,
            'xpathparser:1' => 0,
            'xpathparser:2' => 0,
            'xpathparser:3' => 0,
            'xpathparser:4' => 0,
            'xpathparser:5' => 0,
            'xpathparser:6' => 0,
            'xpathparser:7' => 0,
            'xpathparser:8' => 0,
            'xpathparser:9' => 0,
            'xpathparser:10' => 0,
            'xpathparser:11' => 0,
            'xpathparser:12' => 0,
            'xpathparser:13' => 0,
            'xpathparser:14' => 0,
            'xpathparser:15' => 0,
            'xpathparser:16' => 0,
            'xpathparser:17' => 0,
            'xpathparser:18' => 0,
            'xpathparser:19' => 0,
            'xpathparser:20' => 0,
            'xpathparser:21' => 0,
            'xpathparser:22' => 0,
            'xpathparser:23' => 0,
            'xpathparser:24' => 0,
            'xpathparser:25' => 0,
            'xpathparser:26' => 0,
            'xpathparser:27' => 0,
            'xpathparser:28' => 0,
            'xpathparser:29' => 0,
            'xpathparser:30' => 0,
            'xpathparser:31' => 0,
            'xpathparser:32' => 0,
          ),
        ),
      ),
    ),
    'processor' => array(
      'plugin_key' => 'FeedsNodeProcessor',
      'config' => array(
        'input_format' => '2',
        'update_existing' => '0',
        'expire' => '-1',
        'mappings' => array(
          0 => array(
            'source' => 'xpathparser:0',
            'target' => 'title',
            'unique' => FALSE,
          ),
          1 => array(
            'source' => 'parent:uid',
            'target' => 'uid',
            'unique' => FALSE,
          ),
          2 => array(
            'source' => 'xpathparser:1',
            'target' => 'url',
            'unique' => 0,
          ),
          3 => array(
            'source' => 'xpathparser:2',
            'target' => 'guid',
            'unique' => 1,
          ),
          4 => array(
            'source' => 'xpathparser:3',
            'target' => 'field_ag_langtitle',
            'unique' => FALSE,
          ),
          5 => array(
            'source' => 'xpathparser:4',
            'target' => 'field_ag_number',
            'unique' => FALSE,
          ),
          6 => array(
            'source' => 'xpathparser:5',
            'target' => 'field_ag_subtitle',
            'unique' => FALSE,
          ),
          7 => array(
            'source' => 'xpathparser:6',
            'target' => 'field_ag_volume',
            'unique' => FALSE,
          ),
          8 => array(
            'source' => 'xpathparser:7',
            'target' => 'field_ag_edition',
            'unique' => FALSE,
          ),
          9 => array(
            'source' => 'xpathparser:8',
            'target' => 'field_ag_language',
            'unique' => FALSE,
          ),
          10 => array(
            'source' => 'xpathparser:9',
            'target' => 'field_ag_langsubt',
            'unique' => FALSE,
          ),
          11 => array(
            'source' => 'xpathparser:10',
            'target' => 'field_ag_isbn',
            'unique' => FALSE,
          ),
          12 => array(
            'source' => 'xpathparser:11',
            'target' => 'field_ag_langtext',
            'unique' => FALSE,
          ),
          13 => array(
            'source' => 'xpathparser:12',
            'target' => 'field_ag_abstract',
            'unique' => FALSE,
          ),
          14 => array(
            'source' => 'xpathparser:13',
            'target' => 'field_ag_issn',
            'unique' => FALSE,
          ),
          15 => array(
            'source' => 'xpathparser:14',
            'target' => 'field_ag_pagination',
            'unique' => FALSE,
          ),
          16 => array(
            'source' => 'xpathparser:15',
            'target' => 'field_ag_form',
            'unique' => FALSE,
          ),
          17 => array(
            'source' => 'xpathparser:16',
            'target' => 'field_ag_notes',
            'unique' => FALSE,
          ),
          18 => array(
            'source' => 'xpathparser:17',
            'target' => 'field_ag_phlocation',
            'unique' => FALSE,
          ),
          19 => array(
            'source' => 'xpathparser:18',
            'target' => 'field_ag_locator',
            'unique' => FALSE,
          ),
          20 => array(
            'source' => 'xpathparser:19',
            'target' => 'field_ag_booknumber',
            'unique' => FALSE,
          ),
          21 => array(
            'source' => 'xpathparser:20',
            'target' => 'field_ag_publishplace',
            'unique' => FALSE,
          ),
          22 => array(
            'source' => 'xpathparser:21',
            'target' => 'field_ag_journal:title',
            'unique' => FALSE,
          ),
          23 => array(
            'source' => 'xpathparser:22',
            'target' => 'field_ag_author:title',
            'unique' => FALSE,
          ),
          24 => array(
            'source' => 'xpathparser:23',
            'target' => 'field_ag_journal_issue:title',
            'unique' => FALSE,
          ),
          25 => array(
            'source' => 'xpathparser:24',
            'target' => 'field_ag_corauthor:title',
            'unique' => FALSE,
          ),
          26 => array(
            'source' => 'xpathparser:25',
            'target' => 'field_ag_confname:title',
            'unique' => FALSE,
          ),
          27 => array(
            'source' => 'xpathparser:26',
            'target' => 'field_ag_book:title',
            'unique' => FALSE,
          ),
          28 => array(
            'source' => 'xpathparser:27',
            'target' => 'field_ag_publisher:title',
            'unique' => FALSE,
          ),
          29 => array(
            'source' => 'xpathparser:28',
            'target' => 'field_ag_type',
            'unique' => FALSE,
          ),
          30 => array(
            'source' => 'xpathparser:29',
            'target' => 'field_ag_agris',
            'unique' => FALSE,
          ),
          31 => array(
            'source' => 'xpathparser:30',
            'target' => 'field_ag_datepub:start',
            'unique' => FALSE,
          ),
          32 => array(
            'source' => 'xpathparser:31',
            'target' => 'field_ag_agrovocimport',
            'unique' => FALSE,
          ),
          33 => array(
            'source' => 'xpathparser:32',
            'target' => 'field_ag_if_imported',
            'unique' => FALSE,
          ),
        ),
        'author' => '1',
        'authorize' => 0,
        'bundle' => 'agdlios',
      ),
    ),
    'content_type' => 'agris_dlio_upload',
    'update' => 0,
    'import_period' => '-1',
    'expire_period' => 3600,
    'import_on_create' => 0,
  );
  $export['agris_ap_dlio_upload_importer'] = $feeds_importer;

  $feeds_importer = new stdClass();
  $feeds_importer->disabled = FALSE; /* Edit this to true to make a default feeds_importer disabled initially */
  $feeds_importer->api_version = 1;
  $feeds_importer->id = 'agris_ap_pre_importer_conferences';
  $feeds_importer->config = array(
    'name' => 'Agris AP pre-importer: conferences',
    'description' => '',
    'fetcher' => array(
      'plugin_key' => 'FeedsHTTPFetcher',
      'config' => array(
        'auto_detect_feeds' => FALSE,
        'use_pubsubhubbub' => FALSE,
        'designated_hub' => '',
      ),
    ),
    'parser' => array(
      'plugin_key' => 'FeedsXPathParserXML',
      'config' => array(
        'sources' => array(
          'xpathparser:0' => 'text()',
          'xpathparser:1' => 'text()',
        ),
        'rawXML' => array(
          'xpathparser:0' => 0,
          'xpathparser:1' => 0,
        ),
        'context' => '/ags:resources/ags:resource//ags:creatorConference',
        'exp' => array(
          'errors' => 1,
          'tidy' => 0,
          'tidy_encoding' => '',
          'debug' => array(
            'context' => 0,
            'xpathparser:0' => 0,
            'xpathparser:1' => 0,
          ),
        ),
      ),
    ),
    'processor' => array(
      'plugin_key' => 'FeedsNodeProcessor',
      'config' => array(
        'input_format' => '2',
        'update_existing' => '0',
        'expire' => '-1',
        'mappings' => array(
          0 => array(
            'source' => 'xpathparser:0',
            'target' => 'title',
            'unique' => FALSE,
          ),
          1 => array(
            'source' => 'xpathparser:1',
            'target' => 'guid',
            'unique' => 1,
          ),
        ),
        'author' => '1',
        'authorize' => 0,
        'bundle' => 'agconference',
      ),
    ),
    'content_type' => 'agris_conferences_feed',
    'update' => 0,
    'import_period' => '-1',
    'expire_period' => 3600,
    'import_on_create' => 0,
  );
  $export['agris_ap_pre_importer_conferences'] = $feeds_importer;

  $feeds_importer = new stdClass();
  $feeds_importer->disabled = FALSE; /* Edit this to true to make a default feeds_importer disabled initially */
  $feeds_importer->api_version = 1;
  $feeds_importer->id = 'agris_ap_pre_importer_corporate_authors';
  $feeds_importer->config = array(
    'name' => 'Agris AP pre-importer: corporate authors',
    'description' => 'This importer imports corp. author records from the DLIO feed into the corp. body reference list. Use it before importing DLIOs',
    'fetcher' => array(
      'plugin_key' => 'FeedsHTTPFetcher',
      'config' => array(
        'auto_detect_feeds' => FALSE,
        'use_pubsubhubbub' => FALSE,
        'designated_hub' => '',
      ),
    ),
    'parser' => array(
      'plugin_key' => 'FeedsXPathParserXML',
      'config' => array(
        'sources' => array(
          'xpathparser:0' => 'text()',
          'xpathparser:1' => 'text()',
        ),
        'rawXML' => array(
          'xpathparser:0' => 0,
          'xpathparser:1' => 0,
        ),
        'context' => '/ags:resources/ags:resource//ags:creatorCorporate',
        'exp' => array(
          'errors' => 1,
          'tidy' => 0,
          'tidy_encoding' => '',
          'debug' => array(
            'context' => 0,
            'xpathparser:0' => 0,
            'xpathparser:1' => 0,
          ),
        ),
      ),
    ),
    'processor' => array(
      'plugin_key' => 'FeedsNodeProcessor',
      'config' => array(
        'input_format' => '2',
        'update_existing' => '0',
        'expire' => '-1',
        'mappings' => array(
          0 => array(
            'source' => 'xpathparser:0',
            'target' => 'title',
            'unique' => FALSE,
          ),
          1 => array(
            'source' => 'xpathparser:1',
            'target' => 'guid',
            'unique' => 1,
          ),
        ),
        'author' => '1',
        'authorize' => 0,
        'bundle' => 'agcorporate_body',
      ),
    ),
    'content_type' => 'agris_corporate_feed',
    'update' => 0,
    'import_period' => '-1',
    'expire_period' => 3600,
    'import_on_create' => 0,
  );
  $export['agris_ap_pre_importer_corporate_authors'] = $feeds_importer;

  $feeds_importer = new stdClass();
  $feeds_importer->disabled = FALSE; /* Edit this to true to make a default feeds_importer disabled initially */
  $feeds_importer->api_version = 1;
  $feeds_importer->id = 'agris_ap_pre_importer_journals';
  $feeds_importer->config = array(
    'name' => 'Agris AP pre-importer: journals',
    'description' => 'This importer imports journal records from the DLIO feed into the DLIO (journal) reference list. Use it before importing DLIO',
    'fetcher' => array(
      'plugin_key' => 'FeedsHTTPFetcher',
      'config' => array(
        'auto_detect_feeds' => FALSE,
        'use_pubsubhubbub' => FALSE,
        'designated_hub' => '',
      ),
    ),
    'parser' => array(
      'plugin_key' => 'FeedsXPathParserXML',
      'config' => array(
        'sources' => array(
          'xpathparser:0' => 'ags:citationTitle[1]/text()',
          'xpathparser:1' => 'ags:citationTitle[1]/text()',
          'xpathparser:3' => 'ags:citationTitle/text()',
          'xpathparser:4' => 'ags:citationIdentifier[@scheme="ags:ISSN"]',
          'xpathparser:5' => 'ags:citationTitle[2]/text()',
        ),
        'rawXML' => array(
          'xpathparser:0' => 0,
          'xpathparser:1' => 0,
          'xpathparser:3' => 0,
          'xpathparser:4' => 0,
          'xpathparser:5' => 0,
        ),
        'context' => '/ags:resources/ags:resource/ags:citation',
        'exp' => array(
          'errors' => 1,
          'tidy' => 0,
          'tidy_encoding' => '',
          'debug' => array(
            'context' => 0,
            'xpathparser:0' => 0,
            'xpathparser:1' => 0,
            'xpathparser:3' => 0,
            'xpathparser:4' => 0,
            'xpathparser:5' => 0,
          ),
        ),
        'allow_override' => TRUE,
      ),
    ),
    'processor' => array(
      'plugin_key' => 'FeedsNodeProcessor',
      'config' => array(
        'input_format' => '2',
        'update_existing' => '2',
        'expire' => '-1',
        'mappings' => array(
          0 => array(
            'source' => 'xpathparser:0',
            'target' => 'title',
            'unique' => FALSE,
          ),
          1 => array(
            'source' => 'xpathparser:1',
            'target' => 'guid',
            'unique' => 1,
          ),
          2 => array(
            'source' => 'xpathparser:3',
            'target' => 'field_ag_type',
            'unique' => FALSE,
          ),
          3 => array(
            'source' => 'xpathparser:4',
            'target' => 'field_ag_issn',
            'unique' => FALSE,
          ),
          4 => array(
            'source' => 'xpathparser:5',
            'target' => 'field_ag_subtitle',
            'unique' => FALSE,
          ),
        ),
        'author' => '1',
        'bundle' => 'agperiodical',
      ),
    ),
    'content_type' => 'agris_journals_feed',
    'update' => 0,
    'import_period' => '-1',
    'expire_period' => 3600,
    'import_on_create' => 0,
    'process_in_background' => FALSE,
  );
  $export['agris_ap_pre_importer_journals'] = $feeds_importer;

  $feeds_importer = new stdClass();
  $feeds_importer->disabled = FALSE; /* Edit this to true to make a default feeds_importer disabled initially */
  $feeds_importer->api_version = 1;
  $feeds_importer->id = 'agris_ap_pre_importer_publishers';
  $feeds_importer->config = array(
    'name' => 'Agris AP pre-importer: publishers',
    'description' => 'This importer imports publishers records from the DLIO feed into the Corporate body reference list. Use it before importing DLIO',
    'fetcher' => array(
      'plugin_key' => 'FeedsHTTPFetcher',
      'config' => array(
        'auto_detect_feeds' => FALSE,
        'use_pubsubhubbub' => FALSE,
        'designated_hub' => '',
      ),
    ),
    'parser' => array(
      'plugin_key' => 'FeedsXPathParserXML',
      'config' => array(
        'sources' => array(
          'xpathparser:0' => 'text()',
          'xpathparser:1' => 'text()',
        ),
        'rawXML' => array(
          'xpathparser:0' => 0,
          'xpathparser:1' => 0,
        ),
        'context' => '/ags:resources/ags:resource/dc:publisher/ags:publisherName',
        'exp' => array(
          'errors' => 1,
          'tidy' => 0,
          'tidy_encoding' => '',
          'debug' => array(
            'context' => 0,
            'xpathparser:0' => 0,
            'xpathparser:1' => 0,
          ),
        ),
      ),
    ),
    'processor' => array(
      'plugin_key' => 'FeedsNodeProcessor',
      'config' => array(
        'input_format' => '2',
        'update_existing' => '0',
        'expire' => '-1',
        'mappings' => array(
          0 => array(
            'source' => 'xpathparser:0',
            'target' => 'title',
            'unique' => FALSE,
          ),
          1 => array(
            'source' => 'xpathparser:1',
            'target' => 'guid',
            'unique' => 1,
          ),
        ),
        'author' => '1',
        'authorize' => 0,
        'bundle' => 'agcorporate_body',
      ),
    ),
    'content_type' => 'agris_publishers_feed',
    'update' => 0,
    'import_period' => '-1',
    'expire_period' => 3600,
    'import_on_create' => 0,
  );
  $export['agris_ap_pre_importer_publishers'] = $feeds_importer;

  $feeds_importer = new stdClass();
  $feeds_importer->disabled = FALSE; /* Edit this to true to make a default feeds_importer disabled initially */
  $feeds_importer->api_version = 1;
  $feeds_importer->id = 'agris_apxml_Import';
  $feeds_importer->config = array(
    'name' => 'Agris AP DLIO Importer ',
    'description' => 'This importer imports DLIO records from an Agris AP export',
    'fetcher' => array(
      'plugin_key' => 'FeedsHTTPFetcher',
      'config' => array(
        'auto_detect_feeds' => FALSE,
        'use_pubsubhubbub' => FALSE,
        'designated_hub' => '',
      ),
    ),
    'parser' => array(
      'plugin_key' => 'FeedsXPathParserXML',
      'config' => array(
        'sources' => array(
          'xpathparser:0' => 'dc:title[not(*)][1]/text()',
          'xpathparser:2' => '@ags:ARN',
          'xpathparser:7' => 'dc:description/ags:descriptionEdition',
          'xpathparser:10' => 'dc:identifier[@scheme="ags:ISBN"]',
          'xpathparser:13' => 'dc:identifier[@scheme="ags:ISSN"]',
          'xpathparser:14' => 'dc:format/dcterms:extent',
          'xpathparser:15' => 'dc:format/dcterms:medium',
          'xpathparser:16' => 'dc:description/ags:descriptionNotes',
          'xpathparser:25' => 'dc:creator/ags:creatorConference',
          'xpathparser:28' => 'dc:type[1]/text()',
          'xpathparser:29' => 'dc:subject/ags:subjectClassification[@scheme="ags:ASC"]',
          'xpathparser:32' => 'dc:subject/ags:subjectThesaurus[@xml:lang="en"]',
          'xpathparser:34' => 'dc:source[1]/text()',
          'xpathparser:35' => '',
          'xpathparser:52' => 'dc:title[1]/@xml:lang',
          'xpathparser:53' => 'dc:language',
          'xpathparser:56' => 'dc:date/dcterms:dateIssued',
          'xpathparser:57' => 'dc:title/dcterms:alternative',
          'xpathparser:58' => 'ags:citation/ags:citationNumber[1]',
          'xpathparser:59' => 'ags:citation/ags:citationNumber[1]',
          'xpathparser:60' => 'dc:title/dcterms:alternative/@xml:lang',
          'xpathparser:61' => 'dc:description/dcterms:abstract/text()',
          'xpathparser:62' => 'dc:description/dcterms:abstract/@xml:lang',
          'xpathparser:63' => 'agls:availability/ags:availabilityLocation',
          'xpathparser:64' => 'agls:availability/ags:availabilityNumber',
          'xpathparser:65' => '',
          'xpathparser:66' => 'dc:source[1]/text()',
          'xpathparser:67' => 'ags:citation/ags:citationTitle[1]/text()',
          'xpathparser:68' => 'dc:publisher/ags:publisherPlace',
          'xpathparser:69' => 'dc:publisher/ags:publisherName',
          'xpathparser:70' => 'dc:creator/ags:creatorPersonal',
          'xpathparser:71' => 'dc:creator/ags:creatorCorporate',
          'xpathparser:72' => 'dc:identifier[@scheme="ags:EISSN"]',
          'xpathparser:73' => 'ags:citation/ags:citationNumber[1]',
          'xpathparser:74' => 'dc:identifier[@scheme="dcterms:URI"]',
        ),
        'rawXML' => array(
          'xpathparser:0' => 0,
          'xpathparser:2' => 0,
          'xpathparser:7' => 0,
          'xpathparser:10' => 0,
          'xpathparser:13' => 0,
          'xpathparser:14' => 0,
          'xpathparser:15' => 0,
          'xpathparser:16' => 0,
          'xpathparser:25' => 0,
          'xpathparser:28' => 0,
          'xpathparser:29' => 0,
          'xpathparser:32' => 0,
          'xpathparser:34' => 0,
          'xpathparser:35' => 0,
          'xpathparser:52' => 0,
          'xpathparser:53' => 0,
          'xpathparser:56' => 0,
          'xpathparser:57' => 0,
          'xpathparser:58' => 0,
          'xpathparser:59' => 0,
          'xpathparser:60' => 0,
          'xpathparser:61' => 0,
          'xpathparser:62' => 0,
          'xpathparser:63' => 0,
          'xpathparser:64' => 0,
          'xpathparser:65' => 0,
          'xpathparser:66' => 0,
          'xpathparser:67' => 0,
          'xpathparser:68' => 0,
          'xpathparser:69' => 0,
          'xpathparser:70' => 0,
          'xpathparser:71' => 0,
          'xpathparser:72' => 0,
          'xpathparser:73' => 0,
          'xpathparser:74' => 0,
        ),
        'context' => '/ags:resources/ags:resource',
        'exp' => array(
          'errors' => 1,
          'debug' => array(
            'context' => 0,
            'xpathparser:0' => 0,
            'xpathparser:2' => 0,
            'xpathparser:7' => 0,
            'xpathparser:10' => 0,
            'xpathparser:13' => 0,
            'xpathparser:14' => 0,
            'xpathparser:15' => 0,
            'xpathparser:16' => 0,
            'xpathparser:25' => 0,
            'xpathparser:28' => 0,
            'xpathparser:29' => 0,
            'xpathparser:32' => 0,
            'xpathparser:34' => 0,
            'xpathparser:35' => 0,
            'xpathparser:52' => 0,
            'xpathparser:53' => 0,
            'xpathparser:56' => 0,
            'xpathparser:57' => 0,
            'xpathparser:58' => 0,
            'xpathparser:59' => 0,
            'xpathparser:60' => 0,
            'xpathparser:61' => 0,
            'xpathparser:62' => 0,
            'xpathparser:63' => 0,
            'xpathparser:64' => 0,
            'xpathparser:65' => 0,
            'xpathparser:66' => 0,
            'xpathparser:67' => 0,
            'xpathparser:68' => 0,
            'xpathparser:69' => 0,
            'xpathparser:70' => 0,
            'xpathparser:71' => 0,
            'xpathparser:72' => 0,
            'xpathparser:73' => 0,
            'xpathparser:74' => 0,
          ),
        ),
        'allow_override' => 1,
      ),
    ),
    'processor' => array(
      'plugin_key' => 'FeedsNodeProcessor',
      'config' => array(
        'input_format' => '1',
        'update_existing' => '0',
        'expire' => '-1',
        'mappings' => array(
          0 => array(
            'source' => 'xpathparser:0',
            'target' => 'title',
            'unique' => FALSE,
          ),
          1 => array(
            'source' => 'xpathparser:2',
            'target' => 'guid',
            'unique' => 1,
          ),
          2 => array(
            'source' => 'xpathparser:7',
            'target' => 'field_ag_edition',
            'unique' => FALSE,
          ),
          3 => array(
            'source' => 'xpathparser:10',
            'target' => 'field_ag_isbn',
            'unique' => FALSE,
          ),
          4 => array(
            'source' => 'xpathparser:13',
            'target' => 'field_ag_issn',
            'unique' => FALSE,
          ),
          5 => array(
            'source' => 'xpathparser:14',
            'target' => 'field_ag_pagination',
            'unique' => FALSE,
          ),
          6 => array(
            'source' => 'xpathparser:15',
            'target' => 'field_ag_form',
            'unique' => FALSE,
          ),
          7 => array(
            'source' => 'xpathparser:16',
            'target' => 'field_ag_notes',
            'unique' => FALSE,
          ),
          8 => array(
            'source' => 'xpathparser:25',
            'target' => 'field_ag_confname:title',
            'unique' => FALSE,
          ),
          9 => array(
            'source' => 'xpathparser:28',
            'target' => 'field_ag_type',
            'unique' => FALSE,
          ),
          10 => array(
            'source' => 'xpathparser:29',
            'target' => 'field_ag_agris',
            'unique' => FALSE,
          ),
          11 => array(
            'source' => 'xpathparser:32',
            'target' => 'field_ag_agrovoc',
            'unique' => FALSE,
          ),
          12 => array(
            'source' => 'xpathparser:34',
            'target' => 'field_ag_source',
            'unique' => FALSE,
          ),
          13 => array(
            'source' => 'xpathparser:35',
            'target' => 'field_ag_link_to_jissue',
            'unique' => FALSE,
          ),
          14 => array(
            'source' => 'xpathparser:52',
            'target' => 'field_ag_langtitle',
            'unique' => FALSE,
          ),
          15 => array(
            'source' => 'xpathparser:53',
            'target' => 'field_ag_language',
            'unique' => FALSE,
          ),
          16 => array(
            'source' => 'xpathparser:56',
            'target' => 'field_ag_datepub:start',
            'unique' => FALSE,
          ),
          17 => array(
            'source' => 'xpathparser:57',
            'target' => 'field_ag_subtitle',
            'unique' => FALSE,
          ),
          18 => array(
            'source' => 'xpathparser:58',
            'target' => 'field_ag_volume',
            'unique' => FALSE,
          ),
          19 => array(
            'source' => 'xpathparser:59',
            'target' => 'field_ag_number',
            'unique' => FALSE,
          ),
          20 => array(
            'source' => 'xpathparser:60',
            'target' => 'field_ag_langsubt',
            'unique' => FALSE,
          ),
          21 => array(
            'source' => 'xpathparser:61',
            'target' => 'field_ag_abstract',
            'unique' => FALSE,
          ),
          22 => array(
            'source' => 'xpathparser:62',
            'target' => 'field_ag_langtext',
            'unique' => FALSE,
          ),
          23 => array(
            'source' => 'xpathparser:63',
            'target' => 'field_ag_phlocation',
            'unique' => FALSE,
          ),
          24 => array(
            'source' => 'xpathparser:64',
            'target' => 'field_ag_locator',
            'unique' => FALSE,
          ),
          25 => array(
            'source' => 'xpathparser:65',
            'target' => 'field_ag_booknumber',
            'unique' => FALSE,
          ),
          26 => array(
            'source' => 'xpathparser:66',
            'target' => 'field_ag_book:title',
            'unique' => FALSE,
          ),
          27 => array(
            'source' => 'xpathparser:67',
            'target' => 'field_ag_journal:title',
            'unique' => FALSE,
          ),
          28 => array(
            'source' => 'xpathparser:68',
            'target' => 'field_ag_publishplace',
            'unique' => FALSE,
          ),
          29 => array(
            'source' => 'xpathparser:69',
            'target' => 'field_ag_publisher:title',
            'unique' => FALSE,
          ),
          30 => array(
            'source' => 'xpathparser:70',
            'target' => 'field_ag_author:title',
            'unique' => FALSE,
          ),
          31 => array(
            'source' => 'xpathparser:71',
            'target' => 'field_ag_corauthor:title',
            'unique' => FALSE,
          ),
          32 => array(
            'source' => 'xpathparser:72',
            'target' => 'field_ag_eissn',
            'unique' => FALSE,
          ),
          33 => array(
            'source' => 'xpathparser:73',
            'target' => 'field_pages',
            'unique' => FALSE,
          ),
          34 => array(
            'source' => 'xpathparser:74',
            'target' => 'field_ag_resource_url:url',
            'unique' => FALSE,
          ),
        ),
        'author' => '1',
        'bundle' => 'agdlios',
      ),
    ),
    'content_type' => 'xmlimport_dlios',
    'update' => 0,
    'import_period' => '-1',
    'expire_period' => 3600,
    'import_on_create' => 0,
    'process_in_background' => FALSE,
  );
  $export['agris_apxml_Import'] = $feeds_importer;

  $feeds_importer = new stdClass();
  $feeds_importer->disabled = FALSE; /* Edit this to true to make a default feeds_importer disabled initially */
  $feeds_importer->api_version = 1;
  $feeds_importer->id = 'books_feed';
  $feeds_importer->config = array(
    'name' => 'Books feed',
    'description' => 'import books',
    'fetcher' => array(
      'plugin_key' => 'FeedsHTTPFetcher',
      'config' => array(
        'auto_detect_feeds' => FALSE,
        'use_pubsubhubbub' => FALSE,
        'designated_hub' => '',
      ),
    ),
    'parser' => array(
      'plugin_key' => 'FeedsXPathParserXML',
      'config' => array(
        'sources' => array(
          'xpathparser:0' => 'ags:citationTitle/text()',
          'xpathparser:1' => 'ags:citationTitle/text()',
          'xpathparser:2' => '',
          'xpathparser:3' => 'ags:citationTitle/text()',
          'xpathparser:4' => 'ags:citationIdentifier[@scheme="ags:ISSN"]',
        ),
        'rawXML' => array(
          'xpathparser:0' => 0,
          'xpathparser:1' => 0,
          'xpathparser:2' => 0,
          'xpathparser:3' => 0,
          'xpathparser:4' => 0,
        ),
        'context' => '/ags:resources/ags:resource/ags:citation',
        'exp' => array(
          'errors' => 0,
          'debug' => array(
            'context' => 0,
            'xpathparser:0' => 0,
            'xpathparser:1' => 0,
            'xpathparser:2' => 0,
            'xpathparser:3' => 0,
            'xpathparser:4' => 0,
          ),
        ),
      ),
    ),
    'processor' => array(
      'plugin_key' => 'FeedsNodeProcessor',
      'config' => array(
        'input_format' => '5',
        'update_existing' => '0',
        'expire' => '-1',
        'mappings' => array(
          0 => array(
            'source' => 'xpathparser:1',
            'target' => 'guid',
            'unique' => 1,
          ),
          1 => array(
            'source' => 'xpathparser:2',
            'target' => 'title',
            'unique' => FALSE,
          ),
          2 => array(
            'source' => 'xpathparser:3',
            'target' => 'field_ag_isbn',
            'unique' => FALSE,
          ),
          3 => array(
            'source' => 'xpathparser:4',
            'target' => 'field_ag_type',
            'unique' => FALSE,
          ),
        ),
        'author' => '1',
        'authorize' => 0,
        'bundle' => 'dlios',
      ),
    ),
    'content_type' => 'books_feed',
    'update' => 0,
    'import_period' => '-1',
    'expire_period' => 3600,
    'import_on_create' => 0,
  );
  $export['books_feed'] = $feeds_importer;

  $feeds_importer = new stdClass();
  $feeds_importer->disabled = FALSE; /* Edit this to true to make a default feeds_importer disabled initially */
  $feeds_importer->api_version = 1;
  $feeds_importer->id = 'dlio_pre_importer_authors';
  $feeds_importer->config = array(
    'name' => 'Agris AP pre-importer: authors',
    'description' => 'This importer imports author records from the DLIO feed into the Authors reference list. Use it before importing DLIOs',
    'fetcher' => array(
      'plugin_key' => 'FeedsHTTPFetcher',
      'config' => array(
        'auto_detect_feeds' => FALSE,
        'use_pubsubhubbub' => FALSE,
        'designated_hub' => '',
      ),
    ),
    'parser' => array(
      'plugin_key' => 'FeedsXPathParserXML',
      'config' => array(
        'sources' => array(
          'xpathparser:2' => 'text()',
          'xpathparser:3' => 'text()',
          'xpathparser:4' => 'text()',
          'xpathparser:5' => 'text()',
          'xpathparser:6' => 'text()',
        ),
        'rawXML' => array(
          'xpathparser:2' => 0,
          'xpathparser:3' => 0,
          'xpathparser:4' => 0,
          'xpathparser:5' => 0,
          'xpathparser:6' => 0,
        ),
        'context' => '/ags:resources/ags:resource//ags:creatorPersonal',
        'exp' => array(
          'errors' => 1,
          'tidy' => 0,
          'tidy_encoding' => '',
          'debug' => array(
            'context' => 0,
            'xpathparser:2' => 0,
            'xpathparser:3' => 0,
            'xpathparser:4' => 0,
            'xpathparser:5' => 0,
            'xpathparser:6' => 0,
          ),
        ),
        'allow_override' => 1,
      ),
    ),
    'processor' => array(
      'plugin_key' => 'FeedsNodeProcessor',
      'config' => array(
        'input_format' => '2',
        'update_existing' => '0',
        'expire' => '-1',
        'mappings' => array(
          0 => array(
            'source' => 'xpathparser:2',
            'target' => 'guid',
            'unique' => 1,
          ),
          1 => array(
            'source' => 'xpathparser:3',
            'target' => 'field_ag_lastname',
            'unique' => FALSE,
          ),
          2 => array(
            'source' => 'xpathparser:4',
            'target' => 'title',
            'unique' => FALSE,
          ),
          3 => array(
            'source' => 'xpathparser:5',
            'target' => 'field_ag_name',
            'unique' => FALSE,
          ),
          4 => array(
            'source' => 'xpathparser:6',
            'target' => 'field_ag_affiliation',
            'unique' => FALSE,
          ),
        ),
        'author' => '1',
        'bundle' => 'agauthor',
      ),
    ),
    'content_type' => 'agris_authors_feed',
    'update' => 0,
    'import_period' => '-1',
    'expire_period' => 3600,
    'import_on_create' => 0,
    'process_in_background' => FALSE,
  );
  $export['dlio_pre_importer_authors'] = $feeds_importer;

  $feeds_importer = new stdClass();
  $feeds_importer->disabled = FALSE; /* Edit this to true to make a default feeds_importer disabled initially */
  $feeds_importer->api_version = 1;
  $feeds_importer->id = 'event_feed_importer';
  $feeds_importer->config = array(
    'name' => 'Event Feed Importer',
    'description' => 'To import feeds using the Event AP metadata set',
    'fetcher' => array(
      'plugin_key' => 'FeedsHTTPFetcher',
      'config' => array(
        'auto_detect_feeds' => FALSE,
        'use_pubsubhubbub' => FALSE,
        'designated_hub' => '',
      ),
    ),
    'parser' => array(
      'plugin_key' => 'FeedsXPathParserXML',
      'config' => array(
        'sources' => array(
          'xpathparser:0' => 'title',
          'xpathparser:1' => 'description',
          'xpathparser:2' => '//ags:locationCity',
          'xpathparser:3' => '//ags:locationCountry',
          'xpathparser:4' => '//ags:dateStart',
          'xpathparser:5' => '//ags:dateEnd',
          'xpathparser:6' => 'link',
          'xpathparser:7' => '',
          'xpathparser:8' => 'guid',
        ),
        'rawXML' => array(
          'xpathparser:0' => 0,
          'xpathparser:1' => 0,
          'xpathparser:2' => 0,
          'xpathparser:3' => 0,
          'xpathparser:4' => 0,
          'xpathparser:5' => 0,
          'xpathparser:6' => 0,
          'xpathparser:7' => 0,
          'xpathparser:8' => 0,
        ),
        'context' => '/rss/channel/item',
        'exp' => array(
          'errors' => 0,
          'debug' => array(
            'context' => 0,
            'xpathparser:0' => 0,
            'xpathparser:1' => 0,
            'xpathparser:2' => 0,
            'xpathparser:3' => 0,
            'xpathparser:4' => 0,
            'xpathparser:5' => 0,
            'xpathparser:6' => 0,
            'xpathparser:7' => 0,
            'xpathparser:8' => 0,
          ),
        ),
      ),
    ),
    'processor' => array(
      'plugin_key' => 'FeedsNodeProcessor',
      'config' => array(
        'input_format' => '1',
        'update_existing' => '1',
        'expire' => '-1',
        'mappings' => array(
          0 => array(
            'source' => 'xpathparser:0',
            'target' => 'title',
            'unique' => FALSE,
          ),
          1 => array(
            'source' => 'xpathparser:1',
            'target' => 'body',
            'unique' => FALSE,
          ),
          2 => array(
            'source' => 'xpathparser:2',
            'target' => 'field_ag_location',
            'unique' => FALSE,
          ),
          3 => array(
            'source' => 'xpathparser:3',
            'target' => 'field_ag_country',
            'unique' => FALSE,
          ),
          4 => array(
            'source' => 'xpathparser:4',
            'target' => 'field_ag_date:start',
            'unique' => FALSE,
          ),
          5 => array(
            'source' => 'xpathparser:5',
            'target' => 'field_ag_date:end',
            'unique' => FALSE,
          ),
          6 => array(
            'source' => 'xpathparser:6',
            'target' => 'field_ag_source_url',
            'unique' => FALSE,
          ),
          7 => array(
            'source' => 'xpathparser:8',
            'target' => 'guid',
            'unique' => 1,
          ),
        ),
        'author' => 0,
        'authorize' => 0,
        'bundle' => 'agrssevents',
      ),
    ),
    'content_type' => 'ageventsfeed',
    'update' => 0,
    'import_period' => '10800',
    'expire_period' => 3600,
    'import_on_create' => 0,
  );
  $export['event_feed_importer'] = $feeds_importer;

  $feeds_importer = new stdClass();
  $feeds_importer->disabled = FALSE; /* Edit this to true to make a default feeds_importer disabled initially */
  $feeds_importer->api_version = 1;
  $feeds_importer->id = 'job_feed_importer';
  $feeds_importer->config = array(
    'name' => 'Job Feed Importer',
    'description' => 'Parses job feeds with elements from the Ag-Job AP',
    'fetcher' => array(
      'plugin_key' => 'FeedsHTTPFetcher',
      'config' => array(
        'auto_detect_feeds' => FALSE,
        'use_pubsubhubbub' => FALSE,
        'designated_hub' => '',
      ),
    ),
    'parser' => array(
      'plugin_key' => 'FeedsXPathParserXML',
      'config' => array(
        'sources' => array(
          'xpathparser:0' => 'title',
          'xpathparser:1' => 'description',
          'xpathparser:2' => '//ags:locationCity',
          'xpathparser:3' => '//ags:locationCountry',
          'xpathparser:6' => 'link',
          'xpathparser:7' => '',
          'xpathparser:8' => '//ags:dateEnd',
          'xpathparser:9' => 'guid',
        ),
        'rawXML' => array(
          'xpathparser:0' => 0,
          'xpathparser:1' => 0,
          'xpathparser:2' => 0,
          'xpathparser:3' => 0,
          'xpathparser:6' => 0,
          'xpathparser:7' => 0,
          'xpathparser:8' => 0,
          'xpathparser:9' => 0,
        ),
        'context' => '/rss/channel/item',
        'exp' => array(
          'errors' => 0,
          'debug' => array(
            'context' => 0,
            'xpathparser:0' => 0,
            'xpathparser:1' => 0,
            'xpathparser:2' => 0,
            'xpathparser:3' => 0,
            'xpathparser:6' => 0,
            'xpathparser:7' => 0,
            'xpathparser:8' => 0,
            'xpathparser:9' => 0,
          ),
        ),
      ),
    ),
    'processor' => array(
      'plugin_key' => 'FeedsNodeProcessor',
      'config' => array(
        'input_format' => '1',
        'update_existing' => '1',
        'expire' => '-1',
        'mappings' => array(
          0 => array(
            'source' => 'xpathparser:0',
            'target' => 'title',
            'unique' => FALSE,
          ),
          1 => array(
            'source' => 'xpathparser:1',
            'target' => 'body',
            'unique' => FALSE,
          ),
          2 => array(
            'source' => 'xpathparser:2',
            'target' => 'field_ag_location',
            'unique' => FALSE,
          ),
          3 => array(
            'source' => 'xpathparser:3',
            'target' => 'field_ag_country',
            'unique' => FALSE,
          ),
          4 => array(
            'source' => 'xpathparser:6',
            'target' => 'field_ag_source_url',
            'unique' => FALSE,
          ),
          5 => array(
            'source' => 'xpathparser:8',
            'target' => 'field_ag_deadline:start',
            'unique' => FALSE,
          ),
          6 => array(
            'source' => 'xpathparser:9',
            'target' => 'guid',
            'unique' => 1,
          ),
        ),
        'author' => 0,
        'authorize' => 0,
        'bundle' => 'agrssjob',
      ),
    ),
    'content_type' => 'ag_job_feed',
    'update' => 0,
    'import_period' => '10800',
    'expire_period' => 3600,
    'import_on_create' => 0,
  );
  $export['job_feed_importer'] = $feeds_importer;

  return $export;
}
