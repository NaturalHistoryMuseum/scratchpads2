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
  private $citation_list;

  public function __construct($doi = '', $id = '')
  {
    $this->setDOI($doi);
    $this->setUserID($id);
    $this->setURL(self::BASE_URL);
    $this->field_map = array();
    $this->type_map = array();
    $this->citation_list = FALSE;
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

    $request_options = array('method' => 'GET');
    $result = drupal_http_request($this->query, $request_options);

    if ($result->code != 200) {
      drupal_set_message(t('HTTP error: !error when trying to contact crossref.org for XML input', array('!error' => $result->code)),'error');
      return;
    }
    if (empty($result->data)) {
      drupal_set_message(t('Did not get any data from crossref.org'),'error');
      return;
    }
    $sxml = @simplexml_load_string($result->data);
    if (!isset($sxml->doi_record)) {
    	drupal_set_message(t('Failed to retrieve data for doi ') . $this->doi, 'error');
      return;
    }

    if ($error = (string)$sxml->doi_record->crossref->error) {
      drupal_set_message($error,'error');
      return;
    }
    $this->nodes = array();
    $this->parser = drupal_xml_parser_create($result->data);
    // use case-folding so we are sure to find the tag in
    xml_parser_set_option($this->parser, XML_OPTION_CASE_FOLDING, FALSE);
    xml_parser_set_option($this->parser, XML_OPTION_SKIP_WHITE, TRUE);

    xml_set_object($this->parser, $this);
    xml_set_element_handler($this->parser, 'unixref_startElement', 'unixref_endElement');
    xml_set_character_data_handler($this->parser, 'unixref_characterData');

    if(!xml_parse($this->parser, $result->data)){
      drupal_set_message(sprintf("XML error: %s at line %d",
      xml_error_string(xml_get_error_code($this->parser)),
      xml_get_current_line_number($this->parser)),'error');
    }

    xml_parser_free($this->parser);

    return $this->node;
  }


  function unixref_startElement($parser, $name, $attrs) {
    switch ($name) {
      case 'doi_record' :
        $this->node = array();
        $this->node['biblio_contributors'] = array();
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
        if (isset($attrs['media_type']) ) $this->attribute = $attrs['media_type'];
        $this->element = $name;
        break;
      case 'isbn':
        if (isset($attrs['media_type']) ) $this->attribute = $attrs['media_type'];
        $this->element = $name;
        break;
      case 'i':  // HTML font style tags
      case 'b':
      case 'u':
      case 'sub':
      case 'sup':
        $this->unixref_characterData(NULL, ' <' . $name . '>');
        break;
      case 'doi_data':
        $this->doi_data = TRUE;
        break;
      case 'citation_list':
      	$this->citation_list = TRUE;
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
      case 'journal_article':
        if (!isset($this->node['biblio_date']) || empty($this->node['biblio_date'])) {
          $day   = !empty($this->node['day'])   ? $this->node['day']   : 1;
          $month = !empty($this->node['month']) ? $this->node['month'] : 1;
          $year  = !empty($this->node['year'])  ? $this->node['year']  : 0;
          if ($year) {
            $this->node['biblio_date'] = date("M-d-Y", mktime(0, 0, 0, $day, $month, $year));
          }
        }
        if ((!isset($this->node['biblio_year']) || empty($this->node['biblio_year'])) && isset($this->node['year'])) {
          $this->node['biblio_year'] = $this->node['year'];
        }
        break;
      case 'conference_paper':
      case 'content_item':
      case 'report-paper_metadata':
      case 'standard_metadata':
      case 'database_date':
      case 'component':
        if ((!isset($this->node['biblio_year']) || empty($this->node['biblio_year'])) && isset($this->node['year'])) {
          $this->node['biblio_year'] = $this->node['year'];
          unset($this->node['year']);
        }
//        $this->node['biblio_doi']  = $this->node['doi'];
        break;
      case 'issn':
      case 'isbn':
        $this->attribute = '';
        break;
      case 'i':  // HTML font style tags
      case 'b':
      case 'u':
      case 'sub':
      case 'sup':
        $this->unixref_characterData(NULL, '</' . $name . '> ');
        break;
      case 'doi_data':
        $this->doi_data = FALSE;
        break;
      case 'citation_list':
        $this->citation_list = FALSE;
        break;
      default :
    }
  }

  function unixref_characterData($parser, $data) {
    $data = htmlspecialchars_decode($data);
    if (trim($data) && !$this->citation_list) {
      switch ($this->element) {
        case 'surname' :
        	$this->_set_contrib_data('lastname', $data);
          break;
        case 'given_name' :
          $this->_set_contrib_data('firstname', $data);
          break;
        case 'suffix':
        	$this->_set_contrib_data('suffix', $data);
          break;
        case 'affiliation' :
        	$this->_set_contrib_data('affiliation', $data);
          break;
        case 'organization':
        	$this->_set_contrib_data('name', $data);
          break;
        case 'year':
        case 'month':
        case 'day':
         $this->node[$this->element] = $data;
          break;
        case 'issn':
        case 'isbn':
          if ($this->attribute == 'print') {
            if ($field = $this->_unixref_field_map(trim($this->element))) {
              $this->_set_data($field, $data);
            }
          }
          break;
        case 'doi':
          if ($this->doi_data) {
            if ($field = $this->_unixref_field_map(trim($this->element))) {
              $this->_set_data($field, $data);
            }
          }
          break;
        case 'resource':
          if ($this->doi_data) {
              $this->_set_data('biblio_url', $data);
          }
          break;

        default:
          if ($field = $this->_unixref_field_map(trim($this->element))) {
            $this->_set_data($field, $data);
          }

      }
    }
  }

  function _set_contrib_data($field, $data) {
    $this->contributors[$this->contrib_count][$field] = (isset($this->contributors[$this->contrib_count][$field]) ?
                                                        $this->contributors[$this->contrib_count][$field] . $data :
                                                        $data);
  }

  function _set_data($field, $data) {
    $this->node[$field] = (isset($this->node[$field]) ?
                          $this->node[$field] . $data :
                          $data);
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