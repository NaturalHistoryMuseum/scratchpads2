<?php

/**
 * @file
 * Views handler for showing a feeds source config field.
 */

/**
 *
 */
class feeds_views_handler_field_source extends views_handler_field {

  /**
   * Override parent::render().
   */
  public function render($values) {
    $value = unserialize($values->{$this->field_alias});
    if (isset($value['FeedsHTTPFetcher']['source'])) {
      return check_url($value['FeedsHTTPFetcher']['source']);
    }
    elseif (isset($value['FeedsFileFetcher']['source'])) {
      return file_create_url($value['FeedsFileFetcher']['source']);
    }
    return '';
  }

  /**
   * Disallow advanced rendering.
   */
  public function allow_advanced_render() {
    return FALSE;
  }

}
