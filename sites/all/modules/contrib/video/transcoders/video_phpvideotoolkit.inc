<?php

/*
 * @file
 * Transcoder class file to handle ffmpeg settings and conversions.
 *
 */

class video_phpvideotoolkit implements transcoder_interface {

  // Naming for our radio options.  Makes it easy to extend our transcoders.
  private $name = 'PHP Video Toolkit (FFMPEG)';
  private $value = 'video_phpvideotoolkit';
  protected $params = array();
  protected $audio_bitrate = 64;
  protected $video_bitrate = 200;
  protected $video_width = 640;
  protected $video_height = 480;
  protected $ffmpeg = '/usr/bin/ffmpeg';
  protected $nice;
  protected $video_ext = 'flv';
  protected $toolkit;

  public function __construct() {
    $this->params['audiobitrate'] = variable_get('video_ffmpeg_helper_auto_cvr_audio_bitrate', $this->audio_bitrate);
    $this->params['videobitrate'] = variable_get('video_ffmpeg_helper_auto_cvr_video_bitrate', $this->video_bitrate);
    //@todo: move this to the actual widget and save in video_files table.
    $this->params['size'] = variable_get('video_ffmpeg_width', $this->video_width) . 'x' . variable_get('video_ffmpeg_height', $this->video_height);
    $this->params['ffmpeg'] = variable_get('video_ffmpeg_path', $this->ffmpeg);
    $this->nice = variable_get('video_ffmpeg_nice_enable', false) ? 'nice -n 19 ' : '';
    $this->params['videoext'] = variable_get('video_ffmpeg_ext', $this->video_ext);
    $this->params['enable_faststart'] = variable_get('video_ffmpeg_enable_faststart', 0);
    $this->params['faststart_cmd'] = variable_get('video_ffmpeg_faststart_cmd', '/usr/bin/qt-faststart');

    $use_version = 'php5';
    // 	check if php5 is ok
    if ($use_version == 'php5' && version_compare(PHP_VERSION, '5.0.0', '<')) {
      $use_version = 'php4';
    }

    module_load_include('php', 'video', 'libraries/phpvideotoolkit/phpvideotoolkit.' . $use_version);

    $this->toolkit = new PHPVideoToolkit($this->params['ffmpeg'], file_directory_temp() . '/');
  }

  // Returns an array of available encoding & decoding codecs
  public function get_codecs() {
    $info = $this->toolkit->getFFmpegInfo(false);

    $available_codecs = $info['codecs'];

    $codecs = array('decode' => array(), 'encode' => array());

    foreach ($available_codecs as $key => $value) {
      $codecs['encode'][$key] = array();
      $codecs['decode'][$key] = array();

      foreach ($value as $codec_key => $codec) {
        if ($codec['encode']) {
          $codecs['encode'][$key][$codec_key] = $codec['fullname'];
        }
        if ($codec['decode']) {
          $codecs['decode'][$key][$codec_key] = $codec['fullname'];
        }
      }
    }

    return $codecs;
  }

