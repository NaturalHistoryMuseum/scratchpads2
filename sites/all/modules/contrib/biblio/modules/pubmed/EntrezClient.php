<?php
/**
 * @file EntrezClient.php
 * Provides Entrez client to retrieve items from the NCBI databases
 * Orginally writen by Stefan Freudenberg
 */

class BiblioEntrezClient
{
  const DEFAULT_DATABASE = 'pubmed';

  const BASE_URL = 'http://eutils.ncbi.nlm.nih.gov/entrez/eutils/';

  private $database = self::DEFAULT_DATABASE;

  private $useHistory = 'y';

  private $webEnvironment;

  private $queryKey;

  private $tool;

  private $email;

  private $term;

  private $dateRange;

  private $returnMax = 100;

  private $sort;

  private $query;

  private $count;

  /**
   * Sets a web environment.
   *
   * @param string $webEnvironment
   */
  public function setWebEnvironment($webEnvironment)
  {
    $this->webEnvironment = $webEnvironment;
  }

  /**
   * Returns the web environment from the previous ESearch results.
   *
   * This value may change with each utility call. If WebEnv is used, History
   * search numbers can be included in an ESummary URL, e.g.,
   * term=cancer+AND+%23X (where %23 replaces # and X is the History search
   * number).
   *
   * @return string
   */
  public function getWebEnvironment()
  {
    return $this->webEnvironment;
  }

  /**
   * Sets a history search number.
   *
   * @param int $key
   */
  public function setQueryKey($key)
  {
    $this->queryKey = $key;
  }

  /**
   * Returns the history search number from the previous ESearch results.
   *
   * @return int
   */
  public function getQueryKey()
  {
    return $this->queryKey;
  }

  /**
   * Sets the entrez database to be queried.
   *
   * Values available from EInfo, PubMed is the default db.
   *
   * @param string $database
   * @see getAvailableDatabases
   */
  public function setDatabase($database)
  {
    $this->database = strtolower($database);
  }

  /**
   * Returns the database to be queried in the next search.
   *
   * @return string
   */
  public function getDatabase()
  {
    return $this->database;
  }

  /**
   * Returns the available entrez databases from EInfo.
   *
   * @return array
   * @throws Exception
   */
  public function getAvailableDatabases()
  {
    $databases = array();

    $url = self::BASE_URL . 'einfo.fcgi';
    $result = @simplexml_load_file($url);

    if (!$result) {
      throw new Exception('Query ' . $url . ' failed.');
    }

    if (isset($result->DbList->DbName)) {
      foreach ($result->DbList->DbName as $name) {
        $databases[] = (string)$name;
      }
    }

    return $databases;
  }

  /**
   * Sets a string identifying the resource.
   *
   * A string with no internal spaces that identifies the resource which is
   * using Entrez links (e.g., tool=flybase). This argument is used to help
   * NCBI provide better service to third parties generating Entrez queries
   * from programs. As with any query system, it is sometimes possible to ask
   * the same question different ways, with different effects on performance.
   * NCBI requests that developers sending batch requests include a constant
   * 'tool' argument for all requests using the utilities.
   *
   * @param string $tool
   */
  public function setTool($tool)
  {
    $this->tool = str_replace(array(" ", "\n", "\r"), '', $tool);
  }

  /**
   * Returns the resource identifier.
   *
   * @return string
   */
  public function getTool()
  {
    return $this->tool;
  }

  /**
   * Sets a contact email address for NCBI.
   *
   * If you choose to provide an email address, we will use it to contact you
   * if there are problems with your queries or if we are changing software
   * interfaces that might specifically affect your requests. If you choose
   * not to include an email address we cannot provide specific help to you,
   * but you can still sign up for utilities-announce to receive general
   * announcements.
   *
   * @param string $email
   */
  public function setEmail($email)
  {
    $this->email = $email;
  }

  /**
   * Returns the NCBI contact email address.
   *
   * @return string
   */
  public function getEmail()
  {
    return $this->email;
  }

