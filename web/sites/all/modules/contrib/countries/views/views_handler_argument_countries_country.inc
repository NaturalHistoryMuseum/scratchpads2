<?php

/**
 * @file
 * Argument handler for countries properties.
 */

class views_handler_argument_countries_country extends views_handler_argument_string {

  /**
   * Override the behavior of title_query() based on the user selected options.
   */
  function title_query() {
    $titles = array();
    foreach ($this->value as $country_property) {
      if ($country = countries_country_lookup($country_property, $this->real_field)) {
        $property = empty($this->options['title_property']) ? 'name' : $this->options['title_property'];
        if ($label = country_property($country, $property)) {
          $titles[] = $label;
        }
      }
    }
    return $titles;
  }

  function option_definition() {
    $options = parent::option_definition();
    $options['title_property'] = array('default' => 'name');
    return $options;
  }

  function options_form(&$form, &$form_state) {
    $form['title_property'] = array(
      '#type' => 'select',
      '#title' => t('Title property'),
      '#options' => countries_core_properties(),
      '#required' => TRUE,
      '#default_value' => $this->options['title_property'],
      '#description' => t('Defines what property that is used when overriding the title or breadcrumb.'),
      '#fieldset' => 'argument_present',
    );
    unset($form['title_property']['#options']['enabled']);
    unset($form['title_property']['#options']['continent']);

    parent::options_form($form, $form_state);
  }
}
