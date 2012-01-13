<?php

class ScratchpadsBiblioSearchSolrBaseQuery extends SolrBaseQuery implements DrupalSolrQueryInterface{

  function __construct($name, $solr, array $params = array(), $sortstring = '', $base_path = ''){
    // Here we teak the $sortstring to reflect what is set in the $_GET array.
    $order = isset($_GET['order']) ? $_GET['order'] : t('Authors');
    switch($order){
      case t('Year'):
        $sortstring = 'tus_biblio_year ' . (isset($_GET['sort']) ? $_GET['sort'] : 'desc');
        break;
      case t('Authors'):
        $sortstring = 'tus_biblio_first_author_lastname ' . (isset($_GET['sort']) ? $_GET['sort'] : 'asc');
        break;
      case t('Title'):
        $sortstring = 'sort_label ' . (isset($_GET['sort']) ? $_GET['sort'] : 'asc');
        break;
    }
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
      ),
      'tus_biblio_first_author_lastname' => array(
        'title' => t('First author surname'),
        'default' => 'asc'
      )
    );
  }
}