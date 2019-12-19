<?php

/**
 *
 * Implentation of hook_block_view_alter
 * Move the view class to the container
 * @param array $blocks
 * @param array $data
 */
function scratchpads_block_view_alter(&$data, $block)
{
  if (isset($data['content']) && ($block->module == 'views' || $block->region == 'content')) {
    // Move the view classes to the block classes
    if (preg_match('/(grid-[0-9])/', $data['content']['#markup'], $matches)) {
      if (count($matches)) {
        $data['content']['#markup'] = str_replace($matches[1], '', $data['content']['#markup']);
        $classes = array(
          $matches[1]
        );
        if (strpos($data['content']['#markup'], 'alpha')) {
          str_replace('alpha', '', $data['content']['#markup']);
          $classes[] = 'alpha';
        }
        if (strpos($data['content']['#markup'], 'omega')) {
          str_replace('omega', '', $data['content']['#markup']);
          $classes[] = 'omega';
        }
        $data['class'] = $classes;
      }
    } else {
      $data['class'] = array('gridless');
    }
  }
}

function scratchpads_contextual_links_view_alter(&$element, &$items)
{
  if (isset($element['#element']['#block'])) {
    // Remove contextual links for some blocks
    switch ($element['#element']['#block']->module) {
      case 'search':
        switch ($element['#element']['#block']->delta) {
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

function scratchpads_form_user_login_block_alter(&$form, &$form_state, $form_id)
{
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

function scratchpads_user_login_form_validate(&$form, &$form_state)
{
  // If there's validation errors, we don't want to display
  if (form_get_errors()) {
    unset($form['#attributes']['style']);
  }
}

function scratchpads_form_search_block_form_alter(&$form, &$form_state, $form_id)
{
  $form['actions']['#weight'] = -10;
}

function scratchpads_biblio_tabular($variables)
{
  module_load_include('inc', 'biblio', '/includes/biblio.contributors');
  $node = $variables['node'];
  $base = $variables['base'];
  static $citeproc;
  if (module_exists('popups')) {
    popups_add_popups();
  }
  $tid = $node->biblio_type;
  $fields = _biblio_get_field_information($node->biblio_type, TRUE);
  if (!isset($node->biblio_type_name) && isset($node->biblio_type)) { // needed for preview
    if (($pub_type = db_query('SELECT t.tid, t.name FROM {biblio_types} as t WHERE t.tid=:tid', array(
      ':tid' => $node->biblio_type
    ))->fetchObject())) {
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
  if (!empty($node->biblio_doi)) {
    $doi_url = '';
    if (($doi_start = strpos($node->biblio_doi, '10.')) !== FALSE) {
      $doi = substr($node->biblio_doi, $doi_start);
      $doi_url .= 'http://dx.doi.org/' . $doi;
      $doi = l($doi, $doi_url, $attrib);
    }
  }
  foreach ($fields as $key => $row) {
    // handling the contributor categories like any other field orders them correctly by weight
    if ($row['type'] == 'contrib_widget' && ($authors = biblio_get_contributor_category($node->biblio_contributors, $row['fid']))) {
      foreach ($authors as $author_index => $author) {
        if (empty($author['firstname']) && empty($author['lastname'])) {
          $authors[$author_index]['literal'] = TRUE;
        }
      }
      $data = biblio_format_authors($authors);
    } elseif (empty($node->{$row['name']}) || $row['name'] == 'biblio_coins') {
      continue;
    } else {
      switch ($row['name']) {
        case 'biblio_keywords':
          $data = _biblio_keyword_links($node->{$row['name']}, $base);
          break;
        case 'biblio_url':
          $data = l($node->{$row['name']}, $node->{$row['name']}, $attrib);
          break;
        case 'biblio_doi':
          $data = $doi;
          break;
        default:
          if ($row['type'] == 'text_format') {
            $data = check_markup($node->{$row['name']}, $node->biblio_formats[$row['name']]);
          } else {
            $data = check_plain($node->{$row['name']});
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
  if (isset($node->body) && !empty($node->body) && user_access('view full text')) {
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

function scratchpads_user_login_block($variables)
{
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

function scratchpads_theme()
{
  return array(
    'user_login_block' => array(
      'arguments' => array(
        'form' => NULL
      ),
      'render element' => 'form'
    )
  );
}
