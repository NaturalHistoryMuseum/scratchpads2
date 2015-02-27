<?php

class weight_handler_field_weight extends views_handler_field {

  function init(&$view, &$options) {
    parent::init($view, $options);
    $this->additional_fields['nid'] = array('table' => 'node', 'field' => 'nid');
  }

  function render($values) {
    return '<!--form-item-' . $this->options['id'] . '--' . $this->view->row_index . '-->';
  }

  function views_form(&$form, &$form_state) {
    // The view is empty, abort.
    if (empty($this->view->result)) {
      return;
    }

    // Initialize $nids array to store nids for saving weights.
    $nids = array();

    // Get the weight field settings.
    $settings = _weight_get_settings();

    $field_name = $this->options['id'];
    $form[$field_name] = array(
      '#tree' => TRUE,
    );

    // At this point, the query has already been run, so we can access the results
    foreach ($this->view->result as $row_id => $row) {
      $type = $this->get_node_type($row->nid);
      if (!empty($row->weight_weights_weight) || $row->weight_weights_weight === "0") {
        $weight = $row->weight_weights_weight;
      }
      else {
        if (array_key_exists($type, $settings)) {
          $weight = $settings[$type]['default'];
        }
        else {
          $weight = 0;
        }
      }
      $nids[] = $row->nid;
      if (array_key_exists($type, $settings)) {
        $options = _weight_get_options($settings[$type]['range']);
      }
      else {
        $options = array();
      }

      $form[$field_name][$row_id][$row->nid] = array(
        '#type' => 'select',
        '#options' =>  $options,
        '#default_value' => $weight,
        '#attributes' => array('class' => array('weight-weight')),
        '#access' => user_access('assign node weight') && node_access('update', node_load($row->nid)),
      );
    }

    $form['nids'] = array(
      '#type' => 'value',
      '#value' => $nids,
    );

    $form['#submit'][] = 'weight_views_submit';
    $form['#action'] = request_uri();
  }

  function get_node_type($nid) {
    $type = db_query("SELECT type FROM {node} WHERE nid=:nid",
      array(':nid' => $nid))->fetchField();

    return $type;
  }
}
