<?php

/**
 * @file
 * Contains FeedsAccountSwitcherInterface interface.
 */

/**
 * Defines an interface for a service for safe account switching.
 */
interface FeedsAccountSwitcherInterface {

  /**
   * Safely switches to another account.
   *
   * Each invocation of FeedsAccountSwitcherInterface::switchTo() must be
   * matched by a corresponding invocation of
   * FeedsAccountSwitcherInterface::switchBack() in the same function.
   *
   * @param object $account
   *   The account to switch to.
   *
   * @return FeedsAccountSwitcherInterface
   *   $this.
   */
  public function switchTo($account);

  /**
   * Reverts to a previous account after switching.
   *
   * @return FeedsAccountSwitcherInterface
   *   $this.
   *
   * @throws FeedsAccountSwitcherException
   *   When there are no more account switches to revert.
   */
  public function switchBack();

}
