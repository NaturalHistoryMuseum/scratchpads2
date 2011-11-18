<?php 
/*
 * @file
 * Theme file to handle ogg theora output.
 * 
 * Variables passed.
 * $video is the video object.
 * $node is the node object.
 * 
 */
?> 
<applet=code="com.fluendo.player.Cortado.class" archive="<?php print $video->theora_player; ?>" width="<?php print $video->player_width; ?>" height="<?php print $video->player_height; ?>">
  <param name="url" value="<?php print $video->files->{$video->player}->url; ?>" />
  <param name="local" value="false" />
  <param name="mode" value="zero" />
  <param name="keepaspect" value="true" />
  <param name="video" value="true" />
  <param name="audio" value="true" />
  <param name="seekable" value="true" />
  <param name="bufferSize" value="200" />
  <?php print t('No video?  Get the Latest Cortado !plugin', array('!plugin' => l(t('Plugin'), 'http://www.theora.org/cortado/'))); ?>
</applet>