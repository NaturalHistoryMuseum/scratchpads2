<?php
/**
 * @file
 * Contains the Views field handler for the Page Title field.
 */

/**
 * Field handler to provide simple renderer that allows linking to a node.
 *
 * This is a copy from views_handler_field_node.inc in the Views module.
 * Additions are commented inline.
 */
class views_handler_field_node_page_title extends views_handler_field {
  /**
   * Constructor to provide additional fields to add.
   */
  function construct() {
    parent::construct();
    //$this->additional_fields['nid'] = 'nid';

    // Page Title: Load the node title for later.
    //$this->additional_fields['title'] = 'title';
    if (module_exists('translation')) {
      $this->additional_fields['language'] = 'language';
    }
  }

  function option_definition() {
    $options = parent::option_definition();
    $options['link_to_node'] = array('default' => FALSE);

    // Page Title: Adding the node title fallback option and default value.
    $options['use_node_title'] = array('default' => FALSE);

    return $options;
  }

  /**
   * Provide link to node option and fallback to node title option.
   */
  function options_form(&$form, &$form_state) {
    parent::options_form($form, $form_state);

    $form['link_to_node'] = array(
      '#title' => t('Link this field to its node'),
      '#description' => t('This will override any other link you have set.'),
      '#type' => 'checkbox',
      '#default_value' => !empty($this->options['link_to_node']),
    );

    // Page Title: Adding the form field for the node title fallback option.
    $form['use_node_title'] = array(
      '#title' => t('Fall back on to Node: Title'),
      '#description' => t('If no Page Title is set for a node then the normal node title will be used instead.'),
      '#type' => 'checkbox',
      '#default_value' => !empty($this->options['use_node_title']),
    );
  }

  /**
   * Render whatever the data is as a link to the node.
   *
   * Data should be made XSS safe prior to calling this function.
   */
  function render_link($data, $values) {
    if (!empty($this->options['link_to_node']) && $data !== NULL && $data !== '') {
      $this->options['alter']['make_link'] = TRUE;
      $this->options['alter']['path'] = "node/" . $values->{$this->aliases['nid']};
      if (isset($this->aliases['language'])) {
        $languages = language_list();
        if (isset($languages[$values->{$this->aliases['language']}])) {
          $this->options['alter']['language'] = $languages[$values->{$this->aliases['language']}];
        }
        else {
          unset($this->options['alter']['language']);
        }
      }
    }
    return $data;
  }

  function render($values) {
    if (empty($values->{$this->field_alias}) && !empty($this->options['use_node_title'])) {
      return $this->render_link(check_plain($values->{$this->aliases['title']}), $values);
    }
    else {
      return $this->render_link(check_plain($values->{$this->field_alias}), $values);
    }
  }
}

