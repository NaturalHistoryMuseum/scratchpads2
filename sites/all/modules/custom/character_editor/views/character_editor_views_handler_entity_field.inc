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
    $options['type'] = array(
      'default' => 'character_entity_field'
    );
    return $options;
  }

  function query(){
    if(empty($this->view->args[0])){return;}
    $filter = !empty($_GET['character_entity_field']) ? $_GET['character_entity_field'] : '';
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
    if($base_field != 'id'){
      $q1->addExpression('CONCAT(\'taxonomy_term:\', t.tid)', $base_field);
    }
    if($filter){
      $q1->condition('t.name', $filter . '%', 'LIKE');
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
    if($base_field != 'id'){
      $q2->addExpression('CONCAT(\'node:\', nid)', $base_field);
    }
    if($filter){
      $q2->condition('n.title', $filter . '%', 'LIKE');
    }
    // And merge the two.
    $merged = $q1->union($q2, 'UNION ALL');
    // Prepare the count query. With unions we can't rely on countQuery.
    $count_q1 = db_select("field_data_field_taxonomic_name", "ft");
    $count_q1->addField('ft', 'field_taxonomic_name_tid', 'tid');
    $count_q1->condition('ft.entity_type', 'character_editor_project');
    $count_q1->condition('ft.entity_id', $character_project);
    if($filter){
      $count_q1->addJoin('INNER', 'taxonomy_term_data', 't', 't.tid = ft.field_taxonomic_name_tid');
      $count_q1->condition('t.name', $filter . '%', 'LIKE');
    }
    $count_q2 = db_select("field_data_field_char_proj_specimen", "fs");
    $count_q2->addField('fs', 'field_char_proj_specimen_nid', 'nid');
    $count_q2->condition('fs.entity_type', 'character_editor_project');
    $count_q2->condition('fs.entity_id', $character_project);
    if($filter){
      $count_q2->addJoin('INNER', 'node', 'n', 'n.nid = fs.field_char_proj_specimen_nid');
      $count_q2->condition('n.title', $filter . '%', 'LIKE');
    }
    $count_query = db_select($count_q1->union($count_q2, 'UNION ALL'));
    $count_query->addExpression('COUNT(*)');
    // We cannot set the final query here - this will ensure it gets set during
    // hook_views_pre_execute.
    $str = (string)$merged;
    $this->view->character_editor_query = $merged;
    $this->view->character_editor_count_query = $count_query;
  }

  function render($values){
    $attributes = array(
      'depth' => $values->depth,
      'treestate' => ''
    );
    if($values->children > 0){
      $attributes['treestate'] = $values->collapsed ? 'collapsed' : 'expanded';
    }
    $name = '';
    if(preg_match('/^taxonomy_term:/', $values->id)){
      $name = l($values->name, preg_replace('/^taxonomy_term:/', 'taxonomy/term/', $values->id));
      $name = "<strong>$name</strong>";
      $parents = taxonomy_get_parents_all(str_replace('taxonomy_term:', '', $values->id));
      $parents = array_reverse($parents);
      array_pop($parents);
      if(count($parents)){
        foreach($parents as $key => $term){
          $parents[$key] = $term->name;
        }
        $attributes = array(
          'title' => implode(' &raquo; ', $parents)
        );
      }
    }else{
      $name = l($values->name, preg_replace('/^node:/', 'node/', $values->id));
      $parent = taxonomy_term_load(str_replace('taxonomy_term:', '', $values->parent));
      if($parent){
        $attributes = array(
          'title' => t('Specimen of @taxon_name', array(
            '@taxon_name' => $parent->name
          ))
        );
      }
    }
    $result = '<span';
    foreach($attributes as $key => $attr){
      $result .= " $key=\"$attr\"";
    }
    $result .= '>' . $name . '</span>';
    return $result;
  }
}
