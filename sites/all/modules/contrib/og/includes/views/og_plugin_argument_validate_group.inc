<?php

/**
 * Validate whether an argument is a valid group.
 *
 * This supports numeric arguments (GID). This validator also sets the
 * argument's title to the group label, which is the main reason behind this
 * validator.
 */
class og_plugin_argument_validate_group extends views_plugin_argument_validate {

  function validate_argument($argument) {
    $group = og_load($argument);
    if (!$group) {
      return FALSE;
    }

    $this->argument->argument = $argument;
    $this->argument->validated_title = og_label($group->gid);
    return TRUE;
  }
}
