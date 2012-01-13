<?php

class ScratchpadsBiblioSearchSolrBaseQuery extends SolrBaseQuery implements DrupalSolrQueryInterface{

  function __construct($name, $solr, array $params = array(), $sortstring = '', $base_path = ''){
    parent::__construct($name, $solr, $params, $sortstring, $base_path);
  }

  protected function defaultSorts(){
    return array(
      'score' => array(
        'title' => t('Relevancy'),
        'default' => 'desc'
      ),
      'sort_label' => array(
        'title' => t('Title'),
        'default' => 'asc'
      ),
      'bundle' => array(
        'title' => t('Type'),
        'default' => 'asc'
      ),
      'sort_name' => array(
        'title' => t('Author'),
        'default' => 'asc'
      ),
      'ds_created' => array(
        'title' => t('Date'),
        'default' => 'desc'
      ),
      'tus_biblio_year' => array(
        'title' => t('Year'),
        'default' => 'desc'
      )
    );
  }
}