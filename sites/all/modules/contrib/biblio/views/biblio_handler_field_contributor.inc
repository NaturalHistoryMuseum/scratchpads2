<?php
class biblio_handler_field_contributor extends biblio_handler_field {
  function construct() {
    module_load_include('inc', 'biblio', 'includes/biblio_theme');
    
    parent::construct();
    $this->additional_fields['vid'] = array('table' => 'biblio', 'field' => 'vid');
    $this->auth_category = isset($this->definition['auth_category']) ? $this->definition['auth_category'] : 1;
  }

  function option_definition() {
    $options = parent::option_definition();
    $options['style_name'] = array('default' => biblio_get_style());
    return $options;
  }
  function options_form(&$form, &$form_state) {
    parent::options_form($form, $form_state);
    module_load_include('inc', 'biblio', 'includes/biblio.admin');
    $form['style_name'] = array(
      '#type' => 'radios',
      '#title' => t('Style'),
      '#default_value' => $this->options['style_name'],
      '#options' => biblio_get_styles(),
      '#description' => t('Define the layout of author lists.')
    );
  }

  function query() {
    $this->add_additional_fields();
    $this->field_alias = $this->aliases['vid'];
    // add the biblio_type field as tid
    $this->query->add_field($this->table_alias, 'biblio_type', 'biblio_tid');
  }

  function pre_render($values) {
    $vids = array();
    $this->items = array();

    foreach ($values as $result) {
      // Don't add empty results to the array.
      if (!empty($result->{$this->aliases['vid']})) {
        $vids[] = $result->{$this->aliases['vid']};
      }
    }

    if (count($vids)) {
      // Optimize query if $vids has only 1 item in the array, to use '=' instead of 'IN'.
      if (count($vids) > 1) {
        $vidstr = 'vid IN (' . implode(',', $vids) . ')';
      }
      else {
        $vidstr = 'vid = ' . $vids[0];
      }
      $result = db_query('SELECT * FROM {biblio_contributor} bc
                          INNER JOIN {biblio_contributor_data} bcd ON bc.cid = bcd.cid
                          WHERE vid IN (:vids}
                          AND auth_category = :cat
                          ORDER BY bc.rank ',
                          array(':vids' => implode(',', $vids), ':cat' => $this->auth_category));
      foreach ($result as $item) {
        // Clean up the $item from vid.
        $vid = $item['vid']; unset($item['vid']);
        $this->items[$vid][] = $item;
      }
    }
  }

  function render($values) {
    parent::set_label($values);
    $vid = $values->{$this->field_alias};
    if (!isset($this->items[$vid])) return NULL;
    return biblio_format_authors($this->items[$vid]);
  }
}