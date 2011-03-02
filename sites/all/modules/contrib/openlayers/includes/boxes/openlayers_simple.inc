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
      'preset' => ''
    );
  }

  /**
   * Implementation of boxes_content::options_form().
   */
  public function options_form() {
    $form = array();
    // Map Preset
    $form['preset'] = array(
      '#type' => 'select',
      '#title' => t('Map preset'),
      '#description' => t('This is map that will be used to render the view.'),
      '#options' => openlayers_preset_options(),
      '#default_value' => $this->options['preset'] ?
        $this->options['preset'] : variable_get('openlayers_default_preset', 'default'),
    );
    return $form;
  }

  /**
   * Implementation of boxes_content::options_form().
   */
  public function render() {
    $title = isset($this->title) ? check_plain($this->title) : NULL;
    $preset = openlayers_preset_load($this->options['preset']);

    return array(
      'delta' => $this->delta, // Crucial.
      'title' => $title,
      'subject' => $title,
      'content' => openlayers_render_map($preset->data, $preset->name)
    );
  }
}
