<?php

/**
 * @file
 * Default theme implementation to display a region.
 *
 * Available variables:
 * - $content: The content for this region, typically blocks.
 * - $classes: String of classes that can be used to style contextually through
 *   CSS. It can be manipulated through the variable $classes_array from
 *   preprocess functions. The default values can be one or more of the following:
 *   - region: The current template type, i.e., "theming hook".
 *   - region-[name]: The name of the region with underscores replaced with
 *     dashes. For example, the page_top region would have a region-page-top class.
 * - $region: The name of the region variable as defined in the theme's .info file.
 *
 * Helper variables:
 * - $classes_array: Array of html class attribute values. It is flattened
 *   into a string within the variable $classes.
 * - $is_admin: Flags true when the current user is an administrator.
 * - $is_front: Flags true when presented in the front page.
 * - $logged_in: Flags true when the current user is a logged-in member.
 *
 * @see template_preprocess()
 * @see template_preprocess_region()
 * @see template_process()
 * @see template_process_region()
 */
?>
<?php if (isset($content)): ?>
  <?php $tag = $title ? 'section' : 'div'; ?>
  <<?php print $tag; ?> class="<?php print $classes; ?>" <?php print $attributes; ?>>
    <?php if($title): ?>
      <?php print render($title_prefix); ?>
        <h1 <?php print $title_attributes; ?>><?php print $title; ?></h1>
      <?php print render($title_suffix); ?>
    <?php endif; ?>
    <?php if (isset($tabs) && count($tabs) > 0): ?>
      <div id="content-tabs" class=""><?php print render($tabs); ?></div><!-- /#content-tabs -->
    <?php endif; ?>
    <?php if(isset($content)): ?>
      <?php print $content; ?>
    <?php endif; ?>
  </<?php print $tag; ?>>
<?php endif; ?>
