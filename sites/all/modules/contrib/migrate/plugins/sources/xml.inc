<?php

/**
 * @file
 * Support for migration from XML sources.
 *
 * NOTE: There are two methods supported in this file.
 *
 * 1) List - ids are listed in an index xml file and the data for each item is
 *      stored in a separate xml file per item. Use MigrateSourceList class
 *      as the source.
 *
 * 2) MultiItems - ids are part of the item and all items are stored in a
 *      single xml file. Use MigrateSourceMultiItems class as the source.
 *
 * Both of these methods are described in more detail in the wine migration
 * example.
 */

/* =========================================================================== */
/*                              List Method                                    */
/* =========================================================================== */
/**
 * Implementation of MigrateList, for retrieving a list of IDs to be migrated
 * from an XML document.
 */
class MigrateListXML extends MigrateList {
  /**
   * A URL pointing to an XML document containing a list of IDs to be processed.
   *
   * @var string
   */
  protected $listUrl;

  public function __construct($list_url) {
    parent::__construct();
    $this->listUrl = $list_url;
    // Suppress errors during parsing, so we can pick them up after
    libxml_use_internal_errors(TRUE);
  }

  /**
   * Our public face is the URL we're getting items from
   *
   * @return string
   */
  public function __toString() {
    return $this->listUrl;
  }

  /**
   * Load the XML at the given URL, and return an array of the IDs found within it.
   *
   * @return array
   */
  public function getIdList() {
    migrate_instrument_start("Retrieve $this->listUrl");
    $xml = simplexml_load_file($this->listUrl);
    migrate_instrument_stop("Retrieve $this->listUrl");
    if ($xml) {
      return $this->getIDsFromXML($xml);
    }
    else {
      Migration::displayMessage(t(
        'Loading of !listUrl failed:',
        array('!listUrl' => $this->listUrl)
      ));
      foreach (libxml_get_errors() as $error) {
        Migration::displayMessage(MigrateItemsXML::parseLibXMLError($error));
      }
      return NULL;
    }
  }

  /**
   * Given an XML object, parse out the IDs for processing and return them as an
   * array. The default implementation assumes the IDs are simply the values of
   * the top-level elements - in most cases, you will need to override this to
   * reflect your particular XML structure.
   *
   * @param SimpleXMLElement $xml
   *
   * @return array
   */
  protected function getIDsFromXML(SimpleXMLElement $xml) {
    $ids = array();
    foreach ($xml as $element) {
      $ids[] = (string)$element;
    }
    return array_unique($ids);
  }

  /**
   * Return a count of all available IDs from the source listing. The default
   * implementation assumes the count of top-level elements reflects the number
   * of IDs available - in many cases, you will need to override this to reflect
   * your particular XML structure.
   */
  public function computeCount() {
    $xml = simplexml_load_file($this->listUrl);
    // Number of sourceid elements beneath the top-level element
    $count = count($xml);
    return $count;
  }
}

/**
 * Implementation of MigrateItem, for retrieving a parsed XML document given
 * an ID provided by a MigrateList class.
 */
class MigrateItemXML extends MigrateItem {
  /**
   * A URL pointing to an XML document containing the data for one item to be
   * migrated.
   *
   * @var string
   */
  protected $itemUrl;

  public function __construct($item_url) {
    parent::__construct();
    $this->itemUrl = $item_url;
    // Suppress errors during parsing, so we can pick them up after
    libxml_use_internal_errors(TRUE);
  }

  /**
   * Implementors are expected to return an object representing a source item.
   *
   * @param mixed $id
   *
   * @return stdClass
   */
  public function getItem($id) {
    // Make sure we actually have an ID
    if (empty($id)) {
      return NULL;
    }
    $item_url = $this->constructItemUrl($id);
    // And make sure we actually got a URL to fetch
    if (empty($item_url)) {
      return NULL;
    }
    // Get the XML object at the specified URL;
    $xml = $this->loadXmlUrl($item_url);
    if ($xml) {
      $return = new stdclass;
      $return->xml = $xml;
      return $return;
    }
    else {
      $migration = Migration::currentMigration();
      $message =  t('Loading of !objecturl failed:', array('!objecturl' => $item_url));
      foreach (libxml_get_errors() as $error) {
        $message .= "\n" . $error->message;
      }
      $migration->getMap()->saveMessage(
        array($id), $message, MigrationBase::MESSAGE_ERROR);
      libxml_clear_errors();
      return NULL;
    }
  }

