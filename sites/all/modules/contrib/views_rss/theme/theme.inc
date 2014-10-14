<?php

/**
 * @file
 * Theme functions for Views RSS module.
 */

/**
 * Template preprocessor for views-view-views-rss.tpl.php.
 */
function template_preprocess_views_view_views_rss(&$variables) {
  $view = $variables['view'];

  // Start building the feed array compatible with format_xml_elements().
  $rss_feed = array(
    'key' => 'rss',
    'attributes' => array(
      'version' => '2.0',
    ),
    'value' => array(),
  );

  // Prepare namespaces and add them to feed array.
  // xml:base cannot be defined by hook_views_rss_namespaces() implementation,
  // as we don't want its value to be cached, it needs to be dynamic.
  $rss_feed['attributes']['xml:base'] = url('<front>', array('absolute' => TRUE));

  // All other namespaces are taken from hook_views_rss_namespaces() implementations.
  foreach (views_rss_get('namespaces') as $module => $module_namespaces) {
    foreach ($module_namespaces as $namespace => $definition) {

      // Check if definition provided through modules hooks
      // should be overwritten by module configuration.
      if (
        isset($view->style_plugin->options['namespaces'][$module][$namespace])
        && !empty($view->style_plugin->options['namespaces'][$module][$namespace])
      ) {
        $definition['uri'] = $view->style_plugin->options['namespaces'][$module][$namespace];
      }

      // Add namespace to feed array.
      if (isset($definition['uri']) && !empty($definition['uri'])) {
        // Namespaces with prefix, for example xml:base="" or xmlns:dc=""
        if (isset($definition['prefix']) && !empty($definition['prefix'])) {
          $namespace_key = $definition['prefix'] . ':' . $namespace;
          $rss_feed['attributes'][$namespace_key] = $definition['uri'];
        }
        // Namespaces without prefix, for example: content="" or foaf=""
        else {
          $rss_feed['attributes'][$namespace] = $definition['uri'];
        }
      }

    }
  }

  // Prepare <channel> elements and add them to feed array.
  $rss_channel = array();
  foreach (views_rss_get('channel_elements') as $module => $module_channel_elements) {
    foreach ($module_channel_elements as $element => $definition) {
      list($namespace, $element_name) = views_rss_extract_element_names($element, 'core');

      // Try to fetch namespace value from view configuration.
      if (isset($view->style_plugin->options['channel'][$namespace][$module][$element_name])) {
        $value = $view->style_plugin->options['channel'][$namespace][$module][$element_name];
      }
      // Otherwise check if it was provided by element definition.
      elseif (isset($definition['default_value'])) {
        $value = $definition['default_value'];
      }
      else {
        $value = NULL;
      }

      // Start building XML channel element array compatible with format_xml_elements().
      $rss_elements = array(
        array(
          'key' => $element,
          'value' => $value,
        ),
      );

      // Preprocess element value.
      if (isset($definition['preprocess functions']) && is_array($definition['preprocess functions'])) {
        foreach ($definition['preprocess functions'] as $preprocess_function) {
          if (function_exists($preprocess_function)) {
            $item_variables = array(
              'elements' => &$rss_elements,
              'item' => $view->style_plugin->options['channel'],
              'view' => $view,
            );
            $preprocess_function($item_variables);
          }
        }
      }

      // If there is no value and no attributes (in case of self-closing elements)
      // already set for the element at this stage, it is not going to be set
      // at any point further, so the element should not be added to the feed.
      foreach ($rss_elements as $key => $rss_element) {
        if (empty($rss_element['value']) && empty($rss_element['attributes'])) {
          unset($rss_elements[$key]);
        }
      }
      if (empty($rss_elements)) continue;

      // Add XML element(s) to the channel array.
      $rss_channel = array_merge($rss_channel, $rss_elements);
    }
  }
  // Channel array should not be added to the feed array yet, as first
  // it needs to get populated with all <item> elements.

  // Prepare <item> elements and add them to the channel array.
  $item_elements = views_rss_get('item_elements');
  $items = $view->style_plugin->map_rows($variables['rows']);
  foreach ($items as $item_key => $item) {

    // Start building XML item element array compatible with format_xml_elements().
    $rss_item = array(
      'key' => 'item',
      'value' => array(),
    );

    // Preprocess whole item array before preprocessing separate elements.
    foreach (module_implements('views_rss_preprocess_item') as $module) {
      $preprocess_function = $module . '_views_rss_preprocess_item';
      $item_variables = array(
        'item' => &$item,
        'view' => $view,
      );
      // Add raw row if generated based on raw item values provided by field formatter.
      if (!empty($view->views_rss['raw_items'][$item_key])) {
        $item_variables['raw'] = $view->views_rss['raw_items'][$item_key];
      }
      $preprocess_function($item_variables);
    }

    // Process each element separately.
    foreach ($item as $module => $module_item_elements) {
      foreach ($module_item_elements as $element => $value) {

        // Start building XML element array compatible with format_xml_elements().
        $rss_elements = array(
          array(
            'key' => $element,
            'value' => $value,
          ),
        );

        // Preprocess element initial value if required.
        if (isset($item_elements[$module][$element]['preprocess functions']) && is_array($item_elements[$module][$element]['preprocess functions'])) {
          foreach ($item_elements[$module][$element]['preprocess functions'] as $preprocess_function) {
            if (function_exists($preprocess_function)) {
              $item_variables = array(
                'elements' => &$rss_elements,
                'item' => $item,
                'view' => $view,
              );
              // Add raw item if provided by field formatter.
              if (!empty($view->views_rss['raw_items'][$item_key][$module][$element])) {
                $item_variables['raw'] = $view->views_rss['raw_items'][$item_key][$module][$element];
              }
              $preprocess_function($item_variables);
            }
          }
        }
        // If no preprocess function was defined, and we have received
        // #rss_element value (XML element array) from the formatter, it should
        // be added to the feed array without any further modifications.
        elseif (
          !empty($view->views_rss['raw_items'][$item_key][$module][$element])
          && is_array($view->views_rss['raw_items'][$item_key][$module][$element])
        ) {
          // At this point we don't know yet if we got #rss_elements in raw
          // values, so do not overwrite and empty main $rss_elements yet, just
          // start working with new $formatter_rss_elements - it could be
          // overwritten once we are sure we have all required values.
          $formatter_rss_elements = array();
          foreach ($view->views_rss['raw_items'][$item_key][$module][$element] as $raw_item) {
            if (!empty($raw_item['rendered']['#rss_element'])) {
              $formatter_rss_elements[] = $raw_item['rendered']['#rss_element'];
            }
          }
          // Now we can overwrite main $rss_elements.
          if (!empty($formatter_rss_elements)) {
            $rss_elements = $formatter_rss_elements;
          }
        }

        // If there is no value and no attributes (in case of self-closing elements)
        // already set for the element at this stage, it is not going to be set
        // at any point further, so the element should not be added to the feed.
        foreach ($rss_elements as $key => $rss_element) {
          if (empty($rss_element['value']) && empty($rss_element['attributes'])) {
            unset($rss_elements[$key]);
          }
        }
        if (empty($rss_elements)) continue;

        // Add XML element(s) to the item array.
        $rss_item['value'] = array_merge($rss_item['value'], $rss_elements);
      }
    }

    // Add XML item element to the channe; array.
    $rss_channel[] = $rss_item;
  }

  // Finally, let's put together the whole feed array.
  $rss_feed['value'][] = array(
    'key' => 'channel',
    'value' => $rss_channel,
  );

  // Allow for altering it before final render and passing it to the template.
  drupal_alter('views_rss_feed', $rss_feed);

  // Assign rendered feed to the template variables.
  $variables['rss_feed'] = format_xml_elements(array($rss_feed));

  // Set XML header.
  drupal_add_http_header('Content-Type', 'application/rss+xml; charset=utf-8');
}