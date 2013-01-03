<?php
/**
 * 
 * This module uses what it calls "schema array", to represent
 * an XML file schema and describe how to map Drupal fields
 * to the XML format. When the schema is applied there is an associated
 * object, from which fields may be fetched (using #field). The object
 * itself can be changed during the processing (using #load).
 * 
 * The schema array has the following format:
 *
 *  array(
 *    #min_occurence:   Minimum number of occurence. Defaults to 1.
 *                      An exception will be raised if this constraint is not respected
 *    #max_occurence':  Maximum number of occurence. Defaults to 1, use -1 for unlimited
 *                      An expception will be raised if this constraint is not respected
 *    #attributes:      Attributes to add to the element, as an array of name => value
 *    #process:         Optional function to process the value of the node before insertion.
 *                      available functions are:
 *                        - _pensoft_publication_xml_process_html: to insert HTML values
 *    #process arguments : If the process function is called, this is passed as additional argument
 *    
 *    #load:            Optional function to load the object from which fields are fetched, which
 *                      replaces the currently used object. The function is given the current
 *                      value (as defined by #value or #field) as argument.
 *                      
 *                      Note that this only affects the children of the current node, not the current
 *                      node itself or any other node processed subsequently.
 *                      
 *    #field :         A field on the current mapping object to use as value when generating the XML.
 *    
 *                      The field on the object can be a single value, or a Drupal value array:
 *                      array("und" => array("value" => ...)). If the format is different, use '#process'
 *                      to read the value from the array.
 *                      
 *                      If the field is multi-valued, then each value is inserted as a separate entry
 *                      (as long as the min/max constraints are respected)
 *    
 *    #value :          A static value to insert when generating the XML. This is inserted as many
 *                      times as required by #min_occurence.
 *                      
 *                      If both #value and #field are present, #field is used for constraint checking
 *                      (and for #load), but it's the value of #value that is inserted.
 *     
 *    #condition:       If present, the current node (and it's children) are only processed if the field
 *                      described by #condition is not empty.
 *
 *   <tag name>:        A schema array for child elements. There can be any number of
 *                      such entries. To allow for several of the same tag (for instance, several
 *                      iteration of a tag mapping to different fields) add #<number> at the end
 *                      of the tag.
 *  );
 *
 */

/**
 * This class is used to parse an XSD schema into a schema array
 * 
 */
class PensoftPublicationXSDParser {
  /**
   * Construct the parser from the URL
   * 
   */
  function __construct($url) {
    $this->schema_url = $url;
  }
  
  /**
   * Parse the given schema and return the associated schema array.
   * This will throw an Exception on error.
   * 
   */
  function get_schema_array() {
    $dom = simplexml_load_file($this->schema_url);
    if ($dom === false) {
      throw new Exception("Could not load/parse schema at " . $this->schema_url);
    }
    
    $this->dom = $dom;

    $this->schema = $this->_create_elements($dom);
    return $this->schema;
  }

  /**
   * Create the schema array representing the xsd:elements under
   * the current element.
   */
  function _create_elements($current) {
    $schema = array();
    $elements = $current->xpath('xsd:element');
    
    foreach ($elements as $element) {
      $vars = get_object_vars($element);
      $attributes = $vars['@attributes'];
      if (isset($attributes['type'])) {
        $schema[$attributes['name']] = $this->_get_type($attributes['type']);
      } else {
        $schema[$attributes['name']] = array();
      }
    
      if (isset($attributes['minOccurs']) && $attributes['minOccurs'] != 1) {
        $schema[$attributes['name']]['#min_occurence'] = $attributes['minOccurs'] == "unbounded" ? -1 : intval($attributes['minOccurs']);
      }
      if (isset($attributes['maxOccurs']) && $attributes['maxOccurs'] != 1) {
        $schema[$attributes['name']]['#max_occurence'] = $attributes['maxOccurs'] == "unbounded" ? -1 : intval($attributes['maxOccurs']);
      }
    }
    
    return $schema;    
  }
  
