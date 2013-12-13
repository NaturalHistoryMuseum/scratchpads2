<?php

/**
 * Character editor views field handler
 *
 * We need a view that returns two different entity types: terms and nodes.
 * It is possible to create a view that returns the same field from two
 * different entity types, by using hook_views_data to declare that field's
 * table as a base table ; however it is not possible to do this with
 * base properties such as name/title as they do not share a common table.
 *
 * We implement this here by using a custom field that causes the query to
 * be re-written, returning values for a single field that combine both
 * taxonomy_term and node entities. The custom field can then take care of
 * rendering the item as required depending on it's entity type.
 * 
 */
class character_editor_views_handler_entity_field extends views_handler_field{

  function option_definition(){
    $options = parent::option_definition();
    $options['type'] = array('default' => 'character_entity_field');
    return $options;
  }
  
  function query(){
    if (empty($this->view->args[0])){return;}
    $base_field = $this->view->base_field;
    $character_project = $this->view->args[0];
    // We cannot use the views query system to build our query, as it requires unions,
    // so we use drupal queries instead.
    // Create a query to select the projects' terms.
    $q1 = db_select('field_data_field_taxonomic_name', 'f');
    $q1->condition('f.entity_type', 'character_editor_project');
    $q1->condition('f.entity_id', $character_project);
    $q1->addJoin('INNER', 'taxonomy_term_data', 't', 't.tid = f.field_taxonomic_name_tid');
    $q1->addJoin('LEFT', 'taxonomy_term_hierarchy', 'h', 'h.tid = t.tid');
    $q1->addExpression('CONCAT(\'taxonomy_term:\', t.tid)', 'id');
    $q1->addField('t', 'name', 'name');
    $q1->addExpression('CONCAT(\'taxonomy_term:\', h.parent)', 'parent');
    $q1->addExpression('0', 'depth');
    if ($base_field != 'id'){
      $q1->addExpression('CONCAT(\'taxonomy_term:\', t.tid)', $base_field);
    }
    // Create a query to select the project's specimens
    $q2 = db_select('field_data_field_char_proj_specimen', 'f');
    $q2->condition('f.entity_type', 'character_editor_project');
    $q2->condition('f.entity_id', $character_project);
    $q2->addJoin('INNER', 'node', 'n', 'n.nid = f.field_char_proj_specimen_nid');
    $q2->addJoin('LEFT', 'field_data_field_taxonomic_name', 'tn', 'tn.entity_id = n.nid');
    $q2->condition('tn.entity_type', 'node');
    $q2->addExpression('CONCAT(\'node:\', nid)', 'id');
    $q2->addField('n', 'title', 'name');
    $q2->addExpression('CONCAT(\'taxonomy_term:\', tn.field_taxonomic_name_tid)', 'parent');
    $q2->addExpression('0', 'depth');
    if ($base_field != 'id'){
      $q2->addExpression('CONCAT(\'node:\', nid)', $base_field);
    }
    // And merge the two.
    $merged = $q1->union($q2, 'UNION ALL');
    // We cannot set the final query here - this will ensure it gets set during
    // hook_views_pre_execute.
    $this->view->character_editor_query = $merged;
  }

  function render($values){
    $attributes = array(
      'depth' => $values->depth,
      'treestate' => ''
    );
    if ($values->children > 0){
      $attributes['treestate'] = $values->collapsed ? 'collapsed' : 'expanded';
    }
    $name = '';
    if (preg_match('/^taxonomy_term:/', $values->id)){
      $name = l($values->name, preg_replace('/^taxonomy_term:/', 'term/', $values->id));
      $name = "<strong>$name</strong>";
    } else {
      $name = l($values->name, preg_replace('/^node:/', 'node/', $values->id));
    }
    $result = '<span';
    foreach ($attributes as $key => $attr){
      $result .= " $key=\"$attr\"";
    }
    $result .= $name . '</span>';
    return $result;
  }
}