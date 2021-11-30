<?php

/**
 * Copyright (c) 2007-2009, Conduit Internet Technologies, Inc.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 *  - Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 *  - Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *  - Neither the name of Conduit Internet Technologies, Inc. nor the names of
 *    its contributors may be used to endorse or promote products derived from
 *    this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 *
 * @copyright Copyright 2007-2009 Conduit Internet Technologies, Inc. (http://conduit-it.com)
 * @license New BSD (http://solr-php-client.googlecode.com/svn/trunk/COPYING)
 * @version $Id: Service.php 22 2009-11-09 22:46:54Z donovan.jimenez $
 *
 * @package Apache
 * @subpackage Solr
 * @author Donovan Jimenez <djimenez@conduit-it.com>
 */

/**
 * Additional code Copyright (c) 2008-2011 by Robert Douglass, James McKinney,
 * Jacob Singh, Alejandro Garza, Peter Wolanin, Nick Veenhof and additional
 * contributors.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or (at
 * your option) any later version.

 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
 * or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License
 * for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program as the file LICENSE.txt; if not, please see
 * http://www.gnu.org/licenses/old-licenses/gpl-2.0.txt.
 */

/**
 * Starting point for the Solr API. Represents a Solr server resource and has
 * methods for pinging, adding, deleting, committing, optimizing and searching.
 */

class DrupalApacheSolrService implements DrupalApacheSolrServiceInterface {
  /**
   * How NamedLists should be formatted in the output.  This specifically effects facet counts. Valid values
   * are 'map' (default) or 'flat'.
   *
   */
  const NAMED_LIST_FORMAT = 'map';

  /**
   * Servlet mappings
   */
  const PING_SERVLET = 'admin/ping';
  const UPDATE_SERVLET = 'update';
  const SEARCH_SERVLET = 'select';
  const LUKE_SERVLET = 'admin/luke';
  const SYSTEM_SERVLET = 'admin/system';
  const STATS_SERVLET = 'admin/stats.jsp';
  const STATS_SERVLET_4 = 'admin/mbeans?wt=xml&stats=true';

  /**
   * Server url
   *
   * @var array
   */
  protected $parsed_url;

  /**
   * Constructed servlet full path URLs
   *
   * @var string
   */
  protected $update_url;

  /**
   * Default HTTP timeout when one is not specified (initialized to default_socket_timeout ini setting)
   *
   * var float
   */
  protected $_defaultTimeout;
  protected $env_id;
  protected $luke;
  protected $stats;
  protected $system_info;

  /**
   * Flag that denotes whether to use soft commits for Solr 4.x, defaults to FALSE.
   *
   * @var bool
   */
  protected $soft_commit = FALSE;

  /**
   * Call the /admin/ping servlet, to test the connection to the server.
   *
   * @param $timeout
   *   maximum time to wait for ping in seconds, -1 for unlimited (default 2).
   * @return
   *   (float) seconds taken to ping the server, FALSE if timeout occurs.
   */
  public function ping($timeout = 2) {
    $start = microtime(TRUE);

    if ($timeout <= 0.0) {
      $timeout = -1;
    }
    $pingUrl = $this->_constructUrl(self::PING_SERVLET);
    // Attempt a HEAD request to the solr ping url.
    $options = array(
      'method' => 'HEAD',
      'timeout' => $timeout,
    );
    $response = $this->_makeHttpRequest($pingUrl, $options);

    if ($response->code == 200) {
      // Add 0.1 ms to the ping time so we never return 0.0.
      return microtime(TRUE) - $start + 0.0001;
    }
    else {
      return FALSE;
    }
  }

  /**
   * Flags whether to use soft commits for Solr 4.x.
   *
   * @param bool $soft_commit
   *   Whether or not to use soft commits for Solr 4.x.
   */
  public function setSoftCommit($soft_commit) {
    $this->soft_commit = (bool) $soft_commit;
  }

  /**
   * Returns the flag that denotes whether to use soft commits for Solr 4.x.
   *
   * @return bool
   *   Whether to use soft commits for Solr 4.x.
   */
  public function getSoftCommit() {
    return $this->soft_commit;
  }

