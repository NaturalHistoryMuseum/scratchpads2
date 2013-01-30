<?php
class biblio_handler_field_keyword extends biblio_handler_field {
  function init(&$view, &$options) {
    module_load_include('inc', 'biblio', 'includes/biblio.keywords');
    parent::init($view, $options);
    $this->multiple = TRUE;
    $this->additional_fields['vid'] = array('table' => 'biblio', 'field' => 'vid');
  }

  function option_definition() {
    $options = parent::option_definition();
    $options['separator'] = array('default' => '; ');

    return $options;
  }

  function options_form(&$form, &$form_state) {
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

  function query() {
    $this->add_additional_fields();
    $this->field_alias = $this->aliases['vid'];
  }

  function post_execute(&$values) {
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

  function render($values) {
    parent::set_label($values);
    $vid = $values->{$this->field_alias};
    if (!isset($this->items[$vid])) return NULL;
    return implode($this->options['separator'], $this->items[$vid]);
  }


}