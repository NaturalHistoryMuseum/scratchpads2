<?php

/**
 *
 */
class biblio_handler_field_keyword extends biblio_handler_field {

  /**
   *
   */
  public function init(&$view, &$options) {
    module_load_include('inc', 'biblio', 'includes/biblio.keywords');
    parent::init($view, $options);
    $this->multiple = TRUE;
    $this->additional_fields['vid'] = array('table' => 'biblio', 'field' => 'vid');
  }

  /**
   *
   */
  public function option_definition() {
    $options = parent::option_definition();
    $options['separator'] = array('default' => '; ');

    return $options;
  }

  /**
   *
   */
  public function options_form(&$form, &$form_state) {
    $form['separator'] = array(
      '#type' => 'textfield',
      '#size' => 5,
      '#title' => t('Keyword separator'),
      '#default_value' => $this->options['separator'],
      '#required' => TRUE,
      '#description' => t('Enter the character which will be used to separate the keywords.'),
      '#fieldset' => 'separators',
    );
    parent::options_form($form, $form_state);

  }

  /**
   *
   */
  public function query() {
    $this->add_additional_fields();
    $this->field_alias = $this->aliases['vid'];
  }

  /**
   *
   */
  public function post_execute(&$values) {
    $vids = array();
    $this->items = array();
    $filter = '';

    foreach ($values as $result) {
      // Don't add empty results to the array.
      if (isset($this->aliases['vid']) && !empty($result->{$this->aliases['vid']})) {
        $vids[] = $result->{$this->aliases['vid']};
      }
    }

    if (count($vids)) {
      $this->items = biblio_load_keywords_multiple($vids);
    }
  }

  /**
   *
   */
  public function render($values) {
    parent::set_label($values);
    $vid = $values->{$this->field_alias};
    if (!isset($this->items[$vid])) {
      return NULL;
    }
    return implode($this->options['separator'], $this->items[$vid]);
  }

}