  /**
   * Call the /admin/system servlet
   *
   * @return
   *   (array) With all the system info
   */
  protected function setSystemInfo() {
    $url = $this->_constructUrl(self::SYSTEM_SERVLET, array('wt' => 'json'));
    if ($this->env_id) {
      $this->system_info_cid = $this->env_id . ":system:" . drupal_hash_base64($url);
      $cache = cache_get($this->system_info_cid, 'cache_apachesolr');
      if (isset($cache->data)) {
        $this->system_info = json_decode($cache->data);
      }
    }
    // Second pass to populate the cache if necessary.
    if (empty($this->system_info)) {
      $response = $this->_sendRawGet($url);
      $this->system_info = json_decode($response->data);
      if ($this->env_id) {
        cache_set($this->system_info_cid, $response->data, 'cache_apachesolr');
      }
    }
  }

  /**
   * Get information about the Solr Core.
   *
   * @return
   *   (string) system info encoded in json
   */
  public function getSystemInfo() {
    if (!isset($this->system_info)) {
      $this->setSystemInfo();
    }
    return $this->system_info;
  }

  /**
   * Sets $this->luke with the meta-data about the index from admin/luke.
   */
  protected function setLuke($num_terms = 0) {
    if (empty($this->luke[$num_terms])) {
      $params = array(
        'numTerms' => "$num_terms",
        'wt' => 'json',
        'json.nl' => self::NAMED_LIST_FORMAT,
      );
      $url = $this->_constructUrl(self::LUKE_SERVLET, $params);
      if ($this->env_id) {
        $cid = $this->env_id . ":luke:" . drupal_hash_base64($url);
        $cache = cache_get($cid, 'cache_apachesolr');
        if (isset($cache->data)) {
          $this->luke = $cache->data;
        }
      }
    }
    // Second pass to populate the cache if necessary.
    if (empty($this->luke[$num_terms])) {
      $this->luke[$num_terms] = $this->_sendRawGet($url);
      if ($this->env_id) {
        cache_set($cid, $this->luke, 'cache_apachesolr');
      }
    }
  }

  /**
   * Get just the field meta-data about the index.
   */
  public function getFields($num_terms = 0) {
    return $this->getLuke($num_terms)->fields;
  }

  /**
   * Get meta-data about the index.
   */
  public function getLuke($num_terms = 0) {
    if (!isset($this->luke[$num_terms])) {
      $this->setLuke($num_terms);
    }
    return $this->luke[$num_terms];
  }

  /**
   * Get the current solr version. This could be 1, 3 or 4
   *
   * @return int
   *   1, 3 or 4. Does not give a more details version, for that you need
   *   to get the system info.
   */
  public function getSolrVersion() {
    $system_info = $this->getSystemInfo();
    // Get our solr version number
    if (isset($system_info->lucene->{'solr-spec-version'})) {
      return $system_info->lucene->{'solr-spec-version'}[0];
    }
    return 0;
  }

  /**
   * Sets $this->stats with the information about the Solr Core form
   */
  protected function setStats() {
    $data = $this->getLuke();
    $solr_version = $this->getSolrVersion();
    // Only try to get stats if we have connected to the index.
    if (empty($this->stats) && isset($data->index->numDocs)) {
      if ($solr_version >= 4) {
        $url = $this->_constructUrl(self::STATS_SERVLET_4);
      }
      else {
        $url = $this->_constructUrl(self::STATS_SERVLET);
      }
      if ($this->env_id) {
        $this->stats_cid = $this->env_id . ":stats:" . drupal_hash_base64($url);
        $cache = cache_get($this->stats_cid, 'cache_apachesolr');
        if (isset($cache->data)) {
          $this->stats = simplexml_load_string($cache->data);
        }
      }
      // Second pass to populate the cache if necessary.
      if (empty($this->stats)) {
        $response = $this->_sendRawGet($url);
        $this->stats = simplexml_load_string($response->data);
        if ($this->env_id) {
          cache_set($this->stats_cid, $response->data, 'cache_apachesolr');
        }
      }
    }
  }

  /**
   * Get information about the Solr Core.
   *
   * Returns a Simple XMl document
   */
  public function getStats() {
    if (!isset($this->stats)) {
      $this->setStats();
    }
    return $this->stats;
  }

