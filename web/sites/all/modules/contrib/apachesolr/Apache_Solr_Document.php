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
 * @version $Id: Document.php 15 2009-08-04 17:53:08Z donovan.jimenez $
 *
 * @package Apache
 * @subpackage Solr
 * @author Donovan Jimenez <djimenez@conduit-it.com>
 */

/**
 * Additional code Copyright (c) 2011 by Peter Wolanin, and 
 * additional contributors.
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
 * Holds Key / Value pairs that represent a Solr Document along with any associated boost
 * values. Field values can be accessed by direct dereferencing such as:
 *
 * @code
 * $document->title = 'Something';
 * echo $document->title;
 *
 * Additionally, the field values can be iterated with foreach
 *
 * @code
 *   foreach ($document as $fieldName => $fieldValue) {
 *   ...
 *   }
 * </code>
 */
class ApacheSolrDocument implements IteratorAggregate {

  /**
   * Document boost value
   *
   * @var float
   */
  protected $_documentBoost = FALSE;

  /**
   * Document field values, indexed by name
   *
   * @var array
   */
  protected $_fields = array();

  /**
   * Document field boost values, indexed by name
   *
   * @var array array of floats
   */
  protected $_fieldBoosts = array();

  /**
   * Clear all boosts and fields from this document
   */
  public function clear() {
    $this->_documentBoost = FALSE;

    $this->_fields = array();
    $this->_fieldBoosts = array();
  }

  /**
   * Get current document boost
   *
   * @return mixed
   *   will be false for default, or else a float
   */
  public function getBoost() {
    return $this->_documentBoost;
  }

  /**
   * Set document boost factor
   *
   * @param mixed $boost
   *   Use false for default boost, else cast to float that should be > 0 or will be treated as false
   */
  public function setBoost($boost) {
    $boost = (float) $boost;

    if ($boost > 0.0) {
      $this->_documentBoost = $boost;
    }
    else {
      $this->_documentBoost = FALSE;
    }
  }

  /**
   * Add a value to a multi-valued field
   *
   * NOTE: the solr XML format allows you to specify boosts
   * PER value even though the underlying Lucene implementation
   * only allows a boost per field. To remedy this, the final
   * field boost value will be the product of all specified boosts
   * on field values - this is similar to SolrJ's functionality.
   *
   * @code
   *   $doc = new ApacheSolrDocument();
   *   $doc->addField('foo', 'bar', 2.0);
   *   $doc->addField('foo', 'baz', 3.0);
   *   // resultant field boost will be 6!
   *   echo $doc->getFieldBoost('foo');
   *
   * @param string $key
   * @param mixed $value
   * @param mixed $boost
   *   Use false for default boost, else cast to float that should be > 0 or will be treated as false
   */
  public function addField($key, $value, $boost = FALSE) {
    if (!isset($this->_fields[$key])) {
      // create holding array if this is the first value
      $this->_fields[$key] = array();
    }
    else if (!is_array($this->_fields[$key])) {
      // move existing value into array if it is not already an array
      $this->_fields[$key] = array($this->_fields[$key]);
    }

    if ($this->getFieldBoost($key) === FALSE) {
      // boost not already set, set it now
      $this->setFieldBoost($key, $boost);
    }
    else if ((float) $boost > 0.0) {
      // multiply passed boost with current field boost - similar to SolrJ implementation
      $this->_fieldBoosts[$key] *= (float) $boost;
    }

    // add value to array
    $this->_fields[$key][] = $value;
  }

  /**
   * Handle the array manipulation for a multi-valued field
   *
   * @param string $key
   * @param string $value
   * @param mixed $boost
   *   Use false for default boost, else cast to float that should be > 0 or will be treated as false
   *
   * @deprecated Use addField(...) instead
   */
  public function setMultiValue($key, $value, $boost = FALSE) {
    $this->addField($key, $value, $boost);
  }

  /**
   * Get field information
   *
   * @param string $key
   * @return mixed associative array of info if field exists, false otherwise
   */
  public function getField($key) {
    if (isset($this->_fields[$key])) {
      return array(
        'name' => $key,
        'value' => $this->_fields[$key],
        'boost' => $this->getFieldBoost($key)
      );
    }

    return FALSE;
  }

  /**
   * Set a field value. Multi-valued fields should be set as arrays
   * or instead use the addField(...) function which will automatically
   * make sure the field is an array.
   *
   * @param string $key
   * @param mixed $value
   * @param mixed $boost
   *   Use false for default boost, else cast to float that should be > 0 or will be treated as false
   */
  public function setField($key, $value, $boost = FALSE) {
    $this->_fields[$key] = $value;
    $this->setFieldBoost($key, $boost);
  }

