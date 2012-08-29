<?php
/**
 * Implementation of image_effects_text.
 */

/**
 * Real implementation of hook_help() called by image_effects_text_help().
 *
 * Experimental diagnostic page to assist locating valid fonts on the system.
 * Only tuned for Ubuntu so far. I've been unable do find ubiquitous tools that
 * provide useful font listings.'
 */
function image_effects_text_help_inc($path, $arg) {
  $output = "<p>
    For text rendering to work on a server, we <em>must</em>
    know the system path to the font <em>file</em>, not just the name.
    Font library handling differs too much on different systems and
    the available PHP toolkits are unable to return good diagnostics.
    </p><p>
    On Debian/Ubuntu, you may find your fonts in and under
    <code>/usr/share/fonts/truetype/</code>
    eg <code>'/usr/share/fonts/truetype/ttf-bitstream-vera/VeraMono.ttf'</code>
    </p><p>
    On OSX, they are probably in <code>/Library/Fonts/</code>
    eg <code>'/Library/Fonts/Times New Roman Bold Italic.ttf'</code>
    </p><p>
    On Windows, they are probably in <code>C:\\WINDOWS\Fonts\</code>
    eg <code>'C:\\WINDOWS\\Fonts\\comic.ttf'</code>
    </p><p>
    Of course, this will change if you deploy to a different server!
    so the best approach is to place your own TTF font file inside your private
    or public files directory and use that. Just give the filename with the
    'private://' or 'public://' scheme prefix and it should be found.
    </p>
  ";
  $output .= t("<p>Files directory is !files</p>", array('!files' => variable_get('file_public_path', conf_path() . '/files')));

  return $output;
}

/**
 * Builds the form structure for the overlay text image effect.
 *
 * Note that this is not a complete form, it only contains the portion of the
 * form for configuring the effect options. Therefore it does not not need to
 * include metadata about the effect, nor a submit button.
 *
 * @param array $data
 *   The current configuration for this image effect.
 *
 * @return array
 *   The form definition for this effect.
 */