  /**
   * The default implementation simply replaces the :id token in the URL with
   * the ID obtained from MigrateListXML. Override if the item URL is not
   * so easily expressed from the ID.
   *
   * @param mixed $id
   */
  protected function constructItemUrl($id) {
    return str_replace(':id', $id, $this->itemUrl);
  }

  /**
   * Default XML loader - just use Simplexml directly. This can be overridden for
   * preprocessing of XML (removal of unwanted elements, caching of XML if the
   * source service is slow, etc.)
   */
  protected function loadXmlUrl($item_url) {
    return simplexml_load_file($item_url);
  }
}

/**
 * Adds xpath info to field mappings for XML sources
 */
class MigrateXMLFieldMapping extends MigrateFieldMapping {
  /**
   * The xpath used to retrieve the data for this field from the XML.
   *
   * @var string
   */
  protected $xpath;
  public function getXpath() {
    return $this->xpath;
  }

  /**
   * Add an xpath to this field mapping
   *
   * @param string $xpath
   */
  public function xpath($xpath) {
    $this->xpath = $xpath;
    return $this;
  }
}

/**
 * Migrations using XML sources should extend this class instead of Migration.
 */
abstract class XMLMigration extends Migration {
  /**
   * Override the default addFieldMapping(), so we can create our special
   * field mapping class.
   * TODO: Find a cleaner way to just substitute a different mapping class
   *
   * @param string $destinationField
   *  Name of the destination field.
   * @param string $sourceField
   *  Name of the source field (optional).
   */
  public function addFieldMapping($destination_field, $source_field = NULL) {
    // Warn of duplicate mappings
    if (!is_null($destination_field) && isset($this->fieldMappings[$destination_field])) {
      self::displayMessage(
        t('!name addFieldMapping: !dest was previously mapped, overridden',
          array('!name' => $this->machineName, '!dest' => $destination_field)),
        'warning');
    }
    $mapping = new MigrateXMLFieldMapping($destination_field, $source_field);
    if (is_null($destination_field)) {
      $this->fieldMappings[] = $mapping;
    }
    else {
      $this->fieldMappings[$destination_field] = $mapping;
    }
    return $mapping;
  }

  /**
   * A normal $data_row has all the input data as top-level fields - in this
   * case, however, the data is embedded within a SimpleXMLElement object in
   * $data_row->xml. Explode that out to the normal form, and pass on to the
   * normal implementation.
   */
  protected function applyMappings() {
    // We only know what data to pull from the xpaths in the mappings.
    foreach ($this->fieldMappings as $mapping) {
      $source = $mapping->getSourceField();
      if ($source) {
        $xpath = $mapping->getXpath();
        if ($xpath) {
          // Derived class may override applyXpath()
          $this->sourceValues->$source = $this->applyXpath($this->sourceValues, $xpath);
        }
      }
    }
    parent::applyMappings();
  }

  /**
   * Default implementation - straightforward xpath application
   *
   * @param $data_row
   * @param $xpath
   */
  public function applyXpath($data_row, $xpath) {
    $result = $data_row->xml->xpath($xpath);
    if ($result) {
      if (count($result) > 1) {
        $return = array();
        foreach ($result as $record) {
          $return[] = (string)$record;
        }
        return $return;
      }
      else {
        return (string)$result[0];
      }
    }
    else {
      return NULL;
    }
  }
}

