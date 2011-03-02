<?php
/**
 * @file
 */
class openlayers_views_plugin_row_point extends views_plugin_row {
  function option_definition() {
    $options = parent::option_definition();

    $options['latitude'] = array('default' => array());
    $options['longitude'] = array('default' => array());
    return $options;
  }

  /**
   * Provide a form for setting options.
   */
  function options_form(&$form, &$form_state) {
    $options = $this->display->handler->get_field_labels();

    $form['latitude'] = array(
      '#type' => 'select',
      '#title' => t('Latitude Field'),
      '#options' => $options,
      '#default_value' => $this->options['latitude']
    );
    $form['longitude'] = array(
      '#type' => 'select',
      '#title' => t('Longitude Field'),
      '#options' => $options,
      '#default_value' => $this->options['longitude']
    );
  }

  /**
   * Render a row object. This usually passes through to a theme template
   * of some form, but not always.
   */
  function render($row) {
    return theme($this->theme_functions(),
      array(
        'view' => $this->view,
        'options' => $this->options,
        'row' => $row,
        'field_alias' => isset($this->field_alias) ? $this->field_alias : '',
      ));
  }
}
