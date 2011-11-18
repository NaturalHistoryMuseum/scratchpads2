<?php

/**
 * @file
 * Provides the administration settings for the Flowplayer Drupal module.
 */

/**
 * Administration settings for the Flowplayer Drupal module.
 */
function flowplayer_admin_settings() {
  $form = array();
  $form['flowplayer_key'] = array(
    '#type' => 'textfield',
    '#title' => t('License Key'),
    '#description' => t('The optional <a href="!commercial">commercial license key</a> associated with your Flowplayer account.', array('!commercial' => 'http://flowplayer.org/documentation/commercial.html')),
    '#default_value' => variable_get('flowplayer_key', ''),
  );

  $form['behavior'] = array(
    '#type' => 'fieldset',
    '#title' => t('Behavior'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );
  // Add scaling property.
  $form['behavior']['flowplayer_scaling'] = array(
    '#type' => 'select',
    '#title' => t('Scaling'),
    '#options' => array(
      'fit' => t('Fit to window: Preserves aspect ratio'),
      'half' => t('Half size: Preserves aspect ratio'),
      'orig' => t('Original size: Scaled to fit if too large'),
      'scale' => t('Scale: Fill all available space, ignoring metadata'),
    ),
    '#description' => t('The <a href="@options">scaling-property</a> for the video. Scale is the default option.', array('@options' => 'http://flowplayer.org/documentation/configuration/clips.html#scaling')),
    '#default_value' => variable_get('flowplayer_scaling', 'scale'),
  );

  // Construct the color picker
  $form['color'] = array(
    '#type' => 'fieldset',
    '#title' => t('Player colors'),
    '#attributes' => array('id' => 'flowplayer-color'),
  );
  $form['color']['picker'] = array(
    '#type' => 'markup',
    '#markup' => '<div id="flowplayer-color-picker"></div>',
  );

  // Add Farbtastic color picker and the Flowplayer administration JS/CSS
  drupal_add_css('misc/farbtastic/farbtastic.css', 'module', 'all', FALSE);
  drupal_add_js('misc/farbtastic/farbtastic.js');
  drupal_add_js(drupal_get_path('module', 'flowplayer') . '/flowplayer.admin.js');
  drupal_add_css(drupal_get_path('module', 'flowplayer') . '/flowplayer.admin.css');

  // The selector controls
  $names = array(
    'backgroundColor' => t('Control bar'),
    'sliderColor' => t('Sliders'),
    'buttonColor' => t('Buttons'),
    'buttonOverColor' => t('Mouseover'),
    'durationColor' => t('Total time'),
    'timeColor' => t('Time'),
    'progressColor' => t('Progress'),
    'bufferColor' => t('Buffer'),
  );
  foreach ($names as $name => $clean_name) {
    $form['color']['flowplayer_color_' . $name] = array(
      '#type' => 'textfield',
      '#title' => $clean_name,
      '#default_value' => variable_get('flowplayer_color_' . $name, ''),
      '#size' => 7,
      '#maxlength' => 7,
      '#attributes' => array('rel' => $name),
    );
  }

  // Construct the styling
  $form['styling'] = array(
    '#type' => 'fieldset',
    '#title' => t('Controlbar styling'),
    '#attributes' => array('id' => 'flowplayer-styling'),
  );
  $form['styling']['flowplayer_buttons'] = array(
    '#type' => 'checkboxes',
    '#title' => t('Buttons'),
    '#options' => array(
      'stop' => t('Stop'),
      'play' => t('Play/Pause'),
      'scrubber' => t('Scrubber'),
      'time' => t('Time'),
      'mute' => t('Mute'),
      'volume' => t('Volume'),
      'fullscreen' => t('Fullscreen'),
    ),
    '#default_value' => variable_get('flowplayer_buttons', array('play', 'scrubber', 'time', 'mute', 'volume', 'fullscreen')),
  );
  $form['styling']['flowplayer_background_gradient'] = array(
    '#type' => 'select',
    '#title' => t('Gradient'),
    '#options' => array(
      'none' => t('None'),
      'low' => t('Low'),
      'medium' => t('Medium'),
      'high' => t('High'),
    ),
    '#default_value' => variable_get('flowplayer_background_gradient', 'medium'),
  );
  $form['styling']['flowplayer_border_radius'] = array(
    '#type' => 'select',
    '#title' => t('Border Radius'),
    '#options' => drupal_map_assoc(array(0, 10, 15, 23)),
    '#default_value' => variable_get('flowplayer_border_radius', 0),
  );

  // Create the preview
  $config = array(
    'clip' => array(
      'url' => 'http://pseudo01.hddn.com/vod/demo.flowplayervod/flowplayer-700.flv',
      'autoPlay' => FALSE,
    ),
    'onLoad' => 'flowplayerAdminInit',
  );
  $form['preview'] = array(
    '#type' => 'fieldset',
    '#title' => t('Preview'),
    '#description' => theme('flowplayer', array('config' => $config, 'id' => 'flowplayer-preview', 'attributes' => array('style' => 'width:640px;height:380px;'))),
    '#attributes' => array('id' => 'flowplayer-preview-wrapper'),
  );

  return system_settings_form($form);
}