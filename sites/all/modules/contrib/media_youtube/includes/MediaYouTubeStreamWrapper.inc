<?php

/**
 *  @file
 *  Create a YouTube Stream Wrapper class for the Media/Resource module.
 */

/**
 *  Create an instance like this:
 *  $youtube = new ResourceYouTubeStreamWrapper('youtube://?v=[video-code]');
 */
class MediaYouTubeStreamWrapper extends MediaReadOnlyStreamWrapper {
  protected $base_url = 'http://youtube.com/watch';

  function getTarget($f) {
    return FALSE;
  }

  static function getMimeType($uri, $mapping = NULL) {
    return 'video/youtube';
  }

  function getOriginalThumbnailPath() {
    $parts = $this->get_parameters();
    return 'http://img.youtube.com/vi/'. check_plain($parts['v']) .'/0.jpg';
  }

  function getLocalThumbnailPath() {
    $parts = $this->get_parameters();
    $local_path = 'public://media-youtube/' . check_plain($parts['v']) . '.jpg';
    if (!file_exists($local_path)) {
      $dirname = drupal_dirname($local_path);
      file_prepare_directory($dirname, FILE_CREATE_DIRECTORY | FILE_MODIFY_PERMISSIONS);
      @copy($this->getOriginalThumbnailPath(), $local_path);
    }
    return $local_path;
  }
}
