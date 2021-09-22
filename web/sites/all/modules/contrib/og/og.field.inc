<?php


/**
 * @file
 * Field module functionality for the Organic groups module.
 */

/**
 * Implements hook_field_info().
 */
function og_field_info() {
  return array(
    'group' => array(
      'label' => t('Group audience'),
      'description' => t('This field stores groups associated with the content.'),
      'default_widget' => OG_AUDIENCE_WIDGET,
      'default_formatter' => 'og_list_default',
      'property_callbacks' => array('og_field_property_callback'),
    ),
  );
}

/**
 * OG audience field metadata callback.
 */
function og_field_property_callback(&$info, $entity_type, $field, $instance, $field_type) {
  // Set the property type to "group".
  $field_type['property_type'] = 'group';

  // Then apply the default.
  entity_metadata_field_default_property_callback($info, $entity_type, $field, $instance, $field_type);
}

/**
 * Implements hook_field_formatter_info().
 */
function og_field_formatter_info() {
  return array(
    'og_list_default' => array(
      'label' => t('Group default list'),
      'field types' => array('group'),
    ),
  );
}

/**
 * Implements hook_field_formatter_view().
 */
function og_field_formatter_view($entity_type, $entity, $field, $instance, $langcode, $items, $display) {
  $element = array();

  if ($field['field_name'] == OG_AUDIENCE_FIELD && !empty($items[0])) {
    foreach ($items as $delta => $item) {
      if ($group = og_get_group('group', $item['gid'])) {
        if ($group->access()) {
          $label = og_label($group->gid);
          $entity = entity_load($group->entity_type, array($group->etid));
          $entity = current($entity);
          // Get the entity type of the group entity.
          $uri = entity_uri($group->entity_type, $entity);
          $element[$delta] = array(
            '#type' => 'link',
            '#title' => $label,
            '#href' => $uri['path'],
            '#options' => array(
              'html' => TRUE,
            ),
          );
        }
        else {
          // No need to show private groups several times, so remember if it was
          // already added.
          $added = &drupal_static(__FUNCTION__, FALSE);

          if (!$added) {
            $added = TRUE;
            $element[$delta] = array(
              '#markup' => '- ' . t('Private group') . ' -',
            );
          }
        }
      }
    }
  }

  return $element;
}


/**
 * Implements hook_field_widget_info().
 */
function og_field_widget_info() {
  return array(
    OG_AUDIENCE_WIDGET => array(
      'label' => t('Group audience'),
      'field types' => array('group'),
      'behaviors' => array(
        'multiple values' => FIELD_BEHAVIOR_CUSTOM,
      ),
      'settings' => array(
        'opt_group' => 'auto',
      ),
    ),
    OG_AUDIENCE_AUTOCOMPLETE_WIDGET => array(
      'label'       => t('Autocomplete text field'),
      'description' => t('Display the list of referenceable groups as a textfield with autocomplete behaviour.'),
      'field types' => array('group'),
      'settings'    => array(
        'autocomplete_match' => 'contains',
        'size' => 60,
        'autocomplete_path' => 'group/autocomplete',
      ),
    ),
  );
}

/**
 * Implements hook_field_widget_settings_form().
 */
