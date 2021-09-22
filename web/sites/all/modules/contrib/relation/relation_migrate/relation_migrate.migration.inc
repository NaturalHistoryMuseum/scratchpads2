<?php

/**
 * @file
 * Migration for entityreference fields.
 */

abstract class RelationMigrateReference extends Migration {
  /**
   * Constructor.
   *
   * @param $field_type Field type machine name.
   */
  public function __construct($field_type) {
    parent::__construct();
    $this->fields = array_filter(variable_get('relation_migrate_' . $field_type . '_fields', array()));
    $this->relation_type = variable_get('relation_migrate_' . $field_type . '_relation_type', NULL);
    $this->dependencies = array();
    $this->description = 'Copy the contents from the ' . $field_type . ' fields to relation entities.';
    $this->map = new MigrateSQLMap($this->machineName,
      array(
        'source_type' => array(
          'type' => 'varchar',
          'length' => 128,
          'not null' => TRUE,
        ),
        'source_id' => array(
          'type' => 'int',
          'unsigned' => TRUE,
          'not null' => TRUE,
        ),
        'destination_type' => array(
          'type' => 'varchar',
          'length' => 128,
          'not null' => TRUE,
        ),
        'destination_id' => array(
          'type' => 'int',
          'unsigned' => TRUE,
          'not null' => TRUE,
        ),
        'delta' => array(
          'type' => 'int',
          'unsigned' => TRUE,
          'not null' => TRUE,
        ),
      ),
      MigrateDestinationRelation::getKeySchema()
    );

    $this->destination = new MigrateDestinationRelation($this->relation_type);

    $this->addFieldMapping('uid')
      ->defaultValue(variable_get('relation_migrate_' . $field_type . '_user', 1))
      ->description(t('The owner of relation.'));
  }

  public function prepare(stdClass $relation, stdClass $source_row) {
    $relation->endpoints[LANGUAGE_NONE] = array(
      array('entity_type' => $source_row->source_type, 'entity_id' => $source_row->source_id),
      array('entity_type' => $source_row->destination_type, 'entity_id' => $source_row->destination_id),
    );
  }
}

class RelationMigrateEntityReference extends RelationMigrateReference {
  public function __construct() {
    parent::__construct('entityreference');
    $this->source = new MigrateSourceEntityReference($this->fields);
  }
}

class RelationMigrateNodeReference extends RelationMigrateReference {
  public function __construct() {
    parent::__construct('node_reference');
    $this->source = new MigrateSourceNodeReference($this->fields);
  }
}

class RelationMigrateUserReference extends RelationMigrateReference {
  public function __construct() {
    parent::__construct('user_reference');
    $this->source = new MigrateSourceUserReference($this->fields);
  }
}

class RelationMigrateTermReference extends RelationMigrateReference {
  public function __construct() {
    parent::__construct('taxonomy_term_reference');
    $this->source = new MigrateSourceTermReference($this->fields);
  }
}
