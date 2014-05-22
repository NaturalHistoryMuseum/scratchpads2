<?php

/**
 * This class is used to build XML from an array schema
 *
 */
class SchemaXMLBuilder{

  /**
   * Creatre a new builder from a schema and settings and
   * a number of modifiers (objects that implement SchemaXMLModifierInterface)
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
  function __construct($name, $array_schema, $settings, $modifiers = array()){
    $this->name = $name;
    $this->array_schema = $array_schema;
    $this->settings = $settings;
    $this->modifiers = $modifiers;
  }

  /**
   * This function sets the active build context
   *
   * XXX We could probably simplify the context management as such:
   * - all items not specified get passed down to children contexts ;
   * - all items specified as 'child_xxx' get additionnaly passed down as 'xxx'
   *
   */
  function _set_context($context){
    $default = array(
      'entity' => NULL,
      'entity_type' => NULL,
      'wrapper' => NULL,
      'child_entity' => NULL,
      'child_entity_type' => NULL,
      'child_wrapper' => NULL,
      'field' => NULL,
      'delta' => NULL,
      'raw_value' => NULL,
      'value_to_insert' => NULL,
      'schema' => NULL,
      'relation' => NULL,
      'relation_wrapper' => NULL,
      'child_relation' => NULL,
      'child_relation_wrapper' => NULL
    );
    $this->_build_context[] = (object)array_merge($default, $context);
  }

  /**
   * Modify the current context
   */
  function _modify_context($mod){
    $idx = count($this->_build_context) - 1;
    if($idx >= 0){
      $this->_build_context[$idx] = (object)array_merge((array)$this->_build_context[$idx], $mod);
    }
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
   * This function builds the XML from the given entity
   *
   * This function will throw an exception on errors
   * (unless 'no-error' was set), and return
   * the DOM document.
   *
   */
  function build_xml($entity_type, $entity, $xsd_url = NULL, $version = '1.0', $encoding = 'UTF-8'){
    $this->_dom = new DOMDocument($version, $encoding);
    foreach($this->modifiers as $modifier){
      $modifier->start_building($this->array_schema, $entity_type, $entity, $this->_dom);
    }
    $this->path = array();
    $this->_build_context = array();
    $wrapper = entity_metadata_wrapper($entity_type, $entity);
    $this->_set_context(array(
      'entity' => $entity,
      'entity_type' => $entity_type,
      'wrapper' => $wrapper,
      'schema' => $this->array_schema
    ));
    $this->_build_xml_iteration($this->array_schema, $wrapper, $this->_dom);
    $this->_pop_context();
    $xml = $this->_dom->saveXML();
    if($xsd_url && empty($this->settings['no-error'])){
      $this->_validate_xml($xml, $xsd_url, $version, $encoding);
    }
    return $xml;
  }

  /**
   * From a schema array, entity wrapper and xml element, populates the XML
   * with the computed values.
   *
   * This function returns TRUE if some nodes were inserted because
   * they had an actual value (defined by #field or #value). This is
   * used for backtracking and removing un-needed parent elements.
   *
   */
  function _build_xml_iteration($schema, $root_wrapper, $xml_element){
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
      // Prepare wrapper and context
      $wrapper = $root_wrapper;
      $this->path[] = $tag;
      $parent_context = $this->_get_context();
      $this->_set_context(array(
        'entity' => $wrapper->raw(),
        'entity_type' => $wrapper->type(),
        'wrapper' => $wrapper,
        'relation' => $parent_context->child_relation,
        'relation_wrapper' => $parent_context->child_relation_wrapper
      ));
      // Test for coherence
      if(isset($child_schema['#child_relation']) && (isset($child_schema['#field']) || isset($child_schema['#child_entitiy']))){throw new Exception("Schema cannot include #child_relation and #field/#child_entity");}
      // Look for a '#entity' directive
      if(isset($child_schema['#entity'])){
        try{
          $wrapper = $this->_load_entity($child_schema['#entity']);
        }
        catch(Exception $e){
          $this->_generate_error($e->getMessage());
          $wrapper = NULL;
        }
        if(!$wrapper){
          $this->_pop_context();
          array_pop($this->path);
          continue;
        }
        // Replace the context with the loaded one
        $this->_pop_context();
        $this->_set_context(array(
          'entity' => $wrapper->raw(),
          'entity_type' => $wrapper->type(),
          'wrapper' => $wrapper
        ));
        // XXX should we keep the relation if the entity is being changed ?
      }
      // Check if there is a conditional
      if(isset($child_schema['#condition'])){
        if(is_array($child_schema['#condition']) || is_callable($child_schema['#condition'])){
          if(is_array($child_schema['#condition'])){
            $arguments = $child_schema['#condition'];
            $cond_func = array_shift($arguments);
          }else{
            $cond_func = $child_schema['#condition'];
            $arguments = array();
          }
          $arguments = array_merge(array(
            $this->_get_context()
          ), $arguments);
          if(!call_user_func_array($cond_func, $arguments)){
            $this->_pop_context();
            array_pop($this->path);
            continue;
          }
        }else{
          if(isset($child_schema['#child_relation'])){throw new Exception("Schema cannot define both #child_relation and a #condition that relies on a field");}
          $cond_field = $child_schema['#condition'];
          $cond_value = $this->_read_values($wrapper, $cond_field);
          if(empty($cond_value)){
            $this->_pop_context();
            array_pop($this->path);
            continue;
          }
        }
      }
      // If this is a field, obtain all the values' contexts
      if(isset($child_schema['#child_relation'])){
        $child_schema['#values_contexts'] = $this->_get_relation_values_contexts($child_schema, $tag, $wrapper);
      }else if(isset($child_schema['#field'])){
        $child_schema['#values_contexts'] = $this->_get_field_values_contexts($child_schema, $tag, $wrapper);
      }
      // Check the occurence constraint works
      $this->_build_xml_check_constraint($child_schema, $tag, $wrapper);
      // Insert the value and recurse
      if(isset($child_schema['#field'])){
        $insert_child = $this->_build_schema_insert_field($child_schema, $tag, $wrapper, $xml_element);
        $inserted = $inserted || $insert_child;
      }else if(isset($child_schema['#value'])){
        $insert_child = $this->_build_schema_insert_value($child_schema, $tag, $wrapper, $xml_element);
        $inserted = $inserted || $insert_child;
      }else{
        $insert_child = $this->_build_schema_insert_blank($child_schema, $tag, $wrapper, $xml_element);
        $inserted = $inserted || $insert_child;
      }
      array_pop($this->path);
      $this->_pop_context();
    }
    return $inserted;
  }

