<?php

/**
 * @file
 * Implements taxonomy term flags. 
 */

/**
 * Implements a term flag.
 */
class flag_term extends flag_flag {
  function options() {
    $options = parent::options();
    $options += array(
      'show_on_term_page' => TRUE,
    );
    return $options;
  }

  function options_form(&$form) {
    parent::options_form($form);

    $options = array();
    foreach (taxonomy_get_vocabularies() as $vocab) {
      $options[$vocab->vid] = check_plain($vocab->name);
    }
    $form['access']['types'] = array(
      '#type' => 'checkboxes',
      '#title' => t('The vocabularies whose terms will be flaggable'),
      '#options' => $options,
      '#default_value' => $this->types,
      '#required' => TRUE,
      '#weight' => 10,
      '#access' => empty($flag->locked['types']),
    );
    $form['display']['show_on_term_page'] = array(
      '#type' => 'checkbox',
      '#title' => t('Display link on taxonomy term pages'),
      '#default_value' => $this->show_on_term_page,
      '#access' => empty($this->locked['show_on_term_page']),
    );
  }

  function _load_content($content_id) {
    return taxonomy_term_load($content_id);
  }

  function applies_to_content_object($term) {
    if ($term && in_array($term->vid, $this->types)) {
      return TRUE;
    }
    return FALSE;
  }

  function get_content_id($term) {
    return $term->tid;
  }

  function uses_hook_link($teaser) {
    if ($this->show_on_term_page) {
      return TRUE;
    }
    return FALSE;
  }

  function get_labels_token_types() {
    return array('term');
  }

  function replace_tokens($label, $contexts, $options, $content_id) {
    if ($content_id && ($term = $this->fetch_content($content_id))) {
      $contexts['term'] = $term;
    }
    return parent::replace_tokens($label, $contexts, $options, $content_id);
  }

  function get_flag_action($content_id) {
    $flag_action = parent::get_flag_action($content_id);
    $term = $this->fetch_content($content_id);
    $flag_action->content_title = $term->name;
    $flag_action->content_url = _flag_url('taxonomy/term/' . $term->tid);
    return $flag_action;
  }

  function get_relevant_action_objects($content_id) {
    return array(
      'taxonomy_term' => $this->fetch_content($content_id),
    );
  }
  
  function get_views_info() {
    return array(
      'views table' => 'taxonomy_term_data',
      'join field' => 'tid',
      'title field' => 'name',
      'title' => t('Taxonomy term flag'),
      'help' => t('Limit results to only those terms flagged by a certain flag; Or display information about the flag set on a term.'),
      'counter title' => t('Taxonomy term flag counter'),
      'counter help' => t('Include this to gain access to the flag counter field.'),
    );
  }

  function applies_to_content_id_array($content_ids) {
    $passed = array();
    foreach ($content_ids as $tid) {
      if ($this->applies_to_content_id($tid)) {
        $passed[$tid] = TRUE;
      }
    }
    return $passed;
  }
}
