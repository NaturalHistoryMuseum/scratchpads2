<?php
/**
 * @file Helper functions for the text2canvas action for imagecache
 *
 * @author Dan Morrison http://coders.co.nz
 *
 * Individually configurable rounded corners logic contributed by canaryMason
 * 2009 03 http://drupal.org/node/402112
 *
 * Better algorithm for trimming rounded corners from donquixote
 * 2009 09 http://drupal.org/node/564036
 *
 */

if (! function_exists('imageapi_image_overlay') ) {
  module_load_include('inc', 'imagecache_actions', 'imageapi_image_overlay');
}
if (! function_exists('imagecache_actions_pos_form') ) {
  module_load_include('inc', 'imagecache_actions', 'utility-form');
}
if (! function_exists('imagecache_actions_keyword_filter') ) {
  module_load_include('inc', 'imagecache_actions', 'utility');
}


/**
 * Implementation of imagecache_hook_form()
 *
 * Settings for preparing a canvas.
 *
 * @param $action array of settings for this action
 * @return a form definition
 */
function canvasactions_definecanvas_form($action) {
  module_load_include('inc', 'imagecache_actions', 'utility-color');
  $defaults = array(
    'RGB' => array(
      'HEX' => '#333333',
    ),
    'under' => TRUE,
    'exact' => array(
      'width' => '',
      'height' => '',
      'xpos' => 'center',
      'ypos' => 'center',
    ),
    'relative' => array(
      'leftdiff' => '',
      'rightdiff' => '',
      'topdiff' => '',
      'bottomdiff' => '',
    ),
  );
  $action = array_merge($defaults, (array) $action);

  $form = array(
    'RGB' => imagecache_rgb_form($action['RGB']),
    'help' => array(
      '#type' => 'markup',
      '#value' => t('Enter no color value for transparent. This will have the effect of adding clear margins around the image.'),
      '#prefix' => '<p>',
      '#suffix' => '</p>',
    ),
    'under' => array(
      '#type' => 'checkbox',
      '#title' => t('Resize canvas <em>under</em> image (possibly cropping)'),
      '#default_value' => $action['under'],
      '#description' => t('If <em>not</em> set, this will create a solid flat layer, probably totally obscuring the source image'),
    ),
  );
  $form['info'] = array('#value' => t('Enter values in ONLY ONE of the below options. Either exact or relative. Most values are optional - you can adjust only one dimension as needed. If no useful values are set, the current base image size will be used.'));
  $form['exact'] = array(
    '#type' => 'fieldset',
    '#collapsible' => TRUE,
    '#title' => 'Exact size',
    'help' => array(
      '#type' => 'markup',
      '#value' => t('Set the canvas to a precise size, possibly cropping the image. Use to start with a known size.'),
      '#prefix' => '<p>',
      '#suffix' => '</p>',
    ),
    'width' => array(
      '#type' => 'textfield',
      '#title' => t('Width'),
      '#default_value' => $action['exact']['width'],
      '#description' => t('Enter a value in pixels or percent'),
      '#size' => 5,
    ),
    'height' => array(
      '#type' => 'textfield',
      '#title' => t('Height'),
      '#default_value' => $action['exact']['height'],
      '#description' => t('Enter a value in pixels or percent'),
      '#size' => 5,
    ),
  );
  $form['exact'] = array_merge($form['exact'], imagecache_actions_pos_form($action['exact']));
  if (! $action['exact']['width'] && !$action['exact']['height']) {
    $form['exact']['#collapsed'] = TRUE;
  }

  $form['relative'] = array(
    '#type' => 'fieldset',
    '#collapsible' => TRUE,
    '#title' => t('Relative size'),
    'help' => array(
      '#type' => 'markup',
      '#value' => '<p>' . t('Set the canvas to a relative size, based on the current image dimensions. Use to add simple borders or expand by a fixed amount. Negative values may crop the image.') . '</p>',
    ),
    'leftdiff' => array(
      '#type' => 'textfield',
      '#title' => t('left difference'),
      '#default_value' => $action['relative']['leftdiff'],
      '#size' => 6,
      '#description' => t('Enter an offset in pixels.'),
    ),
    'rightdiff' => array(
      '#type' => 'textfield',
      '#title' => t('right difference'),
      '#default_value' => $action['relative']['rightdiff'],
      '#size' => 6,
      '#description' => t('Enter an offset in pixels.'),
    ),
    'topdiff' => array(
      '#type' => 'textfield',
      '#title' => t('top difference'),
      '#default_value' => $action['relative']['topdiff'],
      '#size' => 6,
      '#description' => t('Enter an offset in pixels.'),
    ),
    'bottomdiff' => array(
      '#type' => 'textfield',
      '#title' => t('bottom difference'),
      '#default_value' => $action['relative']['bottomdiff'],
      '#size' => 6,
      '#description' => t('Enter an offset in pixels.'),
    ),
  );
  if (! $action['relative']['leftdiff'] && !$action['relative']['rightdiff'] && !$action['relative']['topdiff'] && !$action['relative']['bottomdiff']) {
    $form['relative']['#collapsed'] = TRUE;
  }

  $form['#submit'][] = 'canvasactions_definecanvas_form_submit';
  return $form;
}

