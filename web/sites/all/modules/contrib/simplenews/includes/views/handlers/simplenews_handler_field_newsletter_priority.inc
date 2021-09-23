<?php

/**
 * @file
 * Views field handler for simplenews newsletter priority.
 */

/**
 * Display simplenews newsletter priorities. See simplenews.admin.inc
 * @ row 1427 for definition of the simplenews_get_priority() function
 */
class simplenews_handler_field_newsletter_priority extends views_handler_field {
  function render($values) {
    module_load_include('inc', 'simplenews', 'includes/simplenews.admin');
    $p = simplenews_get_priority();
    return check_plain($p[$values->{$this->field_alias}]);
  }
}
