<?php

/**
 * Base class for all Drupal-to-Drupal migration classes.
 */
abstract class DrupalMigration extends DynamicMigration {
  /**
   * Connection key for the DatabaseConnection holding the source Drupal
   * installation.
   *
   * @var
   */
  protected $sourceConnection;

  /**
   * The major version of the Drupal install serving as the migration
   * source, e.g. '6'.
   *
   * @var int
   */
  protected $sourceVersion;

  /**
   * Options to be passed to source constructors.
   *
   * @var array
   */
  protected $sourceOptions = array();
  /**
   * Derived classes may add source field definitions before calling their
   * parent constructor.
   *
   * @var array
   */
  protected $sourceFields = array();

  /**
   * While much of the version-specific work can be done in the leaf classes,
   * to share data and behavior among all classes for a given Drupal version
   * we use this helper object.
   *
   * @var DrupalVersionInterface
   */
  protected $versionSpecific;

  /**
   * Map format mappings from the source system to the destination. Automated
   * generation of these mappings is version-dependent (through Drupal 6 they
   * were identified by numeric IDs, Drupal 7 introduced machine names).
   *
   * @var array
   */
  protected $formatMappings = array();

  /**
   * Required arguments:
   *
   * source_connection - Connection key for the DatabaseConnection holding the
   *  source Drupal installation.
   * source_version - Major version number (as an integer) of the source install.
   * machine_name - Machine name under which a particular migration is registered.
   * description - Description of the migration.
   *
   * Optional arguments:
   *
   * dependencies - Array of migrations that must be run before this one.
   * soft_dependencies - Array of migrations that should be listed before this one.
   * format_mappings - Array keyed by source format IDs or machine names, with
   *  the values being the corresponding D7 machine name. If unspecified,
   * source_options - Array to be passed as options to source constructors.
   *
   * @param array $arguments
   */
  public function __construct($arguments) {
    $this->arguments = $arguments;
    if (!isset($arguments['group'])) {
      $arguments['group'] = MigrateGroup::getInstance('default');
    }
    parent::__construct($arguments['group']);
    $this->sourceConnection = $arguments['source_connection'];
    $this->sourceVersion = $arguments['source_version'];
    $this->description = $arguments['description'];
    if (!empty($arguments['dependencies'])) {
      $this->dependencies = $arguments['dependencies'];
    }
    if (!empty($arguments['soft_dependencies'])) {
      $this->softDependencies = $arguments['soft_dependencies'];
    }
    $this->sourceOptions = array('map_joinable' => FALSE, 'cache_counts' => TRUE,
                     'cache_key' => 'migrate_' . $this->machineName);
    if (!empty($arguments['source_options'])) {
      $this->sourceOptions = array_merge($this->sourceOptions,
                                         $arguments['source_options']);
    }

    // Handle shared version-specific stuff
    $version_class = 'DrupalVersion' . $this->sourceVersion;
    $this->versionSpecific = new $version_class($arguments);

    if (!empty($arguments['format_mappings'])) {
      $this->formatMappings = $arguments['format_mappings'];
    } else {
      $this->formatMappings = $this->versionSpecific->getDefaultFormatMappings();
    }
  }

  /**
   * Use the explicit machine_name argument as this migration's machine name.
   */
  protected function generateMachineName($class_name = NULL) {
    return $this->arguments['machine_name'];
  }

  /**
   * Field mapping callback: translate an incoming format ID (through D6) or
   * machine name (D7+) to a D7 format machine name.
   *
   * @param $format
   * @return string
   */
  protected function mapFormat($format) {
    if (isset($this->formatMappings[$format])) {
      return $this->formatMappings[$format];
    } else {
      return NULL;
    }
  }
}

/**
 * There should be an implementation of this interface, named
 * DrupalVersion{version #}, for each Drupal version supported as a source. It
 * will implement any functions needed by multiple version-specific classes
 * (e.g., nodes as well as users).
 */
interface DrupalVersionInterface {
  /**
   * Pass the migration class arguments through to the version class.
   *
   * @param $arguments
   */
  public function __construct($arguments);

  /**
   * @abstract
   * Returns an array keyed by the source system's format identifier (integer ID
   * or machine name), with the destination Drupal 7 machine name as the value.
   */
  public function getDefaultFormatMappings();

  /**
   * @abstract
   * Given a source path (e.g., node/1234 or user/35), return the alias from
   * the source database.
   *
   * @param $pattern
   * @param $id
   */
  public function getPath($source);
}

/**
 * Implementation of hook_migrate_api().
 */
function migrate_d2d_migrate_api() {
  $api = array('api' => 2);
  return $api;
}
