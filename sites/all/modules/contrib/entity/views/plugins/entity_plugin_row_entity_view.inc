<?php

/**
 * @file
 * Row style plugin for displaying the results as entities.
 */

/**
 * Plugin class for displaying Views results with entity_view.
 */
class entity_plugin_row_entity_view extends views_plugin_row {
  /**
   * The entity type of the entity being displayed.
   */
  protected $entity_type, $skip_load, $entities = array();

  public function init(&$view, &$display, $options = NULL) {
    parent::init($view, $display, $options);

    $table_data = views_fetch_data($this->view->base_table);
    $this->entity_type = $table_data['table']['entity type'];
    $this->skip_load = !empty($table_data['table']['skip entity load']);

    // Set base table and field information as used by views_plugin_row to
    // select the entity id if used with default query class.
    $info = entity_get_info($this->entity_type);
    if (!empty($info['base table']) && $info['base table'] == $this->view->base_table) {
      $this->base_table = $info['base table'];
      $this->base_field = $info['entity keys']['id'];
    }
  }

  public function option_definition() {
    $options = parent::option_definition();
    $options['view_mode'] = array('default' => 'full');
    return $options;
  }

  public function options_form(&$form, &$form_state) {
    parent::options_form($form, $form_state);

    $entity_info = entity_get_info($this->entity_type);
    $options = array();
    if (!empty($entity_info['view modes'])) {
      foreach ($entity_info['view modes'] as $mode => $settings) {
        $options[$mode] = $settings['label'];
      }
    }

    if (count($options) > 1) {
      $form['view_mode'] = array(
        '#type' => 'select',
        '#options' => $options,
        '#title' => t('View mode'),
        '#default_value' => $this->options['view_mode'],
      );
    }
    else {
      $form['view_mode_info'] = array(
        '#type' => 'item',
        '#title' => t('View mode'),
        '#description' => t('Only one view mode is available for this entity type.'),
        '#markup' => $options ? current($options) : t('Default'),
      );
      $form['view_mode'] = array(
        '#type' => 'value',
        '#value' => $options ? key($options) : 'default',
      );
    }

    return $form;
  }

  /**
   * Use entity_load() to load all entities at once if they aren't loaded yet.
   */
  public function pre_render($results) {
    $this->entities = array();
    foreach ($results as $result) {
      if (!isset($result->entity)) {
        $id = entity_id($this->entity_type, $result);
        $this->entities[$id] = $result;
      }
      else {
        $id = (is_object($result->entity) ? entity_id($this->entity_type, $result->entity) : $result->entity);
        $this->entities[$id] = $result->entity;
      }
      // Force loading in case there is no object at all.
      if (!empty($id) && $id == $this->entities[$id]) {
        $this->skip_load = FALSE;
      }
    }
    if (!$this->skip_load) {
      $this->entities = entity_load($this->entity_type, array_keys($this->entities));
    }
  }

  public function render($result) {
    if (!isset($result->entity)) {
      $id = entity_id($this->entity_type, $result);
    }
    else {
      $id = (is_object($result->entity) ? entity_id($this->entity_type, $result->entity) : $result->entity);
    }
    if (isset($id)) {
      $content = entity_view($this->entity_type, array($id => $this->entities[$id]), $this->options['view_mode']);
      return drupal_render($content);
    }
  }
}
