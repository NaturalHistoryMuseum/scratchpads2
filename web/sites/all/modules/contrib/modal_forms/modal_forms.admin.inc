<?php

/**
 * @file
 * Administrative page callbacks for the modal_forms module.
 */

/**
 * General configuration form for controlling the modal_forms behaviour.
 */
function modal_forms_admin_settings() {

  // Login settings.
  $form['modal_forms_login_settings'] = array(
    '#type' => 'fieldset',
    '#title' => t('Login links settings'),
  );
  $form['modal_forms_login_settings']['modal_forms_login'] = array(
    '#type' => 'checkbox',
    '#title' => t('Enable for login links'),
    '#default_value' => variable_get('modal_forms_login', 0),
    '#description' => t('Automatically activate Modal forms for links to user/login.'),
  );
  $form['modal_forms_login_settings']['modal_forms_login_links'] = array(
    '#type' => 'radios',
    '#title' => t('Display links'),
    '#options' => array(
      0 => t('No links'),
      1 => t('Show links'),
      2 => t('Show links and open them in a modal'),
    ),
    '#default_value' => variable_get('modal_forms_login_links', 0),
    '#description' => t('Display the "Create new account" (if allowed) and "Request new password" links below the login form.'),
    '#states' => array(
      'visible' => array(
        ':input[name="modal_forms_login"]' => array('checked' => TRUE),
      ),
    ),
  );

  // Register settings.
  $form['modal_forms_register_settings'] = array(
    '#type' => 'fieldset',
    '#title' => t('Register links settings'),
  );
  $form['modal_forms_register_settings']['modal_forms_register'] = array(
    '#type' => 'checkbox',
    '#title' => t('Enable for register new account links'),
    '#default_value' => variable_get('modal_forms_register', 0),
    '#description' => t('Automatically activate Modal forms for links to user/register.'),
  );

  // Password settings.
  $form['modal_forms_password_settings'] = array(
    '#type' => 'fieldset',
    '#title' => t('Password links settings'),
  );
  $form['modal_forms_password_settings']['modal_forms_password'] = array(
    '#type' => 'checkbox',
    '#title' => t('Enable for request new password links'),
    '#default_value' => variable_get('modal_forms_password', 0),
    '#description' => t('Automatically activate Modal forms for links to user/password.'),
  );

  // Comments settings.
  $form['modal_forms_comment_settings'] = array(
    '#type' => 'fieldset',
    '#title' => t('Comment links settings'),
  );
  $form['modal_forms_comment_settings']['modal_forms_comment'] = array(
    '#type' => 'checkbox',
    '#title' => t('Enable for comment links'),
    '#default_value' => variable_get('modal_forms_comment', 0),
    '#description' => t('Automatically activate Modal forms for links to comment.'),
  );

  // Contact settings.
  $form['modal_forms_contact_settings'] = array(
    '#type' => 'fieldset',
    '#title' => t('Contact links settings'),
  );
  $form['modal_forms_contact_settings']['modal_forms_contact'] = array(
    '#type' => 'checkbox',
    '#title' => t('Enable for contact links'),
    '#default_value' => variable_get('modal_forms_contact', 0),
    '#description' => t('Automatically activate Modal forms for links to contact.'),
  );

  // Styles and options settings.
  $form['modal_forms_custom_settings'] = array(
      '#type' => 'fieldset',
      '#title' => t('Styles and options'),
      '#collapsible' => TRUE,
      '#collapsed' => TRUE,
  );
  $form['modal_forms_custom_settings']['modal_forms_background_color'] = array(
      '#type' => 'textfield',
      '#title' => t('Background color'),
      '#default_value' => variable_get('modal_forms_background_color', '#000'),
      '#size' => 7,
      '#maxlength' => 7,
      '#required' => TRUE,
      '#description' => t('The color of the background behind the modal. Should be entered in html hex notation (eg #000).'),
  );
  $form['modal_forms_custom_settings']['modal_forms_opacity'] = array(
      '#type' => 'textfield',
      '#title' => t('Background opacity'),
      '#default_value' => variable_get('modal_forms_opacity', 0.85),
      '#size' => 3,
      '#maxlength' => 5,
      '#required' => TRUE,
      '#description' => t('The opacity of the background behind the modal. Should be entered as a decimal value (eg 0.85 = 85% opacity).'),
  );

  // Modal popup size settings
  $modal_types = array('fixed' => t('Fixed'), 'scale' => t('Scale'));
  // Popup Small
  $form['modal_forms_custom_settings']['modal_forms_popup_small'] = array(
    '#type' => 'fieldset',
    '#title' => t('Modal popup small'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );
  $form['modal_forms_custom_settings']['modal_forms_popup_small']['modal_forms_popup_small_type'] = array(
      '#type' => 'radios',
      '#title' => t('Type'),
      '#default_value' => variable_get('modal_forms_popup_small_type', 'fixed'),
      '#options' => $modal_types,
      '#description' => t('Fixed sizes should be entered in pixels. Scaled sizes should be entered as a decimal percentage of the screen region (eg 0.8 = 80% of screen width/height).'),
  );
  $form['modal_forms_custom_settings']['modal_forms_popup_small']['modal_forms_popup_small_width'] = array(
    '#type' => 'textfield',
    '#title' => t('Width'),
    '#default_value' => variable_get('modal_forms_popup_small_width', 300),
    '#size' => 3,
    '#maxlength' => 5,
    '#required' => TRUE,
  );
  $form['modal_forms_custom_settings']['modal_forms_popup_small']['modal_forms_popup_small_height'] = array(
    '#type' => 'textfield',
    '#title' => t('Height'),
    '#default_value' => variable_get('modal_forms_popup_small_height', 300),
    '#size' => 3,
    '#maxlength' => 5,
    '#required' => TRUE,
  );
  // Popup Medium
  $form['modal_forms_custom_settings']['modal_forms_popup_medium'] = array(
    '#type' => 'fieldset',
    '#title' => t('Modal popup medium'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );
  $form['modal_forms_custom_settings']['modal_forms_popup_medium']['modal_forms_popup_medium_type'] = array(
    '#type' => 'radios',
    '#title' => t('Type'),
    '#default_value' => variable_get('modal_forms_popup_medium_type', 'fixed'),
    '#options' => $modal_types,
    '#description' => t('Fixed sizes should be entered in pixels. Scaled sizes should be entered as a decimal percentage of the screen region (eg 0.8 = 80% of screen width/height).'),
  );
  $form['modal_forms_custom_settings']['modal_forms_popup_medium']['modal_forms_popup_medium_width'] = array(
    '#type' => 'textfield',
    '#title' => t('Width'),
    '#default_value' => variable_get('modal_forms_popup_medium_width', 550),
    '#size' => 3,
    '#maxlength' => 5,
    '#required' => TRUE,
  );
  $form['modal_forms_custom_settings']['modal_forms_popup_medium']['modal_forms_popup_medium_height'] = array(
    '#type' => 'textfield',
    '#title' => t('Height'),
    '#default_value' => variable_get('modal_forms_popup_medium_height', 450),
    '#size' => 3,
    '#maxlength' => 5,
    '#required' => TRUE,
  );
  // Popup Large
  $form['modal_forms_custom_settings']['modal_forms_popup_large'] = array(
    '#type' => 'fieldset',
    '#title' => t('Modal popup large'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );
  $form['modal_forms_custom_settings']['modal_forms_popup_large']['modal_forms_popup_large_type'] = array(
    '#type' => 'radios',
    '#title' => t('Type'),
    '#default_value' => variable_get('modal_forms_popup_large_type', 'scale'),
    '#options' => $modal_types,
    '#description' => t('Fixed sizes should be entered in pixels. Scaled sizes should be entered as a decimal percentage of the screen region (eg 0.8 = 80% of screen width/height).'),
  );
  $form['modal_forms_custom_settings']['modal_forms_popup_large']['modal_forms_popup_large_width'] = array(
    '#type' => 'textfield',
    '#title' => t('Width'),
    '#default_value' => variable_get('modal_forms_popup_large_width', 0.8),
    '#size' => 3,
    '#maxlength' => 5,
    '#required' => TRUE,
  );
  $form['modal_forms_custom_settings']['modal_forms_popup_large']['modal_forms_popup_large_height'] = array(
    '#type' => 'textfield',
    '#title' => t('Height'),
    '#default_value' => variable_get('modal_forms_popup_large_height', 0.8),
    '#size' => 3,
    '#maxlength' => 5,
    '#required' => TRUE,
  );

  // Page activation/deactivation settings.
  $form['modal_forms_advanced_settings'] = array(
    '#type' => 'fieldset',
    '#title' => t('Page activation settings'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );
  $form['modal_forms_advanced_settings']['modal_forms_visibility'] = array(
    '#type' => 'radios',
    '#title' => t('Show Modal forms on specific pages'),
    '#options' => array(0 => t('All pages except those listed'), 1 => t('Only the listed pages')),
    '#default_value' => variable_get('modal_forms_visibility', 0),
  );
  $form['modal_forms_advanced_settings']['modal_forms_pages'] = array(
    '#type' => 'textarea',
    '#title' => '<span class="element-invisible">' . t('Pages') . '</span>',
    '#default_value' => variable_get('modal_forms_pages', "admin*\nimagebrowser*\nimg_assist*\nimce*\nnode/add/*\nnode/*/edit\nprint/*\nprintpdf/*\nsystem/ajax\nsystem/ajax/*"),
    '#description' => t("Specify pages by using their paths. Enter one path per line. The '*' character is a wildcard. Example paths are %blog for the blog page and %blog-wildcard for every personal blog. %front is the front page.", array('%blog' => 'blog', '%blog-wildcard' => 'blog/*', '%front' => '<front>')),
  );

  return system_settings_form($form);
}
