<?php

/**
 * @file
 * Administrative page callbacks for the colorbox module.
 */

/**
 * General configuration form for controlling the colorbox behaviour.
 */
function colorbox_admin_settings() {
  drupal_add_js(drupal_get_path('module', 'colorbox') . '/js/colorbox_admin_settings.js', array('preprocess' => FALSE));

  $colorbox_path = colorbox_get_path();

  $form['colorbox_plugin_settings'] = array(
    '#type' => 'fieldset',
    '#title' => t('Colorbox plugin settings'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE
  );
  $form['colorbox_plugin_settings']['colorbox_path'] = array(
    '#type' => 'textfield',
    '#title' => t('Path'),
    '#default_value' => $colorbox_path,
    '#description' => t('The location where Colorbox plugin is installed. Relative paths are from the Drupal root directory.'),
    '#after_build' => array('_colorbox_admin_settings_check_plugin_path'),
  );
  $form['colorbox_plugin_settings']['colorbox_compression_type'] = array(
    '#type' => 'radios',
    '#title' => t('Choose Colorbox compression level'),
    '#options' => array(
      'min' => t('Production (Minified)'),
      'none' => t('Development (Uncompressed Code)'),
    ),
    '#default_value' => variable_get('colorbox_compression_type', 'min'),
  );

  if (module_exists('image_old')) {
    $form['colorbox_image_module'] = array(
      '#type' => 'fieldset',
      '#title' => t('Image module settings'),
    );
    $form['colorbox_image_module']['colorbox_auto_image_nodes'] = array(
      '#type' => 'radios',
      '#title' => t('Image module integration'),
      '#options' => array(0 => t('Disabled'), 1 => t('Enabled')),
      '#default_value' => variable_get('colorbox_auto_image_nodes', 0),
      '#description' => t('Select if Colorbox should be activated for the Image module.'),
    );
    $derivative_options = array();
    $sizes = image_get_sizes();
    foreach ($sizes as $label => $size) {
      $derivative_options[$label] = $size['label'];
    }
    $form['colorbox_image_module']['colorbox_image_derivative'] = array(
      '#type' => 'select',
      '#title' => t('Image derivative'),
      '#options' => $derivative_options,
      '#default_value' => variable_get('colorbox_image_derivative', 'preview'),
      '#description' => t('Select which image derivative to load in the Colorbox.'),
    );
  }

  if (module_exists('insert')) {
    $form['colorbox_insert_module'] = array(
      '#type' => 'fieldset',
      '#title' => t('Insert module settings'),
    );
    $image_styles = image_style_options(FALSE);
    $form['colorbox_insert_module']['colorbox_image_style'] = array(
      '#type' => 'select',
      '#title' => t('Image style'),
      '#empty_option' => t('None (original image)'),
      '#options' => $image_styles,
      '#default_value' => variable_get('colorbox_image_style', ''),
      '#description' => t('Select which image style to use for viewing images in the colorbox.'),
    );
    $form['colorbox_insert_module']['colorbox_insert_gallery'] = array(
      '#type' => 'radios',
      '#title' => t('Insert image gallery'),
      '#default_value' => variable_get('colorbox_insert_gallery', 0),
      '#options' => array(0 => t('Per page gallery'), 3 => t('No gallery')),
      '#description' => t('Should the gallery be all images on the page (default) or disabled.'),
    );
  }

  $form['colorbox_login_settings'] = array(
    '#type' => 'fieldset',
    '#title' => t('Login links settings')
  );
  $form['colorbox_login_settings']['colorbox_login'] = array(
    '#type' => 'checkbox',
    '#title' => t('Enable for login links'),
    '#default_value' => variable_get('colorbox_login', 0),
    '#description' => t('Automatically activate Colorbox for links to user/login.'),
  );
  $form['colorbox_login_settings']['colorbox_login_links'] = array(
    '#type' => 'radios',
    '#title' => t('Display links'),
    '#options' => array(0 => t('No links'), 1 => t('Show links'), 2 => t('Show links and open them in a Colorbox')),
    '#default_value' => variable_get('colorbox_login_links', 0),
    '#description' => t('Display the "Create new account" (if allowed) and "Request new password" links below the login form.'),
    '#after_build' => array('_colorbox_admin_settings_login_links_load'),
    '#states' => array(
      'visible' => array(
        ':input[name="colorbox_login"]' => array('checked' => TRUE),
      ),
    ),
  );

  $form['colorbox_extra_settings'] = array(
    '#type' => 'fieldset',
    '#title' => t('Extra settings'),
  );
  $form['colorbox_extra_settings']['colorbox_load'] = array(
    '#type' => 'checkbox',
    '#title' => t('Enable Colorbox load'),
    '#default_value' => variable_get('colorbox_load', 0),
    '#description' => t('This enables custom links that can open forms and paths in a Colorbox. Add the class "colorbox-load" to the link and build the url like this for forms "/colorbox/form/[form_id]?destination=some_path&width=500&height=500" and like this for paths "[path]?width=500&height=500&iframe=true" or "[path]?width=500&height=500" if you don\'t want an iframe. Other modules may activate this for easy Colorbox integration.'),
  );
  $form['colorbox_extra_settings']['colorbox_inline'] = array(
    '#type' => 'checkbox',
    '#title' => t('Enable Colorbox inline'),
    '#default_value' => variable_get('colorbox_inline', 0),
    '#description' => t('This enables custom links that can open inline content in a Colorbox. Add the class "colorbox-inline" to the link and build the url like this "?width=500&height=500&inline=true#id-of-content". Other modules may activate this for easy Colorbox integration.'),
  );

  $form['colorbox_custom_settings'] = array(
    '#type' => 'fieldset',
    '#title' => t('Styles and options'),
  );
  $colorbox_styles = array(
    'default' => t('Default'),
    'stockholmsyndrome' => t('Stockholm Syndrome'),
    $colorbox_path . '/example1' => t('Example 1'),
    $colorbox_path . '/example2' => t('Example 2'),
    $colorbox_path . '/example3' => t('Example 3'),
    $colorbox_path . '/example4' => t('Example 4'),
    $colorbox_path . '/example5' => t('Example 5'),
    'none' => t('None'),
  );
  $form['colorbox_custom_settings']['colorbox_style'] = array(
    '#type' => 'select',
    '#title' => t('Style'),
    '#options' => $colorbox_styles,
    '#default_value' => variable_get('colorbox_style', 'default'),
    '#description' => t('Select the style to use for the Colorbox. The example styles are the ones that come with the Colorbox plugin. Select "None" if you have added Colorbox styles to your theme.'),
  );
  $form['colorbox_custom_settings']['colorbox_custom_settings_activate'] = array(
    '#type' => 'radios',
    '#title' => t('Options'),
    '#options' => array(0 => t('Default'), 1 => t('Custom')),
    '#default_value' => variable_get('colorbox_custom_settings_activate', 0),
    '#description' => t('Use the default or custom options for Colorbox.'),
    '#prefix' => '<div class="colorbox-custom-settings-activate">',
    '#suffix' => '</div>',
  );

  $js_hide = variable_get('colorbox_custom_settings_activate', 0) ? '' : ' js-hide';
  $form['colorbox_custom_settings']['wrapper_start'] = array(
    '#markup' => '<div class="colorbox-custom-settings' . $js_hide . '">',
  );

  $form['colorbox_custom_settings']['colorbox_transition_type'] = array(
    '#type' => 'radios',
    '#title' => t('Transition type'),
    '#options' => array('elastic' => t('Elastic'), 'fade' => t('Fade'), 'none' => t('None')),
    '#default_value' => variable_get('colorbox_transition_type', 'elastic'),
    '#description' => t('The transition type.'),
  );
  $form['colorbox_custom_settings']['colorbox_transition_speed'] = array(
    '#type' => 'select',
    '#title' => t('Transition speed'),
    '#options' => drupal_map_assoc(array(100, 150, 200, 250, 300, 350, 400, 450, 500, 550, 600)),
    '#default_value' => variable_get('colorbox_transition_speed', 350),
    '#description' => t('Sets the speed of the fade and elastic transitions, in milliseconds.'),
  );
  $form['colorbox_custom_settings']['colorbox_opacity'] = array(
    '#type' => 'select',
    '#title' => t('Opacity'),
    '#options' => drupal_map_assoc(array('0', '0.10', '0.15', '0.20', '0.25', '0.30', '0.35', '0.40', '0.45', '0.50', '0.55', '0.60', '0.65', '0.70', '0.75', '0.80', '0.85', '0.90', '0.95', '1')),
    '#default_value' => variable_get('colorbox_opacity', '0.85'),
    '#description' => t('The overlay opacity level. Range: 0 to 1.'),
  );
  $form['colorbox_custom_settings']['colorbox_text_current'] = array(
    '#type' => 'textfield',
    '#title' => t('Current'),
    '#default_value' => variable_get('colorbox_text_current', '{current} of {total}'),
    '#size' => 30,
    '#description' => t('Text format for the content group / gallery count. {current} and {total} are detected and replaced with actual numbers while Colorbox runs.'),
  );
  $form['colorbox_custom_settings']['colorbox_text_previous'] = array(
    '#type' => 'textfield',
    '#title' => t('Previous'),
    '#default_value' => variable_get('colorbox_text_previous', '« Prev'),
    '#size' => 30,
    '#description' => t('Text for the previous button in a shared relation group.'),
  );
  $form['colorbox_custom_settings']['colorbox_text_next'] = array(
    '#type' => 'textfield',
    '#title' => t('Next'),
    '#default_value' => variable_get('colorbox_text_next', 'Next »'),
    '#size' => 30,
    '#description' => t('Text for the next button in a shared relation group.'),
  );
  $form['colorbox_custom_settings']['colorbox_text_close'] = array(
    '#type' => 'textfield',
    '#title' => t('Close'),
    '#default_value' => variable_get('colorbox_text_close', 'Close'),
    '#size' => 30,
    '#description' => t('Text for the close button. The "Esc" key will also close Colorbox.'),
  );
  $form['colorbox_custom_settings']['colorbox_overlayclose'] = array(
    '#type' => 'checkbox',
    '#title' => t('Overlay close'),
    '#default_value' => variable_get('colorbox_overlayclose', 1),
    '#description' => t('Enable closing ColorBox by clicking on the background overlay.'),
  );
  $form['colorbox_custom_settings']['colorbox_maxwidth'] = array(
    '#type' => 'textfield',
    '#title' => t('Max width'),
    '#default_value' => variable_get('colorbox_maxwidth', '100%'),
    '#size' => 30,
    '#description' => t('Set a maximum width for loaded content. Example: "100%", 500, "500px".'),
  );
  $form['colorbox_custom_settings']['colorbox_maxheight'] = array(
    '#type' => 'textfield',
    '#title' => t('Max height'),
    '#default_value' => variable_get('colorbox_maxheight', '100%'),
    '#size' => 30,
    '#description' => t('Set a maximum height for loaded content. Example: "100%", 500, "500px".'),
  );
  $form['colorbox_custom_settings']['colorbox_initialwidth'] = array(
    '#type' => 'textfield',
    '#title' => t('Initial width'),
    '#default_value' => variable_get('colorbox_initialwidth', '300'),
    '#size' => 30,
    '#description' => t('Set the initial width, prior to any content being loaded. Example: "100%", 500, "500px".'),
  );
  $form['colorbox_custom_settings']['colorbox_initialheight'] = array(
    '#type' => 'textfield',
    '#title' => t('Initial height'),
    '#default_value' => variable_get('colorbox_initialheight', '100'),
    '#size' => 30,
    '#description' => t('Set the initial height, prior to any content being loaded. Example: "100%", 500, "500px".'),
  );
  $form['colorbox_custom_settings']['colorbox_fixed'] = array(
    '#type' => 'checkbox',
    '#title' => t('Fixed'),
    '#default_value' => variable_get('colorbox_fixed', 1),
    '#description' => t('If the ColorBox should be displayed in a fixed position within the visitor\'s viewport or relative to the document.'),
  );

  $form['colorbox_custom_settings']['colorbox_slideshow_settings'] = array(
    '#type' => 'fieldset',
    '#title' => t('Slideshow settings'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE
  );
  $form['colorbox_custom_settings']['colorbox_slideshow_settings']['colorbox_slideshow'] = array(
    '#type' => 'radios',
    '#title' => t('Slideshow'),
    '#options' => array(0 => t('Off'), 1 => t('On')),
    '#default_value' => variable_get('colorbox_slideshow', 0),
    '#description' => t('An automatic slideshow to a content group / gallery.'),
    '#prefix' => '<div class="colorbox-slideshow-settings-activate">',
    '#suffix' => '</div>',
  );
  $form['colorbox_custom_settings']['colorbox_scrolling'] = array(
    '#type' => 'radios',
    '#title' => t('Scrollbars'),
    '#options' => array(0 => t('Off'), 1 => t('On')),
    '#default_value' => variable_get('colorbox_scrolling', 1),
    '#description' => t('If false, Colorbox will hide scrollbars for overflowing content. This could be used on conjunction with the resize method for a smoother transition if you are appending content to an already open instance of Colorbox.'),
  );

  $js_hide = variable_get('colorbox_slideshow', 0) ? '' : ' js-hide';
  $form['colorbox_custom_settings']['colorbox_slideshow_settings']['wrapper_start'] = array(
    '#markup' => '<div class="colorbox-slideshow-settings' . $js_hide . '">',
  );

  $form['colorbox_custom_settings']['colorbox_slideshow_settings']['colorbox_slideshowauto'] = array(
    '#type' => 'checkbox',
    '#title' => t('Slideshow autostart'),
    '#default_value' => variable_get('colorbox_slideshowauto', 1),
    '#description' => t('If the slideshow should automatically start to play.'),
  );
  $form['colorbox_custom_settings']['colorbox_slideshow_settings']['colorbox_slideshowspeed'] = array(
    '#type' => 'select',
    '#title' => t('Slideshow speed'),
    '#options' => drupal_map_assoc(array(1000, 1500, 2000, 2500, 3000, 3500, 4000, 4500, 5000, 5500, 6000)),
    '#default_value' => variable_get('colorbox_slideshowspeed', 2500),
    '#description' => t('Sets the speed of the slideshow, in milliseconds.'),
  );
  $form['colorbox_custom_settings']['colorbox_slideshow_settings']['colorbox_text_start'] = array(
    '#type' => 'textfield',
    '#title' => t('Start slideshow'),
    '#default_value' => variable_get('colorbox_text_start', 'start slideshow'),
    '#size' => 30,
    '#description' => t('Text for the slideshow start button.'),
  );
  $form['colorbox_custom_settings']['colorbox_slideshow_settings']['colorbox_text_stop'] = array(
    '#type' => 'textfield',
    '#title' => t('Stop slideshow'),
    '#default_value' => variable_get('colorbox_text_stop', 'stop slideshow'),
    '#size' => 30,
    '#description' => t('Text for the slideshow stop button.'),
  );

  $form['colorbox_custom_settings']['colorbox_slideshow_settings']['wrapper_stop'] = array(
    '#markup' => '</div>',
  );

  $form['colorbox_custom_settings']['wrapper_stop'] = array(
    '#markup' => '</div>',
  );

  $form['colorbox_advanced_settings'] = array(
    '#type' => 'fieldset',
    '#title' => t('Advanced settings'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE
  );
  $form['colorbox_advanced_settings']['colorbox_caption_trim'] = array(
    '#type' => 'radios',
    '#title' => t('Caption shortening'),
    '#options' => array(0 => t('Default'), 1 => t('Yes')),
    '#default_value' => variable_get('colorbox_caption_trim', 0),
    '#description' => t('If the caption should be made shorter in the Colorbox to avoid layout problems. The default is to shorten for the example styles, they need it, but not for other styles.'),
  );
  $form['colorbox_advanced_settings']['colorbox_caption_trim_length'] = array(
    '#type' => 'select',
    '#title' => t('Caption max length'),
    '#options' => drupal_map_assoc(array(40, 45, 50, 55, 60, 70, 75, 80, 85, 90, 95, 100, 105, 110, 115, 120)),
    '#default_value' => variable_get('colorbox_caption_trim_length', 75),
    '#states' => array(
      'visible' => array(
        ':input[name="colorbox_caption_trim"]' => array('value' => '1'),
      ),
    ),
  );
  $form['colorbox_advanced_settings']['colorbox_pages'] = array(
    '#type' => 'textarea',
    '#title' => t('Deactivate Colorbox on specific pages'),
    '#default_value' => variable_get('colorbox_pages', "admin*\nimg_assist*\nimce*\nnode/add/*\nnode/*/edit"),
    '#description' => t("Enter one page per line as Drupal paths. The '*' character is a wildcard. Example paths are %blog for the blog page and %blog-wildcard for every personal blog. %front is the front page.", array('%blog' => 'blog', '%blog-wildcard' => 'blog/*', '%front' => '<front>')),
  );

  return system_settings_form($form);
}

/**
 * Checks if the directory in $form_element exists and contains a file named
 * 'jquery.colorbox.js'. If validation fails, the form element is flagged
 * with an error from within the file_check_directory function.
 *
 * @param $form_element
 *   The form element containing the name of the directory to check.
 */
function _colorbox_admin_settings_check_plugin_path($form_element) {
  $library_path = $form_element['#value'];
  if (!is_dir($library_path) || !(file_exists($library_path . '/colorbox/jquery.colorbox.js') && file_exists($library_path . '/colorbox/jquery.colorbox-min.js'))) {
    form_set_error($form_element['#parents'][0], t('You need to download the !colorbox and extract the entire contents of the archive into the %path folder of your server.', array('!colorbox' => l(t('Colorbox plugin'), 'http://colorpowered.com/colorbox/'), '%path' => $library_path)));
  }

  return $form_element;
}

/**
 * Enable the Colorbox load feature of the login links are set to open in a Colorbox.
 */
function _colorbox_admin_settings_login_links_load($form_element) {
  $login_links = $form_element['#value'];
  if ($login_links == 2) {
    variable_set('colorbox_load', 1);
  }

  return $form_element;
}
