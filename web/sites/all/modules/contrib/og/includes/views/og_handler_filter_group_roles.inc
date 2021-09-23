<?php
/**
 * Filter handler for user roles
 */
class og_handler_filter_group_roles extends views_handler_filter_many_to_one {

  /**
   * Adds in the the uid contraint for the membership table.
   */
  function query() {
    parent::query();

    // If there is a users table present constraint the roles uid to match the
    // users table.
    // @todo: Make the used user relationship configurable and optional.
    foreach ($this->query->relationships as $alias => $info) {
      if ($info['table'] == 'users') {
        $user_table_alias = $alias;
        break;
      }
    }
    if (!empty($user_table_alias)) {
      $this->query->add_where_expression(0, $this->table_alias . '.uid = ' . $user_table_alias . '.uid');
    }
  }

  function get_value_options() {
    $this->value_options = og_roles();
    // Remove OG_ANONYMOUS_ROLE and OG_AUTHENTICATED_ROLE.
    foreach (array(OG_ANONYMOUS_ROLE, OG_AUTHENTICATED_ROLE) as $name) {
      $rid = array_search($name, $this->value_options);
      unset($this->value_options[$rid]);
    }
  }
}