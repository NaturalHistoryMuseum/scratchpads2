<?php

/**
 * A handler to provide a field that is completely custom by the administrator.
 *
 * @ingroup views_field_handlers
 */
class character_editor_views_handler_field extends views_handler_field{

  function query(){
    // do nothing -- to override the parent query.
  }

  function render($values){
    // Nothing to render.
    $this->field_alias = $this->options['id'];
    $value = $this->get_value($values);
    return $value;
  }
}