/* =========================================================================== */
/*                            MultiItems Method                                */
/* =========================================================================== */
/**
 * Implementation of MigrateItems, for providing a list of IDs and for
 * retrieving a parsed XML document given an ID from this list.
 */
class MigrateItemsXML extends MigrateItems {
  /**
   * A URL pointing to an XML document containing the ids and data.
   *
   * @var string
   */
  protected $xmlUrl;

  /**
   * Stores the loaded XML document.
   *
   * @var SimpleXMLElement
   */
  protected $xml = FALSE;

  /**
   * xpath identifying the element used for each item
   */
  protected $itemXpath;
  public function getItemXpath() {
    return $this->itemXpath;
  }

  /**
   * xpath identifying the subelement under itemXpath that holds the id for
   * each item.
   */
  protected $itemIDXpath;
  public function getIDXpath() {
    return $this->itemIDXpath;
  }

  public function __construct($xml_url, $item_xpath='item', $itemID_xpath='id') {
    parent::__construct();
    $this->xmlUrl = $xml_url;
    $this->itemXpath = $item_xpath;
    $this->itemIDXpath = $itemID_xpath;

    // Suppress errors during parsing, so we can pick them up after
    libxml_use_internal_errors(TRUE);
  }

  /**
   * Our public face is the URL we're getting items from
   *
   * @return string
   */
  public function __toString() {
    return 'url = ' . $this->xmlUrl . ' | item xpath = ' . $this->itemXpath .
                                      ' | item ID xpath = ' . $this->itemIDXpath;
  }

  /**
   * Load and return the xml from the defined xmlUrl.
   * @return SimpleXMLElement
   */
  public function &xml() {
    if (!$this->xml && !empty($this->xmlUrl)) {
      $this->xml = simplexml_load_file($this->xmlUrl);
      if (!$this->xml) {
        Migration::displayMessage(t(
          'Loading of !xmlUrl failed:',
          array('!xmlUrl' => $this->xmlUrl)
        ));
        foreach (libxml_get_errors() as $error) {
          Migration::displayMessage(self::parseLibXMLError($error));
        }
      }
    }
    return $this->xml;
  }

  /**
   * Parses a LibXMLError to a error message string.
   * @param LibXMLError $error
   * @return string
   */
  public static function parseLibXMLError(LibXMLError $error) {
    $error_code_name = 'Unknown Error';
    switch ($error->level) {
      case LIBXML_ERR_WARNING:
        $error_code_name = t('Warning');
        break;
      case LIBXML_ERR_ERROR:
        $error_code_name = t('Error');
        break;
      case LIBXML_ERR_FATAL:
        $error_code_name = t('Fatal Error');
        break;
    }
    return t(
       "!libxmlerrorcodename !libxmlerrorcode: !libxmlerrormessage\n" .
      "Line: !libxmlerrorline\n" .
      "Column: !libxmlerrorcolumn\n" .
      "File: !libxmlerrorfile",
      array(
        '!libxmlerrorcodename' => $error_code_name,
        '!libxmlerrorcode' => $error->code,
        '!libxmlerrormessage' => trim($error->message),
        '!libxmlerrorline' => $error->line,
        '!libxmlerrorcolumn' => $error->column,
        '!libxmlerrorfile' => (($error->file)) ? $error->file : NULL,
      )
    );
  }

  /**
   * Load the XML at the given URL, and return an array of the IDs found
   * within it.
   *
   * @return array
   */
  public function getIdList() {
    migrate_instrument_start("Retrieve $this->xmlUrl");
    $xml = $this->xml();
    migrate_instrument_stop("Retrieve $this->xmlUrl");
    if ($xml) {
      return $this->getIDsFromXML($xml);
    }
    return NULL;
  }

