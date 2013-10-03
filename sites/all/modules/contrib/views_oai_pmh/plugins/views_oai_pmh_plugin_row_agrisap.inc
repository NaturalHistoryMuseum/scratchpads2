<?php
class views_oai_pmh_plugin_row_agrisap extends views_plugin_row {

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
    $agrisap_errors = array();
    $link = $this->display->handler->option_link('Row options', 'row_options');

    $field_handlers = $this->display->handler->get_handlers('field');
    foreach ($field_handlers as $id => $field) {
      $ui_label = $field->ui_name();
      $label = $field->label();
      if ((empty($label)) || ((substr($label, 0, 3) != 'dc:') && (substr($label, 0, 4) != 'ags:') && (substr($label, 0, 8) != 'dcterms:'))) {
        $agrisap_errors[] = t('The field "@title" does not have an Agris Ap label associated with it. Go to the !link page to fix it.', array('@title' => $ui_label, '!link' => $link));
      }
    }
    return array_merge($errors, $agrisap_errors);
  }

  function options_form(&$form, &$form_state) {
    parent::options_form($form, $form_state);
   // $labels = $this->display->handler->get_field_labels();
    $field_handlers = $this->display->handler->get_handlers('field');
    foreach ($field_handlers as $id => $handler) {
      $ui_labels[$id] = $handler->ui_name();
      if ($label = $handler->label()) {
        $labels[$id] = $label;
      }

    }
    $agrisap_elements = array(
      'none'					 => t('None'),
      'dc:title'			 => 'dc:title',
	  'dcterms:alternative'			 => 'dcterms:alternative',
	  'ags:creatorPersonal'     => 'ags:creatorPersonal',
	  'ags:creatorCorporate' => 'ags:creatorCorporate',
	   'ags:creatorConference' => 'ags:creatorConference',
	   'ags:publisherName' => 'ags:publisherName',
	   'ags:publisherPlace' => 'ags:publisherPlace',
	   'dcterms:dateIssued'     => 'dcterms:dateIssued',
	   'ags:descriptionEdition'     => 'ags:descriptionEdition',
	   'ags:descriptionNotes' => 'ags:descriptionNotes',
       'dcterms:abstract'        => 'dcterms:abstract',
	   'ags:subjectClassification'   => 'ags:subjectClassification',
      'ags:subjectThesaurus'        => 'ags:subjectThesaurus',
	  'dc:type'        => 'dc:type',
	  'dc:identifier'        => 'dc:identifier',
      'dcterms:extent'      => 'dcterms:extent',
	  'dcterms:medium'      => 'dcterms:medium',
	  'dc:language'    => 'dc:language',
      'ags:availabilityLocation'  => 'ags:availabilityLocation',
      'ags:availabilityNumber'      => 'ags:availabilityNumber',
      'ags:citationTitle'    => 'ags:citationTitle',
      'ags:citationIdentifier'    => 'ags:citationIdentifier',
      'ags:citationNumber'    => 'ags:citationNumber',
      'dc:source'      => 'dc:source',
	  'ags:agrovoc_uri'      => 'ags:agrovoc_uri',
	  
    );
    $form['oai_labels'] = array(
      '#type' => 'fieldset',
      '#title' => t('Drupal field to Agris Ap mapping'),
      '#theme' => 'oai_field_mapper_form',
    );
    foreach ($ui_labels as $id => $label) {
      $form['oai_labels'][$id] = array(
        '#type' => 'select',
        '#options' => $agrisap_elements,
        '#default_value' => isset($labels[$id]) && in_array($labels[$id], $agrisap_elements) ? $labels[$id] : 'none',
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
        $agrisap_labels = $form_state['values'][$section]['oai_labels'];
        foreach ($field_handlers as $id => $field) {
          $options = $field->options;
          if ($agrisap_labels[$id] != 'none') {
            $options['custom_label'] = TRUE;
            $options['label'] = $agrisap_labels[$id];
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
