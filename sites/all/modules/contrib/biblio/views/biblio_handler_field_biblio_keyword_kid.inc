<?php

class biblio_handler_field_biblio_keyword_kid extends views_handler_field_prerender_list {
  function init(&$view, $options) {
    parent::init($view, $options);
    $this->multiple = TRUE;
    if ($view->base_table == 'node_revision') {
      $this->additional_fields['vid'] = array('table' => 'node_revision', 'field' => 'vid');
    }
    else {
      $this->additional_fields['vid'] = array('table' => 'node', 'field' => 'vid');
    }
  }


  function pre_render($values) {
    $this->field_alias = $this->aliases['vid'];
    $vids = array();
    foreach ($values as $result) {
      if (!empty($result->{$this->aliases['vid']})) {
        $vids[] = $result->{$this->aliases['vid']};
      }
    }
    if ($vids) {
      $query = db_select('biblio_keyword_data', 'bkd');
      $query->innerJoin('biblio_keyword', 'bk', 'bkd.kid = bk.kid');
      $query->fields('bkd');
      $query->addField('bk', 'vid', 'node_vid');
      $query->orderby('bkd.word');
      $query->condition('bk.vid', $vids);
      $query->addMetaData('base_table', 'biblio_keyword');
      $query->addTag('node_access');
      $result = $query->execute();

      foreach ($result as $term) {
        $this->items[$term->node_vid][$term->kid] = check_plain($term->word);
      }
    }
  }
}
