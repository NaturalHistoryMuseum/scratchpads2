<?php

/**
 * @file
 * Views handler field.
 */

class footnotes_views_handler_field extends views_handler_field_markup {
  function init(&$view, $options) {
    parent::init($view, $options);
  }

  function render_link($data, $values) {
    return $data;
  }
}

// vim: ts=2 sw=2 et syntax=php