function og_field_widget_settings_form($field, $instance) {
  $widget   = $instance['widget'];
  $defaults = field_info_widget_settings($widget['type']);
  $settings = array_merge($defaults, $widget['settings']);
  $form = array();

  if ($widget['type'] == OG_AUDIENCE_WIDGET) {

    $form['opt_group'] = array(
      '#type' => 'radios',
      '#title' => t('Input type'),
      '#description' => t('Select the input type that should be used to get the groups audience. Note that the <em>Always show "other groups"</em> option will show all groups including the ones the user is a not a member of.'),
      '#options' => array(
        'auto' => t('Automatically decide the input according to user permissions (Recommended)'),
        'never' => t('Never show "other groups"'),
        'always' => t('Always show "other groups"'),
      ),
      '#default_value' => $settings['opt_group'],
      '#required' => TRUE,
    );
  }
  elseif ($widget['type'] == OG_AUDIENCE_AUTOCOMPLETE_WIDGET) {
    $form['autocomplete_match'] = array(
      '#type'             => 'select',
      '#title'            => t('Autocomplete matching'),
      '#default_value'    => $settings['autocomplete_match'],
      '#options'          => array(
        'starts_with'     => t('Starts with'),
        'contains'        => t('Contains'),
      ),
      '#description'      => t('Select the method used to collect autocomplete suggestions. Note that <em>Contains</em> can cause performance issues on sites with thousands of nodes.'),
    );
    $form['size'] = array(
      '#type'             => 'textfield',
      '#title'            => t('Size of textfield'),
      '#default_value'    => $settings['size'],
      '#element_validate' => array('element_validate_integer_positive'),
      '#required'         => TRUE,
    );
  }

  return $form;
}

/**
 * Implements hook_field_widget_form().
 *
 * Unlike options_field_widget_form() our widget's logic is a bit different, as
 * the form element type is a result of the user's access to the groups.
 * For example a privileged user may see all groups as an optgroup select list,
 * where the groups are divided to "My groups" and "Other groups". This means
 * that the $element['#type'] is a result of the options passed to
 * $element['#options'].
 */
