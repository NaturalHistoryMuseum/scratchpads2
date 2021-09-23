<?php 

function scratchpads_em_preprocess_block(&$block){
  if ($block['block']->module == 'emonocot_blocks' && ($block['block']->delta == 'typification_overview' || $block['block']->delta == 'hybrid_children')){
   $block['attributes_array']['class'][] = 'grid-4';
   $block['attributes_array']['class'][] = 'alpha';
  }
}