/**
 * Implementation of theme_hook() for imagecache_ui.module
 */
function theme_canvasactions_definecanvas_summary($variables) {
  $action = $variables['data'];
  if ($action['exact']['width'] || $action['exact']['width']) {
    $output = $action['exact']['width'] . 'x' . $action['exact']['height'];
    $output .= " (" . $action['exact']['xpos'] . ', ' . $action['exact']['ypos'] . ") ";
  }
  else {
    $output = ' left:' . $action['relative']['leftdiff'];
    $output .= ' right:' . $action['relative']['rightdiff'];
    $output .= ' top:' . $action['relative']['topdiff'];
    $output .= ' bottom:' . $action['relative']['bottomdiff'];
  }
  $output .= theme('imagecacheactions_rgb', array('RGB' => $action['RGB']));
  $output .= ($action['under']) ? t(" <b>under</b> image ") : t(" <b>over</b> image ");
  return $output;
}

/**
 * Implementation of hook_image()
 *
 * Creates a solid background canvas
 *
 * Process the imagecache action on the passed image
 *
 * @param $image
 * array defining an image file, including  :
 *
 *   $image- >source as the filename,
 *
 *   $image->info array
 *
 *   $image->resource handle on the image object
 */
function canvasactions_definecanvas_image($image, $action = array()) {

  // May be given either exact or relative dimensions.
  // ED: check on width OR height
  //if ($action['exact']['width'] || $action['exact']['width']) {
  if ($action['exact']['width'] || $action['exact']['height']) {
    // Allows only one dimension to be used if the other is unset.
    if (! $action['exact']['width']) {
      $ction['exact']['width'] = $image->info['width'];
    }
    if (! $action['exact']['height']) {
      $action['exact']['height'] = $image->info['height'];
    }

    $targetsize['width'] = imagecache_actions_percent_filter($action['exact']['width'], $image->info['width']);
    $targetsize['height'] = imagecache_actions_percent_filter($action['exact']['height'], $image->info['height']);

    $targetsize['left'] = image_filter_keyword($action['exact']['xpos'], $targetsize['width'], $image->info['width']);
    $targetsize['top'] = image_filter_keyword($action['exact']['ypos'], $targetsize['height'], $image->info['height']);

  }
  else {
    // calculate relative size
    $targetsize['width'] = $image->info['width'] + $action['relative']['leftdiff'] +  $action['relative']['rightdiff'];
    $targetsize['height'] = $image->info['height'] + $action['relative']['topdiff'] +  $action['relative']['bottomdiff'];
    $targetsize['left'] = $action['relative']['leftdiff'];
    $targetsize['top'] = $action['relative']['topdiff'];
  }

  // convert from hex (as it is stored in the UI)
  if ($action['RGB']['HEX'] && $deduced = imagecache_actions_hex2rgba($action['RGB']['HEX'])) {
    $action['RGB'] = array_merge($action['RGB'], $deduced);
  }

  // All the maths is done, now defer to the api toolkits;
  $action['targetsize'] = $targetsize;

  $success = image_toolkit_invoke('definecanvas', $image, array($action));
  if ($success) {
    $image->info['width'] = $targetsize['width'];
    $image->info['height'] = $targetsize['height'];
  }
  return $success;
}

/**
 * Draw a color (or transparency) behind an image
 *
 * $targetsize is an array expected to contain a width,height and a left,top
 * offset.
 */
