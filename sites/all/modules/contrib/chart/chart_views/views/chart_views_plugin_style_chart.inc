<?php
/**
 * @file
 * Contains the chart display plugin.
 *
 * @author Jimmy Berry ("boombatower", http://drupal.org/user/214218)
 */

/**
 * The plugin that handles the chart style.
 *
 * @ingroup views_style_plugins
 */
class chart_views_plugin_style_chart extends views_plugin_style {
  function option_definition () {
    $options = parent::option_definition();

    $options['type'] = array('default' => 'bhs');
    $options['width'] = array('default' => 600);
    $options['height'] = array('default' => 400);
    $options['color_scheme'] = array('default' => 'default');
    $options['label_append_value'] = array('default' => FALSE);

    return $options;
  }

  function options_form(&$form, &$form_state) {
    parent::options_form($form, $form_state);

    $form['type'] = array(
      '#type' => 'radios',
      '#title' => t('Type'),
      '#description' => t('Chart type, see <a href="http://code.google.com/apis/chart/">Google Chart API documentation</a>.'),
      '#options' => chart_types(),
      '#required' => TRUE,
      '#default_value' => $this->options['type'],
    );
    $form['width'] = array(
      '#type' => 'textfield',
      '#title' => t('Width'),
      '#description' => t('Chart width in pixels'),
      '#size' => 8,
      '#required' => TRUE,
      '#default_value' => $this->options['width'],
    );
    $form['height'] = array(
      '#type' => 'textfield',
      '#title' => t('Height'),
      '#description' => t('Chart height in pixels'),
      '#size' => 8,
      '#required' => TRUE,
      '#default_value' => $this->options['height'],
    );
    $form['color_scheme'] = array(
      '#type' => 'select',
      '#title' => t('Color scheme'),
      '#description' => t('Color scheme, as defined by hook_chart_color_schemes().'),
      '#options' => drupal_map_assoc(array_keys(chart_color_schemes())),
      '#required' => TRUE,
      '#default_value' => $this->options['color_scheme'],
    );
    $form['label_append_value'] = array(
      '#type' => 'checkbox',
      '#title' => t('Append value to label'),
      '#description' => t('If checked, calculated values will be appended in the chart legend.'),
      '#default_value' => $this->options['label_append_value'],
    );
  }

  function render() {
    $charts = array();
    $sets = $this->render_grouping($this->view->result, $this->options['grouping']);
    $one = count($sets) == 1;
    $set = 0;
    foreach ($sets as $title => $records) {
      $chart = array(
        '#theme' => 'chart',
        '#chart_id' => drupal_clean_css_identifier($this->view->name . '-' . $this->display->id . '-' . $set++),
        '#type' => $this->options['type'],
        // If only one set then use the view title, otherwise if the set has a
        // title then set it as the chart title.
        '#title' => $one ? $this->view->human_name : $title,
        '#size' => array(
          '#width' => $this->options['width'],
          '#height' => $this->options['height'],
        ),
        '#adjust_resolution' => TRUE,
        '#data' => array(),
        '#labels' => array(),
        '#data_colors' => array(),
      );

      foreach ($records as $row_index => $row) {
        foreach ($this->view->field as $key => $field) {
          if (!$field->options['exclude']) {
            $chart['#data'][$field->field_alias][] = (int) $row->{$field->field_alias};
            $chart['#labels'][] = $field->options['label'] . ($this->options['label_append_value'] ? ': ' . $row->{$field->field_alias} : ''); // @TODO Provide a way to change format.
            $chart['#data_colors'][$field->field_alias] = chart_unique_color($field->field_alias, $this->options['color_scheme']);
          }
        }
      }

      // Allow modules to alter the chart based on views context.
      drupal_alter('chart_views', $chart, $this->view->name, $this->display->id);

      // Since view expects string output we can't save render array for later.
      $charts[$chart['#chart_id']] = drupal_render($chart);
    }

    return implode($charts);
  }
}
