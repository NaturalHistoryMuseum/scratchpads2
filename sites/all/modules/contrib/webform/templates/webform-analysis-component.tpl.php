<?php
/**
 * @file
 * Template for rendering an individual component's analysis data.
 *
 * Available variables:
 * - $component: The component whose analysis is being rendered.
 * - $component_analysis: A renderable containing this components analysis.
 * - $data: An array of array containing the analysis data. Contains the keys:
 *   - table_header: If this table has more than a single column, an array
 *     of header labels.
 *   - table_rows: If this component has a table that should be rendered, an
 *     array of values
 */
?>
<div class="<?php print $classes; ?>">
  <div class="webform-analysis-component-inner">
    <h3><?php print check_plain($component['name']); ?></h3>
    <?php print drupal_render_children($component_analysis); ?>
  </div>
</div>