function image_gd_definecanvas($image, $action = array()) {
  $targetsize = $action['targetsize'];
  $RGB = $action['RGB'];

  $newcanvas = imagecreatetruecolor($targetsize['width'], $targetsize['height']);

  if ($RGB['HEX']) {
    $background = imagecolorallocate($newcanvas, $RGB['red'], $RGB['green'], $RGB['blue']);
  }
  else {
    // No color, attempt transparency, assume white
    $background = imagecolorallocatealpha($newcanvas, 255, 255, 255, 127);
    imagesavealpha($newcanvas, TRUE);
    imagealphablending($newcanvas, FALSE);
    imagesavealpha($image->resource, TRUE);
  }
  imagefilledrectangle($newcanvas, 0, 0, $targetsize['width'], $targetsize['height'], $background);
  #  imagealphablending($newcanvas, TRUE);

  if ($action['under']) {
    $canvas_object = (object) array(
      'resource' => $newcanvas,
      'info' => array(
        'width' => $targetsize['width'],
        'height' => $targetsize['height'],
        'mime_type' => $image->info['mime_type'],
        'extension' => $image->info['extension'],
      ),
      'toolkit' => $image->toolkit,
    );
    imageapi_image_overlay($canvas_object, $image, $targetsize['left'], $targetsize['top'], 100, TRUE);
  }
  else {
    $image->resource = $newcanvas;
  }
  return TRUE;
}

/**
 * Draw a color (or transparency) behind an image
 * $targetsize is an array expected to contain a width,height and a left,top
 * offset.
 *
 * Patched with code at issue http://drupal.org/node/844386
 */
function image_imageapi_imagemagick_definecanvas($image, $action = array()) {
  $targetsize = $action['targetsize'];
  $RGB = $action['RGB'];
  $backgroundcolor = $RGB['HEX'] != '' ? '#' . $RGB['HEX'] : 'None';
  $backgroundcolor = escapeshellarg($backgroundcolor);

  # TODO needs work.
  #
  $background = "-background $backgroundcolor";
  $crop = "-crop {$targetsize['width']}x{$targetsize['height']}-{$targetsize['left']}-{$targetsize['top']}!";

  if ($action['under']) {
    $extent = "-extent {$targetsize['width']}x{$targetsize['height']}-{$targetsize['left']}-{$targetsize['top']}!";
    $compose = "$background $crop $extent";
  }
  else {
    // see http://www.imagemagick.org/Usage/canvas/#solid
    $draw = "+matte -fill $backgroundcolor -colorize 100%";
    $compose = "$background $crop $draw";
  }

  $image->ops[] = $compose;
  return TRUE;
}

////////////////////////////////////////////////

/**
 * Place a given image under the current canvas
 *
 * Implementation of imagecache_hook_form()
 *
 * @param $action array of settings for this action
 * @return a form definition
 */
function canvasactions_canvas2file_form($action) {
  if (image_get_toolkit() != 'gd') {
    drupal_set_message('Overlays are not currently supported by using imagemagick. This effect requires GD image toolkit only.', 'warning');
  }

  $defaults = array(
    'xpos' => '0',
    'ypos' => '0',
    'alpha' => '100',
    'path' => '',
    'dimensions' => 'original',
  );
  $action = array_merge($defaults, (array) $action);

  $form = imagecache_actions_pos_form($action);
  $form['alpha'] = array(
    '#type' => 'textfield',
    '#title' => t('opacity'),
    '#default_value' => $action['alpha'],
    '#size' => 6,
    '#description' => t('Opacity: 0-100. Be aware that values other than 100% may be slow to process.'),
  );
  $form['path'] = array(
    '#type' => 'textfield',
    '#title' => t('file name'),
    '#default_value' => $action['path'],
    '#description' => t('File may be in the "files/" folder, or relative to the Drupal siteroot.'),
    '#element_validate' => array('canvasactions_file2canvas_validate_file'),
  );
  $form['dimensions'] = array(
    '#type' => 'radios',
    '#title' => t('final dimensions'),
    '#default_value' => $action['dimensions'],
    '#options' => array(
      'original' => 'original (dimensions are retained)',
      'background' => 'background (image will be forced to match the size of the background)',
      'minimum' => 'minimum (image may be cropped)',
      'maximum' => 'maximum (image may end up with gaps)',
    ),
    '#description' => t('What to do when the background image is a different size from the source image. Backgrounds are not tiled, but may be arbitrarily large.'),
  );
  return $form;
}

