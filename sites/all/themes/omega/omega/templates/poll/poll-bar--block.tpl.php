<?php

/**
 * @file
 * Default theme implementation to display the bar for a single choice in a
 * poll.
 *
 * Variables available:
 * - $title: The title of the poll.
 * - $votes: The number of votes for this choice
 * - $total_votes: The number of votes for this choice
 * - $percentage: The percentage of votes for this choice.
 * - $vote: The choice number of the current user's vote.
 * - $voted: Set to TRUE if the user voted for this choice.
 *
 * @see template_preprocess_poll_bar()
 */
?>
<div class="poll-bars">
  <div class="poll-bars__text"><?php print $title; ?></div>
  <div class="poll-bars__bar">
    <div style="width: <?php print $percentage; ?>%;" class="foreground"></div>
  </div>
  <div class="poll-bars__percent">
    <?php print $percentage; ?>%
  </div>
</div>
