<?php
class biblio_contributors {
  public $data;
  public $sort_name;
  public $sort_letter;
  private $categories;

  function __construct($vid = NULL) {
    if ($vid) {
      $this->categories = array(1 => '', 2 => '', 3 => '', 4 => '', 5 => '');
      $this->sort_name = '';
      $this->sort_letter = '';
      $this->data = $this->_load_contributors($vid);
    }
  }
  function _load_contributors($vid) {
    $contributors = array();
    $query = db_select('biblio_contributor', 'bc', array('fetch' => PDO::FETCH_ASSOC));
    $query->join('biblio_contributor_data', 'bcd', 'bc.cid = bcd.cid');
    $query->fields('bcd');
    $query->fields('bc', array('auth_type', 'auth_category', 'rank'));
    $query->condition('bc.vid', $vid)->orderBy('bc.rank', 'ASC');
    $result = $query->execute();

    foreach  ($result as $creator ) {
      $contributors[] = $creator;
      $this->categories[$creator['auth_category']]++;
      if (empty($this->sort_name)) {
        $this->sort_name = $creator['lastname'];
        $this->sort_letter = drupal_substr(drupal_ucfirst(ltrim($this->sort_name)), 0, 1);
      }
    }

    return $contributors;
  }

  function has_category($category) {
    return isset($this->categories[$category]) ? $this->categories[$category] : NULL;
  }

  function get_all() {
    return $this->data;
  }

  function get_category($category = NULL) {
    if (empty($category)) return;
    $authors = array();
    foreach ($this->data as $author) {
      if ($author['auth_category'] == $category) {
        $authors[] = $author;
      }
    }
    return $authors;
  }
}