  public function generate_thumbnails($video) {
    global $user;
    // Setup our thmbnail path.
    $video_thumb_path = variable_get('video_thumb_path', 'videos/thumbnails');
    // Get the file system directory.
    $schema_thumb_path = file_default_scheme() . '://' . $video_thumb_path . '/' . $video['fid'];
    file_prepare_directory($schema_thumb_path, FILE_CREATE_DIRECTORY);
    // Total thumbs to generate
    $total_thumbs = variable_get('video_thumbs', 5);
    $videofile = file_load($video['fid']);
    //get the actual video file path from the stream wrappers
    $videopath = drupal_realpath($videofile->uri);
    //get the playtime from the current transcoder
    $duration = $this->get_playtime($videopath);

    $files = NULL;
    for ($i = 1; $i <= $total_thumbs; $i++) {
      $seek = ($duration / $total_thumbs) * $i - 1;  //adding minus one to prevent seek times equaling the last second of the video
      $filename = file_munge_filename("video-thumb-for-" . $video['fid'] . "-$i.jpg", '', TRUE);
      $thumbfile = $schema_thumb_path . '/' . $filename;
      //skip files already exists, this will save ffmpeg traffic
      if (!is_file(drupal_realpath($thumbfile))) {
        $result = $this->toolkit->setInputFile($videopath);
        if (!$result) {
// 			if there was an error then get it 
          $error_msg = t($this->toolkit->getLastError());
          watchdog('transcoder', $error_msg, array(), WATCHDOG_ERROR);
          $this->toolkit->reset();
          continue;
        }

        $this->toolkit->extractFrame($seek);

        $result = $this->toolkit->setOutput(drupal_realpath($schema_thumb_path) . '/', $filename, PHPVideoToolkit::OVERWRITE_EXISTING);
        if (!$result) {
// 			if there was an error then get it 
          $error_msg = t($this->toolkit->getLastError());
          watchdog('transcoder', $error_msg, array(), WATCHDOG_ERROR);
          $this->toolkit->reset();
          continue;
        }

        $result = $this->toolkit->execute(false, true);
        if ($result !== PHPVideoToolkit::RESULT_OK) {
// 			if there was an error then get it
          $error_msg = t($this->toolkit->getLastError());
          watchdog('transcoder', $error_msg, array(), WATCHDOG_ERROR);
          $this->toolkit->reset();
          continue;
        }

        if (!file_exists(drupal_realpath($thumbfile))) {
          $error_param = array('%file' => $thumbfile);
          $error_msg = t("Error generating thumbnail for video: generated file %file does not exist.", $error_param);
          // Log the error message.
          watchdog('transcoder', $error_msg, array(), WATCHDOG_ERROR);
          continue;
        }
      }
      // Begin building the file object.
      // @TODO : use file_munge_filename()
      $file = new stdClass();
      $file->uid = $user->uid;
      $file->status = 0;
      $file->filename = trim($filename);
      $file->uri = $thumbfile;
      $file->filemime = file_get_mimetype($filename);
      $file->filesize = filesize(drupal_realpath($thumbfile));
      $file->timestamp = time();
      $files[] = $file;
    }
    return $files;
  }

