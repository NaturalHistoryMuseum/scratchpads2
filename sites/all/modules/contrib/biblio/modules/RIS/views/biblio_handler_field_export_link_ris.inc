<?php
class biblio_handler_field_export_link_ris extends biblio_handler_field_export_link {

  function render($values) {
    if (user_access('show export links')) {
      $base = variable_get('biblio_base', 'biblio');
      $nid = $this->get_value($values, 'nid');
      $this->options['alter']['make_link'] = TRUE;
      $this->options['alter']['path'] = "$base/export/ris/$nid";
      $text = !empty($this->options['text']) ? $this->options['text'] : t('RIS');
      return $text;
          }
  }

}
