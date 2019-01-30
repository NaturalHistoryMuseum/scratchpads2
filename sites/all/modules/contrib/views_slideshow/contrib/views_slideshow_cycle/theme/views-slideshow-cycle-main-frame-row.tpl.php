<?php

/**
 * @file
 * Views Slideshow cycle: Main frame row.
 *
 * - $variables: Contains theme variables.
 * - $classes: Classes.
 * - $rendered_items: Rendered items.
 *
 * @ingroup vss_templates
 */
?>
<div id="views_slideshow_cycle_div_<?php print $variables['vss_id']; ?>_<?php print $variables['count']; ?>" class="<?php print $classes; ?>" <?php print $aria; ?>>
  <?php print $rendered_items; ?>
</div>
