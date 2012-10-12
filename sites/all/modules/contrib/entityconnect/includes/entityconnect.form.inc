<?php
/**
 * @file
 * Handles all form alters and submit functions for entityconnect.
 */

/**
 * Implements hook_field_attach_form().
 *
 * Here we attach a "Add" et "Edit" submit button
 * for each allowed entityreference field.
 */
function _entityconnect_field_attach_form($entity_type, $entity, &$form, &$form_state, $field_name, $field, $langcode = LANGUAGE_NONE) {
  // Set value with root level information.
  $language = $form[$field_name]['#language'];
  $field_container = &$form[$field_name];
  $widget_container = &$field_container[$language];

  if (isset($widget_container['#field_parents'])) {
    foreach ($widget_container['#field_parents'] as $key1 => $parent) {
      if (!isset($parents)) {
        $parents = $parent;
      }
      else {
        $parents .= "-" . $parent;
      }
    }
  }

  if (!empty($entity_type) && !empty($entity)) {
    list(, , $bundle) = entity_extract_ids($entity_type, $entity);
    $instance = field_info_instance($form['#entity_type'], $field_name, $bundle);
  }

  if (!isset($instance['entityconnect_unload_add'])) {
    $instance['entityconnect_unload_add'] = 0;
  }

  if (!isset($instance['entityconnect_unload_edit'])) {
    $instance['entityconnect_unload_edit'] = 0;
  }

  $addbuttonallowed = $instance['entityconnect_unload_add'];
  $editbuttonallowed = $instance['entityconnect_unload_edit'];

  $field = field_info_field($field_name);
  $acceptable_types = NULL;

  // We get bundles referenced.
  if ($field['module'] == 'entityreference') {
    $entity_reference_info = entityreference_get_selection_handler($field, $instance);
    $entity_type = $entity_reference_info->field['settings']['target_type'];

    if (isset($entity_reference_info->field['settings']['handler_settings']['target_bundles'])) {
      $acceptable_types = $entity_reference_info->field['settings']['handler_settings']['target_bundles'];
    }
    elseif (isset($entity_reference_info->field['settings']['handler_settings']['view'])) {
      $name = $entity_reference_info->field['settings']['handler_settings']['view']['view_name'];
      $display = $entity_reference_info->field['settings']['handler_settings']['view']['display_name'];
      $views = views_get_view($name);
      $views_display = $views->display;

      if (isset($views_display[$display]->display_options['filters']['type'])) {
        $acceptable_types = $views_display[$display]->display_options['filters']['type']['value'];
      }
      else {
        $acceptable_types = $views_display['default']->display_options['filters']['type']['value'];
      }
    }
  }
  elseif ($field['module'] == 'node_reference') {
    $entity_type = 'node';
    $acceptable_types = $field['settings']['referenceable_types'];
  }
  elseif ($field['module'] == 'user_reference') {
    $entity_type = 'user';
    $acceptable_types = $field['settings']['referenceable_roles'];
  }

  // We currently should separate Autocomplete widget from others default widget
  // because "Edit" button will not react well on multiple selected items.
  // Autocomplete widget has not #type value, so we are testing it
  // via $widget_container['#type'].
  // We also test if $widget_container['view'] is set to support
  // the widget provided by entityreference_view_widget module.
  if (isset($widget_container['#type']) || isset($widget_container['view'])) {
    isset($parents) ? $parents : $parents = '';
    $extra_class = isset($widget_container['#type']) ? $widget_container['#type'] : 'autocomplete';
    $extra_class .= (!isset($widget_container['#cardinality']) || $widget_container['#cardinality'] == 1) ? ' single-value' : ' multiple-values';
    if (user_access('entityconnect add button') && $addbuttonallowed == 0) {
      entityconnect_add_form_element_new($field_container, $language, $field_name, $parents, 'all', $extra_class, $instance, $entity_type, $acceptable_types);
    }
    if (user_access('entityconnect edit button') && $editbuttonallowed == 0) {
      entityconnect_add_form_element_edit($field_container, $language, $field_name, $parents, 'all', $extra_class, $entity_type);
    }
  }
  else {
    // Type is not set, so multiple widgets.
    // We have to add some extra js for single value fields so we add
    // a class to let the js know.
    $extra_class = isset($widget_container['#type']) ? $widget_container['#type'] : 'autocomplete';
    $extra_class .= (!isset($widget_container['#cardinality']) || $widget_container['#cardinality'] == 1) ? ' single-value' : ' multiple-values';
    foreach ($widget_container as $key => &$widget_element) {
      if (is_numeric($key)) {
        if (user_access('entityconnect add button') && $addbuttonallowed == 0) {
          isset($parents) ? $parents : $parents = '';
          entityconnect_add_form_element_new($widget_element, $language, $field_name, $parents, $key, $extra_class, $instance, $entity_type, $acceptable_types);
        }
        if (user_access('entityconnect edit button') && $editbuttonallowed == 0) {
          isset($parents) ? $parents : $parents = '';
          entityconnect_add_form_element_edit($widget_element, $language, $field_name, $parents, $key, $extra_class, $entity_type);
        }
      }
    }
  }
}

