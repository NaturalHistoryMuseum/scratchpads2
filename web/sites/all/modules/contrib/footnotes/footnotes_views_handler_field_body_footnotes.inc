<?php

/**
 * @file
 * Views handler for the body footnotes.
 */

class footnotes_views_handler_field_body_footnotes extends footnotes_views_handler_field {
  function init(&$view, $options) {
    parent::init($view, $options);
  }

  function render_name($data, $values) {
    $start = strpos($data, '<ul class="footnotes">');
    // By default footnotes are at the end, but they could be anywhere really.
    // $end   = strpos($data, '</ul><!-- end footnotes -->')+strlen('</ul><!-- end footnotes -->');
    if ($start !== FALSE) {
      // $data = substr($data, $start, $end);
      $data = substr($data, $start);
    }
    return $data;
  }

  function render($values) {
    $body = parent::render($values);
    return $this->render_link($this->render_name($body, $values), $values);
  }
};

// vim: ts=2 sw=2 et syntax=php
