<?php
/**
 * @file
 * site_map.theme.inc
 *
 * Site map theme functions.
 */


/**
 * Returns HTML for a site map feed icon legend.
 */
function theme_site_map_rss_legend() {
  $output .= '<p><strong>' . t('Legend:') . '</strong><br />';
  $output .= '<span class="rss">' . theme('site_map_feed_icon', array('type' => 'node')) . '</span> ' . t('Link to a content RSS feed');
  $output .= '<br /><span class="rss">' . theme('site_map_feed_icon', array('type' => 'comment')) . '</span> ' . t('Link to a comment RSS feed');
  $output .= '</p>';

  return $output;
}


/**
 * Preprocesses the variables for theme_site_map_box().
 */
function template_preprocess_site_map_box(&$variables) {
  $variables['attributes']['class'][] = 'site-map-box';
}


/**
 * Returns HTML for a themed site map box.
 *
 * @param array $variables
 *   An associative array containing:
 *   - title: The subject of the box.
 *   - content: The content of the box.
 *   - attributes:  Optional attributes for the box.
 *
 * @return string
 *   Returns sitemap display in DIV.
 */
function theme_site_map_box($variables) {
  $title = $variables['title'];
  $content = $variables['content'];
  $attributes = $variables['attributes'];
  $options = $variables['options'];

  $output = '';
  if (!empty($title) || !empty($content)) {
    $output .= '<div' . drupal_attributes($attributes) . '>';
    if (!empty($title) && isset($options['show_titles'])) {
      $output .= '<h2 class="title">' . $title . '</h2>';
    }
    if (!empty($content)) {
      $output .= '<div class="content">' . $content . '</div>';
    }
    $output .= '</div>';
  }

  return $output;
}


/**
 * Returns HTML for a feed icon with link.
 *
 * @param array $variables
 *   An associative array containing:
 *   - url: The url of the feed.
 *   - type: The type of feed icon.
 *
 * @return string
 *   Constructs and returns html with feed image icon.
 */
function theme_site_map_feed_icon($variables) {
  $output = '';

  switch ($variables['type']) {
    case 'node':
      $output = theme('image', array(
        'path' => drupal_get_path('module', 'site_map') . '/feed-small.png',
        'title' => t('Syndicate content'),
        'alt' => t('Syndicate content'),
        )
      );
      break;

    case 'comment':
      $output = theme('image', array(
        'path' => drupal_get_path('module', 'site_map') . '/feed-small-comment.png',
        'title' => t('Syndicate comments'),
        'alt' => t('Syndicate comments'),
        )
      );
      break;
  }

  if (!empty($variables['url'])) {
    $output = l($output, $variables['url'], array('attributes' => array('class' => 'feed-link'), 'html' => TRUE));
  }

  return $output;
}


/**
 * Preprocesses the rendered tree for theme_site_map_menu_tree().
 *
 * This is a clone of the core template_preprocess_menu_tree() function
 * with the exception of the site_map specific class name used in the
 * UL that also allow themers to override the function only
 * for the site map page.
 */
function template_preprocess_site_map_menu_tree(&$variables) {
  $variables['tree'] = $variables['tree']['#children'];
}


/**
 * Returns HTML for a wrapper for a menu sub-tree.
 *
 * This is a clone of the core theme_menu_tree() function with the exception of
 * the site_map specific class name used in the UL that also allow themers to
 * override the function only for the site map page.
 *
 * @param array $variables
 *   An associative array containing:
 *   - tree: An HTML string containing the tree's items.
 *
 * @return string
 *   Returns the html string with the <ul> for the menu tree.
 *
 * @see template_preprocess_menu_tree()
 * @ingroup themeable
 */
function theme_site_map_menu_tree($variables) {
  return '<ul class="site-map-menu">' . $variables['tree'] . '</ul>';
}


/**
 * Returns HTML for a menu link and submenu.
 *
 * This is a one by one clone of the core theme_menu_link() function that allows
 * custom theming of the site map page items.
 *
 * @param array $variables
 *   An associative array containing:
 *   - element: Structured array data for a menu link.
 *
 * @return string
 *   Returns html string for menu link.
 *
 * @ingroup themeable
 */
function theme_site_map_menu_link(array $variables) {
  $element = $variables['element'];
  $sub_menu = '';

  if ($element['#below']) {
    $sub_menu = drupal_render($element['#below']);
  }
  $output = l($element['#title'], $element['#href'], $element['#localized_options']);
  return '<li' . drupal_attributes($element['#attributes']) . '>' . $output . $sub_menu . "</li>\n";
}


/**
 * Preprocesses the variables for site-map.tpl.php.
 *
 * @see site-map.tpl.php
 */
function template_preprocess_site_map(&$variables) {
  $message = variable_get('site_map_message', array());
  if (!empty($message)) {
    $variables['message'] = check_markup($message['value'], $message['format']);
  }

  if ((variable_get('site_map_show_rss_links', 1) != 0) && module_exists('commentrss') && variable_get('commentrss_site', COMMENTRSS_SITE_FRONT_PAGE)) {
    $variables['rss_legend'] = theme('site_map_rss_legend');
  }

  if (variable_get('site_map_show_front', 1)) {
    $variables['front_page'] = _site_map_front_page();
  }

  if (variable_get('site_map_show_titles', 1)) {
    $variables['show_titles'] = TRUE;
  }

  if (variable_get('site_map_show_blogs', 1)) {
    $variables['blogs'] = _site_map_blogs();
  }

  // Compile the books trees.
  $variables['books'] = _site_map_books();

  // Compile the menu trees.
  $variables['menus'] = _site_map_menus();

  if (variable_get('site_map_show_faq', 0)) {
    $variables['faq'] = _site_map_faq();
  }

  // Compile the vocabulary trees.
  $variables['taxonomys'] = _site_map_taxonomys();

  // Invoke all custom modules and integrate themed HTML into the site map.
  $additional = module_invoke_all('site_map');
  foreach ($additional as $themed_site_map_code) {
    $variables['additional'] .= $themed_site_map_code;
  }
}
