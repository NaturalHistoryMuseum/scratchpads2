<?php

/**
 * @file
 * Null-op mail class, to keep migration from spamming innocent users.
 */

class MigrateMailIgnore extends DefaultMailSystem {
  /**
   * On an email request, do nothing and say we did.
   *
   * @see http://php.net/manual/en/function.mail.php
   * @see drupal_mail()
   *
   * @param $message
   *   A message array, as described in hook_mail_alter().
   * @return
   *   TRUE if the mail was successfully accepted, otherwise FALSE.
   */
  public function mail(array $message) {
    return TRUE;
  }
}
