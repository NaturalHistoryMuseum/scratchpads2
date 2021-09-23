<?php

/**
 * Theme preprocessor
 * Preprocess a relation select view
 */
function relation_select_preprocess_views_view_relation_select(&$variables) {
  $variables['class'] = 'views-view-relation-select';
  $view = $variables['view'];
  $result = $variables['result'] = $variables['rows'];
  $handler = $view->style_plugin;
  $fields = &$view->field;
  $columns = $handler->sanitize_columns($fields);
  $renders = $handler->render_fields($result);
  $variables['rows'] = array();
  foreach ($columns as $field => $column) {
    foreach ($result as $num => $row) {
      if ($field == $column && empty($fields[$field]->options['exclude'])) {
        $variables['rows'][$num][$column] = $renders[$num][$field];
      }
    }
  }
  $count = 0;
  foreach ($variables['rows'] as $num => $row) {
    $variables['row_classes'][$num][] = ($count++ % 2 == 0) ? 'odd' : 'even';
    if ($row_class = $handler->get_row_class($num)) {
      $variables['row_classes'][$num][] = $row_class;
    }
    // IS this an EFQ view?
    if ($view->efq) {
      $entity = $result[$num];
      $entity_type = $result[$num]->entity_type;
      list($entity_id, $entity_revision, $bundle) = entity_extract_ids($entity_type, $entity);
    }
    else {
      $entity_id = $result[$num]->{$view->query->base_field};
      $entity_type = relation_select_entity_get_type_from_view($view);
      $entity = entity_load($entity_type, array($entity_id));
      $bundle = relation_select_entity_get_bundle($entity_type, reset($entity));
    }
    $variables['row_entity'][$num] = array(
      'entity_type' => $entity_type,
      'entity_id' => $entity_id,
      'bundle' => $bundle
    );
  }
  $variables['row_classes'][0][] = 'views-row-first';
  $variables['row_classes'][count($variables['row_classes']) - 1][] = 'views-row-last';
}

/**
 * Theme function
 * Output a link to open the add relation page
 */
function theme_relation_select_link($variables) {
  $entity_type = $variables['entity_type'];
  $entity_id = $variables['entity_id'];
  return l('Add relation', 'relation-select/add/' . $variables['entity_type'] . '/' . $variables['entity_id'] . (isset($variables['relation_type']) ? '/' . $variables['relation_type'] : ''));
}

/**
 * Theme preprocessor
 * Suggest different theme function for theming the value
 * Allows people to change the display
 */
function relation_select_preprocess_relation_select_item_value(&$variables) {
  $exploded_value = explode(':', $variables['element']['#value']);
  $variables['entity_type'] = $exploded_value[0];
  $variables['entity_id'] = $exploded_value[1];
  $variables['theme_hook_suggestions'][] = 'relation_select_' . $variables['entity_type'] . 'item_value';
}

function relation_select_preprocess_relation_select_field(&$variables) {
  $variables['list_type'] = 'ul';
  $variables['items'] = array();
  foreach ($variables['related_entities'] as $related_entity) {
    $link = entity_uri($related_entity->entity_type, $related_entity);
    $variables['items'][] = l(entity_label($related_entity->entity_type, $related_entity), $link['path']);
  }

  $variables['theme_hook_suggestions'][] = 'relation_select_field__' . $variables['field_name'];
  $variables['theme_hook_suggestions'][] = 'relation_select_field__' . $variables['entity_type'] . '__' . $variables['bundle'] . '__' . $variables['field_name'];
}

/**
 * Theme function
 * Output a selected value
 */
function theme_relation_select_item_value($variables) {
  $entity = entity_load($variables['entity_type'], array(
    $variables['entity_id']
  ));
  if (is_array($entity)) {
    $entity = current($entity);
  }
  if ($label = entity_label($variables['entity_type'], $entity)) {
    $output = '<span class="title">' . $label . '</span> (' . $variables['element']['#value'] . ')';
  }
  else {
    $output = $variables['element']['#value'];
  }
  return $output;
}

function theme_relation_select_item_wrapper($variables) {
  $path = drupal_get_path('module', 'relation_select');
  $element = $variables['element'];
  $output = '<div class="rs-wrapper">' . $element['#children'];
  $output .= theme('relation_select_item_value', $variables);
  if (!empty($element['#description'])) {
    $output .= '<div class="description">' . $element['#description'] . '</div>';
  }
  $output .= '</div>';
  return $output;
}

function theme_relation_select_attachment_before($variables) {
  $path = drupal_get_path('module', 'relation_select');
  return theme('image', array(
    'path' => $path . '/img/close.png',
    'width' => 24,
    'height' => 24,
    'alt' => t('Close'),
    'attributes' => array(
      'class' => 'relation-select-close'
    )
  ));
}
