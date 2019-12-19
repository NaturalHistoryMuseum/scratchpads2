<?php

/**
 * @file
 * Default theme implementation to display the poll results in a block.
 *
 * Variables available:
 * - $title: The title of the poll.
 * - $results: The results of the poll.
 * - $votes: The total results in the poll.
 * - $links: Render array of the links for this poll.
 * - $nid: The nid of the poll
 * - $cancel_form: A form to cancel the user's vote, if allowed.
 * - $raw_links: The raw array of links. Should be run through theme('links')
 *   if used.
 * - $vote: The choice number of the current user's vote.
 *
 * @see template_preprocess_poll_results()
 */
?>
<div class="poll-results poll-results--block">
  <h4><?php print $title ?></h4>
  <?php print $results ?>
  <div class="poll-results__total">
    <?php print t('Total votes: @votes', array('@votes' => $votes)); ?>
  </div>
</div>
<?php print render($links); ?>
