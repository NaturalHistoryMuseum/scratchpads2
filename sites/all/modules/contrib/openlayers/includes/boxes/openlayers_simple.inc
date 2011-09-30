<?php

/**
 * OpenLayers Boxk
 */
class openlayers_simple extends boxes_box {
  /**
   * Implementation of boxes_content::options_defaults().
   */
  public function options_defaults() {
    return array(
      'map' => ''
    );
  }

  /**
   * Implementation of boxes_content::options_form().
   */
  public function options_form(&$form_state) {
    $form = array();
    // Map objects
    $form['map'] = array(
      '#type' => 'select',
      '#title' => t('Map'),
      '#description' => t('This is map that will be used to render the view.'),
      '#options' => openlayers_map_options(),
      '#default_value' => $this->options['map'] ?
        $this->options['map'] : variable_get('openlayers_default_map', 'default'),
    );
    return $form;
  }

  /**
   * Implementation of boxes_content::options_form().
   */
  public function render() {
    $title = isset($this->title) ? check_plain($this->title) : NULL;
    $map = openlayers_map_load($this->options['map']);

    return array(
      'delta' => $this->delta, // Crucial.
      'title' => $title,
      'subject' => $title,
      'content' => openlayers_render_map($map, $map->name)
    );
  }
}