  /**
   * Get the currently set field boost for a document field
   *
   * @param string $key
   * @return float
   *   currently set field boost, false if one is not set
   */
  public function getFieldBoost($key) {
    return isset($this->_fieldBoosts[$key]) ? $this->_fieldBoosts[$key] : FALSE;
  }

  /**
   * Set the field boost for a document field
   *
   * @param string $key
   *   field name for the boost
   * @param mixed $boost
   *   Use false for default boost, else cast to float that should be > 0 or will be treated as false
   */
  public function setFieldBoost($key, $boost) {
    $boost = (float) $boost;

    if ($boost > 0.0) {
      $this->_fieldBoosts[$key] = $boost;
    }
    else {
      $this->_fieldBoosts[$key] = FALSE;
    }
  }

  /**
   * Return current field boosts, indexed by field name
   *
   * @return array
   */
  public function getFieldBoosts() {
    return $this->_fieldBoosts;
  }

  /**
   * Get the names of all fields in this document
   *
   * @return array
   */
  public function getFieldNames() {
    return array_keys($this->_fields);
  }

  /**
   * Get the values of all fields in this document
   *
   * @return array
   */
  public function getFieldValues() {
    return array_values($this->_fields);
  }

  /**
   * IteratorAggregate implementation function. Allows usage:
   *
   * @code
   *   foreach ($document as $key => $value) {
   *     ...
   *   }
   *
   */
  public function getIterator() {
    $arrayObject = new ArrayObject($this->_fields);

    return $arrayObject->getIterator();
  }

  /**
   * Magic get for field values
   *
   * @param string $key
   * @return mixed
   */
  public function __get($key) {
    return $this->_fields[$key];
  }

  /**
   * Magic set for field values. Multi-valued fields should be set as arrays
   * or instead use the addField(...) function which will automatically
   * make sure the field is an array.
   *
   * @param string $key
   * @param mixed $value
   */
  public function __set($key, $value) {
    $this->setField($key, $value);
  }

  /**
   * Magic isset for fields values.  Do not call directly. Allows usage:
   *
   * @code
   *   isset($document->some_field);
   *
   * @param string $key
   * @return boolean
   *   Whether the given key is set in the document
   */
  public function __isset($key) {
    return isset($this->_fields[$key]);
  }

  /**
   * Magic unset for field values. Do not call directly. Allows usage:
   *
   * @code
   *   unset($document->some_field);
   *
   * @param string $key
   */
  public function __unset($key) {
    unset($this->_fields[$key]);
    unset($this->_fieldBoosts[$key]);
  }

  /**
   * Create an XML fragment from a ApacheSolrDocument instance appropriate for use inside a Solr add call
   *
   * @param ApacheSolrDocument $document
   *
   * @return string
   *   an xml formatted string from the given document
   */
  public static function documentToXml(ApacheSolrDocument $document) {
    $xml = '<doc';

    if ($document->getBoost() !== FALSE) {
      $xml .= ' boost="' . $document->getBoost() . '"';
    }

    $xml .= '>';

    foreach ($document as $key => $value) {
      $key = htmlspecialchars($key, ENT_QUOTES, 'UTF-8');
      $fieldBoost = $document->getFieldBoost($key);

      if (is_array($value)) {
        foreach ($value as $multivalue) {
          $xml .= '<field name="' . $key . '"';

          if ($fieldBoost !== FALSE) {
            $xml .= ' boost="' . $fieldBoost . '"';

            // Only set the boost for the first field in the set
            $fieldBoost = FALSE;
          }

          $xml .= '>' . htmlspecialchars($multivalue, ENT_NOQUOTES, 'UTF-8') . '</field>';
        }
      }
      else {
        $xml .= '<field name="' . $key . '"';

        if ($fieldBoost !== FALSE) {
          $xml .= ' boost="' . $fieldBoost . '"';
        }

        $xml .= '>' . htmlspecialchars($value, ENT_NOQUOTES, 'UTF-8') . '</field>';
      }
    }

    $xml .= '</doc>';

    // Remove any control characters to avoid Solr XML parser exception
    return self::stripCtrlChars($xml);
  }

  /**
   * Replace control (non-printable) characters from string that are invalid to Solr's XML parser with a space.
   *
   * @param string $string
   * @return string
   */
  public static function stripCtrlChars($string) {
    // See:  http://w3.org/International/questions/qa-forms-utf-8.html
    // Printable utf-8 does not include any of these chars below x7F
    return preg_replace('@[\x00-\x08\x0B\x0C\x0E-\x1F]@', ' ', $string);
  }
}