/**
 * Here we attach a "Add" submit button.
 */
function entityconnect_add_form_element_new(&$widget_element, $language, $field_name, $parents, $key, $extra_class, $instance, $entity_type, $acceptable_types = NULL) {
  $widget_element["add_entityconnect__{$field_name}_{$key}_{$parents}"] = array(
    '#type' => 'submit',
    '#limit_validation_errors' => array(),
    '#value' => t('New content'),
    '#name' => "add_entityconnect__{$field_name}_{$key}_{$parents}",
    '#prefix' => "<div class = 'entityconnect-add $extra_class'>",
    '#suffix' => '</div>',
    '#key' => $key,
    '#field' => $field_name,
    '#entity_type_target' => $entity_type,
    '#acceptable_types' => $acceptable_types,
    '#add_child' => TRUE,
    '#language' => $language,
    '#submit' => array('entityconnect_include_form', 'entityconnect_add_edit_button_submit'),
    '#weight' => -2,
    '#attached' => array(
      'js' => array(
        drupal_get_path('module', 'entityconnect') . "/theme/js/entityconnect.js",
      ),
      'css' => array(
        drupal_get_path('module', 'entityconnect') . "/theme/css/entityconnect.css",
      ),
    ),
  );
}

/**
 * Here we attach a "Edit" submit button.
 */
function entityconnect_add_form_element_edit(&$widget_element, $language, $field_name, $parents, $key, $extra_class, $entity_type) {
  $widget_element["edit_entityconnect__{$field_name}_{$key}_{$parents}"] = array(
    '#type' => 'submit',
    '#limit_validation_errors' => array(array($field_name)),
    '#value' => t('Edit content'),
    '#name' => "edit_entityconnect__{$field_name}_{$key}_{$parents}",
    '#prefix' => "<div class = 'entityconnect-edit $extra_class'>",
    '#suffix' => '</div>',
    '#key' => $key,
    '#field' => $field_name,
    '#entity_type_target' => $entity_type,
    '#add_child' => FALSE,
    '#language' => $language,
    '#submit' => array('entityconnect_include_form', 'entityconnect_add_edit_button_submit'),
    '#weight' => -2,
    '#attached' => array(
      'js' => array(
        drupal_get_path('module', 'entityconnect') . "/theme/js/entityconnect.js",
      ),
      'css' => array(
        drupal_get_path('module', 'entityconnect') . "/theme/css/entityconnect.css",
      ),
    ),
  );
}

/**
 * Call when a new entity is to be added or edited.
 *
 * We cache the current state and form
 * and redirect to the add or edit page with an append build_cached_id.
 */