/**
 * Implementation of theme_hook() for imagecache_ui.module
 */
function theme_canvasactions_canvas2file_summary($variables) {
  $data = $variables['data'];

  $filepath = $data['path'];
  if (!file_exists($filepath) ) {
    $filepath = file_default_scheme() . '://' . $data['path'];
  }
  $file_url = url($filepath);
  return "xpos:{$data['xpos']} , ypos:{$data['ypos']} alpha:{$data['alpha']}%. file path: <a href='$file_url'>" . basename($data['path']) . "</a>, dimensions:{$data['dimensions']}";
}

/**
 * Place the source image on the current background
 *
 * Implementation of hook_image()
 *
 * Note - this is currently incompatable with imagemagick, due to the way it
 * addresses $image->resource directly - a gd only thing.
 *
 * @param $image
 * @param $action
 */
function canvasactions_canvas2file_image(&$image, $action = array()) {
  // Search for full (siteroot) paths, then file dir paths, then relative to the current theme
  $filepath = $action['path'];
  if (!file_exists($filepath) ) {
    $filepath = file_default_scheme() . '://' . $action['path'];
  }
  if (! file_exists($filepath) ) {
    trigger_error("Failed to load underlay image $filepath.", E_USER_ERROR);
    return FALSE;
  }

  $underlay = image_load($filepath, $image->toolkit);

  // To handle odd sizes, we will resize/crop the background image to the desired dimensions before
  // starting the merge. The built-in imagecopymerge, and the watermark library both do not
  // allow overlays to be bigger than the target.
  // Adjust size
  $crop_rules = array(
    'xoffset' => 0,
    'yoffset' => 0,
  );
  if (empty($action['dimensions'])) {
    $action['dimensions'] = 'original';
  }
  switch ($action['dimensions']) {
    case 'original':
      // If the underlay is smaller than the target size,
      // then when preparing the underlay by cropping it,
      // the offsets may need to be negative
      // which will produce a 'cropped' image larger than the original
      // In this case, we need to calculate the position of the bg image
      // in relation to the space it will occupy under the top layer
      #$crop_rules['xoffset'] = $underlay->info['width'] - $image->info['width'] ;

      $crop_rules['width'] = $image->info['width'];
      $crop_rules['height']  = $image->info['height'];
      break;
    case 'background':
      $crop_rules['width'] = $underlay->info['width'];
      $crop_rules['height'] = $underlay->info['height'];
      break;
    case 'minimum':
      $crop_rules['width'] = min($underlay->info['width'], $image->info['width']);
      $crop_rules['height'] = min($underlay->info['height'], $image->info['height']);
      break;
    case 'maximum':
      $crop_rules['width'] = max($underlay->info['width'], $image->info['width']);
      $crop_rules['height'] = max($underlay->info['height'], $image->info['height']);
      break;
  }
  // imageapi crop assumes upsize is legal.

  // Crop both before processing to avoid unwanted processing.
  image_crop_effect($underlay, $crop_rules);
  # BUG - this doesn't position either
  // Actually this fails because imagecache_crop fills it with solid color when 'cropping' to a larger size.
  #imagecache_crop_image($image, $crop_rules);
  #dpm(get_defined_vars());
  // This func modifies the underlay image by ref, placing the current canvas on it
  //TODO: module image, function: image_effect_apply does not pass by reference, does this work?
  if (imageapi_image_overlay($underlay, $image, $action['xpos'], $action['ypos'], $action['alpha'], TRUE)) {
    #$image->resource = $underlay->resource;
    $image = $underlay;
    return TRUE;
  }
}

////////////////////////////////////////////////


/**
 * Place a given image on top of the current canvas
 *
 * Implementation of imagecache_hook_form()
 *
 * @param $action array of settings for this action
 * @return a form definition
 */
