<?php

/**
 * @file
 * Contains FeedsUserProcessor.
 */

/**
 * Option to block users not found in the feed.
 *
 * @var string
 */
define('FEEDS_BLOCK_NON_EXISTENT', 'block');

/**
 * Feeds processor plugin. Create users from feed items.
 */
class FeedsUserProcessor extends FeedsProcessor {
  /**
   * Search by role name.
   */
  const ROLE_SEARCH_NAME = 'name';

  /**
   * Search by role id.
   */
  const ROLE_SEARCH_RID = 'rid';

  /**
   * Unencrypted password.
   */
  const PASS_UNENCRYPTED = 'none';

  /**
   * MD5 encrypted password.
   */
  const PASS_MD5 = 'md5';

  /**
   * SHA512 encrypted password.
   */
  const PASS_SHA512 = 'sha512';

  /**
   * Define entity type.
   */
  public function entityType() {
    return 'user';
  }

  /**
   * Implements parent::entityInfo().
   */
  protected function entityInfo() {
    $info = parent::entityInfo();
    $info['label plural'] = t('Users');
    return $info;
  }

  /**
   * Creates a new user account in memory and returns it.
   */
  protected function newEntity(FeedsSource $source) {
    $account = parent::newEntity($source);
    $account->uid = 0;
    $account->roles = array_filter($this->config['roles']);
    $account->status = $this->config['status'];
    $account->is_new = TRUE;

    return $account;
  }

  /**
   * Loads an existing user.
   */
  protected function entityLoad(FeedsSource $source, $uid) {
    $user = parent::entityLoad($source, $uid);

    // Copy the password so that we can compare it again at save.
    $user->feeds_original_pass = $user->pass;

    // Reset roles and status when an user is replaced.
    if ($this->config['update_existing'] == FEEDS_REPLACE_EXISTING) {
      $user->roles = array_filter($this->config['roles']);
      $user->status = $this->config['status'];

      // Unserialize user data if it is still serialized.
      if (!empty($user->data) && @unserialize($user->data)) {
        $user->data = unserialize($user->data);
      }
      else {
        $user->data = array();
      }
    }

    return $user;
  }

  /**
   * Validates a user account.
   */
  protected function entityValidate($account, FeedsSource $source = NULL) {
    parent::entityValidate($account);

    if (empty($account->name) || empty($account->mail) || !valid_email_address($account->mail)) {
      throw new FeedsValidationException(t('User name missing or email not valid.'));
    }

    // Check when an user ID gets set or changed during processing if that user
    // ID is not already in use.
    $uid_is_set = FALSE;
    if (!empty($account->uid)) {
      $is_new = !empty($account->feeds_item->is_new);
      $different = !empty($account->feeds_item->entity_id) && $account->feeds_item->entity_id != $account->uid;
      if ($is_new || $different) {
        // Flag that an user ID is explicitly set. This will skip the
        // validations for existing user name and mail address defined below.
        $uid_is_set = TRUE;

        // Check if the given user ID already exists.
        $exists = entity_load_unchanged('user', $account->uid);
        if ($exists) {
          throw new FeedsValidationException(t('Could not update user ID to @uid since that ID is already in use.', array(
            '@uid' => $account->uid,
          )));
        }
      }
    }

    // Look for other validation constraints.
    $errors = array();

    // Validate the user name.
    if ($error = user_validate_name($account->name)) {
      $errors[] = $error;
    }

    // Check for an existing user name, unless an user ID is explicitly set.
    if (!$uid_is_set && (bool) db_select('users')->fields('users', array('uid'))->condition('uid', $account->uid, '<>')->condition('name', db_like($account->name), 'LIKE')->range(0, 1)->execute()->fetchField()) {
      $errors[] = t("The name '@name' is already taken.", array('@name' => $account->name));
    }

    // Check for an existing mail address, unless an user ID is explicitly set.
    if (!$uid_is_set && (bool) db_select('users')->fields('users', array('uid'))->condition('uid', $account->uid, '<>')->condition('mail', db_like($account->mail), 'LIKE')->range(0, 1)->execute()->fetchField()) {
      $errors[] = t("The e-mail address '@email' is already taken.", array('@email' => $account->mail));
    }

    // Timezone validation.
    if (!empty($account->timezone) && !array_key_exists($account->timezone, system_time_zones())) {
      $errors[] = t("Failed importing '@name'. User's timezone is not valid.", array('@name' => $account->name));
    }

    if (!empty($errors)) {
      throw new FeedsValidationException(implode("\n", $errors));
    }
  }

