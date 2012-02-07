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

$presets['scale_rotate'] = array (
  'presetname' => 'scale_rotate',
  '#weight' => 1.3,

  'actions' => array (
    1 => array (
      'weight' => '1',
      'module' => 'imagecache',
      'action' => 'imagecache_scale',
      'data' => array (
        'width' => '',
        'height' => '150',
        'upscale' => TRUE,
      ),
    ),
    2 => array (
      'weight' => '2',
      'module' => 'imagecache',
      'action' => 'imagecache_rotate',
      'data' => array (        
        'degrees' => '15',
        'random' => 0,
        'bgcolor' => '',
      ),
    ),
  ),
);