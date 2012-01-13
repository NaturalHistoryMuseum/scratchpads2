<?php

/**
 * @file
 * Media browser internet plugin.
 */

class MediaBrowserInternet extends MediaBrowserPlugin {

  function view() {
    // @todo: implement the multiselect argument here.
    $from_web_form = drupal_get_form('media_internet_add',  $this->params['types'], $this->params['multiselect']);
    return array(
      'form' => $from_web_form,
      '#attached' => array(
        //'js' => array($path . '/js/plugins/media.fromurl.js'),
      ),
    );
  }

}