function image_effects_text_form_inc($data) {
  // Use of functions imagecache_file_...() creates a dependency on file utility.inc.
  module_load_include('inc', 'imagecache_actions', 'utility');
  // Use of function imagecache_rgb_form() creates a dependency on file utility-color.inc.
  module_load_include('inc', 'imagecache_actions', 'utility-color');

  // Note: we also need to check for the existence of the module: admin has
  //   all rights, so user_acccess(...) returns TRUE even if the module is not
  //   enabled and the permission does not exist.
  // A user without the 'use PHP for settings' permission (defined by the core
  // PHP filter module) may not:
  // - Select the 'PHP code' text source option if it is currently not selected.
  // - Change the 'PHP code' textarea.
  $allow_dynamic = user_access('use PHP for settings') && module_exists('php');
  $defaults = array(
    'size' => 12,
    'angle' => 0,
    'xpos' => '0',
    'ypos' => '0',
    'halign' => 'left',
    'valign' => 'bottom',
    'RGB' => array('HEX' => '#000000'),
    'alpha' => 100,
    'fontfile' => 'lhandw.ttf',
    'text_source' => 'text',
    'text' => 'Hello World!',
    'php' => 'return \'Hello World!\'',
  );
  $data += $defaults;
  $form = array(
    'size' => array(
      '#type' => 'textfield',
      '#title' => t('Font size'),
      '#default_value' => $data['size'],
      '#description' => t('The font size in points. Only in GD1 this is in pixels.'),
      '#size' => 3,
    ),
    'xpos' => array(
      '#type' => 'textfield',
      '#title' => t('X offset'),
      '#default_value' => $data['xpos'],
      '#description' => t('Enter an offset in pixels or use a keyword: <em>left</em>, <em>center</em>, or <em>right</em>. Syntax like <em>right-20</em> is also valid.'),
      '#size' => 10,
    ),
    'ypos' => array(
      '#type' => 'textfield',
      '#title' => t('Y offset'),
      '#default_value' => $data['ypos'],
      '#description' => t('Enter an offset in pixels or use a keyword: <em>top</em>, <em>center</em>, or <em>bottom</em>. Syntax like <em>bottom-20</em> is also valid.'),
      '#size' => 10,
    ),
    'halign' => array(
      '#type' => 'select',
      '#title' => t('Horizontal alignment'),
      '#default_value' => $data['halign'],
      '#description' => t('The horizontal alignment of the text around the given %xpos.', array('%xpos' => t('X offset'))),
      '#options' => array('left' => t('Left'), 'center' => t('Center'), 'right' => t('Right')),
    ),
    'valign' => array(
      '#type' => 'select',
      '#title' => t('Vertical alignment'),
      '#default_value' => $data['valign'],
      '#description' => t('The vertical alignment of the text around the given %ypos.', array('%ypos' => t('Y offset'))),
      '#options' => array('top' => t('Top'), 'center' => t('Center'), 'bottom' => t('Bottom')),
    ),
    'RGB' => imagecache_rgb_form($data['RGB']),
    'alpha' => array(
      '#type' => 'textfield',
      '#title' => t('Opacity'),
      '#default_value' => $data['alpha'] ? $data['alpha'] : 100,
      '#size' => 3,
      '#description' => t('Opacity: 1-100.'),
    ),
    'angle' => array(
      '#type' => 'textfield',
      '#title' => t('Angle'),
      '#default_value' => $data['angle'],
      '#description' => t('Angle: The angle in degrees, with 0 degrees being left-to-right reading text. Higher values represent a counter-clockwise rotation. For example, a value of 90 would result in bottom-to-top reading text.'),
      '#size' => 3,
    ),
    'fontfile' => array(
      '#type' => 'textfield',
      '#title' => t('Font file name'),
      '#default_value' => $data['fontfile'],
      '#description' => imagecache_actions_file_field_description(),
      '#element_validate' => array('imagecache_actions_validate_file'),
    ),
    'text_help' => array(
      '#type' => 'item',
      '#title' => t('Text'),
      '#description' => t('<p>Select the source of the text:</p>
<ul>
<li><strong>Image alt</strong>: the alt text of an image field referring to this image is taken.</li>
<li><strong>Image title</strong>: the title text of an image field referring to this image is taken.</li>
<li><strong>Static text</strong>: a text that will be the same for each image, e.g. a copyright message. You can define the text in the text field below the drop down.</li>
<li><strong>PHP code</strong>: a piece of PHP code that returns the text to display. You can define the PHP code in the text area below the drop down. You will need the \'%use_php\' permission, defined by the \'PHP filter\' module.</li>
</ul>
<p>See the help for an extensive explanation of the possibilities.</p>',
        array('%use_php' => t('Use PHP for settings'))),
    ),
    'text_source' => array(
      '#type' => 'select',
      '#title' => t('Text source'),
      '#default_value' => $data['text_source'],
      '#options' => array('alt' => t('Image alt'), 'title' => t('Image title'), 'text' => t('Static text'), 'php' => t('PHP code')),
    ),
    'text' => array(
      '#type' => 'textfield',
      '#title' => t('Text'),
      '#default_value' => $data['text'],
      '#states' => array(
        'visible' => array(':input[name="data[text_source]"]' => array('value' => 'text')),
      ),
    ),
    'php' => array(
      '#type' => 'textarea',
      '#rows' => 5,
      '#title' => t('PHP code'),
      '#default_value' => $data['php'],
      '#disabled' => !$allow_dynamic,
      '#states' => array(
        'visible' => array(':input[name="data[text_source]"]' => array('value' => 'php')),
      ),
    ),
  );
  if (!$allow_dynamic && $data['text_source'] !== 'php') {
    unset($form['text_fieldset']['text_source']['#options']['php']);
  }

  $form['#element_validate'][] = 'image_effects_text_form_validate';
  return $form;
}

