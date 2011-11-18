<?php
/*
 * @file
 * Theme file to handle divx output.
 *
 * Variables passed.
 * $video is the video object.
 * $node is the node object.
 *
 * http://labs.divx.com/Webplayer
 * http://labs.divx.com/WebPlayerCodeGenerator
 *
 */
?>

<object classid="clsid:67DABFBF-D0AB-41fa-9C46-CC0F21721616" width="<?php print $video->player_width; ?>" height="<?php print $video->player_height; ?>" codebase="http://go.divx.com/plugin/DivXBrowserPlugin.cab">
  <param name="src" value="<?php print $video->files->{$video->player}->url; ?>" />
  <param value="<?php print $video->autoplay ? 'true' : 'false'; ?>" name="autoPlay">
  <param name="pluginspage" value="http://go.divx.com/plugin/download/" />
  <param value="none" name="custommode">
  <param name="previewImage" value="<?php print $video->thumbnail->url; ?>" />
  <object class="video-object" type="video/divx" data="<?php print $video->files->{$video->player}->url; ?>" previewImage="<?php print $video->thumbnail->url; ?>" width="<?php print $video->player_width; ?>" height="<?php print $video->player_height; ?>" autoplay="<?php print $video->autoplay ? 'true' : 'false'; ?>" mode="large" custommode="none">
    <?php print t('No video?  Get the DivX Web Player !plugin', array('!plugin' => l(t('Plugin'), 'http://go.divx.com/plugin/download/'))); ?>
  </object>
</object>