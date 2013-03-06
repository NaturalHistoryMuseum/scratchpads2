<?php
function _biblio_field_extra_fields() {
  $extras['node']['biblio']['form'] = array();
  $fields = array();

    $result = db_query("SELECT * FROM {biblio_fields} b
              INNER JOIN {biblio_field_type} bt ON b.fid = bt.fid
              INNER JOIN {biblio_field_type_data} btd ON btd.ftdid=bt.ftdid
              WHERE bt.tid=:tid ORDER BY bt.weight ASC", array(':tid' => 0), array('fetch' => PDO::FETCH_ASSOC));

    foreach ($result as $row) {
        $fields[$row['name']] = $row;
    }
    _biblio_localize_fields($fields);

    $extras['node']['biblio']['form']['biblio_type'] = array(
      'label'       => t('Publication Type'),
      'description' => t('Biblio module form.'),
      'weight'      => -4
    );

    foreach ($fields as $key => $fld) {
      $label = check_plain($fld['title']);
      if ($fld['type'] == 'textarea' ||  $fld['type'] == 'contrib_widget') {
        $key = $key . '_field';
        $label = $label . ' (' . t('Fieldset') . ')';
      }
      $extras['node']['biblio']['form'][$key] = array(
        'label'       => $label,
        'description' => t('Biblio module form.'),
        'weight'      => $fld['weight'] / 10
      );
    }
    $extras['node']['biblio']['form']['other_fields'] = array(
        'label'       => t('Other Biblio Fields') . ' (' . t('Fieldset') . ')',
        'description' => t('Biblio module form.'),
        'weight'      => 0
    );

    $extras['user']['user'] = array(
        'form' => array(
            'biblio_fieldset' => array(
                'label' => t('User specific Biblio settings'),
                'description' => t('Biblio module account form elements.'),
                'weight' => -10,
            ),
        ),
    );

  return $extras;
}