/**
 * Element validation callback for the effect form.
 * @see http://api.drupal.org/api/drupal/developer--topics--forms_api_reference.html/7#element_validate
 */
function image_effects_text_form_validate($element, &$form_state, $form) {
  if (!is_numeric($element['size']['#value']) || $element['size']['#value'] <= 0) {
    form_error($element['size'], t('%field must be a positive number.', array('%field' => t('Font size'))));
  }
  if (!is_numeric($element['alpha']['#value']) || $element['alpha']['#value'] < 0 || $element['alpha']['#value'] > 100) {
    form_error($element['alpha'], t('%field must be a number between 1 and 100.', array('%field' => t('Opacity'))));
  }
  if (!is_numeric($element['angle']['#value'])) {
    form_error($element['angle'], t('%field must be a number.', array('%field' => t('Angle'))));
  }
}

/**
 * Implementation of theme_hook() for text image effect.
 *
 * @param array $variables
 *   An associative array containing:
 *   - data: The current configuration for this resize effect.
 *
 * @return string
 *   The HTML for the summary of a text image effect.
 * @ingroup themeable
 */
function theme_image_effects_text_summary($variables) {
  $data = $variables['data'];
  switch ($data['text_source']) {
    case 'alt':
      $text = 'image alt';
      break;
    case 'title':
      $text = 'image title';
      break;
    case 'text':
      $text = $data['text'];
      break;
    case 'php':
      $text = 'PHP code';
      break;
  }
  return 'Text: ' . $text . '; Position: ' . $data['xpos'] . ',' . $data['ypos'] . '; Alignment: ' . $data['halign'] . ',' . $data['valign'];
}

/**
 * (Real implementation of) Image effect callback; Overlay text on an image
 * resource.
 *
 * @param object $image
 *   An image object returned by image_load().
 *
 * @param array $data
 *   An array of attributes to use when performing the resize effect with the
 *   following items:
 *   - "width": An integer representing the desired width in pixels.
 *   - "height": An integer representing the desired height in pixels.
 *
 * @return boolean
 *   true on success, false on failure to apply the effect.
 */
function image_effects_text_effect_inc($image, $data) {
  // Use of imagecache_actions_hex2rgba() ,the imagecache_file_...() functions,
  // and imagecache_actions_get_image_context() create a dependency on
  // file utility.inc.
  module_load_include('inc', 'imagecache_actions', 'utility');

  // Massage the data and pass it on to the toolkit dependent part.
  // Start with a straight copy.
  $params = $data;

  // Find out where the font file is located and if it is readable.
  $params['fontpath'] = imagecache_actions_find_file($data['fontfile']);
  if ($params['fontpath'] === FALSE) {
    drupal_set_message(t("Failed to locate the requested font %fontfile. Cannot overlay text onto image.", array('%fontfile' => $data['fontfile'])), 'error');
    return FALSE;
  }

  // Get the text to overlay.
  $params['text'] = image_effects_text_get_text($image, $params);
  if ($params['text'] === FALSE) {
    drupal_set_message(t("Failed to evaluate text (is the 'PHP Filter' module enabled?). Not overlaying text."), 'error');
    return FALSE;
  }

  // Parse offsets
  $params['xpos'] = image_effects_text_get_offset($data['xpos'], $image->info['width'], $image->info['height'], $image->info['width']);
  $params['ypos'] = image_effects_text_get_offset($data['ypos'], $image->info['width'], $image->info['height'], $image->info['height']);

  // Convert color from hex (as it is stored in the UI).
  $params['RGB'] = $data['RGB'];
  if($params['RGB']['HEX'] && $deduced = imagecache_actions_hex2rgba($params['RGB']['HEX'])) {
    $params['RGB'] += $deduced;
  }

  // Make int's of various parameters
  $params['size'] = (int) $params['size'];
  $params['xpos'] = (int) $params['xpos'];
  $params['ypos'] = (int) $params['ypos'];
  // Hand over to toolkit
  return image_toolkit_invoke('image_effects_text', $image, array($params));
}

