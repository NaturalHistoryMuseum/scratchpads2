<?php

/**
 * @file
 * Snippet field handler for Apache Solr Views.
 */

class apachesolr_views_snippet_handler_field extends views_handler_field {

  /**
   * Retrieve value from Solr result document.
   */
  public function get_value($doc, $field = NULL) {
    $response = $this->query->apachesolr_response;

    // We default to getting snippets from the body content and comments.
    $hl_fl = array('content', 'ts_comments');

    $params = $this->query->get_params();
    if (isset($params['hl.fl'])) {
      $hl_fl = $params['hl.fl'];
    }

    // Start with an empty snippets array.
    $snippets = array();

    // Find the nicest available snippet.
    foreach ($hl_fl as $hl_param) {
      if (isset($response->highlighting->{$doc->id}->$hl_param)) {
        // Merge arrays preserving keys.
        foreach ($response->highlighting->{$doc->id}->$hl_param as $value) {
          $snippets[$hl_param] = $value;
        }
      }
    }
    // If there's no snippet at this point, add the teaser.
    if (!$snippets) {
      if (isset($doc->teaser)) {
        $snippets[] = truncate_utf8($doc->teaser, 256, TRUE);
      }
    }

    $hook = 'apachesolr_search_snippets';
    if (isset($doc->entity_type)) {
      $hook .= '__' . $doc->entity_type;
    }
    if (!empty($doc->bundle)) {
      $hook .= '__' . $doc->bundle;
    }
    return theme($hook, array('doc' => $doc, 'snippets' => $snippets));
  }

  /**
   * Render the snippet field.
   */
  public function render($values) {
    $value = $this->get_value($values);
    return $this->sanitize_value($value, 'xss');
  }
}
