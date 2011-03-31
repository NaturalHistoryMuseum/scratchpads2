<?php

/**
 * @file
 * Page callbacks for the Terms of Use module.
 */

/**
 * Menu callback; show settings form.
 * @see terms_of_use_admin_settings_validate()
 *
 */
function terms_of_use_admin_settings() {

  // Adding the fieldset for node specification.
  $form['terms_of_use_text'] = array(
    '#type' => 'fieldset',
    '#prefix' => '<div id="fieldset-wrapper">',
    '#suffix' => '</div>',
  );
  $form['terms_of_use_text']['terms_of_use_node_title'] = array(
    '#type' => 'textfield',
    '#title' => t('Title of the post where your Terms of Use are published'),
    '#default_value' => variable_get('terms_of_use_node_title', ''),
    '#description' => t('Node <em>title</em> of the page or story (or blog entry or book page) where your Terms of Use are published.'),
    '#autocomplete_path' => 'node/autocomplete',
  );
  $form['terms_of_use_text']['terms_of_use_pick_node_id'] = array(
    '#type' => 'button',
    '#value' => t('I prefer to specify the node id'),
    '#weight' => 10,
    '#ajax' => array(
      'callback' => 'terms_of_use_js',
      'wrapper' => 'fieldset-wrapper',
    ),
  );
  // Adding the fieldset for form specification.
  $form['terms_of_use_form'] = array(
    '#type' => 'fieldset',
  );
  $form['terms_of_use_form']['terms_of_use_fieldset_name'] = array(
    '#type' => 'textfield',
    '#title' => t('Label for the fieldset'),
    '#default_value' => variable_get('terms_of_use_fieldset_name', t('Terms of Use')),
    '#description' => t('The text for the Terms of Use and the [x] checkbox are contained in a fieldset. Type here the title for that fieldset.'),
  );
  $form['terms_of_use_form']['terms_of_use_checkbox_label'] = array(
    '#type' => 'textfield',
    '#title' => t('Label for the checkbox'),
    '#default_value' => variable_get('terms_of_use_checkbox_label', t('I agree with these terms')),
    '#description' => t('Type here something like "I agree with these terms." or "I CERTIFY THAT I AM OVER THE AGE OF 18 YEARS OLD.", without quotes. You can use the token @link to insert a link to the Terms in this label. For example, the label can be: "I agree with the @link.", without quotes. You may want to link to the Terms if you prefer not to show the full text of the Terms in the registration form. If you use the token, the Terms will not be shown.'),
  );

  return system_settings_form($form);
}

/**
 * Validate the terms_of_use_admin_settings form.
 *
 * @see terms_of_use_admin_settings()
 */
function terms_of_use_admin_settings_validate($form, &$form_state) {
  if (isset($form_state['values']['terms_of_use_node_id'])) {
    $nid = $form_state['values']['terms_of_use_node_id'];
    if (empty($nid)) {
      form_set_error('terms_of_use_node_id', t('You must specify a node <em>nid</em>.'));
    }
    else {
      $node = node_load($nid);
      if ($node == FALSE) {
        form_set_error('terms_of_use_node_id', t('No post was published with <em>nid</em> !nid.', array('!nid' => $nid)));
      }
      else {
        variable_set('terms_of_use_node_title', $node->title);
      }
    }
  }
  elseif (!empty($form_state['values']['terms_of_use_node_title'])) {
    $nid = db_select('node', 'n')
        ->fields('n', array('nid'))
        ->condition('n.title', db_like($form_state['values']['terms_of_use_node_title']), 'LIKE')
        ->condition('n.status', 1)
        ->range(0, 1)
        ->addTag('node_access')
        ->execute()
        ->fetchField();

    if (!$nid) {
      form_set_error('terms_of_use_node_title', t('No post was published with this title.'));
    }
    else {
      variable_set('terms_of_use_node_id', $nid);
    }
  }
  else {
    form_set_error('terms_of_use_node_title', t('You must specify a node title.'));
  }
}

/**
 * Helper function for autocompletion
 */
function terms_of_use_autocomplete($string = '') {
  $matches = array();
  if ($string != '') {
    $result = db_select('node', 'n')
        ->fields('n', array('nid', 'title'))
        ->condition('n.title', '%' . db_like($string) . '%', 'LIKE')
        ->condition('n.status', 1)
        ->range(0, 10)
        ->addTag('node_access')
        ->execute();

    foreach ($result as $node) {
      $matches[$node->title] = $node->title;
    }
  }

  drupal_json_output($matches);
}

/**
 * Menu callback for AHAH addition.
 */
function terms_of_use_js($form, &$form_state) {
  // Build the new form.
  $form_build_id = $_POST['form_build_id'];

  if (isset($form['terms_of_use_text']['terms_of_use_node_title'])) {
    // Create the extra field
    $form['terms_of_use_text']['terms_of_use_node_id'] = array(
      '#type' => 'textfield',
      '#title' => t('Node id where your Terms of Use are published'),
      '#default_value' => variable_get('terms_of_use_node_id', ''),
      '#description' => t('Node <em>id</em> of the page or story (or blog entry or book page) where your Terms of Use are published.'),
    );
    unset($form['terms_of_use_text']['terms_of_use_node_title']);
    $form['terms_of_use_text']['terms_of_use_pick_node_id']['#value'] = t('I prefer to provide the title of the post');
  }
  else {
    // Create the extra field
    $form['terms_of_use_text']['terms_of_use_node_title'] = array(
      '#type' => 'textfield',
      '#title' => t('Title of the post where your Terms of Use are published'),
      '#default_value' => variable_get('terms_of_use_node_title', ''),
      '#description' => t('Node <em>title</em> of the page or story (or blog entry or book page) where your Terms of Use are published.'),
      '#autocomplete_path' => 'node/autocomplete',
    );
    unset($form['terms_of_use_text']['terms_of_use_node_id']);
    $form['terms_of_use_text']['terms_of_use_pick_node_id']['#value'] = t('I prefer to specify the node id');
  }

  // Rebuild the form.
  // @todo: Resolve the bug when user click node_id button and then cannot return back to node_title field.
  $form = form_builder($form_build_id, $form, $form_state);
  form_set_cache($form_build_id, $form, $form_state);

  return $form['terms_of_use_text'];
}
