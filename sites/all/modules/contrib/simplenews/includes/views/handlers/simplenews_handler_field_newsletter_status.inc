<?php

/**
 * @file
 * Views handler for simplenews sent status.
 */

/**
 * Display newsletter sent status.
 */
class simplenews_handler_field_newsletter_status extends views_handler_field {
  function render($values) {
    switch ($values->{$this->field_alias}) {
      case SIMPLENEWS_STATUS_SEND_NOT:
      default:
        return t('Not sent');
      case SIMPLENEWS_STATUS_SEND_PENDING:
        return t('Pending');
      case SIMPLENEWS_STATUS_SEND_READY:
        return t('Sent');
    }
  }
}
