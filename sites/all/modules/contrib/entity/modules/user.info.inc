<?php

/**
 * @file
 * Provides info about the user entity.
 */

/**
 * Implements hook_entity_property_info() on top of user module.
 * @see entity_entity_property_info()
 */
function entity_metadata_user_entity_property_info() {
  $info = array();
  // Add meta-data about the user properties.
  $properties = &$info['user']['properties'];

  $properties['uid'] = array(
    'label' => t("User ID"),
    'type' => 'integer',
    'description' => t("The unique ID of the user account."),
    'schema field' => 'uid',
  );
  $properties['name'] = array(
    'label' => t("Name"),
    'description' => t("The login name of the user account."),
    'getter callback' => 'entity_metadata_user_get_properties',
    'setter callback' => 'entity_property_verbatim_set',
    'sanitize' => 'filter_xss',
    'required' => TRUE,
    'access callback' => 'entity_metadata_user_properties_access',
    'schema field' => 'name',
  );
  $properties['mail'] = array(
    'label' => t("Email"),
    'description' => t("The email address of the user account."),
    'setter callback' => 'entity_property_verbatim_set',
    'validation callback' => 'valid_email_address',
    'required' => TRUE,
    'access callback' => 'entity_metadata_user_properties_access',
    'schema field' => 'mail',
  );
  $properties['url'] = array(
    'label' => t("URL"),
    'description' => t("The URL of the account profile page."),
    'getter callback' => 'entity_metadata_user_get_properties',
    'type' => 'uri',
  );
  $properties['edit_url'] = array(
    'label' => t("Edit URL"),
    'description' => t("The url of the account edit page."),
    'getter callback' => 'entity_metadata_user_get_properties',
    'type' => 'uri',
  );
  $properties['last_login'] = array(
    'label' => t("Last login"),
    'description' => t("The date the user last logged in to the site."),
    'getter callback' => 'entity_metadata_user_get_properties',
    'type' => 'date',
    'schema field' => 'login',
  );
  $properties['created'] = array(
    'label' => t("Created"),
    'description' => t("The date the user account was created."),
    'type' => 'date',
    'schema field' => 'created',
  );
  $properties['roles'] = array(
    'label' => t("User roles"),
    'description' => t("The roles of the user."),
    'type' => 'list<integer>',
    'getter callback' => 'entity_metadata_user_get_properties',
    'setter callback' => 'entity_metadata_user_set_properties',
    'setter permission' => 'administer users',
    'options list' => 'entity_metadata_user_roles',
    'access callback' => 'entity_metadata_user_properties_access',
  );
  return $info;
}

