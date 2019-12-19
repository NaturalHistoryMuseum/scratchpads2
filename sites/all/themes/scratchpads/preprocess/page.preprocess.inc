<?php

/**
 * Implements hook_preprocess_page().
 */
function scratchpads_preprocess_page(&$vars)
{
  // Resize the logo so it uses the thumbnail image style
  if (variable_get('resize_logo', 1) && $vars['logo']) {
    $filename = urldecode(basename($vars['logo']));
    if (file_exists('public://' . $filename)) {
      $vars['logo'] = image_style_url('thumbnail', $filename);
    }
  }
  if (isset($vars['tabs']) && empty($vars['tabs']['#primary'])) {
    $vars['tabs'] = array();
  }
}

/**
 * Implements hook_preprocess_zone().
 */
function scratchpads_preprocess_zone(&$vars)
{
  if ($vars['elements']['#zone'] == 'content') {
    // Add a class so we know when there's a side bar or not
    if (!empty($vars['elements']['sidebar'])) {
      $vars['content_attributes_array']['class'][] = 'has-sidebar';
    }
  }
  $path = drupal_get_path('theme', 'scratchpads');
  drupal_add_css($path . '/css/ie8.css', array(
    'media' => 'all',
    'browsers' => array(
      'IE' => '(lt IE 9)',
      '!IE' => FALSE
    )
  ));
}

/**
 * Implements hook_preprocess_html().
 */
function scratchpads_preprocess_html(&$vars)
{
  // Add a class for a specific domain to allow for some site only CSS (eg, Vlads logo)
  global $base_url;
  $vars['attributes_array']['class'][] = 'site-' . str_replace('.', '-', parse_url($base_url, PHP_URL_HOST));
}

function scratchpads_preprocess_node(&$variables)
{
  if ($variables['view_mode'] == 'linked_node') {
    $node_info = node_type_load($variables['type']);
    $variables['title'] = $node_info->name . ' <span style="font-size:80%">(' . $variables['title'] . ')</span>';
    $variables['display_submitted'] = false;
  }
}

/**
 * Implements hook_preprocess_region().
 */
function scratchpads_preprocess_region(&$vars)
{
  global $user;
  switch ($vars['elements']['#region']) {
    case 'secondary_menu':
      $links = array();
      if (module_exists('scratchpads_contact') && user_access('access site-wide contact form')) {
        $categories = scratchpads_contact_get_categories();
        foreach ($categories as $category) {
          $links[] = array(
            'href' => 'contact/' . $category->cid,
            'title' => $category->category
          );
        }
      }
      if (user_is_logged_in()) {
        $links[] = array(
          'href' => url('help.scratchpads.eu'),
          'title' => t("Help"),
          'attributes' => array(
            'target' => '_blank'
          )
        );
      }
      $vars['links'] = theme('links', array(
        'links' => $links
      ));
      break;
  }
}

function scratchpads_preprocess_breadcrumb(&$variables)
{
  if (count($variables['breadcrumb'])) {
    $variables['breadcrumb'][] = drupal_get_title();
    if (function_exists('scratchpads_tweaks_translate_after_check_plain')) {
      foreach ($variables['breadcrumb'] as $key => $value) {
        $variables['breadcrumb'][$key] = scratchpads_tweaks_translate_after_check_plain($value);
      }
    }
  }
}

function scratchpads_preprocess_user_picture(&$variables)
{
  if (!module_exists('gravatar')) {
    return;
  }
  if (variable_get('user_pictures', 0)) {
    // Load the full user object since it is not provided with nodes, comments,
    // or views displays.
    $account = _gravatar_load_account($variables['account']);
    $filepath = _gravatar_get_account_user_picture($account);
    // We check for the image in cache_image_sizes, if it's not there, we get
    // the image from the server and check its size.
    $file_path_md5 = md5($filepath);
    $data = cache_get($file_path_md5, 'cache_image_sizes');
    if ($data) {
      $gravatar_img_size = $data->data;
    } elseif (!empty($filepath)) {
      $gravatar_img_size = getimagesize($filepath);
      // We only cache for one week if we don't have an image.  This means a
      // user can add a gravatar image, and it will get picked up after one
      // week.
      cache_set($file_path_md5, $gravatar_img_size, 'cache_image_sizes', $gravatar_img_size ? CACHE_PERMANENT : time() + 604800);
    } else {
      $gravatar_img_size = 0;
    }
    $default = FALSE;
    // If there is no picture, check to see if there is a default picture
    if (!is_array($gravatar_img_size) && variable_get('user_picture_default', '')) {
      $filepath = variable_get('user_picture_default', '');
      $default = TRUE;
    }
    // no picture and no default
    if ((!is_array($gravatar_img_size)) && !$default) {
      $variables['user_picture'] = '';
    } else {
      if (!empty($filepath)) {
        $alt = t($filepath, array(
          '@user' => format_username($account)
        ));
        if (module_exists('image') && file_valid_uri($filepath) && $style = variable_get('user_picture_style', '')) {
          $image_style_args = array(
            'style_name' => $style,
            'path' => $filepath,
            'alt' => $alt,
            'title' => $alt
          );
          if (arg(0) != 'user') {
            $variables['user_picture'] = theme('image', array(
              'path' => $filepath,
              'alt' => $alt,
              'attributes' => array(
                'width' => '20px',
                'height' => '20px'
              )
            ));
          }
        } elseif (arg(0) != 'user') {
          $variables['user_picture'] = theme('image', array(
            'path' => $filepath,
            'alt' => $alt,
            'attributes' => array(
              'width' => '20px',
              'height' => '20px'
            )
          ));
        } else {
          $variables['user_picture'] = theme('image', array(
            'path' => $filepath,
            'alt' => $alt,
            'title' => $alt
          ));
        }
        if ($account->uid && user_access('access user profiles')) {
          // Create link to the user's profile.
          $attributes = array(
            'title' => t('View user profile.')
          );
          $variables['user_picture'] = l($variables['user_picture'], 'user/' . $account->uid, array(
            'attributes' => $attributes,
            'html' => TRUE
          ));
        } elseif (!empty($account->homepage)) {
          // If user is anonymous, create link to the commenter's homepage.
          $attributes = array(
            'title' => t('View user website.'),
            'rel' => 'external nofollow'
          );
          $variables['user_picture'] = l($variables['user_picture'], $account->homepage, array(
            'attributes' => $attributes,
            'html' => TRUE
          ));
        }
      }
    }
  }
}

/**
 * Implements hook_preprocess_maintenance_page().
 *
 * This is a copy from the Bartik theme.
 */
function scratchpads_preprocess_maintenance_page(&$variables)
{
  // By default, site_name is set to Drupal if no db connection is available
  // or during site installation. Setting site_name to an empty string makes
  // the site and update pages look cleaner.
  // @see template_preprocess_maintenance_page
  if (!$variables['db_is_active']) {
    $variables['site_name'] = '';
  }
  if (function_exists('alpha_css_include')) {
    alpha_css_include();
  }
}