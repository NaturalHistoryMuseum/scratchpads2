<?php
function biblio_rtf_views_data() {
  $data = array();
   $data['biblio']['rtf_export'] = array(
    'title' => t('Export link - Rich Text'),
    'help'  => t("Provides a link to export the data in Rich Text format"),
    'field' => array(
      'handler' => 'biblio_handler_field_export_link_rtf',
      'group' => t('Biblio'),
    ),
  );

  return $data;
}