  /**
   * Return all the child entities of a given relation,
   * as contexts
   */
  function _get_relation_values_contexts($schema, $tag, $wrapper){
    // Entity metadata wrappers don't give us access to the relation itself,
    // so we need to load it directly.
    $current_context = $this->_get_context();
    $contexts = array();
    $query = relation_query($wrapper->type(), $wrapper->getIdentifier());
    $query->entityCondition('bundle', $schema['#child_relation']);
    $results = $query->execute();
    foreach($results as $relation_data){
      $relation = relation_load($relation_data->rid);
      $relation_wrapper = entity_metadata_wrapper('relation', $relation);
      $related_entities = field_get_items('relation', $relation, 'endpoints');
      foreach($related_entities as $entity_data){
        if($entity_data['entity_type'] == $wrapper->type() && $entity_data['entity_id'] == $wrapper->getIdentifier()){
          continue;
        }
        $related_entity = entity_load_single($entity_data['entity_type'], $entity_data['entity_id']);
        $related_wrapper = entity_metadata_wrapper($entity_data['entity_type'], $related_entity);
        $contexts[] = array(
          'entity' => $wrapper->raw(),
          'entity_type' => $wrapper->type(),
          'wrapper' => $wrapper,
          'child_entity_type' => $entity_data['entity_type'],
          'child_entity' => $related_entity,
          'child_wrapper' => $related_wrapper,
          'child_relation' => $relation,
          'child_relation_wrapper' => $relation_wrapper,
          'relation' => $current_context->relation,
          'relation_wrapper' => $current_context->relation_wrapper
        );
      }
    }
    return $contexts;
  }

