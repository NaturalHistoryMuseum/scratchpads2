<?php

/**
 * @file
 * Contains the default style plugin.
 */
/**
 * Default style plugin to render rows one after another with no
 * decorations.
 *
 * @ingroup views_style_plugins
 */
class eolapi_plugin_style_eolapi extends views_plugin_style{

  /**
   * Set default options
   */
  function options(&$options){
    parent::options($options);
  }

  function options_form(&$form, &$form_state){
    parent::options_form($form, $form_state);
  }

  function even_empty(){
    return TRUE;
  }

  function render(){
    if(count($this->view->result)){
      return parent::render();
    }else{
      return kpr($this, 1);
      return "<pre>" . print_r($this, 1) . "</pre>";
    }
  }
}