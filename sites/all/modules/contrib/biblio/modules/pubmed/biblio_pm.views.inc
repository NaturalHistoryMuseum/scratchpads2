<?php
function biblio_pm_views_data() {

  $data['biblio_pubmed']['table']['group']  = t('Biblio');
  $data['biblio_pubmed']['table']['base']  = array(
      'field' => 'nid',
      'title' => t('PubMed Table'),
      'help' => t('This table contains PubMed Identifiers'),
      'weight' => 10,
  );

  $data['biblio_pubmed']['table']['join'] = array(
      'node' => array(
          'left_field' => 'nid',
          'field' => 'nid',
      ),
  );

  $data['biblio_pubmed']['biblio_pubmed_id'] = array(
      'title' => t('PubMed ID'),
      'help' => t('The PubMed ID (http://pubmed.org)'),
      'field' => array(
          'handler' => 'views_handler_field',
          'click sortable' => TRUE,
      ),
      'sort' => array(
          'handler' => 'views_handler_sort',
      ),
      'filter' => array(
          'handler' => 'views_handler_filter_integer',
      ),
      'argument' => array(
          'handler' => 'views_handler_argument_integer',
      ),

  );
  $data['biblio_pubmed']['biblio_pmcid'] = array(
      'title' => t('PMCID'),
      'help' => t('The PubMed Central ID (http://www.ncbi.nlm.nih.gov/pmc/)'),
      'field' => array(
          'handler' => 'views_handler_field',
          'click sortable' => TRUE,
      ),
      'sort' => array(
          'handler' => 'views_handler_sort',
      ),
      'filter' => array(
          'handler' => 'views_handler_filter_string',
      ),
      'argument' => array(
          'handler' => 'views_handler_argument_string',
      ),

  );
  $data['biblio_pubmed_grant_info']['table']['group']  = t('Biblio');
  $data['biblio_pubmed_grant_info']['table']['base']  = array(
      'field' => 'nid',
      'title' => t('PubMed Grant Table'),
      'help' => t('This table contains Grant information for PubMed entries'),
      'weight' => 10,
  );

  $data['biblio_pubmed_grant_info']['table']['join'] = array(
      'node' => array(
          'left_field' => 'nid',
          'field' => 'nid',
      ),
  );
  $data['biblio_pubmed_grant_info']['biblio_pubmed_id'] = array(
      'title' => t('PubMed ID'),
      'help' => t('The PubMed ID (http://pubmed.org)'),
      'field' => array(
          'handler' => 'views_handler_field',
          'click sortable' => TRUE,
      ),
      'sort' => array(
          'handler' => 'views_handler_sort',
      ),
      'filter' => array(
          'handler' => 'views_handler_filter_integer',
      ),
      'argument' => array(
          'handler' => 'views_handler_argument_integer',
      ),
  );
  $data['biblio_pubmed_grant_info']['grantid'] = array(
      'title' => t('PubMed Grant ID'),
      'help' => t('The PubMed ID (http://pubmed.org)'),
      'field' => array(
          'handler' => 'views_handler_field',
          'click sortable' => TRUE,
      ),
      'sort' => array(
          'handler' => 'views_handler_sort',
      ),
      'filter' => array(
          'handler' => 'views_handler_filter_string',
      ),
      'argument' => array(
          'handler' => 'views_handler_argument_string',
      ),
  );
  $data['biblio_pubmed_grant_info']['acronym'] = array(
      'title' => t('PubMed Grant Acronym'),
      'help' => t('The PubMed ID (http://pubmed.org)'),
      'field' => array(
          'handler' => 'views_handler_field',
          'click sortable' => TRUE,
      ),
      'sort' => array(
          'handler' => 'views_handler_sort',
      ),
      'filter' => array(
          'handler' => 'views_handler_filter_string',
      ),
      'argument' => array(
          'handler' => 'views_handler_argument_string',
      ),
  );
  $data['biblio_pubmed_grant_info']['agency'] = array(
      'title' => t('PubMed Grant Agency'),
      'help' => t('The PubMed ID (http://pubmed.org)'),
      'field' => array(
          'handler' => 'views_handler_field',
          'click sortable' => TRUE,
      ),
      'sort' => array(
          'handler' => 'views_handler_sort',
      ),
      'filter' => array(
          'handler' => 'views_handler_filter_string',
      ),
      'argument' => array(
          'handler' => 'views_handler_argument_string',
      ),
  );
  $data['biblio_pubmed_grant_info']['country'] = array(
      'title' => t('PubMed Grant Country'),
      'help' => t('The PubMed ID (http://pubmed.org)'),
      'field' => array(
          'handler' => 'views_handler_field',
          'click sortable' => TRUE,
      ),
      'sort' => array(
          'handler' => 'views_handler_sort',
      ),
      'filter' => array(
          'handler' => 'views_handler_filter_string',
      ),
      'argument' => array(
          'handler' => 'views_handler_argument_string',
      ),
  );

  return $data;
}

