<?php

/**
 * @file
 *   Dummy object to simulate a Solr Service
 *
 */
class DummySolr implements DrupalApacheSolrServiceInterface {

  function getId() {
    return __CLASS__;
  }

  function getFields($num_terms = 0) {
    return (object) array(
       'is_uid' =>
      (object) array(
         'type' => 'long',
         'schema' => 'I-S----OF-----',
      ),
       'content' =>
      (object) array(
         'type' => 'text',
         'schema' => 'ITS-V---------',
      ),
       'im_3_field_tags' =>
      (object) array(
         'type' => 'long',
         'schema' => 'I-SM---OF-----',
         'dynamicBase' => 'im_*',
      ),
       'entity_type' =>
      (object) array(
         'type' => 'string',
         'schema' => 'I-S----OF----l',
      ),
       'ds_last_comment_or_change' =>
      (object) array(
         'type' => 'tdate',
         'schema' => 'ITS----OF-----',
      ),
       'nodeaccess_ari4jj_node_access_example_view' =>
      (object) array(
         'type' => 'integer',
         'schema' => 'I--M---OF-----',
         'dynamicBase' => 'nodeaccess*',
      ),
       'entity_id' =>
      (object) array(
         'type' => 'tlong',
         'schema' => 'ITS----OF-----',
      ),
       'ds_changed' =>
      (object) array(
         'type' => 'tdate',
         'schema' => 'ITS----OF-----',
      ),
       'id' =>
      (object) array(
         'type' => 'string',
         'schema' => 'I-S----OF----l',
      ),
       'timestamp' =>
      (object) array(
         'type' => 'date',
         'schema' => 'I-S----OF----l',
      ),
       'label' =>
      (object) array(
         'type' => 'text',
         'schema' => 'ITS-V--O------',
      ),
       'nodeaccess_ari4jj_node_access_example_edit' =>
      (object) array(
         'type' => 'integer',
         'schema' => 'I--M---OF-----',
         'dynamicBase' => 'nodeaccess*',
      ),
       'ds_created' =>
      (object) array(
         'type' => 'tdate',
         'schema' => 'ITS----OF-----',
      ),
       'ss_name' =>
      (object) array(
         'type' => 'text',
         'schema' => 'ITS-V---------',
      ),
       'path' =>
      (object) array(
         'type' => 'string',
         'schema' => 'I-S----OF----l',
      ),
       'taxonomy_names' =>
      (object) array(
         'type' => 'text',
         'schema' => 'IT-MV--O------',
      ),
       'bundle' =>
      (object) array(
         'type' => 'string',
         'schema' => 'I-S----OF----l',
      ),
       'tid' =>
      (object) array(
         'type' => 'long',
         'schema' => 'I-SM---OF-----',
      ),
       'is_tnid' =>
      (object) array(
         'type' => 'long',
         'schema' => 'I-S----OF-----',
      ),
       'nodeaccess_ari4jj_node_access_example_author' =>
      (object) array(
         'type' => 'integer',
         'schema' => 'I--M---OF-----',
         'dynamicBase' => 'nodeaccess*',
      ),
       'tm_vid_1_names' =>
      (object) array(
         'type' => 'text',
         'schema' => 'ITSMV---------',
         'dynamicBase' => 'tm_*',
      ),
       'spell' =>
      (object) array(
         'type' => 'textSpell',
         'schema' => 'ITSM----------',
      ),
       'site' =>
      (object) array(
         'type' => 'string',
         'schema' => 'I-S----OF----l',
      ),
       'is_comment_count' =>
      (object) array(
         'type' => 'tint',
         'schema' => 'ITS----OF-----',
      ),
       'bundle_name' =>
      (object) array(
         'type' => 'string',
         'schema' => 'I-S----OF----l',
      ),
       'hash' =>
      (object) array(
         'type' => 'string',
         'schema' => 'I-S----OF----l',
      ),
       'bs_status' =>
      (object) array(
         'type' => 'boolean',
         'schema' => 'I-S----OF----l',
      ),
       'entity_id' =>
      (object) array(
         'type' => 'long',
         'schema' => 'I-S----OF-----',
      ),
       'url' =>
      (object) array(
         'type' => 'string',
         'schema' => 'I-S----OF----l',
      ),
       'nodeaccess_all' =>
      (object) array(
         'type' => 'integer',
         'schema' => 'I--M---OF-----',
         'dynamicBase' => 'nodeaccess*',
      ),
       'sort_name' =>
      (object) array(
         'type' => 'sortString',
         'schema' => 'IT-----O-----l',
      ),
       'tags_a' =>
      (object) array(
         'type' => 'text',
         'schema' => 'IT-----O------',
         'dynamicBase' => 'tags_*',
      ),
       'bs_sticky' =>
      (object) array(
         'type' => 'boolean',
         'schema' => 'I-S----OF----l',
      ),
       'bs_promote' =>
      (object) array(
         'type' => 'boolean',
         'schema' => 'I-S----OF----l',
      ),
       'teaser' =>
      (object) array(
         'type' => 'text',
         'schema' => '-TS-----------',
      ),
       'im_vid_1' =>
      (object) array(
         'type' => 'long',
         'schema' => 'I-SM---OF-----',
         'dynamicBase' => 'im_*',
      ),
       'bs_translate' =>
      (object) array(
         'type' => 'boolean',
         'schema' => 'I-S----OF----l',
      ),
       'sort_label' =>
      (object) array(
         'type' => 'sortString',
         'schema' => 'IT-----O-----l',
      ),
       'ss_language' =>
      (object) array(
         'type' => 'string',
         'schema' => 'I-S----OF----l',
      ),
       'sm_vid_Tags' =>
      (object) array(
         'type' => 'string',
         'schema' => 'I-SM---OF----l',
         'dynamicBase' => 'sm_*',
      ),
    );
  }

