<?php
/**
 * This functionality is used to parse XML into entities. To do this, it uses
 * a Reverse Schema Array, defined as:
 *
 * array(
 *   <entity type[#n]> => array(
 *     '#xpath': Required, defines the XML nodes used to create/fetch entities.
 *               Should be an absolute xpath.
 *
 *     '#id':    Optional, defines the XML value (relative to '#xpath') that returns the
 *               identifier for this element in the XML. This is used to create a map
 *               from xml id to entity (usefull when the XML uses references)
 *
 *     '#get':    Optional, defines a callable that will fetch or create the entity.
 *                The callable will be invoked with at least two arguments:
 *                  - The CharacterEditorMapXML object ;
 *                  - The XML node to parse to fetch/create the entity.
 *
 *                Entities obtained via '#get' will be re-saved if (and only if) there are
 *                additional fields that are mapped from the XML (and autosave is enabled)
 *
 *                If the callable returns FALSE the entry will be skipped unless '#create'
 *                is also defined.
 *                
 *     '#xget':   Similar to get, except that additional fields are never mapped from the XML
 *                and the entity is never saved. 
 *
 *     '#create': Optional, defines the array of default values used to create an entity
 *                (by calling entity_create). This should at least define the bundle, though
 *                values such as uid, created, etc. are usually expected too.
 *
 *                This may define a '#process' callable that is invoked with the created
 *                entitiy after it is created and before it is saved.
 *
 *                Entities created via '#create' are always saved
 *
 *     <field name[:property name]>: Any number of fields on the entity (with an optional
 *                property name). The assoicated value can either a string (in which case it is
 *                assumed to define the xpath to obtain that field's value) or an array that
 *                defines:
 *                  #xpath: Required, the relative xpath to get the value ;
 *                  #process: Optional, a callable to process the value before saving it.
 *
 *                            The callable is called with the CharacterEditorMapXML object
 *                            as first argument and the current value as second argument.
 *
 *                            The returned value is set on the current field.
 *
 *                 #set: Optional, a callable to process and set the value on the entity directly.
 *                       This is usefull for special cases, such as field collections or relations,
 *                       where attaching the value is not done by setting a field value, and the XML
 *                       does not contain ids that would allow later mapping.
 *
 *                       The callable is invoked with the CharacterEditorMapXML Object
 *                       as first argument, the current entity as second argument, the
 *                       field name property as third and fourth,  and
 *                       the current value as fith argument.
 *
 *                       #set and #process cannot be defined at the same time.
 *
 *                 #default: A default value to use when none is present
 *
 *                 #combine: If this is set and TRUE, then multiple values matching the xpath
 *                           will be combined into one, rather than saved in multiple values
 *                           of the field.
 *                           
 *                           Note: This is currently only supported when #set is set, in
 *                           which case #set is sent an array of values rather than a single
 *                           value.
 *                           
 *                           When this is used with #set the field is never accessed directly, 
 *                           and thus entity metadata wrappers are not used on that field. 
 *                           This can be usefull for entity types that don't expose their data 
 *                           correctly (eg. relations).
 *
 *                If fields are defined then the entity will be saved, regardless of whether
 *                it's been obtained via '#get' or '#create' (unless autosave is disabled)
 *  )
 *)
 *
 * Notes:
 * - The optional '#<id>' after the entity type can be used if the same entity type is required
 *   multiple times in a definition. The value after the '#' is ignored.
 *
 * - At least one of '#get', '#xget' and '#create' is required.
 * 
 * - '#get' and '#xget' are mutually exclusive - however reverse schema arrays hooks 
 *   may define both as long as one of the two is later filtered out using the 
 *   REVERSE_SCHEMA_ARRAY modes when calling reverse_schema_array
 *   
 * - If both '#get/#xget' and '#create' are defined, then '#create' is only invoked if '#get/#xget'
 *   returns false.
 *
 * - callables can either be strings (function names) or arrays, in which case the first
 *   element is assumed to be the callable (which can be any PHP callable) and subsequent
 *   arguments are passed on to the function.
 *
 * - All callables are invoked with the ReverseSchemaArrayMapXML object as first argument,
 *   callable-specific arguments after that and arguments defined in the callable definition
 *   last.
 *
 * - Calls to '#process' and '#set' may raise an excpetion of type ReverseSchemaArrayException
 *   defining how to continue processing (skip current value, skip entity or cancel import) ;
 *
 * - Additional entity creation steps that cannot be done via fields/wrappers are typically
 *   done by adding a '#set' function to the entity id field ;
 *
 * - Object of this class may be used a number of times on the same XML data - for processing
 *   that requires intermeditate steps - however only one DOM document may be used.
 *
 */
