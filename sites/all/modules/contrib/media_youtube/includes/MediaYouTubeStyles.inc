<?php

/**
 * @file
 * Styles class implementation for Media: YouTube.
 */

class MediaYouTubeStyles extends FileStyles {
  public $autoplay;
  public $fullscreen;

  function getAutoplay() {
    return $this->get('autoplay');
  }
  function setAutoplay($value) {
    return $this->set('autoplay', $value);
  }
  function getFullscreen() {
    return $this->get('fullscreen');
  }
  function setFullscreen($value) {
    return $this->set('fullscreen', $value);
  }

  function getImageUri() {
    if ($image_uri = $this->get('imageUri')) {
      return $image_uri;
    }
    $object = $this->getObject();
    if ($object->uri) {
      $wrapper = file_stream_wrapper_get_instance_by_uri($object->uri);
      return $wrapper->getLocalThumbnailPath();
    }
  }
  function video($effect) {
    $variables = array(
      'uri' => $this->getUri(),
      'width' => $this->getWidth(),
      'height' => $this->getHeight(),
      'autoplay' => $this->getAutoplay(),
      'fullscreen' => $this->getFullscreen(),
    );
    $this->setOutput(theme('media_youtube_video', $variables));
  }
}