  /**
   * Given an XML object, parse out the IDs for processing and return them as
   * an array. The location of the IDs in the XML are based on the item xpath
   * and item ID xpath set in the constructor.
   *    eg, xpath = itemXpath . '/' . itemIDXpath
   * IDs are cached.  The list of IDs are returned from the cache except when
   * this is the first call (ie, cache is NULL) OR the refresh parameter is
   * TRUE.
   *
   * @param SimpleXMLElement $xml
   * @param boolean $refresh
   *
   * @return array
   */
  protected $cache_ids = NULL;
  protected function getIDsFromXML(SimpleXMLElement $xml, $refresh = FALSE) {
    if ($refresh !== TRUE && $this->cache_ids != NULL) {
      return $this->cache_ids;
    }

    $this->cache_ids = NULL;
    $result = $xml->xpath($this->itemXpath);

    $ids = array();
    if ($result) {
      foreach ($result as $element) {
        $id = $this->getItemID($element);
        if (!is_null($id)) {
          $ids[] = (string)$id;
        }
      }
    }
    $this->cache_ids = array_unique($ids);
    return $this->cache_ids;
  }

  /**
   * Return a count of all available IDs from the source listing.
   */
  public function computeCount() {
    $count = 0;
    $xml = $this->xml();
    if ($xml) {
      $ids = $this->getIDsFromXML($xml, TRUE);
      $count = count($ids);
    }
    return $count;
  }

  /**
   * Load the XML at the given URL, and return an array of the Items found
   * within it.
   *
   * @return array
   */
  public function getAllItems() {
    $xml = $this->xml();
    if ($xml) {
      return $this->getItemsFromXML($xml);
    }
    return NULL;
  }

  /**
   * Given an XML object, parse out the items for processing and return them as
   * an array. The location of the items in the XML are based on the item xpath
   * set in the constructor.  Items are cached.  The list of items are returned
   * from the cache except when this is the first call (ie, cache is NULL) OR
   * the refresh parameter is TRUE.
   *
   * Items are cached as an array of key=ID and value=stdclass object with
   * attribute xml containing the xml SimpleXMLElement object of the item.
   *
   * @param SimpleXMLElement $xml
   * @param boolean $refresh
   *
   * @return array
   */
  protected $cache_items = NULL;
  public function getItemsFromXML(SimpleXMLElement $xml, $refresh=FALSE) {
    if ($refresh !== FALSE && $this->cache_items != NULL) {
      return $this->cache_items;
    }

    $this->cache_items = NULL;
    $items = array();
    $result = $xml->xpath($this->itemXpath);

    if ($result) {
      foreach ($result as $item_xml) {
        $id = $this->getItemID($item_xml);
        $item = new stdclass;
        $item->xml = $item_xml;
        $items[$id] = $item;
      }
      $this->cache_items = $items;
      return $items;
    }
    else {
      return NULL;
    }
  }

  /**
   * Get the item ID from the itemXML based on itemIDXpath.
   *
   * @return string
   */
  protected function getItemID($itemXML) {
    return $this->getElementValue($itemXML, $this->itemIDXpath);
  }

  /**
   * Get an element from the itemXML based on an xpath.
   *
   * @return string
   */
  protected function getElementValue($itemXML, $xpath) {
    $value = NULL;
    if ($itemXML) {
      $result = $itemXML->xpath($xpath);
      if ($result)
        $value = (string)$result[0];
    }
    return $value;
  }

  /**
   * Implementors are expected to return an object representing a source item.
   * Items are cached as an array of key=ID and value=stdclass object with
   * attribute xml containing the xml SimpleXMLElement object of the item.
   *
   * @param mixed $id
   *
   * @return stdClass
   */
  public function getItem($id) {
    // Make sure we actually have an ID
    if (empty($id)) {
      return NULL;
    }
    $items = $this->getAllItems();
    $item = $items[$id];
    if ($item) {
      return $item;
    }
    else {
      $migration = Migration::currentMigration();
      $message =  t('Loading of item XML for ID !id failed:', array('!id' => $id));
      foreach (libxml_get_errors() as $error) {
        $message .= "\n" . $error->message;
      }
      $migration->getMap()->saveMessage(
        array($id), $message, MigrationBase::MESSAGE_ERROR);
      libxml_clear_errors();
      return NULL;
    }
  }
}

