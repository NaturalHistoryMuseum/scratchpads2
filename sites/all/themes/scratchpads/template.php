<?php

/**
 *
 * Implentation of hook_block_view_alter
 * Move the view class to the container
 * @param array $blocks
 * @param array $data
 */
function scratchpads_block_view_alter(&$data, $block){
  if(isset($data['content']) && $block->module == 'views'){
    // Move the view classes to the block classes
    if(preg_match('/(grid-[0-9])/', $data['content']['#markup'], $matches)){
      if(count($matches)){
        $data['content']['#markup'] = str_replace($matches[1], '', $data['content']['#markup']);
        $classes = array(
          $matches[1]
        );
        if(strpos($data['content']['#markup'], 'alpha')){
          str_replace('alpha', '', $data['content']['#markup']);
          $classes[] = 'alpha';
        }
        if(strpos($data['content']['#markup'], 'omega')){
          str_replace('omega', '', $data['content']['#markup']);
          $classes[] = 'omega';
        }
        $data['class'] = $classes;
      }
    }
  }
}

/**
 * Implements hook_preprocess_maintenance_page().
 *
 * This is a copy from the Bartik theme.
 */
function scratchpads_preprocess_maintenance_page(&$variables){
  // By default, site_name is set to Drupal if no db connection is available
  // or during site installation. Setting site_name to an empty string makes
  // the site and update pages look cleaner.
  // @see template_preprocess_maintenance_page
  if(!$variables['db_is_active']){
    $variables['site_name'] = '';
  }
  if(function_exists('alpha_css_include')){
    alpha_css_include();
  }
}

function scratchpads_contextual_links_view_alter(&$element, &$items){
  if(isset($element['#element']['#block'])){
    // Remove contextual links for some blocks
    switch($element['#element']['#block']->module){
      case 'search':
        switch($element['#element']['#block']->delta){
          case 'form':
            unset($element['#links']);
            break;
        }
        break;
      case 'scratchpads_colour':
      case 'scratchpads_blocks':
        unset($element['#links']);
        break;
    }
  }
}

function scratchpads_form_user_login_block_alter(&$form, &$form_state, $form_id){
  $form['links']['#weight'] = 100;
  $form['#suffix'] = l('Log in', 'user', array(
    'attributes' => array(
      'class' => array(
        'scratchpads-slide-toggle'
      )
    )
  ));
  $form['#validate'][] = 'scratchpads_user_login_form_validate';
  $form['#attributes']['style'] = 'display:none';
}

function scratchpads_user_login_form_validate(&$form, &$form_state){
  // If there's validation errors, we don't want to display
  if(form_get_errors()){
    unset($form['#attributes']['style']);
  }
}

function scratchpads_preprocess_breadcrumb(&$variables){
  if(count($variables['breadcrumb'])){
    $variables['breadcrumb'][] = drupal_get_title();
    if(function_exists('scratchpads_tweaks_translate_after_check_plain')){
      foreach($variables['breadcrumb'] as $key => $value){
        $variables['breadcrumb'][$key] = scratchpads_tweaks_translate_after_check_plain($value);
      }
    }
  }
}

