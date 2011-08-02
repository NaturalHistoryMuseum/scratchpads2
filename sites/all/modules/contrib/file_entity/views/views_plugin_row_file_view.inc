<?php

/**
 * @file
 * Contains the file view row style plugin.
 */

/**
 * Plugin which performs a file_view on the resulting object.
 *
 * Most of the code on this object is in the theme function.
 */
class views_plugin_row_file_view extends views_plugin_row {
  // Basic properties that let the row style follow relationships.
  var $base_table = 'file_managed';
  var $base_field = 'fid';

  // Stores the files loaded with pre_render.
  var $files = array();

  function option_definition() {
    $options = parent::option_definition();

    $options['view_mode'] = array('default' => 'default');
    $options['links'] = array('default' => TRUE);
    return $options;
  }

  function options_form(&$form, &$form_state) {
    parent::options_form($form, $form_state);

    $form['view_mode'] = array(
      '#type' => 'select',
      '#options' => $this->options_form_summary_options(),
      '#title' => t('View mode'),
      '#default_value' => $this->options['view_mode'],
    );
    $form['links'] = array(
      '#type' => 'checkbox',
      '#title' => t('Display links'),
      '#default_value' => $this->options['links'],
    );
  }

  /**
   * Return the main options, which are shown in the summary title.
   */
  function options_form_summary_options() {
    $entity_info = entity_get_info('file');
    $options = array('default' => t('Default'));
    if (!empty($entity_info['view modes'])) {
      foreach ($entity_info['view modes'] as $mode => $settings) {
        $options[$mode] = $settings['label'];
      }
    }
    return $options;
  }

  function summary_title() {
    $options = $this->options_form_summary_options();
    return check_plain($options[$this->options['view_mode']]);
  }

  function pre_render($values) {
    $fids = array();
    foreach ($values as $row) {
      $fids[] = $row->{$this->field_alias};
    }
    $this->files = file_load_multiple($fids);
  }

  function render($row) {
    $file = $this->files[$row->{$this->field_alias}];
    $file->view = $this->view;
    $build = file_view($file, $this->options['view_mode']);

    return drupal_render($build);
  }
}
