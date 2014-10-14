<?php
/**
 * @file
 * agridrupal_views_rdf_oai.features.menu_custom.inc
 */

/**
 * Implements hook_menu_default_menu_custom().
 */
function agridrupal_views_rdf_oai_menu_default_menu_custom() {
  $menus = array();

  // Exported menu: menu-access.
  $menus['menu-access'] = array(
    'menu_name' => 'menu-access',
    'title' => 'Private area',
    'description' => 'Menu for loggin in or registering',
    'language' => 'und',
    'i18n_mode' => 0,
  );
  // Exported menu: menu-databases.
  $menus['menu-databases'] = array(
    'menu_name' => 'menu-databases',
    'title' => 'Directories',
    'description' => 'Menu for navigation the functionalities to search the various directories (institutions, experts...)',
    'language' => 'und',
    'i18n_mode' => 0,
  );
  // Exported menu: menu-documenttype.
  $menus['menu-documenttype'] = array(
    'menu_name' => 'menu-documenttype',
    'title' => 'Documents',
    'description' => 'Menu for navigating the functionalities to browse / search documents',
    'language' => 'und',
    'i18n_mode' => 0,
  );
  // Exported menu: menu-reference.
  $menus['menu-reference'] = array(
    'menu_name' => 'menu-reference',
    'title' => 'Reference',
    'description' => 'Menu for links to the reference lists and controlled vocabularies',
    'language' => 'und',
    'i18n_mode' => 0,
  );
  // Translatables
  // Included for use with string extractors like potx.
  t('Directories');
  t('Documents');
  t('Menu for links to the reference lists and controlled vocabularies');
  t('Menu for loggin in or registering');
  t('Menu for navigating the functionalities to browse / search documents');
  t('Menu for navigation the functionalities to search the various directories (institutions, experts...)');
  t('Private area');
  t('Reference');


  return $menus;
}
