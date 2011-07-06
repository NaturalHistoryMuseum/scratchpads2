<?php

/**
 * @file
 * 
 * Description of the tinytax.callbacks.inc file.
 */
function tinytax_js($term){
  return theme('tinytax_branch', array('tid' => $term->tid, 'vid' => $term->vid));;
}