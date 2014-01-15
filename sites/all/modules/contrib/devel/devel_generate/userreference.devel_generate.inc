<?php

function userreference_devel_generate($object, $field) {
  if (field_behaviors_widget('multiple values', $instance) == FIELD_BEHAVIOR_CUSTOM) {
    return devel_generate_multiple('_userreference_devel_generate', $object, $field);
  }
  else {
    return _userreference_devel_generate($object, $field);
  }
}

function _userreference_devel_generate($object, $field) {
  $object_field = array();
  $allowed_values = userreference_allowed_values($field);
  if (isset($allowed_values['none'])) {
    unset($allowed_values['none']);
  }
  if (!empty($allowed_values)) {
    // Just pick one of the specified allowed values.
    $object_field['uid'] = array_rand($allowed_values);
  }
  return $object_field;
}
