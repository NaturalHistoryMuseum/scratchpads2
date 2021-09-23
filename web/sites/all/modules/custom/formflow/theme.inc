<?php

function theme_formflow_wizard_trail(&$vars){
  ctools_include('wizard');
  $element = $vars['element'];
  return theme('ctools_wizard_trail', array(
    'trail' => $element['#value']
  ));
}

/**
 * Returns HTML for an element's children fieldsets as a formflow
 *
 * @param $variables
 * An associative array containing:
 * - element: An associative array containing the properties and children of the
 * fieldset. Properties used: #children.
 *
 * @ingroup themeable
 */
function theme_formflow_group($variables){
	$output = '';
  $element = $variables['element'];
  $output .= '<div class="formflow-group">' . $element['#children'] . '</div>';
  return $output;
}