  /**
   * Get summary information about the Solr Core.
   */
  public function getStatsSummary() {
    $stats = $this->getStats();
    $solr_version = $this->getSolrVersion();

    $summary = array(
     '@pending_docs' => '',
     '@autocommit_time_seconds' => '',
     '@autocommit_time' => '',
     '@deletes_by_id' => '',
     '@deletes_by_query' => '',
     '@deletes_total' => '',
     '@schema_version' => '',
     '@core_name' => '',
     '@index_size' => '',
    );

    if (!empty($stats)) {
      if ($solr_version <= 3) {
        $docs_pending_xpath = $stats->xpath('//stat[@name="docsPending"]');
        $summary['@pending_docs'] = (int) trim(current($docs_pending_xpath));
        $max_time_xpath = $stats->xpath('//stat[@name="autocommit maxTime"]');
        $max_time = (int) trim(current($max_time_xpath));
        // Convert to seconds.
        $summary['@autocommit_time_seconds'] = $max_time / 1000;
        $summary['@autocommit_time'] = format_interval($max_time / 1000);
        $deletes_id_xpath = $stats->xpath('//stat[@name="deletesById"]');
        $summary['@deletes_by_id'] = (int) trim(current($deletes_id_xpath));
        $deletes_query_xpath = $stats->xpath('//stat[@name="deletesByQuery"]');
        $summary['@deletes_by_query'] = (int) trim(current($deletes_query_xpath));
        $summary['@deletes_total'] = $summary['@deletes_by_id'] + $summary['@deletes_by_query'];
        $schema = $stats->xpath('/solr/schema[1]');
        $summary['@schema_version'] = trim($schema[0]);
        $core = $stats->xpath('/solr/core[1]');
        $summary['@core_name'] = trim($core[0]);
        $size_xpath = $stats->xpath('//stat[@name="indexSize"]');
        $summary['@index_size'] = trim(current($size_xpath));
      }
      else {
        $system_info = $this->getSystemInfo();
        $docs_pending_xpath = $stats->xpath('//lst["stats"]/long[@name="docsPending"]');
        $summary['@pending_docs'] = (int) trim(current($docs_pending_xpath));
        $max_time_xpath = $stats->xpath('//lst["stats"]/str[@name="autocommit maxTime"]');
        $max_time = (int) trim(current($max_time_xpath));
        // Convert to seconds.
        $summary['@autocommit_time_seconds'] = $max_time / 1000;
        $summary['@autocommit_time'] = format_interval($max_time / 1000);
        $deletes_id_xpath = $stats->xpath('//lst["stats"]/long[@name="deletesById"]');
        $summary['@deletes_by_id'] = (int) trim(current($deletes_id_xpath));
        $deletes_query_xpath = $stats->xpath('//lst["stats"]/long[@name="deletesByQuery"]');
        $summary['@deletes_by_query'] = (int) trim(current($deletes_query_xpath));
        $summary['@deletes_total'] = $summary['@deletes_by_id'] + $summary['@deletes_by_query'];
        $schema = $system_info->core->schema;
        $summary['@schema_version'] = $schema;
        $core = $stats->xpath('//lst["core"]/str[@name="coreName"]');
        $summary['@core_name'] = trim(current($core));
        $size_xpath = $stats->xpath('//lst["core"]/str[@name="indexSize"]');
        $summary['@index_size'] = trim(current($size_xpath));
      }
    }

    return $summary;
  }

  /**
   * Clear cached Solr data.
   */
  public function clearCache() {
    // Don't clear cached data if the server is unavailable.
    if (@$this->ping()) {
      $this->_clearCache();
    }
    else {
      throw new Exception('No Solr instance available when trying to clear the cache.');
    }
  }

  protected function _clearCache() {
    if ($this->env_id) {
      cache_clear_all($this->env_id . ":stats:", 'cache_apachesolr', TRUE);
      cache_clear_all($this->env_id . ":luke:", 'cache_apachesolr', TRUE);
    }
    $this->luke = array();
    $this->stats = NULL;
  }

  /**
   * Constructor
   *
   * @param $url
   *   The URL to the Solr server, possibly including a core name.  E.g. http://localhost:8983/solr/
   *   or https://search.example.com/solr/core99/
   * @param $env_id
   *   The machine name of a corresponding saved configuration used for loading
   *   data like which facets are enabled.
   */
  public function __construct($url, $env_id = NULL) {
    $this->env_id = $env_id;
    $this->setUrl($url);

    // determine our default http timeout from ini settings
    $this->_defaultTimeout = (int) ini_get('default_socket_timeout');

    // double check we didn't get 0 for a timeout
    if ($this->_defaultTimeout <= 0) {
      $this->_defaultTimeout = 60;
    }
  }

  function getId() {
    return $this->env_id;
  }

