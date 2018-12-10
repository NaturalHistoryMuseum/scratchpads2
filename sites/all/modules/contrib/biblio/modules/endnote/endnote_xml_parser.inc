<?php
/*
 * @file endnote_xml_parser.inc
 *
 */
class EndNoteXMLParser {
  private $parser;
  private $format;
  private $node;
  private $keyword_count;
  private $contributors;
  private $contributor_type;
  private $contrib_count;
  private $titles;
  private $periodical;
  private $dates;
  private $urls;
  private $font_attr;
  private $session_id;
  private $batch_proc;
  private $terms;
  private $nids;
  private $dups;
  private $unmapped;

  function parse($file, $terms, $batch, $session_id) {
    $this->terms = $terms;
    $this->batch_proc = $batch;
    $this->session_id = $session_id;
    $this->nids = array();
    $this->dups = array();
    $this->unmapped = array();

    if (!($fp = fopen($file->uri, "r"))) {
      drupal_set_message(t("could not open XML input"), 'error');
      return;
    }
    $data = fread($fp, 2048);
    if ((strpos($data, 'record') !== FALSE) && (strpos($data, 'ref-type') !== FALSE)) {
      $this->format = 'endnote8';
    }
    elseif (strpos($data, 'RECORD') !== FALSE && strpos($data, 'REFERENCE_TYPE') !== FALSE) {
      $this->format = 'endnote7';
    }
    if ($this->format) {
      $this->parser = drupal_xml_parser_create($data);
      xml_parser_set_option($this->parser, XML_OPTION_CASE_FOLDING, FALSE);
      xml_parser_set_option($this->parser, XML_OPTION_SKIP_WHITE, TRUE);
      xml_set_object($this->parser, $this);
      xml_set_element_handler($this->parser, $this->format . '_startElement', $this->format . '_endElement');
      xml_set_character_data_handler($this->parser, $this->format . '_characterData');

      xml_parse($this->parser, $data, feof($fp));

      while ($data = fread($fp, 2048)) {
        // $data = fread($fp, 2048);
        set_time_limit(300);
        if (!xml_parse($this->parser, $data, feof($fp))) {
          drupal_set_message(sprintf("XML error: %s at line %d",
          xml_error_string(xml_get_error_code($this->parser)),
          xml_get_current_line_number($this->parser)), 'error');
        }
      }
      xml_parser_free($this->parser);
    }

    fclose($fp);

    if (!empty($this->unmapped)) {
      $ignored_tags = array_unique($this->unmapped);
      $message = t("The following elements were ignored because they do not map to any biblio fields:") . ' ';
      $message .= implode(', ', $ignored_tags);
      if (user_access('administer biblio')) {
        $message .= '. ' . t('Click !url if you wish to check the field mapping', array('!url' => l(t('here'), 'admin/config/content/biblio/iomap/edit/' . $this->format)));
      }
      drupal_set_message($message, 'warning');
    }
    return array($this->nids, $this->dups);

  }

  function endnote8_startElement($parser, $name, $attrs) {
    switch ($name) {
      case 'record' :
        $this->node = new stdClass();
        $this->node->biblio_contributors = array();
        break;
      case 'style' :
        $this->font_attr = explode(' ', $attrs['face']);
        foreach ($this->font_attr as $fatt) {
          switch ($fatt) {
            case 'normal':
              break;
            case 'bold':
              $this->endnote8_characterData(NULL, '<b>');
              break;
            case 'italic':
              $this->endnote8_characterData(NULL, '<i>');
              break;
            case 'underline':
              $this->endnote8_characterData(NULL, '<u>');
              break;
            case 'superscript':
              $this->endnote8_characterData(NULL, '<sup>');
              break;
            case 'subscript':
              $this->endnote8_characterData(NULL, '<sub>');
              break;
          }
        }
        break;
      case 'keywords' :
        $this->keyword_count = 0;
        break;
      case 'authors' :
      case 'secondary-authors' :
      case 'tertiary-authors' :
      case 'subsidiary-authors' :
      case 'translated-authors' :
        $this->contributors_type = $name;
        $this->contributors = array();
        $this->contrib_count = 0;
        break;
      case 'author' :
        $this->contributors[$this->contrib_count]['name'] = '';
        $this->element = $name;
        break;
      case 'year' :
      case 'pub-dates' :
      case 'copyright-dates' :
        $this->dates = $name;
        break;
      case 'web-urls' :
      case 'pdf-urls' :
      case 'text-urls' :
      case 'related-urls' :
      case 'image-urls' :
        $this->urls = $name;
        break;
      case 'keyword':
        $this->node->biblio_keywords[$this->keyword_count] = '';
        $this->element = $name;
        break;

      default :
        $this->element = $name;
    }
  }

