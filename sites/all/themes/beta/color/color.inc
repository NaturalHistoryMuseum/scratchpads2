<?php
// $Id: color.inc,v 1.7 2010/12/14 19:53:14 dries Exp $

// Put the logo path into JavaScript for the live preview.
drupal_add_js(array('color' => array('logo' => theme_get_setting('logo', 'beta'))), 'setting');

$info = array(
  // Available colors and color labels used in theme.
  'fields' => array(
    'text' => t('Text color'),
    'link' => t('Link color'),
    'top' => t('Background Top'),
    'bottom' => t('Background Bottom'),
    'base' => t('Base Color'),
    'secondary' => t('Secondary Color'),
    'contentheaders' => t('Content Headers'),
    'sitetitle' => t('Site Title'),
    'siteslogan' => t('Slogan/Active/Hover Menus'),
    'contentbg' => t('Content Background'),
  ),
  // Pre-defined color schemes.
  'schemes' => array(
    'default' => array(
      'title' => t('Beta - Blue'),
      'colors' => array(
        'text' => '#6a6a6a',       // default text colors
        'link' => '#00598e',       // default link colors
        'top' => '#1c5d85',        // top gradient color (page bg)
        'bottom' => '#02283e',     // bottom gradient color (page bg)
        'base' => '#114667',       // primary content gradient start (content bg)
        'secondary' => '#103449',  // primary content gradient end (content bg)
        'contentheaders' => '#0073ba',  // primary content gradient end (content bg)
        'sitetitle' => '#eeeeee',  // primary content gradient end (content bg)
        'siteslogan' => '#9a9a9a',  // primary content gradient end (content bg)
        'contentbg' => '#ffffff',  // primary content background color
      ),
    ),
    
    'beta_dark' => array(
      'title' => t('Beta - Dark'),
      'colors' => array(
        'text' => '#7a7a7a',
        'link' => '#3a3a3a',
        'top' => '#3a3a3a',
        'bottom' => '#1a1a1a',
        'base' => '#4a4a4a',
        'secondary' => '#2a2a2a',
        'contentheaders' => '#3a3a3a',  // primary content gradient end (content bg)
        'sitetitle' => '#eeeeee',  // primary content gradient end (content bg)
        'siteslogan' => '#9a9a9a',  // primary content gradient end (content bg)
        'contentbg' => '#ffffff',
      ),
    ),
    /*
    'beta_plush' => array(
      'title' => t('Beta - Plush'),
      'colors' => array(
        'text' => '#7a7a7a',
        'link' => '#c40b71',
        'top' => '#562741',
        'bottom' => '#1b1b1b',
        'base' => '#562741',
        'secondary' => '#1b1b1b',
        'contentbg' => '#ffffff',
      ),
    ),
    */
  ),
  // CSS files (excluding @import) to rewrite with new color scheme.
  'css' => array(
    'css/colors.css',
  ),

  // Files to copy.
  'copy' => array(
    'logo.png',
  ),

  // Gradient definitions.
  'gradients' => array(
    array(
      // (x, y, width, height).
      'dimension' => array(0, 0, 0, 0),
      // Direction of gradient ('vertical' or 'horizontal').
      'direction' => 'vertical',
      // Keys of colors to use for the gradient.
      'colors' => array('top', 'bottom'),
    ),
  ),

  // Color areas to fill (x, y, width, height).
  'fill' => array(),

  // Coordinates of all the theme slices (x, y, width, height)
  // with their filename as used in the stylesheet.
  'slices' => array(),

  // Reference color used for blending. Matches the base.png's colors.
  'blend_target' => '#ffffff',

  // Preview files.
  'preview_css' => 'color/preview.css',
  'preview_js' => 'color/preview.js',
  'preview_html' => 'color/preview.html',

  // Base file for image generation.
  'base_image' => 'color/base.png',
);