function entityconnect_add_edit_button_submit(&$form, &$form_state) {
  $cache_id = "entityconnect-" . $form['#build_id'];
  $field = $form_state['clicked_button']['#field'];
  $key   = $form_state['clicked_button']['#key'];

  $entity_type = $form_state['clicked_button']['#entity_type_target'];
  $acceptable_types = isset($form_state['clicked_button']['#acceptable_types']) ? $form_state['clicked_button']['#acceptable_types'] : NULL;

  $field_info = field_info_field($field);

  // Get the list of all parents element for the clicked button.
  $parents = isset($form_state['clicked_button']['#parents']) ? $form_state['clicked_button']['#parents'] : NULL;
  $key_exists = NULL;
  $field_container = entityconnect_array_get_nested_value($form_state['input'], $parents, $key_exists);

  if ($field_info['module'] == 'entityreference') {
    if ($key_exists) {
      $target_id = isset($field_container['target_id']) ? $field_container['target_id'] : '';
      if ($target_id == '' && is_array($field_container)) {
        foreach ($field_container as $key => $value) {
          if (is_array($value)) {
            foreach ($value as $key2 => $value2) {
              if (!is_null($value2)) {
                $target_id[$key2] = $value2;
              }
            }
          }
          else {
            $target_id = $value;
          }
        }
      }
    }

    // TODO : Find the original problem of the entity connect use with
    // Field Collection.
    if (!empty($target_id)) {
      // Take "label (entity id)', match the id from parenthesis.
      if (preg_match("/.+\((\d+)\)/", $target_id, $matches)) {
        $target_id = $matches[1];
      }
    }
  }
  elseif ($field_info['module'] == 'node_reference') {
    if ($key_exists) {
      $target_id = isset($field_container['nid']) ? $field_container['nid'] : '';
      if ($target_id == '' && is_array($field_container)) {
        foreach ($field_container as $key => $value) {
          if (is_array($value)) {
            foreach ($value as $key2 => $value2) {
              if (!is_null($value2)) {
                $target_id[$key2] = $value2;
              }
            }
          }
          else {
            $target_id = $value;
          }
        }
      }
    }

    // TODO : Find the original problem of the entity connect use with
    // Field Collection.
    if (!empty($target_id)) {
      // Take "label (entity id)', match the id from parenthesis.
      if (preg_match("/.+\[nid:(\d+)\]/", $target_id, $matches)) {
        $target_id = $matches[1];
      }
    }
  }
  elseif ($field_info['module'] == 'user_reference') {
    if ($key_exists) {
      $target_id = isset($field_container['uid']) ? $field_container['uid'] : '';
      if ($target_id == '' && is_array($field_container)) {
        foreach ($field_container as $key => $value) {
          if (is_array($value)) {
            foreach ($value as $key2 => $value2) {
              if (!is_null($value2)) {
                $target_id[$key2] = $value2;
              }
            }
          }
          else {
            $target_id = $value;
          }
        }
      }
    }

    // TODO : Find the original problem of the entity connect use with
    // Field Collection.
    if (!empty($target_id)) {
      // Take "label (entity id)', match the id from parenthesis.
      if (preg_match("/.+\[uid:(\d+)\]/", $target_id, $matches)) {
        $target_id = $matches[1];
      }
    }
  }

  $data = array(
    'form'       => $form,
    'form_state' => $form_state,
    'dest'       => $_GET['q'],
    'field'      => $field,
    'field_info' => $field_info,
    'key'        => $key,
    'add_child'  => $form_state['clicked_button']['#add_child'],
    'target_id'  => $target_id,
    'target_entity_type' => $entity_type,
    'acceptable_types' => $acceptable_types,
  );

  entityconnect_cache_set($cache_id, $data);

  unset($_GET['destination']);
  if ($data['add_child']) {
    $form_state['redirect'] = "admin/entityconnect/add/$cache_id";
  }
  else {
    if ($data['target_id']) {
      $form_state['redirect'] = "admin/entityconnect/edit/$cache_id";
    }
  }
}

/**
 * Sets submit button on child create form.
 *
 * On submiting of a child form we set:
 * the target_id in the cache entry
 * the rediect to our redirect page.
 */
function entityconnect_child_form_submit(&$form, &$form_state) {
  $cache_id = $form_state['values']['parent_build_cache_id'];

  if ($cache_id && ($cache = cache_get($cache_id))) {
    $data = $cache->data;
    $entity_type = $cache->data['target_entity_type'];

    switch ($entity_type) {
      case 'node':
        $data['target_id'] = $form_state['values']['nid'];
        break;

      case 'user':
        $data['target_id'] = $form_state['values']['name'];
        break;

      case 'taxonomy_term':
        $data['target_id'] = $form_state['values']['tid'];
        break;

      case 'taxonomy_vocabulary':
        $data['target_id'] = $form_state['values']['vid'];
        break;

      default:
        break;
    }

    entityconnect_cache_set($cache_id, $data);
    $form_state['redirect'] = "admin/entityconnect/return/$cache_id";
  }
}

/**
 * Sets the redirect to a admin/entityconnect/redirect page.
 */