  /**
   * Create the schema array representing the given xsd:complexType element
   * 
   */
  function _get_type($type) {
    $types = $this->dom->xpath('xsd:complexType[@name="' . $type . '"]');
    if (count($types) == 0) {
      throw new Exception("Parse error: could not find complex type " . $type);
    } else if (count($types) > 1) {
      throw new Exception("Invalid XSD schema: more than one definition of complex type " . $type);
    }

    $type = array_shift($types);           
    $sequences = $type->xpath('xsd:sequence');
    if (count($sequences) != 1) {
      throw new Exception("XSD Schema parsing: we only support complex types with one sequence for type " . $type);
    }
    
    $sequence = array_shift($sequences);
    return $this->_create_elements($sequence);
  }  
}

/**
 * This class is used to build XML from an array schema
 * 
 */
class PensoftPublicationXMLBuilder{
  /**
   * Creatre a new builder from a schema and settings.
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
  function __construct($array_schema, $settings) {
    $this->array_schema = $array_schema;
    $this->settings = $settings;
  }
  
  /**
   * This function builds the XML from the given object.
   * 
   * This function will throw an exception on errors
   * (unless 'no-error' was set), and return
   * the DOM document.
   * 
   */
  function build_xml($object, $version = '1.0', $encoding = 'UTF-8') {
    $this->_root_object = $object;
    $this->_dom = new DOMDocument($version, $encoding);
    
    $this->_build_xml_iteration($this->array_schema, $this->_root_object, $this->_dom);
    
    return $this->_dom->saveXML();
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
  function _build_xml_iteration($schema, $object, $xml_element) {
    $inserted = FALSE;
    
    foreach ($schema as $tag => $child_schema) {
      // Ignore properties, they should be dealt with at the level above
      if (preg_match('/^#/', $tag)) {
        continue;
      }
      
      // Remove tag numbers
      if (preg_match('/^(.+)#\d+$/', $tag, $matches)) {
        $tag = $matches[1];
      }
      
      // Check if there is a conditional
      if (isset($child_schema['#condition'])) {
        $field = $child_schema['#condition'];
        if (empty($object->$field)) {
          continue;
        }
      }
      
      // Check the occurence constraint works
      $this->_build_xml_check_constraint($child_schema, $tag, $object);
      
      // Insert the value and recurse
      if (isset($child_schema['#field'])) {
        $insert_child = $this->_build_schema_insert_field($child_schema, $tag, $object, $xml_element);
        $inserted = $inserted || $insert_child;
      } else if (isset($child_schema['#value'])) {
        $insert_child = $this->_build_schema_insert_value($child_schema, $tag, $object, $xml_element);        
        $inserted = $inserted || $insert_child;
      } else {
        $insert_child = $this->_build_schema_insert_blank($child_schema, $tag, $object, $xml_element);        
        $inserted = $inserted || $insert_child;
      }
    }
    
    return $inserted;
  }
  
  /**
   * Insert a #field value, and recurse through the schema as appropriate
   * 
   */
  function _build_schema_insert_field($schema, $tag, $object, $xml_element) {
    $field = $schema['#field'];
    $values = $this->_read_values($object->$field);
    $inserted = FALSE;
    
    foreach ($values as $value) {
      $value_to_insert = $value;
      
      if (isset($schema['#value'])) {
        $value_to_insert = $schema['#value'];
      }
      
      $child_element = $this->_insert_xml_element($tag, $schema, $value_to_insert, $xml_element);
      $inserted = TRUE;
      
      $child_object = $this->_load_child_object($schema, $object, $value);
      $this->_build_xml_iteration($schema, $child_object, $child_element);
    }
    
    return $inserted;
  }

  /**
   * Insert a #value, and recurse through the schema as appropriate
   * 
   */
  function _build_schema_insert_value($schema, $tag, $object, $xml_element) {
    $inserted = FALSE;
    $count = (!isset($schema['#min_occurence']) || $schema['#min_occurence'] == 0) ? 1 : $schema['#min_occurence'];
    for ($i = 0; $i < $count; $i++) {
      $child_element = $this->_insert_xml_element($tag, $schema, $schema['#value'], $xml_element);
      $inserted = TRUE;
      $this->_build_xml_iteration($schema, $object, $child_element);
    }
    
    return $inserted;
  }

  /**
   * Insert a blank value if the element has #min_occurence > 0 or if one of the
   * children inserts a real (#value of #field) value.
   *
   */  
  function _build_schema_insert_blank($schema, $tag, $object, $xml_element) {
    $min_occurence = isset($schema['#min_occurence']) ? $schema['#min_occurence'] : 1;
    $count = ((!isset($schema['#min_occurence']) || $min_occurence == 0) ? 1 : $schema['#min_occurence']);
    $inserted = FALSE;
    
    for ($i = 0; $i < $count; $i++) {
      $child_element = $this->_insert_xml_element($tag, $schema, NULL, $xml_element);
      $insert_child = $this->_build_xml_iteration($schema, $object, $child_element);
      if (!$insert_child && $min_occurence == 0) {
        $xml_element->removeChild($child_element);
      } else if ($insert_child) {
        $inserted = TRUE;
      }
    }
    
    return $inserted;
  }

  /**
   * Load and return the object to be used by a node's children.
   * 
   */
  function _load_child_object($schema, $object, $value) {
    if (isset($schema['#load'])) {
      $arguments = array($value);
      $object = call_user_func_array($schema['#load'], $arguments);
    }
  
    return $object;
  }
  
  /**
   * Insert an element given it's array schema into the DOM.
   * 
   */
  function _insert_xml_element($tag, $schema, $value, $xml_element) {
    // Apply process function
    if (isset($schema['#process'])) {
      $f = $schema['#process'];
      if (!function_exists($f)) {
        throw new Exception("Process function $f for tag $tag does not exist");
      }
      $args = array($value);
      if (!empty($schema['#process arguments'])) {
        $args = array_merge($args, $schema['#process arguments']);
      }
      $value = call_user_func_array($f, $args);
    }
  
    // Check for standard drupal field value format
    if (is_array($value) && isset($value['value'])) {
      $value = $value['value'];
    }
  
    // Create the xml element
    $child_element = $xml_element->appendChild($this->_dom->createElement($tag));
  
    // Add attributes
    if (!empty($schema['#attributes'])) {
      foreach ($schema['#attributes'] as $attr_name => $attr_value) {
        $child_element->setAttribute($attr_name, $attr_value);
      }
    }
  
    // Add value
    if ($value !== NULL) {
      $child_element->nodeValue = (string)$value;
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
  function _build_xml_check_constraint(&$schema, $tag, $object) {
    $min_occurence = isset($schema['#min_occurence']) ? $schema['#min_occurence'] : 1;
    $max_occurence = isset($schema['#max_occurence']) ? $schema['#max_occurence'] : 1;
    
    if (!empty($schema['#field'])) {
      $field = $schema['#field'];
      if (isset($object->$field)) {
        $field_value = $this->_read_values($object->$field);
      } else {
        $field_value = array();
      }
      $count = count($field_value);
      if ($count < $min_occurence || ($max_occurence >= 0 && $count > $max_occurence)) {
        if ($this->settings['force-empty-values'] && $count == 0) {
          $schema['#value'] = '';
          unset($schema['#field']);
          drupal_set_message("Ignored: XML contraint violation: $tag occurence is $min_occurence, $max_occurence ; "
          . "associated field $field appears $count times. Value forced to empty string.", "warning");
        } else if ($this->settings['no-error']) {
          drupal_set_message("Ignored: XML contraint violation: $tag occurence is $min_occurence, $max_occurence ; "
          . "associated field $field appears $count times", "warning");
        } else {
          throw new Exception("XML contraint violation: $tag occurence is $min_occurence, $max_occurence ; associated field $field appears $count times");
        }
      }
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
  function _read_values($value, $language = 'und') {
    if (is_array($value) && isset($value[$language])) {
      return $this->_read_values($value[$language], $language);
    } else if (is_array($value)) {
      return $value;
    } else {
      return array($value);
    }
  }
  
}

/**
 * Build the XML for a publication. This function builds the
 * overall document structure, and then calls functions to build
 * each section.
 */
function _pensoft_publication_build_xml($publication) {
  // Parse the schema
  $url = variable_get('pensoft_publication_xml_schema_url', 'http://localhost/scratchpads/sites/all/modules/custom/publication/modules/pensoft_publication/research_article.xsd');
  //$source_schema = _pensoft_publication_xml_parse_schema($url);
  try {
    $parser = new PensoftPublicationXSDParser($url);
    $source_schema = $parser->get_schema_array();
  } catch (Exception $e) {
    drupal_set_message("The publication could not be published because we were unable to parse the Pensoft XSD Schema : " . $e->getMessage(), 'error');
  }
  
  // Now merge the schema with drupal field mapping
  include ('pensoft_publication.xml.schema.inc'); 
  $map_schema = array('document' => array(
    'document_info' => _pensoft_publication_xml_document_info_schema(),
    'objects' => array(
      'article_metadata' => _pensoft_publication_xml_article_metadata_schema(),
      'introduction' => _pensoft_publication_xml_introduction_schema(),
      'material_and_methods' => _pensoft_publication_xml_material_and_methods_schema(),
      'systematics' => _pensoft_publication_xml_systematics_schema(),
      'acknowledgements' => _pensoft_publication_xml_acknowledgements_schema(),
      'supplementary_files' => _pensoft_publication_xml_files_schema()
    ),
  ));
  $schema = array_merge_recursive($source_schema, $map_schema);
  
  // Build the XML
  try {
    $builder = new PensoftPublicationXMLBuilder($schema, array('force-empty-values' => TRUE));
    $xml = $builder->build_xml($publication);
  } catch (Exception $e) {
    drupal_set_message($e->getMessage(), 'error');
    return FALSE;
  }   
    
  // Validation and error display
  $xml_per_line = preg_replace('/>/', ">\n", $xml);
  $parsed_dom = new DOMDocument('1.0', 'UTF-8');
  $parsed_dom->loadXML($xml_per_line);
  libxml_use_internal_errors(true);  
  if (!$parsed_dom->schemaValidate($url)) {
    $lines = explode("\n", $xml_per_line);
    $errors = libxml_get_errors();
    foreach ($errors as $error) {
      $type = 'error';
      if ($error->level == LIBXML_ERR_WARNING) {
        $type = 'warning';
      }
      
      // Get some context.
      $context = array();
      $num = 5;
      if ($error->line > 2) {
        $count = ($error->line > $num) ? $num : ($error->line - 1);
        $context = array_slice($lines, $error->line - $count - 1, $count);
        foreach ($context as $key => $value) {
          $context[$key] = htmlspecialchars($value);
        }
      }
      $context[] = '<strong>' . htmlspecialchars($lines[$error->line-1]) . '</strong>';
      
      drupal_set_message('XML validation error: ' . $error->code . ' - ' . $error->message . ' : <br/>' . implode("<br/>", $context), $type);
    }
    libxml_clear_errors();
    libxml_use_internal_errors(false);
    
    return FALSE;
  }
  libxml_clear_errors();
  libxml_use_internal_errors(false);
  
  return $xml;
}

/**
 * Proccess a field to insert a country value
 * 
 */
function _pensoft_publication_xml_process_country($value) {
  return $value['iso2'];
}

/**
 * Loader function for the author/contributor fields
 */
function _pensoft_publication_xml_load_user($value) {
  if (isset($value['endpoints'])) {
    $endpoint = array_shift(array_values($value['endpoints']));
    list($type, $user_id) = explode(':', $endpoint);
    $user = user_load($user_id);
    return $user;    
  }
  return NULL;
}

/**
 * Loader function for files
 */
function _pensoft_publication_xml_load_file($value) {
  $value['url'] = url($value['uri'], array('absolute' => TRUE));
  return ((object)$value);
}

/**
 * Loader function for the treatment/taxon description
 */
function _pensoft_publication_xml_load_treatment($value) {
  $endpoint = array_shift(array_values($value['endpoints']));
  list($type, $nid) = explode(':', $endpoint);
  $rid = $value['relation_id'];
  $tid = $node->field_taxonomic_name["und"][0]["tid"];
  
  $relation = relation_load($rid);
  $node = node_load($nid);
  $term = taxonomy_term_load($tid);
  
  $combine = array_merge((array)$node, (array)$relation, (array)$term);
  return ((object)$combine);
}
