<?php

/**
 * Base class for migrations of Drupal taxonomy terms from another Drupal
 * installation.
 */
abstract class DrupalTermMigration extends DrupalMigration {
  /**
   * The machine name, or vocabulary ID (pre-D7), of the vocabulary we're
   * migrating from.
   *
   * @var mixed
   */
  protected $sourceVocabulary;

  /**
   * The machine name of the Drupal 7 vocabulary we're migrating into.
   *
   * @var string
   */
  protected $destinationVocabulary;

  /**
   * The version-specific class must implement termQuery(), returning
   * a query object to obtain the term data for the specified source vocabulary.
   * The query should sort by parent, to make sure root terms are created first.
   */
  abstract protected function termQuery();

  /**
   * In addition to the arguments supported by DrupalMigration, we add the
   * following required arguments:
   *
   * source_vocabulary - Unique identifier for the source vocabulary (a vid
   *   through Drupal 6, machine name for Drupal 7 and later). This may be a
   *   comma-separated list, to support merging multiple vocabularies into one.
   * destination_vocabulary - Machine name of the destination vocabulary.
   *
   * @param array $arguments
   */
  public function __construct(array $arguments) {
    parent::__construct($arguments);
    $this->sourceVocabulary = $arguments['source_vocabulary'];
    $this->destinationVocabulary = $arguments['destination_vocabulary'];

    // Create our three main objects - source, destination, and map
    $this->source = new MigrateSourceSQL($this->termQuery(), $this->sourceFields,
      NULL, $this->sourceOptions);

    $this->destination = new MigrateDestinationTerm($this->destinationVocabulary);

    $this->map = new MigrateSQLMap($this->machineName,
      array(
        'tid' => array('type' => 'int',
                       'unsigned' => TRUE,
                       'not null' => TRUE,
                       'description' => 'Source term ID',
                      ),
      ),
      MigrateDestinationTerm::getKeySchema()
    );

    // Mappings are straight-forward
    $this->addSimpleMappings(array('name', 'description', 'weight'));
    $this->addFieldMapping('parent', 'parent')
         ->sourceMigration($this->machineName);

    // TODO: Handle path automatically like users and nodes
    $this->addUnmigratedDestinations(array('parent_name', 'path'));

    // TODO: Support custom fields
  }
}
