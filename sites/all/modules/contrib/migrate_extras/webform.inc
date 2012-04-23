<?php

/**
 * Destination class for the webform_submissions table.
 *
 * Working component types:
 * - email
 * - date ('Y-m-d')
 * - file (use the file id)
 * - markup
 * - pagebreak (content is ignored)
 * - select (looks up the key by default, pass 'source_type' => 'value' as an
 *   argument to try to match the value)
 * - textfield
 * - textarea
 * - time ('H:i:s')
 * Untested/needs work:
 * - grid
 * - hidden
 */
class MigrateDestinationWebformSubmission extends MigrateDestination {
  static public function getKeySchema() {
    return array(
      'sid' => array(
        'type' => 'int',
        'not null' => TRUE,
        'unsigned' => TRUE,
      ),
    );
  }

  /**
   * The webform of the destination.
   *
   * @var string
   */
  protected $node;
  public function getWebform() {
    return $this->node;
  }

  /**
   * An array mapping our custom names to component ids.
   *
   * @var array
   */
  protected $component_cids;

  /**
   * Constructs a destination for a given webform node.
   *
   * @param object $node
   *   A node object that's type has been enabled for webform use.
   */
  public function __construct($node) {
    parent::__construct();

    if (empty($node)) {
      throw new Exception(t("You must provide a webform node"));
    }
    // Make sure it's a webform node.
    $types = webform_variable_get('webform_node_types');
    if (!in_array($node->type, $types)) {
      throw new Exception(t("The node must be configured to accept webform submissions but %type was not", array('%type' => $node->type)));
    }
    $this->node = $node;

    // Webform expects the component values to be keyed by cid, so we need a
    // hash to map prefixed field names to cid.
    $this->component_cids = array();
    foreach ($this->node->webform['components'] as $component) {
      $this->component_cids['data_' . $component['form_key']] = $component['cid'];
    }

    // We use the functions in this file in import() but load it here so we
    // only do it once.
    module_load_include('inc', 'webform', 'includes/webform.submissions');
  }

  public function __toString() {
    return t('Submission for the <a href="!link">%title</a> Webform', array(
      '!link' => url('node/' . $this->node->nid),
      '%title' => $this->node->title,
    ));
  }

  /**
   * Returns a list of fields available to be mapped.
   *
   * @return array
   *   Keys: machine names of the fields (to be passed to addFieldMapping)
   *   Values: Human-friendly descriptions of the fields.
   */
  public function fields() {
    // Fields defined by the schema. nid is omitted since it should come from
    // $this->node.
    $fields = array(
      'sid' => t('The unique identifier for this submission.'),
      'uid' => t('The id of the user that completed this submission.'),
      'is_draft' => t('Is this a draft of the submission?'),
      'submitted' => t('Timestamp of when the form was submitted.'),
      'remote_addr' => t('The IP address of the user that submitted the form.'),
    );

    // Create a field for each component on the webform.
    foreach ($this->node->webform['components'] as $component) {
      // TODO: Seems like we should skip over page break components.
      $fields['data_' . $component['form_key']] = t('@type: @name', array('@type' => $component['type'], '@name' => $component['name']));
    }

    // Then add in anything provided by handlers.
    $fields += migrate_handler_invoke_all('WebformSubmission', 'fields', $this->node);

    return $fields;
  }

  /**
   * Give handlers a shot at modifying the object before saving it.
   *
   * @param $entity
   *   Webform submission object to build. Prefilled with any fields mapped in
   *   the Migration.
   * @param $source_row
   *   Raw source data object - passed through to prepare handlers.
   */
  public function prepare($entity, stdClass $source_row) {
    $migration = Migration::currentMigration();
    $entity->migrate = array(
      'machineName' => $migration->getMachineName(),
    );
    // Call any general object handlers.
    migrate_handler_invoke_all('WebformSubmission', 'prepare', $entity, $source_row, $this->node);

    // Then call any prepare handler for this specific Migration.
    if (method_exists($migration, 'prepare')) {
      $migration->prepare($entity, $source_row, $this->node);
    }
  }

