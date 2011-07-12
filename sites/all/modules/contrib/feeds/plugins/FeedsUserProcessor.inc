<?php

/**
 * @file
 * FeedsUserProcessor class.
 */

/**
 * Feeds processor plugin. Create users from feed items.
 */
class FeedsUserProcessor extends FeedsProcessor {

  /**
   * Implementation of FeedsProcessor::process().
   */
  public function process(FeedsImportBatch $batch, FeedsSource $source) {

    // Count number of created and updated nodes.
    $created  = $updated = $failed = 0;

    while ($item = $batch->shiftItem()) {

      if (!($uid = $this->existingItemId($batch, $source)) || $this->config['update_existing']) {

        // Map item to a term.
        $account = $this->map($batch);

        // Check if user name and mail are set, otherwise continue.
        if (empty($account->name) || empty($account->mail) || !valid_email_address($account->mail)) {
          $failed++;
          continue;
        }

        // Add term id if available.
        if (!empty($uid)) {
          $account->uid = $uid;
        }

        // Save the user.
        user_save($account, (array) $account);
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

        if ($uid) {
          $updated++;
        }
        else {
          $created++;
        }
      }
    }

    // Set messages.
    if ($failed) {
      drupal_set_message(
        format_plural(
          $failed,
          'There was @number user that could not be imported because either their name or their email was empty or not valid. Check import data and mapping settings on User processor.',
          'There were @number users that could not be imported because either their name or their email was empty or not valid. Check import data and mapping settings on User processor.',
          array('@number' => $failed)
        ),
        'error'
      );
    }
    if ($created) {
      drupal_set_message(format_plural($created, 'Created @number user.', 'Created @number users.', array('@number' => $created)));
    }
    elseif ($updated) {
      drupal_set_message(format_plural($updated, 'Updated @number user.', 'Updated @number users.', array('@number' => $updated)));
    }
    else {
      drupal_set_message(t('There are no new users.'));
    }
  }

  /**
   * Implementation of FeedsProcessor::clear().
   */
  public function clear(FeedsBatch $batch, FeedsSource $source) {
    // Do not support deleting users as we have no way of knowing which ones we
    // imported.
    throw new Exception(t('User processor does not support deleting users.'));
  }

  /**
   * Execute mapping on an item.
   */
  protected function map(FeedsImportBatch $batch, $target_account = NULL) {
    // Prepare user account object.
    if (empty($target_account)) {
      $target_account = new stdClass();
    }

    $target_account->uid = 0;
    $target_account->roles = array_filter($this->config['roles']);
    $target_account->status = $this->config['status'];

    // Have parent class do the iterating.
    return parent::map($batch, $target_account);
  }

  /**
   * Override parent::configDefaults().
   */
  public function configDefaults() {
    return array(
      'roles' => array(),
      'update_existing' => FALSE,
      'status' => 1,
      'mappings' => array(),
    );
  }

  /**
   * Override parent::configForm().
   */
  public function configForm(&$form_state) {
    $form = array();

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
    // @todo Implement true updating.
    $form['update_existing'] = array(
      '#type' => 'checkbox',
      '#title' => t('Replace existing users'),
      '#description' => t('If an existing user is found for an imported user, replace it. Existing users will be determined using mappings that are a "unique target".'),
      '#default_value' => $this->config['update_existing'],
    );
    return $form;
  }

  /**
   * Set target element.
   */
  public function setTargetElement(&$target_item, $target_element, $value) {
    $target_item->$target_element = $value;
  }

  /**
   * Return available mapping targets.
   */
  public function getMappingTargets() {
    $targets = array(
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
        'name' => t('Unencrypted Password'),
        'description' => t('The unencrypted user password.'),
      ),
    );
    if (module_exists('openid')) {
      $targets['openid'] = array(
        'name' => t('OpenID identifier'),
        'description' => t('The OpenID identifier of the user. <strong>CAUTION:</strong> Use only for migration purposes, misconfiguration of the OpenID identifier can lead to severe security breaches like users gaining access to accounts other than their own.'),
        'optional_unique' => TRUE,
       );
    }

    // Let other modules expose mapping targets.
    self::loadMappers();
    drupal_alter('feeds_user_processor_targets', $targets);

    return $targets;
  }

  /**
   * Get id of an existing feed item term if available.
   */
  protected function existingItemId(FeedsImportBatch $batch, FeedsSource $source) {

    // Iterate through all unique targets and try to find a user for the
    // target's value.
    foreach ($this->uniqueTargets($batch) as $target => $value) {
      switch ($target) {
        case 'name':
          $uid = db_result(db_query("SELECT uid FROM {users} WHERE name = '%s'", $value));
          break;
        case 'mail':
          $uid = db_result(db_query("SELECT uid FROM {users} WHERE mail = '%s'", $value));
          break;
        case 'openid':
          $uid = db_result(db_query("SELECT uid FROM {authmap} WHERE authname = '%s' AND module = 'openid'", $value));
          break;
      }
      if ($uid) {
        // Return with the first nid found.
        return $uid;
      }
    }
    return 0;
  }
}
