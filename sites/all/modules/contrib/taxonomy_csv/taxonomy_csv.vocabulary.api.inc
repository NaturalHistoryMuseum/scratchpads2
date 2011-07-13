<?php
// $Id: taxonomy_csv.vocabulary.api.inc,v 1.1.2.5 2011/01/06 18:08:27 danielkm Exp $

/**
 * @file
 * Prepare and manage vocabularies.
 */

/**
 * Prepare a regular thesaurus (Drupal 6 vocabulary) from a basic D7 vocabulary.
 *
 * A regular and simplified thesaurus contains not only generic and specific
 * terms (hierarchy of 'broader' and 'narrower' terms) and description, but
 * links between them ('related' terms) and synonyms (non descriptor terms:
 * 'used for' and 'use instead') too. This function creates such a vocabulary
 * for Drupal 7.
 *
 * @param $name
 *   Name of vocabulary. If vocabulary exists, it's updated, else it's created.
 * @param $synonym_field
 *   (Optional) Array of field options. If FALSE, don't create synonym field.
 *   Warning: this module manages synonyms only if name is 'taxonomy_synonym'.
 * @param $relation_field
 *   (Optional) Array of field options. If FALSE, don't create relation field.
 *   Warning: this module manages relations only if name is 'taxonomy_relation'.
 *
 * @return
 *   Updated vocabulary object or FALSE
 */
function taxonomy_csv_thesaurus_create($name = '', $synonym_field = array(), $relation_field = array()) {
  // Load or create vocabulary.
  $vocabulary = taxonomy_csv_vocabulary_name_check($name) ?
    taxonomy_csv_vocabulary_load_name($name) :
    taxonomy_csv_vocabulary_create($name);

  // Prepare synonyms field if needed.
  if ($synonym_field !== FALSE) {
    $synonym_field += array(
      'field_name'   => 'taxonomy_synonym',
      'label'        => 'Synonyms',
      'description'  => 'Synonyms of this term',
      'type'         => 'text',
      'cardinality'  => FIELD_CARDINALITY_UNLIMITED,
      'translatable' => FALSE,
    );
    $result = taxonomy_csv_vocabulary_field_add($vocabulary->machine_name, $synonym_field);
  }

  // Prepare relations field if needed.
  if ($relation_field !== FALSE) {
    $relation_field += array(
      'field_name'   => 'taxonomy_relation',
      'label'        => 'Related terms',
      'description'  => 'Relations to other terms',
      'type'         => 'taxonomy_term_reference',
      'cardinality'  => FIELD_CARDINALITY_UNLIMITED,
      'translatable' => FALSE,
      'settings'     => array('allowed_values' => array(0 => array(
        // Warning: Term reference field type need a specific vocabulary. This
        // can create problems when this field is attached to another
        // vocabulary. So vocabulary_field_add updates this type of field. Even
        // if other vocabularies with this attached field have more references,
        // previous references are always available. Another possibility may be
        // the use of a specific taxonomy_relation field for each vocabulary,
        // but it may be complicate to manage.
        'vid'    => $vocabulary->vid,
        'parent' => 0,
      ))),
    );
    $result = taxonomy_csv_vocabulary_field_add($vocabulary->machine_name, $relation_field);
  }

  return $vocabulary;
}

/**
 * Creates vocabulary by its name and returns vocabulary object.
 *
 * @param $name
 *   (Optional) Name of vocabulary to create.
 *
 * @return
 *   Created vocabulary object.
 */
function taxonomy_csv_vocabulary_create($name = '') {
  $name = _taxonomy_csv_vocabulary_name_create($name);

  // Create an empty vocabulary with default Drupal 7 fields.
  // Hierarchy is updated later.
  $vocabulary = (object) array(
    'name'         => $name,
    'machine_name' => taxonomy_csv_vocabulary_machine_name_create($name),
    'description'  => t('Vocabulary created automatically by Taxonomy csv import/export module'),
    'hierarchy'    => 2,
    'module'       => 'taxonomy',
    'weight'       => 0,
  );

  $result = taxonomy_vocabulary_save($vocabulary);

  return $vocabulary;
}

/**
 * Duplicates a vocabulary object. If not exist, creates an empty vocabulary.
 *
 * @todo To be finished.
 *
 * @param $vocabulary_id
 *   Vocabulary id to duplicate.
 *
 * @return
 *   Duplicated vocabulary object.
 */
