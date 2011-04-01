<?php


/**
 * Views handler for menu links.
 */

class menu_node_views_field_link_title extends views_handler_field {

  function construct() {
    parent::construct();
    if (!empty($this->options['link_to_item'])) {
      $this->additional_fields['link_path'] = array('table' => 'menu_links', 'field' => 'link_path');
    }
    $this->additional_fields['display_depth'] = array('table' => 'menu_links', 'field' => 'depth');
  }

  function option_definition() {
    $options = parent::option_definition();
    $options['link_to_item'] = array('default' => TRUE);
    $options['display_depth'] = array('default' => '');
    return $options;
  }

  /**
   * Provide link to node option
   */
  function options_form(&$form, &$form_state) {
    parent::options_form($form, $form_state);
    $form['link_to_item'] = array(
      '#title' => t('Link this field to its menu item'),
      '#description' => t('This will override any other link you have set.'),
      '#type' => 'checkbox',
      '#default_value' => !empty($this->options['link_to_item']),
    );
    $form['display_depth'] = array(
      '#title' => t('Mark item depth'),
      '#description' => t('Enter a text element to use as a display marker.'),
      '#type' => 'textfield',
      '#size' => 10,
      '#default_value' => $this->options['display_depth'],
    );

  }

  
  function render($values) {
    if (empty($this->options['link_to_item'])) {
      $output = check_plain($values->{$this->field_alias});
    }
    else {
      $output = l($values->{$this->field_alias}, $values->{$this->aliases['link_path']});
    }
    $text = check_plain($this->options['display_depth']);
    $max = $values->{$this->aliases['display_depth']};
    return str_repeat($text, $max) .' '. $output;
  }
}