  /**
   * Sets the search terms for the next query.
   *
   * The search command uses terms or phrases with or without Boolean
   * operators.  See the PubMed or Entrez help for information about search
   * field descriptions and tags. Search fields and tags are database specific.
   *
   * @param string $term
   */
  public function setTerm($term)
  {
    $this->term = $term;
    $this->webEnvironment = NULL;
    $this->count = NULL;
  }

  /**
   * Returns the current search terms.
   *
   * @return string
   */
  public function getTerm()
  {
    return $this->term;
  }

  /**
   * Sets two specific dates bounding the results.
   *
   * @param $minDate
   * @param $maxDate
   * @throws Exception
   */
  public function setDateRange($minDate, $maxDate=null)
  {
    if (is_null($maxDate)) {
      $maxDate = date('Y/m/d');
    } else {
      $maxDate = date('Y/m/d', strtotime($maxDate));
    }

    $minDate = date('Y/m/d', strtotime($minDate));

    if ($maxDate < $minDate) {
      throw new Exception('First argument must be an earlier date.');
    }

    $this->dateRange = array($minDate, $maxDate);
  }

  /**
   * Returns the specified date range bounding the results.
   *
   * @return array
   *   a pair of dates
   */
  public function getDateRange()
  {
    return $this->dateRange;
  }

  /**
   * Returns the minimum date of the specified date range bounding the results.
   *
   * @return string
   */
  public function getMinDate()
  {
    return $this->dateRange[0];
  }

  /**
   * Returns the maximum date of the specified date range bounding the results.
   *
   * @return string
   */
  public function getMaxDate()
  {
    return $this->dateRange[1];
  }

  /**
   * Sets the maximum number of items retrieved by a search query.
   *
   * @param int $number
   * @see search
   */
  public function setReturnMax($number)
  {
    $this->returnMax = $number;
  }

  /**
   * Returns the maximum number of items retrieved by a search query.
   *
   * @return int
   */
  public function getReturnMax()
  {
    return $this->returnMax;
  }

  /**
   * Returns the URL of the last executed query.
   *
   * @return string
   */
  public function getLastQuery()
  {
    return $this->query;
  }

  /**
   * Returns up to the maximum number of items from the result set starting
   * at $retstart.
   *
   * If this is the first search for a given term a web environment and a query
   * key is retrieved from the NCBI server in addition to the result set.
   * See http://eutils.ncbi.nlm.nih.gov/corehtml/query/static/esearch_help.html
   *
   * @param int $retStart
   *   the sequential number of the first record retrieved - default=0
   *   which will retrieve the first record
   * @return SimpleXMLElement
   *   an array of PubMed IDs
   * @throws Exception
   * @see setReturnMax
   * @see setRelativeDate
   */
  public function search($retStart=0)
  {
    if (!is_null($this->webEnvironment)) {
      $params['WebEnv'] = $this->webEnvironment;
      $params['query_key'] = $this->queryKey;
    } else {
      $params['usehistory'] = $this->useHistory;
      $params['tool'] = $this->getTool();
      $params['email'] = $this->getEmail();
      $params['term'] = $this->getTerm();
    }

    if (isset($this->dateRange)) {
      $params['mindate'] = $this->getMinDate();
      $params['maxdate'] = $this->getMaxDate();
    }

    $params['retstart'] = $retStart;
    $params['retmax'] = $this->getReturnMax();
    $params['db'] = $this->getDatabase();

    $this->query = self::BASE_URL . 'esearch.fcgi?' . http_build_query($params);
    $result = @simplexml_load_file($this->query);

    if (!$result) {
      throw new Exception('Query ' . $this->query . ' failed.');
    }

    if (isset($result->WebEnv)) {
      $this->webEnvironment = (string)$result->WebEnv;
      $this->queryKey = (int)$result->QueryKey;
      $this->count = (int)$result->Count;
    }

    return $result;
  }

