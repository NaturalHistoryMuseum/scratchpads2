<?php

/**
 * @file
 * Field handler to provide additional control for the email field.
 */

class views_rss_handler_field_user_mail extends views_handler_field_user_mail {

  function option_definition() {
    $options = parent::option_definition();
    $options['rss_format'] = array('default' => 'default');
    return $options;
  }

  function options_form(&$form, &$form_state) {
    parent::options_form($form, $form_state);
    $form['rss_format'] = array(
      '#title' => t('Display type'),
      '#type' => 'select',
      '#options' => array(
        'default' => t('Generic email'),
        'with_name' => t('RSS <author> element (with user name)'),
      ),
      '#default_value' => isset($this->options['rss_format']) ? $this->options['rss_format'] : 'default',
      '#weight' => 0,
    );
    // Hide "Link this field" radios if "RSS <author> element"
    // is selected as "Display type".
    $form['link_to_user']['#states'] = array(
      'visible' => array(
        ':input[name="options[rss_format]"]' => array('!value' => 'with_name'),
      ),
    );
  }

  function render_link($data, $values) {
    if ($this->options['rss_format'] == 'with_name') {
      // Try to load full user object based on email address.
      $value = $data;
      if ($account = user_load_by_mail($data)) {
        $value = $account->mail . ' (' . $account->name . ')';
      }
      // Basic XML element details.
      $rss_element = array(
        'key' => 'author',
        'value' => $value,
      );
      // Prepare and store raw-rendered values in view results,
      // the same way as values from standard field formatters are added.
      $rendered = array(
        '#item' => $data,
        '#markup' => format_xml_elements(array($rss_element)),
        '#rss_element' => $rss_element,
        '#settings' => $this->options,
      );
      // Store raw-rendered values in view results, the same way
      // as values from standard field formatters are added.
      $row_index = $this->view->row_index;
      $field_name = 'field_' . $this->field;
      $this->view->result[$row_index]->{$field_name}[0] = array(
        'rendered' => $rendered,
        'raw' => $data,
      );
      // Return formatted XML element.
      return format_xml_elements(array($rss_element));
    }
    // Standard Views render_link().
    return parent::render_link($data, $values);
  }

}
