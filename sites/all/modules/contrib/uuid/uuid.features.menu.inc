<?php

/**
 * @file
 * Features support to export menu items with UUID entities.
 */

/**
 * Implements hook_features_export_options().
 */
function uuid_menu_links_features_export_options() {
  global $menu_admin;
  // Need to set this to TRUE in order to get menu links that the
  // current user may not have access to (i.e. user/login)
  $menu_admin = TRUE;
  $menu_links = menu_parent_options(menu_get_menus(), array('mlid' => 0));
  $options = array();
  foreach ($menu_links as $key => $name) {
    list($menu_name, $mlid) = explode(':', $key, 2);
    if ($mlid != 0) {
      $link = menu_link_load($mlid);
      $identifier = uuid_menu_links_features_identifier($link);
      $options[$identifier] = "{$menu_name}: {$name}";
    }
  }
  $menu_admin = FALSE;
  return $options;
}

/**
 * Callback for generating the menu link exportable identifier.
 */
function uuid_menu_links_features_identifier($link) {
  if (isset($link['menu_name'])) {
    $id = $link['menu_name'] . ':';
  }
  elseif (isset($link['uuid_menu_name'])) {
    $id = $link['uuid_menu_name'] . ':';
  }
  else {
    return FALSE;
  }

  if (isset($link['link_path'])) {
    $id .= uuid_menu_path_to_uri_transform($link['link_path']);
  }
  elseif (isset($link['uuid_link_path'])) {
    $id .= $link['uuid_link_path'];
  }
  else {
    return FALSE;
  }

  return $id;
}

/**
 * Implements hook_features_export().
 */
function uuid_menu_links_features_export($data, &$export, $module_name = '') {
  // Default hooks are provided by the feature module so we need to add
  // it as a dependency.
  $export['dependencies']['features'] = 'features';
  $export['dependencies']['menu'] = 'menu';
  $export['dependencies']['uuid'] = 'uuid';

  // Collect a link to module map
  $pipe = array();
  $map = features_get_default_map('uuid_menu_links', 'uuid_menu_links_features_identifier');
  foreach ($data as $identifier) {
    if ($link = uuid_menu_link_load($identifier)) {
      // If this link is provided by a different module, add it as a dependency.
      if (isset($map[$identifier]) && $map[$identifier] != $module_name) {
        $export['dependencies'][$map[$identifier]] = $map[$identifier];
      }
      else {
        $export['features']['uuid_menu_links'][$identifier] = $identifier;
      }
      // For now, exclude a variety of common menus from automatic export.
      // They may still be explicitly included in a Feature if the builder
      // chooses to do so.
      if (!in_array($link['menu_name'], array('features', 'primary-links', 'secondary-links', 'navigation', 'admin', 'devel'))) {
        $pipe['menu_custom'][] = $link['menu_name'];
      }
    }
  }
  return $pipe;
}

/**
 * Implements hook_features_export_render()
 */
function uuid_menu_links_features_export_render($module, $data) {
  $code = array();
  $code[] = '  $uuid_menu_links = array();';
  $code[] = '';

  $translatables = array();
  foreach ($data as $identifier) {
    if ($link = uuid_menu_link_load($identifier)) {
      uuid_menu_link_make_universal($link, TRUE);

      $code[] = "  // Exported menu link: {$identifier}";
      $code[] = "  \$uuid_menu_links['{$identifier}'] = ". features_var_export($link, '  ') .";";
      $translatables[] = $link['link_title'];
    }
  }
  if (!empty($translatables)) {
    $code[] = features_translatables_export($translatables, '  ');
  }

  $code[] = '';
  $code[] = '  return $uuid_menu_links;';
  $code = implode("\n", $code);
  return array('uuid_menu_default_menu_links' => $code);
}

/**
 * Implements hook_features_revert().
 */
function uuid_menu_links_features_revert($module) {
  uuid_menu_links_features_rebuild($module);
}

/**
 * Implements hook_features_rebuild().
 */
function uuid_menu_links_features_rebuild($module) {
  if ($menu_links = features_get_default('uuid_menu_links', $module)) {
    uuid_menu_links_features_rebuild_ordered($menu_links);
  }
}

