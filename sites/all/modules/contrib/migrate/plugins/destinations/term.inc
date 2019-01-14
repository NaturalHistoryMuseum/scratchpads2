<?php

/**
 * @file
 * Support for taxonomy term destinations.
 */

// TODO:
// Make sure this works with updates, explicit destination keys
// taxonomy_term_save() is doing a cache_clear_all and an automatic insertion for parent.

/**
 * Destination class implementing migration into terms.
 */
class MigrateDestinationTerm extends MigrateDestinationEntity {

  /**
   * Boolean indicating whether to permit duplicate terms to be created.
   *
   * @var bool
   */
  protected $allowDuplicateTerms = FALSE;

  static public function getKeySchema() {
    return array(
      'tid' => array(
        'type' => 'int',
        'unsigned' => TRUE,
        'description' => 'ID of destination term',
      ),
    );
  }

  /**
   * Return an options array for term destinations.
   *
   * @param string $language
   *  Default language for terms created via this destination class.
   * @param string $text_format
   *  Default text format for terms created via this destination class.
   * @param string $allow_duplicate_terms
   *  Default text format for terms created via this destination class.
   */
  static public function options($language, $text_format, $allow_duplicate_terms = FALSE) {
    return compact('language', 'text_format', 'allow_duplicate_terms');
  }

  /**
   * Basic initialization
   *
   * @param array $options
   *  Options applied to terms.
   */
  public function __construct($bundle, array $options = array()) {
    parent::__construct('taxonomy_term', $bundle, $options);

    if (isset($options['allow_duplicate_terms'])) {
      $this->allowDuplicateTerms = $options['allow_duplicate_terms'];
    }
  }

  /**
   * Returns a list of fields available to be mapped for this vocabulary
   * (bundle)
   *
   * @param Migration $migration
   *  Optionally, the migration containing this destination.
   *
   * @return array
   *  Keys: machine names of the fields (to be passed to addFieldMapping)
   *  Values: Human-friendly descriptions of the fields.
   */
  public function fields($migration = NULL) {
    $fields = array();
    // First the core (taxonomy_term_data table) properties
    $fields['tid'] = t('<a href="@doc">Existing term ID</a>',
      array('@doc' => 'http://drupal.org/node/1349702#tid'));
    $fields['name'] = t('<a href="@doc">Name</a>',
      array('@doc' => 'http://drupal.org/node/1349702#name'));
    $fields['description'] = t('<a href="@doc">Description</a>',
      array('@doc' => 'http://drupal.org/node/1349702#description'));
    $fields['parent'] = t('<a href="@doc">Parent (by Drupal term ID)</a>',
      array('@doc' => 'http://drupal.org/node/1349702#parent'));
    // TODO: Remove parent_name, implement via arguments
    $fields['parent_name'] = t('<a href="@doc">Parent (by name)</a>',
      array('@doc' => 'http://drupal.org/node/1349702#parent_name'));
    $fields['format'] = t('<a href="@doc">Format</a>',
      array('@doc' => 'http://drupal.org/node/1349702#format'));
    $fields['weight'] = t('<a href="@doc">Weight</a>',
      array('@doc' => 'http://drupal.org/node/1349702#weight'));

    // Then add in anything provided by handlers
    $fields += migrate_handler_invoke_all('entity', 'fields', $this->entityType, $this->bundle, $migration);
    $fields += migrate_handler_invoke_all('taxonomy_term', 'fields', $this->entityType, $this->bundle, $migration);

    return $fields;
  }

  /**
   * Delete a migrated term
   *
   * @param $ids
   *  Array of fields representing the key (in this case, just tid).
   */
  public function rollback(array $key) {
    $tid = reset($key);

    /*
     * This load() happens soon delete() anyway. We load here in order to
     * avoid notices when term has already been deleted. That is easily possible
     * considering how deleting a term parent also deletes children in same call.
     */
    migrate_instrument_start('taxonomy_term_load');
    if (taxonomy_term_load($tid)) {
      migrate_instrument_stop('taxonomy_term_load');
      migrate_instrument_start('taxonomy_term_delete');
      $this->prepareRollback($tid);
      $result = (bool) taxonomy_term_delete($tid);
      $this->completeRollback($tid);
      migrate_instrument_stop('taxonomy_term_delete');
    }
    else {
      migrate_instrument_stop('taxonomy_term_load');
      // If it didn't exist, consider this a success
      $result = TRUE;
    }
    return $result;
  }

