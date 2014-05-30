<?php

/**
 * character_editor_theme_slickgrid_controls
 */
function character_editor_theme_slickgrid_controls($variables){
  $output = theme_slickgrid_controls($variables);
  if($variables['view']->name == 'character_editor'){
    $style_options = $variables['view']->display_handler->get_option('style_options');
    if(!empty($style_options['editable'])){
      $img = url(drupal_get_path('module', 'character_editor') . '/images/refresh.png');
      $label = t('Recalculate');
      $id = arg(1);
      $output .= '<div class="slickgrid-control slickgrid-refresh-inheritance">' . l("<img src='$img' /><span class='slickgrid-control-button enabled'>$label</span>", "character-editor-project/$id/apply-inheritance", array(
        'html' => TRUE
      )) . '</div>';
    }
  }
  return $output;
}

/**
 * theme_character_editor_description_row
 *
 * Given a character project id and a taxon id, theme a row representing that taxon
 * within the character project.
 */
function theme_character_editor_description_row($variables){
  if(empty($variables['project_id']) || empty($variables['tid'])){return '';}
  $project_model = new CharacterProjectModel('character_editor_project', $variables['project_id']);
  // Get the character tree
  $tree = $project_model->getTree();
  $flags = $project_model->getFlags();
  // Get the row values
  $values = $project_model->getRowValues(array(
    (object)array(
      'id' => 'taxonomy_term:' . $variables['tid']
    )
  ));
  if(empty($values)){return '';}
  $taxon_values = reset($values);
  // Build the table
  $header = array();
  $row = array();
  foreach($tree as $tree_item){
    $tree_item_entity = character_editor_wrapper($tree_item->id());
    if($tree_item_entity->type->value() == 'group'){
      continue;
    }
    $field = $tree_item->columnId();
    if(property_exists($taxon_values, $field)){
      $data = (array)json_decode($taxon_values->{$field});
      if(!empty($data['disabled'])){
        continue;
      }else{
        $cell = '<div class="character-matrix-row-cell">';
        $title = '';
        if(isset($data['metadata']->flag) && $data['metadata']->flag){
          $cell .= '<div class="character-matrix-row-flag">' . t($flags[$data['metadata']->flag]['flag']) . '</div>';
          $title = t($flags[$data['metadata']->flag]['flag']);
        }else{
          $cell .= '<div class="character-matrix-row-flag">&nbsp;</div>';
        }
        if(!empty($data['data'])){
          $cell .= '<div class="character-matrix-row-data">' . htmlspecialchars($data['data']) . '</div>';
        }
        $cell .= '</div>';
        $row[] = array(
          'data' => $cell,
          'title' => $title
        );
      }
    }else{
      $row[] = '';
    }
    $header[] = $tree_item->model()->label();
  }
  $table = theme('table', array(
    'header' => $header,
    'rows' => array(
      $row
    ),
    'sticky' => FALSE
  ));
  $term = taxonomy_term_load($variables['tid']);
  $link = l($project_model->label(), 'character-editor-project/' . $variables['project_id'], array(
    'attributes' => array(
      'title' => t('View in matrix editor')
    ),
    'query' => array(
      'character_entity_field' => $term->name
    )
  ));
  return "<div class='character-matrix-row'><h3>" . $link . "</h3>" . "<div class='character-matrix-table'>" . $table . '</div></div>';
}
