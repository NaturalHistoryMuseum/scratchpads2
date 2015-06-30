<?php
/**
 * @file
 * Template for printing out the contents of the "Analysis" tab on a Webform.
 *
 * Available variables:
 * - $node: The node object for this webform.
 * - $component: If a single components analysis is being printed, this will
 *   contain a Webform component. Otherwise all components are having their
 *   analysis printed on the same page.
 * - $analysis: A renderable object containing the following children:
 *   - 'form': A form for selecting which components should be included in the
 *     analysis.
 *   - 'data': An render array of analysis results for each component enabled.
 */
?>
<div class="webform-analysis">
  <?php print drupal_render($analysis['form']['help']); ?>

  <div class="webform-analysis-data">
    <?php print drupal_render($analysis['data']); ?>
  </div>
  <?php print drupal_render($analysis['form']); ?>
  <?php /* Print out any remaining part of the renderable. */ ?>
  <?php print drupal_render_children($analysis); ?>
</div>
