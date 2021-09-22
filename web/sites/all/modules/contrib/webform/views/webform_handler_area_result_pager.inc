<?php

/**
 * @file
 * Definition of views_handler_area_result.
 */

/**
 * Views area handler to display some configurable result summary.
 *
 * @ingroup views_area_handlers
 */
class webform_handler_area_result_pager extends views_handler_area_result {

  function option_definition() {
    $options = parent::option_definition();
    $options['content']['default'] = t('Displaying @start - @end of @total. @items_per_page_links');
    $options['tokenize'] = array('default' => FALSE, 'bool' => TRUE);
    return $options;
  }

  function options_form(&$form, &$form_state) {
    parent::options_form($form, $form_state);
    $form['content']['#description'] .= '<br/>' . t('Plus @items_per_page_links -- the list of links to change the items/page, with prompt');

    $form['tokenize'] = array(
      '#type' => 'checkbox',
      '#title' => t('Use replacement tokens from the first row'),
      '#default_value' => $this->options['tokenize'],
    );
  }


  /**
   * Find out the information to render.
   */
  function render($empty = FALSE) {
    $output = parent::render($empty);
    if (is_string($output) && isset($this->view->query->pager)) {
      $output = str_replace('@items_per_page_links', $this->render_items_per_page($this->view->query->pager), $output);
      if ($this->options['tokenize']) {
        $output = $this->view->style_plugin->tokenize_value($output, 0);
        $output = $this->sanitize_value($output, 'xss_admin');
      }
    }
    return $output;
  }

  function query() {
    $view = $this->view;
    if (!empty($_GET['items_per_page']) && $_GET['items_per_page'] > 0) {
      $view->set_items_per_page($_GET['items_per_page']);
    }
    elseif (!empty($_GET['items_per_page']) && $_GET['items_per_page'] == 'All') {
      $view->set_items_per_page(0);
    }
  }

  function render_items_per_page($pager) {
    $sanitized_options = array();
    if (!empty($pager->options['expose']['items_per_page_options'])) {
      $options = explode(',', $pager->options['expose']['items_per_page_options']);
      foreach ($options as $option) {
        if ($pager->total_items <= intval($option)) {
          break;
        }
        $sanitized_options[intval($option)] = intval($option);
      }
      if (!empty($sanitized_options) && !empty($pager->options['expose']['items_per_page_options_all']) && !empty($pager->options['expose']['items_per_page_options_all_label'])) {
        $sanitized_options[0] = $pager->options['expose']['items_per_page_options_all_label'];
      }
      $url_query = $_GET;
      unset($url_query['q']);
      foreach ($sanitized_options as $items_per_page => &$label) {
        $selected = $pager->options['items_per_page'] == $items_per_page ||
                    ($items_per_page == 0 && $pager->total_items < intval($pager->options['items_per_page']));
        $label = l($label, $_GET['q'], array('query' => array('items_per_page' => $label) + $url_query,
                                             'attributes' => array(
                                                'class' => $selected ? array('selected') : array(),
                                            )));

      }
      unset($label); // Drop PHP reference

      // Include CSS needed for 'selected' class.
      drupal_add_library('webform', 'admin');
    }
    return $sanitized_options ? t('Show !links results per page.', array('!links' => implode(' | ', $sanitized_options))) : '';
  }

}
