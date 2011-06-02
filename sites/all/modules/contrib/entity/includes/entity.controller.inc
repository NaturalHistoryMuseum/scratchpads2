<?php

/**
 * @file
 * Provides a controller building upon the core controller but providing more
 * features like full CRUD functionality.
 */

/**
 * Interface for EntityControllers compatible with the entity API.
 */
interface EntityAPIControllerInterface extends DrupalEntityControllerInterface {

  /**
   * Delete permanently saved entities.
   *
   * In case of failures, an exception is thrown.
   *
   * @param $ids
   *   An array of entity IDs.
   */
  public function delete($ids);

  /**
   * Invokes a hook on behalf the entity. For hooks that have a respective
   * field API attacher like insert/update/.. the attacher is called too.
   */
  public function invoke($hook, $entity);

  /**
   * Permanently saves the given entity.
   *
   * In case of failures, an exception is thrown.
   *
   * @param $entity
   *   The entity to save.
   *
   * @return
   *   SAVED_NEW or SAVED_UPDATED is returned depending on the operation
   *   performed.
   */
  public function save($entity);

  /**
   * Create a new entity.
   *
   * @param array $values
   *   An array of values to set, keyed by property name.
   * @return
   *   A new instance of the entity type.
   */
  public function create(array $values = array());

  /**
   * Exports an entity as serialized string.
   *
   * @param $entity
   *   The entity to export.
   * @param $prefix
   *   An optional prefix for each line.
   *
   * @return
   *   The exported entity as serialized string. The format is determined by
   *   the controller and has to be compatible with the format that is accepted
   *   by the import() method.
   */
  public function export($entity, $prefix = '');

  /**
   * Imports an entity from a string.
   *
   * @param string $export
   *   An exported entity as serialized string.
   *
   * @return
   *   An entity object not yet saved.
   */
  public function import($export);

  /**
   * Builds a structured array representing the entity's content.
   *
   * The content built for the entity will vary depending on the $view_mode
   * parameter.
   *
   * @param $entity
   *   An entity object.
   * @param $view_mode
   *   View mode, e.g. 'full', 'teaser'...
   * @param $langcode
   *   (optional) A language code to use for rendering. Defaults to the global
   *   content language of the current request.
   * @return
   *   The renderable array.
   */
  public function buildContent($entity, $view_mode = 'full', $langcode = NULL);

  /**
   * Generate an array for rendering the given entities.
   *
   * @param $entities
   *   An array of entities to render.
   * @param $view_mode
   *   View mode, e.g. 'full', 'teaser'...
   * @param $langcode
   *   (optional) A language code to use for rendering. Defaults to the global
   *   content language of the current request.
   * @return
   *   The renderable array.
   */
  public function view($entities, $view_mode = 'full', $langcode = NULL);
}

/**
 * A controller implementing EntityAPIControllerInterface for the database and
 * being able to deal with exportable entities.
 */
class EntityAPIController extends DrupalDefaultEntityController implements EntityAPIControllerInterface {

  protected $cacheComplete = FALSE;
  protected $nameKey, $statusKey, $moduleKey, $bundleKey;

  /**
   * Overridden.
   * @see DrupalDefaultEntityController#__construct()
   *
   * Allows specifying a name key serving as uniform identifier for this entity
   * type while still internally we are using numeric identifieres.
   */
  public function __construct($entityType) {
    parent::__construct($entityType);
    // Use the name key as primary identifier.
    $this->nameKey = isset($this->entityInfo['entity keys']['name']) ? $this->entityInfo['entity keys']['name'] : $this->idKey;
    if (!empty($this->entityInfo['exportable'])) {
      $this->statusKey = isset($this->entityInfo['entity keys']['status']) ? $this->entityInfo['entity keys']['status'] : 'status';
      $this->moduleKey = isset($this->entityInfo['entity keys']['module']) ? $this->entityInfo['entity keys']['module'] : 'module';
    }
    // If this is the bundle of another entity, set the bundle key.
    if (isset($this->entityInfo['bundle of'])) {
      $info = entity_get_info($this->entityInfo['bundle of']);
      $this->bundleKey = $info['bundle keys']['bundle'];
    }
  }

