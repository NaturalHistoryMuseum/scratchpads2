<?php
function biblio_xml_views_data() {
  $data = array();

  $data['biblio']['xml_export'] = array(
    'title' => t('Export link - EndNote XML'),
    'help'  => t("Provides a link to export the data in EndNote XML format"),
    'field' => array(
      'handler' => 'biblio_handler_field_export_link_endnote_xml',
      'group' => t('Biblio'),
    ),
  );

  return $data;
}