  public function convert_video($video) {

    // This will update our current video status to active.
//    $this->change_status($video->vid, VIDEO_RENDERING_ACTIVE);
    // get the paths so tokens will compatible with this
    // @todo : add best method to get existing file path and add converted there
    $target = str_replace('original', '', drupal_dirname($video->uri));
    $converted_base_dir = $target . 'converted/' . $video->fid;
    if (!file_prepare_directory($converted_base_dir, FILE_CREATE_DIRECTORY)) {
      watchdog('transcoder', 'Video conversion failed.  Could not create the directory: ' . $converted_base_dir, array(), WATCHDOG_ERROR);
      return FALSE;
    }
    //get the actual video file path from the stream wrappers
    $original_video_path = drupal_realpath($video->uri);
    // process presets
    $presets = $video->presets;
    $converted_files = array();
    foreach ($presets as $name => $preset) {
      $settings = $preset['settings'];
      // override with preset settings
      if (isset($settings['width']) && !empty($settings['width']) && isset($settings['height']) && !empty($settings['height'])
          && variable_get('video_use_preset_wxh', FALSE)) {
        $video->dimensions = $settings['width'] . 'x' . $settings['height'];
      }
      $converted_filename = file_munge_filename(str_replace(' ', '_', pathinfo($original_video_path, PATHINFO_FILENAME)) . '.' . $settings['video_extension'], $settings['video_extension']);
      $converted = $converted_base_dir . '/' . $converted_filename;
      //get the actual video file path from the stream wrappers
      $converted_video_path = drupal_realpath($converted);
      $dimensions = $this->dimensions($video);
      $dimension = explode('x', $dimensions);

      $video_info = $this->get_video_info($original_video_path);

      if ($this->params['enable_faststart'] && in_array($settings['video_extension'], array('mov', 'mp4'))) {
        $ffmpeg_output = file_directory_temp() . '/' . basename($converted_video_path);
      } else {
        $ffmpeg_output = $converted_video_path;
      }

      $result = $this->toolkit->setInputFile($original_video_path);
      if (!$result) {
// 			if there was an error then get it
        $error_msg = t($this->toolkit->getLastError());
        watchdog('transcoder', $error_msg, array(), WATCHDOG_ERROR);
        $this->toolkit->reset();
        continue;
      }

      if (!empty($settings['max_frame_rate'])) {
        $result = $this->toolkit->setVideoFrameRate($settings['max_frame_rate']);
        if (!$result) {
          // 			if there was an error then get it
          $error_msg = t($this->toolkit->getLastError());
          watchdog('transcoder', $error_msg, array(), WATCHDOG_ERROR);
          $this->toolkit->reset();
          continue;
        }
      }

      $result = $this->toolkit->setVideoCodec($settings['video_codec'], false);
      if (!$result) {
// 			if there was an error then get it
        $error_msg = t($this->toolkit->getLastError());
        watchdog('transcoder', $error_msg, array(), WATCHDOG_ERROR);
        $this->toolkit->reset();
        continue;
      }

      $settings['audio_sample_rate'] = (!empty($settings['audio_sample_rate'])) ? $settings['audio_sample_rate'] : $video_info['audio']['sample_rate'];

      if ($settings['audio_sample_rate'] < 1000) {
        $settings['audio_sample_rate'] *= 1000;
      }

      $settings['audio_sample_rate'] = min($settings['audio_sample_rate'], 44100);

      $result = $this->toolkit->setAudioSampleFrequency($settings['audio_sample_rate']);
      if (!$result) {
// 			if there was an error then get it
        $error_msg = t($this->toolkit->getLastError());
        watchdog('transcoder', $error_msg, array(), WATCHDOG_ERROR);
        $this->toolkit->reset();
        continue;
      }

      $result = $this->toolkit->setAudioCodec($settings['audio_codec'], false);
      if (!$result) {
// 			if there was an error then get it 
        $error_msg = t($this->toolkit->getLastError());
        watchdog('transcoder', $error_msg, array(), WATCHDOG_ERROR);
        $this->toolkit->reset();
        continue;
      }

      $result = $this->toolkit->setAudioChannels($settings['audio_channels']);
      if (!$result) {
// 			if there was an error then get it 
        $error_msg = t($this->toolkit->getLastError());
        watchdog('transcoder', $error_msg, array(), WATCHDOG_ERROR);
        $this->toolkit->reset();
        continue;
      }

      if (empty($settings['audio_bitrate'])) {
        $settings['audio_bitrate'] = $this->audio_bitrate;
      }

      if ($settings['audio_bitrate'] < 1000) {
        $settings['audio_bitrate'] *= 1000;
      }

      $result = $this->toolkit->setAudioBitRate($settings['audio_bitrate']);
      if (!$result) {
// 			if there was an error then get it 
        $error_msg = t($this->toolkit->getLastError());
        watchdog('transcoder', $error_msg, array(), WATCHDOG_ERROR);
        $this->toolkit->reset();
        continue;
      }

      if (empty($settings['video_bitrate'])) {
        $settings['video_bitrate'] = $this->video_bitrate;
      }

      if ($settings['video_bitrate'] < 1000) {
        $settings['video_bitrate'] *= 1000;
      }

      $result = $this->toolkit->setVideoBitRate($settings['video_bitrate']);
      if (!$result) {
// 			if there was an error then get it
        $error_msg = t($this->toolkit->getLastError());
        watchdog('transcoder', $error_msg, array(), WATCHDOG_ERROR);
        $this->toolkit->reset();
        continue;
      }

      $result = $this->toolkit->setVideoDimensions($dimension[0], $dimension[1]);
      if (!$result) {
// 			if there was an error then get it
        $error_msg = t($this->toolkit->getLastError());
        watchdog('transcoder', $error_msg, array(), WATCHDOG_ERROR);
        $this->toolkit->reset();
        continue;
      }

      $result = $this->toolkit->setOutput(dirname($ffmpeg_output) . '/', $converted_filename, PHPVideoToolkit::OVERWRITE_EXISTING);
      if (!$result) {
// 			if there was an error then get it 
        $error_msg = t($this->toolkit->getLastError());
        watchdog('transcoder', $error_msg, array(), WATCHDOG_ERROR);
        $this->toolkit->reset();
        continue;
      }

      $result = $this->toolkit->execute(false, true);
      if ($result !== PHPVideoToolkit::RESULT_OK) {
// 			if there was an error then get it 
        $error_msg = t($this->toolkit->getLastError());
        watchdog('transcoder', $error_msg, array(), WATCHDOG_ERROR);
        $this->toolkit->reset();
        continue;
      }

      $command_output = $this->toolkit->getLastOutput();
      /*
        if ($ffmpeg_output != $converted_video_path && file_exists($ffmpeg_output)) {
        // Because the transcoder_interface doesn't allow the run_command() to include the ability to pass
        // the command to be execute so we need to fudge the command to run qt-faststart.
        $cmd_path = $this->params['cmd_path'];
        $this->params['cmd_path'] = $this->params['faststart_cmd'];
        $command_output .= $this->run_command($ffmpeg_output . ' ' . $converted_video_path, $verbose);
        $this->params['cmd_path'] = $cmd_path;

        // Delete the temporary output file.
        drupal_unlink($ffmpeg_output);
        }
       */
      //lets check to make sure our file exists, if not error out
      if (!file_exists($converted_video_path) || !filesize($converted_video_path)) {
        watchdog('transcoder', 'Video conversion failed for preset %preset.  FFMPEG reported the following output: ' . $command_output, array('%orig' => $video->uri, '%preset' => $name), WATCHDOG_ERROR);
        $this->change_status($video->vid, VIDEO_RENDERING_FAILED);
        return FALSE;
      }
      // Setup our converted video object
      $video_info = pathinfo($converted_video_path);
      //update our converted video
      $video->converted = new stdClass();
      $video->converted->vid = $video->vid;
      $video->converted->filename = $video_info['basename'];
      $video->converted->uri = $converted;
      $video->converted->filemime = file_get_mimetype($converted);
      $video->converted->filesize = filesize($converted);
      $video->converted->status = VIDEO_RENDERING_COMPLETE;
      $video->converted->preset = $name;
      $video->converted->completed = time();
      $converted_files[] = $video->converted;
    }

    // Update our video_files table with the converted video information.
    db_update('video_files')
        ->fields(array(
          'status' => VIDEO_RENDERING_COMPLETE,
          'completed' => time(),
          'data' => serialize($converted_files)))
        ->condition('vid', $video->converted->vid, '=')
        ->execute();
    watchdog('transcoder', 'Successfully converted %orig to %dest', array('%orig' => $video->uri, '%dest' => $video->converted->uri), WATCHDOG_INFO);
    return TRUE;
  }

