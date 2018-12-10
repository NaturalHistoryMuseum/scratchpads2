<?php

/**
 * @package PHP-MARC
 */

//-----------------------------------------------------------------------------
//
// Copyright (C) 2003-2005 Oy Realnode Ab
//
//-----------------------------------------------------------------------------
//
// php-marc.php
//     Part of the Emilda Project (http://www.emilda.org/)
//
// Description
//     MARC Record parser. Syntatically and logically identical to
//     the Perl library MARC::Record. MARC parsing rules have been
//     checked up from MARC::Record.
//
// Authors
//     Christoffer Landtman <landtman (at) realnode com>
//
//-----------------------------------------------------------------------------
//
// This program is free software; you can redistribute it and/or
// modify it under the terms of the GNU General Public License
// as published by the Free Software Foundation; either version 2
// of the License, or (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software
// Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
//
//-----------------------------------------------------------------------------
//
// $Revision$
//
//-----------------------------------------------------------------------------

/**
 * Hexadecimal value for Subfield indicator
 * @global hex SUBFIELD_INDICATOR
 */
define("SUBFIELD_INDICATOR", "\x1F");
/**
 * Hexadecimal value for End of Field
 * @global hex END_OF_FIELD
 */
define("END_OF_FIELD", "\x1E");
/**
 * Hexadecimal value for End of Record
 * @global hex END_OF_RECORD
 */
define("END_OF_RECORD", "\x1D");
/**
 * Length of the Directory
 * @global integer DIRECTORY_ENTRY_LEN
 */
define("DIRECTORY_ENTRY_LEN", 12);
/**
 * Length of the Leader
 * @global integer LEADER_LEN
 */
define("LEADER_LEN", 24);

/**
 * Class File
 * Class to read MARC records from file(s)
 */
