<?php
// $ID:  $
/**
 * @file
 *   Test imagecache preset.
 * 
 * Created on Dec 29, 2009
 *
 * @author 'dman' Dan Morrison http://coders.co.nz/
 */

$presets['scale'] = array (
  'presetname' => 'scale',
  '#weight' => 1.1,
  'actions' => array (
    0 => array (
      'weight' => '-1',
      'module' => 'imagecache',
      'action' => 'imagecache_scale',
      'data' => array (
        'width' => '100',
        'height' => '100%',
        'upscale' => 0,
      ),
    ),
  )
);