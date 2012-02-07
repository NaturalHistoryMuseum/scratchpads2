<?php

/**
 * Preprocessor for theme('block').
 * Allows for setting class name within the view
 */
function scratchpads_alpha_preprocess_block(&$vars){
  $block = &$vars['elements']['#block'];
  $vars['attributes_array']['class'] = isset($vars['attributes_array']['class']) ? $vars['attributes_array']['class'] : array();
  if(isset($block->class)){
    $vars['attributes_array']['class'] = array_merge($vars['attributes_array']['class'], $block->class);
  }
  if($block->region == 'slide_top'){
  	if(isset($block->subject)){
  		$block->subject = NULL;	
  	}
    
  }
}