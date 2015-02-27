<?php

/**
 * @file
 * The default options available with Views Slideshow: cycle.
 */

/**
 * Implements hook_views_slideshow_slideshow_info().
 */
function views_slideshow_cycle_views_slideshow_slideshow_info() {
  $options = array(
    'views_slideshow_cycle' => array(
      'name' => t('Cycle'),
      'accepts' => array(
        'goToSlide',
        'nextSlide',
        'pause',
        'play',
        'previousSlide',
      ),
      'calls' => array(
        'transitionBegin',
        'transitionEnd',
        'goToSlide',
        'pause',
        'play',
        'nextSlide',
        'previousSlide',
      ),
    ),
  );
  return $options;
}

/**
 * Implements hook_views_slideshow_option_definition().
 */
function views_slideshow_cycle_views_slideshow_option_definition() {
  $options['views_slideshow_cycle'] = array(
    'contains' => array(
      // Transition
      'effect' => array('default' => 'fade'),
      'transition_advanced' => array('default' => 0),
      'timeout' => array('default' => 5000),
      'speed' => array('default' => 700), //normal
      'delay' => array('default' => 0),
      'sync' => array('default' => 1),
      'random' => array('default' => 0),

      // Action
      'pause' => array('default' => 1),
      'pause_on_click' => array('default' => 0),
      'action_advanced' => array('default' => 0),
      'start_paused' => array('default', 0),
      'remember_slide' => array('default' => 0),
      'remember_slide_days' => array('default' => 1),
      'pause_in_middle' => array('default' => 0),
      'pause_when_hidden' => array('default' => 0),
      'pause_when_hidden_type' => array('default' => 'full'),
      'amount_allowed_visible' => array('default' => ''),
      'nowrap' => array('default' => 0),
      'fixed_height' => array('default' => 1),
      'items_per_slide' => array('default' => 1),
      'wait_for_image_load' => array('default' => 1),
      'wait_for_image_load_timeout' => array('default' => 3000),

      // Internet Explorer Tweaks
      'cleartype' => array('default' => 'true'),
      'cleartypenobg' => array('default' => 'false'),

      // Advanced
      'advanced_options' => array('default' => '{}'),
    ),
  );
  return $options;
}

/**
 * Implements hook_views_slideshow_slideshow_slideshow_type_form().
 */