  /**
   * Builds and executes the query for loading.
   *
   * @return The results in a Traversable object.
   */
  public function query($ids, $conditions, $revision_id = FALSE) {
    // Build the query.
    $query = $this->buildQuery($ids, $conditions, $revision_id);
    $result = $query->execute();
    if (!empty($this->entityInfo['entity class'])) {
      $result->setFetchMode(PDO::FETCH_CLASS, $this->entityInfo['entity class'], array(array(), $this->entityType));
    }
    return $result;
  }

  protected function buildQuery($ids, $conditions = array(), $revision_id = FALSE) {
    // Add the id condition ourself, as we might have a separate name key.
    $query = parent::buildQuery(array(), $conditions, $revision_id);
    if ($ids) {
      // Support loading by numeric ids as well as by machine names.
      $key = is_numeric(reset($ids)) ? $this->idKey : $this->nameKey;
      $query->condition("base.$key", $ids, 'IN');
    }
    return $query;
  }

  /**
   * Overridden.
   * @see DrupalDefaultEntityController#load($ids, $conditions)
   *
   * In contrast to the parent implementation we factor out query execution, so
   * fetching can be further customized easily. Also we add any in code defined
   * objects.
   */
  public function load($ids = array(), $conditions = array()) {
    $entities = array();

    // For exportable entities, make sure the defaults have been built.
    if (!empty($this->entityInfo['exportable']) && empty($GLOBALS['conf']['entity_defaults_built'][$this->entityType])) {
      _entity_defaults_rebuild($this->entityType);
    }

    // Revisions are not statically cached, and require a different query to
    // other conditions, so separate the revision id into its own variable.
    if ($this->revisionKey && isset($conditions[$this->revisionKey])) {
      $revision_id = $conditions[$this->revisionKey];
      unset($conditions[$this->revisionKey]);
    }
    else {
      $revision_id = FALSE;
    }

    // Create a new variable which is either a prepared version of the $ids
    // array for later comparison with the entity cache, or FALSE if no $ids
    // were passed. The $ids array is reduced as items are loaded from cache,
    // and we need to know if it's empty for this reason to avoid querying the
    // database when all requested entities are loaded from cache.
    $passed_ids = !empty($ids) ? array_flip($ids) : FALSE;

    // Try to load entities from the static cache.
    if (!$revision_id) {
      $entities = $this->cacheGet($ids, $conditions);
      // If any entities were loaded, remove them from the ids still to load.
      if ($passed_ids) {
        $ids = array_keys(array_diff_key($passed_ids, $entities));
      }
    }

    // Load any remaining entities from the database. This is the case if $ids
    // is set to FALSE (so we load all entities), if there are any ids left to
    // load or if loading a revision.
    if (!($this->cacheComplete && $ids === FALSE && !$conditions) && ($ids === FALSE || $ids || $revision_id)) {
      $queried_entities = array();
      foreach ($this->query($ids, $conditions, $revision_id) as $record) {
        // Skip entities already retrieved from cache.
        if (isset($entities[$record->{$this->nameKey}])) {
          continue;
        }

        // Care for serialized columns.
        $schema = drupal_get_schema($this->entityInfo['base table']);

        foreach ($schema['fields'] as $field => $info) {
          if (!empty($info['serialize']) && isset($record->$field)) {
            $record->$field = unserialize($record->$field);
            // Support automatic merging of 'data' fields into the entity.
            if (!empty($info['merge']) && is_array($record->$field)) {
              foreach ($record->$field as $key => $value) {
                $record->$key = $value;
              }
              unset($record->$field);
            }
          }
        }

        $queried_entities[$record->{$this->nameKey}] = $record;
      }
    }

    // Pass all entities loaded from the database through $this->attachLoad(),
    // which attaches fields (if supported by the entity type) and calls the
    // entity type specific load callback, for example hook_node_load().
    if (!empty($queried_entities)) {
      $this->attachLoad($queried_entities, $revision_id);
      $entities += $queried_entities;
    }

    if ($this->cache) {
      // Add entities to the cache if we are not loading a revision.
      if (!empty($queried_entities) && !$revision_id) {
        $this->cacheSet($queried_entities);

        // Remember we have cached all entities now.
        if (!$conditions && $ids === FALSE) {
          $this->cacheComplete = TRUE;
        }
      }
    }
    // Ensure that the returned array is ordered the same as the original
    // $ids array if this was passed in and remove any invalid ids.
    if ($passed_ids && $passed_ids = array_intersect_key($passed_ids, $entities)) {
      foreach ($passed_ids as $id => $value) {
        $passed_ids[$id] = $entities[$id];
      }
      $entities = $passed_ids;
    }
    return $entities;
  }

