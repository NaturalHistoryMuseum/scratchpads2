<?php

class biblio_handler_sort_contributor_lastname extends views_handler_sort {

  function query() {
    parent::query();
//    $this->query->add_where(0, "biblio_contributor.rank = 0");
  }

}

