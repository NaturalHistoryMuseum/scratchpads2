<?php

/**
 * @file
 * Contains the submission view row style plugin.
 */

/**
 * Plugin which performs a webform_submission_render on the resulting object.
 *
 * Most of the code on this object is in the theme function.
 *
 * @ingroup views_row_plugins
 */
class webform_views_plugin_row_submission_view extends views_plugin_row {
  // Basic properties that let the row style follow relationships.
  public $base_table = 'webform_submissions';
  public $base_field = 'sid';

  // Stores the nodes loaded with pre_render.
  private $submissions = array();
  private $nodes = array();

  function option_definition() {
    $options = parent::option_definition();

    $options['format'] = array('default' => 'html');

    return $options;
  }

  function options_form(&$form, &$form_state) {
    parent::options_form($form, $form_state);

    $options = $this->options_form_summary_options();
    $form['format'] = array(
      '#type' => 'radios',
      '#options' => $options,
      '#title' => t('Display mode'),
      '#default_value' => $this->options['format'],
    );
  }

  /**
   * Return the main options, which are shown in the summary title.
   */
  function options_form_summary_options() {
    return array(
      'html' => t('HTML'),
      'text' => t('Plain text'),
    );
  }

  function summary_title() {
    $options = $this->options_form_summary_options();
    return check_plain($options[$this->options['format']]);
  }

  function pre_render($values) {
    $sids = array();
    foreach ($values as $row) {
      $sids[] = $row->{$this->field_alias};
    }
    module_load_include('inc', 'webform', 'includes/webform.submissions');
    $this->submissions = $sids ? webform_get_submissions(array('sid' => $sids)) : array();

    $nids = array();
    foreach ($this->submissions as $sid => $submission) {
      $nids[] = $submission->nid;
    }
    $nids = array_unique($nids);
    $this->nodes = $nids ? node_load_multiple($nids) : array();
  }

  function render($row) {
    if (isset($this->submissions[$row->{$this->field_alias}])) {
      $submission = $this->submissions[$row->{$this->field_alias}];
      $node = $this->nodes[$submission->nid];
      $submission->view = $this->view;
      $format = $this->options['format'];
      $build = webform_submission_render($node, $submission, NULL, $format);

      // Add extra theme functions:
      $themes = array();
      foreach ($build['#theme'] as $hook) {
        $themes = array_merge($themes, _views_theme_functions($hook, $this->view, $this->view->display[$this->view->current_display]));
      }
      $build['#theme'] = $themes;

      // Render built submission, and if unsanitized plain text is used, make it safe for display.
      $render = drupal_render($build);
      return $format == 'html' ? $render : nl2br(check_plain($render));
    }
  }
}