  function endnote8_endElement($parser, $name) {
    //    global $this->node, $nids, $this->element, $terms, $batch_proc, $session_id, $this->contributors_type, $this->contrib_count, $this->dates, $this->urls, $this->keyword_count, $this->font_attr;
    switch ($name) {
      case 'record' :
        $this->element = $this->contributors_type = $this->contrib_count = $this->dates = $this->urls = '';
        $this->node->biblio_xml_md5 = md5(serialize($this->node));
        if ( !($dup = $this->biblio_xml_check_md5($this->node->biblio_xml_md5)) ) {
          biblio_save_node($this->node, $this->terms, $this->batch_proc, $this->session_id);
          if (!empty($this->node->nid)) $this->nids[] = $this->node->nid;
        }
        else {
          $this->dups[] = $dup;
        }
        break;
      case 'authors' :
      case 'secondary-authors' :
      case 'tertiary-authors' :
      case 'subsidiary-authors' :
      case 'translated-authors' :
        $this->contributors_type = '';
        foreach ($this->contributors as $contributor) {
          $this->node->biblio_contributors[] = $contributor;
        }
        break;
      case 'author' :
        switch ($this->contributors_type) {
          case 'authors' :
            $this->contributors[$this->contrib_count]['auth_category'] = 1;
            $this->contributors[$this->contrib_count]['auth_type'] =  1;
            break;
          case 'secondary-authors' :
            $this->contributors[$this->contrib_count]['auth_category'] = 2;
            $this->contributors[$this->contrib_count]['auth_type'] = 2;
            break;
          case 'tertiary-authors' :
            $this->contributors[$this->contrib_count]['auth_category'] = 3;
            $this->contributors[$this->contrib_count]['auth_type'] = 3;
            break;
          case 'subsidiary-authors' :
            $this->contributors[$this->contrib_count]['auth_category'] = 4;
            $this->contributors[$this->contrib_count]['auth_type'] = 4;
            break;
          case 'translated-authors' :
            $this->contributors[$this->contrib_count]['auth_category'] = 5;
            $this->contributors[$this->contrib_count]['auth_type'] = 5;
            break;
        }
        $this->contrib_count++;
        break;
      case 'keyword' :
        $this->keyword_count++;
        break;
      case 'year' :
      case 'pub-dates' :
      case 'copyright-dates' :
        $this->dates = '';
        break;
      case 'web-urls' :
      case 'pdf-urls' :
      case 'text-urls' :
      case 'related-urls' :
      case 'image-urls' :
        $this->urls = '';
        break;
      case 'ref-type':
        $this->node->biblio_type = $this->type_map($this->node->biblio_type);
        $this->element = '';
        break;
      case 'style' :
        foreach ($this->font_attr as $fatt) {
          switch ($fatt) {
            case 'normal':
              break;
            case 'bold':
              $this->endnote8_characterData(NULL, '</b>');
              break;
            case 'italic':
              $this->endnote8_characterData(NULL, '</i>');
              break;
            case 'underline':
              $this->endnote8_characterData(NULL, '</u>');
              break;
            case 'superscript':
              $this->endnote8_characterData(NULL, '</sup>');
              break;
            case 'subscript':
              $this->endnote8_characterData(NULL, '</sub>');
              break;
          }
        }
        $this->font_attr = array();
        break;
      default :
        $this->element = '';
    }


  }

  function endnote8_characterData($parser, $data) {
    // first replace any carriage returns with html line breaks
    $data = str_ireplace("\r", "<br/>", $data);
    if (trim(htmlspecialchars_decode($data))) {
      switch ($this->element) {
        //Author information
        case 'author' :
          $this->contributors[$this->contrib_count]['name'] .= $data;
          break;
        case 'keyword' :
          $this->node->biblio_keywords[$this->keyword_count] .= $data;
          break;
        case 'dates' :
          switch ($this->dates) {
            case 'year' :
              $this->node->biblio_year .= $data;
              break;
          }
          break;
        case 'date' :
          switch ($this->dates) {
            case 'pub-dates' :
              $this->node->biblio_date .= $data;
              break;
            case 'copyright-dates' :
              break;
          }
          break;
        case 'urls' :
        case 'url' :
          switch ($this->urls) {
            case 'web-urls' :
              $this->node->biblio_url .= $data;
              break;
            case 'pdf-urls' :
            case 'text-urls' :
            case 'image-urls' :
              break;
            case 'related-urls' :
          }
          break;
        case 'title':
          $this->node->title .= $data;
          break;
        default:
          if ($field = $this->field_map(trim($this->element))) {
            $this->node->$field .= $data;
          }
          else {
            if (!in_array($this->element, $this->unmapped)) {
              $this->unmapped[] = $this->element;
            }
          }
      }
    }
  }

