<?php

/**
 * @file
 * This file contains no working PHP code; it exists to provide additional
 * documentation for doxygen as well as to document hooks in the standard
 * Drupal manner.
 */


/**
 * @defgroup legal Legal module integrations.
 *
 * Module integrations with the legal module.
 */

/**
 * @defgroup legal_hooks Legal hooks
 * @{
 * Hooks that can be implemented by other modules in order to extend legal.
 */

/**
 * Runs actions when legal terms are accepted
 *
 * @param $data
 *   Data relating to the current acceptance. Includes:
 *   uid - The ID of the user that accepted the terms.
 *   version - The version of Terms accepted.
 *   revision - The revision of Terms accepted.
 *   accepted - The timestamp the Terms were accepted at.
 *   language - The current language.
 */
function hook_legal_accepted($data) {
  if (module_exists('rules')) {
    $user       = user_load($data['uid']);
    $conditions = legal_get_conditions($data['language']);
    rules_invoke_event('legal_accepted', $user, $conditions, $data['accepted']);
  }
}

/**
 * @}
 */