/**
 * Makes an XMLReader object iterable, returning elements matching a restricted
 * xpath-like syntax.
 */
class MigrateXMLReader implements Iterator {

  /**
   * The XMLReader we are encapsulating.
   *
   * @var XMLReader
   */
  public $reader;

  /**
   * URL of the source XML file.
   *
   * @var string
   */
  public $url;

  /**
   * Array of the element names from the query, 0-based from the first (root)
   * element. For example, '//file/article' would be stored as
   * array(0 => 'file', 1 => 'article').
   *
   * @var array
   */
  protected $elementsToMatch = array();

  /**
   * If the element query is filtering by an attribute name=value, the name of
   * the attribute in question.
   *
   * @var string
   */
  protected $attributeName = NULL;

  /**
   * If the element query is filtering by an attribute name=value, the value of
   * the attribute in question.
   *
   * @var string
   */
  protected $attributeValue = NULL;

  /**
   * Array representing the path to the current element as we traverse the XML.
   * For example, if in an XML string like '<file><article>...</article></file>'
   * we are positioned within the article element, currentPath will be
   * array(0 => 'file', 1 => 'article').
   *
   * @var array
   */
  protected $currentPath = array();

  /**
   * Query string used to retrieve the elements from the XML file.
   *
   * @var string
   */
  public $elementQuery;

  /**
   * Xpath query string used to retrieve the primary key value from each element.
   *
   * @var string
   */
  public $idQuery;

  /**
   * Current element object when iterating.
   *
   * @var SimpleXMLElement
   */
  protected $currentElement = NULL;

  /**
   * Value of the ID for the current element when iterating.
   *
   * @var string
   */
  protected $currentId = NULL;

  /**
   * Prepares our extensions to the XMLReader object.
   *
   * @param $xml_url
   *  URL of the XML file to be parsed.
   * @param $element_query
   *  Query string in a restricted xpath format, for selecting elements to be
   *  returned by the interator. Supported syntax:
   *  - The full path to the element must be specified; i.e., /file/article
   *    rather than //article.
   *  - The elements may be filtered by attribute value by appending
   *    [@attribute="value"].
   * @param $id_query
   *  Query string to the unique identifier for an element, relative to the root
   *  of that element. This supports the full xpath syntax.
   */
  public function __construct($xml_url, $element_query, $id_query) {
    $this->reader = new XMLReader;
    $this->url = $xml_url;
    $this->elementQuery = $element_query;
    $this->idQuery = $id_query;

    // Suppress errors during parsing, so we can pick them up after
    libxml_use_internal_errors(TRUE);

    // Parse the element query. First capture group is the element path, second
    // (if present) is the attribute.
    preg_match_all('|^/([^\[]+)(.*)$|', $element_query, $matches);
    $element_path = $matches[1][0];
    $this->elementsToMatch = explode('/', $element_path);
    $attribute_query = $matches[2][0];
    if ($attribute_query) {
      // Matches [@attribute="value"] (with either single- or double-quotes).
      preg_match_all('|^\[@([^=]+)=[\'"](.*)[\'"]\]$|', $attribute_query, $matches);
      $this->attributeName = $matches[1][0];
      $this->attributeValue = $matches[2][0];
    }
  }

  /**
   * Implementation of Iterator::rewind().
   *
   * @return void
   */
  public function rewind() {
    // (Re)open the provided URL.
    $this->reader->close();
    $status = $this->reader->open($this->url);
    if (!$status) {
      Migration::displayMessage(t('Could not open XML file !url',
                                array('!url' => $this->url)));
    }

    // Reset our path tracker
    $this->currentPath = array();

    // Load the first matching element and its ID.
    $this->next();
  }