/**
 * Implements hook_features_enable_feature().
 */
function uuid_menu_links_features_enable_feature($module) {
  uuid_menu_links_features_rebuild($module);
}

/**
 * Generate a depth tree of all menu links.
 */
function uuid_menu_links_features_rebuild_ordered($menu_links, $reset = FALSE) {
  static $ordered;
  static $all_links;
  if (!isset($ordered) || $reset) {
    $ordered = array();
    features_include_defaults(array('uuid_entities', 'uuid_menu_links'));
    $unordered = features_get_default('uuid_menu_links');

    // Order all links by depth.
    if ($unordered) {
      do {
        $current = count($unordered);
        foreach ($unordered as $key => &$link) {
          $identifier = uuid_menu_links_features_identifier($link);
          if ($identifier) {
            $parent = (isset($link['uuid_parent_path']) && $link['uuid_parent_path']) ? $link['uuid_menu_name'] . ":" . $link['uuid_parent_path'] : '';
            if (empty($parent)) {
              $ordered[$identifier] = 0;
              $all_links[$identifier] = $link;
              unset($unordered[$key]);
            }
            elseif (isset($ordered[$parent])) {
              $ordered[$identifier] = $ordered[$parent] + 1;
              $all_links[$identifier] = $link;
              unset($unordered[$key]);
            }
          }
        }
      } while (count($unordered) < $current);
    }
    asort($ordered);
  }

  // Ensure any default menu items that do not exist are created.
  foreach (array_keys($ordered) as $identifier) {
    $link = $all_links[$identifier];
    uuid_menu_link_make_local($link);
    if (!empty($link) && $link['link_path']) {
      menu_link_save($link);
    }
  }
}

/**
 * Load a menu link by its menu_name:link_path identifier.
 */
function uuid_menu_link_load($identifier) {
  list($menu_name, $uri) = explode(':', $identifier, 2);
  $link_path = uuid_menu_uri_to_path_transform($uri);

  $link = db_select('menu_links')
    ->fields('menu_links', array('menu_name', 'mlid', 'plid', 'link_path', 'router_path', 'link_title', 'options', 'module', 'hidden', 'external', 'has_children', 'expanded', 'weight'))
    ->condition('menu_name', $menu_name)
    ->condition('link_path', $link_path)
    ->execute()
    ->fetchAssoc();
  if ($link) {
    $link['options'] = unserialize($link['options']);
    return $link;
  }

  return FALSE;
}

/**
 * Load all universal UUIDs of menu link.
 */
function uuid_menu_link_make_universal(&$link, $prepare_for_export = FALSE) {
  $link['uuid_link_path'] = uuid_menu_path_to_uri_transform($link['link_path']);

  $bid = explode('book-toc-', $link['menu_name']);
  if (isset($bid[1])) {
    $link['uuid_menu_name'] = 'book-toc-' . current(entity_get_uuid_by_id('node', array($bid[1])));
  }
  else {
    $link['uuid_menu_name'] = $link['menu_name'];
  }

  if (isset($link['parent_path'])) {
    $link['uuid_parent_path'] = uuid_menu_path_to_uri_transform($link['parent_path']);
  }
  elseif (isset($link['plid'])) {
    $parent = menu_link_load($link['plid']);
    $link['uuid_parent_path'] = uuid_menu_path_to_uri_transform($parent['link_path']);
  }


  for ($depth = 1; $depth <= MENU_MAX_DEPTH; $depth++) {
    if (isset($link["p$depth"])) {
      if ($link["p$depth"]) {
        $parent_link = menu_link_load($link["p$depth"]);
        $link["uuid_p$depth"] = uuid_menu_path_to_uri_transform($parent_link['link_path']);
      }
      if ($prepare_for_export) {
        unset($link["p$depth"]);
      }
    }
  }

  if ($prepare_for_export) {
    foreach (array('link_path', 'mlid', 'plid', 'nid', 'menu_name', 'href') as $property) {
      if (isset($link[$property])) {
        unset($link[$property]);
      }
    }
  }
}

