<?php
function biblio_tagged_views_data() {
  $data = array();

  $data['biblio']['tagged_export'] = array(
    'title' => t('Export link - EndNote tagged'),
    'help'  => t("Provides a link to export the data in EndNote tagged format"),
    'field' => array(
      'handler' => 'biblio_handler_field_export_link_endnote_tagged',
      'group' => t('Biblio'),
    ),
  );

  return $data;
}