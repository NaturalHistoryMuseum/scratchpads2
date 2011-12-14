<?php
/**
 * @file
 * Display style plugin for Views that renders rows as a jPlayer playlist.
 */

/**
 * Style plugin to display a jPlayer playlist.
 */
class jplayer_style_plugin extends views_plugin_style {

  // Allow some options for the style.
  function option_definition() {
    $options = parent::option_definition();
    $options['path_field'] = array('default' => NULL);
    $options['label_field'] = array('default' => NULL);

    // Get the formatter default settings into the option_definition.
    $formatter = jplayer_field_formatter_info();
    $settings = $formatter['jplayer_player']['settings'];
    foreach ($settings as $name => $default_value) {
      $options[$name] = array('default' => $default_value);
    }

    return $options;
  }

  // Create the options form.
  function options_form(&$form, &$form_state) {
    parent::options_form($form, $form_state);

    // Build a list of all fields.
    $options = array();
    foreach ($this->display->handler->get_handlers('field') as $field => $handler) {
      if ($label = $handler->label()) {
        $options[$field] = $label;
      }
      else {
        $options[$field] = $handler->ui_name();
      }
    }

    if (empty($options)) {
      $options[''] = t('No available fields');
    }

    $form['path_field'] = array(
      '#type' => 'select',
      '#title' => t('File path fields'),
      '#options' => $options,
      '#multiple' => TRUE,
      '#size' => 4,
      '#default_value' => $this->options['path_field'],
      '#description' => t('Select the fields that will contain a file path to an mp3 file. If multiple fields are selected, the first one that contains a value will be used. This field will be hidden from view unless there are no other fields visible'),
      '#weight' => -5,
    );

    // Get the formatter settings into the options form.
    $field = 'jplayer_view_pseudo_field';
    $instance = array(
      'display' => array(
        'view' => array(
          'type' => 'jplayer_player',
          'settings' => $this->options,
        ),
      ),
    );

    $form += jplayer_field_formatter_settings_form($field, $instance, 'view', $form, $form_state);
  }

  // Ensure we have all the settings necessary to render into tabs.
  function validate() {
    $errors = parent::validate();

    // Ensure that we're using the field row style.
    if (!$this->row_plugin->uses_fields()) {
      $errors[] = t('Display "@display" uses the "@style" row style, but the jPlayer display style requires use of the "Fields" row style.', array('@display' => $this->display->display_title, '@style' => $this->row_plugin->definition['title']));
    }

    // Ensure that a file path field is selected.
    if (empty($this->options['path_field'])) {
      $errors[] = t('The jPlayer display style requires being configured so that at least one field is selected to provide a file path.');
    }

    return $errors;
  }

  // Override the render functionality.
  function render() {
    if (empty($this->row_plugin)) {
      vpr('views_plugin_style_default: Missing row plugin');
      return;
    }

    $view = $this->view;

    // Group the rows according to the grouping field, if specified.
    $sets = $this->render_grouping($this->view->result, $this->options['grouping']);
    $fields = $this->display->handler->get_handlers('field');

    $items = array();

    foreach ($sets as $title => $records) {
      foreach ($records as $row_index => $row) {
        $filepath = NULL;
        foreach ($this->options['path_field'] as $field_name) {
          if (isset($row->{$fields[$field_name]->field_alias})) {
            $filepath = $row->{$fields[$field_name]->field_alias};
            break;
          }
        }

        $this->view->row_index = $row_index;
        $label = trim(strip_tags($this->row_plugin->render($row)));
        if (empty($label)) {
          $label = basename($filepath);
        }

        $items[] = array(
          'url' => file_create_url($filepath),
          'label' => $label,
        );
      }
    }

    $output = theme('jplayer_view_playlist', array('view' => $view, 'items' => $items));

    // If doing a live preview, add the JavaScript directly to the output.
    if (isset($view->live_preview) && $view->live_preview) {
      $js = drupal_add_js(JS_DEFAULT);
      $settings = array();
      foreach ($js['setting'] as $js_settings) {
        if (isset($js_settings['jPlayer'])) {
          $settings = $js_settings['jPlayer'];
          break;
        }
      }

      $output .= "<script type=\"text/javascript\">\n";
      $output .= "Drupal.settings.jPlayer = Drupal.settings.jPlayer || {};\n";
      $output .= "jQuery.extend(Drupal.settings.jPlayer, " . drupal_json_encode($settings) . ");\n";
      $output .= "</script>\n";
    }

    unset($view->row_index);
    return $output;
  }

  // Because we don't provide templates, but Views may complain when visiting
  // the theme information if it can't find any. Specify the name of our theme
  // function to keep it from causing any errors.
  /**
   * @todo Please document this function.
   * @see http://drupal.org/node/1354
   */
  function theme_functions() {
    // TODO: Should this theme functions be declared in hook_theme()?
    return array('jplayer');
  }
}

