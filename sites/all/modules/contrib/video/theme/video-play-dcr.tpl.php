<?php 
/*
 * @file
 * Theme file to handle director output.
 * 
 * Variables passed.
 * $video is the video object.
 * $node is the node object.
 */
?> 
<object classid="clsid:166B1BCA-3F9C-11CF-8075-444553540000" type="application/x-director" width="<?php print $video->player_width; ?>" height="<?php print $video->player_height; ?>" codebase="http://download.macromedia.com/pub/shockwave/cabs/director/sw.cab#version=10,0,0,0">
  <param name="src" value="<?php print $video->files->{$video->player}->url; ?>" />
  <object class="video-object" type="application/x-director" data="<?php print $video->files->{$video->player}->url; ?>" width="<?php print $video->player_width; ?>" height="<?php print $video->player_height; ?>" mode="zero">
    <?php print t('No video?  Get the Director !plugin', array('!plugin' => l(t('Plugin'), 'http://www.macromedia.com/shockwave/download/'))); ?>
  </object>
</object>