function entityconnect_child_form_cancel(&$form, &$form_state) {
  $cache_id = $form_state['clicked_button']['#parent_build_cache_id'];
  if ($cache_id && ($cache = cache_get($cache_id))) {
    $form_state['redirect'] = "admin/entityconnect/return/$cache_id";
  }
}

/**
 * Complete entityreference field on parent form with the target_id value.
 *
 * This is for when we return to the parent page
 * we find the cached form and form_state clean up the form_state a bit
 * and mark it to be rebuilt.
 *
 * If the cache as a target_id we set that in the input.
 */
function entityconnect_return_form_alter(&$form, &$form_state, $form_id, $cache_id, $cache) {
  if (!isset($form_state['#entityconnect_processed'])) {
    $form_state = $cache->data['form_state'];
    $form = $cache->data['form'];
    $form_state['submitted'] = FALSE;
    $form_state['rebuild'] = TRUE;
    $data = $cache->data;
    $parents = isset($form_state['clicked_button']['#parents']) ? $form_state['clicked_button']['#parents'] : NULL;

    unset($form_state['clicked_button']);
    unset($form_state['submit_handlers']);

    $key_exists = NULL;
    $field_container = entityconnect_array_get_nested_value($form, $parents, $key_exists);
    if ($key_exists) {
      $language = isset($field_container['#language']) ? $field_container['#language'] : LANGUAGE_NONE;
    }

    isset($language) ? $widget_container = &$field_container[$language] : $widget_container = $field_container;
    isset($widget_container['#type']) ? $widget_container_type = $widget_container['#type'] : $widget_container_type = 'autocomplete';

    $field_info = field_info_field($data['field']);

    if (isset($data['target_id']) && $form_state['rebuild'] === TRUE) {
      switch ($data['target_entity_type']) {
        case 'node':
          if ($data['target_id'] != 0 && $node = node_load($data['target_id'])) {
            // ['#default_value'] should have differents build
            // function of the widget type.
            switch ($widget_container_type) {
              // Autocomplete tags style.
              case 'textfield':
                $element['#default_value'] = $node->title;
                $element['#default_value'] .= ' (' . $node->nid . ')';
                if (!empty ($widget_container['#value'])) {
                  $element['#default_value'] .= ', ' . $widget_container['#value'];
                }
                break;

              // Select list.
              case 'select':
                if ($widget_container['#multiple'] == FALSE) {
                  $element['#default_value'] = $node->nid;
                }
                else {
                  $element['#default_value'] = $node->nid;
                  $element['#default_value'] = $widget_container['#value'] + array($element['#default_value'] => $element['#default_value']);
                }
                break;

              // Radios widget.
              case 'radios':
                $element['#default_value'] = $node->nid;
                break;

              // Checkboxes widget.
              case 'checkboxes':
                $element['#default_value'] = $node->nid;
                $element['#default_value'] = $widget_container['#value'] + array($element['#default_value'] => $element['#default_value']);
                break;

              default:
                if ($field_info['module'] == 'entityreference') {
                  $element['#default_value'] = $node->title;
                  $element['#default_value'] .= ' (' . $node->nid . ')';
                }

                if ($field_info['module'] == 'node_reference') {
                  $element['#default_value'] = $node->nid;
                  // Use the autocomplete value so we are sure we validate.
                  $element['#default_value'] = node_reference_autocomplete_value($element, FALSE, $form_state);
                }
                break;
            }
          }
          break;

        case 'user':
          if (isset($data['target_id']) && $user = user_load_by_name($data['target_id'])) {
            switch ($widget_container_type) {
              // Autocomplete tags style.
              case 'textfield':
                $element['#default_value'] = $user->name;
                $element['#default_value'] .= ' (' . $user->uid . ')';
                if (!empty ($widget_container['#value'])) {
                  $element['#default_value'] .= ', ' . $widget_container['#value'];
                }
                break;

              // Select list.
              case 'select':
                if ($widget_container['#multiple'] == FALSE) {
                  $element['#default_value'] = $user->uid;
                }
                else {
                  $element['#default_value'] = $user->uid;
                  $element['#default_value'] = $widget_container['#value'] + array($element['#default_value'] => $element['#default_value']);
                }
                break;

              // Radios widget.
              case 'radios':
                $element['#default_value'] = $user->uid;
                break;

              // Checkboxes widget.
              case 'checkboxes':
                $element['#default_value'] = $user->uid;
                $element['#default_value'] = $form[$data['field']][$language]['#value'] + array($element['#default_value'] => $element['#default_value']);
                break;

              default:
                if ($field_info['module'] == 'entityreference') {
                  $element['#default_value'] = $user->name;
                  $element['#default_value'] .= ' (' . $user->uid . ')';
                }

                if ($field_info['module'] == 'user_reference') {
                  $element['#default_value'] = $user->uid;
                  $element['#default_value'] = user_reference_autocomplete_value($element, FALSE, $form_state);
                }
                break;
            }
          }
          break;

        case 'taxonomy_term':
          if (($data['target_id'] != NULL || $data['target_id'] != 0) && $term = taxonomy_term_load($data['target_id'])) {
            switch ($widget_container_type) {
              // Autocomplete tags style.
              case 'textfield':
                $element['#default_value'] = $term->name;
                $element['#default_value'] .= ' (' . $term->tid . ')';
                if (!empty ($widget_container['#value'])) {
                  $element['#default_value'] .= ', ' . $widget_container['#value'];
                }
                break;

              // Select list.
              case 'select':
                if ($widget_container['#multiple'] == FALSE) {
                  $element['#default_value'] = $term->tid;
                }
                else {
                  $element['#default_value'] = $term->tid;
                  $element['#default_value'] = $widget_container['#value'] + array($element['#default_value'] => $element['#default_value']);
                }
                break;

              // Radios widget.
              case 'radios':
                $element['#default_value'] = $term->tid;
                break;

              // Checkboxes widget.
              case 'checkboxes':
                $element['#default_value'] = $term->tid;
                $element['#default_value'] = $form[$data['field']][$language]['#value'] + array($element['#default_value'] => $element['#default_value']);
                break;

              default:
                if ($field_info['module'] == 'entityreference') {
                  $element['#default_value'] = $term->name;
                  $element['#default_value'] .= ' (' . $term->tid . ')';
                }
                break;
            }
          }
          break;

        case 'taxonomy_vocabulary':
          if (($data['target_id'] != NULL || $data['target_id'] != 0) && $vocabulary = taxonomy_vocabulary_load($data['target_id'])) {
            switch ($widget_container_type) {
              // Autocomplete tags style.
              case 'textfield':
                $element['#default_value'] = $vocabulary->name;
                $element['#default_value'] .= ' (' . $vocabulary->vid . ')';
                if (!empty ($widget_container['#value'])) {
                  $element['#default_value'] .= ', ' . $widget_container['#value'];
                }
                break;

              // Select list.
              case 'select':
                if ($widget_container['#multiple'] == FALSE) {
                  $element['#default_value'] = $vocabulary->vid;
                }
                else {
                  $element['#default_value'] = $vocabulary->vid;
                  $element['#default_value'] = $widget_container['#value'] + array($element['#default_value'] => $element['#default_value']);
                }
                break;

              // Radios widget.
              case 'radios':
                $element['#default_value'] = $vocabulary->vid;
                break;

              // Checkboxes widget.
              case 'checkboxes':
                $element['#default_value'] = $vocabulary->vid;
                $element['#default_value'] = $form[$data['field']][$language]['#value'] + array($element['#default_value'] => $element['#default_value']);
                break;

              default:
                if ($field_info['module'] == 'entityreference') {
                  $element['#default_value'] = $vocabulary->name;
                  $element['#default_value'] .= ' (' . $vocabulary->vid . ')';
                }
                break;
            }
          }
          break;

        default:
          break;
      }
      _entityconnect_alter_form_state_input($form_state, $field_info['module'], $widget_container_type, $parents, $language, $element['#default_value']);
    }
    $form_state['#entityconnect_processed'] = TRUE;
    $form = drupal_rebuild_form($form_id, $form_state, $form);
  }
}

