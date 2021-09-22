<?php

/**
 * @file
 * Provides Entity metadata integration.
 */

/**
 * Extend the defaults.
 */
class OgMetadataController extends EntityDefaultMetadataController {

  public function entityPropertyInfo() {
    $info = parent::entityPropertyInfo();
    $properties = &$info[$this->type]['properties'];

    // Initialize values
    $properties = !empty($properties) ? $properties : array();
    $properties += array(
      'entity_type' => array(),
    );

    $properties['entity'] = array(
      'label' => t('Entity'),
      'type' => 'entity',
      'description' => t('The entity that is a group.'),
      'getter callback' => 'og_entity_getter',
      'setter permission' => 'administer group',
      'required' => TRUE,
    );

    $properties['entity_type'] += array(
      'required' => TRUE,
      'options list' => 'og_get_all_group_entity',
    );

    $properties['created']['type'] = 'date';

    $properties['state']['type'] = 'integer';
    $properties['state'] += array(
      'setter callback' => 'entity_property_verbatim_set',
      'options list' => 'og_group_states',
      'required' => TRUE,
    );

    // Custom properties.
    $properties['members'] = array(
      'label' => t("Members"),
      'type' => 'list<user>',
      'description' => t("A list of members of the group, including active, pending and blocked memberships."),
      'getter callback' => 'og_get_properties',
    );

    $properties['manager'] = array(
      'label' => t("Manager"),
      'type' => 'user',
      'description' => t("The user account of the group manager."),
      'getter callback' => 'og_get_properties',
    );

    $properties['url'] = array(
      'label' => t('URL'),
      'type' => 'uri',
      'description' => t("The URL to the entity that is a group."),
      'getter callback' => 'og_get_properties',
    );

    return $info;
  }
}

/**
 * Extend the defaults.
 */
class OgMembershipMetadataController extends EntityDefaultMetadataController {

  public function entityPropertyInfo() {
    $info = parent::entityPropertyInfo();
    $properties = &$info[$this->type]['properties'];

    // Initialize values
    $properties = !empty($properties) ? $properties : array();
    $properties += array(
      'entity_type' => array(),
    );

    // Group ID is provided in the "group" property.
    unset($properties['gid']);
    $properties['group'] = array(
      'label' => t("OG group"),
      'type' => 'group',
      'description' => t("The OG group associated with the OG membership."),
      'required' => TRUE,
      'getter callback' => 'entity_property_getter_method',
      'schema field' => 'gid',
    );

    // Entity id is provided via the "entity" property.
    unset($properties['etid']);

    $properties['entity'] = array(
      'label' => t('Entity'),
      'type' => 'entity',
      'description' => t('The entity that is a group content.'),
      'getter callback' => 'og_entity_getter',
      'setter callback' => 'og_entity_setter',
      'setter permission' => 'administer group',
      'required' => TRUE,
    );

    $properties['created']['type'] = 'date';
    $properties['created'] += array(
      'setter callback' => 'entity_property_verbatim_set',
    );

    $properties['state']['type'] = 'integer';
    $properties['state'] += array(
      'setter callback' => 'entity_property_verbatim_set',
      'options list' => 'og_group_content_states',
      'required' => TRUE,
    );

    $properties['entity_type'] += array(
      'setter callback' => 'entity_property_verbatim_set',
    );

    $properties['type']['type'] = 'og_membership_type';
    $properties['type'] += array(
      'required' => TRUE,
      'setter callback' => 'entity_property_verbatim_set',
    );

    return $info;
  }
}