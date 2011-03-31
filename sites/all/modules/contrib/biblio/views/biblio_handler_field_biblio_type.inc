<?php

class biblio_handler_field_biblio_type extends views_handler_field {
  function init(&$view, $options) {
    parent::init($view, $options);
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
    //print_r($values);
    if ($vids) {

      //$result = db_query("SELECT bt.name AS node_vid, bkd.* FROM {biblio_keyword_data} bkd INNER JOIN {biblio_keyword} bk ON bkd.kid = bk.kid WHERE bk.vid IN (" . implode(', ', $vids) . ") ORDER BY  bkd.word");
      $result = db_query("SELECT name, tid
                          FROM {biblio_types} t
                          WHERE IN (:vids)", array(':vids' => implode(', ', $vids)));

      foreach ($result as $term) {
//        if (empty($this->options['link_to_taxonomy'])) {
          $this->items[$term->node_vid][$term->kid] = check_plain($term->word);
//        }
//        else {
//          $this->items[$term->node_vid][$term->kid] = l($term->word, taxonomy_term_path($term));
//        }
      }
    }
  }
}
