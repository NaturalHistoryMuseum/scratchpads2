<?php
/**
 * @file
 * Define Linkit term plugin class.
 */

class LinkitPluginTaxonomy_Term extends LinkitPluginEntity {
  function __construct($plugin, $profile) {
    /**
     * The term entity dont use the same column name as in the entity keys
     * bundle definition, so lets add it our self.
     */
    $this->entity_key_bundle = 'vid';
    parent::__construct($plugin, $profile);
  }

  /**
   * When "group_by_bundle" is active, we need to add the bundle name to the
   * group, else just return the entity label.
   *
   * @return a string with the group name.
   */
  function buildGroup($entity) {
    // Get the entity label.
    $group = $this->entity_info['label'];

    if (isset($this->conf['group_by_bundle']) && $this->conf['group_by_bundle']) {
      $bundles = $this->entity_info['bundles'];
      $bundle_name = $bundles[$entity->vocabulary_machine_name]['label'];
      $group .= ' · ' . check_plain($bundle_name);
    }
    return $group;
  }

  /**
   * Settings for the term plugin.
   */
  function buildSettingsForm() {
    $form = parent::buildSettingsForm();

    // The entity plugin uses the entity name for the #token_types, but terms
    // is a special case, its name is "Taxonomy_term" and the tokens are defined
    // (in the taxonomy module) with just "term".
    if (isset($form[$this->plugin['name']]['token_help']['help']['#token_types'])) {
      $form[$this->plugin['name']]['token_help']['help']['#token_types'] = array('term');
    }
    return $form;
  }

  /**
   * The autocomplete callback function for the Linkit Taxonomy term plugin.
   */
  function autocomplete_callback() {
    // The term entity dont use the entity keys bundle definition, its using the
    // vid instead, so lets 'translate' the bundle names to vids.
    if (isset($this->entity_key_bundle) && isset($this->conf['bundles']) ) {
      $bundles = array_filter($this->conf['bundles']);

      // Get all vocabularies.
      $vocabularies = taxonomy_vocabulary_get_names();
      // Temp storage for values.
      $tmp_bundles = array();
      foreach ($bundles as $bundle) {
        $tmp_bundles[] = $vocabularies[$bundle]->{$this->entity_key_bundle};
      }

      // Assign the new values as the bundles.
      $this->conf['bundles'] = $tmp_bundles;
    }
    // Call the parent.
    return parent::autocomplete_callback();
  }
}