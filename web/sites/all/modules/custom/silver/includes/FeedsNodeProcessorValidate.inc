<?php

/**
 * Creates after validation nodes from feed items.
 */
class FeedsNodeProcessorValidate extends FeedsNodeProcessor{

  /*protected function entityValidate($node){
    module_load_include('pages.inc', 'node');
    $form = drupal_get_form($node->type . '_node_form', $node);
    $form_state = array(
      'values' => array()
    );
    foreach($node as $key => $value){
      $form_state['values'][$key] = $value;
    }
    node_validate($node, $form, $form_state);
    entity_form_field_validate('node', $form, $form_state);
    $errors = form_get_errors();
  }*/
}