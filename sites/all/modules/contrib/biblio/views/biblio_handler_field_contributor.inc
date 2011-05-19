<?php
class biblio_handler_field_contributor extends biblio_handler_field {
  function construct() {
    module_load_include('inc', 'biblio', 'includes/biblio_theme');

    parent::construct();
    $this->additional_fields['vid'] = array('table' => 'biblio', 'field' => 'vid');
  }

  function option_definition() {
    $options = parent::option_definition();
    $options['auth_category'] = array('default' => 1);
    return $options;
  }
  function options_form(&$form, &$form_state) {
    parent::options_form($form, $form_state);
    $form['auth_category'] = array(
      '#type' => 'select',
      '#title' => t('Category of Author'),
      '#default_value' => $this->options['auth_category'],
      '#options' => array(
        0 => t('All categories'),
        1 => t('Primary'),
        2 => t('Secondary'),
        3 => t('Tirtiary'),
        4 => t('Subsidiary'),
        5 => t('Corporate/Institutional')
      ),
      '#description' => t('Define the layout of author lists.')
    );

  }

  function query() {
    $this->add_additional_fields();
    $this->field_alias = $this->aliases['vid'];
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
      $place_holders = array();
      $place_holders[':vids'] =  $vids;
      if ($this->options['auth_category'] > 0) {
        $filter = 'AND auth_category = :cat';
        $place_holders[':cat'] = $this->options['auth_category'];
      }
      // Optimize query if $vids has only 1 item in the array, to use '=' instead of 'IN'.
      $result = db_query('SELECT * FROM {biblio_contributor} bc
                          INNER JOIN {biblio_contributor_data} bcd ON bc.cid = bcd.cid
                          WHERE vid IN (:vids) '.
                          $filter
                          . ' ORDER BY vid ASC, bc.rank ASC',
                          $place_holders,
                          array('fetch' => PDO::FETCH_ASSOC));
      foreach ($result as $item) {
        $this->items[$item['vid']][] = $item;
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