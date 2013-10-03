<?php
class views_oai_pmh_plugin_row_dc extends views_plugin_row {

  function render($row) {

    return theme($this->theme_functions(),
    array(
          'view' => $this->view,
          'options' => $this->options,
          'row' => $row
    ));
  }

  function validate() {
    $errors = parent::validate();
    $dc_errors = array();
    $link = $this->display->handler->option_link('Row options', 'row_options');

    $field_handlers = $this->display->handler->get_handlers('field');
    foreach ($field_handlers as $id => $field) {
      $ui_label = $field->ui_name();
      $label = $field->label();
      if (empty($label) || substr($label, 0, 3) != 'dc:' ) {
        $dc_errors[] = t('The field "@title" does not have a Dublin Core label associated with it. Go to the !link page to fix it.', array('@title' => $ui_label, '!link' => $link));
      }
    }
    return array_merge($errors, $dc_errors);
  }

  function options_form(&$form, &$form_state) {
    parent::options_form($form, $form_state);

    $field_handlers = $this->display->handler->get_handlers('field');

    foreach ($field_handlers as $id => $handler) {
      $ui_labels[$id] = $handler->ui_name();
      if ($label = $handler->label()) {
        $labels[$id] = $label;
      }
    }

    $dc_elements = array(
      'none'					 => t('None'),
      'dc:title'			 => 'dc:title',
      'dc:creator'     => 'dc:creator',
      'dc:subject'     => 'dc:subject',
      'dc:description' => 'dc:description',
      'dc:publisher'   => 'dc:publisher',
      'dc:contributor' => 'dc:contributor',
      'dc:date'        => 'dc:date',
      'dc:type'        => 'dc:type',
      'dc:format'      => 'dc:format',
      'dc:identifier'  => 'dc:identifier',
      'dc:source'      => 'dc:source',
      'dc:language'    => 'dc:language',
      'dc:relation'    => 'dc:relation',
      'dc:coverage'    => 'dc:coverage',
      'dc:rights'      => 'dc:rights',
    );

    $form['oai_labels'] = array(
      '#type' => 'fieldset',
      '#title' => t('Drupal field to Dublin Core mapping'),
      '#theme' => 'oai_field_mapper_form',
    );

    foreach ($ui_labels as $id => $label) {
      $form['oai_labels'][$id] = array(
        '#type' => 'select',
        '#options' => $dc_elements,
        '#default_value' => isset($labels[$id]) && in_array($labels[$id], $dc_elements) ? $labels[$id] : 'none',
      );
      $form['oai_labels'][$id]['drupal_label'] = array(
        '#markup' => $ui_labels[$id],
      );
    }
  }
  function options_submit(&$form, &$form_state) {
    // It is very important to call the parent function here:
    parent::options_submit($form, $form_state);
    $section = $form_state['section'];
    switch ($section) {
      case 'row_options':
        $field_handlers = $this->display->handler->get_handlers('field');
        $dc_labels = $form_state['values'][$section]['oai_labels'];
        foreach ($field_handlers as $id => $field) {
          $options = $field->options;
          if ($dc_labels[$id] != 'none') {
            $options['custom_label'] = TRUE;
            $options['label'] = $dc_labels[$id];
            $options['hide_empty'] = 1;
          }
          else {
            unset($options['custom_label']);
            $options['label'] = '';
          }
          $form_state['view']->set_item($form_state['display_id'], 'field', $id, $options);
        }
        break;
    }
  }
}