  protected function applyConditions($entities, $conditions = array()) {
    if ($conditions) {
      foreach ($entities as $key => $entity) {
        $entity_values = (array) $entity;
        if (array_diff_assoc($conditions, $entity_values)) {
          unset($entities[$key]);
        }
      }
    }
    return $entities;
  }

  /**
   * Overridden.
   * @see includes/DrupalDefaultEntityController#cacheGet($ids, $conditions)
   *
   * If there is nameKey given, we index our entities by this key. This
   * overrides cacheGet() to respect that when applying $conditions.
   */
  protected function cacheGet($ids, $conditions = array()) {
    if (!empty($this->entityCache)) {
      // First get the entities by ids, then apply the conditions.
      $entities = is_array($ids) ? array_intersect_key($this->entityCache, array_flip($ids)) : $this->entityCache;
      return $this->applyConditions($entities, $conditions);
    }
    return array();
  }

  /**
   * Overridden.
   * @see DrupalDefaultEntityController::attachLoad()
   *
   * Fixed to make attaching fields to entities having a name key work.
   */
  protected function attachLoad(&$queried_entities, $revision_id = FALSE) {
    // Attach fields.
    if ($this->entityInfo['fieldable']) {
      // Field API assumes queried entities are keyed by the idkey, thus
      // adapt the array accordingly for it.
      $entities = $this->getEntitiesKeyedByNumericID($queried_entities);
      if ($revision_id) {
        field_attach_load_revision($this->entityType, $entities);
      }
      else {
        field_attach_load($this->entityType, $entities);
      }
    }

    // Call hook_entity_load().
    foreach (module_implements('entity_load') as $module) {
      $function = $module . '_entity_load';
      $function($queried_entities, $this->entityType);
    }
    // Call hook_TYPE_load(). The first argument for hook_TYPE_load() are
    // always the queried entities, followed by additional arguments set in
    // $this->hookLoadArguments.
    $args = array_merge(array($queried_entities), $this->hookLoadArguments);
    foreach (module_implements($this->entityInfo['load hook']) as $module) {
      call_user_func_array($module . '_' . $this->entityInfo['load hook'], $args);
    }
  }

  /**
   * Converts an array of entities in an array of entities keyed by numeric id, i.e. by id key.
   */
  protected function getEntitiesKeyedByNumericID($entities) {
    if ($this->nameKey != $this->idKey) {
      $result = array();
      foreach ($entities as $entity) {
        $result[$entity->{$this->idKey}] = $entity;
      }
      return $result;
    }
    return $entities;
  }

  public function resetCache(array $ids = NULL) {
    $this->cacheComplete = FALSE;
    parent::resetCache($ids);
  }