function og_field_widget_form(&$form, &$form_state, $field, $instance, $langcode, $items, $delta, $base) {
  $element = $base;
  $widget = $instance['widget'];

  switch ($widget['type']) {
    case OG_AUDIENCE_AUTOCOMPLETE_WIDGET:
      $element += array(
        '#type' => 'textfield',
        '#default_value' => isset($items[$delta]['gid']) ? $items[$delta]['gid'] : NULL,
        '#autocomplete_path' => $instance['widget']['settings']['autocomplete_path'] . '/' . $field['field_name'],
        '#size' => $instance['widget']['settings']['size'],
        '#element_validate' => array('og_field_audience_autocomplete_validate'),
        '#value_callback' => 'og_field_audience_autocomplete_value',
      );

      $return = array('gid' => $element);

      break;

    case OG_AUDIENCE_WIDGET:
      // Determine if a user may see other groups as-well.
      $opt_group = FALSE;

      if ($instance['widget']['settings']['opt_group'] == 'always' || ($instance['widget']['settings']['opt_group'] == 'auto' && user_access('administer group'))) {
        $opt_group = TRUE;
      }

      // If this is a user account, base the list on the account being edited.
      $account = NULL;
      if (isset($form['#user'])) {
        $account = $form['#user'];
      }

      // Get all the groups divided to "content groups" and "other groups".
      $audience = og_field_audience_options($opt_group, $account);

      // Get all groups that should be excluded.
      $excludes = array();
      // If it's an existing group, then exclude itself, as in some cases a group
      // can act also as a group content, but we want to prevent associating the
      // group to itself.
      if (!empty($form['#' . $element['#entity_type']])) {
        list($id) = entity_extract_ids($element['#entity_type'], $form['#' . $element['#entity_type']]);
        if (($group = og_get_group($element['#entity_type'], $id))) {
          $excludes[$group->gid] = $group->gid;
        }
      }

      // Get default values from URL, or from the edited entity.
      $default_values = og_get_context_by_url();
      // Keep the group ID of the selected items, as they might be needed again,
      // and we don't want to iterate over the items again.
      $items_gid = array();
      if (!empty($items)) {
        $gids = array();
        foreach ($items as $item) {
          $gids[] = $item['gid'];
        }
        // In case there are deleted groups, we trim the list to only the
        // existing ones.
        $gids = array_keys(og_load_multiple($gids));
        foreach ($gids as $gid) {
          $default_values[] = $gid;
          $items_gid[] = $gid;
        }
      }
// Since array_unique compares values according to (string)$a==(string)$b, this also cleans up duplicates such as "1" and 1
      $default_values = array_unique($default_values);

      foreach (array('content groups', 'other groups') as $key) {
        if (!empty($audience[$key])) {
          // Get the label un-sanitized, as they will be laster sanitized
          // according to the form type.
          $audience[$key] = og_label_multiple($audience[$key], FALSE);
        }
      }

      // The group options presented to the user.
      $options = array();
      $hidden_selected_gids = array();
      $type = 'select';
      if ($opt_group) {
        // Show "My groups" and "Other groups".
        if ($my_groups = array_diff_key($audience['content groups'], $excludes)) {
          $options += array(t('My groups') => $my_groups);
        }
        if ($other_groups = array_diff_key($audience['other groups'], $excludes)) {
          $options += array(t('Other groups') => $other_groups);
        }
      }
      else {
        // Show only "My groups".
        $options = array_diff_key($audience['content groups'], $excludes);
        // If there are items that are already selected but the user doesn't
        // have access to them, we need to keep track of them.
        $hidden_selected_gids = drupal_map_assoc(array_diff($items_gid, array_keys($options)));
      }

      if (empty($options)) {
        // There are no group, so don't show any input element.
        $type = 'item';
        $element['#description'] = t('There are no groups you can select from.');
        $properties = array();
      }
      else {
        if (empty($element['#description'])) {
          $element['#description'] = t('Select the groups this content should be associated with.');
        }

        $element['#multiple'] = $field['cardinality'] > 1 || $field['cardinality'] == FIELD_CARDINALITY_UNLIMITED;;
        // Don't make the field required, if there are no groups.
        $element['#required'] = $element['#required'] && !empty($options);

        $properties = _options_properties($type, $element['#multiple'], $element['#required'], $options);
        // If the element isn't required and cardinality is more than 1, and there
        // are some options, and a "none" option.
        if (!$element['#required'] && !$element['#multiple']) {
          // Use a dummy instance in order to use theme('options_none');
          $dummy_instance['widget']['type'] = 'options_select';
          $options = array('_none' => theme('options_none', array('instance' => $dummy_instance))) + $options;
        }
      }


      $element += array(
        // Input should be TRUE only if there are groups that can be selected.
        '#input' => $type != 'item',
        '#type' => $type,
        '#options' => $options,
        '#default_value' => $default_values,
        '#attributes' => array('class' => array('group-audience')),
        '#disabled' => empty($options),
        // Re-use options widget element validation, to correctly transform
        // submitted values from field => delta to delta => field.
        // @see options_field_widget().
        '#value_key' => 'gid',
        '#element_validate' => $type != 'item' ? array('options_field_widget_validate') : array(),
        '#properties' => $properties,
        // Add OG specific context.
        '#opt_group' => $opt_group,
        '#audience' => $audience,
        '#hidden_selected_gids' => $hidden_selected_gids,
      );

      $return = $element;
      break;
  }

  return $return;
}

/**
 * Default value function for OG audience fields.
 *
 * Set the default from the URL context. This works even if the widget is
 * not shown, e.g. due to restricted field access.
 *
 * @see og_og_fields_info()
 */
function og_field_audience_default_value($entity_type, $entity, $field, $instance, $langcode) {
  $gids = og_get_context_by_url();
  if (!$gids) {
    return;
  }
  $items = array();
  $groups = og_load_multiple($gids);
  foreach ($groups as $gid => $group) {
    // Check access to group, and user is a group member.
    if ($group->access() && og_is_member($gid)) {
      $items[] = array('gid' => $gid);
    }
  }
  return $items;
}

/**
 * Implements hook_field_is_empty().
 */
function og_field_is_empty($item, $field) {
  return empty($item['gid']);
}

/**
 * Implements hook_field_attach_insert().
 */
function og_field_attach_insert($entity_type, $entity) {
  og_field_crud_group('insert', $entity_type, $entity);
}

/**
 * Implements hook_field_attach_update().
 */