  /**
   * Save a user account.
   */
  protected function entitySave($account) {
    if ($this->config['defuse_mail']) {
      $account->mail = $account->mail . '_test';
    }

    $edit = (array) $account;

    // Remove pass from $edit if the password is unchanged.
    if (isset($account->feeds_original_pass) && $account->pass == $account->feeds_original_pass) {
      unset($edit['pass']);
    }

    // Check if the user ID changed when updating users.
    if (!empty($account->feeds_item->entity_id) && $account->feeds_item->entity_id != $account->uid) {
      // The user ID of the existing user is different. Try to update the user ID.
      db_update('users')
        ->fields(array(
          'uid' => $account->uid,
        ))
        ->condition('uid', $account->feeds_item->entity_id)
        ->execute();
    }

    user_save($account, $edit);

    // If an encrypted password was given, directly set this in the database.
    if ($account->uid && !empty($account->pass_crypted)) {
      db_update('users')
        ->fields(array('pass' => $account->pass_crypted))
        ->condition('uid', $account->uid)
        ->execute();
    }

    if ($account->uid && !empty($account->openid)) {
      $authmap = array(
        'uid' => $account->uid,
        'module' => 'openid',
        'authname' => $account->openid,
      );
      if (SAVED_UPDATED != drupal_write_record('authmap', $authmap, array('uid', 'module'))) {
        drupal_write_record('authmap', $authmap);
      }
    }
  }

  /**
   * Delete multiple user accounts.
   */
  protected function entityDeleteMultiple($uids) {
    // Prevent user 1 from being deleted.
    if (in_array(1, $uids)) {
      $uids = array_diff($uids, array(1));

      // But do delete the associated feeds item.
      db_delete('feeds_item')
        ->condition('entity_type', $this->entityType())
        ->condition('entity_id', 1)
        ->execute();
    }
    user_delete_multiple($uids);
  }

  /**
   * Override parent::configDefaults().
   */
  public function configDefaults() {
    return array(
      'roles' => array(),
      'status' => 1,
      'defuse_mail' => FALSE,
    ) + parent::configDefaults();
  }

  /**
   * Override parent::configForm().
   */
  public function configForm(&$form_state) {
    $form = parent::configForm($form_state);
    $form['status'] = array(
      '#type' => 'radios',
      '#title' => t('Status'),
      '#description' => t('Select whether users should be imported active or blocked.'),
      '#options' => array(0 => t('Blocked'), 1 => t('Active')),
      '#default_value' => $this->config['status'],
    );

    $roles = user_roles(TRUE);
    unset($roles[2]);
    if (count($roles)) {
      $form['roles'] = array(
        '#type' => 'checkboxes',
        '#title' => t('Additional roles'),
        '#description' => t('Every user is assigned the "authenticated user" role. Select additional roles here.'),
        '#default_value' => $this->config['roles'],
        '#options' => $roles,
      );
    }
    $form['defuse_mail'] = array(
      '#type' => 'checkbox',
      '#title' => t('Defuse e-mail addresses'),
      '#description' => t('This appends _test to all imported e-mail addresses to ensure they cannot be used as recipients.'),
      '#default_value' => $this->config['defuse_mail'],
    );
    $form['update_non_existent']['#options'][FEEDS_BLOCK_NON_EXISTENT] = t('Block non-existent users');
    return $form;
  }

  /**
   * Overrides FeedsProcessor::map().
   *
   * Ensures that the user is assigned additional roles that are configured on
   * the settings. The roles could have been revoked when there was mapped to
   * the "roles_list" target.
   */
  protected function map(FeedsSource $source, FeedsParserResult $result, $target_item = NULL) {
    $target_item = parent::map($source, $result, $target_item);

    // Assign additional roles as configured.
    $roles = array_filter($this->config['roles']);
    foreach ($roles as $rid) {
      $target_item->roles[$rid] = $rid;
    }

    return $target_item;
  }

