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
}

class relation_handler_join extends views_join {
  /**
   * Build the SQL for the join this object represents.
   */
  function build_join($select_query, $table, $view_query) {
    $field = field_info_field('endpoints');
    $relation_data_table_name = _field_sql_storage_tablename($field);
    $entity_id_field_name = _field_sql_storage_columnname('endpoints', 'entity_id');
    $entity_type_field_name = _field_sql_storage_columnname('endpoints', 'entity_type');
    $r_index_field_name = _field_sql_storage_columnname('endpoints', 'r_index');
    // Join the left table with the entity type to the endpoints field data table.
    $left = $view_query->get_table_info($this->left_table);
    $entity_type_left = $this->definition['entity_type_left'];
    $conditions = "$left[alias].$this->left_field = %alias.$entity_id_field_name AND %alias.$entity_type_field_name = '$entity_type_left'";
    if ($this->definition['directional'] && $this->options['r_index'] > -1) {
      $conditions .= " AND %alias.$r_index_field_name = " . $this->options['r_index'];
    }
    // Left join alias.
    $l = $select_query->addJoin($this->type, $relation_data_table_name, NULL, $conditions);

    // Execute a self-join.
    $entity_type_right = $this->definition['entity_type_right'];
    // entity_id here is the ID of the relation entity.
    $relation_type = $this->definition['relation_type'];
    $conditions = "%alias.entity_id = $l.entity_id AND %alias.$r_index_field_name != $l.$r_index_field_name AND %alias.$entity_type_field_name = '$entity_type_right' AND %alias.bundle = '$relation_type'";
    // Right join alias.
    $r = $select_query->addJoin($this->type, $relation_data_table_name, NULL, $conditions);

    // Join the right table to the relation_data table.
    $conditions = "%alias.$this->field = $r.$entity_id_field_name";
    $select_query->addJoin($this->type, $table['table'], $table['alias'], $conditions);
  }
}

class relation_handler_base_left_join extends views_join {
  /**
   * Build the SQL for the join this object represents.
   */
  function build_join($select_query, $table, $view_query) {
    $field = field_info_field('endpoints');
    $relation_data_table_name = _field_sql_storage_tablename($field);
    $entity_id_field_name = _field_sql_storage_columnname('endpoints', 'entity_id');
    $entity_type_field_name = _field_sql_storage_columnname('endpoints', 'entity_type');
    $r_index_field_name = _field_sql_storage_columnname('endpoints', 'r_index');
    // Join the left table with the entity type to the relation table.
    $left = $view_query->get_table_info($this->left_table);
    $entity_type_left = $this->definition['entity_type_left'];
    $conditions = "$left[alias].$this->left_field = %alias.$entity_id_field_name AND %alias.$entity_type_field_name = '$entity_type_left' AND %alias.$r_index_field_name = 0";
    // Left join alias.
    $l = $select_query->addJoin($this->type, $relation_data_table_name, NULL, $conditions);
    // entity_id here is the ID of the relation entity.
    $relation_type = $this->definition['relation_type'];
    $conditions = "%alias.rid = $l.entity_id AND %alias.relation_type = '$relation_type'";
    $select_query->addJoin($this->type, $table['table'], $table['alias'], $conditions);
  }
}

class relation_handler_base_right_join extends views_join {
  /**
   * Build the SQL for the join this object represents.
   */
  function build_join($select_query, $table, $view_query) {
    $field = field_info_field('endpoints');
    $relation_table = $view_query->get_table_info($this->left_table);
    $relation_data_table_name = _field_sql_storage_tablename($field);
    $entity_id_field_name = _field_sql_storage_columnname('endpoints', 'entity_id');
    $entity_type_field_name = _field_sql_storage_columnname('endpoints', 'entity_type');
    $r_index_field_name = _field_sql_storage_columnname('endpoints', 'r_index');
    $conditions = "$relation_table[alias].rid = %alias.entity_id AND %alias.$r_index_field_name = 1";
    $l = $select_query->addJoin($this->type, $relation_data_table_name, NULL, $conditions);
    $entity_type_right = $this->definition['entity_type_right'];
    $conditions = "$l.$entity_type_field_name = '$entity_type_right' AND %alias.$this->field = $l.$entity_id_field_name";
    $select_query->addJoin($this->type, $table['table'], $table['alias'], $conditions);
  }
}
