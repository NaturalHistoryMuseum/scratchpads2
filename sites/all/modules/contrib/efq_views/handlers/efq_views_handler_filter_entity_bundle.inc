<?php
// $Id$

/**
 * Filter based on entity bundle.
 */
class efq_views_handler_filter_entity_bundle extends efq_views_handler_filter_entity_in_operator {
  function get_value_options() {
    if (!isset($this->value_options)) {
      $bundles = field_info_bundles($this->definition['entity_type']);
      $this->value_title = t('Bundle');

      // EFQ: Mixed, display bundles for all entity types
      if (!isset($this->definition['entity_type'])) {
        foreach ($bundles as $entity_type => $entity_bundles) {
          foreach ($entity_bundles as $bundle => $info) {
            $label = isset($info['label']) ? $info['label'] : $bundle;
            $options[$bundle] = $label;
          }
        }
      }
      else { // Display bundles for the selected entity type only.
        foreach ($bundles as $bundle => $info) {
          $label = isset($info['label']) ? $info['label'] : $bundle;
          $options[$bundle] = $label;
        }
      }

      $this->value_options = $options;
    }
  }
}
