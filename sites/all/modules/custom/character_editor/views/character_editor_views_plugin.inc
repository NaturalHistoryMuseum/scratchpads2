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
    $result = $this->view->project_model->getRowValues($result);
  }
}
?>