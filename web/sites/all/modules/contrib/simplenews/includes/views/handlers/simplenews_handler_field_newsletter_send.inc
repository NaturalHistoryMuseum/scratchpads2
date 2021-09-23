<?php
/**
 * @file
 *
 * Views handler that displays a link to send a newsletter.
 */

/**
 * Views handler that displays a link to send a newsletter.
 */
class simplenews_handler_field_newsletter_send extends views_handler_field_node_link {

  /**
   * Renders the link.
   */
  function render_link($node, $values) {
    // Ensure user has access to delete this node.
    if (!user_access('send newsletter')) {
      return;
    }
    $this->options['alter']['make_link'] = TRUE;
    $this->options['alter']['query'] = drupal_get_destination();
    if ($node->simplenews->status == SIMPLENEWS_STATUS_SEND_NOT) {
      $this->options['alter']['path'] = "node/$node->nid/simplenews";
      $text = !empty($this->options['text']) ? $this->options['text'] : t('Send newsletter');
      return $text;
    }
    else {
      return;
    }
  }
}