  /**
   * Get some information from the video file
   */
  public function get_video_info($video) {
    $video_info = $this->toolkit->getFileInfo($video);
    return $video_info;
  }

  /**
   * Return the playtime seconds of a video
   */
  public function get_playtime($video) {
    $video_info = $this->get_video_info($video);

    return $video_info['duration']['seconds'];
  }

  /*
   * Return the dimensions of a video
   */

  public function get_dimensions($video) {
    $video_info = $this->get_video_info($video);
    $res = array('width' => 0, 'height' => 0);
    // Get dimensions
    $res['width'] = $video_info['video']['dimensions']['width'] ? $video_info['video']['dimensions']['width'] : NULL;
    $res['height'] = $video_info['video']['dimensions']['height'] ? $video_info['video']['dimensions']['height'] : NULL;

    return $res;
  }

  /**
   * Interface Implementations
   * @see sites/all/modules/video/includes/transcoder_interface#get_name()
   */
  public function get_name() {
    return $this->name;
  }

  /**
   * Interface Implementations
   * @see sites/all/modules/video/includes/transcoder_interface#get_value()
   */
  public function get_value() {
    return $this->value;
  }

  /**
   * Interface Implementations
   * @see sites/all/modules/video/includes/transcoder_interface#get_help()
   */
  public function get_help() {
    return l(t('PHP Video Toolkit online documentation'), 'http://sourceforge.net/projects/phpvideotoolkit/');
  }