  /**
   * Implements EntityAPIControllerInterface.
   */
  public function invoke($hook, $entity) {
    if (!empty($this->entityInfo['fieldable']) && function_exists($function = 'field_attach_' . $hook)) {
      $function($this->entityType, $entity);
    }

    if (isset($this->entityInfo['bundle of']) && $type = $this->entityInfo['bundle of']) {
      // Call field API bundle attachers for the entity we are a bundle of.
      if ($hook == 'insert') {
        field_attach_create_bundle($type, $entity->{$this->bundleKey});
      }
      elseif ($hook == 'delete') {
        field_attach_delete_bundle($type, $entity->{$this->bundleKey});
      }
      elseif ($hook == 'update' && $id = $entity->{$this->nameKey}) {
        if ($entity->original->{$this->bundleKey} != $entity->{$this->bundleKey}) {
          field_attach_rename_bundle($type, $entity->original->{$this->bundleKey}, $entity->{$this->bundleKey});
        }
      }
    }
    // Invoke the hook. If rules is there, use the rule funtion so that a rules
    // event is invoked too.
    if (module_exists('rules')) {
      rules_invoke_all($this->entityType . '_' . $hook, $entity);
    }
    else {
      module_invoke_all($this->entityType . '_' . $hook, $entity);
    }
    // Invoke the respective entity level hook.
    if ($hook == 'presave' || $hook == 'insert' || $hook == 'update' || $hook == 'delete') {
      module_invoke_all('entity_' . $hook, $entity, $this->entityType);
    }
  }

  /**
   * Implements EntityAPIControllerInterface.
   *
   * @param $transaction
   *   Optionally a DatabaseTransaction object to use. Allows overrides to pass
   *   in their transaction object.
   */
  public function delete($ids, DatabaseTransaction $transaction = NULL) {
    $entities = $ids ? $this->load($ids) : FALSE;
    if (!$entities) {
      // Do nothing, in case invalid or no ids have been passed.
      return;
    }
    $transaction = isset($transaction) ? $transaction : db_transaction();

    try {
      db_delete($this->entityInfo['base table'])
        ->condition($this->nameKey, array_keys($entities), 'IN')
        ->execute();
      // Reset the cache as soon as the changes have been applied.
      $this->resetCache($ids);

      foreach ($entities as $id => $entity) {
        $this->invoke('delete', $entity);
        if (entity_has_status($this->entityType, $entity, ENTITY_IN_CODE)) {
          $rebuild = TRUE;
        }
      }
      // Ignore slave server temporarily.
      db_ignore_slave();
    }
    catch (Exception $e) {
      $transaction->rollback();
      watchdog_exception($this->entityType, $e);
      throw $e;
    }
    // We have to postpone rebuilding defaults until the transaction has been
    // committed. This is as variable_set() triggers a merge query and dies with
    // a savepoint exception if it is inside a transaction.
    if (!empty($rebuild)) {
      $transaction = NULL;
      entity_defaults_rebuild(array($this->entityType));
    }
  }

  /**
   * Implements EntityAPIControllerInterface.
   *
   * @param $transaction
   *   Optionally a DatabaseTransaction object to use. Allows overrides to pass
   *   in their transaction object.
   */
  public function save($entity, DatabaseTransaction $transaction = NULL) {
    $transaction = isset($transaction) ? $transaction : db_transaction();
    try {
      // Load the stored entity, if any.
      if (!empty($entity->{$this->nameKey}) && !isset($entity->original)) {
        // In order to properly work in case of name changes, load the original
        // entity using the id key if it is available.
        $entity->original = entity_load_unchanged($this->entityType, !empty($entity->{$this->idKey}) ? $entity->{$this->idKey} : $entity->{$this->nameKey});
      }
      // Update the status for entities getting overridden.
      if (entity_has_status($this->entityType, $entity, ENTITY_IN_CODE) && empty($entity->is_rebuild)) {
        $entity->{$this->statusKey} |= ENTITY_CUSTOM;
      }

      $this->invoke('presave', $entity);

      if (!empty($entity->{$this->idKey}) && empty($entity->is_new)) {
        $return = drupal_write_record($this->entityInfo['base table'], $entity, $this->idKey);
        $this->resetCache(array($entity->{$this->nameKey}));
        $this->invoke('update', $entity);
      }
      else {
        $return = drupal_write_record($this->entityInfo['base table'], $entity);
        $this->invoke('insert', $entity);
      }
      // Ignore slave server temporarily.
      db_ignore_slave();
      unset($entity->is_new);
      unset($entity->original);

      return $return;
    }
    catch (Exception $e) {
      $transaction->rollback();
      watchdog_exception($this->entityType, $e);
      throw $e;
    }
  }

