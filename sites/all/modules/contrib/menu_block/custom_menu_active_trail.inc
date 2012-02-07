<?php
/**
 * @file
 * Add custom menus to the active trail; required to fix Drupal 7.0 core.
 *
 * This file WILL be removed after this core bug is fixed: #942782. You have
 * been warned!
 */

/**
 * Implements hook_menu_insert().
 */
function menu_block_menu_insert($menu) {
  _menu_block_add($menu);
}

/**
 * Implements hook_menu_update().
 */
function menu_block_menu_update($menu) {
  _menu_block_add($menu);
}

/**
 * Implements hook_menu_delete().
 */
function menu_block_menu_delete($menu) {
  $active_menus = variable_get('menu_default_active_menus', array_keys(menu_list_system_menus()));
  if (in_array($menu['menu_name'], $active_menus)) {
    $key = array_search($menu['menu_name'], $active_menus);
    unset($active_menus[$key]);
    variable_set('menu_default_active_menus', $active_menus);
  }
}

/**
 * Add a custom menu to the active trail.
 *
 * @param array $menu
 *   A menu item to add to the active trail.
 */
function _menu_block_add($menu) {
  // Make sure the menu is present in the active menus variable so that its
  // items may appear in the menu active trail.
  // @see menu_set_active_menu_names()
  $active_menus = variable_get('menu_default_active_menus', array_keys(menu_list_system_menus()));
  if (!in_array($menu['menu_name'], $active_menus)) {
    $active_menus[] = $menu['menu_name'];
    variable_set('menu_default_active_menus', $active_menus);
  }
}

/**
 * Muck with the static cache of menu_link_get_preferred.
 *
 * In ensure all menus get an active trail, pre-set the static cache of
 * menu_link_get_preferred() for the specific menu we want to render.
 */
function __menu_block_set_menu_trail($menu_name = FALSE) {
  // If there's no menu item, no active trail hack is needed.
  if (($item = menu_get_item()) && $item == FALSE) {
    return;
  }

  $preferred_links = &drupal_static('menu_link_get_preferred');
  $originals = &drupal_static(__FUNCTION__, array());
  $path = $_GET['q'];

  if ($menu_name) {
    // Alter the preferred link so it always checks the requested menu.
    $originals['preferred_link'] = isset($preferred_links[$path]) ? $preferred_links[$path] : NULL;
    $preferred_links[$path] = _menu_link_get_preferred($path, $menu_name);

    // Ensure the active menu always includes the requested menu.
    $originals['active_menus'] = variable_get('menu_default_active_menus', NULL);
    $GLOBALS['conf']['menu_default_active_menus'] = array($menu_name);
  }
  else {
    // Reset the original preferred_link static variables.
    if (is_null($originals['preferred_link'])) {
      unset($preferred_links[$path]);
    }
    else {
      $preferred_links[$path] = $originals['preferred_link'];
    }
    // Reset the original menu_default_active_menus variable.
    $GLOBALS['conf']['menu_default_active_menus'] = $originals['active_menus'];
  }
}

/**
 * Lookup the preferred menu link for a given system path.
 *
 * @param $path
 *   The path, for example 'node/5'. The function will find the corresponding
 *   menu link ('node/5' if it exists, or fallback to 'node/%').
 * @param $menu_name
 *   The name of a menu used to restrict the search for a prefered menu link.
 *   If not specified, all the menus returned by menu_get_active_menu_names()
 *   will be used.
 *
 * @return
 *   A fully translated menu link, or NULL if not matching menu link was
 *   found. The most specific menu link ('node/5' preferred over 'node/%') in
 *   the most preferred menu (as defined by menu_get_active_menu_names()) is
 *   returned.
 */
function _menu_link_get_preferred($path = NULL, $menu_name = '') {
  $preferred_links = &drupal_static(__FUNCTION__);

  if (!isset($path)) {
    $path = $_GET['q'];
  }

  if (!isset($preferred_links[$path][$menu_name])) {
    $preferred_links[$path][$menu_name] = FALSE;

    // Look for the correct menu link by building a list of candidate paths,
    // which are ordered by priority (translated hrefs are preferred over
    // untranslated paths). Afterwards, the most relevant path is picked from
    // the menus, ordered by menu preference.
    $item = menu_get_item($path);
    $path_candidates = array();
    // 1. The current item href.
    $path_candidates[$item['href']] = $item['href'];
    // 2. The tab root href of the current item (if any).
    if ($item['tab_parent'] && ($tab_root = menu_get_item($item['tab_root_href']))) {
      $path_candidates[$tab_root['href']] = $tab_root['href'];
    }
    // 3. The current item path (with wildcards).
    $path_candidates[$item['path']] = $item['path'];
    // 4. The tab root path of the current item (if any).
    if (!empty($tab_root)) {
      $path_candidates[$tab_root['path']] = $tab_root['path'];
    }

    // Retrieve a list of menu names, ordered by preference.
    if ($menu_name !== '') {
      $menu_names = array($menu_name);
    }
    else {
      $menu_names = menu_get_active_menu_names();
    }

    $query = db_select('menu_links', 'ml', array('fetch' => PDO::FETCH_ASSOC));
    $query->leftJoin('menu_router', 'm', 'm.path = ml.router_path');
    $query->fields('ml');
    // Weight must be taken from {menu_links}, not {menu_router}.
    $query->addField('ml', 'weight', 'link_weight');
    $query->fields('m');
    $query->condition('ml.link_path', $path_candidates, 'IN');

    // Sort candidates by link path and menu name.
    $candidates = array();
    foreach ($query->execute() as $candidate) {
      $candidate['weight'] = $candidate['link_weight'];
      $candidates[$candidate['link_path']][$candidate['menu_name']] = $candidate;
    }

    // Pick the most specific link, in the most preferred menu.
    $preferred_link = FALSE;
    foreach ($path_candidates as $link_path) {
      if (isset($candidates[$link_path])) {
        foreach ($menu_names as $menu) {
          if (!$preferred_links[$path][$menu] && isset($candidates[$link_path][$menu])) {
            $candidate_item = $candidates[$link_path][$menu];
            $map = explode('/', $path);
            _menu_translate($candidate_item, $map);
            if ($candidate_item['access']) {
              $preferred_links[$path][$menu] = $candidate_item;
              if (!$preferred_link) {
                $preferred_link = $candidate_item;
              }
            }
          }
        }
      }
    }
    if ($menu_name === '' && $preferred_link) {
      $preferred_links[$path][$menu_name] = $preferred_link;
    }
  }

  return $preferred_links[$path][$menu_name];
}