  /**
   * Returns the number of results for the previously set search terms.
   *
   * @return int
   * @throws Exception
   */
  public function count()
  {
    if (is_null($this->count)) {
      $params['tool'] = $this->getTool();
      $params['email'] = $this->getEmail();
      $params['db'] = $this->getDatabase();
      $params['term'] = $this->getTerm();
      $params['rettype'] = 'count';

      if (isset($this->dateRange)) {
        $params['mindate'] = $this->getMinDate();
        $params['maxdate'] = $this->getMaxDate();
      }

      $this->query = self::BASE_URL . 'esearch.fcgi?' . http_build_query($params);
      $result = @simplexml_load_file($this->query);

      if (!$result) {
        throw new Exception('Query ' . $this->query . ' failed.');
      }

      if (isset($result->Count)) {
        $this->count = (int)$result->Count;
      }
    }

    return $this->count;
  }

  /**
   * Returns the document identified by the given PubMed ID as a SimpleXMl
   * object. The root element is PubmedArticleSet.
   *
   * @param int $id
   * @return SimpleXMLElement
   */
  public function fetch($id)
  {
    $params['db'] = $this->getDatabase();
    $params['retmode'] = 'xml';
    $params['id'] = $id;

    $this->query = self::BASE_URL . 'efetch.fcgi?' . http_build_query($params);
    $request_options = array(
      'method' => 'POST');
    $result = drupal_http_request($this->query, $request_options);
    if ($result->code != 200) {
      throw new Exception('Query ' . $this->query . ' failed.');
    }
    $result = @simplexml_load_string($result->data);

    if (!$result) {
      throw new Exception('Query ' . $this->query . ' failed.');
    }

    return $result;
  }
  public function fetchSummaries($retStart=0) {
    return $this->fetchRecords($retStart, TRUE);
  }
  public function fetchResult($retStart=0) {
    return $this->fetchRecords($retStart);
  }

  /**
   * Returns up to the maximum number of results starting at $retstart
   * found by the previous search.
   *
   * In order to return results this method must be called after search. The
   * search method retrieves a web environment and query key from the NCBI
   * server which is used to fetch the results. After setting a new search term
   * the old web environment is deleted and search must be executed again
   * before utilizing this method.
   *
   * The root element of the returned SimpleXML object is PubmedArticleSet.
   *
   * @param $retStart
   *   the sequential number of the first record retrieved - default=0
   *   which will retrieve the first record
   * @return SimpleXMLElement
   * @throws Exception
   * @see search
   * @see setReturnMax
   */
  public function fetchRecords($retStart=0, $summaries = FALSE) {
    if (is_null($this->webEnvironment)) {
      throw new Exception(t('No web environment set.'));
    }

    $params['WebEnv'] = $this->webEnvironment;
    $params['query_key'] = $this->queryKey;
    $params['retstart'] = $retStart;
    $params['retmax'] = $this->getReturnMax();
    $params['db'] = $this->getDatabase();
    $params['retmode'] = 'xml';

    if (isset($this->dateRange)) {
      $params['mindate'] = $this->getMinDate();
      $params['maxdate'] = $this->getMaxDate();
    }
    if ($summaries) {
      $this->query = self::BASE_URL . 'esummary.fcgi?' . http_build_query($params);
    }
    else {
      $this->query = self::BASE_URL . 'efetch.fcgi?' . http_build_query($params);
    }
    $request_options = array('method' => 'POST');
    $result = drupal_http_request($this->query, $request_options);

    if ($result->code != 200) {
      throw new Exception('Query ' . $this->query . ' failed.');
    }

    $result = @simplexml_load_string($result->data);


    if (isset($result->body->pre->ERROR)) return FALSE;

    return $result;
  }

  public function post($uids) {
    $params['db'] = $this->getDatabase();
    $params['id'] = implode(',', $uids);
    $this->query = self::BASE_URL . 'epost.fcgi?' . http_build_query($params);
    $request_options = array('method' => 'POST');
    $result = drupal_http_request($this->query, $request_options);

    if ($result->code != 200) {
      throw new Exception('Query ' . $this->query . ' failed.');
    }

    $result = @simplexml_load_string($result->data);

    if (!$result) {
      throw new Exception('Query ' . $this->query . ' failed.');
    }

    if (isset($result->WebEnv)) {
      $this->webEnvironment = (string)$result->WebEnv;
      $this->queryKey = (int)$result->QueryKey;
      $this->count = (int)$result->Count;
    }

  }

}