/**
 * GD toolkit specific implementation of this image effect.
 *
 * @param object $image
 * @param array $params
 *   An array containing the parameters for this effect.
 *
 * @return bool
 *   true on success, false otherwise.
 */
function image_gd_image_effects_text($image, $params) {
  // Convert color and alpha to GD alpha and color value.
  // GD alpha value: 0 = opaque, 127 = transparent.
  $params['alpha'] = (int) ((1 - ($params['alpha'] / 100)) * 127);
  $color = imagecolorallocatealpha($image->resource, $params['RGB']['red'], $params['RGB']['green'], $params['RGB']['blue'], $params['alpha']);
  if ($color !== FALSE) {
    $bounds = NULL;
    // Adjust Y position for vertical alignment (if different from bottom).
    if ($params['valign'] !== 'bottom') {
      // Get bounding box.
      // PHP Manual: "This function requires both the GD library and the » FreeType library."
      // So it is not more demanding than imagettftext, which we need anyway.
      $bounds = imagettfbbox($params['size'], 0, $params['fontpath'], $params['text']);
      if ($bounds === FALSE) {
        drupal_set_message(t('Failed to calculate text dimensions using GD toolkit. Ignoring the alignment settings.'), 'warning');
      }
      else {
        // Get height of bounding box.
        $height = $bounds[1] - $bounds[7];
        // Shift ypos down (full height on bottom, half the height on center).
        $params['ypos'] += $params['valign'] === 'center' ? (int) ($height/2) : $height;
      }
    }
    // Adjust X position for horizontal alignment (if different from left).
    if ($params['halign'] !== 'left') {
      // Get bounding box.
      // PHP Manual: "This function requires both the GD library and the » FreeType library."
      // So it is not more demanding than imagettftext, which we need anyway.
      if ($bounds === NULL) {
        $bounds = imagettfbbox($params['size'], 0, $params['fontpath'], $params['text']);
        if ($bounds === FALSE) {
          drupal_set_message(t('Failed to calculate text dimensions using GD toolkit. Ignoring the alignment.'), 'warning');
        }
      }
      if ($bounds !== FALSE) {
        // Get width of bounding box.
        $width = $bounds[2] - $bounds[0];
        // Shift xpos to the left (full width on right, half the width on center).
        $params['xpos'] -= $params['halign'] === 'center' ? (int) ($width/2) : $width;
      }
    }

    // PHP Manual: "This function requires both the GD library and the » FreeType library."
    $bounds = imagettftext($image->resource, $params['size'], $params['angle'], $params['xpos'], $params['ypos'], $color, $params['fontpath'], $params['text']);
    return $bounds !== FALSE;
  }
  return FALSE;
}

/**
 * Imagemagick toolkit specific implementation of this image effect.
 *
 * Text in Imagemagick:
 * @link http://www.imagemagick.org/script/command-line-options.php?#draw
 * @link http://www.imagemagick.org/script/command-line-options.php?#annotate
 *
 * UTF-8/non-ascii characters
 * Though the online imagemagick manual mentions some problems with accented
 * characters, it worked fine for me in a Windows Vista shell. TBC on other
 * OS'es (including linux)
 * (@see: http://www.imagemagick.org/Usage/windows/#character_encoding)
 *
 * Alignment in Imagemagick:
 * This is not directly supported, though a justicifcation option has been
 * proposed: @link http://www.imagemagick.org/Usage/bugs/future/#justification.
 *
 * What we do have is the gravity option:
 * @link http://www.imagemagick.org/Usage/annotating/#gravity
 * Gravity is used to position a text, but it also automatically applies a
 * justification based on that placement. So we use gravity here for alignment,
 * but will thus have to rebase our positioning.
 *
 * Gravity  |halign|valign |hpos change|vpos change
 * ------------------------------------------------
 * NorthWest left   top     0           0
 * North     center top     -width/2    0
 * NorthEast right  top     -width      0
 * West      left   center  0           -height/2
 * Center    center center  -width/2    -height/2
 * East      right  center  -width      -height/2
 * SouthWest left   bottom  0           -height
 * South     center bottom  -width/2    -height
 * SouthEast right  bottom  -width      -height
 */