  /**
   * Implements EntityAPIControllerInterface.
   */
  public function create(array $values = array()) {
    // Add is_new property if it is not set.
    $values += array('is_new' => TRUE);
    if (isset($this->entityInfo['entity class']) && $class = $this->entityInfo['entity class']) {
      return new $class($values, $this->entityType);
    }
    return (object) $values;
  }

  /**
   * Implements EntityAPIControllerInterface.
   *
   * @return
   *   A serialized string in JSON format suitable for the import() method.
   */
  public function export($entity, $prefix = '') {
    $vars = get_object_vars($entity);
    unset($vars[$this->statusKey], $vars[$this->moduleKey], $vars['is_new']);
    if ($this->nameKey != $this->idKey) {
      unset($vars[$this->idKey]);
    }
    return entity_var_json_export($vars, $prefix);
  }

  /**
   * Implements EntityAPIControllerInterface.
   *
   * @param $export
   *   A serialized string in JSON format as produced by the export() method.
   */
  public function import($export) {
    $vars = drupal_json_decode($export);
    if (is_array($vars)) {
      return $this->create($vars);
    }
    return FALSE;
  }

  /**
   * Implements EntityAPIControllerInterface.
   *
   * @param $content
   *   Optionally. Allows pre-populating the built content to ease overridding
   *   this method.
   */
  public function buildContent($entity, $view_mode = 'full', $langcode = NULL, $content = array()) {
    // Remove previously built content, if exists.
    $entity->content = $content;
    $langcode = isset($langcode) ? $langcode : $GLOBALS['language_content']->language;

    // Add in fields.
    if (!empty($this->entityInfo['fieldable'])) {
      $entity->content += field_attach_view($this->entityType, $entity, $view_mode, $langcode);
    }
    // Invoke hook_ENTITY_view() to allow modules to add their additions.
    if (module_exists('rules')) {
      rules_invoke_all($this->entityType . '_view', $entity, $view_mode, $langcode);
    }
    else {
      module_invoke_all($this->entityType . '_view', $entity, $view_mode, $langcode);
    }
    module_invoke_all('entity_view', $entity, $this->entityType, $view_mode, $langcode);
    $build = $entity->content;
    unset($entity->content);
    return $build;
  }

  /**
   * Implements EntityAPIControllerInterface.
   */
  public function view($entities, $view_mode = 'full', $langcode = NULL) {
    if (!empty($this->entityInfo['fieldable'])) {
      // Field API assumes queried entities are keyed by the idkey, thus
      // adapt the array accordingly for it.
      field_attach_prepare_view($this->entityType, $this->getEntitiesKeyedByNumericID($entities), $view_mode);
    }
    entity_prepare_view($this->entityType, $entities);
    $langcode = isset($langcode) ? $langcode : $GLOBALS['language_content']->language;

    $view = array();
    foreach ($entities as $key => $entity) {
      $build = entity_build_content($this->entityType, $entity, $view_mode, $langcode);
      $build += array(
        // If the entity type provides an implementation, use this instead the
        // generic one.
        '#theme' => 'entity',
        '#entity_type' => $this->entityType,
        '#entity' => $entity,
        '#view_mode' => $view_mode,
        '#language' => $langcode,
      );
      // Allow modules to modify the structured entity.
      drupal_alter(array($this->entityType . '_view', 'entity_view'), $build, $this->entityType);
      $view[$this->entityType][$key] = $build;
    }
    return $view;
  }

}
