<?php
class views_oai_pmh_plugin_row_ls extends views_plugin_row {

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
    $ls_errors = array();
    $link = $this->display->handler->option_link('Row options', 'row_options');

    $field_handlers = $this->display->handler->get_handlers('field');
    foreach ($field_handlers as $id => $field) {
      $ui_label = $field->ui_name();
      $label = $field->label();
      if (empty($label) || substr($label, 0, 3) != 'set' ) {
        $ls_errors[] = t('The field "@term" does not have a right label associated with it. Go to the !link page to fix it.', array('@term' => $ui_label, '!link' => $link));
      }
    }
    return array_merge($errors, $ls_errors);
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

    $ls_elements = array(
      'none'					 => t('None'),
      'setSpec'			 => 'setSpec',
      'setName'     => 'setName',
      
    );

    $form['oai_labels'] = array(
      '#type' => 'fieldset',
      '#title' => t('Drupal field to oai listsets mapping'),
      '#theme' => 'oai_field_mapper_form',
    );

    foreach ($ui_labels as $id => $label) {
      $form['oai_labels'][$id] = array(
        '#type' => 'select',
        '#options' => $ls_elements,
        '#default_value' => isset($labels[$id]) && in_array($labels[$id], $ls_elements) ? $labels[$id] : 'none',
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
        $ls_labels = $form_state['values'][$section]['oai_labels'];
        foreach ($field_handlers as $id => $field) {
          $options = $field->options;
          if ($ls_labels[$id] != 'none') {
            $options['custom_label'] = TRUE;
            $options['label'] = $ls_labels[$id];
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