  /**
   * Check the reponse code and thow an exception if it's not 200.
   *
   * @param stdClass $response
   *   response object.
   *
   * @return
   *  response object
   * @thows Exception
   */
  protected function checkResponse($response) {
    $code = (int) $response->code;
    if ($code != 200) {
      // Report where the user's code called the apachesolr code
      $caller = $this->findCaller();
      watchdog(
        'Apache Solr',
        t('Environment @env_id; HTTP Status: %http_status; <br>Message: %status_message; <br>Response: %response; <br>Request: %request; <br>Caller: %function (line %line of %file)'),
        array(
          '@env_id' => $this->getId(),
          '%http_status' => $code,
          '%status_message' => $response->status_message,
          '%response' => $response->data,
          '%request' => empty($response->request) ? t('Unknown') : $response->request,
          '%function' => isset($caller['class']) ? $caller['class'].'->'.$caller['function'].'()' : $caller['function'].'()',
          '%line' => $caller['line'],
          '%file' => $caller['file'],
        ),
        WATCHDOG_ERROR
      );
      throw new Exception('HTTP ' . $code . '; ' . $response->status_message);
    }
    return $response;
  }

  /**
   * Determine the routine that called this query.
   *
   * We define "the routine that called this query" as the first entry in
   * the call stack that is not inside /apachesolr/. That makes the climbing
   * logic very simple, and handles variable stack depth and hook functions.
   *
   * Copied from includes/database/log.inc
   *
   * @link http://www.php.net/debug_backtrace
   * @return
   *   This method returns a stack trace entry similar to that generated by
   *   debug_backtrace(). However, it flattens the trace entry and the trace
   *   entry before it so that we get the function and args of the function that
   *   called into the apachesolr module, not the function and args of the
   *   Solr call itself.
   */
  public function findCaller() {
    $stack = debug_backtrace();
    $stack_count = count($stack);
    for ($i = 0; $i < $stack_count; ++$i) {
      if (!isset($stack[$i]['file']) || strpos($stack[$i]['file'], DIRECTORY_SEPARATOR . 'apachesolr' . DIRECTORY_SEPARATOR) === FALSE) {
        return array(
          'file' => isset($stack[$i]['file']) ? $stack[$i]['file'] : t('Unknown'),
          'line' => isset($stack[$i]['line']) ? $stack[$i]['line'] : t('Unknown'),
          'function' => $stack[$i + 1]['function'],
          'class' => isset($stack[$i + 1]['class']) ? $stack[$i + 1]['class'] : NULL,
          'type' => isset($stack[$i + 1]['type']) ? $stack[$i + 1]['type'] : NULL,
          'args' => $stack[$i + 1]['args'],
        );
      }
    }
  }

  /**
   * Make a request to a servlet (a path) that's not a standard path.
   *
   * @param string $servlet
   *   A path to be added to the base Solr path. e.g. 'extract/tika'
   *
   * @param array $params
   *   Any request parameters when constructing the URL.
   *
   * @param array $options
   *  @see drupal_http_request() $options.
   *
   * @return
   *  response object
   *
   * @thows Exception
   */
  public function makeServletRequest($servlet, $params = array(), $options = array()) {
    // Add default params.
    $params += array(
      'wt' => 'json',
      'json.nl' => self::NAMED_LIST_FORMAT,
    );

    $url = $this->_constructUrl($servlet, $params);
    $response = $this->_makeHttpRequest($url, $options);
    return $this->checkResponse($response);
  }

  /**
   * Central method for making a GET operation against this Solr Server
   */
  protected function _sendRawGet($url, $options = array()) {
    $response = $this->_makeHttpRequest($url, $options);
    return $this->checkResponse($response);
  }

  /**
   * Central method for making a POST operation against this Solr Server
   */
  protected function _sendRawPost($url, $options = array()) {
    $options['method'] = 'POST';
    // Normally we use POST to send XML documents.
    if (!isset($options['headers']['Content-Type'])) {
      $options['headers']['Content-Type'] = 'text/xml; charset=UTF-8';
    }
    $response = $this->_makeHttpRequest($url, $options);
    return $this->checkResponse($response);
  }