  /**
   * Interface Implementations
   * @see sites/all/modules/video/includes/transcoder_interface#admin_settings()
   */
  public function admin_settings() {
    $form = array();
    $form = array();
    $form['video_ffmpeg_toolkit_start'] = array(
      '#type' => 'markup',
      '#markup' => '<div id="video_phpvideotoolkit">',
    );
    $form['phpvideotoolkit']['video_ffmpeg_nice_enable'] = array(
      '#type' => 'checkbox',
      '#title' => t('Enable the use of <b>nice</b> when calling the command.'),
      '#default_value' => variable_get('video_ffmpeg_nice_enable', TRUE),
      '#description' => t('The nice command Invokes a command with an altered scheduling priority.  This option may not be available on windows machines, so disable it.')
    );
    // FFMPEG
    $form['phpvideotoolkit']['transcoders'] = array(
      '#type' => 'fieldset',
      '#title' => t('Path to Transcoder Executables'),
      '#collapsible' => TRUE,
      '#collapsed' => FALSE
    );
    $form['phpvideotoolkit']['transcoders']['video_ffmpeg_path'] = array(
      '#type' => 'textfield',
      '#title' => t('FFMPEG'),
      '#description' => t('Absolute path to ffmpeg executable. This will provide a token of !ffmpeg to preset commands.'),
      '#default_value' => variable_get('video_ffmpeg_path', '/usr/bin/ffmpeg'),
    );
    $form['video_ffmpeg_toolkit_end'] = array(
      '#type' => 'markup',
      '#markup' => '</div>',
    );
    return $form;
  }

  /**
   * Interface Implementations
   * @see sites/all/modules/video/includes/transcoder_interface#admin_settings_validate()
   */
  public function admin_settings_validate($form, &$form_state) {
    return;
  }

  /**
   * Interface Implementations
   * @see sites/all/modules/video/includes/transcoder_interface#create_job()
   */
  public function create_job($video, $nid) {
    return db_insert('video_files')
        ->fields(array(
          'fid' => $video['fid'],
          'nid' => $nid,
          'status' => VIDEO_RENDERING_PENDING,
          'dimensions' => $video['dimensions'],
        ))
        ->execute();
  }

  /**
   * Interface Implementations
   * @see sites/all/modules/video/includes/transcoder_interface#delete_job()
   */
  public function delete_job($video) {
    $video = (object) $video;
    if (!$video = $this->load_job($video->fid))
      return;
    // converted output values
    $converted = unserialize($video->data);
    if (!empty($converted)) {
      foreach ($converted as $file) {
        if (file_exists(drupal_realpath($file->uri)))
          @drupal_unlink($file->uri);
      }
    }
    //now delete our rows.
    db_delete('video_files')
        ->condition('fid', $video->fid)
        ->execute();
  }

  /**
   * Interface Implementations
   * @see sites/all/modules/video/includes/transcoder_interface#load_job()
   */
  public function load_job($fid) {
    $job = null;
    $job = db_query("SELECT f.*, vf.vid, vf.nid, vf.dimensions, vf.data, vf.status as video_status
      FROM {video_files} vf LEFT JOIN {file_managed} f ON vf.fid = f.fid WHERE f.fid=vf.fid AND f.fid = :fid",
                array(':fid' => $fid))
            ->fetch();
    if (!empty($job))
      return $job;
    else
      return FALSE;
  }

  /**
   * Interface Implementations
   * @see sites/all/modules/video/includes/transcoder_interface#load_job_queue()
   */
  public function load_job_queue() {
    $total_videos = variable_get('video_ffmpeg_instances', 5);
    $videos = array();
    $result = db_query_range('SELECT f.*, vf.vid, vf.nid, vf.dimensions, vf.status as video_status
      FROM {video_files} vf LEFT JOIN {file_managed} f ON vf.fid = f.fid
      WHERE vf.status = :vstatus AND f.status = :fstatus ORDER BY f.timestamp',
            0, $total_videos, array(':vstatus' => VIDEO_RENDERING_PENDING, ':fstatus' => FILE_STATUS_PERMANENT));
    foreach ($result as $row) {
      $videos[] = $row;
    }
    return $videos;
  }

