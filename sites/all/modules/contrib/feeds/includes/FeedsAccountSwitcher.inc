<?php

/**
 * @file
 * Contains FeedsAccountSwitcher class.
 */

/**
 * An implementation of FeedsAccountSwitcherInterface.
 *
 * This allows for safe switching of user accounts by ensuring that session
 * data for one user is not leaked in to others. It also provides a stack that
 * allows reverting to a previous user after switching.
 */
class FeedsAccountSwitcher implements FeedsAccountSwitcherInterface {

  /**
   * A stack of previous overridden accounts.
   *
   * @var object[]
   */
  protected $accountStack = array();

  /**
   * The current user service.
   *
   * @var object
   */
  protected $currentUser = array();

  /**
   * The original state of session prior to account switching.
   *
   * @var bool
   */
  protected $originalSessionSave;

  /**
   * Constructs a new FeedsAccountSwitcher.
   *
   * @param object $current_user
   *   (optional) The current user.
   */
  public function __construct($current_user = NULL) {
    if (is_null($current_user)) {
      global $user;
      $this->currentUser = $user;
    }
    else {
      $this->currentUser = $current_user;
    }
  }

  /**
   * {@inheritdoc}
   */
  public function switchTo($account) {
    // Prevent session information from being saved and push previous account.
    if (!isset($this->originalSessionSave)) {
      // Ensure that only the first session saving status is saved.
      $this->originalSessionSave = drupal_save_session();
      drupal_save_session(FALSE);
    }
    array_push($this->accountStack, $this->currentUser);
    $this->currentUser = $account;
    $this->activateCurrentUser();

    return $this;
  }

  /**
   * {@inheritdoc}
   */
  public function switchBack() {
    // Restore the previous account from the stack.
    if (!empty($this->accountStack)) {
      $this->currentUser = array_pop($this->accountStack);
      $this->activateCurrentUser();
    }
    else {
      throw new FeedsAccountSwitcherException('No more accounts to revert to.');
    }
    // Restore original session saving status if all account switches are
    // reverted.
    if (empty($this->accountStack)) {
      if ($this->originalSessionSave) {
        drupal_save_session($this->originalSessionSave);
      }
    }
    return $this;
  }

  /**
   * Assigns current user from this class to the global $user object.
   */
  protected function activateCurrentUser() {
    global $user;
    $user = $this->currentUser;
  }

}