Class File {

  /**
   * ========== VARIABLE DECLARATIONS ==========
   */

  /**
   * Array containing raw records
   * @var array
   */
  var $raw;
  /**
   * Array of warnings
   * @var array
   */
  var $warn;
  /**
   * Current position in the array of records
   * @var integer
   */
  var $pointer;

  /**
   * ========== ERROR FUNCTIONS ==========
   */

  /**
   * Croaking function
   *
   * Similar to Perl's croak function, which ends parsing and raises an
   * user error with a descriptive message.
   * @param string The message to display
   */
  function _croak($msg) {
    trigger_error($msg, E_USER_ERROR);
  }

  /**
   * Fuction to issue warnings
   *
   * Warnings will not be displayed unless explicitly accessed, but all
   * warnings issued during parse will be stored
   * @param string Warning
   * @return string Last added warning
   */
  function _warn($msg) {
    $this->warn[] = $msg;
    return $msg;
  }

  /**
   * Get warning(s)
   *
   * Get either all warnings or a specific warning ID
   * @param integer ID of the warning
   * @return array|string Return either Array of all warnings or specific warning
   */
  function warnings($id = "") {
    if (!$id) {
      return $this->warn;
    } else {
      if (array_key_exists($id, $this->warn)) {
        return $this->warn[$id];
      } else {
        return "Invalid warning ID: $id";
      }
    }
  }

  /**
   * ========== PROCESSING FUNCTIONS ==========
   */

  /**
   * Return the next raw MARC record
   *
   * Returns th nexts raw MARC record from the read file, unless all
   * records already have been read.
   * @return string|FALSE Either a raw record or False
   */
  function _next() {
    /**
     * Exit if we are at the end of the file
     */
    if ($this->pointer >= count($this->raw)) {
      return FALSE;
    }

    /**
     * Read next line
     */
    $usmarc = $this->raw[$this->pointer++];

    // remove illegal stuff that sometimes occurs between records
    // preg_replace does not know what to do with \x00, thus omitted.
    $usmarc = preg_replace("/^[\x0a\x0d]+/", "", $usmarc);

    /**
     * Record validation
     */
    if ( strlen($usmarc) < 5 ) {
      $this->_warn( "Couldn't find record length" );
    }
    $reclen = substr($usmarc,0,5);
    if ( preg_match("/^\d{5}$/", $reclen) || $reclen != strlen($usmarc) ) {
      $this->_warn( "Invalid record length \"$reclen\"" );
    }

    return $usmarc;
  }

  /**
   * Read in MARC record file
   *
   * This function will read in MARC record files that either
   * contain a single MARC record, or numerous records.
   * @param string Name of the file
   * @return string Returns warning if issued during read
   */
  function file($in) {
    if (file_exists($in)) {
      $input = file($in);
      $recs = explode(END_OF_RECORD, join("", $input));
      // Append END_OF_RECORD as we lost it when splitting
      // Last is not record, as it is empty because every record ends
      // with END_OF_RECORD.
      for ($i = 0; $i < (count($recs)-1); $i++) {
        $this->raw[] = $recs[$i].END_OF_RECORD;
      }
      $this->pointer = 0;
    } else {
      return $this->_warn("Invalid input file: $i");
    }
  }

  /**
   * Return next Record-object
   *
   * Decode the next raw MARC record and return
   * @return Record A Record object
   */
  function next() {
    if ($raw = $this->_next()) {
      return $this->decode($raw);
    } else {
      return FALSE;
    }
  }

  /**
   * Decode a given raw MARC record
   *
   * "Port" of Andy Lesters MARC::File::USMARC->decode() function into PHP. Ideas and
   * "rules" have been used from USMARC::decode().
   *
   * @param string Raw MARC record
   * @return Record Decoded MARC Record object
   */
  function decode($text) {
    if (!preg_match("/^\d{5}/", $text, $matches)) {
      $this->_croak('Record length "'.substr( $text, 0, 5 ).'" is not numeric');
    }

    $marc = new Record;

    // Store record length
    $reclen = $matches[0];

    if ($reclen != strlen($text)) {
      $this->_croak( "Invalid record length: Leader says $reclen bytes, but it's actually ".strlen($text));
    }

    if (substr($text, -1, 1) != END_OF_RECORD)
      $this->_croak("Invalid record terminator");

      // Store leader
    $marc->leader(substr( $text, 0, LEADER_LEN ));

    // bytes 12 - 16 of leader give offset to the body of the record
    $data_start = 0 + substr( $text, 12, 5 );

    // immediately after the leader comes the directory (no separator)
    $dir = substr( $text, LEADER_LEN, $data_start - LEADER_LEN - 1 );  // -1 to allow for \x1e at end of directory

    // character after the directory must be \x1e
    if (substr($text, $data_start-1, 1) != END_OF_FIELD) {
      $this->_croak("No directory found");
    }

    // All directory entries 12 bytes long, so length % 12 must be 0
    if (strlen($dir) % DIRECTORY_ENTRY_LEN != 0) {
      $this->_croak("Invalid directory length");
    }

    // go through all the fields
    $nfields = strlen($dir) / DIRECTORY_ENTRY_LEN;
    for ($n=0; $n<$nfields; $n++) {
      // As pack returns to key 1, leave place 0 in list empty
      list(, $tagno) = unpack("A3", substr($dir, $n*DIRECTORY_ENTRY_LEN, DIRECTORY_ENTRY_LEN));
      list(, $len) = unpack("A3/A4", substr($dir, $n*DIRECTORY_ENTRY_LEN, DIRECTORY_ENTRY_LEN));
      list(, $offset) = unpack("A3/A4/A5", substr($dir, $n*DIRECTORY_ENTRY_LEN, DIRECTORY_ENTRY_LEN));

      // Check directory validity
      if (!preg_match("/^[0-9A-Za-z]{3}$/", $tagno)) {
        $this->_croak("Invalid tag in directory: \"$tagno\"");
      }
      if (!preg_match("/^\d{4}$/", $len)) {
        $this->_croak("Invalid length in directory, tag $tagno: \"$len\"");
      }
      if (!preg_match("/^\d{5}$/", $offset)) {
        $this->_croak("Invalid offset in directory, tag $tagno: \"$offset\"");
      }
      if ($offset + $len > $reclen) {
        $this->_croak("Directory entry runs off the end of the record tag $tagno");
      }

      $tagdata = substr( $text, $data_start + $offset, $len );

      if ( substr($tagdata, -1, 1) == END_OF_FIELD ) {
        # get rid of the end-of-tag character
        $tagdata = substr($tagdata, 0, -1);
        --$len;
      } else {
        $this->_croak("field does not end in end of field character in tag $tagno");
      }

      if ( preg_match("/^\d+$/", $tagno) && ($tagno < 10) ) {
        $marc->append_fields(new Field($tagno, $tagdata));
      } else {
        $subfields = explode(SUBFIELD_INDICATOR, $tagdata);
        $indicators = array_shift($subfields);

        if ( strlen($indicators) > 2 || strlen( $indicators ) == 0 ) {
          $this->_warn("Invalid indicators \"$indicators\" forced to blanks for tag $tagno\n");
          list($ind1,$ind2) = array(" ", " ");
        } else {
          $ind1 = substr( $indicators, 0, 1 );
          $ind2 = substr( $indicators, 1, 1 );
        }

        // Split the subfield data into subfield name and data pairs
        $subfield_data = array();
        foreach ($subfields as $subfield) {
          if ( strlen($subfield) > 0 ) {
            $subfield_data[substr($subfield, 0, 1)][] = substr($subfield, 1);
          } else {
            $this->_warn( "Entirely empty subfield found in tag $tagno" );
          }
        }

        if (!isset($subfield_data)) {
          $this->_warn( "No subfield data found $location for tag $tagno" );
        }

        $marc->append_fields(new Field($tagno, $ind1, $ind2, $subfield_data ));
      }
    }
    return $marc;
  }

  /**
   * Get the number of records available in this Record
   * @return int The number of records
   */
  function num_records() {
    return count($this->raw);
  }
}

