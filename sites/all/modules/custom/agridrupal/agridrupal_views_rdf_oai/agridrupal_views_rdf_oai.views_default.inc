<?php
/**
 * @file
 * agridrupal_views_rdf_oai.views_default.inc
 */

/**
 * Implements hook_views_default_views().
 */
function agridrupal_views_rdf_oai_views_default_views() {
  $export = array();

  $view = new view();
  $view->name = 'Ag_Documents';
  $view->description = '';
  $view->tag = '';
  $view->base_table = 'node';
  $view->human_name = '';
  $view->core = 0;
  $view->api_version = '3.0';
  $view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */

  /* Display: Documents: list and search */
  $handler = $view->new_display('default', 'Documents: list and search', 'default');
  $handler->display->display_options['title'] = 'Documents';
  $handler->display->display_options['use_more_always'] = FALSE;
  $handler->display->display_options['access']['type'] = 'none';
  $handler->display->display_options['cache']['type'] = 'none';
  $handler->display->display_options['query']['type'] = 'views_query';
  $handler->display->display_options['query']['options']['distinct'] = TRUE;
  $handler->display->display_options['exposed_form']['type'] = 'basic';
  $handler->display->display_options['exposed_form']['options']['submit_button'] = 'Search';
  $handler->display->display_options['exposed_form']['options']['reset_button'] = TRUE;
  $handler->display->display_options['pager']['type'] = 'full';
  $handler->display->display_options['style_plugin'] = 'table';
  $handler->display->display_options['style_options']['columns'] = array(
    'title' => 'title',
    'field_ag_type' => 'field_ag_type',
    'field_ag_journal_issue' => 'field_ag_journal_issue',
    'field_ag_confname' => 'field_ag_confname',
    'field_ag_author' => 'field_ag_author',
    'field_ag_corauthor' => 'field_ag_author',
    'field_ag_datepub' => 'field_ag_datepub',
  );
  $handler->display->display_options['style_options']['default'] = 'title';
  $handler->display->display_options['style_options']['info'] = array(
    'title' => array(
      'sortable' => 1,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '<br />',
      'empty_column' => 0,
    ),
    'field_ag_type' => array(
      'sortable' => 0,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'field_ag_journal_issue' => array(
      'sortable' => 0,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'field_ag_confname' => array(
      'sortable' => 0,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'field_ag_author' => array(
      'align' => '',
      'separator' => '<br />',
      'empty_column' => 0,
    ),
    'field_ag_corauthor' => array(
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'field_ag_datepub' => array(
      'sortable' => 1,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
  );
  /* Header: Global: Text area */
  $handler->display->display_options['header']['text']['id'] = 'area';
  $handler->display->display_options['header']['text']['table'] = 'views';
  $handler->display->display_options['header']['text']['field'] = 'area';
  $handler->display->display_options['header']['text']['content'] = '<?php
global $base_url;
?>
<div align="right"><a href="<?php echo $base_url;?>/ag_documents/rss" target="_blank"><img src="<?php echo $base_url;?>/sites/default/files/images/RSS_feed.png"> RSS feed</a><br /><i>[compliant with the Agris AP]</i><br />
<a href="<?php echo $base_url;?>/ag_documents/rdf" target="_blank"><img src="<?php echo $base_url;?>/sites/default/files/images/rdf-icon.gif"> RDF feed</a><br />
<a href="<?php echo $base_url;?>/ag_dlios-xml" target="_blank"><img src="<?php echo $base_url;?>/sites/default/files/images/xml.png"> Agris AP XML</a></div>
By default records are diaplayed in alphabetical order by title.
Clicking on a column header will sort records according to that header.';
  $handler->display->display_options['header']['text']['format'] = '3';
  /* Footer: Global: Text area */
  $handler->display->display_options['footer']['text']['id'] = 'area';
  $handler->display->display_options['footer']['text']['table'] = 'views';
  $handler->display->display_options['footer']['text']['field'] = 'area';
  $handler->display->display_options['footer']['text']['empty'] = TRUE;
  $handler->display->display_options['footer']['text']['content'] = '<?php
    $view = views_get_current_view();
    if ($view->total_rows > 0)
      print \'<hr /><center><div class="view-counter">Total: \' . $view->total_rows . \'</div></center>\';
?>
<script language="javascript">
  selects = document.getElementsByTagName("select");
  for (i=0; i<selects.length; i++) {
    if (selects[i].size > 4)
      selects[i].size = 4;
  }
</script>';
  $handler->display->display_options['footer']['text']['format'] = '3';
  /* Relationship: Broken/missing handler */
  $handler->display->display_options['relationships']['field_test_value']['id'] = 'field_test_value';
  $handler->display->display_options['relationships']['field_test_value']['table'] = 'field_data_field_test';
  $handler->display->display_options['relationships']['field_test_value']['field'] = 'field_test_value';
  /* Field: Content: Title */
  $handler->display->display_options['fields']['title']['id'] = 'title';
  $handler->display->display_options['fields']['title']['table'] = 'node';
  $handler->display->display_options['fields']['title']['field'] = 'title';
  /* Field: Content: Title */
  $handler->display->display_options['fields']['title_1']['id'] = 'title_1';
  $handler->display->display_options['fields']['title_1']['table'] = 'node';
  $handler->display->display_options['fields']['title_1']['field'] = 'title';
  $handler->display->display_options['fields']['title_1']['label'] = '';
  $handler->display->display_options['fields']['title_1']['exclude'] = TRUE;
  $handler->display->display_options['fields']['title_1']['link_to_node'] = FALSE;
  /* Sort criterion: Content: Title */
  $handler->display->display_options['sorts']['title']['id'] = 'title';
  $handler->display->display_options['sorts']['title']['table'] = 'node';
  $handler->display->display_options['sorts']['title']['field'] = 'title';
  $handler->display->display_options['filter_groups']['operator'] = 'OR';
  $handler->display->display_options['filter_groups']['groups'] = array(
    1 => 'AND',
    2 => 'AND',
  );
  /* Filter criterion: Content: Published */
  $handler->display->display_options['filters']['status']['id'] = 'status';
  $handler->display->display_options['filters']['status']['table'] = 'node';
  $handler->display->display_options['filters']['status']['field'] = 'status';
  $handler->display->display_options['filters']['status']['value'] = '1';
  $handler->display->display_options['filters']['status']['group'] = 1;
  $handler->display->display_options['filters']['status']['expose']['operator'] = FALSE;
  /* Filter criterion: Content: Type */
  $handler->display->display_options['filters']['type']['id'] = 'type';
  $handler->display->display_options['filters']['type']['table'] = 'node';
  $handler->display->display_options['filters']['type']['field'] = 'type';
  $handler->display->display_options['filters']['type']['value'] = array(
    'agdlios' => 'agdlios',
  );
  $handler->display->display_options['filters']['type']['group'] = 1;
  $handler->display->display_options['filters']['type']['expose']['operator'] = FALSE;
  /* Filter criterion: Content: Title */
  $handler->display->display_options['filters']['title']['id'] = 'title';
  $handler->display->display_options['filters']['title']['table'] = 'node';
  $handler->display->display_options['filters']['title']['field'] = 'title';
  $handler->display->display_options['filters']['title']['operator'] = 'contains';
  $handler->display->display_options['filters']['title']['group'] = 1;
  $handler->display->display_options['filters']['title']['exposed'] = TRUE;
  $handler->display->display_options['filters']['title']['expose']['operator_id'] = 'title_op';
  $handler->display->display_options['filters']['title']['expose']['label'] = 'Title';
  $handler->display->display_options['filters']['title']['expose']['operator'] = 'title_op';
  $handler->display->display_options['filters']['title']['expose']['identifier'] = 'title';

  /* Display: Documents: advanced search */
  $handler = $view->new_display('page', 'Documents: advanced search', 'page_1');
  $handler->display->display_options['defaults']['title'] = FALSE;
  $handler->display->display_options['title'] = 'Documents: advanced search';
  $handler->display->display_options['defaults']['pager'] = FALSE;
  $handler->display->display_options['pager']['type'] = 'full';
  $handler->display->display_options['pager']['options']['items_per_page'] = '25';
  $handler->display->display_options['pager']['options']['offset'] = '0';
  $handler->display->display_options['pager']['options']['id'] = '0';
  $handler->display->display_options['pager']['options']['quantity'] = '9';
  $handler->display->display_options['defaults']['header'] = FALSE;
  /* Header: Global: Text area */
  $handler->display->display_options['header']['text']['id'] = 'area';
  $handler->display->display_options['header']['text']['table'] = 'views';
  $handler->display->display_options['header']['text']['field'] = 'area';
  $handler->display->display_options['header']['text']['content'] = 'By default records are displayed in alphabetical order by title.
Clicking on a column header will sort records according to that header.';
  $handler->display->display_options['header']['text']['format'] = '1';
  $handler->display->display_options['defaults']['relationships'] = FALSE;
  /* Relationship: Content: Agrovoc terms (field_ag_agrovoc) */
  $handler->display->display_options['relationships']['field_ag_agrovoc_tid']['id'] = 'field_ag_agrovoc_tid';
  $handler->display->display_options['relationships']['field_ag_agrovoc_tid']['table'] = 'field_data_field_ag_agrovoc';
  $handler->display->display_options['relationships']['field_ag_agrovoc_tid']['field'] = 'field_ag_agrovoc_tid';
  /* Relationship: Content: Agris Subject Categories (field_ag_agris) */
  $handler->display->display_options['relationships']['field_ag_agris_tid']['id'] = 'field_ag_agris_tid';
  $handler->display->display_options['relationships']['field_ag_agris_tid']['table'] = 'field_data_field_ag_agris';
  $handler->display->display_options['relationships']['field_ag_agris_tid']['field'] = 'field_ag_agris_tid';
  $handler->display->display_options['defaults']['fields'] = FALSE;
  /* Field: Content: Title */
  $handler->display->display_options['fields']['title']['id'] = 'title';
  $handler->display->display_options['fields']['title']['table'] = 'node';
  $handler->display->display_options['fields']['title']['field'] = 'title';
  /* Field: Content: Document type */
  $handler->display->display_options['fields']['field_ag_type']['id'] = 'field_ag_type';
  $handler->display->display_options['fields']['field_ag_type']['table'] = 'field_data_field_ag_type';
  $handler->display->display_options['fields']['field_ag_type']['field'] = 'field_ag_type';
  $handler->display->display_options['fields']['field_ag_type']['label'] = '';
  $handler->display->display_options['fields']['field_ag_type']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['field_ag_type']['type'] = 'taxonomy_term_reference_plain';
  /* Field: Content: Journal issue */
  $handler->display->display_options['fields']['field_ag_journal_issue']['id'] = 'field_ag_journal_issue';
  $handler->display->display_options['fields']['field_ag_journal_issue']['table'] = 'field_data_field_ag_journal_issue';
  $handler->display->display_options['fields']['field_ag_journal_issue']['field'] = 'field_ag_journal_issue';
  $handler->display->display_options['fields']['field_ag_journal_issue']['label'] = '';
  $handler->display->display_options['fields']['field_ag_journal_issue']['element_label_colon'] = FALSE;
  /* Field: Content: Conference */
  $handler->display->display_options['fields']['field_ag_confname']['id'] = 'field_ag_confname';
  $handler->display->display_options['fields']['field_ag_confname']['table'] = 'field_data_field_ag_confname';
  $handler->display->display_options['fields']['field_ag_confname']['field'] = 'field_ag_confname';
  $handler->display->display_options['fields']['field_ag_confname']['label'] = '';
  $handler->display->display_options['fields']['field_ag_confname']['element_label_colon'] = FALSE;
  /* Field: Content: Author(s) */
  $handler->display->display_options['fields']['field_ag_author']['id'] = 'field_ag_author';
  $handler->display->display_options['fields']['field_ag_author']['table'] = 'field_data_field_ag_author';
  $handler->display->display_options['fields']['field_ag_author']['field'] = 'field_ag_author';
  $handler->display->display_options['fields']['field_ag_author']['label'] = 'Author';
  $handler->display->display_options['fields']['field_ag_author']['delta_offset'] = '0';
  /* Field: Content: Corporate author(s) */
  $handler->display->display_options['fields']['field_ag_corauthor']['id'] = 'field_ag_corauthor';
  $handler->display->display_options['fields']['field_ag_corauthor']['table'] = 'field_data_field_ag_corauthor';
  $handler->display->display_options['fields']['field_ag_corauthor']['field'] = 'field_ag_corauthor';
  $handler->display->display_options['fields']['field_ag_corauthor']['label'] = '';
  $handler->display->display_options['fields']['field_ag_corauthor']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['field_ag_corauthor']['delta_offset'] = '0';
  /* Field: Content: Date of publication */
  $handler->display->display_options['fields']['field_ag_datepub']['id'] = 'field_ag_datepub';
  $handler->display->display_options['fields']['field_ag_datepub']['table'] = 'field_data_field_ag_datepub';
  $handler->display->display_options['fields']['field_ag_datepub']['field'] = 'field_ag_datepub';
  $handler->display->display_options['fields']['field_ag_datepub']['settings'] = array(
    'format_type' => 'medium',
    'fromto' => 'both',
    'multiple_number' => '',
    'multiple_from' => '',
    'multiple_to' => '',
  );
  $handler->display->display_options['defaults']['filter_groups'] = FALSE;
  $handler->display->display_options['filter_groups']['operator'] = 'OR';
  $handler->display->display_options['defaults']['filters'] = FALSE;
  /* Filter criterion: Content: Published */
  $handler->display->display_options['filters']['status']['id'] = 'status';
  $handler->display->display_options['filters']['status']['table'] = 'node';
  $handler->display->display_options['filters']['status']['field'] = 'status';
  $handler->display->display_options['filters']['status']['value'] = '1';
  $handler->display->display_options['filters']['status']['group'] = 1;
  $handler->display->display_options['filters']['status']['expose']['operator'] = FALSE;
  /* Filter criterion: Content: Type */
  $handler->display->display_options['filters']['type']['id'] = 'type';
  $handler->display->display_options['filters']['type']['table'] = 'node';
  $handler->display->display_options['filters']['type']['field'] = 'type';
  $handler->display->display_options['filters']['type']['value'] = array(
    'agdlios' => 'agdlios',
  );
  $handler->display->display_options['filters']['type']['group'] = 1;
  $handler->display->display_options['filters']['type']['expose']['operator_id'] = 'type_op';
  $handler->display->display_options['filters']['type']['expose']['label'] = 'Type';
  $handler->display->display_options['filters']['type']['expose']['operator'] = 'type_op';
  $handler->display->display_options['filters']['type']['expose']['identifier'] = 'type';
  /* Filter criterion: Content: Language */
  $handler->display->display_options['filters']['language']['id'] = 'language';
  $handler->display->display_options['filters']['language']['table'] = 'node';
  $handler->display->display_options['filters']['language']['field'] = 'language';
  $handler->display->display_options['filters']['language']['value'] = array(
    '***CURRENT_LANGUAGE***' => '***CURRENT_LANGUAGE***',
    'und' => 'und',
  );
  $handler->display->display_options['filters']['language']['group'] = 1;
  $handler->display->display_options['filters']['language']['expose']['operator'] = FALSE;
  /* Filter criterion: Search: Search Terms */
  $handler->display->display_options['filters']['keys']['id'] = 'keys';
  $handler->display->display_options['filters']['keys']['table'] = 'search_index';
  $handler->display->display_options['filters']['keys']['field'] = 'keys';
  $handler->display->display_options['filters']['keys']['group'] = 1;
  $handler->display->display_options['filters']['keys']['exposed'] = TRUE;
  $handler->display->display_options['filters']['keys']['expose']['operator_id'] = 'keys_op';
  $handler->display->display_options['filters']['keys']['expose']['label'] = 'Free search in all fields';
  $handler->display->display_options['filters']['keys']['expose']['operator'] = 'keys_op';
  $handler->display->display_options['filters']['keys']['expose']['identifier'] = 'keys';
  /* Filter criterion: Content: Title */
  $handler->display->display_options['filters']['title']['id'] = 'title';
  $handler->display->display_options['filters']['title']['table'] = 'node';
  $handler->display->display_options['filters']['title']['field'] = 'title';
  $handler->display->display_options['filters']['title']['operator'] = 'contains';
  $handler->display->display_options['filters']['title']['group'] = 1;
  $handler->display->display_options['filters']['title']['exposed'] = TRUE;
  $handler->display->display_options['filters']['title']['expose']['operator_id'] = 'title_op';
  $handler->display->display_options['filters']['title']['expose']['label'] = 'Word(s) in title';
  $handler->display->display_options['filters']['title']['expose']['operator'] = 'title_op';
  $handler->display->display_options['filters']['title']['expose']['identifier'] = 'title';
  /* Filter criterion: Content: Document type (field_ag_type) */
  $handler->display->display_options['filters']['field_ag_type_tid']['id'] = 'field_ag_type_tid';
  $handler->display->display_options['filters']['field_ag_type_tid']['table'] = 'field_data_field_ag_type';
  $handler->display->display_options['filters']['field_ag_type_tid']['field'] = 'field_ag_type_tid';
  $handler->display->display_options['filters']['field_ag_type_tid']['group'] = 1;
  $handler->display->display_options['filters']['field_ag_type_tid']['exposed'] = TRUE;
  $handler->display->display_options['filters']['field_ag_type_tid']['expose']['operator_id'] = 'field_ag_type_tid_op';
  $handler->display->display_options['filters']['field_ag_type_tid']['expose']['label'] = 'Document type';
  $handler->display->display_options['filters']['field_ag_type_tid']['expose']['operator'] = 'field_ag_type_tid_op';
  $handler->display->display_options['filters']['field_ag_type_tid']['expose']['identifier'] = 'field_ag_type_tid';
  $handler->display->display_options['filters']['field_ag_type_tid']['expose']['remember_roles'] = array(
    2 => '2',
    3 => 0,
    1 => 0,
    6 => 0,
    7 => 0,
    4 => 0,
    5 => 0,
  );
  $handler->display->display_options['filters']['field_ag_type_tid']['reduce_duplicates'] = TRUE;
  $handler->display->display_options['filters']['field_ag_type_tid']['type'] = 'select';
  $handler->display->display_options['filters']['field_ag_type_tid']['vocabulary'] = 'vocabulary_1';
  $handler->display->display_options['filters']['field_ag_type_tid']['hierarchy'] = 1;
  /* Filter criterion: Content: Author(s) (field_ag_author) */
  $handler->display->display_options['filters']['field_ag_author_nid']['id'] = 'field_ag_author_nid';
  $handler->display->display_options['filters']['field_ag_author_nid']['table'] = 'field_data_field_ag_author';
  $handler->display->display_options['filters']['field_ag_author_nid']['field'] = 'field_ag_author_nid';
  $handler->display->display_options['filters']['field_ag_author_nid']['relationship'] = 'field_test_value';
  $handler->display->display_options['filters']['field_ag_author_nid']['group'] = 1;
  $handler->display->display_options['filters']['field_ag_author_nid']['exposed'] = TRUE;
  $handler->display->display_options['filters']['field_ag_author_nid']['expose']['operator_id'] = 'field_ag_author_nid_op';
  $handler->display->display_options['filters']['field_ag_author_nid']['expose']['label'] = 'Author ';
  $handler->display->display_options['filters']['field_ag_author_nid']['expose']['operator'] = 'field_ag_author_nid_op';
  $handler->display->display_options['filters']['field_ag_author_nid']['expose']['identifier'] = 'field_ag_author_nid';
  /* Filter criterion: Content: Journal (field_ag_journal) */
  $handler->display->display_options['filters']['field_ag_journal_nid']['id'] = 'field_ag_journal_nid';
  $handler->display->display_options['filters']['field_ag_journal_nid']['table'] = 'field_data_field_ag_journal';
  $handler->display->display_options['filters']['field_ag_journal_nid']['field'] = 'field_ag_journal_nid';
  $handler->display->display_options['filters']['field_ag_journal_nid']['group'] = 1;
  $handler->display->display_options['filters']['field_ag_journal_nid']['exposed'] = TRUE;
  $handler->display->display_options['filters']['field_ag_journal_nid']['expose']['operator_id'] = 'field_ag_journal_nid_op';
  $handler->display->display_options['filters']['field_ag_journal_nid']['expose']['label'] = 'Journal';
  $handler->display->display_options['filters']['field_ag_journal_nid']['expose']['operator'] = 'field_ag_journal_nid_op';
  $handler->display->display_options['filters']['field_ag_journal_nid']['expose']['identifier'] = 'field_ag_journal_nid';
  $handler->display->display_options['filters']['field_ag_journal_nid']['expose']['remember_roles'] = array(
    2 => '2',
    3 => 0,
    1 => 0,
    6 => 0,
    7 => 0,
    4 => 0,
    5 => 0,
  );
  /* Filter criterion: Content: Conference (field_ag_confname) */
  $handler->display->display_options['filters']['field_ag_confname_nid']['id'] = 'field_ag_confname_nid';
  $handler->display->display_options['filters']['field_ag_confname_nid']['table'] = 'field_data_field_ag_confname';
  $handler->display->display_options['filters']['field_ag_confname_nid']['field'] = 'field_ag_confname_nid';
  $handler->display->display_options['filters']['field_ag_confname_nid']['group'] = 1;
  $handler->display->display_options['filters']['field_ag_confname_nid']['exposed'] = TRUE;
  $handler->display->display_options['filters']['field_ag_confname_nid']['expose']['operator_id'] = 'field_ag_confname_nid_op';
  $handler->display->display_options['filters']['field_ag_confname_nid']['expose']['label'] = 'Conference';
  $handler->display->display_options['filters']['field_ag_confname_nid']['expose']['operator'] = 'field_ag_confname_nid_op';
  $handler->display->display_options['filters']['field_ag_confname_nid']['expose']['identifier'] = 'field_ag_confname_nid';
  /* Filter criterion: Content: Access restriction (field_ag_acces) */
  $handler->display->display_options['filters']['field_ag_acces_tid']['id'] = 'field_ag_acces_tid';
  $handler->display->display_options['filters']['field_ag_acces_tid']['table'] = 'field_data_field_ag_acces';
  $handler->display->display_options['filters']['field_ag_acces_tid']['field'] = 'field_ag_acces_tid';
  $handler->display->display_options['filters']['field_ag_acces_tid']['group'] = 1;
  $handler->display->display_options['filters']['field_ag_acces_tid']['exposed'] = TRUE;
  $handler->display->display_options['filters']['field_ag_acces_tid']['expose']['operator_id'] = 'field_ag_acces_tid_op';
  $handler->display->display_options['filters']['field_ag_acces_tid']['expose']['label'] = 'Access';
  $handler->display->display_options['filters']['field_ag_acces_tid']['expose']['operator'] = 'field_ag_acces_tid_op';
  $handler->display->display_options['filters']['field_ag_acces_tid']['expose']['identifier'] = 'field_ag_acces_tid';
  $handler->display->display_options['filters']['field_ag_acces_tid']['expose']['remember_roles'] = array(
    2 => '2',
    3 => 0,
    1 => 0,
    6 => 0,
    7 => 0,
    4 => 0,
    5 => 0,
  );
  $handler->display->display_options['filters']['field_ag_acces_tid']['reduce_duplicates'] = TRUE;
  $handler->display->display_options['filters']['field_ag_acces_tid']['type'] = 'select';
  $handler->display->display_options['filters']['field_ag_acces_tid']['vocabulary'] = 'vocabulary_5';
  /* Filter criterion: Content: Agrovoc terms (field_ag_agrovoc) */
  $handler->display->display_options['filters']['field_ag_agrovoc_tid']['id'] = 'field_ag_agrovoc_tid';
  $handler->display->display_options['filters']['field_ag_agrovoc_tid']['table'] = 'field_data_field_ag_agrovoc';
  $handler->display->display_options['filters']['field_ag_agrovoc_tid']['field'] = 'field_ag_agrovoc_tid';
  $handler->display->display_options['filters']['field_ag_agrovoc_tid']['value'] = '';
  $handler->display->display_options['filters']['field_ag_agrovoc_tid']['group'] = 1;
  $handler->display->display_options['filters']['field_ag_agrovoc_tid']['exposed'] = TRUE;
  $handler->display->display_options['filters']['field_ag_agrovoc_tid']['expose']['operator_id'] = 'field_ag_agrovoc_tid_op';
  $handler->display->display_options['filters']['field_ag_agrovoc_tid']['expose']['label'] = 'Topic (Agrovoc)';
  $handler->display->display_options['filters']['field_ag_agrovoc_tid']['expose']['operator'] = 'field_ag_agrovoc_tid_op';
  $handler->display->display_options['filters']['field_ag_agrovoc_tid']['expose']['identifier'] = 'field_ag_agrovoc_tid';
  $handler->display->display_options['filters']['field_ag_agrovoc_tid']['expose']['remember_roles'] = array(
    2 => '2',
    3 => 0,
    1 => 0,
    6 => 0,
    7 => 0,
    4 => 0,
    5 => 0,
  );
  $handler->display->display_options['filters']['field_ag_agrovoc_tid']['reduce_duplicates'] = TRUE;
  $handler->display->display_options['filters']['field_ag_agrovoc_tid']['vocabulary'] = 'vocabulary_16';
  /* Filter criterion: Content: Agris Subject Categories (field_ag_agris) */
  $handler->display->display_options['filters']['field_ag_agris_tid']['id'] = 'field_ag_agris_tid';
  $handler->display->display_options['filters']['field_ag_agris_tid']['table'] = 'field_data_field_ag_agris';
  $handler->display->display_options['filters']['field_ag_agris_tid']['field'] = 'field_ag_agris_tid';
  $handler->display->display_options['filters']['field_ag_agris_tid']['exposed'] = TRUE;
  $handler->display->display_options['filters']['field_ag_agris_tid']['expose']['operator_id'] = 'field_ag_agris_tid_op';
  $handler->display->display_options['filters']['field_ag_agris_tid']['expose']['label'] = 'Agris Subject Categories (field_ag_agris)';
  $handler->display->display_options['filters']['field_ag_agris_tid']['expose']['operator'] = 'field_ag_agris_tid_op';
  $handler->display->display_options['filters']['field_ag_agris_tid']['expose']['identifier'] = 'field_ag_agris_tid';
  $handler->display->display_options['filters']['field_ag_agris_tid']['expose']['multiple'] = TRUE;
  $handler->display->display_options['filters']['field_ag_agris_tid']['expose']['remember_roles'] = array(
    2 => '2',
    3 => 0,
    1 => 0,
    6 => 0,
    7 => 0,
    4 => 0,
    5 => 0,
  );
  $handler->display->display_options['filters']['field_ag_agris_tid']['type'] = 'select';
  $handler->display->display_options['filters']['field_ag_agris_tid']['vocabulary'] = 'vocabulary_10';
  $handler->display->display_options['path'] = 'ag_documents';

  /* Display: Documents list */
  $handler = $view->new_display('page', 'Documents list', 'page_2');
  $handler->display->display_options['defaults']['title'] = FALSE;
  $handler->display->display_options['title'] = 'Documents: catalog';
  $handler->display->display_options['defaults']['header'] = FALSE;
  /* Header: Global: Text area */
  $handler->display->display_options['header']['text']['id'] = 'text';
  $handler->display->display_options['header']['text']['table'] = 'views';
  $handler->display->display_options['header']['text']['field'] = 'area';
  $handler->display->display_options['header']['text']['content'] = 'By default records are diaplayed in alphabetical order by title.
Clicking on a column header will sort records according to that header.
<?php
    $view = views_get_current_view();
    if ($view->total_rows > 0)
      print \'<hr /><center><div class="view-counter">Total: \' . $view->total_rows . \'</div></center>\';
?>';
  $handler->display->display_options['header']['text']['format'] = '3';
  $handler->display->display_options['defaults']['footer'] = FALSE;
  /* Footer: Global: Text area */
  $handler->display->display_options['footer']['text']['id'] = 'text';
  $handler->display->display_options['footer']['text']['table'] = 'views';
  $handler->display->display_options['footer']['text']['field'] = 'area';
  $handler->display->display_options['footer']['text']['empty'] = TRUE;
  $handler->display->display_options['footer']['text']['content'] = '<?php
global $base_url;
?>
<div align="center"><a href="<?php echo $base_url;?>/ag_documents/rss" target="_blank"><img src="<?php echo $base_url;?>/sites/default/files/images/RSS_feed.png"> RSS feed</a> <i>[compliant with the Agris AP]</i> | 
<a href="<?php echo $base_url;?>/sparql2?query=PREFIX+rdf%3A+%3Chttp%3A%2F%2Fwww.w3.org%2F1999%2F02%2F22-rdf-syntax-ns%23%3E%0D%0APREFIX+bibo%3A+%3Chttp%3A%2F%2Fpurl.org%2Fontology%2Fbibo%2F%3E+%0D%0APREFIX+dcterms%3A+%3Chttp%3A%2F%2Fpurl.org%2Fdc%2Fterms%2F%3E+%0D%0ADESCRIBE+%3Fdoc+%3Fauthor+%3Fjournal+%0D%0AWHERE+{%0D%0A++%3Fdoc+rdf%3Atype+bibo%3ADocument+.%0D%0A++%3Fdoc+dcterms%3Atitle+%3Ftitle+.%0D%0A++OPTIONAL+{%3Fdoc+dcterms%3Acreator+%3Fauthor}+.%0D%0A++OPTIONAL+{%3Fdoc+dcterms%3Asource+%3Fjournal}+.%0D%0A++OPTIONAL+{%3Fjournal+rdf%3Atype+bibo%3APeriodical}+.%0D%0A}%0D%0AORDER+BY+%3Ftitle%0D%0ALIMIT+500+OFFSET+0&output=htmltab&jsonp=&key=" target="_blank" alt="Rdf" title="Sparql query show only first 500 records.To get more records change parameters: \'limit\' or \'offset\' in the URL."><img src="<?php echo $base_url;?>/sites/default/files/images/rdf-icon.gif"> RDF feed</a> | 
<a href="<?php echo $base_url;?>/ag_dlios-xml" target="_blank"><img src="<?php echo $base_url;?>/sites/default/files/images/xml.png"> Agris AP XML</a></div>
<script language="javascript">
  selects = document.getElementsByTagName("select");
  for (i=0; i<selects.length; i++) {
    if (selects[i].size > 4)
      selects[i].size = 4;
  }
</script>
';
  $handler->display->display_options['footer']['text']['format'] = '3';
  $handler->display->display_options['defaults']['relationships'] = FALSE;
  $handler->display->display_options['defaults']['fields'] = FALSE;
  /* Field: Content: Title */
  $handler->display->display_options['fields']['title']['id'] = 'title';
  $handler->display->display_options['fields']['title']['table'] = 'node';
  $handler->display->display_options['fields']['title']['field'] = 'title';
  /* Field: Content: Author(s) */
  $handler->display->display_options['fields']['field_ag_author']['id'] = 'field_ag_author';
  $handler->display->display_options['fields']['field_ag_author']['table'] = 'field_data_field_ag_author';
  $handler->display->display_options['fields']['field_ag_author']['field'] = 'field_ag_author';
  $handler->display->display_options['fields']['field_ag_author']['label'] = 'Author';
  $handler->display->display_options['fields']['field_ag_author']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['field_ag_author']['delta_offset'] = '0';
  /* Field: Content: Corporate author(s) */
  $handler->display->display_options['fields']['field_ag_corauthor']['id'] = 'field_ag_corauthor';
  $handler->display->display_options['fields']['field_ag_corauthor']['table'] = 'field_data_field_ag_corauthor';
  $handler->display->display_options['fields']['field_ag_corauthor']['field'] = 'field_ag_corauthor';
  $handler->display->display_options['fields']['field_ag_corauthor']['label'] = '';
  $handler->display->display_options['fields']['field_ag_corauthor']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['field_ag_corauthor']['delta_offset'] = '0';
  /* Field: Content: Date of publication */
  $handler->display->display_options['fields']['field_ag_datepub']['id'] = 'field_ag_datepub';
  $handler->display->display_options['fields']['field_ag_datepub']['table'] = 'field_data_field_ag_datepub';
  $handler->display->display_options['fields']['field_ag_datepub']['field'] = 'field_ag_datepub';
  $handler->display->display_options['fields']['field_ag_datepub']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['field_ag_datepub']['settings'] = array(
    'format_type' => 'short',
    'fromto' => 'both',
    'multiple_number' => '',
    'multiple_from' => '',
    'multiple_to' => '',
  );
  $handler->display->display_options['defaults']['arguments'] = FALSE;
  $handler->display->display_options['defaults']['filter_groups'] = FALSE;
  $handler->display->display_options['filter_groups']['operator'] = 'OR';
  $handler->display->display_options['filter_groups']['groups'] = array(
    1 => 'AND',
    2 => 'AND',
  );
  $handler->display->display_options['defaults']['filters'] = FALSE;
  /* Filter criterion: Content: Published */
  $handler->display->display_options['filters']['status']['id'] = 'status';
  $handler->display->display_options['filters']['status']['table'] = 'node';
  $handler->display->display_options['filters']['status']['field'] = 'status';
  $handler->display->display_options['filters']['status']['value'] = '1';
  $handler->display->display_options['filters']['status']['group'] = 1;
  $handler->display->display_options['filters']['status']['expose']['operator'] = FALSE;
  /* Filter criterion: Content: Type */
  $handler->display->display_options['filters']['type']['id'] = 'type';
  $handler->display->display_options['filters']['type']['table'] = 'node';
  $handler->display->display_options['filters']['type']['field'] = 'type';
  $handler->display->display_options['filters']['type']['value'] = array(
    'agdlios' => 'agdlios',
  );
  $handler->display->display_options['filters']['type']['group'] = 1;
  $handler->display->display_options['filters']['type']['expose']['operator'] = FALSE;
  /* Filter criterion: Content: Title */
  $handler->display->display_options['filters']['title']['id'] = 'title';
  $handler->display->display_options['filters']['title']['table'] = 'node';
  $handler->display->display_options['filters']['title']['field'] = 'title';
  $handler->display->display_options['filters']['title']['operator'] = 'contains';
  $handler->display->display_options['filters']['title']['group'] = 1;
  $handler->display->display_options['filters']['title']['exposed'] = TRUE;
  $handler->display->display_options['filters']['title']['expose']['operator_id'] = 'title_op';
  $handler->display->display_options['filters']['title']['expose']['label'] = 'Search';
  $handler->display->display_options['filters']['title']['expose']['operator'] = 'title_op';
  $handler->display->display_options['filters']['title']['expose']['identifier'] = 'title';
  $handler->display->display_options['path'] = 'ag_documents-catalog';

  /* Display: Feed RSS of documents */
  $handler = $view->new_display('feed', 'Feed RSS of documents', 'feed_2');
  $handler->display->display_options['defaults']['title'] = FALSE;
  $handler->display->display_options['title'] = 'RSS feed of documents';
  $handler->display->display_options['pager']['type'] = 'none';
  $handler->display->display_options['pager']['options']['offset'] = '0';
  $handler->display->display_options['style_plugin'] = 'rss';
  $handler->display->display_options['style_options']['description'] = 'Documents';
  $handler->display->display_options['row_plugin'] = 'node_rss';
  $handler->display->display_options['defaults']['header'] = FALSE;
  $handler->display->display_options['defaults']['footer'] = FALSE;
  $handler->display->display_options['defaults']['relationships'] = FALSE;
  $handler->display->display_options['defaults']['fields'] = FALSE;
  /* Field: Content: Title */
  $handler->display->display_options['fields']['title']['id'] = 'title';
  $handler->display->display_options['fields']['title']['table'] = 'node';
  $handler->display->display_options['fields']['title']['field'] = 'title';
  $handler->display->display_options['fields']['title']['label'] = '';
  $handler->display->display_options['fields']['title']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['title']['link_to_node'] = FALSE;
  /* Field: Content: Agrovoc terms */
  $handler->display->display_options['fields']['field_ag_agrovoc']['id'] = 'field_ag_agrovoc';
  $handler->display->display_options['fields']['field_ag_agrovoc']['table'] = 'field_data_field_ag_agrovoc';
  $handler->display->display_options['fields']['field_ag_agrovoc']['field'] = 'field_ag_agrovoc';
  $handler->display->display_options['fields']['field_ag_agrovoc']['label'] = '';
  $handler->display->display_options['fields']['field_ag_agrovoc']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['field_ag_agrovoc']['type'] = 'taxonomy_term_reference_plain';
  $handler->display->display_options['fields']['field_ag_agrovoc']['group_rows'] = FALSE;
  $handler->display->display_options['fields']['field_ag_agrovoc']['delta_offset'] = '0';
  $handler->display->display_options['defaults']['arguments'] = FALSE;
  $handler->display->display_options['defaults']['filter_groups'] = FALSE;
  $handler->display->display_options['defaults']['filters'] = FALSE;
  /* Filter criterion: Content: Published */
  $handler->display->display_options['filters']['status']['id'] = 'status';
  $handler->display->display_options['filters']['status']['table'] = 'node';
  $handler->display->display_options['filters']['status']['field'] = 'status';
  $handler->display->display_options['filters']['status']['value'] = '1';
  $handler->display->display_options['filters']['status']['group'] = 1;
  $handler->display->display_options['filters']['status']['expose']['operator'] = FALSE;
  /* Filter criterion: Content: Type */
  $handler->display->display_options['filters']['type']['id'] = 'type';
  $handler->display->display_options['filters']['type']['table'] = 'node';
  $handler->display->display_options['filters']['type']['field'] = 'type';
  $handler->display->display_options['filters']['type']['value'] = array(
    'agdlios' => 'agdlios',
  );
  $handler->display->display_options['filters']['type']['group'] = 1;
  $handler->display->display_options['filters']['type']['expose']['operator'] = FALSE;
  /* Filter criterion: Content: Document type (field_ag_type) */
  $handler->display->display_options['filters']['field_ag_type_tid']['id'] = 'field_ag_type_tid';
  $handler->display->display_options['filters']['field_ag_type_tid']['table'] = 'field_data_field_ag_type';
  $handler->display->display_options['filters']['field_ag_type_tid']['field'] = 'field_ag_type_tid';
  $handler->display->display_options['filters']['field_ag_type_tid']['operator'] = 'not';
  $handler->display->display_options['filters']['field_ag_type_tid']['value'] = array(
    1 => '1',
    7 => '7',
  );
  $handler->display->display_options['filters']['field_ag_type_tid']['group'] = 1;
  $handler->display->display_options['filters']['field_ag_type_tid']['type'] = 'select';
  $handler->display->display_options['filters']['field_ag_type_tid']['vocabulary'] = 'vocabulary_1';
  $handler->display->display_options['path'] = 'ag_documents/rss';
  $handler->display->display_options['sitename_title'] = 0;
  $export['Ag_Documents'] = $view;

  $view = new view();
  $view->name = 'Ag_ListRecordsAP';
  $view->description = '';
  $view->tag = '';
  $view->base_table = 'node';
  $view->human_name = '';
  $view->core = 0;
  $view->api_version = '3.0';
  $view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */

  /* Display: Defaults */
  $handler = $view->new_display('default', 'Defaults', 'default');
  $handler->display->display_options['title'] = 'ListRecords Agris AP';
  $handler->display->display_options['items_per_page'] = 0;
  $handler->display->display_options['use_more_always'] = FALSE;
  $handler->display->display_options['access']['type'] = 'none';
  $handler->display->display_options['cache']['type'] = 'none';
  $handler->display->display_options['query']['type'] = 'views_query';
  $handler->display->display_options['query']['options']['distinct'] = TRUE;
  $handler->display->display_options['exposed_form']['type'] = 'basic';
  $handler->display->display_options['pager']['type'] = 'full';
  $handler->display->display_options['style_plugin'] = 'views_xml_style';
  $handler->display->display_options['style_options']['root_element'] = 'resources';
  $handler->display->display_options['style_options']['top_child_object'] = 'resource';
  $handler->display->display_options['style_options']['plaintext_output'] = 1;
  $handler->display->display_options['style_options']['content_type'] = 'default';
  $handler->display->display_options['style_options']['header'] = '';
  $handler->display->display_options['style_options']['using_views_api_mode'] = 0;
  /* Relationship: Broken/missing handler */
  $handler->display->display_options['relationships']['field_ag_book_nid']['id'] = 'field_ag_book_nid';
  $handler->display->display_options['relationships']['field_ag_book_nid']['table'] = 'node_data_field_ag_book';
  $handler->display->display_options['relationships']['field_ag_book_nid']['field'] = 'field_ag_book_nid';
  $handler->display->display_options['relationships']['field_ag_book_nid']['label'] = 'Book';
  /* Relationship: Broken/missing handler */
  $handler->display->display_options['relationships']['field_ag_type_value']['id'] = 'field_ag_type_value';
  $handler->display->display_options['relationships']['field_ag_type_value']['table'] = 'node_data_field_ag_type';
  $handler->display->display_options['relationships']['field_ag_type_value']['field'] = 'field_ag_type_value';
  $handler->display->display_options['relationships']['field_ag_type_value']['label'] = 'Document type';
  /* Relationship: Broken/missing handler */
  $handler->display->display_options['relationships']['field_ag_journal_nid']['id'] = 'field_ag_journal_nid';
  $handler->display->display_options['relationships']['field_ag_journal_nid']['table'] = 'node_data_field_ag_journal';
  $handler->display->display_options['relationships']['field_ag_journal_nid']['field'] = 'field_ag_journal_nid';
  $handler->display->display_options['relationships']['field_ag_journal_nid']['label'] = 'Journal title';
  /* Field: Content: Title */
  $handler->display->display_options['fields']['title']['id'] = 'title';
  $handler->display->display_options['fields']['title']['table'] = 'node';
  $handler->display->display_options['fields']['title']['field'] = 'title';
  $handler->display->display_options['fields']['title']['label'] = 'dc:title';
  $handler->display->display_options['fields']['title']['link_to_node'] = FALSE;
  /* Field: Broken/missing handler */
  $handler->display->display_options['fields']['field_ag_subtitle_value']['id'] = 'field_ag_subtitle_value';
  $handler->display->display_options['fields']['field_ag_subtitle_value']['table'] = 'node_data_field_ag_subtitle';
  $handler->display->display_options['fields']['field_ag_subtitle_value']['field'] = 'field_ag_subtitle_value';
  $handler->display->display_options['fields']['field_ag_subtitle_value']['label'] = 'dcterms:alternative';
  /* Field: Broken/missing handler */
  $handler->display->display_options['fields']['field_ag_author_nid']['id'] = 'field_ag_author_nid';
  $handler->display->display_options['fields']['field_ag_author_nid']['table'] = 'node_data_field_ag_author';
  $handler->display->display_options['fields']['field_ag_author_nid']['field'] = 'field_ag_author_nid';
  $handler->display->display_options['fields']['field_ag_author_nid']['label'] = 'ags:creatorPersonal';
  /* Field: Broken/missing handler */
  $handler->display->display_options['fields']['field_ag_confname_nid']['id'] = 'field_ag_confname_nid';
  $handler->display->display_options['fields']['field_ag_confname_nid']['table'] = 'node_data_field_ag_confname';
  $handler->display->display_options['fields']['field_ag_confname_nid']['field'] = 'field_ag_confname_nid';
  $handler->display->display_options['fields']['field_ag_confname_nid']['label'] = 'ags:creatorConference';
  /* Field: Broken/missing handler */
  $handler->display->display_options['fields']['field_ag_corauthor_nid']['id'] = 'field_ag_corauthor_nid';
  $handler->display->display_options['fields']['field_ag_corauthor_nid']['table'] = 'node_data_field_ag_corauthor';
  $handler->display->display_options['fields']['field_ag_corauthor_nid']['field'] = 'field_ag_corauthor_nid';
  $handler->display->display_options['fields']['field_ag_corauthor_nid']['label'] = 'ags:creatorCorporate';
  /* Field: Broken/missing handler */
  $handler->display->display_options['fields']['field_ag_publishplace_value']['id'] = 'field_ag_publishplace_value';
  $handler->display->display_options['fields']['field_ag_publishplace_value']['table'] = 'node_data_field_ag_publishplace';
  $handler->display->display_options['fields']['field_ag_publishplace_value']['field'] = 'field_ag_publishplace_value';
  $handler->display->display_options['fields']['field_ag_publishplace_value']['label'] = 'ags:publisherPlace';
  /* Field: Broken/missing handler */
  $handler->display->display_options['fields']['field_ag_publisher_nid']['id'] = 'field_ag_publisher_nid';
  $handler->display->display_options['fields']['field_ag_publisher_nid']['table'] = 'node_data_field_ag_publisher';
  $handler->display->display_options['fields']['field_ag_publisher_nid']['field'] = 'field_ag_publisher_nid';
  $handler->display->display_options['fields']['field_ag_publisher_nid']['label'] = 'ags:publisherName';
  /* Field: Broken/missing handler */
  $handler->display->display_options['fields']['phpcode_4']['id'] = 'phpcode_4';
  $handler->display->display_options['fields']['phpcode_4']['table'] = 'customfield';
  $handler->display->display_options['fields']['phpcode_4']['field'] = 'phpcode';
  $handler->display->display_options['fields']['phpcode_4']['label'] = '';
  $handler->display->display_options['fields']['phpcode_4']['exclude'] = TRUE;
  /* Field: Broken/missing handler */
  $handler->display->display_options['fields']['field_ag_datepub_value']['id'] = 'field_ag_datepub_value';
  $handler->display->display_options['fields']['field_ag_datepub_value']['table'] = 'node_data_field_ag_datepub';
  $handler->display->display_options['fields']['field_ag_datepub_value']['field'] = 'field_ag_datepub_value';
  $handler->display->display_options['fields']['field_ag_datepub_value']['label'] = 'dcterms:dateIssued';
  /* Field: Broken/missing handler */
  $handler->display->display_options['fields']['field_ag_agris_value']['id'] = 'field_ag_agris_value';
  $handler->display->display_options['fields']['field_ag_agris_value']['table'] = 'node_data_field_ag_agris';
  $handler->display->display_options['fields']['field_ag_agris_value']['field'] = 'field_ag_agris_value';
  $handler->display->display_options['fields']['field_ag_agris_value']['label'] = 'ags:subjectClassification';
  /* Field: Broken/missing handler */
  $handler->display->display_options['fields']['field_ag_agrovoc_value']['id'] = 'field_ag_agrovoc_value';
  $handler->display->display_options['fields']['field_ag_agrovoc_value']['table'] = 'node_data_field_ag_agrovoc';
  $handler->display->display_options['fields']['field_ag_agrovoc_value']['field'] = 'field_ag_agrovoc_value';
  $handler->display->display_options['fields']['field_ag_agrovoc_value']['label'] = 'ags:subjectThesaurus';
  /* Field: Broken/missing handler */
  $handler->display->display_options['fields']['field_ag_edition_value']['id'] = 'field_ag_edition_value';
  $handler->display->display_options['fields']['field_ag_edition_value']['table'] = 'node_data_field_ag_edition';
  $handler->display->display_options['fields']['field_ag_edition_value']['field'] = 'field_ag_edition_value';
  $handler->display->display_options['fields']['field_ag_edition_value']['label'] = 'ags:descriptionEdition';
  /* Field: Broken/missing handler */
  $handler->display->display_options['fields']['field_ag_notes_value']['id'] = 'field_ag_notes_value';
  $handler->display->display_options['fields']['field_ag_notes_value']['table'] = 'node_data_field_ag_notes';
  $handler->display->display_options['fields']['field_ag_notes_value']['field'] = 'field_ag_notes_value';
  $handler->display->display_options['fields']['field_ag_notes_value']['label'] = 'ags:descriptionNotes';
  /* Field: Broken/missing handler */
  $handler->display->display_options['fields']['field_ag_abstract_value']['id'] = 'field_ag_abstract_value';
  $handler->display->display_options['fields']['field_ag_abstract_value']['table'] = 'node_data_field_ag_abstract';
  $handler->display->display_options['fields']['field_ag_abstract_value']['field'] = 'field_ag_abstract_value';
  $handler->display->display_options['fields']['field_ag_abstract_value']['label'] = 'dcterms:abstract';
  /* Field: Broken/missing handler */
  $handler->display->display_options['fields']['upload_fid']['id'] = 'upload_fid';
  $handler->display->display_options['fields']['upload_fid']['table'] = 'node';
  $handler->display->display_options['fields']['upload_fid']['field'] = 'upload_fid';
  $handler->display->display_options['fields']['upload_fid']['label'] = 'dc:identifier';
  $handler->display->display_options['fields']['upload_fid']['alter']['alter_text'] = TRUE;
  $handler->display->display_options['fields']['upload_fid']['alter']['text'] = '[upload_fid-path]';
  $handler->display->display_options['fields']['upload_fid']['hide_empty'] = TRUE;
  /* Field: Broken/missing handler */
  $handler->display->display_options['fields']['field_ag_isbn_value']['id'] = 'field_ag_isbn_value';
  $handler->display->display_options['fields']['field_ag_isbn_value']['table'] = 'node_data_field_ag_isbn';
  $handler->display->display_options['fields']['field_ag_isbn_value']['field'] = 'field_ag_isbn_value';
  $handler->display->display_options['fields']['field_ag_isbn_value']['label'] = 'dc:identifier';
  /* Field: Broken/missing handler */
  $handler->display->display_options['fields']['field_ag_issn_value']['id'] = 'field_ag_issn_value';
  $handler->display->display_options['fields']['field_ag_issn_value']['table'] = 'node_data_field_ag_issn';
  $handler->display->display_options['fields']['field_ag_issn_value']['field'] = 'field_ag_issn_value';
  $handler->display->display_options['fields']['field_ag_issn_value']['label'] = 'dc:identifier';
  /* Field: Broken/missing handler */
  $handler->display->display_options['fields']['field_ag_type_value']['id'] = 'field_ag_type_value';
  $handler->display->display_options['fields']['field_ag_type_value']['table'] = 'node_data_field_ag_type';
  $handler->display->display_options['fields']['field_ag_type_value']['field'] = 'field_ag_type_value';
  $handler->display->display_options['fields']['field_ag_type_value']['label'] = 'dc:type';
  /* Field: Broken/missing handler */
  $handler->display->display_options['fields']['field_ag_pagination_value']['id'] = 'field_ag_pagination_value';
  $handler->display->display_options['fields']['field_ag_pagination_value']['table'] = 'node_data_field_ag_pagination';
  $handler->display->display_options['fields']['field_ag_pagination_value']['field'] = 'field_ag_pagination_value';
  $handler->display->display_options['fields']['field_ag_pagination_value']['label'] = 'dcterms:extent';
  /* Field: Broken/missing handler */
  $handler->display->display_options['fields']['field_ag_form_value']['id'] = 'field_ag_form_value';
  $handler->display->display_options['fields']['field_ag_form_value']['table'] = 'node_data_field_ag_form';
  $handler->display->display_options['fields']['field_ag_form_value']['field'] = 'field_ag_form_value';
  $handler->display->display_options['fields']['field_ag_form_value']['label'] = 'dcterms:medium';
  /* Field: Broken/missing handler */
  $handler->display->display_options['fields']['field_ag_language_value']['id'] = 'field_ag_language_value';
  $handler->display->display_options['fields']['field_ag_language_value']['table'] = 'node_data_field_ag_language';
  $handler->display->display_options['fields']['field_ag_language_value']['field'] = 'field_ag_language_value';
  $handler->display->display_options['fields']['field_ag_language_value']['label'] = 'dc:language';
  /* Field: Broken/missing handler */
  $handler->display->display_options['fields']['field_ag_phlocation_value']['id'] = 'field_ag_phlocation_value';
  $handler->display->display_options['fields']['field_ag_phlocation_value']['table'] = 'node_data_field_ag_phlocation';
  $handler->display->display_options['fields']['field_ag_phlocation_value']['field'] = 'field_ag_phlocation_value';
  $handler->display->display_options['fields']['field_ag_phlocation_value']['label'] = 'ags:availabilityLocation';
  /* Field: Broken/missing handler */
  $handler->display->display_options['fields']['field_ag_locator_value']['id'] = 'field_ag_locator_value';
  $handler->display->display_options['fields']['field_ag_locator_value']['table'] = 'node_data_field_ag_locator';
  $handler->display->display_options['fields']['field_ag_locator_value']['field'] = 'field_ag_locator_value';
  $handler->display->display_options['fields']['field_ag_locator_value']['label'] = 'ags:availabilityNumber';
  /* Field: Broken/missing handler */
  $handler->display->display_options['fields']['field_ag_journal_nid']['id'] = 'field_ag_journal_nid';
  $handler->display->display_options['fields']['field_ag_journal_nid']['table'] = 'node_data_field_ag_journal';
  $handler->display->display_options['fields']['field_ag_journal_nid']['field'] = 'field_ag_journal_nid';
  $handler->display->display_options['fields']['field_ag_journal_nid']['label'] = 'ags:citationTitle';
  /* Field: Broken/missing handler */
  $handler->display->display_options['fields']['field_ag_issn_value_1']['id'] = 'field_ag_issn_value_1';
  $handler->display->display_options['fields']['field_ag_issn_value_1']['table'] = 'node_data_field_ag_issn';
  $handler->display->display_options['fields']['field_ag_issn_value_1']['field'] = 'field_ag_issn_value';
  $handler->display->display_options['fields']['field_ag_issn_value_1']['relationship'] = 'field_ag_journal_nid';
  $handler->display->display_options['fields']['field_ag_issn_value_1']['label'] = 'ags:citationIdentifier';
  /* Field: Broken/missing handler */
  $handler->display->display_options['fields']['field_ag_book_nid']['id'] = 'field_ag_book_nid';
  $handler->display->display_options['fields']['field_ag_book_nid']['table'] = 'node_data_field_ag_book';
  $handler->display->display_options['fields']['field_ag_book_nid']['field'] = 'field_ag_book_nid';
  $handler->display->display_options['fields']['field_ag_book_nid']['label'] = 'ags:citationTitle';
  $handler->display->display_options['fields']['field_ag_book_nid']['exclude'] = TRUE;
  /* Field: Broken/missing handler */
  $handler->display->display_options['fields']['phpcode_2']['id'] = 'phpcode_2';
  $handler->display->display_options['fields']['phpcode_2']['table'] = 'customfield';
  $handler->display->display_options['fields']['phpcode_2']['field'] = 'phpcode';
  $handler->display->display_options['fields']['phpcode_2']['label'] = 'ags:citationIdentifier';
  $handler->display->display_options['fields']['phpcode_2']['exclude'] = TRUE;
  /* Field: Broken/missing handler */
  $handler->display->display_options['fields']['phpcode_1']['id'] = 'phpcode_1';
  $handler->display->display_options['fields']['phpcode_1']['table'] = 'customfield';
  $handler->display->display_options['fields']['phpcode_1']['field'] = 'phpcode';
  $handler->display->display_options['fields']['phpcode_1']['label'] = 'ags:citationNumber';
  /* Field: Content: Title */
  $handler->display->display_options['fields']['title_1']['id'] = 'title_1';
  $handler->display->display_options['fields']['title_1']['table'] = 'node';
  $handler->display->display_options['fields']['title_1']['field'] = 'title';
  $handler->display->display_options['fields']['title_1']['relationship'] = 'field_ag_book_nid';
  $handler->display->display_options['fields']['title_1']['label'] = 'dc:source';
  $handler->display->display_options['fields']['title_1']['link_to_node'] = FALSE;
  /* Sort criterion: Content: Nid */
  $handler->display->display_options['sorts']['nid']['id'] = 'nid';
  $handler->display->display_options['sorts']['nid']['table'] = 'node';
  $handler->display->display_options['sorts']['nid']['field'] = 'nid';
  /* Sort criterion: Content: Post date */
  $handler->display->display_options['sorts']['created']['id'] = 'created';
  $handler->display->display_options['sorts']['created']['table'] = 'node';
  $handler->display->display_options['sorts']['created']['field'] = 'created';
  $handler->display->display_options['sorts']['created']['granularity'] = 'day';
  /* Contextual filter: Taxonomy term: Name */
  $handler->display->display_options['arguments']['name']['id'] = 'name';
  $handler->display->display_options['arguments']['name']['table'] = 'taxonomy_term_data';
  $handler->display->display_options['arguments']['name']['field'] = 'name';
  $handler->display->display_options['arguments']['name']['exception']['title_enable'] = TRUE;
  $handler->display->display_options['arguments']['name']['default_argument_type'] = 'fixed';
  $handler->display->display_options['arguments']['name']['summary']['format'] = 'default_summary';
  $handler->display->display_options['arguments']['name']['specify_validation'] = TRUE;
  $handler->display->display_options['arguments']['name']['validate']['type'] = 'taxonomy_term';
  $handler->display->display_options['arguments']['name']['validate']['fail'] = 'ignore';
  $handler->display->display_options['arguments']['name']['limit'] = '0';
  /* Contextual filter: Date: Date (node) */
  $handler->display->display_options['arguments']['date_argument']['id'] = 'date_argument';
  $handler->display->display_options['arguments']['date_argument']['table'] = 'node';
  $handler->display->display_options['arguments']['date_argument']['field'] = 'date_argument';
  $handler->display->display_options['arguments']['date_argument']['exception']['title_enable'] = TRUE;
  $handler->display->display_options['arguments']['date_argument']['default_argument_type'] = 'fixed';
  $handler->display->display_options['arguments']['date_argument']['summary']['format'] = 'default_summary';
  $handler->display->display_options['arguments']['date_argument']['specify_validation'] = TRUE;
  $handler->display->display_options['arguments']['date_argument']['validate']['fail'] = 'ignore';
  $handler->display->display_options['arguments']['date_argument']['year_range'] = '-200:+0';
  $handler->display->display_options['arguments']['date_argument']['granularity'] = 'day';
  $handler->display->display_options['arguments']['date_argument']['date_fields'] = array(
    'node_revisions.timestamp' => 'node_revisions.timestamp',
  );
  $handler->display->display_options['arguments']['date_argument']['date_method'] = 'AND';
  /* Contextual filter: Global: Null */
  $handler->display->display_options['arguments']['null']['id'] = 'null';
  $handler->display->display_options['arguments']['null']['table'] = 'views';
  $handler->display->display_options['arguments']['null']['field'] = 'null';
  $handler->display->display_options['arguments']['null']['exception']['title_enable'] = TRUE;
  $handler->display->display_options['arguments']['null']['title_enable'] = TRUE;
  $handler->display->display_options['arguments']['null']['title'] = '%3';
  $handler->display->display_options['arguments']['null']['default_argument_type'] = 'fixed';
  $handler->display->display_options['arguments']['null']['summary']['format'] = 'default_summary';
  $handler->display->display_options['arguments']['null']['specify_validation'] = TRUE;
  $handler->display->display_options['arguments']['null']['validate']['type'] = 'php';
  $handler->display->display_options['arguments']['null']['validate']['fail'] = 'ignore';
  /* Filter criterion: Content: Type */
  $handler->display->display_options['filters']['type']['id'] = 'type';
  $handler->display->display_options['filters']['type']['table'] = 'node';
  $handler->display->display_options['filters']['type']['field'] = 'type';
  $handler->display->display_options['filters']['type']['value'] = array(
    'agdlios' => 'agdlios',
  );
  $handler->display->display_options['filters']['type']['group'] = '0';
  $handler->display->display_options['filters']['type']['expose']['operator'] = FALSE;
  /* Filter criterion: Content: Published */
  $handler->display->display_options['filters']['status']['id'] = 'status';
  $handler->display->display_options['filters']['status']['table'] = 'node';
  $handler->display->display_options['filters']['status']['field'] = 'status';
  $handler->display->display_options['filters']['status']['value'] = '1';
  $handler->display->display_options['filters']['status']['group'] = '0';
  $handler->display->display_options['filters']['status']['expose']['operator'] = FALSE;
  /* Filter criterion: Broken/missing handler */
  $handler->display->display_options['filters']['field_ag_type_value']['id'] = 'field_ag_type_value';
  $handler->display->display_options['filters']['field_ag_type_value']['table'] = 'node_data_field_ag_type';
  $handler->display->display_options['filters']['field_ag_type_value']['field'] = 'field_ag_type_value';
  $handler->display->display_options['filters']['field_ag_type_value']['operator'] = 'not';
  $handler->display->display_options['filters']['field_ag_type_value']['value'] = array(
    7 => '7',
  );
  $handler->display->display_options['filters']['field_ag_type_value']['group'] = '0';
  $handler->display->display_options['filters']['field_ag_type_value']['expose']['operator'] = FALSE;

  /* Display: ListRecordsARG */
  $handler = $view->new_display('page', 'ListRecordsARG', 'page_1');
  $handler->display->display_options['defaults']['title'] = FALSE;
  $handler->display->display_options['title'] = 'ListRecordsAP';
  $handler->display->display_options['defaults']['arguments'] = FALSE;
  /* Contextual filter: Taxonomy term: Name */
  $handler->display->display_options['arguments']['name']['id'] = 'name';
  $handler->display->display_options['arguments']['name']['table'] = 'taxonomy_term_data';
  $handler->display->display_options['arguments']['name']['field'] = 'name';
  $handler->display->display_options['arguments']['name']['exception']['title_enable'] = TRUE;
  $handler->display->display_options['arguments']['name']['default_argument_type'] = 'fixed';
  $handler->display->display_options['arguments']['name']['summary']['format'] = 'default_summary';
  $handler->display->display_options['arguments']['name']['specify_validation'] = TRUE;
  $handler->display->display_options['arguments']['name']['validate']['type'] = 'taxonomy_term';
  $handler->display->display_options['arguments']['name']['validate']['fail'] = 'empty';
  $handler->display->display_options['arguments']['name']['limit'] = '0';
  /* Contextual filter: Date: Date (node) */
  $handler->display->display_options['arguments']['date_argument']['id'] = 'date_argument';
  $handler->display->display_options['arguments']['date_argument']['table'] = 'node';
  $handler->display->display_options['arguments']['date_argument']['field'] = 'date_argument';
  $handler->display->display_options['arguments']['date_argument']['exception']['title_enable'] = TRUE;
  $handler->display->display_options['arguments']['date_argument']['default_argument_type'] = 'fixed';
  $handler->display->display_options['arguments']['date_argument']['summary']['format'] = 'default_summary';
  $handler->display->display_options['arguments']['date_argument']['specify_validation'] = TRUE;
  $handler->display->display_options['arguments']['date_argument']['validate']['fail'] = 'empty';
  $handler->display->display_options['arguments']['date_argument']['year_range'] = '-200:+0';
  $handler->display->display_options['arguments']['date_argument']['granularity'] = 'day';
  $handler->display->display_options['arguments']['date_argument']['date_fields'] = array(
    'node.created' => 'node.created',
  );
  $handler->display->display_options['arguments']['date_argument']['date_method'] = 'AND';
  /* Contextual filter: Global: Null */
  $handler->display->display_options['arguments']['null']['id'] = 'null';
  $handler->display->display_options['arguments']['null']['table'] = 'views';
  $handler->display->display_options['arguments']['null']['field'] = 'null';
  $handler->display->display_options['arguments']['null']['exception']['title_enable'] = TRUE;
  $handler->display->display_options['arguments']['null']['title_enable'] = TRUE;
  $handler->display->display_options['arguments']['null']['title'] = '%3';
  $handler->display->display_options['arguments']['null']['default_argument_type'] = 'fixed';
  $handler->display->display_options['arguments']['null']['summary']['format'] = 'default_summary';
  $handler->display->display_options['arguments']['null']['specify_validation'] = TRUE;
  $handler->display->display_options['arguments']['null']['validate']['type'] = 'php';
  $handler->display->display_options['arguments']['null']['validate']['fail'] = 'ignore';
  $handler->display->display_options['path'] = 'ag_ListRecords';

  /* Display: GetRecords */
  $handler = $view->new_display('page', 'GetRecords', 'page_2');
  $handler->display->display_options['defaults']['arguments'] = FALSE;
  /* Contextual filter: Content: Nid */
  $handler->display->display_options['arguments']['nid']['id'] = 'nid';
  $handler->display->display_options['arguments']['nid']['table'] = 'node';
  $handler->display->display_options['arguments']['nid']['field'] = 'nid';
  $handler->display->display_options['arguments']['nid']['default_action'] = 'empty';
  $handler->display->display_options['arguments']['nid']['exception']['title_enable'] = TRUE;
  $handler->display->display_options['arguments']['nid']['title_enable'] = TRUE;
  $handler->display->display_options['arguments']['nid']['title'] = '%1';
  $handler->display->display_options['arguments']['nid']['default_argument_type'] = 'fixed';
  $handler->display->display_options['arguments']['nid']['summary']['format'] = 'default_summary';
  $handler->display->display_options['arguments']['nid']['specify_validation'] = TRUE;
  $handler->display->display_options['path'] = 'ag_GetRecord/%';

  /* Display: Set */
  $handler = $view->new_display('page', 'Set', 'page_3');
  $handler->display->display_options['defaults']['arguments'] = FALSE;
  /* Contextual filter: Taxonomy term: Name */
  $handler->display->display_options['arguments']['name']['id'] = 'name';
  $handler->display->display_options['arguments']['name']['table'] = 'taxonomy_term_data';
  $handler->display->display_options['arguments']['name']['field'] = 'name';
  $handler->display->display_options['arguments']['name']['default_action'] = 'empty';
  $handler->display->display_options['arguments']['name']['exception']['title_enable'] = TRUE;
  $handler->display->display_options['arguments']['name']['default_argument_type'] = 'fixed';
  $handler->display->display_options['arguments']['name']['summary']['format'] = 'default_summary';
  $handler->display->display_options['arguments']['name']['specify_validation'] = TRUE;
  $handler->display->display_options['arguments']['name']['validate']['type'] = 'taxonomy_term';
  $handler->display->display_options['arguments']['name']['validate']['fail'] = 'empty';
  $handler->display->display_options['arguments']['name']['limit'] = '0';
  /* Contextual filter: Global: Null */
  $handler->display->display_options['arguments']['null']['id'] = 'null';
  $handler->display->display_options['arguments']['null']['table'] = 'views';
  $handler->display->display_options['arguments']['null']['field'] = 'null';
  $handler->display->display_options['arguments']['null']['exception']['title_enable'] = TRUE;
  $handler->display->display_options['arguments']['null']['title_enable'] = TRUE;
  $handler->display->display_options['arguments']['null']['title'] = '%3';
  $handler->display->display_options['arguments']['null']['default_argument_type'] = 'fixed';
  $handler->display->display_options['arguments']['null']['summary']['format'] = 'default_summary';
  $handler->display->display_options['arguments']['null']['specify_validation'] = TRUE;
  $handler->display->display_options['arguments']['null']['validate']['type'] = 'php';
  $handler->display->display_options['arguments']['null']['validate']['fail'] = 'ignore';
  $handler->display->display_options['path'] = 'ag_ListRecordsSet';

  /* Display: testListrec */
  $handler = $view->new_display('page', 'testListrec', 'page_4');
  $handler->display->display_options['defaults']['style_plugin'] = FALSE;
  $handler->display->display_options['style_plugin'] = 'default';
  $handler->display->display_options['defaults']['style_options'] = FALSE;
  $handler->display->display_options['defaults']['row_plugin'] = FALSE;
  $handler->display->display_options['row_plugin'] = 'fields';
  $handler->display->display_options['defaults']['row_options'] = FALSE;
  $handler->display->display_options['defaults']['relationships'] = FALSE;
  $handler->display->display_options['defaults']['fields'] = FALSE;
  /* Field: Content: Title */
  $handler->display->display_options['fields']['title']['id'] = 'title';
  $handler->display->display_options['fields']['title']['table'] = 'node';
  $handler->display->display_options['fields']['title']['field'] = 'title';
  $handler->display->display_options['fields']['title']['label'] = 'dc:title';
  $handler->display->display_options['fields']['title']['link_to_node'] = FALSE;
  /* Field: Content: Post date */
  $handler->display->display_options['fields']['created']['id'] = 'created';
  $handler->display->display_options['fields']['created']['table'] = 'node';
  $handler->display->display_options['fields']['created']['field'] = 'created';
  $handler->display->display_options['fields']['created']['date_format'] = 'custom';
  $handler->display->display_options['fields']['created']['custom_date_format'] = 'Y-m-d';
  $handler->display->display_options['defaults']['sorts'] = FALSE;
  $handler->display->display_options['defaults']['arguments'] = FALSE;
  /* Contextual filter: Date: Date (node) */
  $handler->display->display_options['arguments']['date_argument']['id'] = 'date_argument';
  $handler->display->display_options['arguments']['date_argument']['table'] = 'node';
  $handler->display->display_options['arguments']['date_argument']['field'] = 'date_argument';
  $handler->display->display_options['arguments']['date_argument']['default_action'] = 'empty';
  $handler->display->display_options['arguments']['date_argument']['exception']['title_enable'] = TRUE;
  $handler->display->display_options['arguments']['date_argument']['default_argument_type'] = 'fixed';
  $handler->display->display_options['arguments']['date_argument']['summary']['format'] = 'default_summary';
  $handler->display->display_options['arguments']['date_argument']['specify_validation'] = TRUE;
  $handler->display->display_options['arguments']['date_argument']['validate']['fail'] = 'empty';
  $handler->display->display_options['arguments']['date_argument']['year_range'] = '-200:+0';
  $handler->display->display_options['arguments']['date_argument']['granularity'] = 'day';
  $handler->display->display_options['arguments']['date_argument']['date_fields'] = array(
    'node.created' => 'node.created',
  );
  $handler->display->display_options['arguments']['date_argument']['date_method'] = 'AND';
  /* Contextual filter: Global: Null */
  $handler->display->display_options['arguments']['null']['id'] = 'null';
  $handler->display->display_options['arguments']['null']['table'] = 'views';
  $handler->display->display_options['arguments']['null']['field'] = 'null';
  $handler->display->display_options['arguments']['null']['exception']['title_enable'] = TRUE;
  $handler->display->display_options['arguments']['null']['title_enable'] = TRUE;
  $handler->display->display_options['arguments']['null']['title'] = '%3';
  $handler->display->display_options['arguments']['null']['default_argument_type'] = 'fixed';
  $handler->display->display_options['arguments']['null']['summary']['format'] = 'default_summary';
  $handler->display->display_options['arguments']['null']['specify_validation'] = TRUE;
  $handler->display->display_options['arguments']['null']['validate']['type'] = 'php';
  $handler->display->display_options['arguments']['null']['validate']['fail'] = 'ignore';
  $handler->display->display_options['path'] = 'ag_testlistrecords';

  /* Display: ListRecordsRange */
  $handler = $view->new_display('page', 'ListRecordsRange', 'page_5');
  $handler->display->display_options['defaults']['arguments'] = FALSE;
  /* Contextual filter: Date: Date (node) */
  $handler->display->display_options['arguments']['date_argument']['id'] = 'date_argument';
  $handler->display->display_options['arguments']['date_argument']['table'] = 'node';
  $handler->display->display_options['arguments']['date_argument']['field'] = 'date_argument';
  $handler->display->display_options['arguments']['date_argument']['default_action'] = 'empty';
  $handler->display->display_options['arguments']['date_argument']['exception']['title_enable'] = TRUE;
  $handler->display->display_options['arguments']['date_argument']['default_argument_type'] = 'fixed';
  $handler->display->display_options['arguments']['date_argument']['summary']['format'] = 'default_summary';
  $handler->display->display_options['arguments']['date_argument']['specify_validation'] = TRUE;
  $handler->display->display_options['arguments']['date_argument']['validate']['fail'] = 'empty';
  $handler->display->display_options['arguments']['date_argument']['year_range'] = '-200:+0';
  $handler->display->display_options['arguments']['date_argument']['granularity'] = 'day';
  $handler->display->display_options['arguments']['date_argument']['date_fields'] = array(
    'node.created' => 'node.created',
  );
  $handler->display->display_options['arguments']['date_argument']['date_method'] = 'AND';
  /* Contextual filter: Global: Null */
  $handler->display->display_options['arguments']['null']['id'] = 'null';
  $handler->display->display_options['arguments']['null']['table'] = 'views';
  $handler->display->display_options['arguments']['null']['field'] = 'null';
  $handler->display->display_options['arguments']['null']['exception']['title_enable'] = TRUE;
  $handler->display->display_options['arguments']['null']['title_enable'] = TRUE;
  $handler->display->display_options['arguments']['null']['title'] = '%3';
  $handler->display->display_options['arguments']['null']['default_argument_type'] = 'fixed';
  $handler->display->display_options['arguments']['null']['summary']['format'] = 'default_summary';
  $handler->display->display_options['arguments']['null']['specify_validation'] = TRUE;
  $handler->display->display_options['arguments']['null']['validate']['type'] = 'php';
  $handler->display->display_options['arguments']['null']['validate']['fail'] = 'ignore';
  $handler->display->display_options['path'] = 'ag_ListRecordsR';

  /* Display: ListRecords */
  $handler = $view->new_display('page', 'ListRecords', 'page_6');
  $handler->display->display_options['defaults']['arguments'] = FALSE;
  /* Contextual filter: Global: Null */
  $handler->display->display_options['arguments']['null']['id'] = 'null';
  $handler->display->display_options['arguments']['null']['table'] = 'views';
  $handler->display->display_options['arguments']['null']['field'] = 'null';
  $handler->display->display_options['arguments']['null']['exception']['title_enable'] = TRUE;
  $handler->display->display_options['arguments']['null']['title_enable'] = TRUE;
  $handler->display->display_options['arguments']['null']['title'] = '%3';
  $handler->display->display_options['arguments']['null']['default_argument_type'] = 'fixed';
  $handler->display->display_options['arguments']['null']['summary']['format'] = 'default_summary';
  $handler->display->display_options['arguments']['null']['specify_validation'] = TRUE;
  $handler->display->display_options['arguments']['null']['validate']['type'] = 'php';
  $handler->display->display_options['arguments']['null']['validate']['fail'] = 'ignore';
  $handler->display->display_options['path'] = 'ag_ListRecordsE';
  $export['Ag_ListRecordsAP'] = $view;

  $view = new view();
  $view->name = 'Ag_News';
  $view->description = '';
  $view->tag = '';
  $view->base_table = 'node';
  $view->human_name = '';
  $view->core = 0;
  $view->api_version = '3.0';
  $view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */

  /* Display: Defaults */
  $handler = $view->new_display('default', 'Defaults', 'default');
  $handler->display->display_options['title'] = 'News from partners';
  $handler->display->display_options['use_more_always'] = FALSE;
  $handler->display->display_options['access']['type'] = 'none';
  $handler->display->display_options['cache']['type'] = 'none';
  $handler->display->display_options['query']['type'] = 'views_query';
  $handler->display->display_options['exposed_form']['type'] = 'basic';
  $handler->display->display_options['exposed_form']['options']['submit_button'] = 'Search';
  $handler->display->display_options['exposed_form']['options']['reset_button'] = TRUE;
  $handler->display->display_options['pager']['type'] = 'some';
  $handler->display->display_options['pager']['options']['items_per_page'] = 5;
  $handler->display->display_options['style_plugin'] = 'default';
  $handler->display->display_options['row_plugin'] = 'fields';
  /* Field: Content: Title */
  $handler->display->display_options['fields']['title']['id'] = 'title';
  $handler->display->display_options['fields']['title']['table'] = 'node';
  $handler->display->display_options['fields']['title']['field'] = 'title';
  $handler->display->display_options['fields']['title']['label'] = '';
  /* Field: Broken/missing handler */
  $handler->display->display_options['fields']['body']['id'] = 'body';
  $handler->display->display_options['fields']['body']['table'] = 'field_revision_data';
  $handler->display->display_options['fields']['body']['field'] = 'body-revision_id';
  $handler->display->display_options['fields']['body']['label'] = '';
  $handler->display->display_options['fields']['body']['alter']['max_length'] = '100';
  $handler->display->display_options['fields']['body']['alter']['strip_tags'] = TRUE;
  $handler->display->display_options['fields']['body']['alter']['trim'] = TRUE;
  /* Field: Content: Path */
  $handler->display->display_options['fields']['path']['id'] = 'path';
  $handler->display->display_options['fields']['path']['table'] = 'node';
  $handler->display->display_options['fields']['path']['field'] = 'path';
  $handler->display->display_options['fields']['path']['relationship'] = 'feed_nid';
  $handler->display->display_options['fields']['path']['label'] = '';
  $handler->display->display_options['fields']['path']['exclude'] = TRUE;
  /* Field: Content: Title */
  $handler->display->display_options['fields']['title_1']['id'] = 'title_1';
  $handler->display->display_options['fields']['title_1']['table'] = 'node';
  $handler->display->display_options['fields']['title_1']['field'] = 'title';
  $handler->display->display_options['fields']['title_1']['relationship'] = 'feed_nid';
  $handler->display->display_options['fields']['title_1']['label'] = '';
  $handler->display->display_options['fields']['title_1']['alter']['alter_text'] = TRUE;
  $handler->display->display_options['fields']['title_1']['alter']['text'] = '<i>source: <a href="[path]"><span>[title_1]</span></a></i>';
  $handler->display->display_options['fields']['title_1']['link_to_node'] = FALSE;
  /* Filter criterion: Content: Published */
  $handler->display->display_options['filters']['status']['id'] = 'status';
  $handler->display->display_options['filters']['status']['table'] = 'node';
  $handler->display->display_options['filters']['status']['field'] = 'status';
  $handler->display->display_options['filters']['status']['value'] = '1';
  $handler->display->display_options['filters']['status']['group'] = '0';
  $handler->display->display_options['filters']['status']['expose']['operator'] = FALSE;
  /* Filter criterion: Content: Type */
  $handler->display->display_options['filters']['type']['id'] = 'type';
  $handler->display->display_options['filters']['type']['table'] = 'node';
  $handler->display->display_options['filters']['type']['field'] = 'type';
  $handler->display->display_options['filters']['type']['value'] = array(
    'agnews' => 'agnews',
  );
  $handler->display->display_options['filters']['type']['group'] = '0';
  $handler->display->display_options['filters']['type']['expose']['operator'] = FALSE;
  /* Filter criterion: Search: Search Terms */
  $handler->display->display_options['filters']['keys']['id'] = 'keys';
  $handler->display->display_options['filters']['keys']['table'] = 'search_index';
  $handler->display->display_options['filters']['keys']['field'] = 'keys';
  $handler->display->display_options['filters']['keys']['group'] = '0';
  $handler->display->display_options['filters']['keys']['exposed'] = TRUE;
  $handler->display->display_options['filters']['keys']['expose']['operator_id'] = 'keys_op';
  $handler->display->display_options['filters']['keys']['expose']['label'] = 'Search';
  $handler->display->display_options['filters']['keys']['expose']['operator'] = 'keys_op';
  $handler->display->display_options['filters']['keys']['expose']['identifier'] = 'keys';
  /* Filter criterion: Content: Title */
  $handler->display->display_options['filters']['title']['id'] = 'title';
  $handler->display->display_options['filters']['title']['table'] = 'node';
  $handler->display->display_options['filters']['title']['field'] = 'title';
  $handler->display->display_options['filters']['title']['relationship'] = 'feed_nid';
  $handler->display->display_options['filters']['title']['operator'] = 'contains';
  $handler->display->display_options['filters']['title']['group'] = '0';
  $handler->display->display_options['filters']['title']['exposed'] = TRUE;
  $handler->display->display_options['filters']['title']['expose']['operator_id'] = 'title_op';
  $handler->display->display_options['filters']['title']['expose']['label'] = 'Feed title';
  $handler->display->display_options['filters']['title']['expose']['operator'] = 'title_op';
  $handler->display->display_options['filters']['title']['expose']['identifier'] = 'title';

  /* Display: Latest external news */
  $handler = $view->new_display('page', 'Latest external news', 'page_1');
  $handler->display->display_options['defaults']['pager'] = FALSE;
  $handler->display->display_options['pager']['type'] = 'some';
  $handler->display->display_options['pager']['options']['items_per_page'] = '25';
  $handler->display->display_options['pager']['options']['offset'] = '0';
  $handler->display->display_options['defaults']['relationships'] = FALSE;
  /* Relationship: Feeds item: Owner feed */
  $handler->display->display_options['relationships']['feed_nid']['id'] = 'feed_nid';
  $handler->display->display_options['relationships']['feed_nid']['table'] = 'feeds_item';
  $handler->display->display_options['relationships']['feed_nid']['field'] = 'feed_nid';
  $handler->display->display_options['defaults']['fields'] = FALSE;
  /* Field: Content: Title */
  $handler->display->display_options['fields']['title']['id'] = 'title';
  $handler->display->display_options['fields']['title']['table'] = 'node';
  $handler->display->display_options['fields']['title']['field'] = 'title';
  $handler->display->display_options['fields']['title']['label'] = '';
  /* Field: Content: Post date */
  $handler->display->display_options['fields']['field_ag_post_date']['id'] = 'field_ag_post_date';
  $handler->display->display_options['fields']['field_ag_post_date']['table'] = 'field_data_field_ag_post_date';
  $handler->display->display_options['fields']['field_ag_post_date']['field'] = 'field_ag_post_date';
  $handler->display->display_options['fields']['field_ag_post_date']['label'] = '';
  $handler->display->display_options['fields']['field_ag_post_date']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['field_ag_post_date']['settings'] = array(
    'format_type' => 'date',
    'fromto' => 'both',
    'multiple_number' => '',
    'multiple_from' => '',
    'multiple_to' => '',
  );
  /* Field: Content: Path */
  $handler->display->display_options['fields']['path']['id'] = 'path';
  $handler->display->display_options['fields']['path']['table'] = 'node';
  $handler->display->display_options['fields']['path']['field'] = 'path';
  $handler->display->display_options['fields']['path']['relationship'] = 'feed_nid';
  $handler->display->display_options['fields']['path']['label'] = '';
  $handler->display->display_options['fields']['path']['exclude'] = TRUE;
  /* Field: Content: Title */
  $handler->display->display_options['fields']['title_1']['id'] = 'title_1';
  $handler->display->display_options['fields']['title_1']['table'] = 'node';
  $handler->display->display_options['fields']['title_1']['field'] = 'title';
  $handler->display->display_options['fields']['title_1']['relationship'] = 'feed_nid';
  $handler->display->display_options['fields']['title_1']['label'] = '';
  $handler->display->display_options['fields']['title_1']['alter']['alter_text'] = TRUE;
  $handler->display->display_options['fields']['title_1']['alter']['text'] = '<i>source: <a href="[path]"><span>[title_1]</span></a></i>';
  $handler->display->display_options['fields']['title_1']['link_to_node'] = FALSE;
  $handler->display->display_options['defaults']['filter_groups'] = FALSE;
  $handler->display->display_options['defaults']['filters'] = FALSE;
  /* Filter criterion: Content: Published */
  $handler->display->display_options['filters']['status']['id'] = 'status';
  $handler->display->display_options['filters']['status']['table'] = 'node';
  $handler->display->display_options['filters']['status']['field'] = 'status';
  $handler->display->display_options['filters']['status']['value'] = '1';
  $handler->display->display_options['filters']['status']['group'] = '0';
  $handler->display->display_options['filters']['status']['expose']['operator'] = FALSE;
  /* Filter criterion: Content: Type */
  $handler->display->display_options['filters']['type']['id'] = 'type';
  $handler->display->display_options['filters']['type']['table'] = 'node';
  $handler->display->display_options['filters']['type']['field'] = 'type';
  $handler->display->display_options['filters']['type']['value'] = array(
    'agrssnews' => 'agrssnews',
  );
  $handler->display->display_options['filters']['type']['group'] = '0';
  $handler->display->display_options['filters']['type']['expose']['operator'] = FALSE;
  /* Filter criterion: Search: Search Terms */
  $handler->display->display_options['filters']['keys']['id'] = 'keys';
  $handler->display->display_options['filters']['keys']['table'] = 'search_index';
  $handler->display->display_options['filters']['keys']['field'] = 'keys';
  $handler->display->display_options['filters']['keys']['group'] = '0';
  $handler->display->display_options['filters']['keys']['exposed'] = TRUE;
  $handler->display->display_options['filters']['keys']['expose']['operator_id'] = 'keys_op';
  $handler->display->display_options['filters']['keys']['expose']['label'] = 'Search';
  $handler->display->display_options['filters']['keys']['expose']['operator'] = 'keys_op';
  $handler->display->display_options['filters']['keys']['expose']['identifier'] = 'keys';
  /* Filter criterion: Content: Title */
  $handler->display->display_options['filters']['title']['id'] = 'title';
  $handler->display->display_options['filters']['title']['table'] = 'node';
  $handler->display->display_options['filters']['title']['field'] = 'title';
  $handler->display->display_options['filters']['title']['relationship'] = 'feed_nid';
  $handler->display->display_options['filters']['title']['operator'] = 'contains';
  $handler->display->display_options['filters']['title']['group'] = '0';
  $handler->display->display_options['filters']['title']['exposed'] = TRUE;
  $handler->display->display_options['filters']['title']['expose']['operator_id'] = 'title_op';
  $handler->display->display_options['filters']['title']['expose']['label'] = 'Feed title';
  $handler->display->display_options['filters']['title']['expose']['operator'] = 'title_op';
  $handler->display->display_options['filters']['title']['expose']['identifier'] = 'title';
  $handler->display->display_options['path'] = 'ag_partners-news';

  /* Display: Latest external news */
  $handler = $view->new_display('block', 'Latest external news', 'block_1');
  $handler->display->display_options['defaults']['use_more'] = FALSE;
  $handler->display->display_options['use_more'] = TRUE;
  $handler->display->display_options['defaults']['use_more_always'] = FALSE;
  $handler->display->display_options['defaults']['use_more_always'] = FALSE;
  $handler->display->display_options['use_more_always'] = FALSE;
  $handler->display->display_options['defaults']['use_more_text'] = FALSE;
  $handler->display->display_options['defaults']['relationships'] = FALSE;
  /* Relationship: Feeds item: Owner feed */
  $handler->display->display_options['relationships']['feed_nid']['id'] = 'feed_nid';
  $handler->display->display_options['relationships']['feed_nid']['table'] = 'feeds_item';
  $handler->display->display_options['relationships']['feed_nid']['field'] = 'feed_nid';
  $handler->display->display_options['defaults']['filter_groups'] = FALSE;
  $handler->display->display_options['defaults']['filters'] = FALSE;
  /* Filter criterion: Content: Published */
  $handler->display->display_options['filters']['status']['id'] = 'status';
  $handler->display->display_options['filters']['status']['table'] = 'node';
  $handler->display->display_options['filters']['status']['field'] = 'status';
  $handler->display->display_options['filters']['status']['value'] = '1';
  $handler->display->display_options['filters']['status']['group'] = 1;
  $handler->display->display_options['filters']['status']['expose']['operator'] = FALSE;
  /* Filter criterion: Content: Type */
  $handler->display->display_options['filters']['type']['id'] = 'type';
  $handler->display->display_options['filters']['type']['table'] = 'node';
  $handler->display->display_options['filters']['type']['field'] = 'type';
  $handler->display->display_options['filters']['type']['value'] = array(
    'agrssnews' => 'agrssnews',
  );
  $handler->display->display_options['filters']['type']['group'] = 1;
  $handler->display->display_options['filters']['type']['expose']['operator'] = FALSE;

  /* Display: Latest news */
  $handler = $view->new_display('page', 'Latest news', 'page_2');
  $handler->display->display_options['defaults']['title'] = FALSE;
  $handler->display->display_options['title'] = 'Our latest news';
  $handler->display->display_options['defaults']['pager'] = FALSE;
  $handler->display->display_options['pager']['type'] = 'some';
  $handler->display->display_options['pager']['options']['items_per_page'] = '25';
  $handler->display->display_options['pager']['options']['offset'] = '0';
  $handler->display->display_options['defaults']['relationships'] = FALSE;
  $handler->display->display_options['defaults']['fields'] = FALSE;
  /* Field: Content: Title */
  $handler->display->display_options['fields']['title']['id'] = 'title';
  $handler->display->display_options['fields']['title']['table'] = 'node';
  $handler->display->display_options['fields']['title']['field'] = 'title';
  $handler->display->display_options['fields']['title']['label'] = '';
  /* Field: Content: Post date */
  $handler->display->display_options['fields']['created']['id'] = 'created';
  $handler->display->display_options['fields']['created']['table'] = 'node';
  $handler->display->display_options['fields']['created']['field'] = 'created';
  $handler->display->display_options['fields']['created']['label'] = '';
  $handler->display->display_options['fields']['created']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['created']['date_format'] = 'date';
  /* Field: Content: Body */
  $handler->display->display_options['fields']['body']['id'] = 'body';
  $handler->display->display_options['fields']['body']['table'] = 'field_data_body';
  $handler->display->display_options['fields']['body']['field'] = 'body';
  $handler->display->display_options['fields']['body']['label'] = '';
  $handler->display->display_options['fields']['body']['alter']['max_length'] = '200';
  $handler->display->display_options['fields']['body']['alter']['strip_tags'] = TRUE;
  $handler->display->display_options['fields']['body']['alter']['trim'] = TRUE;
  $handler->display->display_options['fields']['body']['alter']['html'] = TRUE;
  $handler->display->display_options['fields']['body']['element_label_colon'] = FALSE;
  $handler->display->display_options['defaults']['filter_groups'] = FALSE;
  $handler->display->display_options['defaults']['filters'] = FALSE;
  /* Filter criterion: Content: Published */
  $handler->display->display_options['filters']['status']['id'] = 'status';
  $handler->display->display_options['filters']['status']['table'] = 'node';
  $handler->display->display_options['filters']['status']['field'] = 'status';
  $handler->display->display_options['filters']['status']['value'] = '1';
  $handler->display->display_options['filters']['status']['group'] = '0';
  $handler->display->display_options['filters']['status']['expose']['operator'] = FALSE;
  /* Filter criterion: Content: Type */
  $handler->display->display_options['filters']['type']['id'] = 'type';
  $handler->display->display_options['filters']['type']['table'] = 'node';
  $handler->display->display_options['filters']['type']['field'] = 'type';
  $handler->display->display_options['filters']['type']['value'] = array(
    'agnews' => 'agnews',
  );
  $handler->display->display_options['filters']['type']['group'] = '0';
  $handler->display->display_options['filters']['type']['expose']['operator'] = FALSE;
  $handler->display->display_options['path'] = 'ag_news';

  /* Display: Latest news */
  $handler = $view->new_display('block', 'Latest news', 'block_2');
  $handler->display->display_options['defaults']['title'] = FALSE;
  $handler->display->display_options['title'] = 'Our latest news';
  $handler->display->display_options['defaults']['use_more'] = FALSE;
  $handler->display->display_options['use_more'] = TRUE;
  $handler->display->display_options['defaults']['use_more_always'] = FALSE;
  $handler->display->display_options['defaults']['use_more_always'] = FALSE;
  $handler->display->display_options['use_more_always'] = FALSE;
  $handler->display->display_options['defaults']['use_more_text'] = FALSE;
  $handler->display->display_options['defaults']['pager'] = FALSE;
  $handler->display->display_options['pager']['type'] = 'some';
  $handler->display->display_options['pager']['options']['items_per_page'] = '3';
  $handler->display->display_options['pager']['options']['offset'] = '0';
  $handler->display->display_options['defaults']['relationships'] = FALSE;
  $handler->display->display_options['defaults']['fields'] = FALSE;
  /* Field: Content: Title */
  $handler->display->display_options['fields']['title']['id'] = 'title';
  $handler->display->display_options['fields']['title']['table'] = 'node';
  $handler->display->display_options['fields']['title']['field'] = 'title';
  $handler->display->display_options['fields']['title']['label'] = '';
  /* Field: Content: Body */
  $handler->display->display_options['fields']['body']['id'] = 'body';
  $handler->display->display_options['fields']['body']['table'] = 'field_data_body';
  $handler->display->display_options['fields']['body']['field'] = 'body';
  $handler->display->display_options['fields']['body']['label'] = '';
  $handler->display->display_options['fields']['body']['alter']['max_length'] = '200';
  $handler->display->display_options['fields']['body']['alter']['strip_tags'] = TRUE;
  $handler->display->display_options['fields']['body']['alter']['trim'] = TRUE;
  $handler->display->display_options['fields']['body']['alter']['html'] = TRUE;
  $handler->display->display_options['fields']['body']['element_label_colon'] = FALSE;
  /* Field: Content: Post date */
  $handler->display->display_options['fields']['field_ag_post_date']['id'] = 'field_ag_post_date';
  $handler->display->display_options['fields']['field_ag_post_date']['table'] = 'field_data_field_ag_post_date';
  $handler->display->display_options['fields']['field_ag_post_date']['field'] = 'field_ag_post_date';
  $handler->display->display_options['fields']['field_ag_post_date']['label'] = '';
  $handler->display->display_options['fields']['field_ag_post_date']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['field_ag_post_date']['settings'] = array(
    'format_type' => 'medium',
    'fromto' => 'both',
    'multiple_number' => '',
    'multiple_from' => '',
    'multiple_to' => '',
  );
  $handler->display->display_options['defaults']['filter_groups'] = FALSE;
  $handler->display->display_options['defaults']['filters'] = FALSE;
  /* Filter criterion: Content: Published */
  $handler->display->display_options['filters']['status']['id'] = 'status';
  $handler->display->display_options['filters']['status']['table'] = 'node';
  $handler->display->display_options['filters']['status']['field'] = 'status';
  $handler->display->display_options['filters']['status']['value'] = '1';
  $handler->display->display_options['filters']['status']['group'] = '0';
  $handler->display->display_options['filters']['status']['expose']['operator'] = FALSE;
  /* Filter criterion: Content: Type */
  $handler->display->display_options['filters']['type']['id'] = 'type';
  $handler->display->display_options['filters']['type']['table'] = 'node';
  $handler->display->display_options['filters']['type']['field'] = 'type';
  $handler->display->display_options['filters']['type']['value'] = array(
    'agnews' => 'agnews',
  );
  $handler->display->display_options['filters']['type']['group'] = '0';
  $handler->display->display_options['filters']['type']['expose']['operator'] = FALSE;

  /* Display: News RDF Feed */
  $handler = $view->new_display('feed', 'News RDF Feed', 'feed_1');
  $handler->display->display_options['defaults']['title'] = FALSE;
  $handler->display->display_options['title'] = 'Our latest news';
  $handler->display->display_options['items_per_page'] = 0;
  $handler->display->display_options['pager']['type'] = 'some';
  $handler->display->display_options['pager']['options']['items_per_page'] = '50';
  $handler->display->display_options['pager']['options']['offset'] = '0';
  $handler->display->display_options['style_plugin'] = 'rss';
  $handler->display->display_options['row_plugin'] = 'node_rss';
  $handler->display->display_options['defaults']['relationships'] = FALSE;
  $handler->display->display_options['defaults']['sorts'] = FALSE;
  /* Sort criterion: Content: Post date */
  $handler->display->display_options['sorts']['created']['id'] = 'created';
  $handler->display->display_options['sorts']['created']['table'] = 'node';
  $handler->display->display_options['sorts']['created']['field'] = 'created';
  $handler->display->display_options['sorts']['created']['granularity'] = 'minute';
  $handler->display->display_options['defaults']['filter_groups'] = FALSE;
  $handler->display->display_options['defaults']['filters'] = FALSE;
  /* Filter criterion: Content: Published */
  $handler->display->display_options['filters']['status']['id'] = 'status';
  $handler->display->display_options['filters']['status']['table'] = 'node';
  $handler->display->display_options['filters']['status']['field'] = 'status';
  $handler->display->display_options['filters']['status']['value'] = '1';
  $handler->display->display_options['filters']['status']['group'] = '0';
  $handler->display->display_options['filters']['status']['expose']['operator'] = FALSE;
  /* Filter criterion: Content: Type */
  $handler->display->display_options['filters']['type']['id'] = 'type';
  $handler->display->display_options['filters']['type']['table'] = 'node';
  $handler->display->display_options['filters']['type']['field'] = 'type';
  $handler->display->display_options['filters']['type']['value'] = array(
    'agnews' => 'agnews',
  );
  $handler->display->display_options['filters']['type']['group'] = '0';
  $handler->display->display_options['filters']['type']['expose']['operator'] = FALSE;
  $handler->display->display_options['path'] = 'ag_news/rdf';
  $handler->display->display_options['displays'] = array(
    'page_2' => 'page_2',
    'block_2' => 'block_2',
    'default' => 0,
    'page_1' => 0,
    'block_1' => 0,
  );
  $handler->display->display_options['sitename_title'] = 0;

  /* Display: News RSS Feed */
  $handler = $view->new_display('feed', 'News RSS Feed', 'feed_2');
  $handler->display->display_options['defaults']['title'] = FALSE;
  $handler->display->display_options['title'] = 'Our latest news';
  $handler->display->display_options['items_per_page'] = 0;
  $handler->display->display_options['pager']['type'] = 'some';
  $handler->display->display_options['style_plugin'] = 'rss';
  $handler->display->display_options['row_plugin'] = 'node_rss';
  $handler->display->display_options['defaults']['sorts'] = FALSE;
  /* Sort criterion: Content: Post date */
  $handler->display->display_options['sorts']['created']['id'] = 'created';
  $handler->display->display_options['sorts']['created']['table'] = 'node';
  $handler->display->display_options['sorts']['created']['field'] = 'created';
  $handler->display->display_options['defaults']['filter_groups'] = FALSE;
  $handler->display->display_options['defaults']['filters'] = FALSE;
  /* Filter criterion: Content: Published */
  $handler->display->display_options['filters']['status']['id'] = 'status';
  $handler->display->display_options['filters']['status']['table'] = 'node';
  $handler->display->display_options['filters']['status']['field'] = 'status';
  $handler->display->display_options['filters']['status']['value'] = '1';
  $handler->display->display_options['filters']['status']['group'] = '0';
  $handler->display->display_options['filters']['status']['expose']['operator'] = FALSE;
  /* Filter criterion: Content: Type */
  $handler->display->display_options['filters']['type']['id'] = 'type';
  $handler->display->display_options['filters']['type']['table'] = 'node';
  $handler->display->display_options['filters']['type']['field'] = 'type';
  $handler->display->display_options['filters']['type']['value'] = array(
    'agnews' => 'agnews',
  );
  $handler->display->display_options['filters']['type']['group'] = '0';
  $handler->display->display_options['filters']['type']['expose']['operator'] = FALSE;
  $handler->display->display_options['path'] = 'ag_news/rss';
  $handler->display->display_options['sitename_title'] = 0;
  $export['Ag_News'] = $view;

  $view = new view();
  $view->name = 'Ag_Upcoming_events';
  $view->description = '';
  $view->tag = '';
  $view->base_table = 'node';
  $view->human_name = '';
  $view->core = 0;
  $view->api_version = '3.0';
  $view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */

  /* Display: Defaults */
  $handler = $view->new_display('default', 'Defaults', 'default');
  $handler->display->display_options['title'] = 'Upcoming events';
  $handler->display->display_options['items_per_page'] = 0;
  $handler->display->display_options['use_more_always'] = TRUE;
  $handler->display->display_options['access']['type'] = 'none';
  $handler->display->display_options['cache']['type'] = 'none';
  $handler->display->display_options['query']['type'] = 'views_query';
  $handler->display->display_options['exposed_form']['type'] = 'basic';
  $handler->display->display_options['pager']['type'] = 'full';
  $handler->display->display_options['style_plugin'] = 'table';
  $handler->display->display_options['style_options']['grouping'] = '';
  $handler->display->display_options['style_options']['columns'] = array(
    'title' => 'title',
    'field_date_value' => 'field_date_value',
    'field_date_value2' => 'field_date_value2',
    'field_location_value' => 'field_location_value',
    'field_country_value_1' => 'field_country_value_1',
  );
  $handler->display->display_options['style_options']['default'] = 'field_date_value';
  $handler->display->display_options['style_options']['info'] = array(
    'title' => array(
      'sortable' => 1,
      'separator' => '',
    ),
    'field_date_value' => array(
      'sortable' => 1,
      'separator' => '',
    ),
    'field_date_value2' => array(
      'sortable' => 1,
      'separator' => '',
    ),
    'field_location_value' => array(
      'sortable' => 1,
      'separator' => '',
    ),
    'field_country_value_1' => array(
      'sortable' => 1,
      'separator' => '',
    ),
  );
  $handler->display->display_options['style_options']['order'] = 'desc';
  /* Header: Global: Text area */
  $handler->display->display_options['header']['text']['id'] = 'area';
  $handler->display->display_options['header']['text']['table'] = 'views';
  $handler->display->display_options['header']['text']['field'] = 'area';
  $handler->display->display_options['header']['text']['content'] = '<?php
global $base_url;
?>
<div align="right"><a href="<?php echo $base_url;?>/ag_events/rss" target="_blank"><img src="<?php echo $base_url;?>/sites/default/files/images/RSS_feed.png"> RSS feed</a><br /><i>[compliant with the <a href="ftp://ftp.fao.org/docrep/fao/010/ai255e/ai255e00.pdf" target="_blank">Ag-Event AP</a>]</i><br />
<a href="<?php echo $base_url;?>/ag_events/rdf" target="_blank"><img src="<?php echo $base_url;?>/sites/default/files/images/rdf-icon.gif"> RDF feed</a><br />&nbsp;</div>';
  $handler->display->display_options['header']['text']['format'] = '3';
  /* Field: Content: Title */
  $handler->display->display_options['fields']['title']['id'] = 'title';
  $handler->display->display_options['fields']['title']['table'] = 'node';
  $handler->display->display_options['fields']['title']['field'] = 'title';
  $handler->display->display_options['fields']['title']['label'] = '';
  $handler->display->display_options['fields']['title']['alter']['text'] = 'x';
  /* Sort criterion: Content: Date -  start date (field_ag_date) */
  $handler->display->display_options['sorts']['field_ag_date_value']['id'] = 'field_ag_date_value';
  $handler->display->display_options['sorts']['field_ag_date_value']['table'] = 'field_data_field_ag_date';
  $handler->display->display_options['sorts']['field_ag_date_value']['field'] = 'field_ag_date_value';
  $handler->display->display_options['sorts']['field_ag_date_value']['order'] = 'DESC';
  /* Filter criterion: Content: Published */
  $handler->display->display_options['filters']['status']['id'] = 'status';
  $handler->display->display_options['filters']['status']['table'] = 'node';
  $handler->display->display_options['filters']['status']['field'] = 'status';
  $handler->display->display_options['filters']['status']['value'] = '1';
  $handler->display->display_options['filters']['status']['group'] = '0';
  $handler->display->display_options['filters']['status']['expose']['operator'] = FALSE;
  /* Filter criterion: Content: Type */
  $handler->display->display_options['filters']['type']['id'] = 'type';
  $handler->display->display_options['filters']['type']['table'] = 'node';
  $handler->display->display_options['filters']['type']['field'] = 'type';
  $handler->display->display_options['filters']['type']['value'] = array(
    'agevent' => 'agevent',
  );
  $handler->display->display_options['filters']['type']['group'] = '0';
  $handler->display->display_options['filters']['type']['expose']['operator'] = FALSE;

  /* Display: Upcoming events */
  $handler = $view->new_display('page', 'Upcoming events', 'page_1');
  $handler->display->display_options['defaults']['sorts'] = FALSE;
  $handler->display->display_options['path'] = 'ag_upcoming-events';

  /* Display: Upcoming events block */
  $handler = $view->new_display('block', 'Upcoming events block', 'block_1');
  $handler->display->display_options['defaults']['use_more'] = FALSE;
  $handler->display->display_options['use_more'] = TRUE;
  $handler->display->display_options['defaults']['use_more_always'] = FALSE;
  $handler->display->display_options['defaults']['use_more_always'] = FALSE;
  $handler->display->display_options['use_more_always'] = TRUE;
  $handler->display->display_options['defaults']['use_more_text'] = FALSE;
  $handler->display->display_options['defaults']['style_plugin'] = FALSE;
  $handler->display->display_options['style_plugin'] = 'default';
  $handler->display->display_options['style_options']['grouping'] = '';
  $handler->display->display_options['defaults']['style_options'] = FALSE;
  $handler->display->display_options['defaults']['row_plugin'] = FALSE;
  $handler->display->display_options['row_plugin'] = 'fields';
  $handler->display->display_options['defaults']['row_options'] = FALSE;
  $handler->display->display_options['defaults']['empty'] = FALSE;
  /* No results behavior: Global: Text area */
  $handler->display->display_options['empty']['text']['id'] = 'area';
  $handler->display->display_options['empty']['text']['table'] = 'views';
  $handler->display->display_options['empty']['text']['field'] = 'area';
  $handler->display->display_options['empty']['text']['content'] = 'No upcoming events';
  $handler->display->display_options['empty']['text']['format'] = '1';
  $handler->display->display_options['defaults']['sorts'] = FALSE;
  /* Sort criterion: Broken/missing handler */
  $handler->display->display_options['sorts']['field_ag_date_value']['id'] = 'field_ag_date_value';
  $handler->display->display_options['sorts']['field_ag_date_value']['table'] = 'node_data_field_ag_date';
  $handler->display->display_options['sorts']['field_ag_date_value']['field'] = 'field_ag_date_value';
  $handler->display->display_options['defaults']['filter_groups'] = FALSE;
  $handler->display->display_options['defaults']['filters'] = FALSE;
  /* Filter criterion: Content: Published */
  $handler->display->display_options['filters']['status']['id'] = 'status';
  $handler->display->display_options['filters']['status']['table'] = 'node';
  $handler->display->display_options['filters']['status']['field'] = 'status';
  $handler->display->display_options['filters']['status']['value'] = '1';
  $handler->display->display_options['filters']['status']['group'] = '0';
  $handler->display->display_options['filters']['status']['expose']['operator'] = FALSE;
  /* Filter criterion: Content: Type */
  $handler->display->display_options['filters']['type']['id'] = 'type';
  $handler->display->display_options['filters']['type']['table'] = 'node';
  $handler->display->display_options['filters']['type']['field'] = 'type';
  $handler->display->display_options['filters']['type']['value'] = array(
    'agevent' => 'agevent',
  );
  $handler->display->display_options['filters']['type']['group'] = '0';
  $handler->display->display_options['filters']['type']['expose']['operator'] = FALSE;
  /* Filter criterion: Date: Date (node) */
  $handler->display->display_options['filters']['date_filter']['id'] = 'date_filter';
  $handler->display->display_options['filters']['date_filter']['table'] = 'node';
  $handler->display->display_options['filters']['date_filter']['field'] = 'date_filter';
  $handler->display->display_options['filters']['date_filter']['operator'] = '>=';
  $handler->display->display_options['filters']['date_filter']['value']['min'] = array(
    'date' => '',
  );
  $handler->display->display_options['filters']['date_filter']['value']['max'] = array(
    'date' => '',
  );
  $handler->display->display_options['filters']['date_filter']['value']['value'] = array(
    'date' => '',
  );
  $handler->display->display_options['filters']['date_filter']['group'] = '0';
  $handler->display->display_options['filters']['date_filter']['expose']['operator'] = FALSE;
  $handler->display->display_options['filters']['date_filter']['form_type'] = 'date_text';
  $handler->display->display_options['filters']['date_filter']['default_date'] = 'now';
  $handler->display->display_options['filters']['date_filter']['date_fields'] = array(
    'node_data_field_date.field_date_value' => 'node_data_field_date.field_date_value',
  );

  /* Display: All events page */
  $handler = $view->new_display('page', 'All events page', 'page_2');
  $handler->display->display_options['defaults']['title'] = FALSE;
  $handler->display->display_options['title'] = 'All our events';
  $handler->display->display_options['defaults']['sorts'] = FALSE;
  /* Sort criterion: Broken/missing handler */
  $handler->display->display_options['sorts']['field_ag_date_value']['id'] = 'field_ag_date_value';
  $handler->display->display_options['sorts']['field_ag_date_value']['table'] = 'node_data_field_ag_date';
  $handler->display->display_options['sorts']['field_ag_date_value']['field'] = 'field_ag_date_value';
  $handler->display->display_options['defaults']['filter_groups'] = FALSE;
  $handler->display->display_options['defaults']['filters'] = FALSE;
  /* Filter criterion: Content: Published */
  $handler->display->display_options['filters']['status']['id'] = 'status';
  $handler->display->display_options['filters']['status']['table'] = 'node';
  $handler->display->display_options['filters']['status']['field'] = 'status';
  $handler->display->display_options['filters']['status']['value'] = '1';
  $handler->display->display_options['filters']['status']['group'] = '0';
  $handler->display->display_options['filters']['status']['expose']['operator'] = FALSE;
  /* Filter criterion: Content: Type */
  $handler->display->display_options['filters']['type']['id'] = 'type';
  $handler->display->display_options['filters']['type']['table'] = 'node';
  $handler->display->display_options['filters']['type']['field'] = 'type';
  $handler->display->display_options['filters']['type']['value'] = array(
    'agevent' => 'agevent',
  );
  $handler->display->display_options['filters']['type']['group'] = '0';
  $handler->display->display_options['filters']['type']['expose']['operator'] = FALSE;
  $handler->display->display_options['path'] = 'ag_all-events';

  /* Display: All events block */
  $handler = $view->new_display('block', 'All events block', 'block_2');
  $handler->display->display_options['defaults']['title'] = FALSE;
  $handler->display->display_options['title'] = 'All our events';
  $handler->display->display_options['defaults']['use_more'] = FALSE;
  $handler->display->display_options['use_more'] = TRUE;
  $handler->display->display_options['defaults']['use_more_always'] = FALSE;
  $handler->display->display_options['defaults']['use_more_always'] = FALSE;
  $handler->display->display_options['use_more_always'] = TRUE;
  $handler->display->display_options['defaults']['use_more_text'] = FALSE;
  $handler->display->display_options['defaults']['link_display'] = FALSE;
  $handler->display->display_options['link_display'] = 'page_2';
  $handler->display->display_options['defaults']['style_plugin'] = FALSE;
  $handler->display->display_options['style_plugin'] = 'default';
  $handler->display->display_options['style_options']['grouping'] = '';
  $handler->display->display_options['defaults']['style_options'] = FALSE;
  $handler->display->display_options['defaults']['row_plugin'] = FALSE;
  $handler->display->display_options['row_plugin'] = 'fields';
  $handler->display->display_options['row_options']['inline'] = array(
    'field_date_value' => 'field_date_value',
    'field_date_value2' => 'field_date_value2',
    'field_location_value' => 'field_location_value',
    'field_country_value_1' => 'field_country_value_1',
  );
  $handler->display->display_options['row_options']['separator'] = ' - ';
  $handler->display->display_options['defaults']['row_options'] = FALSE;
  $handler->display->display_options['defaults']['fields'] = FALSE;
  /* Field: Content: Title */
  $handler->display->display_options['fields']['title']['id'] = 'title';
  $handler->display->display_options['fields']['title']['table'] = 'node';
  $handler->display->display_options['fields']['title']['field'] = 'title';
  $handler->display->display_options['fields']['title']['label'] = '';
  /* Field: Broken/missing handler */
  $handler->display->display_options['fields']['field_ag_date_value']['id'] = 'field_ag_date_value';
  $handler->display->display_options['fields']['field_ag_date_value']['table'] = 'node_data_field_ag_date';
  $handler->display->display_options['fields']['field_ag_date_value']['field'] = 'field_ag_date_value';
  /* Field: Broken/missing handler */
  $handler->display->display_options['fields']['field_ag_date_value2']['id'] = 'field_ag_date_value2';
  $handler->display->display_options['fields']['field_ag_date_value2']['table'] = 'node_data_field_ag_date';
  $handler->display->display_options['fields']['field_ag_date_value2']['field'] = 'field_ag_date_value2';
  /* Field: Broken/missing handler */
  $handler->display->display_options['fields']['field_ag_location_value']['id'] = 'field_ag_location_value';
  $handler->display->display_options['fields']['field_ag_location_value']['table'] = 'node_data_field_ag_location';
  $handler->display->display_options['fields']['field_ag_location_value']['field'] = 'field_ag_location_value';
  /* Field: Broken/missing handler */
  $handler->display->display_options['fields']['field_ag_country_value']['id'] = 'field_ag_country_value';
  $handler->display->display_options['fields']['field_ag_country_value']['table'] = 'node_data_field_ag_country';
  $handler->display->display_options['fields']['field_ag_country_value']['field'] = 'field_ag_country_value';
  $handler->display->display_options['defaults']['filter_groups'] = FALSE;
  $handler->display->display_options['defaults']['filters'] = FALSE;
  /* Filter criterion: Content: Published */
  $handler->display->display_options['filters']['status']['id'] = 'status';
  $handler->display->display_options['filters']['status']['table'] = 'node';
  $handler->display->display_options['filters']['status']['field'] = 'status';
  $handler->display->display_options['filters']['status']['value'] = '1';
  $handler->display->display_options['filters']['status']['group'] = '0';
  $handler->display->display_options['filters']['status']['expose']['operator'] = FALSE;
  /* Filter criterion: Content: Type */
  $handler->display->display_options['filters']['type']['id'] = 'type';
  $handler->display->display_options['filters']['type']['table'] = 'node';
  $handler->display->display_options['filters']['type']['field'] = 'type';
  $handler->display->display_options['filters']['type']['value'] = array(
    'agevent' => 'agevent',
  );
  $handler->display->display_options['filters']['type']['group'] = '0';
  $handler->display->display_options['filters']['type']['expose']['operator'] = FALSE;

  /* Display: Feed RDF */
  $handler = $view->new_display('feed', 'Feed RDF', 'feed_1');
  $handler->display->display_options['defaults']['title'] = FALSE;
  $handler->display->display_options['title'] = 'All events';
  $handler->display->display_options['pager']['type'] = 'some';
  $handler->display->display_options['style_plugin'] = 'rss';
  $handler->display->display_options['row_plugin'] = 'node_rss';
  $handler->display->display_options['defaults']['filter_groups'] = FALSE;
  $handler->display->display_options['defaults']['filters'] = FALSE;
  /* Filter criterion: Content: Published */
  $handler->display->display_options['filters']['status']['id'] = 'status';
  $handler->display->display_options['filters']['status']['table'] = 'node';
  $handler->display->display_options['filters']['status']['field'] = 'status';
  $handler->display->display_options['filters']['status']['value'] = '1';
  $handler->display->display_options['filters']['status']['group'] = '0';
  $handler->display->display_options['filters']['status']['expose']['operator'] = FALSE;
  /* Filter criterion: Content: Type */
  $handler->display->display_options['filters']['type']['id'] = 'type';
  $handler->display->display_options['filters']['type']['table'] = 'node';
  $handler->display->display_options['filters']['type']['field'] = 'type';
  $handler->display->display_options['filters']['type']['value'] = array(
    'agevent' => 'agevent',
  );
  $handler->display->display_options['filters']['type']['group'] = '0';
  $handler->display->display_options['filters']['type']['expose']['operator'] = FALSE;
  $handler->display->display_options['path'] = 'ag_events/rdf';
  $handler->display->display_options['sitename_title'] = 0;

  /* Display: Feed RSS */
  $handler = $view->new_display('feed', 'Feed RSS', 'feed_2');
  $handler->display->display_options['defaults']['title'] = FALSE;
  $handler->display->display_options['title'] = 'All events';
  $handler->display->display_options['pager']['type'] = 'some';
  $handler->display->display_options['style_plugin'] = 'rss';
  $handler->display->display_options['style_options']['description'] = 'Events';
  $handler->display->display_options['row_plugin'] = 'node_rss';
  $handler->display->display_options['defaults']['filter_groups'] = FALSE;
  $handler->display->display_options['defaults']['filters'] = FALSE;
  /* Filter criterion: Content: Published */
  $handler->display->display_options['filters']['status']['id'] = 'status';
  $handler->display->display_options['filters']['status']['table'] = 'node';
  $handler->display->display_options['filters']['status']['field'] = 'status';
  $handler->display->display_options['filters']['status']['value'] = '1';
  $handler->display->display_options['filters']['status']['group'] = '0';
  $handler->display->display_options['filters']['status']['expose']['operator'] = FALSE;
  /* Filter criterion: Content: Type */
  $handler->display->display_options['filters']['type']['id'] = 'type';
  $handler->display->display_options['filters']['type']['table'] = 'node';
  $handler->display->display_options['filters']['type']['field'] = 'type';
  $handler->display->display_options['filters']['type']['value'] = array(
    'agevent' => 'agevent',
  );
  $handler->display->display_options['filters']['type']['group'] = '0';
  $handler->display->display_options['filters']['type']['expose']['operator'] = FALSE;
  $handler->display->display_options['path'] = 'ag_events/rss';
  $handler->display->display_options['sitename_title'] = 0;

  /* Display: Event calendar page */
  $handler = $view->new_display('page', 'Event calendar page', 'page_3');
  $handler->display->display_options['defaults']['title'] = FALSE;
  $handler->display->display_options['title'] = 'Events calendar';
  $handler->display->display_options['defaults']['style_plugin'] = FALSE;
  $handler->display->display_options['style_plugin'] = 'calendar_style';
  $handler->display->display_options['defaults']['style_options'] = FALSE;
  $handler->display->display_options['defaults']['row_plugin'] = FALSE;
  $handler->display->display_options['row_plugin'] = 'fields';
  $handler->display->display_options['defaults']['row_options'] = FALSE;
  $handler->display->display_options['defaults']['header'] = FALSE;
  $handler->display->display_options['defaults']['fields'] = FALSE;
  /* Field: Content: Title */
  $handler->display->display_options['fields']['title']['id'] = 'title';
  $handler->display->display_options['fields']['title']['table'] = 'node';
  $handler->display->display_options['fields']['title']['field'] = 'title';
  $handler->display->display_options['fields']['title']['label'] = '';
  /* Field: Broken/missing handler */
  $handler->display->display_options['fields']['field_ag_date_value']['id'] = 'field_ag_date_value';
  $handler->display->display_options['fields']['field_ag_date_value']['table'] = 'node_data_field_ag_date';
  $handler->display->display_options['fields']['field_ag_date_value']['field'] = 'field_ag_date_value';
  $handler->display->display_options['fields']['field_ag_date_value']['label'] = 'From';
  /* Field: Broken/missing handler */
  $handler->display->display_options['fields']['field_ag_date_value2']['id'] = 'field_ag_date_value2';
  $handler->display->display_options['fields']['field_ag_date_value2']['table'] = 'node_data_field_ag_date';
  $handler->display->display_options['fields']['field_ag_date_value2']['field'] = 'field_ag_date_value2';
  $handler->display->display_options['fields']['field_ag_date_value2']['label'] = 'To';
  /* Field: Broken/missing handler */
  $handler->display->display_options['fields']['field_ag_location_value']['id'] = 'field_ag_location_value';
  $handler->display->display_options['fields']['field_ag_location_value']['table'] = 'node_data_field_ag_location';
  $handler->display->display_options['fields']['field_ag_location_value']['field'] = 'field_ag_location_value';
  $handler->display->display_options['fields']['field_ag_location_value']['label'] = 'Location';
  /* Field: Broken/missing handler */
  $handler->display->display_options['fields']['field_ag_country_value']['id'] = 'field_ag_country_value';
  $handler->display->display_options['fields']['field_ag_country_value']['table'] = 'node_data_field_ag_country';
  $handler->display->display_options['fields']['field_ag_country_value']['field'] = 'field_ag_country_value';
  $handler->display->display_options['fields']['field_ag_country_value']['label'] = 'Country';
  $handler->display->display_options['defaults']['sorts'] = FALSE;
  $handler->display->display_options['defaults']['arguments'] = FALSE;
  /* Contextual filter: Date: Date (node) */
  $handler->display->display_options['arguments']['date_argument']['id'] = 'date_argument';
  $handler->display->display_options['arguments']['date_argument']['table'] = 'node';
  $handler->display->display_options['arguments']['date_argument']['field'] = 'date_argument';
  $handler->display->display_options['arguments']['date_argument']['default_action'] = 'default';
  $handler->display->display_options['arguments']['date_argument']['exception']['title_enable'] = TRUE;
  $handler->display->display_options['arguments']['date_argument']['title_enable'] = TRUE;
  $handler->display->display_options['arguments']['date_argument']['title'] = 'Calendar of events %1';
  $handler->display->display_options['arguments']['date_argument']['summary']['format'] = 'default_summary';
  $handler->display->display_options['arguments']['date_argument']['specify_validation'] = TRUE;
  $handler->display->display_options['arguments']['date_argument']['date_fields'] = array(
    'node_data_field_ag_date.field_ag_date_value' => 'node_data_field_ag_date.field_ag_date_value',
  );
  $handler->display->display_options['arguments']['date_argument']['date_method'] = 'AND';
  $handler->display->display_options['defaults']['filter_groups'] = FALSE;
  $handler->display->display_options['defaults']['filters'] = FALSE;
  /* Filter criterion: Content: Published */
  $handler->display->display_options['filters']['status']['id'] = 'status';
  $handler->display->display_options['filters']['status']['table'] = 'node';
  $handler->display->display_options['filters']['status']['field'] = 'status';
  $handler->display->display_options['filters']['status']['value'] = '1';
  $handler->display->display_options['filters']['status']['group'] = '0';
  $handler->display->display_options['filters']['status']['expose']['operator'] = FALSE;
  /* Filter criterion: Content: Type */
  $handler->display->display_options['filters']['type']['id'] = 'type';
  $handler->display->display_options['filters']['type']['table'] = 'node';
  $handler->display->display_options['filters']['type']['field'] = 'type';
  $handler->display->display_options['filters']['type']['value'] = array(
    'agevent' => 'agevent',
  );
  $handler->display->display_options['filters']['type']['group'] = '0';
  $handler->display->display_options['filters']['type']['expose']['operator'] = FALSE;
  $handler->display->display_options['path'] = 'ag_all-events-calendar';

  /* Display: Date browser */
  $handler = $view->new_display('date_nav', 'Date browser', 'date_nav_1');

  /* Display: Calendar Block */
  $handler = $view->new_display('block', 'Calendar Block', 'block_3');
  $handler->display->display_options['defaults']['title'] = FALSE;
  $handler->display->display_options['title'] = 'Calendar';
  $handler->display->display_options['defaults']['use_more'] = FALSE;
  $handler->display->display_options['use_more'] = TRUE;
  $handler->display->display_options['defaults']['use_more_always'] = FALSE;
  $handler->display->display_options['defaults']['use_more_always'] = FALSE;
  $handler->display->display_options['use_more_always'] = TRUE;
  $handler->display->display_options['defaults']['use_more_text'] = FALSE;
  $handler->display->display_options['defaults']['link_display'] = FALSE;
  $handler->display->display_options['link_display'] = 'page_3';
  $handler->display->display_options['defaults']['style_plugin'] = FALSE;
  $handler->display->display_options['style_plugin'] = 'calendar_style';
  $handler->display->display_options['defaults']['style_options'] = FALSE;
  $handler->display->display_options['defaults']['row_plugin'] = FALSE;
  $handler->display->display_options['row_plugin'] = 'fields';
  $handler->display->display_options['defaults']['row_options'] = FALSE;
  $handler->display->display_options['defaults']['header'] = FALSE;
  $handler->display->display_options['defaults']['fields'] = FALSE;
  /* Field: Content: Title */
  $handler->display->display_options['fields']['title']['id'] = 'title';
  $handler->display->display_options['fields']['title']['table'] = 'node';
  $handler->display->display_options['fields']['title']['field'] = 'title';
  $handler->display->display_options['fields']['title']['label'] = '';
  $handler->display->display_options['fields']['title']['alter']['alter_text'] = TRUE;
  $handler->display->display_options['fields']['title']['alter']['text'] = 'x';
  /* Field: Broken/missing handler */
  $handler->display->display_options['fields']['field_ag_date_value']['id'] = 'field_ag_date_value';
  $handler->display->display_options['fields']['field_ag_date_value']['table'] = 'node_data_field_ag_date';
  $handler->display->display_options['fields']['field_ag_date_value']['field'] = 'field_ag_date_value';
  $handler->display->display_options['fields']['field_ag_date_value']['label'] = 'From';
  /* Field: Broken/missing handler */
  $handler->display->display_options['fields']['field_ag_date_value2']['id'] = 'field_ag_date_value2';
  $handler->display->display_options['fields']['field_ag_date_value2']['table'] = 'node_data_field_ag_date';
  $handler->display->display_options['fields']['field_ag_date_value2']['field'] = 'field_ag_date_value2';
  $handler->display->display_options['fields']['field_ag_date_value2']['label'] = 'From';
  /* Field: Broken/missing handler */
  $handler->display->display_options['fields']['field_ag_country_value']['id'] = 'field_ag_country_value';
  $handler->display->display_options['fields']['field_ag_country_value']['table'] = 'node_data_field_ag_country';
  $handler->display->display_options['fields']['field_ag_country_value']['field'] = 'field_ag_country_value';
  $handler->display->display_options['fields']['field_ag_country_value']['label'] = 'Country';
  /* Field: Broken/missing handler */
  $handler->display->display_options['fields']['field_ag_location_value']['id'] = 'field_ag_location_value';
  $handler->display->display_options['fields']['field_ag_location_value']['table'] = 'node_data_field_ag_location';
  $handler->display->display_options['fields']['field_ag_location_value']['field'] = 'field_ag_location_value';
  $handler->display->display_options['fields']['field_ag_location_value']['label'] = 'Location';
  $handler->display->display_options['defaults']['sorts'] = FALSE;
  $handler->display->display_options['defaults']['arguments'] = FALSE;
  /* Contextual filter: Date: Date (node) */
  $handler->display->display_options['arguments']['date_argument']['id'] = 'date_argument';
  $handler->display->display_options['arguments']['date_argument']['table'] = 'node';
  $handler->display->display_options['arguments']['date_argument']['field'] = 'date_argument';
  $handler->display->display_options['arguments']['date_argument']['default_action'] = 'default';
  $handler->display->display_options['arguments']['date_argument']['exception']['title_enable'] = TRUE;
  $handler->display->display_options['arguments']['date_argument']['summary']['format'] = 'default_summary';
  $handler->display->display_options['arguments']['date_argument']['specify_validation'] = TRUE;
  $handler->display->display_options['arguments']['date_argument']['date_fields'] = array(
    'node_data_field_ag_date.field_ag_date_value' => 'node_data_field_ag_date.field_ag_date_value',
  );
  $handler->display->display_options['defaults']['filter_groups'] = FALSE;
  $handler->display->display_options['defaults']['filters'] = FALSE;
  /* Filter criterion: Content: Published */
  $handler->display->display_options['filters']['status']['id'] = 'status';
  $handler->display->display_options['filters']['status']['table'] = 'node';
  $handler->display->display_options['filters']['status']['field'] = 'status';
  $handler->display->display_options['filters']['status']['value'] = '1';
  $handler->display->display_options['filters']['status']['group'] = '0';
  $handler->display->display_options['filters']['status']['expose']['operator'] = FALSE;
  /* Filter criterion: Content: Type */
  $handler->display->display_options['filters']['type']['id'] = 'type';
  $handler->display->display_options['filters']['type']['table'] = 'node';
  $handler->display->display_options['filters']['type']['field'] = 'type';
  $handler->display->display_options['filters']['type']['value'] = array(
    'agevent' => 'agevent',
  );
  $handler->display->display_options['filters']['type']['group'] = '0';
  $handler->display->display_options['filters']['type']['expose']['operator'] = FALSE;
  $export['Ag_Upcoming_events'] = $view;

  $view = new view();
  $view->name = 'Ag_Upcoming_exevents';
  $view->description = '';
  $view->tag = '';
  $view->base_table = 'node';
  $view->human_name = '';
  $view->core = 0;
  $view->api_version = '3.0';
  $view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */

  /* Display: Defaults */
  $handler = $view->new_display('default', 'Defaults', 'default');
  $handler->display->display_options['title'] = 'Upcoming events from partners';
  $handler->display->display_options['use_more_always'] = FALSE;
  $handler->display->display_options['access']['type'] = 'none';
  $handler->display->display_options['cache']['type'] = 'none';
  $handler->display->display_options['query']['type'] = 'views_query';
  $handler->display->display_options['exposed_form']['type'] = 'basic';
  $handler->display->display_options['pager']['type'] = 'full';
  $handler->display->display_options['style_plugin'] = 'table';
  $handler->display->display_options['style_options']['grouping'] = '';
  $handler->display->display_options['style_options']['columns'] = array(
    'title' => 'title',
    'field_date_value' => 'field_date_value',
    'field_date_value2' => 'field_date_value2',
    'field_location_value' => 'field_location_value',
    'field_country_value' => 'field_country_value',
    'path' => 'path',
    'title_1' => 'title',
  );
  $handler->display->display_options['style_options']['default'] = 'field_date_value';
  $handler->display->display_options['style_options']['info'] = array(
    'title' => array(
      'sortable' => 0,
      'separator' => '<br /> ',
    ),
    'field_date_value' => array(
      'sortable' => 1,
      'separator' => '',
    ),
    'field_date_value2' => array(
      'sortable' => 1,
      'separator' => '',
    ),
    'field_location_value' => array(
      'sortable' => 0,
      'separator' => '',
    ),
    'field_country_value' => array(
      'sortable' => 1,
      'separator' => '',
    ),
    'path' => array(
      'separator' => '',
    ),
    'title_1' => array(
      'sortable' => 0,
      'separator' => '',
    ),
  );
  $handler->display->display_options['style_options']['order'] = 'desc';
  /* Relationship: Broken/missing handler */
  $handler->display->display_options['relationships']['feed_nid']['id'] = 'feed_nid';
  $handler->display->display_options['relationships']['feed_nid']['table'] = 'feeds_node_item';
  $handler->display->display_options['relationships']['feed_nid']['field'] = 'feed_nid';
  $handler->display->display_options['relationships']['feed_nid']['label'] = 'Owner feed';
  $handler->display->display_options['relationships']['feed_nid']['required'] = TRUE;
  /* Field: Content: Title */
  $handler->display->display_options['fields']['title']['id'] = 'title';
  $handler->display->display_options['fields']['title']['table'] = 'node';
  $handler->display->display_options['fields']['title']['field'] = 'title';
  $handler->display->display_options['fields']['title']['label'] = 'Name';
  /* Field: Broken/missing handler */
  $handler->display->display_options['fields']['field_ag_date_value2']['id'] = 'field_ag_date_value2';
  $handler->display->display_options['fields']['field_ag_date_value2']['table'] = 'node_data_field_ag_date';
  $handler->display->display_options['fields']['field_ag_date_value2']['field'] = 'field_ag_date_value2';
  $handler->display->display_options['fields']['field_ag_date_value2']['label'] = 'To';
  /* Field: Broken/missing handler */
  $handler->display->display_options['fields']['field_ag_date_value']['id'] = 'field_ag_date_value';
  $handler->display->display_options['fields']['field_ag_date_value']['table'] = 'node_data_field_ag_date';
  $handler->display->display_options['fields']['field_ag_date_value']['field'] = 'field_ag_date_value';
  $handler->display->display_options['fields']['field_ag_date_value']['label'] = 'From';
  /* Field: Broken/missing handler */
  $handler->display->display_options['fields']['field_ag_location_value']['id'] = 'field_ag_location_value';
  $handler->display->display_options['fields']['field_ag_location_value']['table'] = 'node_data_field_ag_location';
  $handler->display->display_options['fields']['field_ag_location_value']['field'] = 'field_ag_location_value';
  $handler->display->display_options['fields']['field_ag_location_value']['label'] = 'Location';
  /* Field: Broken/missing handler */
  $handler->display->display_options['fields']['field_ag_country_value']['id'] = 'field_ag_country_value';
  $handler->display->display_options['fields']['field_ag_country_value']['table'] = 'node_data_field_ag_country';
  $handler->display->display_options['fields']['field_ag_country_value']['field'] = 'field_ag_country_value';
  $handler->display->display_options['fields']['field_ag_country_value']['label'] = 'Country';
  /* Field: Content: Path */
  $handler->display->display_options['fields']['path']['id'] = 'path';
  $handler->display->display_options['fields']['path']['table'] = 'node';
  $handler->display->display_options['fields']['path']['field'] = 'path';
  $handler->display->display_options['fields']['path']['relationship'] = 'feed_nid';
  $handler->display->display_options['fields']['path']['label'] = '';
  $handler->display->display_options['fields']['path']['exclude'] = TRUE;
  /* Field: Content: Title */
  $handler->display->display_options['fields']['title_1']['id'] = 'title_1';
  $handler->display->display_options['fields']['title_1']['table'] = 'node';
  $handler->display->display_options['fields']['title_1']['field'] = 'title';
  $handler->display->display_options['fields']['title_1']['relationship'] = 'feed_nid';
  $handler->display->display_options['fields']['title_1']['label'] = '';
  $handler->display->display_options['fields']['title_1']['alter']['alter_text'] = TRUE;
  $handler->display->display_options['fields']['title_1']['alter']['text'] = '<i>source: <a href="[path]"><span>[title_1]</span></a></i>';
  $handler->display->display_options['fields']['title_1']['link_to_node'] = FALSE;
  /* Sort criterion: Broken/missing handler */
  $handler->display->display_options['sorts']['field_ag_date_value']['id'] = 'field_ag_date_value';
  $handler->display->display_options['sorts']['field_ag_date_value']['table'] = 'node_data_field_ag_date';
  $handler->display->display_options['sorts']['field_ag_date_value']['field'] = 'field_ag_date_value';
  /* Filter criterion: Content: Published */
  $handler->display->display_options['filters']['status']['id'] = 'status';
  $handler->display->display_options['filters']['status']['table'] = 'node';
  $handler->display->display_options['filters']['status']['field'] = 'status';
  $handler->display->display_options['filters']['status']['value'] = '1';
  $handler->display->display_options['filters']['status']['group'] = '0';
  $handler->display->display_options['filters']['status']['expose']['operator'] = FALSE;
  /* Filter criterion: Content: Type */
  $handler->display->display_options['filters']['type']['id'] = 'type';
  $handler->display->display_options['filters']['type']['table'] = 'node';
  $handler->display->display_options['filters']['type']['field'] = 'type';
  $handler->display->display_options['filters']['type']['value'] = array(
    'agevent' => 'agevent',
  );
  $handler->display->display_options['filters']['type']['group'] = '0';
  $handler->display->display_options['filters']['type']['expose']['operator'] = FALSE;
  /* Filter criterion: Date: Date (node) */
  $handler->display->display_options['filters']['date_filter']['id'] = 'date_filter';
  $handler->display->display_options['filters']['date_filter']['table'] = 'node';
  $handler->display->display_options['filters']['date_filter']['field'] = 'date_filter';
  $handler->display->display_options['filters']['date_filter']['operator'] = '>=';
  $handler->display->display_options['filters']['date_filter']['group'] = '0';
  $handler->display->display_options['filters']['date_filter']['expose']['operator'] = FALSE;
  $handler->display->display_options['filters']['date_filter']['form_type'] = 'date_text';
  $handler->display->display_options['filters']['date_filter']['default_date'] = 'now';
  $handler->display->display_options['filters']['date_filter']['default_to_date'] = 'now';
  $handler->display->display_options['filters']['date_filter']['date_fields'] = array(
    'node_data_field_date.field_date_value' => 'node_data_field_date.field_date_value',
    'node_data_field_date.field_date_value2' => 'node_data_field_date.field_date_value2',
  );
  /* Filter criterion: Search: Search Terms */
  $handler->display->display_options['filters']['keys']['id'] = 'keys';
  $handler->display->display_options['filters']['keys']['table'] = 'search_index';
  $handler->display->display_options['filters']['keys']['field'] = 'keys';
  $handler->display->display_options['filters']['keys']['group'] = '0';
  $handler->display->display_options['filters']['keys']['exposed'] = TRUE;
  $handler->display->display_options['filters']['keys']['expose']['operator_id'] = 'keys_op';
  $handler->display->display_options['filters']['keys']['expose']['label'] = 'Search';
  $handler->display->display_options['filters']['keys']['expose']['operator'] = 'keys_op';
  $handler->display->display_options['filters']['keys']['expose']['identifier'] = 'keys';
  /* Filter criterion: Content: Title */
  $handler->display->display_options['filters']['title']['id'] = 'title';
  $handler->display->display_options['filters']['title']['table'] = 'node';
  $handler->display->display_options['filters']['title']['field'] = 'title';
  $handler->display->display_options['filters']['title']['relationship'] = 'feed_nid';
  $handler->display->display_options['filters']['title']['operator'] = 'contains';
  $handler->display->display_options['filters']['title']['group'] = '0';
  $handler->display->display_options['filters']['title']['exposed'] = TRUE;
  $handler->display->display_options['filters']['title']['expose']['operator_id'] = 'title_op';
  $handler->display->display_options['filters']['title']['expose']['label'] = 'Search feed title';
  $handler->display->display_options['filters']['title']['expose']['operator'] = 'title_op';
  $handler->display->display_options['filters']['title']['expose']['identifier'] = 'title';

  /* Display: Upcoming events from partners page */
  $handler = $view->new_display('page', 'Upcoming events from partners page', 'page_1');
  $handler->display->display_options['defaults']['sorts'] = FALSE;
  /* Sort criterion: Broken/missing handler */
  $handler->display->display_options['sorts']['field_ag_date_value']['id'] = 'field_ag_date_value';
  $handler->display->display_options['sorts']['field_ag_date_value']['table'] = 'node_data_field_ag_date';
  $handler->display->display_options['sorts']['field_ag_date_value']['field'] = 'field_ag_date_value';
  $handler->display->display_options['defaults']['filter_groups'] = FALSE;
  $handler->display->display_options['defaults']['filters'] = FALSE;
  /* Filter criterion: Content: Published */
  $handler->display->display_options['filters']['status']['id'] = 'status';
  $handler->display->display_options['filters']['status']['table'] = 'node';
  $handler->display->display_options['filters']['status']['field'] = 'status';
  $handler->display->display_options['filters']['status']['value'] = '1';
  $handler->display->display_options['filters']['status']['group'] = '0';
  $handler->display->display_options['filters']['status']['expose']['operator'] = FALSE;
  /* Filter criterion: Content: Type */
  $handler->display->display_options['filters']['type']['id'] = 'type';
  $handler->display->display_options['filters']['type']['table'] = 'node';
  $handler->display->display_options['filters']['type']['field'] = 'type';
  $handler->display->display_options['filters']['type']['value'] = array(
    'agevent' => 'agevent',
  );
  $handler->display->display_options['filters']['type']['group'] = '0';
  $handler->display->display_options['filters']['type']['expose']['operator'] = FALSE;
  /* Filter criterion: Date: Date (node) */
  $handler->display->display_options['filters']['date_filter']['id'] = 'date_filter';
  $handler->display->display_options['filters']['date_filter']['table'] = 'node';
  $handler->display->display_options['filters']['date_filter']['field'] = 'date_filter';
  $handler->display->display_options['filters']['date_filter']['operator'] = '>=';
  $handler->display->display_options['filters']['date_filter']['value']['min'] = array(
    'date' => '',
  );
  $handler->display->display_options['filters']['date_filter']['value']['max'] = array(
    'date' => '',
  );
  $handler->display->display_options['filters']['date_filter']['value']['value'] = array(
    'date' => '',
  );
  $handler->display->display_options['filters']['date_filter']['group'] = '0';
  $handler->display->display_options['filters']['date_filter']['expose']['operator'] = FALSE;
  $handler->display->display_options['filters']['date_filter']['form_type'] = 'date_text';
  $handler->display->display_options['filters']['date_filter']['default_date'] = 'now';
  $handler->display->display_options['filters']['date_filter']['default_to_date'] = 'now';
  $handler->display->display_options['filters']['date_filter']['date_fields'] = array(
    'node_data_field_date.field_date_value' => 'node_data_field_date.field_date_value',
    'node_data_field_date.field_date_value2' => 'node_data_field_date.field_date_value2',
  );
  $handler->display->display_options['path'] = 'ag_upcoming-partner-events';

  /* Display: Upcoming events from partner block */
  $handler = $view->new_display('block', 'Upcoming events from partner block', 'block_1');
  $handler->display->display_options['defaults']['use_more'] = FALSE;
  $handler->display->display_options['use_more'] = TRUE;
  $handler->display->display_options['defaults']['use_more_always'] = FALSE;
  $handler->display->display_options['defaults']['use_more_always'] = FALSE;
  $handler->display->display_options['use_more_always'] = FALSE;
  $handler->display->display_options['defaults']['use_more_text'] = FALSE;
  $handler->display->display_options['defaults']['fields'] = FALSE;
  /* Field: Content: Title */
  $handler->display->display_options['fields']['title']['id'] = 'title';
  $handler->display->display_options['fields']['title']['table'] = 'node';
  $handler->display->display_options['fields']['title']['field'] = 'title';
  $handler->display->display_options['fields']['title']['label'] = 'Name';
  /* Field: Broken/missing handler */
  $handler->display->display_options['fields']['field_ag_date_value']['id'] = 'field_ag_date_value';
  $handler->display->display_options['fields']['field_ag_date_value']['table'] = 'node_data_field_ag_date';
  $handler->display->display_options['fields']['field_ag_date_value']['field'] = 'field_ag_date_value';
  /* Field: Broken/missing handler */
  $handler->display->display_options['fields']['field_ag_date_value2']['id'] = 'field_ag_date_value2';
  $handler->display->display_options['fields']['field_ag_date_value2']['table'] = 'node_data_field_ag_date';
  $handler->display->display_options['fields']['field_ag_date_value2']['field'] = 'field_ag_date_value2';
  /* Field: Broken/missing handler */
  $handler->display->display_options['fields']['field_ag_location_value']['id'] = 'field_ag_location_value';
  $handler->display->display_options['fields']['field_ag_location_value']['table'] = 'node_data_field_ag_location';
  $handler->display->display_options['fields']['field_ag_location_value']['field'] = 'field_ag_location_value';
  /* Field: Broken/missing handler */
  $handler->display->display_options['fields']['field_ag_country_value']['id'] = 'field_ag_country_value';
  $handler->display->display_options['fields']['field_ag_country_value']['table'] = 'node_data_field_ag_country';
  $handler->display->display_options['fields']['field_ag_country_value']['field'] = 'field_ag_country_value';
  $handler->display->display_options['defaults']['sorts'] = FALSE;
  /* Sort criterion: Broken/missing handler */
  $handler->display->display_options['sorts']['field_ag_date_value']['id'] = 'field_ag_date_value';
  $handler->display->display_options['sorts']['field_ag_date_value']['table'] = 'node_data_field_ag_date';
  $handler->display->display_options['sorts']['field_ag_date_value']['field'] = 'field_ag_date_value';

  /* Display: All events from partners page */
  $handler = $view->new_display('page', 'All events from partners page', 'page_2');
  $handler->display->display_options['defaults']['title'] = FALSE;
  $handler->display->display_options['title'] = 'All events from partners';
  $handler->display->display_options['defaults']['sorts'] = FALSE;
  /* Sort criterion: Broken/missing handler */
  $handler->display->display_options['sorts']['field_date_value']['id'] = 'field_date_value';
  $handler->display->display_options['sorts']['field_date_value']['table'] = 'node_data_field_date';
  $handler->display->display_options['sorts']['field_date_value']['field'] = 'field_date_value';
  $handler->display->display_options['sorts']['field_date_value']['order'] = 'DESC';
  /* Sort criterion: Broken/missing handler */
  $handler->display->display_options['sorts']['field_date_value2']['id'] = 'field_date_value2';
  $handler->display->display_options['sorts']['field_date_value2']['table'] = 'node_data_field_date';
  $handler->display->display_options['sorts']['field_date_value2']['field'] = 'field_date_value2';
  $handler->display->display_options['sorts']['field_date_value2']['order'] = 'DESC';
  $handler->display->display_options['defaults']['filter_groups'] = FALSE;
  $handler->display->display_options['defaults']['filters'] = FALSE;
  /* Filter criterion: Content: Published */
  $handler->display->display_options['filters']['status']['id'] = 'status';
  $handler->display->display_options['filters']['status']['table'] = 'node';
  $handler->display->display_options['filters']['status']['field'] = 'status';
  $handler->display->display_options['filters']['status']['value'] = '1';
  $handler->display->display_options['filters']['status']['group'] = '0';
  $handler->display->display_options['filters']['status']['expose']['operator'] = FALSE;
  /* Filter criterion: Content: Type */
  $handler->display->display_options['filters']['type']['id'] = 'type';
  $handler->display->display_options['filters']['type']['table'] = 'node';
  $handler->display->display_options['filters']['type']['field'] = 'type';
  $handler->display->display_options['filters']['type']['value'] = array(
    'agevent' => 'agevent',
  );
  $handler->display->display_options['filters']['type']['group'] = '0';
  $handler->display->display_options['filters']['type']['expose']['operator'] = FALSE;
  /* Filter criterion: Search: Search Terms */
  $handler->display->display_options['filters']['keys']['id'] = 'keys';
  $handler->display->display_options['filters']['keys']['table'] = 'search_index';
  $handler->display->display_options['filters']['keys']['field'] = 'keys';
  $handler->display->display_options['filters']['keys']['group'] = '0';
  $handler->display->display_options['filters']['keys']['exposed'] = TRUE;
  $handler->display->display_options['filters']['keys']['expose']['operator_id'] = 'keys_op';
  $handler->display->display_options['filters']['keys']['expose']['label'] = 'Search';
  $handler->display->display_options['filters']['keys']['expose']['operator'] = 'keys_op';
  $handler->display->display_options['filters']['keys']['expose']['identifier'] = 'keys';
  /* Filter criterion: Content: Title */
  $handler->display->display_options['filters']['title']['id'] = 'title';
  $handler->display->display_options['filters']['title']['table'] = 'node';
  $handler->display->display_options['filters']['title']['field'] = 'title';
  $handler->display->display_options['filters']['title']['relationship'] = 'feed_nid';
  $handler->display->display_options['filters']['title']['operator'] = 'contains';
  $handler->display->display_options['filters']['title']['group'] = '0';
  $handler->display->display_options['filters']['title']['exposed'] = TRUE;
  $handler->display->display_options['filters']['title']['expose']['operator_id'] = 'title_op';
  $handler->display->display_options['filters']['title']['expose']['label'] = 'Search feed title';
  $handler->display->display_options['filters']['title']['expose']['operator'] = 'title_op';
  $handler->display->display_options['filters']['title']['expose']['identifier'] = 'title';
  $handler->display->display_options['path'] = 'ag_all-partners-events';

  /* Display: All events from partners block */
  $handler = $view->new_display('block', 'All events from partners block', 'block_2');
  $handler->display->display_options['defaults']['title'] = FALSE;
  $handler->display->display_options['title'] = 'All events from partners';
  $handler->display->display_options['defaults']['use_more'] = FALSE;
  $handler->display->display_options['use_more'] = TRUE;
  $handler->display->display_options['defaults']['use_more_always'] = FALSE;
  $handler->display->display_options['defaults']['use_more_always'] = FALSE;
  $handler->display->display_options['use_more_always'] = FALSE;
  $handler->display->display_options['defaults']['use_more_text'] = FALSE;
  $handler->display->display_options['defaults']['link_display'] = FALSE;
  $handler->display->display_options['link_display'] = 'page_2';
  $handler->display->display_options['defaults']['fields'] = FALSE;
  /* Field: Content: Title */
  $handler->display->display_options['fields']['title']['id'] = 'title';
  $handler->display->display_options['fields']['title']['table'] = 'node';
  $handler->display->display_options['fields']['title']['field'] = 'title';
  $handler->display->display_options['fields']['title']['label'] = 'Name';
  /* Field: Broken/missing handler */
  $handler->display->display_options['fields']['field_ag_date_value']['id'] = 'field_ag_date_value';
  $handler->display->display_options['fields']['field_ag_date_value']['table'] = 'node_data_field_ag_date';
  $handler->display->display_options['fields']['field_ag_date_value']['field'] = 'field_ag_date_value';
  $handler->display->display_options['fields']['field_ag_date_value']['label'] = 'From';
  /* Field: Broken/missing handler */
  $handler->display->display_options['fields']['field_ag_date_value2']['id'] = 'field_ag_date_value2';
  $handler->display->display_options['fields']['field_ag_date_value2']['table'] = 'node_data_field_ag_date';
  $handler->display->display_options['fields']['field_ag_date_value2']['field'] = 'field_ag_date_value2';
  $handler->display->display_options['fields']['field_ag_date_value2']['label'] = 'To';
  /* Field: Broken/missing handler */
  $handler->display->display_options['fields']['field_ag_country_value']['id'] = 'field_ag_country_value';
  $handler->display->display_options['fields']['field_ag_country_value']['table'] = 'node_data_field_ag_country';
  $handler->display->display_options['fields']['field_ag_country_value']['field'] = 'field_ag_country_value';
  $handler->display->display_options['fields']['field_ag_country_value']['label'] = 'Country';
  /* Field: Broken/missing handler */
  $handler->display->display_options['fields']['field_ag_location_value']['id'] = 'field_ag_location_value';
  $handler->display->display_options['fields']['field_ag_location_value']['table'] = 'node_data_field_ag_location';
  $handler->display->display_options['fields']['field_ag_location_value']['field'] = 'field_ag_location_value';
  $handler->display->display_options['fields']['field_ag_location_value']['label'] = 'Location';
  $handler->display->display_options['defaults']['sorts'] = FALSE;
  /* Sort criterion: Broken/missing handler */
  $handler->display->display_options['sorts']['field_date_value']['id'] = 'field_date_value';
  $handler->display->display_options['sorts']['field_date_value']['table'] = 'node_data_field_date';
  $handler->display->display_options['sorts']['field_date_value']['field'] = 'field_date_value';
  $handler->display->display_options['sorts']['field_date_value']['order'] = 'DESC';
  /* Sort criterion: Broken/missing handler */
  $handler->display->display_options['sorts']['field_date_value2']['id'] = 'field_date_value2';
  $handler->display->display_options['sorts']['field_date_value2']['table'] = 'node_data_field_date';
  $handler->display->display_options['sorts']['field_date_value2']['field'] = 'field_date_value2';
  $handler->display->display_options['sorts']['field_date_value2']['order'] = 'DESC';
  $handler->display->display_options['defaults']['filter_groups'] = FALSE;
  $handler->display->display_options['defaults']['filters'] = FALSE;
  /* Filter criterion: Content: Published */
  $handler->display->display_options['filters']['status']['id'] = 'status';
  $handler->display->display_options['filters']['status']['table'] = 'node';
  $handler->display->display_options['filters']['status']['field'] = 'status';
  $handler->display->display_options['filters']['status']['value'] = '1';
  $handler->display->display_options['filters']['status']['group'] = '0';
  $handler->display->display_options['filters']['status']['expose']['operator'] = FALSE;
  /* Filter criterion: Content: Type */
  $handler->display->display_options['filters']['type']['id'] = 'type';
  $handler->display->display_options['filters']['type']['table'] = 'node';
  $handler->display->display_options['filters']['type']['field'] = 'type';
  $handler->display->display_options['filters']['type']['value'] = array(
    'agevent' => 'agevent',
  );
  $handler->display->display_options['filters']['type']['group'] = '0';
  $handler->display->display_options['filters']['type']['expose']['operator'] = FALSE;
  $export['Ag_Upcoming_exevents'] = $view;

  $view = new view();
  $view->name = 'Ag_browse_by';
  $view->description = '';
  $view->tag = '';
  $view->base_table = 'node';
  $view->human_name = '';
  $view->core = 0;
  $view->api_version = '3.0';
  $view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */

  /* Display: Defaults */
  $handler = $view->new_display('default', 'Defaults', 'default');
  $handler->display->display_options['title'] = 'Browse resources by author';
  $handler->display->display_options['use_more_always'] = FALSE;
  $handler->display->display_options['access']['type'] = 'none';
  $handler->display->display_options['cache']['type'] = 'none';
  $handler->display->display_options['query']['type'] = 'views_query';
  $handler->display->display_options['exposed_form']['type'] = 'basic';
  $handler->display->display_options['pager']['type'] = 'full';
  $handler->display->display_options['pager']['options']['items_per_page'] = 25;
  $handler->display->display_options['style_plugin'] = 'list';
  $handler->display->display_options['style_options']['grouping'] = '';
  $handler->display->display_options['row_plugin'] = 'fields';
  $handler->display->display_options['row_options']['inline'] = array(
    'field_ag_issn' => 'field_ag_issn',
    'field_ag_publishplace' => 'field_ag_publishplace',
  );
  /* Header: Global: Text area */
  $handler->display->display_options['header']['text']['id'] = 'area';
  $handler->display->display_options['header']['text']['table'] = 'views';
  $handler->display->display_options['header']['text']['field'] = 'area';
  $handler->display->display_options['header']['text']['empty'] = TRUE;
  $handler->display->display_options['header']['text']['content'] = 'Here you can browse the catalog by personal author.
<br />Clicking on a name will display all the bibliographic records where the author is the personal author.';
  $handler->display->display_options['header']['text']['format'] = '2';
  /* Field: Content: Nid */
  $handler->display->display_options['fields']['nid']['id'] = 'nid';
  $handler->display->display_options['fields']['nid']['table'] = 'node';
  $handler->display->display_options['fields']['nid']['field'] = 'nid';
  $handler->display->display_options['fields']['nid']['exclude'] = TRUE;
  /* Field: Content: Title */
  $handler->display->display_options['fields']['title']['id'] = 'title';
  $handler->display->display_options['fields']['title']['table'] = 'node';
  $handler->display->display_options['fields']['title']['field'] = 'title';
  $handler->display->display_options['fields']['title']['label'] = '';
  $handler->display->display_options['fields']['title']['alter']['make_link'] = TRUE;
  $handler->display->display_options['fields']['title']['alter']['path'] = 'ag_dlios-by-authors/[nid]/[title]';
  $handler->display->display_options['fields']['title']['link_to_node'] = FALSE;
  /* Field: Broken/missing handler */
  $handler->display->display_options['fields']['phpcode']['id'] = 'phpcode';
  $handler->display->display_options['fields']['phpcode']['table'] = 'customfield';
  $handler->display->display_options['fields']['phpcode']['field'] = 'phpcode';
  $handler->display->display_options['fields']['phpcode']['label'] = '';
  $handler->display->display_options['fields']['phpcode']['alter']['path'] = 'dlios-by-authors/[nid]';
  /* Field: Broken/missing handler */
  $handler->display->display_options['fields']['field_ag_confdate_value']['id'] = 'field_ag_confdate_value';
  $handler->display->display_options['fields']['field_ag_confdate_value']['table'] = 'node_data_field_ag_confdate';
  $handler->display->display_options['fields']['field_ag_confdate_value']['field'] = 'field_ag_confdate_value';
  /* Field: Broken/missing handler */
  $handler->display->display_options['fields']['field_ag_conflocation_value']['id'] = 'field_ag_conflocation_value';
  $handler->display->display_options['fields']['field_ag_conflocation_value']['table'] = 'node_data_field_ag_conflocation';
  $handler->display->display_options['fields']['field_ag_conflocation_value']['field'] = 'field_ag_conflocation_value';
  /* Sort criterion: Content: Title */
  $handler->display->display_options['sorts']['title']['id'] = 'title';
  $handler->display->display_options['sorts']['title']['table'] = 'node';
  $handler->display->display_options['sorts']['title']['field'] = 'title';
  /* Contextual filter: Broken/missing handler */
  $handler->display->display_options['arguments']['field_ag_confname_nid']['id'] = 'field_ag_confname_nid';
  $handler->display->display_options['arguments']['field_ag_confname_nid']['table'] = 'node_data_field_ag_confname';
  $handler->display->display_options['arguments']['field_ag_confname_nid']['field'] = 'field_ag_confname_nid';
  $handler->display->display_options['arguments']['field_ag_confname_nid']['default_argument_type'] = 'fixed';
  $handler->display->display_options['arguments']['field_ag_confname_nid']['summary']['format'] = 'default_summary';
  /* Contextual filter: Global: Null */
  $handler->display->display_options['arguments']['null']['id'] = 'null';
  $handler->display->display_options['arguments']['null']['table'] = 'views';
  $handler->display->display_options['arguments']['null']['field'] = 'null';
  $handler->display->display_options['arguments']['null']['default_argument_type'] = 'fixed';
  $handler->display->display_options['arguments']['null']['summary']['format'] = 'default_summary';
  /* Filter criterion: Content: Published */
  $handler->display->display_options['filters']['status']['id'] = 'status';
  $handler->display->display_options['filters']['status']['table'] = 'node';
  $handler->display->display_options['filters']['status']['field'] = 'status';
  $handler->display->display_options['filters']['status']['value'] = '1';
  $handler->display->display_options['filters']['status']['group'] = '0';
  $handler->display->display_options['filters']['status']['expose']['operator'] = FALSE;
  /* Filter criterion: Content: Type */
  $handler->display->display_options['filters']['type']['id'] = 'type';
  $handler->display->display_options['filters']['type']['table'] = 'node';
  $handler->display->display_options['filters']['type']['field'] = 'type';
  $handler->display->display_options['filters']['type']['value'] = array(
    'agauthor' => 'agauthor',
  );
  $handler->display->display_options['filters']['type']['group'] = '0';
  $handler->display->display_options['filters']['type']['expose']['operator'] = FALSE;

  /* Display: Browse by author */
  $handler = $view->new_display('page', 'Browse by author', 'page_1');
  $handler->display->display_options['defaults']['header'] = FALSE;
  /* Header: Global: Text area */
  $handler->display->display_options['header']['text']['id'] = 'area';
  $handler->display->display_options['header']['text']['table'] = 'views';
  $handler->display->display_options['header']['text']['field'] = 'area';
  $handler->display->display_options['header']['text']['empty'] = TRUE;
  $handler->display->display_options['header']['text']['content'] = 'This is the authority list of authors. The list is taken from the catalog and includes all authors that are indicated as "personal authors" of one or more of the bibliographic resources in the catalog.

If there is a corresponding full record in the directory of experts, a link to it is displayed.

';
  $handler->display->display_options['header']['text']['format'] = '2';
  $handler->display->display_options['defaults']['fields'] = FALSE;
  /* Field: Content: Nid */
  $handler->display->display_options['fields']['nid']['id'] = 'nid';
  $handler->display->display_options['fields']['nid']['table'] = 'node';
  $handler->display->display_options['fields']['nid']['field'] = 'nid';
  $handler->display->display_options['fields']['nid']['exclude'] = TRUE;
  /* Field: Content: Title */
  $handler->display->display_options['fields']['title']['id'] = 'title';
  $handler->display->display_options['fields']['title']['table'] = 'node';
  $handler->display->display_options['fields']['title']['field'] = 'title';
  $handler->display->display_options['fields']['title']['label'] = '';
  $handler->display->display_options['fields']['title']['alter']['make_link'] = TRUE;
  $handler->display->display_options['fields']['title']['alter']['path'] = 'ag_dlios-by-authors/[nid]/[title]';
  $handler->display->display_options['fields']['title']['link_to_node'] = FALSE;
  $handler->display->display_options['defaults']['arguments'] = FALSE;
  $handler->display->display_options['defaults']['filter_groups'] = FALSE;
  $handler->display->display_options['defaults']['filters'] = FALSE;
  /* Filter criterion: Content: Published */
  $handler->display->display_options['filters']['status']['id'] = 'status';
  $handler->display->display_options['filters']['status']['table'] = 'node';
  $handler->display->display_options['filters']['status']['field'] = 'status';
  $handler->display->display_options['filters']['status']['value'] = '1';
  $handler->display->display_options['filters']['status']['group'] = '0';
  $handler->display->display_options['filters']['status']['expose']['operator'] = FALSE;
  /* Filter criterion: Content: Type */
  $handler->display->display_options['filters']['type']['id'] = 'type';
  $handler->display->display_options['filters']['type']['table'] = 'node';
  $handler->display->display_options['filters']['type']['field'] = 'type';
  $handler->display->display_options['filters']['type']['value'] = array(
    'agauthor' => 'agauthor',
  );
  $handler->display->display_options['filters']['type']['group'] = '0';
  $handler->display->display_options['filters']['type']['expose']['operator'] = FALSE;
  $handler->display->display_options['path'] = 'ag_byauthor';

  /* Display: Authors */
  $handler = $view->new_display('page', 'Authors', 'page_2');
  $handler->display->display_options['defaults']['title'] = FALSE;
  $handler->display->display_options['title'] = 'Personal authors';
  $handler->display->display_options['defaults']['style_plugin'] = FALSE;
  $handler->display->display_options['style_plugin'] = 'list';
  $handler->display->display_options['style_options']['grouping'] = '';
  $handler->display->display_options['defaults']['style_options'] = FALSE;
  $handler->display->display_options['defaults']['row_plugin'] = FALSE;
  $handler->display->display_options['row_plugin'] = 'fields';
  $handler->display->display_options['row_options']['inline'] = array(
    'view_node' => 'view_node',
    'field_expert_record_nid' => 'field_expert_record_nid',
    'phpcode' => 'phpcode',
  );
  $handler->display->display_options['row_options']['separator'] = ' | ';
  $handler->display->display_options['defaults']['row_options'] = FALSE;
  $handler->display->display_options['defaults']['header'] = FALSE;
  /* Header: Global: Text area */
  $handler->display->display_options['header']['text']['id'] = 'area';
  $handler->display->display_options['header']['text']['table'] = 'views';
  $handler->display->display_options['header']['text']['field'] = 'area';
  $handler->display->display_options['header']['text']['empty'] = TRUE;
  $handler->display->display_options['header']['text']['content'] = 'This is the authority list of authors. The list is taken from the catalog and includes all authors that are indicated as "personal authors" of one or more of the bibliographic resources in the catalog.
<br /><br />If there is a corresponding full record in the directory of experts, a link to it is displayed.
';
  $handler->display->display_options['header']['text']['format'] = '2';
  $handler->display->display_options['defaults']['relationships'] = FALSE;
  $handler->display->display_options['defaults']['fields'] = FALSE;
  /* Field: Content: Nid */
  $handler->display->display_options['fields']['nid']['id'] = 'nid';
  $handler->display->display_options['fields']['nid']['table'] = 'node';
  $handler->display->display_options['fields']['nid']['field'] = 'nid';
  $handler->display->display_options['fields']['nid']['exclude'] = TRUE;
  /* Field: Content: Title */
  $handler->display->display_options['fields']['title']['id'] = 'title';
  $handler->display->display_options['fields']['title']['table'] = 'node';
  $handler->display->display_options['fields']['title']['field'] = 'title';
  $handler->display->display_options['fields']['title']['label'] = '';
  $handler->display->display_options['fields']['title']['alter']['path'] = 'dlios-by-authors/[nid]';
  $handler->display->display_options['fields']['title']['link_to_node'] = FALSE;
  /* Field: Content: Link */
  $handler->display->display_options['fields']['view_node']['id'] = 'view_node';
  $handler->display->display_options['fields']['view_node']['table'] = 'views_entity_node';
  $handler->display->display_options['fields']['view_node']['field'] = 'view_node';
  $handler->display->display_options['fields']['view_node']['label'] = '';
  $handler->display->display_options['fields']['view_node']['text'] = 'record in authority file';
  /* Field: Global: Custom text */
  $handler->display->display_options['fields']['nothing']['id'] = 'nothing';
  $handler->display->display_options['fields']['nothing']['table'] = 'views';
  $handler->display->display_options['fields']['nothing']['field'] = 'nothing';
  $handler->display->display_options['fields']['nothing']['label'] = '';
  $handler->display->display_options['fields']['nothing']['alter']['text'] = 'related bibliographic records';
  $handler->display->display_options['fields']['nothing']['alter']['make_link'] = TRUE;
  $handler->display->display_options['fields']['nothing']['alter']['path'] = 'ag_dlios-by-authors/[nid]/[title]';
  $handler->display->display_options['fields']['nothing']['alter']['replace_spaces'] = TRUE;
  $handler->display->display_options['fields']['nothing']['element_label_colon'] = FALSE;
  /* Field: Content: Title */
  $handler->display->display_options['fields']['title_1']['id'] = 'title_1';
  $handler->display->display_options['fields']['title_1']['table'] = 'node';
  $handler->display->display_options['fields']['title_1']['field'] = 'title';
  $handler->display->display_options['fields']['title_1']['relationship'] = 'feed_nid';
  $handler->display->display_options['fields']['title_1']['exclude'] = TRUE;
  $handler->display->display_options['fields']['title_1']['link_to_node'] = FALSE;
  $handler->display->display_options['defaults']['arguments'] = FALSE;
  $handler->display->display_options['path'] = 'ag_authors';

  /* Display: Browse by corporate body */
  $handler = $view->new_display('page', 'Browse by corporate body', 'page_3');
  $handler->display->display_options['defaults']['title'] = FALSE;
  $handler->display->display_options['title'] = 'Browse resources by corporate body';
  $handler->display->display_options['defaults']['header'] = FALSE;
  /* Header: Global: Text area */
  $handler->display->display_options['header']['text']['id'] = 'area';
  $handler->display->display_options['header']['text']['table'] = 'views';
  $handler->display->display_options['header']['text']['field'] = 'area';
  $handler->display->display_options['header']['text']['empty'] = TRUE;
  $handler->display->display_options['header']['text']['content'] = 'Here you can browse the catalog by corporate author.
<br />Clicking on a name will display all the bibliographic records where the corporate body is the author';
  $handler->display->display_options['header']['text']['format'] = '2';
  $handler->display->display_options['defaults']['fields'] = FALSE;
  /* Field: Content: Nid */
  $handler->display->display_options['fields']['nid']['id'] = 'nid';
  $handler->display->display_options['fields']['nid']['table'] = 'node';
  $handler->display->display_options['fields']['nid']['field'] = 'nid';
  $handler->display->display_options['fields']['nid']['exclude'] = TRUE;
  /* Field: Content: Title */
  $handler->display->display_options['fields']['title']['id'] = 'title';
  $handler->display->display_options['fields']['title']['table'] = 'node';
  $handler->display->display_options['fields']['title']['field'] = 'title';
  $handler->display->display_options['fields']['title']['label'] = '';
  $handler->display->display_options['fields']['title']['alter']['make_link'] = TRUE;
  $handler->display->display_options['fields']['title']['alter']['path'] = 'ag_dlios-by-corporatebody/[nid]/[title]';
  $handler->display->display_options['fields']['title']['link_to_node'] = FALSE;
  $handler->display->display_options['defaults']['arguments'] = FALSE;
  $handler->display->display_options['defaults']['filter_groups'] = FALSE;
  $handler->display->display_options['defaults']['filters'] = FALSE;
  /* Filter criterion: Content: Published */
  $handler->display->display_options['filters']['status']['id'] = 'status';
  $handler->display->display_options['filters']['status']['table'] = 'node';
  $handler->display->display_options['filters']['status']['field'] = 'status';
  $handler->display->display_options['filters']['status']['value'] = '1';
  $handler->display->display_options['filters']['status']['group'] = '0';
  $handler->display->display_options['filters']['status']['expose']['operator'] = FALSE;
  /* Filter criterion: Content: Type */
  $handler->display->display_options['filters']['type']['id'] = 'type';
  $handler->display->display_options['filters']['type']['table'] = 'node';
  $handler->display->display_options['filters']['type']['field'] = 'type';
  $handler->display->display_options['filters']['type']['value'] = array(
    'agcorporate_body' => 'agcorporate_body',
  );
  $handler->display->display_options['filters']['type']['group'] = '0';
  $handler->display->display_options['filters']['type']['expose']['operator'] = FALSE;
  $handler->display->display_options['path'] = 'ag_bycorporatebody';

  /* Display: Corporate bodies */
  $handler = $view->new_display('page', 'Corporate bodies', 'page_4');
  $handler->display->display_options['defaults']['title'] = FALSE;
  $handler->display->display_options['title'] = 'Corporate bodies';
  $handler->display->display_options['defaults']['header'] = FALSE;
  /* Header: Global: Text area */
  $handler->display->display_options['header']['text']['id'] = 'area';
  $handler->display->display_options['header']['text']['table'] = 'views';
  $handler->display->display_options['header']['text']['field'] = 'area';
  $handler->display->display_options['header']['text']['empty'] = TRUE;
  $handler->display->display_options['header']['text']['content'] = 'This is the authority list of corporate bodies. The list is taken from the catalog and includes all corporate bodies that are indicated as "corporate authors" of one or more of the bibliographic resources in the catalog.
<br /><br />If there is a corresponding full record in the directory of Institutions, a link to it is displayed.';
  $handler->display->display_options['header']['text']['format'] = '2';
  $handler->display->display_options['defaults']['relationships'] = FALSE;
  $handler->display->display_options['defaults']['fields'] = FALSE;
  /* Field: Content: Nid */
  $handler->display->display_options['fields']['nid']['id'] = 'nid';
  $handler->display->display_options['fields']['nid']['table'] = 'node';
  $handler->display->display_options['fields']['nid']['field'] = 'nid';
  $handler->display->display_options['fields']['nid']['exclude'] = TRUE;
  /* Field: Content: Title */
  $handler->display->display_options['fields']['title']['id'] = 'title';
  $handler->display->display_options['fields']['title']['table'] = 'node';
  $handler->display->display_options['fields']['title']['field'] = 'title';
  $handler->display->display_options['fields']['title']['label'] = '';
  $handler->display->display_options['fields']['title']['link_to_node'] = FALSE;
  /* Field: Content: Link */
  $handler->display->display_options['fields']['view_node']['id'] = 'view_node';
  $handler->display->display_options['fields']['view_node']['table'] = 'views_entity_node';
  $handler->display->display_options['fields']['view_node']['field'] = 'view_node';
  $handler->display->display_options['fields']['view_node']['label'] = '';
  $handler->display->display_options['fields']['view_node']['text'] = 'record in authority file';
  /* Field: Global: Custom text */
  $handler->display->display_options['fields']['nothing']['id'] = 'nothing';
  $handler->display->display_options['fields']['nothing']['table'] = 'views';
  $handler->display->display_options['fields']['nothing']['field'] = 'nothing';
  $handler->display->display_options['fields']['nothing']['label'] = '';
  $handler->display->display_options['fields']['nothing']['alter']['text'] = 'related bibliographic records';
  $handler->display->display_options['fields']['nothing']['alter']['make_link'] = TRUE;
  $handler->display->display_options['fields']['nothing']['alter']['path'] = 'ag_dlios-by-corporatebody/[nid]';
  $handler->display->display_options['defaults']['arguments'] = FALSE;
  $handler->display->display_options['defaults']['filter_groups'] = FALSE;
  $handler->display->display_options['defaults']['filters'] = FALSE;
  /* Filter criterion: Content: Published */
  $handler->display->display_options['filters']['status']['id'] = 'status';
  $handler->display->display_options['filters']['status']['table'] = 'node';
  $handler->display->display_options['filters']['status']['field'] = 'status';
  $handler->display->display_options['filters']['status']['value'] = '1';
  $handler->display->display_options['filters']['status']['group'] = '0';
  $handler->display->display_options['filters']['status']['expose']['operator'] = FALSE;
  /* Filter criterion: Content: Type */
  $handler->display->display_options['filters']['type']['id'] = 'type';
  $handler->display->display_options['filters']['type']['table'] = 'node';
  $handler->display->display_options['filters']['type']['field'] = 'type';
  $handler->display->display_options['filters']['type']['value'] = array(
    'agcorporate_body' => 'agcorporate_body',
  );
  $handler->display->display_options['filters']['type']['group'] = '0';
  $handler->display->display_options['filters']['type']['expose']['operator'] = FALSE;
  $handler->display->display_options['path'] = 'ag_corporate-bodies';

  /* Display: Browse by book */
  $handler = $view->new_display('page', 'Browse by book', 'page_5');
  $handler->display->display_options['defaults']['title'] = FALSE;
  $handler->display->display_options['title'] = 'Browse resources by book';
  $handler->display->display_options['defaults']['header'] = FALSE;
  /* Header: Global: Text area */
  $handler->display->display_options['header']['text']['id'] = 'area';
  $handler->display->display_options['header']['text']['table'] = 'views';
  $handler->display->display_options['header']['text']['field'] = 'area';
  $handler->display->display_options['header']['text']['empty'] = TRUE;
  $handler->display->display_options['header']['text']['content'] = 'Here you can browse our bibliographic records of journal articles by book title.
<br />Clicking on a book title will display the book chapter in the catalog that were published in that book.';
  $handler->display->display_options['header']['text']['format'] = '2';
  $handler->display->display_options['defaults']['relationships'] = FALSE;
  $handler->display->display_options['defaults']['fields'] = FALSE;
  /* Field: Content: Nid */
  $handler->display->display_options['fields']['nid']['id'] = 'nid';
  $handler->display->display_options['fields']['nid']['table'] = 'node';
  $handler->display->display_options['fields']['nid']['field'] = 'nid';
  $handler->display->display_options['fields']['nid']['exclude'] = TRUE;
  /* Field: Content: Title */
  $handler->display->display_options['fields']['title']['id'] = 'title';
  $handler->display->display_options['fields']['title']['table'] = 'node';
  $handler->display->display_options['fields']['title']['field'] = 'title';
  $handler->display->display_options['fields']['title']['label'] = '';
  $handler->display->display_options['fields']['title']['alter']['make_link'] = TRUE;
  $handler->display->display_options['fields']['title']['alter']['path'] = 'ag_dlios-by-books/[nid]/[title]';
  $handler->display->display_options['fields']['title']['link_to_node'] = FALSE;
  $handler->display->display_options['defaults']['arguments'] = FALSE;
  $handler->display->display_options['defaults']['filter_groups'] = FALSE;
  $handler->display->display_options['defaults']['filters'] = FALSE;
  /* Filter criterion: Content: Published */
  $handler->display->display_options['filters']['status']['id'] = 'status';
  $handler->display->display_options['filters']['status']['table'] = 'node';
  $handler->display->display_options['filters']['status']['field'] = 'status';
  $handler->display->display_options['filters']['status']['value'] = '1';
  $handler->display->display_options['filters']['status']['group'] = '0';
  $handler->display->display_options['filters']['status']['expose']['operator'] = FALSE;
  /* Filter criterion: Content: Type */
  $handler->display->display_options['filters']['type']['id'] = 'type';
  $handler->display->display_options['filters']['type']['table'] = 'node';
  $handler->display->display_options['filters']['type']['field'] = 'type';
  $handler->display->display_options['filters']['type']['value'] = array(
    'agdlios' => 'agdlios',
  );
  $handler->display->display_options['filters']['type']['group'] = '0';
  $handler->display->display_options['filters']['type']['expose']['operator'] = FALSE;
  /* Filter criterion: Content: Document type (field_ag_type) */
  $handler->display->display_options['filters']['field_ag_type_tid']['id'] = 'field_ag_type_tid';
  $handler->display->display_options['filters']['field_ag_type_tid']['table'] = 'field_data_field_ag_type';
  $handler->display->display_options['filters']['field_ag_type_tid']['field'] = 'field_ag_type_tid';
  $handler->display->display_options['filters']['field_ag_type_tid']['value'] = array(
    0 => '1',
  );
  $handler->display->display_options['filters']['field_ag_type_tid']['vocabulary'] = 'vocabulary_1';
  $handler->display->display_options['path'] = 'ag_bybook';

  /* Display: Browse by conference */
  $handler = $view->new_display('page', 'Browse by conference', 'page_7');
  $handler->display->display_options['defaults']['title'] = FALSE;
  $handler->display->display_options['title'] = 'Browse resources by conference';
  $handler->display->display_options['defaults']['header'] = FALSE;
  /* Header: Global: Text area */
  $handler->display->display_options['header']['text']['id'] = 'area';
  $handler->display->display_options['header']['text']['table'] = 'views';
  $handler->display->display_options['header']['text']['field'] = 'area';
  $handler->display->display_options['header']['text']['empty'] = TRUE;
  $handler->display->display_options['header']['text']['content'] = 'Here you can browse our bibliographic records of conference papers, posters and proceedings by conference.
<br />Conferences are sorted by date, from the most recent to the oldest.';
  $handler->display->display_options['header']['text']['format'] = '2';
  $handler->display->display_options['defaults']['fields'] = FALSE;
  /* Field: Content: Nid */
  $handler->display->display_options['fields']['nid']['id'] = 'nid';
  $handler->display->display_options['fields']['nid']['table'] = 'node';
  $handler->display->display_options['fields']['nid']['field'] = 'nid';
  $handler->display->display_options['fields']['nid']['exclude'] = TRUE;
  /* Field: Content: Title */
  $handler->display->display_options['fields']['title']['id'] = 'title';
  $handler->display->display_options['fields']['title']['table'] = 'node';
  $handler->display->display_options['fields']['title']['field'] = 'title';
  $handler->display->display_options['fields']['title']['label'] = '';
  $handler->display->display_options['fields']['title']['alter']['make_link'] = TRUE;
  $handler->display->display_options['fields']['title']['alter']['path'] = 'ag_dlios-by-conferences/[nid]/[title]';
  $handler->display->display_options['fields']['title']['link_to_node'] = FALSE;
  $handler->display->display_options['defaults']['arguments'] = FALSE;
  $handler->display->display_options['defaults']['filter_groups'] = FALSE;
  $handler->display->display_options['defaults']['filters'] = FALSE;
  /* Filter criterion: Content: Published */
  $handler->display->display_options['filters']['status']['id'] = 'status';
  $handler->display->display_options['filters']['status']['table'] = 'node';
  $handler->display->display_options['filters']['status']['field'] = 'status';
  $handler->display->display_options['filters']['status']['value'] = '1';
  $handler->display->display_options['filters']['status']['group'] = '0';
  $handler->display->display_options['filters']['status']['expose']['operator'] = FALSE;
  /* Filter criterion: Content: Type */
  $handler->display->display_options['filters']['type']['id'] = 'type';
  $handler->display->display_options['filters']['type']['table'] = 'node';
  $handler->display->display_options['filters']['type']['field'] = 'type';
  $handler->display->display_options['filters']['type']['value'] = array(
    'agconference' => 'agconference',
  );
  $handler->display->display_options['filters']['type']['group'] = '0';
  $handler->display->display_options['filters']['type']['expose']['operator'] = FALSE;
  $handler->display->display_options['path'] = 'ag_byconference';

  /* Display: Conferences */
  $handler = $view->new_display('page', 'Conferences', 'page_8');
  $handler->display->display_options['defaults']['title'] = FALSE;
  $handler->display->display_options['title'] = 'Conferences';
  $handler->display->display_options['defaults']['group_by'] = FALSE;
  $handler->display->display_options['defaults']['header'] = FALSE;
  /* Header: Global: Text area */
  $handler->display->display_options['header']['text']['id'] = 'area';
  $handler->display->display_options['header']['text']['table'] = 'views';
  $handler->display->display_options['header']['text']['field'] = 'area';
  $handler->display->display_options['header']['text']['empty'] = TRUE;
  $handler->display->display_options['header']['text']['content'] = 'Here you can browse our list of Conferences.
Conferences are sorted by date, from the most recent to the oldest.
Clicking on the title will display the Conference record.
Clicking on the number in brackets will display resources (proceedings, papers, posters) from that Conference.

';
  $handler->display->display_options['header']['text']['format'] = '2';
  $handler->display->display_options['defaults']['fields'] = FALSE;
  /* Field: Content: Nid */
  $handler->display->display_options['fields']['nid']['id'] = 'nid';
  $handler->display->display_options['fields']['nid']['table'] = 'node';
  $handler->display->display_options['fields']['nid']['field'] = 'nid';
  $handler->display->display_options['fields']['nid']['exclude'] = TRUE;
  /* Field: Content: Title */
  $handler->display->display_options['fields']['title']['id'] = 'title';
  $handler->display->display_options['fields']['title']['table'] = 'node';
  $handler->display->display_options['fields']['title']['field'] = 'title';
  $handler->display->display_options['fields']['title']['label'] = '';
  $handler->display->display_options['fields']['title']['element_label_colon'] = FALSE;
  /* Field: Content: Title */
  $handler->display->display_options['fields']['title_1']['id'] = 'title_1';
  $handler->display->display_options['fields']['title_1']['table'] = 'node';
  $handler->display->display_options['fields']['title_1']['field'] = 'title';
  $handler->display->display_options['fields']['title_1']['label'] = '';
  $handler->display->display_options['fields']['title_1']['alter']['alter_text'] = TRUE;
  $handler->display->display_options['fields']['title_1']['alter']['text'] = 'related bibliographic records';
  $handler->display->display_options['fields']['title_1']['alter']['make_link'] = TRUE;
  $handler->display->display_options['fields']['title_1']['alter']['path'] = 'ag_dlios-by-conferences/[nid]/[title]';
  $handler->display->display_options['fields']['title_1']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['title_1']['link_to_node'] = FALSE;
  /* Field: Content: Conference Date */
  $handler->display->display_options['fields']['field_ag_confdate']['id'] = 'field_ag_confdate';
  $handler->display->display_options['fields']['field_ag_confdate']['table'] = 'field_data_field_ag_confdate';
  $handler->display->display_options['fields']['field_ag_confdate']['field'] = 'field_ag_confdate';
  $handler->display->display_options['fields']['field_ag_confdate']['label'] = '';
  $handler->display->display_options['fields']['field_ag_confdate']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['field_ag_confdate']['hide_empty'] = TRUE;
  $handler->display->display_options['fields']['field_ag_confdate']['settings'] = array(
    'format_type' => 'short',
    'fromto' => 'both',
    'multiple_number' => '',
    'multiple_from' => '',
    'multiple_to' => '',
  );
  /* Field: Content: Conference Location */
  $handler->display->display_options['fields']['field_ag_conflocation']['id'] = 'field_ag_conflocation';
  $handler->display->display_options['fields']['field_ag_conflocation']['table'] = 'field_data_field_ag_conflocation';
  $handler->display->display_options['fields']['field_ag_conflocation']['field'] = 'field_ag_conflocation';
  $handler->display->display_options['fields']['field_ag_conflocation']['label'] = '';
  $handler->display->display_options['fields']['field_ag_conflocation']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['field_ag_conflocation']['hide_empty'] = TRUE;
  $handler->display->display_options['defaults']['sorts'] = FALSE;
  $handler->display->display_options['defaults']['arguments'] = FALSE;
  $handler->display->display_options['defaults']['filter_groups'] = FALSE;
  $handler->display->display_options['defaults']['filters'] = FALSE;
  /* Filter criterion: Content: Published */
  $handler->display->display_options['filters']['status']['id'] = 'status';
  $handler->display->display_options['filters']['status']['table'] = 'node';
  $handler->display->display_options['filters']['status']['field'] = 'status';
  $handler->display->display_options['filters']['status']['value'] = '1';
  $handler->display->display_options['filters']['status']['group'] = '0';
  $handler->display->display_options['filters']['status']['expose']['operator'] = FALSE;
  /* Filter criterion: Content: Type */
  $handler->display->display_options['filters']['type']['id'] = 'type';
  $handler->display->display_options['filters']['type']['table'] = 'node';
  $handler->display->display_options['filters']['type']['field'] = 'type';
  $handler->display->display_options['filters']['type']['value'] = array(
    'agconference' => 'agconference',
  );
  $handler->display->display_options['filters']['type']['group'] = '0';
  $handler->display->display_options['filters']['type']['expose']['operator'] = FALSE;
  $handler->display->display_options['path'] = 'ag_conferences';

  /* Display: Browse by journal */
  $handler = $view->new_display('page', 'Browse by journal', 'page_9');
  $handler->display->display_options['defaults']['title'] = FALSE;
  $handler->display->display_options['title'] = 'Browse resources by journal';
  $handler->display->display_options['defaults']['header'] = FALSE;
  /* Header: Global: Text area */
  $handler->display->display_options['header']['text']['id'] = 'area';
  $handler->display->display_options['header']['text']['table'] = 'views';
  $handler->display->display_options['header']['text']['field'] = 'area';
  $handler->display->display_options['header']['text']['empty'] = TRUE;
  $handler->display->display_options['header']['text']['content'] = 'Here you can browse our bibliographic records of journal articles by journal title.
<br />Clicking on a journal title will display the articles in the catalog that were published in that journal.';
  $handler->display->display_options['header']['text']['format'] = '2';
  $handler->display->display_options['defaults']['fields'] = FALSE;
  /* Field: Content: Nid */
  $handler->display->display_options['fields']['nid']['id'] = 'nid';
  $handler->display->display_options['fields']['nid']['table'] = 'node';
  $handler->display->display_options['fields']['nid']['field'] = 'nid';
  $handler->display->display_options['fields']['nid']['exclude'] = TRUE;
  /* Field: Content: Title */
  $handler->display->display_options['fields']['title']['id'] = 'title';
  $handler->display->display_options['fields']['title']['table'] = 'node';
  $handler->display->display_options['fields']['title']['field'] = 'title';
  $handler->display->display_options['fields']['title']['label'] = '';
  $handler->display->display_options['fields']['title']['alter']['make_link'] = TRUE;
  $handler->display->display_options['fields']['title']['alter']['path'] = 'ag_dlios-by-journals/[nid]/[title]';
  $handler->display->display_options['fields']['title']['link_to_node'] = FALSE;
  $handler->display->display_options['defaults']['arguments'] = FALSE;
  $handler->display->display_options['defaults']['filter_groups'] = FALSE;
  $handler->display->display_options['defaults']['filters'] = FALSE;
  /* Filter criterion: Content: Published */
  $handler->display->display_options['filters']['status']['id'] = 'status';
  $handler->display->display_options['filters']['status']['table'] = 'node';
  $handler->display->display_options['filters']['status']['field'] = 'status';
  $handler->display->display_options['filters']['status']['value'] = '1';
  $handler->display->display_options['filters']['status']['group'] = '0';
  $handler->display->display_options['filters']['status']['expose']['operator'] = FALSE;
  /* Filter criterion: Content: Type */
  $handler->display->display_options['filters']['type']['id'] = 'type';
  $handler->display->display_options['filters']['type']['table'] = 'node';
  $handler->display->display_options['filters']['type']['field'] = 'type';
  $handler->display->display_options['filters']['type']['value'] = array(
    'agperiodical' => 'agperiodical',
  );
  $handler->display->display_options['filters']['type']['group'] = '0';
  $handler->display->display_options['filters']['type']['expose']['operator'] = FALSE;
  $handler->display->display_options['path'] = 'ag_byjournal';

  /* Display: Journals */
  $handler = $view->new_display('page', 'Journals', 'page_10');
  $handler->display->display_options['defaults']['title'] = FALSE;
  $handler->display->display_options['title'] = 'Journals';
  $handler->display->display_options['defaults']['header'] = FALSE;
  /* Header: Global: Text area */
  $handler->display->display_options['header']['text']['id'] = 'area';
  $handler->display->display_options['header']['text']['table'] = 'views';
  $handler->display->display_options['header']['text']['field'] = 'area';
  $handler->display->display_options['header']['text']['empty'] = TRUE;
  $handler->display->display_options['header']['text']['content'] = 'This is the authority list of journals. The list is taken from the catalog and includes all journals where one or more of the journal articles in the catalog were published.
';
  $handler->display->display_options['header']['text']['format'] = '2';
  $handler->display->display_options['defaults']['relationships'] = FALSE;
  $handler->display->display_options['defaults']['fields'] = FALSE;
  /* Field: Content: Nid */
  $handler->display->display_options['fields']['nid']['id'] = 'nid';
  $handler->display->display_options['fields']['nid']['table'] = 'node';
  $handler->display->display_options['fields']['nid']['field'] = 'nid';
  $handler->display->display_options['fields']['nid']['exclude'] = TRUE;
  /* Field: Content: Title */
  $handler->display->display_options['fields']['title']['id'] = 'title';
  $handler->display->display_options['fields']['title']['table'] = 'node';
  $handler->display->display_options['fields']['title']['field'] = 'title';
  $handler->display->display_options['fields']['title']['label'] = '';
  $handler->display->display_options['fields']['title']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['title']['link_to_node'] = FALSE;
  /* Field: Content: ISSN */
  $handler->display->display_options['fields']['field_ag_issn']['id'] = 'field_ag_issn';
  $handler->display->display_options['fields']['field_ag_issn']['table'] = 'field_data_field_ag_issn';
  $handler->display->display_options['fields']['field_ag_issn']['field'] = 'field_ag_issn';
  $handler->display->display_options['fields']['field_ag_issn']['element_default_classes'] = FALSE;
  $handler->display->display_options['fields']['field_ag_issn']['hide_empty'] = TRUE;
  $handler->display->display_options['fields']['field_ag_issn']['type'] = 'text_plain';
  /* Field: Content: Place of Publication */
  $handler->display->display_options['fields']['field_ag_publishplace']['id'] = 'field_ag_publishplace';
  $handler->display->display_options['fields']['field_ag_publishplace']['table'] = 'field_data_field_ag_publishplace';
  $handler->display->display_options['fields']['field_ag_publishplace']['field'] = 'field_ag_publishplace';
  $handler->display->display_options['fields']['field_ag_publishplace']['label'] = '';
  $handler->display->display_options['fields']['field_ag_publishplace']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['field_ag_publishplace']['hide_empty'] = TRUE;
  $handler->display->display_options['fields']['field_ag_publishplace']['delta_offset'] = '0';
  /* Field: Content: Link */
  $handler->display->display_options['fields']['view_node']['id'] = 'view_node';
  $handler->display->display_options['fields']['view_node']['table'] = 'views_entity_node';
  $handler->display->display_options['fields']['view_node']['field'] = 'view_node';
  $handler->display->display_options['fields']['view_node']['label'] = '';
  $handler->display->display_options['fields']['view_node']['text'] = 'record in authority file';
  /* Field: Global: Custom text */
  $handler->display->display_options['fields']['nothing']['id'] = 'nothing';
  $handler->display->display_options['fields']['nothing']['table'] = 'views';
  $handler->display->display_options['fields']['nothing']['field'] = 'nothing';
  $handler->display->display_options['fields']['nothing']['label'] = '';
  $handler->display->display_options['fields']['nothing']['alter']['text'] = 'articles in the catalog';
  $handler->display->display_options['fields']['nothing']['alter']['make_link'] = TRUE;
  $handler->display->display_options['fields']['nothing']['alter']['path'] = 'ag_dlios-by-journals/[nid]';
  $handler->display->display_options['defaults']['arguments'] = FALSE;
  $handler->display->display_options['defaults']['filter_groups'] = FALSE;
  $handler->display->display_options['defaults']['filters'] = FALSE;
  /* Filter criterion: Content: Published */
  $handler->display->display_options['filters']['status']['id'] = 'status';
  $handler->display->display_options['filters']['status']['table'] = 'node';
  $handler->display->display_options['filters']['status']['field'] = 'status';
  $handler->display->display_options['filters']['status']['value'] = '1';
  $handler->display->display_options['filters']['status']['group'] = '0';
  $handler->display->display_options['filters']['status']['expose']['operator'] = FALSE;
  /* Filter criterion: Content: Type */
  $handler->display->display_options['filters']['type']['id'] = 'type';
  $handler->display->display_options['filters']['type']['table'] = 'node';
  $handler->display->display_options['filters']['type']['field'] = 'type';
  $handler->display->display_options['filters']['type']['value'] = array(
    'agperiodical' => 'agperiodical',
  );
  $handler->display->display_options['filters']['type']['group'] = '0';
  $handler->display->display_options['filters']['type']['expose']['operator'] = FALSE;
  $handler->display->display_options['path'] = 'ag_journals';
  $export['Ag_browse_by'] = $view;

  $view = new view();
  $view->name = 'Ag_by_doctype';
  $view->description = '';
  $view->tag = '';
  $view->base_table = 'taxonomy_term_data';
  $view->human_name = '';
  $view->core = 0;
  $view->api_version = '3.0';
  $view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */

  /* Display: Defaults */
  $handler = $view->new_display('default', 'Defaults', 'default');
  $handler->display->display_options['title'] = 'Browse by type';
  $handler->display->display_options['use_more_always'] = FALSE;
  $handler->display->display_options['access']['type'] = 'none';
  $handler->display->display_options['cache']['type'] = 'none';
  $handler->display->display_options['query']['type'] = 'views_query';
  $handler->display->display_options['query']['options']['distinct'] = TRUE;
  $handler->display->display_options['exposed_form']['type'] = 'basic';
  $handler->display->display_options['pager']['type'] = 'none';
  $handler->display->display_options['pager']['options']['offset'] = '0';
  $handler->display->display_options['style_plugin'] = 'list';
  $handler->display->display_options['style_options']['grouping'] = '';
  $handler->display->display_options['row_plugin'] = 'fields';
  $handler->display->display_options['row_options']['inline'] = array(
    'name' => 'name',
    'phpcode' => 'phpcode',
  );
  /* Field: Taxonomy term: Term ID */
  $handler->display->display_options['fields']['tid']['id'] = 'tid';
  $handler->display->display_options['fields']['tid']['table'] = 'taxonomy_term_data';
  $handler->display->display_options['fields']['tid']['field'] = 'tid';
  $handler->display->display_options['fields']['tid']['exclude'] = TRUE;
  /* Field: Taxonomy term: Name */
  $handler->display->display_options['fields']['name']['id'] = 'name';
  $handler->display->display_options['fields']['name']['table'] = 'taxonomy_term_data';
  $handler->display->display_options['fields']['name']['field'] = 'name';
  $handler->display->display_options['fields']['name']['label'] = '';
  $handler->display->display_options['fields']['name']['alter']['make_link'] = TRUE;
  $handler->display->display_options['fields']['name']['alter']['path'] = 'dlios-by-doctype/[tid]/[name]';
  /* Field: Broken/missing handler */
  $handler->display->display_options['fields']['phpcode']['id'] = 'phpcode';
  $handler->display->display_options['fields']['phpcode']['table'] = 'customfield';
  $handler->display->display_options['fields']['phpcode']['field'] = 'phpcode';
  $handler->display->display_options['fields']['phpcode']['label'] = '';
  /* Sort criterion: Taxonomy term: Name */
  $handler->display->display_options['sorts']['name']['id'] = 'name';
  $handler->display->display_options['sorts']['name']['table'] = 'taxonomy_term_data';
  $handler->display->display_options['sorts']['name']['field'] = 'name';
  /* Filter criterion: Taxonomy term: Vocabulary */
  $handler->display->display_options['filters']['vid']['id'] = 'vid';
  $handler->display->display_options['filters']['vid']['table'] = 'taxonomy_term_data';
  $handler->display->display_options['filters']['vid']['field'] = 'vid';
  $handler->display->display_options['filters']['vid']['value'] = array(
    1 => '1',
  );
  $handler->display->display_options['filters']['vid']['group'] = '0';
  $handler->display->display_options['filters']['vid']['expose']['operator'] = FALSE;

  /* Display: ag_by_type */
  $handler = $view->new_display('page', 'ag_by_type', 'page_1');
  $handler->display->display_options['defaults']['fields'] = FALSE;
  /* Field: Taxonomy term: Term ID */
  $handler->display->display_options['fields']['tid']['id'] = 'tid';
  $handler->display->display_options['fields']['tid']['table'] = 'taxonomy_term_data';
  $handler->display->display_options['fields']['tid']['field'] = 'tid';
  $handler->display->display_options['fields']['tid']['exclude'] = TRUE;
  $handler->display->display_options['fields']['tid']['separator'] = '';
  /* Field: Taxonomy term: Name */
  $handler->display->display_options['fields']['name']['id'] = 'name';
  $handler->display->display_options['fields']['name']['table'] = 'taxonomy_term_data';
  $handler->display->display_options['fields']['name']['field'] = 'name';
  $handler->display->display_options['fields']['name']['label'] = '';
  $handler->display->display_options['fields']['name']['alter']['make_link'] = TRUE;
  $handler->display->display_options['fields']['name']['alter']['path'] = 'ag_dlios-by-doctype/[tid]/[name]';
  $handler->display->display_options['path'] = 'ag_bydoctype';
  $export['Ag_by_doctype'] = $view;

  $view = new view();
  $view->name = 'Ag_dlios_browse_by';
  $view->description = '';
  $view->tag = '';
  $view->base_table = 'node';
  $view->human_name = '';
  $view->core = 0;
  $view->api_version = '3.0';
  $view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */

  /* Display: dlios-by */
  $handler = $view->new_display('default', 'dlios-by', 'default');
  $handler->display->display_options['title'] = 'Documents by authors';
  $handler->display->display_options['use_more_always'] = FALSE;
  $handler->display->display_options['access']['type'] = 'none';
  $handler->display->display_options['cache']['type'] = 'none';
  $handler->display->display_options['query']['type'] = 'views_query';
  $handler->display->display_options['query']['options']['distinct'] = TRUE;
  $handler->display->display_options['exposed_form']['type'] = 'basic';
  $handler->display->display_options['pager']['type'] = 'full';
  $handler->display->display_options['pager']['options']['items_per_page'] = '25';
  $handler->display->display_options['pager']['options']['offset'] = '0';
  $handler->display->display_options['pager']['options']['id'] = '0';
  $handler->display->display_options['style_plugin'] = 'list';
  $handler->display->display_options['style_options']['type'] = 'ol';
  $handler->display->display_options['row_plugin'] = 'fields';
  $handler->display->display_options['row_options']['inline'] = array(
    'title' => 'title',
    'field_ag_datepub' => 'field_ag_datepub',
  );
  $handler->display->display_options['row_options']['separator'] = ' - ';
  /* Footer: Global: Text area */
  $handler->display->display_options['footer']['text']['id'] = 'area';
  $handler->display->display_options['footer']['text']['table'] = 'views';
  $handler->display->display_options['footer']['text']['field'] = 'area';
  $handler->display->display_options['footer']['text']['content'] = '<?php
    $view = views_get_current_view();
    if ($view->total_rows > 0)
      print \'<hr /><center><div class="view-counter">Total: \' . $view->total_rows . \'</div></center>\';
?>
';
  $handler->display->display_options['footer']['text']['format'] = '3';
  /* Relationship: Broken/missing handler */
  $handler->display->display_options['relationships']['field_test_value']['id'] = 'field_test_value';
  $handler->display->display_options['relationships']['field_test_value']['table'] = 'field_data_field_test';
  $handler->display->display_options['relationships']['field_test_value']['field'] = 'field_test_value';
  /* Field: Content: Title */
  $handler->display->display_options['fields']['title']['id'] = 'title';
  $handler->display->display_options['fields']['title']['table'] = 'node';
  $handler->display->display_options['fields']['title']['field'] = 'title';
  $handler->display->display_options['fields']['title']['label'] = '';
  /* Field: Content: Date of publication */
  $handler->display->display_options['fields']['field_ag_datepub']['id'] = 'field_ag_datepub';
  $handler->display->display_options['fields']['field_ag_datepub']['table'] = 'field_data_field_ag_datepub';
  $handler->display->display_options['fields']['field_ag_datepub']['field'] = 'field_ag_datepub';
  $handler->display->display_options['fields']['field_ag_datepub']['label'] = '';
  $handler->display->display_options['fields']['field_ag_datepub']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['field_ag_datepub']['settings'] = array(
    'format_type' => 'year',
    'fromto' => 'both',
    'multiple_number' => '',
    'multiple_from' => '',
    'multiple_to' => '',
  );
  /* Contextual filter: Global: Null */
  $handler->display->display_options['arguments']['null']['id'] = 'null';
  $handler->display->display_options['arguments']['null']['table'] = 'views';
  $handler->display->display_options['arguments']['null']['field'] = 'null';
  $handler->display->display_options['arguments']['null']['exception']['title_enable'] = TRUE;
  $handler->display->display_options['arguments']['null']['title_enable'] = TRUE;
  $handler->display->display_options['arguments']['null']['title'] = 'Documents of type: %2';
  $handler->display->display_options['arguments']['null']['default_argument_type'] = 'fixed';
  $handler->display->display_options['arguments']['null']['summary']['format'] = 'default_summary';
  $handler->display->display_options['arguments']['null']['specify_validation'] = TRUE;
  /* Filter criterion: Content: Type */
  $handler->display->display_options['filters']['type']['id'] = 'type';
  $handler->display->display_options['filters']['type']['table'] = 'node';
  $handler->display->display_options['filters']['type']['field'] = 'type';
  $handler->display->display_options['filters']['type']['value'] = array(
    'agdlios' => 'agdlios',
  );
  $handler->display->display_options['filters']['type']['group'] = '0';
  $handler->display->display_options['filters']['type']['expose']['operator'] = FALSE;
  /* Filter criterion: Content: Published */
  $handler->display->display_options['filters']['status']['id'] = 'status';
  $handler->display->display_options['filters']['status']['table'] = 'node';
  $handler->display->display_options['filters']['status']['field'] = 'status';
  $handler->display->display_options['filters']['status']['value'] = '1';
  $handler->display->display_options['filters']['status']['group'] = '0';
  $handler->display->display_options['filters']['status']['expose']['operator'] = FALSE;

  /* Display: dlios-by-doctype */
  $handler = $view->new_display('page', 'dlios-by-doctype', 'page_1');
  $handler->display->display_options['defaults']['title'] = FALSE;
  $handler->display->display_options['title'] = 'Documents by type';
  $handler->display->display_options['defaults']['relationships'] = FALSE;
  $handler->display->display_options['defaults']['arguments'] = FALSE;
  /* Contextual filter: Content: Document type (field_ag_type) */
  $handler->display->display_options['arguments']['field_ag_type_tid']['id'] = 'field_ag_type_tid';
  $handler->display->display_options['arguments']['field_ag_type_tid']['table'] = 'field_data_field_ag_type';
  $handler->display->display_options['arguments']['field_ag_type_tid']['field'] = 'field_ag_type_tid';
  $handler->display->display_options['arguments']['field_ag_type_tid']['default_action'] = 'empty';
  $handler->display->display_options['arguments']['field_ag_type_tid']['title_enable'] = TRUE;
  $handler->display->display_options['arguments']['field_ag_type_tid']['title'] = 'Document by %1';
  $handler->display->display_options['arguments']['field_ag_type_tid']['default_argument_type'] = 'fixed';
  $handler->display->display_options['arguments']['field_ag_type_tid']['summary']['number_of_records'] = '0';
  $handler->display->display_options['arguments']['field_ag_type_tid']['summary']['format'] = 'default_summary';
  $handler->display->display_options['arguments']['field_ag_type_tid']['summary_options']['items_per_page'] = '25';
  /* Contextual filter: Global: Null */
  $handler->display->display_options['arguments']['null']['id'] = 'null';
  $handler->display->display_options['arguments']['null']['table'] = 'views';
  $handler->display->display_options['arguments']['null']['field'] = 'null';
  $handler->display->display_options['arguments']['null']['exception']['title_enable'] = TRUE;
  $handler->display->display_options['arguments']['null']['title_enable'] = TRUE;
  $handler->display->display_options['arguments']['null']['title'] = 'Documents of type: %2';
  $handler->display->display_options['arguments']['null']['default_argument_type'] = 'fixed';
  $handler->display->display_options['arguments']['null']['summary']['format'] = 'default_summary';
  $handler->display->display_options['arguments']['null']['specify_validation'] = TRUE;
  $handler->display->display_options['path'] = 'ag_dlios-by-doctype';

  /* Display: dlios-by-authors */
  $handler = $view->new_display('page', 'dlios-by-authors', 'page_2');
  $handler->display->display_options['defaults']['relationships'] = FALSE;
  $handler->display->display_options['defaults']['arguments'] = FALSE;
  /* Contextual filter: Content: Author(s) (field_ag_author) */
  $handler->display->display_options['arguments']['field_ag_author_nid']['id'] = 'field_ag_author_nid';
  $handler->display->display_options['arguments']['field_ag_author_nid']['table'] = 'field_data_field_ag_author';
  $handler->display->display_options['arguments']['field_ag_author_nid']['field'] = 'field_ag_author_nid';
  $handler->display->display_options['arguments']['field_ag_author_nid']['default_action'] = 'empty';
  $handler->display->display_options['arguments']['field_ag_author_nid']['title_enable'] = TRUE;
  $handler->display->display_options['arguments']['field_ag_author_nid']['title'] = 'Documents by %1';
  $handler->display->display_options['arguments']['field_ag_author_nid']['default_argument_type'] = 'fixed';
  $handler->display->display_options['arguments']['field_ag_author_nid']['summary']['number_of_records'] = '0';
  $handler->display->display_options['arguments']['field_ag_author_nid']['summary']['format'] = 'default_summary';
  $handler->display->display_options['arguments']['field_ag_author_nid']['summary_options']['items_per_page'] = '25';
  /* Contextual filter: Global: Null */
  $handler->display->display_options['arguments']['null']['id'] = 'null';
  $handler->display->display_options['arguments']['null']['table'] = 'views';
  $handler->display->display_options['arguments']['null']['field'] = 'null';
  $handler->display->display_options['arguments']['null']['exception']['title_enable'] = TRUE;
  $handler->display->display_options['arguments']['null']['default_argument_type'] = 'fixed';
  $handler->display->display_options['arguments']['null']['summary']['format'] = 'default_summary';
  $handler->display->display_options['arguments']['null']['specify_validation'] = TRUE;
  $handler->display->display_options['path'] = 'ag_dlios-by-authors';

  /* Display: dlios-by-books */
  $handler = $view->new_display('page', 'dlios-by-books', 'page_3');
  $handler->display->display_options['defaults']['title'] = FALSE;
  $handler->display->display_options['title'] = 'Documents by books';
  $handler->display->display_options['defaults']['relationships'] = FALSE;
  $handler->display->display_options['defaults']['arguments'] = FALSE;
  /* Contextual filter: Content: Book (field_ag_book) */
  $handler->display->display_options['arguments']['field_ag_book_nid']['id'] = 'field_ag_book_nid';
  $handler->display->display_options['arguments']['field_ag_book_nid']['table'] = 'field_data_field_ag_book';
  $handler->display->display_options['arguments']['field_ag_book_nid']['field'] = 'field_ag_book_nid';
  $handler->display->display_options['arguments']['field_ag_book_nid']['default_action'] = 'empty';
  $handler->display->display_options['arguments']['field_ag_book_nid']['title_enable'] = TRUE;
  $handler->display->display_options['arguments']['field_ag_book_nid']['title'] = 'Documents belonging to book "%1"';
  $handler->display->display_options['arguments']['field_ag_book_nid']['default_argument_type'] = 'fixed';
  $handler->display->display_options['arguments']['field_ag_book_nid']['summary']['number_of_records'] = '0';
  $handler->display->display_options['arguments']['field_ag_book_nid']['summary']['format'] = 'default_summary';
  $handler->display->display_options['arguments']['field_ag_book_nid']['summary_options']['items_per_page'] = '25';
  /* Contextual filter: Global: Null */
  $handler->display->display_options['arguments']['null']['id'] = 'null';
  $handler->display->display_options['arguments']['null']['table'] = 'views';
  $handler->display->display_options['arguments']['null']['field'] = 'null';
  $handler->display->display_options['arguments']['null']['exception']['title_enable'] = TRUE;
  $handler->display->display_options['arguments']['null']['default_argument_type'] = 'fixed';
  $handler->display->display_options['arguments']['null']['summary']['format'] = 'default_summary';
  $handler->display->display_options['arguments']['null']['specify_validation'] = TRUE;
  $handler->display->display_options['path'] = 'ag_dlios-by-books';

  /* Display: dlios-by-conferences */
  $handler = $view->new_display('page', 'dlios-by-conferences', 'page_4');
  $handler->display->display_options['defaults']['title'] = FALSE;
  $handler->display->display_options['title'] = 'Documents by conferences';
  $handler->display->display_options['defaults']['relationships'] = FALSE;
  $handler->display->display_options['defaults']['arguments'] = FALSE;
  /* Contextual filter: Content: Conference (field_ag_confname) */
  $handler->display->display_options['arguments']['field_ag_confname_nid']['id'] = 'field_ag_confname_nid';
  $handler->display->display_options['arguments']['field_ag_confname_nid']['table'] = 'field_data_field_ag_confname';
  $handler->display->display_options['arguments']['field_ag_confname_nid']['field'] = 'field_ag_confname_nid';
  $handler->display->display_options['arguments']['field_ag_confname_nid']['default_action'] = 'empty';
  $handler->display->display_options['arguments']['field_ag_confname_nid']['title_enable'] = TRUE;
  $handler->display->display_options['arguments']['field_ag_confname_nid']['title'] = 'Documents belonging to conference "%1"';
  $handler->display->display_options['arguments']['field_ag_confname_nid']['default_argument_type'] = 'fixed';
  $handler->display->display_options['arguments']['field_ag_confname_nid']['summary']['number_of_records'] = '0';
  $handler->display->display_options['arguments']['field_ag_confname_nid']['summary']['format'] = 'default_summary';
  $handler->display->display_options['arguments']['field_ag_confname_nid']['summary_options']['items_per_page'] = '25';
  /* Contextual filter: Global: Null */
  $handler->display->display_options['arguments']['null']['id'] = 'null';
  $handler->display->display_options['arguments']['null']['table'] = 'views';
  $handler->display->display_options['arguments']['null']['field'] = 'null';
  $handler->display->display_options['arguments']['null']['exception']['title_enable'] = TRUE;
  $handler->display->display_options['arguments']['null']['default_argument_type'] = 'fixed';
  $handler->display->display_options['arguments']['null']['summary']['format'] = 'default_summary';
  $handler->display->display_options['arguments']['null']['specify_validation'] = TRUE;
  $handler->display->display_options['path'] = 'ag_dlios-by-conferences';

  /* Display: dlios-by-corporatebody */
  $handler = $view->new_display('page', 'dlios-by-corporatebody', 'page_5');
  $handler->display->display_options['defaults']['title'] = FALSE;
  $handler->display->display_options['title'] = 'Documents by corporate body';
  $handler->display->display_options['defaults']['relationships'] = FALSE;
  $handler->display->display_options['defaults']['arguments'] = FALSE;
  /* Contextual filter: Content: Corporate author(s) (field_ag_corauthor) */
  $handler->display->display_options['arguments']['field_ag_corauthor_nid']['id'] = 'field_ag_corauthor_nid';
  $handler->display->display_options['arguments']['field_ag_corauthor_nid']['table'] = 'field_data_field_ag_corauthor';
  $handler->display->display_options['arguments']['field_ag_corauthor_nid']['field'] = 'field_ag_corauthor_nid';
  $handler->display->display_options['arguments']['field_ag_corauthor_nid']['default_action'] = 'empty';
  $handler->display->display_options['arguments']['field_ag_corauthor_nid']['title_enable'] = TRUE;
  $handler->display->display_options['arguments']['field_ag_corauthor_nid']['title'] = 'Documents by %1';
  $handler->display->display_options['arguments']['field_ag_corauthor_nid']['default_argument_type'] = 'fixed';
  $handler->display->display_options['arguments']['field_ag_corauthor_nid']['summary']['number_of_records'] = '0';
  $handler->display->display_options['arguments']['field_ag_corauthor_nid']['summary']['format'] = 'default_summary';
  $handler->display->display_options['arguments']['field_ag_corauthor_nid']['summary_options']['items_per_page'] = '25';
  /* Contextual filter: Global: Null */
  $handler->display->display_options['arguments']['null']['id'] = 'null';
  $handler->display->display_options['arguments']['null']['table'] = 'views';
  $handler->display->display_options['arguments']['null']['field'] = 'null';
  $handler->display->display_options['arguments']['null']['exception']['title_enable'] = TRUE;
  $handler->display->display_options['arguments']['null']['default_argument_type'] = 'fixed';
  $handler->display->display_options['arguments']['null']['summary']['format'] = 'default_summary';
  $handler->display->display_options['arguments']['null']['specify_validation'] = TRUE;
  $handler->display->display_options['path'] = 'ag_dlios-by-corporatebody';

  /* Display: dlios-by-journals */
  $handler = $view->new_display('page', 'dlios-by-journals', 'page_6');
  $handler->display->display_options['defaults']['title'] = FALSE;
  $handler->display->display_options['title'] = 'Documents by journals';
  $handler->display->display_options['defaults']['relationships'] = FALSE;
  $handler->display->display_options['defaults']['arguments'] = FALSE;
  /* Contextual filter: Content: Journal (field_ag_journal) */
  $handler->display->display_options['arguments']['field_ag_journal_nid']['id'] = 'field_ag_journal_nid';
  $handler->display->display_options['arguments']['field_ag_journal_nid']['table'] = 'field_data_field_ag_journal';
  $handler->display->display_options['arguments']['field_ag_journal_nid']['field'] = 'field_ag_journal_nid';
  $handler->display->display_options['arguments']['field_ag_journal_nid']['default_action'] = 'empty';
  $handler->display->display_options['arguments']['field_ag_journal_nid']['title_enable'] = TRUE;
  $handler->display->display_options['arguments']['field_ag_journal_nid']['title'] = 'Documents published in "%1"';
  $handler->display->display_options['arguments']['field_ag_journal_nid']['default_argument_type'] = 'fixed';
  $handler->display->display_options['arguments']['field_ag_journal_nid']['summary']['number_of_records'] = '0';
  $handler->display->display_options['arguments']['field_ag_journal_nid']['summary']['format'] = 'default_summary';
  $handler->display->display_options['arguments']['field_ag_journal_nid']['summary_options']['items_per_page'] = '25';
  /* Contextual filter: Global: Null */
  $handler->display->display_options['arguments']['null']['id'] = 'null';
  $handler->display->display_options['arguments']['null']['table'] = 'views';
  $handler->display->display_options['arguments']['null']['field'] = 'null';
  $handler->display->display_options['arguments']['null']['exception']['title_enable'] = TRUE;
  $handler->display->display_options['arguments']['null']['default_argument_type'] = 'fixed';
  $handler->display->display_options['arguments']['null']['summary']['format'] = 'default_summary';
  $handler->display->display_options['arguments']['null']['specify_validation'] = TRUE;
  $handler->display->display_options['path'] = 'ag_dlios-by-journals';

  /* Display: dlios-by-subjectsAgris */
  $handler = $view->new_display('page', 'dlios-by-subjectsAgris', 'page_7');
  $handler->display->display_options['defaults']['title'] = FALSE;
  $handler->display->display_options['title'] = 'Documents by Agris subjects';
  $handler->display->display_options['defaults']['relationships'] = FALSE;
  $handler->display->display_options['defaults']['arguments'] = FALSE;
  /* Contextual filter: Content: Has taxonomy term ID */
  $handler->display->display_options['arguments']['tid']['id'] = 'tid';
  $handler->display->display_options['arguments']['tid']['table'] = 'taxonomy_index';
  $handler->display->display_options['arguments']['tid']['field'] = 'tid';
  $handler->display->display_options['arguments']['tid']['exception']['title_enable'] = TRUE;
  $handler->display->display_options['arguments']['tid']['title_enable'] = TRUE;
  $handler->display->display_options['arguments']['tid']['title'] = 'Documents on topic "%1 - %2"';
  $handler->display->display_options['arguments']['tid']['default_argument_type'] = 'fixed';
  $handler->display->display_options['arguments']['tid']['summary']['number_of_records'] = '0';
  $handler->display->display_options['arguments']['tid']['summary']['format'] = 'default_summary';
  $handler->display->display_options['arguments']['tid']['summary_options']['items_per_page'] = '25';
  $handler->display->display_options['arguments']['tid']['specify_validation'] = TRUE;
  $handler->display->display_options['arguments']['tid']['validate']['type'] = 'taxonomy_term';
  $handler->display->display_options['arguments']['tid']['validate_options']['vocabularies'] = array(
    'vocabulary_10' => 'vocabulary_10',
  );
  $handler->display->display_options['arguments']['tid']['validate_options']['type'] = 'tids';
  $handler->display->display_options['arguments']['tid']['validate']['fail'] = 'empty';
  /* Contextual filter: Global: Null */
  $handler->display->display_options['arguments']['null']['id'] = 'null';
  $handler->display->display_options['arguments']['null']['table'] = 'views';
  $handler->display->display_options['arguments']['null']['field'] = 'null';
  $handler->display->display_options['arguments']['null']['exception']['title_enable'] = TRUE;
  $handler->display->display_options['arguments']['null']['default_argument_type'] = 'fixed';
  $handler->display->display_options['arguments']['null']['summary']['format'] = 'default_summary';
  $handler->display->display_options['arguments']['null']['specify_validation'] = TRUE;
  $handler->display->display_options['path'] = 'ag_dlios-by-subjects';

  /* Display: dlios-by-subjectsAgrovoc */
  $handler = $view->new_display('page', 'dlios-by-subjectsAgrovoc', 'page_8');
  $handler->display->display_options['defaults']['title'] = FALSE;
  $handler->display->display_options['title'] = 'Documents by Agrovoc subjects';
  $handler->display->display_options['defaults']['relationships'] = FALSE;
  $handler->display->display_options['defaults']['arguments'] = FALSE;
  /* Contextual filter: Content: Has taxonomy term ID */
  $handler->display->display_options['arguments']['tid']['id'] = 'tid';
  $handler->display->display_options['arguments']['tid']['table'] = 'taxonomy_index';
  $handler->display->display_options['arguments']['tid']['field'] = 'tid';
  $handler->display->display_options['arguments']['tid']['exception']['title'] = 'Tous / Toutes';
  $handler->display->display_options['arguments']['tid']['title_enable'] = TRUE;
  $handler->display->display_options['arguments']['tid']['title'] = 'Documents on topic "%1"';
  $handler->display->display_options['arguments']['tid']['default_argument_type'] = 'fixed';
  $handler->display->display_options['arguments']['tid']['summary']['number_of_records'] = '0';
  $handler->display->display_options['arguments']['tid']['summary']['format'] = 'default_summary';
  $handler->display->display_options['arguments']['tid']['summary_options']['items_per_page'] = '25';
  $handler->display->display_options['arguments']['tid']['specify_validation'] = TRUE;
  $handler->display->display_options['arguments']['tid']['validate']['type'] = 'taxonomy_term';
  $handler->display->display_options['arguments']['tid']['validate_options']['vocabularies'] = array(
    'vocabulary_16' => 'vocabulary_16',
  );
  $handler->display->display_options['arguments']['tid']['validate_options']['type'] = 'tids';
  $handler->display->display_options['arguments']['tid']['validate']['fail'] = 'empty';
  $handler->display->display_options['arguments']['tid']['reduce_duplicates'] = TRUE;
  $handler->display->display_options['path'] = 'ag_dliosbysubjectsagrovoc';
  $export['Ag_dlios_browse_by'] = $view;

  $view = new view();
  $view->name = 'Ag_dliosxml';
  $view->description = '';
  $view->tag = '';
  $view->base_table = 'node';
  $view->human_name = '';
  $view->core = 0;
  $view->api_version = '3.0';
  $view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */

  /* Display: Defaults */
  $handler = $view->new_display('default', 'Defaults', 'default');
  $handler->display->display_options['title'] = 'Agris AP export';
  $handler->display->display_options['items_per_page'] = 0;
  $handler->display->display_options['use_more_always'] = FALSE;
  $handler->display->display_options['access']['type'] = 'none';
  $handler->display->display_options['cache']['type'] = 'none';
  $handler->display->display_options['query']['type'] = 'views_query';
  $handler->display->display_options['query']['options']['distinct'] = TRUE;
  $handler->display->display_options['query']['options']['query_comment'] = FALSE;
  $handler->display->display_options['exposed_form']['type'] = 'basic';
  $handler->display->display_options['pager']['type'] = 'full';
  $handler->display->display_options['pager']['options']['items_per_page'] = '100';
  $handler->display->display_options['pager']['options']['offset'] = '0';
  $handler->display->display_options['pager']['options']['id'] = '0';
  $handler->display->display_options['pager']['options']['quantity'] = '9';
  $handler->display->display_options['style_plugin'] = 'views_xml_style';
  $handler->display->display_options['style_options']['root_element'] = 'resources';
  $handler->display->display_options['style_options']['top_child_object'] = 'resource';
  $handler->display->display_options['style_options']['plaintext_output'] = 1;
  $handler->display->display_options['style_options']['escape_as_CDATA'] = 'yes';
  $handler->display->display_options['style_options']['content_type'] = 'default';
  $handler->display->display_options['style_options']['header'] = '';
  $handler->display->display_options['style_options']['using_views_api_mode'] = 0;
  /* Field: Content: Title */
  $handler->display->display_options['fields']['title']['id'] = 'title';
  $handler->display->display_options['fields']['title']['table'] = 'node';
  $handler->display->display_options['fields']['title']['field'] = 'title';
  $handler->display->display_options['fields']['title']['label'] = 'dc:title';
  $handler->display->display_options['fields']['title']['link_to_node'] = FALSE;
  /* Field: Content: Conference */
  $handler->display->display_options['fields']['field_ag_confname']['id'] = 'field_ag_confname';
  $handler->display->display_options['fields']['field_ag_confname']['table'] = 'field_data_field_ag_confname';
  $handler->display->display_options['fields']['field_ag_confname']['field'] = 'field_ag_confname';
  $handler->display->display_options['fields']['field_ag_confname']['label'] = 'ags:creatorConference';
  $handler->display->display_options['fields']['field_ag_confname']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['field_ag_confname']['hide_empty'] = TRUE;
  $handler->display->display_options['fields']['field_ag_confname']['type'] = 'node_reference_plain';
  /* Field: Content: Date of publication */
  $handler->display->display_options['fields']['field_ag_datepub']['id'] = 'field_ag_datepub';
  $handler->display->display_options['fields']['field_ag_datepub']['table'] = 'field_data_field_ag_datepub';
  $handler->display->display_options['fields']['field_ag_datepub']['field'] = 'field_ag_datepub';
  $handler->display->display_options['fields']['field_ag_datepub']['label'] = 'dcterms:dateIssued';
  $handler->display->display_options['fields']['field_ag_datepub']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['field_ag_datepub']['settings'] = array(
    'format_type' => 'iso_date',
    'fromto' => 'both',
    'multiple_number' => '',
    'multiple_from' => '',
    'multiple_to' => '',
  );
  /* Field: Content: Agris Subject Categories */
  $handler->display->display_options['fields']['field_ag_agris']['id'] = 'field_ag_agris';
  $handler->display->display_options['fields']['field_ag_agris']['table'] = 'field_data_field_ag_agris';
  $handler->display->display_options['fields']['field_ag_agris']['field'] = 'field_ag_agris';
  $handler->display->display_options['fields']['field_ag_agris']['label'] = 'ags:subjectClassification';
  $handler->display->display_options['fields']['field_ag_agris']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['field_ag_agris']['hide_empty'] = TRUE;
  $handler->display->display_options['fields']['field_ag_agris']['type'] = 'taxonomy_term_reference_plain';
  $handler->display->display_options['fields']['field_ag_agris']['delta_offset'] = '0';
  $handler->display->display_options['fields']['field_ag_agris']['separator'] = '; ';
  /* Field: Content: Agrovoc terms */
  $handler->display->display_options['fields']['field_ag_agrovoc']['id'] = 'field_ag_agrovoc';
  $handler->display->display_options['fields']['field_ag_agrovoc']['table'] = 'field_data_field_ag_agrovoc';
  $handler->display->display_options['fields']['field_ag_agrovoc']['field'] = 'field_ag_agrovoc';
  $handler->display->display_options['fields']['field_ag_agrovoc']['label'] = 'ags:subjectThesaurus';
  $handler->display->display_options['fields']['field_ag_agrovoc']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['field_ag_agrovoc']['hide_empty'] = TRUE;
  $handler->display->display_options['fields']['field_ag_agrovoc']['type'] = 'taxonomy_term_reference_link';
  $handler->display->display_options['fields']['field_ag_agrovoc']['delta_offset'] = '0';
  $handler->display->display_options['fields']['field_ag_agrovoc']['separator'] = ';';
  /* Field: Content: Edition */
  $handler->display->display_options['fields']['field_ag_edition']['id'] = 'field_ag_edition';
  $handler->display->display_options['fields']['field_ag_edition']['table'] = 'field_data_field_ag_edition';
  $handler->display->display_options['fields']['field_ag_edition']['field'] = 'field_ag_edition';
  $handler->display->display_options['fields']['field_ag_edition']['label'] = 'ags:descriptionEdition';
  $handler->display->display_options['fields']['field_ag_edition']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['field_ag_edition']['hide_empty'] = TRUE;
  /* Field: Content: Notes */
  $handler->display->display_options['fields']['field_ag_notes']['id'] = 'field_ag_notes';
  $handler->display->display_options['fields']['field_ag_notes']['table'] = 'field_data_field_ag_notes';
  $handler->display->display_options['fields']['field_ag_notes']['field'] = 'field_ag_notes';
  $handler->display->display_options['fields']['field_ag_notes']['label'] = 'ags:descriptionNotes';
  $handler->display->display_options['fields']['field_ag_notes']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['field_ag_notes']['hide_empty'] = TRUE;
  /* Field: Content: File attachments */
  $handler->display->display_options['fields']['upload']['id'] = 'upload';
  $handler->display->display_options['fields']['upload']['table'] = 'field_data_upload';
  $handler->display->display_options['fields']['upload']['field'] = 'upload';
  $handler->display->display_options['fields']['upload']['label'] = 'dc:identifier';
  $handler->display->display_options['fields']['upload']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['upload']['hide_empty'] = TRUE;
  $handler->display->display_options['fields']['upload']['click_sort_column'] = 'fid';
  $handler->display->display_options['fields']['upload']['type'] = 'file_url_plain';
  $handler->display->display_options['fields']['upload']['delta_offset'] = '0';
  /* Field: Content: ISBN */
  $handler->display->display_options['fields']['field_ag_isbn']['id'] = 'field_ag_isbn';
  $handler->display->display_options['fields']['field_ag_isbn']['table'] = 'field_data_field_ag_isbn';
  $handler->display->display_options['fields']['field_ag_isbn']['field'] = 'field_ag_isbn';
  $handler->display->display_options['fields']['field_ag_isbn']['label'] = 'dc:identifier';
  $handler->display->display_options['fields']['field_ag_isbn']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['field_ag_isbn']['hide_empty'] = TRUE;
  $handler->display->display_options['fields']['field_ag_isbn']['type'] = 'text_plain';
  /* Field: Content: ISSN */
  $handler->display->display_options['fields']['field_ag_issn']['id'] = 'field_ag_issn';
  $handler->display->display_options['fields']['field_ag_issn']['table'] = 'field_data_field_ag_issn';
  $handler->display->display_options['fields']['field_ag_issn']['field'] = 'field_ag_issn';
  $handler->display->display_options['fields']['field_ag_issn']['label'] = 'dc:identifier';
  $handler->display->display_options['fields']['field_ag_issn']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['field_ag_issn']['hide_empty'] = TRUE;
  /* Field: Content: Document type */
  $handler->display->display_options['fields']['field_ag_type']['id'] = 'field_ag_type';
  $handler->display->display_options['fields']['field_ag_type']['table'] = 'field_data_field_ag_type';
  $handler->display->display_options['fields']['field_ag_type']['field'] = 'field_ag_type';
  $handler->display->display_options['fields']['field_ag_type']['label'] = 'dc:type';
  $handler->display->display_options['fields']['field_ag_type']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['field_ag_type']['hide_empty'] = TRUE;
  $handler->display->display_options['fields']['field_ag_type']['type'] = 'taxonomy_term_reference_plain';
  /* Field: Content: Extent */
  $handler->display->display_options['fields']['field_ag_pagination']['id'] = 'field_ag_pagination';
  $handler->display->display_options['fields']['field_ag_pagination']['table'] = 'field_data_field_ag_pagination';
  $handler->display->display_options['fields']['field_ag_pagination']['field'] = 'field_ag_pagination';
  $handler->display->display_options['fields']['field_ag_pagination']['label'] = 'dcterms:extent';
  $handler->display->display_options['fields']['field_ag_pagination']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['field_ag_pagination']['type'] = 'text_plain';
  /* Field: Content: Form */
  $handler->display->display_options['fields']['field_ag_form']['id'] = 'field_ag_form';
  $handler->display->display_options['fields']['field_ag_form']['table'] = 'field_data_field_ag_form';
  $handler->display->display_options['fields']['field_ag_form']['field'] = 'field_ag_form';
  $handler->display->display_options['fields']['field_ag_form']['label'] = 'dcterms:medium';
  $handler->display->display_options['fields']['field_ag_form']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['field_ag_form']['hide_empty'] = TRUE;
  /* Field: Content: Language */
  $handler->display->display_options['fields']['field_ag_language']['id'] = 'field_ag_language';
  $handler->display->display_options['fields']['field_ag_language']['table'] = 'field_data_field_ag_language';
  $handler->display->display_options['fields']['field_ag_language']['field'] = 'field_ag_language';
  $handler->display->display_options['fields']['field_ag_language']['label'] = 'dc:language';
  $handler->display->display_options['fields']['field_ag_language']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['field_ag_language']['hide_empty'] = TRUE;
  /* Field: Global: PHP */
  $handler->display->display_options['fields']['php']['id'] = 'php';
  $handler->display->display_options['fields']['php']['table'] = 'views';
  $handler->display->display_options['fields']['php']['field'] = 'php';
  $handler->display->display_options['fields']['php']['label'] = 'ags:citationIdentifier';
  $handler->display->display_options['fields']['php']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['php']['hide_empty'] = TRUE;
  $handler->display->display_options['fields']['php']['use_php_setup'] = 0;
  $handler->display->display_options['fields']['php']['php_value'] = '$source=$row->field_ag_journal;
if ($source != "" ){
$source=node_load($source);
$ids=$source->field_ag_issn[und][0][\'value\'];}
else
{
$source=$row->field_ag_book;
$source=node_load($source);
$ids=$source->field_ag_issn[und][0][\'value\'];
}
print($ids);

';
  $handler->display->display_options['fields']['php']['use_php_click_sortable'] = '0';
  $handler->display->display_options['fields']['php']['php_click_sortable'] = '';
  /* Field: Global: PHP */
  $handler->display->display_options['fields']['php_1']['id'] = 'php_1';
  $handler->display->display_options['fields']['php_1']['table'] = 'views';
  $handler->display->display_options['fields']['php_1']['field'] = 'php';
  $handler->display->display_options['fields']['php_1']['label'] = 'ags:citationNumber';
  $handler->display->display_options['fields']['php_1']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['php_1']['hide_empty'] = TRUE;
  $handler->display->display_options['fields']['php_1']['use_php_setup'] = 0;
  $handler->display->display_options['fields']['php_1']['php_value'] = '$number = $row->field_ag_volume . " " .$row->field_ag_number;
print($number); 
';
  $handler->display->display_options['fields']['php_1']['use_php_click_sortable'] = '0';
  $handler->display->display_options['fields']['php_1']['php_click_sortable'] = '';
  /* Sort criterion: Content: Updated date */
  $handler->display->display_options['sorts']['changed']['id'] = 'changed';
  $handler->display->display_options['sorts']['changed']['table'] = 'node';
  $handler->display->display_options['sorts']['changed']['field'] = 'changed';
  $handler->display->display_options['sorts']['changed']['order'] = 'DESC';
  /* Sort criterion: Content: Title */
  $handler->display->display_options['sorts']['title']['id'] = 'title';
  $handler->display->display_options['sorts']['title']['table'] = 'node';
  $handler->display->display_options['sorts']['title']['field'] = 'title';
  /* Filter criterion: Content: Type */
  $handler->display->display_options['filters']['type']['id'] = 'type';
  $handler->display->display_options['filters']['type']['table'] = 'node';
  $handler->display->display_options['filters']['type']['field'] = 'type';
  $handler->display->display_options['filters']['type']['value'] = array(
    'agdlios' => 'agdlios',
  );
  $handler->display->display_options['filters']['type']['group'] = '0';
  $handler->display->display_options['filters']['type']['expose']['operator'] = FALSE;
  /* Filter criterion: Content: Published */
  $handler->display->display_options['filters']['status']['id'] = 'status';
  $handler->display->display_options['filters']['status']['table'] = 'node';
  $handler->display->display_options['filters']['status']['field'] = 'status';
  $handler->display->display_options['filters']['status']['value'] = '1';
  $handler->display->display_options['filters']['status']['group'] = '0';
  $handler->display->display_options['filters']['status']['expose']['operator'] = FALSE;

  /* Display: dliosxml */
  $handler = $view->new_display('page', 'dliosxml', 'page_1');
  $handler->display->display_options['defaults']['relationships'] = FALSE;
  /* Relationship: Content: Book (field_ag_book) */
  $handler->display->display_options['relationships']['field_ag_book_nid']['id'] = 'field_ag_book_nid';
  $handler->display->display_options['relationships']['field_ag_book_nid']['table'] = 'field_data_field_ag_book';
  $handler->display->display_options['relationships']['field_ag_book_nid']['field'] = 'field_ag_book_nid';
  $handler->display->display_options['relationships']['field_ag_book_nid']['delta'] = '-1';
  /* Relationship: Content: Journal (field_ag_journal) */
  $handler->display->display_options['relationships']['field_ag_journal_nid']['id'] = 'field_ag_journal_nid';
  $handler->display->display_options['relationships']['field_ag_journal_nid']['table'] = 'field_data_field_ag_journal';
  $handler->display->display_options['relationships']['field_ag_journal_nid']['field'] = 'field_ag_journal_nid';
  $handler->display->display_options['relationships']['field_ag_journal_nid']['delta'] = '-1';
  $handler->display->display_options['defaults']['fields'] = FALSE;
  /* Field: Content: Title */
  $handler->display->display_options['fields']['title']['id'] = 'title';
  $handler->display->display_options['fields']['title']['table'] = 'node';
  $handler->display->display_options['fields']['title']['field'] = 'title';
  $handler->display->display_options['fields']['title']['label'] = 'dc:title';
  $handler->display->display_options['fields']['title']['link_to_node'] = FALSE;
  /* Field: Content: Subtitle */
  $handler->display->display_options['fields']['field_ag_subtitle']['id'] = 'field_ag_subtitle';
  $handler->display->display_options['fields']['field_ag_subtitle']['table'] = 'field_data_field_ag_subtitle';
  $handler->display->display_options['fields']['field_ag_subtitle']['field'] = 'field_ag_subtitle';
  $handler->display->display_options['fields']['field_ag_subtitle']['label'] = 'dcterms:alternative';
  $handler->display->display_options['fields']['field_ag_subtitle']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['field_ag_subtitle']['type'] = 'text_plain';
  $handler->display->display_options['fields']['field_ag_subtitle']['delta_offset'] = '0';
  $handler->display->display_options['fields']['field_ag_subtitle']['separator'] = ';';
  /* Field: Content: Author(s) */
  $handler->display->display_options['fields']['field_ag_author']['id'] = 'field_ag_author';
  $handler->display->display_options['fields']['field_ag_author']['table'] = 'field_data_field_ag_author';
  $handler->display->display_options['fields']['field_ag_author']['field'] = 'field_ag_author';
  $handler->display->display_options['fields']['field_ag_author']['label'] = 'ags:creatorPersonal';
  $handler->display->display_options['fields']['field_ag_author']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['field_ag_author']['type'] = 'node_reference_plain';
  $handler->display->display_options['fields']['field_ag_author']['delta_offset'] = '0';
  $handler->display->display_options['fields']['field_ag_author']['separator'] = ';';
  /* Field: Content: Corporate author(s) */
  $handler->display->display_options['fields']['field_ag_corauthor']['id'] = 'field_ag_corauthor';
  $handler->display->display_options['fields']['field_ag_corauthor']['table'] = 'field_data_field_ag_corauthor';
  $handler->display->display_options['fields']['field_ag_corauthor']['field'] = 'field_ag_corauthor';
  $handler->display->display_options['fields']['field_ag_corauthor']['label'] = 'ags:creatorCorporate';
  $handler->display->display_options['fields']['field_ag_corauthor']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['field_ag_corauthor']['type'] = 'node_reference_plain';
  $handler->display->display_options['fields']['field_ag_corauthor']['settings'] = array(
    'node_reference_view_mode' => 'full',
  );
  $handler->display->display_options['fields']['field_ag_corauthor']['delta_offset'] = '0';
  $handler->display->display_options['fields']['field_ag_corauthor']['separator'] = ';';
  /* Field: Content: Conference */
  $handler->display->display_options['fields']['field_ag_confname']['id'] = 'field_ag_confname';
  $handler->display->display_options['fields']['field_ag_confname']['table'] = 'field_data_field_ag_confname';
  $handler->display->display_options['fields']['field_ag_confname']['field'] = 'field_ag_confname';
  $handler->display->display_options['fields']['field_ag_confname']['label'] = 'ags:creatorConference';
  $handler->display->display_options['fields']['field_ag_confname']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['field_ag_confname']['hide_empty'] = TRUE;
  $handler->display->display_options['fields']['field_ag_confname']['type'] = 'node_reference_plain';
  /* Field: Content: Publisher */
  $handler->display->display_options['fields']['field_ag_publisher']['id'] = 'field_ag_publisher';
  $handler->display->display_options['fields']['field_ag_publisher']['table'] = 'field_data_field_ag_publisher';
  $handler->display->display_options['fields']['field_ag_publisher']['field'] = 'field_ag_publisher';
  $handler->display->display_options['fields']['field_ag_publisher']['label'] = 'ags:publisherName';
  $handler->display->display_options['fields']['field_ag_publisher']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['field_ag_publisher']['type'] = 'node_reference_plain';
  $handler->display->display_options['fields']['field_ag_publisher']['delta_offset'] = '0';
  $handler->display->display_options['fields']['field_ag_publisher']['separator'] = ';';
  /* Field: Content: Place of Publication */
  $handler->display->display_options['fields']['field_ag_publishplace']['id'] = 'field_ag_publishplace';
  $handler->display->display_options['fields']['field_ag_publishplace']['table'] = 'field_data_field_ag_publishplace';
  $handler->display->display_options['fields']['field_ag_publishplace']['field'] = 'field_ag_publishplace';
  $handler->display->display_options['fields']['field_ag_publishplace']['label'] = 'ags:publisherPlace';
  $handler->display->display_options['fields']['field_ag_publishplace']['delta_offset'] = '0';
  $handler->display->display_options['fields']['field_ag_publishplace']['separator'] = ';';
  /* Field: Content: Date of publication */
  $handler->display->display_options['fields']['field_ag_datepub']['id'] = 'field_ag_datepub';
  $handler->display->display_options['fields']['field_ag_datepub']['table'] = 'field_data_field_ag_datepub';
  $handler->display->display_options['fields']['field_ag_datepub']['field'] = 'field_ag_datepub';
  $handler->display->display_options['fields']['field_ag_datepub']['label'] = 'dcterms:dateIssued';
  $handler->display->display_options['fields']['field_ag_datepub']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['field_ag_datepub']['hide_empty'] = TRUE;
  $handler->display->display_options['fields']['field_ag_datepub']['settings'] = array(
    'format_type' => 'iso_date',
    'fromto' => 'both',
    'multiple_number' => '',
    'multiple_from' => '',
    'multiple_to' => '',
  );
  /* Field: Content: Agris Subject Categories */
  $handler->display->display_options['fields']['field_ag_agris']['id'] = 'field_ag_agris';
  $handler->display->display_options['fields']['field_ag_agris']['table'] = 'field_data_field_ag_agris';
  $handler->display->display_options['fields']['field_ag_agris']['field'] = 'field_ag_agris';
  $handler->display->display_options['fields']['field_ag_agris']['label'] = 'ags:subjectClassification';
  $handler->display->display_options['fields']['field_ag_agris']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['field_ag_agris']['hide_empty'] = TRUE;
  $handler->display->display_options['fields']['field_ag_agris']['type'] = 'taxonomy_term_reference_plain';
  $handler->display->display_options['fields']['field_ag_agris']['delta_offset'] = '0';
  $handler->display->display_options['fields']['field_ag_agris']['separator'] = ';';
  /* Field: Content: Agrovoc terms */
  $handler->display->display_options['fields']['field_ag_agrovoc']['id'] = 'field_ag_agrovoc';
  $handler->display->display_options['fields']['field_ag_agrovoc']['table'] = 'field_data_field_ag_agrovoc';
  $handler->display->display_options['fields']['field_ag_agrovoc']['field'] = 'field_ag_agrovoc';
  $handler->display->display_options['fields']['field_ag_agrovoc']['label'] = 'agrovoc_uri';
  $handler->display->display_options['fields']['field_ag_agrovoc']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['field_ag_agrovoc']['hide_empty'] = TRUE;
  $handler->display->display_options['fields']['field_ag_agrovoc']['type'] = 'agrovocfield_term_reference_agrovoc_uri';
  $handler->display->display_options['fields']['field_ag_agrovoc']['settings'] = array(
    'only_agid' => 1,
    'strip_html' => 0,
  );
  $handler->display->display_options['fields']['field_ag_agrovoc']['delta_offset'] = '0';
  $handler->display->display_options['fields']['field_ag_agrovoc']['separator'] = ';';
  /* Field: Content: Agrovoc terms */
  $handler->display->display_options['fields']['field_ag_agrovoc_1']['id'] = 'field_ag_agrovoc_1';
  $handler->display->display_options['fields']['field_ag_agrovoc_1']['table'] = 'field_data_field_ag_agrovoc';
  $handler->display->display_options['fields']['field_ag_agrovoc_1']['field'] = 'field_ag_agrovoc';
  $handler->display->display_options['fields']['field_ag_agrovoc_1']['label'] = 'ags:subjectThesaurus';
  $handler->display->display_options['fields']['field_ag_agrovoc_1']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['field_ag_agrovoc_1']['type'] = 'taxonomy_term_reference_plain';
  $handler->display->display_options['fields']['field_ag_agrovoc_1']['settings'] = array(
    'only_agid' => 0,
    'strip_html' => 0,
  );
  $handler->display->display_options['fields']['field_ag_agrovoc_1']['delta_offset'] = '0';
  $handler->display->display_options['fields']['field_ag_agrovoc_1']['separator'] = ';';
  /* Field: Content: Edition */
  $handler->display->display_options['fields']['field_ag_edition']['id'] = 'field_ag_edition';
  $handler->display->display_options['fields']['field_ag_edition']['table'] = 'field_data_field_ag_edition';
  $handler->display->display_options['fields']['field_ag_edition']['field'] = 'field_ag_edition';
  $handler->display->display_options['fields']['field_ag_edition']['label'] = 'ags:descriptionEdition';
  $handler->display->display_options['fields']['field_ag_edition']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['field_ag_edition']['hide_empty'] = TRUE;
  /* Field: Content: Notes */
  $handler->display->display_options['fields']['field_ag_notes']['id'] = 'field_ag_notes';
  $handler->display->display_options['fields']['field_ag_notes']['table'] = 'field_data_field_ag_notes';
  $handler->display->display_options['fields']['field_ag_notes']['field'] = 'field_ag_notes';
  $handler->display->display_options['fields']['field_ag_notes']['label'] = 'ags:descriptionNotes';
  $handler->display->display_options['fields']['field_ag_notes']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['field_ag_notes']['hide_empty'] = TRUE;
  /* Field: Content: Abstract */
  $handler->display->display_options['fields']['field_ag_abstract']['id'] = 'field_ag_abstract';
  $handler->display->display_options['fields']['field_ag_abstract']['table'] = 'field_data_field_ag_abstract';
  $handler->display->display_options['fields']['field_ag_abstract']['field'] = 'field_ag_abstract';
  $handler->display->display_options['fields']['field_ag_abstract']['label'] = 'dcterms:abstract';
  $handler->display->display_options['fields']['field_ag_abstract']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['field_ag_abstract']['type'] = 'text_plain';
  $handler->display->display_options['fields']['field_ag_abstract']['delta_offset'] = '0';
  $handler->display->display_options['fields']['field_ag_abstract']['separator'] = ';';
  /* Field: Content: File attachments */
  $handler->display->display_options['fields']['upload']['id'] = 'upload';
  $handler->display->display_options['fields']['upload']['table'] = 'field_data_upload';
  $handler->display->display_options['fields']['upload']['field'] = 'upload';
  $handler->display->display_options['fields']['upload']['label'] = 'dc:identifier';
  $handler->display->display_options['fields']['upload']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['upload']['hide_empty'] = TRUE;
  $handler->display->display_options['fields']['upload']['click_sort_column'] = 'fid';
  $handler->display->display_options['fields']['upload']['type'] = 'file_url_plain';
  $handler->display->display_options['fields']['upload']['delta_offset'] = '0';
  /* Field: Content: Web URL */
  $handler->display->display_options['fields']['field_ag_resource_url']['id'] = 'field_ag_resource_url';
  $handler->display->display_options['fields']['field_ag_resource_url']['table'] = 'field_data_field_ag_resource_url';
  $handler->display->display_options['fields']['field_ag_resource_url']['field'] = 'field_ag_resource_url';
  $handler->display->display_options['fields']['field_ag_resource_url']['label'] = 'dc:identifier';
  $handler->display->display_options['fields']['field_ag_resource_url']['hide_empty'] = TRUE;
  $handler->display->display_options['fields']['field_ag_resource_url']['click_sort_column'] = 'url';
  $handler->display->display_options['fields']['field_ag_resource_url']['type'] = 'link_title_plain';
  /* Field: Content: ISBN */
  $handler->display->display_options['fields']['field_ag_isbn']['id'] = 'field_ag_isbn';
  $handler->display->display_options['fields']['field_ag_isbn']['table'] = 'field_data_field_ag_isbn';
  $handler->display->display_options['fields']['field_ag_isbn']['field'] = 'field_ag_isbn';
  $handler->display->display_options['fields']['field_ag_isbn']['label'] = 'dc:identifier';
  $handler->display->display_options['fields']['field_ag_isbn']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['field_ag_isbn']['hide_empty'] = TRUE;
  $handler->display->display_options['fields']['field_ag_isbn']['type'] = 'text_plain';
  /* Field: Content: ISSN */
  $handler->display->display_options['fields']['field_ag_issn']['id'] = 'field_ag_issn';
  $handler->display->display_options['fields']['field_ag_issn']['table'] = 'field_data_field_ag_issn';
  $handler->display->display_options['fields']['field_ag_issn']['field'] = 'field_ag_issn';
  $handler->display->display_options['fields']['field_ag_issn']['label'] = 'dc:identifier';
  $handler->display->display_options['fields']['field_ag_issn']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['field_ag_issn']['hide_empty'] = TRUE;
  /* Field: Content: E_ISSN */
  $handler->display->display_options['fields']['field_ag_eissn']['id'] = 'field_ag_eissn';
  $handler->display->display_options['fields']['field_ag_eissn']['table'] = 'field_data_field_ag_eissn';
  $handler->display->display_options['fields']['field_ag_eissn']['field'] = 'field_ag_eissn';
  $handler->display->display_options['fields']['field_ag_eissn']['label'] = 'dc:identifier';
  $handler->display->display_options['fields']['field_ag_eissn']['element_label_colon'] = FALSE;
  /* Field: Content: Document type */
  $handler->display->display_options['fields']['field_ag_type']['id'] = 'field_ag_type';
  $handler->display->display_options['fields']['field_ag_type']['table'] = 'field_data_field_ag_type';
  $handler->display->display_options['fields']['field_ag_type']['field'] = 'field_ag_type';
  $handler->display->display_options['fields']['field_ag_type']['label'] = 'dc:type';
  $handler->display->display_options['fields']['field_ag_type']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['field_ag_type']['hide_empty'] = TRUE;
  $handler->display->display_options['fields']['field_ag_type']['type'] = 'taxonomy_term_reference_plain';
  /* Field: Content: Extent */
  $handler->display->display_options['fields']['field_ag_pagination']['id'] = 'field_ag_pagination';
  $handler->display->display_options['fields']['field_ag_pagination']['table'] = 'field_data_field_ag_pagination';
  $handler->display->display_options['fields']['field_ag_pagination']['field'] = 'field_ag_pagination';
  $handler->display->display_options['fields']['field_ag_pagination']['label'] = 'dcterms:extent';
  $handler->display->display_options['fields']['field_ag_pagination']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['field_ag_pagination']['hide_empty'] = TRUE;
  $handler->display->display_options['fields']['field_ag_pagination']['type'] = 'text_plain';
  /* Field: Content: Form */
  $handler->display->display_options['fields']['field_ag_form']['id'] = 'field_ag_form';
  $handler->display->display_options['fields']['field_ag_form']['table'] = 'field_data_field_ag_form';
  $handler->display->display_options['fields']['field_ag_form']['field'] = 'field_ag_form';
  $handler->display->display_options['fields']['field_ag_form']['label'] = 'dcterms:medium';
  $handler->display->display_options['fields']['field_ag_form']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['field_ag_form']['hide_empty'] = TRUE;
  /* Field: Content: Language */
  $handler->display->display_options['fields']['field_ag_language']['id'] = 'field_ag_language';
  $handler->display->display_options['fields']['field_ag_language']['table'] = 'field_data_field_ag_language';
  $handler->display->display_options['fields']['field_ag_language']['field'] = 'field_ag_language';
  $handler->display->display_options['fields']['field_ag_language']['label'] = 'dc:language';
  $handler->display->display_options['fields']['field_ag_language']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['field_ag_language']['hide_empty'] = TRUE;
  $handler->display->display_options['fields']['field_ag_language']['delta_offset'] = '0';
  $handler->display->display_options['fields']['field_ag_language']['separator'] = ';';
  /* Field: Content: Physical Location */
  $handler->display->display_options['fields']['field_ag_phlocation']['id'] = 'field_ag_phlocation';
  $handler->display->display_options['fields']['field_ag_phlocation']['table'] = 'field_data_field_ag_phlocation';
  $handler->display->display_options['fields']['field_ag_phlocation']['field'] = 'field_ag_phlocation';
  $handler->display->display_options['fields']['field_ag_phlocation']['label'] = 'ags:availabilityLocation';
  $handler->display->display_options['fields']['field_ag_phlocation']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['field_ag_phlocation']['delta_offset'] = '0';
  $handler->display->display_options['fields']['field_ag_phlocation']['separator'] = ';';
  /* Field: Content: Shelf Locator */
  $handler->display->display_options['fields']['field_ag_locator']['id'] = 'field_ag_locator';
  $handler->display->display_options['fields']['field_ag_locator']['table'] = 'field_data_field_ag_locator';
  $handler->display->display_options['fields']['field_ag_locator']['field'] = 'field_ag_locator';
  $handler->display->display_options['fields']['field_ag_locator']['label'] = 'ags:availabilityNumber';
  $handler->display->display_options['fields']['field_ag_locator']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['field_ag_locator']['delta_offset'] = '0';
  $handler->display->display_options['fields']['field_ag_locator']['separator'] = ';';
  /* Field: Content: Journal */
  $handler->display->display_options['fields']['field_ag_journal']['id'] = 'field_ag_journal';
  $handler->display->display_options['fields']['field_ag_journal']['table'] = 'field_data_field_ag_journal';
  $handler->display->display_options['fields']['field_ag_journal']['field'] = 'field_ag_journal';
  $handler->display->display_options['fields']['field_ag_journal']['label'] = 'ags:citationTitle';
  $handler->display->display_options['fields']['field_ag_journal']['hide_empty'] = TRUE;
  $handler->display->display_options['fields']['field_ag_journal']['type'] = 'node_reference_plain';
  /* Field: Content: Book */
  $handler->display->display_options['fields']['field_ag_book']['id'] = 'field_ag_book';
  $handler->display->display_options['fields']['field_ag_book']['table'] = 'field_data_field_ag_book';
  $handler->display->display_options['fields']['field_ag_book']['field'] = 'field_ag_book';
  $handler->display->display_options['fields']['field_ag_book']['label'] = 'dc:source';
  $handler->display->display_options['fields']['field_ag_book']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['field_ag_book']['hide_empty'] = TRUE;
  $handler->display->display_options['fields']['field_ag_book']['type'] = 'node_reference_plain';
  /* Field: Content: Number */
  $handler->display->display_options['fields']['field_ag_number']['id'] = 'field_ag_number';
  $handler->display->display_options['fields']['field_ag_number']['table'] = 'field_data_field_ag_number';
  $handler->display->display_options['fields']['field_ag_number']['field'] = 'field_ag_number';
  $handler->display->display_options['fields']['field_ag_number']['label'] = '';
  $handler->display->display_options['fields']['field_ag_number']['exclude'] = TRUE;
  $handler->display->display_options['fields']['field_ag_number']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['field_ag_number']['hide_empty'] = TRUE;
  /* Field: Content: Volume */
  $handler->display->display_options['fields']['field_ag_volume']['id'] = 'field_ag_volume';
  $handler->display->display_options['fields']['field_ag_volume']['table'] = 'field_data_field_ag_volume';
  $handler->display->display_options['fields']['field_ag_volume']['field'] = 'field_ag_volume';
  $handler->display->display_options['fields']['field_ag_volume']['label'] = 'ags:citationNumber';
  $handler->display->display_options['fields']['field_ag_volume']['alter']['alter_text'] = TRUE;
  $handler->display->display_options['fields']['field_ag_volume']['alter']['text'] = 'v.[field_ag_volume]/n.[field_ag_number]';
  $handler->display->display_options['fields']['field_ag_volume']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['field_ag_volume']['hide_empty'] = TRUE;
  $handler->display->display_options['fields']['field_ag_volume']['hide_alter_empty'] = FALSE;
  $handler->display->display_options['fields']['field_ag_volume']['type'] = 'text_plain';
  /* Field: Content: ISSN */
  $handler->display->display_options['fields']['field_ag_issn_1']['id'] = 'field_ag_issn_1';
  $handler->display->display_options['fields']['field_ag_issn_1']['table'] = 'field_data_field_ag_issn';
  $handler->display->display_options['fields']['field_ag_issn_1']['field'] = 'field_ag_issn';
  $handler->display->display_options['fields']['field_ag_issn_1']['relationship'] = 'field_ag_journal_nid';
  $handler->display->display_options['fields']['field_ag_issn_1']['label'] = 'ags:citationIdentifier';
  $handler->display->display_options['fields']['field_ag_issn_1']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['field_ag_issn_1']['hide_empty'] = TRUE;
  $handler->display->display_options['path'] = 'ag_dlios-xml';

  /* Display: Page */
  $handler = $view->new_display('page', 'Page', 'page_2');
  $handler->display->display_options['defaults']['style_plugin'] = FALSE;
  $handler->display->display_options['style_plugin'] = 'default';
  $handler->display->display_options['defaults']['style_options'] = FALSE;
  $handler->display->display_options['defaults']['row_plugin'] = FALSE;
  $handler->display->display_options['row_plugin'] = 'fields';
  $handler->display->display_options['row_options']['hide_empty'] = TRUE;
  $handler->display->display_options['defaults']['row_options'] = FALSE;
  $handler->display->display_options['path'] = 'ag_oai-results';
  $export['Ag_dliosxml'] = $view;

  $view = new view();
  $view->name = 'Ag_experts_institution';
  $view->description = 'Experts: list / search';
  $view->tag = 'custom';
  $view->base_table = 'node';
  $view->human_name = '';
  $view->core = 0;
  $view->api_version = '3.0';
  $view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */

  /* Display: Experts: search and browse */
  $handler = $view->new_display('default', 'Experts: search and browse', 'default');
  $handler->display->display_options['title'] = 'Experts';
  $handler->display->display_options['use_more_always'] = FALSE;
  $handler->display->display_options['access']['type'] = 'none';
  $handler->display->display_options['cache']['type'] = 'none';
  $handler->display->display_options['query']['type'] = 'views_query';
  $handler->display->display_options['query']['options']['distinct'] = TRUE;
  $handler->display->display_options['exposed_form']['type'] = 'basic';
  $handler->display->display_options['exposed_form']['options']['submit_button'] = 'Search';
  $handler->display->display_options['exposed_form']['options']['reset_button'] = TRUE;
  $handler->display->display_options['pager']['type'] = 'full';
  $handler->display->display_options['pager']['options']['items_per_page'] = 15;
  $handler->display->display_options['style_plugin'] = 'table';
  $handler->display->display_options['style_options']['grouping'] = '';
  $handler->display->display_options['style_options']['columns'] = array(
    'title' => 'title',
    'field_nationality_value' => 'field_nationality_value',
    'field_country_value' => 'field_country_value',
    'field_degree_value' => 'field_degree_value',
    'field_institution_nid' => 'field_institution_nid',
    'field_position_value' => 'field_position_value',
  );
  $handler->display->display_options['style_options']['default'] = 'title';
  $handler->display->display_options['style_options']['info'] = array(
    'title' => array(
      'sortable' => 1,
      'separator' => '',
    ),
    'field_nationality_value' => array(
      'sortable' => 1,
      'separator' => '',
    ),
    'field_country_value' => array(
      'sortable' => 1,
      'separator' => '',
    ),
    'field_degree_value' => array(
      'sortable' => 1,
      'separator' => '',
    ),
    'field_institution_nid' => array(
      'separator' => '',
    ),
    'field_position_value' => array(
      'separator' => '',
    ),
  );
  /* Footer: Global: Text area */
  $handler->display->display_options['footer']['text']['id'] = 'text';
  $handler->display->display_options['footer']['text']['table'] = 'views';
  $handler->display->display_options['footer']['text']['field'] = 'area';
  $handler->display->display_options['footer']['text']['empty'] = TRUE;
  $handler->display->display_options['footer']['text']['content'] = '<script language="javascript">
  selects = document.getElementsByTagName("select");
  for (i=0; i<selects.length; i++) {
    if (selects[i].size > 4)
      selects[i].size = 4;
  }
</script>
<?php
  global $base_url;
?>
<div align="right"><a href="<?php echo $base_url;?>/ag_institutions/rss" target="_blank"><img src="<?php echo $base_url;?>/sites/default/files/images/RSS_feed.png" /> RSS feed</a> [<i>compliant with the Ag-Org AP</i>] - <a href="<?php echo $base_url;?>/ag_institutions/rdf" target="_blank"><img src="http://www.w3.org/RDF/icons/rdf_flyer.24" /> RDF feed</a></div>';
  $handler->display->display_options['footer']['text']['format'] = '3';
  /* Field: Content: Title */
  $handler->display->display_options['fields']['title']['id'] = 'title';
  $handler->display->display_options['fields']['title']['table'] = 'node';
  $handler->display->display_options['fields']['title']['field'] = 'title';
  $handler->display->display_options['fields']['title']['label'] = 'Name';
  /* Sort criterion: Content: Title */
  $handler->display->display_options['sorts']['title']['id'] = 'title';
  $handler->display->display_options['sorts']['title']['table'] = 'node';
  $handler->display->display_options['sorts']['title']['field'] = 'title';
  /* Filter criterion: Content: Published */
  $handler->display->display_options['filters']['status']['id'] = 'status';
  $handler->display->display_options['filters']['status']['table'] = 'node';
  $handler->display->display_options['filters']['status']['field'] = 'status';
  $handler->display->display_options['filters']['status']['value'] = '1';
  $handler->display->display_options['filters']['status']['group'] = '0';
  $handler->display->display_options['filters']['status']['expose']['operator'] = FALSE;
  /* Filter criterion: Content: Type */
  $handler->display->display_options['filters']['type']['id'] = 'type';
  $handler->display->display_options['filters']['type']['table'] = 'node';
  $handler->display->display_options['filters']['type']['field'] = 'type';
  $handler->display->display_options['filters']['type']['value'] = array(
    'agprofile' => 'agprofile',
  );
  $handler->display->display_options['filters']['type']['group'] = '0';
  $handler->display->display_options['filters']['type']['expose']['operator'] = FALSE;
  /* Filter criterion: Content: Title */
  $handler->display->display_options['filters']['title']['id'] = 'title';
  $handler->display->display_options['filters']['title']['table'] = 'node';
  $handler->display->display_options['filters']['title']['field'] = 'title';
  $handler->display->display_options['filters']['title']['operator'] = 'contains';
  $handler->display->display_options['filters']['title']['group'] = '0';
  $handler->display->display_options['filters']['title']['exposed'] = TRUE;
  $handler->display->display_options['filters']['title']['expose']['operator_id'] = 'title_op';
  $handler->display->display_options['filters']['title']['expose']['label'] = 'Name';
  $handler->display->display_options['filters']['title']['expose']['operator'] = 'title_op';
  $handler->display->display_options['filters']['title']['expose']['identifier'] = 'title';
  /* Filter criterion: Content: Has taxonomy term */
  $handler->display->display_options['filters']['tid']['id'] = 'tid';
  $handler->display->display_options['filters']['tid']['table'] = 'taxonomy_index';
  $handler->display->display_options['filters']['tid']['field'] = 'tid';
  $handler->display->display_options['filters']['tid']['value'] = '';
  $handler->display->display_options['filters']['tid']['group'] = '0';
  $handler->display->display_options['filters']['tid']['exposed'] = TRUE;
  $handler->display->display_options['filters']['tid']['expose']['operator_id'] = 'tid_op';
  $handler->display->display_options['filters']['tid']['expose']['label'] = 'Field of activity (Agrovoc terms) ';
  $handler->display->display_options['filters']['tid']['expose']['operator'] = 'tid_op';
  $handler->display->display_options['filters']['tid']['expose']['identifier'] = 'tid';
  $handler->display->display_options['filters']['tid']['reduce_duplicates'] = TRUE;
  $handler->display->display_options['filters']['tid']['vocabulary'] = 'vocabulary_16';

  /* Display: Experts: advanced search */
  $handler = $view->new_display('page', 'Experts: advanced search', 'page_1');
  $handler->display->display_options['defaults']['title'] = FALSE;
  $handler->display->display_options['title'] = 'Experts: advanced search';
  $handler->display->display_options['defaults']['fields'] = FALSE;
  /* Field: Content: Title */
  $handler->display->display_options['fields']['title']['id'] = 'title';
  $handler->display->display_options['fields']['title']['table'] = 'node';
  $handler->display->display_options['fields']['title']['field'] = 'title';
  $handler->display->display_options['fields']['title']['label'] = 'Name';
  /* Field: Content: Country */
  $handler->display->display_options['fields']['field_ag_country_1']['id'] = 'field_ag_country_1';
  $handler->display->display_options['fields']['field_ag_country_1']['table'] = 'field_data_field_ag_country';
  $handler->display->display_options['fields']['field_ag_country_1']['field'] = 'field_ag_country';
  $handler->display->display_options['fields']['field_ag_country_1']['label'] = 'Nationality';
  $handler->display->display_options['fields']['field_ag_country_1']['element_label_colon'] = FALSE;
  /* Field: Content: Country */
  $handler->display->display_options['fields']['field_ag_country']['id'] = 'field_ag_country';
  $handler->display->display_options['fields']['field_ag_country']['table'] = 'field_data_field_ag_country';
  $handler->display->display_options['fields']['field_ag_country']['field'] = 'field_ag_country';
  $handler->display->display_options['fields']['field_ag_country']['element_label_colon'] = FALSE;
  /* Field: Content: Highest degree */
  $handler->display->display_options['fields']['field_ag_degree']['id'] = 'field_ag_degree';
  $handler->display->display_options['fields']['field_ag_degree']['table'] = 'field_data_field_ag_degree';
  $handler->display->display_options['fields']['field_ag_degree']['field'] = 'field_ag_degree';
  $handler->display->display_options['fields']['field_ag_degree']['element_label_colon'] = FALSE;
  /* Field: Content: Position */
  $handler->display->display_options['fields']['field_ag_position']['id'] = 'field_ag_position';
  $handler->display->display_options['fields']['field_ag_position']['table'] = 'field_data_field_ag_position';
  $handler->display->display_options['fields']['field_ag_position']['field'] = 'field_ag_position';
  $handler->display->display_options['fields']['field_ag_position']['exclude'] = TRUE;
  $handler->display->display_options['fields']['field_ag_position']['type'] = 'text_plain';
  $handler->display->display_options['fields']['field_ag_position']['delta_offset'] = '0';
  /* Field: Content: Institution */
  $handler->display->display_options['fields']['field_ag_institution']['id'] = 'field_ag_institution';
  $handler->display->display_options['fields']['field_ag_institution']['table'] = 'field_data_field_ag_institution';
  $handler->display->display_options['fields']['field_ag_institution']['field'] = 'field_ag_institution';
  $handler->display->display_options['fields']['field_ag_institution']['label'] = 'Latest affiliation(s)';
  $handler->display->display_options['fields']['field_ag_institution']['type'] = 'node_reference_plain';
  $handler->display->display_options['fields']['field_ag_institution']['delta_limit'] = '2';
  $handler->display->display_options['fields']['field_ag_institution']['delta_offset'] = '0';
  $handler->display->display_options['fields']['field_ag_institution']['multi_type'] = 'ul';
  /* Field: Content: Acronym */
  $handler->display->display_options['fields']['field_ag_acronym']['id'] = 'field_ag_acronym';
  $handler->display->display_options['fields']['field_ag_acronym']['table'] = 'field_data_field_ag_acronym';
  $handler->display->display_options['fields']['field_ag_acronym']['field'] = 'field_ag_acronym';
  $handler->display->display_options['fields']['field_ag_acronym']['element_label_colon'] = FALSE;
  $handler->display->display_options['defaults']['filter_groups'] = FALSE;
  $handler->display->display_options['defaults']['filters'] = FALSE;
  /* Filter criterion: Content: Published */
  $handler->display->display_options['filters']['status']['id'] = 'status';
  $handler->display->display_options['filters']['status']['table'] = 'node';
  $handler->display->display_options['filters']['status']['field'] = 'status';
  $handler->display->display_options['filters']['status']['value'] = '1';
  $handler->display->display_options['filters']['status']['group'] = 1;
  $handler->display->display_options['filters']['status']['expose']['operator'] = FALSE;
  /* Filter criterion: Content: Type */
  $handler->display->display_options['filters']['type']['id'] = 'type';
  $handler->display->display_options['filters']['type']['table'] = 'node';
  $handler->display->display_options['filters']['type']['field'] = 'type';
  $handler->display->display_options['filters']['type']['value'] = array(
    'agprofile' => 'agprofile',
  );
  $handler->display->display_options['filters']['type']['group'] = 1;
  $handler->display->display_options['filters']['type']['expose']['operator'] = FALSE;
  /* Filter criterion: Content: Title */
  $handler->display->display_options['filters']['title']['id'] = 'title';
  $handler->display->display_options['filters']['title']['table'] = 'node';
  $handler->display->display_options['filters']['title']['field'] = 'title';
  $handler->display->display_options['filters']['title']['operator'] = 'contains';
  $handler->display->display_options['filters']['title']['group'] = 1;
  $handler->display->display_options['filters']['title']['exposed'] = TRUE;
  $handler->display->display_options['filters']['title']['expose']['operator_id'] = 'title_op';
  $handler->display->display_options['filters']['title']['expose']['label'] = 'Name';
  $handler->display->display_options['filters']['title']['expose']['operator'] = 'title_op';
  $handler->display->display_options['filters']['title']['expose']['identifier'] = 'title';
  $handler->display->display_options['filters']['title']['expose']['remember_roles'] = array(
    2 => '2',
    3 => 0,
    1 => 0,
    6 => 0,
    7 => 0,
    4 => 0,
    5 => 0,
  );
  /* Filter criterion: Content: Country (field_ag_country) */
  $handler->display->display_options['filters']['field_ag_country_get_list_from_file']['id'] = 'field_ag_country_get_list_from_file';
  $handler->display->display_options['filters']['field_ag_country_get_list_from_file']['table'] = 'field_data_field_ag_country';
  $handler->display->display_options['filters']['field_ag_country_get_list_from_file']['field'] = 'field_ag_country_get_list_from_file';
  $handler->display->display_options['filters']['field_ag_country_get_list_from_file']['group'] = 1;
  $handler->display->display_options['filters']['field_ag_country_get_list_from_file']['exposed'] = TRUE;
  $handler->display->display_options['filters']['field_ag_country_get_list_from_file']['expose']['operator_id'] = 'field_ag_country_get_list_from_file_op';
  $handler->display->display_options['filters']['field_ag_country_get_list_from_file']['expose']['label'] = 'Nationality';
  $handler->display->display_options['filters']['field_ag_country_get_list_from_file']['expose']['operator'] = 'field_ag_country_get_list_from_file_op';
  $handler->display->display_options['filters']['field_ag_country_get_list_from_file']['expose']['identifier'] = 'field_ag_country_get_list_from_file';
  $handler->display->display_options['filters']['field_ag_country_get_list_from_file']['expose']['remember_roles'] = array(
    2 => '2',
    3 => 0,
    1 => 0,
    6 => 0,
    7 => 0,
    4 => 0,
    5 => 0,
  );
  /* Filter criterion: Content: Agrovoc terms (field_ag_agrovoc) */
  $handler->display->display_options['filters']['field_ag_agrovoc_tid']['id'] = 'field_ag_agrovoc_tid';
  $handler->display->display_options['filters']['field_ag_agrovoc_tid']['table'] = 'field_data_field_ag_agrovoc';
  $handler->display->display_options['filters']['field_ag_agrovoc_tid']['field'] = 'field_ag_agrovoc_tid';
  $handler->display->display_options['filters']['field_ag_agrovoc_tid']['value'] = '';
  $handler->display->display_options['filters']['field_ag_agrovoc_tid']['group'] = 1;
  $handler->display->display_options['filters']['field_ag_agrovoc_tid']['exposed'] = TRUE;
  $handler->display->display_options['filters']['field_ag_agrovoc_tid']['expose']['operator_id'] = 'field_ag_agrovoc_tid_op';
  $handler->display->display_options['filters']['field_ag_agrovoc_tid']['expose']['label'] = 'Expertise (Agrovoc terms)';
  $handler->display->display_options['filters']['field_ag_agrovoc_tid']['expose']['operator'] = 'field_ag_agrovoc_tid_op';
  $handler->display->display_options['filters']['field_ag_agrovoc_tid']['expose']['identifier'] = 'field_ag_agrovoc_tid';
  $handler->display->display_options['filters']['field_ag_agrovoc_tid']['expose']['remember_roles'] = array(
    2 => '2',
    3 => 0,
    1 => 0,
    6 => 0,
    7 => 0,
    4 => 0,
    5 => 0,
  );
  $handler->display->display_options['filters']['field_ag_agrovoc_tid']['reduce_duplicates'] = TRUE;
  $handler->display->display_options['filters']['field_ag_agrovoc_tid']['vocabulary'] = 'vocabulary_16';
  /* Filter criterion: Content: Agris Subject Categories (field_ag_agris) */
  $handler->display->display_options['filters']['field_ag_agris_tid']['id'] = 'field_ag_agris_tid';
  $handler->display->display_options['filters']['field_ag_agris_tid']['table'] = 'field_data_field_ag_agris';
  $handler->display->display_options['filters']['field_ag_agris_tid']['field'] = 'field_ag_agris_tid';
  $handler->display->display_options['filters']['field_ag_agris_tid']['value'] = '';
  $handler->display->display_options['filters']['field_ag_agris_tid']['group'] = 1;
  $handler->display->display_options['filters']['field_ag_agris_tid']['exposed'] = TRUE;
  $handler->display->display_options['filters']['field_ag_agris_tid']['expose']['operator_id'] = 'field_ag_agris_tid_op';
  $handler->display->display_options['filters']['field_ag_agris_tid']['expose']['label'] = 'Expertise (ASC categories)';
  $handler->display->display_options['filters']['field_ag_agris_tid']['expose']['operator'] = 'field_ag_agris_tid_op';
  $handler->display->display_options['filters']['field_ag_agris_tid']['expose']['identifier'] = 'field_ag_agris_tid';
  $handler->display->display_options['filters']['field_ag_agris_tid']['expose']['remember_roles'] = array(
    2 => '2',
    3 => 0,
    1 => 0,
    6 => 0,
    7 => 0,
    4 => 0,
    5 => 0,
  );
  $handler->display->display_options['filters']['field_ag_agris_tid']['type'] = 'select';
  $handler->display->display_options['filters']['field_ag_agris_tid']['vocabulary'] = 'vocabulary_10';
  /* Filter criterion: Content: Gender (field_ag_gender) */
  $handler->display->display_options['filters']['field_ag_gender_value']['id'] = 'field_ag_gender_value';
  $handler->display->display_options['filters']['field_ag_gender_value']['table'] = 'field_data_field_ag_gender';
  $handler->display->display_options['filters']['field_ag_gender_value']['field'] = 'field_ag_gender_value';
  $handler->display->display_options['filters']['field_ag_gender_value']['group'] = 1;
  $handler->display->display_options['filters']['field_ag_gender_value']['exposed'] = TRUE;
  $handler->display->display_options['filters']['field_ag_gender_value']['expose']['operator_id'] = 'field_ag_gender_value_op';
  $handler->display->display_options['filters']['field_ag_gender_value']['expose']['label'] = 'Gender ';
  $handler->display->display_options['filters']['field_ag_gender_value']['expose']['operator'] = 'field_ag_gender_value_op';
  $handler->display->display_options['filters']['field_ag_gender_value']['expose']['identifier'] = 'field_ag_gender_value';
  $handler->display->display_options['filters']['field_ag_gender_value']['expose']['remember_roles'] = array(
    2 => '2',
    3 => 0,
    1 => 0,
    6 => 0,
    7 => 0,
    4 => 0,
    5 => 0,
  );
  /* Filter criterion: Content: Working language(s) (field_ag_working_language) */
  $handler->display->display_options['filters']['field_ag_working_language_get_list_from_file']['id'] = 'field_ag_working_language_get_list_from_file';
  $handler->display->display_options['filters']['field_ag_working_language_get_list_from_file']['table'] = 'field_data_field_ag_working_language';
  $handler->display->display_options['filters']['field_ag_working_language_get_list_from_file']['field'] = 'field_ag_working_language_get_list_from_file';
  $handler->display->display_options['filters']['field_ag_working_language_get_list_from_file']['group'] = 1;
  $handler->display->display_options['filters']['field_ag_working_language_get_list_from_file']['exposed'] = TRUE;
  $handler->display->display_options['filters']['field_ag_working_language_get_list_from_file']['expose']['operator_id'] = 'field_ag_working_language_get_list_from_file_op';
  $handler->display->display_options['filters']['field_ag_working_language_get_list_from_file']['expose']['label'] = 'Working language(s) ';
  $handler->display->display_options['filters']['field_ag_working_language_get_list_from_file']['expose']['operator'] = 'field_ag_working_language_get_list_from_file_op';
  $handler->display->display_options['filters']['field_ag_working_language_get_list_from_file']['expose']['identifier'] = 'field_ag_working_language_get_list_from_file';
  $handler->display->display_options['filters']['field_ag_working_language_get_list_from_file']['expose']['remember_roles'] = array(
    2 => '2',
    3 => 0,
    1 => 0,
    6 => 0,
    7 => 0,
    4 => 0,
    5 => 0,
  );
  /* Filter criterion: Content: Institution (field_ag_institution) */
  $handler->display->display_options['filters']['field_ag_institution_nid']['id'] = 'field_ag_institution_nid';
  $handler->display->display_options['filters']['field_ag_institution_nid']['table'] = 'field_data_field_ag_institution';
  $handler->display->display_options['filters']['field_ag_institution_nid']['field'] = 'field_ag_institution_nid';
  $handler->display->display_options['filters']['field_ag_institution_nid']['group'] = 1;
  $handler->display->display_options['filters']['field_ag_institution_nid']['exposed'] = TRUE;
  $handler->display->display_options['filters']['field_ag_institution_nid']['expose']['operator_id'] = 'field_ag_institution_nid_op';
  $handler->display->display_options['filters']['field_ag_institution_nid']['expose']['label'] = 'Institution';
  $handler->display->display_options['filters']['field_ag_institution_nid']['expose']['operator'] = 'field_ag_institution_nid_op';
  $handler->display->display_options['filters']['field_ag_institution_nid']['expose']['identifier'] = 'field_ag_institution_nid';
  $handler->display->display_options['filters']['field_ag_institution_nid']['expose']['remember_roles'] = array(
    2 => '2',
    3 => 0,
    1 => 0,
    6 => 0,
    7 => 0,
    4 => 0,
    5 => 0,
  );
  $handler->display->display_options['path'] = 'ag_experts';

  /* Display: Experts directory */
  $handler = $view->new_display('page', 'Experts directory', 'page_2');
  $handler->display->display_options['defaults']['title'] = FALSE;
  $handler->display->display_options['title'] = 'Experts: directory';
  $handler->display->display_options['defaults']['header'] = FALSE;
  /* Header: Global: Text area */
  $handler->display->display_options['header']['text']['id'] = 'text';
  $handler->display->display_options['header']['text']['table'] = 'views';
  $handler->display->display_options['header']['text']['field'] = 'area';
  $handler->display->display_options['header']['text']['empty'] = TRUE;
  $handler->display->display_options['header']['text']['content'] = 'By default records are diaplayed in alphabetical order by name.
Clicking on a column header will sort records according to that header.';
  $handler->display->display_options['header']['text']['format'] = '3';
  $handler->display->display_options['defaults']['footer'] = FALSE;
  /* Footer: Global: Text area */
  $handler->display->display_options['footer']['text']['id'] = 'text';
  $handler->display->display_options['footer']['text']['table'] = 'views';
  $handler->display->display_options['footer']['text']['field'] = 'area';
  $handler->display->display_options['footer']['text']['empty'] = TRUE;
  $handler->display->display_options['footer']['text']['content'] = '<script language="javascript">
  selects = document.getElementsByTagName("select");
  for (i=0; i<selects.length; i++) {
    if (selects[i].size > 4)
      selects[i].size = 4;
  }
</script>
<?php
global $base_url;
?>
<div align="right"><a href="<?php echo $base_url;?>/ag_experts/rss" target="_blank"><img src="<?php echo $base_url;?>/sites/default/files/images/RSS_feed.png" /> RSS feed</a> - <a href="<?php echo $base_url;?>/ag_experts/rdf" target="_blank"><img src="http://www.w3.org/RDF/icons/rdf_flyer.24" /> RDF feed (FOAF)</a></div>';
  $handler->display->display_options['footer']['text']['format'] = '3';
  $handler->display->display_options['defaults']['fields'] = FALSE;
  /* Field: Content: Title */
  $handler->display->display_options['fields']['title']['id'] = 'title';
  $handler->display->display_options['fields']['title']['table'] = 'node';
  $handler->display->display_options['fields']['title']['field'] = 'title';
  $handler->display->display_options['fields']['title']['label'] = 'Name';
  /* Field: Content: Country */
  $handler->display->display_options['fields']['field_ag_country']['id'] = 'field_ag_country';
  $handler->display->display_options['fields']['field_ag_country']['table'] = 'field_data_field_ag_country';
  $handler->display->display_options['fields']['field_ag_country']['field'] = 'field_ag_country';
  /* Field: Content: Institution */
  $handler->display->display_options['fields']['field_ag_institution']['id'] = 'field_ag_institution';
  $handler->display->display_options['fields']['field_ag_institution']['table'] = 'field_data_field_ag_institution';
  $handler->display->display_options['fields']['field_ag_institution']['field'] = 'field_ag_institution';
  $handler->display->display_options['fields']['field_ag_institution']['label'] = 'Latest affiliation(s)';
  $handler->display->display_options['fields']['field_ag_institution']['delta_limit'] = '2';
  $handler->display->display_options['fields']['field_ag_institution']['delta_offset'] = '0';
  $handler->display->display_options['fields']['field_ag_institution']['multi_type'] = 'ul';
  $handler->display->display_options['defaults']['arguments'] = FALSE;
  $handler->display->display_options['defaults']['filter_groups'] = FALSE;
  $handler->display->display_options['defaults']['filters'] = FALSE;
  /* Filter criterion: Content: Published */
  $handler->display->display_options['filters']['status']['id'] = 'status';
  $handler->display->display_options['filters']['status']['table'] = 'node';
  $handler->display->display_options['filters']['status']['field'] = 'status';
  $handler->display->display_options['filters']['status']['value'] = '1';
  $handler->display->display_options['filters']['status']['group'] = '0';
  $handler->display->display_options['filters']['status']['expose']['operator'] = FALSE;
  /* Filter criterion: Content: Type */
  $handler->display->display_options['filters']['type']['id'] = 'type';
  $handler->display->display_options['filters']['type']['table'] = 'node';
  $handler->display->display_options['filters']['type']['field'] = 'type';
  $handler->display->display_options['filters']['type']['value'] = array(
    'agprofile' => 'agprofile',
  );
  $handler->display->display_options['filters']['type']['group'] = '0';
  $handler->display->display_options['filters']['type']['expose']['operator'] = FALSE;
  /* Filter criterion: Content: Title */
  $handler->display->display_options['filters']['title']['id'] = 'title';
  $handler->display->display_options['filters']['title']['table'] = 'node';
  $handler->display->display_options['filters']['title']['field'] = 'title';
  $handler->display->display_options['filters']['title']['operator'] = 'contains';
  $handler->display->display_options['filters']['title']['group'] = '0';
  $handler->display->display_options['filters']['title']['exposed'] = TRUE;
  $handler->display->display_options['filters']['title']['expose']['operator_id'] = 'title_op';
  $handler->display->display_options['filters']['title']['expose']['label'] = 'Name';
  $handler->display->display_options['filters']['title']['expose']['operator'] = 'title_op';
  $handler->display->display_options['filters']['title']['expose']['identifier'] = 'title';
  $handler->display->display_options['path'] = 'ag_experts-directory';

  /* Display: Institutions: advanced search */
  $handler = $view->new_display('page', 'Institutions: advanced search', 'page_3');
  $handler->display->display_options['defaults']['title'] = FALSE;
  $handler->display->display_options['title'] = 'Institutions: advanced search';
  $handler->display->display_options['defaults']['style_plugin'] = FALSE;
  $handler->display->display_options['style_plugin'] = 'table';
  $handler->display->display_options['style_options']['columns'] = array(
    'title' => 'title',
    'field_ag_acronym' => 'title',
    'field_ag_country' => 'field_ag_country',
    'field_type' => 'field_type',
    'field_ag_website' => 'title',
  );
  $handler->display->display_options['style_options']['default'] = 'title';
  $handler->display->display_options['style_options']['info'] = array(
    'title' => array(
      'sortable' => 1,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => ' ',
      'empty_column' => 0,
    ),
    'field_ag_acronym' => array(
      'sortable' => 0,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'field_ag_country' => array(
      'sortable' => 0,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'field_type' => array(
      'sortable' => 0,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'field_ag_website' => array(
      'sortable' => 0,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
  );
  $handler->display->display_options['defaults']['style_options'] = FALSE;
  $handler->display->display_options['defaults']['row_plugin'] = FALSE;
  $handler->display->display_options['defaults']['row_options'] = FALSE;
  $handler->display->display_options['defaults']['header'] = FALSE;
  /* Header: Global: Text area */
  $handler->display->display_options['header']['text']['id'] = 'text';
  $handler->display->display_options['header']['text']['table'] = 'views';
  $handler->display->display_options['header']['text']['field'] = 'area';
  $handler->display->display_options['header']['text']['content'] = '<?php
global $base_url;
?>
By default records are diaplayed in alphabetical order by name.
Clicking on a column header will sort records according to that header.
<br />See Agris category descriptions <a href="<?php echo $base_url;?>/ag_vocabularies" target="_blank">here</a>.';
  $handler->display->display_options['header']['text']['format'] = '3';
  $handler->display->display_options['defaults']['footer'] = FALSE;
  /* Footer: Global: Text area */
  $handler->display->display_options['footer']['text']['id'] = 'text';
  $handler->display->display_options['footer']['text']['table'] = 'views';
  $handler->display->display_options['footer']['text']['field'] = 'area';
  $handler->display->display_options['footer']['text']['empty'] = TRUE;
  $handler->display->display_options['footer']['text']['content'] = '<script language="javascript">
  selects = document.getElementsByTagName("select");
  for (i=0; i<selects.length; i++) {
    if (selects[i].size > 4)
      selects[i].size = 4;
  }
</script>';
  $handler->display->display_options['footer']['text']['format'] = '3';
  $handler->display->display_options['defaults']['relationships'] = FALSE;
  /* Relationship: Content: Agris Subject Categories (field_ag_agris) */
  $handler->display->display_options['relationships']['field_ag_agris_tid']['id'] = 'field_ag_agris_tid';
  $handler->display->display_options['relationships']['field_ag_agris_tid']['table'] = 'field_data_field_ag_agris';
  $handler->display->display_options['relationships']['field_ag_agris_tid']['field'] = 'field_ag_agris_tid';
  $handler->display->display_options['defaults']['fields'] = FALSE;
  /* Field: Content: Title */
  $handler->display->display_options['fields']['title']['id'] = 'title';
  $handler->display->display_options['fields']['title']['table'] = 'node';
  $handler->display->display_options['fields']['title']['field'] = 'title';
  $handler->display->display_options['fields']['title']['label'] = 'Name';
  /* Field: Content: Acronym */
  $handler->display->display_options['fields']['field_ag_acronym']['id'] = 'field_ag_acronym';
  $handler->display->display_options['fields']['field_ag_acronym']['table'] = 'field_data_field_ag_acronym';
  $handler->display->display_options['fields']['field_ag_acronym']['field'] = 'field_ag_acronym';
  $handler->display->display_options['fields']['field_ag_acronym']['label'] = '';
  $handler->display->display_options['fields']['field_ag_acronym']['alter']['alter_text'] = TRUE;
  $handler->display->display_options['fields']['field_ag_acronym']['alter']['text'] = '([field_ag_acronym-value])';
  $handler->display->display_options['fields']['field_ag_acronym']['element_label_colon'] = FALSE;
  /* Field: Content: Country */
  $handler->display->display_options['fields']['field_ag_country']['id'] = 'field_ag_country';
  $handler->display->display_options['fields']['field_ag_country']['table'] = 'field_data_field_ag_country';
  $handler->display->display_options['fields']['field_ag_country']['field'] = 'field_ag_country';
  $handler->display->display_options['fields']['field_ag_country']['element_label_colon'] = FALSE;
  /* Field: Content: Year of establishment */
  $handler->display->display_options['fields']['field_ag_year']['id'] = 'field_ag_year';
  $handler->display->display_options['fields']['field_ag_year']['table'] = 'field_data_field_ag_year';
  $handler->display->display_options['fields']['field_ag_year']['field'] = 'field_ag_year';
  $handler->display->display_options['fields']['field_ag_year']['label'] = 'Year';
  $handler->display->display_options['fields']['field_ag_year']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['field_ag_year']['settings'] = array(
    'format_type' => 'date',
    'fromto' => 'both',
    'multiple_number' => '',
    'multiple_from' => '',
    'multiple_to' => '',
  );
  /* Field: Content: Institution type */
  $handler->display->display_options['fields']['field_ag_institution_type']['id'] = 'field_ag_institution_type';
  $handler->display->display_options['fields']['field_ag_institution_type']['table'] = 'field_data_field_ag_institution_type';
  $handler->display->display_options['fields']['field_ag_institution_type']['field'] = 'field_ag_institution_type';
  $handler->display->display_options['fields']['field_ag_institution_type']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['field_ag_institution_type']['type'] = 'taxonomy_term_reference_plain';
  /* Field: Content: Agrovoc terms */
  $handler->display->display_options['fields']['field_ag_agrovoc']['id'] = 'field_ag_agrovoc';
  $handler->display->display_options['fields']['field_ag_agrovoc']['table'] = 'field_data_field_ag_agrovoc';
  $handler->display->display_options['fields']['field_ag_agrovoc']['field'] = 'field_ag_agrovoc';
  $handler->display->display_options['fields']['field_ag_agrovoc']['label'] = 'Agrovoc';
  $handler->display->display_options['fields']['field_ag_agrovoc']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['field_ag_agrovoc']['type'] = 'taxonomy_term_reference_plain';
  $handler->display->display_options['fields']['field_ag_agrovoc']['delta_offset'] = '0';
  $handler->display->display_options['defaults']['filter_groups'] = FALSE;
  $handler->display->display_options['defaults']['filters'] = FALSE;
  /* Filter criterion: Content: Published */
  $handler->display->display_options['filters']['status']['id'] = 'status';
  $handler->display->display_options['filters']['status']['table'] = 'node';
  $handler->display->display_options['filters']['status']['field'] = 'status';
  $handler->display->display_options['filters']['status']['value'] = '1';
  $handler->display->display_options['filters']['status']['group'] = 1;
  $handler->display->display_options['filters']['status']['expose']['operator'] = FALSE;
  /* Filter criterion: Content: Type */
  $handler->display->display_options['filters']['type']['id'] = 'type';
  $handler->display->display_options['filters']['type']['table'] = 'node';
  $handler->display->display_options['filters']['type']['field'] = 'type';
  $handler->display->display_options['filters']['type']['value'] = array(
    'aginstitution' => 'aginstitution',
  );
  $handler->display->display_options['filters']['type']['group'] = 1;
  $handler->display->display_options['filters']['type']['expose']['operator'] = FALSE;
  /* Filter criterion: Content: Title */
  $handler->display->display_options['filters']['title']['id'] = 'title';
  $handler->display->display_options['filters']['title']['table'] = 'node';
  $handler->display->display_options['filters']['title']['field'] = 'title';
  $handler->display->display_options['filters']['title']['operator'] = 'contains';
  $handler->display->display_options['filters']['title']['group'] = 1;
  $handler->display->display_options['filters']['title']['exposed'] = TRUE;
  $handler->display->display_options['filters']['title']['expose']['operator_id'] = 'title_op';
  $handler->display->display_options['filters']['title']['expose']['label'] = 'Word(s) in institution\'s name';
  $handler->display->display_options['filters']['title']['expose']['operator'] = 'title_op';
  $handler->display->display_options['filters']['title']['expose']['identifier'] = 'title';
  /* Filter criterion: Content: Country (field_ag_country) */
  $handler->display->display_options['filters']['field_ag_country_get_list_from_file']['id'] = 'field_ag_country_get_list_from_file';
  $handler->display->display_options['filters']['field_ag_country_get_list_from_file']['table'] = 'field_data_field_ag_country';
  $handler->display->display_options['filters']['field_ag_country_get_list_from_file']['field'] = 'field_ag_country_get_list_from_file';
  $handler->display->display_options['filters']['field_ag_country_get_list_from_file']['group'] = 1;
  $handler->display->display_options['filters']['field_ag_country_get_list_from_file']['exposed'] = TRUE;
  $handler->display->display_options['filters']['field_ag_country_get_list_from_file']['expose']['operator_id'] = 'field_ag_country_get_list_from_file_op';
  $handler->display->display_options['filters']['field_ag_country_get_list_from_file']['expose']['label'] = 'Country';
  $handler->display->display_options['filters']['field_ag_country_get_list_from_file']['expose']['operator'] = 'field_ag_country_get_list_from_file_op';
  $handler->display->display_options['filters']['field_ag_country_get_list_from_file']['expose']['identifier'] = 'field_ag_country_get_list_from_file';
  $handler->display->display_options['filters']['field_ag_country_get_list_from_file']['expose']['remember_roles'] = array(
    2 => '2',
    3 => 0,
    1 => 0,
    6 => 0,
    7 => 0,
    4 => 0,
    5 => 0,
  );
  /* Filter criterion: Content: Agrovoc terms (field_ag_agrovoc) */
  $handler->display->display_options['filters']['field_ag_agrovoc_tid']['id'] = 'field_ag_agrovoc_tid';
  $handler->display->display_options['filters']['field_ag_agrovoc_tid']['table'] = 'field_data_field_ag_agrovoc';
  $handler->display->display_options['filters']['field_ag_agrovoc_tid']['field'] = 'field_ag_agrovoc_tid';
  $handler->display->display_options['filters']['field_ag_agrovoc_tid']['value'] = '';
  $handler->display->display_options['filters']['field_ag_agrovoc_tid']['group'] = 1;
  $handler->display->display_options['filters']['field_ag_agrovoc_tid']['exposed'] = TRUE;
  $handler->display->display_options['filters']['field_ag_agrovoc_tid']['expose']['operator_id'] = 'field_ag_agrovoc_tid_op';
  $handler->display->display_options['filters']['field_ag_agrovoc_tid']['expose']['label'] = 'Field of activity (Agrovoc terms) ';
  $handler->display->display_options['filters']['field_ag_agrovoc_tid']['expose']['operator'] = 'field_ag_agrovoc_tid_op';
  $handler->display->display_options['filters']['field_ag_agrovoc_tid']['expose']['identifier'] = 'field_ag_agrovoc_tid';
  $handler->display->display_options['filters']['field_ag_agrovoc_tid']['expose']['remember_roles'] = array(
    2 => '2',
    3 => 0,
    1 => 0,
    6 => 0,
    7 => 0,
    4 => 0,
    5 => 0,
  );
  $handler->display->display_options['filters']['field_ag_agrovoc_tid']['reduce_duplicates'] = TRUE;
  $handler->display->display_options['filters']['field_ag_agrovoc_tid']['vocabulary'] = 'vocabulary_16';
  /* Filter criterion: Content: Agris Subject Categories (field_ag_agris) */
  $handler->display->display_options['filters']['field_ag_agris_tid']['id'] = 'field_ag_agris_tid';
  $handler->display->display_options['filters']['field_ag_agris_tid']['table'] = 'field_data_field_ag_agris';
  $handler->display->display_options['filters']['field_ag_agris_tid']['field'] = 'field_ag_agris_tid';
  $handler->display->display_options['filters']['field_ag_agris_tid']['exposed'] = TRUE;
  $handler->display->display_options['filters']['field_ag_agris_tid']['expose']['operator_id'] = 'field_ag_agris_tid_op';
  $handler->display->display_options['filters']['field_ag_agris_tid']['expose']['label'] = 'Field of activity (ASC categories)';
  $handler->display->display_options['filters']['field_ag_agris_tid']['expose']['operator'] = 'field_ag_agris_tid_op';
  $handler->display->display_options['filters']['field_ag_agris_tid']['expose']['identifier'] = 'field_ag_agris_tid';
  $handler->display->display_options['filters']['field_ag_agris_tid']['expose']['remember_roles'] = array(
    2 => '2',
    3 => 0,
    1 => 0,
    6 => 0,
    7 => 0,
    4 => 0,
    5 => 0,
  );
  $handler->display->display_options['filters']['field_ag_agris_tid']['type'] = 'select';
  $handler->display->display_options['filters']['field_ag_agris_tid']['vocabulary'] = 'vocabulary_10';
  $handler->display->display_options['path'] = 'ag_institutions';

  /* Display: Institutions directory */
  $handler = $view->new_display('page', 'Institutions directory', 'page_4');
  $handler->display->display_options['defaults']['title'] = FALSE;
  $handler->display->display_options['title'] = 'Institutions: directory';
  $handler->display->display_options['defaults']['header'] = FALSE;
  /* Header: Global: Text area */
  $handler->display->display_options['header']['text']['id'] = 'text';
  $handler->display->display_options['header']['text']['table'] = 'views';
  $handler->display->display_options['header']['text']['field'] = 'area';
  $handler->display->display_options['header']['text']['content'] = 'By default records are diaplayed in alphabetical order by name.
Clicking on a column header will sort records according to that header.
';
  $handler->display->display_options['header']['text']['format'] = '3';
  $handler->display->display_options['defaults']['footer'] = FALSE;
  /* Footer: Global: Text area */
  $handler->display->display_options['footer']['text']['id'] = 'text';
  $handler->display->display_options['footer']['text']['table'] = 'views';
  $handler->display->display_options['footer']['text']['field'] = 'area';
  $handler->display->display_options['footer']['text']['empty'] = TRUE;
  $handler->display->display_options['footer']['text']['content'] = '<script language="javascript">
  selects = document.getElementsByTagName("select");
  for (i=0; i<selects.length; i++) {
    if (selects[i].size > 4)
      selects[i].size = 4;
  }
</script>
<?php
global $base_url;
?>
<div align="right"><a href="<?php echo $base_url;?>/ag_institutions/rss" target="_blank"><img src="<?php echo $base_url;?>/sites/default/files/images/RSS_feed.png" /> RSS feed</a> [<i>compliant with the Ag-Org AP</i>] - <a href="<?php echo $base_url;?>/ag_institutions/rdf" target="_blank"><img src="http://www.w3.org/RDF/icons/rdf_flyer.24" /> RDF feed</a></div>';
  $handler->display->display_options['footer']['text']['format'] = '3';
  $handler->display->display_options['defaults']['fields'] = FALSE;
  /* Field: Content: Title */
  $handler->display->display_options['fields']['title']['id'] = 'title';
  $handler->display->display_options['fields']['title']['table'] = 'node';
  $handler->display->display_options['fields']['title']['field'] = 'title';
  $handler->display->display_options['fields']['title']['label'] = 'Name';
  $handler->display->display_options['defaults']['arguments'] = FALSE;
  $handler->display->display_options['defaults']['filter_groups'] = FALSE;
  $handler->display->display_options['defaults']['filters'] = FALSE;
  /* Filter criterion: Content: Published */
  $handler->display->display_options['filters']['status']['id'] = 'status';
  $handler->display->display_options['filters']['status']['table'] = 'node';
  $handler->display->display_options['filters']['status']['field'] = 'status';
  $handler->display->display_options['filters']['status']['value'] = '1';
  $handler->display->display_options['filters']['status']['group'] = '0';
  $handler->display->display_options['filters']['status']['expose']['operator'] = FALSE;
  /* Filter criterion: Content: Type */
  $handler->display->display_options['filters']['type']['id'] = 'type';
  $handler->display->display_options['filters']['type']['table'] = 'node';
  $handler->display->display_options['filters']['type']['field'] = 'type';
  $handler->display->display_options['filters']['type']['value'] = array(
    'aginstitution' => 'aginstitution',
  );
  $handler->display->display_options['filters']['type']['group'] = '0';
  $handler->display->display_options['filters']['type']['expose']['operator'] = FALSE;
  /* Filter criterion: Content: Title */
  $handler->display->display_options['filters']['title']['id'] = 'title';
  $handler->display->display_options['filters']['title']['table'] = 'node';
  $handler->display->display_options['filters']['title']['field'] = 'title';
  $handler->display->display_options['filters']['title']['operator'] = 'contains';
  $handler->display->display_options['filters']['title']['group'] = '0';
  $handler->display->display_options['filters']['title']['exposed'] = TRUE;
  $handler->display->display_options['filters']['title']['expose']['operator_id'] = 'title_op';
  $handler->display->display_options['filters']['title']['expose']['label'] = 'Word(s) in institution\'s name ';
  $handler->display->display_options['filters']['title']['expose']['operator'] = 'title_op';
  $handler->display->display_options['filters']['title']['expose']['identifier'] = 'title';
  $handler->display->display_options['path'] = 'ag_institutions-directory';

  /* Display: Institutions Feed */
  $handler = $view->new_display('feed', 'Institutions Feed', 'feed_3');
  $handler->display->display_options['defaults']['title'] = FALSE;
  $handler->display->display_options['title'] = 'Institutions';
  $handler->display->display_options['defaults']['link_display'] = FALSE;
  $handler->display->display_options['link_display'] = 'page_3';
  $handler->display->display_options['pager']['type'] = 'full';
  $handler->display->display_options['style_plugin'] = 'rss_fields';
  $handler->display->display_options['style_options']['channel'] = array(
    'core' => array(
      'views_rss_core' => array(
        'description' => '',
        'language' => '',
        'category' => '',
        'image' => '',
        'copyright' => '',
        'managingEditor' => '',
        'webMaster' => '',
        'generator' => '',
        'docs' => '',
        'cloud' => '',
        'ttl' => '',
        'skipHours' => '',
        'skipDays' => '',
      ),
    ),
  );
  $handler->display->display_options['style_options']['item'] = array(
    'core' => array(
      'views_rss_core' => array(
        'title' => '',
        'link' => '',
        'description' => '',
        'author' => '',
        'category' => '',
        'comments' => '',
        'enclosure' => '',
        'guid' => '',
        'pubDate' => '',
      ),
    ),
    'dc' => array(
      'views_rss_dc' => array(
        'title' => '',
        'creator' => '',
        'subject' => '',
        'description' => '',
        'publisher' => '',
        'contributor' => '',
        'date' => '',
        'type' => '',
        'format' => '',
        'identifier' => '',
        'source' => '',
        'language' => '',
        'relation' => '',
        'coverage' => '',
        'rights' => '',
      ),
    ),
  );
  $handler->display->display_options['style_options']['feed_settings'] = array(
    'absolute_paths' => 1,
    'feed_in_links' => 0,
  );
  $handler->display->display_options['defaults']['header'] = FALSE;
  /* Header: Global: Text area */
  $handler->display->display_options['header']['text']['id'] = 'area';
  $handler->display->display_options['header']['text']['table'] = 'views';
  $handler->display->display_options['header']['text']['field'] = 'area';
  $handler->display->display_options['header']['text']['content'] = '<?php
global $base_url;
?>
<div align="right"><a href="<?php echo $base_url;?>/ag_institutions/rss" target="_blank"><img src="<?php echo $base_url;?>/sites/default/files/images/RSS_feed.png" /> RSS feed</a><br />[<i>compliant with the Ag-Org AP</i>]<br  /><a href="<?php echo $base_url;?>/ag_institutions/rdf" target="_blank"><img src="http://www.w3.org/RDF/icons/rdf_flyer.24" /> RDF feed</a><br />&nbsp;</div>
By default records are diaplayed in alphabetical order by name.
Clicking on a column header will sort records according to that header.
';
  $handler->display->display_options['header']['text']['format'] = '3';
  $handler->display->display_options['defaults']['footer'] = FALSE;
  /* Footer: Global: Text area */
  $handler->display->display_options['footer']['text']['id'] = 'area';
  $handler->display->display_options['footer']['text']['table'] = 'views';
  $handler->display->display_options['footer']['text']['field'] = 'area';
  $handler->display->display_options['footer']['text']['empty'] = TRUE;
  $handler->display->display_options['footer']['text']['content'] = '<script language="javascript">
  selects = document.getElementsByTagName("select");
  for (i=0; i<selects.length; i++) {
    if (selects[i].size > 4)
      selects[i].size = 4;
  }
</script>';
  $handler->display->display_options['footer']['text']['format'] = '3';
  $handler->display->display_options['defaults']['arguments'] = FALSE;
  $handler->display->display_options['defaults']['filter_groups'] = FALSE;
  $handler->display->display_options['defaults']['filters'] = FALSE;
  /* Filter criterion: Content: Published */
  $handler->display->display_options['filters']['status']['id'] = 'status';
  $handler->display->display_options['filters']['status']['table'] = 'node';
  $handler->display->display_options['filters']['status']['field'] = 'status';
  $handler->display->display_options['filters']['status']['value'] = '1';
  $handler->display->display_options['filters']['status']['group'] = '0';
  $handler->display->display_options['filters']['status']['expose']['operator'] = FALSE;
  /* Filter criterion: Content: Type */
  $handler->display->display_options['filters']['type']['id'] = 'type';
  $handler->display->display_options['filters']['type']['table'] = 'node';
  $handler->display->display_options['filters']['type']['field'] = 'type';
  $handler->display->display_options['filters']['type']['value'] = array(
    'aginstitution' => 'aginstitution',
  );
  $handler->display->display_options['filters']['type']['group'] = '0';
  $handler->display->display_options['filters']['type']['expose']['operator'] = FALSE;
  /* Filter criterion: Content: Title */
  $handler->display->display_options['filters']['title']['id'] = 'title';
  $handler->display->display_options['filters']['title']['table'] = 'node';
  $handler->display->display_options['filters']['title']['field'] = 'title';
  $handler->display->display_options['filters']['title']['operator'] = 'contains';
  $handler->display->display_options['filters']['title']['group'] = '0';
  $handler->display->display_options['filters']['title']['exposed'] = TRUE;
  $handler->display->display_options['filters']['title']['expose']['operator_id'] = 'title_op';
  $handler->display->display_options['filters']['title']['expose']['label'] = 'Name';
  $handler->display->display_options['filters']['title']['expose']['operator'] = 'title_op';
  $handler->display->display_options['filters']['title']['expose']['identifier'] = 'title';
  $handler->display->display_options['path'] = 'ag_institutions/rdf';
  $handler->display->display_options['sitename_title'] = 0;

  /* Display: Institutions RSS Feed */
  $handler = $view->new_display('feed', 'Institutions RSS Feed', 'feed_4');
  $handler->display->display_options['defaults']['title'] = FALSE;
  $handler->display->display_options['title'] = 'Institutions';
  $handler->display->display_options['defaults']['link_display'] = FALSE;
  $handler->display->display_options['link_display'] = 'page_3';
  $handler->display->display_options['pager']['type'] = 'full';
  $handler->display->display_options['style_plugin'] = 'rss';
  $handler->display->display_options['row_plugin'] = 'node_rss';
  $handler->display->display_options['defaults']['header'] = FALSE;
  /* Header: Global: Text area */
  $handler->display->display_options['header']['text']['id'] = 'area';
  $handler->display->display_options['header']['text']['table'] = 'views';
  $handler->display->display_options['header']['text']['field'] = 'area';
  $handler->display->display_options['header']['text']['content'] = '<?php
global $base_url;
?>
<div align="right"><a href="<?php echo $base_url;?>/ag_institutions/rss" target="_blank"><img src="<?php echo $base_url;?>/sites/default/files/images/RSS_feed.png" /> RSS feed</a><br />[<i>compliant with the Ag-Org AP</i>]<br  /><a href="<?php echo $base_url;?>/ag_institutions/rdf" target="_blank"><img src="http://www.w3.org/RDF/icons/rdf_flyer.24" /> RDF feed</a><br />&nbsp;</div>
By default records are diaplayed in alphabetical order by name.
Clicking on a column header will sort records according to that header.
';
  $handler->display->display_options['header']['text']['format'] = '3';
  $handler->display->display_options['defaults']['footer'] = FALSE;
  /* Footer: Global: Text area */
  $handler->display->display_options['footer']['text']['id'] = 'area';
  $handler->display->display_options['footer']['text']['table'] = 'views';
  $handler->display->display_options['footer']['text']['field'] = 'area';
  $handler->display->display_options['footer']['text']['empty'] = TRUE;
  $handler->display->display_options['footer']['text']['content'] = '<script language="javascript">
  selects = document.getElementsByTagName("select");
  for (i=0; i<selects.length; i++) {
    if (selects[i].size > 4)
      selects[i].size = 4;
  }
</script>';
  $handler->display->display_options['footer']['text']['format'] = '3';
  $handler->display->display_options['defaults']['relationships'] = FALSE;
  $handler->display->display_options['defaults']['arguments'] = FALSE;
  $handler->display->display_options['defaults']['filter_groups'] = FALSE;
  $handler->display->display_options['defaults']['filters'] = FALSE;
  /* Filter criterion: Content: Published */
  $handler->display->display_options['filters']['status']['id'] = 'status';
  $handler->display->display_options['filters']['status']['table'] = 'node';
  $handler->display->display_options['filters']['status']['field'] = 'status';
  $handler->display->display_options['filters']['status']['value'] = '1';
  $handler->display->display_options['filters']['status']['group'] = '0';
  $handler->display->display_options['filters']['status']['expose']['operator'] = FALSE;
  /* Filter criterion: Content: Type */
  $handler->display->display_options['filters']['type']['id'] = 'type';
  $handler->display->display_options['filters']['type']['table'] = 'node';
  $handler->display->display_options['filters']['type']['field'] = 'type';
  $handler->display->display_options['filters']['type']['value'] = array(
    'aginstitution' => 'aginstitution',
  );
  $handler->display->display_options['filters']['type']['group'] = '0';
  $handler->display->display_options['filters']['type']['expose']['operator'] = FALSE;
  $handler->display->display_options['path'] = 'ag_institutions/rss';
  $handler->display->display_options['sitename_title'] = 0;
  $export['Ag_experts_institution'] = $view;

  $view = new view();
  $view->name = 'Ag_partners_vacancies';
  $view->description = 'Vacancies from partners';
  $view->tag = 'custom';
  $view->base_table = 'node';
  $view->human_name = '';
  $view->core = 0;
  $view->api_version = '3.0';
  $view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */

  /* Display: Vacancies from partners */
  $handler = $view->new_display('default', 'Vacancies from partners', 'default');
  $handler->display->display_options['title'] = 'Vacancies from partners';
  $handler->display->display_options['use_more_always'] = FALSE;
  $handler->display->display_options['access']['type'] = 'none';
  $handler->display->display_options['cache']['type'] = 'none';
  $handler->display->display_options['query']['type'] = 'views_query';
  $handler->display->display_options['exposed_form']['type'] = 'basic';
  $handler->display->display_options['pager']['type'] = 'full';
  $handler->display->display_options['style_plugin'] = 'table';
  $handler->display->display_options['style_options']['grouping'] = '';
  $handler->display->display_options['style_options']['columns'] = array(
    'title' => 'title',
    'field_deadline_value' => 'field_deadline_value',
    'body' => 'title',
    'path' => 'path',
    'title_2' => 'title',
  );
  $handler->display->display_options['style_options']['default'] = 'field_deadline_value';
  $handler->display->display_options['style_options']['info'] = array(
    'title' => array(
      'sortable' => 0,
      'separator' => '<br />',
    ),
    'field_deadline_value' => array(
      'sortable' => 1,
      'separator' => '',
    ),
    'body' => array(
      'separator' => '',
    ),
    'path' => array(
      'separator' => '',
    ),
    'title_2' => array(
      'sortable' => 0,
      'separator' => '',
    ),
  );
  $handler->display->display_options['style_options']['order'] = 'desc';
  /* Relationship: Broken/missing handler */
  $handler->display->display_options['relationships']['feed_nid']['id'] = 'feed_nid';
  $handler->display->display_options['relationships']['feed_nid']['table'] = 'feeds_node_item';
  $handler->display->display_options['relationships']['feed_nid']['field'] = 'feed_nid';
  $handler->display->display_options['relationships']['feed_nid']['label'] = 'Owner feed';
  /* Field: Content: Title */
  $handler->display->display_options['fields']['title']['id'] = 'title';
  $handler->display->display_options['fields']['title']['table'] = 'node';
  $handler->display->display_options['fields']['title']['field'] = 'title';
  $handler->display->display_options['fields']['title']['label'] = 'Name';
  /* Field: Broken/missing handler */
  $handler->display->display_options['fields']['body']['id'] = 'body';
  $handler->display->display_options['fields']['body']['table'] = 'field_revision_data';
  $handler->display->display_options['fields']['body']['field'] = 'body-revision_id';
  $handler->display->display_options['fields']['body']['label'] = '';
  $handler->display->display_options['fields']['body']['alter']['max_length'] = '200';
  $handler->display->display_options['fields']['body']['alter']['strip_tags'] = TRUE;
  $handler->display->display_options['fields']['body']['alter']['trim'] = TRUE;
  $handler->display->display_options['fields']['body']['alter']['html'] = TRUE;
  /* Field: Broken/missing handler */
  $handler->display->display_options['fields']['field_ag_deadline_value']['id'] = 'field_ag_deadline_value';
  $handler->display->display_options['fields']['field_ag_deadline_value']['table'] = 'node_data_field_ag_deadline';
  $handler->display->display_options['fields']['field_ag_deadline_value']['field'] = 'field_ag_deadline_value';
  /* Field: Content: Path */
  $handler->display->display_options['fields']['path']['id'] = 'path';
  $handler->display->display_options['fields']['path']['table'] = 'node';
  $handler->display->display_options['fields']['path']['field'] = 'path';
  $handler->display->display_options['fields']['path']['relationship'] = 'feed_nid';
  $handler->display->display_options['fields']['path']['label'] = '';
  $handler->display->display_options['fields']['path']['exclude'] = TRUE;
  /* Field: Content: Title */
  $handler->display->display_options['fields']['title_2']['id'] = 'title_2';
  $handler->display->display_options['fields']['title_2']['table'] = 'node';
  $handler->display->display_options['fields']['title_2']['field'] = 'title';
  $handler->display->display_options['fields']['title_2']['relationship'] = 'feed_nid';
  $handler->display->display_options['fields']['title_2']['label'] = '';
  $handler->display->display_options['fields']['title_2']['alter']['alter_text'] = TRUE;
  $handler->display->display_options['fields']['title_2']['alter']['text'] = '<i>source: <a href="[path]"><span>[title_2]</span></a></i>';
  $handler->display->display_options['fields']['title_2']['link_to_node'] = FALSE;
  /* Sort criterion: Broken/missing handler */
  $handler->display->display_options['sorts']['field_ag_deadline_value']['id'] = 'field_ag_deadline_value';
  $handler->display->display_options['sorts']['field_ag_deadline_value']['table'] = 'node_data_field_ag_deadline';
  $handler->display->display_options['sorts']['field_ag_deadline_value']['field'] = 'field_ag_deadline_value';
  /* Filter criterion: Content: Published */
  $handler->display->display_options['filters']['status']['id'] = 'status';
  $handler->display->display_options['filters']['status']['table'] = 'node';
  $handler->display->display_options['filters']['status']['field'] = 'status';
  $handler->display->display_options['filters']['status']['value'] = '1';
  $handler->display->display_options['filters']['status']['group'] = '0';
  $handler->display->display_options['filters']['status']['expose']['operator'] = FALSE;
  /* Filter criterion: Content: Type */
  $handler->display->display_options['filters']['type']['id'] = 'type';
  $handler->display->display_options['filters']['type']['table'] = 'node';
  $handler->display->display_options['filters']['type']['field'] = 'type';
  $handler->display->display_options['filters']['type']['value'] = array(
    'agvacancy' => 'agvacancy',
  );
  $handler->display->display_options['filters']['type']['group'] = '0';
  $handler->display->display_options['filters']['type']['expose']['operator'] = FALSE;
  /* Filter criterion: Date: Date (node) */
  $handler->display->display_options['filters']['date_filter']['id'] = 'date_filter';
  $handler->display->display_options['filters']['date_filter']['table'] = 'node';
  $handler->display->display_options['filters']['date_filter']['field'] = 'date_filter';
  $handler->display->display_options['filters']['date_filter']['operator'] = '>=';
  $handler->display->display_options['filters']['date_filter']['value']['min'] = array(
    'date' => '',
  );
  $handler->display->display_options['filters']['date_filter']['value']['max'] = array(
    'date' => '',
  );
  $handler->display->display_options['filters']['date_filter']['value']['value'] = array(
    'date' => '',
  );
  $handler->display->display_options['filters']['date_filter']['group'] = '0';
  $handler->display->display_options['filters']['date_filter']['expose']['operator'] = FALSE;
  $handler->display->display_options['filters']['date_filter']['form_type'] = 'date_text';
  $handler->display->display_options['filters']['date_filter']['default_date'] = 'now';
  $handler->display->display_options['filters']['date_filter']['default_to_date'] = 'now';
  $handler->display->display_options['filters']['date_filter']['date_fields'] = array(
    'node_data_field_date.field_date_value' => 'node_data_field_date.field_date_value',
  );

  /* Display: Current vacancies from partners */
  $handler = $view->new_display('page', 'Current vacancies from partners', 'page_1');
  $handler->display->display_options['defaults']['title'] = FALSE;
  $handler->display->display_options['title'] = 'Current vacancies from partners';
  $handler->display->display_options['defaults']['style_plugin'] = FALSE;
  $handler->display->display_options['style_plugin'] = 'table';
  $handler->display->display_options['style_options']['grouping'] = '';
  $handler->display->display_options['style_options']['columns'] = array(
    'title' => 'title',
    'field_deadline_value' => 'field_deadline_value',
    'body' => 'title',
    'path' => 'path',
    'title_2' => 'title',
  );
  $handler->display->display_options['style_options']['default'] = 'field_deadline_value';
  $handler->display->display_options['style_options']['info'] = array(
    'title' => array(
      'sortable' => 0,
      'separator' => '<br />',
    ),
    'field_deadline_value' => array(
      'sortable' => 1,
      'separator' => '',
    ),
    'body' => array(
      'separator' => '',
    ),
    'path' => array(
      'separator' => '',
    ),
    'title_2' => array(
      'sortable' => 0,
      'separator' => '',
    ),
  );
  $handler->display->display_options['defaults']['style_options'] = FALSE;
  $handler->display->display_options['defaults']['row_plugin'] = FALSE;
  $handler->display->display_options['defaults']['row_options'] = FALSE;
  $handler->display->display_options['path'] = 'ag_current-partner-vacancies';

  /* Display: Current vacancies from partners block */
  $handler = $view->new_display('block', 'Current vacancies from partners block', 'block_1');
  $handler->display->display_options['defaults']['title'] = FALSE;
  $handler->display->display_options['title'] = 'Current vacancies from partners';
  $handler->display->display_options['defaults']['use_more'] = FALSE;
  $handler->display->display_options['use_more'] = TRUE;
  $handler->display->display_options['defaults']['use_more_always'] = FALSE;
  $handler->display->display_options['defaults']['use_more_always'] = FALSE;
  $handler->display->display_options['use_more_always'] = FALSE;
  $handler->display->display_options['defaults']['use_more_text'] = FALSE;
  $handler->display->display_options['defaults']['style_plugin'] = FALSE;
  $handler->display->display_options['style_plugin'] = 'table';
  $handler->display->display_options['style_options']['grouping'] = '';
  $handler->display->display_options['style_options']['columns'] = array(
    'title' => 'title',
    'field_deadline_value' => 'field_deadline_value',
    'body' => 'title',
    'path' => 'path',
    'title_2' => 'title',
  );
  $handler->display->display_options['style_options']['default'] = 'field_deadline_value';
  $handler->display->display_options['style_options']['info'] = array(
    'title' => array(
      'sortable' => 0,
      'separator' => '<br />',
    ),
    'field_deadline_value' => array(
      'sortable' => 1,
      'separator' => '',
    ),
    'body' => array(
      'separator' => '',
    ),
    'path' => array(
      'separator' => '',
    ),
    'title_2' => array(
      'sortable' => 0,
      'separator' => '',
    ),
  );
  $handler->display->display_options['defaults']['style_options'] = FALSE;
  $handler->display->display_options['defaults']['row_plugin'] = FALSE;
  $handler->display->display_options['defaults']['row_options'] = FALSE;
  $handler->display->display_options['defaults']['filter_groups'] = FALSE;
  $handler->display->display_options['defaults']['filters'] = FALSE;
  /* Filter criterion: Content: Published */
  $handler->display->display_options['filters']['status']['id'] = 'status';
  $handler->display->display_options['filters']['status']['table'] = 'node';
  $handler->display->display_options['filters']['status']['field'] = 'status';
  $handler->display->display_options['filters']['status']['value'] = '1';
  $handler->display->display_options['filters']['status']['group'] = '0';
  $handler->display->display_options['filters']['status']['expose']['operator'] = FALSE;
  /* Filter criterion: Content: Type */
  $handler->display->display_options['filters']['type']['id'] = 'type';
  $handler->display->display_options['filters']['type']['table'] = 'node';
  $handler->display->display_options['filters']['type']['field'] = 'type';
  $handler->display->display_options['filters']['type']['value'] = array(
    'agvacancy' => 'agvacancy',
  );
  $handler->display->display_options['filters']['type']['group'] = '0';
  $handler->display->display_options['filters']['type']['expose']['operator'] = FALSE;
  /* Filter criterion: Date: Date (node) */
  $handler->display->display_options['filters']['date_filter']['id'] = 'date_filter';
  $handler->display->display_options['filters']['date_filter']['table'] = 'node';
  $handler->display->display_options['filters']['date_filter']['field'] = 'date_filter';
  $handler->display->display_options['filters']['date_filter']['operator'] = '>=';
  $handler->display->display_options['filters']['date_filter']['value']['min'] = array(
    'date' => '',
  );
  $handler->display->display_options['filters']['date_filter']['value']['max'] = array(
    'date' => '',
  );
  $handler->display->display_options['filters']['date_filter']['value']['value'] = array(
    'date' => '',
  );
  $handler->display->display_options['filters']['date_filter']['group'] = '0';
  $handler->display->display_options['filters']['date_filter']['expose']['operator'] = FALSE;
  $handler->display->display_options['filters']['date_filter']['form_type'] = 'date_text';
  $handler->display->display_options['filters']['date_filter']['default_date'] = 'now';
  $handler->display->display_options['filters']['date_filter']['date_fields'] = array(
    'node_data_field_date.field_date_value' => 'node_data_field_date.field_date_value',
  );

  /* Display: All vacancies from partners page */
  $handler = $view->new_display('page', 'All vacancies from partners page', 'page_2');
  $handler->display->display_options['defaults']['title'] = FALSE;
  $handler->display->display_options['title'] = 'All vacancies from partners';
  $handler->display->display_options['defaults']['filter_groups'] = FALSE;
  $handler->display->display_options['defaults']['filters'] = FALSE;
  /* Filter criterion: Content: Published */
  $handler->display->display_options['filters']['status']['id'] = 'status';
  $handler->display->display_options['filters']['status']['table'] = 'node';
  $handler->display->display_options['filters']['status']['field'] = 'status';
  $handler->display->display_options['filters']['status']['value'] = '1';
  $handler->display->display_options['filters']['status']['group'] = '0';
  $handler->display->display_options['filters']['status']['expose']['operator'] = FALSE;
  /* Filter criterion: Content: Type */
  $handler->display->display_options['filters']['type']['id'] = 'type';
  $handler->display->display_options['filters']['type']['table'] = 'node';
  $handler->display->display_options['filters']['type']['field'] = 'type';
  $handler->display->display_options['filters']['type']['value'] = array(
    'agvacancy' => 'agvacancy',
  );
  $handler->display->display_options['filters']['type']['group'] = '0';
  $handler->display->display_options['filters']['type']['expose']['operator'] = FALSE;
  $handler->display->display_options['path'] = 'ag_all-partner-vacancies';

  /* Display: All vacancies from partners block */
  $handler = $view->new_display('block', 'All vacancies from partners block', 'block_2');
  $handler->display->display_options['defaults']['title'] = FALSE;
  $handler->display->display_options['title'] = 'All vacancies from partners';
  $handler->display->display_options['defaults']['use_more'] = FALSE;
  $handler->display->display_options['use_more'] = TRUE;
  $handler->display->display_options['defaults']['use_more_always'] = FALSE;
  $handler->display->display_options['defaults']['use_more_always'] = FALSE;
  $handler->display->display_options['use_more_always'] = FALSE;
  $handler->display->display_options['defaults']['use_more_text'] = FALSE;
  $handler->display->display_options['defaults']['link_display'] = FALSE;
  $handler->display->display_options['link_display'] = 'page_2';
  $handler->display->display_options['defaults']['filter_groups'] = FALSE;
  $handler->display->display_options['defaults']['filters'] = FALSE;
  /* Filter criterion: Content: Published */
  $handler->display->display_options['filters']['status']['id'] = 'status';
  $handler->display->display_options['filters']['status']['table'] = 'node';
  $handler->display->display_options['filters']['status']['field'] = 'status';
  $handler->display->display_options['filters']['status']['value'] = '1';
  $handler->display->display_options['filters']['status']['group'] = '0';
  $handler->display->display_options['filters']['status']['expose']['operator'] = FALSE;
  /* Filter criterion: Content: Type */
  $handler->display->display_options['filters']['type']['id'] = 'type';
  $handler->display->display_options['filters']['type']['table'] = 'node';
  $handler->display->display_options['filters']['type']['field'] = 'type';
  $handler->display->display_options['filters']['type']['value'] = array(
    'agvacancy' => 'agvacancy',
  );
  $handler->display->display_options['filters']['type']['group'] = '0';
  $handler->display->display_options['filters']['type']['expose']['operator'] = FALSE;
  $export['Ag_partners_vacancies'] = $view;

  $view = new view();
  $view->name = 'Ag_vacancies';
  $view->description = 'Vacancies';
  $view->tag = 'custom';
  $view->base_table = 'node';
  $view->human_name = '';
  $view->core = 0;
  $view->api_version = '3.0';
  $view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */

  /* Display: Vacancies */
  $handler = $view->new_display('default', 'Vacancies', 'default');
  $handler->display->display_options['title'] = 'Vacancies';
  $handler->display->display_options['use_more_always'] = FALSE;
  $handler->display->display_options['access']['type'] = 'none';
  $handler->display->display_options['cache']['type'] = 'none';
  $handler->display->display_options['query']['type'] = 'views_query';
  $handler->display->display_options['exposed_form']['type'] = 'basic';
  $handler->display->display_options['pager']['type'] = 'full';
  $handler->display->display_options['style_plugin'] = 'table';
  $handler->display->display_options['style_options']['grouping'] = '';
  $handler->display->display_options['style_options']['columns'] = array(
    'title' => 'title',
    'field_country_value' => 'field_country_value',
    'field_date_value' => 'field_date_value',
    'field_date_value2' => 'field_date_value2',
    'field_location_value' => 'field_location_value',
  );
  $handler->display->display_options['style_options']['default'] = '-1';
  $handler->display->display_options['style_options']['info'] = array(
    'title' => array(
      'sortable' => 0,
      'separator' => '',
    ),
    'field_country_value' => array(
      'sortable' => 0,
      'separator' => '',
    ),
    'field_date_value' => array(
      'sortable' => 0,
      'separator' => '',
    ),
    'field_date_value2' => array(
      'sortable' => 0,
      'separator' => '',
    ),
    'field_location_value' => array(
      'sortable' => 0,
      'separator' => '',
    ),
  );
  /* Header: Global: Text area */
  $handler->display->display_options['header']['text']['id'] = 'area';
  $handler->display->display_options['header']['text']['table'] = 'views';
  $handler->display->display_options['header']['text']['field'] = 'area';
  $handler->display->display_options['header']['text']['content'] = '<?php
global $base_url;
?>
<div align="right"><a href="<?php echo $base_url;?>/ag_vacancies/rss" target="_blank"><img src="<?php echo $base_url;?>/sites/default/files/images/RSS_feed.png"> RSS feed</a><br /><i>[compliant with the Vacancy subset of the <a href="ftp://ftp.fao.org/docrep/fao/010/ai255e/ai255e00.pdf" target="_blank">Ag-Event AP</a>]</i><br />
<a href="<?php echo $base_url;?>/ag_vacancies/rdf" target="_blank"><img src="http://www.w3.org/RDF/icons/rdf_flyer.24"> RDF feed</a>
<br />&nbsp;</div>';
  $handler->display->display_options['header']['text']['format'] = '3';
  /* Field: Content: Title */
  $handler->display->display_options['fields']['title']['id'] = 'title';
  $handler->display->display_options['fields']['title']['table'] = 'node';
  $handler->display->display_options['fields']['title']['field'] = 'title';
  $handler->display->display_options['fields']['title']['label'] = 'Name';
  /* Filter criterion: Content: Published */
  $handler->display->display_options['filters']['status']['id'] = 'status';
  $handler->display->display_options['filters']['status']['table'] = 'node';
  $handler->display->display_options['filters']['status']['field'] = 'status';
  $handler->display->display_options['filters']['status']['value'] = '1';
  $handler->display->display_options['filters']['status']['group'] = '0';
  $handler->display->display_options['filters']['status']['expose']['operator'] = FALSE;
  /* Filter criterion: Content: Type */
  $handler->display->display_options['filters']['type']['id'] = 'type';
  $handler->display->display_options['filters']['type']['table'] = 'node';
  $handler->display->display_options['filters']['type']['field'] = 'type';
  $handler->display->display_options['filters']['type']['value'] = array(
    'agvacancy' => 'agvacancy',
  );
  $handler->display->display_options['filters']['type']['group'] = '0';
  $handler->display->display_options['filters']['type']['expose']['operator'] = FALSE;
  /* Filter criterion: Date: Date (node) */
  $handler->display->display_options['filters']['date_filter']['id'] = 'date_filter';
  $handler->display->display_options['filters']['date_filter']['table'] = 'node';
  $handler->display->display_options['filters']['date_filter']['field'] = 'date_filter';
  $handler->display->display_options['filters']['date_filter']['operator'] = '>=';
  $handler->display->display_options['filters']['date_filter']['value']['min'] = array(
    'date' => '',
  );
  $handler->display->display_options['filters']['date_filter']['value']['max'] = array(
    'date' => '',
  );
  $handler->display->display_options['filters']['date_filter']['value']['value'] = array(
    'date' => '',
  );
  $handler->display->display_options['filters']['date_filter']['group'] = '0';
  $handler->display->display_options['filters']['date_filter']['expose']['operator'] = FALSE;
  $handler->display->display_options['filters']['date_filter']['form_type'] = 'date_text';
  $handler->display->display_options['filters']['date_filter']['default_date'] = 'now';
  $handler->display->display_options['filters']['date_filter']['default_to_date'] = 'now';
  $handler->display->display_options['filters']['date_filter']['date_fields'] = array(
    'node_data_field_date.field_date_value' => 'node_data_field_date.field_date_value',
  );

  /* Display: Current vacancies */
  $handler = $view->new_display('page', 'Current vacancies', 'page_1');
  $handler->display->display_options['defaults']['title'] = FALSE;
  $handler->display->display_options['title'] = 'Current vacancies';
  $handler->display->display_options['defaults']['sorts'] = FALSE;
  $handler->display->display_options['path'] = 'ag_current-vacancies';

  /* Display: Current vacancies block */
  $handler = $view->new_display('block', 'Current vacancies block', 'block_1');
  $handler->display->display_options['defaults']['title'] = FALSE;
  $handler->display->display_options['title'] = 'Current vacancies';
  $handler->display->display_options['defaults']['use_more'] = FALSE;
  $handler->display->display_options['use_more'] = TRUE;
  $handler->display->display_options['defaults']['use_more_always'] = FALSE;
  $handler->display->display_options['defaults']['use_more_always'] = FALSE;
  $handler->display->display_options['use_more_always'] = FALSE;
  $handler->display->display_options['defaults']['use_more_text'] = FALSE;

  /* Display: All vacancies page */
  $handler = $view->new_display('page', 'All vacancies page', 'page_2');
  $handler->display->display_options['defaults']['title'] = FALSE;
  $handler->display->display_options['title'] = 'All vacancies';
  $handler->display->display_options['defaults']['sorts'] = FALSE;
  $handler->display->display_options['defaults']['filter_groups'] = FALSE;
  $handler->display->display_options['defaults']['filters'] = FALSE;
  /* Filter criterion: Content: Published */
  $handler->display->display_options['filters']['status']['id'] = 'status';
  $handler->display->display_options['filters']['status']['table'] = 'node';
  $handler->display->display_options['filters']['status']['field'] = 'status';
  $handler->display->display_options['filters']['status']['value'] = '1';
  $handler->display->display_options['filters']['status']['group'] = '0';
  $handler->display->display_options['filters']['status']['expose']['operator'] = FALSE;
  /* Filter criterion: Content: Type */
  $handler->display->display_options['filters']['type']['id'] = 'type';
  $handler->display->display_options['filters']['type']['table'] = 'node';
  $handler->display->display_options['filters']['type']['field'] = 'type';
  $handler->display->display_options['filters']['type']['value'] = array(
    'agvacancy' => 'agvacancy',
  );
  $handler->display->display_options['filters']['type']['group'] = '0';
  $handler->display->display_options['filters']['type']['expose']['operator'] = FALSE;
  $handler->display->display_options['path'] = 'ag_all-vacancies';

  /* Display: All vacancies block */
  $handler = $view->new_display('block', 'All vacancies block', 'block_2');
  $handler->display->display_options['defaults']['title'] = FALSE;
  $handler->display->display_options['title'] = 'All vacancies';
  $handler->display->display_options['defaults']['use_more'] = FALSE;
  $handler->display->display_options['use_more'] = TRUE;
  $handler->display->display_options['defaults']['use_more_always'] = FALSE;
  $handler->display->display_options['defaults']['use_more_always'] = FALSE;
  $handler->display->display_options['use_more_always'] = FALSE;
  $handler->display->display_options['defaults']['use_more_text'] = FALSE;
  $handler->display->display_options['defaults']['link_display'] = FALSE;
  $handler->display->display_options['link_display'] = 'page_2';
  $handler->display->display_options['defaults']['sorts'] = FALSE;
  /* Sort criterion: Broken/missing handler */
  $handler->display->display_options['sorts']['field_ag_deadline_value']['id'] = 'field_ag_deadline_value';
  $handler->display->display_options['sorts']['field_ag_deadline_value']['table'] = 'node_data_field_ag_deadline';
  $handler->display->display_options['sorts']['field_ag_deadline_value']['field'] = 'field_ag_deadline_value';
  $handler->display->display_options['defaults']['filter_groups'] = FALSE;
  $handler->display->display_options['defaults']['filters'] = FALSE;
  /* Filter criterion: Content: Published */
  $handler->display->display_options['filters']['status']['id'] = 'status';
  $handler->display->display_options['filters']['status']['table'] = 'node';
  $handler->display->display_options['filters']['status']['field'] = 'status';
  $handler->display->display_options['filters']['status']['value'] = '1';
  $handler->display->display_options['filters']['status']['group'] = '0';
  $handler->display->display_options['filters']['status']['expose']['operator'] = FALSE;
  /* Filter criterion: Content: Type */
  $handler->display->display_options['filters']['type']['id'] = 'type';
  $handler->display->display_options['filters']['type']['table'] = 'node';
  $handler->display->display_options['filters']['type']['field'] = 'type';
  $handler->display->display_options['filters']['type']['value'] = array(
    'agvacancy' => 'agvacancy',
  );
  $handler->display->display_options['filters']['type']['group'] = '0';
  $handler->display->display_options['filters']['type']['expose']['operator'] = FALSE;

  /* Display: Vacancies RSS Feed */
  $handler = $view->new_display('feed', 'Vacancies RSS Feed', 'feed_1');
  $handler->display->display_options['items_per_page'] = 0;
  $handler->display->display_options['pager']['type'] = 'some';
  $handler->display->display_options['style_plugin'] = 'rss';
  $handler->display->display_options['row_plugin'] = 'node_rss';
  $handler->display->display_options['defaults']['filter_groups'] = FALSE;
  $handler->display->display_options['defaults']['filters'] = FALSE;
  /* Filter criterion: Content: Published */
  $handler->display->display_options['filters']['status']['id'] = 'status';
  $handler->display->display_options['filters']['status']['table'] = 'node';
  $handler->display->display_options['filters']['status']['field'] = 'status';
  $handler->display->display_options['filters']['status']['value'] = '1';
  $handler->display->display_options['filters']['status']['group'] = '0';
  $handler->display->display_options['filters']['status']['expose']['operator'] = FALSE;
  /* Filter criterion: Content: Type */
  $handler->display->display_options['filters']['type']['id'] = 'type';
  $handler->display->display_options['filters']['type']['table'] = 'node';
  $handler->display->display_options['filters']['type']['field'] = 'type';
  $handler->display->display_options['filters']['type']['value'] = array(
    'agvacancy' => 'agvacancy',
  );
  $handler->display->display_options['filters']['type']['group'] = '0';
  $handler->display->display_options['filters']['type']['expose']['operator'] = FALSE;
  $handler->display->display_options['path'] = 'ag_vacancies/rss';
  $export['Ag_vacancies'] = $view;

  $view = new view();
  $view->name = 'ag_experts';
  $view->description = 'Experts: list / search';
  $view->tag = 'custom';
  $view->base_table = 'node';
  $view->human_name = 'Ag_experts';
  $view->core = 0;
  $view->api_version = '3.0';
  $view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */

  /* Display: Experts: search and browse */
  $handler = $view->new_display('default', 'Experts: search and browse', 'default');
  $handler->display->display_options['title'] = 'Experts';
  $handler->display->display_options['use_more_always'] = FALSE;
  $handler->display->display_options['access']['type'] = 'none';
  $handler->display->display_options['cache']['type'] = 'none';
  $handler->display->display_options['query']['type'] = 'views_query';
  $handler->display->display_options['query']['options']['distinct'] = TRUE;
  $handler->display->display_options['exposed_form']['type'] = 'basic';
  $handler->display->display_options['exposed_form']['options']['submit_button'] = 'Search';
  $handler->display->display_options['exposed_form']['options']['reset_button'] = TRUE;
  $handler->display->display_options['pager']['type'] = 'full';
  $handler->display->display_options['pager']['options']['items_per_page'] = 15;
  $handler->display->display_options['style_plugin'] = 'views_json';
  $handler->display->display_options['style_options']['plaintext_output'] = 1;
  $handler->display->display_options['style_options']['jsonp_prefix'] = '';
  $handler->display->display_options['style_options']['using_views_api_mode'] = 0;
  /* Footer: Global: Text area */
  $handler->display->display_options['footer']['text']['id'] = 'text';
  $handler->display->display_options['footer']['text']['table'] = 'views';
  $handler->display->display_options['footer']['text']['field'] = 'area';
  $handler->display->display_options['footer']['text']['empty'] = TRUE;
  $handler->display->display_options['footer']['text']['content'] = '<script language="javascript">
  selects = document.getElementsByTagName("select");
  for (i=0; i<selects.length; i++) {
    if (selects[i].size > 4)
      selects[i].size = 4;
  }
</script>
<?php
  global $base_url;
?>
<div align="right"><a href="<?php echo $base_url;?>/ag_institutions/rss" target="_blank"><img src="<?php echo $base_url;?>/sites/default/files/images/RSS_feed.png" /> RSS feed</a> [<i>compliant with the Ag-Org AP</i>] - <a href="<?php echo $base_url;?>/ag_institutions/rdf" target="_blank"><img src="http://www.w3.org/RDF/icons/rdf_flyer.24" /> RDF feed</a></div>';
  $handler->display->display_options['footer']['text']['format'] = '3';
  /* Field: Content: Title */
  $handler->display->display_options['fields']['title']['id'] = 'title';
  $handler->display->display_options['fields']['title']['table'] = 'node';
  $handler->display->display_options['fields']['title']['field'] = 'title';
  $handler->display->display_options['fields']['title']['label'] = 'Name';
  /* Field: Content: Agrovoc terms */
  $handler->display->display_options['fields']['field_ag_agrovoc']['id'] = 'field_ag_agrovoc';
  $handler->display->display_options['fields']['field_ag_agrovoc']['table'] = 'field_data_field_ag_agrovoc';
  $handler->display->display_options['fields']['field_ag_agrovoc']['field'] = 'field_ag_agrovoc';
  $handler->display->display_options['fields']['field_ag_agrovoc']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['field_ag_agrovoc']['type'] = 'i18n_taxonomy_term_reference_plain';
  $handler->display->display_options['fields']['field_ag_agrovoc']['delta_offset'] = '0';
  /* Field: Content: Email */
  $handler->display->display_options['fields']['field_ag_email']['id'] = 'field_ag_email';
  $handler->display->display_options['fields']['field_ag_email']['table'] = 'field_data_field_ag_email';
  $handler->display->display_options['fields']['field_ag_email']['field'] = 'field_ag_email';
  $handler->display->display_options['fields']['field_ag_email']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['field_ag_email']['type'] = 'email_plain';
  /* Field: Content: First name */
  $handler->display->display_options['fields']['field_ag_name']['id'] = 'field_ag_name';
  $handler->display->display_options['fields']['field_ag_name']['table'] = 'field_data_field_ag_name';
  $handler->display->display_options['fields']['field_ag_name']['field'] = 'field_ag_name';
  $handler->display->display_options['fields']['field_ag_name']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['field_ag_name']['type'] = 'text_plain';
  /* Field: Content: Last name */
  $handler->display->display_options['fields']['field_ag_lastname']['id'] = 'field_ag_lastname';
  $handler->display->display_options['fields']['field_ag_lastname']['table'] = 'field_data_field_ag_lastname';
  $handler->display->display_options['fields']['field_ag_lastname']['field'] = 'field_ag_lastname';
  $handler->display->display_options['fields']['field_ag_lastname']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['field_ag_lastname']['type'] = 'text_plain';
  /* Field: Content: Institution */
  $handler->display->display_options['fields']['field_ag_institution']['id'] = 'field_ag_institution';
  $handler->display->display_options['fields']['field_ag_institution']['table'] = 'field_data_field_ag_institution';
  $handler->display->display_options['fields']['field_ag_institution']['field'] = 'field_ag_institution';
  $handler->display->display_options['fields']['field_ag_institution']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['field_ag_institution']['type'] = 'node_reference_plain';
  $handler->display->display_options['fields']['field_ag_institution']['delta_offset'] = '0';
  /* Field: Content: Personal website */
  $handler->display->display_options['fields']['field_ag_personal_website']['id'] = 'field_ag_personal_website';
  $handler->display->display_options['fields']['field_ag_personal_website']['table'] = 'field_data_field_ag_personal_website';
  $handler->display->display_options['fields']['field_ag_personal_website']['field'] = 'field_ag_personal_website';
  $handler->display->display_options['fields']['field_ag_personal_website']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['field_ag_personal_website']['click_sort_column'] = 'url';
  $handler->display->display_options['fields']['field_ag_personal_website']['type'] = 'link_title_plain';
  /* Sort criterion: Content: Title */
  $handler->display->display_options['sorts']['title']['id'] = 'title';
  $handler->display->display_options['sorts']['title']['table'] = 'node';
  $handler->display->display_options['sorts']['title']['field'] = 'title';
  /* Filter criterion: Content: Published */
  $handler->display->display_options['filters']['status']['id'] = 'status';
  $handler->display->display_options['filters']['status']['table'] = 'node';
  $handler->display->display_options['filters']['status']['field'] = 'status';
  $handler->display->display_options['filters']['status']['value'] = '1';
  $handler->display->display_options['filters']['status']['group'] = '0';
  $handler->display->display_options['filters']['status']['expose']['operator'] = FALSE;
  /* Filter criterion: Content: Type */
  $handler->display->display_options['filters']['type']['id'] = 'type';
  $handler->display->display_options['filters']['type']['table'] = 'node';
  $handler->display->display_options['filters']['type']['field'] = 'type';
  $handler->display->display_options['filters']['type']['value'] = array(
    'agprofile' => 'agprofile',
  );
  $handler->display->display_options['filters']['type']['group'] = '0';
  $handler->display->display_options['filters']['type']['expose']['operator'] = FALSE;

  /* Display: Experts RDF Feed */
  $handler = $view->new_display('feed', 'Experts RDF Feed', 'feed_1');
  $handler->display->display_options['pager']['type'] = 'full';
  $handler->display->display_options['style_plugin'] = 'rss_fields';
  $handler->display->display_options['style_options']['channel'] = array(
    'core' => array(
      'views_rss_core' => array(
        'description' => '',
        'language' => '',
        'category' => '',
        'image' => '',
        'copyright' => '',
        'managingEditor' => '',
        'webMaster' => '',
        'generator' => '',
        'docs' => '',
        'cloud' => '',
        'ttl' => '',
        'skipHours' => '',
        'skipDays' => '',
      ),
    ),
  );
  $handler->display->display_options['style_options']['item'] = array(
    'core' => array(
      'views_rss_core' => array(
        'title' => 'title',
        'link' => '',
        'description' => 'field_ag_position_value',
        'author' => '',
        'category' => '',
        'comments' => '',
        'enclosure' => '',
        'guid' => '',
        'pubDate' => '',
      ),
    ),
    'dc' => array(
      'views_rss_dc' => array(
        'title' => '',
        'creator' => '',
        'subject' => '',
        'description' => '',
        'publisher' => '',
        'contributor' => '',
        'date' => '',
        'type' => '',
        'format' => '',
        'identifier' => '',
        'source' => '',
        'language' => '',
        'relation' => '',
        'coverage' => '',
        'rights' => '',
      ),
    ),
  );
  $handler->display->display_options['style_options']['feed_settings'] = array(
    'absolute_paths' => 1,
    'feed_in_links' => 0,
  );
  $handler->display->display_options['defaults']['arguments'] = FALSE;
  $handler->display->display_options['path'] = 'ag_experts/rdf';

  /* Display: Experts RSS Feed */
  $handler = $view->new_display('feed', 'Experts RSS Feed', 'feed_2');
  $handler->display->display_options['pager']['type'] = 'full';
  $handler->display->display_options['style_plugin'] = 'rss';
  $handler->display->display_options['style_options']['description'] = 'Experts';
  $handler->display->display_options['row_plugin'] = 'node_rss';
  $handler->display->display_options['defaults']['arguments'] = FALSE;
  $handler->display->display_options['path'] = 'ag_experts/rss';

  /* Display: Page */
  $handler = $view->new_display('page', 'Page', 'page_1');
  $handler->display->display_options['defaults']['style_plugin'] = FALSE;
  $handler->display->display_options['style_plugin'] = 'views_json';
  $handler->display->display_options['style_options']['root_object'] = 'persons';
  $handler->display->display_options['style_options']['top_child_object'] = 'person';
  $handler->display->display_options['style_options']['plaintext_output'] = 1;
  $handler->display->display_options['style_options']['jsonp_prefix'] = '';
  $handler->display->display_options['style_options']['using_views_api_mode'] = 0;
  $handler->display->display_options['defaults']['style_options'] = FALSE;
  $handler->display->display_options['defaults']['row_plugin'] = FALSE;
  $handler->display->display_options['defaults']['row_options'] = FALSE;
  $handler->display->display_options['path'] = 'rsstest';
  $export['ag_experts'] = $view;

  $view = new view();
  $view->name = 'browse_category2';
  $view->description = '';
  $view->tag = 'default';
  $view->base_table = 'taxonomy_term_data';
  $view->human_name = 'browse_category2';
  $view->core = 7;
  $view->api_version = '3.0';
  $view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */

  /* Display: Master */
  $handler = $view->new_display('default', 'Master', 'default');
  $handler->display->display_options['title'] = 'AGRIS Subject Categories ';
  $handler->display->display_options['use_more_always'] = FALSE;
  $handler->display->display_options['group_by'] = TRUE;
  $handler->display->display_options['access']['type'] = 'none';
  $handler->display->display_options['cache']['type'] = 'none';
  $handler->display->display_options['query']['type'] = 'views_query';
  $handler->display->display_options['query']['options']['distinct'] = TRUE;
  $handler->display->display_options['exposed_form']['type'] = 'basic';
  $handler->display->display_options['exposed_form']['options']['submit_button'] = 'Search';
  $handler->display->display_options['exposed_form']['options']['reset_button'] = TRUE;
  $handler->display->display_options['pager']['type'] = 'full';
  $handler->display->display_options['pager']['options']['items_per_page'] = '60';
  $handler->display->display_options['pager']['options']['offset'] = '0';
  $handler->display->display_options['pager']['options']['id'] = '0';
  $handler->display->display_options['style_plugin'] = 'table';
  /* Header: Global: Text area */
  $handler->display->display_options['header']['area']['id'] = 'area';
  $handler->display->display_options['header']['area']['table'] = 'views';
  $handler->display->display_options['header']['area']['field'] = 'area';
  $handler->display->display_options['header']['area']['content'] = 'Here you can browse the catalog by subject.
Clicking on a term will display the bibliographic records of resources whose topic is identified by the term.';
  $handler->display->display_options['header']['area']['format'] = '1';
  /* Relationship: Taxonomy term: Content with term */
  $handler->display->display_options['relationships']['nid']['id'] = 'nid';
  $handler->display->display_options['relationships']['nid']['table'] = 'taxonomy_index';
  $handler->display->display_options['relationships']['nid']['field'] = 'nid';
  /* Relationship: Taxonomy term: Representative node */
  $handler->display->display_options['relationships']['tid_representative']['id'] = 'tid_representative';
  $handler->display->display_options['relationships']['tid_representative']['table'] = 'taxonomy_term_data';
  $handler->display->display_options['relationships']['tid_representative']['field'] = 'tid_representative';
  /* Field: Taxonomy term: Term ID */
  $handler->display->display_options['fields']['tid']['id'] = 'tid';
  $handler->display->display_options['fields']['tid']['table'] = 'taxonomy_term_data';
  $handler->display->display_options['fields']['tid']['field'] = 'tid';
  $handler->display->display_options['fields']['tid']['label'] = '';
  $handler->display->display_options['fields']['tid']['exclude'] = TRUE;
  $handler->display->display_options['fields']['tid']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['tid']['separator'] = '';
  /* Field: Taxonomy term: Name */
  $handler->display->display_options['fields']['name']['id'] = 'name';
  $handler->display->display_options['fields']['name']['table'] = 'taxonomy_term_data';
  $handler->display->display_options['fields']['name']['field'] = 'name';
  $handler->display->display_options['fields']['name']['label'] = '';
  $handler->display->display_options['fields']['name']['alter']['make_link'] = TRUE;
  $handler->display->display_options['fields']['name']['alter']['path'] = 'taxonomy/term/[tid]';
  $handler->display->display_options['fields']['name']['alter']['word_boundary'] = FALSE;
  $handler->display->display_options['fields']['name']['alter']['ellipsis'] = FALSE;
  $handler->display->display_options['fields']['name']['element_label_colon'] = FALSE;
  /* Field: COUNT(DISTINCT Content: Nid) */
  $handler->display->display_options['fields']['nid']['id'] = 'nid';
  $handler->display->display_options['fields']['nid']['table'] = 'node';
  $handler->display->display_options['fields']['nid']['field'] = 'nid';
  $handler->display->display_options['fields']['nid']['relationship'] = 'nid';
  $handler->display->display_options['fields']['nid']['group_type'] = 'count_distinct';
  $handler->display->display_options['fields']['nid']['label'] = '';
  $handler->display->display_options['fields']['nid']['alter']['make_link'] = TRUE;
  $handler->display->display_options['fields']['nid']['alter']['path'] = 'ag_dlios-by-subjects/[tid]';
  $handler->display->display_options['fields']['nid']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['nid']['separator'] = '';
  $handler->display->display_options['fields']['nid']['prefix'] = '(';
  $handler->display->display_options['fields']['nid']['suffix'] = ')';
  /* Sort criterion: Taxonomy term: Name */
  $handler->display->display_options['sorts']['name']['id'] = 'name';
  $handler->display->display_options['sorts']['name']['table'] = 'taxonomy_term_data';
  $handler->display->display_options['sorts']['name']['field'] = 'name';
  /* Filter criterion: Taxonomy vocabulary: Machine name */
  $handler->display->display_options['filters']['machine_name']['id'] = 'machine_name';
  $handler->display->display_options['filters']['machine_name']['table'] = 'taxonomy_vocabulary';
  $handler->display->display_options['filters']['machine_name']['field'] = 'machine_name';
  $handler->display->display_options['filters']['machine_name']['value'] = array(
    'vocabulary_10' => 'vocabulary_10',
  );
  $handler->display->display_options['filters']['machine_name']['group'] = 1;
  /* Filter criterion: Taxonomy term: Language */
  $handler->display->display_options['filters']['language_1']['id'] = 'language_1';
  $handler->display->display_options['filters']['language_1']['table'] = 'taxonomy_term_data';
  $handler->display->display_options['filters']['language_1']['field'] = 'language';
  $handler->display->display_options['filters']['language_1']['value'] = array(
    '***CURRENT_LANGUAGE***' => '***CURRENT_LANGUAGE***',
    '***DEFAULT_LANGUAGE***' => '***DEFAULT_LANGUAGE***',
    'und' => 'und',
  );
  $handler->display->display_options['filters']['language_1']['group'] = 1;
  /* Filter criterion: Content: Type */
  $handler->display->display_options['filters']['type']['id'] = 'type';
  $handler->display->display_options['filters']['type']['table'] = 'node';
  $handler->display->display_options['filters']['type']['field'] = 'type';
  $handler->display->display_options['filters']['type']['relationship'] = 'nid';
  $handler->display->display_options['filters']['type']['value'] = array(
    'agdlios' => 'agdlios',
  );

  /* Display: By subject AGRIS */
  $handler = $view->new_display('page', 'By subject AGRIS', 'page');
  $handler->display->display_options['defaults']['pager'] = FALSE;
  $handler->display->display_options['pager']['type'] = 'full';
  $handler->display->display_options['pager']['options']['items_per_page'] = '100';
  $handler->display->display_options['pager']['options']['offset'] = '0';
  $handler->display->display_options['pager']['options']['id'] = '0';
  $handler->display->display_options['pager']['options']['quantity'] = '9';
  $handler->display->display_options['defaults']['style_plugin'] = FALSE;
  $handler->display->display_options['style_plugin'] = 'table';
  $handler->display->display_options['style_options']['columns'] = array(
    'tid' => 'name',
    'description_1' => 'name',
    'name' => 'name',
    'description' => 'description',
    'nid' => 'nid',
  );
  $handler->display->display_options['style_options']['default'] = '-1';
  $handler->display->display_options['style_options']['info'] = array(
    'tid' => array(
      'sortable' => 0,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'description_1' => array(
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'name' => array(
      'sortable' => 0,
      'default_sort_order' => 'asc',
      'align' => 'views-align-left',
      'separator' => '',
      'empty_column' => 0,
    ),
    'description' => array(
      'align' => 'views-align-left',
      'separator' => '',
      'empty_column' => 0,
    ),
    'nid' => array(
      'sortable' => 0,
      'default_sort_order' => 'asc',
      'align' => 'views-align-left',
      'separator' => '',
      'empty_column' => 0,
    ),
  );
  $handler->display->display_options['defaults']['style_options'] = FALSE;
  $handler->display->display_options['defaults']['row_plugin'] = FALSE;
  $handler->display->display_options['defaults']['row_options'] = FALSE;
  $handler->display->display_options['defaults']['header'] = FALSE;
  /* Header: Global: Text area */
  $handler->display->display_options['header']['area']['id'] = 'area';
  $handler->display->display_options['header']['area']['table'] = 'views';
  $handler->display->display_options['header']['area']['field'] = 'area';
  $handler->display->display_options['header']['area']['content'] = 'Here you can browse the catalog by subject.
Clicking on a term will display the bibliographic records of resources whose topic is identified by the term.';
  $handler->display->display_options['header']['area']['format'] = '1';
  $handler->display->display_options['defaults']['relationships'] = FALSE;
  /* Relationship: Taxonomy term: Content with term */
  $handler->display->display_options['relationships']['nid']['id'] = 'nid';
  $handler->display->display_options['relationships']['nid']['table'] = 'taxonomy_index';
  $handler->display->display_options['relationships']['nid']['field'] = 'nid';
  $handler->display->display_options['defaults']['fields'] = FALSE;
  /* Field: Taxonomy term: Term ID */
  $handler->display->display_options['fields']['tid']['id'] = 'tid';
  $handler->display->display_options['fields']['tid']['table'] = 'taxonomy_term_data';
  $handler->display->display_options['fields']['tid']['field'] = 'tid';
  $handler->display->display_options['fields']['tid']['label'] = '';
  $handler->display->display_options['fields']['tid']['exclude'] = TRUE;
  $handler->display->display_options['fields']['tid']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['tid']['separator'] = '';
  /* Field: Taxonomy term: Term description */
  $handler->display->display_options['fields']['description_1']['id'] = 'description_1';
  $handler->display->display_options['fields']['description_1']['table'] = 'taxonomy_term_data';
  $handler->display->display_options['fields']['description_1']['field'] = 'description';
  $handler->display->display_options['fields']['description_1']['label'] = '';
  $handler->display->display_options['fields']['description_1']['exclude'] = TRUE;
  $handler->display->display_options['fields']['description_1']['element_label_colon'] = FALSE;
  /* Field: Taxonomy term: Name */
  $handler->display->display_options['fields']['name']['id'] = 'name';
  $handler->display->display_options['fields']['name']['table'] = 'taxonomy_term_data';
  $handler->display->display_options['fields']['name']['field'] = 'name';
  $handler->display->display_options['fields']['name']['label'] = '';
  $handler->display->display_options['fields']['name']['alter']['make_link'] = TRUE;
  $handler->display->display_options['fields']['name']['alter']['path'] = 'ag_dlios-by-subjects/[tid]/[description_1]';
  $handler->display->display_options['fields']['name']['alter']['word_boundary'] = FALSE;
  $handler->display->display_options['fields']['name']['alter']['ellipsis'] = FALSE;
  $handler->display->display_options['fields']['name']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['name']['element_default_classes'] = FALSE;
  /* Field: Taxonomy term: Term description */
  $handler->display->display_options['fields']['description']['id'] = 'description';
  $handler->display->display_options['fields']['description']['table'] = 'taxonomy_term_data';
  $handler->display->display_options['fields']['description']['field'] = 'description';
  $handler->display->display_options['fields']['description']['label'] = '';
  $handler->display->display_options['fields']['description']['alter']['make_link'] = TRUE;
  $handler->display->display_options['fields']['description']['alter']['path'] = 'ag_dlios-by-subjects/[tid]/[description_1]';
  $handler->display->display_options['fields']['description']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['description']['element_default_classes'] = FALSE;
  /* Field: COUNT(Content: Nid) */
  $handler->display->display_options['fields']['nid']['id'] = 'nid';
  $handler->display->display_options['fields']['nid']['table'] = 'node';
  $handler->display->display_options['fields']['nid']['field'] = 'nid';
  $handler->display->display_options['fields']['nid']['relationship'] = 'nid';
  $handler->display->display_options['fields']['nid']['group_type'] = 'count';
  $handler->display->display_options['fields']['nid']['label'] = '';
  $handler->display->display_options['fields']['nid']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['nid']['element_default_classes'] = FALSE;
  $handler->display->display_options['fields']['nid']['separator'] = '';
  $handler->display->display_options['fields']['nid']['prefix'] = '(';
  $handler->display->display_options['fields']['nid']['suffix'] = ')';
  $handler->display->display_options['path'] = 'ag_bysubject/agris-categories';
  $handler->display->display_options['menu']['type'] = 'default tab';
  $handler->display->display_options['menu']['title'] = 'By Agris category';
  $handler->display->display_options['menu']['weight'] = '0';
  $handler->display->display_options['menu']['context'] = 0;
  $handler->display->display_options['tab_options']['weight'] = '0';

  /* Display: By Subject Agrovoc  */
  $handler = $view->new_display('page', 'By Subject Agrovoc ', 'page_1');
  $handler->display->display_options['defaults']['title'] = FALSE;
  $handler->display->display_options['title'] = 'By Agrovoc terms';
  $handler->display->display_options['defaults']['style_plugin'] = FALSE;
  $handler->display->display_options['style_plugin'] = 'grid';
  $handler->display->display_options['style_options']['row_class'] = 'aligntopleft';
  $handler->display->display_options['style_options']['columns'] = '3';
  $handler->display->display_options['defaults']['style_options'] = FALSE;
  $handler->display->display_options['defaults']['row_plugin'] = FALSE;
  $handler->display->display_options['row_plugin'] = 'fields';
  $handler->display->display_options['row_options']['inline'] = array(
    'name_i18n' => 'name_i18n',
    'nid' => 'nid',
  );
  $handler->display->display_options['row_options']['separator'] = ' ';
  $handler->display->display_options['row_options']['hide_empty'] = TRUE;
  $handler->display->display_options['defaults']['row_options'] = FALSE;
  $handler->display->display_options['defaults']['relationships'] = FALSE;
  /* Relationship: Taxonomy term: Content with term */
  $handler->display->display_options['relationships']['nid']['id'] = 'nid';
  $handler->display->display_options['relationships']['nid']['table'] = 'taxonomy_index';
  $handler->display->display_options['relationships']['nid']['field'] = 'nid';
  $handler->display->display_options['defaults']['fields'] = FALSE;
  /* Field: Taxonomy term: Term ID */
  $handler->display->display_options['fields']['tid']['id'] = 'tid';
  $handler->display->display_options['fields']['tid']['table'] = 'taxonomy_term_data';
  $handler->display->display_options['fields']['tid']['field'] = 'tid';
  $handler->display->display_options['fields']['tid']['label'] = '';
  $handler->display->display_options['fields']['tid']['exclude'] = TRUE;
  $handler->display->display_options['fields']['tid']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['tid']['separator'] = '';
  /* Field: Taxonomy term: Name (translated) */
  $handler->display->display_options['fields']['name_i18n']['id'] = 'name_i18n';
  $handler->display->display_options['fields']['name_i18n']['table'] = 'taxonomy_term_data';
  $handler->display->display_options['fields']['name_i18n']['field'] = 'name_i18n';
  $handler->display->display_options['fields']['name_i18n']['label'] = '';
  $handler->display->display_options['fields']['name_i18n']['alter']['make_link'] = TRUE;
  $handler->display->display_options['fields']['name_i18n']['alter']['path'] = 'ag_dliosbysubjectsagrovoc/[tid]';
  $handler->display->display_options['fields']['name_i18n']['element_label_colon'] = FALSE;
  /* Field: COUNT(DISTINCT Content: Nid) */
  $handler->display->display_options['fields']['nid']['id'] = 'nid';
  $handler->display->display_options['fields']['nid']['table'] = 'node';
  $handler->display->display_options['fields']['nid']['field'] = 'nid';
  $handler->display->display_options['fields']['nid']['relationship'] = 'nid';
  $handler->display->display_options['fields']['nid']['group_type'] = 'count_distinct';
  $handler->display->display_options['fields']['nid']['label'] = '';
  $handler->display->display_options['fields']['nid']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['nid']['separator'] = '';
  $handler->display->display_options['fields']['nid']['prefix'] = '(';
  $handler->display->display_options['fields']['nid']['suffix'] = ')';
  $handler->display->display_options['defaults']['sorts'] = FALSE;
  /* Sort criterion: Taxonomy term: Name */
  $handler->display->display_options['sorts']['name']['id'] = 'name';
  $handler->display->display_options['sorts']['name']['table'] = 'taxonomy_term_data';
  $handler->display->display_options['sorts']['name']['field'] = 'name';
  $handler->display->display_options['defaults']['filter_groups'] = FALSE;
  $handler->display->display_options['defaults']['filters'] = FALSE;
  /* Filter criterion: Taxonomy vocabulary: Machine name */
  $handler->display->display_options['filters']['machine_name']['id'] = 'machine_name';
  $handler->display->display_options['filters']['machine_name']['table'] = 'taxonomy_vocabulary';
  $handler->display->display_options['filters']['machine_name']['field'] = 'machine_name';
  $handler->display->display_options['filters']['machine_name']['value'] = array(
    'vocabulary_16' => 'vocabulary_16',
  );
  $handler->display->display_options['filters']['machine_name']['group'] = 1;
  /* Filter criterion: Content: Type */
  $handler->display->display_options['filters']['type']['id'] = 'type';
  $handler->display->display_options['filters']['type']['table'] = 'node';
  $handler->display->display_options['filters']['type']['field'] = 'type';
  $handler->display->display_options['filters']['type']['relationship'] = 'nid';
  $handler->display->display_options['filters']['type']['value'] = array(
    'agdlios' => 'agdlios',
  );
  $handler->display->display_options['filters']['type']['group'] = 1;
  /* Filter criterion: Taxonomy term: Name (translated) */
  $handler->display->display_options['filters']['name_i18n']['id'] = 'name_i18n';
  $handler->display->display_options['filters']['name_i18n']['table'] = 'taxonomy_term_data';
  $handler->display->display_options['filters']['name_i18n']['field'] = 'name_i18n';
  $handler->display->display_options['filters']['name_i18n']['operator'] = 'contains';
  $handler->display->display_options['filters']['name_i18n']['exposed'] = TRUE;
  $handler->display->display_options['filters']['name_i18n']['expose']['operator_id'] = 'name_i18n_op';
  $handler->display->display_options['filters']['name_i18n']['expose']['label'] = 'Search';
  $handler->display->display_options['filters']['name_i18n']['expose']['operator'] = 'name_i18n_op';
  $handler->display->display_options['filters']['name_i18n']['expose']['identifier'] = 'name_i18n';
  $handler->display->display_options['path'] = 'ag_bysubject/agrovoc';
  $handler->display->display_options['menu']['type'] = 'tab';
  $handler->display->display_options['menu']['title'] = 'By Agrovoc term';
  $handler->display->display_options['menu']['weight'] = '0';
  $handler->display->display_options['menu']['context'] = 0;

  /* Display: By subject  */
  $handler = $view->new_display('page', 'By subject ', 'page_2');
  $handler->display->display_options['defaults']['title'] = FALSE;
  $handler->display->display_options['title'] = 'By subject ';
  $handler->display->display_options['defaults']['pager'] = FALSE;
  $handler->display->display_options['pager']['type'] = 'full';
  $handler->display->display_options['pager']['options']['items_per_page'] = '100';
  $handler->display->display_options['pager']['options']['offset'] = '0';
  $handler->display->display_options['pager']['options']['id'] = '0';
  $handler->display->display_options['pager']['options']['quantity'] = '9';
  $handler->display->display_options['defaults']['style_plugin'] = FALSE;
  $handler->display->display_options['style_plugin'] = 'table';
  $handler->display->display_options['style_options']['columns'] = array(
    'tid' => 'name',
    'description_1' => 'name',
    'name' => 'name',
    'description' => 'description',
    'nid' => 'nid',
  );
  $handler->display->display_options['style_options']['default'] = '-1';
  $handler->display->display_options['style_options']['info'] = array(
    'tid' => array(
      'sortable' => 0,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'description_1' => array(
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'name' => array(
      'sortable' => 0,
      'default_sort_order' => 'asc',
      'align' => 'views-align-left',
      'separator' => '',
      'empty_column' => 0,
    ),
    'description' => array(
      'align' => 'views-align-left',
      'separator' => '',
      'empty_column' => 0,
    ),
    'nid' => array(
      'sortable' => 0,
      'default_sort_order' => 'asc',
      'align' => 'views-align-left',
      'separator' => '',
      'empty_column' => 0,
    ),
  );
  $handler->display->display_options['defaults']['style_options'] = FALSE;
  $handler->display->display_options['defaults']['row_plugin'] = FALSE;
  $handler->display->display_options['defaults']['row_options'] = FALSE;
  $handler->display->display_options['defaults']['relationships'] = FALSE;
  /* Relationship: Taxonomy term: Content with term */
  $handler->display->display_options['relationships']['nid']['id'] = 'nid';
  $handler->display->display_options['relationships']['nid']['table'] = 'taxonomy_index';
  $handler->display->display_options['relationships']['nid']['field'] = 'nid';
  $handler->display->display_options['defaults']['fields'] = FALSE;
  /* Field: Taxonomy term: Term ID */
  $handler->display->display_options['fields']['tid']['id'] = 'tid';
  $handler->display->display_options['fields']['tid']['table'] = 'taxonomy_term_data';
  $handler->display->display_options['fields']['tid']['field'] = 'tid';
  $handler->display->display_options['fields']['tid']['label'] = '';
  $handler->display->display_options['fields']['tid']['exclude'] = TRUE;
  $handler->display->display_options['fields']['tid']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['tid']['separator'] = '';
  /* Field: Taxonomy term: Term description */
  $handler->display->display_options['fields']['description_1']['id'] = 'description_1';
  $handler->display->display_options['fields']['description_1']['table'] = 'taxonomy_term_data';
  $handler->display->display_options['fields']['description_1']['field'] = 'description';
  $handler->display->display_options['fields']['description_1']['label'] = '';
  $handler->display->display_options['fields']['description_1']['exclude'] = TRUE;
  $handler->display->display_options['fields']['description_1']['element_label_colon'] = FALSE;
  /* Field: Taxonomy term: Name */
  $handler->display->display_options['fields']['name']['id'] = 'name';
  $handler->display->display_options['fields']['name']['table'] = 'taxonomy_term_data';
  $handler->display->display_options['fields']['name']['field'] = 'name';
  $handler->display->display_options['fields']['name']['label'] = '';
  $handler->display->display_options['fields']['name']['alter']['make_link'] = TRUE;
  $handler->display->display_options['fields']['name']['alter']['path'] = 'ag_dlios-by-subjects/[tid]/[description_1]';
  $handler->display->display_options['fields']['name']['alter']['word_boundary'] = FALSE;
  $handler->display->display_options['fields']['name']['alter']['ellipsis'] = FALSE;
  $handler->display->display_options['fields']['name']['element_label_colon'] = FALSE;
  /* Field: Taxonomy term: Term description */
  $handler->display->display_options['fields']['description']['id'] = 'description';
  $handler->display->display_options['fields']['description']['table'] = 'taxonomy_term_data';
  $handler->display->display_options['fields']['description']['field'] = 'description';
  $handler->display->display_options['fields']['description']['label'] = '';
  $handler->display->display_options['fields']['description']['alter']['make_link'] = TRUE;
  $handler->display->display_options['fields']['description']['alter']['path'] = 'ag_dlios-by-subjects/[tid]/[description_1]';
  $handler->display->display_options['fields']['description']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['description']['element_default_classes'] = FALSE;
  /* Field: COUNT(DISTINCT Content: Nid) */
  $handler->display->display_options['fields']['nid']['id'] = 'nid';
  $handler->display->display_options['fields']['nid']['table'] = 'node';
  $handler->display->display_options['fields']['nid']['field'] = 'nid';
  $handler->display->display_options['fields']['nid']['relationship'] = 'nid';
  $handler->display->display_options['fields']['nid']['group_type'] = 'count_distinct';
  $handler->display->display_options['fields']['nid']['label'] = '';
  $handler->display->display_options['fields']['nid']['alter']['path'] = 'ag_dlios-by-subjects/[tid]';
  $handler->display->display_options['fields']['nid']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['nid']['separator'] = '';
  $handler->display->display_options['fields']['nid']['prefix'] = '(';
  $handler->display->display_options['fields']['nid']['suffix'] = ')';
  $handler->display->display_options['defaults']['filter_groups'] = FALSE;
  $handler->display->display_options['defaults']['filters'] = FALSE;
  /* Filter criterion: Taxonomy vocabulary: Machine name */
  $handler->display->display_options['filters']['machine_name']['id'] = 'machine_name';
  $handler->display->display_options['filters']['machine_name']['table'] = 'taxonomy_vocabulary';
  $handler->display->display_options['filters']['machine_name']['field'] = 'machine_name';
  $handler->display->display_options['filters']['machine_name']['value'] = array(
    'vocabulary_10' => 'vocabulary_10',
  );
  $handler->display->display_options['filters']['machine_name']['group'] = 1;
  /* Filter criterion: Taxonomy term: Language */
  $handler->display->display_options['filters']['language']['id'] = 'language';
  $handler->display->display_options['filters']['language']['table'] = 'taxonomy_term_data';
  $handler->display->display_options['filters']['language']['field'] = 'language';
  $handler->display->display_options['filters']['language']['value'] = array(
    '***CURRENT_LANGUAGE***' => '***CURRENT_LANGUAGE***',
    '***DEFAULT_LANGUAGE***' => '***DEFAULT_LANGUAGE***',
    'und' => 'und',
  );
  /* Filter criterion: Content: Type */
  $handler->display->display_options['filters']['type']['id'] = 'type';
  $handler->display->display_options['filters']['type']['table'] = 'node';
  $handler->display->display_options['filters']['type']['field'] = 'type';
  $handler->display->display_options['filters']['type']['relationship'] = 'nid';
  $handler->display->display_options['filters']['type']['value'] = array(
    'agdlios' => 'agdlios',
  );
  $handler->display->display_options['path'] = 'ag_bysubject';

  /* Display: Vocabularies */
  $handler = $view->new_display('page', 'Vocabularies', 'page_3');
  $handler->display->display_options['defaults']['title'] = FALSE;
  $handler->display->display_options['title'] = 'Subject vocabularies';
  $handler->display->display_options['defaults']['group_by'] = FALSE;
  $handler->display->display_options['defaults']['pager'] = FALSE;
  $handler->display->display_options['pager']['type'] = 'full';
  $handler->display->display_options['pager']['options']['items_per_page'] = '10';
  $handler->display->display_options['pager']['options']['offset'] = '0';
  $handler->display->display_options['pager']['options']['id'] = '0';
  $handler->display->display_options['pager']['options']['quantity'] = '9';
  $handler->display->display_options['defaults']['style_plugin'] = FALSE;
  $handler->display->display_options['style_plugin'] = 'table';
  $handler->display->display_options['style_options']['columns'] = array(
    'tid' => 'name',
    'name' => 'name',
    'description' => 'description',
    'nid' => 'nid',
  );
  $handler->display->display_options['style_options']['default'] = '-1';
  $handler->display->display_options['style_options']['info'] = array(
    'tid' => array(
      'sortable' => 0,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'name' => array(
      'sortable' => 0,
      'default_sort_order' => 'asc',
      'align' => 'views-align-left',
      'separator' => '',
      'empty_column' => 0,
    ),
    'description' => array(
      'align' => 'views-align-left',
      'separator' => '',
      'empty_column' => 0,
    ),
    'nid' => array(
      'sortable' => 0,
      'default_sort_order' => 'asc',
      'align' => 'views-align-left',
      'separator' => '',
      'empty_column' => 0,
    ),
  );
  $handler->display->display_options['defaults']['style_options'] = FALSE;
  $handler->display->display_options['defaults']['row_plugin'] = FALSE;
  $handler->display->display_options['defaults']['row_options'] = FALSE;
  $handler->display->display_options['defaults']['relationships'] = FALSE;
  $handler->display->display_options['defaults']['fields'] = FALSE;
  /* Field: Taxonomy term: Term ID */
  $handler->display->display_options['fields']['tid']['id'] = 'tid';
  $handler->display->display_options['fields']['tid']['table'] = 'taxonomy_term_data';
  $handler->display->display_options['fields']['tid']['field'] = 'tid';
  $handler->display->display_options['fields']['tid']['label'] = '';
  $handler->display->display_options['fields']['tid']['exclude'] = TRUE;
  $handler->display->display_options['fields']['tid']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['tid']['separator'] = '';
  /* Field: Taxonomy term: Term description */
  $handler->display->display_options['fields']['description_1']['id'] = 'description_1';
  $handler->display->display_options['fields']['description_1']['table'] = 'taxonomy_term_data';
  $handler->display->display_options['fields']['description_1']['field'] = 'description';
  $handler->display->display_options['fields']['description_1']['label'] = '';
  $handler->display->display_options['fields']['description_1']['exclude'] = TRUE;
  $handler->display->display_options['fields']['description_1']['element_label_colon'] = FALSE;
  /* Field: Taxonomy term: Name */
  $handler->display->display_options['fields']['name']['id'] = 'name';
  $handler->display->display_options['fields']['name']['table'] = 'taxonomy_term_data';
  $handler->display->display_options['fields']['name']['field'] = 'name';
  $handler->display->display_options['fields']['name']['label'] = '';
  $handler->display->display_options['fields']['name']['alter']['make_link'] = TRUE;
  $handler->display->display_options['fields']['name']['alter']['path'] = 'ag_dlios-by-subjects/[tid]/[description_1]';
  $handler->display->display_options['fields']['name']['alter']['word_boundary'] = FALSE;
  $handler->display->display_options['fields']['name']['alter']['ellipsis'] = FALSE;
  $handler->display->display_options['fields']['name']['element_label_colon'] = FALSE;
  /* Field: Taxonomy term: Term description */
  $handler->display->display_options['fields']['description']['id'] = 'description';
  $handler->display->display_options['fields']['description']['table'] = 'taxonomy_term_data';
  $handler->display->display_options['fields']['description']['field'] = 'description';
  $handler->display->display_options['fields']['description']['label'] = '';
  $handler->display->display_options['fields']['description']['alter']['make_link'] = TRUE;
  $handler->display->display_options['fields']['description']['alter']['path'] = 'ag_dlios-by-subjects/[tid]/[description_1]';
  $handler->display->display_options['fields']['description']['element_label_colon'] = FALSE;
  $handler->display->display_options['defaults']['filter_groups'] = FALSE;
  $handler->display->display_options['defaults']['filters'] = FALSE;
  /* Filter criterion: Taxonomy vocabulary: Machine name */
  $handler->display->display_options['filters']['machine_name']['id'] = 'machine_name';
  $handler->display->display_options['filters']['machine_name']['table'] = 'taxonomy_vocabulary';
  $handler->display->display_options['filters']['machine_name']['field'] = 'machine_name';
  $handler->display->display_options['filters']['machine_name']['value'] = array(
    'vocabulary_10' => 'vocabulary_10',
  );
  $handler->display->display_options['filters']['machine_name']['group'] = 1;
  /* Filter criterion: Taxonomy term: Language */
  $handler->display->display_options['filters']['language']['id'] = 'language';
  $handler->display->display_options['filters']['language']['table'] = 'taxonomy_term_data';
  $handler->display->display_options['filters']['language']['field'] = 'language';
  $handler->display->display_options['filters']['language']['value'] = array(
    '***CURRENT_LANGUAGE***' => '***CURRENT_LANGUAGE***',
    '***DEFAULT_LANGUAGE***' => '***DEFAULT_LANGUAGE***',
    'und' => 'und',
  );
  $handler->display->display_options['path'] = 'ag_vocabularies';

  /* Display: Vocabularies - Tab 1  */
  $handler = $view->new_display('page', 'Vocabularies - Tab 1 ', 'page_4');
  $handler->display->display_options['defaults']['pager'] = FALSE;
  $handler->display->display_options['pager']['type'] = 'full';
  $handler->display->display_options['pager']['options']['items_per_page'] = '100';
  $handler->display->display_options['pager']['options']['offset'] = '0';
  $handler->display->display_options['pager']['options']['id'] = '0';
  $handler->display->display_options['pager']['options']['quantity'] = '9';
  $handler->display->display_options['defaults']['style_plugin'] = FALSE;
  $handler->display->display_options['style_plugin'] = 'table';
  $handler->display->display_options['style_options']['columns'] = array(
    'tid' => 'name',
    'name' => 'name',
    'description' => 'description',
    'nid' => 'nid',
  );
  $handler->display->display_options['style_options']['default'] = '-1';
  $handler->display->display_options['style_options']['info'] = array(
    'tid' => array(
      'sortable' => 0,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'name' => array(
      'sortable' => 0,
      'default_sort_order' => 'asc',
      'align' => 'views-align-left',
      'separator' => '',
      'empty_column' => 0,
    ),
    'description' => array(
      'align' => 'views-align-left',
      'separator' => '',
      'empty_column' => 0,
    ),
    'nid' => array(
      'sortable' => 0,
      'default_sort_order' => 'asc',
      'align' => 'views-align-left',
      'separator' => '',
      'empty_column' => 0,
    ),
  );
  $handler->display->display_options['defaults']['style_options'] = FALSE;
  $handler->display->display_options['defaults']['row_plugin'] = FALSE;
  $handler->display->display_options['defaults']['row_options'] = FALSE;
  $handler->display->display_options['defaults']['relationships'] = FALSE;
  /* Relationship: Taxonomy term: Content with term */
  $handler->display->display_options['relationships']['nid']['id'] = 'nid';
  $handler->display->display_options['relationships']['nid']['table'] = 'taxonomy_index';
  $handler->display->display_options['relationships']['nid']['field'] = 'nid';
  $handler->display->display_options['defaults']['fields'] = FALSE;
  /* Field: Taxonomy term: Term ID */
  $handler->display->display_options['fields']['tid']['id'] = 'tid';
  $handler->display->display_options['fields']['tid']['table'] = 'taxonomy_term_data';
  $handler->display->display_options['fields']['tid']['field'] = 'tid';
  $handler->display->display_options['fields']['tid']['label'] = '';
  $handler->display->display_options['fields']['tid']['exclude'] = TRUE;
  $handler->display->display_options['fields']['tid']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['tid']['separator'] = '';
  /* Field: Taxonomy term: Term description */
  $handler->display->display_options['fields']['description_1']['id'] = 'description_1';
  $handler->display->display_options['fields']['description_1']['table'] = 'taxonomy_term_data';
  $handler->display->display_options['fields']['description_1']['field'] = 'description';
  $handler->display->display_options['fields']['description_1']['label'] = '';
  $handler->display->display_options['fields']['description_1']['exclude'] = TRUE;
  $handler->display->display_options['fields']['description_1']['element_label_colon'] = FALSE;
  /* Field: Taxonomy term: Name */
  $handler->display->display_options['fields']['name']['id'] = 'name';
  $handler->display->display_options['fields']['name']['table'] = 'taxonomy_term_data';
  $handler->display->display_options['fields']['name']['field'] = 'name';
  $handler->display->display_options['fields']['name']['label'] = '';
  $handler->display->display_options['fields']['name']['alter']['make_link'] = TRUE;
  $handler->display->display_options['fields']['name']['alter']['path'] = 'ag_dlios-by-subjects/[tid]/[description_1]';
  $handler->display->display_options['fields']['name']['alter']['word_boundary'] = FALSE;
  $handler->display->display_options['fields']['name']['alter']['ellipsis'] = FALSE;
  $handler->display->display_options['fields']['name']['element_label_colon'] = FALSE;
  /* Field: Taxonomy term: Term description */
  $handler->display->display_options['fields']['description']['id'] = 'description';
  $handler->display->display_options['fields']['description']['table'] = 'taxonomy_term_data';
  $handler->display->display_options['fields']['description']['field'] = 'description';
  $handler->display->display_options['fields']['description']['label'] = '';
  $handler->display->display_options['fields']['description']['alter']['make_link'] = TRUE;
  $handler->display->display_options['fields']['description']['alter']['path'] = 'ag_dlios-by-subjects/[tid]/[description_1]';
  $handler->display->display_options['fields']['description']['element_label_colon'] = FALSE;
  /* Field: COUNT(DISTINCT Content: Nid) */
  $handler->display->display_options['fields']['nid']['id'] = 'nid';
  $handler->display->display_options['fields']['nid']['table'] = 'node';
  $handler->display->display_options['fields']['nid']['field'] = 'nid';
  $handler->display->display_options['fields']['nid']['relationship'] = 'nid';
  $handler->display->display_options['fields']['nid']['group_type'] = 'count_distinct';
  $handler->display->display_options['fields']['nid']['label'] = '';
  $handler->display->display_options['fields']['nid']['alter']['make_link'] = TRUE;
  $handler->display->display_options['fields']['nid']['alter']['path'] = 'ag_dlios-by-subjects/[tid]';
  $handler->display->display_options['fields']['nid']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['nid']['separator'] = '';
  $handler->display->display_options['fields']['nid']['prefix'] = '(';
  $handler->display->display_options['fields']['nid']['suffix'] = ')';
  $handler->display->display_options['path'] = 'ag_vocabularies/agris-categories';
  $handler->display->display_options['menu']['type'] = 'default tab';
  $handler->display->display_options['menu']['title'] = 'Agris Categories';
  $handler->display->display_options['menu']['weight'] = '0';
  $handler->display->display_options['menu']['context'] = 0;
  $handler->display->display_options['tab_options']['weight'] = '0';

  /* Display: Vocabularies - Tab 2  */
  $handler = $view->new_display('page', 'Vocabularies - Tab 2 ', 'page_5');
  $handler->display->display_options['defaults']['title'] = FALSE;
  $handler->display->display_options['title'] = 'Agrovoc ';
  $handler->display->display_options['defaults']['style_plugin'] = FALSE;
  $handler->display->display_options['style_plugin'] = 'grid';
  $handler->display->display_options['style_options']['row_class'] = 'aligntopleft';
  $handler->display->display_options['style_options']['columns'] = '3';
  $handler->display->display_options['defaults']['style_options'] = FALSE;
  $handler->display->display_options['defaults']['row_plugin'] = FALSE;
  $handler->display->display_options['row_plugin'] = 'fields';
  $handler->display->display_options['row_options']['inline'] = array(
    'tid' => 'tid',
    'name' => 'name',
    'description' => 'description',
    'nid' => 'nid',
  );
  $handler->display->display_options['row_options']['separator'] = ' ';
  $handler->display->display_options['row_options']['hide_empty'] = TRUE;
  $handler->display->display_options['defaults']['row_options'] = FALSE;
  $handler->display->display_options['defaults']['header'] = FALSE;
  /* Header: Global: Text area */
  $handler->display->display_options['header']['area']['id'] = 'area';
  $handler->display->display_options['header']['area']['table'] = 'views';
  $handler->display->display_options['header']['area']['field'] = 'area';
  $handler->display->display_options['header']['area']['content'] = 'Here you can browse the list of Agrovoc terms used in the system.
<br />Clicking on a term will display the official description and additional data (relationships, translations) from the official Agrovoc URI of the term.
<br />Clicking on the number in brackets will display resources on the topic identified by the term. 

';
  $handler->display->display_options['header']['area']['format'] = '1';
  $handler->display->display_options['defaults']['relationships'] = FALSE;
  /* Relationship: Taxonomy term: Content with term */
  $handler->display->display_options['relationships']['nid']['id'] = 'nid';
  $handler->display->display_options['relationships']['nid']['table'] = 'taxonomy_index';
  $handler->display->display_options['relationships']['nid']['field'] = 'nid';
  /* Relationship: Taxonomy term: Representative node */
  $handler->display->display_options['relationships']['tid_representative']['id'] = 'tid_representative';
  $handler->display->display_options['relationships']['tid_representative']['table'] = 'taxonomy_term_data';
  $handler->display->display_options['relationships']['tid_representative']['field'] = 'tid_representative';
  $handler->display->display_options['relationships']['tid_representative']['subquery_sort'] = 'field_data_field_ag_agrovoc.delta';
  $handler->display->display_options['relationships']['tid_representative']['subquery_order'] = 'ASC';
  $handler->display->display_options['relationships']['tid_representative']['subquery_view'] = '';
  $handler->display->display_options['relationships']['tid_representative']['subquery_namespace'] = '';
  $handler->display->display_options['defaults']['fields'] = FALSE;
  /* Field: Taxonomy term: Term ID */
  $handler->display->display_options['fields']['tid']['id'] = 'tid';
  $handler->display->display_options['fields']['tid']['table'] = 'taxonomy_term_data';
  $handler->display->display_options['fields']['tid']['field'] = 'tid';
  $handler->display->display_options['fields']['tid']['label'] = '';
  $handler->display->display_options['fields']['tid']['exclude'] = TRUE;
  $handler->display->display_options['fields']['tid']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['tid']['separator'] = '';
  /* Field: Content: Agrovoc terms */
  $handler->display->display_options['fields']['field_ag_agrovoc']['id'] = 'field_ag_agrovoc';
  $handler->display->display_options['fields']['field_ag_agrovoc']['table'] = 'field_data_field_ag_agrovoc';
  $handler->display->display_options['fields']['field_ag_agrovoc']['field'] = 'field_ag_agrovoc';
  $handler->display->display_options['fields']['field_ag_agrovoc']['relationship'] = 'tid_representative';
  $handler->display->display_options['fields']['field_ag_agrovoc']['label'] = '';
  $handler->display->display_options['fields']['field_ag_agrovoc']['exclude'] = TRUE;
  $handler->display->display_options['fields']['field_ag_agrovoc']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['field_ag_agrovoc']['type'] = 'agrovocfield_term_reference_agrovoc_uri';
  $handler->display->display_options['fields']['field_ag_agrovoc']['settings'] = array(
    'only_agid' => 0,
    'strip_html' => 1,
  );
  $handler->display->display_options['fields']['field_ag_agrovoc']['group_column'] = 'entity_id';
  $handler->display->display_options['fields']['field_ag_agrovoc']['delta_offset'] = '0';
  /* Field: Global: PHP */
  $handler->display->display_options['fields']['php']['id'] = 'php';
  $handler->display->display_options['fields']['php']['table'] = 'views';
  $handler->display->display_options['fields']['php']['field'] = 'php';
  $handler->display->display_options['fields']['php']['label'] = '';
  $handler->display->display_options['fields']['php']['exclude'] = TRUE;
  $handler->display->display_options['fields']['php']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['php']['use_php_setup'] = 0;
  $handler->display->display_options['fields']['php']['php_output'] = '<?php
//print_r($data);
for ($i=0;$i < count($data->field_field_ag_agrovoc); $i++) {
  $id = $data->field_field_ag_agrovoc[$i][\'rendered\'][\'#markup\'];
  $tid = $data->field_field_ag_agrovoc[$i][\'raw\'][\'tid\'];
  if ($tid == $row->tid)
       echo $id;
}
?>';
  $handler->display->display_options['fields']['php']['use_php_click_sortable'] = '0';
  $handler->display->display_options['fields']['php']['php_click_sortable'] = '';
  /* Field: Taxonomy term: Name */
  $handler->display->display_options['fields']['name']['id'] = 'name';
  $handler->display->display_options['fields']['name']['table'] = 'taxonomy_term_data';
  $handler->display->display_options['fields']['name']['field'] = 'name';
  $handler->display->display_options['fields']['name']['label'] = '';
  $handler->display->display_options['fields']['name']['alter']['alter_text'] = TRUE;
  $handler->display->display_options['fields']['name']['alter']['text'] = '<a href="http://aims.fao.org/aos/agrovoc/c_[php]" target="_blank">[name]</a>';
  $handler->display->display_options['fields']['name']['alter']['absolute'] = TRUE;
  $handler->display->display_options['fields']['name']['alter']['target'] = '_blank';
  $handler->display->display_options['fields']['name']['element_label_colon'] = FALSE;
  /* Field: COUNT(DISTINCT Content: Nid) */
  $handler->display->display_options['fields']['nid']['id'] = 'nid';
  $handler->display->display_options['fields']['nid']['table'] = 'node';
  $handler->display->display_options['fields']['nid']['field'] = 'nid';
  $handler->display->display_options['fields']['nid']['relationship'] = 'nid';
  $handler->display->display_options['fields']['nid']['group_type'] = 'count_distinct';
  $handler->display->display_options['fields']['nid']['label'] = '';
  $handler->display->display_options['fields']['nid']['alter']['make_link'] = TRUE;
  $handler->display->display_options['fields']['nid']['alter']['path'] = 'ag_dliosbysubjectsagrovoc/[tid]';
  $handler->display->display_options['fields']['nid']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['nid']['separator'] = '';
  $handler->display->display_options['fields']['nid']['prefix'] = '(';
  $handler->display->display_options['fields']['nid']['suffix'] = ')';
  /* Field: Taxonomy term: AGROVOC URI */
  $handler->display->display_options['fields']['field_agrovoc_uri']['id'] = 'field_agrovoc_uri';
  $handler->display->display_options['fields']['field_agrovoc_uri']['table'] = 'field_data_field_agrovoc_uri';
  $handler->display->display_options['fields']['field_agrovoc_uri']['field'] = 'field_agrovoc_uri';
  $handler->display->display_options['fields']['field_agrovoc_uri']['exclude'] = TRUE;
  $handler->display->display_options['defaults']['filter_groups'] = FALSE;
  $handler->display->display_options['defaults']['filters'] = FALSE;
  /* Filter criterion: Taxonomy vocabulary: Machine name */
  $handler->display->display_options['filters']['machine_name']['id'] = 'machine_name';
  $handler->display->display_options['filters']['machine_name']['table'] = 'taxonomy_vocabulary';
  $handler->display->display_options['filters']['machine_name']['field'] = 'machine_name';
  $handler->display->display_options['filters']['machine_name']['value'] = array(
    'vocabulary_16' => 'vocabulary_16',
  );
  $handler->display->display_options['filters']['machine_name']['group'] = 1;
  /* Filter criterion: Taxonomy term: Language */
  $handler->display->display_options['filters']['language']['id'] = 'language';
  $handler->display->display_options['filters']['language']['table'] = 'taxonomy_term_data';
  $handler->display->display_options['filters']['language']['field'] = 'language';
  $handler->display->display_options['filters']['language']['value'] = array(
    '***CURRENT_LANGUAGE***' => '***CURRENT_LANGUAGE***',
    '***DEFAULT_LANGUAGE***' => '***DEFAULT_LANGUAGE***',
    'und' => 'und',
  );
  /* Filter criterion: Content: Type */
  $handler->display->display_options['filters']['type']['id'] = 'type';
  $handler->display->display_options['filters']['type']['table'] = 'node';
  $handler->display->display_options['filters']['type']['field'] = 'type';
  $handler->display->display_options['filters']['type']['relationship'] = 'nid';
  $handler->display->display_options['filters']['type']['value'] = array(
    'agdlios' => 'agdlios',
  );
  /* Filter criterion: Content: Type */
  $handler->display->display_options['filters']['type_1']['id'] = 'type_1';
  $handler->display->display_options['filters']['type_1']['table'] = 'node';
  $handler->display->display_options['filters']['type_1']['field'] = 'type';
  $handler->display->display_options['filters']['type_1']['relationship'] = 'tid_representative';
  $handler->display->display_options['filters']['type_1']['value'] = array(
    'agdlios' => 'agdlios',
  );
  /* Filter criterion: Taxonomy term: Name */
  $handler->display->display_options['filters']['name']['id'] = 'name';
  $handler->display->display_options['filters']['name']['table'] = 'taxonomy_term_data';
  $handler->display->display_options['filters']['name']['field'] = 'name';
  $handler->display->display_options['filters']['name']['operator'] = 'contains';
  $handler->display->display_options['filters']['name']['exposed'] = TRUE;
  $handler->display->display_options['filters']['name']['expose']['operator_id'] = 'name_op';
  $handler->display->display_options['filters']['name']['expose']['label'] = 'Search';
  $handler->display->display_options['filters']['name']['expose']['operator'] = 'name_op';
  $handler->display->display_options['filters']['name']['expose']['identifier'] = 'name';
  $handler->display->display_options['path'] = 'ag_vocabularies/agrovoc';
  $handler->display->display_options['menu']['type'] = 'tab';
  $handler->display->display_options['menu']['title'] = 'Agrovoc';
  $handler->display->display_options['menu']['weight'] = '0';
  $handler->display->display_options['menu']['context'] = 0;
  $export['browse_category2'] = $view;

  $view = new view();
  $view->name = 'datos_de_usuarios';
  $view->description = 'datos personales de los usuarios activos ';
  $view->tag = 'default';
  $view->base_table = 'users';
  $view->human_name = 'Datos de usuarios';
  $view->core = 7;
  $view->api_version = '3.0';
  $view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */

  /* Display: Master */
  $handler = $view->new_display('default', 'Master', 'default');
  $handler->display->display_options['title'] = 'Users';
  $handler->display->display_options['use_more_always'] = FALSE;
  $handler->display->display_options['use_more_text'] = 'más';
  $handler->display->display_options['access']['type'] = 'perm';
  $handler->display->display_options['access']['perm'] = 'access user profiles';
  $handler->display->display_options['cache']['type'] = 'none';
  $handler->display->display_options['query']['type'] = 'views_query';
  $handler->display->display_options['query']['options']['query_comment'] = FALSE;
  $handler->display->display_options['exposed_form']['type'] = 'basic';
  $handler->display->display_options['exposed_form']['options']['reset_button_label'] = 'Reiniciar';
  $handler->display->display_options['exposed_form']['options']['exposed_sorts_label'] = 'Ordenar por';
  $handler->display->display_options['exposed_form']['options']['sort_asc_label'] = 'Ascendente';
  $handler->display->display_options['pager']['type'] = 'full';
  $handler->display->display_options['pager']['options']['items_per_page'] = '10';
  $handler->display->display_options['pager']['options']['expose']['items_per_page_label'] = 'Elementos por página';
  $handler->display->display_options['pager']['options']['expose']['items_per_page_options_all_label'] = '- Todos -';
  $handler->display->display_options['pager']['options']['expose']['offset_label'] = 'Desplazamiento';
  $handler->display->display_options['style_plugin'] = 'table';
  /* Field: User: Name */
  $handler->display->display_options['fields']['name']['id'] = 'name';
  $handler->display->display_options['fields']['name']['table'] = 'users';
  $handler->display->display_options['fields']['name']['field'] = 'name';
  $handler->display->display_options['fields']['name']['label'] = 'User name';
  $handler->display->display_options['fields']['name']['alter']['word_boundary'] = FALSE;
  $handler->display->display_options['fields']['name']['alter']['ellipsis'] = FALSE;
  /* Field: User: Created date */
  $handler->display->display_options['fields']['created']['id'] = 'created';
  $handler->display->display_options['fields']['created']['table'] = 'users';
  $handler->display->display_options['fields']['created']['field'] = 'created';
  $handler->display->display_options['fields']['created']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['created']['date_format'] = 'custom';
  $handler->display->display_options['fields']['created']['custom_date_format'] = 'd/m/Y';
  /* Field: User: E-mail */
  $handler->display->display_options['fields']['mail']['id'] = 'mail';
  $handler->display->display_options['fields']['mail']['table'] = 'users';
  $handler->display->display_options['fields']['mail']['field'] = 'mail';
  $handler->display->display_options['fields']['mail']['element_label_colon'] = FALSE;
  /* Field: User: Last login */
  $handler->display->display_options['fields']['login']['id'] = 'login';
  $handler->display->display_options['fields']['login']['table'] = 'users';
  $handler->display->display_options['fields']['login']['field'] = 'login';
  $handler->display->display_options['fields']['login']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['login']['date_format'] = 'short';
  /* Field: User: Roles */
  $handler->display->display_options['fields']['rid']['id'] = 'rid';
  $handler->display->display_options['fields']['rid']['table'] = 'users_roles';
  $handler->display->display_options['fields']['rid']['field'] = 'rid';
  $handler->display->display_options['fields']['rid']['label'] = 'user role';
  $handler->display->display_options['fields']['rid']['element_label_colon'] = FALSE;
  /* Sort criterion: User: Created date */
  $handler->display->display_options['sorts']['created']['id'] = 'created';
  $handler->display->display_options['sorts']['created']['table'] = 'users';
  $handler->display->display_options['sorts']['created']['field'] = 'created';
  $handler->display->display_options['sorts']['created']['order'] = 'DESC';
  /* Filter criterion: User: Active */
  $handler->display->display_options['filters']['status']['id'] = 'status';
  $handler->display->display_options['filters']['status']['table'] = 'users';
  $handler->display->display_options['filters']['status']['field'] = 'status';
  $handler->display->display_options['filters']['status']['value'] = '1';
  $handler->display->display_options['filters']['status']['group'] = 1;
  $handler->display->display_options['filters']['status']['expose']['operator_id'] = '';
  $handler->display->display_options['filters']['status']['expose']['label'] = 'Activo';
  $handler->display->display_options['filters']['status']['expose']['operator'] = 'status_op';
  $handler->display->display_options['filters']['status']['expose']['identifier'] = 'status';

  /* Display: Page */
  $handler = $view->new_display('page', 'Page', 'page');
  $handler->display->display_options['path'] = 'usersdata';
  $handler->display->display_options['menu']['type'] = 'normal';
  $handler->display->display_options['menu']['title'] = 'Users';
  $handler->display->display_options['menu']['weight'] = '6';
  $handler->display->display_options['menu']['name'] = 'menu-access';
  $handler->display->display_options['menu']['context'] = 0;

  /* Display: Block */
  $handler = $view->new_display('block', 'Block', 'block_1');
  $handler->display->display_options['defaults']['group_by'] = FALSE;
  $handler->display->display_options['group_by'] = TRUE;
  $handler->display->display_options['defaults']['style_plugin'] = FALSE;
  $handler->display->display_options['style_plugin'] = 'default';
  $handler->display->display_options['defaults']['style_options'] = FALSE;
  $handler->display->display_options['defaults']['row_plugin'] = FALSE;
  $handler->display->display_options['row_plugin'] = 'fields';
  $handler->display->display_options['defaults']['row_options'] = FALSE;
  $handler->display->display_options['defaults']['fields'] = FALSE;
  /* Field: User: Uid */
  $handler->display->display_options['fields']['uid']['id'] = 'uid';
  $handler->display->display_options['fields']['uid']['table'] = 'users';
  $handler->display->display_options['fields']['uid']['field'] = 'uid';
  $handler->display->display_options['fields']['uid']['group_type'] = 'count';
  $handler->display->display_options['fields']['uid']['label'] = 'Active users';
  $handler->display->display_options['fields']['uid']['element_label_colon'] = FALSE;
  $handler->display->display_options['defaults']['sorts'] = FALSE;
  $handler->display->display_options['block_description'] = 'statistica 5';
  $export['datos_de_usuarios'] = $view;

  $view = new view();
  $view->name = 'datos_estad_sticos_documentos';
  $view->description = '';
  $view->tag = 'default';
  $view->base_table = 'node';
  $view->human_name = 'Datos Estadísticos documentos';
  $view->core = 7;
  $view->api_version = '3.0';
  $view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */

  /* Display: Master */
  $handler = $view->new_display('default', 'Master', 'default');
  $handler->display->display_options['title'] = 'Datos Estadísticos documentos';
  $handler->display->display_options['use_more_always'] = FALSE;
  $handler->display->display_options['use_more_text'] = 'más';
  $handler->display->display_options['group_by'] = TRUE;
  $handler->display->display_options['access']['type'] = 'perm';
  $handler->display->display_options['cache']['type'] = 'none';
  $handler->display->display_options['query']['type'] = 'views_query';
  $handler->display->display_options['query']['options']['query_comment'] = FALSE;
  $handler->display->display_options['exposed_form']['type'] = 'basic';
  $handler->display->display_options['exposed_form']['options']['reset_button_label'] = 'Reiniciar';
  $handler->display->display_options['exposed_form']['options']['exposed_sorts_label'] = 'Ordenar por';
  $handler->display->display_options['exposed_form']['options']['sort_asc_label'] = 'Ascendente';
  $handler->display->display_options['pager']['type'] = 'none';
  $handler->display->display_options['pager']['options']['offset'] = '0';
  $handler->display->display_options['style_plugin'] = 'default';
  $handler->display->display_options['row_plugin'] = 'fields';
  /* Field: COUNT(DISTINCT Content: Nid) */
  $handler->display->display_options['fields']['nid']['id'] = 'nid';
  $handler->display->display_options['fields']['nid']['table'] = 'node';
  $handler->display->display_options['fields']['nid']['field'] = 'nid';
  $handler->display->display_options['fields']['nid']['group_type'] = 'count_distinct';
  $handler->display->display_options['fields']['nid']['label'] = 'Cantidad de registros dados de alta';
  $handler->display->display_options['fields']['nid']['element_label_colon'] = FALSE;
  /* Filter criterion: Content: Type */
  $handler->display->display_options['filters']['type']['id'] = 'type';
  $handler->display->display_options['filters']['type']['table'] = 'node';
  $handler->display->display_options['filters']['type']['field'] = 'type';
  $handler->display->display_options['filters']['type']['value'] = array(
    'agdlios' => 'agdlios',
  );
  $handler->display->display_options['filters']['type']['group'] = 1;

  /* Display: Page */
  $handler = $view->new_display('page', 'Page', 'page');
  $handler->display->display_options['defaults']['title'] = FALSE;
  $handler->display->display_options['title'] = 'Published documents';
  $handler->display->display_options['defaults']['fields'] = FALSE;
  /* Field: COUNT(DISTINCT Content: Nid) */
  $handler->display->display_options['fields']['nid']['id'] = 'nid';
  $handler->display->display_options['fields']['nid']['table'] = 'node';
  $handler->display->display_options['fields']['nid']['field'] = 'nid';
  $handler->display->display_options['fields']['nid']['group_type'] = 'count_distinct';
  $handler->display->display_options['fields']['nid']['label'] = 'Published documents';
  $handler->display->display_options['fields']['nid']['element_label_colon'] = FALSE;
  $handler->display->display_options['path'] = 'documentsstat';

  /* Display: Block */
  $handler = $view->new_display('block', 'Block', 'block');
  $handler->display->display_options['defaults']['pager'] = FALSE;
  $handler->display->display_options['pager']['type'] = 'some';
  $handler->display->display_options['pager']['options']['items_per_page'] = '5';
  $handler->display->display_options['defaults']['fields'] = FALSE;
  /* Field: COUNT(DISTINCT Content: Nid) */
  $handler->display->display_options['fields']['nid']['id'] = 'nid';
  $handler->display->display_options['fields']['nid']['table'] = 'node';
  $handler->display->display_options['fields']['nid']['field'] = 'nid';
  $handler->display->display_options['fields']['nid']['group_type'] = 'count_distinct';
  $handler->display->display_options['fields']['nid']['label'] = 'Created documents';
  $handler->display->display_options['fields']['nid']['element_label_colon'] = FALSE;
  $handler->display->display_options['defaults']['sorts'] = FALSE;
  $handler->display->display_options['block_description'] = 'statistica 1';

  /* Display: Block */
  $handler = $view->new_display('block', 'Block', 'block_1');
  $handler->display->display_options['defaults']['fields'] = FALSE;
  /* Field: COUNT(DISTINCT Content: Nid) */
  $handler->display->display_options['fields']['nid']['id'] = 'nid';
  $handler->display->display_options['fields']['nid']['table'] = 'node';
  $handler->display->display_options['fields']['nid']['field'] = 'nid';
  $handler->display->display_options['fields']['nid']['group_type'] = 'count_distinct';
  $handler->display->display_options['fields']['nid']['label'] = 'Published documents';
  $handler->display->display_options['fields']['nid']['element_label_colon'] = FALSE;
  $handler->display->display_options['defaults']['filter_groups'] = FALSE;
  $handler->display->display_options['defaults']['filters'] = FALSE;
  /* Filter criterion: Content: Type */
  $handler->display->display_options['filters']['type']['id'] = 'type';
  $handler->display->display_options['filters']['type']['table'] = 'node';
  $handler->display->display_options['filters']['type']['field'] = 'type';
  $handler->display->display_options['filters']['type']['value'] = array(
    'agdlios' => 'agdlios',
  );
  $handler->display->display_options['filters']['type']['group'] = 1;
  /* Filter criterion: Content: Published */
  $handler->display->display_options['filters']['status']['id'] = 'status';
  $handler->display->display_options['filters']['status']['table'] = 'node';
  $handler->display->display_options['filters']['status']['field'] = 'status';
  $handler->display->display_options['filters']['status']['value'] = '1';
  $handler->display->display_options['block_description'] = 'statistica 2';

  /* Display: Block 2 */
  $handler = $view->new_display('block', 'Block 2', 'block_2');
  $handler->display->display_options['defaults']['fields'] = FALSE;
  /* Field: COUNT(DISTINCT Content: Nid) */
  $handler->display->display_options['fields']['nid']['id'] = 'nid';
  $handler->display->display_options['fields']['nid']['table'] = 'node';
  $handler->display->display_options['fields']['nid']['field'] = 'nid';
  $handler->display->display_options['fields']['nid']['group_type'] = 'count_distinct';
  $handler->display->display_options['fields']['nid']['label'] = 'Unpublished documents';
  $handler->display->display_options['fields']['nid']['element_label_colon'] = FALSE;
  $handler->display->display_options['defaults']['filter_groups'] = FALSE;
  $handler->display->display_options['defaults']['filters'] = FALSE;
  /* Filter criterion: Content: Type */
  $handler->display->display_options['filters']['type']['id'] = 'type';
  $handler->display->display_options['filters']['type']['table'] = 'node';
  $handler->display->display_options['filters']['type']['field'] = 'type';
  $handler->display->display_options['filters']['type']['value'] = array(
    'agdlios' => 'agdlios',
  );
  $handler->display->display_options['filters']['type']['group'] = 1;
  /* Filter criterion: Content: Published */
  $handler->display->display_options['filters']['status']['id'] = 'status';
  $handler->display->display_options['filters']['status']['table'] = 'node';
  $handler->display->display_options['filters']['status']['field'] = 'status';
  $handler->display->display_options['filters']['status']['value'] = '0';
  $handler->display->display_options['block_description'] = 'statistica 3';

  /* Display: Block 3 */
  $handler = $view->new_display('block', 'Block 3', 'block_3');
  $handler->display->display_options['defaults']['group_by'] = FALSE;
  $handler->display->display_options['group_by'] = TRUE;
  $handler->display->display_options['defaults']['query'] = FALSE;
  $handler->display->display_options['query']['type'] = 'views_query';
  $handler->display->display_options['query']['options']['distinct'] = TRUE;
  $handler->display->display_options['query']['options']['query_comment'] = FALSE;
  $handler->display->display_options['defaults']['header'] = FALSE;
  /* Header: Global: PHP */
  $handler->display->display_options['header']['php']['id'] = 'php';
  $handler->display->display_options['header']['php']['table'] = 'views';
  $handler->display->display_options['header']['php']['field'] = 'php';
  $handler->display->display_options['header']['php']['php_output'] = '<?php $tot=0;
         foreach ($results as $res){
              
             $tot=$tot+$res->node_counter_totalcount;
         }
  print "Total views: ". $tot;
?>
';
  $handler->display->display_options['defaults']['fields'] = FALSE;
  /* Field: SUM(Content statistics: Total views) */
  $handler->display->display_options['fields']['totalcount']['id'] = 'totalcount';
  $handler->display->display_options['fields']['totalcount']['table'] = 'node_counter';
  $handler->display->display_options['fields']['totalcount']['field'] = 'totalcount';
  $handler->display->display_options['fields']['totalcount']['group_type'] = 'sum';
  $handler->display->display_options['fields']['totalcount']['label'] = '';
  $handler->display->display_options['fields']['totalcount']['alter']['alter_text'] = TRUE;
  $handler->display->display_options['fields']['totalcount']['alter']['text'] = '  ';
  $handler->display->display_options['fields']['totalcount']['element_label_colon'] = FALSE;
  $handler->display->display_options['block_description'] = 'statistica 4';
  $export['datos_estad_sticos_documentos'] = $view;

  $view = new view();
  $view->name = 'registros_dados_de_alta_';
  $view->description = 'Muestra los registros que se dieron de alta';
  $view->tag = 'default';
  $view->base_table = 'node';
  $view->human_name = 'Registros dados de alta ';
  $view->core = 7;
  $view->api_version = '3.0';
  $view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */

  /* Display: Master */
  $handler = $view->new_display('default', 'Master', 'default');
  $handler->display->display_options['title'] = 'Published documents';
  $handler->display->display_options['use_more_always'] = FALSE;
  $handler->display->display_options['use_more_text'] = 'más';
  $handler->display->display_options['access']['type'] = 'perm';
  $handler->display->display_options['cache']['type'] = 'none';
  $handler->display->display_options['query']['type'] = 'views_query';
  $handler->display->display_options['query']['options']['query_comment'] = FALSE;
  $handler->display->display_options['exposed_form']['type'] = 'basic';
  $handler->display->display_options['exposed_form']['options']['reset_button_label'] = 'Reiniciar';
  $handler->display->display_options['exposed_form']['options']['exposed_sorts_label'] = 'Ordenar por';
  $handler->display->display_options['exposed_form']['options']['sort_asc_label'] = 'Ascendente';
  $handler->display->display_options['pager']['type'] = 'full';
  $handler->display->display_options['pager']['options']['items_per_page'] = '10';
  $handler->display->display_options['pager']['options']['expose']['items_per_page_label'] = 'Elementos por página';
  $handler->display->display_options['pager']['options']['expose']['items_per_page_options_all_label'] = '- Todos -';
  $handler->display->display_options['pager']['options']['expose']['offset_label'] = 'Desplazamiento';
  $handler->display->display_options['style_plugin'] = 'table';
  /* Relationship: Content: Author */
  $handler->display->display_options['relationships']['uid']['id'] = 'uid';
  $handler->display->display_options['relationships']['uid']['table'] = 'node';
  $handler->display->display_options['relationships']['uid']['field'] = 'uid';
  /* Field: Content: Title */
  $handler->display->display_options['fields']['title']['id'] = 'title';
  $handler->display->display_options['fields']['title']['table'] = 'node';
  $handler->display->display_options['fields']['title']['field'] = 'title';
  $handler->display->display_options['fields']['title']['alter']['word_boundary'] = FALSE;
  $handler->display->display_options['fields']['title']['alter']['ellipsis'] = FALSE;
  /* Field: User: Name */
  $handler->display->display_options['fields']['name']['id'] = 'name';
  $handler->display->display_options['fields']['name']['table'] = 'users';
  $handler->display->display_options['fields']['name']['field'] = 'name';
  $handler->display->display_options['fields']['name']['relationship'] = 'uid';
  $handler->display->display_options['fields']['name']['label'] = 'Author';
  /* Field: Content: Post date */
  $handler->display->display_options['fields']['created']['id'] = 'created';
  $handler->display->display_options['fields']['created']['table'] = 'node';
  $handler->display->display_options['fields']['created']['field'] = 'created';
  $handler->display->display_options['fields']['created']['label'] = 'Creation date';
  $handler->display->display_options['fields']['created']['date_format'] = 'custom';
  $handler->display->display_options['fields']['created']['custom_date_format'] = 'd/m/Y';
  /* Sort criterion: Content: Post date */
  $handler->display->display_options['sorts']['created']['id'] = 'created';
  $handler->display->display_options['sorts']['created']['table'] = 'node';
  $handler->display->display_options['sorts']['created']['field'] = 'created';
  $handler->display->display_options['sorts']['created']['order'] = 'DESC';
  /* Filter criterion: Content: Published */
  $handler->display->display_options['filters']['status']['id'] = 'status';
  $handler->display->display_options['filters']['status']['table'] = 'node';
  $handler->display->display_options['filters']['status']['field'] = 'status';
  $handler->display->display_options['filters']['status']['value'] = 1;
  $handler->display->display_options['filters']['status']['group'] = 1;
  $handler->display->display_options['filters']['status']['expose']['operator'] = FALSE;
  /* Filter criterion: Content: Type */
  $handler->display->display_options['filters']['type']['id'] = 'type';
  $handler->display->display_options['filters']['type']['table'] = 'node';
  $handler->display->display_options['filters']['type']['field'] = 'type';
  $handler->display->display_options['filters']['type']['value'] = array(
    'agdlios' => 'agdlios',
  );

  /* Display: Page */
  $handler = $view->new_display('page', 'Page', 'page');
  $handler->display->display_options['path'] = 'published_documents';
  $handler->display->display_options['menu']['type'] = 'normal';
  $handler->display->display_options['menu']['title'] = 'Published documents';
  $handler->display->display_options['menu']['weight'] = '5';
  $handler->display->display_options['menu']['name'] = 'menu-access';
  $handler->display->display_options['menu']['context'] = 0;
  $export['registros_dados_de_alta_'] = $view;

  $view = new view();
  $view->name = 'reporte_de_existencias';
  $view->description = 'Lista el total de la colección digital que se encuentra cargada y registrada en el repositorio';
  $view->tag = 'default';
  $view->base_table = 'node';
  $view->human_name = 'Reporte de existencias';
  $view->core = 7;
  $view->api_version = '3.0';
  $view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */

  /* Display: Master */
  $handler = $view->new_display('default', 'Master', 'default');
  $handler->display->display_options['title'] = 'Reporte de existencias';
  $handler->display->display_options['use_more_always'] = FALSE;
  $handler->display->display_options['use_more_text'] = 'más';
  $handler->display->display_options['access']['type'] = 'perm';
  $handler->display->display_options['cache']['type'] = 'none';
  $handler->display->display_options['query']['type'] = 'views_query';
  $handler->display->display_options['query']['options']['distinct'] = TRUE;
  $handler->display->display_options['query']['options']['query_comment'] = FALSE;
  $handler->display->display_options['exposed_form']['type'] = 'basic';
  $handler->display->display_options['exposed_form']['options']['reset_button_label'] = 'Reiniciar';
  $handler->display->display_options['exposed_form']['options']['exposed_sorts_label'] = 'Ordenar por';
  $handler->display->display_options['exposed_form']['options']['sort_asc_label'] = 'Ascendente';
  $handler->display->display_options['pager']['type'] = 'full';
  $handler->display->display_options['pager']['options']['items_per_page'] = '30';
  $handler->display->display_options['pager']['options']['offset'] = '0';
  $handler->display->display_options['pager']['options']['id'] = '0';
  $handler->display->display_options['style_plugin'] = 'table';
  $handler->display->display_options['style_options']['columns'] = array(
    'title' => 'title',
    'field_ag_author' => 'field_ag_author',
    'field_ag_type' => 'field_ag_type',
    'created' => 'created',
  );
  $handler->display->display_options['style_options']['default'] = 'title';
  $handler->display->display_options['style_options']['info'] = array(
    'title' => array(
      'sortable' => 0,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'field_ag_author' => array(
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'field_ag_type' => array(
      'sortable' => 0,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'created' => array(
      'sortable' => 0,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
  );
  /* Header: Global: Result summary */
  $handler->display->display_options['header']['result']['id'] = 'result';
  $handler->display->display_options['header']['result']['table'] = 'views';
  $handler->display->display_options['header']['result']['field'] = 'result';
  $handler->display->display_options['header']['result']['content'] = '@end displayed resources in this page.
Tot.items in this collection : @total.';
  /* Relationship: Content: Author */
  $handler->display->display_options['relationships']['uid']['id'] = 'uid';
  $handler->display->display_options['relationships']['uid']['table'] = 'node';
  $handler->display->display_options['relationships']['uid']['field'] = 'uid';
  /* Field: Content: Title */
  $handler->display->display_options['fields']['title']['id'] = 'title';
  $handler->display->display_options['fields']['title']['table'] = 'node';
  $handler->display->display_options['fields']['title']['field'] = 'title';
  $handler->display->display_options['fields']['title']['label'] = 'Titulo';
  $handler->display->display_options['fields']['title']['alter']['word_boundary'] = FALSE;
  $handler->display->display_options['fields']['title']['alter']['ellipsis'] = FALSE;
  $handler->display->display_options['fields']['title']['element_label_colon'] = FALSE;
  /* Field: Content: Author(s) */
  $handler->display->display_options['fields']['field_ag_author']['id'] = 'field_ag_author';
  $handler->display->display_options['fields']['field_ag_author']['table'] = 'field_data_field_ag_author';
  $handler->display->display_options['fields']['field_ag_author']['field'] = 'field_ag_author';
  $handler->display->display_options['fields']['field_ag_author']['label'] = 'Autor';
  $handler->display->display_options['fields']['field_ag_author']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['field_ag_author']['delta_offset'] = '0';
  /* Field: Content: Document type */
  $handler->display->display_options['fields']['field_ag_type']['id'] = 'field_ag_type';
  $handler->display->display_options['fields']['field_ag_type']['table'] = 'field_data_field_ag_type';
  $handler->display->display_options['fields']['field_ag_type']['field'] = 'field_ag_type';
  $handler->display->display_options['fields']['field_ag_type']['label'] = 'Tipo';
  $handler->display->display_options['fields']['field_ag_type']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['field_ag_type']['type'] = 'taxonomy_term_reference_plain';
  /* Field: Content: File attachments */
  $handler->display->display_options['fields']['upload']['id'] = 'upload';
  $handler->display->display_options['fields']['upload']['table'] = 'field_data_upload';
  $handler->display->display_options['fields']['upload']['field'] = 'upload';
  $handler->display->display_options['fields']['upload']['label'] = 'Nombre de archivo';
  $handler->display->display_options['fields']['upload']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['upload']['click_sort_column'] = 'fid';
  $handler->display->display_options['fields']['upload']['type'] = 'file_url_plain';
  $handler->display->display_options['fields']['upload']['delta_offset'] = '0';
  /* Field: User: Name */
  $handler->display->display_options['fields']['name']['id'] = 'name';
  $handler->display->display_options['fields']['name']['table'] = 'users';
  $handler->display->display_options['fields']['name']['field'] = 'name';
  $handler->display->display_options['fields']['name']['relationship'] = 'uid';
  $handler->display->display_options['fields']['name']['label'] = 'Usuario';
  $handler->display->display_options['fields']['name']['element_label_colon'] = FALSE;
  /* Field: Content: Post date */
  $handler->display->display_options['fields']['created']['id'] = 'created';
  $handler->display->display_options['fields']['created']['table'] = 'node';
  $handler->display->display_options['fields']['created']['field'] = 'created';
  $handler->display->display_options['fields']['created']['label'] = 'Fecha de carga';
  $handler->display->display_options['fields']['created']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['created']['date_format'] = 'custom';
  $handler->display->display_options['fields']['created']['custom_date_format'] = 'd/m/Y';
  /* Field: User: Roles */
  $handler->display->display_options['fields']['rid']['id'] = 'rid';
  $handler->display->display_options['fields']['rid']['table'] = 'users_roles';
  $handler->display->display_options['fields']['rid']['field'] = 'rid';
  $handler->display->display_options['fields']['rid']['relationship'] = 'uid';
  /* Sort criterion: Content: Post date */
  $handler->display->display_options['sorts']['created']['id'] = 'created';
  $handler->display->display_options['sorts']['created']['table'] = 'node';
  $handler->display->display_options['sorts']['created']['field'] = 'created';
  $handler->display->display_options['sorts']['created']['order'] = 'DESC';
  /* Filter criterion: Content: Published */
  $handler->display->display_options['filters']['status']['id'] = 'status';
  $handler->display->display_options['filters']['status']['table'] = 'node';
  $handler->display->display_options['filters']['status']['field'] = 'status';
  $handler->display->display_options['filters']['status']['value'] = 1;
  $handler->display->display_options['filters']['status']['group'] = 1;
  $handler->display->display_options['filters']['status']['expose']['operator'] = FALSE;
  /* Filter criterion: Content: Type */
  $handler->display->display_options['filters']['type']['id'] = 'type';
  $handler->display->display_options['filters']['type']['table'] = 'node';
  $handler->display->display_options['filters']['type']['field'] = 'type';
  $handler->display->display_options['filters']['type']['value'] = array(
    'agdlios' => 'agdlios',
  );
  /* Filter criterion: Content statistics: Most recent view */
  $handler->display->display_options['filters']['timestamp']['id'] = 'timestamp';
  $handler->display->display_options['filters']['timestamp']['table'] = 'node_counter';
  $handler->display->display_options['filters']['timestamp']['field'] = 'timestamp';
  $handler->display->display_options['filters']['timestamp']['exposed'] = TRUE;
  $handler->display->display_options['filters']['timestamp']['expose']['operator_id'] = 'timestamp_op';
  $handler->display->display_options['filters']['timestamp']['expose']['label'] = 'Most recent view';
  $handler->display->display_options['filters']['timestamp']['expose']['operator'] = 'timestamp_op';
  $handler->display->display_options['filters']['timestamp']['expose']['identifier'] = 'timestamp';

  /* Display: Page */
  $handler = $view->new_display('page', 'Page', 'page');
  $handler->display->display_options['defaults']['title'] = FALSE;
  $handler->display->display_options['title'] = 'Documents report';
  $handler->display->display_options['defaults']['header'] = FALSE;
  /* Header: Global: Result summary */
  $handler->display->display_options['header']['result']['id'] = 'result';
  $handler->display->display_options['header']['result']['table'] = 'views';
  $handler->display->display_options['header']['result']['field'] = 'result';
  $handler->display->display_options['header']['result']['content'] = '@end records displayed.
Total number of collection : @total.';
  $handler->display->display_options['defaults']['fields'] = FALSE;
  /* Field: Content: Title */
  $handler->display->display_options['fields']['title']['id'] = 'title';
  $handler->display->display_options['fields']['title']['table'] = 'node';
  $handler->display->display_options['fields']['title']['field'] = 'title';
  $handler->display->display_options['fields']['title']['alter']['word_boundary'] = FALSE;
  $handler->display->display_options['fields']['title']['alter']['ellipsis'] = FALSE;
  $handler->display->display_options['fields']['title']['element_label_colon'] = FALSE;
  /* Field: Content: Author(s) */
  $handler->display->display_options['fields']['field_ag_author']['id'] = 'field_ag_author';
  $handler->display->display_options['fields']['field_ag_author']['table'] = 'field_data_field_ag_author';
  $handler->display->display_options['fields']['field_ag_author']['field'] = 'field_ag_author';
  $handler->display->display_options['fields']['field_ag_author']['label'] = 'Author';
  $handler->display->display_options['fields']['field_ag_author']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['field_ag_author']['delta_offset'] = '0';
  /* Field: Content: Document type */
  $handler->display->display_options['fields']['field_ag_type']['id'] = 'field_ag_type';
  $handler->display->display_options['fields']['field_ag_type']['table'] = 'field_data_field_ag_type';
  $handler->display->display_options['fields']['field_ag_type']['field'] = 'field_ag_type';
  $handler->display->display_options['fields']['field_ag_type']['label'] = 'Type';
  $handler->display->display_options['fields']['field_ag_type']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['field_ag_type']['type'] = 'taxonomy_term_reference_plain';
  /* Field: Content: File attachments */
  $handler->display->display_options['fields']['upload']['id'] = 'upload';
  $handler->display->display_options['fields']['upload']['table'] = 'field_data_upload';
  $handler->display->display_options['fields']['upload']['field'] = 'upload';
  $handler->display->display_options['fields']['upload']['label'] = 'Source name';
  $handler->display->display_options['fields']['upload']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['upload']['click_sort_column'] = 'fid';
  $handler->display->display_options['fields']['upload']['type'] = 'file_url_plain';
  $handler->display->display_options['fields']['upload']['delta_offset'] = '0';
  /* Field: User: Name */
  $handler->display->display_options['fields']['name']['id'] = 'name';
  $handler->display->display_options['fields']['name']['table'] = 'users';
  $handler->display->display_options['fields']['name']['field'] = 'name';
  $handler->display->display_options['fields']['name']['relationship'] = 'uid';
  $handler->display->display_options['fields']['name']['label'] = 'User';
  $handler->display->display_options['fields']['name']['element_label_colon'] = FALSE;
  /* Field: Content: Post date */
  $handler->display->display_options['fields']['created']['id'] = 'created';
  $handler->display->display_options['fields']['created']['table'] = 'node';
  $handler->display->display_options['fields']['created']['field'] = 'created';
  $handler->display->display_options['fields']['created']['label'] = 'Creation date';
  $handler->display->display_options['fields']['created']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['created']['date_format'] = 'custom';
  $handler->display->display_options['fields']['created']['custom_date_format'] = 'd/m/Y';
  $handler->display->display_options['defaults']['filter_groups'] = FALSE;
  $handler->display->display_options['defaults']['filters'] = FALSE;
  /* Filter criterion: Content: Published */
  $handler->display->display_options['filters']['status']['id'] = 'status';
  $handler->display->display_options['filters']['status']['table'] = 'node';
  $handler->display->display_options['filters']['status']['field'] = 'status';
  $handler->display->display_options['filters']['status']['value'] = 1;
  $handler->display->display_options['filters']['status']['group'] = 1;
  $handler->display->display_options['filters']['status']['expose']['operator'] = FALSE;
  /* Filter criterion: Content: Type */
  $handler->display->display_options['filters']['type']['id'] = 'type';
  $handler->display->display_options['filters']['type']['table'] = 'node';
  $handler->display->display_options['filters']['type']['field'] = 'type';
  $handler->display->display_options['filters']['type']['value'] = array(
    'agdlios' => 'agdlios',
  );
  $handler->display->display_options['path'] = 'documents-reports';
  $handler->display->display_options['menu']['type'] = 'normal';
  $handler->display->display_options['menu']['title'] = 'Documents report';
  $handler->display->display_options['menu']['weight'] = '0';
  $handler->display->display_options['menu']['name'] = 'menu-access';
  $handler->display->display_options['menu']['context'] = 0;

  /* Display: Page */
  $handler = $view->new_display('page', 'Page', 'page_1');
  $handler->display->display_options['defaults']['title'] = FALSE;
  $handler->display->display_options['title'] = 'Most views resources';
  $handler->display->display_options['defaults']['header'] = FALSE;
  /* Header: Global: Text area */
  $handler->display->display_options['header']['area']['id'] = 'area';
  $handler->display->display_options['header']['area']['table'] = 'views';
  $handler->display->display_options['header']['area']['field'] = 'area';
  $handler->display->display_options['header']['area']['content'] = 'List of view resources in a range of dates.';
  $handler->display->display_options['header']['area']['format'] = '1';
  /* Header: Global: Result summary */
  $handler->display->display_options['header']['result']['id'] = 'result';
  $handler->display->display_options['header']['result']['table'] = 'views';
  $handler->display->display_options['header']['result']['field'] = 'result';
  $handler->display->display_options['header']['result']['content'] = '@end resources displayed in this page.';
  $handler->display->display_options['defaults']['relationships'] = FALSE;
  $handler->display->display_options['defaults']['fields'] = FALSE;
  /* Field: Content: Title */
  $handler->display->display_options['fields']['title']['id'] = 'title';
  $handler->display->display_options['fields']['title']['table'] = 'node';
  $handler->display->display_options['fields']['title']['field'] = 'title';
  $handler->display->display_options['fields']['title']['alter']['word_boundary'] = FALSE;
  $handler->display->display_options['fields']['title']['alter']['ellipsis'] = FALSE;
  $handler->display->display_options['fields']['title']['element_label_colon'] = FALSE;
  /* Field: Content: Author(s) */
  $handler->display->display_options['fields']['field_ag_author']['id'] = 'field_ag_author';
  $handler->display->display_options['fields']['field_ag_author']['table'] = 'field_data_field_ag_author';
  $handler->display->display_options['fields']['field_ag_author']['field'] = 'field_ag_author';
  $handler->display->display_options['fields']['field_ag_author']['label'] = 'Author';
  $handler->display->display_options['fields']['field_ag_author']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['field_ag_author']['delta_offset'] = '0';
  /* Field: Content: Document type */
  $handler->display->display_options['fields']['field_ag_type']['id'] = 'field_ag_type';
  $handler->display->display_options['fields']['field_ag_type']['table'] = 'field_data_field_ag_type';
  $handler->display->display_options['fields']['field_ag_type']['field'] = 'field_ag_type';
  $handler->display->display_options['fields']['field_ag_type']['label'] = 'Type';
  $handler->display->display_options['fields']['field_ag_type']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['field_ag_type']['type'] = 'taxonomy_term_reference_plain';
  /* Field: Content: File attachments */
  $handler->display->display_options['fields']['upload']['id'] = 'upload';
  $handler->display->display_options['fields']['upload']['table'] = 'field_data_upload';
  $handler->display->display_options['fields']['upload']['field'] = 'upload';
  $handler->display->display_options['fields']['upload']['label'] = 'Source name';
  $handler->display->display_options['fields']['upload']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['upload']['click_sort_column'] = 'fid';
  $handler->display->display_options['fields']['upload']['type'] = 'file_url_plain';
  $handler->display->display_options['fields']['upload']['delta_offset'] = '0';
  /* Field: Content statistics: Most recent view */
  $handler->display->display_options['fields']['timestamp']['id'] = 'timestamp';
  $handler->display->display_options['fields']['timestamp']['table'] = 'node_counter';
  $handler->display->display_options['fields']['timestamp']['field'] = 'timestamp';
  $handler->display->display_options['fields']['timestamp']['label'] = 'Last view date';
  $handler->display->display_options['fields']['timestamp']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['timestamp']['date_format'] = 'long';
  /* Field: Content statistics: Total views */
  $handler->display->display_options['fields']['totalcount']['id'] = 'totalcount';
  $handler->display->display_options['fields']['totalcount']['table'] = 'node_counter';
  $handler->display->display_options['fields']['totalcount']['field'] = 'totalcount';
  $handler->display->display_options['fields']['totalcount']['element_label_colon'] = FALSE;
  $handler->display->display_options['defaults']['sorts'] = FALSE;
  /* Sort criterion: Content statistics: Total views */
  $handler->display->display_options['sorts']['totalcount']['id'] = 'totalcount';
  $handler->display->display_options['sorts']['totalcount']['table'] = 'node_counter';
  $handler->display->display_options['sorts']['totalcount']['field'] = 'totalcount';
  $handler->display->display_options['sorts']['totalcount']['order'] = 'DESC';
  /* Sort criterion: Content statistics: Most recent view */
  $handler->display->display_options['sorts']['timestamp']['id'] = 'timestamp';
  $handler->display->display_options['sorts']['timestamp']['table'] = 'node_counter';
  $handler->display->display_options['sorts']['timestamp']['field'] = 'timestamp';
  $handler->display->display_options['sorts']['timestamp']['order'] = 'DESC';
  $handler->display->display_options['defaults']['filter_groups'] = FALSE;
  $handler->display->display_options['defaults']['filters'] = FALSE;
  /* Filter criterion: Content: Published */
  $handler->display->display_options['filters']['status']['id'] = 'status';
  $handler->display->display_options['filters']['status']['table'] = 'node';
  $handler->display->display_options['filters']['status']['field'] = 'status';
  $handler->display->display_options['filters']['status']['value'] = 1;
  $handler->display->display_options['filters']['status']['group'] = 1;
  $handler->display->display_options['filters']['status']['expose']['operator'] = FALSE;
  /* Filter criterion: Content: Type */
  $handler->display->display_options['filters']['type']['id'] = 'type';
  $handler->display->display_options['filters']['type']['table'] = 'node';
  $handler->display->display_options['filters']['type']['field'] = 'type';
  $handler->display->display_options['filters']['type']['value'] = array(
    'agdlios' => 'agdlios',
  );
  /* Filter criterion: Content statistics: Most recent view */
  $handler->display->display_options['filters']['timestamp']['id'] = 'timestamp';
  $handler->display->display_options['filters']['timestamp']['table'] = 'node_counter';
  $handler->display->display_options['filters']['timestamp']['field'] = 'timestamp';
  $handler->display->display_options['filters']['timestamp']['exposed'] = TRUE;
  $handler->display->display_options['filters']['timestamp']['expose']['operator_id'] = 'timestamp_op';
  $handler->display->display_options['filters']['timestamp']['expose']['label'] = 'Last view date';
  $handler->display->display_options['filters']['timestamp']['expose']['use_operator'] = TRUE;
  $handler->display->display_options['filters']['timestamp']['expose']['operator'] = 'timestamp_op';
  $handler->display->display_options['filters']['timestamp']['expose']['identifier'] = 'timestamp';
  $handler->display->display_options['filters']['timestamp']['expose']['remember_roles'] = array(
    2 => '2',
    3 => 0,
    1 => 0,
    6 => 0,
    7 => 0,
    4 => 0,
    5 => 0,
  );
  /* Filter criterion: Content statistics: Total views */
  $handler->display->display_options['filters']['totalcount']['id'] = 'totalcount';
  $handler->display->display_options['filters']['totalcount']['table'] = 'node_counter';
  $handler->display->display_options['filters']['totalcount']['field'] = 'totalcount';
  $handler->display->display_options['filters']['totalcount']['exposed'] = TRUE;
  $handler->display->display_options['filters']['totalcount']['expose']['operator_id'] = 'totalcount_op';
  $handler->display->display_options['filters']['totalcount']['expose']['label'] = 'Resources views at least times:';
  $handler->display->display_options['filters']['totalcount']['expose']['use_operator'] = TRUE;
  $handler->display->display_options['filters']['totalcount']['expose']['operator'] = 'totalcount_op';
  $handler->display->display_options['filters']['totalcount']['expose']['identifier'] = 'totalcount';
  $handler->display->display_options['filters']['totalcount']['expose']['remember_roles'] = array(
    2 => '2',
    3 => 0,
    1 => 0,
    6 => 0,
    7 => 0,
    4 => 0,
    5 => 0,
  );
  $handler->display->display_options['path'] = 'most_views_resources';
  $handler->display->display_options['menu']['type'] = 'normal';
  $handler->display->display_options['menu']['title'] = 'Most views resources';
  $handler->display->display_options['menu']['weight'] = '0';
  $handler->display->display_options['menu']['name'] = 'menu-access';
  $handler->display->display_options['menu']['context'] = 0;

  /* Display: Page */
  $handler = $view->new_display('page', 'Page', 'page_2');
  $handler->display->display_options['defaults']['title'] = FALSE;
  $handler->display->display_options['title'] = 'Resources never views';
  $handler->display->display_options['defaults']['header'] = FALSE;
  /* Header: Global: Text area */
  $handler->display->display_options['header']['area']['id'] = 'area';
  $handler->display->display_options['header']['area']['table'] = 'views';
  $handler->display->display_options['header']['area']['field'] = 'area';
  $handler->display->display_options['header']['area']['content'] = 'Lista los registros o documentos del repositorio que han sido consultados en un periodo de tiempo.';
  $handler->display->display_options['header']['area']['format'] = '1';
  /* Header: Global: Result summary */
  $handler->display->display_options['header']['result']['id'] = 'result';
  $handler->display->display_options['header']['result']['table'] = 'views';
  $handler->display->display_options['header']['result']['field'] = 'result';
  $handler->display->display_options['header']['result']['content'] = '@end resources displayed in this page.';
  $handler->display->display_options['defaults']['relationships'] = FALSE;
  $handler->display->display_options['defaults']['fields'] = FALSE;
  /* Field: Content: Title */
  $handler->display->display_options['fields']['title']['id'] = 'title';
  $handler->display->display_options['fields']['title']['table'] = 'node';
  $handler->display->display_options['fields']['title']['field'] = 'title';
  $handler->display->display_options['fields']['title']['alter']['word_boundary'] = FALSE;
  $handler->display->display_options['fields']['title']['alter']['ellipsis'] = FALSE;
  $handler->display->display_options['fields']['title']['element_label_colon'] = FALSE;
  /* Field: Content: Author(s) */
  $handler->display->display_options['fields']['field_ag_author']['id'] = 'field_ag_author';
  $handler->display->display_options['fields']['field_ag_author']['table'] = 'field_data_field_ag_author';
  $handler->display->display_options['fields']['field_ag_author']['field'] = 'field_ag_author';
  $handler->display->display_options['fields']['field_ag_author']['label'] = 'Author';
  $handler->display->display_options['fields']['field_ag_author']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['field_ag_author']['delta_offset'] = '0';
  /* Field: Content: Document type */
  $handler->display->display_options['fields']['field_ag_type']['id'] = 'field_ag_type';
  $handler->display->display_options['fields']['field_ag_type']['table'] = 'field_data_field_ag_type';
  $handler->display->display_options['fields']['field_ag_type']['field'] = 'field_ag_type';
  $handler->display->display_options['fields']['field_ag_type']['label'] = 'Type';
  $handler->display->display_options['fields']['field_ag_type']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['field_ag_type']['type'] = 'taxonomy_term_reference_plain';
  /* Field: Content: File attachments */
  $handler->display->display_options['fields']['upload']['id'] = 'upload';
  $handler->display->display_options['fields']['upload']['table'] = 'field_data_upload';
  $handler->display->display_options['fields']['upload']['field'] = 'upload';
  $handler->display->display_options['fields']['upload']['label'] = 'Resource name';
  $handler->display->display_options['fields']['upload']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['upload']['click_sort_column'] = 'fid';
  $handler->display->display_options['fields']['upload']['type'] = 'file_url_plain';
  $handler->display->display_options['fields']['upload']['delta_offset'] = '0';
  $handler->display->display_options['defaults']['sorts'] = FALSE;
  /* Sort criterion: Content statistics: Most recent view */
  $handler->display->display_options['sorts']['timestamp']['id'] = 'timestamp';
  $handler->display->display_options['sorts']['timestamp']['table'] = 'node_counter';
  $handler->display->display_options['sorts']['timestamp']['field'] = 'timestamp';
  $handler->display->display_options['sorts']['timestamp']['order'] = 'DESC';
  $handler->display->display_options['defaults']['filter_groups'] = FALSE;
  $handler->display->display_options['defaults']['filters'] = FALSE;
  /* Filter criterion: Content: Published */
  $handler->display->display_options['filters']['status']['id'] = 'status';
  $handler->display->display_options['filters']['status']['table'] = 'node';
  $handler->display->display_options['filters']['status']['field'] = 'status';
  $handler->display->display_options['filters']['status']['value'] = 1;
  $handler->display->display_options['filters']['status']['group'] = 1;
  $handler->display->display_options['filters']['status']['expose']['operator'] = FALSE;
  /* Filter criterion: Content: Type */
  $handler->display->display_options['filters']['type']['id'] = 'type';
  $handler->display->display_options['filters']['type']['table'] = 'node';
  $handler->display->display_options['filters']['type']['field'] = 'type';
  $handler->display->display_options['filters']['type']['value'] = array(
    'agdlios' => 'agdlios',
  );
  /* Filter criterion: Content statistics: Total views */
  $handler->display->display_options['filters']['totalcount']['id'] = 'totalcount';
  $handler->display->display_options['filters']['totalcount']['table'] = 'node_counter';
  $handler->display->display_options['filters']['totalcount']['field'] = 'totalcount';
  $handler->display->display_options['filters']['totalcount']['operator'] = '<';
  $handler->display->display_options['filters']['totalcount']['value']['value'] = '1';
  $handler->display->display_options['filters']['totalcount']['expose']['operator_id'] = 'totalcount_op';
  $handler->display->display_options['filters']['totalcount']['expose']['label'] = 'Registros consultados al menos vezes:';
  $handler->display->display_options['filters']['totalcount']['expose']['use_operator'] = TRUE;
  $handler->display->display_options['filters']['totalcount']['expose']['operator'] = 'totalcount_op';
  $handler->display->display_options['filters']['totalcount']['expose']['identifier'] = 'totalcount';
  $handler->display->display_options['path'] = 'resources_never_views';
  $handler->display->display_options['menu']['type'] = 'normal';
  $handler->display->display_options['menu']['title'] = 'Resources never views';
  $handler->display->display_options['menu']['weight'] = '1';
  $handler->display->display_options['menu']['name'] = 'menu-access';
  $handler->display->display_options['menu']['context'] = 0;

  /* Display: Page */
  $handler = $view->new_display('page', 'Page', 'page_3');
  $handler->display->display_options['defaults']['title'] = FALSE;
  $handler->display->display_options['title'] = 'Documents created by a particular user in a defined range of dates';
  $handler->display->display_options['defaults']['header'] = FALSE;
  /* Header: Global: Result summary */
  $handler->display->display_options['header']['result']['id'] = 'result';
  $handler->display->display_options['header']['result']['table'] = 'views';
  $handler->display->display_options['header']['result']['field'] = 'result';
  $handler->display->display_options['header']['result']['content'] = '@end displayed resources in this page.
Tot.items in this collection : @total.';
  $handler->display->display_options['defaults']['fields'] = FALSE;
  /* Field: Content: Title */
  $handler->display->display_options['fields']['title']['id'] = 'title';
  $handler->display->display_options['fields']['title']['table'] = 'node';
  $handler->display->display_options['fields']['title']['field'] = 'title';
  $handler->display->display_options['fields']['title']['alter']['word_boundary'] = FALSE;
  $handler->display->display_options['fields']['title']['alter']['ellipsis'] = FALSE;
  $handler->display->display_options['fields']['title']['element_label_colon'] = FALSE;
  /* Field: Content: Author(s) */
  $handler->display->display_options['fields']['field_ag_author']['id'] = 'field_ag_author';
  $handler->display->display_options['fields']['field_ag_author']['table'] = 'field_data_field_ag_author';
  $handler->display->display_options['fields']['field_ag_author']['field'] = 'field_ag_author';
  $handler->display->display_options['fields']['field_ag_author']['label'] = 'Author';
  $handler->display->display_options['fields']['field_ag_author']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['field_ag_author']['delta_offset'] = '0';
  /* Field: Content: Document type */
  $handler->display->display_options['fields']['field_ag_type']['id'] = 'field_ag_type';
  $handler->display->display_options['fields']['field_ag_type']['table'] = 'field_data_field_ag_type';
  $handler->display->display_options['fields']['field_ag_type']['field'] = 'field_ag_type';
  $handler->display->display_options['fields']['field_ag_type']['label'] = 'Tipo';
  $handler->display->display_options['fields']['field_ag_type']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['field_ag_type']['type'] = 'taxonomy_term_reference_plain';
  /* Field: Content: File attachments */
  $handler->display->display_options['fields']['upload']['id'] = 'upload';
  $handler->display->display_options['fields']['upload']['table'] = 'field_data_upload';
  $handler->display->display_options['fields']['upload']['field'] = 'upload';
  $handler->display->display_options['fields']['upload']['label'] = 'Resource name';
  $handler->display->display_options['fields']['upload']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['upload']['click_sort_column'] = 'fid';
  $handler->display->display_options['fields']['upload']['type'] = 'file_url_plain';
  $handler->display->display_options['fields']['upload']['delta_offset'] = '0';
  /* Field: User: Name */
  $handler->display->display_options['fields']['name']['id'] = 'name';
  $handler->display->display_options['fields']['name']['table'] = 'users';
  $handler->display->display_options['fields']['name']['field'] = 'name';
  $handler->display->display_options['fields']['name']['relationship'] = 'uid';
  $handler->display->display_options['fields']['name']['label'] = 'User';
  $handler->display->display_options['fields']['name']['element_label_colon'] = FALSE;
  /* Field: User: Roles */
  $handler->display->display_options['fields']['rid']['id'] = 'rid';
  $handler->display->display_options['fields']['rid']['table'] = 'users_roles';
  $handler->display->display_options['fields']['rid']['field'] = 'rid';
  $handler->display->display_options['fields']['rid']['relationship'] = 'uid';
  $handler->display->display_options['fields']['rid']['label'] = 'User role';
  $handler->display->display_options['fields']['rid']['element_label_colon'] = FALSE;
  /* Field: Content: Post date */
  $handler->display->display_options['fields']['created']['id'] = 'created';
  $handler->display->display_options['fields']['created']['table'] = 'node';
  $handler->display->display_options['fields']['created']['field'] = 'created';
  $handler->display->display_options['fields']['created']['label'] = 'Creation date ';
  $handler->display->display_options['fields']['created']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['created']['date_format'] = 'custom';
  $handler->display->display_options['fields']['created']['custom_date_format'] = 'd/m/Y';
  $handler->display->display_options['defaults']['filter_groups'] = FALSE;
  $handler->display->display_options['defaults']['filters'] = FALSE;
  /* Filter criterion: Content: Published */
  $handler->display->display_options['filters']['status']['id'] = 'status';
  $handler->display->display_options['filters']['status']['table'] = 'node';
  $handler->display->display_options['filters']['status']['field'] = 'status';
  $handler->display->display_options['filters']['status']['value'] = 1;
  $handler->display->display_options['filters']['status']['group'] = 1;
  $handler->display->display_options['filters']['status']['expose']['operator'] = FALSE;
  /* Filter criterion: Content: Type */
  $handler->display->display_options['filters']['type']['id'] = 'type';
  $handler->display->display_options['filters']['type']['table'] = 'node';
  $handler->display->display_options['filters']['type']['field'] = 'type';
  $handler->display->display_options['filters']['type']['value'] = array(
    'agdlios' => 'agdlios',
  );
  /* Filter criterion: Content: Post date */
  $handler->display->display_options['filters']['created']['id'] = 'created';
  $handler->display->display_options['filters']['created']['table'] = 'node';
  $handler->display->display_options['filters']['created']['field'] = 'created';
  $handler->display->display_options['filters']['created']['exposed'] = TRUE;
  $handler->display->display_options['filters']['created']['expose']['operator_id'] = 'created_op';
  $handler->display->display_options['filters']['created']['expose']['label'] = 'Creation date';
  $handler->display->display_options['filters']['created']['expose']['use_operator'] = TRUE;
  $handler->display->display_options['filters']['created']['expose']['operator'] = 'created_op';
  $handler->display->display_options['filters']['created']['expose']['identifier'] = 'created';
  $handler->display->display_options['filters']['created']['expose']['remember_roles'] = array(
    2 => '2',
    3 => 0,
    1 => 0,
    6 => 0,
    7 => 0,
    4 => 0,
    5 => 0,
  );
  /* Filter criterion: User: Name (raw) */
  $handler->display->display_options['filters']['name']['id'] = 'name';
  $handler->display->display_options['filters']['name']['table'] = 'users';
  $handler->display->display_options['filters']['name']['field'] = 'name';
  $handler->display->display_options['filters']['name']['relationship'] = 'uid';
  $handler->display->display_options['filters']['name']['exposed'] = TRUE;
  $handler->display->display_options['filters']['name']['expose']['operator_id'] = 'name_op';
  $handler->display->display_options['filters']['name']['expose']['label'] = 'User name';
  $handler->display->display_options['filters']['name']['expose']['operator'] = 'name_op';
  $handler->display->display_options['filters']['name']['expose']['identifier'] = 'name';
  $handler->display->display_options['filters']['name']['expose']['remember_roles'] = array(
    2 => '2',
    3 => 0,
    1 => 0,
    6 => 0,
    7 => 0,
    4 => 0,
    5 => 0,
  );
  $handler->display->display_options['path'] = 'users_doc';
  $handler->display->display_options['menu']['type'] = 'normal';
  $handler->display->display_options['menu']['title'] = 'Documents created by a particular user in a defined range of dates';
  $handler->display->display_options['menu']['weight'] = '3';
  $handler->display->display_options['menu']['name'] = 'menu-access';
  $handler->display->display_options['menu']['context'] = 0;
  $export['reporte_de_existencias'] = $view;

  return $export;
}