/**
 * Load all universal UUIDs of menu link.
 */
function uuid_menu_link_make_local(&$link) {
  if (isset($link['uuid_link_path'])) {
    $link['link_path'] = uuid_menu_uri_to_path_transform($link['uuid_link_path']);
  }

  $uuid_bid = explode('book-toc-', $link['uuid_menu_name']);
  if (isset($uuid_bid[1])) {
    $link['menu_name'] = 'book-toc-'. current(entity_get_id_by_uuid('node', array($uuid_bid[1])));
  }
  else {
    $link['menu_name'] = $link['uuid_menu_name'];
  }

  if (isset($link['depth'])) {
    for ($depth = $link['depth']; $depth >= 1; $depth--) {
      if (isset($link["uuid_p$depth"])) {
        $parent_link = uuid_menu_link_load($link['menu_name'] . ":" . uuid_menu_uri_to_path_transform($link["uuid_p$depth"]));
        if (!empty($parent_link)) {
          $link["p$depth"] = $parent_link['mlid'];
        }
        else {
          $link["p$depth"] = 0;
        }
      }
      else {
        $link["p$depth"] = 0;
      }
    }
  }

  if (isset($link['uuid_parent_path'])) {
    $link['parent_path'] = uuid_menu_uri_to_path_transform($link['uuid_parent_path']);
    $parent_link = uuid_menu_link_load($link['menu_name'] . ":" . $link['uuid_parent_path']);
    if (!empty($parent_link)) {
      $link['plid'] = $parent_link['mlid'];
    }
  }

  $local_link = uuid_menu_link_load($link['menu_name'] . ":" . $link['uuid_link_path']);
  if (!empty($local_link)) {
    $link = array_merge($local_link, $link);
  }
}

/**
 * Convert local IDs in drupal paths to UUIDs.
 */
function uuid_menu_path_to_uri_transform(&$path) {
  $uri = '';
  $args = explode('/', $path);
  if (!empty($args) && isset($args[1])) {
    if ($args[0] == 'node' && is_numeric($args[1])) {
      $args[1] = current(entity_get_uuid_by_id('node', array($args[1])));
      $uri = implode('/', $args);
    }
    elseif ($args[0] == 'user' && is_numeric($args[1])) {
      $args[1] = current(entity_get_uuid_by_id('taxonomy_term', array($args[1])));
      $uri = implode('/', $args);
    }
    elseif (isset($args[2]) && $args[0] == 'taxonomy' && $args[1] == 'term' && is_numeric($args[2])) {
      $args[2] = current(entity_get_uuid_by_id('taxonomy_term', array($args[2])));
      $uri = implode('/', $args);
    }
  }

  // Return original path if it doesn't contain IDs.
  if (!$uri) {
    $uri = $path;
  }

  // Allow modules to do custom transformations.
  drupal_alter('uuid_menu_path_to_uri', $path, $uri);

  return $uri;
}

/**
 * Convert local IDs in drupal paths to UUIDs.
 */
function uuid_menu_uri_to_path_transform(&$uri) {
  $path = '';
  $args = explode('/', $uri);
  if (!empty($args) && isset($args[1])) {
    if ($args[0] == 'node') {
      $args[1] = current(entity_get_id_by_uuid('node', array($args[1])));
      $path = $args[1] ? implode('/', $args) : false;
    }
    elseif ($args[0] == 'user') {
      $args[1] = current(entity_get_id_by_uuid('taxonomy_term', array($args[1])));
      $path = $args[1] ? implode('/', $args) : false;
    }
    elseif (isset($args[2]) && $args[0] == 'taxonomy' && $args[1] == 'term') {
      $args[2] = current(entity_get_id_by_uuid('taxonomy_term', array($args[2])));
      $path = $args[2] ? implode('/', $args) : false;
    }
  }

  // If we can't determine source path, leave it as uri.
  if (empty($path) && $path !== false) {
    $path = $uri;
  }

  // Allow modules to do custom transformations.
  drupal_alter('uuid_menu_uri_to_path', $uri, $path);

  return $path;
}