  /**
   * Implementation of Iterator::next().
   *
   * @return void
   */
  public function next() {
    migrate_instrument_start('MigrateXMLReader::next');
    $this->currentElement = $this->currentId = NULL;

    // Loop over each node in the XML file, looking for elements at a path
    // matching the input query string (represented in $this->elementsToMatch).
    while ($this->reader->read()) {
      if ($this->reader->nodeType == XMLREADER::ELEMENT) {
        $this->currentPath[$this->reader->depth] = $this->reader->localName;
        if ($this->currentPath == $this->elementsToMatch) {
          // We're positioned to the right element path - if filtering on an
          // attribute, check that as well before accepting this element.
          if (empty($this->attributeName) ||
              ($this->reader->getAttribute($this->attributeName) == $this->attributeValue)) {
            // We've found a matching element - get a SimpleXML object representing it.
            // We must associate the DOMNode with a DOMDocument to be able to import
            // it into SimpleXML.
            // Despite appearances, this is almost twice as fast as
            // simplexml_load_string($this->readOuterXML());
            $node = $this->reader->expand();
            if ($node) {
              $dom = new DOMDocument();
              $node = $dom->importNode($node, TRUE);
              $dom->appendChild($node);
              $this->currentElement = simplexml_import_dom($node);
              $idnode = $this->currentElement->xpath($this->idQuery);
              $this->currentId = (string)reset($idnode);
              break;
            }
            else {
              foreach (libxml_get_errors() as $error) {
                $error_string = MigrateItemsXML::parseLibXMLError($error);
                if ($migration = Migration::currentMigration()) {
                  $migration->saveMessage($error_string);
                }
                else {
                  Migration::displayMessage($error_string);
                }
              }
            }
          }
        }
      }
      elseif ($this->reader->nodeType == XMLREADER::END_ELEMENT) {
        // Trim currentPath as we exit each element
        unset($this->currentPath[$this->reader->depth]);
      }
    }
    migrate_instrument_stop('MigrateXMLReader::next');
  }

  /**
   * Implementation of Iterator::current().
   *
   * @return null|SimpleXMLElement
   */
  public function current() {
    return $this->currentElement;
  }

  /**
   * Implementation of Iterator::key().
   *
   * @return null|string
   */
  public function key() {
    return $this->currentId;
  }

  /**
   * Implementation of Iterator::valid().
   *
   * @return bool
   */
  public function valid() {
    return !empty($this->currentElement);
  }
}

/**
 * Implementation of MigrateSource, to handle imports from XML files.
 */
class MigrateSourceXML extends MigrateSource {

  /**
   * The MigrateXMLReader object serving as a cursor over the XML source.
   *
   * @var MigrateXMLReader
   */
  protected $reader;

  /**
   * The source URLs to load XML from
   *
   * @var array
   */
  protected $sourceUrls = array();

  /**
   * Holds our current position within the $source_urls array
   *
   * @var int
   */
  protected $activeUrl = NULL;

  /**
   * Store the query string used to recognize elements being iterated
   * so we can create reader objects on the fly.
   *
   * @var string
   */
  protected $elementQuery = '';

  /**
   * Store the query string used to retrieve the primary key value from each
   * element so we can create reader objects on the fly.
   *
   * @var string
   */
  protected $idQuery = '';

  /**
   * Store the reader class used to query XML so we can create reader objects
   * on the fly.
   *
   * @var string
   */
  protected $readerClass = '';

  /**
   * List of available source fields.
   *
   * @var array
   */
  protected $fields = array();

  /**
   * Source constructor.
   *
   * @param string or array $url
   *  URL(s) of the XML source data.
   * @param string $element_query
   *  Query string used to recognize elements being iterated.
   * @param string $id_query
   *  Xpath query string used to retrieve the primary key value from each element.
   * @param array $fields
   *  Optional - keys are field names, values are descriptions. Use to override
   *  the default descriptions, or to add additional source fields which the
   *  migration will add via other means (e.g., prepareRow()).
   * @param boolean $options
   *  Options applied to this source. In addition to the standard MigrateSource
   *  options, we support:
   *  - reader_class: The reader class to instantiate for traversing the XML -
   *    defaults to MigrateXMLReader (any substitutions must be derived from
   *    MigrateXMLReader).
   */
  public function __construct($urls, $element_query, $id_query, array $fields = array(),
      array $options = array()) {
    parent::__construct($options);
    if (empty($options['reader_class'])) {
      $reader_class = 'MigrateXMLReader';
    }
    else {
      $reader_class = $options['reader_class'];
    }

    if (!is_array($urls)) {
      $urls = array($urls);
    }

    $this->sourceUrls = $urls;
    $this->activeUrl = NULL;
    $this->elementQuery = $element_query;
    $this->idQuery = $id_query;
    $this->readerClass = $reader_class;
    $this->fields = $fields;
  }

