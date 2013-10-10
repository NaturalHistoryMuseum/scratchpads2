<?php

/**
 *
 * This module uses what it calls "schema array", to represent
 * an XML file schema and describe how to map a given object
 * to the XML format.
 *
 * The schema array is similar to other type definitions in Drupal
 * (form arrays, render arrays, etc.): items that start with a '#'
 * are properties while other items are actual values or children
 * schemas.
 *
 * Creating an XML file starts with an object (which can be an entity
 * but does not have to be) and a schema array. The schema array is
 * scanned and the XML is generated from the fields of the object.
 *
 * The schema array has the following format:
 *
 *  array(
 *    #min_occurence:   Minimum number of occurence. Defaults to 1.
 *                      An exception will be raised if this constraint is not respected
 *
 *    #max_occurence':  Maximum number of occurence. Defaults to 1, use -1 for unlimited
 *                      An expception will be raised if this constraint is not respected
 *
 *    #attributes:      Attributes to add to the XML element, as an array of name => value/callable function.
 *                      If the value is NULL, it is ignored. The current context is passed as
 *                      argument to the called function.
 *
 *    #process:         Optional callable to process the given value before insertion into the XML document.
 *                      The current context is passed as argument to the called function.
 *
 *    #process arguments : If the process function is called, this is passed as additional argument
 *    
 *    #load:            Optional function to load the object from which fields are fetched, which
 *                      replaces the currently used object. The function is given the value to load
 *                      and the current context as argument.
 *                      
 *                      The loaded object will replace the main object for the children of the current
 *                      item (but not for the item itself, or it's siblings/parents).
 *                      
 *                      If the callable returns a value that evaluates to FALSE, then the element is not
 *                      inserted. If an exception should be raised, the callable should take care of that.
 *
 *    #value :          A static value to insert when generating the XML. This is inserted as many
 *                      times as required by #min_occurence.
 *                      
 *    #field :          A field on the current object to use as value when generating the XML.
 *
 *                      If the value is a scalar, then it is inserted directly.
 *                      If the value is an array, it is expected to be as:
 *                      array(
 *                        <language code> => array(
 *                          <delta> => array(
 *                            'value' => <value>
 *                          ),
 *                          ...
 *                        ),
 *                        ...
 *                      )
 *
 *                      If the value is in a different format, then '#process' should be used to return
 *                      the array of values representing the field.
 *
 *                      If the field is multi-valued then each value is inserted as a separate XML element (and
 *                      this is valiadted against mix/max constrainsts) unless '#merge' is defined.
 *
 *                      If both #value and #field are present, #field is used for constraint checking
 *                      (and for #load), but it's the value of #value that is inserted.
 *
 *    #merge :          If a field (as defined by #field) is multi valued and '#merge' is defined and is
 *                      callable, then that function is called and only one value (the  return value of 
 *                      that function) is inserted.
 *
 *    #condition:       If present, the current item (and it's children) are only processed if the field
 *                      described by #condition is not empty, or the callable function indicated by
 *                      #condition returns TRUE. The callable is passed the current context as argument.
 *
 *    #error_info:      If present, this will be used instead of the field name in error messages
 *                      (eg. field ... is required)
 *
 *    #error_field      If present, the string '%' in the #error_info will be substituted for the value
 *                      of that field on the current object
 *
 *    #restrictions:    Optionally other restrictions. This is an array of the form:
 *                      array(
 *                        'enumeration' => array(... allowed values ...),
 *                        'attributes_enumerations' => array(
 *                          <attribute_name> => array(... allowed values ...),
 *                          ...
 *                        )
 *                      )
 *
 *   <tag name>:        A schema array for child elements. There can be any number of
 *                      such entries. To allow for several of the same tag (for instance, several
 *                      iteration of a tag mapping to different fields) add #<number> at the end
 *                      of the tag.
 *  );
 *
 * The context that is passed to callables is an array defining:
 *
 *   object:          The object being processed
 *   child_object:    The child object (if loaded using #load)
 *   field:           The current field (if applicable)
 *   delta:           The delta of the current value (for fields, if applicable)
 *   raw_value:       The actual value of the field
 *   value_to_insert: The value that will be inserted
 *   schema:          The current schema
 *
 */
/**
 * This class is used to parse an XSD schema into a schema array
 * 
 */
