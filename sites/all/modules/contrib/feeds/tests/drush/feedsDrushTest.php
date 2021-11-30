<?php

namespace Unish;

/**
 * Tests Drush integration for Feeds. Based on Drush 8.
 *
 * To execute this test from the command line:
 * @code
 * UNISH_NO_TIMEOUTS=1 UNISH_DRUPAL_MAJOR_VERSION=7 /path/to/drush/vendor/bin/phpunit --configuration /path/to/drush/tests /path/to/feeds/tests/drush
 * @endcode
 * Replace '/path/to' with the appropriate path to the directory in question.
 * Also be sure to have Drush installed with its dev dependencies and point to
 * the phpunit version that comes with Drush. Fore more information, see the
 * Feeds README file.
 *
 * @group commands
 */
class feedsDrushTest extends CommandUnishTestCase {

  /**
   * {@inheritdoc}
   */
  public function setUp() {
    if (UNISH_DRUPAL_MAJOR_VERSION != 7) {
      $this->markTestSkipped('This version of Feeds is for D7.');
    }

    // Install the standard install profile.
    $site = $this->setUpDrupal(1, TRUE, UNISH_DRUPAL_MAJOR_VERSION, 'standard');
    $root = $this->webroot();
    $this->siteOptions = array(
      'root' => $root,
      'uri' => key($site),
      'yes' => NULL,
    );

    // Copy the local Feeds directory to the test directory.
    $this->copyLocalFeedsDirToTestDir();

    // Enable the feeds_import module.
    $this->execDrush('pm-enable', array('feeds'));
    $this->execDrush('pm-enable', array('feeds_import'));
  }

  /**
   * Copies the local Feeds directory to the directory used in the test.
   */
  protected function copyLocalFeedsDirToTestDir() {
    $source = dirname(dirname(dirname(__FILE__)));
    $dest = $this->getFeedsDir();

    mkdir($dest, 0755, TRUE);
    foreach (
      $iterator = new \RecursiveIteratorIterator(
      new \RecursiveDirectoryIterator($source, \RecursiveDirectoryIterator::SKIP_DOTS),
      \RecursiveIteratorIterator::SELF_FIRST) as $item
    ) {
      if ($item->isDir()) {
        mkdir($dest . "/" . $iterator->getSubPathName());
      }
      else {
        copy($item, $dest . "/" . $iterator->getSubPathName());
      }
    }
  }

  /**
   * Wrapper around drush to always add the site options.
   *
   * @param string command
   *   The drush command to run.
   * @param array args
   *   Command arguments.
   * @param array $options
   *   An associative array containing options.
   */
  protected function execDrush($command, array $args, array $options = array()) {
    return $this->drush($command, $args, $this->siteOptions + $options);
  }

  /**
   * Returns path to Feeds directory.
   */
  protected function getFeedsDir() {
    return $this->webroot() . '/' . $this->drupalSitewideDirectory() . '/modules/feeds';
  }

  /**
   * Tests an import using the file option.
   */
  public function testImportUsingFileOption() {
    // Perform an import.
    $this->execDrush('feeds-import', array('node'), array(
      'file' => $this->getFeedsDir() . '/tests/feeds/content.csv',
    ));

    // Ensure that two nodes were created.
    $eval = "print db_query('SELECT COUNT(*) FROM {node}')->fetchField()";
    $this->execDrush('php-eval', array($eval));
    $this->assertEquals('2', $this->getOutput());
  }

  /**
   * Tests if an importer can get disabled.
   */
  public function testDisableFeedsImporter() {
    // First check that the user importer is enabled.
    $eval = "print feeds_importer('user')->disabled;";
    $this->execDrush('php-eval', array($eval));
    $this->assertEquals('', $this->getOutput());

    // Disable the user importer.
    $this->execDrush('feeds-disable', array('user'));

    // Ensure that the importer is now disabled.
    $this->execDrush('php-eval', array($eval));
    $this->assertEquals('1', $this->getOutput());
  }

  /**
   * Tests that no nodes get imported for a disabled importer.
   */
  public function testNoImportForDisabledImporter() {
    // Disable the node importer.
    $this->execDrush('feeds-disable', array('node'));

    // Try to perform an import. Drush command should fail.
    $this->drush('feeds-import', array('node'), $this->siteOptions + array(
      'file' => $this->getFeedsDir() . '/tests/feeds/content.csv',
    ), NULL, NULL, static::EXIT_ERROR);

    // Ensure that no nodes were imported.
    $eval = "print db_query('SELECT COUNT(*) FROM {node}')->fetchField()";
    $this->execDrush('php-eval', array($eval));
    $this->assertEquals('0', $this->getOutput());
  }

}