  function endnote7_startElement($parser, $name, $attrs) {
    switch ($name) {
      case 'RECORD' :
        $this->node = new stdClass();
        $this->node->biblio_contributors = array();
        $this->node->biblio_type = 102; // we set 102 here because the xml parser won't
        // process a value of 0 (ZERO) which is the
        // ref-type 102. if there is a non-zero value it will be overwritten
        $this->element = '';
        break;
      case 'AUTHORS':
      case 'SECONDARY_AUTHORS':
      case 'TERTIARY_AUTHORS':
      case 'SUBSIDIARY_AUTHORS':
        $this->contrib_count = 0;
        $this->contributors = array();
        break;
      case 'AUTHOR':
      case 'SECONDARY_AUTHOR':
      case 'TERTIARY_AUTHOR':
      case 'SUBSIDIARY_AUTHOR':
          $this->contributors[$this->contrib_count]['name'] = '';
          $this->element = $name;
        break;
      case 'KEYWORDS':
        $this->keyword_count = 0;
        break;
      case 'KEYWORD':
        $this->node->biblio_keywords[$this->keyword_count] = '';
        $this->element = $name;
        break;
      default:
        $this->element = $name;
    }
  }

  function endnote7_endElement($parser, $name) {
    switch ($name) {
      case 'RECORD' :
        $this->node->biblio_xml_md5 = md5(serialize($this->node));
        if ( !($dup = $this->biblio_xml_check_md5($this->node->biblio_xml_md5)) ) {
          biblio_save_node($this->node, $this->terms, $this->batch_proc, $this->session_id);
          if (!empty($this->node->nid)) $this->nids[] = $this->node->nid;
        }
        else {
          $this->dups[] = $dup;
        }
        break;
      case 'AUTHORS':
      case 'SECONDARY_AUTHORS':
      case 'TERTIARY_AUTHORS':
      case 'SUBSIDIARY_AUTHORS':
        $this->contributors_type = '';
        foreach ($this->contributors as $contributor) {
          $this->node->biblio_contributors[] = $contributor;
        }
        break;
      case 'AUTHOR':
        $this->contributors[$this->contrib_count]['auth_category'] = 1;
        $this->contributors[$this->contrib_count]['auth_type'] = 1;
        $this->contrib_count++;
        break;
      case 'SECONDARY_AUTHOR':
        $this->contributors[$this->contrib_count]['auth_category'] = 2;
        $this->contributors[$this->contrib_count]['auth_type'] = 2;
        $this->contrib_count++;
        break;
      case 'TERTIARY_AUTHOR':
        $this->contributors[$this->contrib_count]['auth_category'] = 3;
        $this->contributors[$this->contrib_count]['auth_type'] = 3;
        $this->contrib_count++;
        break;
      case 'SUBSIDIARY_AUTHOR':
        $this->contributors[$this->contrib_count]['auth_category'] = 4;
        $this->contributors[$this->contrib_count]['auth_type'] = 4;
        $this->contrib_count++;
        break;
      case 'KEYWORD':
        $this->keyword_count++;
        break;
      default:

    }
    $this->element = '';
  }

  function endnote7_characterData($parser, $data) {
    if (trim($data)) {
      switch ($this->element) {
        case 'REFERENCE_TYPE':
          $this->node->biblio_type = $this->type_map($data);
          break;
        case 'AUTHOR':
        case 'SECONDARY_AUTHOR':
        case 'TERTIARY_AUTHOR':
        case 'SUBSIDIARY_AUTHOR':
          $this->contributors[$this->contrib_count]['name'] .= $data;
          break;
        case 'KEYWORD':
          $this->node->biblio_keywords[$this->keyword_count] .= $data;
          break;
        case 'TITLE':
          $this->node->title .= $data;
          break;
        default:
          if ($field = $this->field_map(trim($this->element))) {
            $this->node->$field .= $data;
          }
          else {
            if (!in_array($this->element, $this->unmapped)) {
              $this->unmapped[] = $this->element;
            }
          }
      }
    }
  }

  function field_map($enfield) {
    static $fmap = array();
    if (empty($fmap)) {
      $fmap = biblio_get_map('field_map', $this->format);
    }
    return (!empty($fmap[$enfield])) ? $fmap[$enfield] : '';
  }

  function type_map($entype) {
    static $map = array();
    if (empty($map)) {
      $map = biblio_get_map('type_map', $this->format);
    }
    return (isset($map[$entype]))?$map[$entype]:129; //return the biblio type or 129 (Misc) if type not found
  }

  function biblio_xml_check_md5($md5) {
    static $xml_md5s = array();
    if (empty($xml_md5s)) {
      $result = db_query("SELECT * FROM {biblio_xml} ");
      foreach ($result as $row) {
        $xml_md5s[$row->biblio_xml_md5] = $row->nid;
      }
    }
    if (isset($xml_md5s[$md5])) {
      return $xml_md5s[$md5];
    }
    else {
      $xml_md5s[$md5] = TRUE; // gaurd against duplicates in the same import
      return;
    }
  }

}