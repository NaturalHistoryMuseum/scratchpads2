<?php
function biblio_bibtex_views_data() {
  $data = array();
   $data['biblio']['bibtex_export'] = array(
    'title' => t('Export link - Bibtex'),
    'help'  => t("Provides a link to export the data in Bibtex format"),
    'field' => array(
      'handler' => 'biblio_handler_field_export_link_bibtex',
      'group' => t('Biblio'),
    ),
  );

  return $data;
}