  /**
   * Overrides setTargetElement() to operate on a target item that is an user.
   */
  public function setTargetElement(FeedsSource $source, $target_user, $target_element, $value, array $mapping = array()) {
    switch ($target_element) {
      case 'pass':
        $this->setPassTarget($source, $target_user, $target_element, $value, $mapping);
        break;

      case 'created':
        $target_user->created = feeds_to_unixtime($value, REQUEST_TIME);
        break;

      case 'language':
        $target_user->language = strtolower($value);
        break;

      case 'roles_list':
        // Ensure that the role list is an array.
        $value = (array) $value;
        $this->rolesListSetTarget($source, $target_user, $target_element, $value, $mapping);
        break;

      case 'timezone':
        $target_user->timezone = $value;
        break;

      default:
        parent::setTargetElement($source, $target_user, $target_element, $value);
        break;
    }
  }

  /**
   * Return available mapping targets.
   */
  public function getMappingTargets() {
    $targets = parent::getMappingTargets();
    $targets += array(
      'uid' => array(
        'name' => t('User ID'),
        'description' => t('The uid of the user. NOTE: use this feature with care, user ids are usually assigned by Drupal.'),
        'optional_unique' => TRUE,
      ),
      'name' => array(
        'name' => t('User name'),
        'description' => t('Name of the user.'),
        'optional_unique' => TRUE,
      ),
      'mail' => array(
        'name' => t('Email address'),
        'description' => t('Email address of the user.'),
        'optional_unique' => TRUE,
      ),
      'created' => array(
        'name' => t('Created date'),
        'description' => t('The created (e. g. joined) data of the user.'),
      ),
      'pass' => array(
        'name' => t('Password'),
        'description' => t('The user password.'),
        'summary_callbacks' => array(array($this, 'passSummaryCallback')),
        'form_callbacks' => array(array($this, 'passFormCallback')),
      ),
      'status' => array(
        'name' => t('Account status'),
        'description' => t('Whether a user is active or not. 1 stands for active, 0 for blocked.'),
      ),
      'language' => array(
        'name' => t('User language'),
        'description' => t('Default language for the user.'),
      ),
      'timezone' => array(
        'name' => t('Timezone'),
        'description' => t('The timezone identifier, like UTC or Europe/Lisbon.'),
      ),
      'roles_list' => array(
        'name' => t('User roles'),
        'description' => t('User roles provided as role names in comma separated list.'),
        'summary_callbacks' => array(array($this, 'rolesListSummaryCallback')),
        'form_callbacks' => array(array($this, 'rolesListFormCallback')),
      ),
    );
    if (module_exists('openid')) {
      $targets['openid'] = array(
        'name' => t('OpenID identifier'),
        'description' => t('The OpenID identifier of the user. <strong>CAUTION:</strong> Use only for migration purposes, misconfiguration of the OpenID identifier can lead to severe security breaches like users gaining access to accounts other than their own.'),
        'optional_unique' => TRUE,
      );
    }

    $this->getHookTargets($targets);

    return $targets;
  }

  /**
   * Get id of an existing feed item term if available.
   */
  protected function existingEntityId(FeedsSource $source, FeedsParserResult $result) {
    if ($uid = parent::existingEntityId($source, $result)) {
      return $uid;
    }

    // Iterate through all unique targets and try to find a user for the
    // target's value.
    foreach ($this->uniqueTargets($source, $result) as $target => $value) {
      switch ($target) {
        case 'uid':
          $uid = db_query("SELECT uid FROM {users} WHERE uid = :uid", array(':uid' => $value))->fetchField();
          break;

        case 'name':
          $uid = db_query("SELECT uid FROM {users} WHERE name = :name", array(':name' => $value))->fetchField();
          break;

        case 'mail':
          $uid = db_query("SELECT uid FROM {users} WHERE mail = :mail", array(':mail' => $value))->fetchField();
          break;

        case 'openid':
          $uid = db_query("SELECT uid FROM {authmap} WHERE authname = :authname AND module = 'openid'", array(':authname' => $value))->fetchField();
          break;
      }
      if ($uid) {
        // Return with the first nid found.
        return $uid;
      }
    }
    return 0;
  }

  /**
   * Overrides FeedsProcessor::initEntitiesToBeRemoved().
   *
   * Removes user 1 from the list of entities to be removed.
   */
  protected function initEntitiesToBeRemoved(FeedsSource $source, FeedsState $state) {
    parent::initEntitiesToBeRemoved($source, $state);

    // Prevent user 1 from being deleted.
    unset($state->removeList[1]);
  }