/**
 * Wraps cache set.
 *
 * We can set the expire easily if needed.
 */
function entityconnect_cache_set($cid, $data) {
  return cache_set($cid, $data, 'cache', CACHE_PERMANENT);
}

/**
 * Alters child create form.
 *
 * We add a value field to hold the parent build_cache_id
 * then we add a cancel button that run entityconnect_child_form_cancel
 * and a new submit button.
 */
function entityconnect_child_form_alter(&$form, &$form_state, $form_id, $cache_id, $cache) {
  $form['parent_build_cache_id'] = array(
    '#type' => 'value',
    '#value' => $cache_id,
  );
  $form['actions']['cancel'] = array(
    '#type' => 'submit',
    '#value' => t('Cancel'),
    '#submit' => array('entityconnect_include_form', 'entityconnect_child_form_cancel'),
    '#parent_build_cache_id' => $cache_id,
    '#limit_validation_errors' => array(),
    '#weight' => 20,
  );

  switch ($form_id) {
    case 'user_register_form':
      $form['actions']['submit']['#submit'][] = 'user_register_submit';
      break;

    case 'user_profile_form':
      $form['actions']['submit']['#submit'][] = 'user_profile_form_submit';
      break;

    case 'taxonomy_form_term':
      $form['actions']['submit']['#submit'][] = 'taxonomy_form_term_submit';
      break;

    case 'taxonomy_form_vocabulary':
      $form['actions']['submit']['#submit'][] = 'taxonomy_form_vocabulary_submit';
      break;

    default:
      break;
  }
  $form['actions']['submit']['#submit'][] = 'entityconnect_include_form';
  $form['actions']['submit']['#submit'][] = 'entityconnect_child_form_submit';
}