  /**
   * Import a single term.
   *
   * @param $term
   *  Term object to build. Prefilled with any fields mapped in the Migration.
   * @param $row
   *  Raw source data object - passed through to prepare/complete handlers.
   *
   * @return array
   *  Array of key fields (tid only in this case) of the term that was saved if
   *  successful. FALSE on failure.
   */
  public function import(stdClass $term, stdClass $row) {
    $migration = Migration::currentMigration();
    // Updating previously-migrated content?
    if (isset($row->migrate_map_destid1)) {
      if (isset($term->tid)) {
        if ($term->tid != $row->migrate_map_destid1) {
          throw new MigrateException(t("Incoming tid !tid and map destination nid !destid1 don't match",
            array(
              '!tid' => $term->tid,
              '!destid1' => $row->migrate_map_destid1,
            )));
        }
      }
      else {
        $term->tid = $row->migrate_map_destid1;
      }
    }

    // Default to bundle if no vocabulary machine name provided
    if (!isset($term->vocabulary_machine_name)) {
      $term->vocabulary_machine_name = $this->bundle;
    }

    if ($migration->getSystemOfRecord() == Migration::DESTINATION) {
      if (!isset($term->tid)) {
        throw new MigrateException(t('System-of-record is DESTINATION, but no destination tid provided'));
      }
      $rawterm = $term;
      $this->prepare($term, $row);
      $old_term = taxonomy_term_load($term->tid);
      if (empty($old_term)) {
        throw new MigrateException(t('System-of-record is DESTINATION, but term !tid does not exist',
          array('!tid' => $term->tid)));
      }
      foreach ($rawterm as $field => $value) {
        $old_term->{$field} = $term->{$field};
      }
      $term = $old_term;
    }
    else {
      // vid is required
      if (empty($term->vid)) {
        static $vocab_map = array();
        if (!isset($vocab_map[$term->vocabulary_machine_name])) {
          // The keys of the returned array are vids
          $vocabs = taxonomy_vocabulary_load_multiple(array(),
            array('machine_name' => $term->vocabulary_machine_name));
          $vids = array_keys($vocabs);
          if (isset($vids[0])) {
            $vocab_map[$term->vocabulary_machine_name] = $vids[0];
          }
          else {
            $migration->saveMessage(t('No vocabulary found with machine_name !name',
              array('!name' => $term->vocabulary_machine_name)));
            return FALSE;
          }
        }
        $term->vid = $vocab_map[$term->vocabulary_machine_name];
      }

      // Look up parent name if provided
      if (isset($term->parent_name) && trim($term->parent_name)) {
        // Look for the name in the same vocabulary.
        // Note that hierarchies may have multiples of the same name...
        $terms = taxonomy_term_load_multiple(array(),
          array('name' => trim($term->parent_name), 'vid' => $term->vid));
        $tids = array_keys($terms);
        $term->parent = array($tids[0]);
        unset($term->parent_name);
      }
      if (empty($term->parent)) {
        $term->parent = array(0);
      }
      elseif (!is_array($term->parent)) {
        // Convert to an array for comparison in findMatchingTerm().
        // Note: taxonomy_term_save() also normalizes to an array.
        $term->parent = array($term->parent);
      }

      if (isset($term->parent['arguments'])) {
        // Unset arguments here to avoid duplicate entries in the
        // term_hierarchy table.
        unset($term->parent['arguments']);
      }
      if (!isset($term->format)) {
        $term->format = $this->textFormat;
      }
      if (!isset($term->language)) {
        $term->language = $this->language;
      }
      $this->prepare($term, $row);

      if (empty($term->name)) {
        throw new MigrateException(t('Taxonomy term name is required.'));
      }

      if (!$this->allowDuplicateTerms && $existing_term = $this->findMatchingTerm($term)) {
        foreach ($existing_term as $field => $value) {
          if (!isset($term->{$field})) {
            $term->{$field} = $existing_term->{$field};
          }
        }
      }
    }

    // Trying to update an existing term
    if ($migration->getSystemOfRecord() == Migration::DESTINATION) {
      $existing_term = taxonomy_term_load($term->tid);
      if ($existing_term) {
        // Incoming data overrides existing data, so only copy non-existent fields
        foreach ($existing_term as $field => $value) {
          if (!isset($term->{$field})) {
            $term->{$field} = $existing_term->{$field};
          }
        }
      }
    }

    if (isset($term->tid)) {
      $updating = TRUE;
    }
    else {
      $updating = FALSE;
    }

    // Validate field data prior to saving.
    MigrateDestinationEntity::fieldAttachValidate('taxonomy_term', $term);

    migrate_instrument_start('taxonomy_term_save');
    $status = taxonomy_term_save($term);
    migrate_instrument_stop('taxonomy_term_save');
    $this->complete($term, $row);
    if (isset($term->tid)) {
      if ($updating) {
        $this->numUpdated++;
      }
      else {
        $this->numCreated++;
      }
      $return = array($term->tid);
    }
    else {
      $return = FALSE;
    }
    return $return;
  }

  /**
   * Attempt to find a term that has the same name, vocabulary, and parents.
   *
   * @param object $term
   *   A taxonomy term object with at least the name and vid properties defined.
   *
   * @return object
   *   A matching taxonomy term object if found, otherwise FALSE.
   */
  public function findMatchingTerm($term) {
    // See if the term, with the same parentage, already exists - if so,
    // load it
    $candidates = taxonomy_term_load_multiple(array(),
      array('name' => trim($term->name), 'vid' => $term->vid));
    foreach ($candidates as $candidate) {
      $parents = taxonomy_get_parents($candidate->tid);
      // We need to set up $parents as a simple array of tids
      if (empty($parents)) {
        $parents = array(0);
      }
      else {
        // Parents array is tid => term object, make into list of tids
        $new_parents = array();
        foreach ($parents as $parent) {
          $new_parents[] = $parent->tid;
        }
        $parents = $new_parents;
      }
      if ($term->parent == $parents) {
        // We've found a matching term.
        return $candidate;
      }
    }

    return FALSE;
  }
}
