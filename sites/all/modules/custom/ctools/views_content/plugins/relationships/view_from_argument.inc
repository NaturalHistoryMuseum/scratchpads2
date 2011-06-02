<?php
// $Id$

/**
 * @file
 * Plugin to provide an relationship handler for a view by argument input settings.
 */

/**
 * Plugins are described by creating a $plugin array which will be used
 * by the system that includes this file.
 */
$plugin = array(
  'title' => t('View From Argument'),
  'description' => t('Creates a view context from argument input settings.'),
  'context' => 'views_content_view_from_argument_context',
  'get child' => 'views_content_view_from_argument_get_child',
  'get children' => 'views_content_view_from_argument_get_children',
);

function views_content_view_from_argument_get_child($plugin, $parent, $child) {
  $plugins = views_content_view_from_argument_get_children($plugin, $parent);
  return $plugins[$parent . ':' . $child];
}

function views_content_view_from_argument_get_children($plugin, $parent) {
  $types = array();
  // It can be fairly intensive to calculate this, so let's cache this in the
  // cache_views table. The nice thing there is that if views ever change, that
  // table will always be cleared. Except for the occasional default view, so
  // we must use the Views caching functions in order to respect Views caching
  // settings.
  views_include('cache');
  $data = views_cache_get('views_context_views', TRUE);
  if (!empty($data->data)) {
    $types = $data->data;
  }

  if (empty($types)) {
    $types = array();

    $views = views_get_all_views();

    foreach ($views as $view) {
      if (!empty($view->disabled)) {
        continue;
      }

      $view->init_display();

      foreach ($view->display as $id => $display) {
        if (empty($display->handler->context_display)) {
          continue;
        }

        $info = _views_content_context_relationships($view, $display, $parent);
        if ($info) {
          $types[$parent . ':' . $view->name . '-' . $id] = $info;
        }
      }

      $view->destroy();
    }
    views_cache_set('views_context_views', $types, TRUE);
  }

  return $types;
}

function _views_content_context_relationships($view, $display, $parent) {
  // Ensure the handler is the right type, as Views will fall back to
  // the default display if something is broken:
  if (get_class($display->handler) != 'views_content_plugin_display_ctools_context') {
    return;
  }

  $title = t('View @view: @display', array('@view' => $view->name, '@display' => $display->display_title));
  $description = $view->description;
  $contexts = array();

  $arguments = $display->handler->get_argument_input();
  ctools_include('views');
  foreach ($arguments as $argument) {
    $argument['label'] = $argument['name'] ? $argument['name'] : '';
    $contexts[] = ctools_views_get_argument_context($argument);
  }

  return array(
    'title' => $title,
    'description' => filter_xss_admin($description),
    'required context' => $contexts,
    'keyword' => 'view',
    'context' => 'views_content_view_from_argument_context',
    'context name' => $view->name . '-' . $display->id,
    'name' => $parent . ':' . $view->name . '-' . $display->id,
  );
}

/**
 * Return a new context based on an existing context.
 */
function views_content_view_from_argument_context($contexts, $conf) {
  $name = $conf['name'];
  list($plugin, $view_data) = explode(':', $name);
  list($view_name, $display_id) = explode('-', $view_data);
  $keys = array_keys($conf['context']);

  if (empty($contexts[$keys[0]]->data)) {
    return ctools_context_create_empty('view', NULL);
  }

  // Load our view up.
  $data = views_get_view($view_name);
  if ($data) {
    // Set the display.
    $data->set_display($display_id);
    $context_keys = array_keys($contexts);
    foreach ($data->display_handler->get_argument_input() as $id => $argument) {
      if ($argument['type'] == 'context') {
        $key = array_shift($context_keys);
        if (isset($contexts [$key])) {
          if (strpos($argument['context'], '.')) {
            list($context, $converter) = explode('.', $argument['context'], 2);
            $args[] = ctools_context_convert_context($contexts[$key], $converter);
          }
          else {
            $args[] = $contexts[$key]->argument;
          }
        }
      }
    }
    // remove any trailing NULL arguments as these are non-args:
    while (count($args) && end($args) === NULL) {
      array_pop($args);
    }
    $data->set_arguments($args);
    if ($path = $data->display_handler->get_option('inherit_panels_path')) {
      $data->override_path = $_GET['q'];
    }
  }

  if (isset($contexts[$keys[0]]->data)) {
    return ctools_context_create('view', $data);
  }
}