class PublicationXSDParser{

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
  function merge_schemas($a, $b, $diff = FALSE){
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
        $r[$a_key] = $this->merge_schemas($a[$a_key], $b[$a_key], $diff);
        unset($b[$a_key]);
      }
      // Match number #x keys
      foreach($b_keys as $b_key){
        if(preg_match('/^' . preg_quote($a_key, '/') . '#\d+$/', $b_key)){
          $r[$b_key] = $this->merge_schemas($a[$a_key], $b[$b_key], $diff);
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
        $rec_state = PublicationXSDParser::compare_schemas($a[$a_key], $b[$a_key], $description, $path . '>' . $a_key);
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

/**
 * This class is used to generate a readable output of how Drupal
 * fields are mapped to an XML type by parsing an array schema
 */
class PublicationXMLSchemaExplain{

  /**
   * Create a new schema parser from
   * an array schema
   */
  function __construct($schema){
    $this->schema = $schema;
  }

  /**
   * Generates XML-like data which shows the strcuture of the
   * generated XML and the associated Drupal fields/functions
   */
  function explain_with_xml(){
    $output = array();
    $this->_path = array();
    foreach($this->schema as $tag => $child_schema){
      if(preg_match('/^#/', $tag)){
        continue;
      }
      $output[] = "<$tag>";
      $output[] = $this->_explain_with_xml_generate($child_schema);
      $output[] = "</$tag>";
    }
    return $this->_implode_indent($output);
  }

  /**
   * Helper function to build the XML explanation
   */
  function _explain_with_xml_generate($schema){
    $output = array();
    // Show comments
    if(!empty($schema['#comment'])){
      $output[] = "COMMENT: " . $schema['#comment'];
    }
    // Add attributes
    if(!empty($schema['#attributes'])){
      foreach($schema['#attributes'] as $attr_name => $attr_value){
        if($attr_name[0] == '#'){
          continue;
        }
        if(is_object($attr_value)){
          $output[] = "SET ATTR $attr_name = function() {...})();";
        }else if($attr_value !== NULL){
          $output[] = "SET ATTR $attr_name = $attr_value;";
        }
      }
    }
    // Add values
    $value_to_set = '';
    if(isset($schema['#value'])){
      $value_to_set = $schema['#value'];
    }else if(!empty($schema['#field'])){
      $value_to_set = $this->_field_name($schema['#field']);
    }
    if($value_to_set){
      if(!isset($schema['#value']) && isset($schema['#merge'])){
        $value_to_set = $schema['#merge'] . '(' . $value_to_set . ')';
      }
      if(isset($schema['#process'])){
        // XXX need to display process arguments
        if(is_object($schema['#process'])){
          $value_to_set = '<inline function>' .'(' . $value_to_set . ')';
        }else{
          $value_to_set = $schema['#process'] .'(' . $value_to_set . ')';
        }
      }
      $output[] = "SET VALUE = " . $value_to_set . ";";
    }
    // Add conditions
    $condition = FALSE;
    if(isset($schema['#condition'])){
      if(is_callable($schema['#condition'])){
        if(is_object($schema['#condition'])){
          $output[] = "IF (...) {";
          $condition = '...()';
        }else{
          $output[] = "IF " . $schema['#condition'] . " {";
          $condition = $schema['#condition'] . '()';
        }
      }else{
        $elem = $this->_field_name($schema['#condition']);
        $condition = "$elem";
        $output[] = "IF ($condition) {";
      }
    }
    // Load new object
    $has_load = FALSE;
    if(isset($schema['#load'])){
      $elem = $this->_field_name($schema['#field']);
      $path = $schema['#load'] . '(' . $elem;
      if(isset($schema['#load arguments'])){
        $arguments = array();
        foreach($schema['#load arguments'] as $argument){
          if(is_string($argument) && preg_match('/^field:(.+)$/', $argument, $matches)){
            $load_elem = $this->_field_name($matches[1]);
            $arguments[] = $load_elem;
          }else{
            $arguments[] = 'unserialize("' . serialize($argument) . '")';
          }
        }
        $path .= ', ' . implode(', ', $arguments);
      }
      $this->_path[] = $path . ')';
      $has_load = TRUE;
    }
    // Recurse
    $sub = array();
    foreach($schema as $tag => $child_schema){
      if(strpos($tag, '#') === 0){
        continue;
      }
      $sub[] = "<$tag>";
      $sub[] = $this->_explain_with_xml_generate($child_schema);
      $sub[] = "</$tag>";
    }
    if($condition){
      $output[] = $sub;
      $output[] = "} /* $condition */";
    }else{
      $output = array_merge($output, $sub);
    }
    if($has_load){
      array_pop($this->_path);
    }
    return $output;
  }

  /**
   * Helper function to produce field name from series or loads
   */
  function _field_name($field){
    if(count($this->_path)){
      return $this->_path[count($this->_path) - 1] . '->' . $field;
    }else{
      return '$publication->' . $field;
    }
  }

  /**
   * Helper function for indenting text
   */
  function _implode_indent($struct, $indent = 0){
    $out = "";
    foreach($struct as $val){
      if(is_array($val)){
        $sub = $this->_implode_indent($val, $indent + 1);
        if(trim($sub)){
          $out .= $sub;
        }
      }else{
        $out .= str_repeat('  ', $indent) . $val . "\n";
      }
    }
    return $out;
  }
}

/**
 * This class is used to build XML from an array schema
 * 
 */
class PublicationXMLBuilder{

  /**
   * Creatre a new builder from a schema and settings and
   * a number of modifiers (objects that implement PublicationXMLModifierInterface)
   * 
   * The settings is an associative array which may define:
   * 'force-empty-values' : If the schema defines that a tag associated with
   *   a #field should be present (#min_occurence > 0) but the associated
   *   field is empty, setting this to TRUE will insert a blank value in it's
   *   place (the default is FALSE, which would raise an error)
   *   
   * 'no-error' : Ignore contraint errors (no exception thrown). Default
   *              is FALSE
   */
  function __construct($publication_name, $array_schema, $settings, $modifiers = array()){
    $this->publication_name = $publication_name;
    $this->array_schema = $array_schema;
    $this->settings = $settings;
    $this->modifiers = $modifiers;
  }

  /**
   * This function sets the active build context
   */
  function _set_context($context){
    $default = array(
      'object' => NULL,
      'child_object' => NULL,
      'field' => NULL,
      'delta' => NULL,
      'raw_value' => NULL,
      'value_to_insert' => NULL,
      'schema' => NULL
    );
    $this->_build_context[] = (object)array_merge($default, $context);
  }

  /**
   * This functions gets the active build context, as an object
   */
  function _get_context(){
    return end($this->_build_context);
  }

  /**
   * This function pops the build context stack
   */
  function _pop_context(){
    return array_pop($this->_build_context);
  }

  /**
   * This function builds the XML from the given object.
   * 
   * This function will throw an exception on errors
   * (unless 'no-error' was set), and return
   * the DOM document.
   * 
   */
  function build_xml($object, $xsd_url = NULL, $version = '1.0', $encoding = 'UTF-8'){
    $this->_root_object = $object;
    $this->_dom = new DOMDocument($version, $encoding);
    foreach($this->modifiers as $modifier){
      $modifier->start_building($this->array_schema, $this->_root_object, $this->_dom);
    }
    $this->path = array();
    $this->_build_context = array();
    $this->_set_context(array(
      'object' => $this->_root_object,
      'schema' => $this->array_schema
    ));
    $this->_build_xml_iteration($this->array_schema, $this->_root_object, $this->_dom);
    $this->_pop_context();
    $xml = $this->_dom->saveXML();
    if($xsd_url && empty($this->settings['no-error'])){
      $this->_validate_xml($xml, $xsd_url, $version, $encoding);
    }
    return $xml;
  }

  /**
   * From a schema array, object and xml element, populates the XML
   * with the computed values.
   * 
   * This function returns TRUE if some nodes were inserted because
   * they had an actual value (defined by #field or #value). This is
   * used for backtracking and removing un-needed parent elements.
   *  
   */
  function _build_xml_iteration($schema, $object, $xml_element){
    $inserted = FALSE;
    foreach($schema as $tag => $child_schema){
      // Ignore properties, they should be dealt with at the level above
      if(preg_match('/^#/', $tag)){
        continue;
      }
      // Remove tag numbers
      if(preg_match('/^(.+)#\d+$/', $tag, $matches)){
        $tag = $matches[1];
      }
      // Check if there is a conditional
      if(isset($child_schema['#condition'])){
        if(is_callable($child_schema['#condition'])){
          $cond_func = $child_schema['#condition'];
          if(!$cond_func($this->_get_context())){
            continue;
          }
        }else{
          $field = $child_schema['#condition'];
          if(empty($object->$field)){
            continue;
          }
        }
      }
      // If this is a field, obtain all the values' contexts
      if(isset($child_schema['#field'])){
        $child_schema['#values_contexts'] = $this->_get_field_values_contexts($child_schema, $tag, $object);
      }
      // Check the occurence constraint works
      $this->_build_xml_check_constraint($child_schema, $tag, $object);
      // Insert the value and recurse
      $this->path[] = $tag;
      if(isset($child_schema['#field'])){
        $insert_child = $this->_build_schema_insert_field($child_schema, $tag, $object, $xml_element);
        $inserted = $inserted || $insert_child;
      }else if(isset($child_schema['#value'])){
        $insert_child = $this->_build_schema_insert_value($child_schema, $tag, $object, $xml_element);
        $inserted = $inserted || $insert_child;
      }else{
        $insert_child = $this->_build_schema_insert_blank($child_schema, $tag, $object, $xml_element);
        $inserted = $inserted || $insert_child;
      }
      array_pop($this->path);
    }
    return $inserted;
  }

  /**
   * Return all the actual values of a given #field value
   * (applying merges and child object loads), and return
   * an array of contexts for each actual value.
   */
  function _get_field_values_contexts($schema, $tag, $object){
    $result = array();
    $field = $schema['#field'];
    if(isset($schema['#merge']) && function_exists($schema['#merge'])){
      $merge_function = $schema['#merge'];
      $values = array(
        $merge_function($object->$field)
      );
    }else{
      $values = $this->_read_values($object->$field);
    }
    foreach($this->modifiers as $modifier){
      $values = $modifier->insert_value_array($schema, $tag, $values);
    }
    foreach($values as $delta => $value){
      $value_to_insert = $value;
      if(isset($schema['#value'])){
        $value_to_insert = $schema['#value'];
      }
      try{
        $child_object = $this->_load_child_object($schema, $object, $value);
      }
      catch(Exception $e){
        $this->_generate_error($e->getMessage());
        $child_object = NULL;
      }
      if(!$child_object){
        // It's up to the loader to raise errors if need be. Here an empty return means skip it.
        continue;
      }
      $result[] = array(
        'object' => $object,
        'child_object' => $child_object,
        'field' => $field,
        'delta' => $delta,
        'raw_value' => $value,
        'value_to_insert' => $value_to_insert,
        'schema' => $schema
      );
    }
    return $result;
  }

  /**
   * Insert a #field value, and recurse through the schema as appropriate
   *
   */
  function _build_schema_insert_field($schema, $tag, $object, $xml_element){
    if(empty($schema['#values_contexts'])){return FALSE;}
    foreach($schema['#values_contexts'] as $context){
      $this->_set_context($context);
      // XXX The new Modifier functionality means we might end up with a NULL value, and thus
      // should set $inserted to FALSE (depending on min_ocurrence) to provide adequate backtracking.
      $child_element = $this->_insert_xml_element($schema, $tag, $context['value_to_insert'], $xml_element);
      $inserted = TRUE;
      $this->_build_xml_iteration($schema, $context['child_object'], $child_element);
      $this->_pop_context();
    }
    return TRUE;
  }

  /**
   * Insert a #value, and recurse through the schema as appropriate
   * 
   */
  function _build_schema_insert_value($schema, $tag, $object, $xml_element){
    $inserted = FALSE;
    $count = (!isset($schema['#min_occurence']) || $schema['#min_occurence'] == 0) ? 1 : $schema['#min_occurence'];
    $value = $schema['#value'];
    for($i = 0; $i < $count; $i++){
      $this->_set_context(array(
        'object' => $object,
        'delta' => $i,
        'raw_value' => $value,
        'value_to_insert' => $value,
        'schema' => $schema
      ));
      // XXX The new Modifier functionality means we might end up with a NULL value, and thus
      // should set $inserted to FALSE (depending on min_ocurrence) to provide adequate backtracking.
      $child_element = $this->_insert_xml_element($schema, $tag, $value, $xml_element);
      $inserted = TRUE;
      $this->_build_xml_iteration($schema, $object, $child_element);
      $this->_pop_context();
    }
    return $inserted;
  }

  /**
   * Insert a blank value if the element has #min_occurence > 0 or if one of the
   * children inserts a real (#value of #field) value.
   *
   */
  function _build_schema_insert_blank($schema, $tag, $object, $xml_element){
    $min_occurence = isset($schema['#min_occurence']) ? $schema['#min_occurence'] : 1;
    $count = ((!isset($schema['#min_occurence']) || $min_occurence == 0) ? 1 : $schema['#min_occurence']);
    $inserted = FALSE;
    for($i = 0; $i < $count; $i++){
      $this->_set_context(array(
        'object' => $object,
        'delta' => $i,
        'schema' => $schema
      ));
      // XXX The new Modifier functionality means we might end up with a non NULL , and thus
      // should set $inserted to TRUE to provide adequate backtracking.
      $child_element = $this->_insert_xml_element($schema, $tag, NULL, $xml_element);
      $insert_child = $this->_build_xml_iteration($schema, $object, $child_element);
      $this->_pop_context();
      if(!$insert_child && $min_occurence == 0){
        $xml_element->removeChild($child_element);
      }else if($insert_child){
        $inserted = TRUE;
      }
    }
    return $inserted;
  }

  /**
   * Load and return the object to be used by a node's children.
   * 
   */
  function _load_child_object($schema, $object, $value){
    if(isset($schema['#load'])){
      $arguments = array(
        $value,
        $this->_get_context()
      );
      if(isset($schema['#load arguments'])){
        $arguments = array_merge($arguments, $schema['#load arguments']);
      }
      $object = call_user_func_array($schema['#load'], $arguments);
    }
    return $object;
  }

  /**
   * Insert an element given it's array schema into the DOM.
   * 
   */
  function _insert_xml_element($schema, $tag, $value, $xml_element){
    // Apply process function
    if(isset($schema['#process'])){
      $f = $schema['#process'];
      if(!is_callable($f)){throw new Exception("Process function $f for tag $tag does not exist");}
      $args = array(
        $this->_get_context()
      );
      if(!empty($schema['#process arguments'])){
        $args = array_merge($args, $schema['#process arguments']);
      }
      $value = call_user_func_array($f, $args);
    }
    // Check for standard drupal field value format. Anything that is a direct value, or
    // an array definined 'value' will need a 'process' function defined.
    if(is_array($value) && isset($value['value'])){
      $value = $value['value'];
    }
    // Create the xml element
    $child_element = $xml_element->appendChild($this->_dom->createElement($tag));
    // Add attributes
    if(!empty($schema['#attributes'])){
      foreach($schema['#attributes'] as $attr_name => $attr_value){
        if($attr_name[0] == '#' || $attr_value === NULL){
          continue;
        }
        if(is_callable($attr_value)){
          $attr_value = $attr_value($this->_get_context());
        }
        if(!empty($schema['#restrictions']['attributes_enumerations'][$attr_name])){
          if(!in_array($attr_value, $schema['#restrictions']['attributes_enumerations'][$attr_name])){
            // XXX Raise error.
          }
        }
        $child_element->setAttribute($attr_name, $attr_value);
      }
    }
    // Add value
    foreach($this->modifiers as $modifier){
      $value = $modifier->insert_value($tag, $schema, $value);
    }
    if($value !== NULL){
      $fragment = $this->_dom->createDocumentFragment();
      $final_value = $value;
      // Escape the value if needed
      if(empty($schema['#raw'])){
        $final_value = htmlspecialchars($final_value);
      }else{
        $final_value = '<div>' . $final_value . '</div>';
      }
      // Ensure entities are numerical
      $final_value = _publication_xml_translate_entities($final_value);
      $fragment->appendXML($final_value);
      $child_element->appendChild($fragment);
    }
    return $child_element;
  }

  /**
   * This method checks that the constraint of a given schema array are
   * fullfilled by the given object.
   * 
   * Depending on the settings, this might throw an exception or modify the
   * schema array's value to ensure that the constraint are respected.
   */
  function _build_xml_check_constraint(&$schema, $tag, $object){
    $min_occurence = isset($schema['#min_occurence']) ? $schema['#min_occurence'] : 1;
    $max_occurence = isset($schema['#max_occurence']) ? $schema['#max_occurence'] : 1;
    if(!empty($schema['#field'])){
      $field = $schema['#field'];
      $count = count($schema['#values_contexts']);
      if($count < $min_occurence || ($max_occurence >= 0 && $count > $max_occurence)){
        $message_prepare = "the field %field";
        if($count == 0){
          $message_prepare .= " is required.";
        }else if($count < $min_occurence){
          $message_prepare .= " must be present at least %min times, but is only here %count times.";
        }else{
          $message_prepare .= " can be present at most %max times, but is here %count times.";
        }
        $error_field = $field;
        if(isset($schema['#error_info'])){
          $error_field = $schema['#error_info'];
          if(isset($schema['#error_field'])){
            $error_context_field = $schema['#error_field'];
            if(isset($object->$error_context_field)){
              $error_context = reset($this->_read_values($object->$error_context_field));
              if(is_array($error_context) && isset($error_context['value'])){
                $error_context = $error_context['value'];
              }
              $error_field = str_replace('%', $error_context, $error_field);
            }
          }
        }
        $message = t($message_prepare, array(
          '%field' => $error_field,
          '%min' => $min_occurence,
          '%max' => $max_occurence,
          '%count' => $count,
          '%tag' => $tag
        ));
        $this->_generate_error($message);
        if($this->settings['force-empty-values'] && $count == 0){
          // If no exception was raised and we are asked to enter empty values, make sure we adapt the schema
          $schema['#value'] = '';
          unset($schema['#field']);
        }
      }
    }
  }

  /**
   * This function registers an error and either interupts processing by
   * throwing an exception, or logs the error and returns
   */
  function _generate_error($message){
    $message = t('The publication %publication could not be generated, because !message. (XML Path where error occured: %path)', array(
      '%publication' => $this->publication_name,
      '!message' => $message,
      '%path' => implode(' >> ', $this->path)
    ));
    if($this->settings['force-empty-values'] && $count == 0){
      drupal_set_message(t("The following error happened but was ignored due to settings, and an empty value was set in it's place: ") . $message, 'warning');
    }else if($this->settings['no-error']){
      drupal_set_message(t("The following error happened but was ignored due to settings: ") . $message, 'warning');
    }else{
      throw new Exception($message);
    }
  }

  /**
   * Given a value, taken from a field or a static value,
   * return it in a consistent manner as an array
   * containing all the values.
   * 
   * If this comes from a Drupal field and contains a language
   * array, this is removed to return the values directly
   * 
   */
  function _read_values($value, $language = LANGUAGE_NONE){
    if(is_array($value) && isset($value[$language])){
      return $this->_read_values($value[$language], $language);
    }else if(is_array($value)){
      return $value;
    }else{
      return array(
        $value
      );
    }
  }

  /**
   * Validates generated XML against an XSD schema, and throw
   * an Excpetion if the validation fails
   */
  function _validate_xml($xml, $xsd_url, $version = '1.0', $encoding = 'UTF-8'){
    // Try to break the XML down in lines such that we don't disturb content by adding unwanted
    // carriage returns (that would affect enumerations for instance).
    $xml_per_line = preg_replace('/(<\/.*?>)/', "$1\n", $xml);
    $parsed_dom = new DOMDocument($version, $encoding);
    $parsed_dom->loadXML($xml_per_line);
    libxml_use_internal_errors(true);
    if(!$parsed_dom->schemaValidate($xsd_url)){
      $lines = explode("\n", $xml_per_line);
      $errors = libxml_get_errors();
      $error_messages = array();
      foreach($errors as $error){
        $type = 'error';
        if($error->level == LIBXML_ERR_WARNING){
          $type = 'warning';
        }
        // Get some context.
        $context = array();
        $num = 5;
        if($error->line > 2){
          $count = ($error->line > $num) ? $num : ($error->line - 1);
          $context = array_slice($lines, $error->line - $count - 1, $count);
          foreach($context as $key => $value){
            $context[$key] = htmlspecialchars($value);
          }
        }
        $context[] = '<strong>' . htmlspecialchars($lines[$error->line - 1]) . '</strong>';
        $error_messages[] = t("XML validation error: @code - @message !context", array(
          '@code' => $error->code,
          '@message' => $error->message,
          '!context' => '<br/>' . implode('<br/>', $context)
        ));
      }
      libxml_clear_errors();
      libxml_use_internal_errors(false);
      throw new Exception('<ul><li>' . implode('</li><li>', $error_messages) . '</li></ul>');
    }
    libxml_clear_errors();
    libxml_use_internal_errors(false);
  }
}

/**
 * This interface defines a publication xml modifier
 *
 * Such objects are given to the PublicationXMLBuilder,
 * and are called at every iteration. They can be used
 * to:
 * - Track references ;
 * - Order fields ;
 * - Modify/filter content
 *
 * Note: we use registered objects rather than hooks,
 * because the main purpose of this is to rename
 * citations and order cited objects accordingly
 * (eg. Table 1, Table 2, etc.) - this requires that
 * we keep context throughout the process which is
 * not always practical with hooks.
 */
interface PublicationXMLModifierInterface{

  /**
   * function start_building
   *
   * This is called before the XML building starts.
   *
   * $schema: The array schema used for building the XML
   *
   * $object: the object from which the XML
   * will be build
   *
   * $dom: The empty DomDocument object
   */
  public function start_building($schema, $object, $dom);

  /**
   * function insert_value
   *
   * This is called when an actual value is about to be
   * inserted into the DOM, and should return the
   * modified value to insert. Classes implementing
   * this interface should at least return $value here.
   *
   * $tag: The tag of the value being inserted 
   * $schema: The schema for the tag
   * $value: The value itself
   */
  public function insert_value($schema, $tag, $value);

  /**
   * function insert_value_array
   * 
   * This is called when a number of values comming from the
   * same field are going to be inserted sequencially with the
   * same tag. This should return the array of values, and
   * can be used to filter out some values or to order the values.
   * Classes implementing this interface should at least return $values
   * here.
   *
   * Note that each individual value will still go throug 'insert_value'
   * 
   * $schema: The schema for the tag
   * $tag: The tag of the value being inserted
   * $values: The array of values
   */
  public function insert_value_array($schema, $tag, $values);
}

/**
 * Process a country field to return either the iso2 code or
 * the country name
 *
 */
function _publication_xml_process_country($context, $type = 'name'){
  $value = $context->value_to_insert;
  if($type == 'iso2'){
    return $value['iso2'];
  }else{
    $country = country_load($value['iso2']);
    return $country->name;
  }
}

/**
 * Process a map field to return just one property (by default the latitude)
 */
function _publication_xml_process_map($context, $type = 'latitude'){
  $value = $context->value_to_insert;
  if(isset($value[LANGUAGE_NONE][0])){
    $map = (array)($value[LANGUAGE_NONE][0]);
  }else{
    $map = (array)$value;
  }
  return $map[$type];
}

/**
 * Loader function which simply returns the value.
 *
 * This has the effect of replacing the current object
 * with the current value
 */
function _publication_xml_load_replace($value, $context){
  return $value;
}

/**
 * Loader function for the author/contributor fields
 */
function _publication_xml_load_user($value, $context){
  if(is_array($value) && isset($value['uid'])){
    $value = $value['uid'];
  }
  $user = user_load($value);
  if(!$user){throw new Exception(t("No such user id %uid - it looks like a user referenced from your publication has been deleted from the site", array(
      '%uid' => $value
    )));}
  $wrapper = entity_metadata_wrapper('user', $user);
  $given_names = explode(' ', $wrapper->field_user_given_names->value());
  $user->processed_full_name = implode(' ', array(
    $wrapper->field_user_title->value(),
    $wrapper->field_user_given_names->value(),
    $wrapper->field_user_family_name->value()
  ));
  $user->processed_first_name = array_shift($given_names);
  if(count($given_names)){
    $user->processed_middle_names = implode(' ', $given_names);
  }
  return $user;
}

/**
 * Merge function for merging multiple users into one
 */
function _publication_xml_merge_user($field){
  $values = $field[LANGUAGE_NONE];
  $output = array();
  foreach($values as $value){
    $user = _publication_xml_load_user($value);
    $output[] = $user->processed_full_name;
  }
  $obj = new StdClass();
  $obj->merged_full_name = implode(', ', $output);
  return $obj;
}

/**
 * Loader function for files
 */
function _publication_xml_load_file($value, $context){
  $value['url'] = url($value['uri'], array(
    'absolute' => TRUE
  ));
  return ((object)$value);
}

/**
 * Loads the given term
 */
function _publication_xml_load_term($value, $context, $final_name = NULL){
  $term = taxonomy_term_load($value['tid']);
  if($final_name){
    $term->name = $final_name;
  }
  return $term;
}

/**
 * Loads the parent of the given term
 */
function _publication_xml_load_parent_term($value, $context, $rank = NULL, $default = NULL){
  if(is_array($value) && isset($value['tid'])){
    $value = $value['tid'];
  }
  $terms = taxonomy_get_parents($value);
  while(!empty($terms)){
    $current = array_shift($terms);
    if($rank === NULL || $current->field_rank[LANGUAGE_NONE][0]['value'] == $rank){return $current;}
    $parents = taxonomy_get_parents($current->tid);
    $terms = array_merge($terms, $parents);
  }
  return $default;
}

/**
 * Loader function for materials
 */
function _publication_xml_load_specimen($value, $context){
  $specimen = node_load($value['nid']);
  if(!empty($specimen->field_location[$specimen->language][0]['nid'])){
    $location = node_load($specimen->field_location[$specimen->language][0]['nid']);
    $location->location_title = $location->title;
    unset($location->title);
    $specimen = ((object)(array_merge((array)$specimen, (array)($location))));
  }
  $specimen->field_collector[LANGUAGE_NONE] = array();
  $specimen->field_identified_by[LANGUAGE_NONE] = array();
  return $specimen;
}

/**
 * Translate charater entities into numeric entities, as vanilla XML does
 * not accept all HTML entities.
 */
function _publication_xml_translate_entities($str){
  $map = array(
    "&quot;" => "&#x0022;",
    "&amp;" => "&#x0026;",
    "&apos;" => "&#x0027;",
    "&lt;" => "&#x003C;",
    "&gt;" => "&#x003E;",
    "&nbsp;" => "&#x00A0;",
    "&iexcl;" => "&#x00A1;",
    "&cent;" => "&#x00A2;",
    "&pound;" => "&#x00A3;",
    "&curren;" => "&#x00A4;",
    "&yen;" => "&#x00A5;",
    "&brvbar;" => "&#x00A6;",
    "&sect;" => "&#x00A7;",
    "&uml;" => "&#x00A8;",
    "&copy;" => "&#x00A9;",
    "&ordf;" => "&#x00AA;",
    "&laquo;" => "&#x00AB;",
    "&not;" => "&#x00AC;",
    "&shy;" => "&#x00AD;",
    "&reg;" => "&#x00AE;",
    "&macr;" => "&#x00AF;",
    "&deg;" => "&#x00B0;",
    "&plusmn;" => "&#x00B1;",
    "&sup2;" => "&#x00B2;",
    "&sup3;" => "&#x00B3;",
    "&acute;" => "&#x00B4;",
    "&micro;" => "&#x00B5;",
    "&para;" => "&#x00B6;",
    "&middot;" => "&#x00B7;",
    "&cedil;" => "&#x00B8;",
    "&sup1;" => "&#x00B9;",
    "&ordm;" => "&#x00BA;",
    "&raquo;" => "&#x00BB;",
    "&frac14;" => "&#x00BC;",
    "&frac12;" => "&#x00BD;",
    "&frac34;" => "&#x00BE;",
    "&iquest;" => "&#x00BF;",
    "&Agrave;" => "&#x00C0;",
    "&Aacute;" => "&#x00C1;",
    "&Acirc;" => "&#x00C2;",
    "&Atilde;" => "&#x00C3;",
    "&Auml;" => "&#x00C4;",
    "&Aring;" => "&#x00C5;",
    "&AElig;" => "&#x00C6;",
    "&Ccedil;" => "&#x00C7;",
    "&Egrave;" => "&#x00C8;",
    "&Eacute;" => "&#x00C9;",
    "&Ecirc;" => "&#x00CA;",
    "&Euml;" => "&#x00CB;",
    "&Igrave;" => "&#x00CC;",
    "&Iacute;" => "&#x00CD;",
    "&Icirc;" => "&#x00CE;",
    "&Iuml;" => "&#x00CF;",
    "&ETH;" => "&#x00D0;",
    "&Ntilde;" => "&#x00D1;",
    "&Ograve;" => "&#x00D2;",
    "&Oacute;" => "&#x00D3;",
    "&Ocirc;" => "&#x00D4;",
    "&Otilde;" => "&#x00D5;",
    "&Ouml;" => "&#x00D6;",
    "&times;" => "&#x00D7;",
    "&Oslash;" => "&#x00D8;",
    "&Ugrave;" => "&#x00D9;",
    "&Uacute;" => "&#x00DA;",
    "&Ucirc;" => "&#x00DB;",
    "&Uuml;" => "&#x00DC;",
    "&Yacute;" => "&#x00DD;",
    "&THORN;" => "&#x00DE;",
    "&szlig;" => "&#x00DF;",
    "&agrave;" => "&#x00E0;",
    "&aacute;" => "&#x00E1;",
    "&acirc;" => "&#x00E2;",
    "&atilde;" => "&#x00E3;",
    "&auml;" => "&#x00E4;",
    "&aring;" => "&#x00E5;",
    "&aelig;" => "&#x00E6;",
    "&ccedil;" => "&#x00E7;",
    "&egrave;" => "&#x00E8;",
    "&eacute;" => "&#x00E9;",
    "&ecirc;" => "&#x00EA;",
    "&euml;" => "&#x00EB;",
    "&igrave;" => "&#x00EC;",
    "&iacute;" => "&#x00ED;",
    "&icirc;" => "&#x00EE;",
    "&iuml;" => "&#x00EF;",
    "&eth;" => "&#x00F0;",
    "&ntilde;" => "&#x00F1;",
    "&ograve;" => "&#x00F2;",
    "&oacute;" => "&#x00F3;",
    "&ocirc;" => "&#x00F4;",
    "&otilde;" => "&#x00F5;",
    "&ouml;" => "&#x00F6;",
    "&divide;" => "&#x00F7;",
    "&oslash;" => "&#x00F8;",
    "&ugrave;" => "&#x00F9;",
    "&uacute;" => "&#x00FA;",
    "&ucirc;" => "&#x00FB;",
    "&uuml;" => "&#x00FC;",
    "&yacute;" => "&#x00FD;",
    "&thorn;" => "&#x00FE;",
    "&yuml;" => "&#x00FF;",
    "&OElig;" => "&#x0152;",
    "&oelig;" => "&#x0153;",
    "&Scaron;" => "&#x0160;",
    "&scaron;" => "&#x0161;",
    "&Yuml;" => "&#x0178;",
    "&fnof;" => "&#x0192;",
    "&circ;" => "&#x02C6;",
    "&tilde;" => "&#x02DC;",
    "&Alpha;" => "&#x0391;",
    "&Beta;" => "&#x0392;",
    "&Gamma;" => "&#x0393;",
    "&Delta;" => "&#x0394;",
    "&Epsilon;" => "&#x0395;",
    "&Zeta;" => "&#x0396;",
    "&Eta;" => "&#x0397;",
    "&Theta;" => "&#x0398;",
    "&Iota;" => "&#x0399;",
    "&Kappa;" => "&#x039A;",
    "&Lambda;" => "&#x039B;",
    "&Mu;" => "&#x039C;",
    "&Nu;" => "&#x039D;",
    "&Xi;" => "&#x039E;",
    "&Omicron;" => "&#x039F;",
    "&Pi;" => "&#x03A0;",
    "&Rho;" => "&#x03A1;",
    "&Sigma;" => "&#x03A3;",
    "&Tau;" => "&#x03A4;",
    "&Upsilon;" => "&#x03A5;",
    "&Phi;" => "&#x03A6;",
    "&Chi;" => "&#x03A7;",
    "&Psi;" => "&#x03A8;",
    "&Omega;" => "&#x03A9;",
    "&alpha;" => "&#x03B1;",
    "&beta;" => "&#x03B2;",
    "&gamma;" => "&#x03B3;",
    "&delta;" => "&#x03B4;",
    "&epsilon;" => "&#x03B5;",
    "&zeta;" => "&#x03B6;",
    "&eta;" => "&#x03B7;",
    "&theta;" => "&#x03B8;",
    "&iota;" => "&#x03B9;",
    "&kappa;" => "&#x03BA;",
    "&lambda;" => "&#x03BB;",
    "&mu;" => "&#x03BC;",
    "&nu;" => "&#x03BD;",
    "&xi;" => "&#x03BE;",
    "&omicron;" => "&#x03BF;",
    "&pi;" => "&#x03C0;",
    "&rho;" => "&#x03C1;",
    "&sigmaf;" => "&#x03C2;",
    "&sigma;" => "&#x03C3;",
    "&tau;" => "&#x03C4;",
    "&upsilon;" => "&#x03C5;",
    "&phi;" => "&#x03C6;",
    "&chi;" => "&#x03C7;",
    "&psi;" => "&#x03C8;",
    "&omega;" => "&#x03C9;",
    "&thetasym;" => "&#x03D1;",
    "&upsih;" => "&#x03D2;",
    "&piv;" => "&#x03D6;",
    "&ensp;" => "&#x2002;",
    "&emsp;" => "&#x2003;",
    "&thinsp;" => "&#x2009;",
    "&zwnj;" => "&#x200C;",
    "&zwj;" => "&#x200D;",
    "&lrm;" => "&#x200E;",
    "&rlm;" => "&#x200F;",
    "&ndash;" => "&#x2013;",
    "&mdash;" => "&#x2014;",
    "&lsquo;" => "&#x2018;",
    "&rsquo;" => "&#x2019;",
    "&sbquo;" => "&#x201A;",
    "&ldquo;" => "&#x201C;",
    "&rdquo;" => "&#x201D;",
    "&bdquo;" => "&#x201E;",
    "&dagger;" => "&#x2020;",
    "&Dagger;" => "&#x2021;",
    "&bull;" => "&#x2022;",
    "&hellip;" => "&#x2026;",
    "&permil;" => "&#x2030;",
    "&prime;" => "&#x2032;",
    "&Prime;" => "&#x2033;",
    "&lsaquo;" => "&#x2039;",
    "&rsaquo;" => "&#x203A;",
    "&oline;" => "&#x203E;",
    "&frasl;" => "&#x2044;",
    "&euro;" => "&#x20AC;",
    "&image;" => "&#x2111;",
    "&weierp;" => "&#x2118;",
    "&real;" => "&#x211C;",
    "&trade;" => "&#x2122;",
    "&alefsym;" => "&#x2135;",
    "&larr;" => "&#x2190;",
    "&uarr;" => "&#x2191;",
    "&rarr;" => "&#x2192;",
    "&darr;" => "&#x2193;",
    "&harr;" => "&#x2194;",
    "&crarr;" => "&#x21B5;",
    "&lArr;" => "&#x21D0;",
    "&uArr;" => "&#x21D1;",
    "&rArr;" => "&#x21D2;",
    "&dArr;" => "&#x21D3;",
    "&hArr;" => "&#x21D4;",
    "&forall;" => "&#x2200;",
    "&part;" => "&#x2202;",
    "&exist;" => "&#x2203;",
    "&empty;" => "&#x2205;",
    "&nabla;" => "&#x2207;",
    "&isin;" => "&#x2208;",
    "&notin;" => "&#x2209;",
    "&ni;" => "&#x220B;",
    "&prod;" => "&#x220F;",
    "&sum;" => "&#x2211;",
    "&minus;" => "&#x2212;",
    "&lowast;" => "&#x2217;",
    "&radic;" => "&#x221A;",
    "&prop;" => "&#x221D;",
    "&infin;" => "&#x221E;",
    "&ang;" => "&#x2220;",
    "&and;" => "&#x2227;",
    "&or;" => "&#x2228;",
    "&cap;" => "&#x2229;",
    "&cup;" => "&#x222A;",
    "&int;" => "&#x222B;",
    "&there4;" => "&#x2234;",
    "&sim;" => "&#x223C;",
    "&cong;" => "&#x2245;",
    "&asymp;" => "&#x2248;",
    "&ne;" => "&#x2260;",
    "&equiv;" => "&#x2261;",
    "&le;" => "&#x2264;",
    "&ge;" => "&#x2265;",
    "&sub;" => "&#x2282;",
    "&sup;" => "&#x2283;",
    "&nsub;" => "&#x2284;",
    "&sube;" => "&#x2286;",
    "&supe;" => "&#x2287;",
    "&oplus;" => "&#x2295;",
    "&otimes;" => "&#x2297;",
    "&perp;" => "&#x22A5;",
    "&sdot;" => "&#x22C5;",
    "&lceil;" => "&#x2308;",
    "&rceil;" => "&#x2309;",
    "&lfloor;" => "&#x230A;",
    "&rfloor;" => "&#x230B;",
    "&lang;" => "&#x2329;",
    "&rang;" => "&#x232A;",
    "&loz;" => "&#x25CA;",
    "&spades;" => "&#x2660;",
    "&clubs;" => "&#x2663;",
    "&hearts;" => "&#x2665;",
    "&diams;" => "&#x2666;"
  );
  return str_replace(array_keys($map), array_values($map), $str);
}
