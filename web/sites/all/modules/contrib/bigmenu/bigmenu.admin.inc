<?php

/**
 * @file
 * Administrative page callbacks for bigmenu module.
 *
 * Large amounts copied from menu.admin.inc
 *
 * @author Dan (dman) Morrison dan@coders.co.nz
 * @author Adam (acbramley) Bramley adam@catalyst.net.nz
 * @version 2012
 */

/**
 * We re-use as much of the core menu rendering tools as possible.
 */
module_load_include('inc', 'menu', 'menu.admin');

function bigmenu_overview_form($form, &$form_state, $menu, $depth = 1) {
  global $menu_admin;

  $p_depth = 'p' . (string)((int)$depth + 3);
  $sql = "
    SELECT m.load_functions, m.to_arg_functions, m.access_callback, m.access_arguments, m.page_callback, m.page_arguments, m.title, m.title_callback, m.title_arguments, m.type, m.description, ml.*
    FROM {menu_links} ml LEFT JOIN {menu_router} m ON m.path = ml.router_path
    WHERE ml.menu_name = :menu_name
    AND $p_depth = 0
    ORDER BY p1 ASC, p2 ASC, p3 ASC, p4 ASC, p5 ASC, p6 ASC, p7 ASC, p8 ASC, p9 ASC";
  $result = db_query($sql, array(':menu_name' => $menu['menu_name']), array('fetch' => PDO::FETCH_ASSOC));
  $links = array();
  foreach ($result as $item) {
    $links[] = $item;
  }
  $tree = menu_tree_data($links);
  $node_links = array();
  menu_tree_collect_node_links($tree, $node_links);
  // We indicate that a menu administrator is running the menu access check.
  $menu_admin = TRUE;
  menu_tree_check_access($tree, $node_links);
  $menu_admin = FALSE;

  // If the user doesn't have the permission to use big menu, use core's form
  $new_form = user_access('use bigmenu') ? _bigmenu_overview_tree_form($tree) : _menu_overview_tree_form($tree);
  $cached_mlids = bigmenu_get_cached_mlids($form_state);
  $new_form = array_merge($new_form, $cached_mlids);

  $new_form['#menu'] =  $menu;
  if (element_children($new_form)) {
    $new_form['submit'] = array(
      '#type' => 'submit',
      '#value' => t('Save configuration'),
    );
  }
  else {
    $new_form['#empty_text'] = array('#markup' => t('There are no menu items yet.'));
  }

  return $new_form;
}

/**
 * Get the menu items that have already been expanded to merge in with the form during it's construction
 * @param $form_state
 * @return array - The previously cached mlids, including those from expanded parents
 */
function bigmenu_get_cached_mlids($form_state) {
  $cached_mlids = array();
  if (!empty($form_state) && !empty($form_state['rebuild_info']) && !empty($form_state['rebuild_info']['copy'])) {
    $form_build_id = $form_state['rebuild_info']['copy']['#build_id'];
    $cached_form = form_get_cache($form_build_id, $form_state);

    if (!empty($cached_form)) {
      // We have previously expanded one of the menu items so extract the items that were in the cached form
      // so that we can add them to the form that is being built now.
      foreach ($cached_form as $key => $value) {
        $matches = array();
        if (preg_match('/mlid:([0-9]*)/', $key, $matches)) {
          $cached_mlids[$key] = $value;
        }
      }
    }
  }

  return $cached_mlids;
}

/**
 * Form for editing the immediate children of the given menu item id
 *
 * Invoked by menu callback or by json request
 *
 * @return FAPI form.
 */
function bigmenu_slice_form($form, &$form_state, $menu, $menu_link, $depth = 2) {
  $menu_slice = menu_get_menus();

  // DB lookup the children of this item
  global $menu_admin;
  $p_depth = 'p' . $menu_link['depth'];
  $sql = "
    SELECT m.load_functions, m.to_arg_functions, m.access_callback, m.access_arguments, m.page_callback, m.page_arguments, m.title, m.title_callback, m.title_arguments, m.type, m.description, ml.*
    FROM {menu_links} ml LEFT JOIN {menu_router} m ON m.path = ml.router_path
    WHERE ml.menu_name = :menu_name
    AND " . check_plain($p_depth) . " = :mlid
    ORDER BY p1 ASC, p2 ASC, p3 ASC, p4 ASC, p5 ASC, p6 ASC, p7 ASC, p8 ASC, p9 ASC";
  $result = db_query($sql, array(':menu_name' => $menu['menu_name'], ':mlid' => $menu_link['mlid']), array('fetch' => PDO::FETCH_ASSOC));
  $links = array();
  foreach ($result as $item) {
    $links[] = $item;
  }
  $tree = menu_tree_data($links);

  $node_links = array();
  menu_tree_collect_node_links($tree, $node_links);
  // We indicate that a menu administrator is running the menu access check.
  $menu_admin = TRUE;
  menu_tree_check_access($tree, $node_links);
  $menu_admin = FALSE;

  // When doing a slice, don't show the actual parent link item, just the children
  foreach ($tree as $ix => $data) {
    $tree[$ix]['link'] = array();
  }
  // (there was only one, but I didn't know its name)

  $form = _bigmenu_overview_tree_form($tree, $depth);

  $form['#theme'] = 'bigmenu_overview_form';
  $form['#menu'] =  $menu;
  if (element_children($form)) {
    $form['actions'] = array('#type' => 'actions');
    $form['actions']['submit'] = array(
      '#type' => 'submit',
      '#value' => t('Save configuration'),
    );
  }
  else {
    $form['#empty_text'] = array('#markup' => t('There are no menu items yet.'));
  }
  return $form;
}

