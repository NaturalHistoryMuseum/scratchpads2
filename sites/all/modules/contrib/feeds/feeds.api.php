<?php

/**
 * @file
 * Documentation of Feeds hooks.
 */

/**
 * @defgroup pluginapi Plugin API
 * @{
 * Feeds offers a CTools based plugin API.
 *
 * Fetchers, parsers and processors are declared to Feeds as plugins.
 *
 * @see feeds_feeds_plugins()
 * @see FeedsFetcher
 * @see FeedsParser
 * @see FeedsProcessor
 */

/**
 * CTools plugin hook example.
 *
 * This example of a CTools plugin hook needs to be implemented to make
 * hook_feeds_plugins() discoverable by CTools and Feeds. The hook specifies
 * that the hook_feeds_plugins() returns Feeds Plugin API version 1 style
 * plugins.
 */
function hook_ctools_plugin_api($owner, $api) {
  if ($owner == 'feeds' && $api == 'plugins') {
    return array('version' => 1);
  }
}

/**
 * Declare Feeds plugins.
 *
 * Implement this hook to declare Fetcher, Parser or Processor plugins for
 * Feeds. For a working example implementation, see feeds_feeds_plugin().
 * In order for this hook to be invoked, you MUST implement
 * hook_ctools_plugin_api() as well.
 *
 * @see feeds_feeds_plugin()
 */
function hook_feeds_plugins() {
  $info = array();
  $info['MyFetcher'] = array(
    'name' => 'My Fetcher',
    'description' => 'Fetches my stuff.',
    'help' => 'More verbose description here. Will be displayed on fetcher selection menu.',
    'handler' => array(
      'parent' => 'FeedsFetcher',
      'class' => 'MyFetcher',
      'file' => 'MyFetcher.inc',
      // Feeds will look for MyFetcher.inc in the my_module directory.
      'path' => drupal_get_path('module', 'my_module'),
    ),
  );
  $info['MyParser'] = array(
    'name' => 'ODK parser',
    'description' => 'Parse my stuff.',
    'help' => 'More verbose description here. Will be displayed on parser selection menu.',
    'handler' => array(
      // Being directly or indirectly an extension of FeedsParser makes a plugin
      // a parser plugin.
      'parent' => 'FeedsParser',
      'class' => 'MyParser',
      'file' => 'MyParser.inc',
      'path' => drupal_get_path('module', 'my_module'),
    ),
  );
  $info['MyProcessor'] = array(
    'name' => 'ODK parser',
    'description' => 'Process my stuff.',
    'help' => 'More verbose description here. Will be displayed on processor selection menu.',
    'handler' => array(
      'parent' => 'FeedsProcessor',
      'class' => 'MyProcessor',
      'file' => 'MyProcessor.inc',
      'path' => drupal_get_path('module', 'my_module'),
    ),
  );
  return $info;
}

/**
 * @} End of "defgroup pluginapi".
 */

/**
 * @defgroup import Import and clear hooks
 * @{
 */

/**
 * Invoked after a feed source has been parsed, before it will be processed.
 *
 * @param FeedsSource $source
 *   FeedsSource object that describes the source that has been imported.
 * @param FeedsParserResult $result
 *   FeedsParserResult object that has been parsed from the source.
 */
function hook_feeds_after_parse(FeedsSource $source, FeedsParserResult $result) {
  // For example, set title of imported content:
  $result->title = 'Import number ' . my_module_import_id();
}

/**
 * Invoked before a feed source import starts.
 *
 * @param FeedsSource $source
 *   FeedsSource object that describes the source that is going to be imported.
 */
function hook_feeds_before_import(FeedsSource $source) {
  // See feeds_rules module's implementation for an example.
}

/**
 * Invoked before a feed item is updated/created/replaced.
 *
 * This is called every time a feed item is processed no matter if the item gets
 * updated or not.
 *
 * @param FeedsSource $source
 *   The source for the current feed.
 * @param array $item
 *   All the current item from the feed.
 * @param int|null $entity_id
 *   The id of the current item which is going to be updated. If this is a new
 *   item, then NULL is passed.
 */
