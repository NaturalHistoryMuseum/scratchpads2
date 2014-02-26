<?php

class ScratchpadsSolrBaseQuery extends SolrBaseQuery implements DrupalSolrQueryInterface{

  function __construct($name, $solr, array $params = array(), $sortstring = '', $base_path = ''){
    // Tweak the sortstring (we should probably allow tweaking of other params,
    // but for now, this will do).
    drupal_alter('apache_solr_sortstring', $sortstring, $params);
    parent::__construct($name, $solr, $params, $sortstring, $base_path);
  }

  protected function defaultSorts(){
    $default_sorts = parent::defaultSorts();
    drupal_alter('apache_solr_default_sorts', $default_sorts);
    return $default_sorts;
  }
}