  /**
   * Interface Implementations
   * @see sites/all/modules/video/includes/transcoder_interface#load_completed_job()
   */
  public function load_completed_job(&$video) {
    $file = $this->load_job($video->fid);
    $data = unserialize($file->data);
    if (!empty($data))
      foreach ($data as $value) {
        $extension = pathinfo(drupal_realpath($value->uri), PATHINFO_EXTENSION);
        $video->files->{$extension}->filename = $value->filename;
        $video->files->{$extension}->filepath = $value->uri;
        $video->files->{$extension}->filemime = file_get_mimetype($value->uri);
        $video->files->{$extension}->url = file_create_url($value->uri);
        $video->files->{$extension}->extension = $extension;
        $video->player = strtolower($extension);
      }
    else
      return FALSE;
  }

  /**
   * Change the status of the file.
   *
   * @param (int) $vid
   * @param (int) $status
   */
  public function change_status($vid, $status) {
    db_update('video_files')->fields(array(
          'status' => $status,))
        ->condition('vid', $vid, '=')
        ->execute();
  }

  /*
   * Function determines the dimensions you want and compares with the actual wxh of the video.
   *
   * If they are not exact or the aspect ratio does not match, we then figure out how much padding
   * we should add.  We will either add a black bar on the top/bottom or on the left/right.
   *
   * @TODO I need to look more at this function.  I don't really like the guess work here.  Need to implement
   * a better way to check the end WxH.  Maybe compare the final resolution to our defaults?  I don't think
   * that just checking to make sure the final number is even is accurate enough.
   */

  public function dimensions($video) {
    //lets setup our dimensions.  Make sure our aspect ratio matches the dimensions to be used, if not lets add black bars.
    $aspect_ratio = _video_aspect_ratio(drupal_realpath($video->uri));
    $ratio = $aspect_ratio['ratio'];
    $width = $aspect_ratio ['width'];
    $height = $aspect_ratio['height'];

    $wxh = explode('x', $video->dimensions);
    $output_width = $wxh[0];
    $output_height = $wxh[1];
    $output_ratio = number_format($output_width / $output_height, 4);

    if ($output_ratio != $ratio && $width && $height) {
      $options = array();
      // Figure out our black bar padding.
      if ($ratio < $output_width / $output_height) {
        $end_width = $output_height * $ratio;
        $end_height = $output_height;
      } else {
        $end_height = $output_width / $ratio;
        $end_width = $output_width;
      }

      // We need to get back to an even resolution and maybe compare with our defaults?
      // @TODO Make this more exact on actual video dimensions instead of making sure the wxh are even numbers

      if ($end_width == $output_width) {
        // We need to pad the top/bottom of the video
        $padding = round($output_height - $end_height);
        $pad1 = $pad2 = floor($padding / 2);
        if ($pad1 % 2 !== 0) {
          $pad1++;
          $pad2--;
        }
        if (variable_get('video_ffmpeg_pad_method', 0)) {
          $options[] = '-vf "pad=' . round($output_width) . ':' . round($output_height) . ':0:' . $pad1 . '"';
        } else {
          $options[] = '-padtop ' . $pad1;
          $options[] = '-padbottom ' . $pad2;
        }
      } else {
        // We are padding the left/right of the video.
        $padding = round($output_width - $end_width);
        $pad1 = $pad2 = floor($padding / 2);  //@todo does padding need to be an even number?
        if ($pad1 % 2 !== 0) {
          $pad1++;
          $pad2--;
        }
        if (variable_get('video_ffmpeg_pad_method', 0)) {
          $options[] = '-vf "pad=' . round($output_width) . ':' . round($output_height) . ':' . $pad1 . ':0"';
        } else {
          $options[] = '-padleft ' . $pad1;
          $options[] = '-padright ' . $pad2;
        }
      }

      $end_width = round($end_width) % 2 !== 0 ? round($end_width) + 1 : round($end_width);
      $end_height = round($end_height) % 2 !== 0 ? round($end_height) + 1 : round($end_height);
      //add our size to the beginning to make sure it hits our -s
      array_unshift($options, $end_width . 'x' . $end_height);
      return implode(' ', $options);
    } else {
      return $video->dimensions;
    }
  }

}

?>
