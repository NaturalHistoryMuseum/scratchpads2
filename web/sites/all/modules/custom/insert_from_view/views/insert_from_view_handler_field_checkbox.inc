<?php

class insert_from_view_handler_field_checkbox extends views_handler_field{

  function query(){
    // do nothing - here to override parent query
  }

  function render($values){
    return '<input class="insert_from_view_checkbox" type="checkbox" />';
  }
}