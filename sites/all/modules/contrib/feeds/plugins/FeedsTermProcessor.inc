<?php

/**
 * @file
 * FeedsTermProcessor class.
 */

/**
 * Feeds processor plugin. Create taxonomy terms from feed items.
 */
class FeedsTermProcessor extends FeedsProcessor {

  /**
   * Implementation of FeedsProcessor::process().
   */
  public function process(FeedsImportBatch $batch, FeedsSource $source) {

    if (empty($this->config['vocabulary'])) {
      throw new Exception(t('You must define a vocabulary for Taxonomy term processor before importing.'));
    }

    // Count created, updated, and invalid terms.
    $created = $updated = $no_name = 0;

    while ($item = $batch->shiftItem()) {

      // Create/update if item does not exist or update existing is enabled.
      if (!($tid = $this->existingItemId($batch, $source)) || $this->config['update_existing'] != FEEDS_SKIP_EXISTING) {

        // Map feed item to a term.
        $term = array();

        // Add term id if available.
        if ($tid) {
          $term['tid'] = $tid;
        }

        // Load the term if configured to update existing terms.
        if (!empty($term['tid']) && $this->config['update_existing'] == FEEDS_UPDATE_EXISTING) {
          // Load term.
          $term = (array) taxonomy_get_term($term['tid'], TRUE);

          // Load feeds_term_item data.
          if ($feeds_term_item = db_fetch_array(db_query("SELECT imported, guid, url, feed_nid FROM {feeds_term_item} WHERE tid = %d", $term['tid']))) {
            $term['feeds_term_item'] = $feeds_term_item;
          }

          // Allow other modules to act.
          // @todo this breaks if hooks don't exist, or hooks don't return the term
          if (module_implements('feeds_taxonomy_load')) {
            $term = module_invoke_all('feeds_taxonomy_load', $term);
          }
        }

        // Add feeds_term_item data.
        $term['feeds_term_item']['id'] = $this->id;
        if (!isset($term['feeds_term_item']['feed_nid'])) {
          $term['feeds_term_item']['feed_nid'] = $source->feed_nid;
        }
        if (!isset($term['feeds_term_item']['imported'])) {
          $term['feeds_term_item']['imported'] = FEEDS_REQUEST_TIME;
        }

        // Map targets.
        $term = $this->map($batch, $term);

        // Check if term name is set, otherwise continue.
        if (empty($term['name'])) {
          $no_name++;
          continue;
        }

        // Save the term.
        $status = taxonomy_save_term($term);

        // Track new and updated terms.
        if ($status === SAVED_UPDATED) {
          $updated++;
        }
        elseif ($status === SAVED_NEW) {
          $created++;
        }
      }
    }

    // Set messages.
    $vocabulary = $this->vocabulary();
    if ($no_name) {
      drupal_set_message(
        format_plural(
          $no_name,
          'There was @number term that could not be imported because its name was empty. Check the mapping settings for the associated taxonomy term processor.',
          'There were @number terms that could not be imported because their names were empty. Check the mapping settings for the associated taxonomy term processor.',
          array('@number' => $no_name)
        ),
        'error'
      );
    }
    if ($created) {
      drupal_set_message(format_plural($created, 'Created @number term in !vocabulary.', 'Created @number terms in !vocabulary.', array('@number' => $created, '!vocabulary' => $vocabulary->name)));
    }
    elseif ($updated) {
      drupal_set_message(format_plural($updated, 'Updated @number term in !vocabulary.', 'Updated @number terms in !vocabulary.', array('@number' => $updated, '!vocabulary' => $vocabulary->name)));
    }
    else {
      drupal_set_message(t('There are no new terms.'));
    }
  }

  /**
   * Implementation of FeedsProcessor::clear().
   */
  public function clear(FeedsBatch $batch, FeedsSource $source) {
    $deleted = 0;
    $vocabulary = $this->vocabulary();
    $result = db_query("SELECT td.tid
                        FROM {term_data} td
                        JOIN {feeds_term_item} ft ON td.tid = ft.tid
                        WHERE td.vid = %d
                        AND ft.id = '%s'
                        AND ft.feed_nid = %d",
                        $vocabulary->vid, $this->id, $source->feed_nid);
    while ($term = db_fetch_object($result)) {
      if (taxonomy_del_term($term->tid) == SAVED_DELETED) {
        $deleted++;
      }
    }
    // Set messages.
    if ($deleted) {
      drupal_set_message(format_plural($deleted, 'Deleted @number term from !vocabulary.', 'Deleted @number terms from !vocabulary.', array('@number' => $deleted, '!vocabulary' => $vocabulary->name)));
    }
    else {
      drupal_set_message(t('No terms to be deleted.'));
    }
  }

  /**
   * Execute mapping on an item.
   */
  protected function map(FeedsImportBatch $batch, $target_term = NULL) {
    // Prepare term object.
    if (empty($target_term)) {
      $target_term = array();
    }

    // Verify vocabulary.
    if (!$vocabulary = $this->vocabulary()) {
      throw new Exception(t('No vocabulary specified for term processor'));
    }
    $target_term['vid'] = $vocabulary->vid;

    // Have parent class do the mapping.
    $target_term = parent::map($batch, $target_term);

    // Taxonomy module expects synonyms to be supplied as a single string.
    if (isset($target_term['synonyms']) && is_array($target_term['synonyms'])) {
      $target_term['synonyms'] = implode("\n", $target_term['synonyms']);
    }

    return $target_term;
  }

