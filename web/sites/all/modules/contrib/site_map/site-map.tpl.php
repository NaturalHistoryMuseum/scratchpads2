<?php
/**
 * @file
 * Theme implementation to display the site map.
 *
 * Available variables:
 * - $message:
 * - $rss_legend:
 * - $front_page:
 * - $blogs:
 * - $books:
 * - $menus:
 * - $faq:
 * - $taxonomys:
 * - $additional:
 *
 * @see template_preprocess()
 * @see template_preprocess_site_map()
 */
?>

<div id="site-map">
  <?php if($message): ?>
    <div class="site-map-message">
      <?php print $message; ?>
    </div>
  <?php endif; ?>

  <?php if($rss_legend): ?>
    <div class="site-map-rss-legend">
      <?php print $rss_legend; ?>
    </div>
  <?php endif; ?>

  <?php if($front_page): ?>
    <div class="site-map-front-page">
      <?php print $front_page; ?>
    </div>
  <?php endif; ?>

  <?php if($blogs): ?>
    <div class="site-map-blogs">
      <?php print $blogs; ?>
    </div>
  <?php endif; ?>

  <?php if($books): ?>
    <div class="site-map-books">
      <?php print $books; ?>
    </div>
  <?php endif; ?>

  <?php if($menus): ?>
    <div class="site-map-menus">
      <?php print $menus; ?>
    </div>
  <?php endif; ?>

  <?php if($faq): ?>
    <div class="site-map-faq">
      <?php print $faq; ?>
    </div>
  <?php endif; ?>

  <?php if($taxonomys): ?>
    <div class="site-map-taxonomys">
      <?php print $taxonomys; ?>
    </div>
  <?php endif; ?>

  <?php if($additional): ?>
    <div class="site-map-additional">
      <?php print $additional; ?>
    </div>
  <?php endif; ?>
</div>
