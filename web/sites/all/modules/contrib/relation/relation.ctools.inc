<?php

/**
 * @file
 * ctools import and export for Relation module.
 */

/**
 * Page callback for ctools exports.
 */
function relation_export_relation_type($form, &$form_state, $type) {
  drupal_set_title($type->label);
  ctools_include('export');
  $result = ctools_export_load_object('relation_type', 'names', array($type->relation_type));
  $code = relation_relation_type_export($result[$type->relation_type]);
  $lines = substr_count($code, "\n");
  if (!function_exists('ctools_export_crud_load_multiple')) {
    drupal_set_message(t('While manual import works, if you want to save this into a relation_type_default.inc file you need a newer CTools for relation to pick it up later.'), 'warning');
  }

  $form['export'] = array(
    '#title' => t('Export data'),
    '#type' => 'textarea',
    '#value' => $code,
    '#rows' => $lines,
    '#description' => t('Copy the export text and paste it into another myobj using the import function.'),
  );
  return $form;
}

/**
 * Exports a relation type.
 */
function relation_relation_type_export($relation_type, $indent = '') {
  ctools_include('export');
  $additional2 = array(
    'source_bundles' => $relation_type->source_bundles,
    'target_bundles' => $relation_type->target_bundles,
  );
  $output = ctools_export_object('relation_type', $relation_type, $indent, NULL, array(), $additional2);
  return $output;
}