  /**
   * Overrides FeedsProcessor::clean().
   *
   * Block users instead of deleting them.
   *
   * @param FeedsState $state
   *   The FeedsState object for the given stage.
   */
  protected function clean(FeedsState $state) {
    // Delegate to parent if not blocking or option not set.
    if (!isset($this->config['update_non_existent']) || $this->config['update_non_existent'] !== FEEDS_BLOCK_NON_EXISTENT) {
      return parent::clean($state);
    }

    if (!empty($state->removeList)) {
      // @see user_user_operations_block().
      // The following foreach is copied from above function but with an added
      // counter to count blocked users.
      foreach (user_load_multiple($state->removeList) as $account) {
        $this->loadItemInfo($account);
        $account->feeds_item->hash = $this->config['update_non_existent'];
        // For efficiency manually save the original account before applying any
        // changes.
        $account->original = clone $account;
        user_save($account, array('status' => 0));
        $state->blocked++;
      }
    }
  }

  /**
   * Returns default values for mapper "roles_list".
   */
  public function rolesListDefaults() {
    $roles = user_roles(TRUE);
    unset($roles[DRUPAL_AUTHENTICATED_RID]);
    $rids = array_keys($roles);
    $rids = array_combine($rids, $rids);
    return array(
      'role_search' => self::ROLE_SEARCH_NAME,
      'allowed_roles' => $rids,
      'autocreate' => 0,
      'revoke_roles' => 1,
    );
  }

  /**
   * Mapping configuration summary callback for target "roles_list".
   */
  public function rolesListSummaryCallback(array $mapping, $target, array $form, array $form_state) {
    $options = array();

    // Add in defaults.
    $defaults = $this->rolesListDefaults();
    $mapping += $defaults;
    $mapping['allowed_roles'] += $defaults['allowed_roles'];

    // Role search.
    $role_search_options = $this->rolesListRoleSearchOptions();
    $options[] = t('Search roles by: <strong>@search</strong>', array('@search' => $role_search_options[$mapping['role_search']]));

    // Allowed roles.
    $role_names = array();
    $roles = user_roles(TRUE);
    foreach (array_filter($mapping['allowed_roles']) as $rid => $enabled) {
      $role_names[] = $roles[$rid];
    }

    if (empty($role_names)) {
      $role_names = array('<' . t('none') . '>');
    }
    $options[] = t('Allowed roles: %roles', array('%roles' => implode(', ', $role_names)));

    // Autocreate.
    if ($mapping['autocreate']) {
      $options[] = t('Automatically create roles');
    }
    else {
      $options[] = t('Only assign existing roles');
    }

    // Revoke roles.
    if ($mapping['revoke_roles']) {
      $options[] = t('Revoke roles: yes');
    }
    else {
      $options[] = t('Revoke roles: no');
    }

    return implode('<br />', $options);
  }

  /**
   * Mapping configuration form callback for target "roles_list".
   */
  public function rolesListFormCallback(array $mapping, $target, array $form, array $form_state) {
    // Add in defaults.
    $defaults = $this->rolesListDefaults();
    $mapping += $defaults;
    $mapping['allowed_roles'] += $defaults['allowed_roles'];

    $allowed_roles_options = user_roles(TRUE);
    unset($allowed_roles_options[DRUPAL_AUTHENTICATED_RID]);

    return array(
      'role_search' => array(
        '#type' => 'select',
        '#title' => t('Search roles by'),
        '#options' => $this->rolesListRoleSearchOptions(),
        '#default_value' => $mapping['role_search'],
      ),
      'allowed_roles' => array(
        '#type' => 'checkboxes',
        '#title' => t('Allowed roles'),
        '#options' => $allowed_roles_options,
        '#default_value' => $mapping['allowed_roles'],
        '#description' => t('Select the roles to accept from the feed.<br />Any other roles will be ignored.'),
      ),
      'autocreate' => array(
        '#type' => 'checkbox',
        '#title' => t('Auto create'),
        '#description' => t("Create the role if it doesn't exist."),
        '#default_value' => $mapping['autocreate'],
      ),
      'revoke_roles' => array(
        '#type' => 'checkbox',
        '#title' => t('Revoke roles'),
        '#description' => t('If enabled, roles that are not provided by the feed will be revoked for the user. This affects only the "Allowed roles" as configured above.'),
        '#default_value' => $mapping['revoke_roles'],
      ),
    );
  }

  /**
   * Returns role list options.
   */
  public function rolesListRoleSearchOptions() {
    return array(
      self::ROLE_SEARCH_NAME => t('Role name'),
      self::ROLE_SEARCH_RID => t('Role ID'),
    );
  }

