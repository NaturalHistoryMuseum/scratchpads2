<?php

/**
 * Field handler to provide a list of group roles.
 */
class og_handler_field_user_roles extends og_handler_field_prerender_list {
  function construct() {
    parent::construct();
    $this->additional_fields['uid'] = array('table' => 'users', 'field' => 'uid');
  }

  function pre_render($values) {
    $list = array();
    $this->items = array();
    foreach ($values as $value) {
      $list[] = array(
        'uid' => $value->{$this->aliases['uid']},
        'gid' => $value->{$this->field_alias},
      );
    }

    // Array of all the role IDss we will need to get a role name.
    $all_rids = array();
    if ($list) {
      foreach ($list as $value) {
        $rids = og_get_user_roles($value['gid'], $value['uid']);
        foreach ($rids as $rid) {
          $this->items[$value['gid']][$value['uid']][$rid]['rid'] = $rid;
        }
        $all_rids = array_merge($all_rids, $rids);
      }

      // Get the name of all the roles we need to show.
      $all_rids = og_get_user_roles_name($all_rids);

      // Sanitize each role name once.
      array_walk($all_rids, 'check_plain');

      foreach ($this->items as $gid => $gid_value) {
        foreach ($gid_value as $uid => $uid_value) {
          foreach ($uid_value as $rid => $value) {
            $this->items[$gid][$uid][$rid]['role'] = $all_rids[$rid];
          }
        }
      }
    }
  }

  function render_item($rid, $item) {
    return $item['role'];
  }

  function document_self_tokens(&$tokens) {
    $tokens['[' . $this->options['id'] . '-role' . ']'] = t('The name of the role.');
    $tokens['[' . $this->options['id'] . '-rid' . ']'] = t('The role ID of the role.');
  }

  function add_self_tokens(&$tokens, $item) {
    $tokens['[' . $this->options['id'] . '-role' . ']'] = $item['role'];
    $tokens['[' . $this->options['id'] . '-rid' . ']'] = $item['rid'];
  }
}
