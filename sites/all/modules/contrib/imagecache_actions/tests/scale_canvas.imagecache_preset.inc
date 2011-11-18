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

$presets['scale_canvas'] = array (
  'presetname' => 'scale_canvas',
  '#weight' => 2.1,

  'actions' => array (
    1 => array (
      'weight' => '1',
      'module' => 'imagecache',
      'action' => 'imagecache_scale',
      'data' => array (
        'width' => '100',
        'height' => '150',
        'upscale' => TRUE,
      ),
    ),
    2 => array (
      'weight' => '2',
      'module' => 'imagecache_canvasactions',
      'action' => 'canvasactions_definecanvas',
      'data' => array (
        'RGB' => array (
          'HEX' => 'DDDDDD',
        ),
        'under' => 1,
        'exact' => array (
          'width' => '100',
          'height' => '150',
          'xpos' => 'center',
          'ypos' => 'center',
        ),
      ),
    ),
  ),
);