function canvasactions_file2canvas_form($action) {
  if (image_get_toolkit() != 'gd') {
    drupal_set_message('Overlays are not currently supported by using imagemagick. This effect requires GD image toolkit only.', 'warning');
  }

  $defaults = array(
    'xpos' => '',
    'ypos' => '',
    'alpha' => '100',
    'path' => '',
  );
  $action = array_merge($defaults, (array) $action);

  $form = array(
    'help' => array(
      '#type' => 'markup',
      '#value' => t('Note that using a transparent overlay that is larger than the source image may result in unwanted results - a solid background.'),
    ),
  );
  $form += imagecache_actions_pos_form($action);
  $form['alpha'] = array(
    '#type' => 'textfield',
    '#title' => t('opacity'),
    '#default_value' => $action['alpha'],
    '#size' => 6,
    '#description' => t('Opacity: 0-100. <b>Warning:</b> Due to a limitation in the GD toolkit, using an opacity other than 100% requires the system to use an algorithm that\'s much slower than the built-in functions. If you want partial transparency, you are better to use an already-transparent png as the overlay source image.'),
  );
  $form['path'] = array(
    '#type' => 'textfield',
    '#title' => t('file name'),
    '#default_value' => $action['path'],
    '#description' => t('File may be in the "files/" folder, or relative to the Drupal siteroot.'),
    '#element_validate' => array('canvasactions_file2canvas_validate_file'),
  );
  return $form;
}


/**
 * Check if the named file is available
 */
function canvasactions_file2canvas_validate_file(&$element, &$form_status) {
  if (! file_exists($element['#value']) && ! file_exists(file_default_scheme() . '://' . $element['#value'])) {
    form_error($element, t("Unable to find the named file '%filepath' in either the site or the files directory. Please check the path. Use relative paths only.", array('%filepath' => $element['#value'])) );
  }
}

/**
 * Implementation of theme_hook() for imagecache_ui.module
 */
function theme_canvasactions_file2canvas_summary($variables) {
  $action = $variables['data'];
  return '<strong>' . basename($action['path']) . '</strong> x:' . $action['xpos'] . ', y:' . $action['ypos'] . ' alpha:' . (@$action['alpha'] ? $action['alpha'] : 100) . '%';
}

/**
 * Place the source image on the current background
 *
 * Implementation of hook_image()
 *
 *
 * @param $image
 * @param $action
 */
function canvasactions_file2canvas_image($image, $action = array()) {
  // search for full (siteroot) paths, then file dir paths, then relative to the current theme
  if (file_exists($action['path'])) {
    $overlay = image_load($action['path'], $image->toolkit);
  }
  else if (file_exists(file_default_scheme() . '://' . $action['path'])) {
    $overlay = image_load(file_default_scheme() . '://' . $action['path'], $image->toolkit);
  }
  if (! isset($overlay) || ! $overlay) {
    trigger_error(t("Failed to find overlay image %path for imagecache_actions file-to-canvas action. File was not found in the sites 'files' path or the current theme folder.", array('%path' => $action['path'])), E_USER_WARNING);
    // return FALSE;
    // Warn, but continue anyway
    return TRUE;
  }
  if (! isset($action['alpha']) ) {
    $action['alpha'] = 100;
  }
  return imageapi_image_overlay($image, $overlay, $action['xpos'], $action['ypos'], $action['alpha']);
}

///////////////////////////////////////////////////////////////////

/**
 * Place the source image on top of the current canvas
 *
 * Implementation of imagecache_hook_form()
 *
 *
 *
 * @param $action array of settings for this action
 * @return a form definition
 */
function canvasactions_source2canvas_form($action) {
  $defaults = array(
    'xpos' => '',
    'ypos' => '',
    'alpha' => '100',
    'path' => '',
  );
  $action = array_merge($defaults, (array) $action);

  $form = imagecache_actions_pos_form($action);
  $form['alpha'] = array(
    '#type' => 'textfield',
    '#title' => t('opacity'),
    '#default_value' => $action['alpha'],
    '#size' => 6,
    '#description' => t('Opacity: 0-100.'),
  );
  return $form;
}



/**
 * Implementation of theme_hook() for imagecache_ui.module
 */
function theme_canvasactions_source2canvas_summary($variables) {
  $data = $variables['data'];
  return 'xpos:' . $data['xpos'] . ', ypos:' . $data['ypos'] . ' alpha:' . $data['alpha'] . '%';
}

/**
 * Place the source image on the current background
 *
 * Implementation of hook_image()
 *
 *
 * @param $image
 * @param $action
 */