define('REVERSE_SCHEMA_ARRAY_CREATE', 1);
define('REVERSE_SCHEMA_ARRAY_GET', 2);
define('REVERSE_SCHEMA_ARRAY_XGET', 4);
define('REVERSE_SCHEMA_ARRAY_ALL', 7);
define('REVERSE_SCHEMA_ARRAY_SKIP_ENTITY', 1);
define('REVERSE_SCHEMA_ARRAY_SKIP_VALUE', 2);

/**
 * This method is used to fetch the given reverse schema array.
 *
 * It will get the schemas returned by module that implement
 * 'reverse_schema_array($schema_name, $variables)' and
 * 'reverse_schema_array_<schema_name>($variables)'
 * and merge them.
 *
 * The final schema will be altered by calling
 * 'reverse_schema_array_alter($schema_name, $schema, $variables)' and
 * 'reverse_schema_array_<schema_name>_alter($schema, $variables)'.
 * 
 * $rsa_mode is a combination of:
 * REVERSE_SCHEMA_ARRAY_CREATE: If present '#create' statements are returned, otherwise they are removed ;
 * REVERSE_SCHEMA_ARRAY_GET: If present '#get' statements are returned, otherwise they are removed ;
 * REVERSE_SCHEMA_ARRAY_XGET: If present '#xget' statements are returned, otherwise they are removed. If no '#xget' statement
 *                            if present and REVERSE_SCHEMA_ARRAY_GET is not defined then '#get' statements are
 *                            transformed into '#xget' statements.
 */
function reverse_schema_array($schema_name, $rsa_mode = REVERSE_SCHEMA_ARRAY_ALL, $variables = array()){
  $result = array();
  $safe_name = preg_replace('/[^a-zA-Z0-9_]/', '', $schema_name);
  $modules = module_implements('reverse_schema_array');
  foreach($modules as $module){
    $fn = $module . '_reverse_schema_array';
    $sc = $fn($schema_name, $variables);
    $result = array_merge_recursive($result, $sc);
  }
  $modules = module_implements('reverse_schema_array_' . $schema_name);
  foreach($modules as $module){
    $fn = $module . '_reverse_schema_array_' . $safe_name;
    $sc = $fn($variables);
    $result = array_merge_recursive($result, $sc);
  }
  $modules = module_implements('reverse_schema_array_alter');
  foreach($modules as $module){
    $fn = $module . '_reverse_schema_array_alter';
    $fn($schema_name, $result);
  }
  $modules = module_implements('reverse_schema_array_' . $safe_name . '_alter');
  foreach($modules as $module){
    $fn = $module . '_reverse_schema_array_' . $safe_name . '_alter';
    $fn($result);
  }
  foreach($result as $type => $values){
    if(!($rsa_mode & REVERSE_SCHEMA_ARRAY_CREATE)){
      unset($result[$type]['#create']);
    }
    if(!($rsa_mode & REVERSE_SCHEMA_ARRAY_GET)){
      if(($rsa_mode & REVERSE_SCHEMA_ARRAY_XGET) && isset($result[$type]['#get']) && empty($result[$type]['#xget'])){
        $result[$type]['#xget'] = $result[$type]['#get'];
      }
      unset($result[$type]['#get']);
    }
    if(!($rsa_mode & REVERSE_SCHEMA_ARRAY_XGET)){
      unset($result[$type]['#xget']);
    }
  }
  return $result;
}

/**
 * ReverseSchemaArrayMapXMP
 * 
 * This is the main class used to parsing XML into entities.
 * 
 * Usage:
 * 
 * 1. construct with a SimpleXML object, a schema and options:
 * 
 *   $content = simplexml_load_file('example.xml');
 *   $map = new ReverseSchemaArrayMapXml($content, 'http://example.org', array(
 *     'autosave' => FALSE
 *   ));
 * 
 *   Available options are:
 *   - autosave: If TRUE entities will be saved when they are created. Otherwise
 *               they can be saved by calling 'saveEntities' manually.
 *             
 * 2. Get a reverse schema array:
 * 
 *   $schema = reverse_schema_array('example_reverse_schema');
 *   
 * 3. Parse the schema array:
 * 
 *   $map->mapXML($schema);
 *   
 *   This will:
 *   - Add entities to the list of created entities ;
 *   - Add entities to the list of saved entities (or entities that need saving, depending
 *     on the value of 'autosave') ;
 *   - Add messages to the list of messages ;
 *   - Update the info array
 *   
 *   Note that mapXML can be called any number of times with different schemas, allowing for
 *   extra processing to happen in between import steps.
 *   
 * 4. Output information:
 *   $info = $map->flushInfo();
 *   echo "Obtained $info[total] entities,
 *         of which $info[mapped] were mapped
 *         from existing entities and $info[new]
 *         were newly created entities.
 *         
 *         $info[saved] entities need saving;
 *         of which $info[updated] were existing
 *         entities whose fields were updated.
 *         
 *         Messages created during processing:\n";
 *  foreach($map->flushMessages() as $msg){
 *    echo "- $msg ;\n";
 *  }
 *  
 * 5. Save entities:
 *   $map->saveEntities();
 *   
 *   Note that this will not re-save entities if autosave is set to on.
 */
