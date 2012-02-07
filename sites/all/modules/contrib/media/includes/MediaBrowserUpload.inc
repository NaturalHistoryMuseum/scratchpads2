<?php

class MediaBrowserUpload extends MediaBrowserPlugin {

  function view() {
    $path = drupal_get_path('module', 'media');

    module_load_include('inc', 'media', 'includes/media.admin');
    module_load_include('inc', 'media', 'includes/media.pages');

    $attached = array();
    if ($this->params['multiselect'] && module_exists('plupload')) {
      $upload_form_id = 'media_add_upload_multiple';
      $attached['js'] = array($path . '/js/plugins/media.upload_multiple.js');
    }
    else {
      $upload_form_id = 'media_add_upload';
    }

    $upload_form = drupal_get_form($upload_form_id, $this->params);
    return array(
      'form' => $upload_form,
      '#attached' => $attached,
    );
  }

}
