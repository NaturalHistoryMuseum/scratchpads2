<?php

class MediaBrowserLibrary extends MediaBrowserPlugin {

  function view() {
    $path = drupal_get_path('module', 'media');

    return array(
      '#attached' => array(
        'js' => array(
          $path . '/js/plugins/media.library.js',
        ),
        'css' => array(
          //@todo: should move this.
          $path . '/js/plugins/media.library.css',
        ),
      ),
      '#settings' => array(
        'viewMode' => 'thumbnails',
        'getMediaUrl' => url('media/browser/list'),
      ) + $this->params,
      '#markup' => '<div id="container"><div id="scrollbox"><ul id="media-browser-library-list" class="media-list-thumbnails"></ul><div id="status"></div></div></div>',
    );
  }

}