function hook_feeds_before_update(FeedsSource $source, $item, $entity_id) {
  if ($entity_id) {
    $processor = $source->importer->processor;
    db_update('foo_bar')
      ->fields(array(
        'entity_type' => $processor->entityType(),
        'entity_id' => $entity_id,
        'last_seen' => REQUEST_TIME,
      ))
      ->condition('entity_type', $processor->entityType())
      ->condition('entity_id', $entity_id)
      ->execute();
  }
}

/**
 * Invoked before a feed item is validated.
 *
 * @param FeedsSource $source
 *   FeedsSource object that describes the source that is being imported.
 * @param object $entity
 *   The entity object.
 * @param array $item
 *   The parser result for this entity.
 * @param int|null $entity_id
 *   The id of the current item which is going to be updated. If this is a new
 *   item, then NULL is passed.
 */
function hook_feeds_prevalidate(FeedsSource $source, $entity, $item, $entity_id) {
  // Correct a field value to make it pass validation.
  if (isset($entity->myfield)) {
    foreach ($entity->myfield as $language => &$values) {
      // There are only three values allowed. Throw away the rest.
      if (count($values) > 3) {
        $values = array_slice($values, 0, 3);
      }
    }
  }
}

/**
 * Invoked before a feed item is saved.
 *
 * @param FeedsSource $source
 *   FeedsSource object that describes the source that is being imported.
 * @param object $entity
 *   The entity object.
 * @param array $item
 *   The parser result for this entity.
 * @param int|null $entity_id
 *   The id of the current item which is going to be updated. If this is a new
 *   item, then NULL is passed.
 */
function hook_feeds_presave(FeedsSource $source, $entity, $item, $entity_id) {
  if ($entity->feeds_item->entity_type == 'node') {
    // Skip saving this entity.
    $entity->feeds_item->skip = TRUE;
  }
}

/**
 * Invoked after a feed item has been saved.
 *
 * @param FeedsSource $source
 *   FeedsSource object that describes the source that is being imported.
 * @param object $entity
 *   The entity object that has just been saved.
 * @param array $item
 *   The parser result for this entity.
 * @param int|null $entity_id
 *   The id of the current item which is going to be updated. If this is a new
 *   item, then NULL is passed.
 */
function hook_feeds_after_save(FeedsSource $source, $entity, $item, $entity_id) {
  // Although the $entity object is passed by reference, any changes made in
  // this function will be ignored by the FeedsProcessor.
  $config = $source->importer->getConfig();

  if ($config['processor']['config']['purge_unseen_items'] && isset($entity->feeds_item)) {
    $feeds_item = $entity->feeds_item;
    $feeds_item->batch_id = feeds_delete_get_current_batch($feeds_item->feed_nid);

    drupal_write_record('feeds_delete_item', $feeds_item);
  }
}

/**
 * Invoked after a feed source has been imported.
 *
 * @param FeedsSource $source
 *   FeedsSource object that describes the source that has been imported.
 */
function hook_feeds_after_import(FeedsSource $source) {
  // We can also check for an exception in this hook. The exception should not
  // be thrown here, Feeds will handle it.
  if (isset($source->exception)) {
    watchdog('mymodule', 'An exception occurred during importing!', array(), WATCHDOG_ERROR);
    mymodule_panic_reaction($source);
  }
}

/**
 * Invoked after a feed source has been cleared of its items.
 *
 * @param FeedsSource $source
 *   FeedsSource object that describes the source that has been cleared.
 */
function hook_feeds_after_clear(FeedsSource $source) {
}

/**
 * @} End of "defgroup import".
 */

/**
 * @defgroup mappingapi Mapping API
 * @{
 */

/**
 * Alter mapping sources.
 *
 * Use this hook to add additional mapping sources for any parser. Allows for
 * registering a callback to be invoked at mapping time.
 *
 * @see my_source_get_source()
 * @see locale_feeds_parser_sources_alter()
 */
function hook_feeds_parser_sources_alter(&$sources, $content_type) {
  $sources['my_source'] = array(
    'name' => t('Images in description element'),
    'description' => t('Images occurring in the description element of a feed item.'),
    'callback' => 'callback_my_source_get_source',
  );
}

