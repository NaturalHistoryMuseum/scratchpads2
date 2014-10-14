<?php

/**
 * @file
 * Extends the view_plugin_style class to provide new RSS view style.
 */

class views_rss_plugin_style_fields extends views_plugin_style {

  /**
   * Attach this view to another display as a feed.
   */
  function attach_to($display_id, $path, $title) {
    $display = $this->view->display[$display_id]->handler;
    $url_options = array('absolute' => TRUE);
    $input = $this->view->get_exposed_input();
    if ($input) {
      $url_options['query'] = $input;
    }

    $url = url($this->view->get_url(NULL, $path), $url_options);
    if ($display->has_path() && !$this->options['feed_settings']['feed_in_links']) {
      if (empty($this->preview)) {
        drupal_add_feed($url, $title);
      }
    }
    else {
      if (empty($this->view->feed_icon)) {
        $this->view->feed_icon = '';
      }

      $this->view->feed_icon .= theme('feed_icon', array('url' => $url, 'title' => $title));
      drupal_add_html_head_link(array(
        'rel' => 'alternate',
        'type' => 'application/rss+xml',
        'title' => $title,
        'href' => $url,
      ));
    }
  }

  function option_definition() {
    $options = parent::option_definition();

    // Namespace defaults.
    $namespaces = views_rss_get('namespaces');
    if (count($namespaces)) {
      foreach ($namespaces as $module => $module_namespaces) {
        foreach (array_keys($module_namespaces) as $namespace) {
          $options['namespaces'][$module][$namespace] = array('default' => NULL);
        }
      }
    }

    // Channel element defaults.
    $channel_elements = views_rss_get('channel_elements');
    if (count($channel_elements)) {
      foreach ($channel_elements as $module => $module_channel_elements) {
        foreach (array_keys($module_channel_elements) as $element) {
          list($namespace, $element_name) = views_rss_extract_element_names($element, 'core');
          $options['channel'][$namespace][$module][$element_name] = array('default' => NULL);
        }
      }
    }

    // Item element defaults.
    $item_elements = views_rss_get('item_elements');
    if (count($item_elements)) {
      foreach ($item_elements as $module => $module_item_elements) {
        foreach (array_keys($module_item_elements) as $element) {
          list($namespace, $element_name) = views_rss_extract_element_names($element, 'core');
          $options['item'][$namespace][$module][$element_name] = array('default' => NULL);
        }
      }
    }

    // Other feed settings defaults.
    $options['feed_settings']['feed_in_links'] = array('default' => 0);

    return $options;
  }