  /**
   * Return all the actual values of a given #field value
   * (applying merges and child entity loads), and return
   * an array of contexts for each actual value.
   */
  function _get_field_values_contexts($schema, $tag, $wrapper){
    $current_context = $this->_get_context();
    $result = array();
    $field = $schema['#field'];
    $values = $this->_read_values($wrapper, $field);
    if(isset($schema['#merge']) && function_exists($schema['#merge'])){
      $merge_function = $schema['#merge'];
      $values = array(
        $merge_function($values)
      );
    }
    foreach($this->modifiers as $modifier){
      $values = $modifier->insert_value_array($schema, $tag, $values);
    }
    foreach($values as $delta => $value){
      $value_to_insert = $value;
      if(isset($schema['#value'])){
        $value_to_insert = $schema['#value'];
      }
      if(isset($schema['#child_entity'])){
        try{
          $child_wrapper = $this->_load_entity($schema['#child_entity'], $value);
        }
        catch(Exception $e){
          $this->_generate_error($e->getMessage());
          $child_wrapper = NULL;
        }
        if(!$child_wrapper){
          // It's up to the loader to raise errors if need be. Here an empty return means skip it.
          continue;
        }
      }else{
        $child_wrapper = $wrapper;
      }
      $result[] = array(
        'entity' => $wrapper->raw(),
        'entity_type' => $wrapper->type(),
        'wrapper' => $wrapper,
        'child_entity' => $child_wrapper->raw(),
        'child_entity_type' => $child_wrapper->type(),
        'child_wrapper' => $child_wrapper,
        'field' => $field,
        'delta' => $delta,
        'raw_value' => $value,
        'value_to_insert' => $value_to_insert,
        'schema' => $schema,
        'relation' => $current_context->relation,
        'relation_wrapper' => $current_context->relation_wrapper,
        'child_relation' => isset($schema['#child_entity']) ? NULL : $current_context->relation,
        'child_relation' => isset($schema['#child_entity']) ? NULL : $current_context->relation_wrapper
      );
    }
    return $result;
  }

  /**
   * Insert a #field value, and recurse through the schema as appropriate
   *
   */
  function _build_schema_insert_field($schema, $tag, $wrapper, $xml_element){
    if(empty($schema['#values_contexts'])){return FALSE;}
    foreach($schema['#values_contexts'] as $context){
      $this->_set_context($context);
      // XXX The new Modifier functionality means we might end up with a NULL value, and thus
      // should set $inserted to FALSE (depending on min_ocurrence) to provide adequate backtracking.
      $child_element = $this->_insert_xml_element($schema, $tag, $context['value_to_insert'], $xml_element);
      $inserted = TRUE;
      $this->_build_xml_iteration($schema, $context['child_wrapper'], $child_element);
      $this->_pop_context();
    }
    return TRUE;
  }

  /**
   * Insert a #value, and recurse through the schema as appropriate
   *
   */
  function _build_schema_insert_value($schema, $tag, $wrapper, $xml_element){
    $current_context = $this->_get_context();
    $inserted = FALSE;
    if(isset($schema['#values_contexts'])){
      $count = count($schema['#values_contexts']);
    }else{
      $count = (!isset($schema['#min_occurence']) || $schema['#min_occurence'] == 0) ? 1 : $schema['#min_occurence'];
    }
    $value = $schema['#value'];
    for($i = 0; $i < $count; $i++){
      if(isset($schema['#values_contexts'])){
        $schema['#values_contexts'][$i] += array(
          'delta' => $i,
          'raw_value' => $value,
          'value_to_insert' => $value
        );
        $this->_set_context($schema['#values_contexts'][$i]);
        $child_wrapper = $schema['#values_contexts'][$i]['child_wrapper'];
      }else{
        $this->_set_context(array(
          'entity' => $wrapper->raw(),
          'entity_type' => $wrapper->type(),
          'wrapper' => $wrapper,
          'delta' => $i,
          'raw_value' => $value,
          'value_to_insert' => $value,
          'schema' => $schema,
          'relation' => $current_context->relation,
          'relation_wrapper' => $current_context->relation_wrapper,
          'child_relation' => isset($schema['#child_entity']) ? NULL : $current_context->relation,
          'child_relation_wrapper' => isset($schema['#child_entity']) ? NULL : $current_context->relation_wrapper
        ));
        $child_wrapper = $wrapper;
      }
      // XXX The new Modifier functionality means we might end up with a NULL value, and thus
      // should set $inserted to FALSE (depending on min_ocurrence) to provide adequate backtracking.
      $child_element = $this->_insert_xml_element($schema, $tag, $value, $xml_element);
      $inserted = TRUE;
      $this->_build_xml_iteration($schema, $child_wrapper, $child_element);
      $this->_pop_context();
    }
    return $inserted;
  }

