<?php

/**
 * @file
 * Contains the 'flot' field handler.
 */
class views_flot_handler_field extends views_handler_field {

  function option_definition() {
    $options = parent::option_definition();
    $options['flot'] = array(
      'default' => array(
        'axis' => 'x',
        'x' => array(
          'mode' => '',
        ),
        'y' => array(
          'type' => 'line',
          'color' => '',
          'function' => 'count'
        ),
      ),
    );
    return $options;
  }

  /**
   * Provide field rewrite form.
   */
  function options_form(&$form, &$form_state) {
    parent::options_form($form, $form_state);
    if ($this->view->display_handler->get_option('style_plugin') == 'flot_fields') {
      $form['flot'] = array(
        '#type' => 'fieldset',
        '#title' => t('Flot'),
        '#description' => t('Administer flot settings for this field'),
      );

      $form['flot']['axis'] = array(
        '#type' => 'radios',
        '#title' => t('Axis'),
        '#description' => t('Determine what axis this field should show on.'),
        '#options' => array('x' => t('X-axis'), 'y' => t('Y-axis')),
        '#default_value' => $this->options['flot']['axis'],
        '#fieldset' => 'flot',
      );

      $form['flot']['x'] = array(
        '#type' => 'fieldset',
        '#title' => t('Data Settings'),
        '#collapsible' => TRUE,
        '#collapsed' => TRUE,
        '#states' => array(
          'visible' => array(
            ':input[name="options[flot][axis]"]' => array('value' => 'x'),
          ),
        ),
      );

      $form['flot']['x']['mode'] = array(
        '#type' => 'select',
        '#title' => t('Data is time (date)'),
        '#options' => array('' => 'no', 'time' => 'yes'),
        '#default_value' => $this->options['flot']['x']['mode'],
      );

      $form['flot']['y'] = array(
        '#tree' => TRUE,
        '#type' => 'fieldset',
        '#title' => t('Data Settings'),
        '#collapsible' => TRUE,
        '#collapsed' => TRUE,
        '#states' => array(
          'visible' => array(
            ':input[name="options[flot][axis]"]' => array('value' => 'y'),
          ),
        ),
      );

      $form['flot']['y']['type'] = array(
        '#type' => 'select',
        '#title' => t('Graph type'),
        '#options' => array('line' => t('Line'), 'bar' => t('Bar'), 'point' => t('Point'), 'pie' => t('Pie chart')),
        '#description' => t("Choose the type of chart you would like to display."),
        '#default_value' => $this->options['flot']['y']['type'],
      );

      $form['flot']['y']['color'] = array(
        '#type' => 'textfield',
        '#title' => t('Color'),
        '#description' => t("Choose the color for this data. Leave empty for auto generated colors."),
        '#default_value' => $this->options['flot']['y']['color'],
      );

      $form['flot']['y']['function'] = array(
        '#type' => 'select',
        '#title' => t('Function'),
        '#options' => array('count' => t('Count'), 'sum' => t('Sum'), 'min' => t('Min'), 'max' => t('Max'), 'label' => t('Label')),
        '#description' => t('This is only used for search api based views!.'),
        '#default_value' => $this->options['flot']['y']['function'],
      );
    }
  }

}

