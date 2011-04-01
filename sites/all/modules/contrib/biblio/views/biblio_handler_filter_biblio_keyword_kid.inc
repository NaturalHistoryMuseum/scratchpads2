<?php
class biblio_handler_filter_biblio_keyword_kid extends views_handler_filter_many_to_one {
  function has_extra_options() { return TRUE; }

  function get_value_options() {
    $result = db_query("SELECT kd.* FROM {biblio_keyword_data} kd ORDER BY kd.word");
    foreach ($result as $term) {
      $this->value_options[$term->kid] = $term->word;
    }

  }

  function option_definition() {
    $options = parent::option_definition();

    $options['type'] = array('default' => 'textfield');
    $options['limit'] = array('default' => TRUE);
    $options['kid'] = array('default' => 0);

    return $options;
  }

  function extra_options_form(&$form, &$form_state) {

    if ($this->options['limit']) {
      // We only do this when the form is displayed so this query doesn't run
      // unnecessarily just when the object is constructed.

      $form['type'] = array(
      '#type' => 'radios',
      '#title' => t('Selection type'),
      '#options' => array('select' => t('Dropdown'), 'textfield' => t('Autocomplete')),
      '#default_value' => $this->options['type'],
      );
    }
  }
  function value_form(&$form, &$form_state) {
    if ($this->options['type'] == 'textfield') {
      $default = '';
      if ($this->value) {
        $result = db_query("SELECT * FROM {biblio_keyword} bk WHERE bk.kid IN (:kids)",
        array(':kids' => implode(', ', $this->value)));

        foreach ($result as $term) {
          if ($default) {
            $default .= ', ';
          }
          $default .= $term->word;
        }
      }

      $form['value'] = array(
        '#title' => t('Select keywords'),
        '#type' => 'textfield',
        '#default_value' => $default,
      );

      if ($this->options['limit']) {
        $form['value']['#autocomplete_path'] = 'biblio/autocomplete/biblio_keywords';
      }
    }
    else {
      $options = array();
      $result = db_query("SELECT kd.* FROM {biblio_keyword_data} kd ORDER BY kd.word");
      foreach ($result as $term) {
        $options[$term->kid] = $term->word;
      }

      $default_value = (array) $this->value;

      if (!empty($form_state['exposed'])) {
        $identifier = $this->options['expose']['identifier'];

        if (!empty($this->options['expose']['reduce'])) {
          $options = $this->reduce_value_options($options);

          if (empty($this->options['expose']['single']) && !empty($this->options['expose']['optional'])) {
            $default_value = array();
          }
        }

        if (!empty($this->options['expose']['single'])) {
          if (!empty($this->options['expose']['optional']) && (empty($default_value) || !empty($this->options['expose']['reduce']))) {
            $default_value = 'All';
          }
          elseif (empty($default_value)) {
            $keys = array_keys($options);
            $default_value = array_shift($keys);
          }
          else {
            $copy = $default_value;
            $default_value = array_shift($copy);
          }
        }
      }
      $form['value'] = array(
        '#type' => 'select',
        '#title' => t('Select keywords'),
        '#multiple' => TRUE,
        '#options' => $options,
        '#size' => min(9, count($options)),
        '#default_value' => $default_value,
      );

      if (!empty($form_state['exposed']) && !isset($form_state['input'][$identifier])) {
        $form_state['input'][$identifier] = $default_value;
      }
    }


    if (empty($form_state['exposed'])) {
      // Retain the helper option
      $this->helper->options_form($form, $form_state);
    }
  }

}
