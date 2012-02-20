<?php
class biblio_handler_field_biblio_keyword_data_word extends views_handler_field {
  /**
   * Constructor to provide additional field to add.
   *
   * This constructer assumes the term_data table. If using another
   * table, we'll need to be more specific.
   */
  function construct() {
    parent::construct();
    $this->additional_fields['kid'] = 'kid';
  }

  function option_definition() {
    $options = parent::option_definition();
    $options['link_to_keyword'] = array(
    	'default' => FALSE
    );
    $options['multi_type'] = array(
      'default' => 'separator'
    );
    $options['separator'] = array(
      'default' => ', '
    );

    return $options;
  }

  /**
   * Provide link to taxonomy option
   */
  function options_form(&$form, &$form_state) {
    parent::options_form($form, $form_state);
    $form['link_to_keyword'] = array(
      '#title' => t('Link this field to its keyword page'),
      '#description' => t('This will override any other link you have set.'),
      '#type' => 'checkbox',
      '#default_value' => !empty($this->options['link_to_keyword']),
    );
  }
  /**
   * Render whatever the data is as a link to the taxonomy.
   *
   * Data should be made XSS safe prior to calling this function.
   */
  function render_link($data, $values) {
    if (!empty($this->options['link_to_keyword']) && !empty($values->{$this->aliases['kid']}) && $data !== NULL && $data !== '') {
      $kid = $values->{$this->aliases['kid']};
      $base = variable_get('biblio_base', 'biblio');
      $this->options['alter']['make_link'] = TRUE;
      $this->options['alter']['path'] = $base .'/keyword/' . $kid;
    }
    return $data;
  }

  function render($values) {
    return $this->render_link(check_plain($values->{$this->field_alias}), $values);

  }
}
