<?php

class biblio_handler_field extends views_handler_field {

  function init(&$view, &$options) {
    parent::init($view, $options);

    if (!$this->options['biblio_label']) return;
    $this->definition['click sortable'] = array('default' => TRUE);

    $result = db_query("SELECT bft.tid, bftd.title FROM {biblio_field_type} bft
            INNER JOIN {biblio_fields} bf ON bft.fid=bf.fid AND bf.name = :name
            INNER JOIN {biblio_field_type_data} bftd ON bftd.ftdid=bft.ftdid",
                        array(':name' => $options['field']))
                        ;
    foreach ($result as $label) {
      $this->labels[$label->tid] = $label->title;
    }
  }

  function query() {
    // add the biblio_type field as tid
    $this->ensure_my_table();
    if ($this->options['biblio_label']) {
      $this->query->add_field($this->table_alias, 'biblio_type', 'biblio_tid');
    }
    parent::query();
  }

  function option_definition() {
    $options = parent::option_definition();
    $options['biblio_label'] = array('default' => TRUE);

    return $options;
  }
  function options_form(&$form, &$form_state) {
    $form['biblio_label'] = array(
      '#type' => 'checkbox',
      '#title' => t('Use label specific to biblio type'),
      '#description' => 'Check this option to use the type-specific field labels as defined in '
      . l(t('biblio settings'), 'admin/config/content/biblio/fields/type'),
      '#default_value' => $this->options['biblio_label'],
    );
    parent::options_form($form, $form_state);
    $form['label'] += array(
      '#process' => array('ctools_dependent_process'),
      '#dependency' => array(
        'edit-options-biblio-label' => array(0),
      ),
    );
  }
  function set_label(&$values) {
    if (!$this->options['biblio_label']) return;
    $tid = $values->biblio_tid;
    $this->options['label'] = isset($this->labels[$tid]) ? $this->labels[$tid] : $this->labels[0];
  }

  function pre_render(&$values) {
    foreach ($values as $result) {
      if (!empty($result->biblio_biblio_year)) { // this converts values like 9999 or 9998 to "Submitted" and "In Press"
        $result->biblio_biblio_year = _biblio_text_year($result->biblio_biblio_year);
      }
    }
    return $values;
  }

  function render($values) {
   $this->set_label($values);
   return parent::render($values);
  }
}