function og_field_attach_update($entity_type, $entity) {
  og_field_crud_group('update', $entity_type, $entity);
}

/**
 * Implements hook_field_attach_delete().
 */
function og_field_attach_delete($entity_type, $entity) {
  og_field_crud_group('delete', $entity_type, $entity);
}

/**
 * Implements hook_field_insert().
 *
 * DEPRECATE WHEN FIELD SCHEMA CAN BE CHANGED.
 */
function og_field_insert($entity_type, $entity, $field, $instance, $langcode, &$items) {
  og_field_write('insert', $entity_type, $entity, $field, $instance, $langcode, $items);
}

/**
 * Implements hook_field_update().
 *
 * DEPRECATE WHEN FIELD SCHEMA CAN BE CHANGED.
 */
function og_field_update($entity_type, $entity, $field, $instance, $langcode, &$items) {
  og_field_write('update', $entity_type, $entity, $field, $instance, $langcode, $items);
}

 /**
 * Insert or update a field record.
 *
 * DEPRECATE WHEN FIELD SCHEMA CAN BE CHANGED.
 *
 * Since we can not change the field schema we must still populate the fields
 * with non-null values.
 *
 * @param $op
 *   The operation - "insert" or "update".
 */
function og_field_write($op, $entity_type, $entity, $field, $instance, $langcode, &$items) {
  $wrapper = entity_metadata_wrapper($entity_type, $entity);
  $group_memberships = array();
  foreach ($wrapper->og_membership->value() as $group_membership) {
    $group_memberships[$group_membership->gid] = $group_membership;
  }

  foreach ($items as &$item) {
    $gid = $item['gid'];
    if (!empty($group_memberships[$gid])) {
      $item['state'] = $group_memberships[$gid]->state;
      $item['created'] = $group_memberships[$gid]->created;
    }
    else {
      $item += array('state' => OG_STATE_ACTIVE, 'created' => time());
    }

  }
}

/**
 * Implements hook_field_attach_form().
 *
 * Handle translated nodes that act as groups.
 */
function og_field_attach_form($entity_type, $entity, &$form, $form_state, $langcode) {
  if (!empty($form['#node_edit_form']) &&
    (og_is_group_type('node', $form['type']['#value']) || og_is_group_content_type('node', $form['type']['#value'])) &&
    (!empty($entity->tnid) && $entity->tnid != $entity->nid) || (!empty($_GET['translation']) && !empty($_GET['target']))) {

    // Iterate over OG fields that shouldn't be changed in node translation.
    foreach (og_fields_info() as $field_name => $info) {
      if (!empty($form[$field_name]) && $info['disable on node translate']) {
        // Prevent changing the group state on nodes that are translated.
        $description = t('You can not change "@label" field from a translated content.', array('@label' => $info['instance']['label']));

        $tnid = !empty($entity->tnid) ? $entity->tnid : $_GET['translation'];
        if (($node = node_load($tnid)) && node_access('update', $node)) {
          $description .= ' ' . t('Changing this field can only be done via <a href="@node">@title</a>.', array('@node' => url('node/' . $node->nid . '/edit'), '@title' => $node->title));
        }

        $form[$field_name][LANGUAGE_NONE]['#options'] = array();
        $form[$field_name][LANGUAGE_NONE]['#description'] = $description;
        $form[$field_name][LANGUAGE_NONE]['#disabled'] = TRUE;
        $form[$field_name][LANGUAGE_NONE]['#required'] = FALSE;

      }
    }
  }
}

/**
 * Validate handler; Re-add hidden selected group IDs if exist.  Also, to be safe, check for and clean up duplicate entries.
 */
