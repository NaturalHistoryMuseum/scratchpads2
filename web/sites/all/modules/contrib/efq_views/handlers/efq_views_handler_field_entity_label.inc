<?php
// $Id$

/**
 * Field handler for entity labels.
 */
class efq_views_handler_field_entity_label extends views_handler_field {
  /**
   * Called to determine what to tell the clicksorter.
   */
  function click_sort($order) {
    $this->query->query->propertyOrderBy($this->definition['label column'], $order);
  }

  function option_definition() {
    $options = parent::option_definition();
    $options['link_to_entity'] = array('default' => FALSE);
    return $options;
  }

  /**
   * Provide link to entity option
   */
  function options_form(&$form, &$form_state) {
    parent::options_form($form, $form_state);
    $form['link_to_entity'] = array(
      '#title' => t('Link this field to its entity'),
      '#description' => t('This will override any other link you have set.'),
      '#type' => 'checkbox',
      '#default_value' => !empty($this->options['link_to_entity']),
    );
  }

  /**
   * Override the parent's query method, since it doesn't need to do anything.
   */
  function query() {}

  /**
   * Render whatever the data is as a link to the entity.
   *
   * Data should be made XSS safe prior to calling this function.
   */
  function render_link($data, $values) {
    if (!empty($this->options['link_to_entity']) && $data !== NULL && $data !== '') {
      $entity_uri = entity_uri($values->entity_type, $values);
      $this->options['alter']['make_link'] = TRUE;
      $this->options['alter']['path'] = $entity_uri['path'];
      $this->options['alter'] += $entity_uri['options'];
      if (isset($this->aliases['language'])) {
        $languages = language_list();
        if (isset($languages[$values->{$this->aliases['language']}])) {
          $this->options['alter']['language'] = $languages[$values->{$this->aliases['language']}];
        }
        else {
          unset($this->options['alter']['language']);
        }
      }
    }
    else {
      $this->options['alter']['make_link'] = FALSE;
    }
    return $data;
  }

  function render($values) {
    $label = entity_label($values->entity_type, $values);;
    return $this->render_link(check_plain($label), $values);
  }
}
