<?php

/**
 * @file
 * Field formatters for Views RSS: Core Elements module.
 */

/**
 * Implements hook_field_formatter_info().
 */
function views_rss_core_field_formatter_info() {
  $formatters = array(
    // Generic image formatter for <enclosure> element.
    'enclosure_image' => array(
      'label' => t('RSS <enclosure> element'),
      'field types' => array('image'),
    ),
    // Generic file formatter for <enclosure> element.
    'enclosure_file' => array(
      'label' => t('RSS <enclosure> element'),
      'field types' => array('file'),
    ),
  );
  return $formatters;
}

/**
 * Implements hook_field_formatter_settings_form().
 */
function views_rss_core_field_formatter_settings_form($field, $instance, $view_mode, $form, &$form_state) {
  $display = $instance['display'][$view_mode];
  $settings = $display['settings'];
  $element = NULL;

  if ($field['type'] == 'image') {
    $image_styles = image_style_options(FALSE);
    $element['image_style'] = array(
      '#title' => t('Image style'),
      '#type' => 'select',
      '#default_value' => $settings['image_style'],
      '#empty_option' => t('None (original image)'),
      '#options' => $image_styles,
    );
  }

  return $element;
}

/**
 * Implements hook_field_formatter_settings_summary().
 */
function views_rss_core_field_formatter_settings_summary($field, $instance, $view_mode) {
  $display = $instance['display'][$view_mode];
  $settings = $display['settings'];

  $summary = array();

  if ($field['type'] == 'image') {
    $image_styles = image_style_options(FALSE);
    // Unset possible 'No defined styles' option.
    unset($image_styles['']);
    // Styles could be lost because of enabled/disabled modules that defines
    // their styles in code.
    if (isset($image_styles[$settings['image_style']])) {
      $summary[] = t('Image style: @style', array('@style' => $image_styles[$settings['image_style']]));
    }
    else {
      $summary[] = t('Original image');
    }
  }

  return implode('<br />', $summary);
}

/**
 * Implements hook_field_formatter_view().
 *
 * This is a dirty trick here. Essentially, we do not want to call a theme
 * function from here, as it should be called from within a view (amongst
 * other to have $view object in $variables). Therefore here we want to
 * return value only, hence use of array('#markup' => $value). However,
 * in some cases it won't be just a simple string value to return,
 * sometimes we'd want to return an array (for example value with
 * additional arguments) - hence the need to serialize it (plus add
 * "serialized" string at the beginning so that our field preprocess
 * function template_preprocess_views_view_views_rss_field() is able
 * to recognize it as serialized array and treat accordingly.
 *
 * Any better ideas?
 */
function views_rss_core_field_formatter_view($entity_type, $entity, $field, $instance, $langcode, $items, $display) {
  $element = NULL;
  foreach ($items as $delta => $item) {
    // Inside a view item may contain NULL data. In that case, just return.
    if (empty($item['fid'])) {
      unset($items[$delta]);
      continue;
    }
    // Get full image URL based on provided image style.
    if ($field['type'] == 'image' && !empty($display['settings']['image_style']) && $image_style = image_style_load($display['settings']['image_style'])) {
      $uri = image_style_url($display['settings']['image_style'], $item['uri']);
      // Get file size of image preset file (if it has already been created,
      // otherwise just create it first and then get file size).
      $path = image_style_path($display['settings']['image_style'], $item['uri']);
      $real_path = drupal_realpath($path);
      if (file_exists($real_path) || image_style_create_derivative($image_style, $item['uri'], $path)) {
        $item['filesize'] = filesize($real_path);
      }
    }
    else {
      $uri = file_create_url($item['uri']);
    }
    
    // XML element array in format_xml_elements() format.
    $rss_element = array(
      'key' => 'enclosure',
      'attributes' => array(
        'url' => $uri,
        'length' => $item['filesize'],
        'type'   => $item['filemime'],
      ),
    );

    $element[$delta] = array(
      '#item' => $item,
      '#markup' => format_xml_elements(array($rss_element)),
      '#rss_element' => $rss_element,
    );
        
  }
  return $element;
}