function og_field_widget_form_validate($form, &$form_state) {
  // The hidden group IDs.
  $gids = !empty($form[OG_AUDIENCE_FIELD][LANGUAGE_NONE]['#hidden_selected_gids']) ? $form[OG_AUDIENCE_FIELD][LANGUAGE_NONE]['#hidden_selected_gids'] : array();
  if (!empty($form_state['values'][OG_AUDIENCE_FIELD][LANGUAGE_NONE])) {
    foreach ($form_state['values'][OG_AUDIENCE_FIELD][LANGUAGE_NONE] as $key => $value) {
      // Add the selected group IDs.
      $gids[$value['gid']] = $value['gid'];
    }
  }

  $delta = 0;
  $value = array();
  foreach ($gids as $gid) {
    $value[LANGUAGE_NONE][$delta]['gid'] = $gid;
    ++$delta;
  }
  if (empty($value)) {
    return;
  }

  // Set the new values.
  form_set_value($form[OG_AUDIENCE_FIELD], $value, $form_state);
}

/**
 * Create update or delete a group, based on the field CRUD.
 *
 * TODO: Remove this an move to hook_entity_presave().
 *
 * @see og_field_attach_insert().
 * @see og_field_attach_update().
 * @see og_field_attach_delete().
 */
function og_field_crud_group($op, $entity_type, $entity) {
  $property = OG_GROUP_FIELD;

  // If the entity is a translated node, we can return early, as all operations
  // happen on the source node.
  if ($entity_type == 'node'
    && ((!empty($entity->tnid) && $entity->tnid != $entity->nid) || !empty($entity->translation_source->tnid))
    && module_exists('translation')) {
    return;
  }

  if (!empty($entity->{$property}) && empty($entity->og_skip_group_create)) {
    $wrapper = &$entity->{$property}[LANGUAGE_NONE];

    // Get the entity ID.
    list($id) = entity_extract_ids($entity_type, $entity);

    $group = og_get_group($entity_type, $id, TRUE, array(OG_STATE_ACTIVE, OG_STATE_PENDING));
    if ($op == 'delete') {
      if (!empty($group->gid)) {
        // Remove group.
        $group->delete();
      }
    }
    else {
      // Check group is new.
      if (empty($group->gid)) {
        if (!empty($wrapper[0]['value'])) {
          // Save the group to get the group ID.
          $group->save();

          // Subscribe the entity author, if exists.
          if (!empty($entity->uid) && ($account = user_load($entity->uid))) {
            $values = array('entity' => $account);
            og_group($group->gid, $values);
          }
        }
      }
      else {
        // Existing group.
        $save = FALSE;

        if ($group->state == OG_STATE_ACTIVE && empty($wrapper[0]['value'])) {
          $group->state = OG_STATE_PENDING;
          $save = TRUE;
        }
        elseif($group->state == OG_STATE_PENDING && !empty($wrapper[0]['value'])) {
          $group->state = OG_STATE_ACTIVE;
          $save = TRUE;
        }

        // Check if the entity label has changed.
        $label = og_entity_label($entity_type, $entity);

        if ($group->label != $label) {
          $group->label = $label;
          $save = TRUE;
        }

        if ($save) {
          $group->save();
        }
      }

      // Determine if field has changed and roles should be overridden, or
      // reverted, by comparing the default access field of the entity being
      // saved, and its original state.
      $property = OG_DEFAULT_ACCESS_FIELD;
      // The field exists.
      if (isset($entity->{$property})) {
        if (!empty($entity->{$property}[LANGUAGE_NONE][0]['value'])) {
          og_roles_override($group->gid);
        }
        elseif (empty($group->is_new)) {
          // If the field is set to be using default access and there are
          // already overridden roles we delete them.
          og_delete_user_roles_by_group($group->gid);
        }
      }
    }
  }
}


/**
 * Get an array of allowed values for OG audience field.
 *
 * @return
 *   Array keyed by "content groups" and "other groups".
 */
