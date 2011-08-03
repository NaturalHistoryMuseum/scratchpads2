<?php

/*
 * @file
 * Class file used to store metadata on the video.
 *
 */
class flvtool2 extends video_metadata {

  protected $params = array();
  protected $nice = 'nice -n 19';
  protected $meta_command = '-UP';
  protected $meta_command_path = '/usr/bin/flvtool2';
  protected $name = 'FlvTool2';
  protected $value = 'flvtool2';

  public function __construct() {
    $this->params['cmd_path'] = variable_get('video_metadata_path', $this->meta_command_path);
  }

  /**
   * Interface Implementations
   * @see sites/all/modules/video/includes/metadata_interface#get_name()
   */
  public function get_name() {
    return $this->name;
  }

  /**
   * Interface Implementations
   * @see sites/all/modules/video/includes/metadata_interface#get_help()
   */
  public function get_help() {
    return t('!flvtools calculates various meta data and inserts a onMetaData tag in the video. It cuts FLV files and adds cue Points (onCuePoint). A debug command shows the inside of a FLV file and the print command gives meta data information in XML or YAML format.', array('!flvtools' => l(t('FlvTools2 '), 'http://github.com/unnu/flvtool2')));
  }

  /**
   * Interface Implementations
   * @see sites/all/modules/video/includes/metadata_interface#get_value()
   */
  public function get_value() {
    return $this->value;
  }

  public function run_command($options) {
    $command = $this->nice . ' ' . $this->params['cmd_path'] . ' ' . $options . '  2>&1';
    watchdog('video_metadata', 'Executing command: ' . $command, array(), WATCHDOG_DEBUG);
    ob_start();
    passthru($command, $command_return);
    $output = ob_get_contents();
    ob_end_clean();
    return $output;
  }

  public function process($video) {
    $command_output = $this->run_command($this->meta_command . ' ' . $video);
    return $command_output;
  }

  public function admin_settings() {
    $form = array();
    $form['video_flvtool2_start'] = array(
      '#type' => 'markup',
      '#value' => '<div id="video_flvtool2">',
    );
    $form['video_metadata_path'] = array(
      '#type' => 'textfield',
      '#title' => t('Path to FLVTool2'),
      '#description' => t('Executable path to flvtool2.'),
      '#default_value' => variable_get('video_metadata_path', $this->meta_command_path),
    );
    $form['video_flvtool2_end'] = array(
      '#type' => 'markup',
      '#value' => '</div>',
    );
    return $form;
  }

  public function  admin_settings_validate($form, $form_state) {
    return;
  }

}

?>
