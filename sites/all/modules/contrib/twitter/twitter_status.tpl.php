<?php
/**
 * @file
 * Renders a tweet as it does look like at Twitter.com.
 * @see twitter.css
 */
?>
<div class="twitter-status clearfix">
  <div class="avatar">
    <a href="https://twitter.com/<?php print $author->screen_name; ?>" title="<?php print $author->name; ?>">
      <img src="<?php print $author->profile_image_url; ?>" alt="<?php print $author->name; ?>" />
    </a>
  </div>

  <div class="timestamp">
    <?php print $status->time_ago; ?>
  </div>

  <div class="name-handle">
    <div class="name">
      <a href="https://twitter.com/<?php print $author->screen_name; ?>"><?php print $author->name; ?></a>
    </div>

    <div class="handle">
      <a href="https://twitter.com/<?php print $author->screen_name; ?>">@<?php print $author->screen_name; ?></a>
    </div>
  </div>

  <div class="text">
    <?php print _twitter_filter_link(_twitter_filter_hashtag(_twitter_filter_username($status->text, NULL), NULL), NULL); ?>
  </div>

  <ul class="actions">
    <li><a href=
    "https://twitter.com/intent/tweet?in_reply_to=<?php print $status->twitter_id; ?>"><?php print $reply; ?></a></li>

    <li><a href=
    "https://twitter.com/intent/retweet?tweet_id=<?php print $status->twitter_id; ?>"><?php print $retweet; ?></a></li>

    <li><a href=
    "https://twitter.com/intent/favorite?tweet_id=<?php print $status->twitter_id; ?>"><?php print $favorite; ?></a></li>
  </ul>
</div>
