<?php

/**
 * @file
 * Views Slideshow: Template for pager field - field.
 *
 * - $view: The view in use.
 * - $css_id: A field specific css identifier.
 * - $field: The field machine name.
 * - $field_item: The field being rendered.
 * - $field_rendered: The pre-rendered field.
 *
 * @ingroup vss_templates
 */
?>
<div class="views-field-<?php print $css_id; ?>">
  <?php if (is_object($field_item) && $field_item->label()) { ?>
    <label class="view-label-<?php print $css_id; ?>">
      <?php print $field_item->label(); ?>:
    </label>
  <?php } ?>
  <div class="views-content-<?php print $css_id; ?>">
    <?php print $field_rendered; ?>
  </div>
</div>