function canvasactions_source2canvas_image($image, $action = array()) {
  $overlay = image_load($image->source); // this probably means opening the image twice. c'est la vie
  return imageapi_image_overlay($image, $overlay, $action['xpos'], $action['ypos'], $action['alpha']);
}

///////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////

/**
 * Switch between presets depending on logic
 *
 * Implementation of imagecache_hook_form()
 *
 * @param $action array of settings for this action
 * @return a form definition
 */
function canvasactions_aspect_form($action) {
  $defaults = array(
    'ratio_adjustment' => 1,
    'portrait' => NULL,
    'landscape' => NULL,
  );
  $action = array_merge($defaults, (array)$action);

  $form = array(
    'help' => array(
      '#type' => 'markup',
      '#value' => t('You must create the two presets to use <em>before</em> enabling this process.'),
    )
  );

  $styles = image_style_options(TRUE);

  $form['portrait'] = array(
    '#type' => 'select',
    '#title' => t('Style to use if the image is portrait (vertical)'),
    '#default_value' => $action['portrait'],
    '#options' => $styles,
  );
  $form['landscape'] = array(
    '#type' => 'select',
    '#title' => t('Style to use if the image is landscape (horizontal)'),
    '#default_value' => $action['landscape'],
    '#options' => $styles,
  );

  $form['ratio_adjustment'] = array(
    '#type' => 'textfield',
    '#title' => t('Ratio Adjustment (advanced)'),
    '#size' => 3,
    '#default_value' => $action['ratio_adjustment'],
    '#description' => t("
This allows you to bend the rules for how different the proportions need to be to trigger the switch.
<br/>If the (width/height)*n is greater than 1, use 'landscape', otherwise use 'portrait'.
<br/>When n = 1 (the default) it will switch between portrait and landscape modes.
<br/>If n > 1, images that are slightly wide will still be treated as portraits.
If n < 1 then blunt portraits will be treated as landscape.
    "),
  );


  return $form;
}


/**
 * Implementation of theme_hook() for imagecache_ui.module
 */
function theme_canvasactions_aspect_summary($variables) {
  $action = $variables['data'];
  $ratio_adjustment = '';
  if ($action['ratio_adjustment'] != 1) {
    $ratio_adjustment = " (switch at 1:{$action['ratio_adjustment']})";
  }
  return 'Portrait size: <strong>'. $action['portrait'] . '</strong>. Landscape size: <strong>'. $action['landscape'] .'</strong>'. $ratio_adjustment ;
}

/**
 * Choose the action and trigger that.
 *
 * Implementation of hook_image()
 *
 * @param $image
 * @param $action
 */
function canvasactions_aspect_image($image, $action = array()) {
  $ratio_adjustment = 0 + $action['ratio_adjustment'];
  if (!$ratio_adjustment) {
    $ratio_adjustment = 1;
  }
  $aspect = $image->info['width'] / $image->info['height'];
  // width / height * adjustment. If > 1, it's wide.
  $style_name = (($aspect * $ratio_adjustment) > 1) ? $action['landscape'] : $action['portrait'];
  $style = image_style_load($style_name);

  if (empty($style_name)) {
    // Required preset has gone missing?
    watchdog('imagecache_canvasactions', "When running 'aspect' action, I was unable to load sub-action %style_name. Either it's been deleted or the DB needs an update", array('%style_name' => $style_name), WATCHDOG_ERROR);
    return FALSE;
  }

  // Run the preset actions ourself. 
  // Cannot invoke a preset from the top as it handles filenames, not image objects.
  // Ripped from imagecache_build_derivative()
  foreach ($style['effects'] as $sub_effect) {

    // These actions really should interpret the parameters themselves.
    foreach (array('height', 'width') as $param) {
     if (isset($sub_effect['data'][$param])) {
       $sub_effect['data'][$param] = imagecache_actions_percent_filter($sub_effect['data'][$param], $image->info[$param]);
      }
    }

    foreach (array(
      'xoffset' => 'width',
      'yoffset' => 'height',
    ) as $param => $direction) {
      if (isset($sub_effect['data'][$param])) {
        $sub_effect['data'][$param] = image_filter_keyword($sub_effect['data'][$param], $image->info[$direction], $sub_action['data'][$direction]);
      }
    }

    image_effect_apply($image, $sub_effect);
  }
  return TRUE;
}


///////////////////////////////////////////////////////////////////
