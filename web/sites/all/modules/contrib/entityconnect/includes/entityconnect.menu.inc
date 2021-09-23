<?php
/**
 * @file
 * Handles all entityconnect menu item page callbacks.
 */

/**
 * Page callback: Redirects to the form page.
 *
 * We redirect to the form page with the build_cache_id as a get param.
 */
function entityconnect_return($cache_id) {
  $cache = cache_get($cache_id);
  $css_id = "edit-" . str_replace('_', '-', $cache->data['field']);
  drupal_goto($cache->data['dest'], array(
      'query' => array(
        'build_cache_id' => $cache_id,
        'return' => TRUE),
          'fragment' => $css_id));
}

/**
 * Page callback: Load cached form info.
 *
 * This page will load the cached form info, and display links to each of the
 * entityreference types.
 * If there is only one it will redirect to that page.
 *
 * This is mostly a copy and hack up of the node add page.
 *
 * This page is directed to from the entityconnect_add_button_submit.
 */
function entityconnect_add($cache_id) {
  $content = array();
  $cache = cache_get($cache_id);
  $field = field_info_field($cache->data['field']);

  $entity_type = $cache->data['target_entity_type'];
  $acceptable_types = $cache->data['acceptable_types'];

  switch ($entity_type) {
    case 'node':
      if (count($acceptable_types) > 0) {
        foreach (system_admin_menu_block(menu_get_item("node/add")) as $key => $item) {
          $type = str_replace("-", '_', str_replace("node/add/", "", $item['path']));
          if (isset($acceptable_types[$type]) && $acceptable_types[$type]) {
            $item['href'] = $item['href'] . "/$cache_id";
            $content[$key] = $item;
          }
        }

        // If we have only one content-type defined,
        // go directly to the node form.
        if (sizeof($content) == 1) {
          $item = array_pop($content);
          drupal_goto($item['href']);
        }
      }
      else {
        foreach (system_admin_menu_block(menu_get_item("node/add")) as $key => $item) {
          $type = str_replace("-", '_', str_replace("node/add/", "", $item['path']));
          $item['href'] = $item['href'] . "/$cache_id";
          $content[$key] = $item;
        }
      }
      $output = theme('node_add_list', array('content' => $content));
      $output .= l(t('Cancel'), "admin/entityconnect/return/$cache_id");
      break;

    case 'user':
      $item['href'] = "admin/people/create/$cache_id";
      drupal_goto($item['href']);
      break;

    case 'taxonomy_term':
      $voc_list = array();

      if (count($acceptable_types) > 0) {
        foreach (taxonomy_get_vocabularies() as $key => $item) {
          $type = $item->machine_name;
          if (isset($acceptable_types[$type]) && $acceptable_types[$type]) {
            $item->href = "admin/structure/taxonomy/$type/add/$cache_id";
            $content[$key] = $item;
            $voc_list[] = l($item->name, $item->href);
          }
        }

        // If we have only one vocabulary defined,
        // go directly to the term add form.
        if (sizeof($content) == 1) {
          $item = array_pop($content);
          drupal_goto($item->href);
        }
      }
      else {
        foreach (taxonomy_get_vocabularies() as $key => $item) {
          !isset($item->href) ? $item->href = NULL : $item->href;
          $type = $item->machine_name;
          $item->href = "admin/structure/taxonomy/$type/add/$cache_id";
          $content[$key] = $item;
          $voc_list[] = l($item->name, $item->href);
        }
      }
      $output = theme('entityconnect_taxonomy_term_add_list', array('items' => $content));
      $output .= l(t('Cancel'), "admin/entityconnect/return/$cache_id");
      break;

    case 'taxonomy_vocabulary':
      $item['href'] = "admin/structure/taxonomy/add/$cache_id";
      drupal_goto($item['href']);
      break;

    default:
      break;
  }
  return $output;
}

/**
 * Page callback: Redirect to edit form.
 *
 * We use this to redirect to a edit form but pass the build_cache_id.
 */
function entityconnect_edit($cache_id) {
  $cache = cache_get($cache_id);
  $entity_type = $cache->data['target_entity_type'];
  $target_id = $cache->data['target_id'];
  $output = '';

  switch ($entity_type) {
    case 'node':
      if (is_array($target_id)) {
        $info = entity_load($entity_type, $target_id);
        foreach ($info as $key => $value) {
          $content[$key] = array(
            'label' => $value->title,
            'href' => 'node/' . $value->nid . '/edit',
          );
        }
        $output = theme('entityconnect_entity_add_list', array(
          'items' => $content,
          'cache id' => $cache_id)
        );
        $output .= l(t('Cancel'), "admin/entityconnect/return/$cache_id");
      }
      else {
        drupal_goto("node/" . $cache->data['target_id'] . "/edit", array(
            'query' => array("build_cache_id" => $cache_id, "child" => TRUE)));
      }
      break;

    case 'user':
      if (is_array($target_id)) {
        $info = entity_load($entity_type, $target_id);
        foreach ($info as $key => $value) {
          $content[$key] = array(
            'label' => $value->name,
            'href' => 'user/' . $value->uid . '/edit',
          );
        }
        $output = theme('entityconnect_entity_add_list', array(
          'items' => $content,
          'cache id' => $cache_id)
        );
        $output .= l(t('Cancel'), "admin/entityconnect/return/$cache_id");
      }
      else {
        drupal_goto("user/" . $cache->data['target_id'] . "/edit", array(
            'query' => array("build_cache_id" => $cache_id, "child" => TRUE)));
      }
      break;

    case 'taxonomy_term':
      if (is_array($target_id)) {
        $info = entity_load($entity_type, $target_id);
        foreach ($info as $key => $value) {
          $content[$key] = array(
            'label' => $value->name,
            'href' => 'taxonomy/term/' . $value->tid . '/edit',
          );
        }
        $output = theme('entityconnect_entity_add_list', array(
          'items' => $content,
          'cache id' => $cache_id)
        );
        $output .= l(t('Cancel'), "admin/entityconnect/return/$cache_id");
      }
      else {
        drupal_goto("taxonomy/term/" . $cache->data['target_id'] . "/edit", array(
            'query' => array("build_cache_id" => $cache_id, "child" => TRUE)));
      }
      break;

    case 'taxonomy_vocabulary':
      if (is_array($target_id)) {
        $info = entity_load($entity_type, $target_id);
        foreach ($info as $key => $value) {
          $content[$key] = array(
            'label' => $value->name,
            'href' => 'admin/structure/taxonomy/' . $value->name . '/edit',
          );
        }
        $output = theme('entityconnect_entity_add_list', array(
          'items' => $content,
          'cache id' => $cache_id)
        );
        $output .= l(t('Cancel'), "admin/entityconnect/return/$cache_id");
      }
      else {
        $info = entity_load_single($entity_type, $target_id);
        drupal_goto("admin/structure/taxonomy/" . $info->name . "/edit", array(
            'query' => array("build_cache_id" => $cache_id, "child" => TRUE)));
      }
      break;

    default:
      break;
  }
  return $output;
}
