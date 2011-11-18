<?php

/**
 * Returns HTML for an video field widget.
 *
 * @param $variables
 *   An associative array containing:
 *   - element: A render element representing the image field widget.
 *
 * @ingroup themeable
 */
function theme_video_widget($variables) {
  $element = $variables['element'];
  $output = '';
  $output .= '<div class="video-widget form-managed-file clearfix">';

  if (isset($element['preview'])) {
    $output .= '<div class="video-preview">';
    $output .= drupal_render($element['preview']);
    $output .= '</div>';
  }

  $output .= '<div class="video-widget-data">';
  if ($element['fid']['#value'] != 0) {
    $element['filename']['#markup'] .= ' <span class="file-size">(' . format_size($element['#file']->filesize) . ')</span> ';
  }
  $output .= drupal_render_children($element);
  $output .= '</div>';
  $output .= '</div>';

  return $output;
}

/*
 * Default video cck formatter.  Makes sure the video being displayed exists, has been converted (if in que).
 * If not or the video is processed, then it will get the default player for the specific video type for output.
 */

function theme_video($variables) {
  $themed_output = '';
  if (empty($variables['item']['fid']))
    return '';
  $field_settings = $variables['field']['settings'];
  $instance_settings = $variables['instance']['settings'];
  // Only needs to be ran if they are converting videos
  if (isset($field_settings['autoconversion']) && $field_settings['autoconversion']) {
    module_load_include('inc', 'video', '/includes/conversion');
    $conversion = new video_conversion;
    if ($video = $conversion->load_job($variables['item']['fid'])) {
      if ($video->video_status == VIDEO_RENDERING_ACTIVE || $video->video_status == VIDEO_RENDERING_PENDING) {
        return theme('video_inprogress');
      } else if ($video->video_status == VIDEO_RENDERING_FAILED) {
        return theme('video_conversion_failed');
      }
    }
  }
  // Setup our video object
  module_load_include('inc', 'video', '/includes/video_helper');
  $video_helper = new video_helper;
  $video = $video_helper->video_object($variables);
  // Lets spit out our theme based on the extension
  $defaults = video_video_extensions();
  $theme_function = variable_get('video_extension_' . $video->player, $defaults[$video->player]);

  // Lets do some special handling for our flv files, HTML5 to accomdate multiple players.
  switch ($theme_function) {
    case 'video_play_flv':
      return theme('video_flv', array('video' => $video, 'themed_output' => $themed_output));
      break;
    case 'video_play_html5':
      return theme('video_html5', array('video' => $video, 'themed_output' => $themed_output));
      break;
    default:
      return theme($theme_function, array('video' => $video, 'themed_output' => $themed_output));
      break;
  }
}

/*
 * Renders the video thumbnail as a link to the node page.
 */

function theme_video_thumbnail($variables) {
// Inside a view $variables may contain null data. In that case, just return.
  if (empty($variables['item']['fid']))
    return '<!-- File not found: ' . $variables['item']['fid'] . ' -->';

//setup our thumbnail object
  module_load_include('inc', 'video', '/includes/video_helper');
  $video_helper = new video_helper;
// @TODO : change the object to an array
  $thumbnail = (array) $video_helper->thumbnail_object($variables);
// return if no file path found for the video thumbnails and add log message
  if (empty($thumbnail['filepath'])) {
    watchdog('video', 'Unable to find the video thumbnail for the %node.', array('%node' => $variables['entity']->title), WATCHDOG_ERROR);
    return '<!-- File not found: ' . $thumbnail['filepath'] . ' -->';
  }
  $image = array(
    'path' => $thumbnail['filepath'],
    'alt' => $thumbnail['alt'],
  );
// Do not output an empty 'title' attribute.
  if (drupal_strlen($thumbnail['title']) > 0) {
    $image['title'] = $thumbnail['title'];
  }

  if ($variables['video_style']) {
    $image['style_name'] = $variables['video_style'];
    $output = theme('image_style', $image);
  } else {
    $output = theme('image', $image);
  }

  if ($variables['path']) {
    $path = $variables['path']['path'];
    $options = $variables['path']['options'];
    // When displaying an image inside a link, the html option must be TRUE.
    $options['html'] = TRUE;
    $output = l($output, $path, $options);
  }

  return $output;
}