/**
 * Used to update the form state value.
 *
 * Form state value is updated for entityreference,
 * node_reference or user_reference field after adding a new entity.
 *
 * @param array $form_state
 *   The form_state we need to change.
 * @param string $module
 *   As entityconnect deals with 3 different modules,
 *   we need to know which one is used.
 * @param string $widget_type
 *   The type of the widget used for reference field.
 * @param array $parents
 *   The array of all parents of the field.
 *   We used them to change the value to the right level in the array.
 * @param string $language
 *   The language of the field.
 * @param array $element
 *   The value we need to insert.
 */
function _entityconnect_alter_form_state_input(&$form_state, $module, $widget_type, $parents, $language, $element) {
  switch ($widget_type) {
    case 'autocomplete':
      switch ($module) {
        case 'entityreference':
          array_push($parents, "target_id");
          break;

        case 'node_reference':
          array_push($parents, "nid");
          break;

        case 'user_reference':
          array_push($parents, "uid");
          break;
      }
      break;

    case 'textfield':
    case 'select':
    case 'radios':
    case 'checkboxes':
      array_push($parents, $language);
      break;

    default:
      break;
  }
  entityconnect_array_set_nested_value($form_state['input'], $parents, $element);
}

/**
 * Reimplements drupal_array_get_nested_value() to answer specific needs of the module.
 *
 * Entityconnect button adds a new level in parents array,
 * named "add_entityconnect" and "edit_entityconnect" and we need to remove
 * them into our array building. So we need to rewrite
 * drupal_array_set_nested_value() to include these remove.
 *
 * @see drupal_array_get_nested_value()
 */
function &entityconnect_array_get_nested_value(array &$array, array $parents, &$key_exists = NULL) {
  $ref = &$array;

  foreach ($parents as $parent) {
    if (stripos($parent, 'add_entityconnect') === FALSE
            && stripos($parent, 'edit_entityconnect') === FALSE) {
      if (is_array($ref) && array_key_exists($parent, $ref)) {
        $ref = &$ref[$parent];
      }
      else {
        $key_exists = FALSE;
        $null = NULL;
        return $null;
      }
    }
  }
  $key_exists = TRUE;
  return $ref;
}

/**
 * Reimplements drupal_array_set_nested_value() to ansewer specific needs of the module.
 *
 * Entityconnect button adds a new level in parents array,
 * named "add_entityconnect" and "edit_entityconnect" and we need to remove
 * them into our array building. So we need to rewrite
 * drupal_array_set_nested_value() to include these remove.
 *
 * @see drupal_array_set_nested_value()
 */
function entityconnect_array_set_nested_value(array &$array, array $parents, $value, $force = FALSE) {
  $ref = &$array;
  foreach ($parents as $parent) {
    if (stripos($parent, 'add_entityconnect') === FALSE
        && stripos($parent, 'edit_entityconnect') === FALSE) {
      // PHP auto-creates container arrays and NULL entries without error
      // if $ref is NULL, but throws an error if $ref is set, but not an array.
      if ($force && isset($ref) && !is_array($ref)) {
        $ref = array();
      }
      $ref = &$ref[$parent];
    }
  }
  $ref = $value;
}