/**
 * Returns a value to use as a mapping source.
 *
 * Callback for hook_feeds_parser_sources_alter().
 *
 * This function is called on mapping time.
 *
 * @param FeedsSource $source
 *   The FeedsSource object being imported.
 * @param FeedsParserResult $result
 *   The FeedsParserResult object being mapped from.
 * @param string $key
 *   The key specified in the $sources array in
 *   hook_feeds_parser_sources_alter().
 *
 * @return mixed
 *   The value to be extracted from the source.
 *
 * @see hook_feeds_parser_sources_alter()
 * @see locale_feeds_get_source()
 *
 * @ingroup callbacks
 */
function callback_my_source_get_source(FeedsSource $source, FeedsParserResult $result, $key) {
  $item = $result->currentItem();
  return my_source_parse_images($item['description']);
}

/**
 * Adds mapping targets for processors.
 *
 * This hook allows additional target options to be added to the processors
 * mapping form.
 *
 * If the key in $targets[] does not correspond to the actual key on the node
 * object ($node->key), real_target MUST be specified. See mappers/link.inc
 *
 * For an example implementation, see mappers/text.inc
 *
 * @param string $entity_type
 *   The entity type of the target, for instance a 'node' entity.
 * @param string $bundle
 *   The entity bundle to return targets for.
 *
 * @return array
 *   An array whose keys are the target name and whose values are arrays
 *   containing the following keys:
 *   - name: A human readable, translated label for the target.
 *   - description: (optional) A human readable, translated description for the
 *     target.
 *   - callback: The callback used to set the value on the target.
 *   - real_target: (optional) the name of the property on the entity that will
 *     be set by the callback. Specify this if the target name is not equal to
 *     the entity property name. This information will be used to clear the
 *     right target at the beginning of the mapping process.
 *   - optional_unique: (optional) A boolean that indicates whether or not the
 *     target can be used as an unique target. If you set this to TRUE, be sure
 *     to also specify "unique_callbacks".
 *   - unique_callbacks: (optional) An array of callbacks that are used to
 *     retrieve existing entity ids. Existing entities can be updated based on
 *     unique targets.
 *   - form_callbacks: (optional) An array of callbacks that are used to return
 *     a form with additional configuration for a target.
 *   - summary_callbacks: (optional) An array of callbacks that are used to
 *     display values of additional target configuration.
 *   - preprocess_callbacks: (optional) An array of callbacks that are used to
 *     set or change mapping options.
 *   - deprecated: (optional) A boolean that if TRUE, hides the target from the
 *     UI. Use this if you want to rename targets for consistency, but don't
 *     want to break importers that are using the old target name. If an
 *     importer uses this target it will show up as "DEPRECATED" in the UI.
 */
function hook_feeds_processor_targets($entity_type, $bundle) {
  $targets = array();

  if ($entity_type == 'node') {
    // Example 1: provide the minimal info for a target. Description is
    // optional, but recommended.
    // @see my_module_set_target()
    $targets['my_node_field'] = array(
      'name' => t('My custom node field'),
      'description' => t('Description of what my custom node field does.'),
      'callback' => 'callback_my_module_set_target',
    );

    // Example 2: specify "real_target" if the target name is different from
    // the entity property name.
    // Here the target is called "my_node_field2:uri", but the entity property
    // is called "my_node_field2". This will ensure that the property
    // "my_node_field2" is cleared out that the beginning of the mapping
    // process.
    $targets['my_node_field2:uri'] = array(
      'name' => t('My third custom node field'),
      'description' => t('A target that sets a property that does not have the same name as the target.'),
      'callback' => 'my_module_set_target2',
      'real_target' => 'my_node_field2',
    );

    // Example 3: you can make your target selectable as an unique target by
    // setting "optional_unique" to TRUE and specify one or more callbacks to
    // retrieve existing entity id's.
    // @see my_module_mapper_unique()
    $targets['my_node_field3'] = array(
      'name' => t('My third custom node field'),
      'description' => t('A field that can be set as an unique target.'),
      'callback' => 'my_module_set_target3',
      'optional_unique' => TRUE,
      'unique_callbacks' => array('callback_my_module_mapper_unique'),
    );

    // Example 4: use the form and summary callbacks to add additional
    // configuration options for your target. Use the form callbacks to provide
    // a form to set the target configuration. Use the summary callbacks to
    // display the target configuration.
    // @see my_module_form_callback()
    // @see my_module_summary_callback()
    $targets['my_node_field4'] = array(
      'name' => t('My fourth custom node field'),
      'description' => t('A field with additional configuration.'),
      'callback' => 'my_module_set_target4',
      'form_callbacks' => array('callback_my_module_form_callback'),
      'summary_callbacks' => array('callback_my_module_summary_callback'),
    );

    // Example 5: use preprocess callbacks to set or change mapping options.
    // @see my_module_preprocess_callback()
    $targets['my_node_field5'] = array(
      'name' => t('My fifth custom node field'),
      'description' => t('A field with additional configuration.'),
      'callback' => 'my_module_set_target5',
      'preprocess_callbacks' => array('callback_my_module_preprocess_callback'),
    );

    // Example 6: when you want to remove or rename previously provided targets,
    // you can set "deprecated" to TRUE for the old target name. This will make
    // the target to be no longer selectable in the UI. If an importer uses this
    // target it will show up as "DEPRECATED" in the UI.
    // If you want that the target continues to work, you can still specify the
    // callback.
    $targets['deprecated_target'] = array(
      'name' => t('A target that cannot be chosen in the UI.'),
      'deprecated' => TRUE,
    );
  }

  return $targets;
}

