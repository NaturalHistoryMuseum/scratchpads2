<?php

/**
 * @file
 * Contains Views hooks to register Views plugins and handlers.
 */

/**
 * Implements of hook_views_plugins().
 */
function apachesolr_views_views_plugins() {
  return array(
    'module' => 'apachesolr_views',
    'query' => array(
      'apachesolr_views_query' => array(
        'title' => t('Apache Solr Query'),
        'help' => t('Query that allows you to search with Apache Solr.'),
        'handler' => 'apachesolr_views_query',
        'parent' => 'views_query',
      ),
    ),
  );

}

/**
 * Implements of hook_views_data().
 */
function apachesolr_views_views_data() {
  foreach (apachesolr_load_all_environments() as $env_id => $environment) {
    $name = $environment['name'];
    $apachesolr_base_table = 'apachesolr__' . $env_id;

    $data[$apachesolr_base_table]['table']['group'] = t('Apache Solr');
    $data[$apachesolr_base_table]['table']['base'] = array(
      'query class' => 'apachesolr_views_query',
      'title' => t('Apache Solr @name', array('@name' => $name)),
      'help' => t('Searches the site with the Apache Solr search engine for @name', array('@name' => $name)),
    );

    // Get the list of the fields in index directly from Solr.
    try {
      $solr = apachesolr_get_solr($env_id);
      $solr_fields = $solr->getFields(0);
    }
    catch (Exception $e) {
      $solr_fields = array();
      watchdog('Apache Solr Views', nl2br(check_plain($e->getMessage())), NULL, WATCHDOG_WARNING);
    }

    foreach ($solr_fields as $solr_field_name => $solr_field) {

      // We do not allow to display 'sort_*' fields.
      if (strpos($solr_field_name, 'sort_') === 0) {
        continue;
      }

      $field_type = $solr_field->type;

      $field_handler = 'apachesolr_views_handler_field';
      $filter_handler = 'apachesolr_views_handler_filter';
      switch ($field_type) {
        case 'tdate':
          $field_handler = 'apachesolr_views_handler_field_date';
          $filter_handler = 'apachesolr_views_handler_filter_date';
          break;

        case 'text':
        case 'string':
          $filter_handler = 'apachesolr_views_handler_filter_string';
          break;
      }

      $data[$apachesolr_base_table][$solr_field_name] = array(
        'title' => $solr_field_name,
        'help' => filter_xss(apachesolr_field_name_map($solr_field_name)),
        'field' => array(
          'handler' => $field_handler,
          'click sortable' => TRUE,
        ),
        'filter' => array(
          'handler' => $filter_handler,
        ),
        'sort' => array(
          'handler' => 'apachesolr_views_handler_sort',
        ),
        'argument' => array(
          'handler' => 'apachesolr_views_handler_argument',
        ),
      );

      // Default sort field for label.
      $sort_field_name = ($solr_field_name == 'label') ? 'sort_label' : '';
      // Check if corresponding sort_ field exists. We remove prefix from field
      // name (for example prefix "ss_" from "ss_name") and check if "sort_*"
      // field is available.
      if (array_key_exists('sort_' . substr($solr_field_name, 2), $solr_fields)) {
        $sort_field_name = 'sort_' . substr($solr_field_name, 2);
      }
      if (!empty($sort_field_name)) {
        // Use the sort field for click sorting.
        $data[$apachesolr_base_table][$solr_field_name]['field']['click sort field'] = $sort_field_name;
        // And use the sort field for explicit sorts.
        $data[$apachesolr_base_table][$solr_field_name]['sort']['real field'] = $sort_field_name;
      }
    }

    // Keyword field.
    $data[$apachesolr_base_table]['keyword'] = array(
      'title' => t('Search'),
      'help' => t('Fulltext search'),
      'filter' => array(
        'handler' => 'apachesolr_views_keyword_handler_filter',
      ),
    );

    // Snippet field.
    $data[$apachesolr_base_table]['snippet'] = array(
      'title' => t('Snippet'),
      'help' => t('Search snippet'),
      'field' => array(
        'handler' => 'apachesolr_views_snippet_handler_field',
        'click sortable' => TRUE,
      ),
    );

    // Score field.
    $data[$apachesolr_base_table]['score'] = array(
      'title' => t('Score'),
      'help' => t('Score'),
      'field' => array(
        'handler' => 'apachesolr_views_handler_field',
        'click sortable' => TRUE,
      ),
      'sort' => array(
        'handler' => 'apachesolr_views_handler_sort',
      ),
    );
  }

  return $data;
}
