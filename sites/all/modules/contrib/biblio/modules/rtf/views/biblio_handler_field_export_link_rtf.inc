<?php
class biblio_handler_field_export_link_rtf extends biblio_handler_field_export_link {

  function render($values) {
    if (user_access('show export links')) {
      $base = variable_get('biblio_base', 'biblio');
      $nid = $this->get_value($values, 'nid');
      $this->options['alter']['make_link'] = TRUE;
      $this->options['alter']['path'] = "$base/export/rtf/$nid";
      $text = !empty($this->options['text']) ? $this->options['text'] : t('RTF');
      return $text;
          }
  }

}
