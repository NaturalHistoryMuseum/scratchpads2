<?php
// $Id: views_handler_field_node_link_clone.inc,v 1.1 2008/10/14 23:16:56 pwolanin Exp $

/**
* Field handler to present a clone node link.
*
* Closely modeled after views/modules/node/views_handler_field_node_link_edit.inc
*/
class views_handler_field_node_link_clone extends views_handler_field_node_link {
  function construct() {
    parent::construct();
    $this->additional_fields['uid'] = 'uid';
    $this->additional_fields['type'] = 'type';
    $this->additional_fields['format'] = array('table' => 'node_revisions', 'field' => 'format');
  }

  function render($values) {
    // Insure that user has access to clone this node.
    $node = new stdClass();
    $node->nid = $values->{$this->aliases['nid']};
    $node->uid = $values->{$this->aliases['uid']};
    $node->type = $values->{$this->aliases['type']};
    $node->format = $values->{$this->aliases['format']};
    $node->status = 1; // unpublished nodes ignore access control
    if (!clone_access($node)) {
      return;
    }

    $text = !empty($this->options['text']) ? $this->options['text'] : t('clone');
    return l($text, "node/$node->nid/clone", array('query' => drupal_get_destination()));
  }
}