  /**
   * Sets role target on the user entity.
   */
  public function rolesListSetTarget(FeedsSource $source, $entity, $target, array $values, array $mapping) {
    // Add in defaults.
    $defaults = $this->rolesListDefaults();
    $mapping += $defaults;
    $mapping['allowed_roles'] += $defaults['allowed_roles'];

    // Eventually revoke roles. Do not touch roles that are not allowed to set
    // by the source.
    if ($mapping['revoke_roles']) {
      foreach ($mapping['allowed_roles'] as $rid) {
        unset($entity->roles[$rid]);
      }
    }

    foreach ($values as $value) {
      $role = NULL;

      $value = trim($value);
      if (strlen($value) < 1) {
        // No role provided. Continue to the next role.
        continue;
      }

      switch ($mapping['role_search']) {
        case self::ROLE_SEARCH_NAME:
          $role = user_role_load_by_name($value);
          if (!$role && !empty($mapping['autocreate'])) {
            // Create new role if role doesn't exist.
            $role = new stdClass();
            $role->name = $value;
            user_role_save($role);
            $role = user_role_load_by_name($role->name);
          }
          break;

        case self::ROLE_SEARCH_RID:
          $role = user_role_load($value);
          break;
      }

      if ($role) {
        // Check if the role may be assigned.
        if (isset($mapping['allowed_roles'][$role->rid]) && !$mapping['allowed_roles'][$role->rid]) {
          // This role may *not* be assiged.
          continue;
        }
        $entity->roles[$role->rid] = $role->name;
      }
    }
  }

  /**
   * Mapping configuration summary callback for target "pass".
   */
  public function passSummaryCallback($mapping, $target, $form, $form_state) {
    $options = $this->passSummaryCallbackOptions();
    if (!isset($mapping['pass_encryption'])) {
      $mapping['pass_encryption'] = self::PASS_UNENCRYPTED;
    }
    return t('Password encryption: <strong>@encryption</strong>', array('@encryption' => $options[$mapping['pass_encryption']]));
  }

  /**
   * Returns the list of available password encryption methods.
   *
   * Used by ::passSummaryCallback().
   *
   * @return array
   *   An array of password encryption option titles.
   *
   * @see passSummaryCallback()
   */
  protected function passSummaryCallbackOptions() {
    return array(
      self::PASS_UNENCRYPTED => t('None'),
      self::PASS_MD5 => t('MD5'),
      self::PASS_SHA512 => t('SHA512'),
    );
  }

  /**
   * Mapping configuration form callback for target "pass".
   */
  public function passFormCallback($mapping, $target, $form, $form_state) {
    return array(
      'pass_encryption' => array(
        '#type' => 'select',
        '#title' => t('Password encryption'),
        '#options' => $this->passFormCallbackOptions(),
        '#default_value' => !empty($mapping['pass_encryption']) ? $mapping['pass_encryption'] : self::PASS_UNENCRYPTED,
      ),
    );
  }

  /**
   * Returns the list of available password encryption methods.
   *
   * Used by ::passFormCallback().
   *
   * @return array
   *   An array of password encryption option titles.
   *
   * @see passFormCallback()
   */
  protected function passFormCallbackOptions() {
    return array(
      self::PASS_UNENCRYPTED => t('Unencrypted'),
      self::PASS_MD5 => t('MD5 (used in older versions of Drupal)'),
      self::PASS_SHA512 => t('SHA512 (default in Drupal 7)'),
    );
  }

  /**
   * Sets the password on the user target.
   *
   * @see setTargetElement()
   */
  protected function setPassTarget($source, $target_user, $target_element, $value, $mapping) {
    if (empty($value)) {
      return;
    }
    if (!isset($mapping['pass_encryption'])) {
      $mapping['pass_encryption'] = self::PASS_UNENCRYPTED;
    }

    switch ($mapping['pass_encryption']) {
      case self::PASS_MD5:
        require_once DRUPAL_ROOT . '/' . variable_get('password_inc', 'includes/password.inc');
        $new_hash = user_hash_password($value);
        if ($new_hash) {
          // Indicate an updated password.
          $new_hash = 'U' . $new_hash;
          $target_user->pass = $target_user->pass_crypted = $new_hash;
        }
        break;

      case self::PASS_SHA512:
        $target_user->pass = $target_user->pass_crypted = $value;
        break;

      default:
        $target_user->pass = $value;
        break;
    }
  }

}
