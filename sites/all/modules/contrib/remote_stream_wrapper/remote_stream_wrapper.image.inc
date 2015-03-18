<?php

/**
 * Copy of image_style_deliver() for use with remote images.
 */
function remote_stream_wrapper_image_style_deliver($style, $scheme) {
  $args = func_get_args();
  array_shift($args);
  array_shift($args);
  $target = implode('/', $args);

  // Check that the style is defined, the scheme is valid, and the image
  // derivative token is valid. (Sites which require image derivatives to be
  // generated without a token can set the 'image_allow_insecure_derivatives'
  // variable to TRUE to bypass the latter check, but this will increase the
  // site's vulnerability to denial-of-service attacks. To prevent this
  // variable from leaving the site vulnerable to the most serious attacks, a
  // token is always required when a derivative of a derivative is requested.)
  $valid = !empty($style) && file_stream_wrapper_valid_scheme($scheme);
  if (!variable_get('image_allow_insecure_derivatives', FALSE) || strpos(ltrim($target, '\/'), 'styles/') === 0) {
    $valid = $valid && isset($_GET[IMAGE_DERIVATIVE_TOKEN]) && $_GET[IMAGE_DERIVATIVE_TOKEN] === image_style_path_token($style['name'], $scheme . '://' . $target);
  }
  if (!$valid) {
    return MENU_ACCESS_DENIED;
  }

  $image_uri = $scheme . '://' . $target;
  $derivative_uri = remote_stream_wrapper_image_style_path($style['name'], $image_uri);

  // Prevent only the remote files that exist in {file_managed} from having
  // image style derivatives generated. Otherwise this could open up the site
  // to allowing any remote file to be processed.
  if (!remote_stream_wrapper_file_load_by_uri($image_uri)) {
    return MENU_ACCESS_DENIED;
  }

  // Don't start generating the image if the derivative already exists or if
  // generation is in progress in another thread.
  $lock_name = 'image_style_deliver:' . $style['name'] . ':' . drupal_hash_base64($image_uri);
  if (!file_exists($derivative_uri)) {
    $lock_acquired = lock_acquire($lock_name);
    if (!$lock_acquired) {
      // Tell client to retry again in 3 seconds. Currently no browsers are known
      // to support Retry-After.
      drupal_add_http_header('Status', '503 Service Unavailable');
      drupal_add_http_header('Retry-After', 3);
      print t('Image generation in progress. Try again shortly.');
      drupal_exit();
    }
  }

  // Try to generate the image, unless another thread just did it while we were
  // acquiring the lock.
  $success = file_exists($derivative_uri) || image_style_create_derivative($style, $image_uri, $derivative_uri);

  if (!empty($lock_acquired)) {
    lock_release($lock_name);
  }

  if ($success) {
    $image = image_load($derivative_uri);
    file_transfer($image->source, array('Content-Type' => $image->info['mime_type'], 'Content-Length' => $image->info['file_size']));
  }
  else {
    watchdog('image', 'Unable to generate the derived image located at %path.', array('%path' => $derivative_uri));
    drupal_add_http_header('Status', '500 Internal Server Error');
    print t('Error generating image.');
    drupal_exit();
  }
}
