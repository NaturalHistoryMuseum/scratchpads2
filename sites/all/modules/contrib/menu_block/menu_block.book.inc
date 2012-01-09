<?php
/**
 * @file
 * Provides book integration.
 */

/**
 * Implements hook_menu_block_get_menus().
 */
function book_menu_block_get_menus() {
  $menus = array();
  foreach (book_get_books() AS $book) {
    $menus[$book['menu_name']] = $book['title'];
  }
  return $menus;
}

/**
 * Implements hook_menu_block_get_sort_menus().
 */
function book_menu_block_get_sort_menus() {
  return array(
    '/^book\-toc\-.+/' => t('Book navigation'),
  );
}
