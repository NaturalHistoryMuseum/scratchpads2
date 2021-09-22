<?php

/**
 * @file
 * Provides Entity metadata integration.
 */

/**
 * Extend the defaults.
 */
class Profile2MetadataController extends EntityDefaultMetadataController {

  public function entityPropertyInfo() {
    $info = parent::entityPropertyInfo();
    $properties = &$info[$this->type]['properties'];

    $properties['label'] = array(
      'label' => t('Label'),
      'description' => t('The profile label'),
      'getter callback' => 'entity_property_getter_method',
    );

    $properties['type'] = array(
      'type' => 'profile2_type',
      'getter callback' => 'entity_property_getter_method',
      'setter callback' => 'entity_property_verbatim_set',
      'setter permission' => 'administer profiles',
      'required' => TRUE,
      'description' => t('The profile type.'),
    ) + $properties['type'];

    unset($properties['uid']);

    $properties['user'] = array(
      'label' => t("User"),
      'type' => 'user',
      'description' => t("The owner of the profile."),
      'getter callback' => 'entity_property_getter_method',
      'setter callback' => 'entity_property_setter_method',
      'setter permission' => 'administer profiles',
      'required' => TRUE,
      'schema field' => 'uid',
    );
    return $info;
  }
}

/**
 * Implements hook_entity_property_info_alter().
 */
function profile2_entity_property_info_alter(&$info) {
  // Add related profiles to the user object.
  $properties = &$info['user']['properties'];
  foreach (profile2_get_types() as $type_name => $type) {
    $properties['profile_' . $type_name] = array(
      'type' => 'profile2',
      'label' => t('@type_name profile', array('@type_name' => drupal_ucfirst($type->label))),
      'description' => t("The users's @type_name profile.", array('@type_name' => $type->label)),
      'getter callback' => 'profile2_user_get_properties',
      'bundle' => $type_name,
    );
  }
}