  /**
   * Central method for making the actual http request to the Solr Server
   *
   * This is just a wrapper around drupal_http_request().
   */
  protected function _makeHttpRequest($url, array $options = array()) {
    if (!isset($options['method']) || $options['method'] == 'GET' || $options['method'] == 'HEAD') {
      // Make sure we are not sending a request body.
      $options['data'] = NULL;
    }

    $result = drupal_http_request($url, $options);
    if (empty($result->status_message)) {
      $result->status_message = '[unknown error]';
    }

    if (!isset($result->code) || $result->code < 0) {
      $result->code = 0;
      $result->status_message = 'Request failed';
      $result->protocol = 'HTTP/1.0';
    }
    // Additional information may be in the error property.
    if (isset($result->error)) {
      $result->status_message .= ': ' . check_plain($result->error);
    }

    if (!isset($result->data)) {
      $result->data = '';
      $result->response = NULL;
    }
    else {
      $response = json_decode($result->data);
      if (is_object($response)) {
        foreach ($response as $key => $value) {
          $result->$key = $value;
        }
      }
    }
    return $result;
  }


  /**
   * Escape a value for special query characters such as ':', '(', ')', '*', '?', etc.
   *
   * NOTE: inside a phrase fewer characters need escaped, use {@link DrupalApacheSolrService::escapePhrase()} instead
   *
   * @param string $value
   * @return string
   */
  static public function escape($value)
  {
    //list taken from http://lucene.apache.org/java/docs/queryparsersyntax.html#Escaping%20Special%20Characters
    $pattern = '/(\+|-|&&|\|\||!|\(|\)|\{|}|\[|]|\^|"|~|\*|\?|:|\\\)/';
    $replace = '\\\$1';

    return preg_replace($pattern, $replace, $value);
  }

  /**
   * Escape a value meant to be contained in a phrase for special query characters
   *
   * @param string $value
   * @return string
   */
  static public function escapePhrase($value)
  {
    $pattern = '/("|\\\)/';
    $replace = '\\\$1';

    return preg_replace($pattern, $replace, $value);
  }

  /**
   * Convenience function for creating phrase syntax from a value
   *
   * @param string $value
   * @return string
   */
  static public function phrase($value)
  {
    return '"' . self::escapePhrase($value) . '"';
  }

  /**
   * Return a valid http URL given this server's host, port and path and a provided servlet name
   *
   * @param $servlet
   *  A string path to a Solr request handler.
   * @param $params
   * @param $parsed_url
   *   A url to use instead of the stored one.
   *
   * @return string
   */
  protected function _constructUrl($servlet, $params = array(), $added_query_string = NULL) {
    // PHP's built in http_build_query() doesn't give us the format Solr wants.
    $query_string = $this->httpBuildQuery($params);

    if ($query_string) {
      $query_string = '?' . $query_string;
      if ($added_query_string) {
        $query_string = $query_string . '&' . $added_query_string;
      }
    }
    elseif ($added_query_string) {
      $query_string = '?' . $added_query_string;
    }

    $url = $this->parsed_url;
    return $url['scheme'] . $url['user'] . $url['pass'] . $url['host'] . $url['port'] . $url['path'] . $servlet . $query_string;
  }

  /**
   * Get the Solr url
   *
   * @return string
   */
  public function getUrl() {
    return $this->_constructUrl('');
  }

  /**
   * Set the Solr url.
   *
   * @param $url
   *
   * @return $this
   */
  public function setUrl($url) {
    $parsed_url = parse_url($url);

    if (!isset($parsed_url['scheme'])) {
      $parsed_url['scheme'] = 'http';
    }
    $parsed_url['scheme'] .= '://';

    if (!isset($parsed_url['user'])) {
      $parsed_url['user'] = '';
    }
    else {
      $parsed_url['host'] = '@' . $parsed_url['host'];
    }
    $parsed_url['pass'] = isset($parsed_url['pass']) ? ':' . $parsed_url['pass'] : '';
    $parsed_url['port'] = isset($parsed_url['port']) ? ':' . $parsed_url['port'] : '';

    if (isset($parsed_url['path'])) {
      // Make sure the path has a single leading/trailing slash.
      $parsed_url['path'] = '/' . ltrim($parsed_url['path'], '/');
      $parsed_url['path'] = rtrim($parsed_url['path'], '/') . '/';
    }
    else {
      $parsed_url['path'] = '/';
    }
    // For now we ignore query and fragment.
    $this->parsed_url = $parsed_url;
    // Force the update url to be rebuilt.
    unset($this->update_url);
    return $this;
  }

