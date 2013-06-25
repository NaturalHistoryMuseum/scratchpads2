<?php
/**
 * @file
 * scratchpad_relations.features.field.inc
 */

/**
 * Implements hook_field_default_fields().
 */
function scratchpad_relations_field_default_fields() {
  $fields = array();

  // Exported field: 'taxonomy_term-testclassification-field_ecological_relationship'
  $fields['taxonomy_term-testclassification-field_ecological_relationship'] = array(
    'field_config' => array(
      'active' => '1',
      'cardinality' => '-1',
      'deleted' => '0',
      'entity_types' => array(),
      'field_name' => 'field_ecological_relationship',
      'foreign keys' => array(),
      'indexes' => array(),
      'module' => 'relation_add',
      'settings' => array(),
      'translatable' => '0',
      'type' => 'relation_add',
    ),
    'field_instance' => array(
      'bundle' => 'testclassification',
      'deleted' => '0',
      'description' => '',
      'display' => array(
        'default' => array(
          'label' => 'above',
          'module' => 'scratchpad_relations',
          'settings' => array(),
          'type' => 'relation_add_endpoints_and_fields',
          'weight' => 1,
        ),
        'linked_node' => array(
          'label' => 'above',
          'settings' => array(),
          'type' => 'hidden',
          'weight' => 0,
        ),
      ),
      'entity_type' => 'taxonomy_term',
      'field_name' => 'field_ecological_relationship',
      'label' => 'Ecological relationship',
      'required' => 0,
      'settings' => array(
        'relation_type' => array(
          'dispersedSeedOf_seedsDispersedBy' => 'dispersedSeedOf_seedsDispersedBy',
          'dispersedSeedOf_seedsDispersedBy:reverse' => 'dispersedSeedOf_seedsDispersedBy:reverse',
          'flowerVisitedBy_visitedFlowerOf' => 'flowerVisitedBy_visitedFlowerOf',
          'flowerVisitedBy_visitedFlowerOf:reverse' => 'flowerVisitedBy_visitedFlowerOf:reverse',
          'host_parasite' => 'host_parasite',
          'host_parasite:reverse' => 'host_parasite:reverse',
          'nestedIn_usedAsNestBy' => 'nestedIn_usedAsNestBy',
          'nestedIn_usedAsNestBy:reverse' => 'nestedIn_usedAsNestBy:reverse',
          'pathogenOf_infectedBy' => 'pathogenOf_infectedBy',
          'pathogenOf_infectedBy:reverse' => 'pathogenOf_infectedBy:reverse',
          'preysUpon_preyedUponBy' => 'preysUpon_preyedUponBy',
          'preysUpon_preyedUponBy:reverse' => 'preysUpon_preyedUponBy:reverse',
          'resinExtractedBy_extractsResinFrom' => 'resinExtractedBy_extractsResinFrom',
          'resinExtractedBy_extractsResinFrom:reverse' => 'resinExtractedBy_extractsResinFrom:reverse',
        ),
        'user_register_form' => FALSE,
      ),
      'widget' => array(
        'active' => 1,
        'module' => 'relation_add',
        'settings' => array(
          'relation_endpoint_custom_label' => '',
          'relation_endpoint_label' => 'endpoint',
          'relation_endpoint_label_delta' => 0,
          'relation_endpoint_search_by_id' => 0,
        ),
        'type' => 'relation_add',
        'weight' => '32',
      ),
    ),
  );

  // Translatables
  // Included for use with string extractors like potx.
  t('Ecological relationship');

  return $fields;
}
