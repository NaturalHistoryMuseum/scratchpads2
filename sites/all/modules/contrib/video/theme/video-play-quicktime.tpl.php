<?php
/*
 * @file
 * Theme file to handle quicktime output.
 * 
 * Variables passed.
 * $video is the video object.
 * $node is the node object.
 *
 * 
 * <script type="text/javascript" src="/sites/all/libraries/quicktime/AC_QuickTime.js"></script>
 * <script type="text/javascript">
 *   QT_WriteOBJECT('<?php print $video->files->{$video->player}->url; ?>' , '<?php print $video->width; ?>', '<?php print $video->height; ?>' , '');
 * </script>
 */
?>
<object classid="clsid:02BF25D5-8C17-4B23-BC80-D3488ABDDC6B" codebase="http://www.apple.com/qtactivex/qtplugin.cab#version=7,3,0,0"  width="<?php print $video->player_width; ?>" height="<?php print $video->player_height; ?>">
  <param name="src" value="<?php print $video->files->{$video->player}->url; ?>" />
  <param name="controller" value="true" />
  <param name="scale" value="tofit" />
  <param name="autoplay" value="<?php print $video->autoplay ? 'true' : 'false'; ?>" />
  <param name="pluginurl" value="http://www.apple.com/quicktime/download/" />
  <embed src="<?php print $video->files->{$video->player}->url; ?>"
         type="video/quicktime"
         pluginspage="http://www.apple.com/quicktime/download/"
         width="<?php print $video->player_width; ?>"
         height="<?php print $video->player_height; ?>"
         autostart="<?php print $video->autoplay ? 'true' : 'false'; ?>"
         controller="true" >
  </embed>
</object>