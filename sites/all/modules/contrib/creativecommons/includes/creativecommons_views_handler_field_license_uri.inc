<?php

/**
 * @file
 * Provide views field handler for workflow.module.
 */

/**
 * Field handler to provide simple renderer that allows linking to a CC license deed.
 */
class creativecommons_views_handler_field_license_uri extends views_handler_field_url {

  function option_definition() {
    $options = parent::option_definition();
    $options['display_style'] = array('default' => 'icons');
    return $options;
  }

  /**
   * Provide link to node option
   */
  function options_form(&$form, &$form_state) {
    parent::options_form($form, $form_state);

    $cc = creativecommons_get_sample_license();
    $image_keys = array('button_large', 'button_small', 'icons', 'tiny_icons');
    $name_keys = array('full', 'generic', 'short');
    foreach ($image_keys as $key) {
      $display_styles[$key] = ucwords(str_replace('_', ' ', $key)) . ', e.g. ' . $cc->get_image($key);
    }
    foreach ($name_keys as $key) {
      $display_styles[$key] = ucwords(str_replace('_', ' ', $key)) . ', e.g. ' . $cc->get_name($key);
    }

    $form['display_style'] = array(
      '#title' => t('Display Style'),
      '#description' => t('How would you like the license to be displayed?'),
      '#type' => 'radios',
      '#options' => $display_styles,
      '#default_value' => $this->options['display_style'],
    );
  }

  /**
   * Implemented of render().
   *
   * Could use render_altered via $this->options['alt']['text'], but overriding
   * instead to add RDFa info...
   */
  function render($values) {
    $license_uri = $values->{$this->field_alias};
    $cc = new creativecommons_license($license_uri);

    $style = $this->options['display_style'] ? $this->options['display_style'] : 'icons';

    switch ($style) {
      case 'button_large':
      case 'button_small':
      case 'icons':
      case 'tiny_icons':
        $name = $cc->get_image($style);
        $html = TRUE;
        break;
      case 'full':
      case 'generic':
      case 'short':
        $name = check_plain($cc->get_name($style));
        break;
    }

    // Adapated from views_handler_field_url
    if (!empty($this->options['display_as_link'])) {
      $attributes['rel'] = 'license';
      $name = l($name, $license_uri, array('html' => $html, 'attributes' => $attributes));
    }

    $url = url('node/' . $values->nid, array('absolute' => TRUE));
    return '<span xmlns:cc="http://creativecommons.org/ns#" xmlns:dc="http://purl.org/dc/elements/1.1/" about="' . $url . '">' . $name . '</span>';
  }
}