/**
 * Alters the target array.
 *
 * This hook allows modifying the target array.
 *
 * @param array &$targets
 *   Array containing the targets to be offered to the user. Add to this array
 *   to expose additional options.
 * @param string $entity_type
 *   The entity type of the target, for instance a 'node' entity.
 * @param string $bundle
 *   The entity bundle to return targets for.
 *
 * @see hook_feeds_processor_targets()
 */
function hook_feeds_processor_targets_alter(array &$targets, $entity_type, $bundle) {
  // Example: set an existing target as optional unique.
  if ($entity_type == 'node' && $bundle == 'article') {
    if (isset($targets['nid'])) {
      $targets['nid']['unique_callbacks'][] = 'my_module_mapper_unique';
      $targets['nid']['optional_unique'] = TRUE;
    }
  }
}

/**
 * Sets a value on a target.
 *
 * Callback for hook_feeds_processor_targets().
 *
 * This callback is specified on the 'callback' key of the target definition.
 * A target can for example be a field or property on an entity.
 *
 * @param FeedsSource $source
 *   Field mapper source settings.
 * @param object $entity
 *   An entity object, for instance a node object.
 * @param string $target
 *   A string identifying the target on the node.
 * @param array $values
 *   The value to populate the target with.
 * @param array $mapping
 *   Associative array of the mapping settings from the per mapping
 *   configuration form.
 *
 * @see hook_feeds_processor_targets()
 *
 * @ingroup callbacks
 */
function callback_my_module_set_target(FeedsSource $source, $entity, $target, array $values, array $mapping) {
  $entity->{$target}[$entity->language][0]['value'] = reset($values);
  if (isset($source->importer->processor->config['input_format'])) {
    $entity->{$target}[$entity->language][0]['format'] = $source->importer->processor->config['input_format'];
  }
}

/**
 * Returns a form for configuring a target.
 *
 * Callback for hook_feeds_processor_targets().
 *
 * This callback is specified on the 'form_callbacks' key of the target
 * definition.
 * The arguments are the same that callback_my_module_summary_callback() gets.
 *
 * @return array
 *   The per mapping configuration form. Once the form is saved, $mapping will
 *   be populated with the form values.
 *
 * @see hook_feeds_processor_targets()
 * @see callback_my_module_summary_callback()
 *
 * @ingroup callbacks
 */
function callback_my_module_form_callback(array $mapping, $target, array $form, array $form_state) {
  return array(
    'my_setting' => array(
      '#type' => 'checkbox',
      '#title' => t('My setting checkbox'),
      '#default_value' => !empty($mapping['my_setting']),
    ),
  );
}