  /**
   * Provide a form for setting options.
   *
   * @param array $form
   * @param array $form_state
   */
  function options_form(&$form, &$form_state) {
    parent::options_form($form, $form_state);

    $handlers = $this->display->handler->get_handlers('field');
    if (empty($handlers)) {
      drupal_set_message(t('You need at least one field before you can configure your field settings.'), 'error');
    }
    else {

      // Field chooser.
      $field_names = array('' => '--');
      foreach ($handlers as $field => $handler) {
        if ($label = $handler->label()) {
          $field_names[$field] = $label;
        }
        else {
          $field_names[$field] = $handler->ui_name();
        }
      }

      // Element groups could be used both in channel and item settings.
      $element_groups = views_rss_get('element_groups');

      // Channel elements settings.
      $channel_elements = views_rss_get('channel_elements');
      if (count($channel_elements)) {
        foreach ($channel_elements as $module => $module_channel_elements) {
          foreach ($module_channel_elements as $element => $definition) {
            if (!isset($definition['configurable']) || $definition['configurable']) {
              list($namespace, $element_name) = views_rss_extract_element_names($element, 'core');
              // Add fieldset for namespace if not yet added.
              if (!isset($form['channel'][$namespace])) {
                $form['channel'][$namespace] = array(
                  '#type' => 'fieldset',
                  '#title' => t('Channel elements : @namespace', array('@namespace' => $namespace)),
                  '#description' => t('Provide values for &lt;channel&gt; elements in "@namespace" namespace. See <a href="@guide_url">Views RSS documentation</a> for more information.', array(
                    '@namespace' => $namespace,
                    '@guide_url' => url('http://drupal.org/node/1344136'),
                  )),
                  '#collapsible' => TRUE,
                  '#collapsed' => TRUE,
                );
              }
              // Prepare form element.
              $default_value = NULL;
              if (!empty($this->options['channel'][$namespace][$module][$element_name])) {
                $default_value = $this->options['channel'][$namespace][$module][$element_name];
              }
              $form_item = array(
                '#type' => 'textfield',
                '#title' => filter_xss(isset($definition['title']) ? $definition['title'] : $element_name),
                '#description' => filter_xss(isset($definition['description']) ? $definition['description'] : NULL),
                '#default_value' => $default_value,
              );
              // Allow to overwrite default form element.
              if (!empty($definition['settings form'])) {
                $form_item = array_merge($form_item, $definition['settings form']);
                // Make sure that #options is an associative array.
                if (!empty($definition['settings form']['#options'])) {
                  $form_item['#options'] = views_rss_map_assoc($definition['settings form']['#options']);
                }
              }
              if (!empty($definition['settings form options callback'])) {
                $function = $definition['settings form options callback'];
                $form_item['#options'] = views_rss_map_assoc($function());
              }
              // Add help link if provided.
              if (!empty($definition['help'])) {
                $form_item['#description'] .= ' ' . l('[?]', $definition['help'], array('attributes' => array('title' => t('Need more information?'))));
              }
              // Check if element should be displayed in a subgroup.
              if (!empty($definition['group'])) {
                // Add a subgroup to the form if it not yet added.
                if (!isset($form['channel'][$namespace][$module][$definition['group']])) {
                  // Does module provide the group definition?
                  $group_title = !empty($element_groups[$module][$definition['group']]['title']) ? $element_groups[$module][$definition['group']]['title'] : $definition['group'];
                  $group_description = !empty($element_groups[$module][$definition['group']]['description']) ? $element_groups[$module][$definition['group']]['description'] : NULL;
                  $form['channel'][$namespace][$module][$definition['group']] = array(
                    '#type' => 'fieldset',
                    '#title' => filter_xss($group_title),
                    '#description' => filter_xss($group_description),
                    '#collapsible' => TRUE,
                    '#collapsed' => TRUE,
                  );
                }
                $form['channel'][$namespace][$module][$definition['group']][$element_name] = $form_item;
              }
              // Display element normally (not within a subgroup).
              else {
                $form['channel'][$namespace][$module][$element_name] = $form_item;
              }
            }
          }
        }
      }

      // Item elements settings.
      $item_elements = views_rss_get('item_elements');
      if (count($item_elements)) {
        foreach ($item_elements as $module => $module_item_elements) {
          foreach ($module_item_elements as $element => $definition) {
            if (!isset($definition['configurable']) || $definition['configurable']) {
              list($namespace, $element_name) = views_rss_extract_element_names($element, 'core');
              // Add fieldset for namespace if not yet added.
              if (!isset($form['item'][$namespace])) {
                $form['item'][$namespace] = array(
                  '#type' => 'fieldset',
                  '#title' => t('Item elements : @namespace', array('@namespace' => $namespace)),
                  '#description' => t('Select fields containing relevant values for &lt;item&gt; elements in "@namespace" namespace. See <a href="@guide_url">Views RSS documentation</a> for more information.', array(
                    '@namespace' => $namespace,
                    '@guide_url' => url('http://drupal.org/node/1344136'),
                  )),
                  '#collapsible' => TRUE,
                  '#collapsed' => TRUE,
                );
              }
              // Prepare form element.
              $default_value = NULL;
              if (!empty($this->options['item'][$namespace][$module][$element_name])) {
                $default_value = $this->options['item'][$namespace][$module][$element_name];
              }
              $form_item = array(
                '#type' => 'select',
                '#title' => filter_xss(isset($definition['title']) ? $definition['title'] : $element_name),
                '#description' => filter_xss(isset($definition['description']) ? $definition['description'] : NULL),
                '#options' => $field_names,
                '#default_value' => $default_value,
              );
              // Allow to overwrite default form element.
              if (!empty($definition['settings form'])) {
                $form_item = array_merge($form_item, $definition['settings form']);
                // Make sure that #options is an associative array.
                if (!empty($definition['settings form']['#options'])) {
                  $form_item['#options'] = views_rss_map_assoc($definition['settings form']['#options']);
                }
              }
              // Add help link if provided.
              if (isset($definition['help']) && $definition['help']) {
                $form_item['#description'] .= ' ' . l('[?]', $definition['help'], array('attributes' => array('title' => t('Need more information?'))));
              }
              // Check if element should be displayed in a subgroup.
              if (isset($definition['group']) && $definition['group']) {
                // Add a subgroup to the form if it not yet added.
                if (!isset($form['item'][$namespace][$module][$definition['group']])) {
                  // Does module provide the group definition?
                  $group_title = !empty($element_groups[$module][$definition['group']]['title']) ? $element_groups[$module][$definition['group']]['title'] : $definition['group'];
                  $group_description = !empty($element_groups[$module][$definition['group']]['description']) ? $element_groups[$module][$definition['group']]['description'] : NULL;
                  $form['item'][$namespace][$module][$definition['group']] = array(
                    '#type' => 'fieldset',
                    '#title' => filter_xss($group_title),
                    '#description' => filter_xss($group_description),
                    '#collapsible' => TRUE,
                    '#collapsed' => TRUE,
                  );
                }
                $form['item'][$namespace][$module][$definition['group']][$element_name] = $form_item;
              }
              // Display element normally (not within a subgroup).
              else {
                $form['item'][$namespace][$module][$element_name] = $form_item;
              }
            }
          }
        }
      }

      // Undefined namespaces derived from <channel> and/or <item>
      // elements defined by extension modules.
      $namespaces = views_rss_get('namespaces');
      if (count($namespaces)) {
        foreach ($namespaces as $module => $module_namespaces) {
          foreach ($module_namespaces as $namespace => $definition) {
            if (empty($definition['uri'])) {
              // Add fieldset for namespace if not yet added.
              if (!isset($form['namespaces'])) {
                $form['namespaces'] = array(
                  '#type' => 'fieldset',
                  '#title' => t('Namespaces'),
                  '#description' => t('Enter missing URLs for namespaces derived from &lt;channel&gt; and/or &lt;item&gt; elements defined by extension modules.'),
                  '#collapsible' => TRUE,
                  '#collapsed' => TRUE,
                );
              }
              if (!empty($this->options['namespaces'][$module][$namespace])) {
                $default_value = $this->options['namespaces'][$module][$namespace];
              }
              else {
                $default_value = NULL;
              }
              $form['namespaces'][$module][$namespace] = array(
                '#type' => 'textfield',
                '#title' => check_plain($namespace),
                '#default_value' => $default_value,
              );
            }
          }
        }
      }

      // Other feed settings.
      $form['feed_settings'] = array(
        '#type' => 'fieldset',
        '#title' => t('Other feed settings'),
        '#collapsible' => TRUE,
        '#collapsed' => TRUE,
      );
      $form['feed_settings']['absolute_paths'] = array(
        '#type' => 'checkbox',
        '#title' => t("Replace relative paths with absolute URLs"),
        '#description' => t('Enabling this option will replace all relative paths (like <em>/node/1</em>) with absolute URLs (<em>!absolute_url</em>) in all feed elements configured to use this feature (for example &lt;description&gt; element).', array(
          '!absolute_url' => trim($GLOBALS['base_url'], '/') . '/node/1',
        )),
        '#default_value' => !empty($this->options['feed_settings']['absolute_paths']) || !isset($this->options['feed_settings']['absolute_paths']),
        '#weight' => 1,
      );
      $form['feed_settings']['feed_in_links'] = array(
        '#type' => 'checkbox',
        '#title' => t('Display feed icon in the links attached to the view'),
        '#default_value' => !empty($this->options['feed_settings']['feed_in_links']),
        '#weight' => 3,
      );

    }
  }

