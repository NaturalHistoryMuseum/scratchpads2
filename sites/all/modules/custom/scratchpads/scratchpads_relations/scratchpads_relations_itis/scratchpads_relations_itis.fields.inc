<?php

/**
 * Simply returns an array of groups
 */
function scratchpads_relations_itis_groups($vid){
  return array(
    array(
      'group_name' => 'group_ecological_relationship',
      'entity_type' => 'taxonomy_term',
      'bundle' => 'itis_vocabulary',
      'mode' => 'form',
      'parent_name' => 'group_vertical_tabs',
      'label' => 'Ecological relationships',
      'weight' => '15',
      'children' => array(
        0 => 'field_ecological_relationship'
      ),
      'format_type' => 'htab',
      'format_settings' => array(
        'formatter' => 'closed',
        'instance_settings' => array(
          'classes' => '',
          'required_fields' => 1
        )
      )
    )
  );
}

/**
 * Simply returns an array of fields
 */
function scratchpads_relations_itis_fields(){
  return array(
    array(
      'field_config' => array(
        'active' => '1',
        'cardinality' => '-1', // unlimited
        'deleted' => '0',
        'entity_types' => array(),
        'field_name' => 'field_ecological_relationship',
        'foreign keys' => array(),
        'indexes' => array(),
        'module' => 'relation_add',
        'settings' => array(),
        'translatable' => '0',
        'type' => 'relation_add'
      ),
      'field_instance' => array(
        'bundle' => 'tag',
        'deleted' => '0',
        'description' => '',
        'display' => array(
          'default' => array(
            'label' => 'inline',
            'settings' => array(),
            'type' => 'hidden',
            'weight' => '1'
          ),
          'linked_node' => array(
            'label' => 'above',
            'settings' => array(),
            'type' => 'hidden',
            'weight' => 0
          )
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
            'resinExtractedBy_extractsResinFrom:reverse' => 'resinExtractedBy_extractsResinFrom:reverse'
          ),
          'user_register_form' => FALSE
        ),
        'widget' => array(
          'active' => 1,
          'module' => 'relation_add',
          'settings' => array(
            'relation_endpoint_custom_label' => '',
            'relation_endpoint_label' => 'endpoint',
            'relation_endpoint_label_delta' => 0,
            'relation_endpoint_search_by_id' => 0
          ),
          'type' => 'relation_add',
          'weight' => '13'
        )
      )
    )
  );
  // Translatables
  // Included for use with string extractors like potx.
  t('Associated accepted name');
  t('Authors');
  t('Page number');
  t('Rank');
  t('Reference');
  t('Unacceptability Reason');
  t('Unit indicator 1');
  t('Unit indicator 2');
  t('Unit indicator 3');
  t('Unit indicator 4');
  t('Unit name 1');
  t('Unit name 2');
  t('Unit name 3');
  t('Unit name 4');
  t('Usage');
  t('Vernacular names');
  t('Ecological relationship');
}
  