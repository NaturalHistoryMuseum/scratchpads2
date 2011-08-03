<?php

/*
 * @file
 * Class file used to create our video and thumbnail objects.
 * @todo : remove video_zencoder validation form the class
 *
 */

class video_helper {

  public function video_object($variables) {
    $field_settings = $variables['field']['settings'];
    $instance_settings = $variables['instance']['settings'];
    //setup our width x height

    $player_dimensions = explode("x", $variables['item']['player_dimensions']);
    // set the video dimentions
    if (!isset($variables['item']['dimensions'])) {
      $dimensions = explode("x", $instance_settings['default_dimensions']);
      if (!isset($dimensions[0]) || !isset($dimensions[1])) {
        drupal_set_message(t('Something is wrong with your dimensions.  Make sure you enter dimensions in the form of WxH.'), 'error');
      }
    }
    else
      $dimensions = explode("x", $variables['item']['dimensions']);

    if (!isset($player_dimensions[0]) || !isset($player_dimensions[1])) {
      $player_dimensions = explode("x", $instance_settings['default_player_dimensions']);
      if (!isset($player_dimensions[0]) || !isset($player_dimensions[1])) {
        drupal_set_message(t('Something is wrong with your player dimensions.  Make sure you enter the player dimensions in the form of WxH.'), 'error');
      }
    }

    // Build our video object for all types.
    $video = new stdClass();
    $video->fid = $variables['item']['fid'];
    $video->original = $variables['item'];
    $extension = strtolower(pathinfo($variables['item']['filename'], PATHINFO_EXTENSION));
    $video->files->{$extension}->filename = pathinfo($variables['item']['filename'], PATHINFO_FILENAME) . '.' . $extension;
    $video->files->{$extension}->filepath = $variables['item']['uri'];
    $video->files->{$extension}->url = file_create_url($variables['item']['uri']);
    $video->files->{$extension}->uri = $variables['item']['uri'];
    $video->files->{$extension}->extension = $extension;
    // set the player to play
    $video->player = $extension;
    $video->width = trim($dimensions[0]);
    $video->height = trim($dimensions[1]);
    $video->player_width = trim($player_dimensions[0]);
    $video->player_height = trim($player_dimensions[1]);
    // load thumbnail object
    $video->thumbnail = $this->thumbnail_object($variables);
    $video->formatter = $variables['instance']['display']['default']['type'];
    $video->autoplay = variable_get('video_autoplay', FALSE);
    $video->autobuffering = variable_get('video_autobuffering', TRUE);
    $video->theora_player = variable_get('video_ogg_player', 'http://theora.org/cortado.jar');
    // lets find out if we have transcoded this file and update our paths.
    if (isset($field_settings['autoconversion']) && $field_settings['autoconversion']) {
      // discard all existing file data
      module_load_include('inc', 'video', '/includes/conversion');
      $conversion = new video_conversion;
      if ($conversion->load_job($variables['item']['fid'])) {
        // reset the video files object and add converted videos in to it
        $video->files = new stdClass();
        $conversion->load_completed_job($video);
      }
    }
    // Return our object
    return $video;
  }

  public function thumbnail_object($variables) {
    $field_settings = $variables['field']['settings'];
    $instance_settings = $variables['instance']['settings'];
    // Build our thumbnail object
    $thumbnail = new stdClass();
    $thumbnail->filepath = '';
    $thumbnail->url = '';
    //@todo future enhancements for our thumbnails
    $thumbnail->alt = $variables['entity']->title;
    $thumbnail->title = $variables['entity']->title;
//    $thumbnail->description = '';
    // Setup our thumbnail path.

    $use_default_img = isset($variables['item']['use_default_video_thumb']) ? TRUE : FALSE;
    if ($use_default_img && !empty($field_settings['default_video_thumbnail'])) {
      $default_thumbnail = file_load($field_settings['default_video_thumbnail']['fid']);
      // Check the checkbox to use default thumbnail on node
      $thumbnail->filepath = $default_thumbnail->uri;
    } elseif (isset($variables['item']['thumbanail']) ? TRUE : FALSE) {
      // actual video thumbnails is present
      $thumbnail_load = file_load($variables['item']['thumbanail']);
      $thumbnail->filepath = $thumbnail_load->uri;
    } else {
      //need some type of default if nothing is present
//      drupal_set_message(t('No thumbnail has been configured for the video !title.', array('!title' => $variables['entity']->title)), 'error');
      return;
    }

    $thumbnail->url = file_create_url($thumbnail->filepath);
    // uri
    $thumbnail->uri = $thumbnail->filepath;
    //mime
    $thumbnail->filemime = file_get_mimetype($thumbnail->filepath);

    //swftools appends sites/default/files to the front of our path...
    //@todo Is this a setting?  Need to figure this out.
    $thumbnail->swfthumb = $thumbnail->filepath;
    // Return our object
    return $thumbnail;
  }

}
