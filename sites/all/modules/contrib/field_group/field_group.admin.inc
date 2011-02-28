<?php
// $Id: field_group.admin.inc,v 1.2.2.5 2011/01/09 01:03:23 stalski Exp $

/**
 * @file
 * Administration for fieldgroup.
 *
 */
function field_group_admin_settings() {

  $form = array();
  $form['field_group_mark_as_required'] = array(
    '#type' => 'checkbox',
    '#title' => t('Mark tabs as required.'),
    '#default_value' => variable_get('field_group_mark_as_required', FALSE),
    '#description' => t('Mark fieldgroups of the type horizontal and vertical tabs as required when nested fields are marked as required.')
  );

  return system_settings_form($form);
}