<?php

/**
 * Implements hook_field_group_info().
 */
function character_editor_field_group_info(){
  $export = array();
  $all_groups = array(
    'text' => array(
      'group_char_definition' => array(
        'label' => 'Definition',
        'weight' => 0,
        'children' => array(
          'field_char_description',
          'field_char_image',
          'title'
        )
      ),
      'group_char_group' => array(
        'label' => 'Groups',
        'weight' => 1,
        'children' => array()
      ),
      'group_char_condition' => array(
        'label' => 'Conditions',
        'weight' => 2,
        'children' => array(
          'field_char_condition'
        )
      )
    ),
    'dna' => array(
      'group_char_condition' => array(
        'label' => 'Conditions',
        'weight' => 6,
        'children' => array(
          'field_char_condition'
        )
      ),
      'group_char_definition' => array(
        'label' => 'Definition',
        'weight' => 4,
        'children' => array(
          'field_char_description',
          'field_char_image',
          'title'
        )
      ),
      'group_char_group' => array(
        'label' => 'Groups',
        'weight' => 5,
        'children' => array()
      )
    ),
    'controlled' => array(
      'group_char_definition' => array(
        'label' => 'Definition',
        'weight' => 0,
        'children' => array(
          'field_char_and_or',
          'field_char_description',
          'field_char_image',
          'field_char_ordered',
          'title'
        )
      ),
      'group_char_settings' => array(
        'label' => 'Settings',
        'weight' => 1,
        'children' => array(
          'field_char_ordered',
          'field_char_and_or'
        )
      ),
      'group_char_states' => array(
        'label' => 'States',
        'weight' => 2,
        'children' => array(
          'field_char_states'
        )
      ),
      'group_char_group' => array(
        'label' => 'Groups',
        'weight' => 3,
        'children' => array()
      ),
      'group_char_condition' => array(
        'label' => 'Conditions',
        'weight' => 4,
        'children' => array(
          'field_char_condition'
        )
      )
    ),
    'numeric' => array(
      'group_char_definition' => array(
        'label' => 'Definition',
        'weight' => 0,
        'children' => array(
          'field_char_description',
          'field_char_image',
          'field_char_integer',
          'field_char_unit',
          'title'
        )
      ),
      'group_char_settings' => array(
        'label' => 'Settings',
        'weight' => 1,
        'children' => array(
          'field_char_integer'
        )
      ),
      'group_char_group' => array(
        'label' => 'Groups',
        'weight' => 2,
        'children' => array()
      ),
      'group_char_condition' => array(
        'label' => 'Conditions',
        'weight' => 3,
        'children' => array(
          'field_char_condition'
        )
      )
    ),
    'computed' => array(
      'group_char_definition' => array(
        'label' => 'Definition',
        'weight' => 0,
        'children' => array(
          'field_char_description',
          'field_char_image',
          'title'
        )
      ),
      'group_char_expression' => array(
        'label' => 'Expression',
        'weight' => 1,
        'children' => array(
          'field_char_expr'
        )
      ),
      'group_char_group' => array(
        'label' => 'Groups',
        'weight' => 2,
        'children' => array()
      ),
      'group_char_condition' => array(
        'label' => 'Conditions',
        'weight' => 3,
        'children' => array(
          'field_char_condition'
        )
      )
    ),
    'group' => array(
      'group_char_definition' => array(
        'label' => 'Definition',
        'weight' => 0,
        'children' => array(
          'field_char_description',
          'field_char_image',
          'title'
        )
      ),
      'group_char_group' => array(
        'label' => 'Groups',
        'weight' => 1,
        'children' => array()
      )
    )
  );
  foreach($all_groups as $bundle => $groups){
    // Create the grouping group.
    $field_group = new stdClass();
    $field_group->disabled = FALSE;
    $field_group->api_version = 1;
    $field_group->identifier = 'group_char_tabs|character_editor_character|' . $bundle . '|form';
    $field_group->group_name = 'group_char_tabs';
    $field_group->entity_type = 'character_editor_character';
    $field_group->bundle = $bundle;
    $field_group->mode = 'form';
    $field_group->parent_name = '';
    $field_group->data = array(
      'label' => 'Tabs',
      'weight' => '0',
      'children' => array(),
      'format_type' => 'htabs',
      'format_settings' => array(
        'formatter' => '',
        'instance_settings' => array(
          'classes' => ''
        )
      )
    );
    $export['group_char_tabs|character_editor_character|' . $bundle . '|form'] = $field_group;
    // Now create that bundle's specific groups
    foreach($groups as $group => $info){
      $field_group = new stdClass();
      $field_group->disabled = FALSE;
      $field_group->api_version = 1;
      $field_group->identifier = $group . '|character_editor_character|' . $bundle . '|form';
      $field_group->group_name = $group;
      $field_group->entity_type = 'character_editor_character';
      $field_group->bundle = $bundle;
      $field_group->mode = 'form';
      $field_group->parent_name = 'group_char_tabs';
      $field_group->data = array(
        'label' => $info['label'],
        'weight' => $info['weight'],
        'children' => $info['children'],
        'format_type' => 'htab',
        'format_settings' => array(
          'label' => $info['label'],
          'instance_settings' => array(
            'display_mode' => '',
            'required_fields' => 0,
            'classes' => '',
            'description' => ''
          ),
          'formatter' => 'closed'
        )
      );
      $export[$group . '|character_editor_character|' . $bundle . '|form'] = $field_group;
      $export['group_char_tabs|character_editor_character|' . $bundle . '|form']->data['children'][] = $group;
    }
  }
  return $export;
}