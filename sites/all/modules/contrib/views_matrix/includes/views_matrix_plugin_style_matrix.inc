<?php
/**
 * @file
 *   Class definition for views matrix plugin.
 */

/**
 * @class
 *   Views Plugin Class
 */
class views_matrix_plugin_style_matrix extends views_plugin_style {

  public function option_definition() {
    $options = parent::option_definition();

    $options['sticky'] = array('default' => FALSE);
    $options['xconfig'] = array('default' => array(
      'field' => '',
      'sort' => '',
      'class' => '',
    ));
    $options['yconfig'] = array('default' => array(
      'field' => '',
      'sort' => '',
      'class' => '',
    ));

    return $options;
  }

  function options_form(&$form, &$form_state) {
    parent::options_form($form, $form_state);

    $form['sticky'] = array(
      '#type' => 'checkbox',
      '#title' => t('Enable Drupal style "sticky" table headers (Javascript)'),
      '#default_value' => !empty($this->options['sticky']),
      '#description' => t('The sticky header function only applies to column headers.'),
    );

    $field_options = $this->display->handler->get_field_labels(TRUE);

    $form['xconfig'] = array(
      '#type' => 'fieldset',
      '#title' => t('Column header'),
      '#collapsible' => FALSE,
      '#collapsed' => FALSE,
      '#tree' => TRUE,
    );

    $form['xconfig']['field'] = array(
      '#type' => 'select',
      '#title' => t('Field'),
      '#description' => t('Select the field to display in the column header.'),
      '#options' => $field_options,
      '#default_value' => $this->options['xconfig']['field'],
    );

    $form['xconfig']['sort'] = array(
      '#type' => 'select',
      '#title' => t('Sort'),
      '#description' => t('Select the method to sort the field by.'),
      '#options' => array('asc' => t('Ascending'), 'dsc' => t('Descending')),
      '#default_value' => $this->options['xconfig']['sort'],
    );

    $form['xconfig']['class'] = array(
      '#type' => 'textfield',
      '#title' => t('CSS class'),
      '#description' => t('You may use token substitutions from the rewriting section in this class.'),
      '#default_value' => $this->options['xconfig']['class']
    );

    $form['yconfig'] = array(
      '#type' => 'fieldset',
      '#title' => t('Row header'),
      '#collapsible' => FALSE,
      '#collapsed' => FALSE,
      '#tree' => TRUE,
    );

    $form['yconfig']['field'] = array(
      '#type' => 'select',
      '#title' => t('Field'),
      '#description' => t('Select the field to display in the row header.'),
      '#options' => $field_options,
      '#default_value' => $this->options['yconfig']['field'],
    );

    $form['yconfig']['sort'] = array(
      '#type' => 'select',
      '#title' => t('Sort'),
      '#description' => t('Select the method to sort the field by.'),
      '#options' => array('asc' => t('Ascending'), 'dsc' => t('Descending')),
      '#default_value' => $this->options['yconfig']['sort'],
    );

    $form['yconfig']['class'] = array(
      '#type' => 'textfield',
      '#title' => t('CSS class'),
      '#description' => t('You may use token substitutions from the rewriting section in this class.'),
      '#default_value' => $this->options['yconfig']['class']
    );
  }

  /**
   * Validate row and colum header configuration set. This is only used during
   * runtime. parent::validate() cannot be used because it breaks the Views
   * UI.
   */
  public function canRender() {

    $errors = parent::validate();

    if (empty($this->options['xconfig']['field']) || empty($this->options['yconfig']['field'])) {
      // Do not save without x field or y field.
      $errors[] = t('Style @style requires an x and y field to be defined.', array('@style' => $this->definition['title']));
    }
    else {
      // Check if fields exist still.
      $options = $this->display->handler->get_field_labels(TRUE);

      if (!isset($options[$this->options['xconfig']['field']]) || !isset($options[$this->options['yconfig']['field']])) {
        $errors[] = t('An x or y field is defined, but not does not exist.');
      }
    }

    return $errors;
  }

  /**
   * Return the class of the field headers.
   */
  function headerClasses($dimension, $row_index = NULL) {
    $field_handler = @$this->view->field[$this->options["{$dimension}config"]['field']];
    if (empty($field_handler)) return array();
    $classes = explode(' ', $this->options["{$dimension}config"]['class']);
    foreach ($classes as &$class) {
      $class = $field_handler->tokenize_value($class, $row_index);
      $class = views_clean_css_identifier($class);
    }
    return $classes !== FALSE ? $classes : array();
  }
}
