<?php

/**
 * @file
 * Theme functions for the Facet API module.
 */

/**
 * Returns HTML for a "missing" facet link.
 *
 * @param $variables
 *   An associative array containing:
 *   - field_name: The name of the facet field.
 *
 * @ingroup themeable
 */
function theme_facetapi_facet_missing($variables) {
  return t('Missing %field_name', array('%field_name' => $variables['field_name']));
}

/**
 * Returns HTML for the facet title, usually the title of the block.
 *
 * @param $variables
 *   An associative array containing:
 *   - title: The title of the facet.
 *
 * @ingroup themeable
 */
function theme_facetapi_title($variables) {
  return t('Filter by @title:', array('@title' => drupal_strtolower($variables['title'])));
}

/**
 * Returns HTML for an active facet item.
 *
 * @param $variables
 *   An associative array containing the keys 'text', 'path', 'options', and
 *   'count'. See the l() and theme_facetapi_count() functions for information
 *   about these variables.
 *
 * @ingroup themeable
 */
function theme_facetapi_link_inactive($variables) {
  if (isset($variables['count'])) {
    $variables['text'] .= ' ' . theme('facetapi_count', $variables);
  }
  return theme_link($variables);
}

/**
 * Returns HTML for the inactive facet item's count.
 *
 * @param $variables
 *   An associative array containing:
 *   - count: The item's facet count.
 *
 * @ingroup themeable
 */
function theme_facetapi_count($variables) {
  return '(' . (int) $variables['count'] . ')';
}

/**
 * Returns HTML for an inactive facet item.
 *
 * @param $variables
 *   An associative array containing the keys 'text', 'path', and 'options'. See
 *   the l() function for information about these variables.
 *
 * @ingroup themeable
 */
function theme_facetapi_link_active($variables) {
  $suffix = '';
  if (isset($variables['text'])) {
    if (empty($variables['options']['html'])) {
      $suffix = ' ' . check_plain($variables['text']);
    }
    else {
      $suffix = ' ' . $variables['text'];
    }
  }
  $accessible_link_text = '<span class="element-invisible">' . t('Remove @text filter', array('@text' => $variables['text'])) . '</span>';
  $variables['options']['html'] = TRUE;
  $variables['text'] = '(-) ' . $accessible_link_text;
  return theme_link($variables) . $suffix;
}
