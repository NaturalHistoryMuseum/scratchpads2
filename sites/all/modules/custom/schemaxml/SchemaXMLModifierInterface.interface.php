<?php


/**
 * This interface defines a schema xml modifier
 *
 * Such objects are given to the SchemaXMLBuilder,
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
interface SchemaXMLModifierInterface{

  /**
   * function start_building
   *
   * This is called before the XML building starts.
   *
   * $schema: The array schema used for building the XML
   *
   * $entity_type: The type of the entity from which the XML
   *                will be build
   *                
   * $entity: The entity from which the XML will be build
   *
   * $dom: The empty DomDocument object
   */
  public function start_building($schema, $entity_type, $entity, $dom);

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