  /**
   * Allow other modules to validate options form values prior to submit.
   */
  function options_validate(&$form, &$form_state) {
    foreach (module_implements('views_rss_options_form_validate') as $module) {
      $function = $module . '_views_rss_options_form_validate';
      $function($form, $form_state);
    }
  }

  /**
   * Allow other modules to perform any necessary changes
   * to options form values prior to storage.
   */
  function options_submit(&$form, &$form_state) {
    foreach (module_implements('views_rss_options_form_submit') as $module) {
      $function = $module . '_views_rss_options_form_submit';
      $function($form, $form_state);
    }
  }

  /**
   * Make sure the display and all associated handlers are valid.
   */
  function validate() {
    parent::validate();
    $errors = array();
    $channel_elements = views_rss_get('channel_elements');
    $item_elements = views_rss_get('item_elements');
    if (empty($channel_elements) && empty($item_elements)) {
      $errors[] = t('You have not enabled any modules providing feed elements. Please enable at least <a href="@modules_url">Views RSS: Core Elements</a> module.', array(
        '@modules_url' => url('admin/modules', array('fragment' => 'edit-modules-views')),
      ));
    }
    return $errors;
  }
  
  /**
   * Map views row result to an RSS item.
   */
  function map_rows($rows) {
    // Fields must be pre-rendered starting from version 2.3 of Views module.
    $rendered = $raw = array();
    $keys = array_keys($this->view->field);
    foreach ($rows as $count => $row) {
      $this->view->row_index = $count;
      foreach ($keys as $id) {
        $rendered[$count][$id] = $this->view->field[$id]->theme($row);
        // Also let's keep raw value for further processing.
        $field_name = 'field_' . $id;
        if (!empty($row->$field_name)) {
          $raw[$count][$id] = $row->$field_name;
        }
      }
    }

    // Rewrite view rows to XML item rows.
    $items = $raw_items = array();
    $item_elements = views_rss_get('item_elements');
    foreach ($rendered as $id => $row) {
      $item = $raw_item = array();
      foreach ($item_elements as $module => $module_item_elements) {
        foreach (array_keys($module_item_elements) as $element) {
          list($namespace, $element_name) = views_rss_extract_element_names($element, 'core');
          // Assign values for all elements, not only those defined in view settings.
          // If element value is not defined in view settings, let's just assign NULL.
          // It will not be passed to final theme function anyway during processing
          // taking place in template_preprocess_views_view_views_rss().
          if (
            isset($this->options['item'][$namespace][$module][$element_name])
            && isset($row[$this->options['item'][$namespace][$module][$element_name]])
          ) {
            $item[$module][$element] = $row[$this->options['item'][$namespace][$module][$element_name]];
          }
          else {
            $item[$module][$element] = NULL;
          }
          // Keep raw values too.
          if (
            !empty($this->options['item'][$namespace][$module][$element_name])
            && !empty($raw[$id][$this->options['item'][$namespace][$module][$element_name]])
          ) {
            $raw_item[$module][$element] = $raw[$id][$this->options['item'][$namespace][$module][$element_name]];
          }

        }
      }
      $items[$id] = $item;
      $raw_items[$id] = $raw_item;
    }

    $this->view->views_rss['raw_items'] = $raw_items;
    return $items;
  }

}