function taxonomy_csv_vocabulary_duplicate($vocabulary_id) {
  $original_vocabulary = taxonomy_vocabulary_load($vocabulary_id);
  if ($original_vocabulary) {
    // Creates an unused name.
    // Check if name begins with 'Copy of #name' in order to serialize name.
    $name = t('Copy of [!vocabulary_name]', array('!vocabulary_name' => $original_vocabulary->name));
    $name = _taxonomy_csv_vocabulary_name_create((strpos($original_vocabulary->name, $name) === FALSE) ? $name : $original_vocabulary->name);

    // Duplicate original vocabulary. Relations and hierarchy are updated later.
    $duplicated_vocabulary = (object) array(
      'name'         => $name,
      'machine_name' => taxonomy_csv_vocabulary_machine_name_create($name),
      'description'  => $original_vocabulary->description,
      'hierarchy'    => $original_vocabulary->hierarchy,
      'module'       => $original_vocabulary->module,
      'weight'       => $original_vocabulary->weight,
    );

    $result = taxonomy_vocabulary_save($duplicated_vocabulary);

    // Add specific fields.
    $fields = field_info_instances('taxonomy_term', $original_vocabulary->machine_name);
    foreach ($fields as $field) {
      taxonomy_csv_vocabulary_field_add($duplicated_vocabulary->machine_name, $field);
    }

    // Get all terms and attributes of original vocabulary and copy them in the
    // new one in three steps.
    $original_terms = taxonomy_term_load_multiple(
      array(),
      array('vid' => $original_vocabulary->vid)
    );

    // First step: copy each term except term reference fields (parents...).
    $duplicated_terms = array();
    foreach ($original_terms as $original_term) {
      // Check needed when vocabulary allows multiple parents.
      if (!isset($duplicated_terms[$original_term->tid])) {
        // @todo Add other non term reference fields (text...).
        $duplicated_terms[$original_term->tid] = (object) array(
          'vid'         => $duplicated_vocabulary->vid,
          'name'        => $original_term->name,
          'description' => $original_term->description,
          'format'      => $original_term->format,
          'weight'      => $original_term->weight,
        );
        $result = taxonomy_term_save($duplicated_terms[$original_term->tid]);
      }
    }

    // Second step: update duplicated terms with parents.
    foreach ($duplicated_terms as $original_tid => $duplicated_term) {
      $original_parents = taxonomy_get_parents($original_tid);
      if (count($original_parents) > 0) {
        foreach ($original_parents as $original_parent_term_id => $original_parent_term) {
          $duplicated_terms[$original_tid]->parent[] = $duplicated_terms[$original_parent_term_id]->tid;
        }
        $result = taxonomy_term_save($duplicated_terms[$original_tid]);
      }
    }

    // Third step: update duplicated terms with taxonomy_term_reference fields.
    // @todo Finish this step.

    return $duplicated_vocabulary;
  }

  return taxonomy_csv_vocabulary_create();
}

/**
 * Helper to create an unused vocabulary name from a string.
 */
function _taxonomy_csv_vocabulary_name_create($name = '') {
  $name = preg_replace('/.csv$/', '', trim(basename(strval($name))));
  $name = (drupal_strlen($name) == 0) ?
    t('Auto created vocabulary') :
    // Limit to 250 characters.
    drupal_substr($name, 0, 250);

  // Invent a unused vocabulary name.
  if (taxonomy_csv_vocabulary_name_check($name)
      || taxonomy_csv_vocabulary_machine_name_check(taxonomy_csv_vocabulary_machine_name_create($name))) {
    for (
      $i = 2;
      (taxonomy_csv_vocabulary_name_check("$name $i"))
      || taxonomy_csv_vocabulary_machine_name_check(taxonomy_csv_vocabulary_machine_name_create("$name $i"));
      $i++) {
    }
    $name = "$name $i";
  }

  return $name;
}

/**
 * Creates a machine name from a string.
 *
 * The name is created by replacing non alphanumeric character by an underscore.
 * Machine name is defined as first 16 cleaned characters of name and a random
 * five characters serial. Fields module prepends 'taxonomy_' to name and check
 * if total lenght is 21 characters max.
 *
 * @param $name
 *   The string to process.
 *
 * @return
 *   The processed string.
 */
