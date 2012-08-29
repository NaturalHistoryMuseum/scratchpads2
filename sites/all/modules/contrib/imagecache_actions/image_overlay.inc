<?php

/**
 * @file extension to imageapi, provide an overlay action for blending two
 * layers, preserving transparency.
 */

// Not sure where this library will live between upgrade versions.
// Be careful to not conflict with another copy of myself.
if (! function_exists('image_overlay')) {

  /**
   * Place one image over another
   *
   * @param object $image
   *   An image object.
   * @param object $overlay
   *   An image object.
   * @param $x
   *   Position of the overlay
   * @param $y
   *   Position of the overlay
   * @param $alpha
   *   Transparency of the overlay from 0-100. 0 is totally transparent. 100
   * (default) is totally opaque.
   * @param $reverse
   *   BOOL flag to indicate the 'overlay' actually goes under the image. As
   * the imageapi callbacks modify the $image object by reference, this is needed
   * to replace the old image resource with the new one.
   * @return bool success
   *
   * @ingroup imageapi
   */
  function image_overlay($image, &$layer, $x, $y, $alpha = 100, $reverse = FALSE) {
    $x = imagecache_actions_keyword_filter($x, $image->info['width'], $layer->info['width']);
    $y = imagecache_actions_keyword_filter($y, $image->info['height'], $layer->info['height']);
    return image_toolkit_invoke('overlay', $image, array($layer, $x, $y, $alpha, $reverse));
  }

  /**
   * Place one image over another
   * This modifies the passed image by reference
   *
   * This func is nominated for inclusion in imageapi package. Until then, we do
   * it ourselves.
   *
   * NOTE that the PHP libraries are not great at merging images SO we include a
   * library that does it pixel-by-pixel which is INCREDIBLY inefficient. If this
   * can be improved, in a way that supports all transparency, please let us know!
   *
   * A watermark is layer onto image, return the image. An underlay is image onto
   * layer, return the layer. Almost identical, but seeing as we work with
   * resource handles, the handle needs to be swapped before returning.
   *
   * @ingroup imageapi
   * @param $image
   *   Base imageapi object.
   * @param $overlay
   *   May be a filename or an imageAPI object
   * @param $x
   *   Position of the overlay
   * @param $y
   *   Position of the overlay
   * @param $alpha
   *   Transparency of the overlay from 0-100. 0 is totally transparent. 100
   * (default) is totally opaque.
   * @param $reverse
   *   BOOL flag to indicate the 'overlay' actually goes under the image. As
   * the imageapi callbacks modify the $image object by reference, this is needed
   * to replace the old image resource with the new one.
   * @return bool success
   */
  function image_gd_overlay($image, $layer, $x, $y, $alpha = 100, $reverse = FALSE) {
    if (empty($layer->resource)) {
      trigger_error("Invalid input to " . __FUNCTION__ . " 'layer' is not a valid resource");
      #dpm($layer);
      return FALSE;
    }
    // If the given alpha is 100%, we can use imagecopy - which actually works,
    // Is more efficient, and seems to retain the overlays partial transparancy
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
      // Else imagecopymerge fails and we have to use the slow library.
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
   * Improvements on this are welcomed!
   *
   * Please be aware of the limitations of imagemagick libraries out there - the
   * versions distributed on hosted servers (if any) are often several years
   * behind. Using the latest imagemagick release features will make this function
   * unusable in real deployments.
   *
   * @param $image
   *   Base imageapi object.
   * @param $layer
   *   May be a filename or an imageAPI object, gets placed on top
   *   If using reverse, this is going to be the result we carry on working with.
   */
  function image_imagemagick_overlay($image, $layer, $x = 0, $y = 0, $alpha = 100, $reverse = FALSE) {
    // In imagemagick terms:
    // - $image is the destination (the image being constructed)
    // - $layer is the source (the source of the current operation)
    // Add the layer image to the imagemagick command line.
    $image->ops[] = escapeshellarg($layer->source) . ' ';

    // Set its offset. Offset arguments require a sign in front.
    if ($x >= 0) {
      $x = "+$x";
    }
    if ($y >= 0) {
      $y = "+$y";
    }
    $image->ops[] = " -geometry $x$y";

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
      $image->ops[] = "-compose $compose_operator -composite ";
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
      $image->ops[] = "-compose blend -define compose:args=$alpha -composite ";
    }
    return TRUE;
  }

}
