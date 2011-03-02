<?php
/**
 * @file
 */
class openlayers_views_plugin_row_bounding_box extends views_plugin_row {
  // TODO: start and finish
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
