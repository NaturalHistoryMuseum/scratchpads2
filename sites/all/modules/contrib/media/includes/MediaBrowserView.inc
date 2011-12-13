<?php

class MediaBrowserView extends MediaBrowserPlugin {

  function view() {
    $view = views_get_view($this->info['view_name']);
    $display_id = $this->info['view_display_id'];

    // Check if the user has access to see this view.
    if (empty($view) || !$view->access($display_id)) {
      return array();
    }

    return array(
      '#markup' => $view->preview($display_id) . '<div class="form-actions form-wrapper"></div>',
    );
  }
}