function taxonomy_csv_vocabulary_machine_name_create($name) {
  return drupal_substr(preg_replace('/_+/i', '_', preg_replace('/[^a-z0-9\\_]/i', '_', drupal_strtolower(trim(strval($name))))), 0, 16) . strval(rand(10000, 99999));
}

/**
 * Checks if a name is a vocabulary machine_name.
 */
function taxonomy_csv_vocabulary_machine_name_check($name) {
  return (taxonomy_vocabulary_machine_name_load($name) != FALSE);
}

/**
 * Check if a name is a vocabulary name.
 */
function taxonomy_csv_vocabulary_name_check($name) {
  return (taxonomy_vocabulary_load_multiple(FALSE, array('name' => $name)) != FALSE);
}

/**
 * Return vocabulary vid by its name or NULL if none.
 */
function taxonomy_csv_vocabulary_get_id($name) {
  return (key(taxonomy_vocabulary_load_multiple(FALSE, array('name' => $name))));
}

/**
 * Return the vocabulary object matching a vocabulary name.
 */
function taxonomy_csv_vocabulary_load_name($name) {
  return reset(taxonomy_vocabulary_load_multiple(NULL, array('name' => $name)));
}

/**
 * Check and update hierarchy flag of a given vocabulary.
 *
 * Drupal hierarchy check function bugs with vocabularies without hierarchy.
 *
 * @param $vocabulary
 *   The vocabulary object to check and to update.
 * @param $changed_term
 *   (Optional) Useless. Kept for compatibility with taxonomy D7 function.
 * @return
 *   Updated hierarchy level.
 */
function _taxonomy_csv_check_vocabulary_hierarchy($vocabulary, $changed_term = '') {
  $tree = taxonomy_get_tree($vocabulary->vid);
  $hierarchy = 0;
  foreach ($tree as $term) {
    // Check this term's parent count.
    if (count($term->parents) > 1) {
      $hierarchy = 2;
      break;
    }
    // taxonomy.module l. 464 can't detect level 0 with '0 !== array_shift'.
    elseif (count($term->parents) == 1 && 0 != array_shift($term->parents)) {
      $hierarchy = 1;
    }
  }
  if ($hierarchy != $vocabulary->hierarchy) {
    $vocabulary->hierarchy = $hierarchy;
    taxonomy_vocabulary_save($vocabulary);
  }

  return $hierarchy;
}

/**
 * Add a field to a vocabulary.
 *
 * @param $vocabulary_machine_name
 *   Vocabulary machine_name.
 * @param $field
 *   Field array to add.
 *
 * @return
 *   TRUE if success, FALSE else.
 */
function taxonomy_csv_vocabulary_field_add($vocabulary_machine_name, $field) {
  // Check if vocabulary exist.
  $vocabulary = taxonomy_vocabulary_machine_name_load($vocabulary_machine_name);
  if ($vocabulary) {
    // Check if field exists already and create or update it if not exist.
    // No other check is made.
    $prior_field = field_info_field($field['field_name']);

    // Create field.
    if (empty($prior_field)) {
      $field = field_create_field($field);
    }
    // Check if update is needed with taxonomy_term_reference. This is needed,
    // because this type requires to use specific vocabularies.
    elseif ($field['type'] == 'taxonomy_term_reference') {
      $flag = FALSE;
      foreach ($prior_field['settings']['allowed_values'] as $allowed_values) {
        // Don't add new allowed values if they exists already.
        if ($allowed_values == $field['settings']['allowed_values'][0]) {
          $flag = TRUE;
          break;
        }
      }
      if (!$flag) {
        $prior_field['settings']['allowed_values'][] = $field['settings']['allowed_values'][0];
        $result = field_update_field($prior_field);
      }
      $field = field_info_field($field['field_name']);
    }
    // Field is already created, so use it.
    else {
      $field = $prior_field;
    }

    // Check if field is already instanced to vocabulary so attach it if needed.
    $prior_instance = field_info_instance('taxonomy_term', $field['field_name'], $vocabulary->machine_name);
    if (empty($prior_instance)) {
      $result = field_create_instance(array(
        'field_name'  => $field['field_name'],
        'entity_type' => 'taxonomy_term',
        'bundle'      => $vocabulary->machine_name,
        'label'       => $field['label'],
        'description' => $field['description'],
      ));
    }
    return TRUE;
  }
  return FALSE;
}