class ReverseSchemaArrayMapXml{
  // Options
  public $options;
  /* The root SimpleXML Object */
  private $dom;
  /* The XML Schema URI */
  private $schema;
  /* Processing information */
  private $info;
  /* Messages that were created during the parsing.*/
  private $messages = array();
  /* The entities that were loaded/created */
  private $entities = array();
  /* The entities that were loaded/created and for which we could map an XML id */
  private $xml_id_maps = array();
  /* The entities that were  saved or that need saving (if autosave if off) */
  private $save_entities = array();

  /**
   * __construct
   *
   * Create a new CharacterEditorMapXML Object from a SimpleXML object
   * and an XML URI schema.
   *
   * The Schema URI is only used to allow Drupal alter hooks to know which
   * XML is being parsed. No actual valiadation of the XML takes place.
   *
   * The available options are:
   * 'autosave': If TRUE, entities will be saved when they are created. Otherwise
   *             it's the callers responsiblity to do this. Default : TRUE
  */
  function __construct($dom, $schema, $options = array()){
    $this->dom = $dom;
    $this->schema = $schema;
    $this->options = array_merge(array(
      'autosave' => TRUE
    ), $options);
    $this->flushInfo();
  }

  /**
   * addMessage
   *
   * Add a message to the message list. Typically this
   * would be called by callables (#get, #set, etc.)
   * to report problems.
   */
  function addMessage($message){
    $this->messages[] = $message;
  }

  /**
   * getMessages
   * 
   * Returns the current list of messages
   */
  function getMessages(){
    return $this->messages;
  }

  /**
   * flushMessages
   * 
   * Returns the current message list and empties it.
   */
  function flushMessages(){
    $messages = $this->messages;
    $this->messages = array();
    return $messages;
  }

  /**
   * getInfo
   * 
   * Return the info array. This defines:
   * total:   Total number of entities that were obtained. 
   *          Note that total = loaded + new ;
   * loaded:  Number of entities that were obtained via
   *          a '#get' call. In most cases this will be
   *          the number of entities that were mapped
   *          to XML values and loaded from the database,
   *          though '#get' implementation may create
   *          entities.
   * new:     Number of entities that were created ;
   * saved:   Number of entities that were saved/need saving.
   *          Note that saved = new + updated ;
   * updated: Number of entities that were obtained via
   *          '#get' but whose fields were also updated ;
   * skipped: Number of entities that failed to load via '#get'
   *          and for which there were no associated '#create'
   *          directive.
   */
  function getInfo(){
    return $this->info;
  }

  /**
   * flushInfo
   * 
   * Return the info array and reset it
   */
  function flushInfo(){
    $old_info = $this->info;
    $this->info = array(
      'total' => 0,
      'loaded' => 0,
      'new' => 0,
      'saved' => 0,
      'updated' => 0,
      'skipped' => 0
    );
    return $old_info;
  }

  /**
   * saveEntities
   * 
   * Save all the entities that require saving and empty
   * the list.
   * 
   * Note that if 'autosave' is enabled and $force is FALSE,
   * it is assumed all entities have already been saved and 
   * this call merely empties the list of entities that require 
   * saving.
   */
  public function saveEntities($force = FALSE){
    if(!$this->options['autosave']){
      foreach($this->save_entities as $entity_w){
        $entity_w->save();
      }
    }
    $this->save_entities = array();
  }