/**
 * Returns HTML for an image using a specific image style.
 *
 * @param $variables
 *   An associative array containing:
 *   - style_name: The name of the style to be used to alter the original image.
 *   - path: The path of the image file relative to the Drupal files directory.
 *     This function does not work with images outside the files directory nor
 *     with remotely hosted images.
 *   - alt: The alternative text for text-based browsers.
 *   - title: The title text is displayed when the image is hovered in some
 *     popular browsers.
 *   - attributes: Associative array of attributes to be placed in the img tag.
 *   - getsize: If set to TRUE, the image's dimension are fetched and added as
 *     width/height attributes.
 *
 * @ingroup themeable
 */
function theme_video_thumb_style($variables) {
  $style_name = $variables['style_name'];
  $path = $variables['path'];

// theme_image() can only honor the $getsize parameter with local file paths.
// The derivative image is not created until it has been requested so the file
// may not yet exist, in this case we just fallback to the URL.
  $style_path = image_style_path($style_name, $path);
  if (!file_exists($style_path)) {
    $style_path = image_style_url($style_name, $path);
  }
  $variables['path'] = $style_path;
  return theme('image', $variables);
}

/**
 * Displays a "encoding in progress message"
 */
function theme_video_inprogress() {
  return '<div class="video-inprogress">' . t('This video is currently being processed. Please wait.') . '</div>';
}

/**
 * Display an "encoding failed" message"
 */
function theme_video_conversion_failed() {
  return '<div class="video-conversion-failed">' . t('The video conversion process has failed. You might want to submit a simpler video format like <em>mpeg</em> or <em>divx avi</em>.<br />If the problem persists please contact website administrators.') . '</div>';
}

/*
 * Theme wraper on Flahs play on 3rd party
 */

function theme_video_flv($variables) {
  $video = $variables['video'];
// set the video flash player
// Moved to last to recheck incase we changed our extension above.
  $video->flash_player = variable_get('video_extension_' . $video->player . '_flash_player', '');
  switch ($video->flash_player) {
    case 'swftools':
      $options = array(
        'params' => array(
          'width' => $video->player_width,
          'height' => $video->player_height,
        ),
        'othervars' => array(
          //@todo: swftools bug, can't enable this until they fix their pathing for the images.
          'image' => $video->thumbnail->swfthumb,
        ),
      );
      $themed_output = swf($video->files->{$video->player}->url, $options);
      break;
    case 'flowplayer':
      // kjh: use a playlist to display the thumbnail if not auto playing
      if (isset($video->autoplay) && isset($video->thumbnail->url)) {
        $options = array(
          'playlist' => array($video->thumbnail->url,
            array('url' => urlencode($video->files->{$video->player}->url),
              'autoPlay' => $video->autoplay,
              'autoBuffering' => $video->autobuffering,
            ),),);
      } else {
        $options = array(
          'clip' => array('url' => urlencode($video->files->{$video->player}->url),
            'autoPlay' => $video->autoplay,
            'autoBuffering' => $video->autobuffering,
          ),);
      }
      $themed_output = theme('flowplayer', array('config' => $options, 'id' => 'flowplayer-' . $video->formatter, 'attributes' => array('style' => 'width:' . $video->player_width . 'px;height:' . ($video->player_height + 24) . 'px;')));
      break;
    default:
      $themed_output = t('No flash player has been setup. ' . l(t('Please select a player to play Flash videos.'), 'admin/settings/video/players'));
      break;
  }
  return theme('video_play_flv', array('video' => $video, 'themed_output' => $themed_output));
}

/**
 * Theme wrapper for HTML5
 */
function theme_video_html5($variables) {
  $themed_output = NULL;
  $video = $variables['video'];
  echo $video->html5_player = variable_get('video_extension_' . $video->player . '_html5_player', '');
  switch ($video->html5_player) {
    case 'video':
      return theme('video_play_html5', array('video' => $video, 'themed_output' => $themed_output));
      break;
    case 'videojs':
      $items = _video_object_to_array($video->files);
      $items +=array('thumbnail' => (array) $video->thumbnail);
      $attributes = array();
      $attributes['width'] = $video->width;
      $attributes['height'] = $video->height;
      return theme('videojs', array('items' => $items, 'player_id' => 'video-' . $video->fid, 'attributes' => $attributes));
      break;
  }
}