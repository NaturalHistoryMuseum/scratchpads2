<?php

/**
 * @file
 * Views Slideshow: Template for text control - next.
 *
 * - $variables: Contains theme variables.
 * - $classes: Text control classes.
 * - $rendered_control_previous: Rendered control - previous.
 * - $rendered_control_pause: Rendered control - pause.
 * - $rendered_control_next: Rendered control - next.
 *
 * @ingroup vss_templates
 */
?>
<div id="views_slideshow_controls_text_<?php print $variables['vss_id']; ?>" class="<?php print $classes; ?>">
  <?php print $rendered_control_previous; ?>
  <?php print $rendered_control_pause; ?>
  <?php print $rendered_control_next; ?>
</div>