  /**
   * Get the list of entities that were loaded/created.
   * 
   * If $entity_type is defined, then return an array
   * of entity metadatawrappers for that entity type.
   * If it is not defined, then return an array of 
   * entity type to an array of entity metadata wrappers.
   * 
   * If $map is TRUE, the array of entity metadata wrappers
   * is keyed by the XML id we mapped, and entities that
   * were not mapped are not returned.
   */
  public function getEntities($entity_type = NULL, $map = FALSE){
    $use = $this->entities;
    if($map){
      $use = $this->xml_id_maps;
    }
    if(!$entity_type){
      return $use;
    }else if(isset($use[$entity_type])){
      return $use[$entity_type];
    }else{
      return array();
    }
  }

  /**
   * getEntity
   *
   * Given an entity type and a XML element id, return the
   * corresponding entity wrapper (or FALSE if there isn't
   * one)
   */
  public function getEntity($type, $ref){
    if(isset($this->xml_id_maps[$type][$ref])){
      return $this->xml_id_maps[$type][$ref];
    }else{
      return FALSE;
    }
  }

  /**
   * addMapItem
   * 
   * Add an XML id => entity wrapper custom mapping.
   * 
   * This is normally taken care of through mapXML
   * and should not be needed. There are no cases 
   * where a track of the xml id to wrapper object 
   * would be needed such that these wrappers could
   * not have been created via the normal process.
   * 
   * However some work flows may be easier to write
   * by creating more than one entity in #create
   * (eg. when creating field collections which we
   * want to track when they have an id, but are
   * not required to have one) so users can
   * call this in such cases.
   */
  function addMapItem($id, $wrapper = NULL){
    if($wrapper === NULL && is_array($id)){
      foreach($id as $xml_id => $entity_wrapper){
        $this->addMapItem($xml_id, $entity_wrapper);
      }
      return;
    }
    $entity_type = $wrapper->type();
    if(!isset($this->xml_id_maps[$entity_type])){
      $this->xml_id_maps[$entity_type] = array();
    }
    $this->xml_id_maps[$entity_type][$id] = $wrapper;
  }

  /**
   * mapXML
   *
   * Given an xml node, and xml schema name and a reverse schema array
   * this method parses the XML into entities and stores them accordingly.
   */
  function mapXML($definition){
    foreach($definition as $entity_type => $def){
      $entity_type = preg_replace('/#.*$/', '', $entity_type);
      // Set up
      if(!isset($this->entities[$entity_type])){
        $this->entities[$entity_type] = array();
      }
      if(!isset($this->xml_id_maps[$entity_type])){
        $this->xml_id_maps[$entity_type] = array();
      }
      // Ensure the reverse schema array is valid
      if(empty($def['#xpath'])){throw new Exception("No XPATH defined for entity type $entity_type for schema $schema");}
      if(empty($def['#get']) && empty($def['#xget']) && empty($def['#create'])){throw new Exception("Need either #get, #xget or #create for entity type $entity_type for schema $schema");}
      if(isset($def['#get']) && isset($def['#xget'])){throw new Exception("Cannot define both #get and #xget at the same time for entity type $entity_type for schema $schema");}
      // Parse the XML nodes
      foreach($this->dom->xpath($def['#xpath']) as $mapped_node){
        list($entity_w, $save) = $this->createEntity($entity_type, $def, $mapped_node);
        if($entity_w === FALSE){
          continue;
        }
        // Save the entity and create the map
        if($save){
          $this->save_entities[] = $entity_w;
          if($this->options['autosave']){
            entity_save($entity_type, $entity);
          }
        }
        $this->entities[$entity_type][] = $entity_w;
        if(isset($def['#id'])){
          $id_values = $mapped_node->xpath($def['#id']);
          if(!empty($id_values)){
            $id = $this->nodeToString(reset($id_values));
            $this->xml_id_maps[$entity_type][$id] = $entity_w;
          }
        }
      }
    }
  }