  /**
   * Raw update Method. Takes a raw post body and sends it to the update service. Post body
   * should be a complete and well formed xml document.
   *
   * @param string $rawPost
   * @param float $timeout Maximum expected duration (in seconds)
   *
   * @return response object
   *
   * @throws Exception If an error occurs during the service call
   */
  public function update($rawPost, $timeout = FALSE) {
    // @todo: throw exception if updates are disabled.
    if (empty($this->update_url)) {
      // Store the URL in an instance variable since many updates may be sent
      // via a single instance of this class.
      $this->update_url = $this->_constructUrl(self::UPDATE_SERVLET, array('wt' => 'json'));
    }
    $options['data'] = $rawPost;
    if ($timeout) {
      $options['timeout'] = $timeout;
    }
    return $this->_sendRawPost($this->update_url, $options);
  }

  /**
   * Add an array of Solr Documents to the index all at once
   *
   * @param array $documents Should be an array of ApacheSolrDocument instances
   * @param boolean $allowDups
   * @param boolean $overwritePending
   * @param boolean $overwriteCommitted
   *
   * @return response objecte
   *
   * @throws Exception If an error occurs during the service call
   */
  public function addDocuments($documents, $overwrite = NULL, $commitWithin = NULL) {
    $attr = '';

    if (isset($overwrite)) {
      $attr .= ' overwrite="' . empty($overwrite) ? 'false"' : 'true"';
    }
    if (isset($commitWithin)) {
      $attr .= ' commitWithin="' . intval($commitWithin) . '"';
    }

    $rawPost = "<add{$attr}>";
    foreach ($documents as $document) {
      if (is_object($document) && ($document instanceof ApacheSolrDocument)) {
        $rawPost .= ApacheSolrDocument::documentToXml($document);
      }
    }
    $rawPost .= '</add>';

    return $this->update($rawPost);
  }

  /**
   * Send a commit command.  Will be synchronous unless both wait parameters are set to false.
   *
   * @param boolean $optimize Defaults to true
   *   optimizes the index files. Only valid for solr versions <= 3
   * @param boolean $waitFlush
   *   block until index changes are flushed to disk. Only valid for solr versions <= 3
   * @param boolean $waitSearcher
   *   block until a new searcher is opened and registered as the main query searcher, making the changes visible.
   * @param float $timeout
   *   Maximum expected duration of the commit operation on the server (otherwise, will throw a communication exception)
   *
   * @return response object
   *
   * @throws Exception If an error occurs during the service call
   */
  public function commit($optimize = TRUE, $waitFlush = TRUE, $waitSearcher = TRUE, $timeout = 3600) {
    $optimizeValue = $optimize ? 'true' : 'false';
    $flushValue = $waitFlush ? 'true' : 'false';
    $searcherValue = $waitSearcher ? 'true' : 'false';
    $softCommit = $this->soft_commit ? 'true' : 'false';

    $solr_version = $this->getSolrVersion();
    if ($solr_version <= 3) {
      $rawPost = '<commit waitSearcher="' . $searcherValue . '" waitFlush="' . $flushValue . '" optimize="' . $optimizeValue . '" />';
    }
    else {
      $rawPost = '<commit waitSearcher="' . $searcherValue . '" softCommit="' . $softCommit . '" />';
    }

    $response = $this->update($rawPost, $timeout);
    $this->_clearCache();
    return $response;
  }

  /**
   * Create a delete document based on document ID
   *
   * @param string $id Expected to be utf-8 encoded
   * @param float $timeout Maximum expected duration of the delete operation on the server (otherwise, will throw a communication exception)
   *
   * @return response object
   *
   * @throws Exception If an error occurs during the service call
   */
  public function deleteById($id, $timeout = 3600) {
    return $this->deleteByMultipleIds(array($id), $timeout);
  }

  /**
   * Create and post a delete document based on multiple document IDs.
   *
   * @param array $ids Expected to be utf-8 encoded strings
   * @param float $timeout Maximum expected duration of the delete operation on the server (otherwise, will throw a communication exception)
   *
   * @return response object
   *
   * @throws Exception If an error occurs during the service call
   */
  public function deleteByMultipleIds($ids, $timeout = 3600) {
    $rawPost = '<delete>';

    foreach ($ids as $id) {
      $rawPost .= '<id>' . htmlspecialchars($id, ENT_NOQUOTES, 'UTF-8') . '</id>';
    }
    $rawPost .= '</delete>';

    return $this->update($rawPost, $timeout);
  }

