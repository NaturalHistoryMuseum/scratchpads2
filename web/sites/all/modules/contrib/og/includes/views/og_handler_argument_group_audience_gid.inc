<?php

/**
 * @file
 * Contains the group argument for field group audience.
 */

/**
 * Field group audience - gid argument handler.
 */
class og_handler_argument_group_audience_gid extends views_handler_argument_numeric {

  /**
   * Override title(); Get the label of the entity type.
   */
  function title() {
    $title = '';
    if (!empty($this->argument) && $group = og_get_group('group', $this->argument)) {
      if ($group->access()) {
        $title = og_label($group->gid);
      }
    }

    return $title;
  }

}