  /**
   * createEntity
   *
   * Given an entity type, a reverseve schema array for that
   * entity type and an XML node get/create the matching entity
   *
   * Return an array defining the entity wrapper and a flag indicating
   * whether it needs to be saved.
   *
   * If not entity was fetched/created then the entity wrapper is set
   * to FALSE. This is not an indication of error, as it is possible
   * to skip entities when using '#get'. Errors will throw exceptions instead.
   */
  private function createEntity($entity_type, $def, $mapped_node){
    $save = FALSE;
    // Load an existing entity or create a new one
    $entity_w = FALSE;
    $set_fields = TRUE;
    $was_loaded = FALSE;
    if(isset($def['#get']) || isset($def['#xget'])){
      $fn = isset($def['#get']) ? $def['#get'] : $def['#xget'];
      $entity_w = $this->call($fn, $mapped_node);
      if(!$entity_w && !isset($def['#create'])){
        $this->info['skipped']++;
        return array(
          FALSE,
          FALSE
        );
      }else if($entity_w){
        $entity = $entity_w->raw();
        $set_fields = isset($def['#get']);
        $was_loaded = TRUE;
      }
    }
    if(!$entity_w && isset($def['#create'])){
      $process = false;
      if(isset($def['#create']['#process'])){
        $process = $def['#create']['#process'];
        unset($def['#create']['#process']);
      }
      $entity = entity_create($entity_type, $def['#create']);
      $entity_w = entity_metadata_wrapper($entity_type, $entity);
      if($process){
        $this->call($process, $entity_w);
      }
      $save = TRUE;
      $set_fields = TRUE;
    }
    // Apply fields
    $field_names = element_children($def);
    if($set_fields && !empty($field_names)){
      $save = TRUE;
      foreach($field_names as $field_name){
        $process = FALSE;
        $set = FALSE;
        $combine = FALSE;
        $default = FALSE;
        if(is_array($def[$field_name])){
          if(!isset($def[$field_name]['#xpath'])){throw new Exception('xpath required');}
          $values = $mapped_node->xpath($def[$field_name]['#xpath']);
          if(isset($def[$field_name]['#process'])){
            $process = $def[$field_name]['#process'];
          }
          if(isset($def[$field_name]['#set'])){
            $set = $def[$field_name]['#set'];
          }
          if(isset($def[$field_name]['#default'])){
            $default = $def[$field_name]['#default'];
          }
          if(isset($def[$field_name]['#combine'])){
            $combine = TRUE;
          }
          if($process && $set){throw new Exception("#process and #set are mutually exclusive");}
        }else{
          $values = $mapped_node->xpath($def[$field_name]);
        }
        $property = FALSE;
        if(strpos($field_name, ':') !== FALSE){
          list($field_name, $property) = explode(':', $field_name);
        }
        if($combine && $set){
          try{
            $this->call($set, $entity_w, $field_name, $property, $values);
          }
          catch(ReverseSchemaArrayException $e){
            if($e->action & REVERSE_SCHEMA_ARRAY_SKIP_ENTITY){
              $this->info['skipped']++;
              return array(
                FALSE,
                FALSE
              );
            }else if($e->action & REVERSE_SCHEMA_ARRAY_SKIP_VALUE){
              continue;
            }
            throw $e;
          }
        }else{
          $info = $entity_w->getPropertyInfo($field_name);
          if(strpos($info['type'], 'list<') === 0){
            $entity_w->{$field_name} = array();
          }else if(count($values) > 1){
            $values = array(
              reset($values)
            ); // Ensure we only have one value
          }
          if(empty($values) && $default !== FALSE){
            $values = array(
              $default
            );
          }
          foreach($values as $value){
            try{
              if($set){
                $this->call($set, $entity_w, $field_name, $property, $value);
              }else{
                $this->setFieldValue($entity_w, $field_name, $property, $value, $process);
              }
            }
            catch(ReverseSchemaArrayException $e){
              if($e->action & REVERSE_SCHEMA_ARRAY_SKIP_ENTITY){
                $this->info['skipped']++;
                return array(
                  FALSE,
                  FALSE
                );
              }else if($e->action & REVERSE_SCHEMA_ARRAY_SKIP_VALUE){
                continue;
              }
              throw $e;
            }
          }
        }
      }
    }
    $this->info['total']++;
    if($was_loaded){
      $this->info['loaded']++;
      if($save){
        $this->info['saved']++;
        $this->info['updated']++;
      }
    }else{
      $this->info['new']++;
      $this->info['saved']++;
    }
    return array(
      $entity_w,
      $save
    );
  }

  /**
   * nodeToString
   *
   * Simple helper function to convert an XML node, or an array
   * of XML nodes, to their string representation.
   */
  public function nodetoString($value, $sep = ' '){
    if(!is_array($value)){
      $value = array(
        $value
      );
    }
    return implode($sep, array_map('strval', $value));
  }

