<?php

/**
 * @file
 * Custom Views display handler. Extends the Page display by adding our own page title option to it.
 */

class page_title_plugin_display_page_with_page_title extends views_plugin_display_page {
  function option_definition() {
    $options = parent::option_definition();
    $options['page_title_pattern'] = array('default' => '');
    return $options;
  }

  function options_summary(&$categories, &$options) {
    parent::options_summary($categories, $options);

    // Lookup the current title and strip any tags out.
    $title = strip_tags($this->get_option('page_title_pattern'));

    if (empty($title)) {
      $title = t('None');
    }
    elseif (drupal_strlen($title) > 16) {
      $title = drupal_substr($title, 0, 16) . '...';
    }

    $options['page_title_pattern'] = array(
      'category' => 'page',
      'title' => t('Page Title Pattern'),
      'value' => $title,
    );
  }

  function options_form(&$form, &$form_state) {
    parent::options_form($form, $form_state);

    if ($form_state['section'] == 'page_title_pattern') {
      $form['#title'] = t('The <em>Page Title Pattern</em> of this view');

      $form['page_title_pattern'] = array(
        '#title' => t('Page Title Pattern'),
        '#type' => 'textfield',
        '#description' => t('Optionally use this field to define a Page Title Pattern (not the Page Title). You may use the Tokens listed below'),
        '#default_value' => $this->get_option('page_title_pattern'),
      );

      // Add the token help to a collapsed fieldset at the end of the configuration page.
      $form['token_help'] = array(
        '#type' => 'fieldset',
        '#title' => t('Available Tokens List'),
        '#collapsible' => TRUE,
        '#collapsed' => TRUE,
      );
      $form['token_help']['content'] = array(
        '#theme' => 'token_tree',
        '#token_types' => array(),
      );
    }
  }


  function options_submit(&$form, &$form_state) {
    parent::options_submit($form, $form_state);

    if ($form_state['section'] == 'page_title_pattern') {
      $this->set_option('page_title_pattern', $form_state['values']['page_title_pattern']);
    }
  }
}
