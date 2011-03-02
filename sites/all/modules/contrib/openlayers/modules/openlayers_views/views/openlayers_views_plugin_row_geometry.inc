<?php
/**
 * @file
 */
class openlayers_views_plugin_row_geometry extends views_plugin_row {
  function option_definition() {
    $options = parent::option_definition();
    $options['geometry'] = array('default' => array());
    return $options;
  }

  /**
   * Provide a form for setting options.
   */
  function options_form(&$form, &$form_state) {
    $options = $this->display->handler->get_field_labels();

    $form['geometry'] = array(
      '#type' => 'select',
      '#title' => t('Geometry (WKT) Field'),
      '#options' => $options,
      '#default_value' => $this->options['geometry']
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
