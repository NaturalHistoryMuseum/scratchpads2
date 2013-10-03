<?php

class views_oai_pmh_plugin_style extends views_plugin_style {
  protected $oai_args = array();

  function render() {
    $content = '';
    $oai_resumption_token = '';

    if ($this->uses_row_plugin() && empty($this->row_plugin)) {
      vpr('views_plugin_style_default: Missing row plugin');
      return;
    }

    $this->oai_args  = $this->display->handler->get_oai_args();

    if (!count($this->oai_args['errors'])) {
      $content   = $this->render_content();
      if (!empty($content) && in_array($this->oai_args['verb'], array('ListRecords', 'ListIdentifiers'))) {
        $oai_resumption_token = $this->display->handler->build_oai_resumption_token();
      }
    }

    if (empty($content) && in_array($this->oai_args['verb'], array('ListRecords', 'ListIdentifiers','ListSets'))) {
      $this->oai_args['errors'] = $this->display->handler->oai_error('noRecordsMatch');
    }
    if (empty($content) && $this->oai_args['verb'] == 'GetRecord') {
      $this->oai_args['errors'] = $this->display->handler->oai_error('idDoesNotExist', $this->oai_args['identifier']);
    }

    return theme('views_oai_pmh_response', array(
      'view' => $this->view,
      'options' => $this->options,
      'oai_args' => $this->oai_args,
      'oai_content' => $content,
      'resumption_token' => $oai_resumption_token)
    );
  }

  function render_content() {
    switch($this->oai_args['verb']) {
      case 'Identify':
        if (empty($this->display->handler->oai_errors)) {
          return theme('views_oai_pmh_identify_body', array('view' => $this->view));
        }
        break;
      case "ListIdentifiers":
        return $this->render_identifiers();
        break;
      case 'ListMetadataFormats':
        return theme('views_oai_pmh_metadataformats_body');
      case 'GetRecord':
      case 'ListRecords':
        return $this->render_records();
      case 'ListSets':
	   //anto
	   return $this->render_records();
      default:
    }
  }

  function render_identifiers() {
    if (!empty($this->display->handler->oai_args['errors'])) return;

    if ($this->uses_row_plugin() && empty($this->row_plugin)) {
      vpr('views_plugin_style_default: Missing row plugin');
      return;
    }
    $records = '';
    $rows = array();
    foreach ($this->view->result as $row_index => $row) {
    	if ($this->display->handler->oai_args['verb']=="ListSets"){
    	    $records .= theme('views_oai_pmh_record_header', array('tid' => $row->tid, 'node_changed' => $row->node_changed));}
    	    else{
      $records .= theme('views_oai_pmh_record_header', array('nid' => $row->nid, 'node_changed' => $row->node_changed));}
    }
    return $records;
  }


  /**
   * Provide a full list of possible theme templates used by this style.
   */
  function theme_functions($hook = NULL) {
    if (is_null($hook)) {
      $hook = $this->definition['theme'];
    }
    return views_theme_functions($hook, $this->view, $this->display);
  }


}