  /**
   * Give handlers a shot at modifying the object (or taking additional action)
   * after saving it.
   *
   * @param $entity
   *   Webform submission object to build. This is the complete object after
   *   saving.
   * @param $source_row
   *   Raw source data object - passed through to complete handlers.
   */
  public function complete($entity, stdClass $source_row) {
    $migration = Migration::currentMigration();
    // Call any general object handlers.
    migrate_handler_invoke_all('WebformSubmission', 'complete', $entity, $source_row, $this->node);
    // Then call any complete handler for this specific Migration.
    if (method_exists($migration, 'complete')) {
      $migration->complete($entity, $source_row, $this->node);
    }
  }

  /**
   * Import a record.
   *
   * @param $entity
   *   Webform submission object to build. This is the complete object after
   *   saving.
   * @param $source_row
   *   Raw source data object - passed through to complete handlers.
   */
  public function import(stdClass $entity, stdClass $row) {
    // Updating previously-migrated content?
    $migration = Migration::currentMigration();
    if (isset($row->migrate_map_destid1)) {
      if (isset($entity->sid) && $entity->sid != $row->migrate_map_destid1) {
        throw new MigrateException(t("Incoming sid !sid and map destination sid !destid1 don't match",
          array('!sid' => $entity->sid, '!destid1' => $row->migrate_map_destid1)));
      }
      else {
        $entity->sid = $row->migrate_map_destid1;
      }
    }

    $entity->nid = $this->node->nid;

    // Move the data from our custom keys back to webform's component ids.
    $data = array();
    foreach ($this->component_cids as $field_name => $cid) {
      if (isset($entity->$field_name)) {
        // Move the arguments out and kill any extraneous wrapper arrays.
        $value = $entity->$field_name;
        $arguments = array();
        if (is_array($value) && isset($value['arguments'])) {
          $arguments = (array) $value['arguments'];
          unset($value['arguments']);
          $value = count($value) ? reset($value) : $value;
        }
        // Avoid a warning if they passed in an empty array.
        $arguments += array('source_type' => 'key');

        // By default passed to select components are assumed to be the
        // key. If the key should be looked up use the add a
        // array('source_type' => 'value') argument to the field mapping.
        $component = $this->node->webform['components'][$cid];
        if ($component['type'] == 'select' && $arguments['source_type'] == 'value') {
          $options = _webform_select_options($component);
          $id = array_search($value, $options);
          $data[$cid] = ($id === FALSE) ? NULL : $id;
        }
        else {
          $data[$cid] = $value;
        }
        unset($entity->$field_name);
      }
    }
    $entity->data = webform_submission_data($this->node, $data);

    // Invoke migration prepare handlers
    $this->prepare($entity, $row);

    migrate_instrument_start('webform_submission_update/insert');
    // Determine if it's an insert or update.
    if (empty($entity->sid)) {
      $updating = FALSE;
      $sid = webform_submission_insert($this->node, $entity);
    }
    else {
      // If the sid was specified but doesn't exist we'll need to stick an
      // empty record in so webform's update has something to stick to.
      $status = db_merge('webform_submissions')
        ->key(array(
          'sid' => $entity->sid,
        ))
        ->insertFields(array(
          'sid' => $entity->sid,
          'nid' => $entity->nid,
          'submitted' => $entity->submitted,
          'remote_addr' => $entity->remote_addr,
          'is_draft' => $entity->is_draft,
          'bundle' => $entity->bundle,
        ))
        ->execute();
        // If db_merge() makes no changes $status is NULL so make a less
        // elegant comparison.
      $updating = MergeQuery::STATUS_INSERT !== $status;
      $sid = webform_submission_update($this->node, $entity);
    }
    migrate_instrument_stop('webform_submission_update/insert');

    if (isset($sid)) {
      $entity->sid = $sid;

      if ($updating) {
        $this->numUpdated++;
      }
      else {
        $this->numCreated++;
      }
      $return = array($sid);
    }
    else {
      $return = FALSE;
    }

    // Invoke migration complete handlers
    $this->complete($entity, $row);

    return $return;
  }

  /**
   * Delete a batch of submissions at once.
   *
   * @param $sids
   *   Array of submission IDs to be deleted.
   */
  public function bulkRollback(array $sids) {
    migrate_instrument_start(__METHOD__);
    foreach (webform_get_submissions(array('sid' => $sids)) as $submission) {
      webform_submission_delete($this->node, $submission);
    }
    migrate_instrument_stop(__METHOD__);
  }
}