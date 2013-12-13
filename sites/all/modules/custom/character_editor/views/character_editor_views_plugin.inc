<?php

/**
 * Extending the view_plugin_style class to provide a slickgrid style.
 */
class character_editor_views_plugin extends slickgrid_views_plugin{

  function options_form(&$form, &$form_state){
    parent::options_form($form, $form_state);
    // Character editor cannot handle grouping so remove it from the form
    unset($form['grouping']);
    unset($form['editing']['multi_edit']);
    unset($form['editing']['row_selection_checkbox']);
    unset($form['editing']['export_selected_rows']);
    // Prevent some of the controls in the character editot
    // TODO - Do we want people to be able to delete & clone
    // TODO - Can we use built in undo as revisions aren't available?
    foreach(array(
      'delete',
      'clone',
      'undo'
    ) as $control){
      unset($form['editing'][$control]);
    }
  }

  /**
   * Pre-render is used to:
   * - Assign the values of the characters ;
   * - Order the taxon/specimens in a tree ;
   * - Filter out collapsed values.
   */
  function pre_render(&$result){
    $collapsed = array();
    if(isset($_GET['character_entity_field'])){
      $collapsed = explode(',', $_GET['character_entity_field']);
    }
    $ids = array();
    foreach($result as $row){
      $id = $row->id;
      // Assign character values
      if(isset($this->view->characters[$id]) && is_array($this->view->characters[$id])){
        foreach($this->view->characters[$id] as $field => $value){
          $row->{$field} = $value;
        }
      }
      $ids[$id] = $row;
    }
    // Build the tree
    $tree = array();
    foreach($ids as $id => $row){
      $list = array();
      $pid = $id;
      while($pid && !preg_match('/:0$/', $pid)){
        if(isset($ids[$pid])){
          $list[] = $pid;
          $pid = $ids[$pid]->parent;
        }else{
          $pid = 0;
        }
      }
      $list = array_reverse($list);
      $ptr = & $tree;
      foreach($list as $pid){
        if(!isset($ptr[$pid])){
          $ptr[$pid] = array(
            'value' => $ids[$pid],
            'children' => array(),
            'collapsed' => in_array($pid, $collapsed)
          );
        }
        $ptr = & $ptr[$pid]['children'];
      }
      unset($ptr);
    }
    // And order the items
    $result = $this->assign_tree_values($tree, 0);
  }

  private function assign_tree_values($tree, $depth){
    $result = array();
    foreach($tree as $item){
      $item['value']->depth = $depth;
      $item['value']->children = count($item['children']);
      $item['value']->collapsed = $item['collapsed'];
      $result[] = $item['value'];
      if(!$item['collapsed']){
        $result = array_merge($result, $this->assign_tree_values($item['children'], $depth + 1));
      }
    }
    return $result;
  }
}
?>