/**
 * USMARC Class
 * Extension class to File class, which allows passing of raw MARC string
 * instead of filename
 */
Class USMARC Extends File {
  /**
   * Read raw MARC string for decoding
   * @param string Raw MARC
  */
  function usmarc($string) {
    $this->raw[] = $string;
    $this->pointer = 0;
  }
}

/**
 * Record Class
 * Create a MARC Record class
 */
Class Record {

  /**
   * ========== VARIABLE DECLARATIONS ==========
   */

  /**
   * Contain all @link Field objects of the Record
   * @var array
   */
  var $fields;
  /**
   * Leader of the Record
   * @var string
   */
  var $ldr;
  /**
   * Array of warnings
   * @var array
   */
  var $warn;

  /**
   * ========== ERROR FUNCTIONS ==========
   */

  /**
   * Croaking function
   *
   * Similar to Perl's croak function, which ends parsing and raises an
   * user error with a descriptive message.
   * @param string The message to display
   */
  function _croak($msg) {
    trigger_error($msg, E_USER_ERROR);
  }

  /**
   * Fuction to issue warnings
   *
   * Warnings will not be displayed unless explicitly accessed, but all
   * warnings issued during parse will be stored
   * @param string Warning
   * @return string Last added warning
   */
  function _warn($msg) {
    $this->warn[] = $msg;
    return $msg;
  }

  /**
   * Return an array of warnings
   */
  function warnings() {
    return $this->warn;
  }

  /**
   * ========== PROCESSING FUNCTIONS ==========
   */

  /**
   * Start function
   *
   * Set all variables to defaults to create new Record object
   */
  function record() {
    $this->fields = array();
    $this->ldr = str_repeat(' ', 24);
  }

  /**
   * Get/Set Leader
   *
   * If argument specified, sets leader, otherwise gets leader. No validation
   * on the specified leader is performed
   * @param string Leader
   * @return string|null Return leader in case requested.
   */
  function leader($ldr = "") {
    if ($ldr) {
      $this->ldr = $ldr;
    } else {
      return $this->ldr;
    }
  }

  /**
   * Append field to existing
   *
   * Given Field object will be appended to the existing list of fields. Field will be
   * appended last and not in its "correct" location.
   * @param Field The field to append
   */
  function append_fields($field) {
    if (strtolower(get_class($field)) == "field") {
      $this->fields[$field->tagno][] = $field;
    } else {
      $this->_croak(sprintf("Given argument must be Field object, but was '%s'", get_class($field)));
    }
  }

  /**
   * Build Record Directory
   *
   * Generate the directory of the Record according to existing data.
   * @return array Array ( $fields, $directory, $total, $baseaddress )
   */
  function _build_dir() {
    // Vars
    $fields = array();
    $directory = array();

    $dataend = 0;
    foreach ($this->fields as $field_group ) {
      foreach ($field_group as $field) {
        // Get data in raw format
        $str = $field->raw();
        $fields[] = $str;

        // Create directory entry
        $len = strlen($str);
        $direntry = sprintf( "%03s%04d%05d", $field->tagno(), $len, $dataend );
        $directory[] = $direntry;
        $dataend += $len;
      }
    }

    /**
     * Rules from MARC::Record::USMARC
     */
    $baseaddress =
      LEADER_LEN +    // better be 24
      ( count($directory) * DIRECTORY_ENTRY_LEN ) +
      // all the directory entries
      1;              // end-of-field marker


    $total =
      $baseaddress +  // stuff before first field
      $dataend +      // Length of the fields
      1;              // End-of-record marker

    return array($fields, $directory, $total, $baseaddress);
  }

  /**
   * Set Leader lengths
   *
   * Set the Leader lengths of the record according to defaults specified in
   * http://www.loc.gov/marc/bibliographic/ecbdldrd.html
   */
  function leader_lengths($reclen, $baseaddr) {
    $this->ldr = substr_replace($this->ldr, sprintf("%05d", $reclen), 0, 5);
    $this->ldr = substr_replace($this->ldr, sprintf("%05d", $baseaddr), 12, 5);
    $this->ldr = substr_replace($this->ldr, '22', 10, 2);
    $this->ldr = substr_replace($this->ldr, '4500', 20, 4);
  }

  /**
   * Return all Field objects
   * @return array Array of Field objects
   */
  function fields() {
    return $this->fields;
  }

  /**
   * Get specific field
   *
   * Search for field in Record fields based on field name, e.g. 020
   * @param string Field name
   * @return Field|FALSE Return Field if found, otherwise FALSE
   */
  function field($spec) {
    if (array_key_exists($spec, $this->fields)) {
      return $this->fields[$spec][0];
    } else {
      return FALSE;
    }
  }

  /**
   * Get subfield of Field object
   *
   * Returns the value of a specific subfield of a given Field object
   * @param string Name of field
   * @param string Name of subfield
   * @return string|FALSE Return value of subfield if Field exists, otherwise FALSE
   */
  function subfield($field, $subfield) {
    if (!$field = $this->field($field)) {
      return FALSE;
    } else {
      return $field->subfield($subfield);
    }
  }

  /**
   * Delete Field
   *
   * Delete a given field from within a Record
   * @param Field The field to be deleted
   */
  function delete_field($obj) {
    unset($this->fields[$obj->field]);
  }

  /**
   * Clone record
   *
   * Clone a record with all its Fields and subfields
   * @return Record Clone record
   */
  function make_clone() {
    $clone = new Record;
    $clone->leader($this->ldr);

    foreach ($this->fields() as $data) {
      foreach ($data as $field) {
        $clone->append_fields($field);
      }
    }

    return $clone;
  }

  /**
   * ========== OUTPUT FUNCTIONS ==========
   */

  /**
   * Formatted representation of Field
   *
   * Format a Field with a sprintf()-like formatting syntax. The formatting
   * codes are the names of the subfields of the Field.
   * @param string Field name
   * @param string Format string
   * @return string|FALSE Return formatted string if Field exists, otherwise False
   */
  function ffield($tag, $format) {
    $result = "";
    if ($field = $this->field($tag)) {
      for ($i=0; $i<strlen($format); $i++) {
        $curr = $format[$i];
        if ($curr != "%") {
          $result[] = $curr;
        } else {
          $i++;
          $curr = $format[$i];
          if ($curr == "%") {
            $result[] = $curr;
          } else {
            $result[] = $field->subfield($curr);
          }
        }
      }
      return implode("", $result);
    } else {
      return FALSE;
    }
  }

  /**
   * Return Raw
   *
   * Return the Record in raw MARC format.
   * @return string Raw MARC data
   */
  function raw() {
    list ($fields, $directory, $reclen, $baseaddress) = $this->_build_dir();
    $this->leader_lengths($reclen, $baseaddress);

    /**
     * Glue together all parts
     */
    return $this->ldr.implode("", $directory).END_OF_FIELD.implode("", $fields).END_OF_RECORD;
  }

  /**
   * Return formatted
   *
   * Return the Record in a formatted fashion. Similar to the output
   * of the formatted() function in MARC::Record in Perl
   * @return string Formatted representation of MARC record
   */
  function formatted() {
    $formatted = "";
    foreach ($this->fields as $field_group) {
      foreach ($field_group as $field) {
        $formatted .= $field->formatted(). "\n";
      }
    }
    return $formatted;
  }
}