  /**
   * call
   *
   * Calls the function in the given definition. This may either
   * be a string (a function name), or an array whereby the first
   * argument is a callable (any PHP callable) and following
   * values are addition arguments.
   *
   * The callable is called with $this as the first argument,
   * followed by any extra argument that was passed to this function
   * (variable number), followed by the extra arguments defined
   * in the callable definition.
   *
   * Returns the value of the callable.
   *
   * Throws an excpetion if the callable does not exist.
   */
  private function call($def){
    // Get arguments
    $arguments = func_get_args();
    array_shift($arguments);
    array_unshift($arguments, $this);
    // Parse callable
    if(is_array($def)){
      $extra_arguments = $def;
      $callable = array_shift($extra_arguments);
      $arguments = array_merge($arguments, $extra_arguments);
    }else{
      $callable = $def;
    }
    if(!is_callable($callable)){throw new Exception("$callable is not callable");}
    return call_user_func_array($callable, $arguments);
  }

  /**
   * setFieldValue
   *
   * Sets the value of the given field[->property] on the
   * given entity to that of the given XML node.
   *
   * Optionally apply a process function first.
   */
  private function setFieldValue($entity_w, $field_name, $property, $value, $process = FALSE){
    // Process value
    if($process){
      $value = $this->call($process, $value);
    }else{
      $value = $this->nodeToString($value);
    }
    // Check field type and set value
    $info = $entity_w->getPropertyInfo($field_name);
    if(strpos($info['type'], 'list<') === 0){
      if($property){
        $entity_w->{$field_name}[]->{$property} = $value;
      }else{
        $entity_w->{$field_name}[] = $value;
      }
    }else{
      if($property){
        $entity_w->{$field_name}->{$property} = $value;
      }else{
        $entity_w->{$field_name} = $value;
      }
    }
  }
}

/**
 * class ReverseSchemaArrayException
 * 
 * Custom exeption class, which defines an action that may be
 * a combination of:
 * 
 * REVERSE_SCHEMA_ARRAY_SKIP_ENTITY: the current entity being proccessed should
 * be skipped ;
 * REVERSE_SCHEMA_ARRAY_SKIP_VALUE: the current value/field being processed should
 * be skipped ;
 * 
 * Not defining an action will throw the exception out of the import process.
 */
class ReverseSchemaArrayException extends Exception{

  public $action;

  /**
   * __construct
   */
  public function __construct($action, $message = ''){
    parent::__construct($message);
    $this->action = $action;
  }
}

/**
 * reverse_schema_array_process_map
 * 
 * Generic function to apply the XML id map to a value.
 * 
 * Given an entity_type and a value that represents an XML id
 * in the current map, return the matching entity.
 * 
 * If there is no match an empty value is returned unless otherwise
 * specified in $options.
 * 
 * $options may define:
 * - field: Return the given field of the entity rather than the entity itself ;
 * - raw: If TRUE, return the raw entity rather than the wrapper ;
 * - array: If TRUE, return the raw entity as an array rather than the wrapper ;
 * - error: Error message to add to the mapper in case of error. This should
 *           be untranslated, any may use '%ref' as a placeholder ;
 * - throw: If defined and TRUE, a ReverseSchemaArrayException should be
 *           thrown on error. This will use the action parameter and the
 *           error message.
 * - action: The action parameter of the exception.
 */
function reverse_schema_array_process_map($xml_map, $value, $entity_type, $options = array()){
  $value = $xml_map->nodeToString($value);
  $entity_w = $xml_map->getEntity($entity_type, $value);
  if($entity_w){
    if(!empty($options['array'])){
      return (array)($entity_w->raw());
    }else if(!empty($options['raw'])){
      return $entity_w->raw();
    }else if(!empty($options['field'])){return $entity_w->{$options['field']}->value();}
    return $entity_w;
  }else{
    $msg = '';
    if(!empty($options['error'])){
      $msg = t($options['error'], array(
        '%ref' => $value
      ));
      $xml_map->addMessage($msg);
    }
    if(!empty($options['throw'])){
      $action = isset($options['action']) ? $options['action'] : 0;
      throw new ReverseSchemaArrayException($action, $msg);
    }else{
      return '';
    }
  }
}

/**
 * reverse_schema_array_process_boolean
 */
function reverse_schema_array_process_boolean($xml_map, $value){
  $accept = array(
    'true' => 1,
    '1' => 1,
    'false' => 0,
    '0' => 0
  );
  $v = strtolower(trim($xml_map->nodeToString($value)));
  if(isset($accept[$v])){
    return $accept[$v];
  }else{
    return 0;
  }
}

/**
 * reverse_schema_array_process_attach_field_collection
 *
 * A #create process function used to attach the created
 * field collection item to a host entity
 */
function reverse_schema_array_process_attach_field_collection($xml_map, $entity_w, $host_w){
  $entity_w->raw()->setHostEntity($host_w->type(), $host_w->raw());
}