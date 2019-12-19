<?php

/**
 * @file
 * Displays an appropriate icon for a forum post.
 *
 * Available variables:
 * - $new_posts: Indicates whether or not the topic contains new posts.
 * - $icon_class: The icon to display. May be one of 'hot', 'hot-new', 'new',
 *   'default', 'closed', or 'sticky'.
 * - $first_new: Indicates whether this is the first topic with new posts.
 *
 * @see template_preprocess_forum_icon()
 * @see theme_forum_icon()
 *
 * @ingroup themeable
 */
?>
<div class="forum-icon forum-icon--status-<?php print $icon_class; ?>" title="<?php print $icon_title; ?>">
  <?php print $icon_title; ?>
</div>