  /**
   * Return a string representing the source query.
   *
   * @return string
   */
  public function __toString() {
    // Clump the urls into a string
    // This could cause a problem when using a lot of urls, may need to hash
    $urls = implode(', ', $this->sourceUrls);
    return 'urls = ' . $urls .
           ' | item xpath = ' . $this->elementQuery .
           ' | item ID xpath = ' . $this->idQuery;
  }

  /**
   * Returns a list of fields available to be mapped from the source query.
   *
   * @return array
   *  Keys: machine names of the fields (to be passed to addFieldMapping)
   *  Values: Human-friendly descriptions of the fields.
   */
  public function fields() {
    return $this->fields;
  }

  /**
   * Return a count of all available source records.
   */
  public function computeCount() {
    $count = 0;
    foreach ($this->sourceUrls as $url) {
      $reader = new $this->readerClass($url, $this->elementQuery, $this->idQuery);
      foreach ($reader as $element) {
        $count++;
      }
    }

    return $count;
  }

  /**
   * Implementation of MigrateSource::performRewind().
   */
  public function performRewind() {
    // Set the reader back to the beginning of the file (positioned to the
    // first matching element), then apply our logic to make sure we have the
    // first element fulfilling our logic (idlist/map/prepareRow()).
    $this->activeUrl = NULL;
    $this->reader = NULL;
  }

  /**
   * Implementation of MigrationSource::getNextRow().
   *
   * @return stdClass
   *  data for the next row from the XML source files
   */
  public function getNextRow() {
    migrate_instrument_start('MigrateSourceXML::next');

    $source_key = $this->activeMap->getSourceKey();
    $key_name = key($source_key);
    $row = NULL;

    // The reader is now lazy loaded, so it may not be defined yet, need to test if set
    if (isset($this->reader)) {
      // attempt to load the next row
      $this->reader->next();
    }

    // Test the reader for a valid row
    if (isset($this->reader) && $this->reader->valid()) {
      $row = new stdClass;
      $row->$key_name = $this->reader->key();
      $row->xml = $this->reader->current();
    }
    else {
      // The current source is at the end, try to load the next source
      if ($this->getNextSource()) {
        $row = new stdClass;
        $row->$key_name = $this->reader->key();
        $row->xml = $this->reader->current();
      }
    }

    migrate_instrument_stop('MigrateSourceXML::next');
    return $row;
  }

  /**
   * Advances the reader to the next source from source_urls
   *
   * @return bool
   *  TRUE if a valid source was loaded
   */
  public function getNextSource() {
    migrate_instrument_start('MigrateSourceXML::nextSource');

    // Return value
    $status = FALSE;

    while ($this->activeUrl === NULL || (count($this->sourceUrls)-1) > $this->activeUrl) {
      if (is_null($this->activeUrl)) {
        $this->activeUrl = 0;
      }
      else {
        // Increment the activeUrl so we try to load the next source
        $this->activeUrl = $this->activeUrl + 1;
      }
      
      $this->reader = new $this->readerClass($this->sourceUrls[$this->activeUrl], $this->elementQuery, $this->idQuery);
      $this->reader->rewind();

      if ($this->reader->valid()) {
        // We have a valid source
        $status = TRUE;
        break;
      }
    }

    migrate_instrument_stop('MigrateSourceXML::nextSource');
    return $status;
  }
}
