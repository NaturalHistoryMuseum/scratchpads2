<?php


/**
 * @file
 * Hooks provided by the Organic groups context module.
 */

/**
 * @addtgrouproup hooks
 * @{
 */

/**
 * Add context negotiation info.
 * 
 * Define context "handlers".
 * - name: The human readable name of the context handler.
 * - Description: The desciption of the context handler.
 * - callback: The callback function that will evaluate and return the group IDs
 *   that it finds.
 * - menu path: Optional; The menu path as retrieved from menu_get_item() that 
 *   is required for the context handler to be invoked.
 */
function hook_og_context_negotiation_info() {
  $providers = array();

  $providers['foo'] = array(
    'name' => t('Foo'),
    'description' => t("Determine context by checking if some foo value."),
    'callback' => 'foo_og_context_handler',
    // Invoke the context handler only on the following path.
    'menu path' => array('foo/%', 'foo/%/bar'),
  );

  return $providers;
}

/**
 * @} End of "addtgrouproup hooks".
 */