<?php

/**
 * @file
 * Deprecated. This is only here to ease upgrading from previous versions of the
 * entity API, which shipped with this file. This is required, such that the
 * Entity class can be loaded during the drupal bootstrap when updating. After
 * updating, this file and its directory can be safely removed.
 */

/**
 * A common class for entities.
 *
 * It's suggested, but not required, to extend this class and to override
 * __construct() in order to specify a fixed entity type.
 *
 * It is suggested to also implement the label() and uri() methods for the
 * respective callbacks in hook_entity_info(). For that case the provided
 * callbacks entity_class_label() and entity_class_uri() may be specified.
 */
class Entity {

  protected $entityType;
  protected $entityInfo;
  protected $idKey, $nameKey;

  /**
   * Creates a new entity.
   *
   * @see entity_create()
   */
  public function __construct(array $values = array(), $entityType = NULL) {
    if (empty($entityType)) {
      throw new Exception('Cannot created an instance of Entity without a specified entity type.');
    }
    $this->entityType = $entityType;
    $this->entityInfo = entity_get_info($entityType);
    $this->idKey = $this->entityInfo['entity keys']['id'];
    $this->nameKey = isset($this->entityInfo['entity keys']['name']) ? $this->entityInfo['entity keys']['name'] : $this->idKey;

    // Set initial values.
    foreach ($values as $key => $value) {
      $this->$key = $value;
    }
  }

  /**
   * Returns the internal, numeric identifier.
   *
   * For exportable entities, this differs to the uniform identifier returned
   * by Entity::identifier(). The internal identifier is supposed to be used
   * internally in order to refer to a specify, stored (ENTITY_IN_DB)
   * entity. For referring to entities which might not live in the DB, use the
   * uniform identifier.
   * If unsure, use Entity:identifier().
   */
  public function internalIdentifier() {
    return isset($this->{$this->idKey}) ? $this->{$this->idKey} : NULL;
  }

  /**
   * Returns the unified identifer.
   *
   * @return
   *   The identifier of the entity. For exportable entities, this is their
   *   machine readable name.
   *
   * @see entity_id()
   */
  public function identifier() {
    return isset($this->{$this->nameKey}) ? $this->{$this->nameKey} : NULL;
  }

  /**
   * Returns the info of the type of the entity.
   *
   * @see entity_get_info()
   */
  public function entityInfo() {
    return $this->entityInfo;
  }

  /**
   * Returns the type of the entity.
   */
  public function entityType() {
    return $this->entityType;
  }

  /**
   * Returns the label of the entity.
   *
   * Modules may alter the label by specifying another 'label callback' using
   * hook_entity_info_alter().
   *
   * @see entity_label()
   */
  public function label() {
    if (isset($this->entityInfo['label callback']) && $this->entityInfo['label callback'] == 'entity_class_label') {
      return $this->defaultLabel();
    }
    return entity_label($entity_type, $entity);
  }

  /**
   * Override this in order to implement a custom default label and specify
   * 'entity_class_label' as 'label callback' hook_entity_info(). As an
   * alternative to that the label key may be provided in hook_entity_info().
   */
  protected function defaultLabel() {
    return 'unlabelled';
  }

  /**
   * Returns the uri of the entity just as entity_uri().
   *
   * Modules may alter the uri by specifying another 'uri callback' using
   * hook_entity_info_alter().
   *
   * @see entity_uri()
   */
  public function uri() {
    if (isset($this->entityInfo['uri callback']) && $this->entityInfo['uri callback'] == 'entity_class_uri') {
      return $this->defaultUri();
    }
    return entity_uri($entity_type, $entity);
  }

  /**
   * Override this in order to implement a custom default URI and specify
   * 'entity_class_uri' as 'uri callback' hook_entity_info().
   */
  protected function defaultUri() {
    return array('path' => 'default/' . $entity->identifier());
  }

  /**
   * Permanently saves the entity.
   *
   * @see entity_save()
   */
  public function save() {
    return entity_get_controller($this->entityType)->save($this);
  }

  /**
   * Permanently deletes the entity.
   *
   * @see entity_delete()
   */
  public function delete() {
    $id = $this->identifier();
    if (isset($id)) {
      entity_get_controller($this->entityType)->delete(array($id));
    }
  }

  /**
   * Exports the entity.
   *
   * @see entity_export()
   */
  public function export($prefix = '') {
    return entity_get_controller($this->entityType)->export($this, $prefix);
  }

  /**
   * Generate an array for rendering the entity.
   *
   * @see entity_view()
   */
  public function view($view_mode = 'full', $langcode = NULL) {
    return entity_get_controller($this->entityType)->view(array($this->identifier() => $this), $view_mode, $langcode);
  }

  /**
   * Builds a structured array representing the entity's content.
   *
   * @see entity_build_content()
   */
  public function buildContent($view_mode = 'full', $langcode = NULL) {
    return entity_get_controller($this->entityType)->buildContent($this, $view_mode, $langcode);
  }
}

/**
 * These classes are deprecated by "Entity" and are only here for backward
 * compatibility reasons.
 */
class EntityDB extends Entity {}
class EntityExtendable extends Entity {}
class EntityDBExtendable extends Entity {}
