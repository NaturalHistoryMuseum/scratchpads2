<?php

/**
 * @file
 * Hooks provided by the Tools module.
 */

/**
* @return array $tools_settings
*   An associative array of settings for the tools module
*   containing the following key-value pairs:
*   - can_disable: (required) Whether to allow a module to be disabled by the tools module
*   - cannot_disable_message: (optional) A message to display when the module cannot be disabled.
*   - disable_message: (optional) A message to display on the confirmation (Are you sure?) form.
*/
function hook_tools_pre_disable(){
  $tools_settings = array(
    'can_disable' => true,
    'cannot_disable_message' => 'You cannot disable this module because ...',
    'disable_message' => 'Disabling this module will result in ...'
  );
  return $tools_settings;
}