function og_field_audience_options(&$opt_group = FALSE, $account = NULL) {
  $return = &drupal_static(__FUNCTION__, array());

  // If we already have the full list of all groups, immediately return.
  if (isset($return['other groups']) && isset($return['content groups'])) {
    return $return;
  }
  // Or if we need just the groups to which the user has access, return
  // those.
  elseif (!$opt_group && isset($return['content groups'])) {
    return $return;
  }

  if (empty($account)) {
    global $user;
    $account = clone($user);
  }

  // Initialize values.
  $return = array('content groups' => array(), 'other groups' => array());

  $content_groups = og_get_entity_groups('user', $account);
  $return['content groups'] = $content_groups;

  // Only generate all other groups if needed (generally for admins only).
  if ($opt_group) {
    $all_groups = og_get_all_group();
    $return['other groups'] = array_diff($all_groups, $content_groups);
  }

  // Allow other modules to change the audience options. While it can be done
  // via hook_form_alter(), it will require other modules to know too much of
  // the internal work.
  drupal_alter('og_audience_options', $return, $opt_group, $account);

  return $return;
}

/**
 * Implements hook_field_validate().
 *
 * FIXME: Adapt this function to OG.
 *
 * Possible error codes:
 * - 'invalid_nid': nid is not valid for the field (not a valid node id, or the node is not referenceable).
 */
function __og_field_validate($entity_type, $entity, $field, $instance, $langcode, $items, &$errors) {
  // Extract nids to check.
  $ids = array();

  // First check non-numeric "nid's to avoid losing time with them.
  foreach ($items as $delta => $item) {
    if (is_array($item) && !empty($item['nid'])) {
      if (is_numeric($item['nid'])) {
        $ids[] = $item['nid'];
      }
      else {
        $errors[$field['field_name']][$langcode][$delta][] = array(
          'error' => 'invalid_nid',
          'message' => t("%name: invalid input.",
             array('%name' => $instance['label'])),
        );
      }
    }
  }
  // Prevent performance hog if there are no ids to check.
  if ($ids) {
    $refs = og_field_audience_potential_groups('', NULL, $ids);
    foreach ($items as $delta => $item) {
      if (is_array($item)) {
        if (!empty($item['nid']) && !isset($refs[$item['nid']])) {
          $errors[$field['field_name']][$langcode][$delta][] = array(
            'error' => 'invalid_nid',
            'message' => t("%name: this post can't be referenced.",
              array('%name' => $instance['label'])),
          );
        }
      }
    }
  }
}


/**
 * Value callback for a node_reference autocomplete element.
 *
 * Replace the node nid with a node title.
 */
function og_field_audience_autocomplete_value($element, $input = FALSE, $form_state) {
  if ($input === FALSE) {
    // We're building the displayed 'default value': expand the raw nid into
    // "Group title [gid:n]".
    if (!empty($element['#default_value']) && $group = og_load($element['#default_value'])) {
      $value = og_label($group->gid);
      $value .= ' [gid:' . $group->gid . ']';
      return $value;
    }
  }
}

/**
 * Validation callback for a group audience autocomplete element.
 */
function og_field_audience_autocomplete_validate($element, &$form_state, $form) {
  $field = $form_state['field'][$element['#field_name']][$element['#language']]['field'];
  $instance = $form_state['field'][$element['#field_name']][$element['#language']]['instance'];

  $value = $element['#value'];
  $gid = NULL;

  if (!empty($value)) {
    // Check whether we have an explicit "[gid:n]" input.
    preg_match('/^(?:\s*|(.*) )?\[\s*gid\s*:\s*(\d+)\s*\]$/', $value, $matches);
    if (!empty($matches)) {
      // Explicit gid. Check that the 'title' part matches the actual title for
      // the nid.
      list(, $label, $gid) = $matches;
      if (!empty($label)) {
        if ($label != og_label($gid)) {
          form_error($element, t('%name: label mismatch. Please check your selection.', array('%name' => $instance['label'])));
        }
      }
    }
    else {
      // No explicit gid (the submitted value was not populated by autocomplete
      // selection). Get the gid of a referencable node from the entered title.
      if ($reference = og_field_audience_potential_groups($value, 'equals', NULL, 1)) {
        $gid = key($reference);
      }
      else {
        form_error($element, t('%name: found no valid group with that label.', array('%name' => $instance['label'])));
      }
    }
  }

  // Set the element's value as the node id that was extracted from the entered
  // input.
  form_set_value($element, $gid, $form_state);
}