function views_slideshow_cycle_views_slideshow_slideshow_type_form(&$form, &$form_state, &$view) {
  if (!$cycle_path = _views_slideshow_cycle_library_path()) {
    $form['views_slideshow_cycle']['no_cycle_js'] = array(
      '#markup' => '<div style="color: red">' . t('You need to install the jQuery cycle plugin. Create a directory in sites/all/libraries called jquery.cycle, and then copy jquery.cycle.all.min.js or jquery.cycle.all.js into it. You can find the plugin at !url.', array('!url' => l('http://malsup.com/jquery/cycle', 'http://malsup.com/jquery/cycle', array('attributes' => array('target' => '_blank'))))) . '</div>',
    );
  }

  // Transition
  $form['views_slideshow_cycle']['transition'] = array(
    '#markup' => '<h2>' . t('Transition') . '</h2>',
  );

  $effects = array(
    'none' => 'none',
    'blindX' => 'blindX',
    'blindY' => 'blindY',
    'blindZ' => 'blindZ',
    'cover' => 'cover',
    'curtainX' => 'curtainX',
    'curtainY' => 'curtainY',
    'fade' => 'fade',
    'fadeZoom' => 'fadeZoom',
    'growX' => 'growX',
    'growY' => 'growY',
    'scrollUp' => 'scrollUp',
    'scrollDown' => 'scrollDown',
    'scrollLeft' => 'scrollLeft',
    'scrollRight' => 'scrollRight',
    'scrollHorz' => 'scrollHorz',
    'scrollVert' => 'scrollVert',
    'shuffle' => 'shuffle',
    'slideX' => 'slideX',
    'slideY' => 'slideY',
    'toss' => 'toss',
    'turnUp' => 'turnUp',
    'turnDown' => 'turnDown',
    'turnLeft' => 'turnLeft',
    'turnRight' => 'turnRight',
    'uncover' => 'uncover',
    'wipe' => 'wipe',
    'zoom' => 'zoom'
  );
  $form['views_slideshow_cycle']['effect'] = array(
    '#type' => 'select',
    '#title' => t('Effect'),
    '#options' => $effects,
    '#default_value' => $view->options['views_slideshow_cycle']['effect'],
    '#description' => t('The transition effect that will be used to change between images. Not all options below may be relevant depending on the effect. ' . l('Follow this link to see examples of each effect.', 'http://jquery.malsup.com/cycle/browser.html', array('attributes' => array('target' => '_blank')))),
  );

  // Transition advanced options
  $form['views_slideshow_cycle']['transition_advanced'] = array(
    '#type' => 'checkbox',
    '#title' => t('View Transition Advanced Options'),
    '#default_value' => $view->options['views_slideshow_cycle']['transition_advanced'],
  );

  // Need to wrap this so it indents correctly.
  $form['views_slideshow_cycle']['transition_advanced_wrapper'] = array(
    '#markup' => '<div class="vs-dependent">',
  );

  $form['views_slideshow_cycle']['timeout'] = array(
    '#type' => 'textfield',
    '#title' => t('Timer delay'),
    '#default_value' => $view->options['views_slideshow_cycle']['timeout'],
    '#description' => t('Amount of time in milliseconds between transitions. Set the value to 0 to not rotate the slideshow automatically.'),
    '#states' => array(
      'visible' => array(
        ':input[name="style_options[views_slideshow_cycle][transition_advanced]"]' => array('checked' => TRUE),
      ),
    ),
  );
  $form['views_slideshow_cycle']['speed'] = array(
    '#type' => 'textfield',
    '#title' => t('Speed'),
    '#default_value' => $view->options['views_slideshow_cycle']['speed'],
    '#description' => t('Time in milliseconds that each transition lasts. Numeric only!'),
    '#states' => array(
      'visible' => array(
        ':input[name="style_options[views_slideshow_cycle][transition_advanced]"]' => array('checked' => TRUE),
      ),
    ),
  );
  $form['views_slideshow_cycle']['delay'] = array(
    '#type' => 'textfield',
    '#title' => t('Initial slide delay offset'),
    '#default_value' => $view->options['views_slideshow_cycle']['delay'],
    '#description' => t('Amount of time in milliseconds for the first slide to transition. This number will be added to Timer delay to create the initial delay.  For example if Timer delay is 4000 and Initial delay is 2000 then the first slide will change at 6000ms (6 seconds).  If Initial delay is -2000 then the first slide will change at 2000ms (2 seconds).'),
    '#states' => array(
      'visible' => array(
        ':input[name="style_options[views_slideshow_cycle][transition_advanced]"]' => array('checked' => TRUE),
      ),
    ),
  );
  $form['views_slideshow_cycle']['sync'] = array(
    '#type' => 'checkbox',
    '#title' => t('Sync'),
    '#default_value' => $view->options['views_slideshow_cycle']['sync'],
    '#description' => t('The sync option controls whether the slide transitions occur simultaneously. The default is selected which means that the current slide transitions out as the next slide transitions in. By unselecting this option you can get some interesting twists on your transitions.'),
    '#states' => array(
      'visible' => array(
        ':input[name="style_options[views_slideshow_cycle][transition_advanced]"]' => array('checked' => TRUE),
      ),
    ),
  );
  $form['views_slideshow_cycle']['random'] = array(
    '#type' => 'checkbox',
    '#title' => t('Random'),
    '#description' => t('This option controls the order items are displayed. The default setting, unselected, uses the views ordering. Selected will cause the images to display in a random order.'),
    '#default_value' => $view->options['views_slideshow_cycle']['random'],
    '#states' => array(
      'visible' => array(
        ':input[name="style_options[views_slideshow_cycle][transition_advanced]"]' => array('checked' => TRUE),
      ),
    ),
  );

  $form['views_slideshow_cycle']['transition_advanced_wrapper_close'] = array(
    '#markup' => '</div>',
  );

  // Action
  $form['views_slideshow_cycle']['action'] = array(
    '#markup' => '<h2>' . t('Action') . '</h2>',
  );
  $form['views_slideshow_cycle']['pause'] = array(
    '#type' => 'checkbox',
    '#title' => t('Pause on hover'),
    '#default_value' => $view->options['views_slideshow_cycle']['pause'],
    '#description' => t('Pause when hovering on the slideshow image.'),
  );
  $form['views_slideshow_cycle']['pause_on_click'] = array(
    '#type' => 'checkbox',
    '#title' => t('Pause On Click'),
    '#default_value' => $view->options['views_slideshow_cycle']['pause_on_click'],
    '#description' => t('Pause when the slide is clicked.'),
  );

  // Action Advanced Options
  $form['views_slideshow_cycle']['action_advanced'] = array(
    '#type' => 'checkbox',
    '#title' => t('View Action Advanced Options'),
    '#default_value' => $view->options['views_slideshow_cycle']['action_advanced'],
  );

  // Need to wrap this so it indents correctly.
  $form['views_slideshow_cycle']['action_advanced_wrapper'] = array(
    '#markup' => '<div class="vs-dependent">',
  );

  $form['views_slideshow_cycle']['start_paused'] = array(
    '#type' => 'checkbox',
    '#title' => t('Start Slideshow Paused'),
    '#default_value' => $view->options['views_slideshow_cycle']['start_paused'],
    '#description' => t('Start the slideshow in the paused state.'),
    '#states' => array(
      'visible' => array(
        ':input[name="style_options[views_slideshow_cycle][action_advanced]"]' => array('checked' => TRUE),
      ),
    ),
  );
  $form['views_slideshow_cycle']['remember_slide'] = array(
    '#type' => 'checkbox',
    '#title' => t('Start On Last Slide Viewed'),
    '#default_value' => $view->options['views_slideshow_cycle']['remember_slide'],
    '#description' => t('When the user leaves a page with a slideshow and comes back start them on the last slide viewed.'),
    '#states' => array(
      'visible' => array(
        ':input[name="style_options[views_slideshow_cycle][action_advanced]"]' => array('checked' => TRUE),
      ),
    ),
  );
  $form['views_slideshow_cycle']['remember_slide_days'] = array(
    '#type' => 'textfield',
    '#title' => t('Length of Time to Remember Last Slide'),
    '#default_value' => $view->options['views_slideshow_cycle']['remember_slide_days'],
    '#description' => t('The number of days to have the site remember the last slide. Default is 1'),
    '#prefix' => '<div class="vs-dependent">',
    '#suffix' => '</div>',
    '#size' => 4,
    '#states' => array(
      'visible' => array(
        ':input[name="style_options[views_slideshow_cycle][action_advanced]"]' => array('checked' => TRUE),
        ':input[name="style_options[views_slideshow_cycle][remember_slide]"]' => array('checked' => TRUE),
      ),
    ),
  );

  if (!$pause_path = _views_slideshow_cycle_pause_library_path()) {
    $form['views_slideshow_cycle']['pause_in_middle'] = array(
      '#type' => 'checkbox',
      '#title' => t('Pause The Slideshow In The Middle of the Transition'),
      '#default_value' => $view->options['views_slideshow_cycle']['pause_in_middle'],
      '#description' => t('When pausing the slideshow allow it to pause in the middle of tranistioning and not finish the transition until unpaused.'),
      '#states' => array(
        'visible' => array(
          ':input[name="style_options[views_slideshow_cycle][transition_advanced]"]' => array('checked' => TRUE),
        ),
      ),
    );
  }

  $form['views_slideshow_cycle']['pause_when_hidden'] = array(
    '#type' => 'checkbox',
    '#title' => t('Pause When the Slideshow is Not Visible'),
    '#default_value' => $view->options['views_slideshow_cycle']['pause_when_hidden'],
    '#description' => t('When the slideshow is scrolled out of view or when a window is resized that hides the slideshow, this will pause the slideshow.'),
    '#states' => array(
      'visible' => array(
        ':input[name="style_options[views_slideshow_cycle][action_advanced]"]' => array('checked' => TRUE),
      ),
    ),
  );
  $form['views_slideshow_cycle']['pause_when_hidden_type'] = array(
    '#type' => 'select',
    '#title' => t('How to Calculate Amount of Slide that Needs to be Shown'),
    '#options' => array(
      'full' => t('Entire slide'),
      'vertical' => t('Set amount of vertical'),
      'horizontal' => t('Set amount of horizontal'),
      'area' => t('Set total area of the slide'),
    ),
    '#default_value' => $view->options['views_slideshow_cycle']['pause_when_hidden_type'],
    '#description' => t('Choose how to calculate how much of the slide has to be shown. Entire Slide: All the slide has to be shown. Vertical: Set amount of height that has to be shown. Horizontal: Set amount of width that has to be shown. Area: Set total area that has to be shown.'),
    '#prefix' => '<div class="vs-dependent">',
    '#suffix' => '</div>',
    '#states' => array(
      'visible' => array(
        ':input[name="style_options[views_slideshow_cycle][action_advanced]"]' => array('checked' => TRUE),
        ':input[name="style_options[views_slideshow_cycle][pause_when_hidden]"]' => array('checked' => TRUE),
      ),
    ),
  );
  $form['views_slideshow_cycle']['amount_allowed_visible'] = array(
    '#type' => 'textfield',
    '#title' => t('Amount of Slide Needed to be Shown'),
    '#default_value' => $view->options['views_slideshow_cycle']['amount_allowed_visible'],
    '#description' => t("The amount of the slide that needs to be shown to have it rotate. You can set the value in percentage (ex: 50%) or in pixels (ex: 250). The slidehsow will not rotate until it's height/width/total area, depending on the calculation method you have chosen above, is less than the value you have entered in this field."),
    '#size' => 4,
    '#process' => array('views_slideshow_cycle_form_options_js'),
  );
  $form['views_slideshow_cycle']['nowrap'] = array(
    '#type' => 'checkbox',
    '#title' => t('End slideshow after last slide'),
    '#default_value' => $view->options['views_slideshow_cycle']['nowrap'],
    '#description' => t('If selected the slideshow will end when it gets to the last slide.'),
    '#states' => array(
      'visible' => array(
        ':input[name="style_options[views_slideshow_cycle][action_advanced]"]' => array('checked' => TRUE),
      ),
    ),
  );
  $form['views_slideshow_cycle']['fixed_height'] = array(
    '#type' => 'checkbox',
    '#title' => t('Make the slide window height fit the largest slide'),
    '#default_value' => $view->options['views_slideshow_cycle']['fixed_height'],
    '#description' => t('If unselected then if the slides are different sizes the height of the slide area will change as the slides change.'),
    '#states' => array(
      'visible' => array(
        ':input[name="style_options[views_slideshow_cycle][action_advanced]"]' => array('checked' => TRUE),
      ),
    ),
  );
  $form['views_slideshow_cycle']['items_per_slide'] = array(
    '#type' => 'textfield',
    '#title' => t('Items per slide'),
    '#default_value' => $view->options['views_slideshow_cycle']['items_per_slide'],
    '#description' => t('The number of items per slide'),
    '#size' => 4,
    '#states' => array(
      'visible' => array(
        ':input[name="style_options[views_slideshow_cycle][action_advanced]"]' => array('checked' => TRUE),
      ),
    ),
  );
  $form['views_slideshow_cycle']['wait_for_image_load'] = array(
    '#type' => 'checkbox',
    '#title' => t('Wait for all the slide images to load'),
    '#default_value' => $view->options['views_slideshow_cycle']['wait_for_image_load'],
    '#description' => t('If selected the slideshow will not start unless all the slide images are loaded.  This will fix some issues on IE7/IE8/Chrome/Opera.'),
    '#states' => array(
      'visible' => array(
        ':input[name="style_options[views_slideshow_cycle][action_advanced]"]' => array('checked' => TRUE),
      ),
    ),
  );
  $form['views_slideshow_cycle']['wait_for_image_load_timeout'] = array(
    '#type' => 'textfield',
    '#title' => t('Timeout'),
    '#default_value' => $view->options['views_slideshow_cycle']['wait_for_image_load_timeout'],
    '#description' => t('How long should it wait until it starts the slideshow anyway. Time is in milliseconds.'),
    '#prefix' => '<div class="vs-dependent">',
    '#suffix' => '</div>',
    '#states' => array(
      'visible' => array(
        ':input[name="style_options[views_slideshow_cycle][action_advanced]"]' => array('checked' => TRUE),
        ':input[name="style_options[views_slideshow_cycle][wait_for_image_load]"]' => array('checked' => TRUE),
      ),
    ),
  );

  // Need to wrap this so it indents correctly.
  $form['views_slideshow_cycle']['action_advanced_wrapper_close'] = array(
    '#markup' => '</div>',
  );

  // Internet Explorer Tweaks
  $form['views_slideshow_cycle']['ie_tweaks'] = array(
    '#markup' => '<h2>' . t('Internet Explorer Tweaks') . '</h2>',
  );
  $form['views_slideshow_cycle']['cleartype'] = array(
    '#type' => 'checkbox',
    '#title' => t('ClearType'),
    '#default_value' => $view->options['views_slideshow_cycle']['cleartype'],
    '#description' => t('Select if clearType corrections should be applied (for IE).  Some background issues could be fixed by unselecting this option.'),
  );
  $form['views_slideshow_cycle']['cleartypenobg'] = array(
    '#type' => 'checkbox',
    '#title' => t('ClearType Background'),
    '#default_value' => $view->options['views_slideshow_cycle']['cleartypenobg'],
    '#description' => t('Select to disable extra cleartype fixing.  Unselect to force background color setting on slides)'),
  );




  // Advanced Options
  $form['views_slideshow_cycle']['advanced_options_header'] = array(
    '#markup' => '<h2>' . t('jQuery Cycle Custom Options') . '</h2>',
  );

  $json2_path = libraries_get_path('json2');
  if (empty($json2_path) || !file_exists($json2_path . '/json2.js')) {
    $form['views_slideshow_cycle']['no_json_js'] = array(
      '#markup' => '<div>' . t('To use the advanced options you need to download json2.js. You can do this by clicking the download button at !url and extract json2.js to sites/all/libraries/json2', array('!url' => l('https://github.com/douglascrockford/JSON-js', 'https://github.com/douglascrockford/JSON-js', array('attributes' => array('target' => '_blank'))))) . '</div>',
    );
  }
  else {
    $form['views_slideshow_cycle']['advanced_options_info'] = array(
      '#markup' => '<p>' . t('You can find a list of all the available options at !url.  If one of the options you add uses a function, example fxFn, then you need to only enter what goes inside the function call. The variables that are in the documentation on the jquery cycle site will be available to you.', array('!url' => l('http://malsup.com/jquery/cycle/options.html', 'http://malsup.com/jquery/cycle/options.html'))) . '</p>',
    );

    // All the jquery cycle options according to
    // http://malsup.com/jquery/cycle/options.html
    $cycle_options = array(
      0 => 'Select One',
      'activePagerClass' => 'activePagerClass',
      'after' => 'after',
      'allowPagerClickBubble' => 'allowPagerClickBubble',
      'animIn' => 'animIn',
      'animOut' => 'animOut',
      'autostop' => 'autostop',
      'autostopCount' => 'autostopCount',
      'backwards' => 'backwards',
      'before' => 'before',
      'bounce' => 'bounce',
      'cleartype' => 'cleartype',
      'cleartypeNoBg' => 'cleartypeNoBg',
      'containerResize' => 'containerResize',
      'continuous' => 'continuous',
      'cssAfter' => 'cssAfter',
      'cssBefore' => 'cssBefore',
      'delay' => 'delay',
      'easeIn' => 'easeIn',
      'easeOut' => 'easeOut',
      'easing' => 'easing',
      'end' => 'end',
      'fastOnEvent' => 'fastOnEvent',
      'fit' => 'fit',
      'fx' => 'fx',
      'fxFn' => 'fxFn',
      'height' => 'height',
      'manualTrump' => 'manualTrump',
      'metaAttr' => 'metaAttr',
      'next' => 'next',
      'nowrap' => 'nowrap',
      'onPagerEvent' => 'onPagerEvent',
      'onPrevNextEvent' => 'onPrevNextEvent',
      'pager' => 'pager',
      'pagerAnchorBuilder' => 'pagerAnchorBuilder',
      'pagerEvent' => 'pagerEvent',
      'pause' => 'pause',
      'paused' => 'paused',
      'pauseOnPagerHover' => 'pauseOnPagerHover',
      'prev' => 'prev',
      'prevNextEvent' => 'prevNextEvent',
      'random' => 'random',
      'randomizeEffects' => 'randomizeEffects',
      'requeueOnImageNotLoaded' => 'requeueOnImageNotLoaded',
      'requeueTimeout' => 'requeueTimeout',
      'resumed' => 'resumed',
      'rev' => 'rev',
      'shuffle' => 'shuffle',
      'slideExpr' => 'slideExpr',
      'slideResize' => 'slideResize',
      'speed' => 'speed',
      'speedIn' => 'speedIn',
      'speedOut' => 'speedOut',
      'startingSlide' => 'startingSlide',
      'sync' => 'sync',
      'timeout' => 'timeout',
      'timeoutFn' => 'timeoutFn',
      'updateActivePagerLink' => 'updateActivePagerLink',
      'width' => 'width',
    );

    $form['views_slideshow_cycle']['advanced_options_choices'] = array(
      '#type' => 'select',
      '#title' => t('Advanced Options'),
      '#options' => $cycle_options,
      '#process' => array('views_slideshow_cycle_form_options_js'),
    );

    $form['views_slideshow_cycle']['advanced_options_entry'] = array(
      '#type' => 'textarea',
      '#title' => t('Advanced Option Value'),
      '#description' => t('It is important that you click the Update link when you make any changes to the options or those changes will not be saved when you save the form.'),
    );

    $form['views_slideshow_cycle']['advanced_options'] = array(
      '#type' => 'textarea',
      '#default_value' => $view->options['views_slideshow_cycle']['advanced_options'],
    );

    $form['views_slideshow_cycle']['advanced_options_table'] = array(
      '#markup' => '<table style="width: 400px; margin-left: 10px;" id="edit-style-options-views-slideshow-cycle-advanced-options-table"></table>'
    );
  }
}

function views_slideshow_cycle_form_options_js($element, &$form_state, &$form) {
  ctools_add_js('formoptions', 'views_slideshow_cycle');
  return $element;
}

/**
 * Implements hook_views_slideshow_options_form_validate().
 */
function views_slideshow_cycle_views_slideshow_options_form_validate(&$form, &$form_state, &$view) {
  if (!is_numeric($form_state['values']['style_options']['views_slideshow_cycle']['speed'])) {
    form_error($form['views_slideshow_cycle']['speed'], t('!setting must be numeric!', array('Speed')));
  }
  if (!is_numeric($form_state['values']['style_options']['views_slideshow_cycle']['timeout'])) {
    form_error($form['views_slideshow_cycle']['speed'], t('!setting must be numeric!', array('timeout')));
  }
  if (!is_numeric($form_state['values']['style_options']['views_slideshow_cycle']['remember_slide_days'])) {
    form_error($form['views_slideshow_cycle']['remember_slide_days'], t('!setting must be numeric!', array('Slide days')));
  }
}