  protected $last_search = array();

  public function search($query = '', array $params = array(), $method = 'GET') {
    $this->last_search = array('query' => $query, 'params' => $params, 'method' => $method);
    $response = new stdClass();
    $response->response = new stdClass();
    $response->response->numFound = 0;
    $response->response->docs = array();

    return $response;
  }

  public function getLastSearch() {
    return $this->last_search;
  }

  /**
   * Call the /admin/ping servlet, to test the connection to the server.
   *
   * @param $timeout
   *   maximum time to wait for ping in seconds, -1 for unlimited (default 2).
   * @return
   *   (float) seconds taken to ping the server, FALSE if timeout occurs.
   */
  function ping($timeout = 2) {
  }

  /**
   * Get information about the Solr Core.
   *
   * @return
   *   (string) system info encoded in json
   */
  function getSystemInfo() {
  }

  /**
   * Get meta-data about the index.
   */
  function getLuke($num_terms = 0) {
  }

  /**
   * Get information about the Solr Core.
   *
   * Returns a Simple XMl document
   */
  function getStats() {
  }

  /**
   * Get summary information about the Solr Core.
   */
  function getStatsSummary() {
  }

  /**
   * Clear cached Solr data.
   */
  function clearCache() {
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
  function __construct($url, $env_id = NULL) {
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
  function makeServletRequest($servlet, $params = array(), $options = array()) {
  }

  /**
   * Get the Solr url
   *
   * @return string
   */
  function getUrl() {
  }

  /**
   * Set the Solr url.
   *
   * @param $url
   *
   * @return $this
   */
  function setUrl($url) {
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
  function update($rawPost, $timeout = FALSE) {
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
  function addDocuments($documents, $overwrite = NULL, $commitWithin = NULL) {
  }

  /**
   * Send a commit command.  Will be synchronous unless both wait parameters are set to false.
   *
   * @param boolean $optimize Defaults to true
   * @param boolean $waitFlush Defaults to true
   * @param boolean $waitSearcher Defaults to true
   * @param float $timeout Maximum expected duration (in seconds) of the commit operation on the server (otherwise, will throw a communication exception). Defaults to 1 hour
   * @param boolean $softCommit optimize by using a softCommit
   *
   * @return response object
   *
   * @throws Exception If an error occurs during the service call
   */
  function commit($optimize = TRUE, $waitFlush = TRUE, $waitSearcher = TRUE, $timeout = 3600, $softCommit = FALSE) {
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
  function deleteById($id, $timeout = 3600) {
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
  function deleteByMultipleIds($ids, $timeout = 3600) {
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
  function deleteByQuery($rawQuery, $timeout = 3600) {
  }

  /**
   * Send an optimize command.  Will be synchronous unless both wait parameters are set
   * to false.
   *
   * @param boolean $waitFlush
   * @param boolean $waitSearcher
   * @param float $timeout Maximum expected duration of the commit operation on the server (otherwise, will throw a communication exception)
   * @param boolean $softCommit optimize by using a softCommit
   *
   * @return response object
   *
   * @throws Exception If an error occurs during the service call
   */
  function optimize($waitFlush = TRUE, $waitSearcher = TRUE, $timeout = 3600, $softCommit = FALSE) {
  }

  /**
   * Get the current solr version. This could be 1, 3 or 4
   *
   * @return int
   *   1, 3 or 4. Does not give a more details version, for that you need
   *   to get the system info.
   */
  function getSolrVersion() {
  }

  /**
   * Get query name.
   */
  function getName() {
  }

  /**
   * Get query searcher name (for facetapi, views, pages, etc).
   */
  function getSearcher() {
  }

  /**
   * Get context values.
   */
  function getContext() {
  }

  /**
   * Set context value.
   */
  function addContext(array $context) { 
  }
}

