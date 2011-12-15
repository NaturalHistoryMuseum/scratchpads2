<?php
function biblio_ris_views_data() {
  $data = array();
   $data['biblio']['ris_export'] = array(
    'title' => t('Export link - RIS'),
    'help'  => t("Provides a link to export the data in RIS format"),
    'field' => array(
      'handler' => 'biblio_handler_field_export_link_ris',
      'group' => t('Biblio'),
    ),
  );

  return $data;
}