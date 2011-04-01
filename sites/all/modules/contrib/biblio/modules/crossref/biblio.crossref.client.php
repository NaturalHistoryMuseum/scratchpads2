<?php
class BiblioCrossRefClient
{

  const BASE_URL = 'http://www.crossref.org/openurl/';
  private $pid;
  private $doi;
  private $query;
  private $url;
  private $nodes;
  private $node;
  private $parser;
  private $element;
  private $attribute;
  private $auth_category;
  private $contrib_count;
  private $contribtors;
  private $org_count;
  private $field_map;
  private $type_map;

  public function __construct($doi = '', $id = '')
  {
    $this->setDOI($doi);
    $this->setUserID($id);
    $this->setURL(self::BASE_URL);
    $this->field_map = array();
    $this->type_map = array();
  }

  public function setURL($url) {
    $this->url = $url;
  }

  public function setDOI($doi) {
    $this->doi = $doi;
  }

  public function getDOI() {
    return $this->doi;
  }

  public function setUserID($id) {
    $this->pid = $id;
  }

  public function getUserID() {
    return $this->pid;
  }

  public function getQuery() {
    return $this->query;
  }

  public function fetch() {
    $this->query = $this->url . '?pid=' . $this->pid . '&noredirect=true&format=unixref&id=doi%3A' . $this->doi;
    if (!($fp = fopen($this->query, "r"))) {
      drupal_set_message(t('Could not open crossref.org for XML input'),'error');
      return;
    }
    $this->nodes = array();
    $xml = fread($fp, 2048);
    $this->parser = drupal_xml_parser_create($xml);
    // use case-folding so we are sure to find the tag in
    xml_parser_set_option($this->parser, XML_OPTION_CASE_FOLDING, FALSE);
    xml_parser_set_option($this->parser, XML_OPTION_SKIP_WHITE, TRUE);

    xml_set_object($this->parser, $this);
    xml_set_element_handler($this->parser, 'unixref_startElement', 'unixref_endElement');
    xml_set_character_data_handler($this->parser, 'unixref_characterData');

    xml_parse($this->parser, $xml);
    while ($xml = fread($fp, 2048)) {
      set_time_limit(30);
      if (!xml_parse($this->parser, $xml, feof($fp))) {
        drupal_set_message(sprintf("XML error: %s at line %d",
        xml_error_string(xml_get_error_code($this->parser)),
        xml_get_current_line_number($this->parser)),'error');
      }
    }
    xml_parser_free($this->parser);
    fclose($fp);
    //$this->nodes =(!empty($nodes)) ? $nodes : array();
    return $this->node;
  }


