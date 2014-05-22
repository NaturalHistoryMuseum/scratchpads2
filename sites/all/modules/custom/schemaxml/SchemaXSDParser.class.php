<?php

/**
 * This class is used to parse an XSD schema into a schema array
 * 
 */
class SchemaXSDParser{

  /**
   * Construct the parser from the URL
   * 
   */
  function __construct($url){
    $this->schema_url = $url;
  }

  /**
   * Parse the given schema and return the associated schema array.
   * This will throw an Exception on error.
   * 
   */
  function get_schema_array(){
    $dom = simplexml_load_file($this->schema_url);
    if($dom === false){throw new Exception("Could not load/parse schema at " . $this->schema_url);}
    $this->dom = $dom;
    $this->schema = $this->_create_elements($dom);
    return $this->schema;
  }

  /**
   * Create the schema array representing the xsd:elements under
   * the current element.
   */
  function _create_elements($current){
    $schema = array();
    $elements = $current->xpath('xsd:element|xsd:sequence');
    foreach($elements as $element){
      if($element->getName() == 'element'){
        $vars = get_object_vars($element);
        $attributes = $vars['@attributes'];
        if(isset($attributes['type'])){
          $schema[$attributes['name']] = $this->_get_type($attributes['type']);
        }else{
          // Look for in-line type definition
          $inline = $element->xpath('xsd:complexType');
          if(count($inline) > 0){
            if(count($inline) > 1){throw new Exception("Invalid XSD schema: more than one inline complexType");}
            $inline_type = array_shift($inline);
            $schema[$attributes['name']] = $this->_parse_complex_type($inline_type, $attributes['name']);
          }else{
            // Simple value
            $schema[$attributes['name']] = array();
          }
        }
        if(isset($attributes['minOccurs']) && $attributes['minOccurs'] != 1){
          $schema[$attributes['name']]['#min_occurence'] = $attributes['minOccurs'] == "unbounded" ? -1 : intval($attributes['minOccurs']);
        }
        if(isset($attributes['maxOccurs']) && $attributes['maxOccurs'] != 1){
          $schema[$attributes['name']]['#max_occurence'] = $attributes['maxOccurs'] == "unbounded" ? -1 : intval($attributes['maxOccurs']);
        }
      }else if($element->getName() == 'sequence'){
        // A sequence of elements, indicating a repeat within the parent tag.
        // XXX we need to handle this properly - as this is actually wrong. The sequence has it's own minOccurs/maxOccurs.
        // We'd need a '#sequence' element.
        $schema = array_merge($schema, $this->_create_elements($element));
      }else{
        throw new Exception('Unsuported element type within flow : ' . $element->getName());
      }
    }
    return $schema;
  }

  /**
   * Create the schema array representing the given xsd:simple/complexType element
   * 
   */
  function _get_type($type){
    // Handle inbuild types
    switch($type){
      case 'xsd:string':
        return array();
    }
    // Look for a simple/complexType definition
    $types = $this->dom->xpath('xsd:simpleType[@name="' . $type . '"]');
    if(count($types) > 0){
      // Handle simple types
      if(count($types) > 1){throw new Exception("Invalid XSD schema: more than one definition of type " . $type);}
      $type_element = array_shift($types);
      $element = array();
    }else{
      // Handle complex types
      $types = $this->dom->xpath('xsd:complexType[@name="' . $type . '"]');
      if(count($types) == 0){
        throw new Exception("Parse error: could not find type " . $type);
      }else if(count($types) > 1){throw new Exception("Invalid XSD schema: more than one definition of type " . $type);}
      $type_element = array_shift($types);
      $element = $this->_parse_complex_type($type_element, $type);
    }
    // Parse restrictions. XXX Only enumerations are supported.
    $restrictions = $type_element->xpath('xsd:restriction');
    foreach($restrictions as $restriction){
      $enumerations = $restriction->xpath('xsd:enumeration');
      if(count($enumerations)){
        if(!isset($element['#restriction'])){
          $element['#restriction'] = array(
            'enumeration' => array()
          );
        }
        foreach($enumerations as $enumeration){
          $vars = get_object_vars($enumeration);
          $element['#restriction']['enumeration'][] = $vars['@attributes']['value'];
        }
      }
    }
    return $element;
  }

  /**
   * Parse a complexType element, and return the schema array
   */
  function _parse_complex_type($element, $display_name){
    $result = array();
    // Look for attributes
    $attributes = $element->xpath('xsd:attribute|xsd:simpleContent/xsd:extension/xsd:attribute');
    foreach($attributes as $attribute){
      $vars = get_object_vars($attribute);
      $name = $vars['@attributes']['name'];
      $attr_required = isset($vars['@attributes']['use']) && $vars['@attributes']['use'] == 'required';
      $default = NULL;
      // Look for restrictions on the attribute
      $attr_enumerations = $attribute->xpath('xsd:simpleType/xsd:restriction/xsd:enumeration');
      if(count($attr_enumerations)){
        if(!isset($result['#restrictions'])){
          $result['#restrictions'] = array(
            'attributes_enumerations' => array()
          );
        }else if(!isset($result['#restrictions']['attributes_enumerations'])){
          $result['#restrictions']['attributes_enumerations'] = array();
        }
        $result['#restrictions']['attributes_enumerations'][$name] = array();
        foreach($attr_enumerations as $attr_enumeration){
          $vars = get_object_vars($attr_enumeration);
          $result['#restrictions']['attributes_enumerations'][$name][] = $vars['@attributes']['value'];
        }
      }
      if($attr_required && !empty($result['#restrictions']['attributes_enumerations'][$name])){
        $default = reset($result['#restrictions']['attributes_enumerations'][$name]);
      }
      $result['#attributes'] = array(
        $name => $default
      );
    }
    // Look for a sequence
    $sequences = $element->xpath('xsd:sequence');
    if(count($sequences) > 1){
      throw new Exception("XSD Schema parsing: we only support complex types with one sequence for type/inline $display_name");
    }else if(count($sequences) == 1){
      $sequence = array_shift($sequences);
      $result = array_merge($result, $this->_create_elements($sequence));
    }
    return $result;
  }