  /**
   * Create a delete document based on a query and submit it
   *
   * @param string $rawQuery Expected to be utf-8 encoded
   * @param float $timeout Maximum expected duration of the delete operation on the server (otherwise, will throw a communication exception)
   * @return stdClass response object
   *
   * @throws Exception If an error occurs during the service call
   */
  public function deleteByQuery($rawQuery, $timeout = 3600) {
    $rawPost = '<delete><query>' . htmlspecialchars($rawQuery, ENT_NOQUOTES, 'UTF-8') . '</query></delete>';

    return $this->update($rawPost, $timeout);
  }

  /**
   * Send an optimize command.  Will be synchronous unless both wait parameters are set
   * to false.
   *
   * @param boolean $waitFlush
   *   block until index changes are flushed to disk  Removed in Solr 4.0
   * @param boolean $waitSearcher
   *   block until a new searcher is opened and registered as the main query searcher, making the changes visible.
   * @param float $timeout
   *   Maximum expected duration of the commit operation on the server (otherwise, will throw a communication exception)
   *
   * @return response object
   *
   * @throws Exception If an error occurs during the service call
   */
  public function optimize($waitFlush = TRUE, $waitSearcher = TRUE, $timeout = 3600) {
    $flushValue = $waitFlush ? 'true' : 'false';
    $searcherValue = $waitSearcher ? 'true' : 'false';
    $softCommit = $this->soft_commit ? 'true' : 'false';

    $solr_version = $this->getSolrVersion();
    if ($solr_version <= 3) {
      $rawPost = '<optimize waitSearcher="' . $searcherValue . '" waitFlush="' . $flushValue . '" />';
    }
    else {
      $rawPost = '<optimize waitSearcher="' . $searcherValue . '" softCommit="' . $softCommit . '" />';
    }

    return $this->update($rawPost, $timeout);
  }

  /**
   * Like PHP's built in http_build_query(), but uses rawurlencode() and no [] for repeated params.
   */
  protected function httpBuildQuery(array $query, $parent = '') {
    $params = array();

    foreach ($query as $key => $value) {
      $key = ($parent ? $parent : rawurlencode($key));

      // Recurse into children.
      if (is_array($value)) {
        $params[] = $this->httpBuildQuery($value, $key);
      }
      // If a query parameter value is NULL, only append its key.
      elseif (!isset($value)) {
        $params[] = $key;
      }
      else {
        $params[] = $key . '=' . rawurlencode($value);
      }
    }

    return implode('&', $params);
  }

  /**
   * Simple Search interface
   *
   * @param string $query The raw query string
   * @param array $params key / value pairs for other query parameters (see Solr documentation), use arrays for parameter keys used more than once (e.g. facet.field)
   *
   * @return response object
   *
   * @throws Exception If an error occurs during the service call
   */
  public function search($query = '', array $params = array(), $method = 'GET') {
    // Always use JSON. See http://code.google.com/p/solr-php-client/issues/detail?id=6#c1 for reasoning
    $params['wt'] = 'json';
    // Additional default params.
    $params += array(
      'json.nl' => self::NAMED_LIST_FORMAT,
    );
    if ($query) {
      $params['q'] = $query;
    }
    // PHP's built in http_build_query() doesn't give us the format Solr wants.
    $queryString = $this->httpBuildQuery($params);
    // Check string length of the query string, change method to POST
    $len = strlen($queryString);
    // Fetch our threshold to find out when to flip to POST
    $max_len = apachesolr_environment_variable_get($this->env_id, 'apachesolr_search_post_threshold', 3600);

    // if longer than $max_len (default 3600) characters
    // we should switch to POST (a typical server handles 4096 max).
    // If this class is used independently (without environments), we switch automatically to POST at an
    // limit of 1800 chars.
    if (($len > 1800) && (empty($this->env_id) || ($len > $max_len))) {
      $method = 'POST';
    }

    if ($method == 'GET') {
      $searchUrl = $this->_constructUrl(self::SEARCH_SERVLET, array(), $queryString);
      return $this->_sendRawGet($searchUrl);
    }
    else if ($method == 'POST') {
      $searchUrl = $this->_constructUrl(self::SEARCH_SERVLET);
      $options['data'] = $queryString;
      $options['headers']['Content-Type'] = 'application/x-www-form-urlencoded; charset=UTF-8';
      return $this->_sendRawPost($searchUrl, $options);
    }
    else {
      throw new Exception("Unsupported method '$method' for search(), use GET or POST");
    }
  }
}
