<?php

/**
 * @file extension to imageapi, provide an overlay action for blending two
 * layers, preserving transparency.
 */

/**
 * Places one image over another.
 *
 * @param stdClass $image
 *   A valid image object.
 * @param stdClass  $layer
 *   A valid image object to be placed over or under the $image image.
 * @param int $x
 *   Position of the overlay.
 * @param int $y
 *   Position of the overlay.
 * @param int $alpha
 *   Transparency of the overlay from 0-100. 0 is totally transparent. 100
 *   (default) is totally opaque.
 * @param boolean $reverse
 *   Flag to indicate the 'overlay' actually goes under the image.
 *
 * @return boolean
 *   true on success, false otherwise.
 */
function image_overlay(stdClass $image, stdClass $layer, $x, $y, $alpha = 100, $reverse = FALSE) {
  if ($reverse) {
    $x = imagecache_actions_keyword_filter($x, $layer->info['width'], $image->info['width']);
    $y = imagecache_actions_keyword_filter($y, $layer->info['height'], $image->info['height']);
  }
  else {
    $x = imagecache_actions_keyword_filter($x, $image->info['width'], $layer->info['width']);
    $y = imagecache_actions_keyword_filter($y, $image->info['height'], $layer->info['height']);
  }
  return image_toolkit_invoke('overlay', $image, array($layer, $x, $y, $alpha, $reverse));
}

/**
 * GD toolkit specific implementation of the image overlay effect.
 *
 * NOTE that the PHP libraries are not great at merging images SO we include a
 * library that does it pixel-by-pixel which is INCREDIBLY inefficient. If this
 * can be improved, in a way that supports all transparency, please let us know!
 *
 * A watermark is layer onto image, return the image. An underlay is image onto
 * layer, return the layer. Almost identical, but seeing as we work with
 * resource handles, the handle needs to be swapped before returning.
 *
 * @param stdClass $image
 *   An image object.
 * @param stdClass $layer
 *   Image object to be placed over or under the $image image.
 * @param int $x
 *   Position of the overlay.
 * @param int $y
 *   Position of the overlay.
 * @param int $alpha
 *   Transparency of the overlay from 0-100. 0 is totally transparent. 100
 *   (default) is totally opaque.
 * @param boolean $reverse
 *   Flag to indicate that the 'overlay' actually goes under the image.
 *
 * @return boolean
 *   true on success, false otherwise.
 */
function image_gd_overlay(stdClass $image, stdClass $layer, $x, $y, $alpha = 100, $reverse = FALSE) {
  // If the given alpha is 100%, we can use imagecopy - which actually works,
  // is more efficient, and seems to retain the overlays partial transparency.
  // Still does not work great for indexed gifs though?
  if ($reverse) {
    $upper = &$image;
    $lower = &$layer;
  }
  else {
    $upper = &$layer;
    $lower = &$image;
  }
  if ($alpha == 100 && ($upper->info['mime_type'] != 'image/gif')) {
    imagealphablending($lower->resource, TRUE);
    imagesavealpha($lower->resource, TRUE);
    imagealphablending($upper->resource, TRUE);
    imagesavealpha($upper->resource, TRUE);
    imagecopy($lower->resource, $upper->resource, $x, $y, 0, 0, $upper->info['width'], $upper->info['height']);
    imagedestroy($upper->resource);
    $image->resource = $lower->resource;
    $image->info = $lower->info;
  }
  else {
    // imagecopy() cannot be used and we have to use the slow library.
    module_load_include('inc', 'imagecache_actions', 'watermark');
    $watermark = new watermark();
    $result_img = $watermark->create_watermark($lower->resource, $upper->resource, $x, $y, $alpha);
    // Watermark creates a new image resource, so clean up both old images.
    imagedestroy($lower->resource);
    imagedestroy($upper->resource);
    $image->resource = $result_img;
    $image->info = $lower->info;
  }

  return TRUE;
}

/**
 * Imagemagick toolkit specific implementation of the image overlay effect.
 *
 * Please be aware of the limitations of imagemagick libraries out there - the
 * versions distributed on hosted servers (if any) are often several years
 * behind. Using the latest imagemagick release features will make this function
 * unusable in real deployments.
 *
 * @param stdClass $image
 *   An image object.
 * @param stdClass $layer
 *   Image object to be placed over or under the $image image.
 * @param int $x
 *   Position of the overlay.
 * @param int $y
 *   Position of the overlay.
 * @param int $alpha
 *   Transparency of the overlay from 0-100. 0 is totally transparent. 100
 *   (default) is totally opaque.
 * @param boolean $reverse
 *   Flag to indicate that the 'overlay' actually goes under the image.
 *
 * @return boolean
 *   true on success, false otherwise.
 */
function image_imagemagick_overlay(stdClass $image, stdClass $layer, $x = 0, $y = 0, $alpha = 100, $reverse = FALSE) {
  $realPath = imagecache_actions_find_file($layer->source);
  if (!$realPath) {
    return FALSE;
  }
  // Reset any gravity settings from earlier effects.
  $image->ops[] = '-gravity None';

  // In imagemagick terms:
  // - $image is the destination (the image being constructed)
  // - $layer is the source (the source of the current operation)
  // Add the layer image to the imagemagick command line.
  $image->ops[] = escapeshellarg($realPath) . ' ';

  // Set its offset. Offset arguments require a sign in front.
  if ($x >= 0) {
    $x = "+$x";
  }
  if ($y >= 0) {
    $y = "+$y";
  }
  $image->ops[] = "-geometry $x$y";

  // And compose it with the destination.
  if ($alpha == 100) {
    // Lay one image over the other. The transparency channel of the upper
    // image and/or its dimensions (being smaller than the lower image) will
    // determine what remains visible of the lower image).
    //
    // Note: In explicitly setting a -compose operator we reset/overwrite any
    //   previously set one (former versions could produce erroneous results
    //   in combination with other effects before this one).
    if ($reverse) {
      $compose_operator = 'dst-over';
    }
    else {
      $compose_operator = 'src-over';
    }
    $image->ops[] = "-compose $compose_operator -composite";
  }
  else {
    // Alpha is not 100, so this image effect turns into a blend operation.
    // The alpha determines what percentage of the upper image pixel will be
    // taken. From the lower image pixel, 100 - alpha percent will be taken.
    //
    // Note 1: I'm not sure if and how transparency of one or both images is
    //   used in or after the blend operation.
    // Note 2: As of IM v6.5.3-4 (around june 2009) we can use:
    //   -compose blend -define compose:args=30[,70]
    $image->ops[] = "-compose blend -define compose:args=$alpha -composite";
  }
  return TRUE;
}
