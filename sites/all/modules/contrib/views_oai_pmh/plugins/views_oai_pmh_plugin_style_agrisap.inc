<?php
/**
 * @file
 * Plugin include file for export style plugin.
 */

/**
 * Generalized style plugin for export plugins.
 *
 * @ingroup views_style_plugins
 */
class views_oai_pmh_plugin_style_agrisap extends views_oai_pmh_plugin_style {

  function render_records() {
    if (!empty($this->display->handler->oai_errors)) return;
   // $records = array();
    if ($this->uses_row_plugin() && empty($this->row_plugin)) {
      vpr('views_plugin_style_default: Missing row plugin');
      return;
    }
    $records = '';
    if ($this->uses_row_plugin()) {
      $rows = array();
      foreach ($this->view->result as $row_index => $row) {
        $this->view->row_index = $row_index;
        $dc_fields = $this->row_plugin->render($row);
        $records .= theme('views_oai_pmh_agrisap_record', array('view' => $this->view, 'index' => $row_index, 'fields' => $dc_fields));
      }
    }
  return $records;
  }

}
