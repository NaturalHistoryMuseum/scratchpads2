<?php
class biblio_handler_field_export_link extends views_handler_field {
  function init(&$view, &$options) {
    parent::init($view, $options);
    $this->additional_fields['nid'] = array('table' => 'node', 'field' => 'nid');
  }

  function query() {
    $this->add_additional_fields();
  }

  function option_definition() {
    $options = parent::option_definition();

    $options['text'] = array('default' => '', 'translatable' => TRUE);
    $options['label'] = array('default' => '');

    return $options;
  }

  function options_form(&$form, &$form_state) {
    parent::options_form($form, $form_state);
    $text = !empty($this->options['text']) ? $this->options['text'] : $this->definition['format name'];
    $form['text'] = array(
      '#type' => 'textfield',
      '#title' => t('Text to display'),
      '#default_value' => $text,
    );
  }

  function render($values) {
    if (user_access('show export links')) {
      $format = $this->definition['format'];
      $base = variable_get('biblio_base', 'biblio');
      $nid = $this->get_value($values, 'nid');
      $this->options['alter']['make_link'] = TRUE;
      $this->options['alter']['path'] = "$base/export/$format/$nid";
      $text = !empty($this->options['text']) ? $this->options['text'] : $this->definition['format name'];
      return $text;
    }
  }

}
