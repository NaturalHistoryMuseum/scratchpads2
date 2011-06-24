<?php
/**
 * @file
 * Provides an inline cell editor
 */
// Plugin definition
$plugin = array(
  'title' => t('Collapsible taxonomy formatter'),
  'description' => t('Tree formatter.'),
  'js' => array(
    'file' => 'slickgrid.formatters.js'
  ),
  'hooks' => array(
    'views_pre_view' => 'slickgrid_plugin_collapsible_formatter_views_pre_view'
  ),
  'field_types' => array(
    'taxonomy_term_reference_link',
  	'taxonomy_term_reference_plain'
  )
);

function slickgrid_plugin_collapsible_formatter_views_pre_view(&$view, $field_id, $display_id){
  // We need to know what vocabulary this field uses
  $field = $view->get_item($display_id, 'field', $field_id);
  $field_info = (field_info_field($field['field']));
  $vocabulary = taxonomy_vocabulary_machine_name_load($field_info['settings']['allowed_values'][0]['vocabulary']);
  // Add the relationships so we know the parent tid
  $term_node_relationship = array(
    'required' => 0,
    'id' => 'slickgrid_term_node_relationship',
    'table' => 'node',
    'field' => 'term_node_tid',
    'relationship' => 'none',
    'vocabularies' => array(
      $vocabulary->machine_name => $vocabulary->machine_name
    )
  );
  $view->set_item($display_id, 'relationship', 'slickgrid_term_node_relationship', $term_node_relationship);
  $parent_relationship = array(
    'required' => 0,
    'id' => 'slickgrid_parent_relationship',
    'table' => 'taxonomy_term_hierarchy',
    'relationship' => 'slickgrid_term_node_relationship',
    'field' => 'parent'
  );
  $view->set_item($display_id, 'relationship', 'slickgrid_parent_relationship', $parent_relationship);
  // Add the parent term
  $parent_tid_field = array(
    'exclude' => 1,
    'label' => t('Slickgrid parent'),
    'id' => 'slickgrid_parent_tid',
    'table' => 'taxonomy_term_data',
    'field' => 'tid',
    'relationship' => 'slickgrid_parent_relationship'
  );
  $view->set_item($display_id, 'field', 'slickgrid_parent_tid', $parent_tid_field);
  // Ensure the tid of the actual term is available but hidden from the view
  $tid_field = array(
    'exclude' => 1,
    'id' => 'slickgrid_tid',
    'table' => 'taxonomy_term_data',
    'field' => 'tid',
    'relationship' => 'slickgrid_term_node_relationship'
  );
  $view->set_item($display_id, 'field', 'slickgrid_tid', $tid_field);
  // The taxonomy sorts need to come first - so remove the exisitng sorts & re-add them after my own 
  $existing_sorts = $view->get_items('sort');
  // Unset all the existing sorts
  foreach(array_keys($existing_sorts) as $existing_sort_id){
    $view->set_item($display_id, 'sort', $existing_sort_id, null);
  }
  // Add two sorts to the view - one for the parent term, one for the child
  foreach(array(
    'slickgrid_parent_relationship',
    'slickgrid_term_node_relationship'
  ) as $relationship){
    $id = 'slickgrid_sort_' . $relationship;
    $sort = array(
      'order' => 'ASC',
      'id' => $relationship,
      'table' => 'taxonomy_term_data',
      'field' => 'weight',
      'relationship' => $relationship
    );
    $view->set_item($display_id, 'sort', $id, $sort);
  }
  // Re add the original sorts
  foreach($existing_sorts as $existing_sort_id => $existing_sort){
    $view->set_item($display_id, 'sort', $existing_sort_id, $existing_sort);
  }
}