  /**
   * Override parent::configDefaults().
   */
  public function configDefaults() {
    return array(
      'vocabulary' => 0,
      'update_existing' => FEEDS_SKIP_EXISTING,
      'mappings' => array(),
    );
  }

  /**
   * Override parent::configForm().
   */
  public function configForm(&$form_state) {
    $options = array(0 => t('Select a vocabulary'));
    foreach (taxonomy_get_vocabularies() as $vid => $vocab) {
      if (strpos($vocab->module, 'features_') === 0) {
        $options[$vocab->module] = $vocab->name;
      }
      else {
        $options[$vid] = $vocab->name;
      }
    }
    $form = array();
    $form['vocabulary'] = array(
      '#type' => 'select',
      '#title' => t('Import to vocabulary'),
      '#description' => t('Choose the vocabulary to import into. <strong>CAUTION:</strong> when deleting terms through the "Delete items" tab, Feeds will delete <em>all</em> terms from this vocabulary.'),
      '#options' => $options,
      '#default_value' => $this->config['vocabulary'],
    );
    $form['update_existing'] = array(
      '#type' => 'radios',
      '#title' => t('Update existing terms'),
      '#description' => t('Select how existing terms should be updated. Existing terms will be determined using mappings that are a "unique target".'),
      '#options' => array(
        FEEDS_SKIP_EXISTING => 'Do not update existing terms',
        FEEDS_REPLACE_EXISTING => 'Replace existing terms',
        FEEDS_UPDATE_EXISTING => 'Update existing terms (slower than replacing them)',
      ),
      '#default_value' => $this->config['update_existing'],
    );
    return $form;
  }

  /**
   * Override parent::configFormValidate().
   */
  public function configFormValidate(&$values) {
    if (empty($values['vocabulary'])) {
      form_set_error('vocabulary', t('Choose a vocabulary'));
    }
  }

  /**
   * Override setTargetElement to operate on a target item that is a term.
   */
  public function setTargetElement(&$target_item, $target_element, $value) {
    if (in_array($target_element, array('url', 'guid'))) {
      $target_item['feeds_term_item'][$target_element] = $value;
    }
    else {
      parent::setTargetElement($target_item, $target_element, $value);
    }
  }

  /**
   * Return available mapping targets.
   */
  public function getMappingTargets() {
    $targets = array(
      'name' => array(
        'name' => t('Term name'),
        'description' => t('Name of the taxonomy term.'),
        'optional_unique' => TRUE,
      ),
      'description' => array(
        'name' => t('Term description'),
        'description' => t('Description of the taxonomy term.'),
      ),
      'synonyms' => array(
        'name' => t('Term synonyms'),
        'description' => t('One synonym or an array of synonyms of the taxonomy term.'),
      ),
      'url' => array(
        'name' => t('URL'),
        'description' => t('The external URL of the term. E. g. the feed item URL in the case of a syndication feed. May be unique.'),
        'optional_unique' => TRUE,
      ),
      'guid' => array(
        'name' => t('GUID'),
        'description' => t('The external GUID of the term. E. g. the feed item GUID in the case of a syndication feed. May be unique.'),
        'optional_unique' => TRUE,
      ),
    );
    // Let implementers of hook_feeds_term_processor_targets() add their targets.
    $vocabulary = $this->vocabulary();
    drupal_alter('feeds_term_processor_targets', $targets, $vocabulary->vid);
    return $targets;
  }

  /**
   * Get id of an existing feed item term if available.
   */
  protected function existingItemId(FeedsImportBatch $batch, FeedsSource $source) {
    // Iterate through all unique targets and test whether they already
    // exist in the database.
    foreach ($this->uniqueTargets($batch) as $target => $value) {
      switch ($target) {
        case 'url':
        case 'guid':
          $tid = db_result(db_query("SELECT tid FROM {feeds_term_item} WHERE feed_nid = %d AND id = '%s' AND %s = '%s'", $source->feed_nid, $this->id, $target, $value));
          break;
        case 'name':
          $vocabulary = $this->vocabulary();
          $tid = db_result(db_query("SELECT tid FROM {term_data} WHERE name = '%s' AND vid = %d", $value, $vocabulary->vid));
          break;
      }
      if ($tid) {
        // Return the first tid found.
        return $tid;
      }
    }
    return 0;
  }

  /**
   * Return vocabulary to map to.
   *
   * Feeds supports looking up vocabularies by their module name as part of an
   * effort to use the vocabulary.module field as machine name to make
   * vocabularies exportable.
   */
  public function vocabulary() {
    $vocabularies = taxonomy_get_vocabularies();
    if (is_numeric($this->config['vocabulary'])) {
      return $vocabularies[$this->config['vocabulary']];
    }
    else {
      foreach ($vocabularies as $vocabulary) {
        if ($vocabulary->module == $this->config['vocabulary']) {
          return $vocabulary;
        }
      }
    }
  }
}