function scratchpads_preprocess_user_picture(&$variables){
  if(!module_exists('gravatar')){return;}
  if(variable_get('user_pictures', 0)){
    // Load the full user object since it is not provided with nodes, comments,
    // or views displays.
    $account = _gravatar_load_account($variables['account']);
    $filepath = _gravatar_get_account_user_picture($account);
    // We check for the image in cache_image_sizes, if it's not there, we get
    // the image from the server and check its size.
    $file_path_md5 = md5($filepath);
    $data = cache_get($file_path_md5, 'cache_image_sizes');
    if($data){
      $gravatar_img_size = $data->data;
    }elseif(!empty($filepath)){
      $gravatar_img_size = getimagesize($filepath);
      // We only cache for one week if we don't have an image.  This means a
      // user can add a gravatar image, and it will get picked up after one
      // week.
      cache_set($file_path_md5, $gravatar_img_size, 'cache_image_sizes', $gravatar_img_size ? CACHE_PERMANENT : time() + 604800);
    }else{
      $gravatar_img_size = 0;
    }
    $default = FALSE;
    // If there is no picture, check to see if there is a default picture
    if(!is_array($gravatar_img_size) && variable_get('user_picture_default', '')){
      $filepath = variable_get('user_picture_default', '');
      $default = TRUE;
    }
    // no picture and no default
    if((!is_array($gravatar_img_size)) && !$default){
      $variables['user_picture'] = '';
    }else{
      if(!empty($filepath)){
        $alt = t($filepath, array(
          '@user' => format_username($account)
        ));
        if(module_exists('image') && file_valid_uri($filepath) && $style = variable_get('user_picture_style', '')){
          $image_style_args = array(
            'style_name' => $style,
            'path' => $filepath,
            'alt' => $alt,
            'title' => $alt
          );
          if(arg(0) != 'user'){
            $variables['user_picture'] = theme('image', array(
              'path' => $filepath,
              'alt' => $alt,
              'attributes' => array(
                'width' => '20px',
                'height' => '20px'
              )
            ));
          }
        }elseif(arg(0) != 'user'){
          $variables['user_picture'] = theme('image', array(
            'path' => $filepath,
            'alt' => $alt,
            'attributes' => array(
              'width' => '20px',
              'height' => '20px'
            )
          ));
        }else{
          $variables['user_picture'] = theme('image', array(
            'path' => $filepath,
            'alt' => $alt,
            'title' => $alt
          ));
        }
        if($account->uid && user_access('access user profiles')){
          // Create link to the user's profile.
          $attributes = array(
            'title' => t('View user profile.')
          );
          $variables['user_picture'] = l($variables['user_picture'], 'user/' . $account->uid, array(
            'attributes' => $attributes,
            'html' => TRUE
          ));
        }elseif(!empty($account->homepage)){
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
 * Implements hook_process_region().
 */
function scratchpads_process_region(&$vars){
  $theme = alpha_get_theme();
  switch($vars['elements']['#region']){
    case 'content':
      if(isset($theme->page['page']['subtitle'])){
        $vars['subtitle'] = $theme->page['page']['subtitle'];
      }else{
        $vars['subtitle'] = NULL;
      }
      if(isset($theme->page['page']['title'])){
        $vars['title'] = $theme->page['page']['title'];
      }
      break;
  }
}

/**
 * Implements hook_preprocess_region().
 */
function scratchpads_preprocess_region(&$vars){
  global $user;
  switch($vars['elements']['#region']){
    case 'secondary_menu':
      $links = array();
      if(module_exists('scratchpads_contact') && user_access('access site-wide contact form')){
        $categories = scratchpads_contact_get_categories();
        foreach($categories as $category){
          $links[] = array(
            'href' => 'contact/' . $category->cid,
            'title' => $category->category
          );
        }
      }
      if(user_is_logged_in()){
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

function scratchpads_form_search_block_form_alter(&$form, &$form_state, $form_id){
  $form['actions']['#weight'] = -10;
}

function scratchpads_preprocess_node(&$variables){
  if($variables['view_mode'] == 'linked_node'){
    $node_info = node_type_load($variables['type']);
    $variables['title'] = $node_info->name;
    $variables['display_submitted'] = false;
  }
}

function scratchpads_biblio_tabular($variables){
  module_load_include('inc', 'biblio', '/includes/biblio.contributors');
  $node = $variables['node'];
  $base = $variables['base'];
  static $citeproc;
  if(module_exists('popups')){
    popups_add_popups();
  }
  $tid = $node->biblio_type;
  $fields = _biblio_get_field_information($node->biblio_type, TRUE);
  if(!isset($node->biblio_type_name) && isset($node->biblio_type)){ // needed for preview
    if(($pub_type = db_query('SELECT t.tid, t.name FROM {biblio_types} as t WHERE t.tid=:tid', array(
      ':tid' => $node->biblio_type
    ))->fetchObject())){
      $node->biblio_type_name = drupal_ucfirst(_biblio_localize_type($pub_type->tid, $pub_type->name));
    }
  }
  $rows[] = array(
    array(
      'data' => t('Publication Type:'),
      'class' => array(
        'biblio-row-title'
      )
    ),
    array(
      'data' => $node->biblio_type_name
    )
  );
  $attrib = (variable_get('biblio_links_target_new_window', FALSE)) ? array(
    'target' => '_blank'
  ) : array();
  $doi = '';
  if(!empty($node->biblio_doi)){
    $doi_url = '';
    if(($doi_start = strpos($node->biblio_doi, '10.')) !== FALSE){
      $doi = substr($node->biblio_doi, $doi_start);
      $doi_url .= 'http://dx.doi.org/' . $doi;
      $doi = l($doi, $doi_url, $attrib);
    }
  }
  foreach($fields as $key => $row){
    // handling the contributor categories like any other field orders them correctly by weight
    if($row['type'] == 'contrib_widget' && ($authors = biblio_get_contributor_category($node->biblio_contributors, $row['fid']))){
      foreach($authors as $author_index => $author){
        if(empty($author['firstname']) && empty($author['lastname'])){
          $authors[$author_index]['literal'] = TRUE;
        }
      }
      $data = biblio_format_authors($authors);
    }elseif(empty($node->$row['name']) || $row['name'] == 'biblio_coins'){
      continue;
    }else{
      switch($row['name']){
        case 'biblio_keywords':
          $data = _biblio_keyword_links($node->$row['name'], $base);
          break;
        case 'biblio_url':
          $data = l($node->$row['name'], $node->$row['name'], $attrib);
          break;
        case 'biblio_doi':
          $data = $doi;
          break;
        default:
          if($row['type'] == 'text_format'){
            $data = check_markup($node->$row['name'], $node->biblio_formats[$row['name']]);
          }else{
            $data = check_plain($node->$row['name']);
          }
      }
    }
    $rows[] = array(
      array(
        'data' => t($row['title']) . ':',
        'class' => array(
          'biblio-row-title'
        )
      ),
      array(
        'data' => $data
      )
    );
  }
  if(isset($node->body) && !empty($node->body) && user_access('view full text')){
    $rows[] = array(
      array(
        'data' => t('Full Text'),
        'valign' => 'top'
      ),
      array(
        'data' => drupal_render(field_view_field('node', $node, 'body', array(
          'label' => 'hidden'
        )))
      )
    );
  }
  $output = '<div id="biblio-node">';
  $output .= filter_xss($node->biblio_coins, array(
    'span'
  ));
  $header = array();
  $output .= theme('table', array(
    'header' => $header,
    'rows' => $rows
  ));
  $output .= '</div>';
  return $output;
}

/**
 * Implements hook_preprocess_page().
 */
function scratchpads_preprocess_page(&$vars){
  // Resize the logo so it uses the thumbnail image style
  if(variable_get('resize_logo', 1) && $vars['logo']){
    $filename = urldecode(basename($vars['logo']));
    if(file_exists('public://' . $filename)){
      $vars['logo'] = image_style_url('thumbnail', $filename);
    }
  }
  if(isset($vars['tabs']) && empty($vars['tabs']['#primary'])){
    $vars['tabs'] = array();
  }
}

/**
 * Implements hook_preprocess_zone().
 */
function scratchpads_preprocess_zone(&$vars){
  if($vars['elements']['#zone'] == 'content'){
    // Add a class so we know when there's a side bar or not
    if(!empty($vars['elements']['sidebar'])){
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
function scratchpads_preprocess_html(&$vars){
  // Add a class for a specific domain to allow for some site only CSS (eg, Vlads logo)
  global $base_url;
  $vars['attributes_array']['class'][] = 'site-' . str_replace('.', '-', parse_url($base_url, PHP_URL_HOST));
}

function scratchpads_user_login_block($variables){
  $form = $variables['form'];
  $form['name']['#attributes']['tabindex'] = 1;
  $form['pass']['#attributes']['tabindex'] = 2;
  @$form['remember_me']['#attributes']['tabindex'] = 4;
  $form['actions']['submit']['#attributes']['tabindex'] = 3;
  $output = '<div class="clearfix" style="float:right">';
  $output .= drupal_render($form['name']);
  $output .= drupal_render($form['pass']);
  $output .= drupal_render($form['openid_identifier']);
  $output .= drupal_render($form['actions']);
  $output .= '</div>';
  $output .= drupal_render($form['remember_me']);
  $output .= '<div class="account-links">';
  $output .= drupal_render($form['links']);
  $output .= '</div>';
  $form['links']['#attributes']['class'] = array(
    'account-links'
  );
  $output .= drupal_render_children($form);
  return $output;
}

function scratchpads_theme(){
  return array(
    'user_login_block' => array(
      'arguments' => array(
        'form' => NULL
      ),
      'render element' => 'form'
    )
  );
}