  function unixref_startElement($parser, $name, $attrs) {
    switch ($name) {
      case 'doi_record' :
        $this->node = array();
        $this->contributors = array();
        $this->element = $name;
        break;
      case 'book':
      case 'journal':
      case 'standard':
      case 'conference':
      case 'report-paper':
      case 'dissertation':
      case 'database':
      case 'sa_component':
        $this->node['biblio_type'] = $this->_unixref_type_map($name);
        $this->element = $name;
        break;
      case 'journal_article':
      case 'conference_paper':
      case 'content_item':
      case 'report-paper_metadata':
      case 'standard_metadata':
      case 'database_date':
      case 'component':
        $this->node['year'] = '';
        $this->node['doi']  = '';
        $this->element = $name;
        break;
      case 'person_name' :
        $this->auth_category = $this->_unixref_get_contributor_category($attrs['contributor_role']);
        if (!isset($this->contrib_count)) $this->contrib_count = 0;
        $this->element = $name;
        break;
      case 'organization':
        if (!isset($this->org_count)) $this->org_count = 0;
        $this->element = $name;
        break;
      case 'issn':
        if ($attrs['media_type'] == 'print') $this->attribute = 'issn_print';
        $this->element = $name;
        break;
      case 'isbn':
        if ($attrs['media_type'] == 'print') $this->attribute = 'isbn_print';
        $this->element = $name;
        break;
      case 'i':  // HTML font style tags
      case 'b':
      case 'u':
      case 'sub':
      case 'sup':
        $this->unixref_characterData(NULL, ' <' . $name . '>');
        break;
      default :
        $this->element = $name;
    }

  }
  function unixref_decode(&$item, $key) {
    $item = html_entity_decode($item, NULL, 'UTF-8');
  }
  function unixref_endElement($parser, $name) {
    switch ($name) {
      case 'doi_record' :
        $this->node['biblio_contributors'] += $this->contributors;
        array_walk_recursive($this->node, array($this,'unixref_decode') );
        $this->node['biblio_crossref_id']  = $this->getDOI();
        $this->node['biblio_crossref_md5'] = md5(serialize($this->node));
        $this->nodes[] = $this->node; //biblio_save_node($node, $batch, $session_id, $save_node);
        break;
      case 'person_name' :
        $this->contributors[$this->contrib_count]['auth_type'] = _biblio_get_auth_type($this->auth_category, $this->node['biblio_type']);
        $this->contributors[$this->contrib_count]['auth_category'] = $this->auth_category;
        $this->contributors[$this->contrib_count]['name'] =
        $this->contributors[$this->contrib_count]['lastname'];
        if (isset($this->contributors[$this->contrib_count]['firstname'])) {
          $this->contributors[$this->contrib_count]['name'] .=
            ', ' . $this->contributors[$this->contrib_count]['firstname'];
        }

        $this->auth_category = '';
        $this->contrib_count++;
        break;
      case 'organization' :
        $this->contributors[$this->contrib_count]['auth_type'] = _biblio_get_auth_type(5, $this->node['biblio_type']);
        $this->contributors[$this->contrib_count]['auth_category'] = 5;
        $this->contrib_count++;
        break;
      case 'pages':
        if (isset($this->node['biblio_first_page'])) $this->node['biblio_pages'] = $this->node['biblio_first_page'];
        if (isset($this->node{'biblio_last_page'}))  $this->node['biblio_pages'] .= ' - ' . $this->node['biblio_last_page'];
        break;
      case 'publication_date':

        break;
      case 'journal_issue':
        $this->node['biblio_date'] = (!empty($this->node['month']) ? $this->node['month'] . '/':'') . $this->node['year'];
        break;
      case 'journal_article':
      case 'conference_paper':
      case 'content_item':
      case 'report-paper_metadata':
      case 'standard_metadata':
      case 'database_date':
      case 'component':
        $this->node['biblio_year'] = $this->node['year'];
        $this->node['biblio_doi']  = $this->node['doi'];
        break;
      case 'issn':
        if ($this->attribute == 'issn_print' && isset($this->node['issn'])) $this->node['biblio_issn'] = $this->node['issn'];
        $this->node['issn'] = '';
        break;
      case 'isbn':
        if ($this->attribute == 'isbn_print' && isset($this->node['isbn'])) $this->node['biblio_isbn'] = $this->node['isbn'];
        $this->node['isbn'] = '';
        break;
      case 'i':  // HTML font style tags
      case 'b':
      case 'u':
      case 'sub':
      case 'sup':
        $this->unixref_characterData(NULL, '</' . $name . '> ');
        break;
      default :
    }
  }

  function unixref_characterData($parser, $data) {
    $data = htmlspecialchars_decode($data);
    if (trim($data)) {
      switch ($this->element) {
        case 'surname' :
          $this->contributors[$this->contrib_count]['lastname'] = $data;
          break;
        case 'given_name' :
          $this->contributors[$this->contrib_count]['firstname'] = $data;
          break;
        case 'suffix':
          $this->contributors[$this->contrib_count]['suffix'] = $data;
          break;
        case 'affiliation' :
          $this->contributors[$this->contrib_count]['affiliation'] = $data;
          break;
        case 'organization':
          $this->contributors[$this->contrib_count]['name'] = $data;
          break;
        case 'year':
          $this->node['year'] = $data;
          break;
        case 'month':
          $this->node['month'] = $data;
          break;
        case 'day':
          $this->node['day'] = $data;
          break;
        default:
          if ($field = $this->_unixref_field_map(trim($this->element))) {
            $this->node[$field] = $data;
          }

      }
    }
  }
  /*
   * map a unixref XML field to a biblio field
   */
  function _unixref_field_map($field) {
    if (empty($this->field_map)) {
      $this->field_map = unserialize(db_query("SELECT field_map FROM {biblio_type_maps} WHERE format='crossref'")->fetchField());
    }
    return (isset($this->field_map[$field])) ? $this->field_map[$field]: FALSE;
  }

  function _unixref_type_map($type) {
    if (empty($this->type_map)) {
      $this->type_map = unserialize(db_query("SELECT type_map FROM {biblio_type_maps} WHERE format='crossref'")->fetchField());
    }
    return (isset($this->type_map[$type]))?$this->type_map[$type]:129; //return the biblio type or 129 (Misc) if type not found
  }

  function _unixref_get_contributor_category($role) {
    if ($role == 'author') return 1;
    if ($role == 'editor') return 2;
    if ($role == 'chair') return 3;
    if ($role == 'translator') return 4;
    return NULL;
  }
}