function image_imagemagick_image_effects_text($image, $params) {
  static $alignments2gravity = array(
    'left' => array(
      'top' => array(
        'gravity' => 'NorthWest',
        'tx' => 0,
        'ty' => 0,
      ),
      'center' => array(
        'gravity' => 'West',
        'tx' => 0,
        'ty' => -0.5,
      ),
      'bottom' => array(
        'gravity' => 'SouthWest',
        'tx' => 0,
        'ty' => 1, // reversed translation
      ),
    ),
    'center' => array(
      'top' => array(
        'gravity' => 'North',
        'tx' => -0.5,
        'ty' => 0,
      ),
      'center' => array(
        'gravity' => 'Center',
        'tx' => -0.5,
        'ty' => -0.5,
      ),
      'bottom' => array(
        'gravity' => 'South',
        'tx' => -0.5,
        'ty' => 1, // reversed translation
      ),
    ),
    'right' => array(
      'top' => array(
        'gravity' => 'NorthEast',
        'tx' => 1, // reversed translation
        'ty' => 0,
      ),
      'center' => array(
        'gravity' => 'East',
        'tx' => 1, // reversed translation
        'ty' => -0.5,
      ),
      'bottom' => array(
        'gravity' => 'SouthEast',
        'tx' => 1, // reversed translation
        'ty' => 1, // reversed translation
      ),
    ),
  );

  // Convert color and alpha to Imagemagick rgba color argument.
  $alpha = $params['alpha'] / 100;
  $color = 'rgba(' . $params['RGB']['red']. ',' . $params['RGB']['green'] . ',' . $params['RGB']['blue'] . ','. $alpha . ')';

  // Alignment
  $alignment_corrections = $alignments2gravity[$params['halign']][$params['valign']];
  $gravity = $alignment_corrections['gravity'];
  if ($alignment_corrections['tx'] > 0) {
    $params['xpos'] = (int) ($alignment_corrections['tx'] * $image->info['width'] - $params['xpos']);
  }
  else {
    $params['xpos'] += (int) ($alignment_corrections['tx'] * $image->info['width']);
  }
  if ($alignment_corrections['ty'] > 0) {
    $params['ypos'] = (int) ($alignment_corrections['ty'] * $image->info['height'] - $params['ypos']);
  }
  else {
    $params['ypos'] += (int) ($alignment_corrections['ty'] * $image->info['height']);
  }

  // Define the quote to use around the text. This is part of the argument of
  // the -draw command and thus should NOT be the shell argument enclosing
  // character.
  $quote = strstr($_SERVER['SERVER_SOFTWARE'], 'Win32') || strstr($_SERVER['SERVER_SOFTWARE'], 'IIS') ? "'" : '"';
  // and subsequently escape the use of that quote within the text.
  $text = $params['text'];
  $text = str_replace($quote, "\\$quote", $text);

  $image->ops[] = '-font ' . escapeshellarg($params['fontpath']);
  $image->ops[] = "-pointsize {$params['size']}";
  $image->ops[] = '-fill ' . escapeshellarg($color);
  // See issue http://drupal.org/node/1561214, Bootstrap should reset locale settings to UTF-8.
  setlocale(LC_ALL, 'C.UTF-8');
  $image->ops[] = '-draw ' . escapeshellarg("gravity $gravity translate {$params['xpos']},{$params['ypos']} rotate {$params['angle']} text 0,0 $quote$text$quote");

  return TRUE;
}