  /**
   * Insert a blank value if the element has #min_occurence > 0 or if one of the
   * children inserts a real (#value of #field) value.
   *
   */
  function _build_schema_insert_blank($schema, $tag, $wrapper, $xml_element){
    $current_context = $this->_get_context();
    $min_occurence = isset($schema['#min_occurence']) ? $schema['#min_occurence'] : 1;
    if(isset($schema['#values_contexts'])){
      $count = count($schema['#values_contexts']);
    }else{
      $count = ((!isset($schema['#min_occurence']) || $min_occurence == 0) ? 1 : $schema['#min_occurence']);
    }
    $inserted = FALSE;
    for($i = 0; $i < $count; $i++){
      if(isset($schema['#values_contexts'])){
        $schema['#values_contexts'][$i]['delta'] = $i;
        $this->_set_context($schema['#values_contexts'][$i]);
        $child_wrapper = $schema['#values_contexts'][$i]['child_wrapper'];
      }else{
        $this->_set_context(array(
          'entity' => $wrapper->raw(),
          'entity_type' => $wrapper->type(),
          'wrapper' => $wrapper,
          'delta' => $i,
          'schema' => $schema,
          'relation' => $current_context->relation,
          'relation_wrapper' => $current_context->relation_wrapper,
          'child_relation' => isset($schema['#child_entity']) ? NULL : $current_context->relation,
          'child_relation_wrapper' => isset($schema['#child_entity']) ? NULL : $current_context->relation_wrapper
        ));
        $child_wrapper = $wrapper;
      }
      // XXX The new Modifier functionality means we might end up with a non NULL , and thus
      // should set $inserted to TRUE to provide adequate backtracking.
      $child_element = $this->_insert_xml_element($schema, $tag, NULL, $xml_element);
      $insert_child = $this->_build_xml_iteration($schema, $child_wrapper, $child_element);
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
   * Load and return the entity to be used by a node's children.
   *
   */
  function _load_entity($loader, $value = NULL){
    if(is_array($loader)){
      $values = $loader;
      $loader = array_shift($values);
      $arguments = array(
        $this->_get_context()
      );
      if($value !== NULL){
        $arguments[] = $value;
      }
      $arguments = array_merge($arguments, $values);
    }else{
      $arguments = array(
        $this->_get_context()
      );
      if($value !== NULL){
        $arguments[] = $value;
      }
    }
    $info = call_user_func_array($loader, $arguments);
    if(is_array($info) && !empty($info['entity_type']) && !empty($info['entity'])){return entity_metadata_wrapper($info['entity_type'], $info['entity']);}
    return NULL;
  }

  /**
   * Insert an element given it's array schema into the DOM.
   *
   */
  function _insert_xml_element($schema, $tag, $value, $xml_element){
    // Apply process function
    if(isset($schema['#process'])){
      if(is_array($schema['#process'])){
        $arguments = $schema['#process'];
        $f = array_shift($arguments);
      }else{
        $f = $schema['#process'];
        $arguments = array();
      }
      if(!is_callable($f)){throw new Exception("Process function $f for tag $tag does not exist");}
      $arguments = array_merge(array(
        $this->_get_context()
      ), $arguments);
      $value = call_user_func_array($f, $arguments);
    }
    if(is_array($value)){
      if(isset($value['value'])){
        $value = $value['value'];
      }else{
        $value = reset($value);
      }
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
      $final_value = _schemaxml_xml_translate_entities($final_value);
      if($final_value !== FALSE && $final_value !== NULL && trim($final_value) !== ''){
        $fragment->appendXML($final_value);
        $child_element->appendChild($fragment);
      }
    }
    return $child_element;
  }

  /**
   * This method checks that the constraint of a given schema array are
   * fullfilled by the given entity.
   *
   * Depending on the settings, this might throw an exception or modify the
   * schema array's value to ensure that the constraint are respected.
   */
  function _build_xml_check_constraint(&$schema, $tag, $wrapper){
    $min_occurence = isset($schema['#min_occurence']) ? $schema['#min_occurence'] : 1;
    $max_occurence = isset($schema['#max_occurence']) ? $schema['#max_occurence'] : 1;
    if(!empty($schema['#values_contexts'])){
      $count = count($schema['#values_contexts']);
      if($count < $min_occurence || ($max_occurence >= 0 && $count > $max_occurence)){
        if(isset($schema['#child_relation'])){
          $message_prepare = "the relation %field";
        }else{
          $message_prepare = "the field %field";
        }
        if($count == 0){
          $message_prepare .= " is required.";
        }else if($count < $min_occurence){
          $message_prepare .= " must be present at least %min times, but is only here %count times.";
        }else{
          $message_prepare .= " can be present at most %max times, but is here %count times.";
        }
        if(isset($schema['#child_relation'])){
          $error_field = $schema['#child_relation'];
        }else{
          $error_field = $schema['#field'];
        }
        if(isset($schema['#error_info'])){
          $error_field = $schema['#error_info'];
          if(isset($schema['#error_field'])){
            $error_context_field = $schema['#error_field'];
            $error_value = $wrapper->get($error_context_field)->value();
            if(!empty($error_value)){
              $error_context = reset($this->_read_values($wrapper->get($error_context_field)->value()));
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
          unset($schema['#child_relation']);
        }
      }
    }
  }

  /**
   * This function registers an error and either interupts processing by
   * throwing an exception, or logs the error and returns
   */
  function _generate_error($message){
    $message = t('The XML for %name could not be generated, because !message. (XML Path where error occured: %path)', array(
      '%name' => $this->name,
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
   * _read_values
   *
   * Return the value of the given field on the given entity
   * such that the value is always wrapped in an array, even
   * if it's a single scalar value.
   *
   * If the current context defines a relation, and if the queried field
   * exists on the relation then that is returned instead of the value
   * on the current entity.
   *
   */
  function _read_values($wrapper, $field){
    $context = $this->_get_context();
    if(property_exists($context, 'relation') && is_object($context->relation) && property_exists($context->relation, $field)){
      $wrapper = $context->relation_wrapper;
    }
    try{
      if(!preg_match('/^(list<)?countries>?$/', $wrapper->get($field)->type())){
        if(strpos($wrapper->get($field)->type(), 'list<') === 0){
          return array_filter($wrapper->get($field)->value());
        }else{
          return array_filter(array(
            $wrapper->get($field)->value()
          ));
        }
      }
    }
    catch(Exception $e){
      if(!isset($wrapper->raw()->{$field})){throw new Exception(t('Field %field not found at path %path', array(
          '%field' => $field,
          '%path' => implode(' > ', $this->path)
        )));}
    }
    // Not all fields are exposed to wrappers - so if we can't get it
    // through the wrapper, try directly. Also some wrapper implementations
    // have bugs (eg. countries) so we avoid them.
    $value = field_get_items($wrapper->type(), $wrapper->raw(), $field);
    if(!is_array($value)){
      return array_filter(array(
        $value
      ));
    }else{
      $ak = array_keys($value);
      if(!empty($ak) && !is_int(reset($ak))){return array_filter(array(
          $value
        ));}
      return array_filter($value);
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