  /**
   * Merge two schema arrays into one
   */
  static function merge_schemas($a, $b, $diff = FALSE){
    // Base case if either $a or $b is not an array
    if(!is_array($a)){
      if(is_array($b) && $diff){
        $b += array(
          '#comment' => ''
        );
        $b['#comment'] .= " DIFF:Type mismatch, this value was not an array in the source schema;";
      }
      return $b;
    }
    if(!is_array($b)){
      $a[] = $b;
      if($diff){
        $a += array(
          '#comment' => ''
        );
        $a['#comment'] .= " DIFF:Type mismatch, this value was not an array in the provided schema;";
      }
      return $a;
    }
    // Prepare for processing
    $r = array();
    $b_keys = array_keys($b);
    // Start with $a, and look for equivalents in $b
    foreach(array_keys($a) as $a_key){
      // Match same keys
      if(!isset($b[$a_key])){
        $r[$a_key] = $a[$a_key];
        if($diff && strpos($a_key, '#') === FALSE){
          $r += array(
            '#comment' => ''
          );
          $r['#comment'] .= " DIFF: $a_key missing in provided schema;";
        }
      }else{
        $r[$a_key] = SchemaXSDParser::merge_schemas($a[$a_key], $b[$a_key], $diff);
        unset($b[$a_key]);
      }
      // Match number #x keys
      foreach($b_keys as $b_key){
        if(preg_match('/^' . preg_quote($a_key, '/') . '#\d+$/', $b_key)){
          $r[$b_key] = SchemaXSDParser::merge_schemas($a[$a_key], $b[$b_key], $diff);
          unset($b[$b_key]);
        }
      }
    }
    // Now add what is left in $b
    if($diff){
      foreach($b as $b_key => $b_val){
        if(strpos($b_key, '#') === FALSE){
          $b += array(
            '#comment' => ''
          );
          $b['#comment'] .= " DIFF:$b_key missing in source schema;";
        }
      }
    }
    $r = array_merge($r, $b);
    return $r;
  }

  /**
   * Check whether two schema arrays are equivalent. As with XML
   * documents, order matters.
   *
   * This returns TRUE if the schemas are equivalent, and FALSE if not.
   * If the option 'description' parameter if provided (and is an array)
   * this gets filled with an english description of the changes.
   *
   * Note: numbered #x keys must be equal.
   */
  static function compare_schemas($a, $b, &$description = NULL, $path = 'root'){
    // Check values
    if(!is_array($a)){
      if(is_array($b)){
        if(is_array($description)){
          $description[] = "Schema item $path has been changed from a schema array to a value.";
        }
        return FALSE;
      }
      if(is_array($description) && $a !== $b){
        $description[] = "Value of $path has been changed from \"$a\" to \"$b\".";
      }
      return $a == $b;
    }
    if(!is_array($b)){
      if(is_array($description)){
        $description[] = "Schema item $path has been changed from a value to a schema array.";
      }
      return FALSE;
    }
    // Check schema arrays
    $state = TRUE;
    $b_keys = array_keys($b);
    foreach(array_keys($a) as $a_key){
      if(!array_key_exists($a_key, $b)){
        if($a_key == 'min_occurence' && $a[$a_key] == 1){
          continue;
        }else if($a_key == 'max_occurence' && $a[$a_key] == 1){
          continue;
        }else{
          if(is_array($description)){
            $val = $a_key;
            if(!is_array($a[$a_key])){
              $val .= " => \"" . $a[$a_key] . "\"";
            }
            $description[] = "Element $path>$val has been removed.";
          }
          $state = FALSE;
        }
      }else{
        // XXX For enumerations we should do an array diff rather than recurse
        $rec_state = SchemaXSDParser::compare_schemas($a[$a_key], $b[$a_key], $description, $path . '>' . $a_key);
        $state = $rec_state && $state;
      }
      unset($b[$a_key]);
    }
    if(!empty($b)){
      if(isset($b['min_occurrence']) && $b['min_occurence'] == 1){
        unset($b['min_occurence']);
      }
      if(isset($b['max_occurence']) && $b['max_occurence'] == 1){
        unset($b['max_occurence']);
      }
      if(is_array($description) && !empty($b)){
        foreach(array_keys($b) as $b_key){
          $val = $b_key;
          if(!is_array($b[$b_key])){
            $val .= " => \"" . $b[$b_key] . "\"";
          }
          $description[] = "Element $path>$val has been added.";
        }
      }
      return $state && empty($b);
    }
    return $state;
  }
}