/**
 * Returns a string for displaying the target configuration.
 *
 * Callback for hook_feeds_processor_targets().
 *
 * This callback is specified on the 'summary_callbacks' key of the target
 * definition.
 * The arguments are the same that callback_my_module_form_callback() gets.
 *
 * @param array $mapping
 *   Associative array of the mapping settings.
 * @param string $target
 *   Array of target settings, as defined by the processor or
 *   hook_feeds_processor_targets_alter().
 * @param array $form
 *   The whole mapping form.
 * @param array $form_state
 *   The form state of the mapping form.
 *
 * @return string
 *   Returns, as a string that may contain HTML, the summary to display while
 *   the full form isn't visible.
 *   If the return value is empty, no summary and no option to view the form
 *   will be displayed.
 *
 * @see hook_feeds_processor_targets()
 * @see callback_my_module_form_callback()
 *
 * @ingroup callbacks
 */
function callback_my_module_summary_callback(array $mapping, $target, array $form, array $form_state) {
  if (empty($mapping['my_setting'])) {
    return t('My setting <strong>not</strong> active');
  }
  else {
    return t('My setting <strong>active</strong>');
  }
}

/**
 * Looks for an existing entity and returns an entity ID if found.
 *
 * Callback for hook_feeds_processor_targets().
 *
 * This callback is specified on the 'unique_callbacks' key of the target
 * definition.
 *
 * @param FeedsSource $source
 *   The Feed source.
 * @param string $entity_type
 *   Entity type for the entity to be processed.
 * @param string $bundle
 *   Bundle name for the entity to be processed.
 * @param string $target
 *   A string identifying the unique target on the entity.
 * @param array $values
 *   The unique values to be checked.
 *
 * @return int|null
 *   The existing entity id, or NULL if no existing entity is found.
 *
 * @see hook_feeds_processor_targets()
 * @see FeedsProcessor::existingEntityId()
 *
 * @ingroup callbacks
 */
function callback_my_module_mapper_unique(FeedsSource $source, $entity_type, $bundle, $target, array $values) {
  list($field_name, $column) = explode(':', $target . ':value');
  // Example for if the target is a field.
  $query = new EntityFieldQuery();
  $result = $query
    ->entityCondition('entity_type', $entity_type)
    ->entityCondition('bundle', $bundle)
    ->fieldCondition($field_name, $column, $values)
    ->execute();

  if (!empty($result[$entity_type])) {
    return key($result[$entity_type]);
  }
}

/**
 * Changes or sets a mapping option.
 *
 * Callback for hook_feeds_processor_targets().
 *
 * This callback is specified on the 'preprocess_callbacks' key of the target
 * definition.
 *
 * @param array $target
 *   The full target definition.
 * @param array &$mapping
 *   The mapping configuration.
 *
 * @see hook_feeds_processor_targets()
 *
 * @ingroup callbacks
 */
function callback_my_module_preprocess_callback(array $target, array &$mapping) {
  // Add in default values.
  $mapping += array('setting_value' => TRUE);
}

/**
 * Add additional configuration keys to FeedsConfigurable.
 *
 * This hooks allows you to add additional configuration keys to a
 * FeedsConfigurable. This is useful if you also implement a form alter hook to
 * provide extra options for existing Feeds plugins. By implementing one of the
 * Feeds hooks that are invoked during importing, you can act upon such setting.
 *
 * @param FeedsConfigurable $configurable
 *   The configurable item to add default configuration to.
 *
 * @return array
 *   Return an array of default configuration.
 */
function hook_feeds_config_defaults(FeedsConfigurable $configurable) {
  if ($configurable instanceof FeedsImporter) {
    return array(
      'my_module_extra_setting_1' => 0,
      'my_module_extra_setting_2' => NULL,
    );
  }
}

/**
 * A plugin-specific hook to add additional configuration keys.
 *
 * This hook can be used instead of the global hook_feeds_config_defaults() and
 * allows you to add additional configuration keys to a FeedsPlugin.
 *
 * The plugin type can be:
 * - fetcher;
 * - parser;
 * - processor.
 *
 * @param FeedsPlugin $plugin
 *   The plugin to add default configuration to.
 *
 * @return array
 *   Return an array of default configuration.
 *
 * @see hook_feeds_config_defaults()
 */
function hook_feeds_PLUGIN_TYPE_config_defaults(FeedsPlugin $plugin) {
  if ($plugin instanceof FeedsCSVParser) {
    return array(
      'extra_csv_parser_setting' => NULL,
    );
  }
}

/**
 * @} End of "defgroup mappingapi".
 */
