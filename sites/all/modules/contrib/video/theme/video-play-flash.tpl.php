<?php 
/*
 * @file
 * Theme file to handle flash output.
 * 
 * Variables passed.
 * $video is the video object.
 * $node is the node object.
 * 
 */
?> 
<object classid="clsid:D27CDB6E-AE6D-11cf-96B8-444553540000" width="<?php print $video->player_width; ?>" height="<?php print $video->player_height; ?>" codebase="http://download.macromedia.com/pub/shockwave/cabs/flash/swflash.cab#version=6,0,40,0">
  <param name="movie" value="<?php print $video->files->{$video->player}->url; ?>" />
  <param name="autoplay" value="<?php print $video->autoplay; ?>" />
  <param name="wmode" value="transparent" />
  <object class="video-object" type="application/x-shockwave-flash" data="<?php print $video->files->{$video->player}->url; ?>" width="<?php print $video->player_width; ?>" height="<?php print $video->player_height; ?>">
    <?php print t('No video?  Get the Adobe Flash !plugin', array('!plugin' => l(t('Plugin'), 'http://get.adobe.com/flashplayer/'))); ?>
  </object>
</object>