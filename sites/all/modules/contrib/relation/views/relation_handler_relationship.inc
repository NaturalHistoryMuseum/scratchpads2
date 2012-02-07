<?php

/**
 * @file
 * Views relationship support.
 */

class relation_handler_relationship extends views_handler_relationship {
  /**
   * Define r_index option.
   */
  function option_definition() {
    $options = parent::option_definition();
    $options['r_index'] = array('default' => -1);
    return $options;
  }

  /**
   * Let the user choose r_index.
   */
  function options_form(&$form, &$form_state) {
    parent::options_form($form, $form_state);

    $options = $this->options_form_summary_options();
    if ($this->definition['directional']) {
      $form['r_index'] = array(
        '#type' => 'select',
        '#options' => $options,
        '#title' => t('Position of the relationship base'),
        '#default_value' => $this->options['r_index'],
        // check_plain()'d in the definition.
        '#description' => t('Select whether the entity you are adding the relationship to is source or target of !relation_type_label relation.', array('!relation_type_label' => $this->definition['label'])),
      );
    }
  }

  /**
   * Return the main options, which are shown in the summary title.
   */
  function options_form_summary_options() {
    return $this->definition['directional'] ? array(
      -1 => t('Any'),
      0 => t('Source'),
      1 => t('Target'),
    ) : array();
  }

  function query() {
    $field = field_info_field('endpoints');
    $relation_data_table_name = _field_sql_storage_tablename($field);
    $entity_id_field_name = _field_sql_storage_columnname('endpoints', 'entity_id');
    $entity_type_field_name = _field_sql_storage_columnname('endpoints', 'entity_type');
    $r_index_field_name = _field_sql_storage_columnname('endpoints', 'r_index');
    $join_type = empty($this->options['required']) ? 'LEFT' : 'INNER';
    $endpoints_twice = isset($this->definition['entity_type_left']) && isset($this->definition['entity_type_right']);

    $this->ensure_my_table();
    // Join the left table with the entity type to the endpoints field data table.
    $join = new views_join();
    $join->definition = array(
      'left_table' => $this->table_alias,
      'left_field' => $this->real_field,
      'table'      => $relation_data_table_name,
      'field'      => isset($this->definition['entity_type_left']) ? $entity_id_field_name : 'entity_id',
      'type'       => $join_type,
      'extra'      => array(
        array(
          'field' => 'bundle',
          'value' => $this->definition['relation_type'],
        ),
      ),
    );
    if (isset($this->definition['entity_type_left'])) {
      $join->definition['extra'][] = array(
        'field' => $entity_type_field_name,
        'value' => $this->definition['entity_type_left'],
      );
    }
    if ($this->definition['directional'] && $this->options['r_index'] > -1) {
      $join->definition['extra'][] = array(
        'field' => $r_index_field_name,
        'value' => $this->options['r_index'],
      );
    }
    $join->construct();
    $join->adjusted = TRUE;
    $l = $this->query->add_table($relation_data_table_name, $this->relationship, $join);

    if ($endpoints_twice) {
      // Execute a self-join.
      $join = new views_join();
      $join->definition = array(
        'left_table' => $l,
        'left_field' => 'entity_id',
        'table'      => $relation_data_table_name,
        'field'      => 'entity_id',
        'type'       => $join_type,
        'extra'      => array(
          array(
            'field' => $entity_type_field_name,
            'value' => $this->definition['entity_type_right'],
          ),
        ),
      );
      $join->construct();
      $join->adjusted = TRUE;
      $r = $this->query->add_table($relation_data_table_name, $this->relationship, $join);
    }
    else {
      $r = $l;
    }
    $join = new views_join();
    $join->definition = array(
      'left_table' => $r,
      'left_field' => isset($this->definition['entity_type_right']) ? $entity_id_field_name : 'entity_id',
      'table'      => $this->definition['base'],
      'field'      => $this->definition['base field'],
      'type'       => $join_type,
    );
    // There is no query where these conditions could be added earlier:
    // $r might be just $l.
    if ($endpoints_twice && $this->definition['entity_type_left'] == $this->definition['entity_type_right']) {
      $join->definition['extra'][] = array(
        // This definition is a bit funny but there's no other way to tell
        // Views to use this as it is.
        'table' => NULL,
        'field' => "$r.$r_index_field_name != $l.$r_index_field_name AND 1",
        'value' => 1,
      );
    }
    if (isset($this->definition['entity_type_right'])) {
      $join->definition['extra'][] = array(
        'table' => $r,
        'field' => $entity_type_field_name,
        'value' => $this->definition['entity_type_right'],
      );
    }

    $join->construct();
    $join->adjusted = TRUE;
    // use a short alias for this:
    $alias = $this->definition['base'] . '_' . $this->table;
    $this->alias = $this->query->add_relationship($alias, $join, $this->definition['base'], $this->relationship);
  }
}
