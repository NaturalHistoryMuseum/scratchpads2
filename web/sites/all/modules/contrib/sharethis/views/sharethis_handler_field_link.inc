<?php

/**
 * @file
 * Field handler to display ShareThis links.
 */

/**
 * Field handler for ShareThis.
 */
class sharethis_handler_field_link extends views_handler_field_entity {

  function render($values) {
    if ($entity = $this->get_value($values)) {
      return $this->render_sharethis_link($entity);
    }
  }

  function render_sharethis_link($entity) {
    global $base_url;
    $path = $base_url . url('node/' . $entity->nid);
    return sharethis_get_button_HTML(sharethis_get_options_array(), $path, $entity->title);

    /**
     * @todo
     * The line below requires theming of the sharethis button HTML as described
     * in http://drupal.org/node/1335836 . Once the theming issue is resolved,
     * this line can be uncommented/modified to implement that functionality.
     */
    /*
    return theme('sharethis', array(
      'data_options' => sharethis_get_options_array(),
      'm_title' => $entity->title,
      'm_path' => $path,
    ));
    */

  }
}
