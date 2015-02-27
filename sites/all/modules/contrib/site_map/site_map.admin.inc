<?php
/**
 * @file
 * Original author: Nic Ivy
 * Now maintained by by Fredrik Jonsson fredrik at combonet dot se
 */

/**
 * Menu callback; presents the sitemap settings page.
 */
function site_map_admin_settings_form() {

  // Field to set site map page title.
  $form['site_map_page_title'] = array(
    '#type' => 'textfield',
    '#title' => t('Page title'),
    '#default_value' => variable_get('site_map_page_title', t('Site map')),
    '#description' => t('Page title that will be used on the <a href="@link">site map page</a>.', array('@link' => url('sitemap'))),
  );
  // Field to set site map message.
  $site_map_message = variable_get('site_map_message', array('value' => '', 'format' => NULL));
  $form['site_map_message'] = array(
    '#type' => 'text_format',
    '#format' => isset($site_map_message['format']) ? $site_map_message['format'] : NULL,
    '#title' => t('Site map message'),
    '#default_value' => $site_map_message['value'],
    '#description' => t('Define a message to be displayed above the site map.'),
  );

  $form['site_map_content'] = array(
    '#type' => 'fieldset',
    '#title' => t('Site map content'),
  );
  $form['site_map_content']['site_map_show_front'] = array(
    '#type' => 'checkbox',
    '#title' => t('Show front page'),
    '#default_value' => variable_get('site_map_show_front', 1),
    '#description' => t('When enabled, this option will include the front page in the site map.'),
  );
  $form['site_map_content']['site_map_show_titles'] = array(
    '#type' => 'checkbox',
    '#title' => t('Show titles'),
    '#default_value' => variable_get('site_map_show_titles', 1),
    '#description' => t('When enabled, this option will show titles. Disable to not show section titles.'),
  );
  if (module_exists('blog')) {
    $form['site_map_content']['site_map_show_blogs'] = array(
      '#type' => 'checkbox',
      '#title' => t('Show active blog authors'),
      '#default_value' => variable_get('site_map_show_blogs', 1),
      '#description' => t('When enabled, this option will show the 10 most active blog authors.'),
    );
  }
  if (module_exists('book')) {
    $book_options = array();
    foreach (book_get_books() as $book) {
      $book_options[$book['mlid']] = $book['title'];
    }
    $form['site_map_content']['site_map_show_books'] = array(
      '#type' => 'checkboxes',
      '#title' => t('Books to include in the site map'),
      '#default_value' => variable_get('site_map_show_books', array()),
      '#options' => $book_options,
      '#multiple' => TRUE,
    );
    $form['site_map_content']['site_map_books_expanded'] = array(
      '#type' => 'checkbox',
      '#title' => t('Show books expanded'),
      '#default_value' => variable_get('site_map_books_expanded', 1),
      '#description' => t('When enabled, this option will show all children pages for each book.'),
    );
  }

  $menu_options = array();
  $menu_options = menu_get_menus();

  $form['site_map_content']['site_map_show_menus'] = array(
    '#type' => 'checkboxes',
    '#title' => t('Menus to include in the site map'),
    '#default_value' => variable_get('site_map_show_menus', array()),
    '#options' => $menu_options,
    '#multiple' => TRUE,
  );
  // Thanks for fix by zhuber at https://drupal.org/node/1331104#comment-5200266
  $form['site_map_content']['site_map_show_menus_hidden'] = array(
    '#type' => 'checkbox',
    '#title' => t('Show disabled menu items'),
    '#default_value' => variable_get('site_map_show_menus_hidden', 0),
    '#description' => t('When enabled, hidden menu links will also be shown.'),
  );
  if (module_exists('faq')) {
    $form['site_map_content']['site_map_show_faq'] = array(
      '#type' => 'checkbox',
      '#title' => t('Show FAQ content'),
      '#default_value' => variable_get('site_map_show_faq', 0),
      '#description' => t('When enabled, this option will include the content from the FAQ module in the site map.'),
    );
  }
  $vocab_options = array();
  if (module_exists('taxonomy')) {
    foreach (taxonomy_get_vocabularies() as $vocabulary) {
      $vocab_options[$vocabulary->machine_name] = $vocabulary->name;
    }
  }
  $form['site_map_content']['site_map_show_vocabularies'] = array(
    '#type' => 'checkboxes',
    '#title' => t('Categories to include in the site map'),
    '#default_value' => variable_get('site_map_show_vocabularies', array()),
    '#options' => $vocab_options,
    '#multiple' => TRUE,
  );

  $form['site_map_taxonomy_options'] = array(
    '#type' => 'fieldset',
    '#title' => t('Categories settings'),
  );
  $form['site_map_taxonomy_options']['site_map_show_description'] = array(
    '#type' => 'checkbox',
    '#title' => t('Show category description'),
    '#default_value' => variable_get('site_map_show_description', 1),
    '#description' => t('When enabled, this option will show the category description.'),
  );
  $form['site_map_taxonomy_options']['site_map_show_count'] = array(
    '#type' => 'checkbox',
    '#title' => t('Show node counts by categories'),
    '#default_value' => variable_get('site_map_show_count', 1),
    '#description' => t('When enabled, this option will show the number of nodes in each taxonomy term.'),
  );
  $form['site_map_taxonomy_options']['site_map_categories_depth'] = array(
    '#type' => 'textfield',
    '#title' => t('Categories depth'),
    '#default_value' => variable_get('site_map_categories_depth', 'all'),
    '#size' => 3,
    '#maxlength' => 10,
    '#description' => t('Specify how many levels of subcategories should be included. Enter a positive integer, or "all" to include all levels.'),
  );
  $form['site_map_taxonomy_options']['site_map_term_threshold'] = array(
    '#type' => 'textfield',
    '#title' => t('Category count threshold'),
    '#default_value' => variable_get('site_map_term_threshold', 0),
    '#size' => 3,
    '#description' => t('Only show categories whose node counts are greater than this threshold. Set to -1 to disable.'),
  );
  $form['site_map_taxonomy_options']['site_map_forum_threshold'] = array(
    '#type' => 'textfield',
    '#title' => t('Forum count threshold'),
    '#default_value' => variable_get('site_map_forum_threshold', -1),
    '#size' => 3,
    '#description' => t('Only show forums whose node counts are greater than this threshold. Set to -1 to disable.'),
  );

  $form['site_map_rss_options'] = array(
    '#type' => 'fieldset',
    '#title' => t('RSS settings'),
  );
  $form['site_map_rss_options']['site_map_rss_front'] = array(
    '#type' => 'textfield',
    '#title' => t('RSS feed for front page'),
    '#default_value' => variable_get('site_map_rss_front', 'rss.xml'),
    '#description' => t('The RSS feed for the front page, default is rss.xml.'),
  );
  $form['site_map_rss_options']['site_map_show_rss_links'] = array(
    '#type' => 'select',
    '#title' => t('Include RSS links'),
    '#default_value' => variable_get('site_map_show_rss_links', 1),
    '#options' => array(
      0 => t('None'),
      1 => t('Include on the right side'),
      2 => t('Include on the left side'),
    ),
    '#description' => t('When enabled, this option will show links to the RSS feeds for each category and blog.'),
  );
  $form['site_map_rss_options']['site_map_rss_depth'] = array(
    '#type' => 'textfield',
    '#title' => t('RSS feed depth'),
    '#default_value' => variable_get('site_map_rss_depth', 'all'),
    '#size' => 3,
    '#maxlength' => 10,
    '#description' => t('Specify how many subcategories should be included in the RSS feed. Enter "all" to include all subcategories or "0" to include no subcategories.'),
  );

  $form['site_map_css_options'] = array(
    '#type' => 'fieldset',
    '#title' => t('CSS settings'),
  );
  $form['site_map_css_options']['site_map_css'] = array(
    '#type' => 'checkbox',
    '#title' => t('Do not include site map CSS file'),
    '#default_value' => variable_get('site_map_css', 0),
    '#description' => t("If you don't want to load the included CSS file you can check this box."),
  );

  // Make use of the Checkall module if it's installed.
  if (module_exists('checkall')) {
    $form['site_map_content']['site_map_show_books']['#checkall'] = TRUE;
    $form['site_map_content']['site_map_show_menus']['#checkall'] = TRUE;
    $form['site_map_content']['site_map_show_vocabularies']['#checkall'] = TRUE;
  }

  return system_settings_form($form);
}
