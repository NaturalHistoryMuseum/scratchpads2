<?php
function character_editor_theme_slickgrid_controls($variables){
  $output = theme_slickgrid_controls($variables);
  if ($variables['view']->name == 'character_editor'){
    $img = url(drupal_get_path('module', 'character_editor') . '/images/refresh.png');
    $label = t('Refresh inherited values');
    $id = arg(1);
    $output .= '<div class="slickgrid-control slickgrid-refresh-inheritance">'
      . l("<img src='$img' /><span class='slickgrid-control-button enabled'>$label</span>", "character-editor-project/$id/apply-inheritance", array(
      'html' => TRUE
    )) . '</div>';
  }
  return $output;
}


