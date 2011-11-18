<?php

/*
 * @file
 * Class file used to store metadata on the video.
 * 
 */

class video_metadata {

  private $metadata;

  public function __construct($metadata = null) {
    //get our configured transcoder.
    if (!isset($metadata))
      $metadata = variable_get('video_metadata', 'flvtool2');
    if (!module_load_include('inc', 'video', '/metadata/' . $metadata)) {
      $modules = module_list();
      foreach ($modules as $module) {
        $mobule_files = array();
        $module_path = drupal_get_path('module', $module) . '/metadata';
        $mobule_files = file_scan_directory($module_path, '/.*\.inc/');
        if (is_array($mobule_files)) {
          foreach ($mobule_files as $file) {
            if ($file->name == $metadata)
              require_once $file->filename;
          }
        }
//
      }
    }
    if (class_exists($metadata)) {
      $this->metadata = new $metadata;
    } else {
      drupal_set_message(t('The metadata is not configured properly.'), 'error');
    }
  }

  public function process($video) {
    $command_output = $this->metadata->process($video);
    return $command_output;
  }

  public function admin_settings() {
    $form = array();
    $form['video_metadata'] = array(
      '#type' => 'checkbox',
      '#title' => t('Enable Metadata'),
      '#default_value' => variable_get('video_metadata', FALSE),
      '#description' => t('Metadata is particularly useful in video, where information about its contents (such as transcripts of conversations and text descriptions of its scenes) are not directly understandable by a computer, but where efficient search is desirable.'),
    );
    $options = $this->_metadata();
    $form['video_metadata'] = array(
      '#type' => 'radios',
      '#title' => t('Video Metadata'),
      '#default_value' => variable_get('video_metadata', 'flvtool2'),
      '#options' => $options['radios'],
      '#description' => t('!list', array('!list' => theme('item_list', $options['help']))),
      '#prefix' => '<div id="metadata-radios">',
      '#suffix' => '</div>',
    );
    $form = $form + $options['admin_settings'];
    $form['video_metadata_dimensions'] = array(
      '#type' => 'textarea',
      '#title' => t('Selectable Dimensions when uploading videos'),
      '#description' => t('Enter one dimension per line as Video Resolutions.  Each resolution must be in the form of WxH where W=Width and H=Height in pixels.  Example dimensions are 1280x720.'),
      '#default_value' => variable_get("video_metadata_dimensions", video_default_dimensions()),
    );

    return $form;
  }

  private function _metadata() {
    $files = array();
    // Lets find our transcoder classes and build our radio options
    // We do this by scanning our transcoders folder
    $form = array('radios' => array(), 'help' => array(), 'admin_settings' => array());
    $path = drupal_get_path('module', 'video') . '/metadata';
    $files = file_scan_directory($path, '/.*\.inc/');
    // check inside sub modules
    $modules = module_list();
    foreach ($modules as $module) {
      $mobule_files = array();
      $module_path = drupal_get_path('module', $module) . '/metadata';
      $mobule_files = file_scan_directory($module_path, '/.*\.inc/');
      $files = array_merge($files, $mobule_files);
    }

    foreach ($files as $file) {
      if (!module_load_include('inc', 'video', '/metadata/' . $file->name))
        require_once $file->filename;
      $focus = new $file->name;
      $form['radios'][$focus->get_value()] = $focus->get_name();
      $form['help'][] = $focus->get_help();
      $form['admin_settings'] = $form['admin_settings'] + $focus->admin_settings();
    }
    return $form;
  }

  public function admin_settings_validate($form, $form_state) {
    return $this->metadata->admin_settings_validate($form, $form_state);
  }

}

interface metadata_interface {

  public function get_name();

  public function get_help();

  public function process($video);

  public function admin_settings();

  public function admin_settings_validate($form, &$form_state);
}