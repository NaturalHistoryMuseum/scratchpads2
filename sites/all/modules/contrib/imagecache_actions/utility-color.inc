<?php
/**
 * @file Utility functions for color widgets
 */

/**
 * Prepare a subform for displaying RGB fields
 *
 * Helper function to render a common element.
 *
 * Note that any module that re-uses this form also has to declare the theme
 * function in order to ensure it's in the registry.
 */
function imagecache_rgb_form($action) {
  if ($action['HEX'] && $deduced = imagecache_actions_hex2rgba($action['HEX'])) {
    $action = array_merge($action, $deduced);
    $action['HEX'] = ltrim($action['HEX'], '#');
    // With or without # is valid, but trim for consistancy
  }
  $form = array('#theme' => 'imagecacheactions_rgb_form');
  $form['farb'] = array('#weight' => -1); // Placeholder to get its weight right
  $form['HEX'] = array(
    '#type' => 'textfield',
    '#title' => t('HEX'),
    '#default_value' => $action['HEX'],
    '#size' => 7,
    '#element_validate' => array('imagecache_rgb_validate'),
  );

  return $form;
}

/**
 * Element validate handler to ensure a hexadecimal color value.
 * 
 * The core image_effect_color_validate() was not intelligent enough. 
 * Actually parse the value in order to see if it is parsable.
 */
function imagecache_rgb_validate($element, &$form_state) {
  if ($element['#value'] != '') {
    $rgba_value = imagecache_actions_hex2rgba($element['#value']);
    if (!$rgba_value) {
      // Returning NULL means a parse error
      form_error($element, t('!name must be a hexadecimal color value.', array('!name' => $element['#title'])));
    }
  }
}

/**
 * Render the subform in a table
 */
function theme_imagecacheactions_rgb_form($variables) {
  $form = $variables['form'];
  // Add a farb element
  drupal_add_css('misc/farbtastic/farbtastic.css', array('preprocess' => FALSE));
  drupal_add_js('misc/farbtastic/farbtastic.js');
  //  drupal_add_js(drupal_get_path('module', 'imagecache_coloractions') . '/color.js');

  $hex_id = $form['HEX']['#id'];
  $form['farb'] = array(
    '#value' => "<div id=\"$hex_id-farb\" style=\"float:right\"></div>",
    '#weight' => -1,
  );

  // Adds the JS that binds the textarea with the farb element
  $js = "
  $(document).ready(function() {
    farbify($('#$hex_id'), '#$hex_id-farb');
  });

  function farbify(elt, wrapper) {
    var farb = $.farbtastic(wrapper);
    farb.linkTo(function(color) {
        elt
          .css('background-color', color)
          .css('color', this.hsl[2] > 0.5 ? '#000' : '#fff')
          .val(color.substring(1));
      });
    farb.setColor('#' + elt.val());
    elt.bind('keyup', function(){ updateColor(elt, farb); });
  }
  function updateColor(elt, farb) {
    var text = elt.val();
    if (text.length == 6)
      farb.setColor('#' + text);
  }

  ";
  drupal_add_js($js, array('type' => 'inline', 'scope' => JS_DEFAULT));
  $output = drupal_render_children($form);
  return $output;
}


/**
 * @todo Please document this function.
 * @see http://drupal.org/node/1354
 */
function theme_imagecacheactions_rgb($variables) {
  $rgb = $variables['RGB'];
  if ($rgb['HEX']) {
    return " <span style=\"width:2em; border:1px solid white; background-color:#{$rgb['HEX']}\" >&nbsp;#{$rgb['HEX']}&nbsp;</span>";
  }
  else {
    return ' ' . t('Transparent');
  }
}