/**
 * Implements hook_field_widget_error().
 */
function og_field_widget_error($element, $error, $form, &$form_state) {
  form_error($element, $error['message']);
}

/**
 * Fetch an array of all candidate groups.
 *
 * This info is used in various places (allowed values, autocomplete
 * results, input validation...). Some of them only need the nids,
 * others nid + titles, others yet nid + titles + rendered row (for
 * display in widgets).
 *
 * The array we return contains all the potentially needed information,
 * and lets consumers use the parts they actually need.
 *
 * @param $field
 *   The field description.
 * @param $string
 *   Optional string to filter titles on (used by autocomplete).
 * @param $match
 *   Operator to match filtered name against, can be any of:
 *   'contains', 'equals', 'starts_with'
 * @param $ids
 *   Optional node ids to lookup (the $string and $match arguments will be
 *   ignored).
 * @param $limit
 *   If non-zero, limit the size of the result set.
 *
 * @return
 *   An array of valid nodes in the form:
 *   array(
 *     gid => 'rendered' -- The text to display in widgets (can be HTML)
 *   );
 *  @todo Check whether we still need the 'rendered' value (hook_options_list()
 *  does not need it anymore). Should probably be clearer after the 'Views'
 *  mode is ported.
 */
function og_field_audience_potential_groups($string = '', $match = 'contains', $ids = array(), $limit = NULL) {
  $results = &drupal_static(__FUNCTION__, array());

  // Create unique id for static cache.
  $cid = $match . ':'
    . ($string !== '' ? $string : implode('-', $ids))
    . ':' . $limit;
  if (!isset($results[$cid])) {
    $groups = og_field_audience_potential_groups_standard($string, $match, $ids, $limit);

    // Store the results.
    $results[$cid] = !empty($groups) ? $groups : array();
  }

  return $results[$cid];
}

/**
 * Helper function for og_field_audience_potential_groups().
 */
function og_field_audience_potential_groups_standard($string = '', $match = 'contains', $ids = array(), $limit = NULL) {
  $query = og_get_all_group(array(OG_STATE_ACTIVE), array('return query' => TRUE));

  $query->addField('og', 'label');
  if ($string !== '') {
    $args = array();
    switch ($match) {
      case 'contains':
        $title_clause = 'label LIKE :match';
        $args['match'] = '%' . $string . '%';
        break;

      case 'starts_with':
        $title_clause = 'label LIKE :match';
        $args['match'] = $string . '%';
        break;

      case 'equals':
      default: // no match type or incorrect match type: use "="
        $title_clause = 'label = :match';
        $args['match'] = $string;
        break;
    }
    $query->where($title_clause, $args);
  }
  elseif ($ids) {
    $query->condition('gid', $ids, 'IN');
  }

  $query->orderBy('label');

  if ($limit) {
    $query->range(0, $limit);
  }

  $gids = $query->execute()->fetchAllKeyed();
  $groups = og_load_multiple(array_keys($gids));
  foreach ($groups as $group) {
    $label = og_label($group->gid);
    $return[$group->gid] = $label;
  }
  return $return;
}

/**
 * Menu callback for the autocomplete results.
 */
function og_field_audience_autocomplete($field_name, $string = '') {
  $field = field_info_field($field_name);

  $match = isset($field['widget']['autocomplete_match']) ? $field['widget']['autocomplete_match'] : 'contains';
  $matches = array();

  $groups = og_field_audience_potential_groups($string, $match, array(), 10);
  foreach ($groups as $gid => $label) {
    // Add a class wrapper for a few required CSS overrides.
    $matches[$label . " [gid:$gid]"] = '<div class="group-autocomplete">' . $label . '</div>';
  }
  drupal_json_output($matches);
}