/**
 * UTILITY
 */

/**
 * Convert a position into an offset in pixels.
 *
 * Position may be a number of additions and/or subtractions of:
 * - An value, positive or negative, in pixels.
 * - A, positive or negative, percentage (%). The given percentage of the
 *   current dimension will be taken.
 * - 1 of the keywords:
 *   * top: 0
 *   * bottom: the height of the current image
 *   * left: 0
 *   * right: the width of the current image
 *   * center: 50% (of the current dimension)
 * Examples:
 * 0, 20, -20, 90%, 33.3% + 10, right, center - 20, 300 - center, bottom - 50.
 * Note:
 * The algorithm will accept many more situations, though the result may be hard
 * to predict.
 *
 * @param int $width
 *   The length of the horizontal dimension.
 * @param int $height
 *   The length of the vertical dimension.
 * @param int $length
 *   The length of the current dimension (should be either width or height).
 * @param string $position
 *   The string defining the position.
 *
 * @return number
 *   The computed offset in pixels.
 */
function image_effects_text_get_offset($position, $width, $height, $length) {
  $value = 0;

  $tokens = preg_split('/ *(-|\+) */', $position, 0, PREG_SPLIT_DELIM_CAPTURE);
  $sign = 1;
  foreach ($tokens as $token) {
    switch ($token) {
      case '+';
        // Ignore, doesn't change the sign
        break;
      case '-';
        // Flip the sign.
        $sign = -$sign;
        break;
      case 'top':
      case 'left':
        // Actually, top and left are a no-op.
        $value += $sign * 0;
        $sign = 1;
        break;
      case 'bottom':
        // Use height of the image, even if this is for the horizontal position.
        $value += $sign * $height;
        $sign = 1;
        break;
      case 'right':
        // Use width of the image, even if this is for the vertical position.
        $value += $sign * $width;
        $sign = 1;
        break;
      case 'center':
        // half the current dimension as provided by $length.
        $value += $sign * $length/2;
        $sign = 1;
        break;
      default:
        // Value: absolute or percentage
        if (substr($token, -strlen('%')) === '%') {
          $percentage = ((float) substr($token, 0, -strlen('%'))) / 100.0;
          $value += $sign * ($percentage * $length);
        }
        else {
          $value += $sign * (float) $token;
        }
        $sign = 1;
        break;
    }
  }
  return $value;
}

/**
 * Get the text to use for this image.
 *
 * @param object $image
 *   The image the current effect is to be applied to.
 * @param array $data
 *   An array containing the effect data.
 *
 * @return string
 *   Plain string to be placed on the image.
 */
function image_effects_text_get_text($image, $data) {
  if ($data['text_source'] === 'text') {
    $text = $data['text'];
  }
  else {
    // Get context about the image.
    $image_context = imagecache_actions_get_image_context($image, $data);

    if ($data['text_source'] === 'alt' || $data['text_source'] === 'title') {
      // Existence of an image field is not guaranteed, so check for that first.
      $text = isset($image_context['image_field'][$data['text_source']]) ? $image_context['image_field'][$data['text_source']] : '';
    }
    else { // $data['text_source'] === 'php'
      // Process the php using php_eval (rather than eval), but with GLOBAL
      // variables, so they can be passed successfully.
      $GLOBALS['image_context'] = $image_context;
      $GLOBALS['image'] = $image;
      // We don't need to check_plain() the resulting text, as the text is not
      // rendered in a browser but processed on the server.
      $text = module_exists('php') ? php_eval('<'.'?php global $image, $image_context; ' . $data['php'] . ' ?'.'>') : '';

      unset($GLOBALS['image']);
      unset($GLOBALS['image_context']);
    }
  }
  return $text;
}