/**
 * Return a submenu slice form in json format
 * Menu callback, invoked by AJAX
 *
 * Drupals form cache would not allow me to insert arbitrary fields into a form
 * without letting the server know about it.
 */
function bigmenu_slice_form_js($menu, $menu_link, $form_id, $form_build_id) {
  $depth = variable_get('bigmenu_depth', 1);

  $form_state = form_state_defaults();
  $old_form = drupal_get_form('bigmenu_slice_form', $menu, $menu_link, $depth);
  $output = drupal_render($old_form);

  // This returns data to the browser immediately.
  drupal_json_output(array('status' => TRUE, 'data' => $output));

  // After that we will do the slower job of updating the form cache.
  // Reload the bigmenu_overview_form, with the same args (the menu name) as before,
  $args = array($menu);
  $form_state['build_info']['args'] = $args;
  $form_state['values'] = array();
  $form_state['rebuild_info']['copy']['#build_id'] = $form_build_id;
  $form = drupal_rebuild_form($form_id, $form_state, array('#build_id' => $form_build_id));

  exit;
}

/**
 * Theme the menu overview form into a table.
 *
 * A stub to core. No need to change anything. It just adds an extra js file and
 * passes it through.
 *
 * @ingroup themeable
 */
function theme_bigmenu_overview_form($form) {
  drupal_add_js(drupal_get_path('module', 'bigmenu') . '/bigmenu.js');
  drupal_add_css(drupal_get_path('module', 'bigmenu') . '/bigmenu.css');
  return theme('menu_overview_form', $form);
}

/**
 * A stub to core
 */
function bigmenu_overview_form_submit($form, &$form_state) {
  return menu_overview_form_submit($form, $form_state);
}

/**
 * Admin settings form.
 */
function bigmenu_settings($form, &$form_state) {
  $form['bigmenu_settings'] = array(
    '#type' => 'fieldset',
    '#title' => t('Settings'),
  );
  $form['bigmenu_settings']['bigmenu_depth'] = array(
      '#type' => 'select',
      '#title' => t('Big Menu Depth'),
      '#description' => t('This setting controls how many levels are generated at a time when showing children of a menu item.'),
      '#required' => TRUE,
      '#default_value' => variable_get('bigmenu_depth', 1),
      '#options' => array(1 => '1', 2 => '2', 3 => '3'),
  );
  return system_settings_form($form);
}


/**
 * Recursive helper function for menu_overview_form().
 *
 * @see _menu_overview_tree_form()
 *
 * Copied from core, and modified to limit recursion
 */
function _bigmenu_overview_tree_form($tree, $depth = 0) {
  $form = &drupal_static(__FUNCTION__, array('#tree' => TRUE));
  foreach ($tree as $data) {
    $title = '';
    $item = $data['link'];
    // Don't show callbacks; these have $item['hidden'] < 0.
    if ($item && $item['hidden'] >= 0) {
      $mlid = 'mlid:' . $item['mlid'];
      $form[$mlid]['#item'] = $item;
      $form[$mlid]['#attributes'] = $item['hidden'] ? array('class' => array('menu-disabled')) : array('class' => array('menu-enabled'));
      $form[$mlid]['title']['#markup'] = l($item['title'], $item['href'], $item['localized_options']) . ($item['hidden'] ? ' (' . t('disabled') . ')' : '');
      $form[$mlid]['hidden'] = array(
        '#type' => 'checkbox',
        '#title' => t('Enable @title menu link', array('@title' => $item['title'])),
        '#title_display' => 'invisible',
        '#default_value' => !$item['hidden'],
      );
      $form[$mlid]['weight'] = array(
        '#type' => 'weight',
        '#delta' => 50,
        '#default_value' => $item['weight'],
        '#title_display' => 'invisible',
        '#title' => t('Weight for @title', array('@title' => $item['title'])),
      );
      $form[$mlid]['mlid'] = array(
        '#type' => 'hidden',
        '#value' => $item['mlid'],
      );
      $form[$mlid]['plid'] = array(
        '#type' => 'hidden',
        '#default_value' => $item['plid'],
      );
      // Build a list of operations.
      $operations = array();
      $operations['edit'] = array('#type' => 'link', '#title' => t('edit'), '#href' => 'admin/structure/menu/item/' . $item['mlid'] . '/edit');
      // Only items created by the menu module can be deleted.
      if ($item['module'] == 'menu' || $item['updated'] == 1) {
        $operations['delete'] = array('#type' => 'link', '#title' => t('delete'), '#href' => 'admin/structure/menu/item/' . $item['mlid'] . '/delete');
      }
      // Set the reset column.
      elseif ($item['module'] == 'system' && $item['customized']) {
        $operations['reset'] = array('#type' => 'link', '#title' => t('reset'), '#href' => 'admin/structure/menu/item/' . $item['mlid'] . '/reset');
      }
      $form[$mlid]['operations'] = $operations;
    }

    if ($data['below']) {
      if ($depth > 0) {
        _bigmenu_overview_tree_form($data['below'], $depth - 1);
      }
      else {
        // 'below' contains both immediate children and something else
        $strings = array(
          '!show_children' => t('Show children'),
          '%count' => count($data['below']),
          '!tooltip' => t('Click to expand and show child items'),
        );
        $text = strtr('<span class="bigmenu-toggle">+</span> <span class="count" title="!tooltip"><span class="hide-show">!show_children</span> (%count)</span>', $strings);
        $indicator = l($text, "admin/structure/bigmenu-customize/{$item['menu_name']}/subform/{$item['mlid']}", array('html' => TRUE, 'attributes' => array('class' => array('bigmenu-childindictor'))));
        $form[$mlid]['title']['#markup'] .= '<br/>' . $indicator;
        $form[$mlid]['#attributes']['class'][] = 'bigmenu-collapsed';

      }
    }
  }
  return $form;
}
