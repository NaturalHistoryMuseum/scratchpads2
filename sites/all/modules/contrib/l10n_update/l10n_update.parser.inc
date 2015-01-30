<?php

/**
 * @file
 * Extends the update parser to work with releases
 *
 * The update parser uses version tag to index releases. We will use 'language' and 'tag'
 *
 * @todo Parse languages too
 *
 * @todo Update the server side and get rid of this
 */
module_load_include('inc', 'l10n_update');

/**
 * Get server information
 */
function l10n_update_get_server($server) {
  // Fetch up to date information if available
  if (!empty($server['server_url']) && ($fetch = l10n_update_fetch_server($server['server_url']))) {
    $server = array_merge($server, $fetch);
  }
  // If we have an update url this is ok, otherwise we return none
  if (!empty($server['update_url'])) {
    return $server;
  }
  else {
    return FALSE;
  }
}

/**
 * Fetch remote server metadata from a server URL
 *
 * @param unknown_type $server_url
 * @return unknown_type
 */
function l10n_update_fetch_server($url) {
  $xml = l10n_update_http_request($url);
  if (isset($xml->data)) {
    $data[] = $xml->data;
    $parser = new l10n_update_xml_parser;
    return $parser->parse($xml->data);
  }
  else {
    return FALSE;
  }
}

/**
 * Parser for server metadata
 */
class l10n_update_xml_parser {
  var $current_language;
  var $current_server;
  var $current_languages;

  var $servers;


  /**
   * Parse an XML data file.
   *
   * It can contain information for one or more l10n_servers
   *
   * Example data, http://ftp.drupal.org/files/translations/l10n_server.xml
   */
  function parse($data) {
    $parser = xml_parser_create();
    xml_set_object($parser, $this);
    xml_set_element_handler($parser, 'start', 'end');
    xml_set_character_data_handler($parser, "data");
    xml_parse($parser, $data);
    xml_parser_free($parser);

    //return $this->servers;
    return $this->current_server;
  }

  function start($parser, $name, $attr) {
    $this->current_tag = $name;
    switch ($name) {
      case 'L10N_SERVER':
        unset($this->current_object);
        $this->current_server = array();
        $this->current_object = &$this->current_server;
        break;
      case 'LANGUAGES':
        unset($this->current_object);
        $this->current_languages = array();
        $this->current_object = &$this->current_languages;
        //$this->current_object = &$this->current_release;
        break;
      case 'LANGUAGE':
        unset($this->current_object);
        $this->current_language = array();
        $this->current_object = &$this->current_language;
        break;
    }
  }

  function end($parser, $name) {
    switch ($name) {
      case 'L10N_SERVER':
        unset($this->current_object);
        $this->servers[$this->current_server['name']] = $this->current_server;
        //$this->current_server = array();
        break;
      case 'LANGUAGE':
        unset($this->current_object);
        $this->current_languages[$this->current_language['code']] = $this->current_language;
        $this->current_language = array();
        break;
      case 'LANGUAGES':
        $this->current_server['languages'] = $this->current_languages;
        break;
      default:
        $this->current_object[strtolower($this->current_tag)] = trim($this->current_object[strtolower($this->current_tag)]);
        $this->current_tag = '';
    }
  }

  function data($parser, $data) {
    if ($this->current_tag && !in_array($this->current_tag, array('L10N_SERVER', 'LANGUAGES', 'LANGUAGE'))) {
      $tag = strtolower($this->current_tag);
      if (isset($this->current_object[$tag])) {
        $this->current_object[$tag] .= $data;
      }
      else {
        $this->current_object[$tag] = $data;
      }
    }
  }
}