/**
 * Field Class
 * Create a MARC Field object
 */
Class Field {

  /**
   * ========== VARIABLE DECLARATIONS ==========
   */

  /**
   * The tag name of the Field
   * @var string
   */
  var $tagno;
  /**
   * Value of the first indicator
   * @var string
   */
  var $ind1;
  /**
   * Value of the second indicator
   * @var string
   */
  var $ind2;
  /**
   * Array of subfields
   * @var array
   */
  var $subfields = array();
  /**
   * Specify if the Field is a Control field
   * @var bool
   */
  var $is_control;
  /**
   * Array of warnings
   * @var array
   */
  var $warn;
  /**
   * Value of field, if field is a Control field
   * @var string
   */
  var $data;

  /**
   * ========== ERROR FUNCTIONS ==========
   */

  /**
   * Croaking function
   *
   * Similar to Perl's croak function, which ends parsing and raises an
   * user error with a descriptive message.
   * @param string The message to display
   */
  function _croak($msg) {
    trigger_error($msg, E_USER_ERROR);
  }

  /**
   * Fuction to issue warnings
   *
   * Warnings will not be displayed unless explicitly accessed, but all
   * warnings issued during parse will be stored
   * @param string Warning
   * @return string Last added warning
   */
  function _warn($msg) {
    $this->warn[] = $msg;
    return $msg;
  }

  /**
   * Return an array of warnings
   */
  function warnings() {
    return $this->warn;
  }

  /**
   * ========== PROCESSING FUNCTIONS ==========
   */

  /**
   * Field init function
   *
   * Create a new Field object from passed arguments
   * @param array Array ( tagno, ind1, ind2, subfield_data )
   * @return string Returns warnings if any issued during parse
   */
  function field() {
    $args = func_get_args();

    $tagno = array_shift($args);
    $this->tagno = $tagno;

    // Check if valid tag
    if (!preg_match("/^[0-9A-Za-z]{3}$/", $tagno)) {
      return $this->_warn("Tag \"$tagno\" is not a valid tag.");
    }

    // Check if field is Control field
    $this->is_control = (preg_match("/^\d+$/", $tagno) && $tagno < 10);
    if ($this->is_control) {
      $this->data = array_shift($args);
    } else {
      foreach (array("ind1", "ind2") as $indcode) {
        $indicator = array_shift($args);
        if (!preg_match("/^[0-9A-Za-z ]$/", $indicator)) {
          if ($indicator != "") {
            $this->_warn("Illegal indicator '$indicator' in field '$tagno' forced to blank");
          }
          $indicator = " ";
        }
        $this->$indcode = $indicator;
      }

      $subfields = array_shift($args);

      if (count($subfields) < 1) {
        return $this->_warn("Field $tagno must have at least one subfield");
      } else {
        $this->add_subfields($subfields);
      }
    }
  }

  /**
   * Add subfield
   *
   * Appends subfields to existing fields last, not in "correct" plase
   * @param array Subfield data
   * @return string Returns warnings if issued during parse.
   */
  function add_subfields() {
    // Process arguments
    $args = func_get_args();
    if (count($args) == 1 && is_array($args[0])) {
      $args = $args[0];
    }
    // Add subfields, is appropriate
    if ($this->is_control) {
      return $this->_warn("Subfields allowed only for tags bigger or equal to 10");
    } else {
      $this->subfields = array_merge($this->subfields, $args);
    }

    return count($args)/2;
  }

  /**
   * Return Tag number of Field
   */
  function tagno() {
    return $this->tagno;
  }

  /**
   * Set/Get Data of Control field
   *
   * Sets the Data if argument given, otherwise Data returned
   * @param string Data to be set
   * @return string Data of Control field if argument not given
   */
  function data($data = "") {
    if (!$this->is_control) {
      $this->_croak("data() is only allowed for tags bigger or equal to 10");
    }
    if ($data) {
      $this->data = $data;
    } else {
      return $this->data;
    }
  }

  /**
   * Get values of indicators
   *
   * @param string Indicator number
   */
  function indicator($ind) {
    if ($ind == 1) {
      return $this->ind1;
    } elseif ($ind == 2) {
      return $this->ind2;
    } else {
      $this->_warn("Invalid indicator: $ind");
    }
  }

  /**
   * Check if Field is Control field
   *
   * @return bool True or False
   */
  function is_control() {
    return $this->is_control;
  }

  /**
   * Get the value of a subfield
   *
   * Return of the value of the given subfield, if exists
   * @param string Name of subfield
   * @return string|FALSE Value of the subfield if exists, otherwise FALSE
   */
  function subfield($code, $repeatable = FALSE) {
    if (array_key_exists($code, $this->subfields)) {
      return $repeatable ? $this->subfields[$code] : $this->subfields[$code][0];
    } else {
      return $repeatable ? array(): FALSE;
    }
  }

  /**
   * Return array of subfields
   *
   * @return array Array of subfields
   */
  function subfields() {
    return $this->subfields;
  }

  /**
   * Update Field
   *
   * Update Field with given array of arguments.
   * @param array Array of key->value pairs of data
   */
  function update() {
    // Process arguments
    $args = func_get_args();
    if (count($args) == 1 && is_array($args[0])) {
      $args = $args[0];
    }
    if ($this->is_control) {
      $this->data = array_shift($args);
    } else {
      foreach ($args as $subfield => $value) {
        if ($subfield == "ind1") {
          $this->ind1 = $value;
        } elseif ($subfield == "ind2") {
          $this->ind2 = $value;
        } else {
          $this->subfields[$subfield] = $value;
        }
      }
    }
  }

  /**
   * Replace Field with given Field
   *
   * @param Field Field to replace with
   */
  function replace_with($obj) {
    if (strtolower(get_class($obj)) == "field") {
      $this->tagno = $obj->tagno;
      $this->ind1 = $obj->ind1;
      $this->ind2 = $obj->ind2;
      $this->subfields = $obj->subfields;
      $this->is_control = $obj->is_control;
      $this->warn = $obj->warn;
      $this->data = $obj->data;
    } else {
      $this->_croak(sprintf("Argument must be Field-object, but was '%s'", get_class($obj)));
    }
  }

  /**
   * Clone Field
   *
   * @return Field Cloned Field object
   */
  function make_clone() {
    if ($this->is_control) {
      return new Field($this->tagno, $this->data);
    } else {
      return new Field($this->tagno, $this->ind1, $this->ind2, $this->subfields);
    }
  }

  /**
   * ========== OUTPUT FUNCTIONS ==========
   */

  /**
   * Return Field formatted
   *
   * Return Field as string, formatted in a similar fashion to the
   * MARC::Record formatted() functio in Perl
   * @return string Formatted output of Field
   */
  function formatted() {
    // Variables
    $lines = array();
    // Process
    if ($this->is_control) {
      return sprintf("%3s     %s", $this->tagno, $this->data);
    } else {
      $pre = sprintf("%3s %1s%1s", $this->tagno, $this->ind1, $this->ind2);
    }
    // Process subfields
    foreach ($this->subfields as $subfield => $value) {
      $lines[] = sprintf("%6s _%1s%s", $pre, $subfield, $value);
      $pre = "";
    }

    return join("\n", $lines);
  }

  /**
   * Return Field in Raw MARC
   *
   * Return the Field formatted in Raw MARC for saving into MARC files
   * @return string Raw MARC
   */
  function raw() {
    if ($this->is_control) {
      return $this->data.END_OF_FIELD;
    } else {
      $subfields = array();
      foreach ($this->subfields as $subfield => $value) {
        $subfields[] = SUBFIELD_INDICATOR.$subfield.$value;
      }
      return $this->ind1.$this->ind2.implode("", $subfields).END_OF_FIELD;
    }
  }

  /**
   * Return Field as String
   *
   * Return Field formatted as String, with either all subfields or special
   * subfields as specified.
   * @return string Formatted as String
   */
  function string($fields = "") {
    $matches = array();
    if ($fields) {
      for($i=0; $i<strlen($fields); $i++) {
        if (array_key_exists($fields[$i], $this->subfields)) {
          $matches[] = $this->subfields[$fields[$i]];
        }
      }
    } else {
      $matches = $this->subfields;
    }
    return implode(" ", $matches);
  }

}

?>