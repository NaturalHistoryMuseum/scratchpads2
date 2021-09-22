<?php

/**
 * @file media_youtube/includes/media_youtube.styles.inc
 * Styles definitions for Media: YouTube.
 */

/**
 * Implementation of Styles module hook_styles_register().
 */
function media_youtube_styles_register() {
  return array(
    'MediaYouTubeStyles' => array(
      'field_types' => 'file',
      'name' => t('MediaYouTube'),
      'description' => t('Media YouTube styles.'),
      'path' => drupal_get_path('module', 'media_youtube') .'/includes',
      'file' => 'media_youtube.styles.inc',
    ),
  );
}

/**
 *  Implements hook_styles_containers(). (Deprecated in version 2)
 */
function media_youtube_styles_containers() {
  return array(
    'file' => array(
      'containers' => array(
        'media_youtube' => array(
          'label' => t('YouTube Styles'),
          'data' => array(
            'streams' => array(
              'youtube',
            ),
            'mimetypes' => array(
              'video/youtube',
            ),
          ),
          'weight' => 0,
          'filter callback' => 'media_youtube_formatter_filter',
          'themes' => array(
            'field_formatter_styles' => 'media_youtube_field_formatter_styles',
            'styles' => 'media_youtube_styles',
            'preview' => 'media_youtube_preview_style',
          ),
          'description' => t('YouTube Styles will display embedded YouTube videos and thumbnails to your choosing, such as by resizing, setting colors, and autoplay. You can !manage.', array('!manage' => l(t('manage your YouTube styles here'), 'admin/config/media/media-youtube-styles'))),
        ),
      ),
    ),
  );
}

function media_youtube_formatter_filter($variables) {
  if (isset($variables['object'])) {
    $object = isset($variables['object']->file) ? $variables['object']->file : $variables['object'];
    return (file_uri_scheme($object->uri) == 'youtube') && ($object->filemime == 'video/youtube');
  }
}

/**
 * Implementation of the File Styles module's hook_file_styles_filter().
 */
function media_youtube_file_styles_filter($object) {
  $file = isset($object->file) ? $object->file : $object;
  if ((file_uri_scheme($file->uri) == 'youtube') && ($file->filemime == 'video/youtube')) {
    return 'media_youtube';
  }
}

/**
 *  Implements hook_styles_styles().
 */
function media_youtube_styles_styles() {
  $styles = array(
    'file' => array(
      'containers' => array(
        'media_youtube' => array(
          'styles' => array(
            'youtube_thumbnail' => array(
              'name' => 'youtube_thumbnail',
              'effects' => array(
                array('label' => t('Thumbnail'), 'name' => 'thumbnail', 'data' => array('thumbnail' => 1)),
                array('label' => t('Resize'), 'name' => 'resize', 'data' => array('width' => 100, 'height' => 75)),
              ),
            ),
            'youtube_preview' => array(
              'name' => 'youtube_preview',
              'effects' => array(
                array('label' => t('Autoplay'), 'name' => 'autoplay', 'data' => array('autoplay' => 0)),
                array('label' => t('Resize'), 'name' => 'resize', 'data' => array('width' => 220, 'height' => 165)),
              ),
            ),
            'youtube_full' => array(
              'name' => 'youtube_full',
              'effects' => array(
                array('label' => t('Autoplay'), 'name' => 'autoplay', 'data' => array('autoplay' => 0)),
                array('label' => t('Resize'), 'name' => 'resize', 'data' => array('width' => 640, 'height' => 480)),
                array('label' => t('Full screen'), 'name' => 'fullscreen', 'data' => array('fullscreen' => 1)),
              ),
            ),
          ),
        ),
      ),
    ),
  );

  // Allow any image style to be applied to the thumbnail.
  foreach (image_styles() as $style_name => $image_style) {
    $styles['file']['containers']['media_youtube']['styles']['youtube_thumbnail_' . $style_name] = array(
      'name' => 'youtube_thumbnail_' . $style_name,
      'image_style' => $style_name,
      'effects' => array(
        array('label' => t('Thumbnail'), 'name' => 'thumbnail', 'data' => array('thumbnail' => 1)),
      ),
    );
  }

  return $styles;
}

/**
 *  Implements hook_styles_presets().
 */
function media_youtube_styles_presets() {
  $presets = array(
    'file' => array(
      'square_thumbnail' => array(
        'media_youtube' => array(
          'youtube_thumbnail_square_thumbnail',
        ),
      ),
      'thumbnail' => array(
        'media_youtube' => array(
          'youtube_thumbnail',
        ),
      ),
      'small' => array(
        'media_youtube' => array(
          'youtube_preview',
        ),
      ),
      'large' => array(
        'media_youtube' => array(
          'youtube_full',
        ),
      ),
      'original' => array(
        'media_youtube' => array(
          'youtube_full',
        ),
      ),
    ),
  );
  return $presets;
}

/**
 * Implementation of Styles module hook_styles_default_containers().
 */
function media_youtube_styles_default_containers() {
  // We append YouTube to the file containers.
  return array(
    'file' => array(
      'containers' => array(
        'media_youtube' => array(
          'class' => 'MediaYouTubeStyles',
          'name' => 'media_youtube',
          'label' => t('YouTube'),
          'preview' => 'media_youtube_preview_style',
        ),
      ),
    ),
  );
}


/**
 * Implementation of Styles module hook_styles_default_presets().
 */
function media_youtube_styles_default_presets() {
  $presets = array(
    'file' => array(
      'containers' => array(
        'media_youtube' => array(
          'default preset' => 'unlinked_thumbnail',
          'styles' => array(
            'original' => array(
              'default preset' => 'video',
            ),
            'thumbnail' => array(
              'default preset' => 'linked_thumbnail',
            ),
            'square_thumbnail' => array(
              'default preset' => 'linked_square_thumbnail',
            ),
            'medium' => array(
              'default preset' => 'linked_medium',
            ),
            'large' => array(
              'default preset' => 'large_video',
            ),
          ),
          'presets' => array(
            'video' => array(
              array(
                'name' => 'video',
                'settings' => array(),
              ),
            ),
            'large_video' => array(
              array(
                'name' => 'resize',
                'settings' => array(
                  'width' => 640,
                  'height' => 390,
                ),
              ),
              array(
                'name' => 'video',
                'settings' => array(),
              ),
            ),
          ),
        ),
      ),
    ),
  );
  // Allow any image style to be applied to the thumbnail.
  foreach (image_styles() as $style_name => $image_style) {
    $presets['file']['containers']['media_youtube']['presets']['linked_' . $style_name] = array(
      array(
        'name' => 'linkToMedia',
        'settings' => array(),
      ),
      array(
        'name' => 'imageStyle',
        'settings' => array(
          'image_style' => $style_name,
        ),
      ),
      array(
        'name' => 'thumbnail',
        'settings' => array(),
      ),
    );
    $presets['file']['containers']['media_youtube']['presets']['unlinked_' . $style_name] = $presets['file']['containers']['media_youtube']['presets']['linked_' . $style_name];
    array_shift($presets['file']['containers']['media_youtube']['presets']['unlinked_' . $style_name]);
    foreach ($image_style['effects'] as $effect) {
      if (in_array($effect['name'], array('image_scale', 'image_scale_and_crop', 'image_resize', 'image_crop'))) {
        $presets['file']['containers']['media_youtube']['presets']['video_' . $style_name] = array(
          array(
            'name' => 'resize',
            'settings' => $effect['data'],
          ),
          array(
            'name' => 'video',
            'settings' => array(),
          ),
        );
      }
    }
  }
  return $presets;
}
